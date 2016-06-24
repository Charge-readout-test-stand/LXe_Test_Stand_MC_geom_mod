#include "G4RunManager.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"
#include <iostream>
#include <iomanip>

#include "nEXOStackingAction.hh"
#include "nEXOAnalysis.hh"

nEXOStackingAction::nEXOStackingAction():
fIsFirstTrackInNewStage(true)
{}

nEXOStackingAction::~nEXOStackingAction(){}

G4ClassificationOfNewTrack nEXOStackingAction::ClassifyNewTrack(const G4Track* track) 
{
  /// can choose what particles to kill, etc., or ask analysis to perform some action
  //G4cout << "Track id = " << track->GetTrackID() << " parent " << track->GetParentID() << "particle def " << track->GetDefinition()->GetParticleName() << G4endl;
  //track->GetDynamicParticle()->DumpInfo();
  //return fUrgent; // turning off event windowing for now...
  //if(track->GetTrackID() > 1)
  //return fKill;
  
  G4ClassificationOfNewTrack classification = fUrgent;
  G4int trackID = track->GetTrackID();
  static bool haveWarned = false; // precision warning, up to once per event 
  //return fUrgent; // debugging: skip windowing 

  int verboseLevel = G4EventManager::GetEventManager()->GetTrackingManager()->GetVerboseLevel();

  // kill optical photons
  if ( track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition() ) { classification = fKill; }
  if ( track->GetDefinition() == G4NeutrinoE::Definition() ) { classification = fKill; }
  if ( track->GetDefinition() == G4AntiNeutrinoE::Definition() ) { classification = fKill; }
  
  if (classification == fKill && verboseLevel>2) { 
      G4cout << "nEXOStackingAction::ClassifyNewTrack(): killing track " 
        << trackID << ", " << track->GetDefinition()->GetParticleName() << G4endl; 
  }
  if ( classification == fKill ) { return classification; }

  static G4double currentEventTime = 0.0; // start time of the current event
  static G4double nextEventTime = 0.0; // start time of the next event
  static bool isNextEventTimeSet = false; // whether nextEventTime is useful yet

  // approx wfm length for Stanford LXe system: 32 microsecond wfm - 8
  // microsecond delay before trigger
  G4double timeWindow = (32.0-8.0)*1e3; 
  G4double globalTime = track->GetGlobalTime();

  // reset things at start of new event
  if (trackID == 1) {
      currentEventTime = 0.0;
      nextEventTime = 0.0;
      isNextEventTimeSet = false;
      haveWarned = false; 
  }

  // check that we have enough time precision to split events (within 10%)
  // if this is a problem, we need to do something like EXO offline's
  // EXOIsotopeTable
  G4double testTime = globalTime + timeWindow; 
  if ( !haveWarned && testTime - globalTime < timeWindow*0.9 ) {
    G4cout << "nEXOStackingAction::ClassifyNewTrack(): precision problem!!" << G4endl; 
    G4cout 
        << std::setprecision(3) << std::fixed
        << "\ttimeWindow [ms]: " << timeWindow/1e6 << G4endl;
    G4cout << "\ttestTime [s]: " << testTime/1e9 << G4endl;
    G4cout << "\tglobalTime [s]: " << globalTime/1e9 << G4endl;
    G4cout << "\ttestTime-globalTime [s]: " << (testTime-globalTime)/1e9 << G4endl;
    haveWarned = true; 
  }

  // set currentEventTime to nextEventTime, if fIsFirstTrackInNewStage
  G4double dt = globalTime - currentEventTime; 
  if (fIsFirstTrackInNewStage) { 
    fIsFirstTrackInNewStage = false; 
    currentEventTime = nextEventTime;
    isNextEventTimeSet = false; 
    nextEventTime = 0.0; 
    if (verboseLevel>2) {
      G4cout << "nEXOStackingAction::ClassifyNewTrack(): track " << trackID 
        << std::setprecision(3) << std::fixed
        << " t=" << globalTime/1e9
        << " s, dt=" << dt/1e9 
        << " s: setting currentEventTime to: " 
        << currentEventTime/1e9 
        << " s" << G4endl; 
    }
    dt = globalTime - currentEventTime; 
  }

  if (dt < 0.0) {
      G4cout << "nEXOStackingAction::ClassifyNewTrack(): track " << trackID 
        << " warning! negative dt: " << dt << " s" << G4endl; 

  }

  if (verboseLevel>2) {
    G4cout << "nEXOStackingAction::ClassifyNewTrack(): track " << trackID 
        << std::setprecision(3) << std::fixed
        << " t=" << globalTime/1e9
        << " s, dt=" << dt/1e9 
        << " s" << G4endl; 
  }

  // test whether the track should be time windowed, if so move it to waiting
  // stack
  if ( dt > timeWindow ) {
      
    if (!isNextEventTimeSet) {
      nextEventTime = globalTime; 
      isNextEventTimeSet = true; 
      if (verboseLevel>2) {
        G4cout 
          << "nEXOStackingAction::ClassifyNewTrack(): track " << trackID 
          << std::setprecision(3) << std::fixed
          << " t=" << globalTime/1e9
          << " s, dt=" << dt/1e9 
          << " s: dt > timeWindow (" << timeWindow/1e6
          << " ms), setting nextEventTime to: " 
          << nextEventTime/1e9
          << "s" << G4endl; 
      }
    }

    classification = fWaiting;
    if (verboseLevel) {
      G4cout << "nEXOStackingAction::ClassifyNewTrack(): set track " << trackID 
        << " (" 
        << std::setprecision(3) << std::fixed
        << track->GetKineticEnergy()*1e3 << "-keV " << track->GetDefinition()->GetParticleName()
        << "), dt=" << dt/1e9
        << ", time=" << globalTime/1e9
        << " seconds, to waiting" << G4endl;
    }

  } // end test of whether track should be windowed

  return classification;
}


void nEXOStackingAction::NewStage()
{
  // When all urgent tracks have been processed, Geant4 moves the waiting tracks
  // to the urgent stack and calls this method. 

  fIsFirstTrackInNewStage = true; 

  int nTracks = G4EventManager::GetEventManager()->GetStackManager()->GetNTotalTrack();
  int verboseLevel = G4EventManager::GetEventManager()->GetTrackingManager()->GetVerboseLevel();

  if (verboseLevel) {
    G4cout << "nEXOStackingAction::NewStage(): There are " << nTracks  << " tracks from the waiting stack." << G4endl;                                       
  }

  // If there are some new tracks in the urgent stack, write out the tracks that
  // have already been processed. If there are no tracks in the urgent stack,
  // everything will be processed normally by nEXOAnalysis at the end of the event. 
  if(nTracks != 0){      
    // write out the partial event in the earlier time window
    if (verboseLevel) {
      G4cout << "nEXOStackingAction::NewStage(): writing partial event" << G4endl;                                       
    }

    const G4Event* currentEvent = G4RunManager::GetRunManager()->GetCurrentEvent();

    // this just checks the number of energy deposits, if needed, and fills the tree
    nEXOAnalysis::GetInstance()->EndOfEvent(currentEvent);

    // one issue here is that GenX, GenY, GenZ get cleared
    nEXOAnalysis::GetInstance()->PrepareNewEvent(currentEvent);
                                                                                        
  }  // end check on number of total tracks  

}

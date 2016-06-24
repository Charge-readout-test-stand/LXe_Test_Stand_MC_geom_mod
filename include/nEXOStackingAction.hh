#ifndef nEXOStackingAction_h
#define nEXOStackingAction_h 1

#include "globals.hh"

#include "G4Track.hh"
#include "G4UserStackingAction.hh"
#include "G4ClassificationOfNewTrack.hh"

#include "nEXOAnalysis.hh"

/// Control which particles are tracked by G4
class nEXOStackingAction : public G4UserStackingAction
{
public:
  nEXOStackingAction();
  virtual ~nEXOStackingAction();
  
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);

  virtual void NewStage();

  virtual void PrepareNewEvent(){ fIsFirstTrackInNewStage = true; }

private:
  bool fIsFirstTrackInNewStage;
};

#endif

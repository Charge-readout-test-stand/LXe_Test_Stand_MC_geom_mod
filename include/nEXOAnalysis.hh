//////////////////////////////////////////
// nEXOAnalysis.hh //
// Analysis manager to save event info  //
// This class is designed as a singleton//
//////////////////////////////////////////

#ifndef nEXOAnalysis_h
#define nEXOAnalysis_h 1

#include "globals.hh"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4Step.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4SteppingManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4RunManager.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"

#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"

#include "TROOT.h"
#include "TObject.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TBranch.h"
#include "TChain.h"
#include "TTimeStamp.h"
#include "TVector3.h"
#include "TGeoManager.h"
#include "TGeoVolume.h"
#include "TGeoCone.h"
#include "TGeoTube.h"
#include "TParameter.h"

#include "Geant4GM/volumes/Factory.h"
#include "RootGM/volumes/Factory.h"
#include "TGeoManager.h"
#include "G4RegionStore.hh"

#include "nEXOSimplePhysVolManager.hh"
#include "nEXOG4ThermalElectron.hh"

const int MAXTENUM = 300000;
const int MAXOPNUM = 300000;
const int MAXDEPNUM = 200000;

class nEXOAnalysis
{
public:
  static nEXOAnalysis* GetInstance(){
    if(nEXOAnalysis::fInstance == NULL) nEXOAnalysis::fInstance = new nEXOAnalysis();
    return nEXOAnalysis::fInstance;
  }
  
  virtual ~nEXOAnalysis();
  
  void SetROOTFileName(G4String& name){fRootFileName = name;}
  void SetROOTTreeName(G4String& name){fRootTreeName = name;}   
  void SetPropagateOP(G4int flag) { fPropagateOP = flag; }
  void SetPropagateTE(G4int flag) { fPropagateTE = flag; }
  void SetSaveOP(G4int flag) { fSaveOP = flag; }
  void SetSaveTE(G4int flag) { fSaveTE = flag; }
  void SetElectricField(G4double field) { fField = field; }

  void SetSaveOnlyEventsWithDeposits(G4bool saveOnly){ fSaveOnlyEventsWithDeposits = saveOnly; }
  
  G4int GetPropagateOP() { return fPropagateOP; }
  G4int GetPropagateTE() { return fPropagateTE; }
  G4int GetSaveOP() { return fSaveOP; }
  G4int GetSaveTE() { return fSaveTE; }
  G4double GetElectricField() { return fField; }
  G4int GetNTE() { return fNTE; }

  void SetBetaE1(G4double e1) { fBetaE1 = e1; }
  void SetBetaE2(G4double e2) { fBetaE2 = e2; }
  void SetBetaAngle(G4double angle) { fBetaAngle = angle; }

  void SetNestEventEnergy(G4double val) {fNestEventEnergy = val; }
  void AddNestEventEnergy(G4double val) {fNestEventEnergy += val; }
  G4double GetNestEventEnergy() { return fNestEventEnergy; }

  //void WriteGeometry(G4VPhysicalVolume* world);
  void WriteGeometry();
  void SetGeometry(G4VPhysicalVolume* world) { fWorld = world; }
  G4VPhysicalVolume* GetGeometry() { return fWorld; }

  void PrepareNewRun(const G4Run* g4Run);
  void EndOfRun(const G4Run* g4Run);

  void PrepareNewEvent(const G4Event* g4Event);
  void EndOfEvent(const G4Event* g4Event);

  void PreUserTrackingAction(const G4Track* g4Track);
  void PostUserTrackingAction(const G4Track* g4Track);
  
  void SteppingAction(const G4Step* g4Step);

  void GetTE(int i, G4double& E, G4double& X, G4double& Y, G4double& Z, G4double& T);
  void AddThermalElectron(G4ThreeVector pos, G4double eTime);
  void AddOpticalPhoton(G4ThreeVector pos, G4double eTime);
  void AddInitNOP() { fInitNOP++;}

  void SetIsNewSubEvent(G4bool val){ fIsNewSubEvent = val; }
  G4bool GetIsNewSubEvent() { return fIsNewSubEvent; }

private:
  nEXOAnalysis();
  static nEXOAnalysis* fInstance;

  void ResetTreeVariables();
  void SetTreeBranches();
  
  G4String fRootFileName;
  G4String fRootTreeName;
  TFile* fRootFile; 
  TTree* fRootTree;

private:
  
  G4int fEventNumber;
  G4int fSubEventNumber;
  G4double fGenX;
  G4double fGenY;
  G4double fGenZ;
  G4double fBetaE1;
  G4double fBetaE2;
  G4double fBetaAngle;
  G4double fTotalEventEnergy;           // energy deposited in active LXe
  G4double fTotalEventLXeEnergy;        // energy deposited in all LXe
  G4double fNestEventEnergy;            // energy observed by NEST
  G4int fNumDeposits;
  G4double fLengthDeposit[MAXDEPNUM];
  G4double fEnergyDeposit[MAXDEPNUM];
  G4double fPreEnergyDeposit[MAXDEPNUM];
  G4double fPostEnergyDeposit[MAXDEPNUM];
  G4int fTrackNumber[MAXDEPNUM];
  G4double fXpos[MAXDEPNUM];
  G4double fYpos[MAXDEPNUM];
  G4double fZpos[MAXDEPNUM];
  G4double fTglob[MAXDEPNUM];
  G4double fTloc[MAXDEPNUM];

  // record info about particles created by radioactive decay
  G4int fNPrimaries;
  G4int fPdgCode[MAXDEPNUM]; //http://pdg.lbl.gov/mc_particle_id_contents.html
  G4double fKineticEnergy[MAXDEPNUM];
  
  G4int fNOP;
  G4int fInitNOP;
  G4double fOPEnergy[MAXOPNUM];
  G4int fSiPMID[MAXOPNUM];
  G4double fOPTime[MAXOPNUM];
  G4int fOPStopVolume[MAXOPNUM];
  G4double fOPX[MAXOPNUM];
  G4double fOPY[MAXOPNUM];
  G4double fOPZ[MAXOPNUM];
  G4int fNTE;
  G4double fTEEnergy[MAXTENUM];
  G4double fTEX[MAXTENUM];
  G4double fTEY[MAXTENUM];
  G4double fTEZ[MAXTENUM];
  G4double fTET[MAXTENUM];

  G4OpBoundaryProcess* fBoundary;
  G4VPhysicalVolume* fWorld;

  G4int fPropagateOP;
  G4int fPropagateTE;
  G4int fSaveOP;
  G4int fSaveTE;
  G4double fField;

  G4bool fSaveOnlyEventsWithDeposits;
  G4bool fIsNewSubEvent; // for keeping track of event start in NEST 
  
};

class nEXOAnalysisMessenger : public G4UImessenger
{
public:
  nEXOAnalysisMessenger(nEXOAnalysis* analysis);
  virtual ~nEXOAnalysisMessenger();

  virtual void SetNewValue(G4UIcommand* cmd, G4String val);

private:
  nEXOAnalysis* fAnalysis;
  G4UIdirectory* fDirectory;

  G4UIcmdWithAString* fRootFileNameCmd;
  G4UIcmdWithAString* fRootTreeNameCmd;

  G4UIcmdWithAnInteger* fPrintVolumesCmd;
  
  G4UIcmdWithoutParameter* fPrintVolNames;
  G4UIcmdWithAString* fPrintVolTranslationCmd;

  G4UIcmdWithAnInteger* fPropagateOpCmd;
  G4UIcmdWithAnInteger* fPropagateTeCmd;
  G4UIcmdWithAnInteger* fSaveOpCmd;
  G4UIcmdWithAnInteger* fSaveTeCmd;
  G4UIcmdWithABool* fSaveOnlyEventsWithDepositsCmd;
};

#endif

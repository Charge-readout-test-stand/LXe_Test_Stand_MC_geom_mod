#ifndef EXOPhysicsList_h
#define EXOPhysicsList_h 1

#include "globals.hh"
#include "G4VModularPhysicsList.hh"
#include "EXOPhysics.hh"

#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"

class EXOPhysicsListMessenger;

class EXOPhysicsList: public G4VModularPhysicsList
{

private:

    G4double defaultCutValue;
    G4double cutForGamma;
    G4double cutForElectron;
    G4double cutForPositron;
    G4double cutForProton;
    G4double cutForAlpha;
    G4double cutForGenericIon;
    G4double cutInsideTPC;
    G4double cutInnerRegion;

  //G4bool nestOn;
  EXOPhysics* fPhysics;
  EXOPhysicsListMessenger* fMessenger;
public:

    EXOPhysicsList();
    virtual ~EXOPhysicsList();

    virtual void ConstructParticle();
    virtual void ConstructProcess();

    virtual void SetCuts();

  virtual void SetDefaultCut(G4double val){defaultCutValue = val;}
  virtual void SetCutInnerRegion(G4double val){cutInnerRegion = val;}
  virtual void SetCutInsideTPC(G4double val){cutInsideTPC = val;}  
  
  EXOPhysics* GetEXOPhysics(){return fPhysics;}
};

class EXOPhysicsListMessenger : public G4UImessenger
{


private:
  EXOPhysicsList* fPhysicsList;
  G4UIdirectory* fDirectory;

  G4UIcmdWithADoubleAndUnit* fDefaultCutValueCmd;
  G4UIcmdWithADoubleAndUnit* fCutValueInnerRegionCmd;
  G4UIcmdWithADoubleAndUnit* fCutValueInsideTPCCmd;
  G4UIcmdWithABool* fEnableLightCmd;
  //G4UIcmdWithABool* fNESTonCmd;
public:
  EXOPhysicsListMessenger(EXOPhysicsList* physList);
  virtual ~EXOPhysicsListMessenger();
  virtual void SetNewValue(G4UIcommand* cmd, G4String val);
};

#endif

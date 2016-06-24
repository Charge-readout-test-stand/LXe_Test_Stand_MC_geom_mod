
#ifndef nEXOPhysicsList_h
#define nEXOPhysicsList_h 1

#include "globals.hh"
#include "G4VModularPhysicsList.hh"

class G4VPhysicsConstructor;
class nEXOPhysicsListMessenger;

class nEXOStepMax;

class G4EmConfigurator;

class nEXOPhysicsList: public G4VModularPhysicsList
{
  public:

    nEXOPhysicsList(G4String);
    virtual ~nEXOPhysicsList();

    void SetCuts();

    void SetStepMax(G4double);
    nEXOStepMax* GetStepMaxProcess();
    void AddStepMax();

    virtual void ConstructParticle();
    virtual void ConstructProcess();

private:

    void AddPAIModel();
    void AddIonGasModel();

    G4EmConfigurator* fEM_Config;

    nEXOStepMax* fStepMaxProcess;

    nEXOPhysicsListMessenger* fMessenger;

};

#endif

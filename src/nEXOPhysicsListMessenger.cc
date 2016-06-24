#include "globals.hh"

#include "nEXOPhysicsListMessenger.hh"
#include "nEXOPhysicsList.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "G4PhaseSpaceDecayChannel.hh"
#include "G4PionRadiativeDecayChannel.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

nEXOPhysicsListMessenger::nEXOPhysicsListMessenger(nEXOPhysicsList* pPhys)
  : fPhysicsList(pPhys)
{
    fDirectory = new G4UIdirectory("/nEXO/phys/");
    fDirectory->SetGuidance("nEXOPhysicsList control");
 
    fStepMaxCMD = new G4UIcmdWithADoubleAndUnit("/nEXO/phys/stepMax",this);
    fStepMaxCMD->SetGuidance("Set max. step length in the detector");
    fStepMaxCMD->SetParameterName("mxStep",false);
    fStepMaxCMD->SetUnitCategory("Length");
    fStepMaxCMD->SetRange("mxStep>0.0");
    fStepMaxCMD->SetDefaultUnit("mm");
    fStepMaxCMD->AvailableForStates(G4State_PreInit,G4State_Idle);

    fDecayDirectory = new G4UIdirectory("/decay/");
    fDecayDirectory->SetGuidance("Decay chain control commands.");

    fPienuCMD = new G4UIcmdWithoutParameter("/decay/pienu", this);
    fPienuCMD->SetGuidance("Sets the pi+ to decay into e+, nu");
    fPienuCMD->AvailableForStates(G4State_PreInit,G4State_Idle);

    fPimunuCMD = new G4UIcmdWithoutParameter("/decay/pimunu", this);
    fPimunuCMD->SetGuidance("Sets the pi+ to decay into mu+, nu");
    fPimunuCMD->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

nEXOPhysicsListMessenger::~nEXOPhysicsListMessenger()
{
    delete fDirectory;
    delete fDecayDirectory;

    delete fStepMaxCMD;

    delete fPienuCMD;
    delete fPimunuCMD;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOPhysicsListMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{
    if (command == fStepMaxCMD) {
        fPhysicsList->SetStepMax(fStepMaxCMD
                                     ->GetNewDoubleValue(newValue));
    }

    else if (command == fPienuCMD) {
       G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
       G4ParticleDefinition* particleDef = particleTable->FindParticle("pi+");
       G4DecayTable* table = new G4DecayTable();
       G4VDecayChannel* mode = 
                        new G4PhaseSpaceDecayChannel("pi+",1.0,2,"e+","nu_e");
       // G4VDecayChannel* mode = 
       //                  new G4PionRadiativeDecayChannel("pi+",0.000017);
       table->Insert(mode);
       particleDef->SetDecayTable(table);
    }

    else if (command == fPimunuCMD) {
       G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
       G4ParticleDefinition* particleDef = particleTable->FindParticle("pi+");
       G4DecayTable* table = new G4DecayTable();
       G4VDecayChannel* mode =
                     new G4PhaseSpaceDecayChannel("pi+",1.000,2,"mu+","nu_mu");
       table->Insert(mode);
       particleDef->SetDecayTable(table);
    }
}

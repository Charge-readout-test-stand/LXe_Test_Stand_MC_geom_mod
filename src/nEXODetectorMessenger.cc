
#include "nEXODetectorMessenger.hh"
#include "nEXODetectorConstruction.hh"

nEXODetectorMessenger::nEXODetectorMessenger(nEXODetectorConstruction* Det)
 : G4UImessenger(),
   fDetectorConstruction(Det)
{
  fDetDirectory = new G4UIdirectory("/nEXO/det/");
  fDetDirectory->SetGuidance("Detector construction control");

  fCheckOverlapCmd = new G4UIcmdWithABool("/nEXO/det/checkOverlap", this);
  fCheckOverlapCmd->SetGuidance("Toggle CheckOverlap flag");
  fCheckOverlapCmd->AvailableForStates(G4State_PreInit);

  fStepMaxCmd = new G4UIcmdWithADoubleAndUnit("/nEXO/det/stepMax",this);
  fStepMaxCmd->SetGuidance("Define a step max");
  fStepMaxCmd->SetParameterName("stepMax",false);
  fStepMaxCmd->SetUnitCategory("Length");
  fStepMaxCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

nEXODetectorMessenger::~nEXODetectorMessenger()
{
  delete fDetDirectory;

  delete fCheckOverlapCmd;
  delete fStepMaxCmd;

}

void nEXODetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == fCheckOverlapCmd) {
    fDetectorConstruction
      ->SetCheckOverlaps(fCheckOverlapCmd->GetNewBoolValue(newValue));
  }
  if( command == fStepMaxCmd ) {
    fDetectorConstruction
      ->SetMaxStep(fStepMaxCmd->GetNewDoubleValue(newValue));
  }   
}

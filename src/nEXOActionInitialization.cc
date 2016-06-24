#include "nEXOActionInitialization.hh"
#include "nEXOPrimaryGeneratorAction.hh"
#include "nEXORunAction.hh"
#include "nEXOEventAction.hh"
#include "nEXOTrackingAction.hh"
#include "nEXOSteppingAction.hh"
#include "nEXOStackingAction.hh"

nEXOActionInitialization::nEXOActionInitialization()
 : G4VUserActionInitialization()
{}

nEXOActionInitialization::~nEXOActionInitialization()
{}

void nEXOActionInitialization::BuildForMaster() const
{
  SetUserAction(new nEXORunAction);
}

void nEXOActionInitialization::Build() const
{
  SetUserAction(new nEXOPrimaryGeneratorAction());
  nEXORunAction* run_action = new nEXORunAction();
  SetUserAction(run_action);
  nEXOEventAction* event_action = new nEXOEventAction();
  SetUserAction(event_action);
  SetUserAction(new nEXOTrackingAction());
  SetUserAction(new nEXOSteppingAction());
  SetUserAction(new nEXOStackingAction());
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

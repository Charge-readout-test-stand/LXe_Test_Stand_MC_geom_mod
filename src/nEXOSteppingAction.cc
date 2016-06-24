#include "nEXOSteppingAction.hh"

nEXOSteppingAction::nEXOSteppingAction(){}
nEXOSteppingAction::~nEXOSteppingAction(){}

void nEXOSteppingAction::UserSteppingAction(const G4Step* step) 
{
  nEXOAnalysis::GetInstance()->SteppingAction(step);
}

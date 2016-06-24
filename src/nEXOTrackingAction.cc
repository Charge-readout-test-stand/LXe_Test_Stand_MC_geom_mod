#include "nEXOTrackingAction.hh"

nEXOTrackingAction::nEXOTrackingAction(){}
nEXOTrackingAction::~nEXOTrackingAction(){}

void nEXOTrackingAction::PreUserTrackingAction(const G4Track* track)
{
  nEXOAnalysis::GetInstance()->PreUserTrackingAction(track);
}

void nEXOTrackingAction::PostUserTrackingAction(const G4Track* track)
{
  nEXOAnalysis::GetInstance()->PostUserTrackingAction(track);
}

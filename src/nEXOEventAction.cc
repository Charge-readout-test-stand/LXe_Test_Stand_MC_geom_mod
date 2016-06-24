#include "nEXOEventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UImanager.hh"
#include "G4UnitsTable.hh"

nEXOEventAction::nEXOEventAction(){}
nEXOEventAction::~nEXOEventAction(){}

void nEXOEventAction::BeginOfEventAction(const G4Event* evt)
{
  nEXOAnalysis::GetInstance()->PrepareNewEvent(evt);
}

void nEXOEventAction::EndOfEventAction(const G4Event* evt)
{
  nEXOAnalysis::GetInstance()->EndOfEvent(evt);
}

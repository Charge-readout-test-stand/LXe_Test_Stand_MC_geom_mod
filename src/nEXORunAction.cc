#include "nEXORunAction.hh"
#include "nEXORunActionMessenger.hh"

#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"

#include "Randomize.hh"

nEXORunAction::nEXORunAction() 
{
  fMessenger = new nEXORunActionMessenger(this);
  fAnalysisMessenger = new nEXOAnalysisMessenger(nEXOAnalysis::GetInstance());
}

nEXORunAction::~nEXORunAction()
{
  delete fAnalysisMessenger;
  delete fMessenger;
}

void nEXORunAction::BeginOfRunAction(const G4Run* aRun)
{
  
  G4int runN = aRun->GetRunID();
  if ( runN % 1000 == 0 )
    G4cout << "### Run : " << runN << G4endl;
  nEXOAnalysis::GetInstance()->PrepareNewRun(aRun);
    
#ifdef G4VIS_USE
  if (G4VVisManager::GetConcreteInstance()) {
     G4UImanager* UI = G4UImanager::GetUIpointer(); 
     UI->ApplyCommand("/vis/clear/view");
     UI->ApplyCommand("/vis/draw/current");
  } 
#endif
}

void nEXORunAction::EndOfRunAction(const G4Run* aRun)
{
  nEXOAnalysis::GetInstance()->EndOfRun(aRun);
 
#ifdef G4VIS_USE
  if (G4VVisManager::GetConcreteInstance())
    G4UImanager::GetUIpointer()->ApplyCommand("/vis/show/view");
#endif
}

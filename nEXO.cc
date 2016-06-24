#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4PhysListFactory.hh"

#include "nEXODetectorConstruction.hh"

#ifdef G4_V10
#include "nEXOPhysicsList.hh"
#endif

#include "EXOPhysicsList.hh"

#ifdef G4_V10
#include "nEXOActionInitialization.hh"
#else
#include "nEXOPrimaryGeneratorAction.hh"

#include "nEXORunAction.hh"
#include "nEXOEventAction.hh"
#include "nEXOStackingAction.hh"
#include "nEXOTrackingAction.hh"
#include "nEXOSteppingAction.hh"
#endif

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " OpNovice [-m macro ] [-p physListMacro] [-u UIsession] [-t nThreads] [-r seed] "
           << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}

int main(int argc,char** argv)
{
  // Evaluate arguments
  //
  if ( argc > 9 ) {
    PrintUsage();
    return 1;
  }

  G4String macro;
  G4String session;
  G4String physListMacro = "";
#ifdef G4MULTITHREADED
  G4int nThreads = 0;
#endif

  G4long myseed = 345354;

  for ( G4int i=1; i<argc; i=i+2 ) {
     if      ( G4String(argv[i]) == "-m" ) macro         = argv[i+1];
     else if ( G4String(argv[i]) == "-p" ) physListMacro = argv[i+1];
     else if ( G4String(argv[i]) == "-u" ) session       = argv[i+1];
     else if ( G4String(argv[i]) == "-r" ) myseed        = atoi(argv[i+1]);
#ifdef G4MULTITHREADED
     else if ( G4String(argv[i]) == "-t" ) {
                    nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
    }
#endif
    else {
      PrintUsage();
      return 1;
    }
  }

  // Choose the Random engine
  //
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  // Construct the default run manager
  //
#ifdef G4MULTITHREADED
  G4MTRunManager * runManager = new G4MTRunManager;
  if ( nThreads > 0 ) runManager->SetNumberOfThreads(nThreads);
#else
  G4RunManager * runManager = new G4RunManager;
#endif

  // Seed the random number generator manually
  G4Random::setTheSeed(myseed);

  G4Random::showEngineStatus();
  G4cout << "Seed = " << G4Random::getTheSeed() << G4endl;

  // Set mandatory initialization classes
  //
  // Detector construction
  runManager-> SetUserInitialization(new nEXODetectorConstruction());

  // Physics list
  if (physListMacro == ""){
     G4cout << "EXOPhysicsList" << G4endl;
     runManager->SetUserInitialization(new EXOPhysicsList());
  }
  else if (physListMacro == "NTS"){
      G4cout << "NEUTRON STUFF" << G4endl;
      G4PhysListFactory physicsFactory;
      G4VUserPhysicsList *fPhysics = physicsFactory.GetReferencePhysList("QGSP_BERT_HP");
      runManager->SetUserInitialization(fPhysics);
  }
#ifdef G4_V10
  else {
     G4cout << "nEXOPhysicsList" << G4endl;
     G4VModularPhysicsList* physicsList = new nEXOPhysicsList(physListMacro);
     runManager->SetUserInitialization(physicsList);
  }

  // User action initialization
  runManager->SetUserInitialization(new nEXOActionInitialization());
#else

  // Set Mandatory User Action Classes
  runManager->SetUserAction(new nEXOPrimaryGeneratorAction());
  runManager->SetUserAction(new nEXORunAction()); 
  runManager->SetUserAction(new nEXOEventAction());
  runManager->SetUserAction(new nEXOStackingAction());
  runManager->SetUserAction(new nEXOTrackingAction());
  runManager->SetUserAction(new nEXOSteppingAction());
#endif

  // Initialize G4 kernel
  //
//  runManager->Initialize();

#ifdef G4VIS_USE
  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  //
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if ( macro.size() ) {
     // Batch mode
     G4String command = "/control/execute ";
     UImanager->ApplyCommand(command+macro);
  }
  else // Define UI session for interactive mode
  {
#ifdef G4UI_USE
     G4UIExecutive * ui = new G4UIExecutive(argc,argv,session);
#ifdef G4VIS_USE
     UImanager->ApplyCommand("/control/execute vis.mac");
#else
     UImanager->ApplyCommand("/control/execute nEXO.in");
#endif
     if (ui->IsGUI())
        UImanager->ApplyCommand("/control/execute gui.mac");
     ui->SessionStart();
     delete ui;
#endif
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

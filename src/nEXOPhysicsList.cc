#include "nEXOPhysicsList.hh"
#include "nEXOPhysicsListMessenger.hh"

#include "G4UImanager.hh"

#include "G4GenericPhysicsList.hh"

#include "G4OpticalPhysics.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4RadioactiveDecay.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "nEXOStepMax.hh"

#include "G4LossTableManager.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessTable.hh"

#include "G4PionDecayMakeSpin.hh"
#include "G4DecayWithSpin.hh"

#include "G4DecayTable.hh"
#include "G4MuonDecayChannelWithSpin.hh"
#include "G4MuonRadiativeDecayChannelWithSpin.hh"

#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4ProductionCuts.hh"

#include "G4PAIModel.hh"
#include "G4PAIPhotModel.hh"
#include "G4BraggIonGasModel.hh"
#include "G4BetheBlochIonGasModel.hh"

#include "G4IonFluctuations.hh"
#include "G4UniversalFluctuation.hh"

#include "G4EmConfigurator.hh"
#include "G4EmProcessOptions.hh"

#include "G4PhysicsListHelper.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

nEXOPhysicsList::nEXOPhysicsList(G4String physListMacro):G4VModularPhysicsList()
{
    G4cout << "using nEXOPhysicsList" << G4endl; 
    G4LossTableManager::Instance();

    SetDefaultCutValue(10.*mm);

    G4VModularPhysicsList* phys = NULL;
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (physListMacro != "") {
       phys = new G4GenericPhysicsList();
       UImanager->ApplyCommand("/control/execute "+physListMacro);
    } else {

      std::vector<G4String>* myConstr = new std::vector<G4String>;

      // EM Physics

//      myConstr->push_back("G4EmStandardPhysics");
      myConstr->push_back("G4EmStandardPhysics_option1");
//      myConstr->push_back("G4EmStandardPhysics_option2");
//      myConstr->push_back("G4EmStandardPhysics_option3");
//      myConstr->push_back("G4EmStandardPhysics_option4");
//      myConstr->push_back("G4EmLivermorePhysics");
//      myConstr->push_back("G4EmPenelopePhysics");

      G4EmProcessOptions emOptions;
      emOptions.SetFluo(true);  // Activate deexcitation and fluorescence
//      emOptions.SetAuger(true); // Activate Auger effect if deexcitation
//                                // is activated
//      emOptions.SetPIXE(true);  // Activate Particle Induced X-Ray Emission
//                                // (PIXE)

      // Synchroton Radiation & GammaNuclear & MuonNuclear Physics

      myConstr->push_back("G4EmExtraPhysics");

      // Decays

      myConstr->push_back("G4DecayPhysics");

      // Hadron Elastic scattering

      myConstr->push_back("G4HadronElasticPhysics");
//      myConstr->push_back("G4HadronElasticPhysicsHP");
//      myConstr->push_back("G4HadronElasticPhysicsLEND");

      // Hadron Physics

      myConstr->push_back("G4HadronPhysicsFTFP_BERT");
//      myConstr->push_back("G4HadronPhysicsQGSP_BERT");
//      myConstr->push_back("G4HadronPhysicsQGSP_FTFP_BERT");
//      myConstr->push_back("G4HadronPhysicsFTFP_BERT_HP");
//      myConstr->push_back("G4HadronPhysicsQGSP_BERT_HP");
//      myConstr->push_back("G4HadronPhysicsShielding");

      // Stopping Physics

      myConstr->push_back("G4StoppingPhysics");

      // Ion Physics

      myConstr->push_back("G4IonPhysics");
//      myConstr->push_back("G4IonQMDPhysics");
//      myConstr->push_back("G4IonElasticPhysics");

      // Do NOT add G4NeutronTrackingCut with _HP

      myConstr->push_back("G4NeutronTrackingCut");

      phys = new G4GenericPhysicsList(myConstr);
    }

    for (G4int i = 0; ; ++i) {
       G4VPhysicsConstructor* elem =
                  const_cast<G4VPhysicsConstructor*> (phys->GetPhysics(i));
       if (elem == NULL) break;
       G4cout << "RegisterPhysics: " << elem->GetPhysicsName() << G4endl;
       RegisterPhysics(elem);
    }

    // Optical Physics

    G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();

    opticalPhysics->SetWLSTimeProfile("delta");

    opticalPhysics->SetScintillationYieldFactor(1.0);
    opticalPhysics->SetScintillationExcitationRatio(0.0);

    opticalPhysics->SetMaxNumPhotonsPerStep(100);
    opticalPhysics->SetMaxBetaChangePerStep(10.0);

    opticalPhysics->SetTrackSecondariesFirst(kCerenkov,true);
    opticalPhysics->SetTrackSecondariesFirst(kScintillation,true);

    RegisterPhysics(opticalPhysics);

    // G4 Step Limiter

    RegisterPhysics(new G4StepLimiterPhysics());

    // Application StepMax Process

    fStepMaxProcess = new nEXOStepMax();

    fMessenger = new nEXOPhysicsListMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

nEXOPhysicsList::~nEXOPhysicsList()
{
    delete fMessenger;

    delete fStepMaxProcess;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOPhysicsList::ConstructParticle()
{
    G4VModularPhysicsList::ConstructParticle();

    G4DecayTable* MuonPlusDecayTable = new G4DecayTable();
    MuonPlusDecayTable -> Insert(new
                           G4MuonDecayChannelWithSpin("mu+",0.986));
    MuonPlusDecayTable -> Insert(new
                           G4MuonRadiativeDecayChannelWithSpin("mu+",0.014));
    G4MuonPlus::MuonPlusDefinition() -> SetDecayTable(MuonPlusDecayTable);

    G4DecayTable* MuonMinusDecayTable = new G4DecayTable();
    MuonMinusDecayTable -> Insert(new
                            G4MuonDecayChannelWithSpin("mu-",0.986));
    MuonMinusDecayTable -> Insert(new
                            G4MuonRadiativeDecayChannelWithSpin("mu-",0.014));
    G4MuonMinus::MuonMinusDefinition() -> SetDecayTable(MuonMinusDecayTable);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOPhysicsList::ConstructProcess()
{
    G4VModularPhysicsList::ConstructProcess();

    // RadioactiveDecays

    // RegisterPhysics(new G4RadioactiveDecayPhysics());

    G4RadioactiveDecay* radioactiveDecay = new G4RadioactiveDecay();

    radioactiveDecay->SetICM(true);                //Internal Conversion
    radioactiveDecay->SetARM(false);               //Atomic Rearangement

    G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
    ph->RegisterProcess(radioactiveDecay, G4GenericIon::GenericIon());

    fEM_Config = G4LossTableManager::Instance()->EmConfigurator();

    AddPAIModel();
    AddIonGasModel();

    fEM_Config->AddModels();

    G4DecayWithSpin* decayWithSpin = new G4DecayWithSpin();

    G4ProcessTable* processTable = G4ProcessTable::GetProcessTable();

    G4VProcess* decay;
    decay = processTable->FindProcess("Decay",G4MuonPlus::MuonPlus());

    G4ProcessManager* pManager;
    pManager = G4MuonPlus::MuonPlus()->GetProcessManager();

    if (pManager) {
      if (decay) pManager->RemoveProcess(decay);
      pManager->AddProcess(decayWithSpin);
      // set ordering for PostStepDoIt and AtRestDoIt
      pManager ->SetProcessOrdering(decayWithSpin, idxPostStep);
      pManager ->SetProcessOrdering(decayWithSpin, idxAtRest);
    }

    decay = processTable->FindProcess("Decay",G4MuonMinus::MuonMinus());

    pManager = G4MuonMinus::MuonMinus()->GetProcessManager();

    if (pManager) {
      if (decay) pManager->RemoveProcess(decay);
      pManager->AddProcess(decayWithSpin);
      // set ordering for PostStepDoIt and AtRestDoIt
      pManager ->SetProcessOrdering(decayWithSpin, idxPostStep);
      pManager ->SetProcessOrdering(decayWithSpin, idxAtRest);
    }

    G4PionDecayMakeSpin* poldecay = new G4PionDecayMakeSpin();

    decay = processTable->FindProcess("Decay",G4PionPlus::PionPlus());

    pManager = G4PionPlus::PionPlus()->GetProcessManager();

    if (pManager) {
      if (decay) pManager->RemoveProcess(decay);
      pManager->AddProcess(poldecay);
      // set ordering for PostStepDoIt and AtRestDoIt
      pManager ->SetProcessOrdering(poldecay, idxPostStep);
      pManager ->SetProcessOrdering(poldecay, idxAtRest);
    }

    decay = processTable->FindProcess("Decay",G4PionMinus::PionMinus());

    pManager = G4PionMinus::PionMinus()->GetProcessManager();

    if (pManager) {
      if (decay) pManager->RemoveProcess(decay);
      pManager->AddProcess(poldecay);
      // set ordering for PostStepDoIt and AtRestDoIt
      pManager ->SetProcessOrdering(poldecay, idxPostStep);
      pManager ->SetProcessOrdering(poldecay, idxAtRest);
    }

    AddStepMax();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOPhysicsList::SetCuts()
{
    if (verboseLevel > 0) {
        G4cout << "nEXOPhysicsList::SetCuts:";
        G4cout << "Default CutLength : "
               << G4BestUnit(defaultCutValue,"Length")
               << G4endl;
    }

    SetCutsWithDefault();

    G4Region* tpcRegion = G4RegionStore::GetInstance()->
                                   GetRegion("TPCRegion",false);

    if (tpcRegion) {
       G4ProductionCuts* tpcCuts = new G4ProductionCuts();
       tpcCuts->SetProductionCut(0.1*mm);
       tpcCuts->SetProductionCut(0.01*mm,
                                 G4ProductionCuts::GetIndex("gamma"));
       tpcRegion->SetProductionCuts(tpcCuts);
    }

    G4Region* cryostatRegion = G4RegionStore::GetInstance()->
                                   GetRegion("CryostatRegion",false);

    if (cryostatRegion) {
       G4ProductionCuts* innerCuts = new G4ProductionCuts();
       innerCuts->SetProductionCut(1.0*mm);
       innerCuts->SetProductionCut(0.1*mm,
                                   G4ProductionCuts::GetIndex("gamma"));
       cryostatRegion->SetProductionCuts(innerCuts);
    }

    if (verboseLevel>0) DumpCutValuesTable();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOPhysicsList::SetStepMax(G4double step)
{
  fStepMaxProcess->SetStepMax(step);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

nEXOStepMax* nEXOPhysicsList::GetStepMaxProcess()
{
  return fStepMaxProcess;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOPhysicsList::AddStepMax()
{
  // Step limitation seen as a process

  theParticleIterator->reset();
  while ((*theParticleIterator)()){
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();

      if (fStepMaxProcess->IsApplicable(*particle) && !particle->IsShortLived())
      {
         if (pmanager) pmanager ->AddDiscreteProcess(fStepMaxProcess);
      }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOPhysicsList::AddIonGasModel()
{
   theParticleIterator->reset();
   while ((*theParticleIterator)())
   {
     G4ParticleDefinition* particle = theParticleIterator->value();
     G4String particleName = particle->GetParticleName();

     if(particleName == "alpha" ||
        particleName == "He3" ||
        particleName == "GenericIon") {
         G4BraggIonGasModel* mod1 = new G4BraggIonGasModel();
         G4BetheBlochIonGasModel* mod2 = new G4BetheBlochIonGasModel();
         G4double eth = 2.*MeV*particle->GetPDGMass()/proton_mass_c2;
         fEM_Config->SetExtraEmModel(particleName,"ionIoni",mod1,"",0.0,eth,
                                    new G4IonFluctuations());
         fEM_Config->SetExtraEmModel(particleName,"ionIoni",mod2,"",eth,100*TeV,
                                    new G4UniversalFluctuation());
     }
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOPhysicsList::AddPAIModel()
{
    theParticleIterator->reset();
    while ((*theParticleIterator)())
    {
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4String particleName = particle->GetParticleName();

      if(particleName == "e-" || particleName == "e+") {
        G4PAIModel* pai = new G4PAIModel(particle,"PAIModel");
        // G4PAIPhotModel* pai = new G4PAIPhotModel(particle,"PAIPhotModel");
        fEM_Config->SetExtraEmModel(particleName,"eIoni",pai,"TPCRegion",
                                                           0.0,100.*GeV,pai);
      } else if (particleName == "mu-" || particleName == "mu+") {
        G4PAIModel* pai = new G4PAIModel(particle,"PAIModel");
        // G4PAIPhotModel* pai = new G4PAIPhotModel(particle,"PAIPhotModel");
        fEM_Config->SetExtraEmModel(particleName,"muIoni",pai,"TPCRegion",
                                                           0.0,100.*GeV,pai);
      } else if (particleName == "proton" ||
                 particleName == "pi+" ||
                 particleName == "pi-" ) {
        G4PAIModel* pai = new G4PAIModel(particle,"PAIModel");
        // G4PAIPhotModel* pai = new G4PAIPhotModel(particle,"PAIPhotModel");
        fEM_Config->SetExtraEmModel(particleName,"hIoni",pai,"TPCRegion",
                                                           0.0,100.*GeV,pai);
      }
    }
}

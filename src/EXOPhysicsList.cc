#include "EXOPhysicsList.hh"

#include "G4Region.hh"
#include "G4RegionStore.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

EXOPhysicsListMessenger::EXOPhysicsListMessenger(EXOPhysicsList* physList)
{
  fPhysicsList = physList;
  
  fDirectory = new G4UIdirectory("/EXOPhysicsList/");
  fDirectory->SetGuidance("Control of the EXO physics list options.");

  fDefaultCutValueCmd = new G4UIcmdWithADoubleAndUnit("/EXOPhysicsList/defaultCutValue",this);
  fDefaultCutValueCmd->SetGuidance("Minimum length to track particle.");
  fDefaultCutValueCmd->SetUnitCategory("Length");
  fDefaultCutValueCmd->SetDefaultValue(1*mm);
  fDefaultCutValueCmd->AvailableForStates(G4State_Idle);

  fCutValueInnerRegionCmd = new G4UIcmdWithADoubleAndUnit("/EXOPhysicsList/cutValueInnerRegion",this);
  fCutValueInnerRegionCmd->SetGuidance("Minimum length to track particle inside the inner cryostat region.");
  fCutValueInnerRegionCmd->SetUnitCategory("Length");
  fCutValueInnerRegionCmd->SetDefaultValue(100*nm);
  fCutValueInnerRegionCmd->AvailableForStates(G4State_Idle);

  fCutValueInsideTPCCmd = new G4UIcmdWithADoubleAndUnit("/EXOPhysicsList/cutValueInsideTPC",this);
  fCutValueInsideTPCCmd->SetGuidance("Minimum length to track particle inside the TPC.");
  fCutValueInsideTPCCmd->SetUnitCategory("Length");
  fCutValueInsideTPCCmd->SetDefaultValue(100*nm);
  fCutValueInsideTPCCmd->AvailableForStates(G4State_Idle);
  
  fEnableLightCmd = new G4UIcmdWithABool("/EXOPhysicsList/enableLight",this);
  fEnableLightCmd->SetGuidance("Enable light in the physics list");
  fEnableLightCmd->SetDefaultValue(false);
  fEnableLightCmd->AvailableForStates(G4State_PreInit);

}

EXOPhysicsListMessenger::~EXOPhysicsListMessenger()
{
  delete fDirectory;
  delete fDefaultCutValueCmd;
  delete fCutValueInnerRegionCmd;
  delete fCutValueInsideTPCCmd;
}

void EXOPhysicsListMessenger::SetNewValue(G4UIcommand* cmd, G4String val)
{
  if (cmd == fDefaultCutValueCmd)
  {
    fPhysicsList->SetDefaultCut(fDefaultCutValueCmd->GetNewDoubleValue(val));
    fPhysicsList->SetCuts();
  }
  else if (cmd == fCutValueInnerRegionCmd)
  {
    fPhysicsList->SetCutInnerRegion(fCutValueInnerRegionCmd->GetNewDoubleValue(val));
    fPhysicsList->SetCuts();
  }
  else if (cmd == fCutValueInsideTPCCmd)
  {
    fPhysicsList->SetCutInsideTPC(fCutValueInsideTPCCmd->GetNewDoubleValue(val));
    fPhysicsList->SetCuts();
  }
  else if (cmd == fEnableLightCmd){
    fPhysicsList->GetEXOPhysics()->SetEnableOptics(fEnableLightCmd->GetNewBoolValue(val));
  }
  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EXOPhysicsList::EXOPhysicsList() : G4VModularPhysicsList()
{
  // Default cuts are applied everywhere, then a G4Region is defined below to
  // set cuts for innerCryostat and all daughters to much finer cuts.

  fMessenger = new EXOPhysicsListMessenger(this);
  
    defaultCutValue  = 1.0*mm;
    cutForGamma      = defaultCutValue;
    cutForElectron   = defaultCutValue;
    cutForPositron   = defaultCutValue;
    cutForProton     = defaultCutValue;
    cutForAlpha      = defaultCutValue;
    cutForGenericIon = defaultCutValue;
    cutInsideTPC     = 0.1*mm;//100.0*nm;//1.0*mm;//100.0*nm;
    cutInnerRegion   = 1.0*mm;//100.0*nm;//0.1*mm;//100.0*nm;

    SetVerboseLevel(1);

    fPhysics = new EXOPhysics();

    RegisterPhysics(fPhysics);
    G4cout << "using EXOPhysicsList" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EXOPhysicsList::~EXOPhysicsList()
{
  //delete fPhysics;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EXOPhysicsList::ConstructParticle()
{
    G4VModularPhysicsList::ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EXOPhysicsList::ConstructProcess()
{
    G4VModularPhysicsList::ConstructProcess();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EXOPhysicsList::SetCuts()
{
  if (verboseLevel >0) {
    G4cout << "EXOPhysicsList::SetCuts:";
    G4cout << "CutLength: " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }

  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(500*eV, 100*MeV);
  if (cutForGamma>0.     ) SetCutValue(cutForGamma, "gamma");
  if (cutForElectron>0.  ) SetCutValue(cutForElectron, "e-");
  if (cutForPositron>0.  ) SetCutValue(cutForPositron, "e+");
  if (cutForProton>0.    ) SetCutValue(cutForProton, "proton");
  if (cutForAlpha>0.     ) SetCutValue(cutForAlpha, "alpha");
  if (cutForGenericIon>0.) SetCutValue(cutForGenericIon, "GenericIon");

  if (cutInsideTPC>0.) {
    G4Region* region = G4RegionStore::GetInstance()->GetRegion("TPCRegion");
    G4ProductionCuts* cuts = new G4ProductionCuts;
    cuts->SetProductionCut(cutInsideTPC);
    region->SetProductionCuts(cuts);
  }

  if (cutInnerRegion>0.) {
    // Set cuts for inner detector stuff with finer tracking
    G4Region* region =
    G4RegionStore::GetInstance()->GetRegion("CryostatRegion");
    G4ProductionCuts* cuts = new G4ProductionCuts;
    cuts->SetProductionCut(cutInnerRegion,G4ProductionCuts::GetIndex("gamma"));
    cuts->SetProductionCut(cutInnerRegion,G4ProductionCuts::GetIndex("e-"));
    cuts->SetProductionCut(cutInnerRegion,G4ProductionCuts::GetIndex("e+"));
    region->SetProductionCuts(cuts);
    G4cout << " InnerRegion cuts are set to: " << G4endl;
    G4cout << " gamma cut    = " << cuts->GetProductionCut("gamma")/mm
           << "mm"<< G4endl;
    G4cout << " electron cut = " << cuts->GetProductionCut("e-")/mm
           << "mm"<< G4endl;
    G4cout << " positron cut = " << cuts->GetProductionCut("e+")/mm
           << "mm"<< G4endl;
  }
  if (cutInsideTPC>0.) {
    // Set cuts for inner detector stuff with finer tracking
    G4Region* region = G4RegionStore::GetInstance()->GetRegion("TPCRegion");
    G4ProductionCuts* cuts = new G4ProductionCuts;
    cuts->SetProductionCut(cutInsideTPC,G4ProductionCuts::GetIndex("gamma"));
    cuts->SetProductionCut(cutInsideTPC,G4ProductionCuts::GetIndex("e-"));
    cuts->SetProductionCut(cutInsideTPC,G4ProductionCuts::GetIndex("e+"));
    region->SetProductionCuts(cuts);
    G4cout << "InsideTPC cuts are set to: " << G4endl;
    G4cout << " gamma cut    = "<< cuts->GetProductionCut("gamma")/mm
           << "mm" << G4endl;
    G4cout << "  electron cut = " << cuts->GetProductionCut("e-")/mm
           << "mm" << G4endl;
    G4cout << "  positron cut = " << cuts->GetProductionCut("e+")/mm
           << " mm" << G4endl;
  }

  if (verboseLevel>0) DumpCutValuesTable();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

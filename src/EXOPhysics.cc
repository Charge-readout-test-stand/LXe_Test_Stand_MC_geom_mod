#include "globals.hh"
#include "G4ios.hh"

#include "EXOPhysics.hh"
#include "nEXOG4EmLivermorePhysics.hh"

#include "G4LossTableManager.hh"
#include "G4ProcessManager.hh"

#include "G4PhysicsListHelper.hh"
#include "G4BuilderType.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EXOPhysics::EXOPhysics() : G4VPhysicsConstructor("EXOPhysics")
{
  SetVerboseLevel(0);

  enablePenelope   = true;
  enableMuNuclear  = true;
  enableHPNeutron  = true;
  enableOptics     = true;
  
  G4LossTableManager::Instance();
  SetPhysicsType(bElectromagnetic);

  fEMPhysicsList = new nEXOG4EmLivermorePhysics();
  G4cout << "using EXOPhysics" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EXOPhysics::~EXOPhysics()
{}

#include "G4ParticleTypes.hh"
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "nEXOG4ThermalElectron.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EXOPhysics::ConstructParticle()
{
  nEXOG4ThermalElectron::ThermalElectronDefinition(); //NEST

  G4BosonConstructor::ConstructParticle();
  G4LeptonConstructor::ConstructParticle();
  G4MesonConstructor::ConstructParticle();
  G4BaryonConstructor::ConstructParticle();
  G4IonConstructor::ConstructParticle();
  G4ShortLivedConstructor::ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EXOPhysics::ConstructProcess()
{
  ConstructEM();
  ConstructOp();
  ConstructHad();
  ConstructGeneral();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4ionIonisation.hh"
#include "G4RayleighScattering.hh"

// Penelope Model
#include "G4PenelopeRayleighModel.hh"
#include "G4PenelopeComptonModel.hh"
#include "G4PenelopePhotoElectricModel.hh"
#include "G4PenelopeGammaConversionModel.hh"
#include "G4PenelopeIonisationModel.hh"
#include "G4PenelopeBremsstrahlungModel.hh"
#include "G4PenelopeAnnihilationModel.hh"

#include "G4UniversalFluctuation.hh"

#include "G4UrbanMscModel.hh"
#include "G4WentzelVIModel.hh"
#include "G4CoulombScattering.hh"
#include "G4eCoulombScatteringModel.hh"
#include "G4WentzelVIModel.hh"
#include "G4NuclearStopping.hh"
#include "G4IonParametrisedLossModel.hh"

#include "G4EmProcessOptions.hh"

#include "G4UAtomicDeexcitation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EXOPhysics::ConstructEM()
{
  fEMPhysicsList->ConstructProcess();

}

// optics
#include "G4OpticalPhysics.hh"
#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "nEXOG4S1Light.hh"
#include "nEXOG4S2Light.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpWLS.hh"
#include "G4EmSaturation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EXOPhysics::ConstructOp()
{
  if (!enableOptics) return;

  G4Cerenkov* theCerenkovProcess            = new G4Cerenkov("Cerenkov");
  //G4Scintillation* theScintillationProcess = new G4Scintillation("Scintillation");
  nEXOG4S1Light* theScintillationProcess = new nEXOG4S1Light();
  G4OpAbsorption* theAbsorptionProcess     = new G4OpAbsorption();
  G4OpRayleigh* theRayleighScatteringProcess = new G4OpRayleigh();
  G4OpBoundaryProcess* theBoundaryProcess  = new G4OpBoundaryProcess();
//  G4OpWLS* theWLSProcess  = new G4OpWLS();
//  theWLSProcess->UseTimeProfile("delta");

//  theCerenkovProcess->SetMaxNumPhotonsPerStep(300);
  theCerenkovProcess->SetTrackSecondariesFirst(true);

  theScintillationProcess->SetScintillationYieldFactor(1.); // NEST: 1. = enabled, 0. = disabled
  theScintillationProcess->SetTrackSecondariesFirst(true);

  //G4OpticalSurfaceModel themodel = unified;
  //theBoundaryProcess->SetModel(themodel);

  // Use Birks Correction in the Scintillation process

  //G4EmSaturation* emSaturation =
  //                             G4LossTableManager::Instance()->EmSaturation();
  //theScintillationProcess->AddSaturation(emSaturation);

  aParticleIterator->reset();
  while( (*aParticleIterator)() ) {

    G4ParticleDefinition* particle = aParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if (theCerenkovProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(theCerenkovProcess);
      pmanager->SetProcessOrdering(theCerenkovProcess, idxPostStep);
    }

    if (theScintillationProcess->IsApplicable(*particle)) {
      // mostly charged ones
      //pmanager->AddProcess(theScintillationProcess);
      //pmanager->SetProcessOrderingToLast(theScintillationProcess, idxAtRest);
      //pmanager->SetProcessOrderingToLast(theScintillationProcess, idxPostStep);
      pmanager->AddProcess(theScintillationProcess, ordDefault+1, ordInActive, ordDefault+1); //for nest
    }
    if (particleName == "opticalphoton") {
      // photons only
      pmanager->AddDiscreteProcess(theAbsorptionProcess);
      pmanager->AddDiscreteProcess(theRayleighScatteringProcess);
      pmanager->AddDiscreteProcess(theBoundaryProcess);
//      pmanager->AddDiscreteProcess(theWLSProcess);
    } 
  }

}

// neutron physic
#include "G4MuonNuclearProcess.hh"
#include "G4MuonVDNuclearModel.hh"

#include "G4MuonMinusCapture.hh"
#include "G4HadronCaptureProcess.hh"
#include "G4NeutronRadCapture.hh"
#include "G4NeutronCaptureXS.hh"

#include "G4HadronElasticProcess.hh"
#include "G4HadronElastic.hh"
#include "G4NeutronHPElastic.hh"
#include "G4NeutronHPElasticData.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4BGGNucleonInelasticXS.hh"
#include "G4CascadeInterface.hh"

#include "G4FTFModel.hh"
#include "G4TheoFSGenerator.hh"
#include "G4ExcitationHandler.hh"
#include "G4PreCompoundModel.hh"
#include "G4GeneratorPrecompoundInterface.hh"
#include "G4QGSMFragmentation.hh"
#include "G4ExcitedStringDecay.hh"

#include "G4NeutronHPInelastic.hh"
#include "G4NeutronHPInelasticData.hh"
#include "G4HadronCaptureProcess.hh"
#include "G4NeutronHPCapture.hh"
#include "G4NeutronHPCaptureData.hh"
#include "G4HadronFissionProcess.hh"
#include "G4LFission.hh"
#include "G4NeutronHPFission.hh"
#include "G4NeutronHPFissionData.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EXOPhysics::ConstructHad()
{
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

  aParticleIterator->reset();
  while( (*aParticleIterator)() ){

    G4ParticleDefinition* particle = aParticleIterator->value();
    G4String particleName = particle->GetParticleName();

    if       ( particleName == "mu+" ||
               particleName == "mu-"    ) {
      // muon
      if (enableMuNuclear) {
        G4MuonNuclearProcess* theMuNuclearProcess = new G4MuonNuclearProcess();
        G4MuonVDNuclearModel* muNucModel = new G4MuonVDNuclearModel();
        theMuNuclearProcess->RegisterMe(muNucModel);
        ph->RegisterProcess(theMuNuclearProcess,particle);
	if( particleName == "mu-" )
          ph->RegisterProcess(new G4MuonMinusCapture(),particle);
      }

    } else if( particleName == "neutron" ) {
      // neutrons
      if (enableHPNeutron) {

	// elastic scattering
        G4HadronElasticProcess* theElasticProcess = new G4HadronElasticProcess;
        G4HadronElastic* theElasticModel = new G4HadronElastic;
	theElasticModel->SetMinEnergy(19*MeV);
	G4NeutronHPElastic *theElasticNeutronHP = new G4NeutronHPElastic;
	theElasticNeutronHP->SetMaxEnergy(19*MeV);
	theElasticProcess->RegisterMe(theElasticModel);
	theElasticProcess->RegisterMe(theElasticNeutronHP);
	theElasticProcess->AddDataSet(new G4NeutronHPElasticData());
        ph->RegisterProcess(theElasticProcess,particle);

	// inelastic scattering
	G4NeutronInelasticProcess* theInelasticProcess =
	  new G4NeutronInelasticProcess("inelastic");
        theInelasticProcess->
             AddDataSet( new G4BGGNucleonInelasticXS( G4Neutron::Neutron() ) );

        G4FTFModel* theStringModel = new G4FTFModel;
        G4ExcitedStringDecay* theStringDecay =
                     new G4ExcitedStringDecay( new G4LundStringFragmentation );
        theStringModel->SetFragmentationModel( theStringDecay );
        G4PreCompoundModel* thePreEquilib =
                             new G4PreCompoundModel( new G4ExcitationHandler );
        G4GeneratorPrecompoundInterface* theCascade =
                          new G4GeneratorPrecompoundInterface( thePreEquilib );

        G4TheoFSGenerator* theModel = new G4TheoFSGenerator( "FTFP" );
        theModel->SetHighEnergyGenerator( theStringModel );
        theModel->SetTransport( theCascade );
        const G4double theFTFMin =    4.0*GeV;
        const G4double theFTFMax =  100.0*TeV;
        theModel->SetMinEnergy( theFTFMin );
        theModel->SetMaxEnergy( theFTFMax );

        G4CascadeInterface* theBERTModel = new G4CascadeInterface;
        const G4double theBERTMin =  19.0*MeV;
        const G4double theBERTMax =   5.0*GeV;
        theBERTModel->SetMinEnergy( theBERTMin );
        theBERTModel->SetMaxEnergy( theBERTMax );
       
        theInelasticProcess->RegisterMe(theBERTModel);
        theInelasticProcess->RegisterMe(theModel);

	G4NeutronHPInelastic * theInelasticNeutronHP =
	  new G4NeutronHPInelastic;
	theInelasticNeutronHP->SetMaxEnergy(19*MeV);
	theInelasticProcess->RegisterMe(theInelasticNeutronHP);
	theInelasticProcess->AddDataSet(new G4NeutronHPInelasticData());
        ph->RegisterProcess(theInelasticProcess,particle);

	// capture
        G4HadronCaptureProcess *theCaptureProcess =
                                        new G4HadronCaptureProcess("nCapture");

	G4NeutronHPCapture * theCaptureNeutronHP = new G4NeutronHPCapture;

        const G4double theHPMin =      0.0*GeV;
        const G4double theHPMax =     19.0*MeV;

        theCaptureNeutronHP->SetMinEnergy( theHPMin );
        theCaptureNeutronHP->SetMaxEnergy( theHPMax );

	theCaptureProcess->RegisterMe(theCaptureNeutronHP);
	theCaptureProcess->AddDataSet(new G4NeutronHPCaptureData());
        ph->RegisterProcess(theCaptureProcess,particle);

	// fission
	G4HadronFissionProcess* theFissionProcess =
	  new G4HadronFissionProcess;
	G4LFission* theFissionModel = new G4LFission;
	theFissionModel->SetMinEnergy(19*MeV);
	theFissionProcess->RegisterMe(theFissionModel);
	G4NeutronHPFission * theFissionNeutronHP = new G4NeutronHPFission;
	theFissionNeutronHP->SetMaxEnergy(19*MeV);
	theFissionProcess->RegisterMe(theFissionNeutronHP);
	theFissionProcess->AddDataSet(new G4NeutronHPFissionData());
        ph->RegisterProcess(theFissionProcess,particle);

      } else {

        // Elastic scattering
        G4HadronElasticProcess* theElasticProcess =
                                                  new G4HadronElasticProcess();
        G4HadronElastic* protelMod = new G4HadronElastic;
        theElasticProcess->RegisterMe(protelMod);
        G4PhysicsListHelper::GetPhysicsListHelper()->
                                   RegisterProcess(theElasticProcess,particle);
        G4HadronCaptureProcess *theCaptureProcess =
                                        new G4HadronCaptureProcess("nCapture");
        theCaptureProcess->AddDataSet(new G4NeutronCaptureXS());
        theCaptureProcess->RegisterMe(new G4NeutronRadCapture());
        ph->RegisterProcess(theCaptureProcess,particle);
      }
    //} else if( particleName == "antineutron" ) {
    //} else if( particleName == "proton" ) {
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4Decay.hh"
#include "G4IonTable.hh"
#include "G4GenericIon.hh"
#include "G4RadioactiveDecay.hh"

void EXOPhysics::ConstructGeneral()
{
  G4Decay* theDecayProcess = new G4Decay();
  G4RadioactiveDecay*  theRadioactiveDecay = new G4RadioactiveDecay();

  aParticleIterator->reset();
  while( (*aParticleIterator)() ){
    G4ParticleDefinition* particle = aParticleIterator->value();
    if (theDecayProcess->IsApplicable(*particle)) {
       G4PhysicsListHelper::GetPhysicsListHelper()->
                                    RegisterProcess(theDecayProcess,particle);
    }
  }

  G4PhysicsListHelper::GetPhysicsListHelper()->
        RegisterProcess(theRadioactiveDecay,G4GenericIon::GenericIon());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

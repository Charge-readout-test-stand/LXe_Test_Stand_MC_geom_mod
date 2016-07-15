
#include "nEXOMaterials.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4SystemOfUnits.hh"
#include "nEXONestMaterialTable.hh"

nEXOMaterials* nEXOMaterials::fInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

nEXOMaterials::nEXOMaterials()
{
  fNistMan = G4NistManager::Instance();

  fNistMan->SetVerbose(2);

  CreateMaterials();
  AddMatProperties();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

nEXOMaterials::~nEXOMaterials()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

nEXOMaterials* nEXOMaterials::GetInstance()
{
  if (fInstance == 0) fInstance = new nEXOMaterials();
  return fInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* nEXOMaterials::GetMaterial(const G4String material)
{
  G4Material* mat =  fNistMan->FindOrBuildMaterial(material);

  if (!mat) mat = G4Material::GetMaterial(material);
  if (!mat) {G4cout << material << "Not Found, Please Retry"<< G4endl;}

  return mat;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOMaterials::CreateMaterials()
{
  G4double density;

  std::vector<G4String> elements;

  std::vector<G4int>  natoms;
  std::vector<G4double> fractionMass;

  // Materials Definitions
  // =====================

  // Define Vacuum

  fNistMan->FindOrBuildMaterial("G4_Galactic");

  // Define Air

  fNistMan->FindOrBuildMaterial("G4_AIR");

  // Define Water

  fNistMan->FindOrBuildMaterial("G4_WATER");

  // Define Kapton

  fNistMan->FindOrBuildMaterial("G4_KAPTON");

  // Define Light Kapton - density decreased by 1/10 (should be 1.43*g/cm3)

  elements.push_back("H");     fractionMass.push_back(0.026362);
  elements.push_back("C");     fractionMass.push_back(0.691133);
  elements.push_back("N");     fractionMass.push_back(0.073270);
  elements.push_back("O");     fractionMass.push_back(0.209235);

  density = 0.143*g/cm3;

  fNistMan->ConstructNewMaterial("KaptonLight", elements, fractionMass, density);

  elements.clear();
  fractionMass.clear();
  //new G4Material("KaptonLight",density=0.143*g/cm3,"G4_KAPTON");

  // Define Copper

  fNistMan->FindOrBuildMaterial("G4_Cu");

  // Define Al

  fNistMan->FindOrBuildMaterial("G4_Al");

  // Define Lead

  fNistMan->FindOrBuildMaterial("G4_Pb");

  // Define Silicon

  fNistMan->FindOrBuildMaterial("G4_Si");

  fNistMan->FindOrBuildMaterial("liquidXe");

  // Define light Silicon - density decreased by 1/2 (should be 2.33g/cm3)
  //elements.push_back("Si");    natoms.push_back(1);
  
  //density = 1.165*g/cm3;
  
  //fNistMan->ConstructNewMaterial("SiliconLight", elements, fractionMass, density);
  
  //elements.clear();
  //fractionMass.clear();
  //new G4Material("SiliconLight",density=1.165*g/cm3,"G4_Si");

  // Define SILICON DIOXIDE

  fNistMan->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

  // Define Aluminum Oxide (aka Sapphire, Al2O3)

  fNistMan->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");

  // Define Stainless Steel

  fNistMan->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  // Define light Stainless Steel - density = 0.462*g/cm3
  elements.push_back("Fe");     natoms.push_back(74);
  elements.push_back("Cr");     natoms.push_back(8);
  elements.push_back("Ni");     natoms.push_back(18);

  density = 0.462*g/cm3;

  fNistMan->ConstructNewMaterial("StainlessSteelLight", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //new G4Material("StainlessSteelLight",
  //               density=0.462*g/cm3,
  //               "G4_STAINLESS-STEEL");

  // Define Titanium

  fNistMan->FindOrBuildMaterial("G4_Ti");

  // Define Liquid Xenon (Nist-density=2.953*g/cm3 vs nEXO-density=3.057*g/cm3)
  // To use NistManager, we cannot set material temperature and pressure, so here we changed to use normal method to define liquid xenon.
  density = 3.057*g/cm3;

  G4Element* natXe = fNistMan->FindOrBuildElement(54);
  G4Material* liquidXe = new G4Material("liquidXe", density, 1, kStateLiquid, 173.15*kelvin, 1*atmosphere);
  liquidXe->AddElement(natXe, 1.0);

  // Define Teflon

  fNistMan->FindOrBuildMaterial("G4_TEFLON");

  // Define light Teflon - density decreased by 1/2 (should be 2.2*g/cm3)
  elements.push_back("C");    fractionMass.push_back(0.240183);
  elements.push_back("F");    fractionMass.push_back(0.759817);

  density = 1.1*g/cm3;
  
  fNistMan->ConstructNewMaterial("TeflonLight", elements, fractionMass, density);
  
  elements.clear();
  fractionMass.clear();
  //new G4Material("TeflonLight",density=1.1*g/cm3,"G4_TEFLON");

  // Define C4H3OF7

  elements.push_back("C");     natoms.push_back(4);
  elements.push_back("H");     natoms.push_back(3);
  elements.push_back("O");     natoms.push_back(1);
  elements.push_back("F");     natoms.push_back(7);

  density = 1.77*g/cm3;

  fNistMan->ConstructNewMaterial("HFE", elements, natoms, density);

  elements.clear();
  natoms.clear();

  // Define Acrylic - C5O2H8

  elements.push_back("C");     natoms.push_back(5);
  elements.push_back("O");     natoms.push_back(2);
  elements.push_back("H");     natoms.push_back(8);

  density = 1.185*g/cm3;

  fNistMan->ConstructNewMaterial("Acrylic", elements, natoms, density);

  elements.clear();
  natoms.clear();

  // Define TPB
  
  elements.push_back("C");     natoms.push_back(28);
  elements.push_back("H");     natoms.push_back(22);

  density = 1.185*g/cm3; // this number is fake.

  fNistMan->ConstructNewMaterial("TPB", elements, natoms, density);

  elements.clear();
  natoms.clear();

  // Define HDPE

  elements.push_back("H");     fractionMass.push_back(0.144);
  elements.push_back("C");     fractionMass.push_back(0.856);

  density= 0.95*g/cm3;

  fNistMan->ConstructNewMaterial("HDPE", elements, fractionMass, density);

  // Define ShotCrete

  elements.push_back("Al");     fractionMass.push_back(0.061);
  elements.push_back("Ca");     fractionMass.push_back(0.096);
  elements.push_back("Fe");     fractionMass.push_back(0.026);
  elements.push_back("K");      fractionMass.push_back(0.018);
  elements.push_back("Mg");     fractionMass.push_back(0.012);
  elements.push_back("Mn");     fractionMass.push_back(0.004);
  elements.push_back("Na");     fractionMass.push_back(0.023);
  elements.push_back("Si");     fractionMass.push_back(0.280);
  elements.push_back("O");      fractionMass.push_back(0.480);

  density = 2.32*g/cm3;

  fNistMan->ConstructNewMaterial("ShotCrete", elements, fractionMass, density);

  elements.clear();
  fractionMass.clear();

  // Define Concrete

  elements.push_back("Al");     fractionMass.push_back(0.060);
  elements.push_back("Ca");     fractionMass.push_back(0.101);
  elements.push_back("Fe");     fractionMass.push_back(0.026);
  elements.push_back("K");      fractionMass.push_back(0.017);
  elements.push_back("Mg");     fractionMass.push_back(0.012);
  elements.push_back("Mn");     fractionMass.push_back(0.004);
  elements.push_back("Na");     fractionMass.push_back(0.023);
  elements.push_back("Si");     fractionMass.push_back(0.277);
  elements.push_back("O");      fractionMass.push_back(0.480);

  density = 2.32*g/cm3;

  fNistMan->ConstructNewMaterial("Concrete", elements, fractionMass, density);

  elements.clear();
  fractionMass.clear();

  // Define Rock

  elements.push_back("Al");     fractionMass.push_back(0.060);
  elements.push_back("Ca");     fractionMass.push_back(0.101);
  elements.push_back("Fe");     fractionMass.push_back(0.026);
  elements.push_back("K");      fractionMass.push_back(0.017);
  elements.push_back("Mg");     fractionMass.push_back(0.012);
  elements.push_back("Mn");     fractionMass.push_back(0.004);
  elements.push_back("Na");     fractionMass.push_back(0.023);
  elements.push_back("Si");     fractionMass.push_back(0.277);
  elements.push_back("O");      fractionMass.push_back(0.480);

  density=2.32*g/cm3;  

  fNistMan->ConstructNewMaterial("Rock", elements, fractionMass, density);

  elements.clear();
  fractionMass.clear();

  // Carbon fiber

  elements.push_back("C");      fractionMass.push_back(1.);
  density=1.88*g/cm3; //updated based on Allen's request of Feb 12, 2015, was 1.80*g/cm3;

  fNistMan->ConstructNewMaterial("CarbonFiber", elements, fractionMass, density);

  elements.clear();
  fractionMass.clear();

  // Silicone (assumed to be polydimethylsiloxane: [(CH3)2SiO]n )

  elements.push_back("C");      fractionMass.push_back(2.);
  elements.push_back("H");      fractionMass.push_back(6.);
  elements.push_back("Si");      fractionMass.push_back(1.);
  elements.push_back("O");      fractionMass.push_back(1.);
  density=0.965*g/cm3; 

  fNistMan->ConstructNewMaterial("Silicone", elements, fractionMass, density);

  elements.clear();
  fractionMass.clear();

  // SS vessel, 2nd option

  elements.push_back("Fe");     fractionMass.push_back(0.68);
  elements.push_back("Cr");     fractionMass.push_back(0.19);
  elements.push_back("Ni");     fractionMass.push_back(0.10);
  elements.push_back("Mn");     fractionMass.push_back(0.02);
  elements.push_back("Si");     fractionMass.push_back(0.01);

  density = 8.02*g/cm3;

  fNistMan->ConstructNewMaterial("StainlessSteelCryo", elements, fractionMass, density);

  elements.clear();
  fractionMass.clear();
  
  G4cout << G4endl << "The materials defined are: " << G4endl << G4endl;
  
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void nEXOMaterials::AddMatProperties() {

  //============================================================================
  // Add the properties for liquid xenon, most of parameters are from nest.
  //============================================================================
  G4Material* lXe =  fNistMan->FindOrBuildMaterial("liquidXe");

  std::cout << "The temparature of liquid xenon is " << lXe->GetTemperature() << " and density " << lXe->GetDensity()*cm3/g << std::endl;

  num_pp = 22;
  const G4int NUM_PP = num_pp;

  fPhotonWavelengths = new G4double[NUM_PP];
  fPhotonEnergies = new G4double[NUM_PP];

  fPhotonWavelengths[0] = 144.5;
  fPhotonWavelengths[1] = 175.8641;
  fPhotonWavelengths[2] = 177.6278; 
  fPhotonWavelengths[3] = 179.4272;
  fPhotonWavelengths[4] = 193.6;
  fPhotonWavelengths[5] = 250.3;
  fPhotonWavelengths[6] = 303.4;
  fPhotonWavelengths[7] = 340.4;
  fPhotonWavelengths[8] = 410.2;
  fPhotonWavelengths[9] = 467.8;
  fPhotonWavelengths[10] = 508.6;
  fPhotonWavelengths[11] = 546.1;
  fPhotonWavelengths[12] = 627.8;
  fPhotonWavelengths[13] = 706.5;
  fPhotonWavelengths[14] = 766.5;
  fPhotonWavelengths[15] = 844.7;
  fPhotonWavelengths[16] = 1000.0;
  fPhotonWavelengths[17] = 1300.0;
  fPhotonWavelengths[18] = 1529.6;
  fPhotonWavelengths[19] = 1600.0;
  fPhotonWavelengths[20] = 1800.0;
  fPhotonWavelengths[21] = 2058.2;

  for( G4int i=0; i<NUM_PP; i++ )
  {
    fPhotonEnergies[NUM_PP-1-i] = (4.13566743E-15*299792458/(fPhotonWavelengths[i]*1.E-9))*eV;
  }

  fPhotonULEnergies = new G4double[2];
  fPhotonULEnergies[0] = fPhotonEnergies[0];
  fPhotonULEnergies[1] = fPhotonEnergies[NUM_PP-1];
  
  //refractive index values taken from: 'New approach to the calculation
  //of the refractive index of liquid and solid xenon' (some values merely estimated)
  //https://ir.kochi-u.ac.jp/dspace/bitstream/10126/4658/1/Hitachi2005JCP234508.pdf***
  G4double* LXeRefractiveIndex = new G4double[NUM_PP];
  LXeRefractiveIndex[21-0] = 1.82;
  LXeRefractiveIndex[21-1] = 1.663;// originally ~1.63 <-> possible shift needed based on temperature?
  LXeRefractiveIndex[21-2] = 1.648;// originally ~1.61
  LXeRefractiveIndex[21-3] = 1.637;// originally ~1.58
  LXeRefractiveIndex[21-4] = 1.576;
  LXeRefractiveIndex[21-5] = 1.468;
  LXeRefractiveIndex[21-6] = 1.429;
  LXeRefractiveIndex[21-7] = 1.415;
  LXeRefractiveIndex[21-8] = 1.404;
  LXeRefractiveIndex[21-9] = 1.3982;
  LXeRefractiveIndex[21-10] = 1.3951;
  LXeRefractiveIndex[21-11] = 1.392;
  LXeRefractiveIndex[21-12] = 1.3879;
  LXeRefractiveIndex[21-13] = 1.3865;
  LXeRefractiveIndex[21-14] = 1.3861;
  LXeRefractiveIndex[21-15] = 1.386;
  LXeRefractiveIndex[21-16] = 1.386;
  LXeRefractiveIndex[21-17] = 1.385;
  LXeRefractiveIndex[21-18] = 1.385;
  LXeRefractiveIndex[21-19] = 1.384;
  LXeRefractiveIndex[21-20] = 1.384;
  LXeRefractiveIndex[21-21] = 1.383;
  
  fLXeAbsLength = 20.0;

  G4double* LXeAbsLength = new G4double[2];
  LXeAbsLength[0] = fLXeAbsLength*m;
  LXeAbsLength[1] = fLXeAbsLength*m;
  
  G4double* LXeRayLength = new G4double[2];
  LXeRayLength[0] = 40*cm;
  LXeRayLength[1] = 40*cm;

//  fLXeMat = new G4MaterialPropertiesTable(); 
  fLXeMat = new nEXONestMaterialTable();
  fLXeMat->AddConstProperty( "ELECTRICFIELD", 0*volt/cm ); //for missed nooks and crannies
  fLXeMat->AddConstProperty( "ELECTRICFIELDSURFACE", 0*volt/cm );
  fLXeMat->AddConstProperty( "ELECTRICFIELDGATE", 0*volt/cm );
  fLXeMat->AddConstProperty( "ELECTRICFIELDCATHODE", 0*volt/cm );
  fLXeMat->AddConstProperty( "ELECTRICFIELDBOTTOM", 0*volt/cm );
  fLXeMat->AddConstProperty( "TOTALNUM_INT_SITES", -1 );
  fLXeMat->AddProperty("RINDEX", fPhotonEnergies, LXeRefractiveIndex, NUM_PP);
  fLXeMat->AddProperty("ABSLENGTH", fPhotonULEnergies, LXeAbsLength, 2);
  fLXeMat->AddProperty("RAYLEIGH", fPhotonULEnergies, LXeRayLength, 2);
  lXe->SetMaterialPropertiesTable( fLXeMat ); 

  G4cout << "nEXOMaterials::AddMatProperties():  Drift field in LXe [V/cm]: " 
    << fLXeMat->GetConstProperty("ELECTRICFIELD")/(volt/cm)
    << G4endl; 

  // Define SiO2 optical properties
  G4Material* SiO2 =  fNistMan->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  const G4int nSiO2Entries = 20;

  G4double SiO2Energy[nSiO2Entries] =
             {0.602*eV, 0.689*eV, 1.03*eV,  1.926*eV,
              2.583*eV, 2.845*eV, 2.857*eV, 3.124*eV,
              3.457*eV, 3.643*eV, 3.812*eV, 4.086*eV,
              4.511*eV, 4.953*eV, 5.474*eV, 6.262*eV,
              7.000*eV, 8.300*eV, 10.00*eV, 12.60*eV };

  G4double SiO2RefractiveIndex[nSiO2Entries] =
            { 1.520,    1.5241,   1.5323,   1.5423,
              1.550118, 1.553790, 1.55396, 1.55813,
              1.563915, 1.56747, 1.570915, 1.57955,
              1.58752, 1.60032, 1.61818, 1.65087,
              1.698, 1.877, 2.581, 1.390 };

  G4double SiO2Absorption[nSiO2Entries] =
            { 10*cm, 10*cm, 10*cm, 10*cm,
              10*cm, 10*cm, 10*cm, 10*cm,
              10*cm, 10*cm, 10*cm, 10*cm,
              10*cm, 10*cm, 10*cm, 10*cm,
              10*cm, 10*cm, 10*cm, 10*cm };

  fSiO2Mat = new G4MaterialPropertiesTable();
  fSiO2Mat->AddProperty("RINDEX", SiO2Energy, SiO2RefractiveIndex, nSiO2Entries);
  fSiO2Mat->AddProperty("ABSLENGTH", SiO2Energy, SiO2Absorption, nSiO2Entries);
  SiO2->SetMaterialPropertiesTable( fSiO2Mat );

  // Define TPB optical properties
  G4Material* TPB =  fNistMan->FindOrBuildMaterial("TPB");
  const G4int nTPBEntries = 25;

  G4double TPBEnergy[nTPBEntries] =
             {0.602*eV, 0.689*eV, 1.03*eV,  1.926*eV, 2.138*eV, 2.25*eV, 2.38*eV, 2.48*eV,
              2.583*eV, 2.845*eV, 2.857*eV, 2.95*eV, 3.124*eV,
              3.457*eV, 3.643*eV, 3.812*eV, 4.086*eV,
              4.511*eV, 4.953*eV, 5.474*eV, 6.262*eV,
              7.000*eV, 8.300*eV, 10.00*eV, 12.60*eV };

  G4double TPBAbsorption[nSiO2Entries] =
            { 100000*m, 100000*m, 100000*m, 100000*m,
              100000*m, 100000*m, 100000*m, 100000*m,
              100000*m, 100000*m, 100000*m, 100000*m,
              100000*m, 100000*m, 100000*m, 0.001*nm,
              0.001*nm, 0.001*nm, 0.001*nm, 0.001*nm };

  G4double TPBEmission[nTPBEntries] =
            { 0, 0, 0, 0, 0.0005, 0.0015, 0.003, 0.005,
              0.007, 0.011, 0.011, 0.006, 0.002,
              0, 0, 0, 0,
              0, 0, 0, 0,
              0, 0, 0, 0 };

  fTPBMat = new G4MaterialPropertiesTable();
  fTPBMat->AddProperty("RINDEX", SiO2Energy, SiO2RefractiveIndex, nSiO2Entries);
  fTPBMat->AddProperty("WLSABSLENGTH", SiO2Energy, TPBAbsorption, nSiO2Entries);
  fTPBMat->AddProperty("WLSCOMPONENT", TPBEnergy, TPBEmission, nTPBEntries);
  fTPBMat->AddConstProperty("WLSTIMECONSTANT", 0.01*ns);
  TPB->SetMaterialPropertiesTable( fTPBMat );
}

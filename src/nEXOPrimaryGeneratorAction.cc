#include "G4Event.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"

#include "Randomize.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "nEXOPrimaryGeneratorAction.hh"
#include "nEXOPrimaryGeneratorMessenger.hh"
#include "nEXOTPCVesselConstructor.hh"
#include "nEXOAnalysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

nEXOPrimaryGeneratorAction::nEXOPrimaryGeneratorAction()
{
  fMessenger   = new nEXOPrimaryGeneratorMessenger(this);

  fQ_value = 2457.8*keV;
  fBb2nCutOffFraction = 0.;

  fFF_factor = 0;
  fK_spectral_max = fD_spectral_max = fNormalization = 0;

  fGenerator = " ";

  fnCaptureXe_Method = fXeComponent = fCuComponent = " ";
  fCuIsotope = 0;

  fParticleGun1 = new G4ParticleGun();
  //default kinematic
  //
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("opticalphoton");
  
  fParticleGun1->SetParticleDefinition(particle);
  fParticleGun1->SetParticleTime(0.0*ns);
  fParticleGun1->SetParticleMomentumDirection(G4ThreeVector(0,0,0));
  fParticleGun1->SetParticleEnergy(6.966292135*eV);

  fParticleGun2 = new G4GeneralParticleSource();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

nEXOPrimaryGeneratorAction::~nEXOPrimaryGeneratorAction()
{
  delete fMessenger;
  delete fParticleGun1;
  delete fParticleGun2;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if ( fGenerator == "gps" || fGenerator == "GeneralParticleSource" )
     fParticleGun2->GeneratePrimaryVertex(anEvent);
  else if ( fGenerator == "bb0n" ) Generate_bb0n(anEvent);
  else if ( fGenerator == "bb2n" ) Generate_bb2n(anEvent);
  //else if ( fGenerator == "bb2n_endpoint" ) Generate_bb2n_EndPoint(anEvent);
  else if ( fGenerator == "nCaptureXe136" )
              Generate_nCaptureXe136(anEvent);
  else if ( fGenerator == "nCaptureCu" )
              Generate_nCaptureCu(anEvent);
  else {
    G4double cosTheta = 2*G4UniformRand() - 1., phi = 2*3.14159265358979323846*G4UniformRand();
    G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
    G4double ux = cosTheta,
             uy = sinTheta*std::sin(phi),
             uz = sinTheta*std::cos(phi);

    fParticleGun1->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));

    phi = 2*3.14159265358979323846*G4UniformRand();
    G4double r = std::sqrt(G4UniformRand())*650*mm;
    G4double z = r*std::cos(phi),
             y = r*std::sin(phi),
             x = (G4UniformRand()-0.5)*2*650*mm;

    fParticleGun1->SetParticlePosition(G4ThreeVector(x,y,z));
    
    fParticleGun1->GeneratePrimaryVertex(anEvent);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOPrimaryGeneratorAction::Set_generator(G4String generator)
{
  if ( generator == "gps"                      ||
       generator == "GeneralParticleSource"    ||
       generator == "gun"                      ||
       generator == "bb0n"                     ||
       generator == "bb2n"                     ||
       //generator == "bb2n_endpoint"            ||
       generator == "nCaptureXe136"            ||
       generator == "nCaptureCu"                  )
  {
      G4cout << "Using " << generator << " Generator" << G4endl;
      fGenerator = generator;

      Set_norm();

  } else {
      G4cout << "Generator not recognized" << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOPrimaryGeneratorAction::Set_nCaptureXe_Method(G4String method)
{
  G4cout << "***************************************************" << G4endl;

  if (method != "InternalConversions" &&
      method != "RandomGammas" &&
      method != "ImbalancedCascade")
  {
    G4cout << "Error: must be InternalConversions, RandomGammas, or ImbalancedCascade" << G4endl;
  } else {
    G4cout << "setting the method to " << method << G4endl;
    fnCaptureXe_Method = method;
  }

  G4cout << "***************************************************" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOPrimaryGeneratorAction::Set_Xe_Component(G4String component)
{
  G4cout << "***************************************************" << G4endl;

  if (component != "ActiveLXe" && component != "InactiveLXe") {
    G4cout << "Error: must be ActiveLXe or InactiveLXe" << G4endl;
  } else {
    G4cout << "setting the component to " << component << G4endl;
    fXeComponent = component;
  }

  G4cout << "***************************************************" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOPrimaryGeneratorAction::Set_Cu_isotope(G4int isotope)
{
  G4cout << "***************************************************" << G4endl;

  if (isotope != 63 && isotope != 65) {
     G4cout << "Error: Must set Cu isotope to 63 or 65" << G4endl;
     G4cout << "Substituting default isotope for Cu" << G4endl;
     fCuIsotope = 63;
  } else {
     G4cout << "setting the Cu Isotope for n-Capture to " << isotope << G4endl;
     fCuIsotope = isotope;
  }

  G4cout << "***************************************************" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOPrimaryGeneratorAction::Set_Cu_Component(G4String component)
{
  G4cout << "***************************************************" << G4endl;

  if (component != "TPC" &&
      component != "APDPlatter" &&
      component != "FieldRing" &&
      component != "InnerCryo" &&
      component != "OuterCryo") {
     G4cout << "Error: must be the TPC, APDPlatter, Field Ring, InnerCryo, or OuterCryo" << G4endl;
  } else {
     G4cout << "Setting the component to " << component << G4endl;
     fCuComponent = component;
  }

  G4cout << "***************************************************" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOPrimaryGeneratorAction::Set_bb2n_CutOff(G4double frac)
{
  fBb2nCutOffFraction = frac;
  
  G4cout << "Setting the fraction of bb2n cut off to " << fBb2nCutOffFraction << " = " << fBb2nCutOffFraction*fQ_value << " keV\n";

  Set_norm();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOPrimaryGeneratorAction::SetRandDimensions()
{
  G4String tpcVolName = "/nEXO/TPCVessel";
  
  G4ThreeVector tpcPos = nEXOSimplePhysVolManager::GetInstance()->GetVolumeTranslation(tpcVolName);
  
  //G4VPhysicalVolume *tpcPhys = nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(tpcVolName);
  //if(!tpcPhys)
  //  return;

  //G4LogicalVolume *tpcLog = tpcPhys->GetLogicalVolume();
  //if(!tpcLog)
  //  return;

  //const G4Cons *tpcCon = dynamic_cast<const G4Cons*>(tpcLog->GetSolid());

  //G4double tpcHalfLength = tpcCon->GetZHalfLength();
  //G4double tpcRadius = tpcCon->GetOuterRadiusMinusZ();
  //if(tpcCon->GetInnerRadiusMinusZ() > tpcRadius)
  //  tpcRadius = tpcCon->GetInnerRadiusMinusZ();

  nEXOTPCVesselConstructor* tpcVessel = nEXOSimplePhysVolManager::GetInstance()->GetTPCVesselConstructor();
  if(!tpcVessel)
  {
    G4cout << "No TPC vessel constructor has been found to define bb2n/bb0n spatial distribution..." << G4endl;
    return;
  }

  G4double tpcHalfLength = tpcVessel->GetTPCHeightTestStand();
  G4double tpcRadius = tpcVessel->GetTPCRadiusTestStand();
  
  fRandPos = tpcPos;
  fRandHalfLength = tpcHalfLength;
  fRandRadius = tpcRadius;
}

void nEXOPrimaryGeneratorAction::GetUnifRandPosInLXe(G4ThreeVector& pos)
{
  // Return a random position in LXe volume following a uniform distribution through this volume
  // Assumes LXe volume is contained in TPC vessel which is a conical (G4Cons) along z  

  G4Navigator* theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  
  while ( true )
  {
    G4double z0 = fRandPos.z() + ((2.0*G4UniformRand() - 1.0)*fRandHalfLength);
    G4double phi0 = 2*pi*G4UniformRand();
    G4double r0 = fRandRadius*sqrt(G4UniformRand());
    G4double x0 = r0*cos(phi0);
    G4double y0 = r0*sin(phi0);

    pos.set(x0,y0,z0);
    
    G4VPhysicalVolume* volume = theNavigator->LocateGlobalPointAndSetup(pos);

    if (volume->GetName() == "/nEXO/TPCInternals/LXe" || volume->GetName()== "/nEXO/TPCInternals/LXe/ActiveRegion")
      return;
  }
  
  return;
}

void nEXOPrimaryGeneratorAction::Generate_bb0n(G4Event* anEvent)
{
  //
  // K=T1+T2 (sum of kinetic energies T1 & T2).
  // D=T1-T2 (their difference; -K < D < +K; D=0 => T1=T2=T0/2)
  //
  // For bb0n events, K is the Q value divided by electron mass.
  // The sum electron spectrum of the bb0n mode is very simple; it is just a
  // delta-function peak at the end point energy E0 (or T0).
  //
  // Algorithm to generate the energy of the two electrons for the bb0n decay:
  // 1. Randomly generate d that satisfies the phase space constraint
  //
  //    P1*E1*FermiFunction(Z,E1)*P2*E2*FermiFunction(Z,E2)
  //
  // 2. With this set of K and D, calculate T1 and T2
  //    T1 = (K+D)/2
  //    t2 = (K-D)/2
  //
  // To generate an arbitrary probabilistic distribution function f(x),
  // use the Acceptance-Rejection Monte Carlo Method.
  // (cf. Frank Porter's lecture on the Acceptance-Rejection Method.)
  //
  // http://www.hep.caltech.edu/~fcp/statistics/sluo00/sluolec5.pdf
  //
  // Max-------------------
  //    |                 |
  //    |    *** f(x)     |
  //    |   *   **        |
  //    |  *      **      |
  //    | *         **    |
  //    |*            **  |
  //  0 *---------------***
  //    a                 b
  //
  // Generate randomly a point within the box bounded by (a->b, 0->Max).
  // If the point falls under f(x), accept it, otherwise, reject.
  // If it is rejected, repeat the process until a point falls under f(x).
  //

  G4int max_iterations = 100; //The max no. of iterations to satisfy
                              //the Acceptance-Rejection condition

  fParticleGun1->SetParticleDefinition(G4Electron::Electron());

  // Choose electron energies

  G4double t0 = fQ_value/electron_mass_c2;

  G4double k = t0;
  G4double t1 = 0.;
  G4double t2 = 0.;

  // Find the peak of the D spectrum first

  fD_spectral_max = D_bb0n_spectral_max(k);

  // Acceptance-Rejection iteration loop

  G4int n = 0;
  while ( n < max_iterations ) {
    G4double d = k*(2*G4UniformRand()-1.0);   // -k < d < +k
    G4double d_spectral = fD_spectral_max * G4UniformRand();

    // Test whether or not a randomly generated point is
    // below the D spectrum
    if ( d_spectral <= D_bb0n_spectrum(k,d) ) {
       t1 = 0.5*(k+d);
       t2 = 0.5*(k-d);
       break; // Condition is met; stop the loop.
    }
    n++;
  }

  t1 = t1*electron_mass_c2; // scale by the electron mass
  t2 = t2*electron_mass_c2;

  //*****Want to generate a primary vertex within the LXe boundary,
  //*****test whether or not it is actually in the xenon.
  //***** Remember that the axis of the TPC lies along the Z coordinate!
  SetRandDimensions();
  G4ThreeVector decayVertex;
  GetUnifRandPosInLXe(decayVertex);
  //  G4cout << "Final point = " << decayVertex << G4endl;
  fParticleGun1->SetParticlePosition(decayVertex);

  // GENERATE FIRST ELECTRON ------------------------------------

  G4double theta = acos(2*(G4UniformRand()-0.5));
  G4double phi   = 2*pi*G4UniformRand();
  G4double ux1   = sin(theta)*cos(phi);
  G4double uy1   = sin(theta)*sin(phi);
  G4double uz1   = cos(theta);

  fParticleGun1->SetParticleEnergy(t1);
  fParticleGun1->SetParticleMomentumDirection(G4ThreeVector(ux1,uy1,uz1));
  fParticleGun1->GeneratePrimaryVertex(anEvent);

  // GENERATE SECOND ELECTRON ----------------------------------

  // For the bb0n decay, the angular distribution of the electrons 
  // is of the form
  // f(cos)= 1 - beta1*beta2*cos(theta12) with beta = P/E,
  // for the 0+ --> 0+ transition (cf. Boehm & Vogel p.146).
  //
  // This distribution favors a negative cos(theta12), i.e., when the 
  // electrons are pointing at opposite hemispheres.
  //
  // The range of f(cos) is:
  // 1 - beta1*beta2 < f(cos) < 1 + beta1*beta2
  //
  // When the direction of the first electron is chosen, the direction
  // of the second electron has to be generated according to the above
  // angular distribution.
  //
 
  G4double e1 = t1 + electron_mass_c2;
  G4double e2 = t2 + electron_mass_c2;
  G4double p1 = sqrt(e1*e1-electron_mass_c2*electron_mass_c2);
  G4double p2 = sqrt(e2*e2-electron_mass_c2*electron_mass_c2);
  G4double beta1 = p1/e1;
  G4double beta2 = p2/e2;

  G4double theta12 = 0., phi2 = 0.;

  n = 0;
  while ( n < max_iterations) {
    theta12 = acos(2*(G4UniformRand()-0.5));
    G4double angular_distribution = 1.0 + beta1*beta2*(2*G4UniformRand()-1.0);
    // 1 - beta1*beta2 < f(cos) < 1 + beta1*beta2
    if ( angular_distribution <= ( 1.0 - beta1*beta2*cos(theta12) ) ) {
       // Acceptance-Rejection condition for the second electron to satisfy 
       // the angular distribution 

       phi2 = 2*pi*G4UniformRand();
       break;  // Condition is met; stop the loop.
    }
    n++;
  }

  G4double ux2 = sin(theta12)*cos(phi2);
  G4double uy2 = sin(theta12)*sin(phi2);
  G4double uz2 = cos(theta12);

  // Rotate second electron to detector coordinates

  G4double ct = cos(theta);
  G4double st = sin(theta);
  G4double cp = cos(phi);
  G4double sp = sin(phi);

  G4double uxp = ct*ux2 + st*uz2;
  G4double uyp = uy2;
  G4double uzp = -1.0*st*ux2 + ct*uz2;
  ux2 = uxp;
  uy2 = uyp;
  uz2 = uzp;

  uxp = cp*ux2 - sp*uy2;
  uyp = sp*ux2 + cp*uy2;
  uzp = uz2;
  ux2 = uxp;
  uy2 = uyp;
  uz2 = uzp;

  fParticleGun1->SetParticleEnergy(t2);
  fParticleGun1->SetParticleMomentumDirection(G4ThreeVector(ux2,uy2,uz2));
  fParticleGun1->GeneratePrimaryVertex(anEvent);

  nEXOAnalysis::GetInstance()->SetBetaE1( t1 );
  nEXOAnalysis::GetInstance()->SetBetaE2( t2 );
  G4ThreeVector vecBeta1(ux1, uy1, uz1);
  G4ThreeVector vecBeta2(ux2, uy2, uz2);
  nEXOAnalysis::GetInstance()->SetBetaAngle( vecBeta1.theta(vecBeta2) );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double nEXOPrimaryGeneratorAction::D_bb0n_spectral_max(G4double k)
{
  // This function calculates the maximum value of the bb0n D spectrum.
  // fFF_factor = 1 for xe136

  fFF_factor = 1;

  //
  // Energies (E1, E2),  momenta (P1, P2), and kinetic energies (T1, T2)
  // are all divided by the electron mass.
  //
  // K = T0 = fQ_value/electron_mass_c2
  // D = T1-T2 (their difference; Range:  -K < D < +K )
  //
  // T1 = (K+D)/2
  // T2 = (k-D)/2
  //

  G4int z = 54;

  G4int nbins = 100;    // nbins is the # of bins.

  G4double a = -k;
  G4double b =  k;
  G4double dD = (b-a)/nbins;

  G4double phase_space[101] = {0.};

  for (G4int n = 0; n < nbins+1; n++) {

      G4double d;

      if (n != nbins) {
        d = a + n*dD;
      } else {
        d = b;
      }

      G4double e1 = 0.5*(k+d)+1.0;
      G4double e2 = 0.5*(k-d)+1.0;

      G4double p1 = sqrt(e1*e1-1.0);
      G4double p2 = sqrt(e2*e2-1.0);

      G4double t1 = e1 - 1.0;
      G4double t2 = e2 - 1.0;

      phase_space[n] = p1*e1*Fermi_function(z,t1*electron_mass_c2) *
                       p2*e2*Fermi_function(z,t2*electron_mass_c2);
  }

  //search for the peak value of phase space[i]

  G4double d_spectral_max = 0.;

  for (G4int i=0; i<nbins+1; i++) {
      if (phase_space[i] > d_spectral_max) d_spectral_max = phase_space[i];
  }

  return d_spectral_max; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double nEXOPrimaryGeneratorAction::D_bb0n_spectrum(G4double k, G4double d)
{
    G4int z = 54;

    G4double e1 = 0.5*(k+d)+1.0;
    G4double e2 = 0.5*(k-d)+1.0;

    G4double p1 = sqrt(e1*e1-1.0);
    G4double p2 = sqrt(e2*e2-1.0);

    G4double t1 = e1-1.0;
    G4double t2 = e2-1.0;

    return p1*e1*Fermi_function(z,t1*electron_mass_c2) *
           p2*e2*Fermi_function(z,t2*electron_mass_c2);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double nEXOPrimaryGeneratorAction::Fermi_function(G4int z, G4double ke)
{
  // From Schenter+Vogel in Nucl.Sci.Eng,83,393(1983)
  // W is full energy in electron mass units

  G4double alpha = 7.2974e-3; // fine structure constant

  G4double total_energy = ke + electron_mass_c2;
  G4double w = total_energy/electron_mass_c2;

  G4double z0 = z + fFF_factor*2;

  if ( w <= 1 ) w = 1 + 1e-4;

  G4double a = -0.811+4.46e-2*z0+1.08e-4*z0*z0;
  G4double b = 0.673-1.82e-2*z0+6.38e-5*z0*z0;
    
  if ( w > 2.2 ) {
     a = -8.46e-2+2.48e-2*z0+2.37e-4*z0*z0;
     b = 1.15e-2+3.58e-4*z0-6.17e-5*z0*z0;
  }

  G4double x = sqrt(w-1);
  G4double p = sqrt(w*w-1);

  G4double result = exp(a+b*x)*w/p;

  if (p<=0) result = 1; //just to be consistent with the old Fermi Function code

  if (fFF_factor == -1) { // for double positron decays
     G4double v = p/w;
     G4double y = 2*pi*z0*alpha/v;
     G4double yy = 1./exp(y);
     result = result*yy;
  }

  return result;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//void nEXOPrimaryGeneratorAction::Generate_bb2n_EndPoint(G4Event* anEvent)
//{
//  Generate_bb2n(anEvent,0.93); // 0.8*2458 = 1966keV min bb2n energy ~ 2000 keV
//}

void nEXOPrimaryGeneratorAction::Generate_bb2n(G4Event* anEvent)
{
  //
  // K=T1+T2 (sum of kinetic energies T1 and T2; 0 < K < T0)
  // D=T1-T2 (their difference; -K < D < +K; D=0 => T1=T2=T0/2)
  //
  // Algorithm to generate the energy of the two electrons for the bb2n decay:
  //
  //    P1*E1*FermiFunction(Z,E1)*P2*E2*FermiFunction(Z,E2)*pow(T0-K,5)
  //
  // 3. With this set of K and D, calculate T1 and T2
  //    T1 = (K+D)/2
  //    T2 = (K-D)/2
  //
  // This set of K & D (or T1, T2) guarantees to satisfy both
  // the sum electron spectrum dn/dk and the single electron spectra dn/dt0.
  // (cf. Physics of Massive Neutrinosy Felix Boehm & Petr Vogel,pp.145-151.)
  //
  // To generate an arbitrary probabilistic distribution function f(x),
  // use the Acceptance-Rejection Monte Carlo Method.
  // (cf. Frank Porter's lecture on the Acceptance-Rejection Method.)
  //
  // http://www.hep.caltech.edu/~fcp/statistics/sluo00/sluolec5.pdf
  //
  // Max-------------------
  //    |                 |
  //    |    *** f(x)     |
  //    |   *   **        |
  //    |  *      **      |
  //    | *         **    |
  //    |*            **  |
  //  0 *---------------***
  //    a                 b
  //
  // Generate randomly a point within the box bounded by (a->b, 0->Max).
  // If the point falls under f(x), accept it, otherwise, reject.
  // If it is rejected, repeat the process until a point falls under f(x).
  //

  G4int max_iterations = 100; //The max no. of iterations to satisfy
                              //the Acceptance-Rejection condition

  fParticleGun1->SetParticleDefinition(G4Electron::Electron());

  // Choose electron energies

  G4double t0 = fQ_value/electron_mass_c2;

  G4double t1 = 0.;
  G4double t2 = 0.;

  //G4cout << "K_spectral_max = " << fK_spectral_max << G4endl;

  // G4double fK_spectral_max=0.0233;
  // k_spectral_max is now calculated once at initialization.
  // Value of k_spectral_max assumes that the sum electron spectrum dn/dk
  // is normalized to 1, assuming the # of bins=100 (if the # of bins
  // is changed, change also this spectral max value.) With this normalization
  // the max value is slightly less than 0.0233.

  G4double frac = fBb2nCutOffFraction/(1-fBb2nCutOffFraction);//endPointFrac/(1-endPointFrac);
  G4int i = 0;
  while ( i < max_iterations ) {

    G4double k = t0*(G4UniformRand()+frac)/(frac+1);

    G4double k_spectrum = fK_spectral_max*G4UniformRand();

    if ( k_spectrum <= BB2n_sum_spectrum(k)) {
       // Acceptance-Rejection condition for the sum electron spectrum dn/dk

       G4int n = 0;
       while ( n < max_iterations ) {

         G4double d = k*(2*G4UniformRand()-1.0);   // -k < d < +k

         G4double d_spectral = fD_spectral_max * G4UniformRand();

         //Acceptance-Rejection condition for the single electron spectrum dn/t0

         if ( d_spectral <= D_spectrum(k,d)) {
            t1 = 0.5*(k+d);
            t2 = 0.5*(k-d);
            break; // Condition is met; stop the loop.

         }
         n++;
       }
       break;
    }
    i++;
  }

  t1 = t1*electron_mass_c2; // scale by the electron mass
  t2 = t2*electron_mass_c2;

  // Beta source vertex----------------------------------------

  //*****Want to generate a primary vertex within the LXe boundary,
  //*****test whether or not it is actually in the xenon.
  //***** Remember that the axis of the TPC lies along the Z coordinate!
  SetRandDimensions();
  G4ThreeVector decayVertex;
  GetUnifRandPosInLXe(decayVertex);
  //  G4cout << "Final point = " << decayVertex << G4endl;
  fParticleGun1->SetParticlePosition(decayVertex);
  
  // GENERATE FIRST ELECTRON ------------------------------------

  G4double theta = acos(2*(G4UniformRand()-0.5));
  G4double phi   = 2*pi*G4UniformRand();
  G4double ux1   = sin(theta)*cos(phi);
  G4double uy1   = sin(theta)*sin(phi);
  G4double uz1   = cos(theta);

  fParticleGun1->SetParticleEnergy(t1);
  fParticleGun1->SetParticleMomentumDirection(G4ThreeVector(ux1,uy1,uz1));
  fParticleGun1->GeneratePrimaryVertex(anEvent);

  // GENERATE SECOND ELECTRON ----------------------------------

  // For the bb2n decay, the angular distribution of the electrons
  // is of the form
  // f(cos)= 1 - beta1*beta2*cos(theta12) with beta = P/E,
  // for the 0+ --> 0+ transition (cf. Boehm & Vogel p.146).
  //
  // This distribution favors a negative cos(theta12), i.e., when the
  // electrons are pointing at opposite hemispheres.
  //
  // The range of f(cos) is:
  // 1 - beta1*beta2 < f(cos) < 1 + beta1*beta2
  //
  // When the direction of the first electron is chosen, the direction
  // of the second electron has to be generated according to the above
  // angular distribution.
  //

  G4double e1 = t1 + electron_mass_c2;
  G4double e2 = t2 + electron_mass_c2;
  G4double p1 = sqrt(e1*e1-electron_mass_c2*electron_mass_c2);
  G4double p2 = sqrt(e2*e2-electron_mass_c2*electron_mass_c2);
  G4double beta1 = p1/e1;
  G4double beta2 = p2/e2;
  G4double theta12 = 0., phi2 = 0.;

  G4int n = 0;
  while ( n < max_iterations) {
    theta12 = acos(2*(G4UniformRand()-0.5));
    G4double angular_distribution = 1.0 + beta1*beta2*(2*G4UniformRand()-1.0);
    // 1 - beta1*beta2 < f(cos) < 1 + beta1*beta2
    if ( angular_distribution <= ( 1.0 - beta1*beta2*cos(theta12) ) ) {
       // Acceptance-Rejection condition for the second electron to satisfy
       // the angular distribution

      phi2 = 2*pi*G4UniformRand();
      break;   // Condition is met; stop the loop.
    }
    n++;
  }

  G4double ux2 = sin(theta12)*cos(phi2);
  G4double uy2 = sin(theta12)*sin(phi2);
  G4double uz2 = cos(theta12);

  // Rotate second electron to detector coordinates

  G4double ct = cos(theta);
  G4double st = sin(theta);
  G4double cp = cos(phi);
  G4double sp = sin(phi);

  G4double uxp = ct*ux2 + st*uz2;
  G4double uyp = uy2;
  G4double uzp = -1.0*st*ux2 + ct*uz2;
  ux2 = uxp;
  uy2 = uyp;
  uz2 = uzp;

  uxp = cp*ux2 - sp*uy2;
  uyp = sp*ux2 + cp*uy2;
  uzp = uz2;
  ux2 = uxp;
  uy2 = uyp;
  uz2 = uzp;

  fParticleGun1->SetParticleEnergy(t2);
  fParticleGun1->SetParticleMomentumDirection(G4ThreeVector(ux2,uy2,uz2));
  fParticleGun1->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double nEXOPrimaryGeneratorAction::BB2n_sum_spectrum(G4double k)
{
// This function calculates the sum electron spectrum dn/dk for the bb2n decay.
    
  //
  // Energies (E1, E2),  momenta (P1, P2), and kinetic energies (T1, T2)
  // are all divided by the electron mass.
  //
  // K = T1+T2 (Sum of kinetic energies; Range: 0 < K < T0 )
  // D = T1-T2 (their difference; Range:  -K < D < +K )
  //
  // T1 = (K+D)/2
  // t2 = (K-D)/2
  //

  G4int z = 54;

  G4int nbins = 100;    // nbins  is the # of bins.

  G4double t0 = fQ_value/electron_mass_c2;

  G4double a = -k;
  G4double b =  k;
  G4double dD = (b-a)/nbins;

  G4double phase_space[101]={0.};

  for (G4int n = 0; n < nbins+1; n++) {

      G4double d;

      if (n != nbins) {
        d = a + n*dD;
      } else {
        d = b;
      }

      G4double e1 = 0.5*(k+d)+1.0;
      G4double e2 = 0.5*(k-d)+1.0;

      G4double p1 = sqrt(e1*e1-1.0);
      G4double p2 = sqrt(e2*e2-1.0);

      G4double t1 = e1 - 1.0;
      G4double t2 = e2 - 1.0;

      phase_space[n] = p1*e1*Fermi_function(z,t1*electron_mass_c2) *
                       p2*e2*Fermi_function(z,t2*electron_mass_c2)*pow(t0-k,5);
  }

    //
    // d_spectral_max is the peak value for the phase_space[n] spectrum.
    // This value is needed in generating the energy of the second electron.
    //

    G4double d_spectral_max = 0.;

    //search for the peak value of phase space[i]
    for (G4int i=0; i<nbins+1; i++) {
        if (phase_space[i] > d_spectral_max) d_spectral_max = phase_space[i];
    }

    // G4double normalization = 134958.7414*nbins;
    // Normalization is now calculated once at initialization.
    // This factor normalizes the integral dn/dk (sum electron spectrum) to 1.
    // nbins = # of bins. If nbins is changed, normalization should also be
    // adjusted.

    G4double normalized_spectrum = SimpsonsRule(a, b, nbins, phase_space) / 
                                   fNormalization;

    fD_spectral_max = d_spectral_max;

    return normalized_spectrum;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double nEXOPrimaryGeneratorAction::D_spectrum(G4double k, G4double d)

// This is for the bb2n spectrum
{
    G4int z = 54;

    G4double t0 = fQ_value/electron_mass_c2;

    G4double e1 = 0.5*(k+d)+1.0;
    G4double e2 = 0.5*(k-d)+1.0;

    G4double p1 = sqrt(e1*e1-1.0);
    G4double p2 = sqrt(e2*e2-1.0);

    G4double t1 = e1 - 1.0;
    G4double t2 = e2 - 1.0;

    return p1*e1*Fermi_function(z,t1*electron_mass_c2) *
           p2*e2*Fermi_function(z,t2*electron_mass_c2)*pow(t0-k,5);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double nEXOPrimaryGeneratorAction::SimpsonsRule(G4double x0, G4double xn,
                                                  G4int n, G4double f[])
{
  //
  // Simpson's Rule:
  // Partition [a, b] into intervals all of the same width.
  // We must use an even number of intervals, so n will be even.
  // xk = a + kx = a + k (b-a)/n
  //
  // integral(x0, xn)
  // = (xn-x0)/3n *[f(x0) + 4f(x1) + 2f(x2) + 4f(x3) +
  //                         ... + 2f(xn-2) + 4f(xn-1) + f(xn)]
  //
  // Sum all the odd terms and then multiply by 4.
  // Sum all the even terms and then multiply by 2.
  //

    if ( n%2 != 0 ) G4cout << "SimpsonsRule: N is not even";

    G4double sum_odd = 0.;
    for (G4int i = 0; i < n/2; i++) sum_odd += f[2*i+1];
    sum_odd = 4.0*sum_odd;

    G4double sum_even = 0.;
    for (G4int j = 1; j < n/2; j++) sum_even += f[2*j];
    sum_even = 2.0*sum_even;

    return (xn-x0)*(f[0]+sum_odd+sum_even+f[n])/(3.0*n);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOPrimaryGeneratorAction::Set_norm()
{
  // Calculate the normalization factor (normalization) as well
  // as the maximum value of the k distribution (k_spectral_max) which
  // varies for different isotopes.
  // This function should only run once in the beginning.

  // This function should only run once in the beginning.

   G4double t0 = fQ_value/electron_mass_c2;
   G4double ts = fBb2nCutOffFraction*t0;
   
   fNormalization = 1; //to make BB2n_sum_spectrum run the first time

   G4int nbins = 100;//00;

   G4double bb2n_sum_max = 0;
   G4double* bb2n_sum_spec = new G4double[nbins+1];// = {0.};

   G4cout << "Evaluation of bb2n spectrum...\n";
   for (G4int n = 0; n <= nbins; n++) {
     G4double sum_ene_me = ts + (t0 - ts)*n/nbins;
       bb2n_sum_spec[n] = BB2n_sum_spectrum(sum_ene_me);
       if (bb2n_sum_spec[n] > bb2n_sum_max)
         bb2n_sum_max = bb2n_sum_spec[n];
       //std::cout << electron_mass_c2*sum_ene_me << " " << bb2n_sum_spec[n] << std::endl;
   }
   G4cout << "Max: " << bb2n_sum_max << G4endl;

   fNormalization = SimpsonsRule(0, t0, nbins, bb2n_sum_spec);

   G4cout << "normalization " << fNormalization << G4endl;

   fK_spectral_max = bb2n_sum_max*1.01/fNormalization;

   G4cout << "K_spectral_max " << fK_spectral_max << G4endl;

   delete bb2n_sum_spec;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOPrimaryGeneratorAction::Generate_nCaptureXe136(G4Event* anEvent)
{
  struct nCapture_Xe136Info
  {
    G4int jump;
    G4double level, gamma;
    G4double ratio;
  };

  fParticleGun1->SetParticleDefinition(G4Gamma::Gamma());

  //******** Set Vertex of Capture + Excited State Decay *************//

  G4double inactive_Xenon_R = 65.*cm;
  G4double inactive_Xenon_Z = 65.*cm;

  while ( true ) {
    G4double phi0 = 2*pi*G4UniformRand();
    G4double r0 = inactive_Xenon_R*sqrt(G4UniformRand());
    G4double x0 = inactive_Xenon_Z*(2*(G4UniformRand() - 0.5));
    G4double y0 = r0*cos(phi0);
    G4double z0 = r0*sin(phi0);
    
    G4ThreeVector point = G4ThreeVector(x0, y0, z0);
    G4Navigator* theNavigator =
                 G4TransportationManager::GetTransportationManager()->
                                                     GetNavigatorForTracking();
    G4VPhysicalVolume* volume = theNavigator->LocateGlobalPointAndSetup(point);

    if (volume->GetName()== fXeComponent) {
      //G4cout << XeComponent << " chosen" << G4endl;
      fParticleGun1->SetParticlePosition(G4ThreeVector(x0, y0, z0));
      break;
    }
  }

  if (fnCaptureXe_Method == "InternalConversions" ||
      fnCaptureXe_Method == "RandomGammas") {

     nCapture_Xe136Info decay[] = {
       {  3, 4025,    0.00, 100/135. }, // skip to seen lines
       { 32, 4025,-3424.47,  23/135. }, // special to 601
       { 30, 4025,-3039.33,   1. }, // special to 986
       //
       { 30, 4025, 3424.47, 46/87. }, // 601
       { 28, 4025, 3039.33, 15/87. }, // 986
       { 17, 4025, 2184.04, 12/87. }, // 1842
       { 11, 4025, 2088.93,  5/87. }, // 1936
       {  9, 4025, 1829.38,  1/87. }, // 2196
       {  4, 4025, 1535.15,  7/87. }, // 2490
       {  1, 4025, 1416.68,  1. }, // 2609
       { 23, 2609, 2007.80, 10/110. }, // 601
       { 13, 2609,  893.30, 1. }, // 1716
       { 22, 2490, 2490.38, 84/267. }, // 0
       { 20, 2490, 1889.21, 25/267. }, // 601
       { 18, 2490, 1504.30, 100/267. }, // 986
       { 14, 2490, 1187.55, 1. }, // 1303
       { 13, 2196,  893.42, 1. }, // 1303
       { 17, 1936, 1936.05, 63/340. }, // 0
       { 15, 1936, 1335.00, 71/340. }, // 601
       { 13, 1936,  949.85, 100/340. }, // 986
       {  9, 1936,  633.32, 46/340. }, // 1303
       {  6, 1936,  267.92, 1. }, // 1668
       { 12, 1842, 1841.49, 30/130. }, // 0
       {  6, 1842,  538.76, 1. }, // 1303
       { 10, 1716, 1715.55, 100/170. }, // 0
       {  8, 1716, 1114.50, 46/170. }, // 601
       {  3, 1716,  412.82, 1. }, // 1303
       {  6, 1668, 1067.08, 100/167. }, // 601
       {  4, 1668,  681.93, 1. }, // 986
       {  5, 1303, 1302.73, 100/112. }, // 0
       {  3, 1303,  701.68, 10/112. }, // 601
       {  1, 1303,  316.53, 1. }, // 986
       {  1,  986,  385.15, 1. }, // 601
       {  1,  601,  601.05, 1. }, // 0
       {  0,    0,    0.00, 1. },
     };

     G4int decay_num = sizeof(decay)/sizeof(decay[0]);
     //G4int decay_num = GetNumArrayElements();

     //G4cout << "At beginning of loop" << G4endl;
     G4double p = G4UniformRand();
     //G4cout << "p = " << p << G4endl;
     for (G4int j = 0; 0<= j && j < decay_num;) {
        if (p < decay[j].ratio) {
           G4double energy = decay[j].gamma;
           //G4cout << "Energy = " << energy << G4endl;
           if (energy > 0.) {
              //G4cout << "In Energy > 0 " << G4endl;
              fParticleGun1->SetParticleEnergy(energy*keV);
              G4double phi1 = 2*pi*G4UniformRand();
              G4double theta1 = pi*G4UniformRand();
              G4double x1 = sin(theta1)*cos(phi1);
              G4double y1 = sin(theta1)*sin(phi1);
              G4double z1 = cos(theta1);
              fParticleGun1->
                        SetParticleMomentumDirection(G4ThreeVector(x1,y1,z1));
              fParticleGun1->GeneratePrimaryVertex(anEvent);          
           }
           if ( energy < 0.) {
              if (fnCaptureXe_Method == "InternalConversions") {
                 //G4cout << "electron being generated" << G4endl;
                 fParticleGun1->SetParticleDefinition(G4Electron::Electron());
                 energy = -decay[j].gamma;
                 fParticleGun1->SetParticleEnergy(energy*keV);
                 G4double phi1 = 2*pi*G4UniformRand();
                 G4double theta1 = pi*G4UniformRand();
                 G4double x1 = sin(theta1)*cos(phi1);
                 G4double y1 = sin(theta1)*sin(phi1);
                 G4double z1 = cos(theta1);
                 fParticleGun1->
                        SetParticleMomentumDirection(G4ThreeVector(x1,y1,z1));
                 fParticleGun1->GeneratePrimaryVertex(anEvent);          
                 fParticleGun1->SetParticleDefinition(G4Gamma::Gamma());
              }
              else if (fnCaptureXe_Method == "RandomGammas") {
                 G4double level = (G4UniformRand() + G4UniformRand() +1.)*1000.;
                 G4double target = decay[j].level+decay[j].gamma;
                 if (target > 610.) level += 500.;
                 energy = decay[j].level - level;
                 //G4cout << "Energy in 1st 'else' statement" << G4endl;
                 //G4cout << "Energy = " << energy << G4endl;
                 G4double cost = 2*G4UniformRand() - 1.;
                 G4double sint = sqrt(1. - cost*cost);
                 G4double phi = 2*pi*G4UniformRand();
                 G4double vdx = sint*cos(phi);
                 G4double vdy = sint*sin(phi);
                 G4double vdz = cost;
                 fParticleGun1->SetParticleEnergy(energy*keV);
                 fParticleGun1->
                     SetParticleMomentumDirection(G4ThreeVector(vdx,vdy,vdz));
                 fParticleGun1->GeneratePrimaryVertex(anEvent);          
                 energy = level - target;
                 //G4cout << "Energy in 2nd 'else' statement" << G4endl;
                 //G4cout << "Energy = " << energy << G4endl;
                 cost = 2*G4UniformRand() - 1.;
                 sint = sqrt(1. - cost*cost);
                 phi = 2*pi*G4UniformRand();
                 vdx = sint*cos(phi);
                 vdy = sint*sin(phi);
                 vdz = cost;
                 fParticleGun1->SetParticleEnergy(energy*keV);
                 fParticleGun1->
                     SetParticleMomentumDirection(G4ThreeVector(vdx,vdy,vdz));
                 fParticleGun1->GeneratePrimaryVertex(anEvent);
              }
           }
           if (decay[j].jump == 0) break;
           else j += decay[j].jump;
           //G4cout << "Creating new Random Number " << G4endl;
           p = G4UniformRand();
        } else {
           p -= decay[j].ratio;
           j++;
        }
     }
  }

  //**** Start choosing energies of gammas from 4025keV cascade ****//
  if (fnCaptureXe_Method == "ImbalancedCascade") {
     //G4cout << "imbalance" << G4endl;
     nCapture_Xe136Info decay[] = {
       {4,    4025,      0.0,  85./111.},
       {12,   4025,      0.0,   3./111.},
       {20,   4025,      0.0,   8./111.},
       {20,   4025,      0.0,   1.     },
       {19,   4025,   3.4245,   46./85.},
       {17,   4025,   3.0394,   15./85.},
       {10,   4025,   2.1839,   12./85.},
       {6,    4025,   2.0889,   5.3/85.},
       {1,    4025,   1.5351,   1.     },
       {15,   2490,   2.4905,   1.6/5.1},
       {13,   2490,   1.8892,   0.5/5.1},
       {11,   2490,   1.5043,   1.9/5.1},
       {8,    2490,   1.1876,   1.     },
       {10,   1937,   1.3355,   2.5/8.1},
       {8,    1937,   0.9502,    3.5/8.1},
       {3,    1937,   0.2684,    1.     },
       {8,    1841,   1.8415,   3.0/12.9},
       {3,    1841,   0.5389,    1.     },
       {5,    1668,   1.0671,   1.9/2.9},
       {3,    1668,   0.6820,    1.     },
       {4,    1303,   1.3027,   12./14.3},
       {2,    1303,   0.7012,    1.     },
       {1,    986,    0.3852,    1.     },
       {1,    601,    0.6010,    1.     },
       {0,    0,      0.,        1.     },
     };

     G4int decay_num = sizeof(decay)/sizeof(decay[0]);
     //G4int decay_num = GetNumArrayElements();

     G4double p = G4UniformRand();
     for (G4int j = 0; 0<= j && j < decay_num;){
         if (p < decay[j].ratio){
            G4double energy = decay[j].gamma;
            if (energy > 0.){
               fParticleGun1->SetParticleEnergy(energy*MeV);
               G4double phi1 = 2*pi*G4UniformRand();
               G4double theta1 = pi*G4UniformRand();
               G4double x1 = sin(theta1)*cos(phi1);
               G4double y1 = sin(theta1)*sin(phi1);
               G4double z1 = cos(theta1);
               fParticleGun1->
                       SetParticleMomentumDirection(G4ThreeVector(x1,y1,z1));
               // If energy > 0, a gamma is generated, but that doesn't mean
               // the cascade is finished!
               fParticleGun1->GeneratePrimaryVertex(anEvent);          
            }
            // Only break out of loop when 'jump' == 0
                  if (decay[j].jump == 0) break;
                   else j += decay[j].jump;
            p = G4UniformRand();
         } else {
                  p -= decay[j].ratio;
            j++;
         }
     }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOPrimaryGeneratorAction::Generate_nCaptureCu(G4Event* anEvent)
{
  //  G4cout << "In Copper Generator " << G4endl;

  struct nCapture_CuInfo{
    G4int jump;
    G4double level, gamma;
    G4double ratio;
  };

  fParticleGun1->SetParticleDefinition(G4Gamma::Gamma());

  //******** Generate vertex in the Copper Vessel ***************//

  G4double maxZ = 75*cm;
  while(true) {
    G4double x0 = maxZ*(2*(G4UniformRand() - 0.5));
    G4double y0 = maxZ*(2*(G4UniformRand() - 0.5));
    G4double z0 = maxZ*(2*(G4UniformRand() - 0.5));

    G4ThreeVector point = G4ThreeVector(x0, y0, z0);
    G4Navigator* theNavigator = 
                 G4TransportationManager::GetTransportationManager()->
                                                     GetNavigatorForTracking();
    G4VPhysicalVolume* volume = theNavigator->LocateGlobalPointAndSetup(point);
    if (volume->GetName() == fCuComponent) {
       //G4cout << fCuComponent << " chosen" << G4endl;
       //G4cout << "x,y,z = " << x0 << " " << y0 << " " << z0 << G4endl;
       fParticleGun1->SetParticlePosition(point);
       break;
    }
  }

  if (fCuIsotope == 63) {
    //G4cout << "Decay cascade being defined" << G4endl;

    nCapture_CuInfo  decay[] = {
      {300,      7916.419,          7916.26,     0.333437427596},
      {298,      7916.419,          7756.91,    0.0159163485076},
      {296,      7916.419,            7638.,     0.163192940394},
      {293,      7916.419,          7572.32,    0.0175281306349},
      {290,      7916.419,           7555.1,  0.000846185616859},
      {284,      7916.419,          7307.31,    0.0902597991317},
      {279,      7916.419,          7253.05,    0.0418055989282},
      {273,      7916.419,          7177.07,    0.0257885140376},
      {271,      7916.419,           7170.1,   0.00305231240367},
      {268,      7916.419,          7037.83,   0.00390857165883},
      {261,      7916.419,          6988.96,    0.0353584704188},
      {256,      7916.419,          6674.85,    0.0200465402089},
      {253,      7916.419,           6628.9,  0.000997290191298},
      {250,      7916.419,          6618.15,    0.0113832112744},
      {248,      7916.419,          6595.63,   0.00635646576474},
      {243,      7916.419,            6553.,   0.00100736382959},
      {238,      7916.419,          6477.15,    0.0017326657869},
      {233,      7916.419,           6416.9,  0.000956995638115},
      {227,      7916.419,          6394.86,     0.014022504508},
      {225,      7916.419,           6365.6,   0.00106780565937},
      {220,      7916.419,          6321.54,   0.00363658342484},
      {218,      7916.419,          6308.61,   0.00168229759542},
      {216,      7916.419,            6233.,   0.00109802657426},
      {209,      7916.419,           6166.9,   0.00167222395713},
      {205,      7916.419,          6136.05,    0.0019845067443},
      {202,      7916.419,          6063.65,   0.00611469844564},
      {200,      7916.419,           6015.7,   0.00230686316977},
      {197,      7916.419,          6010.83,    0.0160170848905},
      {193,      7916.419,          5866.16,   0.00145060391462},
      {190,      7916.419,            5824.,   0.00302209148878},
      {184,      7916.419,          5771.48,   0.00512748189264},
      {178,      7916.419,          5636.18,   0.00413019170134},
      {176,      7916.419,          5615.01,    0.0049763773182},
      {174,      7916.419,           5600.5,  0.000977142914707},
      {171,      7916.419,          5555.78,   0.00276017689309},
      {167,      7916.419,           5528.2,    0.0012491311487},
      {164,      7916.419,          5450.75,   0.00159163485076},
      {155,      7916.419,          5418.49,    0.0186362308475},
      {154,      7916.419,          5408.88,   0.00401938168008},
      {152,      7916.419,            5385.,  0.000110810021255},
      {151,      7916.419,           5321.3,  0.000554050106277},
      {149,      7916.419,          5280.67,   0.00251840957399},
      {147,      7916.419,           5269.4,  0.000805891063675},
      {142,      7916.419,          5258.67,    0.0103859210831},
      {134,      7916.419,          5190.09,   0.00672919038169},
      {131,      7916.419,          5183.89,   0.00368695161632},
      {128,      7916.419,          5152.11,   0.00270980870161},
      {124,      7916.419,          5139.86,   0.00180318125497},
      {121,      7916.419,           5085.3,   0.00330415336107},
      {118,      7916.419,           5023.2,   0.00184347580816},
      {114,      7916.419,           5019.5,   0.00191399127623},
      {112,      7916.419,          4983.51,   0.00203487493578},
      {109,      7916.419,          4903.08,   0.00152111938269},
      {107,      7916.419,            4883.,  0.000392871893542},
      {102,      7916.419,           4835.1,  0.000302209148878},
      {99,       7916.419,           4803.8,   0.00119876295722},
      {98,       7916.419,           4790.7,   0.00107787929767},
      {94,       7916.419,           4708.9,  0.000856259255155},
      {91,       7916.419,          4658.53,   0.00775670148788},
      {88,       7916.419,          4603.07,    0.0054699855947},
      {85,       7916.419,           4572.5,   0.00100736382959},
      {81,       7916.419,          4562.95,   0.00315304878663},
      {77,       7916.419,          4504.04,   0.00490586185012},
      {75,       7916.419,          4475.66,   0.00481519910546},
      {72,       7916.419,          4450.86,    0.0017326657869},
      {69,       7916.419,           4440.9,   0.00196435946771},
      {67,       7916.419,          4423.12,   0.00141030936143},
      {63,       7916.419,            4405.,   0.00306238604197},
      {61,       7916.419,           4391.9,   0.00133979389336},
      {58,       7916.419,          4320.24,    0.0135994116995},
      {54,       7916.419,           4312.8,   0.00282061872286},
      {52,       7916.419,          4286.62,   0.00339481610573},
      {50,       7916.419,          4204.37,   0.00255870412717},
      {46,       7916.419,          4133.08,   0.00383805619075},
      {38,       7916.419,          4089.11,   0.00255870412717},
      {31,       7916.419,            3883.,   0.00147075119121},
      {25,       7916.419,          3844.44,   0.00493608276501},
      {22,       7916.419,          3775.27,   0.00203487493578},
      {19,       7916.419,           3651.6,  0.000624565574348},
      {15,       7916.419,          3588.52,   0.00342503702062},
      {11,       7916.419,           3482.9,   0.00115846840403},
      {7,        7916.419,           3472.2,   0.00184347580816},
      {5,        7916.419,           3366.8,   0.00145060391462},
      {1,        7916.419,          3153.05,                1.0},
      {182,       4763.39,          3521.02,     0.385650224215},
      {177,       4763.39,           3442.6,     0.210762331839},
      {30,        4763.39,          0960.63,                1.0},
      {47,        4549.86,          1074.49,                1.0},
      {212,       4444.48,          4444.35,     0.676549865229},
      {209,       4444.48,           4166.7,     0.137466307278},
      {195,       4444.48,           3781.8,                1.0},
      {180,       4432.92,           3506.7,     0.214067278287},
      {173,       4432.92,           3145.4,     0.357798165138},
      {147,       4432.92,           2838.2,                1.0},
      {195,       4327.67,           3718.1,     0.141414141414},
      {179,       4327.67,           3431.6,     0.401515151515},
      {57,        4327.67,          1136.59,                1.0},
      {180,       4264.15,          3385.73,     0.631386861314},
      {169,       4264.15,           3022.8,                1.0},
      {186,       4140.83,            3478.,     0.544973544974},
      {56,        4140.83,          1059.95,                1.0},
      {198,       4071.59,           3911.8,     0.170289855072},
      {194,       4071.59,           3729.6,    0.0634057971014},
      {171,       4071.59,          3175.26,     0.278985507246},
      {146,       4071.59,          2572.03,      0.38768115942},
      {102,       4071.59,           1747.3,                1.0},
      {194,       4034.03,           4033.4,     0.170138888889},
      {192,       4034.03,           3874.7,            0.15625},
      {163,       4034.03,            3108.,     0.121527777778},
      {123,       4034.03,          2291.42,                1.0},
      {188,       3990.85,            3714.,     0.248868778281},
      {183,       3990.85,          3628.36,                1.0},
      {187,       3826.92,           3667.6,     0.138936535163},
      {181,       3826.92,          3464.55,     0.445969125214},
      {139,       3826.92,          2365.32,     0.265866209262},
      {92,        3826.92,          1501.94,                1.0},
      {184,       3802.74,            3802.,     0.346570397112},
      {168,       3802.74,            3140.,      0.56678700361},
      {105,       3802.74,           1826.2,                1.0},
      {180,       3783.14,           3623.1,              0.352},
      {157,       3783.14,           2904.6,                0.1},
      {115,       3783.14,          2082.45,                1.0},
      {177,       3711.92,           3552.9,                1.0},
      {53,        3629.42,          0897.06,                1.0},
      {176,       3603.09,           3603.9,     0.154696132597},
      {164,       3603.09,          2993.91,     0.596685082873},
      {151,       3603.09,           2724.8,                1.0},
      {82,        3596.04,          1316.24,     0.164133738602},
      {78,        3596.04,           1241.5,                1.0},
      {81,         3524.7,          1250.45,                1.0},
      {170,       3511.19,           3510.5,      0.33064516129},
      {167,       3511.19,           3232.3,     0.177419354839},
      {148,       3511.19,           2772.2,                1.0},
      {156,       3493.37,           2885.3,                1.0},
      {165,       3475.52,          3316.58,     0.828571428571},
      {150,       3475.52,           2811.1,                1.0},
      {76,        3465.57,          1198.75,      0.28870292887},
      {26,        3465.57,          0532.94,                1.0},
      {60,        3440.28,          0946.64,                1.0},
      {160,       3412.19,           3253.2,     0.366847826087},
      {158,       3412.19,           3133.9,     0.326086956522},
      {117,       3412.19,           2048.9,                1.0},
      {156,       3352.84,           3074.9,     0.203389830508},
      {81,        3352.84,          1447.69,     0.545197740113},
      {39,        3352.84,          0634.78,                1.0},
      {151,       3343.92,           3001.4,     0.537735849057},
      {104,       3343.92,          1844.67,                1.0},
      {153,       3313.11,           3312.4,     0.431034482759},
      {10,        3313.11,          0261.33,                1.0},
      {151,       3257.61,          3257.26,     0.710526315789},
      {86,        3257.61,          1556.84,                1.0},
      {120,       3207.59,          2280.36,     0.348525469169},
      {51,        3207.59,         0831.176,                1.0},
      {60,        3191.07,          0937.01,                1.0},
      {146,        3125.5,           3125.5,                1.0},
      {146,       3111.82,           3111.6,     0.201030927835},
      {9,         3111.82,          0214.97,                1.0},
      {42,        3080.84,            0587.,     0.226519337017},
      {35,        3080.84,          0583.22,                1.0},
      {142,       3051.77,           3052.2,     0.853658536585},
      {107,       3051.77,           1808.5,                1.0},
      {75,        3033.76,          1293.92,                1.0},
      {135,       3013.07,           2666.6,     0.152091254753},
      {92,        3013.07,          1574.36,                1.0},
      {137,       2932.48,          2932.06,                1.0},
      {67,        2896.84,          1127.84,     0.668555240793},
      {54,        2896.84,          0804.29,      0.13597733711},
      {35,        2896.84,          0440.13,                1.0},
      {99,        2892.35,          1649.52,     0.360869565217},
      {44,        2892.35,          0625.35,                1.0},
      {131,       2830.54,           2830.1,     0.391156462585},
      {88,        2830.54,           1476.1,                1.0},
      {129,       2776.56,           2776.8,     0.308988764045},
      {122,       2776.56,           2413.7,     0.216292134831},
      {107,       2776.56,          2037.53,                1.0},
      {125,       2764.19,           2605.2,     0.416666666667},
      {45,        2764.19,          0689.08,                1.0},
      {124,       2732.32,          2732.13,     0.738461538462},
      {112,       2732.32,          2123.06,                1.0},
      {89,        2726.19,          1484.85,    0.0974025974026},
      {84,        2726.19,          1428.17,     0.125541125541},
      {74,        2726.19,           1287.4,    0.0757575757576},
      {50,        2726.19,          0957.27,     0.318181818182},
      {9,         2726.19,          0192.53,                1.0},
      {88,        2717.97,           1790.3,                1.0},
      {90,        2695.22,          1799.48,                1.0},
      {115,       2657.34,           2656.8,     0.161160354553},
      {113,       2657.34,          2497.89,     0.660757453666},
      {87,        2657.34,          1761.01,    0.0668815471394},
      {50,        2657.34,          0974.17,                1.0},
      {73,        2647.91,          1327.62,                1.0},
      {12,        2635.53,          0247.58,                1.0},
      {109,        2594.9,           2594.9,                1.0},
      {109,       2533.65,          2533.53,                1.0},
      {108,       2507.29,          2507.29,                1.0},
      {104,       2497.59,          2153.71,     0.246933769419},
      {92,        2497.59,          1834.22,    0.0678659035159},
      {83,        2497.59,          1619.24,     0.200327064595},
      {55,        2497.59,          0998.28,     0.169255928046},
      {43,        2497.59,          0890.26,     0.131643499591},
      {41,        2497.59,          0814.45,                1.0},
      {95,        2493.57,          1918.69,     0.529100529101},
      {86,        2493.57,          1830.34,                1.0},
      {100,       2465.47,          2465.43,                1.0},
      {73,        2456.69,          1560.94,                1.0},
      {77,        2387.95,           1641.7,                1.0},
      {76,        2376.41,           1630.1,                0.5},
      {39,        2376.41,          0782.29,                1.0},
      {62,        2360.47,          1119.55,                1.0},
      {33,        2354.62,          0747.34,                1.0},
      {93,        2324.75,          2324.75,                1.0},
      {89,        2316.49,          1972.59,                1.0},
      {91,        2301.09,          2141.73,                1.0},
      {80,        2279.75,          1670.92,                1.0},
      {86,        2274.27,           1929.5,     0.218543046358},
      {51,        2274.27,          0953.97,                1.0},
      {82,        2267.01,           1904.8,                1.0},
      {83,        2254.06,          191018,                1.0},
      {80,        2144.53,           1782.2,     0.480487804878},
      {78,        2144.53,          1570.22,     0.109756097561},
      {73,        2144.53,           1535.7,     0.129268292683},
      {68,        2144.53,          1481.75,     0.146341463415},
      {9,         2144.53,          0291.71,                1.0},
      {75,        2092.26,           1729.7,                1.0},
      {38,        2075.12,          0711.94,                1.0},
      {58,         2050.0,           1303.9,              0.528},
      {36,         2050.0,          0695.41,                1.0},
      {70,        1976.33,          1401.66,                1.0},
      {55,       1905.093,         1158.831,     0.800643086817},
      {49,       1905.093,          1009.35,                1.0},
      {74,        1900.28,          1900.25,                1.0},
      {73,        1852.65,          1852.64,     0.784240150094},
      {68,        1852.65,          1508.68,                1.0},
      {67,        1779.54,           1435.3,    0.0721649484536},
      {64,        1779.54,          1417.27,                1.0},
      {63,        1768.99,          1407.08,                1.0},
      {68,         1749.2,           1749.2,                1.0},
      {68,        1742.59,          1742.83,     0.301694915254},
      {63,        1742.59,           1398.7,     0.281355932203},
      {40,        1742.59,          0846.87,                1.0},
      {58,        1739.85,          1165.21,                1.0},
      {63,        1700.65,          1541.56,     0.337579617834},
      {40,        1700.65,          0822.33,                1.0},
      {62,       1683.122,          1683.09,                1.0},
      {54,        1607.31,          1032.68,                1.0},
      {60,       1594.315,          1594.42,     0.284382284382},
      {53,       1594.315,          1232.13,     0.715617715618},
      {51,       1594.315,          1019.59,                1.0},
      {50,        1594.23,          1019.59,                1.0},
      {55,        1550.54,          1391.25,                1.0},
      {55,       1521.147,           1521.2,               0.36},
      {53,       1521.147,          1361.76,     0.424545454545},
      {49,       1521.147,          1177.04,               0.03},
      {41,       1521.147,          0912.37,    0.0881818181818},
      {36,       1521.147,          0858.09,                1.0},
      {50,        1499.18,          1499.54,     0.249128919861},
      {48,        1499.18,          1339.88,     0.393728222997},
      {46,        1499.18,          1220.84,                1.0},
      {21,        1461.35,          0565.43,                1.0},
      {46,        1438.69,          1438.75,     0.214516129032},
      {44,        1438.69,          1279.41,     0.312903225806},
      {38,        1438.69,          1076.35,     0.443548387097},
      {28,        1438.69,           0775.9,                1.0},
      {42,        1363.21,          1363.21,                1.0},
      {42,        1354.25,          1354.68,    0.0986717267552},
      {40,        1354.25,          1194.89,     0.552182163188},
      {34,        1354.25,          0992.11,     0.161290322581},
      {32,        1354.25,          0779.65,                1.0},
      {38,       1320.329,         1320.315,                1.0},
      {37,       1298.123,         1298.134,     0.331437855402},
      {35,       1298.123,         1138.821,                1.0},
      {29,        1287.15,          0924.91,                1.0},
      {34,        1242.65,          1242.56,                1.0},
      {32,       1241.091,          1081.74,     0.193140794224},
      {30,       1241.091,          0962.68,     0.256317689531},
      {20,       1241.091,          0632.34,     0.153429602888},
      {9,        1241.091,         0494.852,                1.0},
      {29,        927.079,          0927.05,    0.0855297157623},
      {27,        927.079,         0767.795,     0.175452196382},
      {25,        927.079,           0648.8,                1.0},
      {25,        895.714,          0736.52,    0.0752256770311},
      {23,        895.714,         0617.433,     0.744232698094},
      {17,        895.714,           0320.7,                1.0},
      {23,        878.277,         0878.277,     0.585253456221},
      {18,        878.277,          0534.11,                1.0},
      {19,        746.245,         0467.992,                1.0},
      {20,        739.051,          0739.12,    0.0679389312977},
      {18,        739.051,         0579.753,     0.631043256997},
      {16,        739.051,         0460.792,    0.0959287531807},
      {13,        739.051,          0395.28,    0.0269720101781},
      {10,        739.051,         0376.851,                1.0},
      {15,          663.0,          0663.06,      0.37247706422},
      {13,          663.0,          0503.65,     0.293577981651},
      {11,          663.0,          0384.74,     0.278899082569},
      {8,           663.0,           0318.9,                1.0},
      {11,        608.784,          0608.75,     0.785498489426},
      {9,         608.784,         0449.512,      0.10775427996},
      {7,         608.784,          0330.47,    0.0251762336354},
      {4,         608.784,         0264.882,                1.0},
      {1,         574.629,         0212.388,                1.0},
      {6,         362.231,           0362.3,    0.0231259968102},
      {4,         362.231,         0202.948,                1.0},
      {4,         343.898,          0343.94,     0.968801313629},
      {2,         343.898,         0184.612,                1.0},
      {2,         278.257,         0278.244,                1.0},
      {1,         159.282,          0159.28,                1.0},
      {0,             0.0,              0.0,                1.0},
    };

    G4int decay_num = sizeof(decay)/sizeof(decay[0]);
    //G4cout << "p = " << p << G4endl;
    for (G4int j = 0; 0<= j && j < decay_num;) {
        G4double p = G4UniformRand();
        if (p < decay[j].ratio) {
           G4double energy = decay[j].gamma;
           //G4cout << "energy = " << energy << G4endl;
           if (energy > 0.){
              //G4cout << "Setting energy " << energy << G4endl;
              fParticleGun1->SetParticleEnergy(energy*keV);
              G4double phi1 = 2*pi*G4UniformRand();
              G4double theta1 = pi*G4UniformRand();
              G4double x1 = sin(theta1)*cos(phi1);
              G4double y1 = sin(theta1)*sin(phi1);
              G4double z1 = cos(theta1);

              fParticleGun1->
                       SetParticleMomentumDirection(G4ThreeVector(x1,y1,z1));
              // If energy > 0, a gamma is generated, but that doesn't mean
              // the cascade is finished!
              fParticleGun1->GeneratePrimaryVertex(anEvent);
              //G4cout <<"After generation" << G4endl;
           }
           // Only break out of loop when 'jump' == 0
           if (decay[j].jump == 0) break;
           else j += decay[j].jump;
           p = G4UniformRand();
        } else {
           p -= decay[j].ratio;
           j++;
        }
    }
  }

  else if (fCuIsotope == 65) {
    //**** Start choosing energies of gammas from cu65ng cascade ****//
    nCapture_CuInfo decay[] = {
      {346,       7066.22,          7065.72,    0.0240776531306},
      {344,       7066.22,          6879.82,   0.00378980100845},
      {342,       7066.22,           6828.1,  0.000237537385145},
      {339,       7066.22,          6790.64,    0.0283965146787},
      {336,       7066.22,          6680.01,     0.149000723409},
      {332,       7066.22,          6600.66,     0.155479015731},
      {328,       7066.22,          6336.16,   0.00155479015731},
      {323,       7066.22,          6243.19,    0.0263450554434},
      {317,       7066.22,          6048.84,    0.0184631331181},
      {313,       7066.22,          6013.46,   0.00129565846443},
      {309,       7066.22,           5907.9,  0.000377900385458},
      {306,       7066.22,          5853.41,    0.0120064351037},
      {301,       7066.22,          5722.03,    0.0041353099323},
      {298,       7066.22,           5626.9,  0.000485871924161},
      {294,       7066.22,          5518.73,   0.00711532440049},
      {291,       7066.22,          5505.56,   0.00102572961767},
      {289,       7066.22,          5488.65,   0.00100413530993},
      {284,       7066.22,          5388.12,   0.00574408585897},
      {276,       7066.22,          5320.23,     0.066402496302},
      {270,       7066.22,          5245.88,    0.0788192232528},
      {264,       7066.22,          5138.98,   0.00283965146787},
      {258,       7066.22,          5047.98,    0.0377900385458},
      {248,       7066.22,            5043.,    0.0632713216796},
      {245,       7066.22,          4941.94,   0.00401654123973},
      {243,       7066.22,          4903.09,   0.00831380848008},
      {241,       7066.22,          4900.15,   0.00345508923848},
      {237,       7066.22,           4805.3,    0.0025049396979},
      {235,       7066.22,           4703.4,  0.000550654847382},
      {233,       7066.22,           4671.2,   0.00211624215857},
      {231,       7066.22,           4617.8,   0.00185711046568},
      {227,       7066.22,          4612.55,   0.00412451277843},
      {224,       7066.22,          4562.96,   0.00361704654653},
      {222,       7066.22,           4545.2,  0.000475074770291},
      {219,       7066.22,           4506.9,  0.000593843462863},
      {215,       7066.22,          4479.82,    0.0115529546412},
      {211,       7066.22,          4468.58,   0.00288284008335},
      {209,       7066.22,          4457.73,   0.00240776531306},
      {206,       7066.22,           4435.2,  0.000593843462863},
      {201,       7066.22,          4401.69,    0.0111210684863},
      {193,       7066.22,          4385.13,    0.0377900385458},
      {190,       7066.22,          4377.75,   0.00831380848008},
      {187,       7066.22,          4326.66,   0.00913439217422},
      {184,       7066.22,          4298.14,   0.00245095392854},
      {180,       7066.22,          4266.54,   0.00124167269508},
      {176,       7066.22,          4251.95,   0.00138203569539},
      {173,       7066.22,           4221.2,   0.00377900385458},
      {170,       7066.22,          4198.32,    0.0117688977186},
      {167,       7066.22,          4122.91,   0.00356306077718},
      {162,       7066.22,          4117.22,   0.00950149540581},
      {156,       7066.22,          4113.25,   0.00669423539955},
      {153,       7066.22,           4078.2,   0.00144681861861},
      {149,       7066.22,          4055.62,   0.00267769415982},
      {146,       7066.22,          4039.82,   0.00809786540268},
      {143,       7066.22,           4020.3,   0.00106891823315},
      {141,       7066.22,          4016.95,   0.00356306077718},
      {135,       7066.22,          3988.21,   0.00139283284926},
      {131,       7066.22,          3974.97,   0.00422168716326},
      {126,       7066.22,          3966.99,   0.00462118185646},
      {123,       7066.22,           3955.1,   0.00104732392541},
      {121,       7066.22,           3923.8,  0.000399494693199},
      {118,       7066.22,            3914.,  0.000712612155436},
      {114,       7066.22,          3900.18,     0.013064556183},
      {111,       7066.22,            3857.,   0.00451321031776},
      {109,       7066.22,          3818.14,   0.00545256270447},
      {106,       7066.22,          3778.38,   0.00520422816546},
      {103,       7066.22,          3732.12,   0.00264530269821},
      {101,       7066.22,          3724.08,   0.00507466231901},
      {98,        7066.22,          3695.05,   0.00157638446506},
      {95,        7066.22,          3668.23,   0.00100413530993},
      {92,        7066.22,          3633.54,   0.00153319584957},
      {89,        7066.22,           3586.2,  0.000809786540268},
      {83,        7066.22,            3578.,   0.00125246984895},
      {78,        7066.22,          3556.99,    0.0045348046255},
      {73,        7066.22,          3530.56,   0.00766597924787},
      {68,        7066.22,          3482.51,   0.00356306077718},
      {63,        7066.22,            3465.,   0.00691017847695},
      {55,        7066.22,           3429.4,    0.0113370115638},
      {52,        7066.22,          3361.11,   0.00164116738828},
      {46,        7066.22,          3316.04,   0.00356306077718},
      {43,        7066.22,          3285.85,   0.00356306077718},
      {37,        7066.22,          3251.26,   0.00259131692886},
      {33,        7066.22,           3169.4,   0.00215943077405},
      {30,        7066.22,           3131.7,   0.00186790761955},
      {27,        7066.22,          3052.91,   0.00248334539016},
      {23,        7066.22,          3009.32,   0.00320675469946},
      {20,        7066.22,           2949.5,   0.00205145923535},
      {16,        7066.22,           2766.2,   0.00129565846443},
      {14,        7066.22,           2603.3,   0.00248334539016},
      {10,        7066.22,           2539.3,  0.000755800770917},
      {5,         7066.22,          2215.88,    0.0175993608085},
      {1,         7066.22,          1988.92,                1.0},
      {209,       5077.21,           3383.6,     0.283752860412},
      {89,        5077.21,          1966.29,     0.443935926773},
      {61,        5077.21,          1589.79,                1.0},
      {211,       4850.76,           3275.2,    0.0656275635767},
      {206,       4850.76,          3172.95,     0.598851517637},
      {202,       4850.76,          3137.18,     0.246103363413},
      {93,        4850.76,           1773.5,                1.0},
      {208,       4527.91,           2968.8,     0.163982430454},
      {190,       4527.91,           2708.2,     0.190336749634},
      {123,       4527.91,          1728.01,                1.0},
      {244,        4462.7,          4276.58,                1.0},
      {240,        4300.2,           4024.9,     0.283950617284},
      {234,        4300.2,           3835.8,     0.234567901235},
      {226,        4300.2,           3476.9,                1.0},
      {225,       4116.41,          3293.49,     0.837209302326},
      {114,       4116.41,          1303.03,                1.0},
      {238,       4056.98,          3871.04,     0.523926380368},
      {234,       4056.98,          3782.18,     0.344785276074},
      {151,       4056.98,          1607.34,                1.0},
      {230,       4013.69,           3627.9,     0.244851258581},
      {214,       4013.69,          2996.78,                1.0},
      {228,       3934.58,           3548.6,     0.551724137931},
      {166,       3934.58,          1916.35,                1.0},
      {232,       3896.38,           3896.3,     0.148648648649},
      {182,       3896.38,           2160.4,     0.175675675676},
      {130,       3896.38,          1298.87,                1.0},
      {229,       3814.66,           3814.2,     0.112456747405},
      {219,       3814.66,           3348.5,     0.195501730104},
      {211,       3814.66,           2991.1,     0.207612456747},
      {162,       3814.66,          1843.71,     0.250865051903},
      {42,        3814.66,          0417.03,                1.0},
      {140,       3780.19,          1416.38,     0.578475336323},
      {77,        3780.19,          0770.64,                1.0},
      {221,        3750.3,           3563.7,     0.188679245283},
      {169,        3750.3,          2004.42,      0.38679245283},
      {153,        3750.3,          1732.27,     0.199685534591},
      {116,        3750.3,           1120.8,    0.0896226415094},
      {55,         3750.3,           0651.1,                1.0},
      {187,       3705.08,          2457.66,     0.561349693252},
      {140,       3705.08,            1682.,                1.0},
      {214,       3636.56,           3450.3,    0.0828138913624},
      {204,       3636.56,          3045.46,     0.204808548531},
      {192,       3636.56,          2619.14,     0.249332146037},
      {188,       3636.56,           2584.3,     0.080142475512},
      {182,       3636.56,          2423.94,     0.170080142476},
      {158,       3636.56,          1890.61,     0.124666073019},
      {117,       3636.56,          1115.48,                1.0},
      {196,        3601.0,           2870.7,     0.110014104372},
      {166,        3601.0,          2023.55,     0.662905500705},
      {154,        3601.0,           1854.3,    0.0761636107193},
      {13,         3601.0,          0114.36,                1.0},
      {200,       3583.53,           3307.4,     0.206378986867},
      {174,       3583.53,           2371.4,     0.243902439024},
      {167,       3583.53,          2144.22,     0.348968105066},
      {51,        3583.53,          0537.55,                1.0},
      {165,       3535.49,          2095.76,     0.223068552775},
      {159,       3535.49,          1975.26,     0.203482045702},
      {149,       3535.49,          1799.84,      0.14907508161},
      {41,        3535.49,           0457.6,                1.0},
      {195,       3508.84,           3322.2,    0.0969696969697},
      {193,       3508.84,          3270.78,     0.690909090909},
      {184,       3508.84,            2918.,    0.0787878787879},
      {172,       3508.84,            2492.,                1.0},
      {192,       3487.05,           3486.2,     0.126036484245},
      {182,       3487.05,          3021.72,     0.358208955224},
      {180,       3487.05,           2896.9,     0.144278606965},
      {172,       3487.05,           2478.2,      0.28192371476},
      {124,       3487.05,           1515.3,                1.0},
      {185,       3479.48,           3241.9,     0.590717299578},
      {60,        3479.48,           0665.6,                1.0},
      {161,       3432.37,          2380.34,     0.823529411765},
      {108,       3432.37,           1408.4,                1.0},
      {179,       3397.63,           3121.3,               0.44},
      {172,       3397.63,           2806.9,                1.0},
      {169,       3371.23,           2641.3,     0.342465753425},
      {77,        3371.23,          0741.94,                1.0},
      {175,       3342.06,           3067.2,                1.0},
      {81,        3333.77,          0747.48,     0.762845849802},
      {54,        3333.77,          0533.96,                1.0},
      {170,       3287.36,           2901.2,     0.140164899882},
      {166,       3287.36,          2821.76,                1.0},
      {133,       3247.83,          1670.32,                1.0},
      {173,       3208.95,           3208.1,     0.268292682927},
      {92,        3208.95,          1042.86,                1.0},
      {171,       3165.77,           3165.8,     0.198113207547},
      {161,       3165.77,          2700.26,     0.679245283019},
      {157,       3165.77,           2436.7,                1.0},
      {164,       3151.97,          2876.19,     0.516616314199},
      {83,        3151.97,          0788.42,                1.0},
      {162,       3141.74,          2866.61,                1.0},
      {165,       3110.86,           3111.4,              0.192},
      {100,       3110.86,          1139.65,                1.0},
      {162,       3099.08,          2912.57,     0.359908883827},
      {156,       3099.08,           2713.3,     0.227790432802},
      {151,       3099.08,           2508.3,     0.250569476082},
      {139,       3099.08,           2082.6,                1.0},
      {159,       3091.37,          3090.95,     0.208010335917},
      {134,       3091.37,          2039.33,     0.107235142119},
      {122,       3091.37,          1652.01,                1.0},
      {155,       3077.29,           2890.6,     0.205930807249},
      {146,       3077.29,           2612.1,     0.444810543657},
      {138,       3077.29,           2254.5,     0.148270181219},
      {132,       3077.29,           2059.9,     0.110378912685},
      {60,        3077.29,          0556.46,                1.0},
      {150,       3048.82,          2862.63,                1.0},
      {146,       3045.95,           2770.7,     0.275862068966},
      {137,       3045.95,           2315.7,                1.0},
      {148,       3026.09,          3025.77,     0.674318507891},
      {95,        3026.09,           1280.2,                1.0},
      {145,       3010.18,           2824.8,     0.321715817694},
      {136,       3010.18,           2545.2,     0.214477211796},
      {111,       3010.18,          1666.15,                1.0},
      {143,       2987.96,           2986.9,             0.4625},
      {78,        2987.96,           1017.1,                1.0},
      {141,       2953.35,          2952.64,      0.36954087346},
      {131,       2953.35,           2488.4,      0.26875699888},
      {123,       2953.35,          2131.25,     0.127659574468},
      {114,       2953.35,          1901.52,     0.170212765957},
      {30,        2953.35,           0289.2,                1.0},
      {132,       2948.76,          2673.42,     0.440771349862},
      {100,       2948.76,          1509.64,     0.241046831956},
      {96,        2948.76,          1401.26,     0.101928374656},
      {32,        2948.76,          0340.19,                1.0},
      {131,       2943.33,           2758.8,     0.617283950617},
      {125,       2943.33,           2557.8,                1.0},
      {128,       2867.69,          2629.61,     0.763440860215},
      {120,       2867.69,          2402.83,                1.0},
      {124,       2844.72,          2569.26,     0.525373134328},
      {115,       2844.72,          2114.78,                1.0},
      {102,       2813.84,           1761.6,     0.163265306122},
      {90,        2813.84,          1374.41,     0.614795918367},
      {67,        2813.84,          0993.48,                1.0},
      {122,       2799.85,           2615.2,     0.146341463415},
      {92,        2799.85,           1553.1,     0.475609756098},
      {69,        2799.85,          1053.88,                1.0},
      {104,       2767.86,          1944.97,     0.655172413793},
      {81,        2767.86,           1220.4,                1.0},
      {117,       2739.16,           2553.1,     0.354609929078},
      {84,        2739.16,           1394.9,                1.0},
      {116,       2688.22,          2687.88,     0.641975308642},
      {113,       2688.22,          2450.91,                1.0},
      {114,       2681.16,          2680.38,      0.20278833967},
      {89,        2681.16,           1629.2,    0.0760456273764},
      {85,        2681.16,            1523.,    0.0228136882129},
      {82,        2681.16,          1468.56,     0.219687367976},
      {64,        2681.16,          0987.18,     0.220532319392},
      {52,        2681.16,          0860.85,    0.0599915504858},
      {46,        2681.16,          0753.91,                1.0},
      {107,       2664.44,          2664.17,     0.322274881517},
      {102,       2664.44,           2389.3,    0.0947867298578},
      {84,        2664.44,          1647.33,     0.369668246445},
      {77,        2664.44,          1506.57,                1.0},
      {87,        2629.29,          1806.57,     0.473684210526},
      {64,        2629.29,          1081.95,                1.0},
      {101,        2608.5,           2608.5,                1.0},
      {99,        2597.49,          2411.58,     0.490463215259},
      {66,        2597.49,          1253.28,      0.25068119891},
      {14,        2597.49,          0234.26,                1.0},
      {96,        2586.27,            2400.,     0.143497757848},
      {69,        2586.27,          1428.18,     0.681614349776},
      {60,        2586.27,          1146.79,                1.0},
      {94,        2560.43,           2561.3,     0.338308457711},
      {63,        2560.43,          1313.29,                1.0},
      {82,        2520.77,          1929.63,                1.0},
      {81,         2503.0,          1912.13,     0.498207885305},
      {66,         2503.0,           1450.6,                1.0},
      {85,        2453.05,          2177.87,     0.348195329087},
      {76,        2453.05,          1723.07,     0.354564755839},
      {30,        2453.05,          0632.67,                1.0},
      {86,        2449.19,           2448.7,                1.0},
      {81,        2394.93,          2120.25,                1.0},
      {72,        2363.63,          1633.89,                1.0},
      {79,        2260.66,          1985.73,     0.639566395664},
      {76,        2260.66,          1874.41,     0.224932249322},
      {57,        2260.66,           1208.8,                1.0},
      {79,        2166.01,          1980.01,                1.0},
      {79,        2163.12,          2163.22,                1.0},
      {78,        2124.09,          2123.95,     0.472140762463},
      {73,        2124.09,           1849.4,                1.0},
      {75,       2023.315,          1837.44,     0.134291658786},
      {71,       2023.315,            1748.,     0.111594477019},
      {68,       2023.315,          1637.49,      0.44826933989},
      {64,       2023.315,           1557.4,    0.0794401361831},
      {60,       2023.315,          1293.71,    0.0766029884623},
      {42,       2023.315,          0810.47,     0.049177227161},
      {37,       2023.315,           0679.3,    0.0313977681105},
      {34,       2023.315,          0583.62,     0.038963495366},
      {9,        2023.315,          0111.93,                1.0},
      {66,        2018.36,          1832.39,     0.335533553355},
      {62,        2018.36,           1743.4,     0.643564356436},
      {53,        2018.36,          1288.63,                1.0},
      {35,        1971.18,          0758.83,     0.241525423729},
      {33,        1971.18,          0723.99,                1.0},
      {50,        1927.19,          1197.21,     0.482009504413},
      {40,        1927.19,          0909.99,    0.0291921249151},
      {31,        1927.19,          0714.66,                1.0},
      {47,        1911.31,           1180.8,     0.224043715847},
      {42,        1911.31,          1088.64,                1.0},
      {57,       1820.352,          1820.21,     0.128453038674},
      {54,       1820.352,          1582.51,     0.220994475138},
      {46,       1820.352,          1355.18,     0.310773480663},
      {38,       1820.352,         0997.648,     0.201657458564},
      {29,       1820.352,         0768.305,                1.0},
      {52,        1745.89,           1746.2,     0.145161290323},
      {50,        1745.89,          1559.86,     0.752688172043},
      {46,        1745.89,          1471.05,                1.0},
      {37,        1735.96,          1006.19,                1.0},
      {36,         1713.2,          0983.21,               0.64},
      {23,         1713.2,          0661.22,                1.0},
      {17,        1694.07,          0482.69,                1.0},
      {45,         1678.0,          1678.19,     0.533515731874},
      {32,         1678.0,          0948.09,     0.261285909713},
      {10,         1678.0,          0334.03,     0.109439124487},
      {1,          1678.0,          0100.15,                1.0},
      {29,        1577.34,          0847.42,                1.0},
      {38,        1560.15,          1322.16,     0.873015873016},
      {6,         1560.15,            0217.,                1.0},
      {34,        1547.39,          1272.32,     0.494488188976},
      {31,        1547.39,          1161.63,     0.088188976378},
      {26,        1547.39,          0956.74,                1.0},
      {35,       1439.408,          1439.37,     0.951417004049},
      {13,       1439.408,          0422.01,                1.0},
      {33,       1344.012,           1343.4,    0.0241477272727},
      {26,       1344.012,          0958.25,     0.255681818182},
      {22,       1344.012,         0878.816,                1.0},
      {26,       1247.152,         0972.108,                1.0},
      {29,       1212.515,          1212.52,     0.608963393774},
      {24,       1212.515,         0937.507,                1.0},
      {23,        1158.09,          0883.03,      0.46556122449},
      {9,         1158.09,          0335.73,                1.0},
      {24,       1052.082,          1052.19,     0.639254738195},
      {21,       1052.082,          0814.27,     0.253774494057},
      {13,       1052.082,          0586.79,                1.0},
      {20,       1017.138,         0831.196,     0.600088573959},
      {11,       1017.138,         0551.953,     0.107617360496},
      {9,        1017.138,         0426.372,     0.124003542958},
      {2,        1017.138,          0194.47,                1.0},
      {11,        1008.49,          0622.69,                1.0},
      {16,        822.691,         0822.676,      0.60032772233},
      {14,        822.691,          0636.68,    0.0436466557426},
      {8,         822.691,         0436.912,     0.283032921198},
      {4,         822.691,         0357.561,                1.0},
      {11,        729.824,         0543.852,     0.904967602592},
      {7,         729.824,           0454.8,                1.0},
      {6,          590.75,         0315.711,                1.0},
      {9,         465.165,         0465.152,     0.949744463373},
      {7,         465.165,          0279.33,    0.0149063032368},
      {3,         465.165,           0190.1,                1.0},
      {6,         385.782,         0385.781,     0.969387755102},
      {4,         385.782,           0199.9,                1.0},
      {4,          275.03,          0274.92,    0.0137531668476},
      {2,          275.03,          0089.18,                1.0},
      {2,         237.822,         0237.821,                1.0},
      {1,         185.953,          0186.01,                1.0},
      {0,             0.0,              0.0,                1.0},
    };

    G4int decay_num = sizeof(decay)/sizeof(decay[0]);
    //G4cout << "Decay Num = " << decay_num << G4endl;
    G4double p = G4UniformRand();
    //G4cout << "p = " << p << G4endl;
    for (G4int j = 0; 0<= j && j < decay_num;) {
        if (p < decay[j].ratio) {
           G4double energy = decay[j].gamma;
           //G4cout << "energy = " << energy << G4endl;
           if (energy > 0.){
              //G4cout << "Setting energy " << energy << G4endl;
              fParticleGun1->SetParticleEnergy(energy*keV);
              G4double phi1 = 2*pi*G4UniformRand();
              G4double theta1 = pi*G4UniformRand();
              G4double x1 = sin(theta1)*cos(phi1);
              G4double y1 = sin(theta1)*sin(phi1);
              G4double z1 = cos(theta1);
              fParticleGun1->
                       SetParticleMomentumDirection(G4ThreeVector(x1,y1,z1));
              // If energy > 0, a gamma is generated, but that doesn't mean
              // the cascade is finished!
              fParticleGun1->GeneratePrimaryVertex(anEvent);
              //G4cout <<"After generation" << G4endl;
           }
           // Only break out of loop when 'jump' == 0
           if (decay[j].jump == 0) break;
           else j += decay[j].jump;
           p = G4UniformRand();
        } else {
           p -= decay[j].ratio;
           j++;
        }
    }

  }

  else {
    G4cout << "No correct isotope specified." << G4endl;
  }

}

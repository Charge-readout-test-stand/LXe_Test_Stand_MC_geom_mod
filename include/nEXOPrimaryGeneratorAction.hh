
#ifndef nEXOPrimaryGeneratorAction_h
#define nEXOPrimaryGeneratorAction_h 1

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"

#include "nEXOSimplePhysVolManager.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Cons.hh"

class G4Event;
class G4ParticleGun;
class G4GeneralParticleSource;
class nEXOPrimaryGeneratorMessenger;

class nEXOPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:

    nEXOPrimaryGeneratorAction();
    virtual ~nEXOPrimaryGeneratorAction();

    void GeneratePrimaries(G4Event* );

    void Set_generator(G4String );

  void GetUnifRandPosInLXe(G4ThreeVector& pos);
    void Generate_bb0n(G4Event* );
  void Generate_bb2n(G4Event* anEvent);
  void Generate_bb2n_EndPoint(G4Event* anEvent);

    void Generate_nCaptureXe136(G4Event*);

    void Generate_nCaptureCu(G4Event*);

    G4double D_bb0n_spectral_max(G4double );
    G4double D_bb0n_spectrum(G4double ,G4double );
    G4double Fermi_function(G4int ,G4double );
    G4double BB2n_sum_spectrum(G4double );
    G4double D_spectrum (G4double ,G4double );
    G4double SimpsonsRule(G4double , G4double, G4int, G4double f[]);

    void Set_norm();
  void SetRandDimensions();
    void Set_nCaptureXe_Method (G4String );
    void Set_Xe_Component(G4String );
    void Set_Cu_isotope(G4int );
    void Set_Cu_Component(G4String );
  void Set_bb2n_CutOff(G4double );

  
  private:

    G4ParticleGun*           fParticleGun1;
    G4GeneralParticleSource* fParticleGun2;

    nEXOPrimaryGeneratorMessenger* fMessenger;

    G4String fGenerator;

    G4double fQ_value;
    G4int fFF_factor;
    G4double fK_spectral_max;
    G4double fD_spectral_max;
    G4double fNormalization;
  
    G4String fnCaptureXe_Method;
    G4String fXeComponent;
    G4int fCuIsotope;
    G4String fCuComponent;

    G4double fBb2nCutOffFraction;

  G4ThreeVector fRandPos;
  G4double fRandHalfLength;
  G4double fRandRadius;
};

#endif

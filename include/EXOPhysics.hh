#ifndef EXOPhysics_h
#define EXOPhysics_h 1

#include "globals.hh"
#include "G4VPhysicsConstructor.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EXOPhysics : public G4VPhysicsConstructor
{
  public:
    EXOPhysics();
   ~EXOPhysics();

  
 
  protected:
    void ConstructGeneral();
    void ConstructEM();
    void ConstructHad();
    void ConstructOp();

  private:

    G4bool enablePenelope;
    G4bool enableMuNuclear;
    G4bool enableHPNeutron;
    G4bool enableOptics;

    G4VPhysicsConstructor* fEMPhysicsList;
  public:
    void ConstructParticle();
    void ConstructProcess();
    void SetEnableOptics(G4bool setval){enableOptics=setval;}
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

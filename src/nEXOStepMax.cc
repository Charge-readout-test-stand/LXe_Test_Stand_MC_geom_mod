#include "G4Track.hh"
#include "G4VParticleChange.hh"

#include "nEXOStepMax.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

nEXOStepMax::nEXOStepMax(const G4String& aName)
  : G4VDiscreteProcess(aName), fMaxChargedStep(DBL_MAX)
{
   if (verboseLevel>0) {
     G4cout << GetProcessName() << " is created "<< G4endl;
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

nEXOStepMax::~nEXOStepMax() { }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

nEXOStepMax::nEXOStepMax(nEXOStepMax& right) : G4VDiscreteProcess(right) { }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool nEXOStepMax::IsApplicable(const G4ParticleDefinition& particle)
{
  return (particle.GetPDGCharge() != 0.);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOStepMax::SetStepMax(G4double step) { fMaxChargedStep = step ; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double nEXOStepMax::PostStepGetPhysicalInteractionLength(
                                              const G4Track&,
                                              G4double,
                                              G4ForceCondition* condition)
{
  // condition is set to "Not Forced"
  *condition = NotForced;

  G4double ProposedStep = DBL_MAX;

  if ( fMaxChargedStep > 0.) ProposedStep = fMaxChargedStep;

   return ProposedStep;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VParticleChange* nEXOStepMax::PostStepDoIt(const G4Track& aTrack,
                                         const G4Step&         )
{
   // do nothing
   aParticleChange.Initialize(aTrack);
   return &aParticleChange;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double nEXOStepMax::GetMeanFreePath(const G4Track&,G4double,G4ForceCondition*)
{
  return 0.;
}

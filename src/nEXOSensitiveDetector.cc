////////////////////////////////
// nEXOSensitiveDetector.cc //
///////////////////////////////

#include "nEXOSensitiveDetector.hh"

nEXOSensitiveDetector::nEXOSensitiveDetector(G4String SDname)
  : G4VSensitiveDetector(SDname)
{
  G4cout << "Creating SD with name: " << SDname << G4endl;
}

nEXOSensitiveDetector::~nEXOSensitiveDetector()
{
}

void nEXOSensitiveDetector::Initialize(G4HCofThisEvent * /*hce*/)
{
}

G4bool nEXOSensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *)
{
  G4StepPoint* preStepPoint = step->GetPreStepPoint();
  const G4VPhysicalVolume* preStepVolume = preStepPoint->GetPhysicalVolume();
  const G4String preVolName = preStepVolume->GetName();

  G4StepPoint* postStepPoint = step->GetPostStepPoint();   
  const G4VPhysicalVolume* postStepVolume = postStepPoint->GetPhysicalVolume();
  const G4String postVolName = postStepVolume->GetName();

  //G4cout << "Pre volume " << preVolName << " post volume " << postVolName << G4endl;

  return true;
}

void nEXOSensitiveDetector::EndOfEvent(G4HCofThisEvent * /*hce*/)
{
}

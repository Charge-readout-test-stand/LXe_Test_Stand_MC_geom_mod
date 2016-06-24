//////////////////////////////////////////
// nEXOSensitiveDetector.hh //
// Sensitive detector for nEXO to interact and extract hit information  //
//////////////////////////////////////////

#ifndef nEXOSensitiveDetector_h
#define nEXOSensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4TouchableHistory;
class G4HCofThisEvent;

class nEXOSensitiveDetector : public G4VSensitiveDetector
{
public:
  nEXOSensitiveDetector(G4String SDname);
  ~nEXOSensitiveDetector();

  void Initialize(G4HCofThisEvent *hce);
  G4bool ProcessHits(G4Step *step, G4TouchableHistory *ROhist);
  void EndOfEvent(G4HCofThisEvent *hce);

};

#endif

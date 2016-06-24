
#ifndef nEXODetectorMessenger_h
#define nEXODetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

class nEXODetectorConstruction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// Messenger class that defines commands for nEXODetectorConstruction.
///
/// It implements commands:
/// - /nEXO/det/stepMax value unit

class nEXODetectorMessenger: public G4UImessenger
{
  public:
    nEXODetectorMessenger(nEXODetectorConstruction* );
    virtual ~nEXODetectorMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    nEXODetectorConstruction*  fDetectorConstruction;

    G4UIdirectory*           fDetDirectory;

    G4UIcmdWithABool* fCheckOverlapCmd;
    G4UIcmdWithADoubleAndUnit* fStepMaxCmd;
};

#endif

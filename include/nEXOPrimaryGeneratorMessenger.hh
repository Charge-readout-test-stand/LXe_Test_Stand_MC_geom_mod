#ifndef nEXOPrimaryGeneratorMessenger_h
#define nEXOPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"

class nEXOPrimaryGeneratorAction;

class nEXOPrimaryGeneratorMessenger: public G4UImessenger
{
  public:

    nEXOPrimaryGeneratorMessenger(nEXOPrimaryGeneratorAction*);
    virtual ~nEXOPrimaryGeneratorMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  private:

    nEXOPrimaryGeneratorAction* fnEXOAction;

    G4UIdirectory*              fDirectory;

    G4UIcmdWithAString*         fGenCmd;
    G4UIcmdWithAString*         fnCaptureXeSimCmd;
    G4UIcmdWithAString*         fXeComponentCmd;
    G4UIcmdWithAnInteger*       fCuIsoCmd;
    G4UIcmdWithAString*         fCuComponentCmd;

  G4UIcmdWithADouble* fBb2nCutOffCmd;
};

#endif

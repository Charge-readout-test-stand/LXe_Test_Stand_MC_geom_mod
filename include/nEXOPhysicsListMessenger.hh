
#ifndef nEXOPhysicsListMessenger_h
#define nEXOPhysicsListMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "G4DecayTable.hh"
#include "G4VDecayChannel.hh"

class nEXOPhysicsList;

class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;

/// Provide control of the physics list and cut parameters

class nEXOPhysicsListMessenger : public G4UImessenger
{
  public:

    nEXOPhysicsListMessenger(nEXOPhysicsList* );
    virtual ~nEXOPhysicsListMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

  private:

    nEXOPhysicsList* fPhysicsList;

    G4UIdirectory* fDirectory;
    G4UIdirectory* fDecayDirectory;

    G4UIcmdWithADoubleAndUnit* fStepMaxCMD;

    G4UIcmdWithoutParameter* fPienuCMD;
    G4UIcmdWithoutParameter* fPimunuCMD;

};

#endif

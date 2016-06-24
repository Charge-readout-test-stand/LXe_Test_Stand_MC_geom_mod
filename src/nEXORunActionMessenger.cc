#include "nEXORunActionMessenger.hh"

#include "nEXORunAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

nEXORunActionMessenger::nEXORunActionMessenger(nEXORunAction* action) : fRunAction(action){}
nEXORunActionMessenger::~nEXORunActionMessenger(){}

void nEXORunActionMessenger::SetNewValue(G4UIcommand* /*command*/, G4String /*newValues*/){}

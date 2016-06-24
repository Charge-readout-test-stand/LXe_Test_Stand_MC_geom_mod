#ifndef nEXORunActionMessenger_h
#define nEXORunActionMessenger_h 1

#include "globals.hh"

#include "G4UImessenger.hh"

class nEXORunAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class nEXORunActionMessenger: public G4UImessenger
{
public:
  nEXORunActionMessenger(nEXORunAction*);
  ~nEXORunActionMessenger();

  void SetNewValue(G4UIcommand*, G4String);

private:
  nEXORunAction*         fRunAction;

};
#endif

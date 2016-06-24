#ifndef nEXOSteppingAction_h
#define nEXOSteppingAction_h 1

#include "globals.hh"

#include "G4UserSteppingAction.hh"

#include "nEXOAnalysis.hh"

class nEXOSteppingAction : public G4UserSteppingAction
{
public:
  nEXOSteppingAction();
  virtual ~nEXOSteppingAction();
  
  virtual void UserSteppingAction(const G4Step*);

private:
};

#endif

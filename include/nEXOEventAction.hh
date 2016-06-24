#ifndef nEXOEventAction_h
#define nEXOEventAction_h 1

#include "globals.hh"

#include "G4UserEventAction.hh"

#include "nEXOAnalysis.hh"

class nEXOEventAction : public G4UserEventAction
{
public:
  nEXOEventAction();
  virtual ~nEXOEventAction();
  
private:
  void BeginOfEventAction(const G4Event* );
  void EndOfEventAction(const G4Event* );

};

#endif

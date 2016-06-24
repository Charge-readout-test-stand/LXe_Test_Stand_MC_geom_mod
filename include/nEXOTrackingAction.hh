#ifndef nEXOTrackingAction_h
#define nEXOTrackingAction_h 1

#include "globals.hh"

#include "G4Track.hh"
#include "G4UserTrackingAction.hh"

#include "nEXOAnalysis.hh"

class nEXOTrackingAction : public G4UserTrackingAction
{
public:
  nEXOTrackingAction();
  virtual ~nEXOTrackingAction();
  
  virtual void PreUserTrackingAction(const G4Track* track);
  virtual void PostUserTrackingAction(const G4Track* track);  

private:
};

#endif


#ifndef nEXORunAction_h
#define nEXORunAction_h 1

#include "globals.hh"
#include "G4UserRunAction.hh"

#include "nEXOAnalysis.hh"

class nEXORunActionMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class G4Run;

class nEXORunAction : public G4UserRunAction
{
public:
  nEXORunAction();
  virtual ~nEXORunAction();

public:
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);

  nEXORunActionMessenger* fMessenger;
  nEXOAnalysisMessenger* fAnalysisMessenger;
};

#endif

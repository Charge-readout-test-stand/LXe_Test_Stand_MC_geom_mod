#ifndef nEXOActionInitialization_h
#define nEXOActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.
///

class nEXOActionInitialization : public G4VUserActionInitialization
{
  public:
    nEXOActionInitialization();
    virtual ~nEXOActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif

//////////////////////////////////////////
// nEXOOuterSphericalCryostatConstructor.hh //
// Class to construct the nEXO outer cryostats. //
//////////////////////////////////////////

#ifndef nEXOOuterSphericalCryostatConstructor_h
#define nEXOOuterSphericalCryostatConstructor_h 1

#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include "G4SystemOfUnits.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "nEXODetectorConstruction.hh"
#include "nEXOComponentConstructor.hh"
#include "nEXOSphericalCryostatComponentConstructor.hh"

/// Construct the outer cryostat component from nEXOSphericalCryostatComponentConstructor
class nEXOOuterSphericalCryostatConstructor : public nEXOSphericalCryostatComponentConstructor
{
public:
  nEXOOuterSphericalCryostatConstructor(G4String name, nEXODetectorConstruction* construct) : nEXOSphericalCryostatComponentConstructor(name,construct) {Init();};
  nEXOOuterSphericalCryostatConstructor(G4String name, nEXOConstructor* construct) : nEXOSphericalCryostatComponentConstructor(name,construct) {Init();};
  virtual ~nEXOOuterSphericalCryostatConstructor(){;};

  virtual G4LogicalVolume* GetPiece(void);
  virtual G4LogicalVolume* GetFillingPiece(void);
  virtual G4LogicalVolume* GetSupportPiece(void);  
  
private:
  void Init(void);   
  
};

class nEXOOuterSphericalCryostatMessenger : public nEXOSphericalCryostatComponentMessenger
{
private:
  nEXOOuterSphericalCryostatConstructor *fConstructor;

public:
  nEXOOuterSphericalCryostatMessenger(nEXOOuterSphericalCryostatConstructor* construct)
    : nEXOSphericalCryostatComponentMessenger(construct, "Control the geometry of the outer cryostat."),
      fConstructor(construct){
  };

  ~nEXOOuterSphericalCryostatMessenger(){
  };
  
  void SetNewValue(G4UIcommand* cmd, G4String val){
    nEXOSphericalCryostatComponentMessenger::SetNewValue(cmd,val);
  };

};

#endif


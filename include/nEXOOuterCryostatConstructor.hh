//////////////////////////////////////////
// nEXOOuterCryostatConstructor.hh //
// Class to construct the nEXO outer cryostats. //
//////////////////////////////////////////

#ifndef nEXOOuterCryostatConstructor_h
#define nEXOOuterCryostatConstructor_h 1

#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include "G4Tubs.hh"
#include "G4Ellipsoid.hh"
#include "G4Polycone.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "nEXODetectorConstruction.hh"
#include "nEXOComponentConstructor.hh"
#include "nEXOCryostatComponentConstructor.hh"

/// Construct the outer cryostat component from nEXOCryostatComponentConstructor
class nEXOOuterCryostatConstructor : public nEXOCryostatComponentConstructor
{
public:
  nEXOOuterCryostatConstructor(G4String name, nEXODetectorConstruction* construct) : nEXOCryostatComponentConstructor(name,construct) {Init();};
  nEXOOuterCryostatConstructor(G4String name, nEXOConstructor* construct) : nEXOCryostatComponentConstructor(name,construct) {Init();};
  virtual ~nEXOOuterCryostatConstructor(){;};

  virtual G4LogicalVolume* GetPiece(void);
  virtual G4LogicalVolume* GetFillingPiece(void);
  
private:
  void Init(void);   
  
};

class nEXOOuterCryostatMessenger : public nEXOCryostatComponentMessenger
{
private:
  nEXOOuterCryostatConstructor *fConstructor;

public:
  nEXOOuterCryostatMessenger(nEXOOuterCryostatConstructor* construct)
    : nEXOCryostatComponentMessenger(construct, "Control the geometry of the outer cryostat."),
      fConstructor(construct){
  };

  ~nEXOOuterCryostatMessenger(){
  };
  
  void SetNewValue(G4UIcommand* cmd, G4String val){
    nEXOCryostatComponentMessenger::SetNewValue(cmd,val);
  };

};

#endif


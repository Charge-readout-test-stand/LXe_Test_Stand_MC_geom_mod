//////////////////////////////////////////
// nEXOInnerCryostatConstructor.hh //
// Class to construct the nEXO inner cryostats. //
//////////////////////////////////////////

#ifndef nEXOInnerCryostatConstructor_h
#define nEXOInnerCryostatConstructor_h 1

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

/// Construct the inner cryostat component from nEXOCryostatComponentConstructor
class nEXOInnerCryostatConstructor : public nEXOCryostatComponentConstructor
{
public:
  nEXOInnerCryostatConstructor(G4String name, nEXODetectorConstruction* construct) : nEXOCryostatComponentConstructor(name,construct) {Init();};
  nEXOInnerCryostatConstructor(G4String name, nEXOConstructor* construct) : nEXOCryostatComponentConstructor(name,construct) {Init();};
  virtual ~nEXOInnerCryostatConstructor(){;};

  virtual G4LogicalVolume* GetPiece(void);
  virtual G4LogicalVolume* GetFillingPiece(void);
  
private:
  void Init(void);   
  
};

class nEXOInnerCryostatMessenger : public nEXOCryostatComponentMessenger
{
private:
  nEXOInnerCryostatConstructor *fConstructor;

public:
  nEXOInnerCryostatMessenger(nEXOInnerCryostatConstructor* construct)
    : nEXOCryostatComponentMessenger(construct, "Control the geometry of the inner cryostat."),
      fConstructor(construct){
  };

  ~nEXOInnerCryostatMessenger(){
  };
  
  void SetNewValue(G4UIcommand* cmd, G4String val){
    nEXOCryostatComponentMessenger::SetNewValue(cmd,val);
  };

};

#endif


//////////////////////////////////////////
// nEXOInnerSphericalCryostatConstructor.hh //
// Class to construct the nEXO inner cryostats. //
//////////////////////////////////////////

#ifndef nEXOInnerSphericalCryostatConstructor_h
#define nEXOInnerSphericalCryostatConstructor_h 1

#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include "G4SystemOfUnits.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "nEXODetectorConstruction.hh"
#include "nEXOComponentConstructor.hh"
#include "nEXOSphericalCryostatComponentConstructor.hh"

/// Construct the inner cryostat component from nEXOSphericalCryostatComponentConstructor
class nEXOInnerSphericalCryostatConstructor : public nEXOSphericalCryostatComponentConstructor
{
public:
  nEXOInnerSphericalCryostatConstructor(G4String name, nEXODetectorConstruction* construct) : nEXOSphericalCryostatComponentConstructor(name,construct) {Init();};
  nEXOInnerSphericalCryostatConstructor(G4String name, nEXOConstructor* construct) : nEXOSphericalCryostatComponentConstructor(name,construct) {Init();};
  virtual ~nEXOInnerSphericalCryostatConstructor(){;};

  virtual G4LogicalVolume* GetPiece(void);
  virtual G4LogicalVolume* GetFillingPiece(void);

  /// Set the offset between inner and outer cryostat centers
  void SetOffsetToOuter(G4double offset){fOffsetToOuter = offset;};
  /// Get the offset between inner and outer cryostat centers
  G4double GetOffsetToOuter(void){return fOffsetToOuter;};
  
private:
  void Init(void);

  G4double fOffsetToOuter;
  
};

class nEXOInnerSphericalCryostatMessenger : public nEXOSphericalCryostatComponentMessenger
{
private:
  nEXOInnerSphericalCryostatConstructor *fConstructor;

public:
  nEXOInnerSphericalCryostatMessenger(nEXOInnerSphericalCryostatConstructor* construct)
    : nEXOSphericalCryostatComponentMessenger(construct, "Control the geometry of the inner cryostat."),
      fConstructor(construct){
  };

  ~nEXOInnerSphericalCryostatMessenger(){
  };
  
  void SetNewValue(G4UIcommand* cmd, G4String val){
    nEXOSphericalCryostatComponentMessenger::SetNewValue(cmd,val);
  };

};

#endif


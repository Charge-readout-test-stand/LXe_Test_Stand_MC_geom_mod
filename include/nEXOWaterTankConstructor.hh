//////////////////////////////////////////
// nEXOWaterTankContructor.hh //
// Class to construct nEXO water tank.
//////////////////////////////////////////

#ifndef nEXOWaterTankContructor_h
#define nEXOWaterTankContructor_h 1

#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Sphere.hh"
#include "G4Torus.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include <G4ThreeVector.hh>

#include "nEXODetectorConstruction.hh"
#include "nEXOComponentConstructor.hh"
#include "nEXOConeComponentConstructor.hh"

/// Construct the water tank of the nEXO detector.
class nEXOWaterTankConstructor : public nEXOConeComponentConstructor
{
public:
  nEXOWaterTankConstructor(G4String name, nEXODetectorConstruction* construct) : nEXOConeComponentConstructor(name,construct){Init();};
  nEXOWaterTankConstructor(G4String name, nEXOConstructor* construct) : nEXOConeComponentConstructor(name,construct){Init();};
  virtual ~nEXOWaterTankConstructor();

  virtual G4LogicalVolume* GetPiece(void);
  virtual G4LogicalVolume* GetFillingPiece(void);

  /// Set the water tank side thickness
  void SetSideThickness(double thickness) {fSideThickness = thickness;}
  /// Set the water tank top thickness
  void SetTopThickness(double thickness) {fTopThickness = thickness;}
  /// Set the water tank bottom thickness
  void SetBottomThickness(double thickness) {fBottomThickness = thickness;}
  
  /// Get the water tank side thickness
  double GetSideThickness(void) {return fSideThickness;}
  /// Get the water tank top thickness
  double GetTopThickness(void) {return fTopThickness;}
  /// Get the water tank bottom thickness
  double GetBottomThickness(void) {return fBottomThickness;}

  /// Get the vertical offset caused by difference in top and bottom thickness
  double GetVerticalOffset(void) {return (GetBottomThickness() - GetTopThickness())/2;}
  
private:
  void Init(void);   

  /// The water tank side thickness
  G4double fSideThickness;
  /// The water tank top thickness
  G4double fTopThickness;
  /// The water tank bottom thickness
  G4double fBottomThickness;
  
};

class nEXOWaterTankMessenger : public nEXOConeComponentConstructorMessenger
{
private:
  nEXOWaterTankConstructor *fConstructor;

public:
  nEXOWaterTankMessenger(nEXOWaterTankConstructor* construct)
    : nEXOConeComponentConstructorMessenger(construct, "Control the geometry of the water tank."),
      fConstructor(construct){
  };

  ~nEXOWaterTankMessenger(){
  };
  
  void SetNewValue(G4UIcommand* cmd, G4String val){
      nEXOConeComponentConstructorMessenger::SetNewValue(cmd,val);
  };

};

#endif


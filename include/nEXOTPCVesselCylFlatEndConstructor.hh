#ifndef nEXOTPCVesselCylFlatEndConstructor_h
#define nEXOTPCVesselCylFlatEndConstructor_h 1

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
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

#include "TString.h"

#include "nEXOTPCVesselConstructor.hh"
#include "nEXOTPCVesselModelConstructor.hh"

/// Construct the TPC Vessel of the nEXO detector.
class nEXOTPCVesselCylFlatEndConstructor : public nEXOTPCVesselModelConstructor
{
public:
  nEXOTPCVesselCylFlatEndConstructor(G4String name, nEXODetectorConstruction* construct) : nEXOTPCVesselModelConstructor(name,construct){Init();};
  nEXOTPCVesselCylFlatEndConstructor(G4String name, nEXOConstructor* construct) : nEXOTPCVesselModelConstructor(name,construct){Init();};
  virtual ~nEXOTPCVesselCylFlatEndConstructor();

  /// Get the (logic) TPC vessel
  virtual G4LogicalVolume* GetPiece(void);
  /// Get the filling shape (usually for LXe)
  virtual G4VSolid* GetFillingShape(void);

  /// Set inner radius top
  void SetInnerRadius(G4double radius){fRadius = radius;}
  /// Set inner length
  void SetInnerLength(G4double length){fLength = length;}

  /// Get inner radius (available for placement on internal components)
  G4double GetInnerRadius(){return fRadius;}
  /// Get inner length (available for placement on internal components)
  G4double GetInnerLength(){return fLength;} 
  /// Get inner radius top (available for placement on internal components)
  double GetInnerTopRadius(){return GetInnerRadius();}
  /// Get inner radius bottom (available for placement on internal components)
  double GetInnerBottomRadius(){return GetInnerRadius();}
  
  /// Set the TPC vessel thickness
  void SetThickness(double thickness) {fThickness = thickness;}
  /// Set the TPC ends thickness
  void SetEndsThickness(double thickness) {fEndsThickness = thickness;}
  
  /// Get the TPC vessel thickness
  double GetThickness(void) {return fThickness;}
  /// Get the TPC ends thickness
  double GetEndsThickness(void) {return fEndsThickness;}
  
private:
  void Init(void);

  /// The TPC vessel inner length
  G4double fLength;
  /// The TPC vessel inner radius
  G4double fRadius;
  /// The TPC vessel thickness
  G4double fThickness;
  /// The TPC ends thickness
  G4double fEndsThickness;  
};

class nEXOTPCVesselCylFlatEndMessenger : public nEXOComponentConstructorMessenger
{
private:
  nEXOTPCVesselCylFlatEndConstructor *fConstructor;

public:
  nEXOTPCVesselCylFlatEndMessenger(nEXOTPCVesselCylFlatEndConstructor* construct)
    : nEXOComponentConstructorMessenger(construct, Form("Control the geometry of the TPC vessel model: %s.", construct->GetLocalName().data()) ),
      fConstructor(construct){
  };

  ~nEXOTPCVesselCylFlatEndMessenger(){
  };
  
  void SetNewValue(G4UIcommand* cmd, G4String val){
    nEXOComponentConstructorMessenger::SetNewValue(cmd,val);
  };

};

#endif


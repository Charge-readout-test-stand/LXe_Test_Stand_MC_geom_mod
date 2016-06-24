#ifndef nEXOTPCVesselModelConstructor_h
#define nEXOTPCVesselModelConstructor_h 1

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

#include "nEXOComponentConstructor.hh"

/// Base constructor of any TPC vessel model of the nEXO detector.
class nEXOTPCVesselModelConstructor : public nEXOComponentConstructor
{
public:
  nEXOTPCVesselModelConstructor(G4String name, nEXODetectorConstruction* construct) : nEXOComponentConstructor(name,construct){Init();}
  nEXOTPCVesselModelConstructor(G4String name, nEXOConstructor* construct) : nEXOComponentConstructor(name,construct){Init();}
  virtual ~nEXOTPCVesselModelConstructor(){;}

  /// Get the (logic) TPC vessel - must be redefined in derived classes (TPC models)
  virtual G4LogicalVolume* GetPiece(void){return 0;}
  /// Get the filling shape (usually for LXe) - must be redefined in derived classes (TPC models)
  virtual G4VSolid* GetFillingShape(void){return 0;}

  /// Get inner length (available for placement on internal components)
  virtual double GetInnerLength(){return 0;}
  /// Get inner radius top (available for placement on internal components)
  virtual double GetInnerTopRadius(){return 0;}
  /// Get inner radius bottom (available for placement on internal components)
  virtual double GetInnerBottomRadius(){return 0;}
  
private:
  void Init(void){;}
};

#endif



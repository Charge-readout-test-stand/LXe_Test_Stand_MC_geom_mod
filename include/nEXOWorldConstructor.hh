//////////////////////////////////////////
// nEXOWorldConstructor.hh //
// Constructs the nEXO world geometry. //
//////////////////////////////////////////

#ifndef nEXOWorldConstructor_h
#define nEXOWorldConstructor_h 1

#include <G4LogicalVolume.hh>
#include <G4SystemOfUnits.hh>
#include "G4Box.hh"
#include "G4VisAttributes.hh"

#include "nEXOConstructor.hh"
#include "nEXOTPCVesselConstructor.hh"
#include "nEXOTPCExternalsConstructor.hh"
#include "nEXOTPCInternalsConstructor.hh"
#include "nEXOSimplePhysVolManager.hh"

class nEXOWorldConstructor : public nEXOConstructor
{
public:
  nEXOWorldConstructor(G4String name, nEXODetectorConstruction* construct) : nEXOConstructor(name,construct){Init();};
  virtual ~nEXOWorldConstructor();

  /// Construct and return a G4 volume for the object.
  virtual G4LogicalVolume* GetPiece(void);

  /// Set the width of the world.
  void SetWidth(double width) {fWidth = width;};

  /// Set the height of the world.
  void SetHeight(double height) {fHeight = height;};

  /// Set the length of the world.
  void SetLength(double length) {fLength = length;};

  /// Get the width of the world.
  double GetWidth(void) {return fWidth;};

  /// Get the height of the world.
  double GetHeight(void) {return fHeight;};

  /// Get the length of the world.
  double GetLength(void) {return fLength;};


private:
  void Init(void);
  
  /// The width of the world to be simulated
  double fWidth;
  
  /// The heigth of the world to be simulated
  double fHeight;
  
  /// The length of the world to be simulated
  double fLength;

  
};
#endif

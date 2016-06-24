//////////////////////////////////////////
// nEXOConeComponentConstructor.hh //
// Base class to construct nEXO components with cone shape  //
//////////////////////////////////////////

#ifndef nEXOConeComponentConstructor_h
#define nEXOConeComponentConstructor_h 1

#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include "G4Cons.hh"

#include "nEXODetectorConstruction.hh"
#include "nEXOComponentConstructor.hh"

/// Construct a cone component to be used by a nEXOModuleConstructor to build component
class nEXOConeComponentConstructor : public nEXOComponentConstructor
{
public:
  nEXOConeComponentConstructor(G4String name, nEXODetectorConstruction* construct) : nEXOComponentConstructor(name,construct) {;};
  nEXOConeComponentConstructor(G4String name, nEXOConstructor* construct) : nEXOComponentConstructor(name,construct) {;};
  virtual ~nEXOConeComponentConstructor(){;};

  virtual G4LogicalVolume* GetPiece(void) = 0;
  
  /// Set the radius1 of the module.
  void SetRadius1(double radius) {fRadius1 = radius;}
  /// Get the radius1 of the module.
  double GetRadius1(void) {return fRadius1;}
  /// Set the radius2 of the module.
  void SetRadius2(double radius) {fRadius2 = radius;}
  /// Get the radius1 of the module.
  double GetRadius2(void) {return fRadius2;}

  /// Set the length of the module.
  void SetLength(double length) {fLength = length;}
  /// Get the length of the module.
  double GetLength(void) {return fLength;}

protected:
  /// The radius of the module at -fDz.
  double fRadius1;
  /// The radius of the module at +fDz.
  double fRadius2;
  /// The length of the module.
  double fLength;
};

class nEXOConeComponentConstructorMessenger : public nEXOComponentConstructorMessenger
{
private:
  nEXOConeComponentConstructor* fConstructor;

public:
  nEXOConeComponentConstructorMessenger(nEXOConeComponentConstructor* construct, const char* guide)
    : nEXOComponentConstructorMessenger(construct,guide),
      fConstructor(construct){;};

  virtual ~nEXOConeComponentConstructorMessenger(){;};

  void SetNewValue(G4UIcommand *cmd, G4String val){
    nEXOComponentConstructorMessenger::SetNewValue(cmd,val);
  }

};
  
#endif

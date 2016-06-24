//////////////////////////////////////////
// nEXOModuleConstructor.hh //
// Base class to construct nEXO modules from components  //
//////////////////////////////////////////

#ifndef nEXOModuleConstructor_h
#define nEXOModuleConstructor_h 1

#include <G4LogicalVolume.hh>

#include "nEXOConstructor.hh"
#include "nEXOComponentConstructor.hh"

/// Construct a module for the nEXO_MC from components
/// Currently, nEXO_MC has two modules: TPC internals and TPC externals

class nEXOModuleConstructor : public nEXOConstructor
{
public:
  nEXOModuleConstructor(G4String name, nEXODetectorConstruction* construct) : nEXOConstructor(name,construct) {Init();};
  nEXOModuleConstructor(G4String name, nEXOConstructor* construct) : nEXOConstructor(name,construct) {Init();};

  virtual ~nEXOModuleConstructor();//{;};

  /// Construct and return a G4 volume for the object.
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
  
private:
  /// The radius of the module at -fDz.
  double fRadius1;
  /// The radius of the module at +fDz.
  double fRadius2;
  /// The length of the module.
  double fLength;

  void Init(void){;};
};

class nEXOModuleConstructorMessenger : public nEXOConstructorMessenger
{
private:
  nEXOModuleConstructor* fConstructor;
  
public:
  nEXOModuleConstructorMessenger(nEXOModuleConstructor* construct, const char* guide = NULL)
    : nEXOConstructorMessenger(construct,guide),
      fConstructor(construct){;};

  virtual ~nEXOModuleConstructorMessenger(){;};

  void SetNewValue(G4UIcommand *cmd, G4String val){
    nEXOConstructorMessenger::SetNewValue(cmd,val);
  };

};

#endif

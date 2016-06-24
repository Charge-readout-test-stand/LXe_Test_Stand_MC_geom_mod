//////////////////////////////////////////
// nEXOSphericalCryostatComponentConstructor.hh //
// Base class to construct nEXO cryostats:
//  cylinder with one partial sphere at each end. //
//////////////////////////////////////////

#ifndef nEXOSphericalCryostatComponentConstructor_h
#define nEXOSphericalCryostatComponentConstructor_h 1

#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include "G4Sphere.hh"
#include "G4Cons.hh"
#include "G4UIcmdWithAString.hh"

#include "nEXODetectorConstruction.hh"
#include "nEXOComponentConstructor.hh"

/// Construct a cryostat component to be used by a nEXOModuleConstructor to build component
class nEXOSphericalCryostatComponentConstructor : public nEXOComponentConstructor
{
public:
  nEXOSphericalCryostatComponentConstructor(G4String name, nEXODetectorConstruction* construct) : nEXOComponentConstructor(name,construct) {;};
  nEXOSphericalCryostatComponentConstructor(G4String name, nEXOConstructor* construct) : nEXOComponentConstructor(name,construct) {;};
  virtual ~nEXOSphericalCryostatComponentConstructor(){;};

  virtual G4LogicalVolume* GetPiece(void) = 0;
  virtual G4LogicalVolume* GetFillingPiece(void) = 0;

  /// Set the cryostat thickness
  void SetThickness(double thickness) {fThickness = thickness;}
  /// Set the inner radius of the cryostat
  void SetRadius(double radius) {fRadius = radius;}
  
  /// Get the cryostat thickness
  double GetThickness(void) {return fThickness;}
  /// Get the cryostat inner radius
  double GetRadius(void) {return fRadius;}

  /// Get the filling material name
  G4String GetFillingMaterialName(void) {return fFillingMaterialName;}
  
private:

  /// The cryostat thickness
  G4double fThickness;
  /// The cryostat inner radius
  G4double fRadius;
  
  /// Filling material name
  G4String fFillingMaterialName;
  
};
  
class nEXOSphericalCryostatComponentMessenger : public nEXOComponentConstructorMessenger
{
private:
  nEXOSphericalCryostatComponentConstructor* fConstructor;

  G4UIcmdWithADoubleAndUnit* fRadiusCmd;
  G4UIcmdWithADoubleAndUnit* fThicknessCmd;

public:
  nEXOSphericalCryostatComponentMessenger(nEXOSphericalCryostatComponentConstructor* construct, const char* guide)
    : nEXOComponentConstructorMessenger(construct,guide),
      fConstructor(construct)
    {
      fRadiusCmd = new G4UIcmdWithADoubleAndUnit(CommandName("setRadius"),this);
      fRadiusCmd->SetGuidance("Set the radius of the spherical cryostat");
      fRadiusCmd->SetParameterName("setRadius",false);
      fRadiusCmd->SetUnitCategory("Length");
      fRadiusCmd->AvailableForStates(G4State_PreInit);

      fThicknessCmd = new G4UIcmdWithADoubleAndUnit(CommandName("setThickness"),this);
      fThicknessCmd->SetGuidance("Set the thickness of the spherical cryostat");
      fThicknessCmd->SetParameterName("setThickness",false);
      fThicknessCmd->SetUnitCategory("Length");
      fThicknessCmd->AvailableForStates(G4State_PreInit);
    };

  virtual ~nEXOSphericalCryostatComponentMessenger(){;};

  void SetNewValue(G4UIcommand *cmd, G4String val){
    if( cmd == fRadiusCmd)
    {
      fConstructor->SetRadius(fRadiusCmd->GetNewDoubleValue(val));
    }
    else if( cmd == fThicknessCmd )
    {
      fConstructor->SetThickness(fThicknessCmd->GetNewDoubleValue(val));
    }   
    else
    {
      nEXOComponentConstructorMessenger::SetNewValue(cmd,val);
    }          
  }

};
  
#endif

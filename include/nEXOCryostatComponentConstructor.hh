//////////////////////////////////////////
// nEXOCryostatComponentConstructor.hh //
// Base class to construct nEXO cryostats:
//  cylinder with one partial sphere at each end. //
//////////////////////////////////////////

#ifndef nEXOCryostatComponentConstructor_h
#define nEXOCryostatComponentConstructor_h 1

#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include "G4Tubs.hh"
#include "G4Sphere.hh"

#include "nEXODetectorConstruction.hh"
#include "nEXOComponentConstructor.hh"

/// Construct a cryostat component to be used by a nEXOModuleConstructor to build component
class nEXOCryostatComponentConstructor : public nEXOComponentConstructor
{
public:
  nEXOCryostatComponentConstructor(G4String name, nEXODetectorConstruction* construct) : nEXOComponentConstructor(name,construct) {;};
  nEXOCryostatComponentConstructor(G4String name, nEXOConstructor* construct) : nEXOComponentConstructor(name,construct) {;};
  virtual ~nEXOCryostatComponentConstructor(){;};

  virtual G4LogicalVolume* GetPiece(void) = 0;
  virtual G4LogicalVolume* GetFillingPiece(void) = 0;

  /// Set the cryostat thickness
  void SetThickness(double thickness) {fThickness = thickness;}
  /// Set the length of the middle of the cryostat
  void SetLength(double length) {fLength = length;}
  /// Set the radius of the middle of the cryostat
  void SetRadius(double radius) {fRadius = radius;}
  /// Set the cryostat ends radius
  void SetEndsRadius(double radius) {fEndsRadius = radius;}

  /// Get the cryostat thickness
  double GetThickness(void) {return fThickness;}
  /// Get the length of the middle of the cryostat
  double GetLength(void) {return fLength;}
  /// Get the cryostat radius
  double GetRadius(void) {return fRadius;}
  /// Get the cryostat ends radius
  double GetEndsRadius(void) {return fEndsRadius;}
  /// Get the max length of the cryostat
  double GetMaxLength(void) {return  GetLength() + 2*(GetEndsRadius() - sqrt(GetEndsRadius()*GetEndsRadius() - GetRadius()*GetRadius()));}


  /// Get the filling material name
  G4String GetFillingMaterialName(void) {return fFillingMaterialName;}
  
private:

  /// The cryostat thickness
  G4double fThickness;
  /// The cryostat length
  G4double fLength;
  /// The cryostat inner radius
  G4double fRadius;
  /// The cryostat ends radius
  G4double fEndsRadius;

  /// Filling material name
  G4String fFillingMaterialName;
  
};
  
class nEXOCryostatComponentMessenger : public nEXOComponentConstructorMessenger
{
private:
  nEXOCryostatComponentConstructor* fConstructor;

  G4UIcmdWithADoubleAndUnit* fRadiusCmd;
  G4UIcmdWithADoubleAndUnit* fThicknessCmd;
  G4UIcmdWithADoubleAndUnit* fLengthCmd;
  G4UIcmdWithADoubleAndUnit* fEndsRadiusCmd;

public:
  nEXOCryostatComponentMessenger(nEXOCryostatComponentConstructor* construct, const char* guide)
    : nEXOComponentConstructorMessenger(construct,guide),
      fConstructor(construct)
    {
      fRadiusCmd = new G4UIcmdWithADoubleAndUnit(CommandName("setRadius"),this);
      fRadiusCmd->SetGuidance("Set the radius of the round caps cryostat");
      fRadiusCmd->SetParameterName("setRadius",false);
      fRadiusCmd->SetUnitCategory("Length");
      fRadiusCmd->AvailableForStates(G4State_PreInit);

      fEndsRadiusCmd = new G4UIcmdWithADoubleAndUnit(CommandName("setEndsRadius"),this);
      fEndsRadiusCmd->SetGuidance("Set the cap radius of the round caps cryostat");
      fEndsRadiusCmd->SetParameterName("setEndsRadius",false);
      fEndsRadiusCmd->SetUnitCategory("Length");
      fEndsRadiusCmd->AvailableForStates(G4State_PreInit);

      fThicknessCmd = new G4UIcmdWithADoubleAndUnit(CommandName("setThickness"),this);
      fThicknessCmd->SetGuidance("Set the thickness of the round caps cryostat");
      fThicknessCmd->SetParameterName("setThickness",false);
      fThicknessCmd->SetUnitCategory("Length");
      fThicknessCmd->AvailableForStates(G4State_PreInit);

      fLengthCmd = new G4UIcmdWithADoubleAndUnit(CommandName("setLength"),this);
      fLengthCmd->SetGuidance("Set the length of the round caps cryostat");
      fLengthCmd->SetParameterName("setLength",false);
      fLengthCmd->SetUnitCategory("Length");
      fLengthCmd->AvailableForStates(G4State_PreInit);
    };

  virtual ~nEXOCryostatComponentMessenger(){;};

  void SetNewValue(G4UIcommand *cmd, G4String val){
    if( cmd == fRadiusCmd)
    {
      fConstructor->SetRadius(fRadiusCmd->GetNewDoubleValue(val));
    }
    else if( cmd == fEndsRadiusCmd)
    {
      fConstructor->SetEndsRadius(fEndsRadiusCmd->GetNewDoubleValue(val));
    }
    else if( cmd == fThicknessCmd )
    {
      fConstructor->SetThickness(fThicknessCmd->GetNewDoubleValue(val));
    }   
    else if( cmd == fLengthCmd )
    {
      fConstructor->SetLength(fLengthCmd->GetNewDoubleValue(val));
    }
    else
    {
      nEXOComponentConstructorMessenger::SetNewValue(cmd,val);
    }
  }

};
  
#endif

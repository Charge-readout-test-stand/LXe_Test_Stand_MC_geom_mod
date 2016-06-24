//////////////////////////////////////////
// nEXOComponentConstructor.hh //
// Base class to construct nEXO components for nEXOModuleConstructor  //
//////////////////////////////////////////

#ifndef nEXOComponentConstructor_h
#define nEXOComponentConstructor_h 1

#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4VSensitiveDetector.hh>
#include "G4UIcmdWithAString.hh"

#include "nEXOConstructor.hh"

/// Construct a component to be used by a nEXOModuleConstructor to build component
class nEXOComponentConstructor : public nEXOConstructor
{
public:
  nEXOComponentConstructor(G4String name, nEXODetectorConstruction* construct) : nEXOConstructor(name,construct), fSensitiveDetector(NULL) {;};
  nEXOComponentConstructor(G4String name, nEXOConstructor* construct) : nEXOConstructor(name,construct), fSensitiveDetector(NULL) {;};
  virtual ~nEXOComponentConstructor(){;};

  /// Set the sensitive detector for this component
  virtual void SetSensitiveDetector(G4VSensitiveDetector* sd){fSensitiveDetector = sd;}
  /// Get the sensitive detector for this component
  virtual G4VSensitiveDetector* GetSensitiveDetector(void){return fSensitiveDetector;}

  /// Set the component material
  void SetMaterial(G4String material) {fComponentMaterialName = material;}
  /// Get the component material
  G4String GetMaterial(void) {return fComponentMaterialName;}

protected:

  /// The sensitive detector for this component
  G4VSensitiveDetector* fSensitiveDetector;

private:

  /// Component material name
  G4String fComponentMaterialName;

};

class nEXOComponentConstructorMessenger : public nEXOConstructorMessenger
{
private:
  nEXOComponentConstructor* fConstructor;

  G4UIcmdWithAString* fComponentMaterialNameCmd;

public:
  nEXOComponentConstructorMessenger(nEXOComponentConstructor* construct, const char* guide)
    : nEXOConstructorMessenger(construct,guide),
      fConstructor(construct)
    {
      fComponentMaterialNameCmd = new G4UIcmdWithAString(CommandName("setMaterial"),this);
      fComponentMaterialNameCmd->SetGuidance("Set the name of the material used to build the cryostat.");
      fComponentMaterialNameCmd->SetParameterName("setMaterial",false);
      fComponentMaterialNameCmd->AvailableForStates(G4State_PreInit);
    };

  virtual ~nEXOComponentConstructorMessenger(){;};

  void SetNewValue(G4UIcommand *cmd, G4String val){
    if( cmd == fComponentMaterialNameCmd)
    {
      fConstructor->SetMaterial(val);
    }
    else
    {
      nEXOConstructorMessenger::SetNewValue(cmd,val);
    }

  }

};
  
#endif

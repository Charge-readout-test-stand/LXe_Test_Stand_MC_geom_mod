#ifndef nEXOTPCVesselConstructor_h
#define nEXOTPCVesselConstructor_h 1

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

#include "nEXOConeComponentConstructor.hh"

/// Construct the TPC Vessel of the nEXO detector.
class nEXOTPCVesselConstructor : public nEXOConeComponentConstructor
{
public:
  nEXOTPCVesselConstructor(G4String name, nEXODetectorConstruction* construct) : nEXOConeComponentConstructor(name,construct){Init();};
  nEXOTPCVesselConstructor(G4String name, nEXOConstructor* construct) : nEXOConeComponentConstructor(name,construct){Init();};
  virtual ~nEXOTPCVesselConstructor();

  virtual G4LogicalVolume* GetPiece(void);

  /// Set the TPC vessel thickness
  void SetThickness(double thickness) {fThickness = thickness;}
  /// Set the TPC ends thickness
  void SetEndsThickness(double thickness) {fEndsThickness = thickness;}
  /// Set the TPC geometry option
  void SetTPCVesselOption(int option) {fOption = option;}
  
  /// Get the TPC vessel thickness
  double GetThickness(void) {return fThickness;}
  /// Get the TPC ends thickness
  double GetEndsThickness(void) {return fEndsThickness;}
  /// Get the TPC geometry option
  int GetTPCVesselOption(void) {return fOption;}

  /// Set the lower inner radius of the TPC vessl
  void SetTPCLowerRadius(G4double radius) {SetRadius1(radius+GetThickness());/*fTPCLowerRadius = r;*/};

  /// Set the optical surface of the TPC vessel
  void SetOpticalSurface(G4String volName);
  
  //-----------------------------------Test Stand----------------------------------------
  void SetTPCRadiusTestStand(G4double radius) {fTPCRadiusTestStand = radius;}
  double GetTPCRadiusTestStand(void) {return fTPCRadiusTestStand;}

  void SetTPCHeightTestStand(G4double height) {fTPCHeightTestStand = height;}
  double GetTPCHeightTestStand(void) {return fTPCHeightTestStand;}

  void SetTPCThicknessTestStand(G4double thickness) {fTPCThicknessTestStand = thickness;}
  double GetTPCThicknessTestStand(void) {return fTPCThicknessTestStand;}
  //------------------------------------------------------------------------------------

private:
  void Init(void);   

  /// The TPC vessel thickness
  G4double fThickness;
  /// The TPC ends thickness
  G4double fEndsThickness;
  /// The option of TPC vessel geometry
  G4int fOption;

  //-----------------------------------Test Stand----------------------------------------
  G4double fTPCRadiusTestStand;
  G4double fTPCHeightTestStand;
  G4double fTPCThicknessTestStand;
  //-------------------------------------------------------------------------------------
  
};

class nEXOTPCVesselMessenger : public nEXOConeComponentConstructorMessenger
{
private:
  nEXOTPCVesselConstructor *fConstructor;

  G4UIcmdWithADoubleAndUnit* fTPCLowerRadiusCmd;
  G4UIcmdWithAnInteger* fTPCVesselOptionCmd;

  //-----------------------------------Test Stand----------------------------------------
  G4UIcmdWithADoubleAndUnit* fTPCRadiusTestStandCmd;
  G4UIcmdWithADoubleAndUnit* fTPCHeightTestStandCmd;
  G4UIcmdWithADoubleAndUnit* fTPCThicknessTestStandCmd;
  //-------------------------------------------------------------------------------------

public:
  nEXOTPCVesselMessenger(nEXOTPCVesselConstructor* construct)
    : nEXOConeComponentConstructorMessenger(construct, "Control the geometry of the TPC vessel."),
      fConstructor(construct){
    
    fTPCLowerRadiusCmd = new G4UIcmdWithADoubleAndUnit(CommandName("tpcLowerRadius"),this);
    fTPCLowerRadiusCmd->SetGuidance("Define TPC lower radius");
    fTPCLowerRadiusCmd->SetParameterName("tpcLowerRadius",false);
    fTPCLowerRadiusCmd->SetUnitCategory("Length");
    fTPCLowerRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fTPCVesselOptionCmd = new G4UIcmdWithAnInteger(CommandName("tpcVesselOption"),this);
    fTPCVesselOptionCmd->SetGuidance("Define tpc vessel geometry option");
    fTPCVesselOptionCmd->SetParameterName("tpcVesselOption",false);
    fTPCVesselOptionCmd->SetDefaultValue(0);
    fTPCVesselOptionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    //-----------------------------------Test Stand----------------------------------------
    fTPCRadiusTestStandCmd = new G4UIcmdWithADoubleAndUnit(CommandName("tpcRadiusTestStand"),this);
    fTPCRadiusTestStandCmd->SetGuidance("Define TPC Radius for Test Stand");
    fTPCRadiusTestStandCmd->SetParameterName("tpcRadiusTestStand",false);
    fTPCRadiusTestStandCmd->SetUnitCategory("Length");
    fTPCRadiusTestStandCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fTPCHeightTestStandCmd = new G4UIcmdWithADoubleAndUnit(CommandName("tpcHeightTestStand"),this);
    fTPCHeightTestStandCmd->SetGuidance("Define TPC Height for Test Stand");
    fTPCHeightTestStandCmd->SetParameterName("tpcHeightTestStand",false);
    fTPCHeightTestStandCmd->SetUnitCategory("Length");
    fTPCHeightTestStandCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fTPCThicknessTestStandCmd  = new G4UIcmdWithADoubleAndUnit(CommandName("tpcThicknessTestStand"),this);
    fTPCThicknessTestStandCmd->SetGuidance("Define TPC Thickness for Test Stand");
    fTPCThicknessTestStandCmd->SetParameterName("tpcHeightThicknessStand",false);
    fTPCThicknessTestStandCmd->SetUnitCategory("Length");
    fTPCThicknessTestStandCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    //-------------------------------------------------------------------------------------

  };

  ~nEXOTPCVesselMessenger(){
    delete fTPCLowerRadiusCmd;
    delete fTPCVesselOptionCmd;
    delete fTPCRadiusTestStandCmd;
  };
  
  void SetNewValue(G4UIcommand* cmd, G4String val){
    if( cmd == fTPCLowerRadiusCmd ) {
      fConstructor->SetTPCLowerRadius(fTPCLowerRadiusCmd->GetNewDoubleValue(val));
    }   
    else if( cmd == fTPCVesselOptionCmd ) {
      fConstructor->SetTPCVesselOption(fTPCVesselOptionCmd->GetNewIntValue(val));
    }
    else if(cmd == fTPCRadiusTestStandCmd){
        //-----------------------------------Test Stand----------------------------------------
        fConstructor->SetTPCRadiusTestStand(fTPCRadiusTestStandCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fTPCHeightTestStandCmd){
        //-----------------------------------Test Stand----------------------------------------
        fConstructor->SetTPCHeightTestStand(fTPCHeightTestStandCmd->GetNewDoubleValue(val));
    }   
    else {
      nEXOConeComponentConstructorMessenger::SetNewValue(cmd,val);
    }
  };

};

#endif


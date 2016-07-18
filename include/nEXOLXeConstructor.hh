#ifndef nEXOLXeConstructor_h
#define nEXOLXeConstructor_h 1

#include "G4LogicalVolume.hh"
#include "G4VSensitiveDetector.hh"
#include "G4SystemOfUnits.hh"
#include "G4Cons.hh"

#include "nEXOConeComponentConstructor.hh"
#include "nEXOTPCVesselConstructor.hh"

/// Construct the region of active LXe of the nEXO detector.
class nEXOLXeConstructor : public nEXOConeComponentConstructor
{
public:
  nEXOLXeConstructor(G4String name, nEXODetectorConstruction* construct) : nEXOConeComponentConstructor(name,construct){Init();};
  nEXOLXeConstructor(G4String name, nEXOConstructor* construct) : nEXOConeComponentConstructor(name,construct){Init();};
  virtual ~nEXOLXeConstructor();

  virtual G4LogicalVolume* GetPiece(void);

  /// Set the dimensions for this LXe volume to fill a given TPC vessel
  void FillTPCVessel(nEXOTPCVesselConstructor& tpcVessel);

  /// Set only active LXe
  void SetAllActive();

  /// Set dimensions for active LXe
  void SetActiveLXeDimensions(double radius1, double radius2, double length);
  /// Set LXe geometry option
  void SetLXeOption(int option) {fOption = option;}

  /// Get active LXe radius1
  double GetActiveLXeRadius1(){return fActiveLXeRadius1;}
  /// Get active LXe radius2
  double GetActiveLXeRadius2(){return fActiveLXeRadius2;}
  /// Get active LXe length
  double GetActiveLXeLength(){return fActiveLXeLength;}
  
  //-----------------------------------Test Stand----------------------------------------
  void SetActiveHeightTestStand(G4double height) {fActiveHeightTestStand = height;}
  double GetActiveHeightTestStand(void) {return fActiveHeightTestStand;}
  
  void SetActiveLengthTestStand(G4double length) {fActiveLengthTestStand = length;}
  double GetActiveLengthTestStand(void) {return fActiveLengthTestStand;}

  void SetActiveWidthTestStand(G4double width) {fActiveWidthTestStand = width;}
  double GetActiveWidthTestStand(void) {return fActiveWidthTestStand;}
  //-------------------------------------------------------------------------------------

private:
  void Init(void);

  /// Flag only active LXe
  bool fOnlyActiveLXe;

  /// The radius of the active LXe at -fDz.
  double fActiveLXeRadius1;
  /// The radius of the active LXe at +fDz.
  double fActiveLXeRadius2;
  /// The length of the active LXe.
  double fActiveLXeLength;
  /// The LXe geometry option
  int fOption;

  //-----------------------------------Test Stand----------------------------------------
  G4double fActiveHeightTestStand;
  G4double fActiveLengthTestStand;
  G4double fActiveWidthTestStand;
  //-------------------------------------------------------------------------------------

};

class nEXOLXeMessenger : public nEXOConeComponentConstructorMessenger
{
private:
  nEXOLXeConstructor *fConstructor;

  //-----------------------------------Test Stand----------------------------------------
  G4UIcmdWithADoubleAndUnit* fActiveHeightTestStandCmd;
  G4UIcmdWithADoubleAndUnit* fActiveLengthTestStandCmd;
  G4UIcmdWithADoubleAndUnit* fActiveWidthTestStandCmd;
  //-------------------------------------------------------------------------------------

public:

  nEXOLXeMessenger(nEXOLXeConstructor* construct)
    : nEXOConeComponentConstructorMessenger(construct, "Control the LXe in the TPC vessel of the nEXO detector."),
      fConstructor(construct){
      //-----------------------------------Test Stand----------------------------------------
      fActiveHeightTestStandCmd = new G4UIcmdWithADoubleAndUnit(CommandName("ActiveHeightTestStand"),this);
      fActiveHeightTestStandCmd->SetGuidance("Define Height of the Test Stand Active Region (box)");
      fActiveHeightTestStandCmd->SetParameterName("ActiveHeightTestStand",false);
      fActiveHeightTestStandCmd->SetUnitCategory("Length");
      fActiveHeightTestStandCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
      fActiveWidthTestStandCmd = new G4UIcmdWithADoubleAndUnit(CommandName("ActiveWidthTestStand"),this);
      fActiveWidthTestStandCmd->SetGuidance("Define Width of the Test Stand Active Region (box)");
      fActiveWidthTestStandCmd->SetParameterName("ActiveWidthTestStand",false);
      fActiveWidthTestStandCmd->SetUnitCategory("Length");
      fActiveWidthTestStandCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

      fActiveLengthTestStandCmd = new G4UIcmdWithADoubleAndUnit(CommandName("ActiveLengthTestStand"),this);
      fActiveLengthTestStandCmd->SetGuidance("Define Length of the Test Stand Active Region (box)");
      fActiveLengthTestStandCmd->SetParameterName("ActiveLengthTestStand",false);
      fActiveLengthTestStandCmd->SetUnitCategory("Length");
      fActiveLengthTestStandCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
      //-------------------------------------------------------------------------------------
     };

  ~nEXOLXeMessenger(){
      delete fActiveHeightTestStandCmd;
      delete fActiveLengthTestStandCmd;
      delete fActiveWidthTestStandCmd;
  };
  
  void SetNewValue(G4UIcommand* cmd, G4String val){
      if( cmd == fActiveHeightTestStandCmd ) {
          fConstructor->SetActiveHeightTestStand(fActiveHeightTestStandCmd->GetNewDoubleValue(val));
      }
      else if( cmd == fActiveWidthTestStandCmd ) {
          fConstructor->SetActiveWidthTestStand(fActiveWidthTestStandCmd->GetNewDoubleValue(val));       
      }
      else if( cmd == fActiveLengthTestStandCmd ) {
          fConstructor->SetActiveLengthTestStand(fActiveLengthTestStandCmd->GetNewDoubleValue(val));
      }
      else {
          nEXOConeComponentConstructorMessenger::SetNewValue(cmd,val);
      }
   };

};

#endif

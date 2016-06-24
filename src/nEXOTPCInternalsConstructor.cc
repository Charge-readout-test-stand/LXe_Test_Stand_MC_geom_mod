#include "nEXOTPCInternalsConstructor.hh"


class nEXOTPCInternalsMessenger : public nEXOModuleConstructorMessenger
{
private:
  nEXOTPCInternalsConstructor* fConstructor;

  G4UIcmdWithADoubleAndUnit* fWWDistanceCmd;
  G4UIcmdWithADoubleAndUnit* fWWDistanceOfAnodeCmd;
  G4UIcmdWithADoubleAndUnit* fFieldRingLengthCmd;
  G4UIcmdWithADoubleAndUnit* fFieldRingThickCmd;
  G4UIcmdWithADoubleAndUnit* fRingSideDistanceCmd;
  G4UIcmdWithADoubleAndUnit* fRadialRingDistanceCmd;
  G4UIcmdWithADouble* fFieldRingRefCmd;
  G4UIcmdWithADouble* fAnodeRefCmd;
  G4UIcmdWithADouble* fCathodeRefCmd;
  G4UIcmdWithADouble* fTeflonRefCmd;
  G4UIcmdWithADouble* fBulgeRefCmd;
  G4UIcmdWithADoubleAndUnit* fBulgeRadiusCmd;
  G4UIcmdWithADouble* fTPCVesselRefCmd;
  G4UIcmdWithADouble* fActiveSiPMPadFracCmd;
  G4UIcmdWithAnInteger* fSiPMOptionCmd;
  G4UIcmdWithAnInteger* fAnodeOptionCmd;
  G4UIcmdWithAnInteger* fRingArrangeIDCmd;
  G4UIcmdWithAnInteger* fInternalsFlagCmd;
  G4UIcmdWithAnInteger* fUseWLSCmd;

  G4UIcmdWithADoubleAndUnit* fCathodeAnodeDistanceCmd;
  G4UIcmdWithADoubleAndUnit* fCathodeToBottomRingDistanceCmd;
  G4UIcmdWithADoubleAndUnit* fAnodeToTopRingDistanceCmd;
  G4UIcmdWithADoubleAndUnit* fAnodeRadiusCmd;
  G4UIcmdWithADoubleAndUnit* fAnodeThicknessCmd;
  G4UIcmdWithADoubleAndUnit* fAnodeZPositionCmd;
  G4UIcmdWithADoubleAndUnit* fCathodeRadiusCmd;
  G4UIcmdWithADoubleAndUnit* fCathodeThicknessCmd;
  G4UIcmdWithADoubleAndUnit* fCathodeZPositionCmd;
  G4UIcmdWithADoubleAndUnit* fSiPMTotalHeightCmd;
  G4UIcmdWithADoubleAndUnit* fSiPMPadHeightCmd;
  G4UIcmdWithADoubleAndUnit* fSiPMPadThicknessCmd;
  G4UIcmdWithADoubleAndUnit* fSiPMStandoffFromTPCCmd;
  G4UIcmdWithADoubleAndUnit* fChargeModuleLengthCmd;
  G4UIcmdWithADoubleAndUnit* fChargeModuleThicknessCmd;
  G4UIcmdWithADoubleAndUnit* fChargeModuleBackingThicknessCmd;
  G4UIcmdWithADoubleAndUnit* fChargePadDiagonalLengthCmd;
  G4UIcmdWithADoubleAndUnit* fChargePadThicknessCmd;
  G4UIcmdWithADoubleAndUnit* fChargePadLinkLengthCmd;
  G4UIcmdWithADoubleAndUnit* fChargePadLinkHeightCmd;

  G4UIcmdWithADoubleAndUnit* fSiPMActiveLayerThicknessCmd;
  G4UIcmdWithADoubleAndUnit* fSiPMActiveLayerDepthCmd;
  G4UIcmdWithADoubleAndUnit* fSiPMBaseLayerThicknessCmd;
  G4UIcmdWithADoubleAndUnit* fSiPMWLSThicknessCmd;

public:
  nEXOTPCInternalsMessenger(nEXOTPCInternalsConstructor* construct)
  : nEXOModuleConstructorMessenger(construct, "Control the geometry of the components inside the nEXO TPC vessel."),
    fConstructor(construct){
    
    fWWDistanceCmd = new G4UIcmdWithADoubleAndUnit(CommandName("wwDistance"),this);
    fWWDistanceCmd->SetGuidance("Define distance between wires");
    fWWDistanceCmd->SetParameterName("wwDistance",false);
    fWWDistanceCmd->SetUnitCategory("Length");
    fWWDistanceCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fWWDistanceOfAnodeCmd = new G4UIcmdWithADoubleAndUnit(CommandName("wwDistanceOfAnode"),this);
    fWWDistanceOfAnodeCmd->SetGuidance("Define distance between wires of Anode");
    fWWDistanceOfAnodeCmd->SetParameterName("wwDistanceOfAnode",false);
    fWWDistanceOfAnodeCmd->SetUnitCategory("Length");
    fWWDistanceOfAnodeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fFieldRingLengthCmd = new G4UIcmdWithADoubleAndUnit(CommandName("fieldRingLength"),this);
    fFieldRingLengthCmd->SetGuidance("Define field ring length");
    fFieldRingLengthCmd->SetParameterName("fieldRingLength",false);
    fFieldRingLengthCmd->SetUnitCategory("Length");
    fFieldRingLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fFieldRingThickCmd = new G4UIcmdWithADoubleAndUnit(CommandName("fieldRingThick"),this);
    fFieldRingThickCmd->SetGuidance("Define field ring thick");
    fFieldRingThickCmd->SetParameterName("fieldRingThick",false);
    fFieldRingThickCmd->SetUnitCategory("Length");
    fFieldRingThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fRingSideDistanceCmd = new G4UIcmdWithADoubleAndUnit(CommandName("ringSideDistance"),this);
    fRingSideDistanceCmd->SetGuidance("Define ring side distance");
    fRingSideDistanceCmd->SetParameterName("ringSideDistance",false);
    fRingSideDistanceCmd->SetUnitCategory("Length");
    fRingSideDistanceCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fRadialRingDistanceCmd = new G4UIcmdWithADoubleAndUnit(CommandName("radialRingDistance"),this);
    fRadialRingDistanceCmd->SetGuidance("Define radial ring distance");
    fRadialRingDistanceCmd->SetParameterName("radialRingDistance",false);
    fRadialRingDistanceCmd->SetUnitCategory("Length");
    fRadialRingDistanceCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
        
    fFieldRingRefCmd = new G4UIcmdWithADouble(CommandName("fieldRingRef"),this);
    fFieldRingRefCmd->SetGuidance("The reflectivity of field rings");
    fFieldRingRefCmd->SetParameterName("fieldRingRef",false);
    fFieldRingRefCmd->SetDefaultValue(1.0);
    fFieldRingRefCmd->AvailableForStates(G4State_PreInit);
    
    fAnodeRefCmd = new G4UIcmdWithADouble(CommandName("anodeRef"),this);
    fAnodeRefCmd->SetGuidance("The reflectivity of anode");
    fAnodeRefCmd->SetParameterName("anodeRef",false);
    fAnodeRefCmd->SetDefaultValue(0.8);
    fAnodeRefCmd->AvailableForStates(G4State_PreInit);
    
    fCathodeRefCmd = new G4UIcmdWithADouble(CommandName("cathodeRef"),this);
    fCathodeRefCmd->SetGuidance("The reflectivity of cathode");
    fCathodeRefCmd->SetParameterName("cathodeRef",false);
    fCathodeRefCmd->SetDefaultValue(0.8);
    fCathodeRefCmd->AvailableForStates(G4State_PreInit);

    fTeflonRefCmd = new G4UIcmdWithADouble(CommandName("teflonRef"),this);
    fTeflonRefCmd->SetGuidance("The reflectivity of teflon");
    fTeflonRefCmd->SetParameterName("teflonRef",false);
    fTeflonRefCmd->SetDefaultValue(0.95);
    fTeflonRefCmd->AvailableForStates(G4State_PreInit);
    
    fBulgeRefCmd = new G4UIcmdWithADouble(CommandName("bulgeRef"),this);
    fBulgeRefCmd->SetGuidance("The reflectivity of bulge");
    fBulgeRefCmd->SetParameterName("bulgeRef",false);
    fBulgeRefCmd->SetDefaultValue(0.8);
    fBulgeRefCmd->AvailableForStates(G4State_PreInit);

    fBulgeRadiusCmd = new G4UIcmdWithADoubleAndUnit(CommandName("bulgeRadius"),this);
    fBulgeRadiusCmd->SetGuidance("The radius of bulge");
    fBulgeRadiusCmd->SetParameterName("bulgeRadius",false);
    fBulgeRadiusCmd->SetUnitCategory("Length");
    fBulgeRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fTPCVesselRefCmd = new G4UIcmdWithADouble(CommandName("tpcVesselRef"),this);
    fTPCVesselRefCmd->SetGuidance("The reflectivity of TPC vessel");
    fTPCVesselRefCmd->SetParameterName("tpcVesselRef",false);
    fTPCVesselRefCmd->SetDefaultValue(0.1);
    fTPCVesselRefCmd->AvailableForStates(G4State_PreInit);
    
    fActiveSiPMPadFracCmd = new G4UIcmdWithADouble(CommandName("activeSiPMPadFrac"),this);
    fActiveSiPMPadFracCmd->SetGuidance("The fraction of active SiPM pad area");
    fActiveSiPMPadFracCmd->SetParameterName("activeSiPMPadFrac",false);
    fActiveSiPMPadFracCmd->SetDefaultValue(0.8);
    fActiveSiPMPadFracCmd->AvailableForStates(G4State_PreInit);
    
    fSiPMOptionCmd = new G4UIcmdWithAnInteger(CommandName("SiPMOption"),this);
    fSiPMOptionCmd->SetGuidance("0: barrel, 1: endcap");
    fSiPMOptionCmd->SetParameterName("SiPMOption",false);
    fSiPMOptionCmd->SetDefaultValue(0);
    fSiPMOptionCmd->AvailableForStates(G4State_PreInit);

    fAnodeOptionCmd = new G4UIcmdWithAnInteger(CommandName("AnodeOption"),this);
    fAnodeOptionCmd->SetGuidance("0: Simple disk, 1: Charge tiles on quartz backing");
    fAnodeOptionCmd->SetParameterName("AnodeOption",false);
    fAnodeOptionCmd->SetDefaultValue(0);
    fAnodeOptionCmd->AvailableForStates(G4State_PreInit);

    fRingArrangeIDCmd = new G4UIcmdWithAnInteger(CommandName("ringArrangeID"),this);
    fRingArrangeIDCmd->SetGuidance("The id of ring arrangement");
    fRingArrangeIDCmd->SetParameterName("ringArrangeID",false);
    fRingArrangeIDCmd->SetDefaultValue(0);
    fRingArrangeIDCmd->AvailableForStates(G4State_PreInit);

    fUseWLSCmd = new G4UIcmdWithAnInteger(CommandName("useWLS"),this);
    fUseWLSCmd->SetGuidance("if use WLS");
    fUseWLSCmd->SetParameterName("useWLS",false);
    fUseWLSCmd->SetDefaultValue(0);
    fUseWLSCmd->AvailableForStates(G4State_PreInit);

    fInternalsFlagCmd = new G4UIcmdWithAnInteger(CommandName("internalsFlag"),this);
    fInternalsFlagCmd->SetGuidance("Which TPC Internals are included");
    fInternalsFlagCmd->SetParameterName("internalsFlag",false);
    fInternalsFlagCmd->SetDefaultValue(2);
    fInternalsFlagCmd->AvailableForStates(G4State_PreInit);

    fCathodeAnodeDistanceCmd = new G4UIcmdWithADoubleAndUnit(CommandName("cathodeAnodeDistance"),this);
    fCathodeAnodeDistanceCmd->SetGuidance("Distance between cathode and anode");
    fCathodeAnodeDistanceCmd->SetParameterName("cathodeAnodeDistance",false);
    fCathodeAnodeDistanceCmd->SetUnitCategory("Length"); //default = 1259.*mm
    fCathodeAnodeDistanceCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fCathodeToBottomRingDistanceCmd = new G4UIcmdWithADoubleAndUnit(CommandName("cathodeToBottomRingDistance"),this);
    fCathodeToBottomRingDistanceCmd->SetGuidance("Distance between cathode and bottom field ring");
    fCathodeToBottomRingDistanceCmd->SetParameterName("cathodeToBottomRingDistance",false);
    fCathodeToBottomRingDistanceCmd->SetUnitCategory("Length"); //default = 18.16.*mm
    fCathodeToBottomRingDistanceCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fAnodeToTopRingDistanceCmd = new G4UIcmdWithADoubleAndUnit(CommandName("anodeToTopRingDistance"),this);
    fAnodeToTopRingDistanceCmd->SetGuidance("Distance between anode and top field ring");
    fAnodeToTopRingDistanceCmd->SetParameterName("anodeToTopRingDistance",false);
    fAnodeToTopRingDistanceCmd->SetUnitCategory("Length"); //default = 18.*mm
    fAnodeToTopRingDistanceCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fAnodeRadiusCmd = new G4UIcmdWithADoubleAndUnit(CommandName("anodeRadius"),this);
    fAnodeRadiusCmd->SetGuidance("Anode radius (only for simple disk anode)");
    fAnodeRadiusCmd->SetParameterName("anodeRadius",false);
    fAnodeRadiusCmd->SetUnitCategory("Length"); //default = 587.7*mm
    fAnodeRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fAnodeThicknessCmd = new G4UIcmdWithADoubleAndUnit(CommandName("anodeThickness"),this);
    fAnodeThicknessCmd->SetGuidance("Anode thickness (only for simple disk anode)");
    fAnodeThicknessCmd->SetParameterName("anodeThickness",false);
    fAnodeThicknessCmd->SetUnitCategory("Length"); //default = 3.*mm
    fAnodeThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fAnodeZPositionCmd = new G4UIcmdWithADoubleAndUnit(CommandName("anodeZPosition"),this);
    fAnodeZPositionCmd->SetGuidance("Anode z-position");
    fAnodeZPositionCmd->SetParameterName("anodeZPosition",false);
    fAnodeZPositionCmd->SetUnitCategory("Length"); //default = 635.5*mm
    fAnodeZPositionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fCathodeRadiusCmd = new G4UIcmdWithADoubleAndUnit(CommandName("cathodeRadius"),this);
    fCathodeRadiusCmd->SetGuidance("Cathode radius");
    fCathodeRadiusCmd->SetParameterName("cathodeRadius",false);
    fCathodeRadiusCmd->SetUnitCategory("Length"); //default = 597.7*mm
    fCathodeRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fCathodeThicknessCmd = new G4UIcmdWithADoubleAndUnit(CommandName("cathodeThickness"),this);
    fCathodeThicknessCmd->SetGuidance("Cathode thickness");
    fCathodeThicknessCmd->SetParameterName("cathodeThickness",false);
    fCathodeThicknessCmd->SetUnitCategory("Length"); //default = 0.25*mm
    fCathodeThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fCathodeZPositionCmd = new G4UIcmdWithADoubleAndUnit(CommandName("cathodeZPosition"),this);
    fCathodeZPositionCmd->SetGuidance("Cathode z-position");
    fCathodeZPositionCmd->SetParameterName("cathodeZPosition",false);
    fCathodeZPositionCmd->SetUnitCategory("Length"); //default = -620.*mm
    fCathodeZPositionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fSiPMTotalHeightCmd = new G4UIcmdWithADoubleAndUnit(CommandName("SiPMTotalHeight"),this);
    fSiPMTotalHeightCmd->SetGuidance("Total height of all SiPMs");
    fSiPMTotalHeightCmd->SetParameterName("SiPMTotalHeight",false);
    fSiPMTotalHeightCmd->SetUnitCategory("Length"); //default = 1246.*mm
    fSiPMTotalHeightCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fSiPMPadHeightCmd = new G4UIcmdWithADoubleAndUnit(CommandName("SiPMPadHeight"),this);
    fSiPMPadHeightCmd->SetGuidance("SiPM pad side length");
    fSiPMPadHeightCmd->SetParameterName("SiPMPadHeight",false);
    fSiPMPadHeightCmd->SetUnitCategory("Length"); //default = 13.4164*mm
    fSiPMPadHeightCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fSiPMPadThicknessCmd = new G4UIcmdWithADoubleAndUnit(CommandName("SiPMPadThickness"),this);
    fSiPMPadThicknessCmd->SetGuidance("SiPM thickness");
    fSiPMPadThicknessCmd->SetParameterName("SiPMPadThickness",false);
    fSiPMPadThicknessCmd->SetUnitCategory("Length"); //default = 3.5*mm
    fSiPMPadThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fSiPMStandoffFromTPCCmd = new G4UIcmdWithADoubleAndUnit(CommandName("SiPMStandoffFromTPC"),this);
    fSiPMStandoffFromTPCCmd->SetGuidance("Distance of SiPM outer surface to TPC vessel");
    fSiPMStandoffFromTPCCmd->SetParameterName("SiPMStandoffFromTPC",false);
    fSiPMStandoffFromTPCCmd->SetUnitCategory("Length"); //default = 7.35*mm
    fSiPMStandoffFromTPCCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fChargeModuleLengthCmd = new G4UIcmdWithADoubleAndUnit(CommandName("chargeModuleLength"),this);
    fChargeModuleLengthCmd->SetGuidance("Charge module side length");
    fChargeModuleLengthCmd->SetParameterName("chargeModuleLength",false);
    fChargeModuleLengthCmd->SetUnitCategory("Length"); //default = 100.5*mm
    fChargeModuleLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fChargeModuleThicknessCmd = new G4UIcmdWithADoubleAndUnit(CommandName("chargeModuleThickness"),this);
    fChargeModuleThicknessCmd->SetGuidance("Charge module thickness");
    fChargeModuleThicknessCmd->SetParameterName("chargeModuleThickness",false);
    fChargeModuleThicknessCmd->SetUnitCategory("Length"); //default = 0.6*mm
    fChargeModuleThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fChargeModuleBackingThicknessCmd = new G4UIcmdWithADoubleAndUnit(CommandName("chargeModuleBackingThickness"),this);
    fChargeModuleBackingThicknessCmd->SetGuidance("Charge module (quartz) backing thickness");
    fChargeModuleBackingThicknessCmd->SetParameterName("chargeModuleBackingThickness",false);
    fChargeModuleBackingThicknessCmd->SetUnitCategory("Length"); //default = 0.5*mm
    fChargeModuleBackingThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fChargePadDiagonalLengthCmd = new G4UIcmdWithADoubleAndUnit(CommandName("chargePadDiagonalLength"),this);
    fChargePadDiagonalLengthCmd->SetGuidance("Charge pad diagonal length");
    fChargePadDiagonalLengthCmd->SetParameterName("chargePadDiagonalLength",false);
    fChargePadDiagonalLengthCmd->SetUnitCategory("Length"); //default = 3.*mm
    fChargePadDiagonalLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fChargePadThicknessCmd = new G4UIcmdWithADoubleAndUnit(CommandName("chargePadThickness"),this);
    fChargePadThicknessCmd->SetGuidance("Charge pad thickness");
    fChargePadThicknessCmd->SetParameterName("chargePadThickness",false);
    fChargePadThicknessCmd->SetUnitCategory("Length"); //default = 0.001*mm
    fChargePadThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fChargePadLinkLengthCmd = new G4UIcmdWithADoubleAndUnit(CommandName("chargePadLinkLength"),this);
    fChargePadLinkLengthCmd->SetGuidance("Charge pad link length");
    fChargePadLinkLengthCmd->SetParameterName("chargePadLinkLength",false);
    fChargePadLinkLengthCmd->SetUnitCategory("Length"); //default = 0.064*mm
    fChargePadLinkLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fChargePadLinkHeightCmd = new G4UIcmdWithADoubleAndUnit(CommandName("chargePadLinkHeight"),this);
    fChargePadLinkHeightCmd->SetGuidance("Charge pad link height");
    fChargePadLinkHeightCmd->SetParameterName("chargePadLinkHeight",false);
    fChargePadLinkHeightCmd->SetUnitCategory("Length"); //default = 0.025*mm
    fChargePadLinkHeightCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fSiPMActiveLayerThicknessCmd = new G4UIcmdWithADoubleAndUnit(CommandName("SiPMActiveLayerThickness"),this);
    fSiPMActiveLayerThicknessCmd->SetGuidance("Thickness of SiPM active layer");
    fSiPMActiveLayerThicknessCmd->SetParameterName("SiPMActiveLayerThickness",false);
    fSiPMActiveLayerThicknessCmd->SetUnitCategory("Length"); //default = 0.001*mm
    fSiPMActiveLayerThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fSiPMActiveLayerDepthCmd = new G4UIcmdWithADoubleAndUnit(CommandName("SiPMActiveLayerDepth"),this);
    fSiPMActiveLayerDepthCmd->SetGuidance("Depth of SiPM active layer from SiPM inner surface");
    fSiPMActiveLayerDepthCmd->SetParameterName("SiPMActiveLayerDepth",false);
    fSiPMActiveLayerDepthCmd->SetUnitCategory("Length"); //default = 1.*mm
    fSiPMActiveLayerDepthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fSiPMBaseLayerThicknessCmd = new G4UIcmdWithADoubleAndUnit(CommandName("SiPMBaseLayerThickness"),this);
    fSiPMBaseLayerThicknessCmd->SetGuidance("Thickness of SiPM pad base layer");
    fSiPMBaseLayerThicknessCmd->SetParameterName("SiPMBaseLayerThickness",false);
    fSiPMBaseLayerThicknessCmd->SetUnitCategory("Length"); //default = 0.5*mm
    fSiPMBaseLayerThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fSiPMWLSThicknessCmd = new G4UIcmdWithADoubleAndUnit(CommandName("SiPMWLSThickness"),this);
    fSiPMWLSThicknessCmd->SetGuidance("Thickness of WLS layer");
    fSiPMWLSThicknessCmd->SetParameterName("SiPMWLSThickness",false);
    fSiPMWLSThicknessCmd->SetUnitCategory("Length"); //default = 0.1*mm
    fSiPMWLSThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  };

  virtual ~nEXOTPCInternalsMessenger(){
    delete fWWDistanceOfAnodeCmd;
    delete fWWDistanceCmd;
    delete fFieldRingLengthCmd;
    delete fFieldRingThickCmd;
    delete fRingSideDistanceCmd;
    delete fRadialRingDistanceCmd;
    delete fFieldRingRefCmd;
    delete fAnodeRefCmd;
    delete fCathodeRefCmd;
    delete fTeflonRefCmd;
    delete fBulgeRefCmd;
    delete fBulgeRadiusCmd;
    delete fTPCVesselRefCmd;
    delete fActiveSiPMPadFracCmd;
    delete fSiPMOptionCmd;
    delete fAnodeOptionCmd;
    delete fRingArrangeIDCmd;
    delete fInternalsFlagCmd;
    delete fUseWLSCmd;
    delete fCathodeAnodeDistanceCmd;
    delete fCathodeToBottomRingDistanceCmd;
    delete fAnodeToTopRingDistanceCmd;
    delete fAnodeRadiusCmd;
    delete fAnodeThicknessCmd;
    delete fAnodeZPositionCmd;
    delete fCathodeRadiusCmd;
    delete fCathodeThicknessCmd;
    delete fCathodeZPositionCmd;
    delete fSiPMTotalHeightCmd;
    delete fSiPMPadHeightCmd;
    delete fSiPMPadThicknessCmd;
    delete fSiPMStandoffFromTPCCmd;
    delete fChargeModuleLengthCmd;
    delete fChargeModuleThicknessCmd;
    delete fChargeModuleBackingThicknessCmd;
    delete fChargePadDiagonalLengthCmd;
    delete fChargePadThicknessCmd;
    delete fChargePadLinkLengthCmd;
    delete fChargePadLinkHeightCmd;
    delete fSiPMActiveLayerThicknessCmd;
    delete fSiPMActiveLayerDepthCmd;
    delete fSiPMBaseLayerThicknessCmd;
    delete fSiPMWLSThicknessCmd;

  };

  void SetNewValue(G4UIcommand *cmd, G4String val){
    if( cmd == fWWDistanceOfAnodeCmd) {
      fConstructor->SetWWDistanceOfAnode(fWWDistanceOfAnodeCmd->GetNewDoubleValue(val));
    }
    else if( cmd == fWWDistanceCmd ) {
      fConstructor->SetWWDistance(fWWDistanceCmd->GetNewDoubleValue(val));
    }   
    else if( cmd == fFieldRingRefCmd ) {
      fConstructor->SetFieldRingRef(fFieldRingRefCmd->GetNewDoubleValue(val));
    }
    else if( cmd == fFieldRingLengthCmd ) {
      fConstructor->SetFieldRingLength(fFieldRingLengthCmd->GetNewDoubleValue(val));
    }   
    else if( cmd == fFieldRingThickCmd ) {
      fConstructor->SetFieldRingThick(fFieldRingThickCmd->GetNewDoubleValue(val));
    }   
    else if( cmd == fRingSideDistanceCmd ) {
      fConstructor->SetRingSideDistance(fRingSideDistanceCmd->GetNewDoubleValue(val));
    }   
    else if( cmd == fRadialRingDistanceCmd ) {
      fConstructor->SetRadialRingDistance(fRadialRingDistanceCmd->GetNewDoubleValue(val));
    }   
    else if( cmd == fAnodeRefCmd ) {
      fConstructor->SetAnodeRef(fAnodeRefCmd->GetNewDoubleValue(val));
    }
    else if( cmd == fCathodeRefCmd ) {
      fConstructor->SetCathodeRef(fCathodeRefCmd->GetNewDoubleValue(val));
    }
    else if( cmd == fTeflonRefCmd ) {
      fConstructor->SetTeflonRef(fTeflonRefCmd->GetNewDoubleValue(val));
    }
    else if( cmd == fTPCVesselRefCmd ) {
      fConstructor->SetTPCVesselRef(fTPCVesselRefCmd->GetNewDoubleValue(val));
    }
    else if( cmd == fBulgeRadiusCmd ) {
      fConstructor->SetBulgeRadius(fBulgeRadiusCmd->GetNewDoubleValue(val));
    }
    else if( cmd == fBulgeRefCmd ) {
      fConstructor->SetBulgeRef(fBulgeRefCmd->GetNewDoubleValue(val));
    }
    else if( cmd == fActiveSiPMPadFracCmd ) {
      fConstructor->SetActiveSiPMPadFrac(fActiveSiPMPadFracCmd->GetNewDoubleValue(val));
    }
    else if( cmd == fSiPMOptionCmd ) {
      fConstructor->SetSiPMOption(fSiPMOptionCmd->GetNewIntValue(val));
    }
    else if( cmd == fAnodeOptionCmd ) {
      fConstructor->SetAnodeOption(fAnodeOptionCmd->GetNewIntValue(val));
    }
    else if( cmd == fRingArrangeIDCmd ) {
      fConstructor->SetRingArrangeID(fRingArrangeIDCmd->GetNewIntValue(val));
    }
    else if( cmd == fUseWLSCmd ) {
      fConstructor->SetUseWLS(fUseWLSCmd->GetNewIntValue(val));
    }
    else if( cmd == fInternalsFlagCmd ) {
      fConstructor->SetInternalsFlag(fInternalsFlagCmd->GetNewIntValue(val));
    }
    else if(cmd == fCathodeAnodeDistanceCmd){
      fConstructor->SetCathodeAnodeDistance(fCathodeAnodeDistanceCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fCathodeToBottomRingDistanceCmd){
      fConstructor->SetCathodeToBottomRingDistance(fCathodeToBottomRingDistanceCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fAnodeToTopRingDistanceCmd){
      fConstructor->SetAnodeToTopRingDistance(fAnodeToTopRingDistanceCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fAnodeRadiusCmd){
      fConstructor->SetAnodeRadius(fAnodeRadiusCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fAnodeThicknessCmd){
      fConstructor->SetAnodeThickness(fAnodeThicknessCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fAnodeZPositionCmd){
      fConstructor->SetAnodeZPosition(fAnodeZPositionCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fCathodeRadiusCmd){
      fConstructor->SetCathodeRadius(fCathodeRadiusCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fCathodeThicknessCmd){
      fConstructor->SetCathodeThickness(fCathodeThicknessCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fCathodeZPositionCmd){
      fConstructor->SetCathodeZPosition(fCathodeZPositionCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fSiPMTotalHeightCmd){
      fConstructor->SetSiPMTotalHeight(fSiPMTotalHeightCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fSiPMPadHeightCmd){
      fConstructor->SetSiPMPadHeight(fSiPMPadHeightCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fSiPMPadThicknessCmd){
      fConstructor->SetSiPMPadThickness(fSiPMPadThicknessCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fSiPMStandoffFromTPCCmd){
      fConstructor->SetSiPMStandoffFromTPC(fSiPMStandoffFromTPCCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fChargeModuleLengthCmd){
      fConstructor->SetChargeModuleLength(fChargeModuleLengthCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fChargeModuleThicknessCmd){
      fConstructor->SetChargeModuleThickness(fChargeModuleThicknessCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fChargeModuleBackingThicknessCmd){
      fConstructor->SetChargeModuleBackingThickness(fChargeModuleBackingThicknessCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fChargePadDiagonalLengthCmd){
      fConstructor->SetChargePadDiagonalLength(fChargePadDiagonalLengthCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fChargePadThicknessCmd){
      fConstructor->SetChargePadThickness(fChargePadThicknessCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fChargePadLinkLengthCmd){
      fConstructor->SetChargePadLinkLength(fChargePadLinkLengthCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fChargePadLinkHeightCmd){
      fConstructor->SetChargePadLinkHeight(fChargePadLinkHeightCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fSiPMActiveLayerThicknessCmd){
      fConstructor->SetSiPMActiveLayerThickness(fSiPMActiveLayerThicknessCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fSiPMActiveLayerDepthCmd){
      fConstructor->SetSiPMActiveLayerDepth(fSiPMActiveLayerDepthCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fSiPMBaseLayerThicknessCmd){
      fConstructor->SetSiPMBaseLayerThickness(fSiPMBaseLayerThicknessCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fSiPMWLSThicknessCmd){
      fConstructor->SetSiPMWLSThickness(fSiPMWLSThicknessCmd->GetNewDoubleValue(val));
    }
    else {
      nEXOModuleConstructorMessenger::SetNewValue(cmd,val);
    }
    
  };
};

nEXOTPCInternalsConstructor::~nEXOTPCInternalsConstructor(){;}

void nEXOTPCInternalsConstructor::Init(void)
{
  SetMessenger(new nEXOTPCInternalsMessenger(this));

  // Parameters
  // -- general
  fInternalsFlag = 2;
  fUseWLS = 0;

  // -- TPC Vessel 
  fTPCLowerRadius = 650*mm;
  fTeflonRef = 0.95;
  fTPCVesselRef = 0.0;

  // -- Field shaping rings
  fRingArrangeID = 2;
  fFieldRingLength = 10*mm;
  fFieldRingThick = 4*mm;
  fWWDistance = 20*mm;
  fRingSideDistance = 60.8*mm;
  fRadialRingDistance = 6*mm;  // Obsolete
  fFieldRingRef = 0.8;
  
  // -- Cathode, bulge and anode
  fCathodeAnodeDistance = 1259.*mm;
  fCathodeToBottomRingDistance = 18.16*mm;
  fAnodeToTopRingDistance = 18.*mm;

  // -- cathode
  fCathodeRef = 0.8;
  fBulgeRadius = 10.*mm;
  fBulgeRef = 0.8;
  fCathodeRadius = 597.7*mm;
  fCathodeThickness = 0.25*mm;
  fCathodeZPosition = -620.*mm;

  // -- anode
  fAnodeRadius = 587.7*mm;
  fAnodeThickness = 3.*mm;
  fAnodeZPosition = 635.5*mm;
  fAnodeRef = 0.5;

  //fModuleLength = 100.5*mm;
  //fModuleThickness = 0.3*mm;
  //fPadDiagonalLength = 1.5*mm;
  //fPadThickness = 0.0005*mm;
  //fPadLinkY1Length = 0.032*mm;
  //fPadLinkHeight = 0.0125*mm;
  //fSiliconThickness = 0.25*mm; // Half-thickness of quartz backing for a tile

  // -- SiPM
  fSiPMTotalHeight = 1246.*mm;
  fSiPMPadHeight = 13.4164*mm;
  fSiPMPadThickness = 3.5*mm;
  fSiPMStandoffFromTPC = 7.35*mm;
  fActiveSiPMPadFrac = 0.8;

  // Charge tiles
  fChargeModuleLength = 100.5*mm;
  fChargeModuleThickness = 0.6*mm;
  fChargeModuleBackingThickness = 0.5*mm;
  fChargePadDiagonalLength = 3.*mm;
  fChargePadThickness = 0.001*mm;
  fChargePadLinkLength = 0.064*mm;
  fChargePadLinkHeight = 0.025*mm;

  fSiPMActiveLayerThickness = 0.001*mm;
  fSiPMActiveLayerDepth = 1.*mm;
  fSiPMBaseLayerThickness = 0.5*mm;
  fSiPMWLSThickness = 0.1*mm;


  // dimensions are those of TPC vessel
  SetRadius1(fTPCLowerRadius + 0.3*cm);
  SetRadius2(65.30*cm);
  SetLength(2*65.66*cm);

  // -- SiPM: Derived quantities
  fNSiPMPadAlongZ = 0;
  std::fill(fNSiPMPadAlongPhi, fNSiPMPadAlongPhi+10000, 0);

  fNSiPMPadAlongY = 0;
  std::fill(fNSiPMPadAlongX, fNSiPMPadAlongX+10000, 0);

  AddConstructor(new nEXOLXeConstructor("LXe",this));

}

void nEXOTPCInternalsConstructor::SetDimensionsForTPCVessel(nEXOTPCVesselConstructor& tpcVessel)
{
  nEXOLXeConstructor& internalLXe = Get<nEXOLXeConstructor>("LXe");
  internalLXe.FillTPCVessel(tpcVessel);
}


G4LogicalVolume* nEXOTPCInternalsConstructor::GetPiece(void)
{
  G4LogicalVolume* logicLXe;
  nEXOLXeConstructor& LXe = Get<nEXOLXeConstructor>("LXe");

  G4cout << "//################################################//" << G4endl;
  G4cout << "//############# nEXO TPC Internals ###############//" << G4endl;
  G4cout << "//################################################//" << G4endl;

  LXe.SetAllActive();
  LXe.SetCheckOverlaps(fCheckOverlaps);
  logicLXe = LXe.GetPiece();
  
  if (fInternalsFlag > 0)
  {
    //BuildOtherParts(logicLXe, LXe.GetRadius1(),LXe.GetRadius2(),LXe.GetLength()/2.);
    BuildOtherParts(nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/LXe/ActiveRegion")->GetLogicalVolume(), LXe.GetRadius1(),LXe.GetRadius2(),LXe.GetLength()/2.);
    
    SetOpticalSurfaces();
  }

  return logicLXe;
}

void nEXOTPCInternalsConstructor::BuildOtherParts(G4LogicalVolume* logicLXe, G4double inactiveLXeR1, G4double inactiveLXeR2, G4double inactiveLXeH)
{
  
  G4cout << "//##################################################//" << G4endl;
  G4cout << "//############### Field Shaping Rings ##############//" << G4endl;
  G4cout << "//##################################################//" << G4endl;

  G4String fieldRingName = GetName() + "/FieldRing";
  G4double fieldRingSweptR;

  //G4double fFieldRingLength = fFieldRingLength; 
  G4double fieldRingThick = fFieldRingThick/2.0;  //not used in default
  G4double fieldRingIR = 0*mm;  //not used in default
  G4double fieldRingOR = fieldRingThick;  //not used in default (fRingArrangeID == 2)

  G4LogicalVolume* logicFieldRing[10000] = {0};
  G4VPhysicalVolume* physFieldRing[10000] = {0};

  //G4int nFieldRings = (G4int) ((fCathodeAnodeDistance-1*cm)/fWWDistance);
  G4int nFieldRings = (G4int) ((fCathodeAnodeDistance-fCathodeToBottomRingDistance-fAnodeToTopRingDistance-fFieldRingThick)/fWWDistance + 1);
 
  // single wire rings (Obsolete as of Apr 2015)
  if(fRingArrangeID == 0) {
    G4cout << "Constructing field wire rings based on single wire ring model." << G4endl;
    for (G4int i = 0; i< nFieldRings; i++) {
      G4double z;
      z = inactiveLXeH - 13*mm - 3*mm - fWWDistance + fieldRingOR - fWWDistance*i;
      
      G4double radius = inactiveLXeR2 + (inactiveLXeH - z)*(inactiveLXeR1 - inactiveLXeR2)/(2*inactiveLXeH);
      fieldRingSweptR = radius - fRingSideDistance - fieldRingThick;
      logicFieldRing[i] = new G4LogicalVolume(new G4Torus(fieldRingName,
                                                          fieldRingIR,fieldRingOR,
                                                          fieldRingSweptR,
                                                          0,360*deg),
                                              FindMaterial("G4_Cu"),
                                              fieldRingName);

      physFieldRing[i] = new G4PVPlacement(0,
                                           G4ThreeVector(0,0,z),
                                           logicFieldRing[i],
                                           fieldRingName,
                                           logicLXe,
                                           false,
                                           i,fCheckOverlaps);

      nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(fieldRingName,physFieldRing[i],i);
    } 
  }

  // 3 engaged wire rings (Obsolete as of Apr 2015)
  if(fRingArrangeID == 1) {
    G4cout << "Constructing field wire rings based on 3 engaged wire ring model." << G4endl;
    for (G4int i = 0; i< nFieldRings; i++) {
      G4double z;
      G4int k = (G4int) (i/3);
      G4int j = (G4int) (i%3);
      z = inactiveLXeH - 13*mm - 3*mm - fWWDistance + fieldRingOR - fWWDistance*k + 4*j*mm;

      G4double radius = inactiveLXeR2 + (inactiveLXeH - z)*(inactiveLXeR1 - inactiveLXeR2)/(2*inactiveLXeH);
      fieldRingSweptR = radius - fRingSideDistance - fieldRingThick;
      logicFieldRing[i] = new G4LogicalVolume(new G4Torus(fieldRingName,
                                                          fieldRingIR,fieldRingOR,
                                                          fieldRingSweptR,
                                                          0,360*deg),
                                              FindMaterial("G4_Cu"),
                                              fieldRingName);

      physFieldRing[i] = new G4PVPlacement(0,
                                           G4ThreeVector(0,0,z),
                                           logicFieldRing[i],
                                           fieldRingName,
                                           logicLXe,
                                           false,
                                           i,fCheckOverlaps);
      nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(fieldRingName,physFieldRing[i],i);   
    }
  }

  // horizontal plate ring (Current baseline, Apr 2015)
  if(fRingArrangeID == 2) {
    G4cout << "Constructing field wire rings based on horizontal plate ring model." << G4endl;
    for (G4int i = 0; i< nFieldRings; i++) {
      // Z position of field ring
      // G4double z = inactiveLXeH - 13*mm - 3*mm - fWWDistance + fieldRingThick - fWWDistance*i;
      G4double z = fAnodeZPosition - fAnodeThickness - fAnodeToTopRingDistance - fFieldRingThick/2 - i*fWWDistance;

      // Radius of TPC vessel at "z"
      G4double radius = inactiveLXeR2 + (inactiveLXeH - z)*(inactiveLXeR1 - inactiveLXeR2)/(2*inactiveLXeH);

      // Radius of the field ring (Major radius of torus)
      //fieldRingSweptR = radius - fRingSideDistance - fFieldRingLength/2.0;
      fieldRingSweptR = radius - fRingSideDistance - fFieldRingLength/2.0;

      G4Tubs* midRing = new G4Tubs("MidFieldRing", fieldRingSweptR - fFieldRingLength/2.0 + fFieldRingThick/2.0, fieldRingSweptR + fFieldRingLength/2.0 - fFieldRingThick/2.0, fFieldRingThick/2.0, 0, 360*deg);
      G4Torus* innerRing = new G4Torus("InnerFieldRing", 0, fFieldRingThick/2.0, fieldRingSweptR - fFieldRingLength/2.0 + fFieldRingThick/2.0, 0, 360*deg);
      G4Torus* outerRing = new G4Torus("OuterFieldRing", 0, fFieldRingThick/2.0, fieldRingSweptR + fFieldRingLength/2.0 - fFieldRingThick/2.0, 0, 360*deg);
      G4UnionSolid* midInnerUnion = new G4UnionSolid("Mid+InnerFieldRing", midRing, innerRing);
      G4UnionSolid* solidFieldRing = new G4UnionSolid(fieldRingName, midInnerUnion, outerRing);
      logicFieldRing[i] = new G4LogicalVolume(solidFieldRing,
                                              FindMaterial("G4_Cu"),
                                              fieldRingName);
      
      physFieldRing[i] = new G4PVPlacement(0,
                                           G4ThreeVector(0,0,z),
                                           logicFieldRing[i],
                                           fieldRingName,
                                           logicLXe,
                                           false,
                                           i,fCheckOverlaps);
      nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(fieldRingName,physFieldRing[i],i);   
    }
  }
 
  //construct two wires struture, instead of plates (Obsolete as of Apr 2015)
  if(fRingArrangeID == 3) {
    G4cout << "Constructing two radial field wires instead of plates." << G4endl;
    G4int iWire = 0;
    for (G4int i = 0; i< nFieldRings; i++) {
      G4double z = inactiveLXeH - 13*mm - 3*mm - fWWDistance + fieldRingOR - fWWDistance*i;

      G4double radius = inactiveLXeR2 + (inactiveLXeH - z)*(inactiveLXeR1 - inactiveLXeR2)/(2*inactiveLXeH);
      
      //outer wire
      fieldRingSweptR = radius - fRingSideDistance - fieldRingThick;
      logicFieldRing[iWire] = new G4LogicalVolume(new G4Torus(fieldRingName,
                                                              fieldRingIR,fieldRingOR,
                                                              fieldRingSweptR,
                                                              0,360*deg),
                                                  FindMaterial("G4_Cu"),
                                                  fieldRingName);

      physFieldRing[iWire] = new G4PVPlacement(0,
                                               G4ThreeVector(0,0,z),
                                               logicFieldRing[iWire],
                                               fieldRingName,
                                               logicLXe,
                                               false,
                                               iWire,fCheckOverlaps);
      
      nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(fieldRingName,physFieldRing[iWire],iWire);   

      //inner wire
      fieldRingSweptR = radius - fRingSideDistance - fieldRingThick - fRadialRingDistance;
      logicFieldRing[iWire+1] = new G4LogicalVolume(new G4Torus(fieldRingName,
                                                                fieldRingIR,fieldRingOR,
                                                                fieldRingSweptR,
                                                                0,360*deg),
                                                    FindMaterial("G4_Cu"),
                                                    fieldRingName);

      physFieldRing[iWire+1] = new G4PVPlacement(0,
                                                 G4ThreeVector(0,0,z),
                                                 logicFieldRing[iWire+1],
                                                 fieldRingName,
                                                 logicLXe,
                                                 false,
                                                 iWire+1,fCheckOverlaps);
      
      nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(fieldRingName,physFieldRing[iWire+1],iWire+1);   

      iWire += 2;
    }
  }

  //conical field cage (Obsolete as of Apr 2015)
  if(fRingArrangeID == 4) {
    G4cout << "Constructing conical field cage with horizontal plates." << G4endl;
    G4double ringPhotoDisArray[6] = {5*mm, 10*mm, 15*mm, 20*mm, 25*mm, 30*mm};
    G4int numRingInGroup[6] = {0};
    G4int resNumRings = nFieldRings - ((G4int) (nFieldRings/6))*6;
    for(G4int i = 0; i < 6; i++) {
      if(resNumRings > 0) {
        numRingInGroup[i] = (G4int) (nFieldRings/6) + 1;
        resNumRings--;
      }
      else {
        numRingInGroup[i] = (G4int) (nFieldRings/6);
      }
    }

    G4int iGroup = 0;
    for (G4int i = 0; i< nFieldRings; i++) {
      if(i < numRingInGroup[0]) iGroup = 0;
      if(i < (numRingInGroup[0] + numRingInGroup[1]) && i >= numRingInGroup[0]) iGroup = 1;
      if(i < (numRingInGroup[0] + numRingInGroup[1] + numRingInGroup[2]) && i >= (numRingInGroup[0]+numRingInGroup[1])) iGroup = 2;
      if(i < (numRingInGroup[0] + numRingInGroup[1] + numRingInGroup[2] + numRingInGroup[3]) && i >= (numRingInGroup[0]+numRingInGroup[1]+numRingInGroup[2])) iGroup = 3;
      if(i < (numRingInGroup[0] + numRingInGroup[1] + numRingInGroup[2] + numRingInGroup[3]+numRingInGroup[4]) && i >= (numRingInGroup[0]+numRingInGroup[1]+numRingInGroup[2]+numRingInGroup[3])) iGroup = 4;
      if(i >= (numRingInGroup[0]+numRingInGroup[1]+numRingInGroup[2]+numRingInGroup[3]+numRingInGroup[4])) iGroup = 5;

      G4double z;
      z = inactiveLXeH - 13*mm - 3*mm - fWWDistance + fieldRingThick - fWWDistance*i;

      G4double radius = inactiveLXeR2 + (inactiveLXeH - z)*(inactiveLXeR1 - inactiveLXeR2)/(2*inactiveLXeH);
      fieldRingSweptR = radius - 10.85*mm - ringPhotoDisArray[iGroup] - fFieldRingLength/2.0;
      G4Tubs* midRing = new G4Tubs("MidFieldRing", fieldRingSweptR - fFieldRingLength/2.0 + fieldRingThick, fieldRingSweptR + fFieldRingLength/2.0 - fieldRingThick, fieldRingThick, 0, 360*deg);
      G4Torus* innerRing = new G4Torus("InnerFieldRing", 0, fieldRingThick, fieldRingSweptR - fFieldRingLength/2.0 + fieldRingThick, 0, 360*deg);
      G4Torus* outerRing = new G4Torus("OuterFieldRing", 0, fieldRingThick, fieldRingSweptR + fFieldRingLength/2.0 - fieldRingThick, 0, 360*deg);
      G4UnionSolid* midInnerUnion = new G4UnionSolid("Mid+InnerFieldRing", midRing, innerRing);
      G4UnionSolid* solidFieldRing = new G4UnionSolid(fieldRingName, midInnerUnion, outerRing);
      logicFieldRing[i] = new G4LogicalVolume(solidFieldRing,
                                              FindMaterial("G4_Cu"),
                                              fieldRingName);

      physFieldRing[i] = new G4PVPlacement(0,
                                           G4ThreeVector(0,0,z),
                                           logicFieldRing[i],
                                           fieldRingName,
                                           logicLXe,
                                           false,
                                           i,fCheckOverlaps);
      nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(fieldRingName, physFieldRing[i], i);
    }
  }

  //Attributes of Field Rings
  G4VisAttributes* RingsAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));
  RingsAtt->SetVisibility(false);
  RingsAtt->SetForceSolid(true);
  for(int i = 0; i < nFieldRings; i++) logicFieldRing[i]->SetVisAttributes(RingsAtt);
  //logicFieldRingLip->SetVisAttributes(RingsAtt);


  G4cout << "//##################################################//" << G4endl;
  G4cout << "//####################### SiPM #####################//" << G4endl;
  G4cout << "//##################################################//" << G4endl;

  //G4double SiPMHeight = (fCathodeAnodeDistance - 11*mm - 2*mm)/2.0;  // Halfheight
  //G4double SiPMPadHeight = 13.4164*mm;//SiPMHeight;//13.4164*mm;  // Full Height
  if (fInternalsFlag == 1)
    fSiPMPadHeight = fSiPMTotalHeight/2.;
  
  G4int iSiPMPad = 0;
  if(fSiPMOption == 0){   // Default
    G4String SiPMName = GetName() + "/SiPMPads";
    fNSiPMPadAlongZ = (G4int) (fSiPMTotalHeight/fSiPMPadHeight);
    G4double dGapAlongZ = (fSiPMTotalHeight - fSiPMPadHeight*fNSiPMPadAlongZ)/(fNSiPMPadAlongZ - 1);

    //G4int nSiPMPadAlongPhi[10000] = {0};

    G4LogicalVolume* logicSiPMPads[10000] = {0};
    G4VPhysicalVolume* physSiPMPads[1000000] = {0};

    G4LogicalVolume* logicActiveSiPMPads[10000] = {0};
    G4VPhysicalVolume* physActiveSiPMPads[10000] = {0};

    G4LogicalVolume* logicSiPMPadBase[10000] = {0};
    G4VPhysicalVolume* physSiPMBase[10000] = {0};

    G4LogicalVolume* logicWLS[10000] = {0};
    G4VPhysicalVolume* physWLS[10000] = {0};

    iSiPMPad = 0;
    for(G4int i = 0; i < fNSiPMPadAlongZ; i++) {
      //G4double SiPMPadPlaceZ = inactiveLXeH-13*mm-3*mm-11*mm-fSiPMPadHeight/2.0-i*fSiPMPadHeight-i*dGapAlongZ;
      G4double SiPMPadPlaceZ = fAnodeZPosition - (fCathodeAnodeDistance-fSiPMTotalHeight)/2. -(0.5+i)*fSiPMPadHeight - i*dGapAlongZ;
      G4double radius = inactiveLXeR2 + (inactiveLXeH - SiPMPadPlaceZ)*(inactiveLXeR1 - inactiveLXeR2)/(2*inactiveLXeH);
      G4double SiPMOR = radius - fSiPMStandoffFromTPC;
      G4double SiPMIR = SiPMOR - fSiPMPadThickness;

      //nSiPMPadAlongPhi[i] = (G4int) (CLHEP::pi*(SiPMIR+SiPMOR)/fSiPMPadHeight);
      fNSiPMPadAlongPhi[i] = (G4int) (CLHEP::pi*(SiPMIR+SiPMOR)/fSiPMPadHeight);
      G4double SiPMPadDPhi = 360.0*fSiPMPadHeight/(CLHEP::pi*(SiPMIR+SiPMOR))*deg;
      G4double dGapAlongPhi = (360.0*deg - SiPMPadDPhi*fNSiPMPadAlongPhi[i])/fNSiPMPadAlongPhi[i];

      logicSiPMPads[i] = new G4LogicalVolume(new G4Tubs(SiPMName,
                                                        SiPMIR, SiPMOR,
                                                        fSiPMPadHeight/2.0,
                                                        0*deg, SiPMPadDPhi),
                                             FindMaterial("G4_SILICON_DIOXIDE"),
                                             SiPMName);
      
      for(G4int j = 0; j < fNSiPMPadAlongPhi[i]; j++) {
        G4RotationMatrix* rotSiPMPad = new G4RotationMatrix();
        rotSiPMPad->rotateZ(j*(SiPMPadDPhi+dGapAlongPhi));
        physSiPMPads[iSiPMPad] = new G4PVPlacement(rotSiPMPad,                             
                                                   G4ThreeVector(0,0,SiPMPadPlaceZ),
                                                   logicSiPMPads[i],
                                                   SiPMName,
                                                   logicLXe,
                                                   false,
                                                   i*10000+j,fCheckOverlaps);

        nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(SiPMName,physSiPMPads[iSiPMPad],iSiPMPad);   
        
        iSiPMPad++;
      }

      //**************************************************//
      //********************     WLS    ******************//
      //**************************************************// 
      if(fUseWLS == 1) {
        G4String WLSName = GetName() + "/WLS";
        logicWLS[i] = new G4LogicalVolume(new G4Tubs(WLSName,
                               SiPMIR, SiPMIR+fSiPMWLSThickness,
                               fSiPMPadHeight/2.0,
                               0*deg, SiPMPadDPhi),
                               FindMaterial("TPB"),
                               WLSName);

        physWLS[i] = new G4PVPlacement(0,
                                       G4ThreeVector(0,0,0),
                                       logicWLS[i],
                                       WLSName,
                                       logicSiPMPads[i],
                                       false,
                                       0,fCheckOverlaps);

        nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(WLSName, physWLS[i], i);   
      }

      //G4cout << "//##################################################//" << G4endl;
      //G4cout << "//#################### ActiveSiPM ##################//" << G4endl;
      //G4cout << "//##################################################//" << G4endl;

      G4String ActiveSiPMName = GetName() + "/ActiveSiPMPads";
      G4double ActiveSiPMPadIR = SiPMIR + fSiPMActiveLayerDepth;
      G4double ActiveSiPMPadOR = ActiveSiPMPadIR + fSiPMActiveLayerThickness;
      G4double ActiveSiPMPadHeight = fSiPMPadHeight*sqrt(fActiveSiPMPadFrac);
      G4double ActiveSiPMPadDPhi = SiPMPadDPhi*sqrt(fActiveSiPMPadFrac);

      logicActiveSiPMPads[i] = new G4LogicalVolume(new G4Tubs(ActiveSiPMName,
                                                              ActiveSiPMPadIR, ActiveSiPMPadOR,
                                                              ActiveSiPMPadHeight/2.0,
                                                              (SiPMPadDPhi-ActiveSiPMPadDPhi)/2.0, ActiveSiPMPadDPhi),
                                                   FindMaterial("G4_Si"),
                                                   ActiveSiPMName);

      physActiveSiPMPads[i] = new G4PVPlacement(0,
                                                G4ThreeVector(0,0,0),
                                                logicActiveSiPMPads[i],
                                                ActiveSiPMName,
                                                logicSiPMPads[i],
                                                false,
                                                0,fCheckOverlaps);
      
      nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(ActiveSiPMName,physActiveSiPMPads[i],i);   
      
      //G4cout << "//##################################################//" << G4endl;
      //G4cout << "//################### SiPMPadBase ##################//" << G4endl;
      //G4cout << "//##################################################//" << G4endl;

      G4String SiPMPadBaseName = GetName() + "/SiPMPadBase";
      G4double SiPMPadBaseIR = ActiveSiPMPadOR;
      G4double SiPMPadBaseOR = SiPMPadBaseIR + fSiPMBaseLayerThickness;
      G4double SiPMPadBaseHeight = ActiveSiPMPadHeight;
      G4double SiPMPadBaseDPhi = ActiveSiPMPadDPhi;
        
      logicSiPMPadBase[i] = new G4LogicalVolume(new G4Tubs(SiPMPadBaseName,
                                                           SiPMPadBaseIR, SiPMPadBaseOR,
                                                           SiPMPadBaseHeight/2.0,
                                                           (SiPMPadDPhi-SiPMPadBaseDPhi)/2.0, SiPMPadBaseDPhi), 
                                                FindMaterial("G4_Si"),
                                                SiPMPadBaseName);                                                                                                                 
      physSiPMBase[i] = new G4PVPlacement(0,
                                          G4ThreeVector(0,0,0),
                                          logicSiPMPadBase[i],
                                          SiPMPadBaseName,
                                          logicSiPMPads[i],
                                          false,
                                          0,fCheckOverlaps);

      nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(SiPMPadBaseName,physSiPMBase[i],i);   
    }

    G4VisAttributes* SiPMAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));  
    SiPMAtt->SetVisibility(false);
    G4VisAttributes* ActiveSiPMAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));  
    ActiveSiPMAtt->SetVisibility(false);
    G4VisAttributes* SiPMBaseAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 0.5));  
    SiPMBaseAtt->SetVisibility(false);
    //SiPMAtt->SetForceSolid(true);
    //logicSiPMs->SetVisAttributes(SiPMAtt);
    for(G4int i = 0; i < fNSiPMPadAlongZ; i++) {
      logicSiPMPads[i]->SetVisAttributes(SiPMAtt);
      logicActiveSiPMPads[i]->SetVisAttributes(ActiveSiPMAtt);
      logicSiPMPadBase[i]->SetVisAttributes(SiPMBaseAtt);
    }
  }

  if(fSiPMOption == 1) {   // Obsolete as of May 2015
    G4String SiPMName = GetName() + "/SiPMPads";
    fNSiPMPadAlongY = (G4int) ((inactiveLXeR2-fSiPMPadHeight/2.0-20*mm)/fSiPMPadHeight);

    G4LogicalVolume* logicSiPMPads[10000] = {0};
    G4VPhysicalVolume* physSiPMPads[1000000] = {0};

    G4LogicalVolume* logicActiveSiPMPads[10000] = {0};
    G4VPhysicalVolume* physActiveSiPMPads[10000] = {0};

    G4LogicalVolume* logicSiPMPadBase[10000] = {0};
    G4VPhysicalVolume* physSiPMBase[10000] = {0};

    G4LogicalVolume* logicWLS[10000] = {0};
    G4VPhysicalVolume* physWLS[10000] = {0};

    iSiPMPad = 0;
    for(G4int i = 0; i < fNSiPMPadAlongY+1; i++) {
      G4double SiPMPadPlaceZ = inactiveLXeH - 13*mm + 3.5*mm/2.0 + 6*mm;
      G4double radius = inactiveLXeR2 + (inactiveLXeH - SiPMPadPlaceZ)*(inactiveLXeR1 - inactiveLXeR2)/(2*inactiveLXeH);
      G4double SiPMOR = radius - 7.35*mm;
      G4double SiPMIR = SiPMOR - 3.5*mm;

      G4double halflength = sqrt((inactiveLXeR2-20*mm)*(inactiveLXeR2-20*mm) - (i*fSiPMPadHeight)*(i*fSiPMPadHeight));
      logicSiPMPads[i] = new G4LogicalVolume(new G4Box(SiPMName,
                                                fSiPMPadHeight/2.0,
                                                fSiPMPadHeight/2.0,
                                                3.5*mm/2.0),
                                                FindMaterial("G4_SILICON_DIOXIDE"),
                                                SiPMName);
      G4double ypos = i*fSiPMPadHeight;
      for(G4int j = 0; j < fNSiPMPadAlongX[i]+1; j++) {
        G4double xpos = j*fSiPMPadHeight;
        physSiPMPads[iSiPMPad] = new G4PVPlacement(0,
                                                   G4ThreeVector(xpos,ypos,SiPMPadPlaceZ),
                                                   logicSiPMPads[i],
                                                   SiPMName,
                                                   logicLXe,
                                                   false,
                                                   i*10000+j*1000+1,fCheckOverlaps);

        nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(SiPMName,physSiPMPads[iSiPMPad],iSiPMPad);

        iSiPMPad++;

        if(j > 0) {
          physSiPMPads[iSiPMPad] = new G4PVPlacement(0,
                                                     G4ThreeVector(-xpos,ypos,SiPMPadPlaceZ),
                                                     logicSiPMPads[i],
                                                     SiPMName,
                                                     logicLXe,
                                                     false,
                                                     i*10000+j*1000+2,fCheckOverlaps);

          nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(SiPMName,physSiPMPads[iSiPMPad],iSiPMPad);
                                    
          iSiPMPad++;
        }
      }
      if(i > 0) {
        for(G4int j = 0; j < fNSiPMPadAlongX[i]+1; j++) {
          G4double xpos = j*fSiPMPadHeight;
          physSiPMPads[iSiPMPad] = new G4PVPlacement(0,
                                                     G4ThreeVector(xpos,-ypos,SiPMPadPlaceZ),
                                                     logicSiPMPads[i],
                                                     SiPMName,
                                                     logicLXe,
                                                     false,
                                                     i*10000+j*1000+4,fCheckOverlaps);
                                    
          nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(SiPMName,physSiPMPads[iSiPMPad],iSiPMPad);

          iSiPMPad++;
          if(j > 0) {
            physSiPMPads[iSiPMPad] = new G4PVPlacement(0,
                                                       G4ThreeVector(-xpos,-ypos,SiPMPadPlaceZ),
                                                       logicSiPMPads[i],
                                                       SiPMName,
                                                       logicLXe,
                                                       false,
                                                       i*10000+j*1000+3,fCheckOverlaps);

            nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(SiPMName,physSiPMPads[iSiPMPad],iSiPMPad);
                                      
            iSiPMPad++;
          }
        }
      }

      //**************************************************//
      //********************     WLS    ******************//
      //**************************************************//
      if(fUseWLS == 1) {
        G4String WLSName = GetName() + "/WLS";
        logicWLS[i] = new G4LogicalVolume(new G4Box(WLSName,
                                          fSiPMPadHeight/2.0, 
                                          fSiPMPadHeight/2.0,
                                          0.1*mm/2),
                                          FindMaterial("TPB"),
                                          WLSName);

        physWLS[i] = new G4PVPlacement(0,
                                       G4ThreeVector(0,0,-3.5*mm/2.0+0.1*mm/2.0),
                                       logicWLS[i],
                                       WLSName,
                                       logicSiPMPads[i],
                                       false,
                                       0,fCheckOverlaps);

        nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(WLSName, physWLS[i], i);
      }

      //**************************************************//
      //******************** ActiveSiPM ******************//
      //**************************************************//
      G4String ActiveSiPMName = GetName() + "/ActiveSiPMPads";
      G4double ActiveSiPMPadHeight = fSiPMPadHeight*sqrt(fActiveSiPMPadFrac);

      logicActiveSiPMPads[i] = new G4LogicalVolume(new G4Box(ActiveSiPMName,
                                                   ActiveSiPMPadHeight/2.0,
                                                   ActiveSiPMPadHeight/2.0,
                                                   0.001*mm/2.0),
                                                   FindMaterial("G4_Si"),
                                                   ActiveSiPMName);

      physActiveSiPMPads[i] = new G4PVPlacement(0,
                                                G4ThreeVector(0,0,-3.5*mm/2.0+1*mm+0.001*mm/2.0),
                                                logicActiveSiPMPads[i],
                                                ActiveSiPMName,
                                                logicSiPMPads[i],
                                                false,
                                                0,fCheckOverlaps);

      nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(ActiveSiPMName, physActiveSiPMPads[i], i);

      //**************************************************//
      //******************* SiPMPadBase *************//
      //**************************************************//
      G4String SiPMPadBaseName = GetName() + "/SiPMPadBase";
      G4double SiPMPadBaseHeight = ActiveSiPMPadHeight;

      logicSiPMPadBase[i] = new G4LogicalVolume(new G4Box(SiPMPadBaseName,
                            SiPMPadBaseHeight/2.0,
                            SiPMPadBaseHeight/2.0,
                            0.5*mm/2.0),
                            FindMaterial("G4_Si"),
                            SiPMPadBaseName);
      physSiPMBase[i] = new G4PVPlacement(0,
                                          G4ThreeVector(0,0,-3.5*mm/2.0+1*mm+0.001*mm+0.5*mm/2.0),
                                          logicSiPMPadBase[i],
                                          SiPMPadBaseName,
                                          logicSiPMPads[i],
                                          false,
                                          0,fCheckOverlaps);
      nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(SiPMPadBaseName, physSiPMBase[i], i);
    }
    G4VisAttributes* SiPMAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));  
    SiPMAtt->SetVisibility(false);
    G4VisAttributes* ActiveSiPMAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));  
    ActiveSiPMAtt->SetVisibility(false);
    G4VisAttributes* SiPMBaseAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 0.5));  
    SiPMBaseAtt->SetVisibility(false);
    //SiPMAtt->SetForceSolid(true);
    //logicSiPMs->SetVisAttributes(SiPMAtt);
    for(G4int i = 0; i < fNSiPMPadAlongZ; i++) {
      logicSiPMPads[i]->SetVisAttributes(SiPMAtt);
      logicActiveSiPMPads[i]->SetVisAttributes(ActiveSiPMAtt);
      logicSiPMPadBase[i]->SetVisAttributes(SiPMBaseAtt);
    }
  }

  if(fSiPMOption == 1) {  // Obsolete as of Apr 2015
    //************************************************//
    //************** Teflon Reflector ****************//
    //************************************************//
    G4String teflonReflectorName = GetName() + "/TReflector";
    G4double teflonReflectorOR = fieldRingSweptR - fFieldRingLength/2.0;
    G4double teflonReflectorIR = teflonReflectorOR - 1*mm;
    G4double teflonReflectorH = fCathodeAnodeDistance - 1.13*cm;

    G4LogicalVolume* logicTReflector =
                     new G4LogicalVolume(new G4Tubs(teflonReflectorName,
                                                    teflonReflectorIR,
                                                    teflonReflectorOR,
                                                    teflonReflectorH/2.0,
                                                    0*deg, 360*deg),
                                         FindMaterial("G4_TEFLON"),
                                         teflonReflectorName);

    G4VPhysicalVolume* physTReflector = new G4PVPlacement(0,
                                                          G4ThreeVector(),
                                                          logicTReflector,
                                                          teflonReflectorName,
                                                          logicLXe,
                                                          false,
                                                          0,fCheckOverlaps);

    nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(teflonReflectorName, physTReflector);

    G4VisAttributes* TRefAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
    TRefAtt->SetVisibility(false);
    logicTReflector->SetVisAttributes(TRefAtt);
  }

  G4cout << "//##################################################//" << G4endl;
  G4cout << "//################# Anode Platters #################//" << G4endl;
  G4cout << "//##################################################//" << G4endl;

  if(fSiPMOption == 0 && fAnodeOption == 0) {  // Baseline Apr 2015
    G4String AnodeName = GetName() + "/Anode";

    G4LogicalVolume* logicAnode = new G4LogicalVolume(new G4Tubs(AnodeName,
                                                                 0.*mm, fAnodeRadius,
                                                                 fAnodeThickness/2.,
                                                                 0*deg,360*deg),
                                                      FindMaterial("G4_SILICON_DIOXIDE"),
                                                      AnodeName);

    G4VPhysicalVolume* physAnode = new G4PVPlacement(0,
                                                     G4ThreeVector(0,0, fAnodeZPosition),
                                                     logicAnode,
                                                     AnodeName,
                                                     logicLXe,
                                                     false,
                                                     0,fCheckOverlaps);

    nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(AnodeName,physAnode);   

    G4VisAttributes* AnodeAtt = new G4VisAttributes(G4Colour(0.5, 1.0, 0.5));
    AnodeAtt->SetVisibility(false);
    //APDPlattAtt->SetForceSolid(true);
    logicAnode->SetVisAttributes(AnodeAtt);
  }

  // Construct charge tiles in place of simple anode disk.
  if(fSiPMOption == 0 && fAnodeOption == 1) {
    // The charge detector consists of tiles (aka module).
    // Each module has a quartz backing.
    // Each module contains chains of pads along X or Y direction, on top of the backing.
    // Pads in the same chain are connected by links.
  
    G4String AnodeName = GetName() + "/Anode";

    // Construct "anode": imaginary LXe volumes for holding charge tiles. 
    G4Box* AnodeSolid = new G4Box(AnodeName, fChargeModuleLength/2., fChargeModuleLength/2., fChargeModuleThickness/2.);
    G4LogicalVolume* AnodeLogical = new G4LogicalVolume(AnodeSolid, FindMaterial("liquidXe"),AnodeName);
    G4int moduleNumber = 0;
    for(G4int i = 0; i <= fTPCLowerRadius/fChargeModuleLength; i++){
      for(G4int j = 0; ((i+1)*fChargeModuleLength)*((i+1)*fChargeModuleLength)+((j+1)*fChargeModuleLength)*((j+1)*fChargeModuleLength) < fTPCLowerRadius*fTPCLowerRadius; j++){
        //if(((i+1)*fChargeModuleLength)*((i+1)*fChargeModuleLength)+((j+1)*fChargeModuleLength)*((j+1)*fChargeModuleLength)<fTPCLowerRadius*fTPCLowerRadius){
          G4VPhysicalVolume* AnodePhysical;
          AnodePhysical = new G4PVPlacement(0,
              G4ThreeVector((i+0.5)*fChargeModuleLength, (j+0.5)*fChargeModuleLength, fAnodeZPosition),
              AnodeLogical,
              AnodeName,
              logicLXe,
              false,
              //(i+1)*100+j+1,
              moduleNumber++,
              fCheckOverlaps);
          AnodePhysical = new G4PVPlacement(0,
              G4ThreeVector((-i-0.5)*fChargeModuleLength, (j+0.5)*fChargeModuleLength, fAnodeZPosition),
              AnodeLogical,
              AnodeName,
              logicLXe,
              false,
              //(i+1)*1000+j+1,fCheckOverlaps);
              moduleNumber++,
              fCheckOverlaps);
          AnodePhysical = new G4PVPlacement(0,
              G4ThreeVector((-i-0.5)*fChargeModuleLength, (-j-0.5)*fChargeModuleLength, fAnodeZPosition),
              AnodeLogical,
              AnodeName,
              logicLXe,
              false,
              //(i+1)*10000+j+1,fCheckOverlaps);
              moduleNumber++,
              fCheckOverlaps);
          AnodePhysical = new G4PVPlacement(0,
              G4ThreeVector((i+0.5)*fChargeModuleLength, (-j-0.5)*fChargeModuleLength, fAnodeZPosition),
              AnodeLogical,
              AnodeName,
              logicLXe,
              false,
              //(i+1)*100000+j+1,fCheckOverlaps);
              moduleNumber++,
              fCheckOverlaps);
      }
    }

    // Construct quartz layer as charge tiles backing
    G4String layerName = AnodeName + "/ModuleBacking";
    G4Box* layerSolid = new G4Box("layer", fChargeModuleLength/2, fChargeModuleLength/2, fChargeModuleBackingThickness/2.);
    G4LogicalVolume* layerLogical = new G4LogicalVolume(layerSolid, FindMaterial("G4_SILICON_DIOXIDE"),layerName);
    G4VPhysicalVolume* layerPhysical = new G4PVPlacement(0,
        G4ThreeVector(0, 0, -fChargeModuleBackingThickness/2.),
        layerLogical,
        layerName,
        AnodeLogical,
        false,
        0,fCheckOverlaps);

    // Construct charge pads
    G4String padName = AnodeName + "/ChargePad";
    G4double fPadLength = (fChargePadDiagonalLength/2. - fChargePadLinkLength/2.)/sqrt(2);
    G4Box* padSolid = new G4Box("pad", fPadLength, fPadLength, fChargePadThickness/2.);
    G4LogicalVolume* padLogical = new G4LogicalVolume(padSolid, FindMaterial("G4_Au"),padName);
    // -- rotate pads by 45 degrees
    G4RotationMatrix* padAxis = new G4RotationMatrix;
    padAxis->rotateZ(M_PI/4.*rad);
    // -- put pads in place
    G4int padNumber = 0;
    for(G4int i=0;i<(fChargeModuleLength-fChargePadDiagonalLength/2.)/(2*fChargePadDiagonalLength/2.);i++){
      for(G4int j=0;j<(fChargeModuleLength-fChargePadDiagonalLength/2.)/(2*fChargePadDiagonalLength/2.);j++){

        // Pads in X-channel
        G4VPhysicalVolume* padxPhysical = new G4PVPlacement(padAxis,
            G4ThreeVector(-fChargeModuleLength/2+fChargePadDiagonalLength/2.-fChargePadLinkLength/2.+i*2*fChargePadDiagonalLength/2.,
                          -fChargeModuleLength/2+fChargePadDiagonalLength/2.-fChargePadLinkLength/2.+j*2*fChargePadDiagonalLength/2.,
                          fChargePadThickness/2.),
            padLogical,
            padName,
            AnodeLogical,
            false,
            //10000+i*100+j,fCheckOverlaps);
            padNumber++,
            fCheckOverlaps);

        // Pads in Y-channel  
        G4VPhysicalVolume* padyPhysical = new G4PVPlacement(padAxis,
            G4ThreeVector(-fChargeModuleLength/2+fChargePadDiagonalLength/2.-fChargePadLinkLength/2.+(j+0.5)*2*fChargePadDiagonalLength/2.,
                          -fChargeModuleLength/2+fChargePadDiagonalLength/2.-fChargePadLinkLength/2.+(i+0.5)*2*fChargePadDiagonalLength/2.,
                          fChargePadLinkHeight/2.),
            padLogical,
            padName,
            AnodeLogical,
            false,
            //20000+i*100+j,fCheckOverlaps);
            padNumber++,
            fCheckOverlaps);
      }
    }

    // Construct links between pads
    G4String linkName = AnodeName + "/ChargePadLink";
    G4double fPadLinkY2Length = 2*fChargePadLinkHeight/2. + fChargePadLinkLength/2.;
    G4Trd* linkSolid = new G4Trd("link", fChargePadThickness/2., fChargePadThickness/2., fChargePadLinkLength/2., fPadLinkY2Length, fChargePadLinkHeight/2.);
    G4LogicalVolume* linkLogical = new G4LogicalVolume(linkSolid, FindMaterial("G4_Au"), linkName);

    // -- rotations for links
    G4RotationMatrix* linkx1 = new G4RotationMatrix;
    linkx1->rotateY(M_PI/2.*rad);
    G4RotationMatrix* linkx2 = new G4RotationMatrix;
    linkx2->rotateY(-M_PI/2.*rad);
    G4RotationMatrix* linky1 = new G4RotationMatrix;
    linky1->rotateZ(M_PI/2.*rad);
    linky1->rotateY(M_PI/2.*rad);
    G4RotationMatrix* linky2 = new G4RotationMatrix;
    linky2->rotateZ(M_PI/2.*rad);
    linky2->rotateY(-M_PI/2.*rad);

    // -- put links in place
    G4int linkNumber = 0;
    for(G4int i=0;i<(fChargeModuleLength-fChargePadDiagonalLength/2.)/(2*fChargePadDiagonalLength/2.);i++){
      for(G4int j=0;j<(fChargeModuleLength-fChargePadDiagonalLength/2.)/(2*fChargePadDiagonalLength/2.)-1;j++){  

        // Create links between x-pads as two trapezoid halves
        G4VPhysicalVolume* linkx1Physical = new G4PVPlacement(linkx1,
            G4ThreeVector(-fChargeModuleLength/2.+fChargePadDiagonalLength/2.-fChargePadLinkLength/2.+(i+0.5)*2*fChargePadDiagonalLength/2.-fChargePadLinkHeight/2., 
                          -fChargeModuleLength/2.+fChargePadDiagonalLength/2.-fChargePadLinkLength/2.+(j+1)*2*fChargePadDiagonalLength/2.,
                          fChargePadThickness/2.),
            linkLogical,
            linkName,
            AnodeLogical,
            false,
            //30000+i*100+j,fCheckOverlaps);
            linkNumber++,
            fCheckOverlaps);
        G4VPhysicalVolume* linkx2Physical = new G4PVPlacement(linkx2,
            G4ThreeVector(-fChargeModuleLength/2.+fChargePadDiagonalLength/2.-fChargePadLinkLength/2.+(i+0.5)*2*fChargePadDiagonalLength/2.+fChargePadLinkHeight/2.,
                          -fChargeModuleLength/2.+fChargePadDiagonalLength/2.-fChargePadLinkLength/2.+(j+1)*2*fChargePadDiagonalLength/2.,
                          fChargePadThickness/2.),
            linkLogical,
            linkName,
            AnodeLogical,
            false,
            //30000+i*100+j,fCheckOverlaps);
            linkNumber++,
            fCheckOverlaps);

        // Create links between y-pads as two trapezoid halves
        G4VPhysicalVolume* linky1Physical = new G4PVPlacement(linky1,
            G4ThreeVector(-fChargeModuleLength/2.+fChargePadDiagonalLength/2.-fChargePadLinkLength/2.+(j+0.5)*2*fChargePadDiagonalLength/2.,
                          -fChargeModuleLength/2.+fChargePadDiagonalLength/2.-fChargePadLinkLength/2.+i*2*fChargePadDiagonalLength/2.+fChargePadLinkHeight/2.,
                          fChargePadThickness/2.),
            linkLogical,
            linkName,
            AnodeLogical,
            false,
            //40000+i*100+j,fCheckOverlaps);
            linkNumber++,
            fCheckOverlaps);
        G4VPhysicalVolume* linky2Physical = new G4PVPlacement(linky2,
            G4ThreeVector(-fChargeModuleLength/2.+fChargePadDiagonalLength/2.-fChargePadLinkLength/2.+(j+0.5)*2*fChargePadDiagonalLength/2.,
                          -fChargeModuleLength/2.+fChargePadDiagonalLength/2.-fChargePadLinkLength/2.+i*2*fChargePadDiagonalLength/2.-fChargePadLinkHeight/2.,
                          fChargePadThickness/2.),
            linkLogical,
            linkName,
            AnodeLogical,
            false,
            //40000+i*100+j,fCheckOverlaps);
            linkNumber++,
            fCheckOverlaps);
      }
    }

    G4VisAttributes* AnodeVisAtt= new G4VisAttributes(G4Colour(0.0,0.0,1.0));
    G4VisAttributes* layerVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
    G4VisAttributes* padVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
    G4VisAttributes* linkVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
    AnodeVisAtt->SetVisibility(true);
    layerVisAtt->SetVisibility(true);
    padVisAtt->SetVisibility(true);
    linkVisAtt->SetVisibility(true);
    AnodeLogical->SetVisAttributes(AnodeVisAtt);
    layerLogical->SetVisAttributes(layerVisAtt);
    padLogical  ->SetVisAttributes(padVisAtt);
    linkLogical->SetVisAttributes(linkVisAtt);
  }

  if(fSiPMOption == 1) {  // Obsolete as of Apr 2015
    G4String AnodeName = GetName() + "/Anode";
    G4LogicalVolume* logicAnode[10000] = {0};
    G4VPhysicalVolume* physAnode[10000] = {0};

    G4double nWires = (G4int) (639.15*mm*2.0/fWWDistanceOfAnode);
    G4int iwire = 0;
    for(G4int i = 0; i < nWires + 1; i++) {
      G4double zHeight = sqrt(645.0*mm*645.0*mm - (639.15*mm - i*fWWDistanceOfAnode)*(639.15*mm - i*fWWDistanceOfAnode));
      logicAnode[i] = new G4LogicalVolume(new G4Tubs(AnodeName,
                                                     0, 0.125/2.0*mm,
                                                     zHeight,
                                                     0*deg,360*deg),
                                          FindMaterial("G4_SILICON_DIOXIDE"),
                                          AnodeName);
      G4RotationMatrix* rotwireY = new G4RotationMatrix();
      rotwireY->rotateY(90*degree);
      physAnode[iwire] = new G4PVPlacement(rotwireY,
                                           G4ThreeVector(0, -639.15*mm+i*fWWDistanceOfAnode, fAnodeZPosition+ 1.5*mm),
                                           logicAnode[i],
                                           AnodeName,
                                           logicLXe,
                                           false,
                                           iwire,fCheckOverlaps);

      nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(AnodeName, physAnode[iwire], iwire);   
      iwire++;

      G4RotationMatrix* rotwireX = new G4RotationMatrix();
      rotwireX->rotateX(90*degree);
      physAnode[iwire] = new G4PVPlacement(rotwireX,
                                           G4ThreeVector(-639.15*mm+i*fWWDistanceOfAnode, 0, fAnodeZPosition+1.5*mm-6*mm),
                                           logicAnode[i],
                                           AnodeName,
                                           logicLXe,
                                           false,
                                           iwire,fCheckOverlaps);

      nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(AnodeName, physAnode[iwire], iwire);   
      iwire++;
    }
    G4VisAttributes* AnodeAtt = new G4VisAttributes(G4Colour(0.5, 1.0, 0.5));
    AnodeAtt->SetVisibility(false);
    for(G4int i = 0; i < nWires + 1; i++) {
      logicAnode[i]->SetVisAttributes(AnodeAtt);
    }
  }
  
  G4cout << "//##################################################//" << G4endl;
  G4cout << "//################# Cathode Platters ###############//" << G4endl;
  G4cout << "//##################################################//" << G4endl;

  G4String CathodeName = GetName() + "/Cathode";

  G4LogicalVolume* logicCathode = new G4LogicalVolume(new G4Tubs(CathodeName,
                                                                 0.*mm, fCathodeRadius,
                                                                 fCathodeThickness/2.,
                                                                 0*deg,360*deg),
                                                      FindMaterial("G4_Cu"),
                                                      CathodeName);
  
  G4VPhysicalVolume* physCathode = new G4PVPlacement(0,
                                                     G4ThreeVector(0,0, fCathodeZPosition),
                                                     logicCathode,
                                                     CathodeName,
                                                     logicLXe,
                                                     false,
                                                     0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(CathodeName,physCathode);   

  G4VisAttributes* CathodeAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 1.0));
  CathodeAtt->SetVisibility(false);
  logicCathode->SetVisAttributes(CathodeAtt);

  G4cout << "//##################################################//" << G4endl;
  G4cout << "//#################### Bulge #######################//" << G4endl;
  G4cout << "//##################################################//" << G4endl;

  G4String BulgeName = GetName() + "/Bulge";
                                       
  G4LogicalVolume* logicBulge = new G4LogicalVolume(new G4Torus(BulgeName,
                                                  0.*mm, fBulgeRadius,
                                                  fCathodeRadius,
                                                  0,360*deg),
                                       FindMaterial("G4_Al"),
                                       BulgeName);
                    
  G4VPhysicalVolume* physBulge = new G4PVPlacement(0,
                                                   G4ThreeVector(0,0,fCathodeZPosition-fCathodeThickness/2.-fBulgeRadius),
                                                   logicBulge,
                                                   BulgeName,
                                                   logicLXe,
                                                   false,
                                                   0,fCheckOverlaps);

  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(BulgeName,physBulge);   
  
  G4VisAttributes* BulgeAtt = new G4VisAttributes(G4Colour(0, 0, 1.0));
  BulgeAtt->SetVisibility(false);
  //BulgeAtt->SetForceSolid(true);
  logicBulge->SetVisAttributes(BulgeAtt);

  //G4cout << "SiPMs along z = " << fNSiPMPadAlongZ << " phi = " << fNSiPMPadAlongPhi << " x = " << fNSiPMPadAlongX << " y = " << fNSiPMPadAlongY << G4endl;
  //for(int i = 0; i < fNSiPMPadAlongZ; i++)
  // {
  //  G4cout << "Phi SiPMs along z = " << i << " is " << fNSiPMPadAlongPhi[i] << G4endl;
  //}
}

void nEXOTPCInternalsConstructor::SetOpticalSurfaces()
{
 
  G4double* RefSpecularlobe = new  G4double[2];
  RefSpecularlobe[0] = 1.0;
  RefSpecularlobe[1] = 1.0;
  G4double* RefSpecularspike = new  G4double[2];
  RefSpecularspike[0] = 0.0;
  RefSpecularspike[1] = 0.0;
  G4double* RefBackscatter = new  G4double[2];
  RefBackscatter[0] = 0.0;
  RefBackscatter[1] = 0.0;
  
  G4double* fieldRingReflectivity = new  G4double[2];
  fieldRingReflectivity[0] = fFieldRingRef;
  fieldRingReflectivity[1] = fFieldRingRef;

  G4double* anodeReflectivity = new  G4double[2];
  anodeReflectivity[0] = fAnodeRef;
  anodeReflectivity[1] = fAnodeRef;

  G4double* cathodeReflectivity = new  G4double[2];
  cathodeReflectivity[0] = fCathodeRef;
  cathodeReflectivity[1] = fCathodeRef;

  G4double* bulgeReflectivity = new  G4double[2];
  bulgeReflectivity[0] = fBulgeRef;
  bulgeReflectivity[1] = fBulgeRef;

  G4double* tpcVesselReflectivity = new  G4double[2];
  tpcVesselReflectivity[0] = fTPCVesselRef;
  tpcVesselReflectivity[1] = fTPCVesselRef;

  G4double* teflonReflectivity = new  G4double[2];
  teflonReflectivity[0] = fTeflonRef;
  teflonReflectivity[1] = fTeflonRef;

  std::cout << "Reflectivity of field ring is " << fFieldRingRef << std::endl;
  std::cout << "Reflectivity of anode is " << fAnodeRef << std::endl;
  std::cout << "Reflectivity of cathode is " << fCathodeRef << std::endl;
  std::cout << "Reflectivity of bulge is " << fBulgeRef << std::endl;
  std::cout << "Reflectivity of teflon is " << fTeflonRef << std::endl;
  //std::cout << "Reflectivity of TPC vessel is " << fTPCVesselRef << std::endl;

  G4double* photonULEnergies = new G4double[2];
  photonULEnergies[0] = (4.13566743E-15*299792458/(144.5*1.E-9))*eV; 
  photonULEnergies[1] = (4.13566743E-15*299792458/(2058.2*1.E-9))*eV;

  //Optical SiO2 and Pd Surfaces
  const G4int nSiEntries = 44;
  //cubic silicon
  G4double SiliconPhotonEnergy[nSiEntries] =
            { 0.602*eV, 0.689*eV, 1.03*eV,  1.926*eV,
              2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
              2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
              2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
              2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
              2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
              3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
              3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
              3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV,
              4.500*eV, 4.960*eV, 5.520*eV, 6.250*eV,
              7.000*eV, 8.250*eV, 10.00*eV, 12.50*eV };
  G4double siliconRealRefractiveIndex[nSiEntries] =
            { 3.43,  3.44,  3.495, 3.861,
              3.916, 3.939, 3.962, 3.986,
              4.015, 4.015, 4.077, 4.11,
              4.15, 4.192, 4.239, 4.293,
              4.348, 4.416, 4.491, 4.577,
              4.676, 4.793, 4.925, 5.091,
              5.305, 5.587, 5.976, 6.548,
              6.863, 6.014, 5.483, 5.293,
              5.179, 5.102, 5.074, 5.055,
              2.451, 1.580, 1.319, 0.968,
              0.682, 0.444, 0.306, 0.255 };
  G4double siliconImaginaryRefractiveIndex[nSiEntries] =
            { 0.001, 0.001, 0.001, 0.015, 
              0.018, 0.02, 0.021, 0.023,
              0.024, 0.024, 0.028, 0.03,
              0.033, 0.036, 0.039, 0.045,
              0.05, 0.057, 0.064, 0.077,
              0.091, 0.109, 0.134, 0.167,
              0.22, 0.303, 0.465, 0.885,
              2.051, 2.912, 2.904, 2.951,
              3.085, 3.269, 3.559, 4.128,
              5.082, 3.632, 3.285, 2.89,
              2.45, 1.90, 1.38, 0.875 };

  //set optical surface for SiPMPadBase and SiO2
  G4OpticalSurface* SiPMBaseRefOpSurface = new G4OpticalSurface("SiPMBaseRefOpSurface");
  SiPMBaseRefOpSurface->SetModel(unified);
  SiPMBaseRefOpSurface->SetType(dielectric_metal);
  SiPMBaseRefOpSurface->SetFinish(polished);
  
  G4OpticalSurface* ActiveSiPMRefOpSurface = new G4OpticalSurface("ActiveSiPMRefOpSurface");
  ActiveSiPMRefOpSurface->SetModel(unified);
  ActiveSiPMRefOpSurface->SetType(dielectric_metal);
  ActiveSiPMRefOpSurface->SetFinish(polished);

  G4int iSiPMPad = 0; 
  if(fSiPMOption == 0) {
    iSiPMPad = 0;
    for(G4int i = 0; i < fNSiPMPadAlongZ; i++) {
      for(G4int j = 0; j < fNSiPMPadAlongPhi[i]; j++) {
        new G4LogicalBorderSurface("SiPMBaseRefSurface",
                                   nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPads", iSiPMPad), // physiSiPMPads[iSiPMPad],
                                   nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPadBase", i),     // physiSiPMBase[i],
                                   SiPMBaseRefOpSurface);
        new G4LogicalBorderSurface("SiPMBaseRefSurface",
                                   nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPadBase", i),     // physiSiPMBase[i],
                                   nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPads", iSiPMPad), // physiSiPMPads[iSiPMPad],
                                   SiPMBaseRefOpSurface);

        new G4LogicalBorderSurface("ActiveSiPMRefSurface",
                                   nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPads", iSiPMPad), // physiSiPMPads[iSiPMPad],
                                   nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/ActiveSiPMPads", i),  // physiActiveSiPMPads[i],
                                   ActiveSiPMRefOpSurface);
        new G4LogicalBorderSurface("ActiveSiPMRefSurface",
                                   nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/ActiveSiPMPads", i),  // physiActiveSiPMPads[i],
                                   nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPads", iSiPMPad), // physiSiPMPads[iSiPMPad],
                                   ActiveSiPMRefOpSurface);
        iSiPMPad++;
      }
    }           
  }
  
  if(fSiPMOption == 1) {
    iSiPMPad = 0;
    for(G4int i = 0; i < fNSiPMPadAlongY + 1; i++) {
      for(G4int j = 0; j < fNSiPMPadAlongX[i] + 1; j++) {
        new G4LogicalBorderSurface("SiPMBaseRefSurface",
                                   nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPads", iSiPMPad),
                                   nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPadBase", i),
                                   SiPMBaseRefOpSurface);
        new G4LogicalBorderSurface("SiPMBaseRefSurface",
                                   nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPadBase", i),
                                   nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPads", iSiPMPad),
                                   SiPMBaseRefOpSurface);

        new G4LogicalBorderSurface("ActiveSiPMRefSurface",
                                   nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPads", iSiPMPad),
                                   nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/ActiveSiPMPads", i),
                                   ActiveSiPMRefOpSurface);
        new G4LogicalBorderSurface("ActiveSiPMRefSurface",
                                   nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/ActiveSiPMPads", i),
                                   nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPads", iSiPMPad),
                                   ActiveSiPMRefOpSurface);
        iSiPMPad++;
        if(j > 0) {
          new G4LogicalBorderSurface("SiPMBaseRefSurface",
                                     nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPads", iSiPMPad),
                                     nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPadBase", i),
                                     SiPMBaseRefOpSurface);
          new G4LogicalBorderSurface("SiPMBaseRefSurface",
                                     nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPadBase", i),
                                     nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPads", iSiPMPad),
                                     SiPMBaseRefOpSurface);

          new G4LogicalBorderSurface("ActiveSiPMRefSurface",
                                     nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPads", iSiPMPad),
                                     nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/ActiveSiPMPads", i),
                                     ActiveSiPMRefOpSurface);
          new G4LogicalBorderSurface("ActiveSiPMRefSurface",
                                     nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/ActiveSiPMPads", i),
                                     nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPads", iSiPMPad),
                                     ActiveSiPMRefOpSurface);
          iSiPMPad++;
        }
      }
      if(i > 0) {
        for(G4int j = 0; j < fNSiPMPadAlongX[i] + 1; j++) {
          new G4LogicalBorderSurface("SiPMBaseRefSurface",
                                     nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPads", iSiPMPad),
                                     nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPadBase", i),
                                     SiPMBaseRefOpSurface);
          new G4LogicalBorderSurface("SiPMBaseRefSurface",
                                     nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPadBase", i),
                                     nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPads", iSiPMPad),
                                     SiPMBaseRefOpSurface);

          new G4LogicalBorderSurface("ActiveSiPMRefSurface",
                                     nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPads", iSiPMPad),
                                     nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/ActiveSiPMPads", i),
                                     ActiveSiPMRefOpSurface);
          new G4LogicalBorderSurface("ActiveSiPMRefSurface",
                                     nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/ActiveSiPMPads", i),
                                     nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPads", iSiPMPad),
                                     ActiveSiPMRefOpSurface);
          iSiPMPad++;
          if(j > 0) {
            new G4LogicalBorderSurface("SiPMBaseRefSurface",
                                       nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPads", iSiPMPad),
                                       nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPadBase", i),
                                       SiPMBaseRefOpSurface);
            new G4LogicalBorderSurface("SiPMBaseRefSurface",
                                       nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPadBase", i),
                                       nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPads", iSiPMPad),
                                       SiPMBaseRefOpSurface);

            new G4LogicalBorderSurface("ActiveSiPMRefSurface",
                                       nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPads", iSiPMPad),
                                       nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/ActiveSiPMPads", i),
                                       ActiveSiPMRefOpSurface);
            new G4LogicalBorderSurface("ActiveSiPMRefSurface",
                                       nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/ActiveSiPMPads", i),
                                       nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/SiPMPads", iSiPMPad),
                                       ActiveSiPMRefOpSurface);
            iSiPMPad++;
          }
        }
      }
    }
  }

  G4MaterialPropertiesTable* SiPMRefOpticalMPT  = new G4MaterialPropertiesTable();
  SiPMRefOpticalMPT->AddProperty("REALRINDEX", SiliconPhotonEnergy, siliconRealRefractiveIndex, nSiEntries);
  SiPMRefOpticalMPT->AddProperty("IMAGINARYRINDEX", SiliconPhotonEnergy, siliconImaginaryRefractiveIndex, nSiEntries);

  SiPMBaseRefOpSurface->SetMaterialPropertiesTable(SiPMRefOpticalMPT);
  ActiveSiPMRefOpSurface->SetMaterialPropertiesTable(SiPMRefOpticalMPT);

  //set optical surface for ActiveSiPMPad and SiO2

  G4VPhysicalVolume* physActiveLXe = nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/LXe/ActiveRegion");
  
  //set optical surface for field shaping rings
  G4OpticalSurface* fieldRingRefOpSurface = new G4OpticalSurface("fieldRingRefOpSurface");
  G4LogicalBorderSurface* fieldRingRefSurface[10000] = {0};
  for(int i = 0; i < 10000; i++) {
    if(nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/FieldRing",i) != NULL)
    fieldRingRefSurface[i] = new G4LogicalBorderSurface("fieldRingRefSurface",
                                                        physActiveLXe,
                                                        nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/FieldRing",i),//physiFieldRing[i],
                                                        fieldRingRefOpSurface);
  }
  fieldRingRefOpSurface->SetModel(unified);
  fieldRingRefOpSurface->SetType(dielectric_metal);
  fieldRingRefOpSurface->SetFinish(polished);

  G4MaterialPropertiesTable* fieldRingRefOpticalMPT  = new G4MaterialPropertiesTable();
  fieldRingRefOpticalMPT->AddProperty("REFLECTIVITY", photonULEnergies, fieldRingReflectivity, 2);
  //fieldRingRefOpticalMPT->AddProperty("SPECULARLOBECONSTANT", photonULEnergies, RefSpecularlobe, 2);
  //fieldRingRefOpticalMPT->AddProperty("SPECULARSPIKECONSTANT", photonULEnergies, RefSpecularspike, 2);
  //fieldRingRefOpticalMPT->AddProperty("BACKSCATTERCONSTANT", photonULEnergies, RefBackscatter, 2);

  fieldRingRefOpSurface->SetMaterialPropertiesTable(fieldRingRefOpticalMPT);
  
  //set optical surface for anode
  G4OpticalSurface* anodeRefOpSurface = new G4OpticalSurface("anodeRefOpSurface");
  if(fSiPMOption == 0) {
    G4LogicalBorderSurface* anodeRefSurface = NULL;
    anodeRefSurface = new G4LogicalBorderSurface("anodeRefSurface",
                                                 physActiveLXe,
                                                 nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/Anode"), // physiAnode,
                                                 anodeRefOpSurface);
  }
  
  if(fSiPMOption == 1) {
    G4LogicalBorderSurface* anodeRefSurface[100000] = {0};
    for(int i = 0; i < 100000; i++) {
      if(nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/Anode",i) == NULL) break;
      anodeRefSurface[i] = new G4LogicalBorderSurface("anodeRefSurface",
                                             physActiveLXe,
                                             nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/Anode",i),
                                             anodeRefOpSurface);
    }
  }

  anodeRefOpSurface->SetModel(unified);
  anodeRefOpSurface->SetType(dielectric_metal);
  anodeRefOpSurface->SetFinish(polished);

  G4MaterialPropertiesTable* anodeRefOpticalMPT  = new G4MaterialPropertiesTable();
  anodeRefOpticalMPT->AddProperty("REFLECTIVITY", photonULEnergies, anodeReflectivity, 2);
  //anodeRefOpticalMPT->AddProperty("SPECULARLOBECONSTANT", photonULEnergies, RefSpecularlobe, 2);
  //anodeRefOpticalMPT->AddProperty("SPECULARSPIKECONSTANT", photonULEnergies, RefSpecularspike, 2);
  //anodeRefOpticalMPT->AddProperty("BACKSCATTERCONSTANT", photonULEnergies, RefBackscatter, 2);

  anodeRefOpSurface->SetMaterialPropertiesTable(anodeRefOpticalMPT);

  //set optical surface for cathode
  G4OpticalSurface* cathodeRefOpSurface = new G4OpticalSurface("cathodeRefOpSurface");
  G4LogicalBorderSurface* cathodeRefSurface = NULL;
  cathodeRefSurface = new G4LogicalBorderSurface("cathodeRefSurface",
                                             physActiveLXe,
                                             nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/Cathode"), // physiCathode,
                                             cathodeRefOpSurface);

  cathodeRefOpSurface->SetModel(unified);
  cathodeRefOpSurface->SetType(dielectric_metal);
  cathodeRefOpSurface->SetFinish(polished);

  G4MaterialPropertiesTable* cathodeRefOpticalMPT  = new G4MaterialPropertiesTable();
  cathodeRefOpticalMPT->AddProperty("REFLECTIVITY", photonULEnergies, cathodeReflectivity, 2);
  //cathodeRefOpticalMPT->AddProperty("SPECULARLOBECONSTANT", photonULEnergies, RefSpecularlobe, 2);
  //cathodeRefOpticalMPT->AddProperty("SPECULARSPIKECONSTANT", photonULEnergies, RefSpecularspike, 2);
  //cathodeRefOpticalMPT->AddProperty("BACKSCATTERCONSTANT", photonULEnergies, RefBackscatter, 2);

  cathodeRefOpSurface->SetMaterialPropertiesTable(cathodeRefOpticalMPT);

  //set optical surface for bulge
  G4OpticalSurface* bulgeRefOpSurface = new G4OpticalSurface("bulgeRefOpSurface");
  G4LogicalBorderSurface* bulgeRefSurface = NULL;
  bulgeRefSurface = new G4LogicalBorderSurface("bulgeRefSurface",
                                             physActiveLXe,
                                             nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/Bulge"), // physiBulge,
                                             bulgeRefOpSurface);

  bulgeRefOpSurface->SetModel(unified);
  bulgeRefOpSurface->SetType(dielectric_metal);
  bulgeRefOpSurface->SetFinish(polished);

  G4MaterialPropertiesTable* bulgeRefOpticalMPT  = new G4MaterialPropertiesTable();
  bulgeRefOpticalMPT->AddProperty("REFLECTIVITY", photonULEnergies, bulgeReflectivity, 2);
  //bulgeRefOpticalMPT->AddProperty("SPECULARLOBECONSTANT", photonULEnergies, RefSpecularlobe, 2);
  //bulgeRefOpticalMPT->AddProperty("SPECULARSPIKECONSTANT", photonULEnergies, RefSpecularspike, 2);
  //bulgeRefOpticalMPT->AddProperty("BACKSCATTERCONSTANT", photonULEnergies, RefBackscatter, 2);

  bulgeRefOpSurface->SetMaterialPropertiesTable(bulgeRefOpticalMPT);

  if(fSiPMOption == 1) {
    //set optical surface for teflon reflector
    G4OpticalSurface* teflonRefOpSurface = new G4OpticalSurface("teflonRefOpSurface");
    G4LogicalBorderSurface* teflonRefSurface = NULL;
    teflonRefSurface = new G4LogicalBorderSurface("teflonRefSurface",
                                               physActiveLXe,
                                               nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName() + "/TReflector"),
                                               teflonRefOpSurface);

    teflonRefOpSurface->SetModel(unified);
    teflonRefOpSurface->SetType(dielectric_dielectric);
    teflonRefOpSurface->SetFinish(groundfrontpainted);

    G4MaterialPropertiesTable* teflonRefOpticalMPT  = new G4MaterialPropertiesTable();
    teflonRefOpticalMPT->AddProperty("REFLECTIVITY", photonULEnergies, teflonReflectivity, 2);

    teflonRefOpSurface->SetMaterialPropertiesTable(teflonRefOpticalMPT);
  }
}

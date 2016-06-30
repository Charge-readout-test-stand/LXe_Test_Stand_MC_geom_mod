#include "nEXOTPCExternalsConstructor.hh"
#include "G4UnionSolid.hh" 
#include "nEXOLXeConstructor.hh"
#include "nEXOTPCInternalsConstructor.hh"
#include "nEXOTPCVesselConstructor.hh"

class nEXOTPCExternalsMessenger : public nEXOModuleConstructorMessenger
{
private:
  nEXOTPCExternalsConstructor* fConstructor;

  G4UIcmdWithAnInteger* fTPCExternalsVersionCmd;
  
  //-----------------------------------Test Stand----------------------------------------
  G4UIcmdWithADoubleAndUnit* fHFERadiusCmd;
  G4UIcmdWithADoubleAndUnit* fHFEHeightCmd;
  G4UIcmdWithADoubleAndUnit* fLabDimensionCmd;
  G4UIcmdWithADoubleAndUnit* fHFEDewarThicknessCmd;
  //-------------------------------------------------------------------------------------


public:
  nEXOTPCExternalsMessenger(nEXOTPCExternalsConstructor* construct)
  : nEXOModuleConstructorMessenger(construct, "Control the geometry of the components outside of the nEXO TPC."),
    fConstructor(construct){

    fTPCExternalsVersionCmd = new G4UIcmdWithAnInteger(CommandName("setVersion"),this);
    fTPCExternalsVersionCmd->SetGuidance("The version of the TPC externals geometry.");
    fTPCExternalsVersionCmd->SetParameterName("setVersion",false);
    fTPCExternalsVersionCmd->SetDefaultValue(1);
    fTPCExternalsVersionCmd->AvailableForStates(G4State_PreInit);

    //-----------------------------------Test Stand----------------------------------------
    fHFERadiusCmd = new  G4UIcmdWithADoubleAndUnit(CommandName("HFERadius"),this);
    fHFERadiusCmd->SetGuidance("Define HFE Radius");
    fHFERadiusCmd->SetParameterName("HFERadius",false);
    fHFERadiusCmd->SetUnitCategory("Length");
    fHFERadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fHFEHeightCmd = new  G4UIcmdWithADoubleAndUnit(CommandName("HFEHeight"),this);
    fHFEHeightCmd->SetGuidance("Define HFE Height");
    fHFEHeightCmd->SetParameterName("HFEHeight",false);
    fHFEHeightCmd->SetUnitCategory("Length");
    fHFEHeightCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fLabDimensionCmd = new  G4UIcmdWithADoubleAndUnit(CommandName("LabDimension"),this);
    fLabDimensionCmd->SetGuidance("Define Lab Box Length/Width/Height");
    fLabDimensionCmd->SetParameterName("LabDimension",false);
    fLabDimensionCmd->SetUnitCategory("Length");
    fLabDimensionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fHFEDewarThicknessCmd = new  G4UIcmdWithADoubleAndUnit(CommandName("HFEDewarThickness"),this);
    fHFEDewarThicknessCmd->SetGuidance("Thickness of the HFE Dewar");
    fHFEDewarThicknessCmd->SetParameterName("HFEDewarThickness",false); 
    fHFEDewarThicknessCmd->SetUnitCategory("Length");
    fHFEDewarThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle); 
    //-------------------------------------------------------------------------------------


  };

  virtual ~nEXOTPCExternalsMessenger(){

    delete fTPCExternalsVersionCmd;

    //-----------------------------------Test Stand----------------------------------------
    delete fHFERadiusCmd;
    delete fHFEHeightCmd;
    delete fLabDimensionCmd;
    delete fHFEDewarThicknessCmd;
    //-------------------------------------------------------------------------------------
  };

  void SetNewValue(G4UIcommand *cmd, G4String val){
    //nEXOModuleConstructorMessenger::SetNewValue(cmd,val);

    if( cmd == fTPCExternalsVersionCmd) {
      fConstructor->SetVersion(fTPCExternalsVersionCmd->GetNewIntValue(val));
    }
    else if (cmd == fHFERadiusCmd){
        //-----------------------------------Test Stand----------------------------------------
        fConstructor->SetHFERadius(fHFERadiusCmd->GetNewDoubleValue(val));
    }
    else if (cmd == fHFEHeightCmd){
        //-----------------------------------Test Stand----------------------------------------
        fConstructor->SetHFEHeight(fHFEHeightCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fLabDimensionCmd){
        //-----------------------------------Test Stand----------------------------------------
        fConstructor->SetLabHeight(fLabDimensionCmd->GetNewDoubleValue(val));
        fConstructor->SetLabLength(fLabDimensionCmd->GetNewDoubleValue(val));
        fConstructor->SetLabWidth(fLabDimensionCmd->GetNewDoubleValue(val));
    }
    else if(cmd == fHFEDewarThicknessCmd){
        //-----------------------------------Test Stand----------------------------------------
        fConstructor->SetHFEDewarThickness(fHFEDewarThicknessCmd->GetNewDoubleValue(val));
    }
    else{
        nEXOModuleConstructorMessenger::SetNewValue(cmd,val);
    }

  };
};

nEXOTPCExternalsConstructor::~nEXOTPCExternalsConstructor(){;}

void nEXOTPCExternalsConstructor::Init(void)
{
  SetMessenger(new nEXOTPCExternalsMessenger(this));

  SetVersion(1);
  
  double radius = 955*cm;
  SetRadius1(radius);
  SetRadius2(radius);
  double length = 2*845*cm + radius;
  SetLength(length);

  fTPCVesselTranslation.setX(0.); fTPCVesselTranslation.setY(0.); fTPCVesselTranslation.setZ(0.);
  fTPCVesselMotherVolume = NULL;


  //-----------------------------------Test Stand----------------------------------------
  G4cout << "TestStand initialized HFE rad and height to 1cm" << G4endl;
  G4cout << "TestStand initialized Lab to be 100cm Cube" << G4endl;
  SetHFERadius(200*mm);
  SetHFEHeight(640*mm);
  SetLabHeight(10*m);
  SetLabWidth(10*m);
  SetLabLength(10*m);
  //-------------------------------------------------------------------------------------

  AddConstructor(new nEXOWaterTankConstructor("WaterShieldTank",this));
  
  AddConstructor(new nEXOOuterCryostatConstructor("OuterCryostat",this));
  AddConstructor(new nEXOInnerCryostatConstructor("InnerCryostat",this));

  AddConstructor(new nEXOOuterSphericalCryostatConstructor("OuterSphericalCryostat",this));
  AddConstructor(new nEXOInnerSphericalCryostatConstructor("InnerSphericalCryostat",this));
}

G4LogicalVolume* nEXOTPCExternalsConstructor::GetPiece(void)
{
  G4cout << "//************************************************//" << G4endl;
  G4cout << "//************* nEXO TPC Externals ***************//" << G4endl;
  G4cout << "//************************************************//" << G4endl;

  G4LogicalVolume *logicExternals = new G4LogicalVolume(new G4Cons(GetName(),
                                                                   0, GetRadius1(),
                                                                   0, GetRadius2(),
                                                                   GetLength()/2.,
                                                                   0, 360*deg),
                                                        FindMaterial("Rock"),
                                                        GetName());

  fTPCVesselMotherVolume = logicExternals;

  if(GetVersion() == 0)
    return GetPieceOld(logicExternals);

  if(GetVersion() == 1)
    return GetPieceV1(logicExternals);

  if(GetVersion() == 2)
    return GetPieceV2(logicExternals);

  if(GetVersion() == 3){
      //----------------------------------------------Test Stand-------------------------------------------
      G4cout << "Creating the lab which is just made of Air with Box l/w/h " << fLabWidth << " "  << fLabHeight << G4endl;
      G4LogicalVolume *logicExternalsTestStand = new G4LogicalVolume(new G4Box(GetName(),
                                                                       fLabLength/2.,
                                                                       fLabHeight/2.,
                                                                       fLabWidth/2.),
                                                                      FindMaterial("G4_AIR"),
                                                                      GetName());
      return GetPieceTestStand(logicExternalsTestStand);
  }

  return NULL;
}

G4LogicalVolume* nEXOTPCExternalsConstructor::GetPieceV1(G4LogicalVolume *logicExternals)
{
  // Water Tank Shield
  
  nEXOWaterTankConstructor& waterShieldTank = Get<nEXOWaterTankConstructor>("WaterShieldTank");
  waterShieldTank.SetCheckOverlaps(fCheckOverlaps);
  G4LogicalVolume* logicWST = waterShieldTank.GetPiece();

  G4VPhysicalVolume* physWST = new G4PVPlacement(0,
                                                 G4ThreeVector(0,0,0),
                                                 logicWST,
                                                 waterShieldTank.GetName(),
                                                 logicExternals,
                                                 false,
                                                 0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(waterShieldTank.GetName(),physWST);
  
  G4LogicalVolume* logicH2O = waterShieldTank.GetFillingPiece();
  double h2oOffset = waterShieldTank.GetVerticalOffset();

  G4VPhysicalVolume* physH2O = new G4PVPlacement(0,
                                                 G4ThreeVector(0,0,h2oOffset),
                                                 logicH2O,
                                                 waterShieldTank.GetName()+"/Water",
                                                 logicWST,
                                                 false,
                                                 0,fCheckOverlaps);
  
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(physH2O->GetName(),physH2O);

  G4VisAttributes* WaterAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
  //WaterAtt->SetVisibility(false);
  //WaterAtt->SetForceSolid(true);  
  logicWST->SetVisAttributes(WaterAtt);
  logicH2O->SetVisAttributes(WaterAtt);

  // Outer Cryostat
  
  nEXOOuterCryostatConstructor& outerCryo = Get<nEXOOuterCryostatConstructor>("OuterCryostat");
  outerCryo.SetCheckOverlaps(fCheckOverlaps);
  G4LogicalVolume* logicOC = outerCryo.GetPiece();

  G4VPhysicalVolume* physOC = new G4PVPlacement(0,
                                                G4ThreeVector(0,0,-outerCryo.GetMaxLength()/2.-h2oOffset),
                                                logicOC,
                                                outerCryo.GetName(),
                                                logicH2O,
                                                false,
                                                0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(outerCryo.GetName(),physOC);
  
  G4VisAttributes* ocAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));
  //WaterAtt->SetVisibility(false);
  //ocAtt->SetForceSolid(true);  
  logicOC->SetVisAttributes(ocAtt);

  G4LogicalVolume* logicVac = outerCryo.GetFillingPiece();
  G4VPhysicalVolume* physVac = new G4PVPlacement(0,
                                                 G4ThreeVector(0,0,outerCryo.GetThickness()),
                                                 logicVac,
                                                 outerCryo.GetName()+"/Vacuum",
                                                 logicOC,
                                                 false,
                                                 0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(physVac->GetName(),physVac);
  G4VisAttributes* vacAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
  //WaterAtt->SetVisibility(false);
  //vacAtt->SetForceSolid(true);  
  logicVac->SetVisAttributes(vacAtt);

  // Inner Cryostat

  nEXOInnerCryostatConstructor& innerCryo = Get<nEXOInnerCryostatConstructor>("InnerCryostat");
  innerCryo.SetCheckOverlaps(fCheckOverlaps);
  G4LogicalVolume* logicIC = innerCryo.GetPiece();

  G4VPhysicalVolume* physIC = new G4PVPlacement(0,
                                                G4ThreeVector(0,0,-outerCryo.GetThickness() + outerCryo.GetMaxLength() - innerCryo.GetMaxLength() - (outerCryo.GetRadius() - innerCryo.GetRadius())),
                                                logicIC,
                                                innerCryo.GetName(),
                                                logicVac,
                                                false,
                                                0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(innerCryo.GetName(),physIC);
  
  G4VisAttributes* icAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 2.0));
  //WaterAtt->SetVisibility(false);
  //icAtt->SetForceSolid(true);  
  logicIC->SetVisAttributes(icAtt);

  G4LogicalVolume* logicHFE = innerCryo.GetFillingPiece();
  G4VPhysicalVolume* physHFE = new G4PVPlacement(0,
                                                 G4ThreeVector(0,0,innerCryo.GetThickness()),
                                                 logicHFE,
                                                 innerCryo.GetName()+"/HFE",
                                                 logicIC,
                                                 false,
                                                 0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(physHFE->GetName(),physHFE);
  G4VisAttributes* hfeAtt = new G4VisAttributes(G4Colour(0.0, 2.0, 2.0));
  //WaterAtt->SetVisibility(false);
  //hfeAtt->SetForceSolid(true);  
  logicHFE->SetVisAttributes(hfeAtt);

  // Others

  fTPCVesselMotherVolume = logicHFE;
  fTPCVesselTranslation.setZ((innerCryo.GetMaxLength() - 2*innerCryo.GetThickness())/2.);

  G4Region*  CryostatRegion = new G4Region("CryostatRegion");
  CryostatRegion->AddRootLogicalVolume(logicOC);

  
  return logicExternals;
}

G4LogicalVolume* nEXOTPCExternalsConstructor::GetPieceV2(G4LogicalVolume *logicExternals)
{
  // Water Tank Shield
  
  nEXOWaterTankConstructor& waterShieldTank = Get<nEXOWaterTankConstructor>("WaterShieldTank");
  waterShieldTank.SetCheckOverlaps(fCheckOverlaps);
  G4LogicalVolume* logicWST = waterShieldTank.GetPiece();

  G4VPhysicalVolume* physWST = new G4PVPlacement(0,
                                                 G4ThreeVector(0,0,0),
                                                 logicWST,
                                                 waterShieldTank.GetName(),
                                                 logicExternals,
                                                 false,
                                                 0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(waterShieldTank.GetName(),physWST);
  
  G4LogicalVolume* logicH2O = waterShieldTank.GetFillingPiece();
  double h2oOffset = waterShieldTank.GetVerticalOffset();

  G4VPhysicalVolume* physH2O = new G4PVPlacement(0,
                                                 G4ThreeVector(0,0,h2oOffset),
                                                 logicH2O,
                                                 waterShieldTank.GetName()+"/Water",
                                                 logicWST,
                                                 false,
                                                 0,fCheckOverlaps);
  
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(physH2O->GetName(),physH2O);

  G4VisAttributes* WaterAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
  //WaterAtt->SetVisibility(false);
  //WaterAtt->SetForceSolid(true);  
  logicWST->SetVisAttributes(WaterAtt);
  logicH2O->SetVisAttributes(WaterAtt);

  // Outer Cryostat
  
  nEXOOuterSphericalCryostatConstructor& outerCryo = Get<nEXOOuterSphericalCryostatConstructor>("OuterSphericalCryostat");
  outerCryo.SetCheckOverlaps(fCheckOverlaps);
  G4LogicalVolume* logicOC = outerCryo.GetPiece();

  G4VPhysicalVolume* physOC = new G4PVPlacement(0,
                                                G4ThreeVector(0,0,-h2oOffset),
                                                logicOC,
                                                outerCryo.GetName(),
                                                logicH2O,
                                                false,
                                                0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(outerCryo.GetName(),physOC);
  
  G4VisAttributes* ocAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));
  //WaterAtt->SetVisibility(false);
  //ocAtt->SetForceSolid(true);  
  logicOC->SetVisAttributes(ocAtt);

  G4LogicalVolume* logicVac = outerCryo.GetFillingPiece();
  G4VPhysicalVolume* physVac = new G4PVPlacement(0,
                                                 G4ThreeVector(0,0,0),
                                                 logicVac,
                                                 outerCryo.GetName()+"/Vacuum",
                                                 logicOC,
                                                 false,
                                                 0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(physVac->GetName(),physVac);
  G4VisAttributes* vacAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
  //WaterAtt->SetVisibility(false);
  //vacAtt->SetForceSolid(true);  
  logicVac->SetVisAttributes(vacAtt);

  G4LogicalVolume* logicOS = outerCryo.GetSupportPiece();
  G4VPhysicalVolume* physOS = new G4PVPlacement(0,
                                                G4ThreeVector(0,0,1300*mm),
                                                logicOS,
                                                outerCryo.GetName()+"/Support",
                                                logicVac,
                                                false,
                                                0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(physOS->GetName(),physOS);
  G4VisAttributes* osAtt = new G4VisAttributes(G4Colour(2.0, 0.0, 0.0));
  //WaterAtt->SetVisibility(false);
  //hfeAtt->SetForceSolid(true);  
  logicOS->SetVisAttributes(osAtt);

  // Inner Cryostat

  nEXOInnerSphericalCryostatConstructor& innerCryo = Get<nEXOInnerSphericalCryostatConstructor>("InnerSphericalCryostat");
  innerCryo.SetCheckOverlaps(fCheckOverlaps);
  G4LogicalVolume* logicIC = innerCryo.GetPiece();

  G4VPhysicalVolume* physIC = new G4PVPlacement(0,
                                                G4ThreeVector(0,0,innerCryo.GetOffsetToOuter()),
                                                logicIC,
                                                innerCryo.GetName(),
                                                logicVac,
                                                false,
                                                0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(innerCryo.GetName(),physIC);
  
  G4VisAttributes* icAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 2.0));
  //WaterAtt->SetVisibility(false);
  //icAtt->SetForceSolid(true);  
  logicIC->SetVisAttributes(icAtt);

  G4LogicalVolume* logicHFE = innerCryo.GetFillingPiece();
  G4VPhysicalVolume* physHFE = new G4PVPlacement(0,
                                                 G4ThreeVector(0,0,0),
                                                 logicHFE,
                                                 innerCryo.GetName()+"/HFE",
                                                 logicIC,
                                                 false,
                                                 0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(physHFE->GetName(),physHFE);
  G4VisAttributes* hfeAtt = new G4VisAttributes(G4Colour(0.0, 2.0, 2.0));
  //WaterAtt->SetVisibility(false);
  //hfeAtt->SetForceSolid(true);  
  logicHFE->SetVisAttributes(hfeAtt);  

  // Others

  fTPCVesselMotherVolume = logicHFE;
  //fTPCVesselTranslation.setZ((innerCryo.GetMaxLength() - 2*innerCryo.GetThickness())/2.);

  G4Region*  CryostatRegion = new G4Region("CryostatRegion");
  CryostatRegion->AddRootLogicalVolume(logicOC);

  
  return logicExternals;
}

G4LogicalVolume* nEXOTPCExternalsConstructor::GetPieceOld(G4LogicalVolume *logicExternals)
{

  G4cout << "//************************************************//" << G4endl;
  G4cout << "//*************** SurroundRock *******************//" << G4endl;
  G4cout << "//************************************************//" << G4endl;

  G4String surroundRockName = GetName() + "/SurroundRock";
  G4double surroundRockR = 955*cm;
  G4double surroundRockH = 845*cm;  //Full height = 1690 cm
  G4LogicalVolume* logicSurroundRock = new G4LogicalVolume(new G4Tubs(surroundRockName,
                                                                      0, surroundRockR,
                                                                      surroundRockH,
								      0, 360*deg),
                                                           FindMaterial("Rock"),
                                                           surroundRockName);

  G4double surroundRockPlaceZ = -110*cm;
  G4VPhysicalVolume* physSurroundRock = new G4PVPlacement(0,
                                                          G4ThreeVector(0,0,surroundRockPlaceZ),
                                                          logicSurroundRock,
                                                          surroundRockName,
                                                          logicExternals,
                                                          false,
                                                          0,fCheckOverlaps);
  
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(surroundRockName,physSurroundRock);

  G4VisAttributes* MineAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
  MineAtt->SetVisibility(false);
  MineAtt->SetForceWireframe(true);
  logicSurroundRock->SetVisAttributes(MineAtt);


  G4cout << "//************************************************//" << G4endl;
  G4cout << "//************* Shotcrete Coating ****************//" << G4endl;
  G4cout << "//************************************************//" << G4endl;

  G4String shotcreteName = GetName() + "/Shotcrete";
  G4double shotcreteR = 755*cm;
  G4double shotcreteH = 735*cm;  //Full height = 1470 cm
  G4LogicalVolume* logicShotcrete =  new G4LogicalVolume(new G4Tubs(shotcreteName,
                                                                    0, shotcreteR,
                                                                    shotcreteH,
                                                                    0, 360*deg),
                                                         FindMaterial("ShotCrete"),
                                                         shotcreteName);

  G4double shotcretePlaceZ = surroundRockH - shotcreteH;
  G4VPhysicalVolume* physShotcrete = new G4PVPlacement(0,
                                                       G4ThreeVector(0,0,shotcretePlaceZ),
                                                       logicShotcrete,
                                                       shotcreteName,
                                                       logicSurroundRock,
                                                       false,
                                                       0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(shotcreteName,physShotcrete);
  logicShotcrete->SetVisAttributes(MineAtt);

  G4cout << "//************************************************//" << G4endl;
  G4cout << "//******************* Floor **********************//" << G4endl;
  G4cout << "//************************************************//" << G4endl;

  G4String floorName = GetName() + "/Floor";
  G4double floorR = shotcreteR;
  G4double floorH = 110*cm;  //Full height = 2.2m
  G4LogicalVolume* logicFloor = new G4LogicalVolume(new G4Tubs(floorName,
                                                               0, floorR,
                                                               floorH,
                                                               0, 360*deg),
                                                    FindMaterial("Concrete"),
                                                    floorName);

  G4double floorPlaceZ = floorH - surroundRockH;
  G4VPhysicalVolume* physFloor = new G4PVPlacement(0,
                                                   G4ThreeVector(0,0,floorPlaceZ),
                                                   logicFloor,
                                                   floorName,
                                                   logicSurroundRock,
                                                   false,
                                                   0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(floorName,physFloor);
  logicFloor->SetVisAttributes(MineAtt);

  //***************************************************//
  // Two separate water shields for calculating fluxes //
  // halfway through the water                         //
  //***************************************************//

  G4cout << "//************************************************//" << G4endl;
  G4cout << "//************* Outer Water Shield 1 *************//" << G4endl;
  G4cout << "//************************************************//" << G4endl;

  G4String outerWater1Name = GetName() + "/OuterWater1";
  G4double outerWater1R = 735.*cm;
  //  G4double outerWaterThick = 148.5*cm;  // Full thickness 297 cm
  G4double outerWater1H = 735.*cm;  // Full height = 1470 cm

  G4LogicalVolume* logicOuterWater1 = new G4LogicalVolume(new G4Tubs(outerWater1Name,
                                                                     0, outerWater1R,
                                                                     outerWater1H,
                                                                     0, 360*deg),
                                                          FindMaterial("G4_WATER"),
                                                          outerWater1Name);

  G4double outerWaterPlace1Z = shotcreteH - outerWater1H;
  G4VPhysicalVolume* physOuterWater1 = new G4PVPlacement(0,
                                                         G4ThreeVector(0,0,outerWaterPlace1Z),
                                                         logicOuterWater1,
                                                         outerWater1Name,
                                                         logicShotcrete,
                                                         false,
                                                         0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(outerWater1Name,physOuterWater1);
  G4VisAttributes* WaterAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
  WaterAtt->SetVisibility(false);
  //WaterAtt->SetForceSolid(true);  
  logicOuterWater1->SetVisAttributes(WaterAtt);

  G4cout << "//************************************************//" << G4endl;
  G4cout << "//************* Outer Water Shield 2 *************//" << G4endl;
  G4cout << "//************************************************//" << G4endl;

  G4String outerWater2Name = GetName() + "/OuterWater2";
  
  G4double HDPETankThick = 25.*cm;
  
  G4double innerWater1R = 440.*cm;
  G4double innerWater1H = 440.*cm;
  
  G4double HDPETankR = innerWater1R + HDPETankThick;
  G4double HDPETankH = innerWater1H + HDPETankThick;

  //halfway through OW tank

  G4double outerWater2R = (outerWater1R + HDPETankR)/2.;
  G4double outerWater2H = (outerWater1H + HDPETankH)/2.;

  G4LogicalVolume* logicOuterWater2 = new G4LogicalVolume(new G4Tubs(outerWater2Name,
                                                                     0, outerWater2R,
                                                                     outerWater2H,
                                                                     0, 360*deg),
                                                          FindMaterial("G4_WATER"),
                                                          outerWater2Name);

  G4double outerWaterPlace2Z = 0;
  G4VPhysicalVolume* physOuterWater2 = new G4PVPlacement(0,
                                                         G4ThreeVector(0,0,outerWaterPlace2Z),
                                                         logicOuterWater2,
                                                         outerWater2Name,
                                                         logicOuterWater1,
                                                         false,
                                                         0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(outerWater2Name,physOuterWater2);
  logicOuterWater2->SetVisAttributes(WaterAtt);

  G4cout << "//************************************************//" << G4endl;
  G4cout << "//****************** HDPE Tank *******************//" << G4endl;
  G4cout << "//************************************************//" << G4endl;

  G4String HDPETankName = GetName() + "/HDPETank";
  G4LogicalVolume* logicHDPETank = new G4LogicalVolume(new G4Tubs(HDPETankName,
                                                                  0, HDPETankR,
                                                                  HDPETankH,
                                                                  0, 360*deg),
                                                       FindMaterial("HDPE"),
                                                       HDPETankName);

  G4double HDPETankPlaceZ = 0.;
  G4VPhysicalVolume* physHDPETank = new G4PVPlacement(0,
                                                      G4ThreeVector(0,0, HDPETankPlaceZ),
                                                      logicHDPETank,
                                                      HDPETankName,
                                                      logicOuterWater2,
                                                      false,
                                                      0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(HDPETankName,physHDPETank);
  G4VisAttributes* HdpeAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
  HdpeAtt->SetVisibility(false);
  HdpeAtt->SetForceWireframe(true);
  //HdpeAtt->SetForceSolid(true);
  logicHDPETank->SetVisAttributes(HdpeAtt);

  G4cout << "//************************************************//" << G4endl;
  G4cout << "//************ Inner Water Shield 1 **************//" << G4endl;
  G4cout << "//************************************************//" << G4endl;  

  G4String innerWater1Name = GetName() + "/InnerWater1";
  G4LogicalVolume* logicInnerWater1 = new G4LogicalVolume(new G4Tubs(innerWater1Name,
                                                                     0, innerWater1R,
                                                                     innerWater1H,
                                                                     0, 360*deg),
                                                          FindMaterial("G4_WATER"),
                                                          innerWater1Name);

  G4double innerWater1PlaceZ = 0.;
  G4VPhysicalVolume* physInnerWater1 = new G4PVPlacement(0,
                                                         G4ThreeVector(0,0,innerWater1PlaceZ),
                                                         logicInnerWater1,
                                                         innerWater1Name,
                                                         logicHDPETank,
                                                         false,
                                                         0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(innerWater1Name,physInnerWater1);
  logicInnerWater1->SetVisAttributes(WaterAtt);

  G4cout << "//************************************************//" << G4endl;
  G4cout << "//************ Inner Water Shield 2 **************//" << G4endl;
  G4cout << "//************************************************//" << G4endl;  

  G4String innerWater2Name = GetName() + "/InnerWater2";
  G4double innerWater2R = 326.*cm;
  G4double innerWater2H = 326.*cm;

  G4LogicalVolume* logicInnerWater2 = new G4LogicalVolume(new G4Tubs(innerWater2Name,
                                                                     0, innerWater2R,
                                                                     innerWater2H,
                                                                     0, 360*deg),
                                                          FindMaterial("G4_WATER"),
                                                          innerWater2Name);

  G4double innerWater2PlaceZ = 0.;
  G4VPhysicalVolume* physInnerWater2 = new G4PVPlacement(0,
                                                         G4ThreeVector(0,0,innerWater2PlaceZ),
                                                         logicInnerWater2,
                                                         innerWater2Name,
                                                         logicInnerWater1,
                                                         false,
                                                         0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(innerWater2Name,physInnerWater2);
  logicInnerWater2->SetVisAttributes(WaterAtt);

  G4cout << "//************************************************//" << G4endl;
  G4cout << "//*************** Outer Cryostat *****************//" << G4endl;
  G4cout << "//************************************************//" << G4endl;

  G4String outerCryoName = GetName() + "/OuterCryo";
  G4double outerCryoR = 187*cm;
  G4double outerCryoH = 187*cm;
  G4LogicalVolume* logicOuterCryo = new G4LogicalVolume(new G4Tubs(outerCryoName,
                                                                   0, outerCryoR,
                                                                   outerCryoH,
                                                                   0, 360*deg),
                                                        FindMaterial("G4_Cu"),
                                                        outerCryoName);

  G4VPhysicalVolume* physOuterCryo = new G4PVPlacement(0,
                                                       G4ThreeVector(0,0,0),
                                                       logicOuterCryo,
                                                       outerCryoName,
                                                       logicInnerWater2,
                                                       false,
                                                       0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(outerCryoName,physOuterCryo);
  G4VisAttributes* CryoAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
  CryoAtt->SetVisibility(false);
  logicOuterCryo->SetVisAttributes(CryoAtt);

  G4cout << "//************************************************//" << G4endl;
  G4cout << "//********** Vacuum Between Cryostats ************//" << G4endl;
  G4cout << "//************************************************//" << G4endl;

  G4String vacuumGapName = GetName() + "/VacuumGap";
  G4double vesselThickness = 4.86*cm;

  G4double outerCryoThick = vesselThickness;

  G4double vacuumGapR = outerCryoR - outerCryoThick;
  G4double vacuumGapH = outerCryoH - outerCryoThick;

  G4LogicalVolume* logicVacuumGap = new G4LogicalVolume(new G4Tubs(vacuumGapName,
                                                                   0, vacuumGapR,
                                                                   vacuumGapH,
                                                                   0,360*deg),
                                                        FindMaterial("G4_Galactic"),
                                                        vacuumGapName);

  G4VPhysicalVolume* physVacuumGap = new G4PVPlacement(0,
                                                       G4ThreeVector(),
                                                       logicVacuumGap,
                                                       vacuumGapName,
                                                       logicOuterCryo,
                                                       false,
                                                       0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(vacuumGapName,physVacuumGap);
  logicVacuumGap->SetVisAttributes(CryoAtt);

  G4cout << "//************************************************//" << G4endl;
  G4cout << "//*************** Inner Cryostat *****************//" << G4endl;
  G4cout << "//************************************************//" << G4endl;

  G4String innerCryoName = GetName() + "/InnerCryo";
  G4double vacuumGapThick = 28.7*cm;

  G4double innerCryoR = vacuumGapR - vacuumGapThick;
  G4double innerCryoH = vacuumGapH - vacuumGapThick;

  G4LogicalVolume* logicInnerCryo = new G4LogicalVolume(new G4Tubs(innerCryoName,
                                                                   0, innerCryoR,
                                                                   innerCryoH,
                                                                   0, 360*deg),
                                                        FindMaterial("G4_Cu"),
                                                        innerCryoName);

  G4VPhysicalVolume* physInnerCryo = new G4PVPlacement(0,
                                                       G4ThreeVector(),
                                                       logicInnerCryo,
                                                       innerCryoName,
                                                       logicVacuumGap,
                                                       false,
                                                       0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(innerCryoName,physInnerCryo);
  logicInnerCryo->SetVisAttributes(CryoAtt);
  
  G4cout << "//************************************************//" << G4endl;
  G4cout << "//******************** HFE ***********************//" << G4endl;
  G4cout << "//************************************************//" << G4endl;

  G4String HFEName = GetName() + "/HFE";
  G4double innerCryoThick = vesselThickness;

  G4double HFER = innerCryoR - innerCryoThick;
  G4double HFEH = innerCryoH - innerCryoThick;

  G4LogicalVolume* logicHFE = new G4LogicalVolume(new G4Tubs(HFEName,
                                                             0, HFER,
                                                             HFEH,
                                                             0, 360*deg),
                                                  FindMaterial("HFE"),
                                                  HFEName);

  G4VPhysicalVolume* physHFE = new G4PVPlacement(0,
                                                 G4ThreeVector(),
                                                 logicHFE,
                                                 HFEName,
                                                 logicInnerCryo,
                                                 false,
                                                 0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(HFEName,physHFE);
  logicHFE->SetVisAttributes(CryoAtt);

  fTPCVesselMotherVolume = logicHFE;


  //***************************************************//
  // Build air cavity based on dimensions of cryopit   //
  //***************************************************//
  
  G4cout << "//************************************************//" << G4endl;
  G4cout << "//******** Air Cavity Surrounding Rock  **********//" << G4endl;
  G4cout << "//************************************************//" << G4endl;

  G4String rockDomeName = GetName() + "/RockDome";
  G4double rockDomeR = surroundRockR;
 
  G4LogicalVolume* logicRockDome = new G4LogicalVolume(new G4Sphere(rockDomeName,
                                                                    0, rockDomeR,
                                                                    0, 360*deg, 0, 90*deg),
                                                       FindMaterial("Rock"),
                                                       rockDomeName);

  G4VPhysicalVolume* physRockDome = new G4PVPlacement(0,
                                                      G4ThreeVector(0,0,surroundRockPlaceZ+surroundRockH),
                                                      logicRockDome,
                                                      rockDomeName,
                                                      logicExternals,
                                                      false,
                                                      0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(rockDomeName,physRockDome);

  logicRockDome->SetVisAttributes(MineAtt);

  G4cout << "//************************************************//" << G4endl;
  G4cout << "//************* Air Cavity Coating ***************//" << G4endl;
  G4cout << "//************************************************//" << G4endl;

  G4String SCDomeName = GetName() + "/ShotDome";
  G4double SCDomeR = shotcreteR;

  G4LogicalVolume* logicShotDome = new G4LogicalVolume(new G4Sphere(SCDomeName,
                                                                    0, SCDomeR,
                                                                    0, 360*deg, 0, 90*deg),
                                                       FindMaterial("ShotCrete"),
                                                       SCDomeName);
  
  G4VPhysicalVolume* physShotDome = new G4PVPlacement(0,
                                                      G4ThreeVector(0,0,0),
                                                      logicShotDome,
                                                      SCDomeName,
                                                      logicRockDome,
                                                      false,
                                                      0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(SCDomeName,physShotDome);
  logicShotDome->SetVisAttributes(MineAtt);

  G4cout << "//************************************************//" << G4endl;
  G4cout << "//***************** Air Cavity *******************//" << G4endl;
  G4cout << "//************************************************//" << G4endl;

  G4String airDomeName = GetName() + "/AirDome";
  G4double airDomeR = outerWater1R;

  G4LogicalVolume* logicAirDome = new G4LogicalVolume(new G4Sphere(airDomeName,
                                                    0, airDomeR,
                                                    0, 360*deg, 0, 90*deg),
                                       FindMaterial("G4_AIR"),
                                       airDomeName);

  G4VPhysicalVolume* physAirDome = new G4PVPlacement(0,
                                                     G4ThreeVector(0,0,0),
                                                     logicAirDome,
                                                     airDomeName,
                                                     logicShotDome,
                                                     false,
                                                     0,fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(airDomeName,physAirDome);
  G4VisAttributes* AirAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
  AirAtt->SetVisibility(false);
  //AirAtt->SetForceSolid(true);
  logicAirDome->SetVisAttributes(AirAtt);

  G4Region*  CryostatRegion = new G4Region("CryostatRegion");
  CryostatRegion->AddRootLogicalVolume(logicOuterCryo);


  return logicExternals;

}



//-----------------------------------Test Stand----------------------------------------
G4LogicalVolume* nEXOTPCExternalsConstructor::GetPieceTestStand(G4LogicalVolume *logicExternalsTestStand)
{


  G4cout << "//*****************************************************//" << G4endl;
  G4cout << "//*************** Test Stand Cryostat *****************//" << G4endl;
  G4cout << "//*****************************************************//" << G4endl;


  G4cout << "//*****************************************************//" << G4endl;
  G4cout << "//*************** Outer Test Stand Cryostat *****************//" << G4endl;
  G4cout << "//*****************************************************//" << G4endl;

  G4String outCryoName = GetName() + "/OuterCryo";

  G4double CryoSteelThickness = 2.0*mm;

  G4VisAttributes* outCryoAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
  outCryoAtt->SetVisibility(false);

  G4double outCryoR = fHFERadius + fHFEDewarThickness;
  G4double outCryoH = fHFEHeight + fHFEDewarThickness;

  G4cout << " Test Stand The Outer CryoStat for HFE has R = " << outCryoR << " and H " << outCryoH  << G4endl;

  G4LogicalVolume* logicOutCryo = new G4LogicalVolume(new G4Tubs(outCryoName,
                                                                   0, outCryoR,outCryoH,
                                                                   0, 360*deg),
                                                        FindMaterial("G4_STAINLESS-STEEL"),
                                                        outCryoName);

  G4VPhysicalVolume* outphysCryo = new G4PVPlacement(0,
                                                       G4ThreeVector(),
                                                       logicOutCryo,
                                                       outCryoName,
                                                       logicExternalsTestStand,
                                                       false,
                                                       0,fCheckOverlaps);

  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(outCryoName,outphysCryo);
  logicOutCryo->SetVisAttributes(outCryoAtt);


  G4cout << "//*****************************************************//" << G4endl;
  G4cout << "//*************** Vacuum Test Stand Cryostat *****************//" << G4endl;
  G4cout << "//*****************************************************//" << G4endl;

  G4String VacuumName = GetName() + "/Vacuum";
  G4VisAttributes* VacuumAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
  VacuumAtt->SetVisibility(false);

  G4double VacuumR = fHFERadius + fHFEDewarThickness - CryoSteelThickness;
  G4double VacuumH = fHFEHeight + fHFEDewarThickness - CryoSteelThickness;

  G4cout << " Test Stand The Vacuum for HFE has R = " << VacuumR << " and H " << VacuumH  << G4endl;

  G4LogicalVolume* logicVacuum = new G4LogicalVolume(new G4Tubs(VacuumName,
                                                                   0, VacuumR, VacuumH,
                                                                   0, 360*deg),
                                                        FindMaterial("G4_Galactic"),
                                                        VacuumName);

  G4VPhysicalVolume* physVacuum = new G4PVPlacement(0,
                                                       G4ThreeVector(),
                                                       logicVacuum,
                                                       VacuumName,
                                                       logicOutCryo,
                                                       false,
                                                       0,fCheckOverlaps);

  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(VacuumName,physVacuum);
  logicVacuum->SetVisAttributes(VacuumAtt);


  G4cout << "//*****************************************************//" << G4endl;
  G4cout << "//*************** InnerTest Stand Cryostat *****************//" << G4endl;
  G4cout << "//*****************************************************//" << G4endl;

  G4String inCryoName = GetName() + "/inCyro";

  G4VisAttributes* inCryoAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));
  inCryoAtt->SetVisibility(false);


  G4double inCryoR = fHFERadius + CryoSteelThickness;
  G4double inCryoH = fHFEHeight + CryoSteelThickness;

  G4cout << " Test Stand Inner Cryo for HFE has R = " << inCryoR << " and H " << inCryoH   << G4endl;
  
  G4LogicalVolume* logicInCryo = new G4LogicalVolume(new G4Tubs(inCryoName,
                                                                   0, inCryoR, inCryoH,
                                                                   0, 360*deg),
                                                        FindMaterial("G4_STAINLESS-STEEL"),
                                                        inCryoName);

  G4VPhysicalVolume* physInCryo = new G4PVPlacement(0,
                                                       G4ThreeVector(),
                                                       logicInCryo,
                                                       inCryoName,
                                                       logicVacuum,
                                                       false,
                                                       0,fCheckOverlaps);

  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(inCryoName,physInCryo);
  logicInCryo->SetVisAttributes(inCryoAtt);
  
    G4Tubs* mainLXeAll = new G4Tubs(GetName(), 0,  125.475*mm, 39.27*mm, 0, 360*deg);

/* Added lower cylinder of inactive LXe */
G4String inactiveLXeLower = GetName() + "/inactiveLXeLowerRegion";
    
    G4Tubs* inactiveLXeLower1  = new G4Tubs(inactiveLXeLower, 0*mm, 50.8*mm, 21.895*mm, 0, 360*deg);

    G4LogicalVolume* logicinactiveLXeLower = new G4LogicalVolume(inactiveLXeLower1, FindMaterial("liquidXe"), inactiveLXeLower);

/* Lower cylinder color */
G4VisAttributes* LowerAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));
logicinactiveLXeLower->SetVisAttributes(LowerAtt);

/*  Joining lower inactive LXe cyl with mainLXe */
G4String inactiveLXe = GetName() + "/inactiveLXeRegion";

	G4UnionSolid* inactiveLXe1 = new G4UnionSolid("mainLXe+inactiveLXeLower", mainLXeAll, inactiveLXeLower1, 0,  G4ThreeVector(0, 0, -61.165*mm));

	G4LogicalVolume* logicinactiveLXe = new G4LogicalVolume(inactiveLXe1, FindMaterial("liquidXe"), inactiveLXe);

	G4VPhysicalVolume* physinactiveLXe = new G4PVPlacement(0,
                                                       G4ThreeVector(0, 0, 0),
                                                       logicinactiveLXe,
                                                       inactiveLXe,
                                                       logicInCryo,
                                                       false,
                                                       0,
                                                       fCheckOverlaps);
    nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(inactiveLXe, physinactiveLXe);

		/* TPC Vessel declare (to make TPC mother of inactiveLXe) 
       G4Tubs* mainTPC = new G4Tubs("mainTPC", 0,  152.02*mm, 64.39*mm, 0, 360*deg);
       logicTPC =  new G4LogicalVolume(mainTPC,
                                    FindMaterial("G4_STAINLESS-STEEL"),
                                    GetName());

    G4VPhysicalVolume* physinactiveLXe = new G4PVPlacement(0,
                                                       G4ThreeVector(0, 0, 0),
                                                       logicinactiveLXeLower,
                                                       inactiveLXe,
                                                       logicTPC,
                                                       false,
                                                       0,
                                                       fCheckOverlaps);
	nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(inactiveLXe, physinactiveLXe);
*/
  G4cout << "//************************************************//" << G4endl;
  G4cout << "//******************** HFE ***********************//" << G4endl;
  G4cout << "//************************************************//" << G4endl;


  G4cout << " Test Stand HFE R and H "<< fHFERadius << "  " << fHFEHeight << G4endl;
  G4cout << " Test Stand G4 Lab l/w/h  "<< fLabWidth  << " " << fLabWidth << "  " << fLabHeight  << G4endl;

  G4String HFEName = GetName() + "/HFE";
  G4VisAttributes* HFEAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));
  HFEAtt->SetVisibility(false);

  G4LogicalVolume* logicHFE = new G4LogicalVolume(new G4Tubs(HFEName,
                                                             0, fHFERadius,
                                                             fHFEHeight,
                                                             0, 360*deg),
                                                  FindMaterial("HFE"),
                                                  HFEName);

  G4VPhysicalVolume* physHFE = new G4PVPlacement(0,
                                                 G4ThreeVector(),
                                                 logicHFE,
                                                 HFEName,
                                                 logicInCryo,
                                                 false,
                                                 0,fCheckOverlaps);
  
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(HFEName,physHFE);
  logicHFE->SetVisAttributes(HFEAtt);

  fTPCVesselMotherVolume = logicHFE;
 
/* Added 'pancake end' to bottm of TPC Vessel */
G4String PancakeEnd = GetName() + "/PancakeEndRegion";
    
    G4Tubs* PancakeEnd1  = new G4Tubs(PancakeEnd, 50.8*mm, 76.8*mm, 10*mm, 0, 360*deg);

    G4LogicalVolume* logicPancakeEnd = new G4LogicalVolume(PancakeEnd1, FindMaterial("G4_STAINLESS-STEEL"), PancakeEnd);

    G4VPhysicalVolume* physPancakeEnd = new G4PVPlacement(0,
                                                       G4ThreeVector(0, 0, -74.39*mm),
                                                       logicPancakeEnd,
                                                       PancakeEnd,
                                                       logicHFE,
                                                       false,
                                                       0,
                                                       fCheckOverlaps);
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(PancakeEnd, physPancakeEnd);

   
  G4Region*  CryostatRegion = new G4Region("CryostatRegion");
  CryostatRegion->AddRootLogicalVolume(logicOutCryo);

  return logicExternalsTestStand;
}
//---------------------------------------------------------------------------------


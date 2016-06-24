#include "nEXOWorldConstructor.hh"

class nEXOWorldMessenger : public nEXOConstructorMessenger
{
private:
  nEXOWorldConstructor* fConstructor;

  //-----------------------------------Test Stand----------------------------------------
  G4UIcmdWithADoubleAndUnit* fWorldDimensionCmd;
  //--------------------------------------------------------------------------------------

public:
  nEXOWorldMessenger(nEXOWorldConstructor* construct) :
    nEXOConstructorMessenger(construct,"Control of the nEXO world geometry."),
    fConstructor(construct){

     //-----------------------------------Test Stand----------------------------------------
    fWorldDimensionCmd = new G4UIcmdWithADoubleAndUnit(CommandName("WorldDimension"),this);
    fWorldDimensionCmd->SetGuidance("Define World Cube length/width/height");
    fWorldDimensionCmd->SetParameterName("WorldDimension",false);
    fWorldDimensionCmd->SetUnitCategory("Length");
    fWorldDimensionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    //--------------------------------------------------------------------------------------



  };

  virtual ~nEXOWorldMessenger(){

    //-----------------------------------Test Stand----------------------------------------
    delete fWorldDimensionCmd;
    //--------------------------------------------------------------------------------------

  };  

  void SetNewValue(G4UIcommand* cmd, G4String val){
    if( cmd == fWorldDimensionCmd) {
      //-----------------------------------Test Stand------------------------------------
      fConstructor->SetWidth(fWorldDimensionCmd->GetNewDoubleValue(val));
      fConstructor->SetHeight(fWorldDimensionCmd->GetNewDoubleValue(val));
      fConstructor->SetLength(fWorldDimensionCmd->GetNewDoubleValue(val));
    }
    else {
      nEXOConstructorMessenger::SetNewValue(cmd,val);
    }
  };
};


void nEXOWorldConstructor::Init(void)
{
  SetMessenger(new nEXOWorldMessenger(this));

  fWidth  = 2*2000.*cm;
  fHeight = fWidth;
  fLength = fWidth; 

  AddConstructor(new nEXOTPCVesselConstructor("TPCVessel",this));
  AddConstructor(new nEXOTPCExternalsConstructor("TPCExternals",this));
  AddConstructor(new nEXOTPCInternalsConstructor("TPCInternals",this));
  
}

nEXOWorldConstructor::~nEXOWorldConstructor(){;}

G4LogicalVolume* nEXOWorldConstructor::GetPiece(void)
{  
  G4cout << "//******************************************************//" << G4endl;
  G4cout << "//********* CONSTRUCT THE MINE -> The World ************//" << G4endl;
  G4cout << "//******************************************************//" << G4endl;


  //-----------------------------------------------Test Stand-----------------------------------------------
  G4cout << " Test Stand Edit World Dimensions w/h/l are (mm) factor of 2 when defining G4Box "<< fWidth << "  " << fHeight << " " << fLength   << G4endl;
  //-----------------------------------------------------------------------------------------------------------

  G4LogicalVolume* logicMine = new G4LogicalVolume(new G4Box(GetName(),
                                                             fWidth/2., 
                                                             fHeight/2., 
                                                             fWidth/2.), 
                                                   FindMaterial("Rock"),
                                                   GetName());

  //logicMine->SetVisAttributes(G4VisAttributes::Invisible);
  G4VisAttributes* mineAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
  mineAtt->SetVisibility(true);
  mineAtt->SetForceWireframe(true);
  logicMine->SetVisAttributes(mineAtt);

  nEXOTPCExternalsConstructor& externals = Get<nEXOTPCExternalsConstructor>("TPCExternals");
  externals.SetCheckOverlaps(fCheckOverlaps);
  G4LogicalVolume* logicExternals = externals.GetPiece();
  G4VPhysicalVolume* physTPCExternals = new G4PVPlacement(0,                     // rotation
                                                          G4ThreeVector(0,0,18.16/2.0),  // position
                                                          logicExternals,        // logical volume
                                                          externals.GetName(),   // name
                                                          logicMine,             // mother volume
                                                          false,                 // no boolean operations
                                                          0,fCheckOverlaps);                    // not a copy
  
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(externals.GetName(),physTPCExternals);  
  
  
  nEXOTPCVesselConstructor& tpcVessel = Get<nEXOTPCVesselConstructor>("TPCVessel");
  tpcVessel.SetCheckOverlaps(fCheckOverlaps);
  G4LogicalVolume* logicTPC = tpcVessel.GetPiece();
  G4RotationMatrix* rotm = new G4RotationMatrix();
  if(externals.GetVersion() == 0)
    rotm->rotateY(-90.*deg);  
  G4VPhysicalVolume* physTPCVessel = new G4PVPlacement(rotm,                  // rotation
                                                       externals.GetTPCVesselTranslation(),//G4ThreeVector(),       // position
                                                       logicTPC,              // logical volume
                                                       tpcVessel.GetName(),   // name
                                                       externals.GetTPCVesselMotherVolume(),        // mother volume nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(externals.GetName() + "/HFE")->GetLogicalVolume(),        // mother volume
                                                       false,                 // no boolean operations
                                                       0,fCheckOverlaps);                    // not a copy
  
  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(tpcVessel.GetName(),physTPCVessel);
  nEXOSimplePhysVolManager::GetInstance()->SetTPCVesselConstructor(&tpcVessel);
  
    
  nEXOTPCInternalsConstructor& internals = Get<nEXOTPCInternalsConstructor>("TPCInternals");
  internals.SetCheckOverlaps(fCheckOverlaps);
  internals.SetDimensionsForTPCVessel(tpcVessel);
  G4LogicalVolume* logicInternals = internals.GetPiece();
  G4VPhysicalVolume* physInternals = new G4PVPlacement(0,                     // no rotation wrt to mother volume (TPC vessel)
                                                       G4ThreeVector(0,0,0),  // position
                                                       logicInternals,        // logical volume
                                                       internals.GetName(),   // name
                                                       logicTPC,              // mother volume
                                                       false,                 // no boolean operations
                                                       0,fCheckOverlaps);                    // not a copy

  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(internals.GetName(),physInternals);  
  
  tpcVessel.SetOpticalSurface(internals.GetName() + "/LXe/ActiveRegion");

  G4Region* TPCRegion = new G4Region("TPCRegion");
  TPCRegion->AddRootLogicalVolume(logicTPC);  
  
  return logicMine;  
}

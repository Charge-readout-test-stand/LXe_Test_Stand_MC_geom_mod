#include "nEXOLXeConstructor.hh"
#include "G4UnionSolid.hh"

nEXOLXeConstructor::~nEXOLXeConstructor(){;}

void nEXOLXeConstructor::Init(void)
{
  SetMessenger(new nEXOLXeMessenger(this));

  // initial LXe dimensions
  SetRadius1(650*mm);
  SetRadius2(65.*cm);
  SetLength(2*65.*cm);

  SetActiveLXeDimensions(GetRadius1(),GetRadius2(),GetLength());  
  
  SetAllActive();
}

void nEXOLXeConstructor::SetAllActive()
{
  fOnlyActiveLXe = true;
  SetActiveLXeDimensions(GetRadius1(),GetRadius2(),GetLength());  
}

void nEXOLXeConstructor::SetActiveLXeDimensions(double radius1, double radius2, double length)
{
  fActiveLXeRadius1 = radius1;
  fActiveLXeRadius2 = radius2;
  fActiveLXeLength = length;  
}

void nEXOLXeConstructor::FillTPCVessel(nEXOTPCVesselConstructor& tpcVessel)
{
  SetLXeOption(tpcVessel.GetTPCVesselOption());

  if(fOption == 2)
  {   //-------------------------------------Test Stand-------------------------------------
      G4cout << " Setting the Stuff for the LXe Stuff " << G4endl;
      SetRadius1(tpcVessel.GetTPCRadiusTestStand() - tpcVessel.GetTPCThicknessTestStand());
      SetRadius2(tpcVessel.GetTPCRadiusTestStand() - tpcVessel.GetTPCThicknessTestStand());
      SetLength(tpcVessel.GetTPCHeightTestStand()  - 2*tpcVessel.GetTPCThicknessTestStand());
      //SetTestStandActive();
      //Edit so that this includes just the box dims for the test stand
      // "/nEXO/TPCInternals/LXe/ActiveHeight"
      // "/nEXO/TPCInternals/LXe/ActiveLength"
      // "/nEXO/TPCInternals/LXe/ActiveWidth"
  }
  else {
      SetRadius1(tpcVessel.GetRadius1() - tpcVessel.GetThickness());
      SetRadius2(tpcVessel.GetRadius2() - tpcVessel.GetThickness());
      SetLength(tpcVessel.GetLength() - 2*tpcVessel.GetEndsThickness());
  }
}

G4LogicalVolume* nEXOLXeConstructor::GetPiece(void)
{
  G4cout << "//################################################//" << G4endl;
  G4cout << "//################### Liquid Xe ##################//" << G4endl;
  G4cout << "//################################################//" << G4endl;

  G4LogicalVolume* logicLXe = 0;
  if(fOption == 0) {
    logicLXe = new G4LogicalVolume(new G4Cons(GetName(),
                                   0, GetRadius1(),
                                   0, GetRadius2(),
                                   GetLength()/2.,
                                   0, 360*deg),
                                   FindMaterial("liquidXe"),
                                   GetName());  
  }
  
  if(fOption == 1) {
    G4Tubs* mainLXe = new G4Tubs("mainLXe", 0, GetRadius2(), GetLength()/2., 0, 360*deg);
    G4Tubs* lotusLXe1 = new G4Tubs("lotusLXe1", GetRadius2(), GetRadius2()+2.0*cm, 2.5/2*cm, 0, 360*deg);
    G4Tubs* lotusLXe2 = new G4Tubs("lotusLXe2", 560*mm, GetRadius2()+2.0*cm, 3.5/2*cm, 0, 360*deg);
    G4UnionSolid* partUnionLXe = new G4UnionSolid("main+lxe1", mainLXe, lotusLXe1, 0, G4ThreeVector(0, 0, -GetLength()/2.+2.5/2*cm));
    G4UnionSolid* solidLXe = new G4UnionSolid(GetName(), partUnionLXe, lotusLXe2, 0, G4ThreeVector(0, 0, -GetLength()/2.-3.5/2*cm));

    logicLXe = new G4LogicalVolume(solidLXe,
                                   FindMaterial("liquidXe"),
                                   GetName());
  }

  if(fOption == 2) {

    G4cout << "LXe for the Test Stand Using R1 =" << GetRadius1() << " L = " << GetLength()   << G4endl;

    G4Tubs* mainLXeAll = new G4Tubs(GetName(), 0,  GetRadius1(), GetLength(), 0, 360*deg);

    logicLXe = new G4LogicalVolume(mainLXeAll, FindMaterial("liquidXe"), GetName());

    
    G4cout << "//################################################//" << G4endl;
    G4cout << "//############### Test Stand Active Liquid Xe ###############//" << G4endl;
    G4cout << "//################################################//" << G4endl;

    G4String activeLXeName = GetName() + "/ActiveRegion";

    

// hi

G4cout << "Active LXe for the Test Stand Using R1 =" << fActiveLengthTestStand << " L = " << fActiveHeightTestStand << G4endl;
 
    //G4Tubs* mainLXeActive = new G4Tubs(activeLXeName, 0,  GetActiveLXeRadius1(), GetActiveLXeLength(), 0, 360*deg);

    //G4Box* mainLXeActive = new G4Box(activeLXeName, fActiveLengthTestStand, fActiveWidthTestStand, fActiveHeightTestStand);
    
    G4Tubs* mainLXeActive = new G4Tubs(activeLXeName, 0, fActiveLengthTestStand, fActiveHeightTestStand, 0, 360*deg);

    G4LogicalVolume* logicActiveLXe = new G4LogicalVolume(mainLXeActive, FindMaterial("liquidXe"), activeLXeName);

    G4VPhysicalVolume* physActiveLXe = new G4PVPlacement(0,
                                                       G4ThreeVector(),
                                                       logicActiveLXe,
                                                       activeLXeName,
                                                       logicLXe,
                                                       false,
                                                       0,
                                                       fCheckOverlaps);

    nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(activeLXeName, physActiveLXe);

    //Attributes of the LXe
    G4VisAttributes* XeAtt = new G4VisAttributes(G4Colour(1.0, 0, 1.0));
    XeAtt->SetVisibility(false);
    //XeAtt->SetForceSolid(true);
    logicLXe->SetVisAttributes(XeAtt);
    //logicActiveLXe->SetVisAttributes(XeAtt);
    
    return logicLXe;
  }

  G4cout << "//################################################//" << G4endl;
  G4cout << "//############### Active Liquid Xe ###############//" << G4endl;
  G4cout << "//################################################//" << G4endl;


  G4cout << "Got To start of Finding LXe" << G4endl;

  G4String activeLXeName = GetName() + "/ActiveRegion";
  G4LogicalVolume* logicActiveLXe = new G4LogicalVolume(new G4Cons(activeLXeName,
                                                                   0, GetActiveLXeRadius1(),
                                                                   0, GetActiveLXeRadius2(),
                                                                   GetActiveLXeLength()/2.,
                                                                   0, 360*deg),
                                                        FindMaterial("liquidXe"),
                                                        activeLXeName);
   
  G4cout << "Got To end of Finding LXe" << G4endl;

  G4VPhysicalVolume* physActiveLXe = new G4PVPlacement(0,
                                                       G4ThreeVector(),
                                                       logicActiveLXe,
                                                       activeLXeName,
                                                       logicLXe,
                                                       false,
                                                       0,
                                                       fCheckOverlaps);
  

  G4cout << "Got To end of Physcial LXe" << G4endl;

  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(activeLXeName, physActiveLXe);   
    

  G4cout << "Added Phys" << G4endl;

  //Attributes of the LXe
  G4VisAttributes* XeAtt = new G4VisAttributes(G4Colour(1.0, 0, 1.0));
  XeAtt->SetVisibility(false);
  //XeAtt->SetForceSolid(true);
  G4cout << " Try Set Visibility " << G4endl;
  logicLXe->SetVisAttributes(XeAtt);
  G4cout << " Did Set Visibility " << G4endl;
  //logicActiveLXe->SetVisAttributes(XeAtt);

  G4cout << "Got To end of Making LXe" << G4endl;
  return logicLXe;
}

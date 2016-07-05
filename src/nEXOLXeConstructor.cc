#include "nEXOLXeConstructor.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
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

/* Inactive LXe region */

    G4cout << "LXe for the Test Stand Using R1 =" << GetRadius1() << " L = " << GetLength()   << G4endl;

    G4Tubs* mainLXeAll = new G4Tubs(GetName(), 0,  125.475*mm, 39.27*mm, 0, 360*deg);

G4String inactiveLXeLower = GetName() + "/inactiveLXeLowerRegion";
 
    G4Tubs* inactiveLXeLower1  = new G4Tubs(inactiveLXeLower, 0*mm, 50.8*mm, 21.895*mm, 0, 360*deg);

    G4UnionSolid* inactiveLXe = new G4UnionSolid("mainLXe+inactiveLXeLower", mainLXeAll, inactiveLXeLower1, 0, G4ThreeVector(0, 0, -61.165*mm));

    logicLXe = new G4LogicalVolume(inactiveLXe, FindMaterial("liquidXe"), GetName());

    G4cout << "//################################################//" << G4endl;
    G4cout << "//############### Test Stand Active Liquid Xe ###############//" << G4endl;
    G4cout << "//################################################//" << G4endl;


/* Active LXe region */    
G4String activeLXeName = GetName() + "/ActiveRegion";

    
G4cout << "Active LXe for the Test Stand Using R1 =" << fActiveLengthTestStand << " L = " << fActiveHeightTestStand << G4endl;
 
    //G4Tubs* mainLXeActive = new G4Tubs(activeLXeName, 0,  GetActiveLXeRadius1(), GetActiveLXeLength(), 0, 360*deg);

    //G4Box* mainLXeActive = new G4Box(activeLXeName, fActiveLengthTestStand, fActiveWidthTestStand, fActiveHeightTestStand);
   

    G4Tubs* mainLXeActive = new G4Tubs(activeLXeName, 0, 105*mm, 9.08*mm, 0, 360*deg);

    G4LogicalVolume* logicActiveLXe = new G4LogicalVolume(mainLXeActive, FindMaterial("liquidXe"), activeLXeName);

    G4VPhysicalVolume* physActiveLXe = new G4PVPlacement(0,
                                                       G4ThreeVector(0, 0, -19.65*mm),
                                                       logicActiveLXe,
                                                       activeLXeName,
                                                       logicLXe,
                                                       false,
                                                       0,
                                                       fCheckOverlaps);

    nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(activeLXeName, physActiveLXe);

/* Added anode disk and quartz tile */
G4String AnodeDisk = GetName() + "/AnodeDiskRegion";
    
    G4Tubs* AnodeDisk1  = new G4Tubs(AnodeDisk, 0*mm, 101.6*mm, 3.175*mm, 0, 360*deg);

G4String AnodeCutOut = GetName() + "/AnodeCutOutRegion";

    G4Box* AnodeCutOut1 = new G4Box(AnodeCutOut, 50.8*mm, 50.8*mm, 3.275*mm);
 
    G4SubtractionSolid* Anode = new G4SubtractionSolid("AnodeDisk-AnodeCutOut", AnodeDisk1, AnodeCutOut1, 0, G4ThreeVector(0,0,0));

G4String AnodeFinalDisk = GetName() + "/AnodeFinalDiskRegion";
 	    
    G4LogicalVolume* logicAnodeFinalDisk = new G4LogicalVolume(Anode, FindMaterial("G4_STAINLESS-STEEL"), AnodeFinalDisk);

    G4VPhysicalVolume* physAnodeFinalDisk = new G4PVPlacement(0,
                                                       G4ThreeVector(0, 0, -6.695*mm),
                                                       logicAnodeFinalDisk,
                                                       AnodeFinalDisk,
                                                       logicLXe,
                                                       false,
                                                       0,
                                                       fCheckOverlaps);
     nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(AnodeFinalDisk, physAnodeFinalDisk);

G4String QuartzTile = GetName() + "/QuartzTileRegion";
	G4Box* QuartzTile1 = new G4Box(QuartzTile, 50.8*mm, 50.8*mm, 0.25*mm);
	G4LogicalVolume* logicQuartzTile = new G4LogicalVolume(QuartzTile1, FindMaterial("G4_SILICON_DIOXIDE"), QuartzTile);
	G4VPhysicalVolume* physQuartzTile = new G4PVPlacement (0,
							G4ThreeVector(0, 0, -10.22*mm),
							logicQuartzTile, 
							QuartzTile, 
							logicLXe, 
							false, 
							0, 
							fCheckOverlaps);
     nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(QuartzTile, physQuartzTile);
  
G4VisAttributes* AnodeDiskAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
logicAnodeFinalDisk->SetVisAttributes(AnodeDiskAtt);
G4VisAttributes* QuartzTileAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));
logicQuartzTile->SetVisAttributes(QuartzTileAtt); 

/* Added cathode rings */
G4String CathodeRingUpper = GetName() + "/CathodeUpperRegion";
    
    G4Tubs* CathodeRingUpper1  = new G4Tubs(CathodeRingUpper, 76.2*mm, 99.06*mm, 0.795*mm, 0, 360*deg);

    G4LogicalVolume* logicCathodeUpper = new G4LogicalVolume(CathodeRingUpper1, FindMaterial("G4_STAINLESS-STEEL"), CathodeRingUpper);

    G4VPhysicalVolume* physCathodeUpper = new G4PVPlacement(0,
                                                       G4ThreeVector(0, 0, -8.185*mm),
                                                       logicCathodeUpper,
                                                       CathodeRingUpper,
                                                       logicActiveLXe,
                                                       false,
                                                       0,
                                                       fCheckOverlaps);
    nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(CathodeRingUpper, physCathodeUpper);


G4String CathodeRingLower = GetName() + "/CathodeLowerRegion";
    
    G4Tubs* CathodeRingLower1  = new G4Tubs(CathodeRingLower, 76.2*mm, 99.06*mm, 1.27*mm, 0, 360*deg);

    G4LogicalVolume* logicCathodeLower = new G4LogicalVolume(CathodeRingLower1, FindMaterial("G4_STAINLESS-STEEL"), CathodeRingLower);

    G4VPhysicalVolume* physCathodeLower = new G4PVPlacement(0,
                                                       G4ThreeVector(0, 0, -30.1*mm),
                                                       logicCathodeLower,
                                                       CathodeRingLower,
                                                       logicLXe,
                                                       false,
                                                       0,
                                                       fCheckOverlaps);
    nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(CathodeRingLower, physCathodeLower);


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


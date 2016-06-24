#include "nEXOWaterTankConstructor.hh"

nEXOWaterTankConstructor::~nEXOWaterTankConstructor(){;}

void nEXOWaterTankConstructor::Init(void)
{
  SetMessenger(new nEXOWaterTankMessenger(this));

  SetMaterial("G4_STAINLESS-STEEL");
  
  SetSideThickness(10.16*cm);
  SetBottomThickness(10.16*cm);
  SetTopThickness(15.24*cm);
  
  SetRadius1(1310.64*cm/2);
  SetRadius2(GetRadius1());
  SetLength(1402.08*cm);
}

G4LogicalVolume* nEXOWaterTankConstructor::GetPiece(void)
{
  G4cout << "//################################################//" << G4endl;
  G4cout << "//################ Water Tank ####################//" << G4endl;
  G4cout << "//################################################//" << G4endl;

  G4LogicalVolume* logicWT = new G4LogicalVolume(new G4Cons(GetName(),
                                                             0, GetRadius1(),
                                                             0, GetRadius2(),
                                                             GetLength()/2.,
                                                             0, 360*deg),
                                                 FindMaterial(GetMaterial()),//FindMaterial("G4_STAINLESS-STEEL"),
                                                 GetName());
  
  G4VisAttributes* wtAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
  wtAtt->SetVisibility(true);
  //tpcAtt->SetForceSolid(true);
  logicWT->SetVisAttributes(wtAtt);

  return logicWT;
}

G4LogicalVolume* nEXOWaterTankConstructor::GetFillingPiece(void)
{
  G4cout << "//################################################//" << G4endl;
  G4cout << "//############### Water in Tank ##################//" << G4endl;
  G4cout << "//################################################//" << G4endl;

  G4LogicalVolume* logicH2O = new G4LogicalVolume(new G4Cons(GetName()+"/Water",
                                                             0, GetRadius1()-GetSideThickness(),
                                                             0, GetRadius2()-GetSideThickness(),
                                                             (GetLength()-GetTopThickness()-GetBottomThickness())/2.,
                                                             0, 360*deg),
                                                  FindMaterial("G4_WATER"),
                                                  GetName()+"/Water");
  
  G4VisAttributes* h2oAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
  h2oAtt->SetVisibility(true);
  //tpcAtt->SetForceSolid(true);
  logicH2O->SetVisAttributes(h2oAtt);

  return logicH2O;
}

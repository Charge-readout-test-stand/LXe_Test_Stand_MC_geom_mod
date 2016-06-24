#include "nEXORepeaterBoxConstructor.hh"

void nEXORepeaterBoxConstructor::Init(void)
{
  SetMessenger(new nEXORepeaterBoxMessenger(this));
  SetMaterial("G4_Cu");
}

G4LogicalVolume* nEXORepeaterBoxConstructor::GetPiece(void)
{

  G4Box* OuterBox = new G4Box("OuterBox",22.86*cm,15.24*cm,15.24*cm);
  G4Box* InnerBox = new G4Box("InnerBox",22.69*cm,15.07*cm,15.07*cm);// 2/8"    //22.64,14.99,14.99*cm for Cu box
  G4SubtractionSolid* RepeaterBox = new G4SubtractionSolid("RepeaterBox",OuterBox,InnerBox);
  G4LogicalVolume* logicRepeaterBox = new G4LogicalVolume(RepeaterBox, FindMaterial("G4_STAINLESS-STEEL"),"RepeaterBox");
  

  return logicRepeaterBox;
}



G4LogicalVolume* nEXORepeaterBoxConstructor::GetFillingPiece(void)
{
  G4Box* RepBoxElectronics = new G4Box("Electronics",5*cm, 4.5*cm, 4*cm);
  G4LogicalVolume* logicRepBoxElectronics = new G4LogicalVolume(RepBoxElectronics,FindMaterial("G4_KAPTON"),"RepBoxElectronics");
  
  return logicRepBoxElectronics;
}

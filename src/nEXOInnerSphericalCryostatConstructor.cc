#include "nEXOInnerSphericalCryostatConstructor.hh"

void nEXOInnerSphericalCryostatConstructor::Init(void)
{
  SetMessenger(new nEXOInnerSphericalCryostatMessenger(this));

  SetMaterial("CarbonFiber");
  
  SetRadius(3380*mm/2);
  SetThickness(12.1*mm);//nominal is 10mm, but increased to match total mass. Real cryostat is more massive only on top (collar).
  SetOffsetToOuter(-403.*mm);
}

G4LogicalVolume* nEXOInnerSphericalCryostatConstructor::GetPiece(void)
{
  G4cout << "//################################################//" << G4endl;
  G4cout << "//########## Inner Spherical Cryostat ############//" << G4endl;
  G4cout << "//################################################//" << G4endl;
  
  G4LogicalVolume* logicIC = new G4LogicalVolume(new G4Sphere(GetName(),
                                                              0, GetRadius()+GetThickness(),
                                                              0, 360*deg,
                                                              0, 180*deg),
                                                 FindMaterial(GetMaterial()),//FindMaterial("G4_Al"),
                                                 GetName());

  return logicIC;
}

G4LogicalVolume* nEXOInnerSphericalCryostatConstructor::GetFillingPiece(void)
{
  
  G4cout << "//################################################//" << G4endl;
  G4cout << "//####### HFE in Spherical Inner Cryostat ########//" << G4endl;
  G4cout << "//################################################//" << G4endl;

  G4LogicalVolume* logicHFE = new G4LogicalVolume(new G4Sphere(GetName()+"/HFE",
                                                               0, GetRadius(),
                                                               0, 360*deg,
                                                               0, 180*deg),
                                                  FindMaterial("HFE"),
                                                  GetName()+"/HFE");

  return logicHFE;
}

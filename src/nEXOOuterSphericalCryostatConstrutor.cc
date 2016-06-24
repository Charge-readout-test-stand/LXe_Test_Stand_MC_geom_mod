#include "nEXOOuterSphericalCryostatConstructor.hh"

void nEXOOuterSphericalCryostatConstructor::Init(void)
{
  SetMessenger(new nEXOOuterSphericalCryostatMessenger(this));

  SetMaterial("CarbonFiber");
  
  SetRadius(4460.*mm/2);
  SetThickness(25.6*mm);//thickness is adjust to match vessel mass with collar, nominal is 20.*mm
}

G4LogicalVolume* nEXOOuterSphericalCryostatConstructor::GetPiece(void)
{
  G4cout << "//################################################//" << G4endl;
  G4cout << "//######### Spherical Outer Cryostat #############//" << G4endl;
  G4cout << "//################################################//" << G4endl;
  
  G4LogicalVolume* logicOC = new G4LogicalVolume(new G4Sphere(GetName(),
                                                              0, GetRadius()+GetThickness(),
                                                              0, 360*deg,
                                                              0, 180*deg),
                                                 FindMaterial(GetMaterial()),//FindMaterial("G4_Al"),
                                                 GetName());

  return logicOC;
}

G4LogicalVolume* nEXOOuterSphericalCryostatConstructor::GetFillingPiece(void)
{
  
  G4cout << "//################################################//" << G4endl;
  G4cout << "//##### Vacuum in Spherical Outer Cryostat #######//" << G4endl;
  G4cout << "//################################################//" << G4endl;

  G4LogicalVolume* logicVac = new G4LogicalVolume(new G4Sphere(GetName()+"/Vacuum",
                                                               0, GetRadius(),
                                                               0, 360*deg,
                                                               0, 180*deg),
                                                  FindMaterial("G4_Galactic"),
                                                  GetName()+"/Vacuum");
  return logicVac;
}

G4LogicalVolume* nEXOOuterSphericalCryostatConstructor::GetSupportPiece(void)
{
  
  G4cout << "//################################################//" << G4endl;
  G4cout << "//##### Support in Spherical Outer Cryostat ######//" << G4endl;
  G4cout << "//################################################//" << G4endl;

  G4LogicalVolume* logicSupport = new G4LogicalVolume(new G4Cons(GetName()+"/Support",
                                                                 1036.3*mm, 1036.3*mm + 60*mm,
                                                                 1367.4*mm, 1367.4*mm + 60*mm,
                                                                 275*mm,
                                                                 0, 360*deg),
                                                      FindMaterial(GetMaterial()),
                                                      GetName()+"/Support");

  return logicSupport;
}

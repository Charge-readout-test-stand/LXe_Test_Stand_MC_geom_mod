#include "nEXOTPCVesselCylFlatEndConstructor.hh"
#include "G4UnionSolid.hh"

nEXOTPCVesselCylFlatEndConstructor::~nEXOTPCVesselCylFlatEndConstructor()
{
}

void nEXOTPCVesselCylFlatEndConstructor::Init(void)
{
  SetMessenger(new nEXOTPCVesselCylFlatEndMessenger(this));
  SetMaterial("G4_Cu");  
  
  // Allen's 2nd model
  double thickness = 2*mm;
  double diameter = 1294*mm;
  double ends = 19.18*mm; // to satisfy total mass
  double halflength = 650*mm;

  double radius = diameter/2.;
  double length = 2*(halflength + ends);
  
  SetThickness(thickness);
  SetEndsThickness(ends);
  SetInnerRadius(radius - thickness);
  SetInnerLength(length - 2*ends);
}

G4LogicalVolume* nEXOTPCVesselCylFlatEndConstructor::GetPiece(void)
{
  G4Tubs* shape = new G4Tubs(GetName(), 0, GetInnerRadius() + GetThickness(), GetInnerLength()/2. + GetEndsThickness(), 0, 360*deg);
  G4LogicalVolume* logicTPC = new G4LogicalVolume(shape,FindMaterial(GetMaterial()),GetName());

  G4VisAttributes* tpcAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
  tpcAtt->SetVisibility(true);
  //tpcAtt->SetForceSolid(true);
  logicTPC->SetVisAttributes(tpcAtt);

  return logicTPC;
}

G4VSolid* nEXOTPCVesselCylFlatEndConstructor::GetFillingShape(void)
{
  double radius = GetInnerRadius();
  double halflength = GetInnerLength()/2.;

  G4String name = GetName() + "/Filling";
  
  return new G4Tubs(name,0,radius,halflength,0,360*deg);
}



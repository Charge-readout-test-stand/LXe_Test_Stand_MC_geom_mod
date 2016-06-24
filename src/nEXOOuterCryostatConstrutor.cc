#include "nEXOOuterCryostatConstructor.hh"

void nEXOOuterCryostatConstructor::Init(void)
{
  SetMessenger(new nEXOOuterCryostatMessenger(this));

  //SetMaterial("G4_Cu");
  SetMaterial("CarbonFiber");
  
  SetLength(2216.2*mm);
  SetRadius(3213.1*mm/2);
  SetThickness(50.8*mm);
  SetEndsRadius(2489.2*mm);
}

G4LogicalVolume* nEXOOuterCryostatConstructor::GetPiece(void)
{
  G4cout << "//################################################//" << G4endl;
  G4cout << "//############### Outer Cryostat #################//" << G4endl;
  G4cout << "//################################################//" << G4endl;

  G4double zEnd = (GetMaxLength() - GetLength())/2.;//GetEndsRadius() - sqrt(GetEndsRadius()*GetEndsRadius() - GetRadius()*GetRadius());
  
  G4int nZplanes = 10;
  std::vector<double> zPlane;
  std::vector<double> rOuter;

  for(int i = 0; i <= nZplanes; i++)
  {
    double iEnd = i*zEnd/nZplanes;
    zPlane.push_back(iEnd);
    rOuter.push_back(sqrt(GetEndsRadius()*GetEndsRadius() - (GetEndsRadius() - iEnd)*(GetEndsRadius() - iEnd)));
  }

  for(int i = 0; i <= nZplanes; i++)
  {
    zPlane.push_back(zPlane.at(i) + GetLength() + zEnd);
    rOuter.push_back(rOuter.at(nZplanes-i));
  }  
  
  G4LogicalVolume* logicOC = new G4LogicalVolume(new G4Polycone(GetName(),
                                                                0, 360*deg,
                                                                zPlane.size(),
                                                                &rOuter[0], &zPlane[0]),
                                                 FindMaterial(GetMaterial()),//FindMaterial("G4_Cu"),
                                                 GetName());

  return logicOC;
}

G4LogicalVolume* nEXOOuterCryostatConstructor::GetFillingPiece(void)
{
  
  G4cout << "//################################################//" << G4endl;
  G4cout << "//########## Vacuum in Outer Cryostat ############//" << G4endl;
  G4cout << "//################################################//" << G4endl;

  G4double zEnd = (GetMaxLength() - GetLength() - 2*GetThickness())/2.;
  G4double endsRadius = GetEndsRadius() - GetThickness();
  
  G4int nZplanes = 10;
  std::vector<double> zPlane;
  std::vector<double> rOuter;

  for(int i = 0; i <= nZplanes; i++)
  {
    double iEnd = i*zEnd/nZplanes;
    zPlane.push_back(iEnd);
    rOuter.push_back(sqrt(endsRadius*endsRadius - (endsRadius - iEnd)*(endsRadius - iEnd)));
  }

  for(int i = 0; i <= nZplanes; i++)
  {
    zPlane.push_back(zPlane.at(i) + GetLength() + zEnd);
    rOuter.push_back(rOuter.at(nZplanes-i));
  }  

  G4LogicalVolume* logicVac = new G4LogicalVolume(new G4Polycone(GetName()+"/Vacuum",
                                                                0, 360*deg,
                                                                zPlane.size(),
                                                                &rOuter[0], &zPlane[0]),
                                                 FindMaterial("G4_Galactic"),
                                                 GetName()+"/Vacuum");

  return logicVac;
}


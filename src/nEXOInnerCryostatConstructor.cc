#include "nEXOInnerCryostatConstructor.hh"

void nEXOInnerCryostatConstructor::Init(void)
{
  SetMessenger(new nEXOInnerCryostatMessenger(this));

  //SetMaterial("G4_Cu");
  SetMaterial("CarbonFiber");
  
  SetLength(1549.4*mm);
  SetRadius(2755.9*mm/2);
  SetThickness(10.0*mm);
  SetEndsRadius(2204.7*mm);
}

G4LogicalVolume* nEXOInnerCryostatConstructor::GetPiece(void)
{
  G4cout << "//################################################//" << G4endl;
  G4cout << "//############### Inner Cryostat #################//" << G4endl;
  G4cout << "//################################################//" << G4endl;

  G4double zEnd = (GetMaxLength() - GetLength())/2.;//GetEndsRadius() - sqrt(GetEndsRadius()*GetEndsRadius() - GetRadius()*GetRadius());
  
  G4int nZplanes = 10;
  std::vector<double> zPlane;
  std::vector<double> rInner;

  for(int i = 0; i <= nZplanes; i++)
  {
    double iEnd = i*zEnd/nZplanes;
    zPlane.push_back(iEnd);
    rInner.push_back(sqrt(GetEndsRadius()*GetEndsRadius() - (GetEndsRadius() - iEnd)*(GetEndsRadius() - iEnd)));
  }

  for(int i = 0; i <= nZplanes; i++)
  {
    zPlane.push_back(zPlane.at(i) + GetLength() + zEnd);
    rInner.push_back(rInner.at(nZplanes-i));
  }  
  
  G4LogicalVolume* logicIC = new G4LogicalVolume(new G4Polycone(GetName(),
                                                                0, 360*deg,
                                                                zPlane.size(),
                                                                &rInner[0], &zPlane[0]),
                                                 FindMaterial(GetMaterial()),//FindMaterial("G4_Cu"),
                                                 GetName());

  return logicIC;
}

G4LogicalVolume* nEXOInnerCryostatConstructor::GetFillingPiece(void)
{
  
  G4cout << "//################################################//" << G4endl;
  G4cout << "//########## Vacuum in Inner Cryostat ############//" << G4endl;
  G4cout << "//################################################//" << G4endl;

  G4double zEnd = (GetMaxLength() - GetLength() - 2*GetThickness())/2.;
  G4double endsRadius = GetEndsRadius() - GetThickness();
  
  G4int nZplanes = 10;
  std::vector<double> zPlane;
  std::vector<double> rInner;

  for(int i = 0; i <= nZplanes; i++)
  {
    double iEnd = i*zEnd/nZplanes;
    zPlane.push_back(iEnd);
    rInner.push_back(sqrt(endsRadius*endsRadius - (endsRadius - iEnd)*(endsRadius - iEnd)));
  }

  for(int i = 0; i <= nZplanes; i++)
  {
    zPlane.push_back(zPlane.at(i) + GetLength() + zEnd);
    rInner.push_back(rInner.at(nZplanes-i));
  }  

  G4LogicalVolume* logicHFE = new G4LogicalVolume(new G4Polycone(GetName()+"/HFE",
                                                                0, 360*deg,
                                                                zPlane.size(),
                                                                &rInner[0], &zPlane[0]),
                                                 FindMaterial("HFE"),
                                                 GetName()+"/HFE");

  return logicHFE;
}


#include "nEXOTPCVesselSodaCanConstructor.hh"
#include "G4UnionSolid.hh"

nEXOTPCVesselSodaCanConstructor::~nEXOTPCVesselSodaCanConstructor()
{
}

void nEXOTPCVesselSodaCanConstructor::Init(void)
{
  SetMessenger(new nEXOTPCVesselSodaCanMessenger(this));
  SetMaterial("G4_Cu");  
  fNzPlanes = 15;
  
  // Allen + Tamer drawings
  G4double thickness = 2.*mm;
  G4double top = 2.*mm;
  G4double bottom = 2.*mm;
  G4double length = 2*650.*mm + bottom + top;
  G4double radius = 656.*mm;

  std::vector<G4double> innerAxial(3,0.);
  std::vector<G4double> innerRadial(3,0.);
  innerAxial[2] = -7.5*mm; innerRadial[2] = 0.*mm;
  innerAxial[1] = -100.*mm; innerRadial[1] = 415.*mm;
  innerAxial[0] = -190.*mm; innerRadial[0] = 455.*mm;
    
  std::vector<G4double> outerAxial(4,0.);
  std::vector<G4double> outerRadial(4,0.);
  outerAxial[0] = innerAxial[0]; outerRadial[0] = 490.*mm;
  outerAxial[1] = -130.*mm; outerRadial[1] = 535.*mm;
  outerAxial[2] = -55.*mm; outerRadial[2] = 650.*mm;
  outerAxial[3] = -30.*mm; outerRadial[3] = radius;

  SetDimensions(length, radius, thickness, top, bottom, innerAxial, innerRadial, outerAxial, outerRadial);

}

void nEXOTPCVesselSodaCanConstructor::SetDimensions(G4double length, G4double radius, G4double thickness, G4double top, G4double bottom,
                                                    std::vector<G4double>& innerAxial, std::vector<G4double>& innerRadial,
                                                    std::vector<G4double>& outerAxial, std::vector<G4double>& outerRadial)
{

  // Set TPC outer edge dimensions and coordinates

  fThickness = thickness;
  fTopThickness = top;
  fBottomThickness = bottom;
  fLength = length;
  fRadius = radius;

  fInnerAxial = innerAxial; fInnerRadial = innerRadial;
  fOuterAxial = outerAxial; fOuterRadial = outerRadial;

  SortCoordinates(fInnerAxial, fInnerRadial, true);
  SortCoordinates(fOuterAxial, fOuterRadial, true);

  size_t nInner = fInnerAxial.size();

  fBottomRadiusAxial = 0.5*(pow(fInnerAxial[nInner-1],2) - pow(fInnerAxial[nInner-2],2) - pow(fInnerRadial[nInner-2],2))/(fInnerAxial[nInner-1] - fInnerAxial[nInner-2]);
  fBottomRadius = fInnerAxial[nInner-1] - fBottomRadiusAxial;

  //G4cout << "bottom circle radius = " << fBottomRadius << " and axial position = " << fBottomRadiusAxial << G4endl;
    
  // Evaluate TPC filling (inner edge) dimensions and coordinates  

  fBottomRadiusFilling = fBottomRadius + fBottomThickness;
  
  fInnerAxialFilling = fInnerAxial;
  fInnerRadialFilling = fInnerRadial;

  size_t last = fInnerAxial.size() - 1;

  fInnerAxialFilling[last] += fBottomThickness;
  
  fInnerAxialFilling[last-1] = fBottomRadiusAxial + fBottomRadiusFilling/sqrt(1 + pow(fInnerRadial[last-1]/(fInnerAxial[last-1] - fBottomRadiusAxial),2));
  fInnerRadialFilling[last-1] = fInnerRadial[last-1]*(fInnerAxialFilling[last-1] - fBottomRadiusAxial)/(fInnerAxial[last-1] - fBottomRadiusAxial);

  if(last >= 2)
  {
    for(int i = last-2; i >= 0; i--)
    {
      fInnerAxialFilling[i] = fInnerAxial[i] + fBottomThickness/sqrt(1 + pow((fInnerAxial[i] - fInnerAxial[i+1])/(fInnerRadial[i] - fInnerRadial[i+1]),2));
      fInnerRadialFilling[i] = fInnerRadial[i] - (fInnerAxial[i] - fInnerAxial[i+1])*(fInnerAxialFilling[i] - fInnerAxial[i])/(fInnerRadial[i] - fInnerRadial[i+1]);
    }
  }
  fInnerAxialFilling[0] = std::max(fInnerAxialFilling[0],fInnerAxial[0] + fBottomThickness);

  //for(size_t i = 0; i < fInnerAxialFilling.size(); i++)
  //  G4cout << i << " inner filling " << fInnerAxialFilling.at(i) << " " << fInnerRadialFilling.at(i) << G4endl;

  fOuterAxialFilling = fOuterAxial;
  fOuterRadialFilling = fOuterRadial;

  size_t corner = fOuterAxial.size()-1;
  for(size_t i = 0; i <= corner ; i++)
  {
    fOuterAxialFilling[i] = fOuterAxial[i] + fBottomThickness/sqrt(1 + pow((fOuterAxial[i] - fOuterAxial[i+1])/(fOuterRadial[i] - fOuterRadial[i+1]),2));
    fOuterRadialFilling[i] = fOuterRadial[i]  - (fOuterAxial[i] - fOuterAxial[i+1])*(fOuterAxialFilling[i] - fOuterAxial[i])/(fOuterRadial[i] - fOuterRadial[i+1]);
  }
  fOuterAxialFilling[corner] = fOuterAxialFilling[corner-1] + (fOuterAxial[corner] - fOuterAxial[corner-1]);
  fOuterRadialFilling[corner] = fOuterRadial[corner] - fThickness;

  fOuterAxialFilling[0] = std::max(fOuterAxialFilling[0],fOuterAxial[0] + fBottomThickness);

  if(fOuterRadialFilling[0] < fInnerRadialFilling[0])
  {
    G4double tempRadial = fOuterRadialFilling[0];
    fOuterRadialFilling[0] = fInnerRadialFilling[0];
    fInnerRadialFilling[0] = tempRadial;
  }

  //for(size_t i = 0; i < fOuterAxialFilling.size(); i++)
  //  G4cout << i << " outer filling " << fOuterAxialFilling.at(i) << " " << fOuterRadialFilling.at(i) << G4endl;

}

void nEXOTPCVesselSodaCanConstructor::SortCoordinates(std::vector<G4double>& axial, std::vector<G4double>& radial, bool ascending)
{
  size_t nCoords = axial.size();
  if(nCoords != radial.size())
  {
    G4cerr << "Radial and axial coordiantes sizes differ. Cannot continue, will exit..." << G4endl;
    exit(1);
  }
  
  std::map<G4double, G4double> sorter;
  for(size_t i = 0; i < nCoords; i++)
    sorter[axial.at(i)] = radial.at(i);

  axial.clear();
  radial.clear();
  if(ascending)
  {
    for(std::map<G4double, G4double>::iterator sorted = sorter.begin(); sorted != sorter.end(); sorted++)
    {
      axial.push_back(sorted->first);
      radial.push_back(sorted->second);
    }
  }
  else
  {
    for(std::map<G4double, G4double>::reverse_iterator sorted = sorter.rbegin(); sorted != sorter.rend(); sorted++)
    {
      axial.push_back(sorted->first);
      radial.push_back(sorted->second);
    }
  }
}

G4LogicalVolume* nEXOTPCVesselSodaCanConstructor::GetPiece(void)
{
  std::vector<double> zPlane;
  std::vector<double> rInner;
  std::vector<double> rOuter;
  FillCoordinates(zPlane,rInner,rOuter);
  CentralizeCoordinates(zPlane);
  CentralizeMainCoordinates(zPlane,false);
  //G4cout << "TPC vessel edge...\n";
  //PrintCoordinates(zPlane,rInner,false);
  //PrintCoordinates(zPlane,rOuter,true);

  G4Polycone* shape = new G4Polycone(GetName(), 0, 360*deg, zPlane.size(), &zPlane[0], &rInner[0], &rOuter[0]);
  G4LogicalVolume* logicTPC = new G4LogicalVolume(shape,FindMaterial(GetMaterial()),GetName());

  G4VisAttributes* tpcAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
  tpcAtt->SetVisibility(true);
  //tpcAtt->SetForceSolid(true);
  logicTPC->SetVisAttributes(tpcAtt);

  return logicTPC;
}

G4VSolid* nEXOTPCVesselSodaCanConstructor::GetFillingShape(void)
{
  std::vector<double> zPlane;
  std::vector<double> rInner;
  std::vector<double> rOuter;
  FillCoordinatesFilling(zPlane,rInner,rOuter);
  CentralizeCoordinates(zPlane);
  CentralizeMainCoordinates(zPlane,true);
  //G4cout << "TPC filling edge...\n";
  //PrintCoordinates(zPlane,rInner,false);
  //PrintCoordinates(zPlane,rOuter,true);

  G4String name = GetName() + "/Filling";

  return new G4Polycone(GetName(), 0, 360*deg, zPlane.size(), &zPlane[0], &rInner[0], &rOuter[0]); // new G4Tubs(name,0,GetInnerRadius(),GetInnerLength()/2.,0,360*deg);
}

void nEXOTPCVesselSodaCanConstructor::CentralizeCoordinates(std::vector<double>& z)
{
  // polycone is created starting from z = 0 (but could be any z_start), so the center is at the bottom/top/z_start of object
  // this function translate values of z to make 0 the center 

  G4double zmax = *std::max_element(z.begin(),z.end());
  G4double zmin = *std::min_element(z.begin(),z.end());
  G4double tz = (zmax + zmin)/2.; // remove zmin and then (zmax - zmin)/2 => translate by (zmax + zmin)/2
  for(std::vector<double>::iterator zi = z.begin(); zi != z.end(); zi++)
    (*zi) -= tz;

  // make the 
}

void nEXOTPCVesselSodaCanConstructor::CentralizeMainCoordinates(std::vector<double>& z, bool isFilling)
{

  G4double zmax = *std::max_element(z.begin(),z.end());
  G4double zmin = *std::min_element(z.begin(),z.end());

  double top = isFilling ? zmax : zmax - fTopThickness;
  double bottom = isFilling ? zmax + fTopThickness - fLength + fBottomThickness : zmax - fLength + fBottomThickness;
  double center = (top + bottom)/2.;

  for(std::vector<double>::iterator zi = z.begin(); zi != z.end(); zi++)
    (*zi) -= center;  
}


void nEXOTPCVesselSodaCanConstructor::FillCoordinates(std::vector<double>& zPlane, std::vector<double>& rInner, std::vector<double>& rOuter)
{

  // list all axial (z) positions to be evaluated and calculate radius along

  // z belonging to bottom round circle
  size_t nZpoints = fInnerAxial.size();
  G4double zi(fInnerAxial.at(nZpoints-1)), zf(fInnerAxial.at(nZpoints-2));
  G4double zs = fabs(zi - zf)*1./fNzPlanes;
  for(G4double z = zi; z > zf; z -= zs)
    zPlane.push_back(z);
  for(std::vector<G4double>::iterator axial = fInnerAxial.begin(); axial != fInnerAxial.end(); axial++)
    zPlane.push_back(*axial);
  for(std::vector<G4double>::iterator axial = fOuterAxial.begin(); axial != fOuterAxial.end(); axial++)
    zPlane.push_back(*axial);
  
  G4double zmax = *std::max_element(fInnerAxial.begin(),fInnerAxial.end());
  zPlane.push_back(zmax + fLength);
  //zPlane.push_back(*fInnerAxial.begin() + fLength); // fInnerAxial is sorted 

  // sort and remove duplicated z's
  std::sort(zPlane.begin(),zPlane.end());
  zPlane.erase(std::unique(zPlane.begin(),zPlane.end()), zPlane.end());

  
  for(std::vector<G4double>::iterator axial = zPlane.begin(); axial != zPlane.end(); axial++)
  {
    G4double z = *axial;
    //G4cout << "Evaluating for z = " << z << G4endl;
    G4double inner = EvalInnerRadius(z);
    //G4cout << "Inner = " << inner << G4endl;
    rInner.push_back(inner);
    G4double outer = EvalOuterRadius(z);
    //G4cout << "Outer = " << outer << G4endl;
    rOuter.push_back(outer);
  }

}

void nEXOTPCVesselSodaCanConstructor::FillCoordinatesFilling(std::vector<double>& zPlane, std::vector<double>& rInner, std::vector<double>& rOuter)
{

  // list all axial (z) positions to be evaluated and calculate radius along

  // z belonging to bottom round circle
  size_t nZpoints = fInnerAxialFilling.size();
  G4double zi(fInnerAxialFilling.at(nZpoints-1)), zf(fInnerAxialFilling.at(nZpoints-2));
  G4double zs = fabs(zi - zf)*1./fNzPlanes;
  for(G4double z = zi; z > zf; z -= zs)
    zPlane.push_back(z);
  for(std::vector<G4double>::iterator axial = fInnerAxialFilling.begin(); axial != fInnerAxialFilling.end(); axial++)
    zPlane.push_back(*axial);
  for(std::vector<G4double>::iterator axial = fOuterAxialFilling.begin(); axial != fOuterAxialFilling.end(); axial++)
    zPlane.push_back(*axial);
  
  G4double zmax = *std::max_element(fInnerAxial.begin(),fInnerAxial.end());
  zPlane.push_back(zmax + fLength - fTopThickness);
  //zPlane.push_back(*fInnerAxial.end() + fLength - fTopThickness );

  // sort and remove duplicated z's
  std::sort(zPlane.begin(),zPlane.end());
  zPlane.erase(std::unique(zPlane.begin(),zPlane.end()), zPlane.end());

  
  for(std::vector<G4double>::iterator axial = zPlane.begin(); axial != zPlane.end(); axial++)
  {
    G4double z = *axial;
    //G4cout << "Evaluating filling for z = " << z << G4endl;
    G4double inner = EvalInnerRadiusFilling(z);
    //G4cout << "Inner filling = " << inner << G4endl;
    rInner.push_back(inner);
    G4double outer = EvalOuterRadiusFilling(z);
    //G4cout << "Outer filling = " << outer << G4endl;
    rOuter.push_back(outer);
  }

}

G4double nEXOTPCVesselSodaCanConstructor::EvalInnerRadius(G4double z)
{
  // Return the inner radius of the edge of the TPC vessel as a function of the axial length z
  // If outside proper vessel z-limits, between zero and max length, returns 0

  size_t nZpoints = fInnerAxial.size();
  if(nZpoints < 3)
    return 0.;
  
  if(z > fInnerAxial.at(nZpoints-1) || z < fInnerAxial.at(0))
    return 0.;

  if(fInnerAxial.at(nZpoints-1) >= z && z >= fInnerAxial.at(nZpoints-2))
  {
    z -= fInnerAxial.at(nZpoints-1);
    return sqrt(pow(fBottomRadius,2) - pow(fabs(fBottomRadius) - fabs(z),2));
  }

  for(size_t i = 2; i < nZpoints; i++)
  {
    if(fInnerAxial.at(nZpoints-i) >= z && z >= fInnerAxial.at(nZpoints-(i+1)))
    {
      z -= fInnerAxial.at(nZpoints-i);
      return fInnerRadial.at(nZpoints-i) + (fInnerRadial.at(nZpoints-(i+1)) - fInnerRadial.at(nZpoints-i))*1./(fInnerAxial.at(nZpoints-(i+1)) - fInnerAxial.at(nZpoints-i)) * z ;
    }
  }

  return 0.;
}

G4double nEXOTPCVesselSodaCanConstructor::EvalOuterRadius(G4double z)
{
  // Return the inner radius of the edge of the TPC vessel as a function of the axial length z
  // If outside proper vessel z-limits, between zero and max length, returns 0

  size_t nZpoints = fOuterAxial.size();
  if(nZpoints < 3)
    return 0.;

  if(z < fOuterAxial.at(0))
    return 0.;
  if(z > fOuterAxial.at(nZpoints - 1))
    return fOuterRadial.at(nZpoints - 1);

  for(size_t i = 0; i < nZpoints-1; i++)
  {
    if(fOuterAxial.at(i) <= z && z <= fOuterAxial.at(i+1))
    {
      z -= fOuterAxial.at(i);
      return fOuterRadial.at(i) + (fOuterRadial.at(i+1) - fOuterRadial.at(i))*1./(fOuterAxial.at(i+1) - fOuterAxial.at(i)) * z ;
    }
  }

  return 0.;
}

G4double nEXOTPCVesselSodaCanConstructor::EvalInnerRadiusFilling(G4double z)
{
  // Return the inner radius of the edge of the TPC vessel as a function of the axial length z
  // If outside proper vessel z-limits, between zero and max length, returns 0

  size_t nZpoints = fInnerAxialFilling.size();
  if(nZpoints < 3)
    return 0.;
  
  if(z > fInnerAxialFilling.at(nZpoints-1) || z < fInnerAxialFilling.at(0))
    return 0.;

  if(fInnerAxialFilling.at(nZpoints-1) >= z && z >= fInnerAxialFilling.at(nZpoints-2))
  {
    z -= fInnerAxialFilling.at(nZpoints-1);
    return sqrt(pow(fBottomRadiusFilling,2) - pow(fabs(fBottomRadiusFilling) - fabs(z),2));
  }

  for(size_t i = 2; i < nZpoints; i++)
  {
    if(fInnerAxialFilling.at(nZpoints-i) >= z && z >= fInnerAxialFilling.at(nZpoints-(i+1)))
    {
      z -= fInnerAxialFilling.at(nZpoints-i);
      return fInnerRadialFilling.at(nZpoints-i) + (fInnerRadialFilling.at(nZpoints-(i+1)) - fInnerRadialFilling.at(nZpoints-i))*1./(fInnerAxialFilling.at(nZpoints-(i+1)) - fInnerAxialFilling.at(nZpoints-i)) * z ;
    }
  }

  return 0.;
}


G4double nEXOTPCVesselSodaCanConstructor::EvalOuterRadiusFilling(G4double z)
{
  // Return the inner radius of the edge of the TPC vessel as a function of the axial length z
  // If outside proper vessel z-limits, between zero and max length, returns 0

  size_t nZpoints = fOuterAxialFilling.size();
  if(nZpoints < 3)
    return 0.;

  if(z < fOuterAxialFilling.at(0))
    return 0.;
  if(z > fOuterAxialFilling.at(nZpoints - 1))
    return fOuterRadialFilling.at(nZpoints - 1);

  for(size_t i = 0; i < nZpoints-1; i++)
  {
    if(fOuterAxialFilling.at(i) <= z && z <= fOuterAxialFilling.at(i+1))
    {
      z -= fOuterAxialFilling.at(i);
      return fOuterRadialFilling.at(i) + (fOuterRadialFilling.at(i+1) - fOuterRadialFilling.at(i))*1./(fOuterAxialFilling.at(i+1) - fOuterAxialFilling.at(i)) * z ;
    }
  }

  return 0.;
}

void nEXOTPCVesselSodaCanConstructor::PrintCoordinates(std::vector<double>& zPlane, std::vector<double>& rOuter, bool increasing)
{
  G4cout << "Printing coordinates for TPC vessel: " << GetLocalName() << G4endl;
  for(size_t i = 0; i < zPlane.size(); i++)
  {
    //G4cout << "g.SetPoint(g.GetN()," << rInner.at(i) << "," << zPlane.at(i) << ")" << G4endl;
    if(increasing)
      G4cout << "g.SetPoint(g.GetN()," << rOuter.at(i) << "," << zPlane.at(i) << ")" << G4endl;
    else
      G4cout << "g.SetPoint(g.GetN()," << rOuter.at(zPlane.size() - 1 - i) << "," << zPlane.at(zPlane.size() - 1 - i) << ")" << G4endl;
      
    //G4cout << zPlane.at(i) << " " << rOuter.at(i) << G4endl;
  }
}

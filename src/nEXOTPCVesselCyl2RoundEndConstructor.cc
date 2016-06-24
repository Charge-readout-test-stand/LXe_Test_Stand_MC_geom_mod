#include "nEXOTPCVesselCyl2RoundEndConstructor.hh"
#include "G4UnionSolid.hh"

nEXOTPCVesselCyl2RoundEndConstructor::~nEXOTPCVesselCyl2RoundEndConstructor()
{
}

void nEXOTPCVesselCyl2RoundEndConstructor::Init(void)
{
  SetMessenger(new nEXOTPCVesselCyl2RoundEndMessenger(this));
  SetMaterial("G4_Cu");  

  fNzPlanes = 15;
  
  // Vance's model  
  fLength = 652.29*mm;
  fRadius = 1306/2.*mm;
  fThickness = 3.*mm;
  fOffLength = 2.*mm;
  fSmallCurvLength = 105.*mm;
  fSmallCurvRadius = 125.*mm;
  fSmallCurvThickness = 8.*mm;
  fCapRadius = 1142.52*mm;
  fCapThickness = 6.*mm;
  
}

void nEXOTPCVesselCyl2RoundEndConstructor::SetDimensions(G4double length, G4double radius, G4double thickness, G4double offset, G4double transition, G4double small, G4double smallthick, G4double cap, G4double capthick)
{
  fLength = length;
  fRadius = radius;
  fThickness = thickness;
  fOffLength = offset;
  fSmallCurvLength = transition;
  fSmallCurvRadius = small;
  fSmallCurvThickness = smallthick;
  fCapRadius = cap;
  fCapThickness = capthick;
}


G4LogicalVolume* nEXOTPCVesselCyl2RoundEndConstructor::GetPiece(void)
{
  std::vector<double> zPlane;
  std::vector<double> rOuter;
  FillCoordinates(zPlane,rOuter);
  CentralizeCoordinates(zPlane);
  //PrintCoordinates(zPlane,rOuter);

  G4Polycone* shape = new G4Polycone(GetName(), 0, 360*deg, zPlane.size(), &rOuter[0], &zPlane[0]);
  G4LogicalVolume* logicTPC = new G4LogicalVolume(shape,FindMaterial(GetMaterial()),GetName());

  G4VisAttributes* tpcAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
  tpcAtt->SetVisibility(true);
  //tpcAtt->SetForceSolid(true);
  logicTPC->SetVisAttributes(tpcAtt);

  return logicTPC;
}

G4VSolid* nEXOTPCVesselCyl2RoundEndConstructor::GetFillingShape(void)
{
  std::vector<double> zPlane;
  std::vector<double> rOuter;
  FillFillingCoordinates(zPlane,rOuter);
  CentralizeCoordinates(zPlane);
  //PrintCoordinates(zPlane,rOuter);

  G4String name = GetName() + "/Filling";

  return new G4Polycone(name, 0, 360*deg, zPlane.size(), &rOuter[0], &zPlane[0]);
}

void nEXOTPCVesselCyl2RoundEndConstructor::CentralizeCoordinates(std::vector<double>& z)
{
  // polycone is created starting from z = 0 (but could be any z_start), so the center is at the bottom/top/z_start of object
  // this function translate values of z to make 0 the center

  G4double zmax = *std::max_element(z.begin(),z.end());
  G4double zmin = *std::min_element(z.begin(),z.end());
  G4double tz = (zmax + zmin)/2.; // remove zmin and then (zmax - zmin)/2 => translate by (zmax + zmin)/2
  for(std::vector<double>::iterator zi = z.begin(); zi != z.end(); zi++)
    (*zi) -= tz;  
}

void nEXOTPCVesselCyl2RoundEndConstructor::FillCoordinates(std::vector<double>& zPlane, std::vector<double>& rOuter)
{
  // r(z) functions goes from zero to max length, only needs to evaluate the  rounds parts in fine steps
  
  G4double zEnd = GetCapAxialLength() + fSmallCurvLength;
  for(int i = 0; i <= fNzPlanes; i++)
  {
    double iEnd = i*zEnd*1./fNzPlanes;
    zPlane.push_back(iEnd);
    double radius = EvalRadius(iEnd);
    rOuter.push_back(EvalRadius(iEnd));
  }
  G4double zStart = GetTotalAxialLength() - zEnd;
  zEnd = GetTotalAxialLength();
  for(int i = 0; i <= fNzPlanes; i++)
  {
    double iEnd = zStart + i*(zEnd - zStart)*1./fNzPlanes;
    zPlane.push_back(iEnd);
    double radius = EvalRadius(iEnd);
    rOuter.push_back(EvalRadius(iEnd));
  }

}

G4double nEXOTPCVesselCyl2RoundEndConstructor::EvalRadius(G4double z)
{
  // Return the radius of the edge pf the TPC vessel as a function of the axial length z
  // If outside proper vessel z-limits, between zero and max length, returns 0
  
  G4double capLength = GetCapAxialLength();
  G4double maxLength = GetTotalAxialLength();
  
  if(0 > z || z > maxLength)
    return 0.;

  G4double zMax = capLength;
  if(z < zMax)
    return sqrt(pow(fCapRadius,2) - pow(fCapRadius - z,2));

  G4double capLengthRadius = sqrt(pow(fCapRadius,2) - pow(fCapRadius - capLength,2));
  zMax += fSmallCurvLength;
  if(z < zMax)
    return capLengthRadius + sqrt(pow(fSmallCurvRadius,2) - pow(fSmallCurvLength - (z - capLength),2)) - sqrt(pow(fSmallCurvRadius,2) - pow(fSmallCurvLength,2));

  zMax += fOffLength + fLength;
  if(z < zMax)
    return fRadius;

  return EvalRadius(maxLength - z);   
}


void nEXOTPCVesselCyl2RoundEndConstructor::GetFillingDimensions(G4double& inLength, G4double& inRadius, G4double& inOffLength, G4double& inSmallCurvLength, G4double& inSmallCurvRadius, G4double& inCapRadius)
{
  inLength = fLength;
  inRadius = fRadius - fThickness;
  inOffLength = fOffLength;
  inSmallCurvLength = fSmallCurvLength;
  inSmallCurvRadius = fSmallCurvRadius - fSmallCurvThickness;
  inCapRadius = fCapRadius - fCapThickness;
}

void nEXOTPCVesselCyl2RoundEndConstructor::FillFillingCoordinates(std::vector<double>& zPlane, std::vector<double>& rOuter)
{
  // r(z) functions goes from zero to max filling length, only needs to evaluate the  rounds parts in fine steps

  G4double inLength, inRadius, inOffLength, inSmallCurvLength, inSmallCurvRadius, inCapRadius;
  GetFillingDimensions(inLength, inRadius, inOffLength, inSmallCurvLength, inSmallCurvRadius, inCapRadius);

  G4double inCapLength = GetCapAxialLength(inCapRadius,inRadius,inSmallCurvRadius,inSmallCurvLength);
  G4double zEnd = inCapLength + inSmallCurvLength;
  for(int i = 0; i <= fNzPlanes; i++)
  {
    double iEnd = i*zEnd/fNzPlanes;
    zPlane.push_back(iEnd);
    rOuter.push_back(EvalFillingRadius(iEnd));
  }
  G4double inTotalAxialLength = 2*(inLength + inOffLength + inSmallCurvLength + inCapLength);  
  G4double zStart = inTotalAxialLength - zEnd;
  zEnd = inTotalAxialLength;
  for(int i = 0; i <= fNzPlanes; i++)
  {
    double iEnd = zStart + i*(zEnd - zStart)/fNzPlanes;
    zPlane.push_back(iEnd);
    rOuter.push_back(EvalFillingRadius(iEnd));
  }

}

G4double nEXOTPCVesselCyl2RoundEndConstructor::EvalFillingRadius(G4double z)
{
  // Return the radius of the filling of the TPC vessel as a function of the axial length z
  // If outside proper vessel z-limits, between zero and max inner length, returns 0

  G4double radius = 0.;

  G4double inLength, inRadius, inOffLength, inSmallCurvLength, inSmallCurvRadius, inCapRadius;
  GetFillingDimensions(inLength, inRadius, inOffLength, inSmallCurvLength, inSmallCurvRadius, inCapRadius);
  G4double inCapLength = GetCapAxialLength(inCapRadius,inRadius,inSmallCurvRadius,inSmallCurvLength);
  G4double maxLength = 2*(inLength + inOffLength + inSmallCurvLength + inCapLength);

  if(0 > z || z > maxLength)
    return radius;

  G4double zMax = inCapLength;
  if(z < zMax)
    return sqrt(pow(inCapRadius,2) - pow(inCapRadius - z,2));

  G4double inCapLengthRadius = sqrt(pow(inCapRadius,2) - pow(inCapRadius - inCapLength,2));
  zMax += inSmallCurvLength;
  if(z < zMax)
    return inCapLengthRadius + sqrt(pow(inSmallCurvRadius,2) - pow(inSmallCurvLength - (z - inCapLength),2)) - sqrt(pow(inSmallCurvRadius,2) - pow(inSmallCurvLength,2)) - (fSmallCurvThickness - fThickness);

  zMax += inOffLength + inLength;
  if(z < zMax)
    return inRadius;

  return EvalFillingRadius(maxLength - z);   
}

void nEXOTPCVesselCyl2RoundEndConstructor::PrintCoordinates(std::vector<double>& zPlane, std::vector<double>& rOuter)
{
  G4cout << "Printing coordinates for TPC vessel: " << GetLocalName() << G4endl;
  for(size_t i = 0; i < zPlane.size(); i++)
    G4cout << "g.SetPoint(g.GetN()," << rOuter.at(i) << "," << zPlane.at(i) << ")" << G4endl;
  //G4cout << zPlane.at(i) << " " << rOuter.at(i) << G4endl;
}

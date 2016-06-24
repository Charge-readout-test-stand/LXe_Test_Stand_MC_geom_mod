#include "nEXOTPCVesselConstructor.hh"
#include "G4UnionSolid.hh"

nEXOTPCVesselConstructor::~nEXOTPCVesselConstructor(){;}

void nEXOTPCVesselConstructor::Init(void)
{
  SetMessenger(new nEXOTPCVesselMessenger(this));

  SetThickness(0.3*cm);
  SetEndsThickness(0.66*cm);
  
  SetRadius1(650*mm + GetThickness());
  SetRadius2(65.*cm + GetThickness());
  SetLength(2*(65.*cm + GetEndsThickness()));

  //-------------------------------Test Stand-------------------------
  G4cout << " Setting the Vessel Radius and Height to 1cm thickness to 1mm "  << G4endl;
  SetTPCRadiusTestStand(1*cm);
  SetTPCHeightTestStand(1*cm);
  SetTPCThicknessTestStand(1*mm);
  //-----------------------------------------------------------------

  fOption = 0;
}

G4LogicalVolume* nEXOTPCVesselConstructor::GetPiece(void)
{
  G4cout << "//################################################//" << G4endl;
  G4cout << "//################ TPC Vessel ####################//" << G4endl;
  G4cout << "//################################################//" << G4endl;

  G4LogicalVolume* logicTPC = 0;
  if(fOption == 0) {
    logicTPC =  new G4LogicalVolume(new G4Cons(GetName(),
                                    0, GetRadius1(),
                                    0, GetRadius2(),
                                    GetLength()/2.,
                                    0, 360*deg),
                                    FindMaterial("G4_Cu"),
                                    GetName());
  }

  if(fOption == 1) {
    G4Tubs* mainTPC = new G4Tubs("mainTPC", 0, GetRadius2(), GetLength()/2., 0, 360*deg);
    G4Tubs* lotusTPC1 = new G4Tubs("lotusTPC1", GetRadius2(), GetRadius2()+2.0*cm, 3.1/2*cm, 0, 360*deg);
    G4Tubs* lotusTPC2 = new G4Tubs("lotusTPC2", (56-0.3)*cm, GetRadius2()+2.0*cm, 3.5/2*cm, 0, 360*deg);
    G4UnionSolid* partUnionTPC = new G4UnionSolid("main+tpc1", mainTPC, lotusTPC1, 0, G4ThreeVector(0, 0, -GetLength()/2.+3.1/2*cm));
    G4UnionSolid* solidTPC = new G4UnionSolid(GetName(), partUnionTPC, lotusTPC2, 0, G4ThreeVector(0, 0, -GetLength()/2.-3.5/2*cm));
    logicTPC = new G4LogicalVolume(solidTPC,
                                   FindMaterial("G4_Cu"),
                                   GetName());
  }
  if(fOption == 2) {
       //-----------------------------------Test Stand----------------------------------------
       
       G4cout << "Making the TPC vessel as a cylinder with R and H  " << fTPCRadiusTestStand << " " << fTPCHeightTestStand << G4endl;
      
       G4Tubs* mainTPC = new G4Tubs("mainTPC", 0,  fTPCRadiusTestStand, fTPCHeightTestStand, 0, 360*deg);
       logicTPC =  new G4LogicalVolume(mainTPC,
                                    FindMaterial("G4_STAINLESS-STEEL"),
                                    GetName());
  }

  G4VisAttributes* tpcAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
  tpcAtt->SetVisibility(false);
  //tpcAtt->SetForceSolid(true);
  logicTPC->SetVisAttributes(tpcAtt);

  return logicTPC;
}

void nEXOTPCVesselConstructor::SetOpticalSurface(G4String volName)
{
  //Optical Cu Surfaces
  const G4int nCuEntries = 32;
  //polycrystalline copper
  G4double copperPhotonEnergy[nCuEntries] =
            { 0.602*eV, 1.00*eV, 1.5*eV, 1.75*eV,
              1.90*eV, 2.40*eV, 2.80*eV, 3.4*eV,
              3.8*eV, 4.0*eV, 4.2*eV, 4.4*eV,
              4.6*eV, 4.8*eV, 5.0*eV, 5.2*eV,
              5.4*eV, 5.6*eV, 5.8*eV, 6.0*eV,
              6.5*eV, 7.0*eV, 7.5*eV, 8.0*eV,
              8.5*eV, 9.0*eV, 9.5*eV, 10*eV,
              11*eV, 12*eV, 13*eV, 14*eV };
  G4double copperRealRefractiveIndex[nCuEntries] =
            { 0.89, 0.433, 0.26, 0.214,
              0.214, 1.12, 1.17, 1.27,
              1.34, 1.34, 1.42, 1.49,
              1.52, 1.53, 1.47, 1.38,
              1.28, 1.18, 1.10, 1.04,
              0.958, 0.972, 1.01, 1.03,
              1.03, 1.03, 1.03, 1.04,
              1.07, 1.09, 1.08, 1.06 };
  G4double copperImaginaryRefractiveIndex[nCuEntries] =
            { 11.0, 8.46, 5.26, 4.24,
              3.67, 2.6,  2.36, 1.95,
              1.81, 1.72, 1.64, 1.64,
              1.67, 1.71, 1.78, 1.80,
              1.78, 1.74, 1.67, 1.59,
              1.37, 1.20, 1.09, 1.03,
              0.979, 0.921, 0.867, 0.818,
              0.754, 0.731, 0.724, 0.724 };

  G4OpticalSurface* tpcVesselRefOpSurface = new G4OpticalSurface("tpcVesselRefOpSurface");
  G4LogicalBorderSurface* tpcVesselRefSurface = NULL;
  tpcVesselRefSurface = new G4LogicalBorderSurface("tpcVesselRefSurface",
                                                   nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(volName),  // physTPCInternals, //physActiveLXe,
                                                   nEXOSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName()),// physTPCVessel, //physiTPCVessel,
                                                   tpcVesselRefOpSurface);

  tpcVesselRefOpSurface->SetModel(unified);
  tpcVesselRefOpSurface->SetType(dielectric_metal);
  tpcVesselRefOpSurface->SetFinish(polished);

  G4MaterialPropertiesTable* tpcVesselRefOpticalMPT  = new G4MaterialPropertiesTable();
  tpcVesselRefOpticalMPT->AddProperty("REALRINDEX", copperPhotonEnergy, copperRealRefractiveIndex, nCuEntries);
  tpcVesselRefOpticalMPT->AddProperty("IMAGINARYRINDEX", copperPhotonEnergy, copperImaginaryRefractiveIndex, nCuEntries);

  tpcVesselRefOpSurface->SetMaterialPropertiesTable(tpcVesselRefOpticalMPT);
}

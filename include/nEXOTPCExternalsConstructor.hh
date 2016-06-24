//////////////////////////////////////////
// nEXOTPCExternalsConstructor.hh //
// Construction of the external components to the nEXO TPC //
//////////////////////////////////////////

#ifndef nEXOTPCExternalsConstructor_h
#define nEXOTPCExternalsConstructor_h 1

#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "nEXOConstructor.hh"
#include "nEXOModuleConstructor.hh"
#include "nEXOComponentConstructor.hh"
#include "nEXOWaterTankConstructor.hh"
#include "nEXOOuterCryostatConstructor.hh"
#include "nEXOInnerCryostatConstructor.hh"
#include "nEXOOuterSphericalCryostatConstructor.hh"
#include "nEXOInnerSphericalCryostatConstructor.hh"


class nEXOTPCExternalsConstructor : public nEXOModuleConstructor
{
public:
  nEXOTPCExternalsConstructor(G4String name, nEXODetectorConstruction* construct) : nEXOModuleConstructor(name, construct){Init();};
  nEXOTPCExternalsConstructor(G4String name, nEXOConstructor* construct) : nEXOModuleConstructor(name, construct){Init();};
  virtual ~nEXOTPCExternalsConstructor();

  void SetVersion(G4int version) {fVersion = version;};
  G4int GetVersion(void) {return fVersion;};

  //----------------------Test Stand-------------------------
  void SetHFERadius(double HFERadius) {fHFERadius = HFERadius;};
  double GetHFERadius(void) {return fHFERadius;};
  
  void SetHFEHeight(double HFEHeight) {fHFEHeight = HFEHeight;};
  double GetHFEHeight(void) {return fHFEHeight;};

  void SetLabHeight(double height) {fLabHeight = height;};
  double GetLabHeight(void) {return fLabHeight;};

  void SetLabLength(double length) {fLabLength = length;};
  double GetLabLength(void) {return fLabLength;};

  void SetLabWidth(double width) {fLabWidth = width;};
  double GetLabWidth(void) {return fLabWidth;};

  void SetHFEDewarThickness(double thickness) {fHFEDewarThickness = thickness;};
  double GetHFEDewarThickness(void) {return fHFEDewarThickness;};
  //---------------------------------------------------------

  G4ThreeVector GetTPCVesselTranslation(void) {return fTPCVesselTranslation;};
  G4LogicalVolume* GetTPCVesselMotherVolume(void) {return fTPCVesselMotherVolume;};
  
  virtual G4LogicalVolume* GetPiece(void);

private:
  void Init(void);

  virtual G4LogicalVolume* GetPieceOld(G4LogicalVolume *logicExternals);
  virtual G4LogicalVolume* GetPieceV1(G4LogicalVolume *logicExternals);
  virtual G4LogicalVolume* GetPieceV2(G4LogicalVolume *logicExternals);
  virtual G4LogicalVolume* GetPieceTestStand(G4LogicalVolume *logicExternals);


  G4int fVersion;
  
  //-----------Test Stand------------
  double fHFERadius;
  double fHFEHeight;
  double fLabWidth;
  double fLabHeight;
  double fLabLength;
  double fHFEDewarThickness;
  //---------------------------------

  G4ThreeVector fTPCVesselTranslation;
  G4LogicalVolume* fTPCVesselMotherVolume;
};
#endif

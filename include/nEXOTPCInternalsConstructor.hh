//////////////////////////////////////////
// nEXOTPCInternalsConstructor.hh //
// Construction of the external components to the nEXO TPC //
//////////////////////////////////////////

#ifndef nEXOTPCInternalsConstructor_h
#define nEXOTPCInternalsConstructor_h 1

#include <map>
#include <algorithm>

#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4Cons.hh"
#include "G4Sphere.hh"
#include "G4Torus.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"


#include "nEXOConstructor.hh"
#include "nEXOModuleConstructor.hh"
#include "nEXOComponentConstructor.hh"
#include "nEXOTPCVesselConstructor.hh"
#include "nEXOLXeConstructor.hh"

class nEXOTPCInternalsConstructor : public nEXOModuleConstructor
{
public:
  nEXOTPCInternalsConstructor(G4String name, nEXODetectorConstruction* construct) : nEXOModuleConstructor(name, construct){Init();};
  nEXOTPCInternalsConstructor(G4String name, nEXOConstructor* construct) : nEXOModuleConstructor(name, construct){Init();};
  virtual ~nEXOTPCInternalsConstructor();

  virtual G4LogicalVolume* GetPiece(void);

  /// Set the dimensions of the internal components based on a given TPC vessel
  void SetDimensionsForTPCVessel(nEXOTPCVesselConstructor& tpcVessel);

  void SetTPCVesselRef (G4double ref){fTPCVesselRef = ref;};
  void SetFieldRingRef (G4double ref){fFieldRingRef = ref;};
  void SetFieldRingLength (G4double len){fFieldRingLength = len;};
  void SetFieldRingThick (G4double thick){fFieldRingThick = thick;};
  void SetAnodeRef (G4double ref){fAnodeRef = ref;};
  void SetCathodeRef (G4double ref){fCathodeRef = ref;};
  void SetTeflonRef (G4double ref){fTeflonRef = ref;};
  void SetBulgeRef (G4double ref){fBulgeRef = ref;};
  void SetBulgeRadius (G4double r){fBulgeRadius = r;};
  void SetActiveSiPMPadFrac(G4double frac ){fActiveSiPMPadFrac = frac;};
  void SetWWDistance(G4double dist ){fWWDistance = dist;};
  void SetWWDistanceOfAnode(G4double dist ){fWWDistanceOfAnode = dist;};
  void SetSiPMOption(G4int opt ){fSiPMOption = opt;};
  void SetAnodeOption(G4int opt ){fAnodeOption = opt;};
  void SetUseWLS(G4int wls ){fUseWLS = wls;};
  void SetRingArrangeID(G4int id ){fRingArrangeID = id;};
  void SetRingSideDistance(G4double dis ){fRingSideDistance = dis;};
  void SetRadialRingDistance(G4double dis ){fRadialRingDistance = dis;};
  void SetInternalsFlag(G4int flag ){fInternalsFlag = flag;};

  void SetCathodeAnodeDistance(G4double ref){fCathodeAnodeDistance = ref;};
  void SetCathodeToBottomRingDistance(G4double ref){fCathodeToBottomRingDistance = ref;};
  void SetAnodeToTopRingDistance(G4double ref){fAnodeToTopRingDistance = ref;};
  void SetAnodeRadius(G4double ref){fAnodeRadius = ref;};
  void SetAnodeThickness(G4double ref){fAnodeThickness = ref;};
  void SetAnodeZPosition(G4double ref){fAnodeZPosition = ref;};
  void SetCathodeRadius(G4double ref){fCathodeRadius = ref;};
  void SetCathodeThickness(G4double ref){fCathodeThickness = ref;};
  void SetCathodeZPosition(G4double ref){fCathodeZPosition = ref;};
  void SetSiPMTotalHeight(G4double ref){fSiPMTotalHeight = ref;};
  void SetSiPMPadHeight(G4double ref){fSiPMPadHeight = ref;};
  void SetSiPMPadThickness(G4double ref){fSiPMPadThickness = ref;};
  void SetSiPMStandoffFromTPC(G4double ref){fSiPMStandoffFromTPC = ref;};
  void SetChargeModuleLength(G4double ref){fChargeModuleLength = ref;};
  void SetChargeModuleThickness(G4double ref){fChargeModuleThickness = ref;};
  void SetChargeModuleBackingThickness(G4double ref){fChargeModuleBackingThickness = ref;};
  void SetChargePadDiagonalLength(G4double ref){fChargePadDiagonalLength = ref;};
  void SetChargePadThickness(G4double ref){fChargePadThickness = ref;};
  void SetChargePadLinkLength(G4double ref){fChargePadLinkLength = ref;};
  void SetChargePadLinkHeight(G4double ref){fChargePadLinkHeight = ref;};
  void SetSiPMActiveLayerThickness(G4double ref){fSiPMActiveLayerThickness = ref;};
  void SetSiPMActiveLayerDepth(G4double ref){fSiPMActiveLayerDepth = ref;};
  void SetSiPMBaseLayerThickness(G4double ref){fSiPMBaseLayerThickness = ref;};
  void SetSiPMWLSThickness(G4double ref){fSiPMWLSThickness = ref;};

  

private:
  void Init(void);
  void BuildOtherParts(G4LogicalVolume* logicLXe, G4double inactiveLXeR1, G4double inactiveLXeR2, G4double inactiveLXeH);
  void SetOpticalSurfaces();

  G4double fTPCVesselRef;
  G4double fFieldRingRef;
  G4double fFieldRingLength;
  G4double fFieldRingThick;
  G4double fAnodeRef;
  G4double fCathodeRef;
  G4double fBulgeRadius;
  G4double fBulgeRef;
  G4double fTeflonRef;
  G4double fActiveSiPMPadFrac;
  G4double fWWDistance;
  G4double fWWDistanceOfAnode;
  G4int fSiPMOption;
  G4int fAnodeOption;
  G4int fRingArrangeID;
  G4int fUseWLS;
  G4double fRingSideDistance;
  G4double fTPCLowerRadius;
  G4double fRadialRingDistance;
  G4int fInternalsFlag;

  G4int fNSiPMPadAlongZ;
  G4int fNSiPMPadAlongPhi[10000];

  G4int fNSiPMPadAlongY;
  G4int fNSiPMPadAlongX[10000];
  
  G4double fPadDiagonalLength;
  G4double fPadThickness;
  G4double fPadLinkXLength;
  G4double fPadLinkY1Length;
  G4double fPadLinkHeight;
  G4double fModuleLength;
  G4double fModuleThickness;
  G4double fSiliconThickness;
  G4double fModuleZ;

  G4double fCathodeAnodeDistance;
  G4double fCathodeToBottomRingDistance;
  G4double fAnodeToTopRingDistance;
  G4double fAnodeRadius;
  G4double fAnodeThickness;
  G4double fAnodeZPosition;
  G4double fCathodeRadius;
  G4double fCathodeThickness;
  G4double fCathodeZPosition;
  G4double fSiPMTotalHeight;
  G4double fSiPMPadHeight;
  G4double fSiPMPadThickness;
  G4double fSiPMStandoffFromTPC;
  G4double fChargeModuleLength;
  G4double fChargeModuleThickness;
  G4double fChargeModuleBackingThickness;
  G4double fChargePadDiagonalLength;
  G4double fChargePadThickness;
  G4double fChargePadLinkLength;
  G4double fChargePadLinkHeight;
  G4double fSiPMActiveLayerThickness;
  G4double fSiPMActiveLayerDepth;
  G4double fSiPMBaseLayerThickness;
  G4double fSiPMWLSThickness;
 


};
#endif

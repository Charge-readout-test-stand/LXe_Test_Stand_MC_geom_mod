#ifndef nEXOTPCVesselSodaCanConstructor_h
#define nEXOTPCVesselSodaCanConstructor_h 1

#include "G4SystemOfUnits.hh"
#include "G4Polycone.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

#include "TString.h"

#include "nEXOTPCVesselConstructor.hh"
#include "nEXOTPCVesselModelConstructor.hh"

/// Construct the TPC Vessel of the nEXO detector.
class nEXOTPCVesselSodaCanConstructor : public nEXOTPCVesselModelConstructor
{
public:
  nEXOTPCVesselSodaCanConstructor(G4String name, nEXODetectorConstruction* construct) : nEXOTPCVesselModelConstructor(name,construct){Init();};
  nEXOTPCVesselSodaCanConstructor(G4String name, nEXOConstructor* construct) : nEXOTPCVesselModelConstructor(name,construct){Init();};
  virtual ~nEXOTPCVesselSodaCanConstructor();

  /// Get the (logic) TPC vessel
  virtual G4LogicalVolume* GetPiece(void);
  /// Get the filling shape (usually for LXe)
  virtual G4VSolid* GetFillingShape(void);

  /// Get inner radius (available for placement on internal components)
  G4double GetInnerRadius(){return fRadius - fThickness;}
  /// Get inner length (available for placement on internal components)
  G4double GetInnerLength(){return fLength - fTopThickness - fBottomThickness;} 
  /// Get inner radius top (available for placement on internal components)
  double GetInnerTopRadius(){return GetInnerRadius();}
  /// Get inner radius bottom (available for placement on internal components)
  double GetInnerBottomRadius(){return GetInnerRadius();}

  /// Set the dimensions for this TPC vessel model
  void SetDimensions(G4double length, G4double radius, G4double thickness, G4double top, G4double bottom,
                     std::vector<G4double>& innerAxial, std::vector<G4double>& innerRadial,
                     std::vector<G4double>& outerAxial, std::vector<G4double>& outerRadial);

  /// Get the total axial length for this vessel
  //G4double GetTotalAxialLength(void){return 2*(fLength + fOffLength + fSmallCurvLength + GetCapAxialLength());}

private:
  void Init(void);

  /// The TPC vessel main body length
  G4double fLength;
  /// The TPC vessel main body radius
  G4double fRadius;
  /// The TPC vessel main body thickness
  G4double fThickness;

  /// The TPC vessel top cap thickness
  G4double fTopThickness;
  /// The TPC vessel bottom round cap thickness
  G4double fBottomThickness;

  /// The bottom reference axial and radial coordinates
  std::vector<G4double> fInnerAxial;
  std::vector<G4double> fInnerRadial;
  /// The bottom outer edge axial and radial coordinates
  std::vector<G4double> fOuterAxial;
  std::vector<G4double> fOuterRadial; 
  /// Number of planes to draw the round bottom
  G4int fNzPlanes;

  G4double fBottomRadiusAxial;
  G4double fBottomRadius;

  G4double fBottomRadiusFilling;
  std::vector<G4double> fInnerAxialFilling;
  std::vector<G4double> fInnerRadialFilling;
  std::vector<G4double> fOuterAxialFilling;
  std::vector<G4double> fOuterRadialFilling; 
  

  /// Get the axial length for the cap part for this vessel
  //G4double GetCapAxialLength(void){return GetCapAxialLength(fCapRadius,fRadius,fSmallCurvRadius,fSmallCurvLength);}
  /// Evaluate the axial length for the cap part given dimensions
  //G4double GetCapAxialLength(G4double rCap, G4double rBody, G4double rSmall, G4double lSmall){return rCap - sqrt(pow(rCap,2) - pow(rBody - rSmall + sqrt(pow(rSmall,2) - pow(lSmall,2)),2));}

  /// Translate vector of axial positions to center values around zero
  void CentralizeCoordinates(std::vector<double>& z);
  void CentralizeMainCoordinates(std::vector<double>& z, bool isFilling);
  
  /// Fill the coordinate values of the edge of the TPC vessel
  void FillCoordinates(std::vector<double>& zPlane, std::vector<double>& rInner, std::vector<double>& rOuter);
  /// Evaluate the radius of the edge of the TPC vessel as a function of the axial length z
  G4double EvalInnerRadius(G4double z);
  G4double EvalOuterRadius(G4double z);

  void FillCoordinatesFilling(std::vector<double>& zPlane, std::vector<double>& rInner, std::vector<double>& rOuter);
  /// Evaluate the radius of the edge of the TPC vessel as a function of the axial length z
  G4double EvalInnerRadiusFilling(G4double z);
  G4double EvalOuterRadiusFilling(G4double z);

  /// Fille the coordinate values of the inner edge of the TPC vessel (edges of LXe/filling)
  //void FillFillingCoordinates(std::vector<double>& zPlane, std::vector<double>& rOuter);
  /// Evaluate the radius of the filling of the TPC vessel as a function of the axial length z
  //G4double EvalFillingRadius(G4double z);

  void SortCoordinates(std::vector<G4double>& axial, std::vector<G4double>& radial, bool ascending);
  /// Print coordinates
  void PrintCoordinates(std::vector<double>& zPlane, std::vector<double>& rOuter, bool increasing);


};

class nEXOTPCVesselSodaCanMessenger : public nEXOComponentConstructorMessenger
{
private:
  nEXOTPCVesselSodaCanConstructor *fConstructor;

public:
  nEXOTPCVesselSodaCanMessenger(nEXOTPCVesselSodaCanConstructor* construct)
    : nEXOComponentConstructorMessenger(construct, Form("Control the geometry of the TPC vessel model: %s.", construct->GetLocalName().data()) ),
      fConstructor(construct){
  };

  ~nEXOTPCVesselSodaCanMessenger(){
  };
  
  void SetNewValue(G4UIcommand* cmd, G4String val){
    nEXOComponentConstructorMessenger::SetNewValue(cmd,val);
  };

};

#endif


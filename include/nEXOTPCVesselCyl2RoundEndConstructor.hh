#ifndef nEXOTPCVesselCyl2RoundEndConstructor_h
#define nEXOTPCVesselCyl2RoundEndConstructor_h 1

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
class nEXOTPCVesselCyl2RoundEndConstructor : public nEXOTPCVesselModelConstructor
{
public:
  nEXOTPCVesselCyl2RoundEndConstructor(G4String name, nEXODetectorConstruction* construct) : nEXOTPCVesselModelConstructor(name,construct){Init();};
  nEXOTPCVesselCyl2RoundEndConstructor(G4String name, nEXOConstructor* construct) : nEXOTPCVesselModelConstructor(name,construct){Init();};
  virtual ~nEXOTPCVesselCyl2RoundEndConstructor();

  /// Get the (logic) TPC vessel
  virtual G4LogicalVolume* GetPiece(void);
  /// Get the filling shape (usually for LXe)
  virtual G4VSolid* GetFillingShape(void);

  /// Get inner radius (available for placement on internal components)
  G4double GetInnerRadius(){return fRadius - fThickness;}
  /// Get inner length (available for placement on internal components)
  G4double GetInnerLength(){return fLength;} 
  /// Get inner radius top (available for placement on internal components)
  double GetInnerTopRadius(){return GetInnerRadius();}
  /// Get inner radius bottom (available for placement on internal components)
  double GetInnerBottomRadius(){return GetInnerRadius();}

  /// Set the dimensions for this TPC vessel model
  void SetDimensions(G4double length, G4double radius, G4double thickness, G4double offset, G4double transition, G4double small, G4double smallthick, G4double cap, G4double capthick);

  /// Get the total axial length for this vessel
  G4double GetTotalAxialLength(void){return 2*(fLength + fOffLength + fSmallCurvLength + GetCapAxialLength());}

private:
  void Init(void);

  /// The TPC vessel main body length
  G4double fLength;
  /// The TPC vessel main body radius
  G4double fRadius;
  /// The TPC vessel main body thickness
  G4double fThickness;

  /// The TPC offset length with extra thickness that is equal that for first/small curvature radius part
  G4double fOffLength;  
  /// The TPC axial distance with small curvature radius
  G4double fSmallCurvLength;
  /// The TPC axial distance with small curvature radius
  G4double fSmallCurvRadius;
  /// The thickness of the TPC along small curvature radius
  G4double fSmallCurvThickness;
  
  /// The TPC axial distance with large curvature radius (caps)
  G4double fCapRadius;
  /// The thickness of the TPC along the caps
  G4double fCapThickness;

  /// Number of planes to draw the round caps
  G4int fNzPlanes;

  /// Get the axial length for the cap part for this vessel
  G4double GetCapAxialLength(void){return GetCapAxialLength(fCapRadius,fRadius,fSmallCurvRadius,fSmallCurvLength);}
  /// Evaluate the axial length for the cap part given dimensions
  G4double GetCapAxialLength(G4double rCap, G4double rBody, G4double rSmall, G4double lSmall){return rCap - sqrt(pow(rCap,2) - pow(rBody - rSmall + sqrt(pow(rSmall,2) - pow(lSmall,2)),2));}

  /// Translate vector of axial positions to center values around zero
  void CentralizeCoordinates(std::vector<double>& z);
  
  /// Fill the coordinate values of the edge of the TPC vessel
  void FillCoordinates(std::vector<double>& zPlane, std::vector<double>& rOuter);
  /// Evaluate the radius of the edge of the TPC vessel as a function of the axial length z
  G4double EvalRadius(G4double z);

  /// Get the dimensions of the filling shape of the TPC vessel
  void GetFillingDimensions(G4double& inLength, G4double& inRadius, G4double& inOffLength, G4double& inSmallCurvLength, G4double& inSmallCurvRadius, G4double& inCapRadius);

  /// Fille the coordinate values of the inner edge of the TPC vessel (edges of LXe/filling)
  void FillFillingCoordinates(std::vector<double>& zPlane, std::vector<double>& rOuter);
  /// Evaluate the radius of the filling of the TPC vessel as a function of the axial length z
  G4double EvalFillingRadius(G4double z);

  /// Print coordinates
  void PrintCoordinates(std::vector<double>& zPlane, std::vector<double>& rOuter);


};

class nEXOTPCVesselCyl2RoundEndMessenger : public nEXOComponentConstructorMessenger
{
private:
  nEXOTPCVesselCyl2RoundEndConstructor *fConstructor;

public:
  nEXOTPCVesselCyl2RoundEndMessenger(nEXOTPCVesselCyl2RoundEndConstructor* construct)
    : nEXOComponentConstructorMessenger(construct, Form("Control the geometry of the TPC vessel model: %s.", construct->GetLocalName().data()) ),
      fConstructor(construct){
  };

  ~nEXOTPCVesselCyl2RoundEndMessenger(){
  };
  
  void SetNewValue(G4UIcommand* cmd, G4String val){
    nEXOComponentConstructorMessenger::SetNewValue(cmd,val);
  };

};

#endif


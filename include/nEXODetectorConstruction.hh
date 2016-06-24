////////////////////////////////////////////////////////
// nEXODetectorConstruction.hh                        //
// Coarse nEXO Geometry header file                   //
////////////////////////////////////////////////////////

#ifndef nEXODetectorConstruction_h
#define nEXODetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

#include "globals.hh"

#include "nEXOSimplePhysVolManager.hh"

class G4VPhysicalVolume;
class G4UserLimits;

class nEXOMaterials;

class nEXODetectorMessenger;

class nEXOConstructor;

class nEXODetectorConstruction : public G4VUserDetectorConstruction
{
public:
  
  nEXODetectorConstruction();
  virtual ~nEXODetectorConstruction();

  // Construct the detector and define the world volume.
  virtual G4VPhysicalVolume* Construct();

  /// Return the detector construction messenger.
  virtual nEXODetectorMessenger* GetMessenger(void){
    return fDetectorMessenger;
  }

  /// Update the geometry information to match stuff read from the macro file.
  void UpdateGeometry();

  void SetMaxStep (G4double );
  void SetCheckOverlaps(G4bool );
  
protected:

  /// This really constructs the detector, but doesn't define materials before it is constructed.
  /// This is called by Construct()
  G4VPhysicalVolume* ConstructDetector();
  
  /// A messenger for the DetectorConstruction object.
  nEXODetectorMessenger* fDetectorMessenger;
  
  /// A constructor to create the world.
  nEXOConstructor* fWorldConstructor;
  
private:

  G4UserLimits* fStepLimit;

  G4bool  fCheckOverlaps;


};

#endif

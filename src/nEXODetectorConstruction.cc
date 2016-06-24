////////////////////////////////////////////////////////
// nEXODetectorConstruction.cc                        //
// Coarse nEXO Geometry                               //
////////////////////////////////////////////////////////

#include "nEXODetectorConstruction.hh"
#include "nEXODetectorMessenger.hh"

#include "G4Material.hh"
#include "nEXOMaterials.hh"
#include "nEXOAnalysis.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Torus.hh"
#include "G4Cons.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4Region.hh"
#include "G4RegionStore.hh"

#include "G4RunManager.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"

#include "G4SDManager.hh"
#include "nEXOSensitiveDetector.hh"

#include "nEXOWorldConstructor.hh"

nEXODetectorConstruction::nEXODetectorConstruction()
{
  fDetectorMessenger = new nEXODetectorMessenger(this);
  fWorldConstructor = new nEXOWorldConstructor("/nEXO",this);
  
  fStepLimit = NULL;

  fCheckOverlaps = false;
  
}

nEXODetectorConstruction::~nEXODetectorConstruction()
{
  delete fStepLimit;
  delete fDetectorMessenger;
}

G4VPhysicalVolume* nEXODetectorConstruction::Construct()
{
  /*
  if (fPhysiWorld) {
     G4cout << " in nEXODetectorConstruction::Construct() " << G4endl;
     G4GeometryManager::GetInstance()->OpenGeometry();
     G4PhysicalVolumeStore::GetInstance()->Clean();
     G4LogicalVolumeStore::GetInstance()->Clean();
     G4SolidStore::GetInstance()->Clean();
  }
  */

  nEXOMaterials::GetInstance();
  return ConstructDetector();
}

G4VPhysicalVolume* nEXODetectorConstruction::ConstructDetector()
{
  G4String name = fWorldConstructor->GetName();
  fWorldConstructor->SetCheckOverlaps(fCheckOverlaps);
  G4LogicalVolume* logicMine = fWorldConstructor->GetPiece();
  
  G4VPhysicalVolume* physMine = new G4PVPlacement(0,                     // rotation
                                                  G4ThreeVector(0,0,0),
                                                  logicMine,             // logical volume
                                                  name,                  // name
                                                  0,                     // mother volume
                                                  false,                 // no boolean operations
                                                  0,fCheckOverlaps);                    // not a copy

  nEXOSimplePhysVolManager::GetInstance()->AddPhysicalVolume(name,physMine);  
  //nEXOSimplePhysVolManager::GetInstance()->PrintAllVolumes();
  
  //G4RunManager::GetRunManager()->DefineWorldVolume(physWorld);

  //nEXOAnalysis::GetInstance()->WriteGeometry(physMine);
  nEXOAnalysis::GetInstance()->SetGeometry(physMine);
  
  // Always return the physical World
  return physMine; 
}

void nEXODetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructDetector());
}

void nEXODetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}

void nEXODetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
  fCheckOverlaps = checkOverlaps;
}

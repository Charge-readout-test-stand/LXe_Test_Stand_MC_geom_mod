//////////////////////////////////////////
// nEXOSimplePhysVolManager.hh //
// Manager of added physical volumes //
// This class is designed as a singleton//
//////////////////////////////////////////

#ifndef nEXOSimplePhysVolManager_h
#define nEXOSimplePhysVolManager_h 1

#include <iostream>
#include <map>
#include <set>
#include <iomanip>

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4VSolid.hh"

class nEXOTPCVesselConstructor;

class nEXOSimplePhysVolManager
{
public:
  static nEXOSimplePhysVolManager* GetInstance(){
    if(nEXOSimplePhysVolManager::fInstance == NULL) nEXOSimplePhysVolManager::fInstance = new nEXOSimplePhysVolManager();
    return nEXOSimplePhysVolManager::fInstance;
  }
  
  virtual ~nEXOSimplePhysVolManager();

  /// Get physical volume
  G4VPhysicalVolume* GetPhysicalVolume(G4String name, G4int index = 0);
  /// Add internal physical volume
  void AddPhysicalVolume(G4String name, G4VPhysicalVolume* volume, G4int index = 0);
  /// Print current volumes in manager
  void PrintAllVolumes();
  /// Print names of the logic volumes associated to the physical volumes in manager
  void PrintLogicVolumeNames();
  /// Print info about volumes in current geometry
  void PrintVolumes(G4int level);
  /// Print translation of given volume in current geometry
  void PrintLogicVolumeTranslation(G4String logVolName);

  /// Set TPC vessel object
  void SetTPCVesselConstructor(nEXOTPCVesselConstructor* tpcVessel){fTPCVessel = tpcVessel;}
  /// Get TPC vessel object
  nEXOTPCVesselConstructor* GetTPCVesselConstructor(){return fTPCVessel;}
  
  /// Get translation of given volume in current geometry
  G4ThreeVector GetVolumeTranslation(G4String logVolName);
  
private:
  nEXOSimplePhysVolManager();
  static nEXOSimplePhysVolManager* fInstance;

  /// Physical volumes contained in a piece of this module
  std::map<G4String, G4VPhysicalVolume*> physVolumes;

  /// TPC vessel used in nEXO geometry
  nEXOTPCVesselConstructor* fTPCVessel;
};

#endif

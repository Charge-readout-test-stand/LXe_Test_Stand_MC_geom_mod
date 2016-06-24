#ifndef nEXOMaterials_h
#define nEXOMaterials_h 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "nEXONestMaterialTable.hh"

class G4MaterialPropertiesTable;

class nEXOMaterials
{
  public:

    virtual ~nEXOMaterials();

    static nEXOMaterials* GetInstance();

    G4Material* GetMaterial(G4String);
 
    inline G4MaterialPropertiesTable *LXeTable() { return fLXeMat; };

  private:
 
    nEXOMaterials();
 
    void CreateMaterials();
    
    void AddMatProperties();

  private:

    static nEXOMaterials* fInstance;

    G4NistManager* fNistMan;
    
    //G4MaterialPropertiesTable* fLXeMat;
    nEXONestMaterialTable* fLXeMat;
    G4MaterialPropertiesTable* fSiO2Mat;
    G4MaterialPropertiesTable* fTPBMat;

    G4double* fPhotonWavelengths;
    G4double* fPhotonEnergies;
    G4double* fPhotonULEnergies;
    G4int num_pp;

    G4double fLXeAbsLength;
};

#endif /*nEXOMaterials_h*/

/* 
 * File:   nEXONestMaterial.hh
 * Author: brodsky3
 *
 * Created on August 17, 2015, 11:44 AM
 */

#ifndef NEXONESTMATERIAL_HH
#define	NEXONESTMATERIAL_HH

#include "G4MaterialPropertiesTable.hh"
#include "G4SystemOfUnits.hh"

struct NESTInteractionSite
{
    G4double pos_x;
    G4double pos_y;
    G4double pos_z;
    
    G4int n_excitons;
    G4int n_ions;
    G4int n_photons;
    G4int n_electrons;
    
    G4double site_size;
    G4double energy;
    G4double time_initial;
    G4double time_final;
    
    G4double local_Efield;
    
    NESTInteractionSite():pos_x(999*km),pos_y(999*km),pos_z(999*km),n_excitons(0),n_ions(0),n_photons(0),n_electrons(0),
    site_size(0),energy(0),time_initial(DBL_MAX),time_final(-1*ns),local_Efield(0){}
    
};

class nEXONestMaterialTable : public G4MaterialPropertiesTable{
public:
    nEXONestMaterialTable();
    virtual ~nEXONestMaterialTable();
   
    std::vector<NESTInteractionSite> sites;
private:

};

#endif	/* NEXONESTMATERIAL_HH */


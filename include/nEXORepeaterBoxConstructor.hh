

#ifndef nEXORepeaterBoxConstructor_h
#define nEXORepeaterBoxConstructor_h 1

#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Box.hh"

#include "nEXODetectorConstruction.hh"
#include "nEXOComponentConstructor.hh"
#include "nEXOConstructor.hh"

class nEXORepeaterBoxConstructor : public nEXOComponentConstructor
{
public:
  nEXORepeaterBoxConstructor(G4String name, nEXODetectorConstruction* construct) : nEXOComponentConstructor(name, construct) {Init();};
  nEXORepeaterBoxConstructor(G4String name, nEXOConstructor* construct) : nEXOComponentConstructor(name, construct) {Init();};

  virtual ~nEXORepeaterBoxConstructor(){;};

  virtual G4LogicalVolume* GetPiece(void);
  virtual G4LogicalVolume* GetFillingPiece(void);
private:
  void Init(void);
};

class nEXORepeaterBoxMessenger : public nEXOComponentConstructorMessenger
{
private:
  nEXORepeaterBoxConstructor *fConstructor;

public:
  nEXORepeaterBoxMessenger(nEXORepeaterBoxConstructor* construct) :nEXOComponentConstructorMessenger(construct, "Control the geometry of the repeater box"), fConstructor(construct){};

  ~nEXORepeaterBoxMessenger(){};
  void SetNewValue(G4UIcommand* cmd, G4String val){nEXOConstructorMessenger::SetNewValue(cmd,val);};
};
#endif 


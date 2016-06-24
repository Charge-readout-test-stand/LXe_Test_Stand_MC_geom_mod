#include "nEXOConstructor.hh"

nEXOConstructor::nEXOConstructor(G4String name, nEXODetectorConstruction* construct)
  : fLocalName(name), fName(name), fConstruction(construct), fParent(NULL), fMessenger(NULL), fVisible(false)
{
  fMessenger = fConstruction->GetMessenger();
}

nEXOConstructor::nEXOConstructor(G4String name, nEXOConstructor* parent)
  : fLocalName(name), fName(name), fConstruction(NULL), fParent(parent), fMessenger(NULL), fVisible(false)
{
  fName = fParent->GetName() + "/" + fLocalName;
  fConstruction = fParent->GetConstruction();
  fMessenger = fParent->GetMessenger();
}

nEXOConstructor::~nEXOConstructor(){;}

G4LogicalVolume* nEXOConstructor::GetPiece(void)
{
  G4Exception("nEXOConstructor::GetPiece()","Not implemented",RunMustBeAborted,"");
  return NULL;
}

G4Material* nEXOConstructor::FindMaterial(G4String name)
{
  //G4Material* material = G4Material::GetMaterial(name,true);
  nEXOMaterials* materials = nEXOMaterials::GetInstance();
  G4Material* material = materials->GetMaterial(name);
  return material;
}

G4String nEXOConstructor::GetName(void)
{
  return fName;
}

G4String nEXOConstructor::GetLocalName(void)
{
  return fLocalName;
}
  
/// Set the local name of the object to be built.
/// The local name of the logical volume being set, and also sets the full name of the volume.
void nEXOConstructor::SetLocalName(const G4String& name)
{
  fLocalName = name;
  fName = name;
  if(fParent) fName = fParent->GetName() + "/" + fName;
}

/// Set the visibility of the constructed object.
void nEXOConstructor::SetVisible(bool vis)
{
  if(fVisible != vis)
    G4cout << "Set visibility for " << GetName() << " from " << fVisible << " to " << vis << G4endl;
  fVisible = vis;
}

// Set the visibility of the constructed object daughters.
void nEXOConstructor::SetVisibleDaughters(bool vis)
{
  for(std::map<G4String,nEXOConstructor*>::iterator sub = fSubConstructors.begin(); sub != fSubConstructors.end(); ++sub)
    sub->second->SetVisible(vis);
}

nEXOConstructorMessenger::nEXOConstructorMessenger(nEXOConstructor *construct, const char* guide)
{
  fConstructor = construct;
  fDirName = construct->GetName() + "/";
  fDirectory = new G4UIdirectory(fDirName);
  if(guide)
    fDirectory->SetGuidance(guide);
  else 
    fDirectory->SetGuidance("Commands for a nEXO geometry.");

}

void nEXOConstructorMessenger::SetNewValue(G4UIcommand* cmd, G4String val)
{
}

nEXOConstructorMessenger::~nEXOConstructorMessenger()
{
  delete fDirectory;
}

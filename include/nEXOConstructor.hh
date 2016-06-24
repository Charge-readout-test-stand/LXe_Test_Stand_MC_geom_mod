//////////////////////////////////////////
// nEXOConstructor.hh //
// Base class to construct nEXO geometry  //
//////////////////////////////////////////

#ifndef nEXOConstructor_h
#define nEXOConstructor_h 1

#include <algorithm>

#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4UImessenger.hh>
#include <G4UIdirectory.hh>
#include <G4ExceptionSeverity.hh>
#include "G4VPhysicalVolume.hh"

#include "nEXODetectorConstruction.hh"
#include "nEXODetectorMessenger.hh"
#include "nEXOMaterials.hh"

/// Base for classes that construct elements of the detector.
/// The method GetPiece() should construct a new unplaced element that is facing up along the Z axis.
/// The calling code should then rotate the object into position and place the object into a large mother volume.

class nEXOConstructor
{
private:
  G4String fLocalName;
  G4String fName;

  /// The G4VUserDetectorConstruction class that this is cooperating with.
  nEXODetectorConstruction *fConstruction;
  
  /// The parent of this constructor.
  nEXOConstructor *fParent;

  /// The user interface messenger that will provide values for this class.
  G4UImessenger *fMessenger;

  /// If this is true, then draw constructed object.
  G4bool fVisible;

  /// The sub constructors that might be used in this class.
  std::map<G4String,nEXOConstructor*> fSubConstructors;

protected:
  /// Check overlaps during placement of volumes
  G4bool  fCheckOverlaps;

  G4Material* FindMaterial(G4String material);

public:
  nEXOConstructor(G4String name, nEXODetectorConstruction *construct);
  nEXOConstructor(G4String name, nEXOConstructor *parent);
  virtual ~nEXOConstructor();
  
  /// Construct and return a G4 volume for the object.
  /// This is a pure virtual function, which means it must be implemented by the inheriting classes.
  virtual G4LogicalVolume *GetPiece(void) = 0;
  
  /// Return the base name of the object that this builds.
  G4String GetName(void);

  /// Return the name of the object that this builds.
  G4String GetLocalName(void);

  /// Set the base name of the logical volume that this builds.
  void SetLocalName(const G4String& name);

  /// Set to turn on/off the check overlaps during placement of volumes
  void SetCheckOverlaps(G4bool checkOverlaps){fCheckOverlaps = checkOverlaps;};

  /// Set the visibility of the constructed object.
  void SetVisible(bool vis);

  /// Set the visibility of the object daughters.
  void SetVisibleDaughters(bool vis);

  /// Get the visibility of the constructed object.
  bool GetVisible(void) const {return fVisible;};

  /// Return the detector construction that is building this piece.
  nEXODetectorConstruction *GetConstruction(void){
    return fConstruction;
  };

  /// Return the messenger for this constructor.
  G4UImessenger* GetMessenger(void) {return fMessenger;};

  // Set the messenger for this constructor.
  void SetMessenger(G4UImessenger* messenger){
    fMessenger = messenger;
  };

  /// Add a new sub constructor to teh current geometry constructor.
  /// This should be done in the derived class constructor.
  /// All geometry constructors that will be used by the derived class should be added using this method.
  void AddConstructor(nEXOConstructor* constructor){
    if(fSubConstructors.find(constructor->GetLocalName()) != fSubConstructors.end())
    {
      G4cout << "Multiply defined constructor name "
             << constructor->GetName()
             << G4endl;
      G4Exception("nEXOConstructor::AddConstructor()","Multiple defines",RunMustBeAborted,"");
    }
    fSubConstructors[constructor->GetLocalName()] = constructor;
  };

  /// Get a sub constructor by name and do the dynamic cast.
  /// This will abort with an error message if you request an undefined name.  
  template <class T> T& Get(G4String name){
    std::map<G4String,nEXOConstructor*>::iterator sub = fSubConstructors.find(name);
    if(sub == fSubConstructors.end())
    {
      G4cout << "Error in " << GetName() << G4endl;
      G4cout << "   Undefined constructor requested " << name << G4endl;
      G4cout << "   The choices are: " << G4endl;
      for(sub = fSubConstructors.begin(); sub != fSubConstructors.end(); ++sub)
      {
        G4cout << "       \"" << sub->first << "\""
               << " for constructor: " << sub->second->GetName()
               << G4endl;
      }
      G4Exception("nEXOConstructor::Get<>","Undefined constructor",RunMustBeAborted,"");
    }
    T* constructor = dynamic_cast<T*>(sub->second);
    if(!constructor)
    {
      G4cout << "Error in " << GetName() << G4endl;
      G4cout << "   Cannot type cast " << name << " to the requested class" << G4endl;
      G4Exception("nEXOConstructor::Get<>","Bad typecast",RunMustBeAborted,"");
    }
    return *constructor;
  }

  /// Find a sub constructor by name and do the dynamic cast.
  /// This returns a pointer that will be NULL if you request an undefined name.
  template <class T> T* Find(G4String name){
    std::map<G4String,nEXOConstructor*>::iterator sub = fSubConstructors.find(name);
    if(sub==fSubConstructors.end()) return NULL;
    T* constructor = dynamic_cast<T*>(sub->second);
    return constructor;
  }

};

class nEXOConstructorMessenger: public G4UImessenger
{
private:
  nEXOConstructor* fConstructor;

  /// The UI directory for this messenger.
  G4UIdirectory* fDirectory;
  
  /// The directory name for this messenger.
  G4String fDirName;

public:
  nEXOConstructorMessenger(nEXOConstructor* constructor, const char* guide=NULL);
  virtual ~nEXOConstructorMessenger();

  /// Return the name of the directory.
  G4String GetDirectory(void) {return fDirName;};

  /// Build a command with the directory prefix.
  G4String CommandName(G4String cmd){
    G4String name = GetDirectory() + cmd;
    //G4cout << "Creating command name: " << name << G4endl;
    return name;
  };

  void SetNewValue(G4UIcommand *cmd, G4String val);
};
#endif

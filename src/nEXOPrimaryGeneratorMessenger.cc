#include "nEXOPrimaryGeneratorMessenger.hh"

#include "nEXOPrimaryGeneratorAction.hh"

nEXOPrimaryGeneratorMessenger::
  nEXOPrimaryGeneratorMessenger(nEXOPrimaryGeneratorAction* nEXOGun) :
    fnEXOAction(nEXOGun)
{
  fDirectory = new G4UIdirectory("/generator/");
  fDirectory->SetGuidance(" Generator commands ");

  fGenCmd = new G4UIcmdWithAString("/generator/setGenerator",this);
  fGenCmd->SetGuidance("Choose the primary event generator");
  fGenCmd->
    SetGuidance(" Choices: gps, GeneralParticleSource(default), gun, bb0n, bb2n, nCaptureXe136, nCaptureCu");
  fGenCmd->SetParameterName("choice", true);
  fGenCmd->SetDefaultValue("GeneralParticleSource");
  fGenCmd->SetCandidates("gps GeneralParticleSource gun bb0n bb2n nCaptureXe136 nCaptureCu");
  fGenCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fnCaptureXeSimCmd =
            new G4UIcmdWithAString("/generator/setnCaptureXeSimMethod", this);
  fnCaptureXeSimCmd->
    SetGuidance("Enter the LXe component to confine n-capture decays (default = RandomGammas)");
  fnCaptureXeSimCmd->
   SetGuidance("Choices: InternalConversions, RandomGammas, ImbalancedCascade");
  fnCaptureXeSimCmd->SetParameterName("choice", true);
  fnCaptureXeSimCmd->SetDefaultValue("RandomGammas");
  fnCaptureXeSimCmd->
          SetCandidates("InternalConversions RandomGammas ImbalancedCascade");
  fnCaptureXeSimCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fXeComponentCmd = new G4UIcmdWithAString("/generator/setXeComponent",this);
  fXeComponentCmd->
    SetGuidance("Enter the LXe component to confine n-capture decays (default = ActiveLXe)");
  fXeComponentCmd->SetGuidance("Choices: ActiveLXe, InactiveLXe");
  fXeComponentCmd->SetParameterName("choice", true);
  fXeComponentCmd->SetDefaultValue("ActiveLXe");
  fXeComponentCmd->SetCandidates("ActiveLXe InactiveLXe");
  fXeComponentCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fCuIsoCmd = new G4UIcmdWithAnInteger("/generator/setCuIsotope",this);
  fCuIsoCmd->
    SetGuidance("Enter the Cu isotope (63 or 65) for n-Capture (default = 63)");
  fCuIsoCmd->SetParameterName("isotope", false, false);
  fCuIsoCmd->SetDefaultValue(63);
  fCuIsoCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fCuComponentCmd = new G4UIcmdWithAString("/generator/setCuComponent",this);
  fCuComponentCmd->
     SetGuidance("Enter the copper component to confine n-capture decays (default = TPC)");
  fCuComponentCmd->
     SetGuidance("Choices: TPC, APDPlatter, FieldRing, InnerCryo, OuterCryo");
  fCuComponentCmd->SetParameterName("choice", true);
  fCuComponentCmd->SetDefaultValue("TPC");
  fCuComponentCmd->
                SetCandidates("TPC APDPlatter FieldRing InnerCryo OuterCryo");
  fCuComponentCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fBb2nCutOffCmd = new G4UIcmdWithADouble("/generator/setBb2nCutOff",this);
  fBb2nCutOffCmd->SetGuidance("Enter the fraction (wrt the Q-value) of the minimum value for the sum of the electron energies (default = 0.)");
  fBb2nCutOffCmd->SetDefaultValue(0.);
  fBb2nCutOffCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

nEXOPrimaryGeneratorMessenger::~nEXOPrimaryGeneratorMessenger()
{
  delete fDirectory;

  delete fGenCmd;
  delete fnCaptureXeSimCmd;
  delete fXeComponentCmd;
  delete fCuIsoCmd;
  delete fCuComponentCmd;
  delete fBb2nCutOffCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEXOPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,
                                                G4String newValue)
{
  if ( command == fGenCmd ) 
     fnEXOAction->Set_generator(newValue);

  if ( command == fnCaptureXeSimCmd )
     fnEXOAction->Set_nCaptureXe_Method(newValue);

  if ( command == fXeComponentCmd )
     fnEXOAction->Set_Xe_Component(newValue);

  if ( command == fCuIsoCmd )
     fnEXOAction->Set_Cu_isotope(fCuIsoCmd->GetNewIntValue(newValue));

  if ( command == fCuComponentCmd)
     fnEXOAction->Set_Cu_Component(newValue);

  if ( command == fBb2nCutOffCmd)
    fnEXOAction->Set_bb2n_CutOff(fBb2nCutOffCmd->GetNewDoubleValue(newValue));
  
}

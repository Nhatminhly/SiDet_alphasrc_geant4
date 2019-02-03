#include "PhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"

PhysicsList::PhysicsList():  G4VUserPhysicsList()
{
  defaultCutValue = 0.01*mm;
  SetVerboseLevel(1);
}

PhysicsList::~PhysicsList()
{;}

void PhysicsList::ConstructParticle() //Xay dung hat ban
{
  // gamma
  G4Gamma::GammaDefinition();
  //alpha
  G4Alpha::AlphaDefinition();
  G4GenericIon::GenericIon();
  // e+/-
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
}


void PhysicsList::ConstructProcess() //Khai bao ve tuong tac (quang dien, compton,...)
{
  AddTransportation();
  ConstructEM();
}

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4RayleighScattering.hh"
#include "G4alphaIonisation.hh"
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4eplusAnnihilation.hh"

#include "G4LossTableManager.hh"

#include "G4UAtomicDeexcitation.hh"
#include "G4hMultipleScattering.hh"
#include "G4ionIonisation.hh"
#include "G4IonParametrisedLossModel.hh"

void PhysicsList::ConstructEM() //Hat gamma, electron, alpha
{
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    
    if (particleName == "gamma") {
      // gamma         
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
      pmanager->AddDiscreteProcess(new G4ComptonScattering);
      pmanager->AddDiscreteProcess(new G4GammaConversion);
      pmanager->AddDiscreteProcess(new G4RayleighScattering);
      
    } else if (particleName == "alpha") {
		pmanager->AddProcess(new G4hMultipleScattering,-1,1,1);
	
	//ionisation
	G4ionIonisation* ionIoni = new G4ionIonisation();
	ionIoni->SetStepFunction(0.1, 20*um);
	pmanager->AddProcess(ionIoni,                   -1, 2, 2);
	} else if (particleName == "GenericIon")
      {
	// OBJECT may be dynamically created as either a GenericIon or nucleus
	// G4Nucleus exists and therefore has particle type nucleus
	// genericIon:
	
	//multiple scattering
	pmanager->AddProcess(new G4hMultipleScattering,-1,1,1);

	//ionisation
	G4ionIonisation* ionIoni = new G4ionIonisation();
	ionIoni->SetEmModel(new G4IonParametrisedLossModel());
	ionIoni->SetStepFunction(0.1, 20*um);
	pmanager->AddProcess(ionIoni,                   -1, 2, 2);

     } else if (particleName == "e-") {
     //electron
     pmanager->AddProcess(new G4eMultipleScattering,-1, 1,1);
     pmanager->AddProcess(new G4eIonisation,       -1, 2,2);
     pmanager->AddProcess(new G4eBremsstrahlung,   -1, 3,3);      

     } else if (particleName == "e+") {
     //positron
     pmanager->AddProcess(new G4eMultipleScattering,-1, 1,1);
     pmanager->AddProcess(new G4eIonisation,       -1, 2,2);
     pmanager->AddProcess(new G4eBremsstrahlung,   -1, 3,3);
     pmanager->AddProcess(new G4eplusAnnihilation,  0,-1,4);

    } 
  }
    ///// Fluoresince (Deexcitation Xray)
	G4UAtomicDeexcitation* de= new G4UAtomicDeexcitation();
	de->SetFluo(true);
	de->SetAuger(true);
	de->SetPIXE(true);
	G4LossTableManager::Instance()->SetAtomDeexcitation(de);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
  //G4VUserPhysicsList::SetCutsWithDefault method sets 
  //the default cut value for all particle types 
  //
  SetCutsWithDefault();
     
  if (verboseLevel>0) DumpCutValuesTable();
}

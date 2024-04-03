#include "physics.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"

#include "G4ComptonScattering.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"	
#include "G4eBremsstrahlung.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

ComptCameraPhysicsList::ComptCameraPhysicsList()
{
	RegisterPhysics(new G4DecayPhysics());
	RegisterPhysics(new G4EmStandardPhysics_option4());
}

void ComptCameraPhysicsList::ConstructProcess()
{

	AddTransportation();
	G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
	G4PhotoElectricEffect* thePhotoElectricEffect = new G4PhotoElectricEffect();
	// I should bias both same way to keep the ratio between them constant
	//thePhotoElectricEffect->SetCrossSectionBiasingFactor(10);
	//theComptonScattering->SetCrossSectionBiasingFactor(10);
	

    // Get the process manager for the gamma particle
    G4ParticleDefinition* gamma = G4Gamma::GammaDefinition();
    G4ProcessManager* pmanager = gamma->GetProcessManager();

    // Add the processes to the process manager
    pmanager->AddDiscreteProcess(theComptonScattering);
    pmanager->AddDiscreteProcess(thePhotoElectricEffect);

	// Get the process manager for the electron particle
	G4ParticleDefinition* electron = G4Electron::ElectronDefinition();
	pmanager = electron->GetProcessManager();

	// Add the processes to the process manager
	G4eMultipleScattering* msc = new G4eMultipleScattering();
	G4eIonisation* ionisation = new G4eIonisation();
	G4eBremsstrahlung* bremsstrahlung = new G4eBremsstrahlung();

	pmanager->AddDiscreteProcess(msc);
	pmanager->AddDiscreteProcess(ionisation);
	pmanager->AddDiscreteProcess(bremsstrahlung);
}

ComptCameraPhysicsList::~ComptCameraPhysicsList()
{
}

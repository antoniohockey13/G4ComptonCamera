#include "physics.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4ComptonScattering.hh"
#include "G4PhotoElectricEffect.hh"
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
	theComptonScattering->SetCrossSectionBiasingFactor(100);
	thePhotoElectricEffect->SetCrossSectionBiasingFactor(100);

    // Get the process manager for the gamma particle
    G4ParticleDefinition* gamma = G4Gamma::GammaDefinition();
    G4ProcessManager* pmanager = gamma->GetProcessManager();

    // Add the processes to the process manager
    pmanager->AddDiscreteProcess(theComptonScattering);
    pmanager->AddDiscreteProcess(thePhotoElectricEffect);
}
ComptCameraPhysicsList::~ComptCameraPhysicsList()
{
}

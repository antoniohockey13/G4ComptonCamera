#include "physics.hh"
#include "G4EmStandardPhysics_option4.hh"

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
	//RegisterPhysics(new G4EmLivermorePhysics());
	//RegisterPhysics(new G4EmPenelopePhysics());	
	RegisterPhysics(new G4EmStandardPhysics_option4());	
}

ComptCameraPhysicsList::~ComptCameraPhysicsList()
{
}

#include "physics.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4GenericBiasingPhysics.hh"

ComptCameraPhysicsList::ComptCameraPhysicsList()
{
	RegisterPhysics(new G4DecayPhysics());
	//RegisterPhysics(new G4EmLivermorePhysics());
	//RegisterPhysics(new G4EmPenelopePhysics());	
	RegisterPhysics(new G4EmStandardPhysics_option4());

	auto* biasingPhysics = new G4GenericBiasingPhysics();
	// Bias gamma only, to speed up the simulation. Biasing is needed to have enough statistics in the detector, 
	// as the Compton cross section is small.
	biasingPhysics->PhysicsBias("gamma");
	RegisterPhysics(biasingPhysics);
}

ComptCameraPhysicsList::~ComptCameraPhysicsList()
{
}

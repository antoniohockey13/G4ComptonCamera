#include "physics.hh"

ComptCameraPhysicsList::ComptCameraPhysicsList()
{
	RegisterPhysics(new G4DecayPhysics());
	RegisterPhysics(new G4EmLivermorePhysics());
	RegisterPhysics(new G4EmStandardPhysics());
	RegisterPhysics(new G4EmPenelopePhysics());	
}
ComptCameraPhysicsList::~ComptCameraPhysicsList()
{
}

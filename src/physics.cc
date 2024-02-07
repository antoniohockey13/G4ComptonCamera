#include "physics.hh"

ComptCameraPhysicsList::ComptCameraPhysicsList()
{
	RegisterPhysics(new G4DecayPhysics());
	RegisterPhysics(new G4EmLivermorePhysics());
}
ComptCameraPhysicsList::~ComptCameraPhysicsList()
{
}

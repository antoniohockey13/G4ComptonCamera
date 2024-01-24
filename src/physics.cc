#include "physics.hh"

ComptCameraPhysicsList::ComptCameraPhysicsList()
{
	RegisterPhysics(new G4EmStandardPhysics_option4());
}
ComptCameraPhysicsList::~ComptCameraPhysicsList()
{
}

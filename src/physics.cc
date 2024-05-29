#include "physics.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"

ComptCameraPhysicsList::ComptCameraPhysicsList()
{
	RegisterPhysics(new G4EmStandardPhysics_option4());
}
ComptCameraPhysicsList::~ComptCameraPhysicsList()
{
}

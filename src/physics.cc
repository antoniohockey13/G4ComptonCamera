#include "physics.hh"
#include "G4EmStandardPhysics_option4.hh"

ComptCameraPhysicsList::ComptCameraPhysicsList()
{
	//RegisterPhysics(new G4DecayPhysics());
	//RegisterPhysics(new G4EmLivermorePhysics());
	//RegisterPhysics(new G4EmStandardPhysics());
	//RegisterPhysics(new G4EmPenelopePhysics());	
	RegisterPhysics(new G4EmStandardPhysics_option4());	
}
ComptCameraPhysicsList::~ComptCameraPhysicsList()
{
}

#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "G4VModularPhysicsList.hh"
// EM physics standard option4 selected cause precise in x-ray region
#include "G4EmStandardPhysics_option4.hh" 
class ComptCameraPhysicsList : public G4VModularPhysicsList 
{
public:
	ComptCameraPhysicsList();
	~ComptCameraPhysicsList();
};

#endif

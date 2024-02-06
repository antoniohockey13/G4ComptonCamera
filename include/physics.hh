#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "G4VModularPhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmStandardPhysics.hh"


class ComptCameraPhysicsList : public G4VModularPhysicsList 
{
public:
	ComptCameraPhysicsList();
	~ComptCameraPhysicsList();
};

#endif

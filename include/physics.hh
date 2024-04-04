#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "G4VUserPhysicsList.hh"

class ComptCameraPhysicsList : public G4VUserPhysicsList 
{
public:
	explicit ComptCameraPhysicsList();
	~ComptCameraPhysicsList() override;
protected:
	void ConstructParticle() override;
	void ConstructProcess() override;

};

#endif

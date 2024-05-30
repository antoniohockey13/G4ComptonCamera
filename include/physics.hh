#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "G4VUserPhysicsList.hh"

class G4VMscModel;

class ComptCameraPhysicsList : public G4VUserPhysicsList
{
public:
	explicit ComptCameraPhysicsList();
	~ComptCameraPhysicsList() override;

protected:
	void ConstructParticle() override;
	void ConstructBosons();
	void ConstructLeptons();
	void ConstructProcess() override;
	void ConstructEM();
	void ConstructGeneral();

	
};

#endif
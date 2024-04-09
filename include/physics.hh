#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "G4VUserPhysicsList.hh"
#include "G4VEmFluctuationModel.hh"

class G4VMscModel;

class ComptCameraPhysicsList : public G4VUserPhysicsList
{
public:
	explicit ComptCameraPhysicsList();
	~ComptCameraPhysicsList() override;
	static G4VEmFluctuationModel* ModelOfFluctuations(G4bool isIon = false);

protected:
	void ConstructParticle() override;
	void ConstructBosons();
	void ConstructLeptons();
	void ConstructProcess() override;
	void ConstructEM();
	void ConstructGeneral();
	void SetCuts() override;
	void ConstructElectronMscProcess(G4VMscModel* msc1, G4VMscModel* msc2, 
									G4ParticleDefinition* particle);
	
};

#endif

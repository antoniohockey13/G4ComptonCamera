#include "physics.hh"

ComptCameraPhysicsList::ComptCameraPhysicsList()
{
	// EM physics standard option4 selected cause precise in x-ray region
	RegisterPhysics(new G4EmStandardPhysics_option4());
	/*
	RegisterPhysics(new G4EmPenelopePhysics());
	RegisterPhysics(new G4EmExtraPhysics());
	RegisterPhysics(new G4HadronElasticPhysicsHP());
	RegisterPhysics(new G4HadronPhysicsFTFP_BERT_HP());
	RegisterPhysics(new G4IonPhysics());
	*/
}
ComptCameraPhysicsList::~ComptCameraPhysicsList()
{
}

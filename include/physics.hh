#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "G4VModularPhysicsList.hh"
// EM physics standard option4 selected cause precise in x-ray region
#include "G4EmStandardPhysics_option4.hh" 
/* 
#include "G4EmPenelopePhysics.hh"
#include "G4EmExtraPhysics.hh"
*/
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
// #include "G4IonPhysics.hh"

class ComptCameraPhysicsList : public G4VModularPhysicsList 
{
public:
	ComptCameraPhysicsList();
	~ComptCameraPhysicsList();
};

#endif

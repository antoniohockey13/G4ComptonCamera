#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics_option4.hh" // EM physics standard option4 selected cause precise in x-ray region

class MyPhysicsList : public G4VModularPhysicsList 
{
public:
	MyPhysicsList();
	~MyPhysicsList();
};

#endif

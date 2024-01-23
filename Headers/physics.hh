#ifndef PHYSICS_HH
#define PHYSDICS_HH

#include "G4EmStandardPhysics_option4.hh" // EM physics standard option4 selected cause precise in x-ray region

class MyPhysicsList : public G4VModularPhysicsList 
{
public:
	MyPhysicsList();
	~MyPhysicsList();
};

#endif

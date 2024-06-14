#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "G4VModularPhysicsList.hh"

class ComptCameraPhysicsList : public G4VModularPhysicsList
{
    public:
        explicit ComptCameraPhysicsList();
        ~ComptCameraPhysicsList() override;
};

#endif

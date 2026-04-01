#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "G4VModularPhysicsList.hh"
#include "G4VPhysicsConstructor.hh"

class ComptCameraEMPhysics : public G4VPhysicsConstructor
{
public:
    explicit ComptCameraEMPhysics(const G4String& name = "ComptCameraEM");
    virtual ~ComptCameraEMPhysics();

    virtual void ConstructParticle() override;
    virtual void ConstructProcess() override;
};

class ComptCameraPhysicsList : public G4VModularPhysicsList
{
public:
    ComptCameraPhysicsList();
    virtual ~ComptCameraPhysicsList();
};

#endif
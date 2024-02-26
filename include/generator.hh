#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"


class ComptCameraPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
	ComptCameraPrimaryGenerator();
	~ComptCameraPrimaryGenerator();

	virtual void GeneratePrimaries(G4Event *);

private:
	G4ParticleGun *_particle_gun;
	G4double _world_width;
};

#endif

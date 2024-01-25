#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
// To import world_width from detector construction to redefine source position
#include "construction.hh"

class ComptCameraPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
	ComptCameraPrimaryGenerator();
	~ComptCameraPrimaryGenerator();

	virtual void GeneratePrimaries(G4Event *);

private:
	G4ParticleGun *fParticleGun;
	G4double _world_width;
};

#endif

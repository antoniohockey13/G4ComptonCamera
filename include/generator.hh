#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4GeneralParticleSource;
class G4Event;

class ComptCameraPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
	public:
		ComptCameraPrimaryGenerator();
		~ComptCameraPrimaryGenerator();

		virtual void GeneratePrimaries(G4Event *) override;

	private:
		G4GeneralParticleSource *_general_particle_source;
		G4double _world_height;
};

#endif

#include "generator.hh"
#include "construction.hh"

#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"

ComptCameraPrimaryGenerator::ComptCameraPrimaryGenerator()
{
    // Get world width from detector construction
    ComptCameraDetectorConstruction detectorConstruction;
    _world_width = detectorConstruction.GetWorldWidth();
    // Define particle gun
    // Number of particles per event
    _particle_gun = new G4ParticleGun(1); 
    // Position and momentum
    G4ThreeVector pos = G4ThreeVector(-_world_width/2, 0.0, 0.0);
    G4ThreeVector mom = G4ThreeVector(1.0, 0.0, 0.0);
    _particle_gun->SetParticlePosition(pos);
    _particle_gun->SetParticleMomentumDirection(mom);
    
    // Particle type 
    _particle_gun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("gamma"));
    // Particle energy
    _particle_gun->SetParticleEnergy(70* keV);
}

ComptCameraPrimaryGenerator::~ComptCameraPrimaryGenerator()
{
    delete _particle_gun;
}

void ComptCameraPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    _particle_gun->GeneratePrimaryVertex(anEvent);
}

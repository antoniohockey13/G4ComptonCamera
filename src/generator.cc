#include "generator.hh"

ComptCameraPrimaryGenerator::ComptCameraPrimaryGenerator()
{
    // Get world width from detector construction
    ComptCameraDetectorConstruction detectorConstruction;
    _world_width = detectorConstruction.GetWorldWidth();
    // Define particle gun
    // Number of particles per event
    fParticleGun = new G4ParticleGun(1); 
    // Position and momentum
    G4ThreeVector pos = G4ThreeVector(-_world_width/2, 0.0, 0.0);
    G4ThreeVector mom = G4ThreeVector(1.0, 0.0, 0.0);
    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    
    // Particle type 
    fParticleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("gamma"));
    // Particle energy
    fParticleGun->SetParticleEnergy(70* keV);
}

ComptCameraPrimaryGenerator::~ComptCameraPrimaryGenerator()
{
    delete fParticleGun;
}

void ComptCameraPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    fParticleGun->GeneratePrimaryVertex(anEvent);
}

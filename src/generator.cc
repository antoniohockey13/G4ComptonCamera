#include "generator.hh"
#include "construction.hh"

#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh" 
#include "Randomize.hh"


ComptCameraPrimaryGenerator::ComptCameraPrimaryGenerator()
{   
    _particle_gun = new G4ParticleGun(1); 
    // Get world width from detector construction
    ComptCameraDetectorConstruction detectorConstruction;
    _world_width = detectorConstruction.GetWorldWidth();

    G4ThreeVector pos = G4ThreeVector(-_world_width/2, 0.0, 0.0);
    _particle_gun->SetParticlePosition(pos);

    // Particle type 
    _particle_gun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("gamma"));
    // Particle energy
    _particle_gun->SetParticleEnergy(70* keV);
    // Number of particles per event
    

    
    // Define angle
    // alpha angle with y axis
    G4double alpha = 0;
    // phi angle with z axis
    G4double phi = 0.0;    
    /*
    // Define particle gun with an isotropic distribution
    G4double alpha_min = -0.9;
    G4double alpha_max = 0.9;

    G4double phi_min = -0.7;
    G4double phi_max = 0.7;    

    G4double alpha = G4UniformRand()*(alpha_max - alpha_min) + alpha_min;
    G4double phi = G4UniformRand()*(phi_max - phi_min) + phi_min;
    */
    G4ThreeVector mom = G4ThreeVector(cos(phi)*cos(alpha), sin(alpha)*cos(phi), cos(alpha)*sin(phi));
    _particle_gun->SetParticleMomentumDirection(mom);
}

ComptCameraPrimaryGenerator::~ComptCameraPrimaryGenerator()
{
    if( _particle_gun != nullptr ) 
    {
        delete _particle_gun;
        _particle_gun = nullptr;
    }
}

void ComptCameraPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    _particle_gun->GeneratePrimaryVertex(anEvent);
}

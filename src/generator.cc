#include "generator.hh"
#include "construction.hh"

#include "G4SystemOfUnits.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Gamma.hh"
#include "Randomize.hh"

ComptCameraPrimaryGenerator::ComptCameraPrimaryGenerator():
    _general_particle_source(nullptr),
    _world_width(-1)
{   
    // Get world width from detector construction
    ComptCameraDetectorConstruction detectorConstruction;
    _world_width = detectorConstruction.GetWorldWidth();
    // Define particle gun
    _general_particle_source = new G4GeneralParticleSource();
    _general_particle_source->SetParticleDefinition(G4Gamma::Definition());
    auto current_source = _general_particle_source->GetCurrentSource();
    current_source->GetPosDist()->SetCentreCoords(G4ThreeVector(-_world_width/2, 0.0, 0.0));   
    current_source->GetPosDist()->SetPosDisShape("Circle");
    current_source->GetPosDist()->SetPosRot1(G4ThreeVector(0, 1, 0));
    current_source->GetPosDist()->SetPosRot2(G4ThreeVector(0, 0, 1));
    // Simulate 1mm collimator
    current_source->GetPosDist()->SetRadius(1*mm);
    
    // Define angle to launch particles
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
    current_source->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(1, 0, 0));   

    current_source->GetEneDist()->SetEnergyDisType("Arb");
    current_source->GetEneDist()->ArbEnergyHistoFile("source_sim_5108.txt"); 
    // --> Need to use macro, not working yet!!! --> use /gps/hist/inter Lin
    //current_source->GetEneDist()->ArbInterpolate("Lin");
}

ComptCameraPrimaryGenerator::~ComptCameraPrimaryGenerator()
{
    if( _general_particle_source != nullptr ) 
    {
        delete _general_particle_source;
        _general_particle_source = nullptr;
    }
}

void ComptCameraPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    _general_particle_source->GeneratePrimaryVertex(anEvent);
}

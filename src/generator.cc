#include "generator.hh"
#include "construction.hh"

#include "G4SystemOfUnits.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Gamma.hh"

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
    current_source->GetPosDist()->SetCentreCoords(G4ThreeVector(-_world_width/2, 0, 0));
    current_source->GetPosDist()->SetPosDisType("Plane");
    current_source->GetPosDist()->SetPosDisShape("Circle");
    current_source->GetPosDist()->SetPosRot1(G4ThreeVector(0, 1, 0));
    current_source->GetPosDist()->SetPosRot2(G4ThreeVector(0, 0, 1));
    current_source->GetPosDist()->SetRadius(1*mm);

    current_source->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(1, 0, 0));   

    current_source->GetEneDist()->SetEnergyDisType("Arb");
G4cout << "Current source: " << current_source << G4endl;
    current_source->GetEneDist()->ArbEnergyHistoFile("Source_sim_biased_109.txt"); 
G4cout << "Personalised energy distribution" << G4endl;
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

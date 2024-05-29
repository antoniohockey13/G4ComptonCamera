#include "generator.hh"
#include "construction.hh"

#include "G4SystemOfUnits.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Electron.hh"

ComptCameraPrimaryGenerator::ComptCameraPrimaryGenerator():
    _general_particle_source(nullptr),
    _world_height(-1)
{
    // Get world width from detector construction
    ComptCameraDetectorConstruction detectorConstruction;
    _world_height = detectorConstruction.GetWorldHeight();
    // Define particle gun
    _general_particle_source = new G4GeneralParticleSource();
    _general_particle_source->SetParticleDefinition(G4Electron::Definition());
    auto current_source = _general_particle_source->GetCurrentSource();
    current_source->GetPosDist()->SetCentreCoords(G4ThreeVector(0, _world_height/2, 0));
    current_source->GetPosDist()->SetPosDisType("Plane");
    current_source->GetPosDist()->SetPosDisShape("Circle");
    current_source->GetPosDist()->SetPosRot1(G4ThreeVector(0, 0, 1));
    current_source->GetPosDist()->SetPosRot2(G4ThreeVector(1, 0, 0));
    current_source->GetPosDist()->SetRadius(2*mm);

    current_source->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(0, -1, 0));
    current_source->GetEneDist()->SetMonoEnergy(30*keV);



    
}

ComptCameraPrimaryGenerator::~ComptCameraPrimaryGenerator()
{
    delete _general_particle_source;
}

void ComptCameraPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    _general_particle_source->GeneratePrimaryVertex(anEvent);
}

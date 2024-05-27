#include "generator.hh"
#include "construction.hh"

#include "G4SystemOfUnits.hh"
#include "G4GeneralParticleSource.hh"

ComptCameraPrimaryGenerator::ComptCameraPrimaryGenerator()
{
    // Get world width from detector construction
    ComptCameraDetectorConstruction detectorConstruction;
    _world_width = detectorConstruction.GetWorldWidth();
    // Define particle gun
    _general_particle_source = new G4GeneralParticleSource();
    

ComptCameraPrimaryGenerator::~ComptCameraPrimaryGenerator()
{
    delete _general_particle_source;
}

void ComptCameraPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    _general_particle_source->GeneratePrimaryVertex(anEvent);
}

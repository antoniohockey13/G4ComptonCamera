#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    G4ThreeVector pos = G4ThreeVector(0.0, 0.0, 0.0);
    G4ThreeVector mom = G4ThreeVector(1.0, 0.0, 0.0);
    fParticleGun = new G4ParticleGun(1); // Number of particles per event
    fParticleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("photon"));
    fParticleGun->SetParticleEnergy(70 * KeV);
    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);

    fParticleGun->GeneratePrimaryVertex(anEvent);
    
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    fParticleGun->GeneratePrimaryVertex(anEvent);
}

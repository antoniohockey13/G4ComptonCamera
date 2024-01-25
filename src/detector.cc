#include "detector.hh"

ComptCamerasensitiveDetector::ComptCamerasensitiveDetector(G4String name): G4VSensitiveDetector(name)
{
}

ComptCamerasensitiveDetector::~ComptCamerasensitiveDetector()
{
}

G4bool ComptCamerasensitiveDetector::ProcessHits(G4Step *a_step, G4TouchableHistory *ro_hist)
{
    // TO DO
}

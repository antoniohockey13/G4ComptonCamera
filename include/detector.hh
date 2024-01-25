#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

class ComptCamerasensitiveDetector: public G4VSensitiveDetector
{
    public:
        ComptCamerasensitiveDetector(G4String);
        ~ComptCamerasensitiveDetector();
    
    private:
	virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
};

#endif

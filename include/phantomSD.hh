#ifndef PHANTOMSD_HH
#define PHANTOMSD_HH
//Copy from example B2a
#include "G4VSensitiveDetector.hh"
#include "phantomHit.hh"

#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

class G4Step;
class G4HCofThisEvent;

class phantomSD: public G4VSensitiveDetector
{
    public:
        phantomSD(const G4String& name, const G4String& hitsCollectionName);
        ~phantomSD() override = default;
    
    // Methods from base class
    void Initialize(G4HCofThisEvent* hitCollection) override;
    G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* hist) override;
    void EndOfEvent(G4HCofThisEvent* hitCollection) override;

    // Get hits collection
    phantomHitsCollection* GetHitsCollection() const { return _hits_collection_phantom;}

    private:
        phantomHitsCollection* _hits_collection_phantom = nullptr;
};

#endif

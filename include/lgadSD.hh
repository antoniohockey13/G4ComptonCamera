#ifndef LGADSD_HH
#define LGADSD_HH

#include "lgadhit.hh"

#include "G4VSensitiveDetector.hh"


class G4Step;
class G4HCofThisEvent;

class lgadSD: public G4VSensitiveDetector
{
    public:
        lgadSD(const G4String& name, const G4String& hitsCollectionName);
        ~lgadSD() override = default;
    
    // Methods from base class
    void Initialize(G4HCofThisEvent* hitCollection) override;
    G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* hist) override;
    void EndOfEvent(G4HCofThisEvent* hitCollection) override;

    // Get hits collection
    lgadHitsCollection* GetHitsCollection() const { return _hits_collection_lgad;}; 

    private:
        lgadHitsCollection* _hits_collection_lgad = nullptr;
};

#endif

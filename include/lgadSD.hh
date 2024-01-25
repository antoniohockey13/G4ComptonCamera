#ifndef LGADSD_HH
#define LGADSD_HH
//Copy from example B2a
#include "G4VSensitiveDetector.hh"
#include "lgadhit.hh"

#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

class G4Step;
class G4HCofThisEvent;

class LGADSD: public G4VSensitiveDetector
{
    public:
        LGADSD(const G4String& name,
                const G4String& hitsCollectionName);
        ~LGADSD() override = default;
    
    // Methods from base class
    void Initialize(G4HCofThisEvent* hitCollection) override;
    G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* hist) override;
    void EndOfEvent(G4HCofThisEvent* hitCollection) override;

    private:
        LGADHitsCollection* _f_hits_collection = nullptr;
};

#endif

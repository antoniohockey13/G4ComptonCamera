#include "lgadSD.hh"
// Copy from example B2a
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"


LGADSD::LGADSD(const G4String& name,
        const G4String& hitsCollectionName)
    : G4VSensitiveDetector(name)
    {
        collectionName.insert(hitsCollectionName);
    }

//

void LGADSD::Initialize(G4HCofThisEvent* hit_collection)
{
    // Create hits collection
    _f_hits_collection = new LGADHitsCollection(SensitiveDetectorName, collectionName[0]);

    // Add this collection in hce
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hit_collection->AddHitsCollection( hcID, _f_hits_collection );
}

G4bool LGADSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    // energy deposit
    G4double _e_dep = aStep->GetTotalEnergyDeposit();
    if (_e_dep==0.) return false;
    auto new_hit = new LGADHit();
    new_hit->SetTrackID  (aStep->GetTrack()->GetTrackID());
    new_hit->SetDetectorNb(aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber());
    new_hit->SetEdep(_e_dep);
    new_hit->SetPos (aStep->GetPostStepPoint()->GetPosition());
    
    _f_hits_collection->insert(new_hit);
    return true;
}

void LGADSD::EndOfEvent(G4HCofThisEvent*)
{
    if (verboseLevel>1) 
    {
        std::size_t nofHits = _f_hits_collection->entries();
        G4cout << G4endl
            << "-------->Hits Collection: in this event they are " << nofHits
            << " hits in the tracker chambers: " << G4endl;
        for ( std::size_t i=0; i<nofHits; i++ ) (*_f_hits_collection)[i]->Print();
    }
}

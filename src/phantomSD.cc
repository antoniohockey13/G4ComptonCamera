#include "phantomSD.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"


phantomSD::phantomSD(const G4String& name, const G4String& hitsCollectionName)
    : G4VSensitiveDetector(name)
    {
        collectionName.insert(hitsCollectionName);
    }

void phantomSD::Initialize(G4HCofThisEvent* hit_collection_phantom)
{
    // Create hits collection
    _hits_collection_phantom = new phantomHitsCollection(SensitiveDetectorName, collectionName[0]);

    // Add this collection in hit_collection
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hit_collection_phantom->AddHitsCollection( hcID, _hits_collection_phantom);
}

G4bool phantomSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    auto* track = aStep->GetTrack();

    if (track->GetParticleDefinition()->GetPDGEncoding() != 22)
        return false;

    if (aStep->GetPreStepPoint()->GetStepStatus() != fGeomBoundary)
        return false;

    auto* _new_hit = new phantomHit();
    _new_hit->SetPos(aStep->GetPostStepPoint()->GetPosition());
    _new_hit->SetMom(aStep->GetPreStepPoint()->GetMomentum());
    _new_hit->SetParticleID(22);
    _new_hit->SetKineticEnergy(aStep->GetPreStepPoint()->GetKineticEnergy());
    _new_hit->SetWeight(track->GetWeight());

    _hits_collection_phantom->insert(_new_hit);
    return true;
}

void phantomSD::EndOfEvent(G4HCofThisEvent*)
{
    if (verboseLevel>1) 
    {
        std::size_t nofHits = _hits_collection_phantom->entries();
        G4cout << G4endl
            << "-------->Hits Collection: in this event they are " << nofHits
            << " hits in the tracker chambers: " << G4endl;
        for ( std::size_t i=0; i<nofHits; i++ ) (*_hits_collection_phantom)[i]->Print();
    }
}

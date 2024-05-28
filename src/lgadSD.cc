#include "lgadSD.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"

lgadSD::lgadSD(const G4String& name, const G4String& hitsCollectionName)
    : G4VSensitiveDetector(name)
    {
        collectionName.insert(hitsCollectionName);
    }

void lgadSD::Initialize(G4HCofThisEvent* hit_collection_lgad)
{
    // Create hits collection
    _hits_collection_lgad = new lgadHitsCollection(SensitiveDetectorName, collectionName[0]);

    // Add this collection in hit_collection
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hit_collection_lgad->AddHitsCollection(hcID, _hits_collection_lgad);
}

G4bool lgadSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    // Only lower than because energy is always positive
    if (aStep->GetTotalEnergyDeposit() <1e-10) 
    {
        return false;
    }
    auto _new_hit = new lgadHit();
    _new_hit->SetTrackID  (aStep->GetTrack()->GetTrackID());
    _new_hit->SetDetectorNb(aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName());
    _new_hit->SetPos (aStep->GetPostStepPoint()->GetPosition());
    _new_hit->SetPreMom (aStep->GetPreStepPoint()->GetMomentum());
    _new_hit->SetPostMom (aStep->GetPostStepPoint()->GetMomentum());
    _new_hit->SetTime(aStep->GetPostStepPoint()->GetGlobalTime());
    _new_hit->SetParticleID(aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding());
    _new_hit->SetParentID(aStep->GetTrack()->GetParentID());
    _new_hit->SetTrackID(aStep->GetTrack()->GetTrackID());
    _new_hit->SetPostKineticEnergy(aStep->GetPostStepPoint()->GetKineticEnergy());
    _new_hit->SetPreKineticEnergy(aStep->GetPreStepPoint()->GetKineticEnergy());
    _new_hit->SetProcessName(aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName());
    _new_hit->SetStepLength(aStep->GetStepLength());
    _hits_collection_lgad->insert(_new_hit);
    
    //_new_hit->Print();
    return true;
}

void lgadSD::EndOfEvent(G4HCofThisEvent*)
{
    if (verboseLevel>1) 
    {
        std::size_t nofHits = _hits_collection_lgad->entries();
        G4cout << G4endl
            << "-------->Hits Collection: in this event they are " << nofHits
            << " hits in the tracker chambers: " << G4endl;
        for ( std::size_t i=0; i<nofHits; i++ ) (*_hits_collection_lgad)[i]->Print();
    }
}

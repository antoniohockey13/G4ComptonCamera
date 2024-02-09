#include "lgadSD.hh"
// Copy from example B2a
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"


LGADSD::LGADSD(const G4String& name, const G4String& hitsCollectionName)
    : G4VSensitiveDetector(name)
    {
        collectionName.insert(hitsCollectionName);
    }

void LGADSD::Initialize(G4HCofThisEvent* hit_collection)
{
    // Create hits collection
    _hits_collection = new LGADHitsCollection(SensitiveDetectorName, collectionName[0]);

    // Add this collection in hit_collection
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hit_collection->AddHitsCollection( hcID, _hits_collection );
}

G4bool LGADSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    // Only lower than because energy is always positive
    if (aStep->GetTotalEnergyDeposit() <1e-10) 
    {
        return false;
    }
    auto _new_hit = new LGADHit();
    _new_hit->SetTrackID  (aStep->GetTrack()->GetTrackID());
    _new_hit->SetDetectorNb(aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName());
    _new_hit->SetPos (aStep->GetPostStepPoint()->GetPosition()/mm);
    _new_hit->SetPreMom (aStep->GetPreStepPoint()->GetMomentum()/keV);
    _new_hit->SetPostMom (aStep->GetPostStepPoint()->GetMomentum()/keV);
    _new_hit->SetTime(aStep->GetPostStepPoint()->GetGlobalTime()/ps);
    _new_hit->SetParticleID(aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding());
    _new_hit->SetParentID(aStep->GetTrack()->GetParentID());
    _new_hit->SetTrackID(aStep->GetTrack()->GetTrackID());
    _new_hit->SetPostKineticEnergy(aStep->GetPostStepPoint()->GetKineticEnergy()/keV);
    _new_hit->SetPreKineticEnergy(aStep->GetPreStepPoint()->GetKineticEnergy()/keV);
    _new_hit->SetProcessName(aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName());
    _new_hit->SetStepLength(aStep->GetStepLength()/mm);
    _hits_collection->insert(_new_hit);
    
    //_new_hit->Print();
    return true;
}

void LGADSD::EndOfEvent(G4HCofThisEvent*)
{
    if (verboseLevel>1) 
    {
        std::size_t nofHits = _hits_collection->entries();
        G4cout << G4endl
            << "-------->Hits Collection: in this event they are " << nofHits
            << " hits in the tracker chambers: " << G4endl;
        for ( std::size_t i=0; i<nofHits; i++ ) (*_hits_collection)[i]->Print();
    }
}

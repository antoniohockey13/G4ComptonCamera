#include "lgadSD.hh"
// Copy from example B2a
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"


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
    // Dont store track in NTuple do it in processHits
    /*
    G4Track *track = aStep->GetTrack();
    // Photon enter detector
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    // Photon leave detector
	G4StepPoint *postStepPoint = aStep->GetPostStepPoint(); 

    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
	// Which detector was hit
    G4int copyNo = touchable->GetCopyNumber(); 
    
	G4VPhysicalVolume *physVol = touchable->GetVolume();
	G4ThreeVector posDetector = physVol->GetTranslation();

	G4int event = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    // Creates instance off singleton G4AnalysisManager. Whenever you change this lines in run.cc you should go to detector.hh to make the same changes
	G4AnalysisManager *anManager = G4AnalysisManager::Instance();															  
	anManager->FillNtupleIColumn(0, 0, event);
    anManager->FillNtupleIColumn(0, 1, copyNo);
	anManager->FillNtupleDColumn(0, 2, posDetector[0]);
	anManager->FillNtupleDColumn(0, 3, posDetector[1]);
	anManager->FillNtupleDColumn(0, 4, posDetector[2]);

	// Get the pre-step kinetic energy
	G4double kinEnergy = aStep->GetPreStepPoint()->GetKineticEnergy();

	// Print out the kinetic energy of the particle
	// G4cout << "Particle Kinetic Energy: " << kinEnergy / MeV << " MeV" << G4endl;
    anManager->FillNtupleDColumn(0, 5, kinEnergy);
	// Export kinEnergy to root

	// Get particle id
	G4ParticleDefinition *particleDef = track->GetDefinition();
	G4int particleID = particleDef->GetPDGEncoding();
	track->GetDynamicParticle()->GetPDGcode();

	// Export particle ID to root
	anManager->FillNtupleIColumn(0, 6, particleID);
	anManager->AddNtupleRow(0);
    */
   
    // Exaple B2a
    // energy deposits
    G4double _e_dep = aStep->GetTotalEnergyDeposit();
    if (_e_dep==0.) return false;

    auto _new_hit = new LGADHit();
    
    _new_hit->SetTrackID  (aStep->GetTrack()->GetTrackID());
    // Not Working 
    _new_hit->SetDetectorNb(aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber());
    _new_hit->SetEdep(_e_dep);
    _new_hit->SetPos (aStep->GetPostStepPoint()->GetPosition());
    _new_hit->SetMom (aStep->GetPostStepPoint()->GetMomentum());

    
    _hits_collection->insert(_new_hit);
    _new_hit->Print();
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

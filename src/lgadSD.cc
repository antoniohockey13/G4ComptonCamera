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
	anManager->AddNtupleRow(0);

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

	anManager->AddNtupleRow(2);


    // Exaple B2a
    // energy deposits
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

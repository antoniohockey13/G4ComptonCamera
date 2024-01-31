#include "event.hh"
#include "lgadhit.hh"

ComptCameraEventAction::ComptCameraEventAction(ComptCameraRunAction *)
{
}
ComptCameraEventAction::~ComptCameraEventAction()
{}

void ComptCameraEventAction::BeginOfEventAction(const G4Event *)
{
}

void ComptCameraEventAction::EndOfEventAction(const G4Event* event)
{
    //get event ID
    G4int event_id = event->GetEventID();
    G4cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << G4endl;
    G4cout << "Event ID: " << event_id << G4endl;
    G4cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << G4endl;
   
    //Get hit_collection
    LGADHitsCollection* hit_collection = static_cast<LGADHitsCollection*>(event->GetHCofThisEvent()->GetHC(0));
    
    //Get analysis manager
    G4AnalysisManager *anManager = G4AnalysisManager::Instance();
   
    // Loop in hit collection to store the data in the tuples
    // Defined in run.cc
    for (size_t i = 0; i < hit_collection->entries(); ++i)
    {
        anManager->FillNtupleIColumn(0, 0, (*hit_collection)[i]->GetTrackID());
        anManager->FillNtupleIColumn(0, 1, (*hit_collection)[i]->GetDetectorNb());
        anManager->FillNtupleDColumn(0, 2, (*hit_collection)[i]->GetPos()[0]);
        anManager->FillNtupleDColumn(0, 3, (*hit_collection)[i]->GetPos()[1]);
        anManager->FillNtupleDColumn(0, 4, (*hit_collection)[i]->GetPos()[2]);
        anManager->FillNtupleDColumn(0, 5, (*hit_collection)[i]->GetMom()[0]);
        anManager->FillNtupleDColumn(0, 5, (*hit_collection)[i]->GetMom()[1]);
        anManager->FillNtupleDColumn(0, 5, (*hit_collection)[i]->GetMom()[2]);
        anManager->FillNtupleDColumn(0, 6, (*hit_collection)[i]->GetEdep());
        //anManager->FillNtupleIColumn(0, 7, (*hit_collection)[i]->GetParticleID());
        anManager->AddNtupleRow(0);
    }
    
}

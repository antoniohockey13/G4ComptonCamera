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
    //Get hit_collection
    LGADHitsCollection* hit_collection = static_cast<LGADHitsCollection*>(event->GetHCofThisEvent()->GetHC(0));
    
    //Get analysis manager
    G4AnalysisManager *anManager = G4AnalysisManager::Instance();
   
    // Loop in hit collection to store the data in the tuples
    // Defined in run.cc
    for (size_t i = 0; i < hit_collection->entries(); ++i)
    {
        anManager->FillNtupleIColumn(0, 0, event_id);
        anManager->FillNtupleIColumn(0, 1, (*hit_collection)[i]->GetDetectorNb());
        anManager->FillNtupleDColumn(0, 2, (*hit_collection)[i]->GetPos()[0]);
        anManager->FillNtupleDColumn(0, 3, (*hit_collection)[i]->GetPos()[1]);
        anManager->FillNtupleDColumn(0, 4, (*hit_collection)[i]->GetPos()[2]);
        anManager->FillNtupleDColumn(0, 5, (*hit_collection)[i]->GetPreMom()[0]);
        anManager->FillNtupleDColumn(0, 6, (*hit_collection)[i]->GetPreMom()[1]);
        anManager->FillNtupleDColumn(0, 7, (*hit_collection)[i]->GetPreMom()[2]);
        anManager->FillNtupleDColumn(0, 8, (*hit_collection)[i]->GetPostMom()[0]);
        anManager->FillNtupleDColumn(0, 9, (*hit_collection)[i]->GetPostMom()[1]);
        anManager->FillNtupleDColumn(0, 10, (*hit_collection)[i]->GetPostMom()[2]);
        anManager->FillNtupleDColumn(0, 11, (*hit_collection)[i]->GetEnergyLost());
        anManager->FillNtupleIColumn(0, 12, (*hit_collection)[i]->GetParticleID());
        anManager->FillNtupleIColumn(0, 13, (*hit_collection)[i]->GetTrackID());
        anManager->FillNtupleIColumn(0, 14, (*hit_collection)[i]->GetParentID());
        anManager->FillNtupleDColumn(0, 15, (*hit_collection)[i]->GetTime());
        anManager->FillNtupleDColumn(0, 16, (*hit_collection)[i]->GetPreKineticEnergy());
        anManager->FillNtupleDColumn(0, 17, (*hit_collection)[i]->GetPostKineticEnergy());
        anManager->FillNtupleSColumn(0, 18, (*hit_collection)[i]->GetProcessName());
        anManager->AddNtupleRow(0);
    }
}

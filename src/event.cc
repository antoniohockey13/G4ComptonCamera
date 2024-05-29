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
        anManager->FillNtupleDColumn(0, 2, (*hit_collection)[i]->GetPos()[0]/mm);
        anManager->FillNtupleDColumn(0, 3, (*hit_collection)[i]->GetPos()[1]/mm);
        anManager->FillNtupleDColumn(0, 4, (*hit_collection)[i]->GetPos()[2]/mm);
        anManager->FillNtupleDColumn(0, 5, (*hit_collection)[i]->GetMom()[0]/keV);
        anManager->FillNtupleDColumn(0, 6, (*hit_collection)[i]->GetMom()[1]/keV);
        anManager->FillNtupleDColumn(0, 7, (*hit_collection)[i]->GetMom()[2]/keV);
        anManager->FillNtupleDColumn(0, 8, (*hit_collection)[i]->GetEdep()/keV);
        anManager->FillNtupleIColumn(0, 9, (*hit_collection)[i]->GetParticleID());
        anManager->FillNtupleIColumn(0, 10, (*hit_collection)[i]->GetTrackID());
        anManager->FillNtupleIColumn(0, 11, (*hit_collection)[i]->GetParentID());
        anManager->FillNtupleIColumn(0, 12, (*hit_collection)[i]->GetTime()/ps);
        anManager->FillNtupleDColumn(0, 13, (*hit_collection)[i]->GetKineticEnergy()/keV);
        anManager->FillNtupleDColumn(0, 14, (*hit_collection)[i]->GetPostKineticEnergy()/keV);
        anManager->FillNtupleSColumn(0, 15, (*hit_collection)[i]->GetProcessName());
        anManager->AddNtupleRow(0);
    }
    
}

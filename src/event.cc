#include "event.hh"
#include "lgadhit.hh"
#include "phantomHit.hh"

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
    //Get hit_collection lgad
    lgadHitsCollection* hit_collection_lgad = static_cast<lgadHitsCollection*>(event->GetHCofThisEvent()->GetHC(0));
    
    //Get analysis manager
    G4AnalysisManager *anManager_lgad = G4AnalysisManager::Instance();
   
    // Loop in hit collection to store the data in the tuples
    // Defined in run.cc
    for (size_t i = 0; i < hit_collection_lgad->entries(); ++i)
    {
        anManager_lgad->FillNtupleIColumn(0, 0, event_id);
        anManager_lgad->FillNtupleIColumn(0, 1, (*hit_collection_lgad)[i]->GetDetectorNb());
        anManager_lgad->FillNtupleDColumn(0, 2, (*hit_collection_lgad)[i]->GetPos()[0]);
        anManager_lgad->FillNtupleDColumn(0, 3, (*hit_collection_lgad)[i]->GetPos()[1]);
        anManager_lgad->FillNtupleDColumn(0, 4, (*hit_collection_lgad)[i]->GetPos()[2]);
        anManager_lgad->FillNtupleDColumn(0, 5, (*hit_collection_lgad)[i]->GetPreMom()[0]);
        anManager_lgad->FillNtupleDColumn(0, 6, (*hit_collection_lgad)[i]->GetPreMom()[1]);
        anManager_lgad->FillNtupleDColumn(0, 7, (*hit_collection_lgad)[i]->GetPreMom()[2]);
        anManager_lgad->FillNtupleDColumn(0, 8, (*hit_collection_lgad)[i]->GetPostMom()[0]);
        anManager_lgad->FillNtupleDColumn(0, 9, (*hit_collection_lgad)[i]->GetPostMom()[1]);
        anManager_lgad->FillNtupleDColumn(0, 10, (*hit_collection_lgad)[i]->GetPostMom()[2]);
        anManager_lgad->FillNtupleDColumn(0, 11, (*hit_collection_lgad)[i]->GetEnergyLost());
        anManager_lgad->FillNtupleIColumn(0, 12, (*hit_collection_lgad)[i]->GetParticleID());
        anManager_lgad->FillNtupleIColumn(0, 13, (*hit_collection_lgad)[i]->GetTrackID());
        anManager_lgad->FillNtupleIColumn(0, 14, (*hit_collection_lgad)[i]->GetParentID());
        anManager_lgad->FillNtupleDColumn(0, 15, (*hit_collection_lgad)[i]->GetTime());
        anManager_lgad->FillNtupleDColumn(0, 16, (*hit_collection_lgad)[i]->GetPreKineticEnergy());
        anManager_lgad->FillNtupleDColumn(0, 17, (*hit_collection_lgad)[i]->GetPostKineticEnergy());
        anManager_lgad->FillNtupleSColumn(0, 18, (*hit_collection_lgad)[i]->GetProcessName());
        anManager_lgad->FillNtupleDColumn(0, 19, (*hit_collection_lgad)[i]->GetComptonAngle());
        anManager_lgad->FillNtupleDColumn(0, 20, (*hit_collection_lgad)[i]->GetStepLength());
        anManager_lgad->AddNtupleRow(0);
    }

    //Get hit_collection phantom
    phantomHitsCollection* hit_collection_phantom = static_cast<phantomHitsCollection*>(event->GetHCofThisEvent()->GetHC(0));
    
    //Get analysis manager
    G4AnalysisManager *anManager_phantom = G4AnalysisManager::Instance();
   
    // Loop in hit collection to store the data in the tuples
    // Defined in run.cc
    for (size_t i = 0; i < hit_collection_phantom->entries(); ++i)
    {
        anManager_phantom->FillNtupleIColumn(1, 0, event_id);
        anManager_phantom->FillNtupleIColumn(1, 1, (*hit_collection_phantom)[i]->GetDetectorNb());
        anManager_phantom->FillNtupleDColumn(1, 2, (*hit_collection_phantom)[i]->GetPos()[0]);
        anManager_phantom->FillNtupleDColumn(1, 3, (*hit_collection_phantom)[i]->GetPos()[1]);
        anManager_phantom->FillNtupleDColumn(1, 4, (*hit_collection_phantom)[i]->GetPos()[2]);
        anManager_phantom->FillNtupleDColumn(1, 5, (*hit_collection_phantom)[i]->GetPreMom()[0]);
        anManager_phantom->FillNtupleDColumn(1, 6, (*hit_collection_phantom)[i]->GetPreMom()[1]);
        anManager_phantom->FillNtupleDColumn(1, 7, (*hit_collection_phantom)[i]->GetPreMom()[2]);
        anManager_phantom->FillNtupleDColumn(1, 8, (*hit_collection_phantom)[i]->GetPostMom()[0]);
        anManager_phantom->FillNtupleDColumn(1, 9, (*hit_collection_phantom)[i]->GetPostMom()[1]);
        anManager_phantom->FillNtupleDColumn(1, 10, (*hit_collection_phantom)[i]->GetPostMom()[2]);
        anManager_phantom->FillNtupleDColumn(1, 11, (*hit_collection_phantom)[i]->GetEnergyLost());
        anManager_phantom->FillNtupleIColumn(1, 12, (*hit_collection_phantom)[i]->GetParticleID());
        anManager_phantom->FillNtupleIColumn(1, 13, (*hit_collection_phantom)[i]->GetTrackID());
        anManager_phantom->FillNtupleIColumn(1, 14, (*hit_collection_phantom)[i]->GetParentID());
        anManager_phantom->FillNtupleDColumn(1, 15, (*hit_collection_phantom)[i]->GetTime());
        anManager_phantom->FillNtupleDColumn(1, 16, (*hit_collection_phantom)[i]->GetPreKineticEnergy());
        anManager_phantom->FillNtupleDColumn(1, 17, (*hit_collection_phantom)[i]->GetPostKineticEnergy());
        anManager_phantom->FillNtupleSColumn(1, 18, (*hit_collection_phantom)[i]->GetProcessName());
        anManager_phantom->FillNtupleDColumn(1, 19, (*hit_collection_phantom)[i]->GetComptonAngle());
        anManager_phantom->FillNtupleDColumn(1, 20, (*hit_collection_phantom)[i]->GetStepLength());
        anManager_phantom->AddNtupleRow(1);
    }
}

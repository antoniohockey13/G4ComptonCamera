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
    auto hit_collection = event->GetHCofThisEvent()->GetHC(0);
    G4cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << G4endl;
    G4cout << "Hit Collection: " << hit_collection << G4endl;
    G4cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << G4endl;
    //Get analysis manager
    G4AnalysisManager *anManager = G4AnalysisManager::Instance();
    // Loop in hit collection to store the data in the tuples
    // Defined in run.cc
    for (auto hit : *hit_collection)
    {
        anManager->FillNtupleIColumn(0, 0, hit->GetEventID());
        anManager->FillNtupleIColumn(0, 1, hit->GetDetectorNb());
        anManager->FillNtupleDColumn(0, 2, hit->GetPos()[0]);
        anManager->FillNtupleDColumn(0, 3, hit->GetPos()[1]);
        anManager->FillNtupleDColumn(0, 4, hit->GetPos()[2]);
        anManager->FillNtupleDColumn(0, 5, hit->GetKinEnergy());
        anManager->FillNtupleIColumn(0, 6, hit->GetParticleID());
        anManager->AddNtupleRow(0);
    }
    
}

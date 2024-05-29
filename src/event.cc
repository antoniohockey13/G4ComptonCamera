#include "event.hh"
#include "phantomHit.hh"
#include "construction.hh"

#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"


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
  
    // Loop in hit collection to store the data in the tuples
    // Defined in run.cc
    //Get analysis manager
    G4AnalysisManager *anManager = G4AnalysisManager::Instance();
    //Get hit_collection phantom
    const G4int phantomID = G4SDManager::GetSDMpointer()->GetCollectionID("phantomHitsCollection");    
    phantomHitsCollection* hit_collection_phantom = static_cast<phantomHitsCollection*>(event->GetHCofThisEvent()->GetHC(phantomID));

    for (size_t i = 0; i < hit_collection_phantom->entries(); ++i)
    {
        anManager->FillNtupleIColumn(0, 0, event_id);
        anManager->FillNtupleDColumn(0, 1, (*hit_collection_phantom)[i]->GetPos()[0]/mm);
        anManager->FillNtupleDColumn(0, 2, (*hit_collection_phantom)[i]->GetPos()[1]/mm);
        anManager->FillNtupleDColumn(0, 3, (*hit_collection_phantom)[i]->GetPos()[2]/mm);
        anManager->FillNtupleDColumn(0, 4, (*hit_collection_phantom)[i]->GetMom()[0]/keV);
        anManager->FillNtupleDColumn(0, 5, (*hit_collection_phantom)[i]->GetMom()[1]/keV);
        anManager->FillNtupleDColumn(0, 6, (*hit_collection_phantom)[i]->GetMom()[2]/keV);
        anManager->FillNtupleDColumn(0, 7, (*hit_collection_phantom)[i]->GetEnergyLost()/keV);
        anManager->FillNtupleIColumn(0, 8, (*hit_collection_phantom)[i]->GetParticleID());
        anManager->AddNtupleRow(0);
    
    }

}

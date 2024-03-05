#include "event.hh"
#include "lgadhit.hh"
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
    //Get hit_collection lgad
    const G4int lgadID = G4SDManager::GetSDMpointer()->GetCollectionID("lgadHitsCollection");
    lgadHitsCollection* hit_collection_lgad = static_cast<lgadHitsCollection*>(event->GetHCofThisEvent()->GetHC(lgadID));
    
    //Get analysis manager
    G4AnalysisManager *anManager = G4AnalysisManager::Instance();
    for (size_t i = 0; i < hit_collection_lgad->entries(); ++i)
    {
        anManager->FillNtupleIColumn(0, 0, event_id);
        anManager->FillNtupleIColumn(0, 1, (*hit_collection_lgad)[i]->GetDetectorNb());
        anManager->FillNtupleDColumn(0, 2, (*hit_collection_lgad)[i]->GetPos()[0]/mm);
        anManager->FillNtupleDColumn(0, 3, (*hit_collection_lgad)[i]->GetPos()[1]/mm);
        anManager->FillNtupleDColumn(0, 4, (*hit_collection_lgad)[i]->GetPos()[2]/mm);
        anManager->FillNtupleDColumn(0, 5, (*hit_collection_lgad)[i]->GetPreMom()[0]/keV);
        anManager->FillNtupleDColumn(0, 6, (*hit_collection_lgad)[i]->GetPreMom()[1]/keV);
        anManager->FillNtupleDColumn(0, 7, (*hit_collection_lgad)[i]->GetPreMom()[2]/keV);
        anManager->FillNtupleDColumn(0, 8, (*hit_collection_lgad)[i]->GetPostMom()[0]/keV);
        anManager->FillNtupleDColumn(0, 9, (*hit_collection_lgad)[i]->GetPostMom()[1]/keV);
        anManager->FillNtupleDColumn(0, 10, (*hit_collection_lgad)[i]->GetPostMom()[2]/keV);
        anManager->FillNtupleDColumn(0, 11, (*hit_collection_lgad)[i]->GetEnergyLost()/keV);
        anManager->FillNtupleIColumn(0, 12, (*hit_collection_lgad)[i]->GetParticleID());
        anManager->FillNtupleIColumn(0, 13, (*hit_collection_lgad)[i]->GetTrackID());
        anManager->FillNtupleIColumn(0, 14, (*hit_collection_lgad)[i]->GetParentID());
        anManager->FillNtupleDColumn(0, 15, (*hit_collection_lgad)[i]->GetTime()/ps);
        anManager->FillNtupleDColumn(0, 16, (*hit_collection_lgad)[i]->GetPreKineticEnergy()/keV);
        anManager->FillNtupleDColumn(0, 17, (*hit_collection_lgad)[i]->GetPostKineticEnergy()/keV);
        anManager->FillNtupleSColumn(0, 18, (*hit_collection_lgad)[i]->GetProcessName());
        anManager->FillNtupleDColumn(0, 19, (*hit_collection_lgad)[i]->GetComptonAngle());
        anManager->FillNtupleDColumn(0, 20, (*hit_collection_lgad)[i]->GetStepLength()/mm);
        anManager->AddNtupleRow(0);
    }

    // Compton hits, photon hit both detectors
    for (size_t i = 0; i < hit_collection_lgad->entries(); ++i)
    {
        // Check if the particle is a photon and hit the first detector
        if (((*hit_collection_lgad)[i]->GetParticleID() == 22) & ((*hit_collection_lgad)[i]->GetDetectorNb() == 1))
        {
            for (size_t j = 0; j < hit_collection_lgad->entries(); ++j)
            {
                // Check if the particle is a photon, hit the second detector and the same track than the first detector hit
                if (((*hit_collection_lgad)[j]->GetParticleID() == 22) & ((*hit_collection_lgad)[j]->GetDetectorNb() == 2) &
                    ((*hit_collection_lgad)[j]->GetTrackID() == (*hit_collection_lgad)[i]->GetTrackID()) &
                    ((*hit_collection_lgad)[j]->GetTime() > (*hit_collection_lgad)[i]->GetTime()))
                {
                    anManager->FillNtupleIColumn(1, 0, event_id);
                    anManager->FillNtupleDColumn(1, 1, (*hit_collection_lgad)[i]->GetPos()[0]/mm);
                    anManager->FillNtupleDColumn(1, 2, (*hit_collection_lgad)[i]->GetPos()[1]/mm);
                    anManager->FillNtupleDColumn(1, 3, (*hit_collection_lgad)[i]->GetPos()[2]/mm);
                    anManager->FillNtupleDColumn(1, 4, (*hit_collection_lgad)[j]->GetPos()[0]/mm);
                    anManager->FillNtupleDColumn(1, 5, (*hit_collection_lgad)[j]->GetPos()[1]/mm);
                    anManager->FillNtupleDColumn(1, 6, (*hit_collection_lgad)[j]->GetPos()[2]/mm);
                    anManager->FillNtupleDColumn(1, 7, (*hit_collection_lgad)[i]->GetPreMom()[0]/keV);
                    anManager->FillNtupleDColumn(1, 8, (*hit_collection_lgad)[i]->GetPreMom()[1]/keV);
                    anManager->FillNtupleDColumn(1, 9, (*hit_collection_lgad)[i]->GetPreMom()[2]/keV);
                    anManager->FillNtupleDColumn(1, 10, (*hit_collection_lgad)[i]->GetPostMom()[0]/keV);
                    anManager->FillNtupleDColumn(1, 11, (*hit_collection_lgad)[i]->GetPostMom()[1]/keV);
                    anManager->FillNtupleDColumn(1, 12, (*hit_collection_lgad)[i]->GetPostMom()[2]/keV);
                    anManager->FillNtupleDColumn(1, 13, (*hit_collection_lgad)[i]->GetTime()/ps);
                    anManager->FillNtupleDColumn(1, 14, (*hit_collection_lgad)[j]->GetTime()/ps);
                    anManager->FillNtupleDColumn(1, 15, (*hit_collection_lgad)[i]->GetComptonAngle());
                    anManager->AddNtupleRow(1);
                }
            }    
        }
        
    }

    //Get hit_collection phantom
    const G4int phantomID = G4SDManager::GetSDMpointer()->GetCollectionID("phantomHitsCollection");    
    if(phantomID != -1)
    {
        phantomHitsCollection* hit_collection_phantom = static_cast<phantomHitsCollection*>(event->GetHCofThisEvent()->GetHC(phantomID));

        for (size_t i = 0; i < hit_collection_phantom->entries(); ++i)
        {
            anManager->FillNtupleIColumn(2, 0, event_id);
            anManager->FillNtupleDColumn(2, 1, (*hit_collection_phantom)[i]->GetPos()[0]/mm);
            anManager->FillNtupleDColumn(2, 2, (*hit_collection_phantom)[i]->GetPos()[1]/mm);
            anManager->FillNtupleDColumn(2, 3, (*hit_collection_phantom)[i]->GetPos()[2]/mm);
            anManager->FillNtupleDColumn(2, 4, (*hit_collection_phantom)[i]->GetMom()[0]/keV);
            anManager->FillNtupleDColumn(2, 5, (*hit_collection_phantom)[i]->GetMom()[1]/keV);
            anManager->FillNtupleDColumn(2, 6, (*hit_collection_phantom)[i]->GetMom()[2]/keV);
            anManager->FillNtupleDColumn(2, 7, (*hit_collection_phantom)[i]->GetEnergyLost()/keV);
            anManager->FillNtupleIColumn(2, 8, (*hit_collection_phantom)[i]->GetParticleID());
            anManager->AddNtupleRow(2);
        }
    }

}

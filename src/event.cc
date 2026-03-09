#include "event.hh"
#include "lgadhit.hh"
#include "phantomHit.hh"
#include "construction.hh"

#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"

#include <set>
#include <vector>

ComptCameraEventAction::ComptCameraEventAction(bool is_phantom) :
    _is_phantom(is_phantom)
{
}

ComptCameraEventAction::~ComptCameraEventAction()
{
}

void ComptCameraEventAction::BeginOfEventAction(const G4Event *)
{
}

void ComptCameraEventAction::EndOfEventAction(const G4Event* event)
{

    G4int event_id = event->GetEventID();

    auto* hce = event->GetHCofThisEvent();
    if (!hce)
    {
        G4cout << "[DEBUG] No HCofThisEvent for event " << event_id << G4endl;
        return;
    }

    G4int lgadID = G4SDManager::GetSDMpointer()->GetCollectionID("lgadHitsCollection");
    if (lgadID < 0)
    {
        G4cout << "[DEBUG] lgadHitsCollection ID not found in event " << event_id << G4endl;
        return;
    }

    auto* hit_collection_lgad =
        static_cast<lgadHitsCollection*>(hce->GetHC(lgadID));

    if (!hit_collection_lgad)
    {
        G4cout << "[DEBUG] hit_collection_lgad is null in event " << event_id << G4endl;
        return;
    }
    //Get analysis manager
    G4AnalysisManager *anManager = G4AnalysisManager::Instance();

    // Only one match per track, to avoid double counting
    std::set<G4int> matchedTracks;

    // Separate hits in the two detectors
    std::vector<lgadHit*> hits_det1;
    std::vector<lgadHit*> hits_det2;
    // Compton hits, photon hit both detectors
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

        if ((*hit_collection_lgad)[i]->GetDetectorNb()==1)
            hits_det1.push_back((*hit_collection_lgad)[i]);

        if ((*hit_collection_lgad)[i]->GetDetectorNb()==2)
            hits_det2.push_back((*hit_collection_lgad)[i]);
        
    }

    // Build all detector1-detector2 pairs (experiment-like pairs)
    for (auto* hit1 : hits_det1)
    {
        for (auto* hit2 : hits_det2)
        {
            G4double dt = hit2->GetTime() - hit1->GetTime();

            G4ThreeVector r12 = (hit2->GetPos() - hit1->GetPos()).unit();
            G4ThreeVector beam_dir(1,0,0);
            G4double theta_geom = beam_dir.angle(r12);

            G4int samePhoton = 0;
            if (hit1->GetTrackID() == hit2->GetTrackID() &&
                hit1->GetParticleID() == 22 &&
                hit2->GetParticleID() == 22)
            {
                samePhoton = 1;
            }
            anManager->FillNtupleIColumn(3, 0, event_id);
            anManager->FillNtupleDColumn(3, 1, hit1->GetPos()[0]/mm);
            anManager->FillNtupleDColumn(3, 2, hit1->GetPos()[1]/mm);
            anManager->FillNtupleDColumn(3, 3, hit1->GetPos()[2]/mm);
            anManager->FillNtupleDColumn(3, 4, hit2->GetPos()[0]/mm);
            anManager->FillNtupleDColumn(3, 5, hit2->GetPos()[1]/mm);
            anManager->FillNtupleDColumn(3, 6, hit2->GetPos()[2]/mm);
            anManager->FillNtupleDColumn(3, 7, hit1->GetTime()/ps);
            anManager->FillNtupleDColumn(3, 8, hit2->GetTime()/ps);
            anManager->FillNtupleDColumn(3, 9, hit1->GetEnergyLost()/keV);
            anManager->FillNtupleDColumn(3,10, hit2->GetEnergyLost()/keV);
            anManager->FillNtupleIColumn(3,11, hit1->GetTrackID());
            anManager->FillNtupleIColumn(3,12, hit2->GetTrackID());
            anManager->FillNtupleIColumn(3,13, hit1->GetParticleID());
            anManager->FillNtupleIColumn(3,14, hit2->GetParticleID());
            anManager->FillNtupleIColumn(3,15, samePhoton);
            anManager->AddNtupleRow(3);
        }
    }

    for (size_t i = 0; i < hit_collection_lgad->entries(); ++i)
    {

        // Check if the particle is a photon and hit the first detector
        if (((*hit_collection_lgad)[i]->GetParticleID() == 22) && ((*hit_collection_lgad)[i]->GetDetectorNb() == 1))
        {
            G4int trackID = (*hit_collection_lgad)[i]->GetTrackID();
            if (matchedTracks.count(trackID) > 0) continue; // Skip if this track has already been matched

            for (size_t j = i; j < hit_collection_lgad->entries(); ++j)
            {

                // Check if the particle is a photon and hit the second detector
                if (((*hit_collection_lgad)[j]->GetParticleID() == 22) && ((*hit_collection_lgad)[j]->GetDetectorNb() == 2) &&
                   ((*hit_collection_lgad)[j]->GetTrackID() == (*hit_collection_lgad)[i]->GetTrackID()) &&
                   ((*hit_collection_lgad)[j]->GetTime() > (*hit_collection_lgad)[i]->GetTime()))
                {
                    G4ThreeVector dir1 = (*hit_collection_lgad)[i]->GetPreMom().unit();
                    G4ThreeVector dir2 = (*hit_collection_lgad)[j]->GetPreMom().unit();
                    G4double theta_mom = dir1.angle(dir2);

                    G4ThreeVector dir12 = ((*hit_collection_lgad)[j]->GetPos() - (*hit_collection_lgad)[i]->GetPos()).unit();
                    G4double theta_geom = dir1.angle(dir12);

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
                    anManager->FillNtupleDColumn(1, 16, (*hit_collection_lgad)[i]->GetEnergyLost()/keV);
                    anManager->FillNtupleDColumn(1, 17, (*hit_collection_lgad)[j]->GetEnergyLost()/keV);
                    anManager->AddNtupleRow(1);

                    matchedTracks.insert(trackID);
                    break;
                }
            }
        }
    }

    //Get hit_collection phantom
    if (_is_phantom)
    {
        if (_phantomHCID < 0)
            _phantomHCID = G4SDManager::GetSDMpointer()->GetCollectionID("phantomHitsCollection");

        if (_phantomHCID >= 0)
        {
            auto* hit_collection_phantom =
                static_cast<phantomHitsCollection*>(hce->GetHC(_phantomHCID));

            if (hit_collection_phantom)
            {
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
    }


    int nDet1 = 0;
    int nDet2 = 0;

    for (size_t i = 0; i < hit_collection_lgad->entries(); ++i)
    {
        if ((*hit_collection_lgad)[i]->GetDetectorNb() == 1) nDet1++;
        if ((*hit_collection_lgad)[i]->GetDetectorNb() == 2) nDet2++;
    }

}
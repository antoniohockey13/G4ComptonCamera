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
#include <map>

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

    G4AnalysisManager* anManager = G4AnalysisManager::Instance();

    // Fill full Hits ntuple and split by detector
    for (size_t i = 0; i < hit_collection_lgad->entries(); ++i)
    {
        auto* hit = (*hit_collection_lgad)[i];

        anManager->FillNtupleIColumn(0, 0, event_id);
        anManager->FillNtupleIColumn(0, 1, hit->GetDetectorNb());
        anManager->FillNtupleDColumn(0, 2, hit->GetPos()[0] / mm);
        anManager->FillNtupleDColumn(0, 3, hit->GetPos()[1] / mm);
        anManager->FillNtupleDColumn(0, 4, hit->GetPos()[2] / mm);
        anManager->FillNtupleDColumn(0, 5, hit->GetPreMom()[0] / keV);
        anManager->FillNtupleDColumn(0, 6, hit->GetPreMom()[1] / keV);
        anManager->FillNtupleDColumn(0, 7, hit->GetPreMom()[2] / keV);
        anManager->FillNtupleDColumn(0, 8, hit->GetPostMom()[0] / keV);
        anManager->FillNtupleDColumn(0, 9, hit->GetPostMom()[1] / keV);
        anManager->FillNtupleDColumn(0,10, hit->GetPostMom()[2] / keV);
        anManager->FillNtupleDColumn(0,11, hit->GetEnergyLost() / keV);
        anManager->FillNtupleIColumn(0,12, hit->GetParticleID());
        anManager->FillNtupleIColumn(0,13, hit->GetTrackID());
        anManager->FillNtupleIColumn(0,14, hit->GetParentID());
        anManager->FillNtupleDColumn(0,15, hit->GetTime() / ps);
        anManager->FillNtupleDColumn(0,16, hit->GetPreKineticEnergy() / keV);
        anManager->FillNtupleDColumn(0,17, hit->GetPostKineticEnergy() / keV);
        anManager->FillNtupleSColumn(0,18, hit->GetProcessName());
        anManager->FillNtupleDColumn(0,19, hit->GetComptonAngle());
        anManager->FillNtupleDColumn(0,20, hit->GetStepLength() / mm);
        anManager->FillNtupleDColumn(0,21, hit->GetWeight());
        anManager->AddNtupleRow(0);

    // For each gamma track:
    // 1) first hit hit in detector 1
    // 2) first hit in detector 2 after that
    struct TrackPairInfo
    {
        lgadHit* firstDet1Interaction = nullptr;
        lgadHit* firstDet2After = nullptr;
    };

    std::map<G4int, TrackPairInfo> trackMap;

    // First gamma hit with energy loss in detector 1
    for (size_t i = 0; i < hit_collection_lgad->entries(); ++i)
    {
        auto* hit = (*hit_collection_lgad)[i];

        if (hit->GetParticleID() != 22) continue;
        if (hit->GetDetectorNb() != 1) continue;
        G4double deltaE = hit->GetPreKineticEnergy() - hit->GetPostKineticEnergy();
        if (deltaE <= 0.) continue;

        G4int trackID = hit->GetTrackID();

        auto it = trackMap.find(trackID);
        if (it == trackMap.end())
        {
            TrackPairInfo info;
            info.firstDet1Interaction = hit;
            trackMap[trackID] = info;
        }
        else
        {
            if (it->second.firstDet1Interaction == nullptr ||
                hit->GetTime() < it->second.firstDet1Interaction->GetTime())
            {
                it->second.firstDet1Interaction = hit;
            }
        }
    }

    // First gamma hit in detector 2 after the detector 1 interaction
    for (size_t i = 0; i < hit_collection_lgad->entries(); ++i)
    {
        auto* hit = (*hit_collection_lgad)[i];

        if (hit->GetParticleID() != 22) continue;
        if (hit->GetDetectorNb() != 2) continue;
        G4double deltaE2 = hit->GetPreKineticEnergy() - hit->GetPostKineticEnergy();
        if (deltaE2 <= 0.) continue;

        G4int trackID = hit->GetTrackID();

        auto it = trackMap.find(trackID);
        if (it == trackMap.end()) continue;
        if (it->second.firstDet1Interaction == nullptr) continue;

        auto* hit1 = it->second.firstDet1Interaction;

        if (hit->GetTime() <= hit1->GetTime()) continue;

        if (it->second.firstDet2After == nullptr ||
            hit->GetTime() < it->second.firstDet2After->GetTime())
        {
            it->second.firstDet2After = hit;
        }
    }

    // Fill ComptonHits ntuple with one row per selected gamma track
    for (const auto& kv : trackMap)
    {
        G4int trackID = kv.first;
        auto* hit1 = kv.second.firstDet1Interaction;
        auto* hit2 = kv.second.firstDet2After;

        if (!hit1 || !hit2) continue;

        G4ThreeVector dir1 = hit1->GetPreMom().unit();
        G4ThreeVector dir2 = hit2->GetPreMom().unit();
        G4double theta_mom = dir1.angle(dir2);

        G4ThreeVector dir12 = (hit2->GetPos() - hit1->GetPos()).unit();
        G4double theta_geom = dir1.angle(dir12);

        anManager->FillNtupleIColumn(1, 0, event_id);
        anManager->FillNtupleDColumn(1, 1, hit1->GetPos()[0] / mm);
        anManager->FillNtupleDColumn(1, 2, hit1->GetPos()[1] / mm);
        anManager->FillNtupleDColumn(1, 3, hit1->GetPos()[2] / mm);
        anManager->FillNtupleDColumn(1, 4, hit2->GetPos()[0] / mm);
        anManager->FillNtupleDColumn(1, 5, hit2->GetPos()[1] / mm);
        anManager->FillNtupleDColumn(1, 6, hit2->GetPos()[2] / mm);
        anManager->FillNtupleDColumn(1, 7, hit1->GetPreMom()[0] / keV);
        anManager->FillNtupleDColumn(1, 8, hit1->GetPreMom()[1] / keV);
        anManager->FillNtupleDColumn(1, 9, hit1->GetPreMom()[2] / keV);
        anManager->FillNtupleDColumn(1,10, hit1->GetPostMom()[0] / keV);
        anManager->FillNtupleDColumn(1,11, hit1->GetPostMom()[1] / keV);
        anManager->FillNtupleDColumn(1,12, hit1->GetPostMom()[2] / keV);
        anManager->FillNtupleDColumn(1,13, hit1->GetTime() / ps);
        anManager->FillNtupleDColumn(1,14, hit2->GetTime() / ps);
        anManager->FillNtupleDColumn(1,15, hit1->GetComptonAngle());
        anManager->FillNtupleDColumn(1,16, hit1->GetEnergyLost() / keV);
        anManager->FillNtupleDColumn(1,17, hit2->GetEnergyLost() / keV);
        anManager->FillNtupleDColumn(1,18, theta_mom / deg);
        anManager->FillNtupleDColumn(1,19, theta_geom / deg);
        anManager->FillNtupleDColumn(1,20, hit1->GetWeight());
        anManager->FillNtupleDColumn(1,21, hit2->GetWeight());
        anManager->AddNtupleRow(1);
    }

    // Phantom hits
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
                    anManager->FillNtupleDColumn(2, 1, (*hit_collection_phantom)[i]->GetPos()[0] / mm);
                    anManager->FillNtupleDColumn(2, 2, (*hit_collection_phantom)[i]->GetPos()[1] / mm);
                    anManager->FillNtupleDColumn(2, 3, (*hit_collection_phantom)[i]->GetPos()[2] / mm);
                    anManager->FillNtupleDColumn(2, 4, (*hit_collection_phantom)[i]->GetMom()[0] / keV);
                    anManager->FillNtupleDColumn(2, 5, (*hit_collection_phantom)[i]->GetMom()[1] / keV);
                    anManager->FillNtupleDColumn(2, 6, (*hit_collection_phantom)[i]->GetMom()[2] / keV);
                    anManager->FillNtupleDColumn(2, 7, (*hit_collection_phantom)[i]->GetEnergyLost() / keV);
                    anManager->FillNtupleIColumn(2, 8, (*hit_collection_phantom)[i]->GetParticleID());
                    anManager->AddNtupleRow(2);
                }
            }
        }
    }
}
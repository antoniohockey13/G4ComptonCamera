#include "run.hh"
#include "lgadhit.hh"


ComptCameraRunAction::ComptCameraRunAction()
{
    
    // Define here with anManager the data wanted to colect
    // Example:
    // Creates instance off singleton G4AnalysisManager
    G4AnalysisManager *anManager = G4AnalysisManager::Instance();

    // Whenever you change this lines in run.cc you should go to lgadSD.cc to make the same changes

    anManager->CreateNtuple("Hits", "Hits");
    anManager->CreateNtupleIColumn("Event");
    anManager->CreateNtupleIColumn("DetectorID");
    anManager->CreateNtupleDColumn("X");
    anManager->CreateNtupleDColumn("Y");
    anManager->CreateNtupleDColumn("Z");
    anManager->CreateNtupleDColumn("Edep");
    anManager->CreateNtupleIColumn("ParticleID");
    anManager->FinishNtuple(0);
    
}
ComptCameraRunAction::~ComptCameraRunAction() {}

void ComptCameraRunAction::BeginOfRunAction(const G4Run *run)
{
    G4AnalysisManager *anManager = G4AnalysisManager::Instance();
    G4int runNumber = run->GetRunID();
    std::string strRunID = std::to_string(runNumber);

    anManager->OpenFile("output" + strRunID + ".root");
}

void ComptCameraRunAction::EndOfRunAction(const G4Run *)
{
    G4AnalysisManager *anManager = G4AnalysisManager::Instance();
    /*
    // Get the hit_collection from lgadSD.cc
    
    // Loop in hit collection to store the data in the tuples
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
    */
    
    anManager->Write();
    anManager->CloseFile();
}

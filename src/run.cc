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
    anManager->CreateNtupleIColumn("Event"); //0
    anManager->CreateNtupleIColumn("DetectorID");
    anManager->CreateNtupleDColumn("X");
    anManager->CreateNtupleDColumn("Y");
    anManager->CreateNtupleDColumn("Z");
    anManager->CreateNtupleDColumn("Momentum"); //5
    anManager->CreateNtupleDColumn("Edep");
    anManager->CreateNtupleIColumn("ParticleID"); //7
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
    anManager->Write();
    anManager->CloseFile();
}

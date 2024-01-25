#include "run.hh"

ComptCameraRunAction::ComptCameraRunAction()
{
    /*
    // Define here with anManager the data wanted to colect
    // Example:
    // Creates instance off singleton G4AnalysisManager
    G4AnalysisManager *anManager = G4AnalysisManager::Instance();

    // Whenever you change this lines in run.cc you should go to detector.hh to make the same changes

    anManager->CreateNtuple("Hits", "Hits");
    anManager->CreateNtupleIColumn("fEvent");
    anManager->CreateNtupleDColumn("fX");
    anManager->CreateNtupleDColumn("fY");
    anManager->CreateNtupleDColumn("fZ");
    anManager->FinishNtuple(0);
    */
}
ComptCameraRunAction::~ComptCameraRunAction() {}

void ComptCameraRunAction::BeginOfRunAction(const G4Run *run)
{
    G4AnalysisManager *anManager = G4AnalysisManager::Instance();
    G4int runNumber = run->GetRunID();
    std::string strRunID = std::to_string(runNumber);

    // anManager->OpenFile("output" + strRunID + ".root");
}

void ComptCameraRunAction::EndOfRunAction(const G4Run *)
{
    //G4AnalysisManager *anManager = G4AnalysisManager::Instance();

    //anManager->Write();
    //anManager->CloseFile();
}

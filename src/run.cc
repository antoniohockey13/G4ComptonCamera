#include "run.hh"

#include "G4AnalysisManager.hh" 

ComptCameraRunAction::ComptCameraRunAction()
{
    G4AnalysisManager *anManager = G4AnalysisManager::Instance();

    anManager->CreateNtuple("PhantomHits", "Phantom Hits");
    anManager->CreateNtupleIColumn("Event"); //0
    anManager->CreateNtupleDColumn("X"); //1
    anManager->CreateNtupleDColumn("Y"); //2
    anManager->CreateNtupleDColumn("Z"); //3
    anManager->CreateNtupleDColumn("MomentumX");//4
    anManager->CreateNtupleDColumn("MomentumY"); //5
    anManager->CreateNtupleDColumn("MomentumZ"); //6
    anManager->CreateNtupleDColumn("ELost"); //7
    anManager->CreateNtupleIColumn("ParticleID"); //8
    anManager->FinishNtuple(0); 

}
ComptCameraRunAction::~ComptCameraRunAction() {}

void ComptCameraRunAction::BeginOfRunAction(const G4Run *run)
{
    G4AnalysisManager *anManager = G4AnalysisManager::Instance();
    G4int runNumber = run->GetRunID();
    std::string strRunID = std::to_string(runNumber);
    G4int thread_number = G4Threading::G4GetThreadId();
    std::string strThreadID = std::to_string(thread_number);
    anManager->OpenFile("output" + strRunID + "-" + strThreadID + ".root");
}


void ComptCameraRunAction::EndOfRunAction(const G4Run *)
{
    G4AnalysisManager *anManager = G4AnalysisManager::Instance();
    // Write and close
    anManager->Write();
    anManager->CloseFile();
}

void ComptCameraRunAction::RecordEvent(const G4Run *)
{
    // This function is not used
}

void ComptCameraRunAction::Merge(const G4Run *)
{
    // This function is not used
}
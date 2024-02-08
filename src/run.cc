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
    anManager->CreateNtupleIColumn("DetectorID"); //1
    anManager->CreateNtupleDColumn("X"); //2
    anManager->CreateNtupleDColumn("Y"); //3
    anManager->CreateNtupleDColumn("Z"); //4
    anManager->CreateNtupleDColumn("MomentumX");//5
    anManager->CreateNtupleDColumn("MomentumY"); //6
    anManager->CreateNtupleDColumn("MomentumZ"); //7
    anManager->CreateNtupleDColumn("ELost"); //8
    anManager->CreateNtupleIColumn("ParticleID"); //9
    anManager->CreateNtupleIColumn("TrackID"); //10
    anManager->CreateNtupleIColumn("ParentID"); //11
    anManager->CreateNtupleDColumn("Time"); //12
    anManager->CreateNtupleDColumn("PreKineticEnergy"); //13
    anManager->CreateNtupleDColumn("PostKineticEnergy"); //14
    anManager->CreateNtupleSColumn("ProcessName"); //15
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

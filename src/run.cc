#include "run.hh"

#include "G4AnalysisManager.hh" 

ComptCameraRunAction::ComptCameraRunAction()
{
    G4AnalysisManager *anManager = G4AnalysisManager::Instance();

    anManager->SetNtupleMerging(true);

    // Whenever you change this lines in run.cc you should go to lgadSD.cc to make the same changes
    anManager->CreateNtuple("Hits", "Hits");
    anManager->CreateNtupleIColumn("Event"); //0
    anManager->CreateNtupleIColumn("DetectorID"); //1
    anManager->CreateNtupleDColumn("X"); //2
    anManager->CreateNtupleDColumn("Y"); //3
    anManager->CreateNtupleDColumn("Z"); //4
    anManager->CreateNtupleDColumn("PreMomentumX");//5
    anManager->CreateNtupleDColumn("PreMomentumY"); //6
    anManager->CreateNtupleDColumn("PreMomentumZ"); //7
    anManager->CreateNtupleDColumn("PostMomentumX"); //8
    anManager->CreateNtupleDColumn("PostMomentumY"); //9
    anManager->CreateNtupleDColumn("PostMomentumZ"); //10
    anManager->CreateNtupleDColumn("ELost"); //11
    anManager->CreateNtupleIColumn("ParticleID"); //12
    anManager->CreateNtupleIColumn("TrackID"); //13
    anManager->CreateNtupleIColumn("ParentID"); //14
    anManager->CreateNtupleDColumn("Time"); //15
    anManager->CreateNtupleDColumn("PreKineticEnergy"); //16
    anManager->CreateNtupleDColumn("PostKineticEnergy"); //17
    anManager->CreateNtupleSColumn("ProcessName"); //18
    anManager->CreateNtupleDColumn("ComptonAngle"); //19
    anManager->CreateNtupleDColumn("StepLength"); //20
    anManager->FinishNtuple(0); 

    anManager->CreateNtuple("ComptonHits", "Compton events hits");
    anManager->CreateNtupleIColumn("Event"); //0
    anManager->CreateNtupleDColumn("X1"); //1
    anManager->CreateNtupleDColumn("Y1"); //2
    anManager->CreateNtupleDColumn("Z1"); //3
    anManager->CreateNtupleDColumn("X2"); //4
    anManager->CreateNtupleDColumn("Y2"); //5
    anManager->CreateNtupleDColumn("Z2"); //6
    anManager->CreateNtupleDColumn("PreMomentumX");//7
    anManager->CreateNtupleDColumn("PreMomentumY"); //8
    anManager->CreateNtupleDColumn("PreMomentumZ"); //9
    anManager->CreateNtupleDColumn("PostMomentumX"); //10
    anManager->CreateNtupleDColumn("PostMomentumY"); //11
    anManager->CreateNtupleDColumn("PostMomentumZ"); //12
    anManager->CreateNtupleDColumn("Time1");//13
    anManager->CreateNtupleDColumn("Time2");//14
    anManager->CreateNtupleDColumn("ComptonAngle"); //15
    anManager->FinishNtuple(1);



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
    anManager->FinishNtuple(2); 

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
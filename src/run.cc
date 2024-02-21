#include "run.hh"
#include "lgadhit.hh"
#include "phantomHit.hh"
#include "construction.hh"


ComptCameraRunAction::ComptCameraRunAction()
{
    
    // Define here with anManager the data wanted to colect
    // Example:
    // Creates instance off singleton G4AnalysisManager
    G4AnalysisManager *anManager_lgad = G4AnalysisManager::Instance();

    // Whenever you change this lines in run.cc you should go to lgadSD.cc to make the same changes

    anManager_lgad->CreateNtuple("Hits LGAD", "Hits LGAD");
    anManager_lgad->CreateNtupleIColumn("Event"); //0
    anManager_lgad->CreateNtupleIColumn("DetectorID"); //1
    anManager_lgad->CreateNtupleDColumn("X"); //2
    anManager_lgad->CreateNtupleDColumn("Y"); //3
    anManager_lgad->CreateNtupleDColumn("Z"); //4
    anManager_lgad->CreateNtupleDColumn("PreMomentumX");//5
    anManager_lgad->CreateNtupleDColumn("PreMomentumY"); //6
    anManager_lgad->CreateNtupleDColumn("PreMomentumZ"); //7
    anManager_lgad->CreateNtupleDColumn("PostMomentumX"); //8
    anManager_lgad->CreateNtupleDColumn("PostMomentumY"); //9
    anManager_lgad->CreateNtupleDColumn("PostMomentumZ"); //10
    anManager_lgad->CreateNtupleDColumn("ELost"); //11
    anManager_lgad->CreateNtupleIColumn("ParticleID"); //12
    anManager_lgad->CreateNtupleIColumn("TrackID"); //13
    anManager_lgad->CreateNtupleIColumn("ParentID"); //14
    anManager_lgad->CreateNtupleDColumn("Time"); //15
    anManager_lgad->CreateNtupleDColumn("PreKineticEnergy"); //16
    anManager_lgad->CreateNtupleDColumn("PostKineticEnergy"); //17
    anManager_lgad->CreateNtupleSColumn("ProcessName"); //18
    anManager_lgad->CreateNtupleDColumn("ComptonAngle"); //19
    anManager_lgad->CreateNtupleDColumn("StepLength"); //20
    anManager_lgad->FinishNtuple(0); 

    ComptCameraDetectorConstruction detectorConstruction;
    if (detectorConstruction.GetPhantomDetector())
    {
        G4AnalysisManager *anManager_phantom = G4AnalysisManager::Instance();
        anManager_phantom->CreateNtuple("Hits phantom", "Hits phantom");
        anManager_phantom->CreateNtupleIColumn("Event"); //0
        anManager_phantom->CreateNtupleIColumn("DetectorID"); //1
        anManager_phantom->CreateNtupleDColumn("X"); //2
        anManager_phantom->CreateNtupleDColumn("Y"); //3
        anManager_phantom->CreateNtupleDColumn("Z"); //4
        anManager_phantom->CreateNtupleDColumn("PreMomentumX");//5
        anManager_phantom->CreateNtupleDColumn("PreMomentumY"); //6
        anManager_phantom->CreateNtupleDColumn("PreMomentumZ"); //7
        anManager_phantom->CreateNtupleDColumn("PostMomentumX"); //8
        anManager_phantom->CreateNtupleDColumn("PostMomentumY"); //9
        anManager_phantom->CreateNtupleDColumn("PostMomentumZ"); //10
        anManager_phantom->CreateNtupleDColumn("ELost"); //11
        anManager_phantom->CreateNtupleIColumn("ParticleID"); //12
        anManager_phantom->CreateNtupleIColumn("TrackID"); //13
        anManager_phantom->CreateNtupleIColumn("ParentID"); //14
        anManager_phantom->CreateNtupleDColumn("Time"); //15
        anManager_phantom->CreateNtupleDColumn("PreKineticEnergy"); //16
        anManager_phantom->CreateNtupleDColumn("PostKineticEnergy"); //17
        anManager_phantom->CreateNtupleSColumn("ProcessName"); //18
        anManager_phantom->CreateNtupleDColumn("ComptonAngle"); //19
        anManager_phantom->CreateNtupleDColumn("StepLength"); //20
        anManager_phantom->FinishNtuple(1);
    }
    
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

#include "run.hh"
#include "G4RunManager.hh"
#include "G4RandomTools.hh"
#include "lgadhit.hh"
#include "phantomHit.hh"
#include "construction.hh"


ComptCameraRunAction::ComptCameraRunAction()
{
    // Set seed
    //G4RandomTools::setSeed(12345);
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


}
ComptCameraRunAction::~ComptCameraRunAction() {}

void ComptCameraRunAction::BeginOfRunAction(const G4Run *run)
{
    G4AnalysisManager *anManager = G4AnalysisManager::Instance();
    G4int runNumber = run->GetRunID();
    std::string strRunID = std::to_string(runNumber);

    // Open separate files for lgadHits and phantomHits
    anManager->OpenFile("output" + strRunID + ".root");
}


void ComptCameraRunAction::EndOfRunAction(const G4Run *)
{
    G4AnalysisManager *anManager = G4AnalysisManager::Instance();

    // Write and close the lgadHits Ntuple


    // Write and close the phantomHits Ntuple
    anManager->Write();
    anManager->CloseFile();
}
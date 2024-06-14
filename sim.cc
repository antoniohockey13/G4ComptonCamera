#include <iostream>

#include "G4RunManagerFactory.hh"
#include "G4Threading.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
//Detectores
#include "construction.hh" 
//Interacciones
#include "physics.hh" 
//Partículas
#include "action.hh" 


int main(int argc, char** argv)
{
    auto* runManager = G4RunManagerFactory::CreateRunManager();
    G4int nThreads = std::max(1,G4Threading::G4GetNumberOfCores()-2);
    runManager->SetNumberOfThreads(nThreads);

    auto * detector = new ComptCameraDetectorConstruction();
    runManager->SetUserInitialization(detector);
    runManager->SetUserInitialization(new ComptCameraPhysicsList());
    runManager->SetUserInitialization(new ComptCameraActionInitialization(detector->IsPhantomDetector()));
    
    runManager->Initialize();	
    
    // Initialization of the visualization manager 
    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    //Get teh user interface pointer manager
    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    if(argc == 1)
    {
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
	UImanager->ApplyCommand("/control/execute vis.mac");
	ui->SessionStart();
        delete ui;
    }
    else
    {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }

    delete visManager;
    delete runManager;

    return 0;
}


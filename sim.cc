#include <iostream>

#include "G4MTRunManager.hh"
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
	G4MTRunManager *runManager = new G4MTRunManager();
	
	runManager->SetNumberOfThreads(3);
	runManager->SetUserInitialization(new ComptCameraDetectorConstruction());
	runManager->SetUserInitialization(new ComptCameraPhysicsList());
	runManager->SetUserInitialization(new ComptCameraActionInitialization());
		
	runManager->Initialize();	

	//Solo crear modelo gráfico si no se le pasa ningun comando
	G4UIExecutive *ui = 0;

	if(argc == 1)
	{
		ui = new G4UIExecutive(argc, argv);
	}


	G4VisManager *visManager = new G4VisExecutive();
	visManager->Initialize();
	
	G4UImanager *UImanager = G4UImanager::GetUIpointer();
	if(ui)
	{
		UImanager->ApplyCommand("/control/execute vis.mac");
		ui->SessionStart();
	}
	else 
	{
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command+fileName);
	}
    return 0;
}


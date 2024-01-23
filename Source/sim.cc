#include <iostream>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "construction.hh" //Detectores
#include "physics.hh" //Interacciones
// #include "action.hh" //Partículas


int main(int argc, char** argv)
{
	G4RunManager *runManager = new G4RunManager();
	
	runManager->SetUserInitialization(new MyDetectorConstruction());
	runManager->SetUserInitialization(new MyPhysicsList());
	runManager->SetUserInitialization(new MyActionInitialization());
	
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

		// No ejecutar si está vis.mac
		UImanager -> ApplyCommand("/vis/open OGL"); //Abre visualizador gráfico
		UImanager -> ApplyCommand("/vis/viewer/set/viewpointVector 1 1 1"); //Cambia punto de vista inicial
		UImanager -> ApplyCommand("/vis/drawVolume"); //Dibuja el volumen
		UImanager -> ApplyCommand("/vis/viewer/set/autoRefresh true"); //Actualiza
		UImanager -> ApplyCommand("/vis/scene/add/trajectories smooth"); //Dibuja las partículas
		UImanager -> ApplyCommand("/vis/scene/endOfEventAction accumulate"); //Muestra todos sucesos a la vez
		
        //Solo si está vis.mac
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

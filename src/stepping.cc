#include "stepping.hh"

//Copy from B4a example

ComptCameraSteppingAction::ComptCameraSteppingAction(ComptCameraEventAction*)
{
}

// TO DO: implement this function
void ComptCameraSteppingAction::UserSteppingAction(const G4Step* step)
{
    G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    const ComptCameraDetectorConstruction *detectorConstruction = static_cast<const ComptCameraDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
}

ComptCameraSteppingAction::~ComptCameraSteppingAction()
{
}




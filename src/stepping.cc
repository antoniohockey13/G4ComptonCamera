#include "stepping.hh"

//Copy from B4a example

ComptCameraSteppingAction::ComptCameraSteppingAction(ComptCameraEventAction*)
{
}

// TO DO: implement this function
void ComptCameraSteppingAction::UserSteppingAction(const G4Step* step)
{
    const ComptCameraDetectorConstruction *detectorConstruction = static_cast<const ComptCameraDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
}

ComptCameraSteppingAction::~ComptCameraSteppingAction()
{
}
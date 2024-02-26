#include "stepping.hh"

#include "construction.hh"
#include "event.hh"

ComptCameraSteppingAction::ComptCameraSteppingAction(ComptCameraEventAction*)
{
}

void ComptCameraSteppingAction::UserSteppingAction(const G4Step* /*step*/)
{
   // const ComptCameraDetectorConstruction *detectorConstruction = static_cast<const ComptCameraDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
}

ComptCameraSteppingAction::~ComptCameraSteppingAction()
{
}
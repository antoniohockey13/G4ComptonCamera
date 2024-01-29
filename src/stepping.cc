#include "stepping.hh"

ComptCameraSteppingAction::ComptCameraSteppingAction(ComptCameraEventAction *event_action)
{
    _f_event_action = event_action;
}

ComptCameraSteppingAction::~ComptCameraSteppingAction()
{
}

void ComptCameraSteppingAction::UserSteppingAction(const G4Step *step)
{
    G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    const ComptCameraDetectorConstruction *detector_construction = static_cast<const ComptCameraDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    G4LogicalVolume *f_scoring_volume1 = detector_construction->GetScoringVolume1();
    G4LogicalVolume *f_scoring_volume2 = detector_construction->GetScoringVolume2();

    if (volume != f_scoring_volume1 && volume != f_scoring_volume2)
    {
        return;
    }
}


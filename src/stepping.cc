#include "stepping.hh"
/*
ComptCameraSteppingAction::ComptCameraSteppingAction(ComptCameraEventAction *event_action)
{
    _f_event_action = event_action;
}

ComptCameraSteppingAction::~ComptCameraSteppingAction()
{
}

void ComptCameraEventAction::UserSteppingAction(const G4Step *step)
{
    G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    const ComptCameraDetectorConstruction *detector_construction = static_cast<const ComptCameraDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    G4LogicalVolume *f_scoring_volume = detectorConstruction->GetScoringVolume();

    if (volume != f_scoring_volume)
    {
        return;
    }
    G4double _e_dep = step->GetTotalEnergyDeposit();
    fEventAction->AddEdep(_e_dep);
}
*/
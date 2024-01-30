#include "stepping.hh"

//Copy from B4a example

ComptCameraSteppingAction::ComptCameraSteppingAction(const ComptCameraDetectorConstruction* detConstruction, ComptCameraEventAction* eventAction)
    : _det_construction(detConstruction), _event_action(eventAction)
{

}

// TO DO: implement this function
void ComptCameraSteppingAction::UserSteppingAction(const G4Step* step)
{

}

ComptCameraSteppingAction::~ComptCameraSteppingAction()
{
}




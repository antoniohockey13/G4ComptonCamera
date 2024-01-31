#ifndef  STEPPING_HH
#define  STEPPING_HH

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "construction.hh"
#include "event.hh"

class ComptCameraDetectorConstruction;

class ComptCameraEventAction;

class ComptCameraSteppingAction : public G4UserSteppingAction
{
    public:
        ComptCameraSteppingAction(ComptCameraEventAction* eventAction);
        ~ComptCameraSteppingAction();

        virtual void UserSteppingAction(const G4Step* step);
};
#endif 

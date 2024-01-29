#ifndef  STEPPING_HH
#define  STEPPING_HH

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "construction.hh"
#include "event.hh"

class ComptCameraSteppingAction : public G4UserSteppingAction
{
    public:
        ComptCameraSteppingAction(ComptCameraEventAction *event_action);
        ~ComptCameraSteppingAction();

        virtual void UserSteppingAction(const G4Step *);
    private:
        ComptCameraEventAction *_f_event_action;
};
#endif 

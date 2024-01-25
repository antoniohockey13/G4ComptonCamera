/*
#ifndef  STEPPING_HH
#define  STEPPING_HH

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "construction.hh"
#include "event.hh"

class ComptCameraEventAction : public G4UserSteppingAction
{
    public:
        ComptCameraEventAction(ComptCameraRunAction *event_action);
        ~ComptCameraEventAction();

        virtual void UserSteppingAction(const G4Step *);
    private:
        ComptCameraRunAction *_f_event_action;
};
#endif 
*/
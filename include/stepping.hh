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
        ComptCameraSteppingAction(const ComptCameraDetectorConstruction* detConstruction, ComptCameraEventAction* eventAction);
        ~ComptCameraSteppingAction() override = default;

        virtual void UserSteppingAction(const G4Step step) override;
    private:
        const ComptCameraDetectorConstruction* _det_construction = nullptr;
        ComptCameraEventAction* _event_action = nullptr;
};
#endif 

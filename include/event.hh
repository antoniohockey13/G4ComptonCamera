#ifndef EVENT_HH
#define EVENT_HH

#include "run.hh"

#include "G4UserEventAction.hh"


class ComptCameraEventAction : public G4UserEventAction
{
    public:
        ComptCameraEventAction(bool is_phantom);
        ComptCameraEventAction() = delete;
        ~ComptCameraEventAction();
        
        virtual void BeginOfEventAction(const G4Event *) override;
        virtual void EndOfEventAction(const G4Event *) override;

    private:
        bool _is_phantom;
};

#endif

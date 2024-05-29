#ifndef EVENT_HH
#define EVENT_HH

#include "run.hh"

#include "G4UserEventAction.hh"


class ComptCameraEventAction : public G4UserEventAction
{
public:
    ComptCameraEventAction(ComptCameraRunAction *);
    ~ComptCameraEventAction();

    virtual void BeginOfEventAction(const G4Event *) override;
    virtual void EndOfEventAction(const G4Event *) override;
};

#endif

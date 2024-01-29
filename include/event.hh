#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "run.hh"

class ComptCameraEventAction : public G4UserEventAction
{
public:
    ComptCameraEventAction(ComptCameraRunAction *);
    ~ComptCameraEventAction();

    virtual void BeginOfEventAction(const G4Event *) override;
    virtual void EndOfEventAction(const G4Event *) override;
};

#endif

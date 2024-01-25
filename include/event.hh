#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "run.hh"

class ComptCameraEventAction : public G4UserEventAction
{
public:
    ComptCameraEventAction(ComptCameraRunAction *);
    ~ComptCameraEventAction();

    virtual void BeginOfEventAction(const G4Event *);
    virtual void EndOfEventAction(const G4Event *);

    void AddEdep(G4double e_dep) { _f_edep += e_dep; }

private:
    G4double _f_edep;
};

#endif

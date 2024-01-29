#include "event.hh"

ComptCameraEventAction::ComptCameraEventAction(ComptCameraRunAction *)
{
}
ComptCameraEventAction::~ComptCameraEventAction()
{}

void ComptCameraEventAction::BeginOfEventAction(const G4Event *)
{
}

void ComptCameraEventAction::EndOfEventAction(const G4Event* event)
{
    //get event ID
    G4int event_id = event->GetEventID();
    G4cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << G4endl;
    G4cout << "Event ID: " << event_id << G4endl;
    G4cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << G4endl;
}

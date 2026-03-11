#include "stepping.hh"

#include "construction.hh"
#include "event.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"

ComptCameraSteppingAction::ComptCameraSteppingAction(ComptCameraEventAction*)
{
}

ComptCameraSteppingAction::~ComptCameraSteppingAction()
{
}

void ComptCameraSteppingAction::UserSteppingAction(const G4Step* step)
{
    auto* track = step->GetTrack();

    auto* event = G4RunManager::GetRunManager()->GetCurrentEvent();
    G4int eventID = event ? event->GetEventID() : -1;

    // limitar debug
    if (eventID >= 10) return;

    auto* pre  = step->GetPreStepPoint();
    auto* post = step->GetPostStepPoint();

    G4String particle = track->GetDefinition()->GetParticleName();

    G4int trackID  = track->GetTrackID();
    G4int parentID = track->GetParentID();

    G4String creator = "primary";
    if (track->GetCreatorProcess())
        creator = track->GetCreatorProcess()->GetProcessName();

    G4String stepProcess = "none";
    if (post->GetProcessDefinedStep())
        stepProcess = post->GetProcessDefinedStep()->GetProcessName();

    G4String preVol  = pre->GetTouchableHandle()->GetVolume()
                     ? pre->GetTouchableHandle()->GetVolume()->GetName() : "None";

    G4String postVol = post->GetTouchableHandle()->GetVolume()
                     ? post->GetTouchableHandle()->GetVolume()->GetName() : "None";


    // ============================
    // NUEVO TRACK
    // ============================

    if (track->GetCurrentStepNumber() == 1)
    {
        G4cout
        << "\n=============================="
        << "\n[NEW TRACK]"
        << "\n Event=" << eventID
        << " TrackID=" << trackID
        << " ParentID=" << parentID
        << " Particle=" << particle
        << " Creator=" << creator
        << "\n StartPos=("
        << track->GetPosition().x()/mm << ", "
        << track->GetPosition().y()/mm << ", "
        << track->GetPosition().z()/mm << ") mm"
        << "\n StartMom=("
        << track->GetMomentum().x()/keV << ", "
        << track->GetMomentum().y()/keV << ", "
        << track->GetMomentum().z()/keV << ") keV"
        << "\n StartEkin=" << track->GetKineticEnergy()/keV << " keV"
        << "\n=============================="
        << G4endl;
    }


    // ============================
    // STEPS
    // ============================

    G4cout
    << "\n[STEP]"
    << " Event=" << eventID
    << " TrackID=" << trackID
    << " ParentID=" << parentID
    << " Particle=" << particle
    << "\n PreVol=" << preVol
    << " PostVol=" << postVol
    << "\n PrePos=("
    << pre->GetPosition().x()/mm << ", "
    << pre->GetPosition().y()/mm << ", "
    << pre->GetPosition().z()/mm << ") mm"
    << "\n PostPos=("
    << post->GetPosition().x()/mm << ", "
    << post->GetPosition().y()/mm << ", "
    << post->GetPosition().z()/mm << ") mm"
    << "\n PreMom=("
    << pre->GetMomentum().x()/keV << ", "
    << pre->GetMomentum().y()/keV << ", "
    << pre->GetMomentum().z()/keV << ") keV"
    << "\n PostMom=("
    << post->GetMomentum().x()/keV << ", "
    << post->GetMomentum().y()/keV << ", "
    << post->GetMomentum().z()/keV << ") keV"
    << "\n PreEkin=" << pre->GetKineticEnergy()/keV
    << " keV PostEkin=" << post->GetKineticEnergy()/keV
    << " keV Edep=" << step->GetTotalEnergyDeposit()/keV
    << " keV"
    << "\n StepLength=" << step->GetStepLength()/mm
    << " mm Process=" << stepProcess
    << G4endl;


    // ============================
    // SECUNDARIOS GENERADOS
    // ============================

    const auto* secondaries = step->GetSecondaryInCurrentStep();

    if (secondaries && !secondaries->empty())
    {
        G4cout
        << "\n[SECONDARIES CREATED]"
        << " Event=" << eventID
        << " ParentTrackID=" << trackID
        << " ParentParticle=" << particle
        << G4endl;

        for (const auto* sec : *secondaries)
        {
            G4String secCreator = "unknown";
            if (sec->GetCreatorProcess())
                secCreator = sec->GetCreatorProcess()->GetProcessName();

            G4cout
            << "  -> TrackID=" << sec->GetTrackID()
            << " ParentID=" << sec->GetParentID()
            << " Particle=" << sec->GetDefinition()->GetParticleName()
            << " Creator=" << secCreator
            << "\n     Pos=("
            << sec->GetPosition().x()/mm << ", "
            << sec->GetPosition().y()/mm << ", "
            << sec->GetPosition().z()/mm << ") mm"
            << "  Ekin=" << sec->GetKineticEnergy()/keV
            << " keV"
            << G4endl;
        }
    }


    // ============================
    // FIN DEL TRACK
    // ============================

    if (track->GetTrackStatus() == fStopAndKill)
    {
        G4cout
        << "\n[TRACK END]"
        << " Event=" << eventID
        << " TrackID=" << trackID
        << " Particle=" << particle
        << "\n FinalPos=("
        << post->GetPosition().x()/mm << ", "
        << post->GetPosition().y()/mm << ", "
        << post->GetPosition().z()/mm << ") mm"
        << "\n FinalEkin=" << post->GetKineticEnergy()/keV
        << " keV"
        << "\n FinalVolume=" << postVol
        << "\n=============================="
        << G4endl;
    }

}
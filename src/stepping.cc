#include "stepping.hh"
#include "event.hh"
#include "GammaTrackInfo.hh"

#include "G4Gamma.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4StackManager.hh"
#include "G4VProcess.hh"
#include "G4StepStatus.hh"


#include "construction.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

ComptCameraSteppingAction::ComptCameraSteppingAction(ComptCameraEventAction* eventAction)
: fEventAction(eventAction)
{
}

ComptCameraSteppingAction::~ComptCameraSteppingAction()
{
}

void ComptCameraSteppingAction::UserSteppingAction(const G4Step* step)
{
    if (!fEnableSplitting) return;

    G4Track* track = step->GetTrack();
    if (!track) return;
    if (track->GetDefinition() != G4Gamma::Definition()) return;

    auto* pre  = step->GetPreStepPoint();
    auto* post = step->GetPostStepPoint();
    if (!pre || !post) return;
    if (!pre->GetPhysicalVolume() || !post->GetPhysicalVolume()) return;

    auto* info = dynamic_cast<GammaTrackInfo*>(track->GetUserInformation());
    if (!info)
    {
        info = new GammaTrackInfo(track->GetTrackID());
        track->SetUserInformation(info);
    }

    const G4String preVolName  = pre->GetPhysicalVolume()->GetName();
    const G4String postVolName = post->GetPhysicalVolume()->GetName();

    const G4VProcess* proc = post->GetProcessDefinedStep();
    const G4String procName = proc ? proc->GetProcessName() : "";

    if (preVolName.find("detector_1_pixel_") != std::string::npos)
    {
        if (procName == "compt" && post->GetKineticEnergy() > 0.)
        {
            info->SetHasComptonInK3(true);
        }
    }

    if (!info->GetHasComptonInK3()) return;


    const auto* det =
        static_cast<const ComptCameraDetectorConstruction*>(
            G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    const G4double xDiag = det->GetModule2FrontFaceX() - 1.0*CLHEP::um;

    const G4double x1 = pre->GetPosition().x();
    const G4double x2 = post->GetPosition().x();

    // solo fotones que avanzan hacia +x y cruzan el plano
    if (!info->GetCrossedK2Plane() && x1 < xDiag && x2 >= xDiag)
    {
        info->SetCrossedK2Plane(true);

        G4ThreeVector p1 = pre->GetPosition();
        G4ThreeVector p2 = post->GetPosition();

        G4double frac = (xDiag - x1) / (x2 - x1);
        G4ThreeVector crossPos = p1 + frac * (p2 - p1);

        G4ThreeVector dir = pre->GetMomentumDirection();

        G4AnalysisManager* anManager = G4AnalysisManager::Instance();

        G4int eventID = -1;
        if (fEventAction) eventID = fEventAction->GetCurrentEventID();

        anManager->FillNtupleIColumn(3, 0, eventID);
        anManager->FillNtupleIColumn(3, 1, track->GetTrackID());
        anManager->FillNtupleIColumn(3, 2, info->GetHistoryID());
        anManager->FillNtupleDColumn(3, 3, track->GetWeight());
        anManager->FillNtupleDColumn(3, 4, crossPos.x()/mm);
        anManager->FillNtupleDColumn(3, 5, crossPos.y()/mm);
        anManager->FillNtupleDColumn(3, 6, crossPos.z()/mm);
        anManager->FillNtupleDColumn(3, 7, dir.x());
        anManager->FillNtupleDColumn(3, 8, dir.y());
        anManager->FillNtupleDColumn(3, 9, dir.z());
        anManager->FillNtupleDColumn(3,10, pre->GetKineticEnergy()/keV);
        anManager->AddNtupleRow(3);
    }

}
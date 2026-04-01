// From example GB01
#include "ChangeCrossSection.hh"

#include "G4BiasingProcessInterface.hh"
#include "G4BOptnChangeCrossSection.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"
#include "G4Track.hh"
#include "G4ProcessManager.hh"
#include "G4BiasingProcessSharedData.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ios.hh"

#include <cfloat>

ChangeCrossSection::ChangeCrossSection(G4String name)
: G4VBiasingOperator(name),
  _compton_operation(nullptr),
  _setup(true),
  _compton_wrapper_found(false),
  _particle_to_bias(nullptr),
  _xs_factor(10.0),
  _bias_only_primary_gamma(true),
  _biased_volume_prefix("detector_1_pixel_")
{
    _particle_to_bias = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
}

ChangeCrossSection::~ChangeCrossSection()
{
    delete _compton_operation;
}

G4bool ChangeCrossSection::IsComptonWrappedProcess(const G4String& procName) const
{
    return (procName == "compt" ||
            procName == "Compton" ||
            procName == "compton" ||
            procName == "G4ComptonScattering");
}

G4bool ChangeCrossSection::IsInBiasedVolume(const G4Track* track) const
{
    if (!track) return false;
    if (!track->GetVolume()) return false;

    const G4String volName = track->GetVolume()->GetName();
    return (volName.find(_biased_volume_prefix) != std::string::npos);
}

G4bool ChangeCrossSection::IsEligibleTrack(const G4Track* track) const
{
    if (!track) return false;
    if (track->GetDefinition() != _particle_to_bias) return false;
    if (_bias_only_primary_gamma && track->GetParentID() != 0) return false;
    if (!IsInBiasedVolume(track)) return false;
    if (track->GetKineticEnergy() <= 0.) return false;
    return true;
}
void ChangeCrossSection::StartRun()
{
    if (!_setup) return;
    if (_particle_to_bias == nullptr)
    {
        G4cout << "[BIAS] Gamma particle definition not found. Biasing disabled." << G4endl;
        _setup = false;
        return;
    }
    const G4ProcessManager* processManager = _particle_to_bias->GetProcessManager();
    const G4BiasingProcessSharedData* sharedData =
        G4BiasingProcessInterface::GetSharedData(processManager);
        
    if (!sharedData)
    {
        G4cout << "[BIAS] No shared biasing data found for gamma. Biasing disabled." << G4endl;
        _setup = false;
        return;
    }

    G4cout << "[BIAS] Wrapped gamma processes available:" << G4endl;
    for (size_t i = 0; i < sharedData->GetPhysicsBiasingProcessInterfaces().size(); ++i)
    {
        const G4BiasingProcessInterface* wrapperProcess =
            sharedData->GetPhysicsBiasingProcessInterfaces()[i];
        const G4String procName = wrapperProcess->GetWrappedProcess()->GetProcessName();
        G4cout << "        - " << procName << G4endl;

        if (IsComptonWrappedProcess(procName))
        {
            _compton_wrapper_found = true;
            if (_compton_operation == nullptr)
            {
                _compton_operation =
                    new G4BOptnChangeCrossSection("XSchange-ComptonOnly");
            }
        }
    }

    if (!_compton_wrapper_found)
    {
        G4cout
            << "[BIAS] No standalone Compton wrapper found. "
            << "Compton-only biasing disabled to avoid biasing GammaGeneralProc."
            << G4endl;
    }
    else
    {
        G4cout
            << "[BIAS] Compton-only biasing enabled with factor "
            << _xs_factor
            << " in volumes matching prefix '"
            << _biased_volume_prefix
            << "'."
            << G4endl;
    }
    _setup = false;
}

G4VBiasingOperation* 
ChangeCrossSection::ProposeOccurenceBiasingOperation(const G4Track* track, 
                                                    const G4BiasingProcessInterface* callingProcess)
{
    if (_setup) StartRun();
    // -- Check if current particle type is the one to bias:
    if (!_compton_wrapper_found) return nullptr;
    if (_compton_operation == nullptr) return nullptr;
    if (!callingProcess) return nullptr;
    if (!IsEligibleTrack(track)) return nullptr;

    // -- select and setup the biasing operation for current callingProcess:
    const G4String procName = callingProcess->GetWrappedProcess()->GetProcessName();
    if (!IsComptonWrappedProcess(procName)) return nullptr;

    // -- Check if the analog cross-section well defined
    // -- process for a gamma below e+e- creation threshold has an DBL_MAX interaction
    // -- length. Nothing is done in this case (ie, let analog process to deal with the case)
    const G4double analogInteractionLength =
        callingProcess->GetWrappedProcess()->GetCurrentInteractionLength();

    if (analogInteractionLength <= 0.) return nullptr;
    if (analogInteractionLength > DBL_MAX / 10.) return nullptr;

    // -- Analog cross-section is well-defined:
    const G4double analogXS = 1.0 / analogInteractionLength;
    // -- Choose a constant cross-section bias. But at this level, this factor can be made
    // -- direction dependent, like in the exponential transform MCNP case, or it
    // -- can be chosen differently, depending on the process, etc.
    // -- fetch the operation associated to this callingProcess:
    // -- get the operation that was proposed to the process in the previous step:
    G4VBiasingOperation* previousOperation =
        callingProcess->GetPreviousOccurenceBiasingOperation();

    // -- now setup the operation to be returned to the process: this
    // -- consists in setting the biased cross-section, and in asking
    // -- the operation to sample its exponential interaction law.
    // -- To do this, to first order, the two lines:
    //        operation->SetBiasedCrossSection( XStransformation * analogXS );
    //        operation->Sample();
    // -- are correct and sufficient.
    // -- But, to avoid having to shoot a random number each time, we sample
    // -- only on the first time the operation is proposed, or if the interaction
    // -- occured. If the interaction did not occur for the process in the previous,
    // -- we update the number of interaction length instead of resampling.
    if (previousOperation == nullptr || _compton_operation->GetInteractionOccured())
    {
        _compton_operation->SetBiasedCrossSection(_xs_factor * analogXS);
        _compton_operation->Sample();
    }
    else
    {
        _compton_operation->UpdateForStep(callingProcess->GetPreviousStepSize());
        _compton_operation->SetBiasedCrossSection(_xs_factor * analogXS);
        _compton_operation->UpdateForStep(0.0);
    }

    return _compton_operation;
}

void ChangeCrossSection::OperationApplied(const G4BiasingProcessInterface* callingProcess,
                                          G4BiasingAppliedCase,
                                          G4VBiasingOperation* occurenceOperationApplied,
                                          G4double,
                                          G4VBiasingOperation*,
                                          const G4VParticleChange*)
{
    if (!_compton_wrapper_found) return;
    if (_compton_operation == nullptr) return;
    if (!callingProcess) return;

    const G4String procName = callingProcess->GetWrappedProcess()->GetProcessName();
    if (!IsComptonWrappedProcess(procName)) return;

    if (_compton_operation == occurenceOperationApplied)
    {
        _compton_operation->SetInteractionOccured();
    }
}
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

#include <cfloat>

ChangeCrossSection::ChangeCrossSection(G4String name)
: G4VBiasingOperator(name),
  _gamma_operation(nullptr),
  _setup(true),
  _particle_to_bias(nullptr),
  _xs_factor(100.0)
{
    _particle_to_bias = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
}

ChangeCrossSection::~ChangeCrossSection()
{
    delete _gamma_operation;
}

void ChangeCrossSection::StartRun()
{
    if (!_setup) return;
    const G4ProcessManager* processManager = _particle_to_bias->GetProcessManager();
    const G4BiasingProcessSharedData* sharedData =
        G4BiasingProcessInterface::GetSharedData(processManager);
        
    if (!sharedData)
    {
        _setup = false;
        return;
    }

    for (size_t i = 0; i < sharedData->GetPhysicsBiasingProcessInterfaces().size(); ++i)
    {
        const G4BiasingProcessInterface* wrapperProcess =
            sharedData->GetPhysicsBiasingProcessInterfaces()[i];
        const G4String procName = wrapperProcess->GetWrappedProcess()->GetProcessName();

        if (procName == "GammaGeneralProc")
        {
            if (_gamma_operation == nullptr)
            {
                _gamma_operation = new G4BOptnChangeCrossSection("XSchange-GammaGeneralProc");
            }
        }
    }

    _setup = false;
}

G4VBiasingOperation* 
ChangeCrossSection::ProposeOccurenceBiasingOperation(const G4Track* track, 
                                                    const G4BiasingProcessInterface* callingProcess)
{
    if (_setup || _gamma_operation == nullptr)
    {
        StartRun();
    }

    if (_gamma_operation == nullptr)
    {
        _gamma_operation = new G4BOptnChangeCrossSection("XSchange-GammaGeneralProc");
    }

    static int nEntryPrint = 0;
    if (nEntryPrint < 30)
    {
        G4String volName = "NULL";
        if (track->GetVolume())
            volName = track->GetVolume()->GetName();

        ++nEntryPrint;
    }
    // -- Check if current particle type is the one to bias:
    if (track->GetDefinition() != _particle_to_bias) return nullptr;

    // -- select and setup the biasing operation for current callingProcess:
    const G4String procName = callingProcess->GetWrappedProcess()->GetProcessName();
    if (procName != "GammaGeneralProc") return nullptr;

    if (_gamma_operation == nullptr)
    {
        G4cout << "[BIAS-EXIT] _gamma_operation is null" << G4endl;
        return nullptr;
    }
    // -- Check if the analog cross-section well defined
    // -- process for a gamma below e+e- creation threshold has an DBL_MAX interaction
    // -- length. Nothing is done in this case (ie, let analog process to deal with the case)
    G4double analogInteractionLength =
        callingProcess->GetWrappedProcess()->GetCurrentInteractionLength();

    if (analogInteractionLength > DBL_MAX / 10.)
    {
        G4cout << "[BIAS-EXIT] analogInteractionLength too large = "
               << analogInteractionLength << G4endl;
        return nullptr;
    }


    // -- Analog cross-section is well-defined:
    G4double analogXS = 1.0 / analogInteractionLength;
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
    if (previousOperation == nullptr || _gamma_operation->GetInteractionOccured())
    {
        _gamma_operation->SetBiasedCrossSection(_xs_factor * analogXS);
        _gamma_operation->Sample();
    }
    else
    {
        _gamma_operation->SetBiasedCrossSection(_xs_factor * analogXS);
        // if (_gamma_operation->GetInteractionOccured())
        // {
        //     _gamma_operation->SetBiasedCrossSection(_xs_factor * analogXS);
        //     _gamma_operation->Sample();
        // }
        // else
        // {
        //     // -- update the 'interaction length' and underneath 'number of interaction lengths'
        //     // -- for past step  (this takes into accout the previous step cross-section value)
        //     _gamma_operation->UpdateForStep(callingProcess->GetPreviousStepSize());
        //     // -- update the cross-section value:
        //     _gamma_operation->SetBiasedCrossSection(_xs_factor * analogXS);
        //     // -- forces recomputation of the 'interaction length' taking into account above
        //     // -- new cross-section value [tricky : to be improved]
        //     _gamma_operation->UpdateForStep(0.0);
        // }
    }

    return _gamma_operation;
}


void ChangeCrossSection::OperationApplied(const G4BiasingProcessInterface* callingProcess,
                                          G4BiasingAppliedCase,
                                          G4VBiasingOperation* occurenceOperationApplied,
                                          G4double weightForOccurenceInteraction,
                                          G4VBiasingOperation*,
                                          const G4VParticleChange*)
{
    if (_gamma_operation == nullptr) return;

    const G4String procName = callingProcess->GetWrappedProcess()->GetProcessName();
    if (procName != "GammaGeneralProc") return;

    if (_gamma_operation == occurenceOperationApplied)
    {
        _gamma_operation->SetInteractionOccured();
    }
}

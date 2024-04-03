// From example GB01
#include "change_cross_section.hh"
#include "G4BiasingProcessInterface.hh"
#include "G4BOptnChangeCrossSection.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4VProcess.hh"

#include "Randomize.hh"
#include "G4InteractionLawPhysical.hh"


ChangeCrossSection::ChangeCrossSection(G4String name)
    : G4VBiasingOperator(name),
    _setup(true)
{
  _particle_to_bias = G4ParticleTable::GetParticleTable()->FindParticle("photon");
}


ChangeCrossSection::~ChangeCrossSection()
{
    for ( std::map< const G4BiasingProcessInterface*, G4BOptnChangeCrossSection* >::iterator 
        it = _change_cross_section_operations.begin();
        it != _change_cross_section_operations.end();
        it++ ) delete (*it).second;
}


void ChangeCrossSection::StartRun()
{
    if (_setup)
    {
        const G4ProcessManager* processManager = _particle_to_bias->GetProcessManager();
        const G4BiasingProcessSharedData* sharedData =
            G4BiasingProcessInterface::GetSharedData( processManager );
        
        if (sharedData) // -- sharedData tested, as is can happen a user attaches an operator to a
                          // -- volume but without defined BiasingProcessInterface processes.
        {
            for ( size_t i = 0 ; i < (sharedData->GetPhysicsBiasingProcessInterfaces()).size(); i++ )
            {
                const G4BiasingProcessInterface* wrapperProcess =
                (sharedData->GetPhysicsBiasingProcessInterfaces())[i];
                
                G4String operationName = "XSchange-" + 
                wrapperProcess->GetWrappedProcess()->GetProcessName();
                
                _change_cross_section_operations[wrapperProcess] = 
                new G4BOptnChangeCrossSection(operationName);
            }
        }
        _setup = false;
    }
}

G4VBiasingOperation* 
ChangeCrossSection::ProposeOccurenceBiasingOperation(const G4Track*            track, 
                                                    const G4BiasingProcessInterface*
                                                    callingProcess)
{
    // -- Check if current particle type is the one to bias:
    if ( track->GetDefinition() != _particle_to_bias ) 
    {
        return 0;
    }  
  
 
    // -- select and setup the biasing operation for current callingProcess:
// No entiendo
    // -- Check if the analog cross-section well defined : for example, the conversion
    // -- process for a gamma below e+e- creation threshold has an DBL_MAX interaction
    // -- length. Nothing is done in this case (ie, let analog process to deal with the case)

    G4double analogInteractionLength =  
        callingProcess->GetWrappedProcess()->GetCurrentInteractionLength();
        
    if (analogInteractionLength > DBL_MAX/10.) return 0;
    // -- Analog cross-section is well-defined:
    G4double analogXS = 1./analogInteractionLength;

  // -- Choose a constant cross-section bias. But at this level, this factor can be made
  // -- direction dependent, like in the exponential transform MCNP case, or it
  // -- can be chosen differently, depending on the process, etc.

    G4double XStransformation = 10;
  
    // -- fetch the operation associated to this callingProcess:
    G4BOptnChangeCrossSection*   operation = _change_cross_section_operations[callingProcess];
    // -- get the operation that was proposed to the process in the previous step:
    G4VBiasingOperation* previousOperation = callingProcess->GetPreviousOccurenceBiasingOperation();
  
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
    if (previousOperation==0)
    {
        operation->SetBiasedCrossSection(XStransformation * analogXS);
        operation->Sample();
    }
    else
    {
        if(operation->GetInteractionOccured())
        {
            operation->SetBiasedCrossSection( XStransformation * analogXS);
            operation->Sample();
        }
        
        else
        {
            // -- update the 'interaction length' and underneath 'number of interaction lengths'
            // -- for past step  (this takes into accout the previous step cross-section value)
            operation->UpdateForStep( callingProcess->GetPreviousStepSize() );
            // -- update the cross-section value:
            operation->SetBiasedCrossSection( XStransformation * analogXS );
            // -- forces recomputation of the 'interaction length' taking into account above
            // -- new cross-section value [tricky : to be improved]
            operation->UpdateForStep(0.0);
        }
    }
    return operation;
}


void ChangeCrossSection::OperationApplied(const G4BiasingProcessInterface*   callingProcess, 
                                                G4BiasingAppliedCase,
                                                G4VBiasingOperation*      occurenceOperationApplied,
                                                G4double,
                                                G4VBiasingOperation*,
                                                const G4VParticleChange*)
{
  G4BOptnChangeCrossSection* operation = _change_cross_section_operations[callingProcess];
  if ( operation ==  occurenceOperationApplied ) operation->SetInteractionOccured();
}


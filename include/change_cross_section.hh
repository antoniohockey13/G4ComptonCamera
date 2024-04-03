#ifndef CHANGE_CROSS_SECTION_HH
#define CHANGE_CROSS_SECTION_HH

// Copy from example GB01
#include "G4VBiasingOperator.hh"
class G4BOptnChangeCrossSection;
class G4ParticleDefinition;
#include <map>

class ChangeCrossSection : public G4VBiasingOperator {
public:
    ChangeCrossSection(G4String name = "ChangeXS");
    virtual ~ChangeCrossSection();
    
    virtual void StartRun();
  
private:
    virtual G4VBiasingOperation*
    ProposeOccurenceBiasingOperation(const G4Track*                            track,
                                    const G4BiasingProcessInterface* callingProcess);
    // Methods not used:
    
    virtual G4VBiasingOperation*
    ProposeFinalStateBiasingOperation(const G4Track*, const G4BiasingProcessInterface*)
    {return 0;}
    
    virtual G4VBiasingOperation*
    ProposeNonPhysicsBiasingOperation(const G4Track*, const G4BiasingProcessInterface*)
    {return 0;}

private:
    using G4VBiasingOperator::OperationApplied;
    
    virtual void OperationApplied( const G4BiasingProcessInterface*                calling_process,
                                G4BiasingAppliedCase                               biasing_case,
                                G4VBiasingOperation*                 occurence_operation_applied,
                                G4double                         weight_for_occurence_interaction,
                                G4VBiasingOperation*                final_state_operation_applied, 
                                const G4VParticleChange*                particle_change_produced );
  
private:
    // -- List of associations between processes and biasing operations:
    std::map< const G4BiasingProcessInterface*, 
            G4BOptnChangeCrossSection*       > _change_cross_section_operations;
            G4bool                                  _setup;
            const G4ParticleDefinition*    _particle_to_bias;

};

#endif

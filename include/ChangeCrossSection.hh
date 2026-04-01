#ifndef CHANGE_CROSS_SECTION_HH
#define CHANGE_CROSS_SECTION_HH

// Copy from example GB01
#include "G4VBiasingOperator.hh"
#include <map>
#include "globals.hh"

class G4BOptnChangeCrossSection;
class G4ParticleDefinition;
class G4BiasingProcessInterface;
class G4VBiasingOperation;
class G4Track;
class G4VParticleChange;

class ChangeCrossSection : public G4VBiasingOperator {
public:
    ChangeCrossSection(G4String name = "ChangeXS");
    virtual ~ChangeCrossSection();
    
    virtual void StartRun() override;
  
private:
    virtual G4VBiasingOperation*
    ProposeOccurenceBiasingOperation(const G4Track* track,
                                    const G4BiasingProcessInterface* callingProcess) override;    
    virtual G4VBiasingOperation*
    ProposeFinalStateBiasingOperation(const G4Track*, const G4BiasingProcessInterface*) override
    {return nullptr;}
    
    virtual G4VBiasingOperation*
    ProposeNonPhysicsBiasingOperation(const G4Track*, const G4BiasingProcessInterface*) override
    {return nullptr;}

private:
    using G4VBiasingOperator::OperationApplied;
    
    virtual void OperationApplied(const G4BiasingProcessInterface* callingProcess,
                                  G4BiasingAppliedCase biasingCase,
                                  G4VBiasingOperation* occurenceOperationApplied,
                                  G4double weightForOccurenceInteraction,
                                  G4VBiasingOperation* finalStateOperationApplied,
                                  const G4VParticleChange* particleChangeProduced) override;
private:
    G4bool IsComptonWrappedProcess(const G4String& procName) const;
    G4bool IsInBiasedVolume(const G4Track* track) const;
    G4bool IsEligibleTrack(const G4Track* track) const;

private:
    // -- List of associations between processes and biasing operations:
    G4BOptnChangeCrossSection* _compton_operation;
    G4bool _setup;
    G4bool _compton_wrapper_found;
    const G4ParticleDefinition* _particle_to_bias;
    G4double _xs_factor;

    G4bool _bias_only_primary_gamma;
    G4String _biased_volume_prefix;
};

#endif
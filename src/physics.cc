#include "physics.hh"

#include "G4DecayPhysics.hh"
#include "G4GenericBiasingPhysics.hh"
#include "G4PhysicsListHelper.hh"

// Particles
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

// Gamma processes
#include "G4PhotoElectricEffect.hh"
#include "G4ComptonScattering.hh"
#include "G4RayleighScattering.hh"
#include "G4GammaConversion.hh"

// e-/e+ processes
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

// EM configuration
#include "G4EmParameters.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

ComptCameraEMPhysics::ComptCameraEMPhysics(const G4String& name)
: G4VPhysicsConstructor(name)
{
}

ComptCameraEMPhysics::~ComptCameraEMPhysics()
{
}

void ComptCameraEMPhysics::ConstructParticle()
{
    G4Gamma::GammaDefinition();
    G4Electron::ElectronDefinition();
    G4Positron::PositronDefinition();
}

void ComptCameraEMPhysics::ConstructProcess()
{
    auto* ph = G4PhysicsListHelper::GetPhysicsListHelper();

    // EM parameters ---
    auto* empar = G4EmParameters::Instance();
    empar->SetDefaults();

    // Gamma
    auto* gamma = G4Gamma::Gamma();

    ph->RegisterProcess(new G4PhotoElectricEffect(), gamma);
    ph->RegisterProcess(new G4ComptonScattering(),  gamma);
    ph->RegisterProcess(new G4RayleighScattering(), gamma);
    ph->RegisterProcess(new G4GammaConversion(),    gamma);

    // Electron
    auto* electron = G4Electron::Electron();

    ph->RegisterProcess(new G4eMultipleScattering(), electron);
    ph->RegisterProcess(new G4eIonisation(),         electron);
    ph->RegisterProcess(new G4eBremsstrahlung(),     electron);

    // Positron
    auto* positron = G4Positron::Positron();

    ph->RegisterProcess(new G4eMultipleScattering(), positron);
    ph->RegisterProcess(new G4eIonisation(),         positron);
    ph->RegisterProcess(new G4eBremsstrahlung(),     positron);
    ph->RegisterProcess(new G4eplusAnnihilation(),   positron);

    G4cout << "[PHYSICS] Custom EM physics registered with explicit gamma processes." << G4endl;
    G4cout << "[PHYSICS] Expect GenericBiasingPhysics to see separate gamma wrappers "
           << "(e.g. compt, phot, Rayl) instead of only GammaGeneralProc." << G4endl;
}

ComptCameraPhysicsList::ComptCameraPhysicsList()
: G4VModularPhysicsList()
{
    SetVerboseLevel(1);

    // Decays
    RegisterPhysics(new G4DecayPhysics());

    // Custom EM physics with explicit gamma processes
    RegisterPhysics(new ComptCameraEMPhysics());

    // Biasing infrastructure
    auto* biasingPhysics = new G4GenericBiasingPhysics();
    biasingPhysics->PhysicsBias("gamma");
    RegisterPhysics(biasingPhysics);

    // Conservative default cut
    SetDefaultCutValue(0.1 * mm);
}

ComptCameraPhysicsList::~ComptCameraPhysicsList()
{
}
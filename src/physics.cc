#include "physics.hh"

#include "G4PhysicsListHelper.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4SystemOfUnits.hh"

//Physics Lists
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
//Processes
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4RayleighScattering.hh"
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"	
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
// Models
#include "G4KleinNishinaModel.hh"
#include "G4LowEPComptonModel.hh"
#include "G4PenelopeGammaConversionModel.hh"
#include "G4LivermorePhotoElectricModel.hh"
#include "G4LivermoreComptonModel.hh"
#include "G4PenelopeIonisationModel.hh"
#include "G4UniversalFluctuation.hh"
#include "G4CoulombScattering.hh"
#include "G4eCoulombScatteringModel.hh"
//#include "G4UrbanMscModel95.hh"
#include "G4WentzelVIModel.hh"
#include "G4MscStepLimitType.hh"




ComptCameraPhysicsList::ComptCameraPhysicsList()
{
	SetVerboseLevel(1);

	//RegisterPhysics(new G4DecayPhysics());
	//RegisterPhysics(new G4EmStandardPhysics_option4());
}

void ComptCameraPhysicsList::ConstructParticle()
{
	G4Gamma::GammaDefinition();
	G4Electron::ElectronDefinition();
	//Positrion needed even if it is not used
	G4Positron::PositronDefinition();
}

void ComptCameraPhysicsList::ConstructProcess()
{
	AddTransportation();
	// Construct EM processes
	auto *helper = G4PhysicsListHelper::GetPhysicsListHelper();

	auto particleIterator = GetParticleIterator();
	particleIterator->reset();

	while ((*particleIterator)())
	{
		G4ParticleDefinition *particle = particleIterator->value();
		G4String particleName = particle->GetParticleName();

		if (particleName == "gamma")
		{
			//Photoelectric
			G4PhotoElectricEffect* photoelectric = new G4PhotoElectricEffect();
			G4VEmModel* theLivermorePEModel = new G4LivermorePhotoElectricModel();
			theLivermorePEModel->SetHighEnergyLimit(10*GeV);
			photoelectric->SetEmModel(theLivermorePEModel, 1);
			helper->RegisterProcess(photoelectric, particle);
			
			//Compton
			G4ComptonScattering* compton = new G4ComptonScattering();
			compton->SetEmModel(new G4KleinNishinaModel());
			G4VEmModel *comptonModel = new G4LivermoreComptonModel();
			comptonModel->SetHighEnergyLimit(2*MeV);
			compton->AddEmModel(0, comptonModel);
			helper->RegisterProcess(compton, particle);

			//Rayleigh
			helper->RegisterProcess(new G4RayleighScattering(), particle);

			// gamma conversion
            G4GammaConversion* gammaConversion = new G4GammaConversion();
			G4VEmModel* thePenelopeGCModel = new G4PenelopeGammaConversionModel();
			thePenelopeGCModel->SetHighEnergyLimit(1*GeV);
            gammaConversion->SetEmModel(thePenelopeGCModel, 1);
            helper->RegisterProcess(gammaConversion, particle);

		}
		else if (particleName == "e-")
		{
			// MSC
			G4eMultipleScattering *msc = new G4eMultipleScattering();
			msc->SetStepLimitType(fUseDistanceToBoundary);
			//G4UrbanMscModel95* msc1 = new G4UrbanMscModel95();
			G4WentzelVIModel* msc2 = new G4WentzelVIModel();
			//msc1->SetHighEnergyLimit(100*MeV);
			msc2->SetLowEnergyLimit(100*MeV);
			//msc->AddEmModel(0, msc1);
			msc->AddEmModel(0, msc2);
			
			helper->RegisterProcess(msc, particle);

			G4eCoulombScatteringModel* ssm = new G4eCoulombScatteringModel();
			G4CoulombScattering* ss = new G4CoulombScattering();
			ss->SetEmModel(ssm,1);
			ss->SetMinKinEnergy(100*MeV);
			ssm->SetLowEnergyLimit(100*MeV);
			ssm->SetActivationLowEnergyLimit(100*MeV);

			helper->RegisterProcess(ss, particle);

			// Ionisation
			G4eIonisation *eIoni = new G4eIonisation();
			eIoni->SetStepFunction(0.2, 100*um);
			G4VEmModel* theIoniPenelope = new G4PenelopeIonisationModel();
			theIoniPenelope->SetHighEnergyLimit(0.1*MeV);
			eIoni->AddEmModel(0, theIoniPenelope, new G4UniversalFluctuation());

			helper->RegisterProcess(eIoni, particle);

			// Bremsstrahlung
			G4eBremsstrahlung* bremsstrahlung = new G4eBremsstrahlung();
		
			helper->RegisterProcess(bremsstrahlung, particle);
		}

		else if (particleName == "e+")
		{
			// MSC
			G4eMultipleScattering *msc = new G4eMultipleScattering();
			msc->SetStepLimitType(fUseDistanceToBoundary);
			//G4UrbanMscModel95* msc1 = new G4UrbanMscModel95();
			G4WentzelVIModel* msc2 = new G4WentzelVIModel();
			//msc1->SetHighEnergyLimit(100*MeV);
			msc2->SetLowEnergyLimit(100*MeV);
			//msc->AddEmModel(0, msc1);
			msc->AddEmModel(0, msc2);
			
			helper->RegisterProcess(msc, particle);

			G4eCoulombScatteringModel* ssm = new G4eCoulombScatteringModel();
			G4CoulombScattering* ss = new G4CoulombScattering();
			ss->SetEmModel(ssm,1);
			ss->SetMinKinEnergy(100*MeV);
			ssm->SetLowEnergyLimit(100*MeV);
			ssm->SetActivationLowEnergyLimit(100*MeV);

			helper->RegisterProcess(ss, particle);

			// Ionisation
			G4eIonisation *eIoni = new G4eIonisation();
			eIoni->SetStepFunction(0.2, 100*um);
			G4VEmModel* theIoniPenelope = new G4PenelopeIonisationModel();
			theIoniPenelope->SetHighEnergyLimit(0.1*MeV);
			eIoni->AddEmModel(0, theIoniPenelope, new G4UniversalFluctuation());

			helper->RegisterProcess(eIoni, particle);

			// Bremsstrahlung
			G4eBremsstrahlung* bremsstrahlung = new G4eBremsstrahlung();
		
			helper->RegisterProcess(bremsstrahlung, particle);

			// Annihilation
			helper->RegisterProcess(new G4eplusAnnihilation(), particle);
		}
	}	


	
	// I should bias both same way to keep the ratio between them constant
	//thePhotoElectricEffect->SetCrossSectionBiasingFactor(10);
	//theComptonScattering->SetCrossSectionBiasingFactor(10);

}
ComptCameraPhysicsList::~ComptCameraPhysicsList()
{
}

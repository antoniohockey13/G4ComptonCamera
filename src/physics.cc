#include "physics.hh"

#include "G4PhysicsListHelper.hh"
#include "G4LossTableManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4EmBuilder.hh"


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
#include "G4ePairProduction.hh"
#include "G4hMultipleScattering.hh"
#include "G4LivermoreIonisationModel.hh"
// Models
#include "G4BetheHeitler5DModel.hh"
#include "G4KleinNishinaModel.hh"
#include "G4LowEPComptonModel.hh"
#include "G4PenelopeGammaConversionModel.hh"
#include "G4LivermorePhotoElectricModel.hh"
#include "G4LivermoreRayleighModel.hh"
#include "G4LivermoreComptonModel.hh"
#include "G4PenelopeIonisationModel.hh"
#include "G4UniversalFluctuation.hh"
#include "G4UrbanFluctuation.hh"
#include "G4CoulombScattering.hh"
#include "G4eCoulombScatteringModel.hh"
#include "G4GoudsmitSaundersonMscModel.hh"
#include "G4SeltzerBergerModel.hh"
#include "G4WentzelVIModel.hh"
#include "G4MscStepLimitType.hh"
#include "G4Generator2BS.hh"
#include "G4GammaGeneralProcess.hh"

#include "G4NuclearStopping.hh"



ComptCameraPhysicsList::ComptCameraPhysicsList()
{
	SetVerboseLevel(1);
	G4EmParameters* param = G4EmParameters::Instance();
	param->SetDefaults();
	param->SetVerbose(1);
	param->SetGeneralProcessActive(true);
	param->SetMinEnergy(100*CLHEP::eV);
	param->SetLowestElectronEnergy(100*CLHEP::eV);
	param->SetNumberOfBinsPerDecade(20);
	param->ActivateAngularGeneratorForIonisation(true);
	param->SetStepFunction(0.2, 10*CLHEP::um);
	param->SetStepFunctionMuHad(0.1, 50*CLHEP::um);
	param->SetStepFunctionLightIons(0.1, 20*CLHEP::um);
	param->SetStepFunctionIons(0.1, 1*CLHEP::um);
	param->SetUseMottCorrection(true); // use Mott-correction for e-/e+ msc gs
	param->SetMscStepLimitType(fUseSafetyPlus); // for e-/e+ msc gs
	param->SetMscSkin(3);              // error-free stepping for e-/e+ msc gs
	param->SetMscRangeFactor(0.08);    // error-free stepping for e-/e+ msc gs
	param->SetMuHadLateralDisplacement(true);
	param->SetFluo(true);
	param->SetUseICRU90Data(true);
	param->SetMaxNIELEnergy(1*CLHEP::MeV);
}

void ComptCameraPhysicsList::ConstructBosons()
{
	G4Gamma::GammaDefinition();
}

void ComptCameraPhysicsList::ConstructLeptons()
{
	G4Electron::ElectronDefinition();
	G4Positron::PositronDefinition();
}

void ComptCameraPhysicsList::ConstructParticle()
{
	ConstructBosons();
	ConstructLeptons();
}

void ComptCameraPhysicsList::ConstructEM()
{
	G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
	G4EmParameters* param = G4EmParameters::Instance();
	G4double highEnergyLimit = param->MscEnergyLimit();

	auto particleIterator = GetParticleIterator();
	particleIterator->reset();

	while ((*particleIterator)())
	{
		G4ParticleDefinition* particle = particleIterator->value();

		G4String particleName = particle->GetParticleName();
		// I should bias both same way to keep the ratio between them constant
		//thePhotoElectricEffect->SetCrossSectionBiasingFactor(10);
		//theComptonScattering->SetCrossSectionBiasingFactor(10);
		if (particleName == "gamma")
		{
			//Photoelectric
			G4PhotoElectricEffect* photoelectric = new G4PhotoElectricEffect();
			G4VEmModel* photoelectricModel = new G4LivermorePhotoElectricModel();
			photoelectric->SetEmModel(photoelectricModel);
			ph->RegisterProcess(photoelectric, particle);
		
			//Compton
			G4ComptonScattering* compton = new G4ComptonScattering();
			G4VEmModel* comptonModel = new G4KleinNishinaModel();
			compton->SetEmModel(comptonModel);
			G4VEmModel* cModel = new G4LowEPComptonModel();
			cModel->SetHighEnergyLimit(20*CLHEP::MeV);
			compton->AddEmModel(0, cModel);
			ph->RegisterProcess(compton, particle);

			//Gamma conversion
			G4GammaConversion* gammaConversion = new G4GammaConversion();
			G4VEmModel* gammaConversionModel = new G4BetheHeitler5DModel();
			gammaConversion->SetEmModel(gammaConversionModel);
			ph->RegisterProcess(gammaConversion, particle);

			G4RayleighScattering* rayleigh = new G4RayleighScattering();
			G4VEmModel* rayleighModel = new G4LivermoreRayleighModel();
			rayleigh->SetEmModel(rayleighModel);
			ph->RegisterProcess(rayleigh, particle);
		}
		else if (particleName == "e-")
		{
			//Msc
			G4eMultipleScattering* msc = new G4eMultipleScattering();
			G4GoudsmitSaundersonMscModel* msc1 = new G4GoudsmitSaundersonMscModel();
			G4WentzelVIModel* msc2 = new G4WentzelVIModel();
			msc1->SetHighEnergyLimit(highEnergyLimit);
			msc2->SetLowEnergyLimit(highEnergyLimit);
			ConstructElectronMscProcess(msc1, msc2, particle);

			//Coulomb scattering
			G4eCoulombScatteringModel* ssm = new G4eCoulombScatteringModel(); 
  			G4CoulombScattering* ss = new G4CoulombScattering();
  			ss->SetEmModel(ssm); 
  			ss->SetMinKinEnergy(highEnergyLimit);
  			ssm->SetLowEnergyLimit(highEnergyLimit);
  			ssm->SetActivationLowEnergyLimit(highEnergyLimit);
			ph->RegisterProcess(ss, particle);

			//Ionisation
			G4eIonisation* eIoni = new G4eIonisation();
			G4VEmModel* eIoniModel = new G4PenelopeIonisationModel();
			eIoni->SetFluctModel(ModelOfFluctuations());
			eIoniModel->SetHighEnergyLimit(0.1*CLHEP::MeV);
			eIoni->AddEmModel(0, eIoniModel);
			ph->RegisterProcess(eIoni, particle);

			//Bremsstrahlung
			G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();
			G4SeltzerBergerModel* eBremModel1 = new G4SeltzerBergerModel();
			G4eBremsstrahlungRelModel* eBremModel2 = new G4eBremsstrahlungRelModel();
			eBremModel1->SetAngularDistribution(new G4Generator2BS());
			eBremModel2->SetAngularDistribution(new G4Generator2BS());
			eBrem->SetEmModel(eBremModel1);
			eBrem->SetEmModel(eBremModel2);
			eBremModel1->SetHighEnergyLimit(CLHEP::GeV);
			ph->RegisterProcess(eBrem, particle);

		}

		else if (particleName =="e+")
		{
			//Msc
			G4eMultipleScattering* msc = new G4eMultipleScattering();
			G4GoudsmitSaundersonMscModel* msc1 = new G4GoudsmitSaundersonMscModel();
			G4WentzelVIModel* msc2 = new G4WentzelVIModel();
			msc1->SetHighEnergyLimit(highEnergyLimit);
			msc2->SetLowEnergyLimit(highEnergyLimit);
			ConstructElectronMscProcess(msc1, msc2, particle);

			//Coulomb scattering
			G4eCoulombScatteringModel* ssm = new G4eCoulombScatteringModel(); 
  			G4CoulombScattering* ss = new G4CoulombScattering();
  			ss->SetEmModel(ssm); 
  			ss->SetMinKinEnergy(highEnergyLimit);
  			ssm->SetLowEnergyLimit(highEnergyLimit);
  			ssm->SetActivationLowEnergyLimit(highEnergyLimit);
			ph->RegisterProcess(ss, particle);

			//Ionisation
			G4eIonisation* eIoni = new G4eIonisation();
			G4VEmModel* eIoniModel = new G4PenelopeIonisationModel();
			eIoni->SetFluctModel(ModelOfFluctuations());
			eIoniModel->SetHighEnergyLimit(0.1*CLHEP::MeV);
			eIoni->AddEmModel(0, eIoniModel);
			ph->RegisterProcess(eIoni, particle);

			//Bremsstrahlung
			G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();
			G4SeltzerBergerModel* eBremModel1 = new G4SeltzerBergerModel();
			G4eBremsstrahlungRelModel* eBremModel2 = new G4eBremsstrahlungRelModel();
			eBremModel1->SetAngularDistribution(new G4Generator2BS());
			eBremModel2->SetAngularDistribution(new G4Generator2BS());
			eBrem->SetEmModel(eBremModel1);
			eBrem->SetEmModel(eBremModel2);
			eBremModel1->SetHighEnergyLimit(CLHEP::GeV);
			ph->RegisterProcess(eBrem, particle);
		}
	}
}

void ComptCameraPhysicsList::ConstructProcess()
{
	AddTransportation();
	ConstructEM();
}




ComptCameraPhysicsList::~ComptCameraPhysicsList()
{
}

void ComptCameraPhysicsList::ConstructElectronMscProcess(G4VMscModel* msc1, G4VMscModel* msc2,
														G4ParticleDefinition* particle)
{
	auto type = G4EmParameters::Instance()->TransportationWithMsc();
	G4ProcessManager* procManager = particle->GetProcessManager();
	auto plist = procManager->GetProcessList();
	G4int ptype = (0 < plist->size()) ? (*plist)[0]->GetProcessSubType() : 0;
	
	if(type != G4TransportationWithMscType::fDisabled && ptype == TRANSPORTATION) 
	{
		// Remove default G4Transportation and replace with G4TransportationWithMsc.
		procManager->RemoveProcess(0);
		G4TransportationWithMs static G4VEmFluctuationModel* ModelOfFluctuations(G4bool isIon = false);c* transportWithMsc = new G4TransportationWithMsc(
					G4TransportationWithMsc::ScatteringType::MultipleScattering);
		
		if(type == G4TransportationWithMscType::fMultipleSteps) 
	{
		transportWithMsc->SetMultipleSteps(true);
    }
	
	transportWithMsc->AddMscModel(msc1);
	if(msc2 != nullptr) 
	{
		transportWithMsc->AddMscModel(msc2);
    }
	
	procManager->AddProcess(transportWithMsc, -1, 0, 0);
	} 
	
	else
	{
		// Register as a separate process.
		G4eMultipleScattering* msc = new G4eMultipleScattering;
		msc->SetEmModel(msc1);
		
		if(msc2 != nullptr) static G4VEmFluctuationModel* ModelOfFluctuations(G4bool isIon = false);
		{
			msc->SetEmModel(msc2);
		}
		
		G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
		ph->RegisterProcess(msc, particle);
	}
}

G4VEmFluctuationModel* ComptCameraPhysicsList::ModelOfFluctuations(G4bool isIon)
{
  G4VEmFluctuationModel* f = nullptr;
  auto ftype = G4EmParameters::Instance()->FluctuationType();
  if (ftype == fDummyFluctuation) {
    f = new G4LossFluctuationDummy();
  } else if (isIon) {
    f = new G4IonFluctuations();
  } else if (ftype == fUrbanFluctuation) {
    f = new G4UrbanFluctuation();
  } else {
    f = new G4UniversalFluctuation();
  }
  return f;
}

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
#include "G4LowEPPolarizedComptonModel.hh"
#include "G4PhotoElectricAngularGeneratorPolarized.hh"
#include "G4LivermorePolarizedRayleighModel.hh"
#include "G4UAtomicDeexcitation.hh"
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
	//SetPhysicsType(bElectromagnetic);
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


	G4double nielEnergyLimit = param->MaxNIELEnergy();
	G4NuclearStopping* pnuc = nullptr;
	if(nielEnergyLimit > 0.0)
	{
		pnuc = new G4NuclearStopping();
		pnuc->SetMaxKinEnergy(nielEnergyLimit);
	}

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
			G4bool polar = param->EnablePolarisation();

			//Photoelectric
			G4PhotoElectricEffect* photoelectric = new G4PhotoElectricEffect();
			G4VEmModel* photoelectricModel = new G4LivermorePhotoElectricModel();
			photoelectric->SetEmModel(photoelectricModel);
			if (polar)
			{
				photoelectricModel->SetAngularDistribution(new G4PhotoElectricAngularGeneratorPolarized());
			}
			//photoelectric->SetCrossSectionBiasingFactor(10);
			ph->RegisterProcess(photoelectric, particle);
		
			//Compton
			G4ComptonScattering* compton = new G4ComptonScattering();
			G4VEmModel* comptonModel = new G4KleinNishinaModel();
			compton->SetEmModel(comptonModel);
			G4VEmModel* cModel = nullptr;
			if (polar)
			{
				cModel = new G4LowEPPolarizedComptonModel();
			}
			else
			{
				cModel = new G4LowEPComptonModel();
			}
			cModel->SetHighEnergyLimit(20*CLHEP::MeV);
			compton->AddEmModel(0, cModel);
			compton->SetCrossSectionBiasingFactor(10);
			ph->RegisterProcess(compton, particle);

			//Gamma conversion
			G4GammaConversion* gammaConversion = new G4GammaConversion();
			G4VEmModel* gammaConversionModel = new G4BetheHeitler5DModel();
			gammaConversion->SetEmModel(gammaConversionModel);
			ph->RegisterProcess(gammaConversion, particle);

			//Rayleigh scattering
			G4RayleighScattering* rayleigh = new G4RayleighScattering();
			if (polar)
			{
				rayleigh->SetEmModel(new G4LivermorePolarizedRayleighModel());
			}
			ph->RegisterProcess(rayleigh, particle);

		}
		else if (particleName == "e-")
		{
			// // Msc
			// G4eMultipleScattering* msc = new G4eMultipleScattering();
			// G4GoudsmitSaundersonMscModel* msc1 = new G4GoudsmitSaundersonMscModel();
			// G4WentzelVIModel* msc2 = new G4WentzelVIModel();
			// msc1->SetHighEnergyLimit(highEnergyLimit);
			// msc2->SetLowEnergyLimit(highEnergyLimit);
			// msc->SetEmModel(msc1);
			// msc->SetEmModel(msc2);
			// ph->RegisterProcess(msc, particle);

			// //Coulomb scattering
			// G4eCoulombScatteringModel* ssm = new G4eCoulombScatteringModel(); 
  			// G4CoulombScattering* ss = new G4CoulombScattering();
  			// ss->SetEmModel(ssm); 
  			// ss->SetMinKinEnergy(highEnergyLimit);
  			// ssm->SetLowEnergyLimit(highEnergyLimit);
  			// ssm->SetActivationLowEnergyLimit(highEnergyLimit);
			// ph->RegisterProcess(ss, particle);

			// // Ionisation
			// G4eIonisation* eIoni = new G4eIonisation();
			// G4VEmModel* eIoniModel = new G4LivermoreIonisationModel();
			// eIoniModel->SetDeexcitationFlag(true);
			// eIoni->SetFluctModel(new G4UrbanFluctuation());
			// eIoniModel->SetHighEnergyLimit(0.1*CLHEP::MeV);
			// eIoni->AddEmModel(0, eIoniModel, new G4UniversalFluctuation());
			// ph->RegisterProcess(eIoni, particle);

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

			//Pair production
			G4ePairProduction* ePairProduction = new G4ePairProduction();
			ph->RegisterProcess(ePairProduction, particle);

		}

		else if (particleName =="e+")
		{
			//Msc
			G4eMultipleScattering* msc = new G4eMultipleScattering();
			G4GoudsmitSaundersonMscModel* msc1 = new G4GoudsmitSaundersonMscModel();
			G4WentzelVIModel* msc2 = new G4WentzelVIModel();
			msc1->SetHighEnergyLimit(highEnergyLimit);
			msc2->SetLowEnergyLimit(highEnergyLimit);
			msc->SetEmModel(msc1);
			msc->SetEmModel(msc2);
			ph->RegisterProcess(msc, particle);

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
			eIoni->SetFluctModel(new G4UrbanFluctuation());
			G4VEmModel* eIoniModel = new G4PenelopeIonisationModel();
			eIoniModel->SetDeexcitationFlag(true);
			eIoniModel->SetHighEnergyLimit(0.1*CLHEP::MeV);
			eIoni->AddEmModel(0, eIoniModel, new G4UniversalFluctuation());
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
	G4EmParameters* params = G4EmParameters::Instance();
  	params->SetFluo(true);
  	params->SetAugerCascade(false);
  	params->SetAuger(false);
  	params->SetPixe(false);
  	params->SetDeexActiveRegion("World", true, false, false);
  	//just tune your cuts, rather than the heavy-handed stuff
  	params->SetDeexcitationIgnoreCut(false);
  	//make gammas follow cuts
  	params->SetApplyCuts(true);
  	params->Dump();
	G4LossTableManager::Instance()->SetAtomDeexcitation(new G4UAtomicDeexcitation());
  
}

void ComptCameraPhysicsList::ConstructProcess()
{
	AddTransportation();
	ConstructEM();
}




ComptCameraPhysicsList::~ComptCameraPhysicsList()
{
}
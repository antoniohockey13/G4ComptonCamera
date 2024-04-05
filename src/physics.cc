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

	//RegisterPhysics(new G4DecayPhysics());
	//RegisterPhysics(new G4EmStandardPhysics_option4());
}

void ComptCameraPhysicsList::ConstructParticle()
{
	G4EmBuilder::ConstructMinimalEmSet();
}

void ComptCameraPhysicsList::ConstructProcess()
{
	AddTransportation();
	G4EmBuilder::PrepareEMPhysics();
	G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
	G4EmParameters* param = G4EmParameters::Instance();
	
	// nuclear stopping
	G4double nielEnergyLimit = 1*CLHEP::MeV;
	G4NuclearStopping* pnuc = nullptr;
	if(nielEnergyLimit > 0.) 
	{
		pnuc = new G4NuclearStopping();
		pnuc->SetMaxKinEnergy(nielEnergyLimit);
	}

	G4double highEnergyLimit = param->MscEnergyLimit();

	//Gamma
	G4ParticleDefinition* particle = G4Gamma::Gamma();

	//Photoelectric
	G4PhotoElectricEffect* photoelectric = new G4PhotoElectricEffect();
	G4VEmModel* photoelectricModel = new G4LivermorePhotoElectricModel();
	photoelectric->SetEmModel(photoelectricModel);

	//Compton
	G4ComptonScattering* compton = new G4ComptonScattering();
	G4VEmModel* comptonModel = new G4KleinNishinaModel();
	compton->SetEmModel(comptonModel);
	G4VEmModel* cModel = new G4LowEPComptonModel();
	cModel->SetHighEnergyLimit(20*CLHEP::MeV);
	compton->AddEmModel(0, cModel);

	//Gamma conversion
	G4GammaConversion* gammaConversion = new G4GammaConversion();
	G4VEmModel* gammaConversionModel = new G4BetheHeitler5DModel();
	gammaConversion->SetEmModel(gammaConversionModel);

	//Rayleigh default is Livermore
	G4RayleighScattering* rayleigh = new G4RayleighScattering();

	
	if(param->GeneralProcessActive()) 
	{
    	G4GammaGeneralProcess* sp = new G4GammaGeneralProcess();
    	sp->AddEmProcess(photoelectric);
    	sp->AddEmProcess(compton);
    	sp->AddEmProcess(gammaConversion);
    	sp->AddEmProcess(rayleigh);
    	G4LossTableManager::Instance()->SetGammaGeneralProcess(sp);
    	ph->RegisterProcess(sp, particle);
	} 
	else 
	{
    	ph->RegisterProcess(photoelectric, particle);
    	ph->RegisterProcess(compton, particle);
    	ph->RegisterProcess(gammaConversion, particle);
    	ph->RegisterProcess(rayleigh, particle);
	}

	//Electron
	particle = G4Electron::Electron();

	//Msc
	G4eMultipleScattering* msc = new G4eMultipleScattering();
	G4GoudsmitSaundersonMscModel* msc1 = new G4GoudsmitSaundersonMscModel();
	G4WentzelVIModel* msc2 = new G4WentzelVIModel();
	msc1->SetHighEnergyLimit(highEnergyLimit);
	msc2->SetLowEnergyLimit(highEnergyLimit);
	msc->SetEmModel(msc1);
	msc->SetEmModel(msc2);

	G4eCoulombScatteringModel* ssm = new G4eCoulombScatteringModel();
	G4CoulombScattering* ss = new G4CoulombScattering();
	ss->SetEmModel(ssm);
	ss->SetMinKinEnergy(highEnergyLimit);
	ssm->SetLowEnergyLimit(highEnergyLimit);
	ssm->SetActivationLowEnergyLimit(highEnergyLimit);

	//ionisation
	G4eIonisation* eIoni = new G4eIonisation();
	G4VEmModel* eIoniModel = new G4LivermoreIonisationModel();
	eIoni->SetFluctModel(new G4UrbanFluctuation());
	eIoniModel->SetHighEnergyLimit(0.1*CLHEP::MeV);
	eIoni->AddEmModel(0, eIoniModel, new G4UniversalFluctuation());

	//Bremsstrahlung
	G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();
	G4SeltzerBergerModel* eBremModel1 = new G4SeltzerBergerModel();
	G4eBremsstrahlungRelModel* eBremModel2 = new G4eBremsstrahlungRelModel();
	eBremModel1->SetAngularDistribution(new G4Generator2BS());
	eBremModel2->SetAngularDistribution(new G4Generator2BS());
	eBrem->SetEmModel(eBremModel1);
	eBrem->SetEmModel(eBremModel2);
	eBremModel1->SetHighEnergyLimit(CLHEP::GeV);

	//Pair production
	G4ePairProduction* ePairProduction = new G4ePairProduction();
	
	ph->RegisterProcess(msc, particle);
	ph->RegisterProcess(eIoni, particle);
	ph->RegisterProcess(eBrem, particle);
	ph->RegisterProcess(ePairProduction, particle);
	ph->RegisterProcess(ss, particle);

	//Positron
	particle = G4Positron::Positron();
	//e-/e+
	msc1 = new G4GoudsmitSaundersonMscModel();
	msc2 = new G4WentzelVIModel();
	msc1->SetHighEnergyLimit(highEnergyLimit);
	msc2->SetLowEnergyLimit(highEnergyLimit);
	msc->SetEmModel(msc1);
	msc->SetEmModel(msc2);

	ssm = new G4eCoulombScatteringModel();
	ss = new G4CoulombScattering();
	ss->SetEmModel(ssm);
	ss->SetMinKinEnergy(highEnergyLimit);
	ssm->SetLowEnergyLimit(highEnergyLimit);
	ssm->SetActivationLowEnergyLimit(highEnergyLimit);

	//ionisation
	eIoni = new G4eIonisation();
	eIoni->SetFluctModel(new G4UrbanFluctuation());
	eIoniModel = new G4PenelopeIonisationModel();
	eIoniModel->SetHighEnergyLimit(0.1*CLHEP::MeV);
	eIoni->AddEmModel(0, eIoniModel, new G4UniversalFluctuation());

	//Bremsstrahlung
	eBrem = new G4eBremsstrahlung();
	eBremModel1 = new G4SeltzerBergerModel();
	eBremModel2 = new G4eBremsstrahlungRelModel();
	eBremModel1->SetAngularDistribution(new G4Generator2BS());
	eBremModel2->SetAngularDistribution(new G4Generator2BS());
	eBrem->SetEmModel(eBremModel1);
	eBrem->SetEmModel(eBremModel2);
	eBremModel1->SetHighEnergyLimit(CLHEP::GeV);

	ph->RegisterProcess(msc, particle);
	ph->RegisterProcess(eIoni, particle);
	ph->RegisterProcess(eBrem, particle);
	ph->RegisterProcess(ePairProduction, particle);
	ph->RegisterProcess(ss, particle);
	ph->RegisterProcess(new G4eplusAnnihilation(), particle);
	
	// I should bias both same way to keep the ratio between them constant
	//thePhotoElectricEffect->SetCrossSectionBiasingFactor(10);
	//theComptonScattering->SetCrossSectionBiasingFactor(10);

}
ComptCameraPhysicsList::~ComptCameraPhysicsList()
{
}

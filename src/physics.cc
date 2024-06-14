#include "physics.hh"

#include "G4DecayPhysics.hh"
//#include "G4EmStandardPhysics_option4.hh"
#include "G4EmPenelopePhysics.hh"


ComptCameraPhysicsList::ComptCameraPhysicsList()
{
    RegisterPhysics(new G4DecayPhysics());
    //RegisterPhysics(new G4EmLivermorePhysics());
   RegisterPhysics(new G4EmPenelopePhysics());	
   //RegisterPhysics(new G4EmStandardPhysics_option4());
   
   /*const G4double cut = 0.1*CLHEP::um;
   SetCutValue(cut, "gamma");
   SetCutValue(cut, "e-");
   SetCutValue(cut, "e+");*/
   
   DumpCutValuesTable();
}

ComptCameraPhysicsList::~ComptCameraPhysicsList()
{
}

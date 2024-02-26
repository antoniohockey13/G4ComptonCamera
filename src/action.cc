#include "action.hh"
#include "generator.hh"
#include "run.hh"
#include "event.hh"
#include "stepping.hh"


ComptCameraActionInitialization::ComptCameraActionInitialization()
{
}

ComptCameraActionInitialization::~ComptCameraActionInitialization()
{
}

void ComptCameraActionInitialization::Build() const
{
	ComptCameraPrimaryGenerator *generator = new ComptCameraPrimaryGenerator();
	SetUserAction(generator);
	
	ComptCameraRunAction *runAction = new ComptCameraRunAction();
	SetUserAction(runAction);
	
	ComptCameraEventAction *eventAction = new ComptCameraEventAction(runAction);
	SetUserAction(eventAction);
	
	ComptCameraSteppingAction *steppingAction = new ComptCameraSteppingAction(eventAction);
	SetUserAction(steppingAction); 
}

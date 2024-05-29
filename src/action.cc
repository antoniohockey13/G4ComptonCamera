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

void ComptCameraActionInitialization::BuildForMaster() const
{
	ComptCameraRunAction *runAction = new ComptCameraRunAction();
	SetUserAction(runAction);
}

void ComptCameraActionInitialization::Build() const
{
	ComptCameraPrimaryGenerator *generator = new ComptCameraPrimaryGenerator();
	SetUserAction(generator);
	
	ComptCameraRunAction *runAction = new ComptCameraRunAction();
	SetUserAction(runAction);

	ComptCameraEventAction *eventAction = new ComptCameraEventAction(runAction);
	SetUserAction(eventAction);
	
        // XXX -- Why is needed?
	//ComptCameraSteppingAction *steppingAction = new ComptCameraSteppingAction(eventAction);
	//SetUserAction(steppingAction); 
}




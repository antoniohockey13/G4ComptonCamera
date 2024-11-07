#include "action.hh"
#include "generator.hh"
#include "run.hh"
#include "event.hh"
#include "stepping.hh"


ComptCameraActionInitialization::ComptCameraActionInitialization(bool is_phantom_present) :
    _is_phantom_present(is_phantom_present)
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

	ComptCameraEventAction *eventAction = new ComptCameraEventAction(_is_phantom_present);
	SetUserAction(eventAction);
}




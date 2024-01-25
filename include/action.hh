#ifndef ACTION_HH
#define ACTION_HH
#include "G4VUserActionInitialization.hh"

#include "generator.hh"
#include "run.hh"
#include "event.hh"
#include "stepping.hh"

class ComptCameraActionInitialization : public G4VUserActionInitialization
{
public:
	ComptCameraActionInitialization();
	~ComptCameraActionInitialization();

	virtual void Build() const;
};

#endif

#ifndef ACTION_HH
#define ACTION_HH
#include "G4VUserActionInitialization.hh"


class ComptCameraActionInitialization : public G4VUserActionInitialization
{
public:
	ComptCameraActionInitialization();
	~ComptCameraActionInitialization();

	virtual void Build() const;
};

#endif

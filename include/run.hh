#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"

class ComptCameraRunAction : public G4UserRunAction
{
public:
	ComptCameraRunAction();
	~ComptCameraRunAction();
	virtual void RecordEvent(const G4Run *);
	virtual void Merge(const G4Run *);
	virtual void BeginOfRunAction(const G4Run *);
	virtual void EndOfRunAction(const G4Run *);

};

#endif

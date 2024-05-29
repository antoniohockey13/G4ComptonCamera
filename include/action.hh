#ifndef ACTION_HH
#define ACTION_HH
#include "G4VUserActionInitialization.hh"


class ComptCameraActionInitialization : public G4VUserActionInitialization
{
    public:
        ComptCameraActionInitialization(bool is_phantom_present);
        ComptCameraActionInitialization() = delete;

        ~ComptCameraActionInitialization();
        
        virtual void Build() const override;
        virtual void BuildForMaster() const override;

    private:
        // Whether the phantom SD is included (needed by the Event action)
        bool _is_phantom_present;

};

#endif

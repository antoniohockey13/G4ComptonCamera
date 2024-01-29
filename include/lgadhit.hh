#ifndef LGADHIT_HH
#define LGADHIT_HH

// Copy from example B2a
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"


class LGADHit : public G4VHit
{
    public:
        LGADHit() = default;
        LGADHit(const LGADHit&) = default;
        ~LGADHit() override = default;

        //operators
        LGADHit& operator=(const LGADHit&) = default;
        G4bool operator==(const LGADHit&) const;

        inline void* operator new(size_t);
        inline void operator delete(void*);
        //methods from base class
        void Draw() override;
        void Print() override;

        //setters
        void SetTrackID(G4int track_id) { _track_id = track_id;};
        void SetDetectorNb(G4int detector_nb) { _detector_nb = detector_nb;};
        void SetEdep(G4double e_dep) { _e_dep = e_dep;};
        void SetPos(G4ThreeVector pos) { _pos = pos;};

        //getters
        G4int GetTrackID() const { return _track_id;};
        G4int GetDetectorNb() const { return _detector_nb;};
        G4double GetEdep() const { return _e_dep;};
        G4ThreeVector GetPos() const { return _pos;};
        G4ThreeVector GetMom() const { return _mom;};

    private:
        G4int _track_id;
        G4int _detector_nb;
        G4double _e_dep = 0;
        G4ThreeVector _pos;
        G4ThreeVector _mom;
};


using LGADHitsCollection = G4THitsCollection<LGADHit>;
extern G4ThreadLocal G4Allocator<LGADHit>* LGADHitAllocator;

inline void* LGADHit::operator new(size_t) 
{
    if (!LGADHitAllocator)
    {
        LGADHitAllocator = new G4Allocator<LGADHit>;
    }
    return (void*)LGADHitAllocator->MallocSingle();
}

//

inline void LGADHit::operator delete(void* hit)
{
    LGADHitAllocator->FreeSingle((LGADHit*) hit);
}

#endif
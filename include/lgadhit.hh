#ifndef LGADHIT_HH
#define LGADHIT_HH

// Copy from example B2a
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"


class LGADHit : public G4VHit
{
    public:
        LGADHit() = default;
        LGADHit(const LGADHit&) = default;
        ~LGADHit() = default;

        //operators
        LGADHit& operator=(const LGADHit&) = default;
        G4bool operator==(const LGADHit&) const;

        //methods from base class
        void Draw() override;
        void Print() override;

        //setters
        void SetTrackID(G4int track_id) { _f_track_id = track_id;};
        void SetDetectorNb(G4int detector_nb) { _f_detector_nb = detector_nb;};
        void SetEdep(G4double e_dep) { _f_e_dep = e_dep;};
        void SetPos(G4ThreeVector pos) { _f_pos = pos;};

        //getters
        G4int GetTrackID() const { return _f_track_id;};
        G4int GetDetectorNb() const { return _f_detector_nb;};
        G4double GetEdep() const { return _f_e_dep;};
        G4ThreeVector GetPos() const { return _f_pos;};
        G4ThreeVector GetMom() const { return _f_mom;};

    private:
        G4int _f_track_id = -1;
        G4int _f_detector_nb = -1;
        G4double _f_e_dep = 0;
        G4ThreeVector _f_pos;
        G4ThreeVector _f_mom;
};

//

using LGADHitsCollection = G4THitsCollection<LGADHit>;
extern G4ThreadLocal G4Allocator<LGADHit>* LGADHitAllocator;
/* No se que hace y falla al compilar
//
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
*/


#endif
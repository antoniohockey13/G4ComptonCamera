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
        //Transform name of the from DetectorX to int(X)
        void SetDetectorNb(G4String detector_name) { _detector_nb = std::stoi(detector_name.erase(0,8));};
        void SetEdep(G4double e_dep) { _e_dep = e_dep;};
        void SetPos(G4ThreeVector pos) { _pos = pos;};
        void SetMom(G4ThreeVector mom) { _mom = mom;};
        void SetParticleID(G4int particle_id) { _particle_id = particle_id;};
        void SetParentID(G4int parent_id) { _parent_id = parent_id;};
        void SetTime(G4double time) { _time = time;};
        void SetKineticEnergy(G4double kinetic_energy) { _kinetic_energy = kinetic_energy;};
        void SetProcessName(G4String process_name) { _process_name = process_name;};

        //getters
        G4int GetTrackID() const { return _track_id;};
        G4int GetParticleID() const { return _particle_id;};
        G4int GetParentID() const { return _parent_id;};
        G4int GetDetectorNb() const { return _detector_nb;};
        G4double GetTime() const { return _time;};
        G4double GetEdep() const { return _e_dep;};
        G4ThreeVector GetPos() const { return _pos;};
        G4ThreeVector GetMom() const { return _mom;};
        G4double GetKineticEnergy() const { return _kinetic_energy;};
        G4String GetProcessName() const { return _process_name;};

    private:
        G4int _track_id;
        G4int _particle_id;
        G4int _parent_id;
        G4int _detector_nb;
        G4double _time;
        G4double _e_dep;
        G4double _kinetic_energy;
        G4ThreeVector _pos;
        G4ThreeVector _mom;
        G4String _process_name;
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
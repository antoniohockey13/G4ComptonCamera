#ifndef LGADHIT_HH
#define LGADHIT_HH

// Copy from example B2a
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"


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
        void SetVolume(G4String volume_name) { _volume_name = volume_name;};
        void SetTrackID(G4int track_id) { _track_id = track_id;};
<<<<<<< HEAD
        void SetParentID(G4int parent_id) { _parent_id = parent_id;};
        void SetParticleID(G4int particle_id) { _particle_id = particle_id;};
        // Take the detector number from the name of the volume
        void SetDetectorNb(G4String volume_name) { _detector_nb = std::stoi(volume_name.substr(8));};
        void SetEdep(G4double e_dep) { _e_dep = e_dep;};
        void SetPos(G4ThreeVector pos) { _pos = pos;};
        void SetMom(G4ThreeVector mom) { _mom = mom;};
        void SetTime(G4double time) { _time = time;};
=======
        //Transform name of the from DetectorX to int(X)
        void SetDetectorNb(G4String detector_name) { _detector_nb = std::stoi(detector_name.erase(0,8));};
        void SetEdep(G4double e_dep) { _e_dep = e_dep;};
        void SetPos(G4ThreeVector pos) { _pos = pos;};
        void SetMom(G4ThreeVector mom) { _mom = mom;};
        void SetParticleID(G4int particle_id) { _particle_id = particle_id;};
        void SetParentID(G4int parent_id) { _parent_id = parent_id;};
        void SetTime(G4double time) { _time = time;};

>>>>>>> temp_work

        //getters
        G4String GetVolume() const { return _volume_name;};
        G4int GetTrackID() const { return _track_id;};
<<<<<<< HEAD
        G4int GetParentID() const { return _parent_id;};
        G4int GetParticleID() const { return _particle_id;};
=======
        G4int GetParticleID() const { return _particle_id;};
        G4int GetParentID() const { return _parent_id;};
>>>>>>> temp_work
        G4int GetDetectorNb() const { return _detector_nb;};
        G4double GetTime() const { return _time;};
        G4double GetEdep() const { return _e_dep;};
        G4ThreeVector GetPos() const { return _pos;};
        G4ThreeVector GetMom() const { return _mom;};
        G4double GetTime() const { return _time;};

    private:
        G4String _volume_name;
        G4int _track_id;
<<<<<<< HEAD
        G4int _parent_id;
        G4int _particle_id;
=======
        G4int _particle_id;
        G4int _parent_id;
>>>>>>> temp_work
        G4int _detector_nb;
        G4double _time;
        G4double _e_dep = 0;
        G4double _time = 0;
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
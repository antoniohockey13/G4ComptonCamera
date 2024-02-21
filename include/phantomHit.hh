#ifndef PHANTOMHIT_HH
#define PHANTOMHIT_HH

// Copy from example B2a
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"


class phantomHit : public G4VHit
{
    public:
        phantomHit() = default;
        phantomHit(const phantomHit&) = default;
        ~phantomHit() override = default;

        //operators
        phantomHit& operator=(const phantomHit&) = default;
        G4bool operator==(const phantomHit&) const;

        inline void* operator new(size_t);
        inline void operator delete(void*);
        //methods from base class
        void Draw() override;
        void Print() override;

        //setters
        void SetTrackID(G4int track_id) { _track_id = track_id;};
        //Transform name of the from DetectorX to int(X)
        void SetDetectorNb(G4String detector_name) { _detector_nb = std::stoi(detector_name.erase(0,8));};
        void SetPos(G4ThreeVector pos) { _pos = pos;};
        void SetPreMom(G4ThreeVector pre_mom) { _pre_mom = pre_mom;};
        void SetPostMom(G4ThreeVector post_mom) { _post_mom = post_mom;};
        void SetParticleID(G4int particle_id) { _particle_id = particle_id;};
        void SetParentID(G4int parent_id) { _parent_id = parent_id;};
        void SetTime(G4double time) { _time = time;};
        void SetPostKineticEnergy(G4double post_kinetic_energy) { _post_kinetic_energy = post_kinetic_energy;};
        void SetPreKineticEnergy(G4double pre_kinetic_energy) { _pre_kinetic_energy = pre_kinetic_energy;};
        void SetProcessName(G4String process_name) { _process_name = process_name;};
        void SetStepLength(G4double step_length) { _step_length = step_length;};

        //getters
        G4int GetTrackID() const { return _track_id;};
        G4int GetParticleID() const { return _particle_id;};
        G4int GetParentID() const { return _parent_id;};
        G4int GetDetectorNb() const { return _detector_nb;};
        G4double GetTime() const { return _time;};
        G4ThreeVector GetPos() const { return _pos;};
        G4ThreeVector GetPreMom() const { return _pre_mom;};
        G4ThreeVector GetPostMom() const { return _post_mom;};
        G4double GetPreKineticEnergy() const { return _pre_kinetic_energy;};
        G4double GetPostKineticEnergy() const { return _post_kinetic_energy;};
        G4double GetEnergyLost() const { return _pre_kinetic_energy-_post_kinetic_energy;};
        G4double GetComptonAngle() const { return _pre_mom.angle(_post_mom);};
        G4String GetProcessName() const { return _process_name;};
        G4double GetStepLength() const { return _step_length;};

    private:
        G4int _track_id;
        G4int _particle_id;
        G4int _parent_id;
        G4int _detector_nb;
        G4double _time;
        G4double _e_dep;
        G4double _pre_kinetic_energy;
        G4double _post_kinetic_energy;
        G4ThreeVector _pos;
        G4ThreeVector _pre_mom;
        G4ThreeVector _post_mom;
        G4String _process_name;
        G4double _step_length;
};


using phantomHitsCollection = G4THitsCollection<phantomHit>;
extern G4ThreadLocal G4Allocator<phantomHit>* phantomHitAllocator;

inline void* phantomHit::operator new(size_t) 
{
    if (!phantomHitAllocator)
    {
        phantomHitAllocator = new G4Allocator<phantomHit>;
    }
    return (void*)phantomHitAllocator->MallocSingle();
}

//

inline void phantomHit::operator delete(void* hit)
{
    phantomHitAllocator->FreeSingle((phantomHit*) hit);
}

#endif
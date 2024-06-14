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
        void SetPos(G4ThreeVector pos) { _pos = pos;};
        void SetMom(G4ThreeVector mom) { _pre_mom = mom;};
        void SetParticleID(G4int particle_id) { _particle_id = particle_id;};
        void SetKineticEnergy(G4double kinetic_energy) { _kinetic_energy = kinetic_energy;};

        //getters
        G4int GetParticleID() const { return _particle_id;};
        G4ThreeVector GetPos() const { return _pos;};
        G4ThreeVector GetMom() const { return _pre_mom;};
        G4double GetKineticEnergy() const { return _kinetic_energy;};

    private:
        G4int _particle_id;
        G4double _kinetic_energy;
        G4ThreeVector _pos;
        G4ThreeVector _pre_mom;
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

inline void phantomHit::operator delete(void* hit)
{
    phantomHitAllocator->FreeSingle((phantomHit*) hit);
}

#endif

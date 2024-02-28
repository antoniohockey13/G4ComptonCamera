#include "phantomHit.hh"

#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"


#include <iomanip>

G4ThreadLocal G4Allocator<phantomHit>* phantomHitAllocator = nullptr;


G4bool phantomHit::operator==(const phantomHit& right) const
{
    return (this == &right) ? true : false;
}

// Draw hit

void phantomHit::Draw()
{
    G4VVisManager* vis_manager = G4VVisManager::GetConcreteInstance();
    if(vis_manager)
    {
        G4Circle circle(_pos);
        circle.SetScreenSize(4.);
        circle.SetFillStyle(G4Circle::filled);
        G4VisAttributes attribs(G4Colour::Blue());
        circle.SetVisAttributes(attribs);
        vis_manager->Draw(circle);
    }
}

void phantomHit::Print()
{
    G4cout
    << "Particle: "
    << _particle_id
    << " Edep: "
    << std::setw(7) << G4BestUnit(_edep,"Energy")
    << " Position: "
    << std::setw(7) << G4BestUnit( _pos,"Length")
    << G4endl;
}


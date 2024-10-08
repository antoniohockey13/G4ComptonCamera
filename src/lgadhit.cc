#include "lgadhit.hh"

#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"


#include <iomanip>

G4ThreadLocal G4Allocator<lgadHit>* lgadHitAllocator = nullptr;

G4bool lgadHit::operator==(const lgadHit& right) const
{
    return (this == &right) ? true : false;
}

// Draw hit

void lgadHit::Draw()
{
    G4VVisManager* vis_manager = G4VVisManager::GetConcreteInstance();
    if(vis_manager)
    {
        G4Circle circle(_pos);
        circle.SetScreenSize(4.);
        circle.SetFillStyle(G4Circle::filled);
        G4VisAttributes attribs(G4Colour::Red());
        circle.SetVisAttributes(attribs);
        vis_manager->Draw(circle);
    }
}

void lgadHit::Print()
{
    G4cout
    << "  trackID: " << _track_id << " Detector Nb: " << _detector_nb
    << " Edep: "
    << std::setw(7) << G4BestUnit(_e_dep,"Energy")
    << " Position: "
    << std::setw(7) << G4BestUnit( _pos,"Length")
    << G4endl;
}
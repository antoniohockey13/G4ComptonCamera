#include "lgadhit.hh"
// Copy from example B2a
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"


#include <iomanip>

G4ThreadLocal G4Allocator<LGADHit>* LGADHitAllocator = nullptr;

//

G4bool LGADHit::operator==(const LGADHit& right) const
{
    return (this == &right) ? true : false;
}

// Draw hit

void LGADHit::Draw()
{
    G4VVisManager* vis_manager = G4VVisManager::GetConcreteInstance();
    if(vis_manager)
    {
        G4Circle circle(_f_pos);
        circle.SetScreenSize(4.);
        circle.SetFillStyle(G4Circle::filled);
        G4VisAttributes attribs(G4Colour::Red());
        circle.SetVisAttributes(attribs);
        vis_manager->Draw(circle);
    }
}

// Print hit information

void LGADHit::Print()
{
    G4cout
    << "  trackID: " << _f_track_id << " chamberNb: " << _f_detector_nb
    << "Edep: "
    << std::setw(7) << G4BestUnit(_f_e_dep,"Energy")
    << " Position: "
    << std::setw(7) << G4BestUnit( _f_pos,"Length")
    << std::setw(7) << G4BestUnit( _f_mom,"Momentun")
    << G4endl;
}


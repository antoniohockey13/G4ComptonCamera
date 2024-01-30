#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "globals.hh" 
#include "tls.hh"
#include "G4VUserDetectorConstruction.hh" // for detector construction
#include "G4VPhysicalVolume.hh" // for physical volumes
#include "G4LogicalVolume.hh" // for logical volumes
#include "G4Box.hh" // for box geometry
#include "G4PVPlacement.hh" // for placement of volumes
#include "G4NistManager.hh" // for NIST materials
#include "G4SystemOfUnits.hh" // for units
#include "G4GenericMessenger.hh" // for UI commands
#include "G4SDManager.hh" // for sensitive detector manager
#include "G4VSensitiveDetector.hh" // for sensitive detector

#include "lgadSD.hh" // for detector class


class ComptCameraDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    ComptCameraDetectorConstruction();
    // Get world width
    G4double GetWorldWidth() {return _world_width;}
    // Set sensitive material 
    //G4LogicalVolume *GetScoringVolume1() const {return f_scoring_volume1;}
    //G4LogicalVolume *GetScoringVolume2() const {return f_scoring_volume2;}
    ~ComptCameraDetectorConstruction();


      virtual G4VPhysicalVolume* Construct();

    private:
      std::map<G4int, G4LogicalVolume*> _detector_map;
      

      G4LogicalVolume *_logic_world;
      G4VPhysicalVolume *_phys_world;
      G4Material *_world_material, *_detector_material;
    

    void _DefineMaterials();
    void _ConstructWorld();
    void _ConstructDetector(G4int const _detector_number, G4double const _detector_distance);
    virtual void ConstructSDandField();

    // Distance between source and detectors
    std::map<G4int, G4double> _detector_distance;
    
    // World dimension in x axis, width
    G4double _world_width;
    // World dimension in y axis, height 
    G4double _world_height;
    // World dimension in z axis, depth	
    G4double _world_depth; 
    
    // Detector size in x and y axis
    G4double _detector_size; 
    // Detector thickness in z axiss
    G4double _detector_thickness;
};
#endif

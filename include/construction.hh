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
    G4LogicalVolume *GetScoringVolume() {return f_scoring_volume1, f_scoring_volume2;}
    ~ComptCameraDetectorConstruction();


    virtual G4VPhysicalVolume* Construct();

  private:
    struct DetectorInfo
    {
      G4Box* solid;
      G4LogicalVolume* logic;
      G4VPhysicalVolume* physical;
    };
    std::map<G4int, DetectorInfo> _detector_map;
    

    G4LogicalVolume *_logic_world, *_logic_detector, *_logic_sensitive, *f_scoring_volume1, *f_scoring_volume2;
    G4VPhysicalVolume *_phys_world, *_phys_detector, *_phys_sensitive;
    G4Box *_solid_world, *_solid_detector, *_solid_sensitive;
    G4Material *_world_material, *_detector_material;
    

    void _DefineMaterials();
    void _ConstructWorld(G4double const _world_width, G4double const _world_height, G4double const _world_depth);
    void _ConstructDetector(G4int const _detector_number, G4double const _detector_distance, G4double const _detector_thickness, G4double const _detector_size, G4double const _world_width);
    virtual void ConstructSDandField();

    // Distance between source and detector 1
    G4double _detector1_distance = -1; 
    // Distance between source and detector 2
    G4double _detector2_distance = -1; 
    // Angle between source and detectors not used yet
	  // G4double _angle = 0; 
    
    // World dimension in x axis, width
    G4double _world_width = -1;
    // World dimension in y axis, height 
    G4double _world_height = -1;
    // World dimension in z axis, depth	
    G4double _world_depth = -1; 
    
    // Detector size in x and y axis
    G4double _detector_size = -1; 
    // Detector thickness in z axiss
    G4double _detector_thickness = -1;
};
#endif

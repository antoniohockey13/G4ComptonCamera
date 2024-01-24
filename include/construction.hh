#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh" // for detector construction
#include "G4VPhysicalVolume.hh" // for physical volumes
#include "G4LogicalVolume.hh" // for logical volumes
#include "G4Box.hh" // for box geometry
#include "G4PVPlacement.hh" // for placement of volumes
#include "G4NistManager.hh" // for NIST materials
#include "G4SystemOfUnits.hh" // for units
#include "G4GenericMessenger.hh" // for UI commands

// #include "detector.hh" // for detector class

class ComptCameraDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  ComptCameraDetectorConstruction();
  ~ComptCameraDetectorConstruction();

  virtual G4VPhysicalVolume* Construct();

private:
    G4LogicalVolume *_logic_world, *_logic_detector1, *_logic_sensitive1, *_logic_detector2, *_logic_sensitive2;
    G4VPhysicalVolume *_phys_world, *_phys_detector1, *_phys_sensitive1, *_phys_detector2, *_phys_sensitive2;
    G4Box *_solid_world, *_solid_detector1, *_solid_sensitive1, *_solid_detector2, *_solid_sensitive2;
    G4Material *_world_material, *_detector_material;
    
    void _DefineMaterials();
    void _ConstructWorld(G4double const _world_width, G4double const _world_height, G4double const _world_depth);
    void _ConstructDetector1(G4double const _detector1_distance, G4double const _detector_thickness, G4double const _detector_size, G4double const _world_width);
    void _ConstructDetector2(G4double const _detector2_distance, G4double const _detector_thickness, G4double const _detector_size, G4double const _world_width);

//    virtual void ConstructSDandField();

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

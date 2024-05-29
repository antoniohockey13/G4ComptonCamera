#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh" 
#include "G4NistManager.hh" 

class ComptCameraDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    ComptCameraDetectorConstruction();
    // Get world width
    G4double GetWorldHeight() {return _world_height;}

    ~ComptCameraDetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct();

    private:
      
      G4LogicalVolume *_logic_world, *_logic_phantom_detector;
      G4VPhysicalVolume *_phys_world;
      G4Material *_world_material, *_target_material, *_detector_material;
    

    void _DefineMaterials();
    void _ConstructWorld();
    void _ConstructPhantomDetector(G4double detector_size, G4double detector_thickness, G4double detector_distance);
    void _ConstructTarget(G4double angle);
    virtual void ConstructSDandField();

    // Distance between source and detectors
    G4int _detector_distance;
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
    // Angle of the target
    G4double _angle;

};
#endif

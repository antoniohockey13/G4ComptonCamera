#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh" 
#include "G4NistManager.hh" 

class ComptCameraDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    ComptCameraDetectorConstruction();
    // Get world width
    G4double GetWorldWidth() {return _world_width;}
    G4bool GetPhantomDetector() {return _phantom_detector;}

    ~ComptCameraDetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct();

    private:
      std::map<G4String, G4LogicalVolume*> _detector_map;
      
      G4LogicalVolume *_logic_world, *_logic_phantom_detector;
      G4VPhysicalVolume *_phys_world;
      G4Material *_world_material, *_detector_material, *_pcb_material;
    

    void _DefineMaterials();
    void _ConstructWorld();
    void _ConstructDetectorsGrid(G4int y_nb_detector, G4int z_nb_detector, G4int const _detector_number, G4double const _detector_distance);
    void _ConstructPhantomDetector();
    void _ConstructPCB(G4double const _detector_distance);
    virtual void ConstructSDandField();

    // Distance between source and detectors
    G4int _detector_distance;
    // Number of detectors in y and z axis
    G4int _y_nb_detector;
    G4int _z_nb_detector;
    G4int _detector_number;   
    G4int _number;
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
    // Space between subdetectors
    G4double _spacing;
    // Phantom detector
    G4bool _phantom_detector;
};
#endif

#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh" 
#include "G4NistManager.hh" 

class G4GenericMessenger;

class ComptCameraDetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        ComptCameraDetectorConstruction();
        ~ComptCameraDetectorConstruction();
        // Get world width
        G4double GetWorldWidth() {return _world_width;}
        G4bool IsPhantomDetector() {return _phantom_detector;}

        G4VPhysicalVolume* Construct() override;

    private:
        std::map<G4int, G4LogicalVolume*> _detector_map;
        G4LogicalVolume * _logic_phantom_detector;
        G4Material *_world_material, *_detector_material;

        void _DefineMaterials();
        void _ConstructWorld();
        void _ConstructDetector(G4LogicalVolume * logic_world,
                G4int detector_number, 
                G4double distance,
                G4double thickness);
        void _ConstructPhantomDetector(G4LogicalVolume* logic_world);
        virtual void ConstructSDandField() override;

        // Distance between source and detectors
        std::map<G4int, std::pair<G4double, G4double>> _detector_distance_thickness;
        G4int _detector_number;   
        G4int _number;
        // World dimension in x axis, width
        G4double _world_width;
        // World dimension in y axis, height 
        G4double _world_height;
        // World dimension in z axis, depth	
        G4double _world_depth; 
        
        // Detector size in x and y axis
        G4double _detector_size_y; 
        // Detector thickness in z axiss
        G4double _detector_size_z;

        // Messenger
        G4GenericMessenger *_messenger;
        
        // Phantom detector
        G4bool _phantom_detector;
};
#endif

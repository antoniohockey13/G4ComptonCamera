#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh" 
#include "G4NistManager.hh" 
#include "G4Colour.hh"
#include <map>
#include "globals.hh"

class G4GenericMessenger;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class ChangeCrossSection;

class ComptCameraDetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        ComptCameraDetectorConstruction();
        ~ComptCameraDetectorConstruction();
        // Get world width
        G4double GetWorldWidth() {return _world_width;}
        G4bool IsPhantomDetector() {return _phantom_detector;}
        
        G4VPhysicalVolume* Construct();

    private:
        void _DefineMaterials();
        G4VPhysicalVolume* _ConstructWorld();
        void _ConstructPhantomDetector();
        virtual void ConstructSDandField() override;
        void _ConstructModuleStack(const G4String& moduleName, G4double xFrontFace, G4double yModuleCenter, G4double zModuleCenter);
        void _ConstructPixelGrid(G4LogicalVolume* logic_active, const G4String& moduleName);
        G4LogicalVolume* _PlaceLayer(const G4String& name, G4Material* material, G4double thickness, G4double sizeY, G4double sizeZ, G4double xCenter, G4double yCenter, G4double zCenter, const G4Colour& colour);

    private:
        // Geometry containers
        std::map<G4String, G4LogicalVolume*> _detector_map;

        G4LogicalVolume* _logic_world;
        G4LogicalVolume*_logic_phantom_detector;
        // Materials
        G4Material* _world_material;
        G4Material* _detector_material;
        G4Material* _pcb_material;
        G4Material* _duct_tape_material;
        G4Material* _service_hybrid_material;
        G4Material* _epoxy_material;
        G4Material* _aln_material;
        G4Material* _laird_material;
        G4Material* _tin_material;

        ChangeCrossSection* _compton_bias_operator = nullptr;

        // World dimensions
        G4double _world_width;
        G4double _world_height;
        G4double _world_depth;
        // Module transverse dimensions
        G4double _module_size_y;
        G4double _module_size_z;
        // LGAD / ETROC transverse sizes
        G4double _lgad_size_y;
        G4double _lgad_size_z;
        G4double _etroc_size_y;
        G4double _etroc_size_z;
        // Layer thicknesses
        G4double _ducttape_thickness;
        G4double _thermalpad_thickness;
        G4double _lairdfilm_thickness;
        G4double _alnbase_thickness;
        G4double _gluelgad_thickness;
        G4double _lgad_thickness;
        G4double _lgad_active_thickness;
        G4double _lgad_sub_thickness;
        G4double _bumpbonds_thickness;
        G4double _etroc_thickness;
        G4double _glueetroc_thickness;
        G4double _modulepcb_thickness;
        G4double _connectorsgap_thickness;
        G4double _readoutboard_thickness;
        G4double _services_thickness;
        // Pixel grid
        G4int _y_nb_detector;
        G4int _z_nb_detector;
        G4int _detector_number;
        G4double _detector_size;
        G4double _detector_thickness;
        G4double _spacing;
        // Distances from source plane
        G4double _detector1_distance;
        G4double _detector2_distance;
        // Phantom detector
        G4bool _phantom_detector;
        // Messenger
        G4GenericMessenger *_messenger;
};
#endif
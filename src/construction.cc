#include "construction.hh"
#include "lgadSD.hh" 
#include "phantomSD.hh"

#include "globals.hh" 

#include "G4GenericMessenger.hh"
#include "G4VisManager.hh" 
#include "G4VPhysicalVolume.hh" 
#include "G4LogicalVolume.hh"
#include "G4Box.hh" 
#include "G4PVPlacement.hh" 
#include "G4SystemOfUnits.hh" 
#include "G4SDManager.hh" 
#include "G4VSensitiveDetector.hh" 
#include "G4NistManager.hh" 

ComptCameraDetectorConstruction::ComptCameraDetectorConstruction() :
    _logic_phantom_detector(nullptr),
    _world_material(nullptr),
    _detector_material(nullptr),
    _messenger(nullptr)
{
    // Define world size including walls 
    _world_width = 682*mm;
    _world_height = 562*mm;
    _world_depth = 354*mm;

    _detector_size_y = 300*mm;
    _detector_size_z = 200*mm;
    _detector_number = 2;
    //Define map with distances
    for (G4int i = 1; i < _detector_number; i++)
    {
        // Distance and thickness
        _detector_distance_thickness[i] = std::make_pair(100*(i)*mm, 150*um);
        // Timing 50um
    }
    _detector_distance_thickness[1] = std::make_pair(100*mm, 150*um);
    _detector_distance_thickness[2] = std::make_pair(200*mm, 150*um);

    //Messenger
    _number = 1;
    // Change number of detectors not working
    _messenger = new G4GenericMessenger(this, "/ComptCamera/detector/", "Detector control");
    // _messenger->DeclareProperty("detector_size", _detector_size, "Detector size, /run/reinitializeGeometry to update");
    //_messenger->DeclareProperty("detector_thickness", _detector_thickness, "Detector thickness, /run/reinitializeGeometry to update");
    _messenger->DeclareProperty("detector_number", _detector_number, "Number of detectors");
    _messenger->DeclareProperty("Number", _number, "Select with detector you want to move with /ComptCamera/detector/detector_distance");
    // Messenger does NOT work with maps
    //_messenger->DeclareProperty("detector_distance", _detector_distance[_number], "Detector distance, /run/reinitializeGeometry to update");

    _phantom_detector = false;
    _DefineMaterials();
}

ComptCameraDetectorConstruction::~ComptCameraDetectorConstruction()
{
    if( _messenger != nullptr )
    {
        delete _messenger;
        _messenger = nullptr;
    }
}

void ComptCameraDetectorConstruction::_DefineMaterials()
{
    G4NistManager* nist = G4NistManager::Instance();
    // World material is air
    _world_material = nist->FindOrBuildMaterial("G4_AIR");
    // Detector material is silicon (LGAD detectors)
    _detector_material = nist->FindOrBuildMaterial("G4_Si");
}

G4VPhysicalVolume* ComptCameraDetectorConstruction::Construct()
{
    auto * phys_vol = _ConstructWorld();
    
    // Loop over detectors and construct them
    for (auto &detector : _detector_distance_thickness)
    {
        _ConstructDetector(_logic_world, detector.first, detector.second.first, detector.second.second);
    }

    if(_phantom_detector)
    {
        _ConstructPhantomDetector(_logic_world);
    }
    
    // Create world physical volume
    return  phys_vol;
}

G4VPhysicalVolume* ComptCameraDetectorConstruction::_ConstructWorld()
{
    // Create world solid
    G4Box* solid_world = new G4Box("World", _world_width/2, _world_height/2, _world_depth/2); 
    // Create world logical volume
    _logic_world = new G4LogicalVolume(solid_world, _world_material, "World");
    _logic_world->SetVisAttributes(G4Color(0.6784,0.8471,0.902,0.3));
    // Create world physical volume
    return new G4PVPlacement(0, G4ThreeVector(0, 0, 0), _logic_world, "World", 0, false, 0); 
}

// Set sensitive detector to logical volume
void ComptCameraDetectorConstruction::ConstructSDandField()
{
    // Copy from example B2a
    G4String lgadSDname = "/lgadSD";
    auto algadSD = new lgadSD(lgadSDname, "lgadHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(algadSD);

    // Make a loop to set sensitive detector to all detectors
    for (auto &detector : _detector_map)
    {
        detector.second->SetSensitiveDetector(algadSD);
    }

    if( _phantom_detector )
    {
        G4String phantomSDname = "/phantomSD";
        auto aphantomSD = new phantomSD(phantomSDname, "phantomHitsCollection");
        G4SDManager::GetSDMpointer()->AddNewDetector(aphantomSD);
        _logic_phantom_detector->SetSensitiveDetector(aphantomSD);
    }
}
    
void ComptCameraDetectorConstruction::_ConstructDetector(G4LogicalVolume * logic_world, G4int detector_number, G4double distance, G4double thickness)
{
    // Create detector solid, length arguments half of the actual length
    G4String name = "Detector" + std::to_string(detector_number);

    G4Box* solid_detector = new G4Box(name, thickness/2, _detector_size_y/2, _detector_size_z/2); 
    // Create detector logical volume
    _detector_map[detector_number] = new G4LogicalVolume(solid_detector, _detector_material, name);
    _detector_map[detector_number]->SetVisAttributes(G4Color(1.0,0.8,0.,0.9));
    // Create detector physical volume
    new G4PVPlacement(0, G4ThreeVector(distance-_world_width/2, 0, 0), _detector_map[detector_number], name, logic_world, false, 0);
    // 0 rotation,  translation, logical volume, name, mother volume, boolean operation, copy numbers
}

void ComptCameraDetectorConstruction::_ConstructPhantomDetector(G4LogicalVolume * logic_world)
{   
    // Create phantom detector solid, length arguments half of the actual length
    G4String name = "PhantomDetector";

    G4Box* solid_phantom_detector = new G4Box(name, 1*mm, _world_height/2, _world_depth/2); 
    // Create phantom detector logical volume
    _logic_phantom_detector = new G4LogicalVolume(solid_phantom_detector, _world_material, name);
    _logic_phantom_detector->SetVisAttributes(G4Color(0.6784,0.8471,0.902,0.3));
    // Create phantom detector physical volume
    new G4PVPlacement(0, G4ThreeVector(-_world_width/2+_detector_distance_thickness[1].first/2, 0, 0), _logic_phantom_detector, name, logic_world, false, 0);

}

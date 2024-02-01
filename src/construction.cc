#include "construction.hh"


ComptCameraDetectorConstruction::ComptCameraDetectorConstruction()
{
    // Define world size including walls 
    // TO DO : Define operating space and walls
    _world_width = 682*mm;
    _world_height = 562*mm;
    _world_depth = 354*mm;

    _detector_size = 300*mm;
    _detector_thickness = 2*mm;
    _detector_number = 2;
    //Define map with distances
    for (G4int i = 1; i < _detector_number; i++)
    {
        _detector_distance[i] = 100*(i)*mm;
    }
    _detector_distance[1] = 100*mm;
    _detector_distance[2] = 200*mm;

    //Messenger
    _number = 1;
    // Cange number of detectors not working
    _messenger = new G4GenericMessenger(this, "/ComptCamera/detector/", "Detector control");
    _messenger->DeclareProperty("detector_size", _detector_size, "Detector size, /run/reinitializeGeometry to update");
    _messenger->DeclareProperty("detector_thickness", _detector_thickness, "Detector thickness, /run/reinitializeGeometry to update");
    _messenger->DeclareProperty("detector_number", _detector_number, "Number of detectors");
    _messenger->DeclareProperty("Number", _number, "Select with detector you want to move with /ComptCamera/detector/detector_distance");
    // Messenge does NOT work with maps
    _messenger->DeclareProperty("detector_distance", _detector_distance[_number], "Detector distance, /run/reinitializeGeometry to update");

    _DefineMaterials();
}
ComptCameraDetectorConstruction::~ComptCameraDetectorConstruction()
{
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
    // Construct world
    _ConstructWorld();

    // Loop over detectors and construct them
    for (auto &detector : _detector_distance    )
    {
        _ConstructDetector(detector.first, detector.second);
    }

    return _phys_world;
}

void ComptCameraDetectorConstruction::_ConstructWorld()
{
    // Create world solid, length arguments half of the actual length
    G4Box* solid_world = new G4Box("World", _world_width/2, _world_height/2, _world_depth/2); 
    // Create world logical volume
    _logic_world = new G4LogicalVolume(solid_world, _world_material, "World"); 
    // Create world physical volume
    _phys_world = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), _logic_world, "World", 0, false, 0); 
}

// Set sensitive detector to logical volume

void ComptCameraDetectorConstruction::ConstructSDandField()
{
    // Copy from example B2a
    G4String lgadSDname = "/lgadSD";
    auto algadSD = new LGADSD(lgadSDname, "LGADHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(algadSD);

    // Make a loop to set sensitive detector to all detectors
    for (auto &detector : _detector_map)
    {
        detector.second->SetSensitiveDetector(algadSD);
    }
}
    
void ComptCameraDetectorConstruction::_ConstructDetector(G4int detector_number, G4double distance)
{
    // Create detector solid, length arguments half of the actual length
    G4String name = "Detector" + std::to_string(detector_number);

    G4Box* solid_detector = new G4Box(name, _detector_thickness/2, _detector_size/2, _detector_size/2); 
    // Create detector logical volume
    _detector_map[detector_number] = new G4LogicalVolume(solid_detector, _detector_material, name);
    
    // Create detector physical volume
    new G4PVPlacement(0, G4ThreeVector(distance-_world_width/2, 0, 0), _detector_map[detector_number], name, _logic_world, false, 0);
    // 0 rotation,  translation, logical volume, name, mother volume, boolean operation, copy numbers
}

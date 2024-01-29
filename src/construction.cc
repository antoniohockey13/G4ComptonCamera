#include "construction.hh"


ComptCameraDetectorConstruction::ComptCameraDetectorConstruction()
{
    // Define world size including walls 
    // TO DO : Define operating space and walls
    _world_width = 682*mm;
    _world_height = 562*mm;
    _world_depth = 354*mm;

    _detector_size = 2*mm;
    _detector_thickness = 0.1*mm;

    // Define detector 1 distance to source
    _detector1_distance = 100*mm;

    // Define detector 2 distance to source
    _detector2_distance = 200*mm;

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
    _ConstructWorld();
    // Cant do loop for detector construction becuase of different distances
    _ConstructDetector(1, _detector1_distance);
    _ConstructDetector(2, _detector2_distance);

    return _phys_world;
}

void ComptCameraDetectorConstruction::_ConstructWorld()
{
    // Create world solid, length arguments half of the actual length
    _solid_world = new G4Box("World", _world_width/2, _world_height/2, _world_depth/2); 
    // Create world logical volume
    _logic_world = new G4LogicalVolume(_solid_world, _world_material, "World"); 
    // Create world physical volume
    _phys_world = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), _logic_world, "World", 0, false, 0);  
    // 0 rotation,  translation, logical volume, name, mother volume, boolean operation, copy number
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
        detector.second.logic->SetSensitiveDetector(algadSD);
    }
}

void ComptCameraDetectorConstruction::_ConstructDetector(G4int detector_number, G4double distance)
{
    // Create detector solid, length arguments half of the actual length
    G4String name = "Detector" + std::to_string(detector_number);
    _solid_detector = new G4Box(name, _detector_size/2, _detector_size/2, _detector_thickness/2); 
    // Create detector logical volume
    _logic_detector = new G4LogicalVolume(_solid_detector, _detector_material, name);
    
    // Create detector physical volume
    _phys_detector = new G4PVPlacement(0, G4ThreeVector(distance-_world_width/2, 0, 0), _logic_detector, "Detector", _logic_world, false, 0);
    // 0 rotation,  translation, logical volume, name, mother volume, boolean operation, copy number

    // Add detector to map
    DetectorInfo detector_info;
    detector_info.logic = _logic_detector;
    _detector_map[detector_number] = detector_info;
}

#include "construction.hh"

ComptCameraDetectorConstruction::ComptCameraDetectorConstruction()
{
    // Define world size including walls 
    // TO DO : Define operating space and walls
    world_width = 682*mm;
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
    _ConstructWorld(world_width, _world_height, _world_depth);
    _ConstructDetector1(_detector1_distance, _detector_thickness, _detector_size, world_width);
    _ConstructDetector2(_detector2_distance, _detector_thickness, _detector_size, world_width);

    return _phys_world;
}

void ComptCameraDetectorConstruction::_ConstructWorld(G4double width, G4double height, G4double depth)
{
    // Create world solid, length arguments half of the actual length
    _solid_world = new G4Box("World", width/2, height/2, depth/2); 
    // Create world logical volume
    _logic_world = new G4LogicalVolume(_solid_world, _world_material, "World"); 
    // Create world physical volume
    _phys_world = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), _logic_world, "World", 0, false, 0);  
    // 0 rotation,  translation, logical volume, name, mother volume, boolean operation, copy number
}
/*
// Set sensitive detector to logical volume
void ComptCameraDetectorConstruction::ConstructSDandField()
{
    _logic_detector1->SetSensitiveDetector(f_scoring_volume1);
    _logic_detector2->SetSensitiveDetector(f_scoring_volume2);
}
*/
void ComptCameraDetectorConstruction::_ConstructDetector1(G4double distance, G4double thickness, G4double size, G4double w_width)
{
    // Create detector solid, length arguments half of the actual length
    _solid_detector1 = new G4Box("Detector1", size/2, size/2, thickness/2);
    // Create detector logical volume 
    _logic_detector1 = new G4LogicalVolume(_solid_detector1, _detector_material, "Detector1");
    f_scoring_volume1 = _logic_detector1;
    // Create detector physical volume
    _phys_detector1 = new G4PVPlacement(0, G4ThreeVector(distance-w_width/2, 0 , 0), _logic_detector1, "Detector1", _logic_world, false, 0);
    // 0 rotation,  translation, logical volume, name, mother volume, boolean operation, copy number
}

void ComptCameraDetectorConstruction::_ConstructDetector2(G4double distance, G4double thickness, G4double size, G4double w_width)
{
    // Create detector solid, length arguments half of the actual length
    _solid_detector2 = new G4Box("Detector2", size/2, size/2, thickness/2); 
    // Create detector logical volume
    _logic_detector2 = new G4LogicalVolume(_solid_detector2, _detector_material, "Detector2");
    f_scoring_volume2 = _logic_detector2;
    // Create detector physical volume
    _phys_detector2 = new G4PVPlacement (0, G4ThreeVector(distance-w_width/2, 0, 0), _logic_detector2, "Detector2", _logic_world, false, 0);
    // 0 rotation,  translation, logical volume, name, mother volume, boolean operation, copy number
}


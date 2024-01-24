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

    DefineMaterials();
}
ComptCameraDetectorConstruction::~ComptCameraDetectorConstruction()
{
}

void ComptCameraDetectorConstruction::DefineMaterials()
{
    G4NistManager* nist = G4NistManager::Instance();
    // World material is air
    _worldMaterial = nist->FindOrBuildMaterial("G4_AIR"); 
    // Detector material is silicon (LGAD detectors)
    detectorMaterial = nist->FindOrBuildMaterial("G4_Si");
}

G4VPhysicalVolume* ComptCameraDetectorConstruction::Construct()
{
    _ConstructWorld(_world_width, _world_height, _world_depth);
    _ConstructDetector1(_detector1_distance, _detector_thickness, _detector_size, _world_width);
    _ConstructDetector2(_detector2_distance, _detector_thickness, _detector_size, _world_width);

    return _physWorld;
}

void ComptCameraDetectorConstruction::ConstructWorld(G4double width, G4double height, G4double depth)
{
    // Create world solid, length arguments half of the actual length
    _solidWorld = new G4Box("World", width/2, height/2, depth/2); 
    // Create world logical volume
    _logicWorld = new G4LogicalVolume(solidWorld, worldMaterial, "World"); 
    // Create world physical volume
    _physWorld = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicWorld, "World", 0, false, 0); 
    // CANT change orgin NEED TO RE DO GEOMETRY 
    // 0 rotation,  translation, logical volume, name, mother volume, boolean operation, copy number
}

void ComptCameraDetectorConstruction::ConstructDetector1(G4double distance, G4double thickness, G4double size, G4double w_width)
{
    // Create detector solid, length arguments half of the actual length
    _solidDetector1 = new G4Box("Detector1", size/2, size/2, thickness/2);
    // Create detector logical volume 
    _logicDetector1 = new G4LogicalVolume(solidDetector1, detectorMaterial, "Detector1");
    // Create detector physical volume
    _physDetector1 = new G4PVPlacement(0, G4ThreeVector(distance-w_width/2, 0 , 0), logicDetector1, "Detector1", logicWorld, false, 0);
    // 0 rotation,  translation, logical volume, name, mother volume, boolean operation, copy number
}

void ComptCameraDetectorConstruction::ConstructDetector2(G4double distance, G4double thickness, G4double size, G4double w_width)
{
    // Create detector solid, length arguments half of the actual length
    _solidDetector2 = new G4Box("Detector2", size/2, size/2, thickness/2); 
    // Create detector logical volume
    _logicDetector2 = new G4LogicalVolume(solidDetector2, detectorMaterial, "Detector2"); 
    // Create detector physical volume
    _physDetector2 = new G4PVPlacement (0, G4ThreeVector(distance-w_width/2, 0, 0), logicDetector2, "Detector2", logicWorld, false, 0);
    // 0 rotation,  translation, logical volume, name, mother volume, boolean operation, copy number
}

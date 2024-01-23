#include "construction.hh"


MyDetectorConstruction::MyDetectorConstruction()
{
    // Define world size including walls 
    // TO DO : Define operating space and walls
    world_width = 682*mm;
    world_height = 562*mm;
    world_depth = 354*mm;

    detector_size = 2*mm;
    detector_thickness = 0.1*mm;

    // Define detector 1 distance to source
    detector1_distance = 100*mm;

    // Define detector 2 distance to source
    detector2_distance = 200*mm;

    DefineMaterials();
}
MyDetectorConstruction::~MyDetectorConstruction()
{
}

void MyDetectorConstruction::DefineMaterials()
{
    G4NistManager* nist = G4NistManager::Instance();

    worldMaterial = nist->FindOrBuildMaterial("G4_AIR"); // World material is air

    detectorMaterial = nist->FindOrBuildMaterial("G4_Si"); // Detector material is silicon (LGAD detectors)
}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
    ConstructWorld(world_width, world_height, world_depth);
    ConstructDetector1(detector1_distance, detector_thickness, detector_size, world_width);
    ConstructDetector2(detector2_distance, detector_thickness, detector_size, world_width);

    return physWorld;
}

void MyDetectorConstruction::ConstructWorld(G4double width, G4double height, G4double depth)
{
    solidWorld = new G4Box("World", width/2, height/2, depth/2); // Create world solid, length arguments half of the actual length
    logicWorld = new G4LogicalVolume(solidWorld, worldMaterial, "World"); // Create world logical volume
    physWorld = new G4PVPlacement(0, G4ThreeVector(-width/2, 0, 0), logicWorld, "World", 0, false, 0); // Create world physical volume
    // 0 rotation,  translation, logical volume, name, mother volume, boolean operation, copy number
}

void MyDetectorConstruction::ConstructDetector1(G4double distance, G4double thickness, G4double size, G4double world_width)
{
    solidDetector1 = new G4Box("Detector1", size/2, size/2, thickness/2); // Create detector solid, length arguments half of the actual length
    logicDetector1 = new G4LogicalVolume(solidDetector1, detectorMaterial, "Detector1"); // Create detector logical volume
    physDetector1 = new G4PVPlacement(0, G4ThreeVector(distance-world_width/2, 0, 0), logicDetector1, "Detector1", logicWorld, false, 0); // Create detector physical volume
    // 0 rotation,  translation, logical volume, name, mother volume, boolean operation, copy number
}

void MyDetectorConstruction::ConstructDetector2(G4double distance, G4double thickness, G4double size, G4double world_width)
{
    solidDetector1 = new G4Box("Detector2", size/2, size/2, thickness/2); // Create detector solid, length arguments half of the actual length
    logicDetector1 = new G4LogicalVolume(solidDetector2, detectorMaterial, "Detector2"); // Create detector logical volume
    physDetector1 = new G4PVPlacement(0, G4ThreeVector(distance-world_width/2, 0, 0), logicDetector2, "Detector2", logicWorld, false, 0); // Create detector physical volume
    // 0 rotation,  translation, logical volume, name, mother volume, boolean operation, copy number
}

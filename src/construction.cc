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
    // Setting algadSD to all logical volumes with the same name
    // of "Lgad_LV".
    //SetSensitiveDetector("Detector", algadSD, true);

    G4cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << G4endl; 
    G4cout << " ComptCameraDetectorConstruction::ConstructSDandField()" << G4endl;
    G4cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << G4endl; 
/* // Copy from braqui work
    LGADSD *sensDet = new LGADSD("SensitiveDetector", "HitsDetector");
    _detector_map[1].logic->SetSensitiveDetector(sensDet);
    _detector_map[2].logic->SetSensitiveDetector(sensDet);
*/
}

void ComptCameraDetectorConstruction::_ConstructDetector(G4int detector_number, G4double distance)
{
    // Create detector solid, length arguments half of the actual length
    _solid_detector = new G4Box("Detector", _detector_size/2, _detector_size/2, _detector_thickness/2); 
    // Create detector logical volume
    _logic_detector = new G4LogicalVolume(_solid_detector, _detector_material, "Detector");
    //Create scoring volume
    if (detector_number == 1)
    {
        f_scoring_volume1 = _logic_detector;
    }
    else if (detector_number == 2)
    {
        f_scoring_volume2 = _logic_detector;
    }
    // Create detector physical volume
    _phys_detector = new G4PVPlacement(0, G4ThreeVector(distance-_world_width/2, 0, 0), _logic_detector, "Detector", _logic_world, false, 0);
    // 0 rotation,  translation, logical volume, name, mother volume, boolean operation, copy number

    // Store detector and its variables in a map
    DetectorInfo detector_info;
    detector_info.solid = _solid_detector;
    detector_info.logic = _logic_detector;
    detector_info.physical = _phys_detector;
    _detector_map[detector_number] = detector_info;
}

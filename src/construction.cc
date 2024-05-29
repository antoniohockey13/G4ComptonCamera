#include "construction.hh"
#include "phantomSD.hh"

#include "globals.hh" 

#include "G4GenericMessenger.hh"
#include "G4VisManager.hh" 
#include "G4VPhysicalVolume.hh" 
#include "G4LogicalVolume.hh"
#include "G4Box.hh" 
#include "G4Tubs.hh"
#include "G4PVPlacement.hh" 
#include "G4SystemOfUnits.hh" 
#include "G4SDManager.hh" 
#include "G4VSensitiveDetector.hh" 
#include "G4SubtractionSolid.hh"

ComptCameraDetectorConstruction::ComptCameraDetectorConstruction()
{

    _world_width =  20*mm;
    _world_height = 20*mm;
    _world_depth =  20*mm;
    // Define detector size
    _detector_size = 1*mm;
    // Timing 50um, 150um
    _detector_thickness = 1*mm;

    _detector_distance = 5*mm;

    _angle = 12*degree; 
    _DefineMaterials();
}

ComptCameraDetectorConstruction::~ComptCameraDetectorConstruction()
{
}

void ComptCameraDetectorConstruction::_DefineMaterials()
{
    G4NistManager* nist = G4NistManager::Instance();
    // World material is air
    _world_material = nist->FindOrBuildMaterial("G4_Galactic");
    // Detector material is silicon (LGAD detectors)
    _target_material = nist->FindOrBuildMaterial("G4_W");
    _detector_material = nist->FindOrBuildMaterial("G4_Si");
}

G4VPhysicalVolume* ComptCameraDetectorConstruction::Construct()
{
    // Construct world
    _ConstructWorld();
    _ConstructTarget(_angle);
    _ConstructPhantomDetector(_detector_size,_detector_thickness, _detector_distance);
    
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
    G4String phantomSDname = "/phantomSD";
    auto aphantomSD = new phantomSD(phantomSDname, "phantomHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(aphantomSD);
    _logic_phantom_detector->SetSensitiveDetector(aphantomSD);
}


void ComptCameraDetectorConstruction::_ConstructPhantomDetector(G4double size, G4double thickness, G4double distance)
{   
    // Create phantom detector
    G4String name = "PhantomDetector";

    G4Box* solid_phantom_detector = new G4Box(name, thickness/2, size/2, size/2); 
    // Create phantom detector logical volume
    _logic_phantom_detector = new G4LogicalVolume(solid_phantom_detector, _detector_material, name);
    
    // Create phantom detector physical volume
    new G4PVPlacement(0, G4ThreeVector(distance, 0, 0), _logic_phantom_detector, name, _logic_world, false, 0);
    // 0 rotation,  translation, logical volume, name, mother volume, boolean operation, copy numbers

}

void ComptCameraDetectorConstruction::_ConstructTarget(G4double angle)
{


    G4Tubs* solid_target_cylinder = new G4Tubs("cylinder", 0*mm, 2*mm, 4*mm, 0, 360*degree); 
    // Create target logical volume
    //G4LogicalVolume* logic_target_cylinder = new G4LogicalVolume(solid_target_cylinder, _world_material, "cylinder");
    
    G4Box* solid_target_box = new G4Box("box", 6*mm, 6*mm, 6*mm);
    
    G4RotationMatrix* yRot = new G4RotationMatrix;
    yRot->rotateY(-angle);                     
    G4ThreeVector Trans(0, 0*mm, 6*mm);

    G4SubtractionSolid* solid_target = new G4SubtractionSolid("target", solid_target_cylinder, solid_target_box, yRot, Trans);
    // Define rotation to align the target with the y-axis
    G4RotationMatrix* rot_y_cylinder = new G4RotationMatrix();
    rot_y_cylinder->rotateX(90*degree); // Rotate 90 degrees around the x-axis

    // Create target physical volume with the specified rotation
    G4LogicalVolume* logic_target = new G4LogicalVolume(solid_target, _target_material, "target");
    new G4PVPlacement(rot_y_cylinder, G4ThreeVector(0, 0, 0), logic_target, "target", _logic_world, false, 0);
}
#include "construction.hh"
#include "lgadSD.hh" 
#include "phantomSD.hh"

#include "globals.hh" 

#include "G4GenericMessenger.hh"
#include "G4VisManager.hh" 
#include "G4VPhysicalVolume.hh" 
#include "G4LogicalVolume.hh"
#include "G4Box.hh" 
#include "G4Tubs.hh"
#include "G4CutTubs.hh" 
#include "G4PVPlacement.hh" 
#include "G4SystemOfUnits.hh" 
#include "G4SDManager.hh" 
#include "G4VSensitiveDetector.hh" 
#include "G4NistManager.hh" 
#include "G4VisAttributes.hh"
#include "G4Region.hh"

ComptCameraDetectorConstruction::ComptCameraDetectorConstruction() :
    _logic_world(nullptr),
    _logic_phantom_detector(nullptr),
    _world_material(nullptr),
    _detector_material(nullptr),
    _pcb_material(nullptr),
    _tungsten(nullptr),
    _messenger(nullptr)
{
    // Define world size including walls 
    _world_width = 48*mm; //48
    _world_height = 56*mm;
    _world_depth = 35*mm;
    // Define detector size
    _detector_size = 1*mm;
    // Timing 50um, 150um
    _detector_thickness = 150*um;
    _detector_number = 1;

    //Define subdetector number
    _y_nb_detector = 2;
    _z_nb_detector = 2;

    //Define map with distances
    
    _detector_distance = 0*mm;
    // Define PCB thickness
    _pcb_thickness = 1*mm;

    //Space between subdetectors
    _spacing = 0.1*mm;

    _number = 1;
    //Messenger
    // Cange number of detectors not working
    _messenger = new G4GenericMessenger(this, "/ComptCamera/detector/", "Detector control");
    _messenger->DeclareProperty("detector_size", _detector_size, "Detector size, /run/reinitializeGeometry to update");
    _messenger->DeclareProperty("detector_thickness", _detector_thickness, "Detector thickness, /run/reinitializeGeometry to update");
    _messenger->DeclareProperty("detector_number", _detector_number, "Number of detectors");
    _messenger->DeclareProperty("Number", _number, "Select with detector you want to move with /ComptCamera/detector/detector_distance");
    // Messenge does NOT work with maps
    //_messenger->DeclareProperty("detector_distance", _detector_distance[_number], "Detector distance, /run/reinitializeGeometry to update");

    _phantom_detector = true;

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
    // World material is vacuum
    _world_material = nist->FindOrBuildMaterial("G4_Galactic");

    // Detector material is silicon (LGAD detectors)
    _detector_material = nist->FindOrBuildMaterial("G4_Si");
    //Epoxy material
    G4Material* epoxy = new G4Material("Epoxy", 1.3 * CLHEP::g / CLHEP::cm3, 3);
    epoxy->AddElement(nist->FindOrBuildElement("H"), 44);
    epoxy->AddElement(nist->FindOrBuildElement("C"), 15);
    epoxy->AddElement(nist->FindOrBuildElement("O"), 7);
    //PCB material
    _pcb_material = new G4Material("PCB", 1.7 * CLHEP::g / CLHEP::cm3, 3);    
    _pcb_material->AddMaterial(nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE"), 0.773);
    _pcb_material->AddMaterial(epoxy, 0.147);
    _pcb_material->AddElement(nist->FindOrBuildElement("Cl"), 0.08);

    // Anode
    _tungsten = nist->FindOrBuildMaterial("G4_W");
}

G4VPhysicalVolume* ComptCameraDetectorConstruction::Construct()
{
    // Construct world and phantom detector
    auto * phys_vol =_ConstructWorld();

    //_ConstructDetectorsGrid(_y_nb_detector, _z_nb_detector, 1, _detector_distance);
    //_ConstructPCB(_detector_distance+_detector_thickness+_pcb_thickness);
    _ConstructTungstenAnode();
    _ConstructCollimator();
    
    if(_phantom_detector)
    {
        _ConstructPhantomDetector();
    }
    

    return phys_vol;
}

G4VPhysicalVolume* ComptCameraDetectorConstruction::_ConstructWorld()
{
    // Create world solid, length arguments half of the actual length
    G4Box* solid_world = new G4Box("World", _world_width/2, _world_height/2, _world_depth/2); 
    // Create world logical volume
    _logic_world = new G4LogicalVolume(solid_world, _world_material, "World"); 
    _logic_world->SetVisAttributes( G4Color(0.6784,0.8471,0.902,0.3) );

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

void ComptCameraDetectorConstruction::_ConstructTungstenAnode()
{
    // Cut vectors: 
    // Anode angle
    // https://www.phywe.com/equipment-accessories/phywe-xr-4-0-x-ray-equipment-accessories/xr4-x-ray-plug-in-w-tube_1740/
    G4ThreeVector lowNorm(0, -std::sin(19*deg), -std::cos(19*deg));
    G4ThreeVector highNorm(0, std::sin(19*deg), std::cos(19*deg));

    const G4double dZ = 1*mm;
    G4CutTubs * anode_w = new G4CutTubs("Anode", 
            0., 
            10/2*mm, 
            dZ/2.*mm,
            0,
            360.0*deg,
            lowNorm,
            highNorm);

    G4LogicalVolume* logic_anode_w = new G4LogicalVolume(anode_w, _tungsten, "Anode");
    logic_anode_w->SetVisAttributes( G4Color(0.3, 0.534, 0.3, 0.7) );

    G4Region* anode_region = new G4Region("AnodeRegion");
    logic_anode_w->SetRegion(anode_region);
    anode_region->AddRootLogicalVolume(logic_anode_w);

    G4RotationMatrix* rot = new G4RotationMatrix;
    rot->rotateX(90*deg);
    rot->rotateY(90*deg);

    new G4PVPlacement(rot, G4ThreeVector(-_world_width/2+2.5*mm, 0.0, 0.0), logic_anode_w, "Anode", _logic_world, false, 0);

    // Create the Duran glass recovering, just a box of a few mm in front of
    G4NistManager* nist = G4NistManager::Instance();
     // Elements
    G4Element* O = nist->FindOrBuildElement("O");
    G4Element* Si = nist->FindOrBuildElement("Si");
    G4Element* B = nist->FindOrBuildElement("B");
    G4Element* Na = nist->FindOrBuildElement("Na");
    G4Element* Al = nist->FindOrBuildElement("Al");

    // Composition of Duran glass (Borosilicate glass)
    G4Material* duranGlass = new G4Material("DuranGlass", 2.23*g/cm3, 5);
    duranGlass->AddElement(O, 65.0 * perCent);
    duranGlass->AddElement(Si, 22.0 * perCent);
    duranGlass->AddElement(B, 12.0 * perCent);
    duranGlass->AddElement(Na, 0.5 * perCent);
    duranGlass->AddElement(Al, 0.5 * perCent);

    // Create phantom detector
    G4String name = "cover";
    G4Box* solid_cover = new G4Box(name, 0.5*mm, 2*mm, 2*mm); 
    // Create phantom detector logical volume
    G4LogicalVolume* logic_cover = new G4LogicalVolume(solid_cover, duranGlass, G4String("logic_"+name));
    logic_cover->SetVisAttributes( G4Color(0.95,0.95,0.90,0.2) );

    // Create phantom detector physical volume
    new G4PVPlacement(0, G4ThreeVector(-_world_width/2+7*mm, 0, 0), logic_cover, name, _logic_world, false, 0);
}


void ComptCameraDetectorConstruction::_ConstructCollimator()
{
    // Create collimator
    const G4String name = "collimator";
    G4Tubs* solid_collimator = new G4Tubs(name,
            0.5*mm,
            11*mm,
            15*mm,
            0,
            360.0*deg); 
    
    G4NistManager* nist = G4NistManager::Instance();
    G4Material*  collimator_material = nist->FindOrBuildMaterial("G4_Pb");


    // Create phantom detector logical volume
    G4LogicalVolume* logic_collimator = new G4LogicalVolume(solid_collimator, collimator_material, G4String("logic_"+name));
    logic_collimator->SetVisAttributes( G4Color(0.9,0.9,0.9,0.3) );
    
    G4RotationMatrix* rot = new G4RotationMatrix;
    rot->rotateY(90*deg);

    // Create phantom detector physical volume
    new G4PVPlacement(rot, G4ThreeVector(0, 0, 0), logic_collimator, name, _logic_world, false, 0);

}

void ComptCameraDetectorConstruction::_ConstructPhantomDetector()
{   
    // Create phantom detector
    G4String name = "PhantomDetector";

    G4Box* solid_phantom_detector = new G4Box(name, 1*um, 1*mm, 1*mm); 
    // Create phantom detector logical volume
    _logic_phantom_detector = new G4LogicalVolume(solid_phantom_detector, _world_material, name);
    _logic_phantom_detector->SetVisAttributes( G4Color(0.6784,0.8471,0.902,0.3) );
    
    // Create phantom detector physical volume
    new G4PVPlacement(0, G4ThreeVector(_world_width/2-2.5*mm, 0, 0), _logic_phantom_detector, name, _logic_world, false, 0);
    // 0 rotation,  translation, logical volume, name, mother volume, boolean operation, copy numbers

}


void ComptCameraDetectorConstruction::_ConstructDetectorsGrid(G4int y_nb_detector, G4int z_nb_detector, G4int detector_number, G4double distance)
{
    // Iterate over rows
    for (G4int i = 0; i < y_nb_detector; ++i)
    {
        G4double yPos = (_detector_size + _spacing)*(i-(_y_nb_detector-1)/2);
        // Iterate over columns
        for (G4int j = 0; j < z_nb_detector; ++j)
        {
            // Calculate the position for each detector in the grid
            G4double xPos = distance - _world_width / 2;
            G4double zPos = (_detector_size + _spacing)*(j-(_z_nb_detector-1)/2);

            // Create a unique name for each detector
            G4String name = "Detector_" + std::to_string(detector_number) + "_" + std::to_string(i) + "_" + std::to_string(j);

            // Create detector solid, length arguments half of the actual length
            G4Box* solid_detector = new G4Box(name, _detector_thickness / 2, _detector_size / 2, _detector_size / 2);

            // Create detector logical volume
            _detector_map[name] = new G4LogicalVolume(solid_detector, _detector_material, name);
            _detector_map[name]->SetVisAttributes( G4Color(1.0, 0.8, 0., 0.9) );

            // Create detector physical volume
            new G4PVPlacement(0, G4ThreeVector(xPos, yPos, zPos), _detector_map[name], name, _logic_world, false, 0);
        }
    }
}

void ComptCameraDetectorConstruction::_ConstructPCB(G4double distance)
{
    G4Box* solid_pcb = new G4Box("PCB", _pcb_thickness, 20/2*mm, 20/2*mm); 
    // Create PCB logical volume
    G4LogicalVolume* logic_pcb = new G4LogicalVolume(solid_pcb, _pcb_material, "PCB");
    logic_pcb->SetVisAttributes( G4Color(4./255, 99/255., 7/255.) );
    // Create PCB physical volume
    new G4PVPlacement(0, G4ThreeVector(distance-_world_width/2, 0, 0), logic_pcb, "PCB", _logic_world, false, 0);
    // 0 rotation,  translation, logical volume, name, mother volume, boolean operation, copy numbers
}

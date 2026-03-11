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
#include "G4VisAttributes.hh"

ComptCameraDetectorConstruction::ComptCameraDetectorConstruction() :
    _logic_world(nullptr),
    _logic_phantom_detector(nullptr),
    _world_material(nullptr),
    _detector_material(nullptr),
    _pcb_material(nullptr),
    _messenger(nullptr)
{
    // Define world size including walls 
    _world_width = 200*mm;
    _world_height = 100*mm;
    _world_depth = 100*mm;

    // Module transverse dimensions:
    _module_size_y = 43.10 * mm;
    _module_size_z = 25.85 * mm;

    // -----------------------------
    // Layer thicknesses from ETL XML
    // -----------------------------
    _ducttape_thickness     = 0.31 * mm;
    _thermalpad_thickness   = 0.25 * mm;
    _lairdfilm_thickness    = 0.08 * mm;
    _alnbase_thickness      = 1.50 * mm;
    _gluelgad_thickness     = 0.10 * mm;
    _lgad_thickness         = 0.30 * mm;
    _lgad_active_thickness  = 0.05 * mm;
    _lgad_sub_thickness     = 0.25 * mm;
    _bumpbonds_thickness    = 0.10 * mm;
    _etroc_thickness        = 0.25 * mm;
    _glueetroc_thickness    = 0.05 * mm;
    _modulepcb_thickness    = 0.45 * mm;
    _connectorsgap_thickness= 0.70 * mm;
    _readoutboard_thickness = 1.00 * mm;
    _services_thickness     = 2.80 * mm;

    _etroc_size_y = 20.8 * mm;
    _etroc_size_z = 22.3 * mm;
    _lgad_size_y = 21.2 * mm;
    _lgad_size_z = 21.2 * mm;
    
    // Define detector size
    _detector_size = 1.3*mm;
    _detector_number = 2;

    //Define subdetector number
    _y_nb_detector = 16;
    _z_nb_detector = 16;

    //Define map with distances
    _detector1_distance = 60*mm;
    _detector2_distance = 150*mm;

    //Space between subdetectors
    _spacing = ( _lgad_size_y - _y_nb_detector * _detector_size ) / (_y_nb_detector - 1);

    _phantom_detector = false;

    _messenger = new G4GenericMessenger(this, "/ComptCamera/detector/", "Detector control");
    _messenger->DeclareProperty("module1_distance", _detector1_distance,
        "Distance from source plane to FRONT FACE of first module");
    _messenger->DeclareProperty("module2_distance", _detector2_distance,
        "Distance from source plane to FRONT FACE of second module");
    _messenger->DeclareProperty("world_width", _world_width,
        "World width in x, /run/reinitializeGeometry to update");

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

    // World material
    _world_material = nist->FindOrBuildMaterial("G4_AIR");

    // Detector material
    _detector_material = nist->FindOrBuildMaterial("G4_Si");

    // Epoxy
    G4Material* epoxy = G4Material::GetMaterial("Epoxy", false);
    if (!epoxy)
    {
        epoxy = new G4Material("Epoxy", 1.3 * CLHEP::g / CLHEP::cm3, 3);
        epoxy->AddElement(nist->FindOrBuildElement("H"), 44);
        epoxy->AddElement(nist->FindOrBuildElement("C"), 15);
        epoxy->AddElement(nist->FindOrBuildElement("O"), 7);
    }
    _epoxy_material = epoxy;

    // PCB
    G4Material* pcb = G4Material::GetMaterial("PCB", false);
    if (!pcb)
    {
        pcb = new G4Material("PCB", 1.7 * CLHEP::g / CLHEP::cm3, 3);
        pcb->AddMaterial(nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE"), 0.773);
        pcb->AddMaterial(epoxy, 0.147);
        pcb->AddElement(nist->FindOrBuildElement("Cl"), 0.08);
    }
    _pcb_material = pcb;
    _service_hybrid_material = pcb;

    // Duct tape
    G4Material* ductTape = G4Material::GetMaterial("DuctTape", false);
    if (!ductTape)
    {
        G4Material* polyethylene = G4Material::GetMaterial("Polyethylene", false);
        if (!polyethylene)
        {
            polyethylene = new G4Material("Polyethylene", 0.94 * CLHEP::g / CLHEP::cm3, 2);
            polyethylene->AddElement(nist->FindOrBuildElement("C"), 2);
            polyethylene->AddElement(nist->FindOrBuildElement("H"), 4);
        }

        G4Material* rubber = G4Material::GetMaterial("Rubber", false);
        if (!rubber)
        {
            rubber = new G4Material("Rubber", 1.1 * CLHEP::g / CLHEP::cm3, 2);
            rubber->AddElement(nist->FindOrBuildElement("C"), 5);
            rubber->AddElement(nist->FindOrBuildElement("H"), 8);
        }

        G4Material* aluminum = nist->FindOrBuildMaterial("G4_Al");

        ductTape = new G4Material("DuctTape", 1.1 * CLHEP::g / CLHEP::cm3, 3);
        ductTape->AddMaterial(polyethylene, 0.6);
        ductTape->AddMaterial(rubber, 0.3);
        ductTape->AddMaterial(aluminum, 0.1);
    }
    _duct_tape_material = ductTape;

    // Aluminium Nitride
    G4Material* aln = G4Material::GetMaterial("AluminiumNitride", false);
    if (!aln)
    {
        aln = new G4Material("AluminiumNitride", 3.26 * CLHEP::g / CLHEP::cm3, 2);
        aln->AddElement(nist->FindOrBuildElement("Al"), 1);
        aln->AddElement(nist->FindOrBuildElement("N"), 1);
    }
    _aln_material = aln;

    // Laird film approximation
    _laird_material = nist->FindOrBuildMaterial("G4_KAPTON");

    // Tin for bump bonds
    _tin_material = nist->FindOrBuildMaterial("G4_Sn");
}

G4VPhysicalVolume* ComptCameraDetectorConstruction::Construct()
{
    _detector_map.clear();
    // Construct world and phantom detector
    auto * phys_vol =_ConstructWorld();
    G4double x_source_plane = -_world_width / 2.0;
    G4double x_front_module1 = x_source_plane + _detector1_distance;
    G4double x_front_module2 = x_source_plane + _detector2_distance;

    _ConstructModuleStack("Module1", x_front_module1, 0.0, 0.0);
    _ConstructModuleStack("Module2", x_front_module2, 0.0, 0.0);

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

void ComptCameraDetectorConstruction::_ConstructModuleStack(const G4String& moduleName, G4double xFrontFace, G4double yModuleCenter, G4double zModuleCenter)
{
    // Sum of all thicknesses
    const G4double totalThickness = _ducttape_thickness + _thermalpad_thickness + _lairdfilm_thickness + _alnbase_thickness + _gluelgad_thickness 
    + _lgad_thickness + _bumpbonds_thickness + _etroc_thickness + _glueetroc_thickness + _modulepcb_thickness + _connectorsgap_thickness
    + _readoutboard_thickness + _services_thickness;

    G4double xModuleCenter = xFrontFace + totalThickness / 2.0;

    G4Box* solid_module = new G4Box(moduleName, totalThickness / 2.0, _module_size_y / 2.0, _module_size_z / 2.0);

    G4LogicalVolume* logic_module = new G4LogicalVolume(solid_module, _world_material, moduleName);
    logic_module->SetVisAttributes(G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.02)));

    new G4PVPlacement(0,G4ThreeVector(xModuleCenter, yModuleCenter, zModuleCenter), logic_module, moduleName, _logic_world, false, 0);

    // Build layers from front face to back face
    G4double xCursor = -totalThickness / 2.0;

    auto placeInsideModule = [&](const G4String& partName, G4Material* material, G4double thickness, G4double sizeY, G4double sizeZ, const G4Colour& colour) -> G4LogicalVolume*
    {
        G4double xLocal = xCursor + thickness / 2.0;

        G4Box* solid = new G4Box(partName, thickness/2.0, sizeY/2.0, sizeZ/2.0);
        G4LogicalVolume* logic = new G4LogicalVolume(solid, material, partName);
        logic->SetVisAttributes(G4VisAttributes(colour));

        new G4PVPlacement(0, G4ThreeVector(xLocal, 0.0, 0.0), logic, partName, logic_module, false, 0);

        xCursor += thickness;
        return logic;
    };

    // duct tape
    placeInsideModule(moduleName + "_DuctTape", _duct_tape_material, _ducttape_thickness, _module_size_y, _module_size_z, G4Colour(0.80, 0.60, 0.20));
    // ThermalPad
    placeInsideModule(moduleName + "_ThermalPad", _epoxy_material, _thermalpad_thickness, _module_size_y, _module_size_z, G4Colour(0.70, 0.30, 0.30));
    // LairdFilm
    placeInsideModule(moduleName + "_LairdFilm", _laird_material,_lairdfilm_thickness, _module_size_y, _module_size_z, G4Colour(0.30, 0.80, 0.80));
    // AlN_Base
    placeInsideModule(moduleName + "_AlN_Base",  _aln_material, _alnbase_thickness, _module_size_y, _module_size_z, G4Colour(0.85, 0.85, 0.85));
    // glueLGAD
    placeInsideModule(moduleName + "_glueLGAD", _epoxy_material, _gluelgad_thickness, _lgad_size_y, _lgad_size_z, G4Colour(0.90, 0.60, 0.20));
    // LGAD container
    G4double xLocalLGAD = xCursor + _lgad_thickness / 2.0;
    G4Box* solid_lgad = new G4Box(moduleName + "_LGAD",  _lgad_thickness/2.0, _lgad_size_y/2.0, _lgad_size_z/2.0);

    G4LogicalVolume* logic_lgad = new G4LogicalVolume(
        solid_lgad,
        _world_material,   // XML has LGAD as container
        moduleName + "_LGAD"
    );
    logic_lgad->SetVisAttributes(G4VisAttributes(G4Colour(0.20, 0.50, 1.00, 0.15)));

    new G4PVPlacement(0, G4ThreeVector(xLocalLGAD, 0.0, 0.0),  logic_lgad,  moduleName + "_LGAD", logic_module, false, 0);

    // Inside LGAD: active + substrate
    G4double xLGADCursor = -_lgad_thickness / 2.0;

    // Active
    G4double xLocalActive = xLGADCursor + _lgad_active_thickness / 2.0;
    G4Box* solid_active = new G4Box(moduleName + "_LGAD_active", _lgad_active_thickness/2.0, _lgad_size_y/2.0, _lgad_size_z/2.0);

    G4LogicalVolume* logic_active = new G4LogicalVolume(solid_active, _detector_material, moduleName + "_LGAD_active");
    logic_active->SetVisAttributes(G4VisAttributes(G4Colour(1.00, 0.00, 0.00, 0.08)));
    new G4PVPlacement(0, G4ThreeVector(xLocalActive, 0.0, 0.0), logic_active, moduleName + "_LGAD_active", logic_lgad, false, 0);
    _ConstructPixelGrid(logic_active, moduleName);

    xLGADCursor += _lgad_active_thickness;

    // Substrate
    G4double xLocalSub = xLGADCursor + _lgad_sub_thickness / 2.0;
    G4Box* solid_sub = new G4Box(moduleName + "_LGAD_Substrate", _lgad_sub_thickness/2.0, _lgad_size_y/2.0, _lgad_size_z/2.0);

    G4LogicalVolume* logic_sub = new G4LogicalVolume(solid_sub, _detector_material, moduleName + "_LGAD_Substrate");
    logic_sub->SetVisAttributes(G4VisAttributes(G4Colour(0.00, 0.20, 1.00)));
    new G4PVPlacement(0, G4ThreeVector(xLocalSub, 0.0, 0.0), logic_sub, moduleName + "_LGAD_Substrate", logic_lgad, false, 0);

    xCursor += _lgad_thickness;

    // bump bonds
    placeInsideModule(moduleName + "_BumpBonds", _tin_material, _bumpbonds_thickness, _lgad_size_y, _lgad_size_z, G4Colour(0.60, 0.60, 0.60));
    // ETROC
    placeInsideModule(moduleName + "_ETROC", _detector_material, _etroc_thickness, _etroc_size_y, _etroc_size_z, G4Colour(0.10, 0.80, 0.10));
    // glueETROC
    placeInsideModule(moduleName + "_glueETROC", _epoxy_material, _glueetroc_thickness, _etroc_size_y, _etroc_size_z, G4Colour(0.95, 0.65, 0.25));
    // ModulePCB
    placeInsideModule(moduleName + "_ModulePCB", _service_hybrid_material, _modulepcb_thickness, _etroc_size_y, _etroc_size_z, G4Colour(0.00, 0.50, 0.00));
    // connectorsGap
    placeInsideModule(moduleName + "_connectorsGap", _world_material, _connectorsgap_thickness, _module_size_y, _module_size_z, G4Colour(0.90, 0.90, 1.00, 0.05));
    // ReadoutBoard
    placeInsideModule(moduleName + "_ReadoutBoard", _service_hybrid_material, _readoutboard_thickness, _module_size_y, _module_size_z, G4Colour(0.00, 0.30, 0.00));
    // servicesModule
    placeInsideModule(moduleName + "_servicesModule", _service_hybrid_material, _services_thickness, _module_size_y, _module_size_z, G4Colour(0.30, 0.20, 0.10));
}

// Set sensitive detector to logical volume
void ComptCameraDetectorConstruction::ConstructSDandField()
{
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();

    G4String lgadSDname = "lgadSD";
    G4VSensitiveDetector* existingLGAD =
        sdManager->FindSensitiveDetector(lgadSDname, false);

    lgadSD* algadSD = nullptr;
    if (!existingLGAD)
    {
        algadSD = new lgadSD(lgadSDname, "lgadHitsCollection");
        sdManager->AddNewDetector(algadSD);
    }
    else
    {
        algadSD = static_cast<lgadSD*>(existingLGAD);
    }

    for (auto& detector : _detector_map)
    {
        detector.second->SetSensitiveDetector(algadSD);
    }
    
    // Create biasing operator once
    if (_compton_bias_operator == nullptr)
    {
        _compton_bias_operator = new ChangeCrossSection("ComptonBiasDet1");
    }

    // Attach biasing ONLY to detector 1 pixels
    for (auto& detector : _detector_map)
    {
        const G4String& volName = detector.first;
        if (volName.contains("detector_1_pixel_"))
        {
            _compton_bias_operator->AttachTo(detector.second);
        }
    }

    if (_phantom_detector)
    {
        G4String phantomSDname = "phantomSD";
        G4VSensitiveDetector* existingPhantom =
            sdManager->FindSensitiveDetector(phantomSDname, false);

        phantomSD* aphantomSD = nullptr;
        if (!existingPhantom)
        {
            aphantomSD = new phantomSD(phantomSDname, "phantomHitsCollection");
            sdManager->AddNewDetector(aphantomSD);
        }
        else
        {
            aphantomSD = static_cast<phantomSD*>(existingPhantom);
        }

        _logic_phantom_detector->SetSensitiveDetector(aphantomSD);
    }
}

void ComptCameraDetectorConstruction::_ConstructPhantomDetector()
{   
    // Create phantom detector
    G4String name = "PhantomDetector";

    G4Box* solid_phantom_detector = new G4Box(name, 1*mm, _world_height/2, _world_depth/2); 
    // Create phantom detector logical volume
    _logic_phantom_detector = new G4LogicalVolume(solid_phantom_detector, _world_material, name);
    _logic_phantom_detector->SetVisAttributes( G4Color(0.6784,0.8471,0.902,0.3) );
    
    // Create phantom detector physical volume
    new G4PVPlacement(0, G4ThreeVector(-_world_width/2+_detector1_distance/2, 0, 0), _logic_phantom_detector, name, _logic_world, false, 0);
    // 0 rotation,  translation, logical volume, name, mother volume, boolean operation, copy numbers

}

void ComptCameraDetectorConstruction::_ConstructPixelGrid(G4LogicalVolume* logic_active, const G4String& moduleName)
{
    // Pixel thickness = active thickness
    G4double pixel_thickness = _lgad_active_thickness;

    // 16x16 pixels
    G4int ny = _y_nb_detector;
    G4int nz = _z_nb_detector;

    G4double pitch = _detector_size + _spacing;

    for (G4int i = 0; i < ny; ++i)
    {
        G4double yPos = pitch * (i - (ny - 1) / 2.0);

        for (G4int j = 0; j < nz; ++j)
        {
            G4double zPos = pitch * (j - (nz - 1) / 2.0);
            G4int detID = (moduleName == "Module1") ? 1 : 2;
            G4String name = "detector_" + G4UIcommand::ConvertToString(detID)
                + "_pixel_" + G4UIcommand::ConvertToString(i)
                + "_" + G4UIcommand::ConvertToString(j);
            G4Box* solid_pixel = new G4Box(name, pixel_thickness / 2.0, _detector_size / 2.0, _detector_size / 2.0);
            G4LogicalVolume* logic_pixel = new G4LogicalVolume(solid_pixel, _detector_material, name);
            logic_pixel->SetVisAttributes(G4VisAttributes(G4Colour(1.0, 0.8, 0.0, 0.9)));
            new G4PVPlacement(0, G4ThreeVector(0.0, yPos, zPos), logic_pixel, name, logic_active, false, i * nz + j);
            _detector_map[name] = logic_pixel;
        }
    }
}
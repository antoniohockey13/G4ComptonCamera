#ifndef CONSTRUCTION_HH
#def CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh" // for detector construction
#include "G4VPhysicalVolume.hh" // for physical volumes
#include "G4LogicalVolume.hh" // for logical volumes
#include "G4Box.hh" // for box geometry
#include "G4PVPlacement.hh" // for placement of volumes
#include "G4NistManager.hh" // for NIST materials
#include "G4SystemOfUnits.hh" // for units
// #include "G4GenericMessenger.hh" // for UI commands

#include "detector.hh" // for detector class

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  MyDetectorConstruction();
  ~MyDetectorConstruction();

  virtual G4VPhysicalVolume* Construct();

private:
    G4LogicalVolume *logicWorld, *logicDetector1, *logicSensitive1, *logicDetector2, *logicSensitive2;
    G4VPhysicalVolume *physWorld, *physDetector1, *physSensitive1 *physDetector2, *physSensitive2;
    G4Box *solidWorld, *solidDetector1, *solidSensitive1, *solidDetector2, *solidSensitive2;
    G4Material *worldMaterial, *detectorMaterial;
    
    void DefineMaterials();
    void ConstructWorld();
    void ConstructDetector1();
    void ConstructDetector2();

    virtual void ConstructSDandField();

    G4double detector1_distance = -1; // Distance between source and detector 1
	G4double detector2_distance = -1; // Distance between source and detector 2
	G4double angle = 0; // Angle between source and detectors

	G4double world_width = -1; // World dimension in x axis, width
	G4double world_height = -1;	// World dimension in y axis, height
	G4double world_depth = -1; // World dimension in z axis, depth
    
    G4double detector_size = -1 // Detector size in x and y axis
    G4double detector_thickness = -1 // Detector thickness in z axiss
}:
#endif

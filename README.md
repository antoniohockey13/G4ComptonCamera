# Compton Camera X-Ray G4 Simulation
Code generated as part of the Master thesis in 2024 for the Master in Particle Physics and Physics of the Cosmos
__Author: Antonio Gómez Carrera__

## Branch structure 
- `main`: Compton camera simulation to generate data for the algorithm. Source emitting photons with E = 70 keV towards to big parallel planar detectors of silicon.
- `Realistic_Construction`: Code to generate a realistic simulation of the used setup  with a source emitting photons with an energy spectrum which is read from a file and 4 silicon pieces (where the E deposition is measured) with an epoxy structure.
- `tungsten-anode`: X-ray machine radiation field simulation

## Code structure

Two main folders ``include`` and ``src`` for the geant4 simulation:
In headers all *.hh files.
In Source all *.cc files.

-action :
It runs the particle gun.

-construction :
Includes the definition of the geometry

-event

-generator :
Generates the particle gun for the experiment. In this case it is an X-Ray source. 

-lgadhit :
It processes the hit of the sensitive material of the LGAD detectors. It stores the track ID, detector number, energy deposited, position of the particle, and momentum of the particle.

-lgadSD :
It creates the sensitive part of the LGAD detectors

-phantomHit :
It processes tge hit in a phantom detector situated close to the source and which detectes without interacting every particle emitted by the source.

-phantomSD : 
Handle the senitive part of the phantom detector.

-physics :
Includes the main physical processes simulated.

-run:
It stores the wanted data of each run in a root file.

Other folders for the result analysis:
-``Results\Absorber_Thickness``: Jupyter notebook analysing the optimal thickness.

-``Results\Validation``: checks different general magnitudes with different files to analyse the simulation results and choose the optimal one.

-``Cone_Intersection`` : compute the reconstruction of each event and handle the source position result

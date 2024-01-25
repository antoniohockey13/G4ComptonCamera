# Compton Camera X-Ray G4 Simulation

## Author: Antonio Gómez Carrera

## Code structure

Two main folders ``include`` and ``src``:
In headers all *.hh files.
In Source all *.cc files.

-construction :
Includes the definition of the geometry

-physics :
Includes the main physical processes simulated.

-generator :
Generates the particle gun for the experiment. In this case it is an X-Ray source. 

-action :
It runs the particle gun.

-lgadhit :
It processes the hit of the sensitive material of the LGAD detectors. It stores the track ID, detector number, energy deposited, position of the particle, and momentum of the particle.

-lgadSD :
It creates the sensitive part of the LGAD detectors

import numpy as np
from sympy import symbols
import read_root

import Cone_Class

# Units
mm = 1
keV = 1
# Constants
M_ELECTRON = 510.99895000*keV
SOURCE_POSITION = np.array([-682/2*mm, 0*mm, 0*mm])


class ConeIntersection:
    def __init__(self):
        self.cones = []

    def add_cone(self, cone):
        self.cones.append(cone)

    def remove_cone(self, cone):
        self.cones.remove(cone)
    
    def voxelise_space(self, x_lower, x_greater, y_lower, y_greater, z_lower, z_greater, voxel_number_side):
        """

        Voxelise the space in x, y, z coordinates.

        Parameters
        ----------
        x_lower : float
            Lower limit in x coordinate.
        x_greater : float
            Greater limit in x coordinate.
        y_lower : float
            Lower limit in y coordinate.
        y_greater : float
            Greater limit in y coordinate.
        z_lower : float
            Lower limit in z coordinate.
        z_greater : float
            Greater limit in z coordinate.
        voxel_number_side : int
            Number of voxels in each side.        
        Returns
        -------
        numpy.array
            Array containing the voxelised 
space.
        """
        len_x = (x_greater - x_lower)/voxel_number_side
        len_y = (y_greater - y_lower)/voxel_number_side
        len_z = (z_greater - z_lower)/voxel_number_side
        voxels ={
            "Number":[] ,
            "x>": [],
            "x<": [],
            "y>": [],
            "y<": [],
            "z>": [],
            "z<": []
            }
        num = 0
        for ix in range(voxel_number_side):
            x1 = x_lower + ix*len_x
            x2 = x_lower + (ix+1)*len_x
            for iy in range(voxel_number_side):
                y1 = y_lower + iy*len_y
                y2 = y_lower + (iy+1)*len_y
                for iz in range(voxel_number_side):
                    voxels["Number"].append(num)
                    voxels["x>"].append(x1)
                    voxels["x<"].append(x2)
                    voxels["y>"].append(y1)
                    voxels["y<"].append(y2)
                    voxels["z>"].append(z_lower + iz*len_z)
                    voxels["z<"].append(z_lower + (iz+1)*len_z)
                    num += 1
        return voxels
    

vertex, hit, theta_m, E_1, E_2, event = read_root.extract_variables("Results/Validation/validation12.root", read_partially=True)
vertex, hit, theta_m, E_1, E_2, event, theta_E = read_root.select_events(vertex, hit, theta_m, E_1, E_2, event, M_ELECTRON, energy_tol=1e-10)

cones = ConeIntersection()
for i in range(len(E_1)):
    if theta_E[i] is not None:
        cone = Cone_Class.Cone(vertex[i], hit[i], E_1[i], E_2[i], theta_m[i], event[i])
        cones.add_cone(cone)
print(cones.voxelise_space(0, 10, 0, 10, 0, 10, 2))
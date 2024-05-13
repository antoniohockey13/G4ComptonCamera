import numpy as np
from tqdm import tqdm
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

    def compute_intersection(self, x_lower, x_greater, y_lower, y_greater, z_lower, z_greater, voxel_number_side, tol = 1e-3):
        """
        Compute the intersection of the cones with the voxels. It is needed an initial guess
        
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
        tol : float
            Tolerance in the voxelisation.
        """
        continue_search = True
        cont = 0
        while continue_search:
            self.voxelise_space(x_lower, x_greater, y_lower, y_greater, z_lower, z_greater, voxel_number_side)
            print(f"Number of cones studied: {len(self.cones)}") #43
            print(cont)
            cont += 1
            best_voxel =  self.select_voxel()
            print(best_voxel)
    
            selected_voxel =  {}
            new_x_lower = []
            new_x_greater = []
            new_y_lower = []
            new_y_greater = []
            new_z_lower = []
            new_z_greater = []

            for i in best_voxel:
                print(self.voxels[i])
                selected_voxel[i] = self.voxels[i]
                new_x_lower.append(self.voxels[i]["x>"])
                new_x_greater.append(self.voxels[i]["x<"])
                new_y_lower.append(self.voxels[i]["y>"])
                new_y_greater.append(self.voxels[i]["y<"])
                new_z_lower.append(self.voxels[i]["z>"])
                new_z_greater.append(self.voxels[i]["z<"])

            
            x_lower = np.min(new_x_lower)
            x_greater = np.max(new_x_greater)
            y_lower = np.min(new_y_lower)
            y_greater = np.max(new_y_greater)
            z_lower = np.min(new_z_lower)
            z_greater = np.max(new_z_greater)
            print(f"x = {x_greater} - {x_lower}\n y = {y_greater} - {y_lower}\n z = {z_greater} - {z_lower}")
            if (x_greater - x_lower) < tol and (y_greater - y_lower) < tol and (z_greater - z_lower) < tol:
                continue_search = False            
        return selected_voxel

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
            Array containing the voxelised space.
        """
        len_x = (x_greater - x_lower)/voxel_number_side
        len_y = (y_greater - y_lower)/voxel_number_side
        len_z = (z_greater - z_lower)/voxel_number_side
        voxels = {}
        num = 0
        for ix in range(voxel_number_side):
            x1 = x_lower + ix*len_x
            x2 = x_lower + (ix+1)*len_x
            for iy in range(voxel_number_side):
                y1 = y_lower + iy*len_y
                y2 = y_lower + (iy+1)*len_y
                for iz in range(voxel_number_side):
                    voxels[num] = {
                        "x>": x1,
                        "x<": x2, 
                        "y>": y1,
                        "y<": y2,
                        "z>": z_lower + iz*len_z,
                        "z<": z_lower + (iz+1)*len_z,}
                    num += 1
        self.voxels = voxels
        return voxels
    
    def select_voxel(self, percentage = 0.9):
        """
        Select the voxel with the highest number of cones.
        
        Parameters
        ----------
        percentage : float
            Percentage of cones that the voxel must contain.
        Returns
        -------
        int
            Number of the voxel.
        """
        intersection = np.zeros(len(self.voxels.keys()))
        for cone in tqdm(self.cones):
            for i, ivox in (self.voxels.items()):
                if cone.cone_intersect_voxel(ivox):
                    intersection[i] += 1
        print(intersection)     
        voxel_selected = []
        for i in range(len(intersection)):
            if intersection[i] > percentage*len(self.cones):
                voxel_selected.append(i)
        if len(voxel_selected) == 0:
            max_value = np.max(intersection)
            voxel_selected = [i for i, val in enumerate(intersection) if val == max_value]

        return voxel_selected


vertex, hit, theta_m, E_1, E_2, event = read_root.extract_variables("Results/Validation/validation12.root", read_partially=False)
vertex, hit, theta_m, E_1, E_2, event, theta_E = read_root.select_events(vertex, hit, theta_m, E_1, E_2, event, M_ELECTRON, energy_tol=1e-10)

cones = ConeIntersection()
for i in range(len(E_1)):
    if theta_E[i] is not None:
        cone = Cone_Class.Cone(vertex[i], hit[i], E_1[i], E_2[i], theta_m[i], event[i])
        cones.add_cone(cone)
print(cones.compute_intersection(-682/2-5, -682/2+6, -5, 6, -6, 5, 2))

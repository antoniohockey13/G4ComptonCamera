import numpy as np
import matplotlib.pyplot as plt
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
            self.plot_voxels_and_cone()
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

            if x_lower == np.min(new_x_lower) and x_greater == np.max(new_x_greater) and y_lower == np.min(new_y_lower) and y_greater == np.max(new_y_greater) and z_lower == np.min(new_z_lower) and z_greater == np.max(new_z_greater):
                continue_search = False
            
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
    

    def plot_voxels_and_cone(self):
        """
        Plot the voxels.
        """
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')

        # Extracting voxel boundaries for plotting
        voxel_bounds = [[(voxel["x<"], voxel["x>"]),
                        (voxel["y<"], voxel["y>"]),
                        (voxel["z<"], voxel["z>"])] for voxel in self.voxels.values()]
        print(voxel_bounds[0])

        # Plotting each voxel
        for bound in voxel_bounds:
            print(bound)
            print(bound[0])
            x, y, z = bound
            for ix in x:
                for iy in y:
                    for iz in z:
                        ax.scatter(ix, iy, iz, color='r')
        self.cones[0].plot_cone_3d_lines(ax)
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_zlabel('Z')
        plt.show()
    
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
        xy_used = []
        for cone in tqdm(self.cones):
            # print("New Cone")
            for i, ivox in (self.voxels.items()):
                x = ivox["x>"]
                y = ivox["y>"]
                if (x, y) not in xy_used:
                    # print("New xy")
                    xy_used.append((x, y))
                    z_reco = cone.reconstruct_z(x, y)
                    for z in z_reco:
                        # print(f"z={z}")
                        for i, ivox in (self.voxels.items()):
                            if ivox["x>"] == x and ivox["y>"] == y:
                                if ivox["z>"] <= z and ivox["z<"] >= z:
                                    intersection[i] += 1
                                    # print(f"Intersection in {i}")
            xy_used = []
        print(intersection)     
        voxel_selected = []
        for i in range(len(intersection)):
            if intersection[i] > percentage*len(self.cones):
                voxel_selected.append(i)
        if len(voxel_selected) == 0:
            max_value = np.max(intersection)
            voxel_selected = [i for i, val in enumerate(intersection) if val == max_value]

        return voxel_selected
    
    def select_voxel_x(self, percentage = 0.9):
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
        yz_used = []
        for cone in tqdm(self.cones):
            # print("New Cone")
            for i, ivox in (self.voxels.items()):
                y = ivox["y>"]
                z = ivox["z>"]
                if (y, z) not in yz_used:
                    # print("New xy")
                    yz_used.append((y, z))
                    x_reco = cone.reconstruct_x(y, z)
                    for x in x_reco:
                        # print(f"z={z}")
                        for i, ivox in (self.voxels.items()):
                            if ivox["y>"] == y and ivox["z>"] == z:
                                if ivox["x>"] <= x and ivox["x<"] >= x:
                                    intersection[i] += 1
                                    # print(f"Intersection in {i}")
            yz_used = []
        print(intersection)     
        voxel_selected = []
        for i in range(len(intersection)):
            if intersection[i] > percentage*len(self.cones):
                voxel_selected.append(i)
        if len(voxel_selected) == 0:
            max_value = np.max(intersection)
            voxel_selected = [i for i, val in enumerate(intersection) if val == max_value]

        return voxel_selected


vertex, hit, theta_m, E_1, E_2, event, theta_E = read_root.extract_variables("Results/Validation/validation12.root", read_partially=True)

cones = ConeIntersection()
for i in range(len(E_1)):
    if theta_E[i] is not None:
        cone = Cone_Class.Cone(vertex[i], hit[i], E_1[i], E_2[i], theta_m[i], event[i])
        cones.add_cone(cone)
        print(f"Cone Nº  {i} correct source:  {cone.check_simulated_source_pos()}")
print(cones.compute_intersection(-682/2-5, -682/2+5, -5, 5, -6, 6, 2))

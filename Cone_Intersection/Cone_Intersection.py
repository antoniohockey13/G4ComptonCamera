import numpy as np
import matplotlib.pyplot as plt
import sympy as sp
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

    def get_voxels(self):
        return self.voxels
    
    def remove_voxels(self):
        self.voxels = {}

    def compute_intersection(self, x_lower, x_greater, y_lower, y_greater, z_lower, z_greater, voxel_number_size_x, voxel_number_size_y, voxel_number_size_z, tol = 1e-3):
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
        voxel_number_side_x : int
            Number of voxels in x side.
        voxel_number_side_y : int
            Number of voxels in y side.
        voxel_number_side_z : int
            Number of voxels in z side.        
        tol : float
            Tolerance in the voxelisation.
        """
        continue_search = True
        cont = 0
        cont_volume = 0
        old_volume = np.inf
        initial_voxel_number_size_x = voxel_number_size_x
        initial_voxel_number_size_y = voxel_number_size_y
        initial_voxel_number_size_z = voxel_number_size_z

        while continue_search:
            # Voxelise the space
            x, y, z = self.voxelise_space(x_lower, x_greater, y_lower, y_greater, z_lower, z_greater, voxel_number_size_x, voxel_number_size_y, voxel_number_size_z)

            print(f"Number of cones studied: {len(self.cones)}")
            print(f"Number of voxels in x: {voxel_number_size_x}\nNumber of voxels in y: {voxel_number_size_y}\nNumber of voxels in z: {voxel_number_size_z}")
            print(cont)
            cont += 1
            # Select the voxel with the highest number of cones
            best_voxel =  self.select_voxel(x, y, z)
            print(best_voxel)

            # Compute new limits of the space
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

            # All voxels equally intersected
            # if len(best_voxel) == len(self.voxels):
            #     # Increasing limits this way we assure that if some side is set as 1 so no voxelization is needed it stays the same way.
            #     # Also change the division number no always multiple of 2 so the voxelization is not always the same
            #     voxel_number_size_x = (voxel_number_size_x-1)*2+1
            #     voxel_number_size_y = (voxel_number_size_y-1)*2+1
            #     voxel_number_size_z = (voxel_number_size_z-1)*2+1
            #     print("All voxels equally intersected")
            # # Some voxels are discarded but the limits stay the same
            # elif x_lower == np.min(new_x_lower) and x_greater == np.max(new_x_greater) and y_lower == np.min(new_y_lower) and y_greater == np.max(new_y_greater) and z_lower == np.min(new_z_lower) and z_greater == np.max(new_z_greater):
            #     print(" Some voxel discard but bigger limits stay the same")
            #     voxel_number_size_x = (voxel_number_size_x-1)*2+1
            #     voxel_number_size_y = (voxel_number_size_y-1)*2+1
            #     voxel_number_size_z = (voxel_number_size_z-1)*2+1
            
            new_x_lower = np.min(new_x_lower)
            new_x_greater = np.max(new_x_greater)
            new_y_lower = np.min(new_y_lower)
            new_y_greater = np.max(new_y_greater)
            new_z_lower = np.min(new_z_lower)
            new_z_greater = np.max(new_z_greater)
            print(f"x = {new_x_greater} - {new_x_lower}\n y = {new_y_greater} - {new_y_lower}\n z = {new_z_greater} - {new_z_lower}")
            new_volume = (new_x_greater - new_x_lower)*(new_y_greater - new_y_lower)*(new_z_greater - new_z_lower)
            print(f"Old volume {old_volume}\n New volume {new_volume}")
            print(f"Delta volume {old_volume - new_volume}")

            if  (old_volume - new_volume) <= tol:
                cont_volume += 1

                if cont_volume == 2:
                    continue_search = False
                else:
                    voxel_number_size_x = (voxel_number_size_x-1)*2+1
                    voxel_number_size_y = (voxel_number_size_y-1)*2+1
                    voxel_number_size_z = (voxel_number_size_z-1)*2+1
            else:
                cont_volume = 0
            # Avoid no limit increasing voxel number
            if voxel_number_size_x > 30:
                voxel_number_size_x = initial_voxel_number_size_x
            if voxel_number_size_y > 30:
                voxel_number_size_y = initial_voxel_number_size_y
            if voxel_number_size_z > 30:
                voxel_number_size_z = initial_voxel_number_size_z

            old_volume = new_volume
            x_lower = new_x_lower
            x_greater = new_x_greater   
            y_lower = new_y_lower
            y_greater = new_y_greater
            z_lower = new_z_lower
            z_greater = new_z_greater
            self.remove_voxels()
              
        return selected_voxel

    def voxelise_space(self, x_lower, x_greater, y_lower, y_greater, z_lower, z_greater, voxel_number_side_x, voxel_number_side_y, voxel_number_side_z):
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
        voxel_number_side_x : int
            Number of voxels in each side in x coordinate.
        voxel_number_side_y : int
            Number of voxels in each side in y coordinate.
        voxel_number_side_z : int
            Number of voxels in each side in z coordinate.         
        Returns
        -------
        numpy.array
            Array containing the x coordinates of the voxels.
        numpy.array
            Array containing the y coordinates of the voxels.
        numpy.array
            Array containing the z coordinates of the voxels.
        """
        # To store the x, y, z values of the voxels limits
        x_used = []
        y_used = []
        z_used = []
        # Length of the voxels
        len_x = (x_greater - x_lower)/voxel_number_side_x
        len_y = (y_greater - y_lower)/voxel_number_side_y
        len_z = (z_greater - z_lower)/voxel_number_side_z
        voxels = {}
        num = 0
        for ix in range(voxel_number_side_x):
            x1 = x_lower + ix*len_x
            x2 = x_lower + (ix+1)*len_x
            if x1 not in x_used:
                x_used.append(x1)
            if x2 not in x_used:
                x_used.append(x2)
            for iy in range(voxel_number_side_y):
                y1 = y_lower + iy*len_y
                y2 = y_lower + (iy+1)*len_y
                if y1 not in y_used:
                    y_used.append(y1)
                if y2 not in y_used:
                    y_used.append(y2)
                for iz in range(voxel_number_side_z):
                    z1 = z_lower + iz*len_z
                    z2 = z_lower + (iz+1)*len_z
                    if z1 not in z_used:
                        z_used.append(z1)
                    if z2 not in z_used:
                        z_used.append(z2)
                    voxels[num] = {
                        "x>": x1,
                        "x<": x2, 
                        "y>": y1,
                        "y<": y2,
                        "z>": z1,
                        "z<": z2}
                    num += 1
        self.voxels = voxels 
        return np.array(x_used), np.array(y_used), np.array(z_used)
    

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
    
    def select_voxel(self, x, y, z):
        """
        Select the voxel with the highest number of cones.
        
        Parameters
        ----------
        x : numpy.array
            Array containing the x coordinates of the voxels.
        y : numpy.array
            Array containing the y coordinates of the voxels.
        z : numpy.array
            Array containing the z coordinates of the voxels.

        Returns
        -------
        list
            List containing the index of the voxels with the highest number of cones.
        """
        # Initialize the intersection array
        intersection = np.zeros(len(self.voxels.keys()))
        # Loop over the cones
        for cone in tqdm(self.cones):
            # Initialize the array to check if the voxel has been already added
            vox_added = np.zeros(len(self.voxels.keys()))
            for ix in x:
                for iy in y:
                    # Fixing x-y to obtain z
                    z_reco = cone.reconstruct_z(ix, iy)
                    for iz_reco in z_reco:
                        # Loop over the voxels
                        for i, ivox in (self.voxels.items()):
                            if (ivox["x>"] == ix or ivox["x<"] == ix) and (ivox["y>"] == iy or ivox["y<"] == iy) and ivox["z>"] <= iz_reco and ivox["z<"] >= iz_reco:
                                # check if the voxel has been already added
                                if vox_added[i] == 0:
                                    vox_added[i] = 1
                                    intersection[i] += 1
                # Fixing x-z to obtain y
                for iz in z:
                    y_reco = cone.reconstruct_y(ix, iz)
                    for iy_reco in y_reco:
                        for i, ivox in (self.voxels.items()):
                            if (ivox["x>"] == ix or ivox["x<"] == ix) and (ivox["z>"] == iz or ivox["z<"] == iz) and ivox["y>"] <= iy_reco and ivox["y<"] >= iy_reco:
                                if vox_added[i] == 0:
                                    vox_added[i] = 1
                                    intersection[i] += 1
        print(intersection)

        max_value = np.max(intersection)
        if max_value != len(self.cones):
            print("Warning: No voxel intersected by all cones. Selecting the one with the highest number of cones.")

        voxel_selected = [i for i, val in enumerate(intersection) if val == max_value]
        return voxel_selected

cones = ConeIntersection()
vertex, hit, theta_m, E_1, E_2, event, theta_E = read_root.extract_variables("Results/Validation/check_reco_divergente.root", read_partially=True)

for i in range(len(E_1)):
    if theta_E[i] is not None:
        cone = Cone_Class.Cone(vertex[i], hit[i], E_1[i], E_2[i], theta_m[i], event[i])
        cones.add_cone(cone)
        print(f"Cone Nº  {i} correct source:  {cone.check_simulated_source_pos()}")

det1_position = vertex[0]

print(cones.compute_intersection(SOURCE_POSITION[0]-50, SOURCE_POSITION[0]+50, -150, 150, -100, 100, voxel_number_size_x = 4, voxel_number_size_y = 5, voxel_number_size_z = 5))
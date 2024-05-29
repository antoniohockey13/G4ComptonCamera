import numpy as np
import matplotlib.pyplot as plt
import sympy as sp
import read_root
import useful_functions as uf

# Units
mm = 1
keV = 1
# Constants
M_ELECTRON = 510.99895000*keV
SOURCE_POSITION = np.array([-682/2*mm, 0*mm, 0*mm])



class Cone:
    def __init__(self, vertex, hit, E1, E2, theta_m, event):
        """
        Parameters
        ----------
        vertex : np.ndarray
            The vertex of the cone.
        hit : np.ndarray
            The hit point of the cone.
        E1 : float
            Energy lost in the first detector.
        E2 : float
            Energy lost in the second detector.
        theta_m : float
            Kinematic angle in radians.
        event : int
            Event number.
        """
        self.vertex = vertex
        self.E1 = E1
        self.E2 = E2
        self.compton_angle = self.aperture_angle()
        self.kinematic_angle = theta_m
        self.hit = hit
        a = hit - vertex
        self.axis = a/np.linalg.norm(a)
        
        self.angle_with_axis = self.angle_with_axis()

        self.event = event
        self.cone_eq = self.cone_equation()

    def aperture_angle(self):
        """
        Returns
        -------
        float
            The aperture angle of the cone.
        """
        try:
            theta = np.arccos(1-M_ELECTRON*self.E1/(self.E2*(self.E1+self.E2)))  
        except RuntimeWarning:
            theta = None
        return theta
    
    def get_event(self):
        """
        Returns
        -------
        int
            The event number.
        """
        return self.event
    
    def angle_with_axis(self):
        """
        Returns
        -------
        float
            The angle between the x-axis and the axis o the cone.
        """
        return np.arccos(self.axis[0]) 
    
    def plot_event_2d(self, title=None):
        """
        Plot the event in the compton plane.

        """

        vertex, hit, source_position, u, d, base = uf.Compute_variables_to_plot(self.hit, self.vertex, SOURCE_POSITION)

        # Cambio de base
        source_position_compton = uf.cambio_base(source_position, base, d)
        vertex_compton = uf.cambio_base(self.vertex, base, d)
        hit_compton = uf.cambio_base(self.hit, base, d)

        # Normal vector to the compton plane
        normal = np.cross(base, d)

        # Plot
        fig = plt.figure()
        ax = fig.add_subplot(111)
        ax.scatter(source_position_compton[0], source_position_compton[1], c='g', marker='o', label='Source')
        ax.scatter(vertex_compton[0], vertex_compton[1], c='r', marker='o', label='Vertex')
        ax.scatter(hit_compton[0], hit_compton[1], c='b', marker='o', label='Hit')


        # Rotar recta angulo theta respecto de vector normal al plano compton
        angles = [self.compton_angle, self.kinematic_angle]
        colours = ['b', 'g']
        label = ['Compton', 'Kinematic']
        vertex_hit = []
        lambda1 = np.linspace(-150, 200, 2)
        for i in range(len(angles)):
            source_hit = []
            for l in lambda1:
                x = l*u
                x_v = x + self.vertex
                x_compton = uf.cambio_base(x_v, base, d)
                vertex_hit.append(x_compton)
        
                x_rot = uf.rotate(angles[i], normal, x)
                x_rot = x_rot + vertex
                x_rot_compton = uf.cambio_base(x_rot, base, d) 
                source_hit.append(x_rot_compton)
        
            
            source_hit = np.array(source_hit)

            ax.plot(source_hit[:,0], source_hit[:,1], c=colours[i], label=f'Source-Hit {label[i]}')
        vertex_hit = np.array(vertex_hit)
        ax.plot(vertex_hit[:,0], vertex_hit[:,1], 'r', label='Vertex-Hit')
        ax.legend(loc='best')
        ax.xaxis.set_label_text(f'd = x = {d}')
        ax.yaxis.set_label_text(f'base = {base}')
        plt.xlim(-400,-100)
        plt.ylim(-100,200)

        #ax.text(-300, 100, f"Min distance: {min_distance(hit, vertex, source, theta):.8f}", fontsize=12)
        if title:
            plt.title(title)
        else:
            plt.title(f"Event {self.event} in the compton plane")
        plt.show()  

    def plot_cone_3d_lines(self,  ax = None, title = None):
        """
        Plot the photon trajectory in 3D.

        """
        if ax is None:
            fig = plt.figure()
            ax = fig.add_subplot(111, projection='3d')
            ax.set_xlabel('X')
            ax.set_ylabel('Y')
            ax.set_zlabel('Z')

        # Plot the source, vertex, hit
        # ax.scatter(SOURCE_POSITION[0], SOURCE_POSITION[1], SOURCE_POSITION[2], c='r', marker='o', label='Source')
        ax.scatter(self.vertex[0], self.vertex[1], self.vertex[2], c='b', marker='o', label='Vertex')
        ax.scatter(self.hit[0], self.hit[1], self.hit[2], c='g', marker='o', label='Hit')

        # Plot the photon trajectory
        ax.plot([self.vertex[0], self.hit[0]], [self.vertex[1], self.hit[1]], [self.vertex[2], self.hit[2]], c='k', label='Photon trajectory')
        point = self.vertex-self.hit
        r = np.array([0,1,0])
        # Rotate in a perpendicular vector

        point_rot = uf.rotate(self.compton_angle, np.cross(point, r), point)
        point_rop_plot = point_rot + self.vertex
        # ax.plot([point_rop_plot[0], self.vertex[0]], [point_rop_plot[1], self.vertex[1]], [point_rop_plot[2], self.vertex[2]], c='r', label='Cone')

        phi = np.linspace(0, 2*np.pi, 100)

        for p in phi:
            point_rot_phi = uf.rotate(p, point, point_rot) + self.vertex
            if p == 0.:
                ax.plot([point_rot_phi[0], self.vertex[0]], [point_rot_phi[1], self.vertex[1]], [point_rot_phi[2], self.vertex[2]], c='y', label='Cone')
            else:
                ax.plot([point_rot_phi[0], self.vertex[0]], [point_rot_phi[1], self.vertex[1]], [point_rot_phi[2], self.vertex[2]], c='y')

        plt.legend(loc='best')
        if title:
            plt.title(title)
        # else:
        #     plt.title(f"Event {self.event}")
        if ax is None:
            plt.show()  

    def min_distance(self):
        """
        Compute the minimum distance between a point and a straight line in 3d.
        Using the compton angle as the aperture angle of the cone.

        Returns
        -------
        float
            The minimum distance between the cone and the source position.
        """

        point = self.vertex-self.hit
        r = np.array([0,1,0])
        # Rotate in a perpendicular vector
        point_rot = uf.rotate(self.compton_angle, np.cross(point, r), point)

        min_dist = np.inf
        phi = np.linspace(0, 2*np.pi, 100)
        for p in phi:
            point_rot_phi = uf.rotate(p, point, point_rot) + self.vertex
            dist = np.linalg.norm(np.cross(point_rot_phi, SOURCE_POSITION))/np.linalg.norm(point_rot_phi)
            if dist < min_dist:
                min_dist = dist
        self.min_distance = min_dist
        return min_dist
    
    def cone_equation(self):
        """
        Compute the equation of the cone in the form of a parametric equation.
        it uses the sympy library to compute the equation.

        Returns
        -------
        sympy expression
            The equation of the cone.
        """
        landa = sp.symbols('lambda')
        phi = sp.symbols('phi')

        point = self.vertex-self.hit
        r = np.array([0,1,0])
        # Rotate in a perpendicular vector
        # Using the kinematic angle
        # point_rot = uf.rotate(self.kinematic_angle, np.cross(point, r), point)
        # Using the compton angle
        point_rot = uf.rotate(self.compton_angle, np.cross(point, r), point)
        point_rot_phi = uf.rotate(phi, point, point_rot)
        
        return landa*point_rot_phi + sp.Matrix(self.vertex)

    def cone_intersect_voxel(self, voxel):
        """
        Compute the intersection of a cone with a voxel.

        Parameters
        ----------
        voxel : np.ndarray
            Array containing the voxel information.

        Returns
        -------
        bool
            True if the cone intersects the voxel, False otherwise.
        """
        eq = self.cone_eq
        eq_x = eq[0]
        eq_y = eq[1]
        eq_z = eq[2]

        eq_x_comp = sp.Eq(eq_x, voxel["x>"])
        eq_y_comp = sp.Eq(eq_y, voxel["y>"])
        out = sp.solvers.solve([eq_x_comp, eq_y_comp], dict=True)
        for i in range(len(out)):
            landa = out[i][sp.symbols('lambda')]
            phi = out[i][sp.symbols('phi')]
            z_i = eq_z.subs(sp.symbols('lambda'), landa).subs(sp.symbols('phi'), phi).evalf()

            # Check if z_i complex number
            if not z_i.is_real:
                # print("Complex number")
                # print(z_i)
                # print(out[i])
                # print(voxel)
                # print(eq_x)
                # print(eq_y)
                # print(self.event)
                return False
            elif z_i > voxel["z>"] and z_i < voxel["z<"]:
                return True
            else:
                eq_z_comp = sp.Eq(eq_z, voxel["z>"])
                out = sp.solvers.solve([eq_y_comp, eq_z_comp], dict=True)
                for i in range(len(out)):
                    landa = out[i][sp.symbols('lambda')]
                    phi = out[i][sp.symbols('phi')]
                    x_i = eq_x.subs(sp.symbols('lambda'), landa).subs(sp.symbols('phi'), phi).evalf()

                    if not x_i.is_real:
                        return False
                    elif x_i > voxel["x>"] and x_i < voxel["x<"]:
                        return True
                    else:
                        eq_z_comp = sp.Eq(eq_z, voxel["z<"])
                        out = sp.solvers.solve([eq_x_comp, eq_z_comp], dict=True)
                        for i in range(len(out)):
                            landa = out[i][sp.symbols('lambda')]
                            phi = out[i][sp.symbols('phi')]
                            y_i = eq_y.subs(sp.symbols('lambda'), landa).subs(sp.symbols('phi'), phi).evalf()
                            if not y_i.is_real:
                                return False
                            if y_i > voxel["y>"] and y_i < voxel["y<"]:
                                return True

        return False
    
    def reconstruct_z(self, x, y):

        eq = self.cone_eq
        eq_x = eq[0]
        eq_y = eq[1]
        eq_z = eq[2]

        eq_x_comp = sp.Eq(eq_x, x)
        eq_y_comp = sp.Eq(eq_y, y)
        out = sp.solvers.solve([eq_x_comp, eq_y_comp], dict=True)
        sol = []
        for i in range(len(out)):
            landa = out[i][sp.symbols('lambda')]
            if landa != 0:
                phi = out[i][sp.symbols('phi')]
                z_i = eq_z.subs(sp.symbols('lambda'), landa).subs(sp.symbols('phi'), phi).evalf()
                if z_i.is_real:
                    sol.append(z_i)
        return sol
    
    def reconstruct_y(self, x, z):

        eq = self.cone_eq
        eq_x = eq[0]
        eq_y = eq[1]
        eq_z = eq[2]

        eq_x_comp = sp.Eq(eq_x, x)
        eq_z_comp = sp.Eq(eq_z, z)
        out = sp.solvers.solve([eq_x_comp, eq_z_comp], dict=True)
        sol = []
        for i in range(len(out)):
            landa = out[i][sp.symbols('lambda')]
            if landa != 0:
                phi = out[i][sp.symbols('phi')]
                y_i = eq_y.subs(sp.symbols('lambda'), landa).subs(sp.symbols('phi'), phi).evalf()
                if y_i.is_real:
                    sol.append(y_i)
        return sol
    
    def check_simulated_source_pos(self):
        """
        Check if the source position is inside the cone.

        Returns
        -------
        bool
            True if the source position is inside the cone, False otherwise.
        """
        eq = self.cone_eq
        eq_x = eq[0]
        eq_y = eq[1]
        eq_z = eq[2]

        eq_x_comp = sp.Eq(eq_x, SOURCE_POSITION[0])
        eq_y_comp = sp.Eq(eq_y, SOURCE_POSITION[1])
        out = sp.solvers.solve([eq_x_comp, eq_y_comp], dict=True)
        for i in range(len(out)):
            landa = out[i][sp.symbols('lambda')]
            phi = out[i][sp.symbols('phi')]
            z_i = eq_z.subs(sp.symbols('lambda'), landa).subs(sp.symbols('phi'), phi).evalf()

            if z_i.is_real and z_i-SOURCE_POSITION[2]<1e-10:
                return True
            elif z_i.is_real:
                print(f"Distance to the source is {z_i-SOURCE_POSITION[2]}")
        return False

# vertex, hit, theta_m, E_1, E_2, event, theta_E = read_root.extract_variables("Results/Validation/validation12.root", read_partially=True)

# cones = []
# for i in range(len(E_1)):
    # cones.append(Cone(vertex[i], hit[i], E_1[i], E_2[i], theta_m[i], event[i]))
# cones[0].plot_event_2d()
# cones[0].plot_cone_3d_lines()
# sp.pprint(cones[0].cone_equation())

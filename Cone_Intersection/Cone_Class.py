import numpy as np
import matplotlib.pyplot as plt
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

    def plot_cone_3d_lines(self, title = None):
        """
        Plot the photon trajectory in 3D.

        """
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_zlabel('Z')

        # Plot the source, vertex, hit
        ax.scatter(SOURCE_POSITION[0], SOURCE_POSITION[1], SOURCE_POSITION[2], c='r', marker='o', label='Source')
        ax.scatter(self.vertex[0], self.vertex[1], self.vertex[2], c='b', marker='o', label='Vertex')
        ax.scatter(self.hit[0], self.hit[1], self.hit[2], c='g', marker='o', label='Hit')

        # Plot the photon trajectory
        ax.plot([self.vertex[0], self.hit[0]], [self.vertex[1], self.hit[1]], [self.vertex[2], self.hit[2]], c='k', label='Photon trajectory')
        point = self.vertex-self.hit
        r = np.array([0,1,0])
        # Rotate in a perpendicular vector

        point_rot = uf.rotate(self.compton_angle, np.cross(point, r), point)
        
        phi = np.linspace(0, 2*np.pi, 100)

        for p in phi:
            point_rot_phi = uf.rotate(p, point, point_rot) + self.vertex
            ax.plot([point_rot_phi[0], self.vertex[0]], [point_rot_phi[1], self.vertex[1]], [point_rot_phi[2], self.vertex[2]], c='y')

        plt.legend(loc='best')
        if title:
            plt.title(title)
        else:
            plt.title(f"Event {self.event}")
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





vertex, hit, theta_m, E_1, E_2, event = read_root.extract_variables("Results/Validation/validation12.root")
vertex, hit, theta_m, E_1, E_2, event, theta_E = read_root.select_events(vertex, hit, theta_m, E_1, E_2, event, M_ELECTRON, energy_tol=1e-10)

cones = []
for i in range(len(E_1)):
    if theta_E[i] is not None:
        cones.append(Cone(vertex[i], hit[i], E_1[i], E_2[i], theta_m[i], event[i]))
cones[0].plot_event_2d()
cones[0].plot_cone_3d_lines()
print(cones[0].min_distance())


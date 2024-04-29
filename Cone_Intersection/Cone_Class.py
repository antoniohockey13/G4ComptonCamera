import numpy as np
import read_root

# Units
mm = 1
keV = 1
# Constants
M_ELECTRON = 510.99895000*keV
SOURCE_POSITION = np.array([-682/2*mm, 0*mm, 0*mm])


class Cone:
    def __init__(self, vertex, hit, E1, E2):
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
        """
        self.vertex = vertex
        self.E1 = E1
        self.E2 = E2
        self.compton_angle = self.aperture_angle()
        self.hit = hit
        a = hit - vertex
        self.axis = a/np.linalg.norm(a)
        
        self.angle_with_axis = self.angle_with_axis()

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
    

vertex, hit, theta_m, E_1, E_2, event = read_root.extract_variables("Results/Validation/validation12.root")
vertex, hit, theta_m, E_1, E_2, event, theta_E = read_root.select_events(vertex, hit, theta_m, E_1, E_2, event, M_ELECTRON, energy_tol=1e-10)

cones = []
for i in range(len(E_1)):
    if theta_E[i] is not None:
        cones.append(Cone(vertex[i], hit[i], E_1[i], E_2[i]))


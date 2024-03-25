import numpy as np
import awkward as ak
from scipy.optimize import fsolve
import matplotlib.pyplot as plt
import ROOT

def cono(pto: np.array, vertex: np.array, theta: float):
    """
    Cone equation in 3D
    
    Parameters:
    pto: np.array
        Point to evaluate
    vertex: np.array
        Vertex of the cone
    theta: float
        Cone angle
    """
    x0, y0, z0 = vertex
    return ((pto[2]-z0)**2 + (pto[1]-y0)**2) * np.cos(theta)**2 - (pto[0]-x0)**2 * np.sin(theta)**2

def rotation(pto: np.array, angles: np.array):
    """
    Rotate a point in 3D space
    
    Parameters:
    pto: np.array
        Point to rotate
    angles: np.array
        Angles to rotate the point with respect to the Y and Z axis
    """
    alpha, beta = angles
    ejeY = np.array([[np.cos(alpha), 0, np.sin(alpha)],
                        [0, 1, 0],
                        [-np.sin(alpha), 0, np.cos(alpha)]])
    
    ejeZ = np.array([[np.cos(beta), -np.sin(beta), 0],
                        [np.sin(beta), np.cos(beta), 0],
                        [0, 0, 1]])
    resultado = np.dot(ejeY, np.dot(ejeZ, pto))
    return resultado

def eq_cono(pto: np.array, detector_info: np.array):
    """
    Equation of the cone in 3D space rotated

    Parameters:
    pto: np.array
        Point to evaluate
    detector_info: np.array
        Detector information (r1, r2, theta, angles)
    """
    r1, r2, theta, angles = detector_info
    pto_rot = rotation(pto, angles)
    return cono(pto_rot, r1, theta)



def sistema_eq_cono(pto, conos):
    """
    System of equations to solve the intersection of a point with the cones

    Parameters:
    pto: np.array
        Point to evaluate
    conos: np.array
        Detector information (r1, r2, theta, angles)
    """
    return [
        eq_cono(pto, conos[0]),
        eq_cono(pto, conos[1]),
        eq_cono(pto, conos[2])
    ]

def rotation_angles(r1, r2):
    alpha = np.arctan2(r2[2]-r1[2], r2[0]-r1[0])
    beta = np.arctan2(r2[1]-r1[1], r2[0]-r1[0])
    return np.array([alpha, beta])

#Conos r1, r2, theta
conos = ak.Array([
    [np.array([-240.9564, 0, 0]), np.array([-141.0432, 87.994580, -109.7145]), 0.9531211],
    [np.array([-241.0494, 0, 0]), np.array([-140.9916, 32.530716, -92.98415]), 0.7776053],
    [np.array([-240.9422, 0, 0]), np.array([-140.9891, -133.9434, -38.89468]), 0.9489951],
    [np.array([-240.9378, 0, 0]), np.array([-141.0645, -10.31931, 135.40849]), 0.9366535],
    [np.array([-240.9442, 0, 0]), np.array([-141.0589, 53.872345, 16.822840]), 0.5143070]
])

conos_angles = ak.Array([
    [np.array([-240.9564, 0, 0]), np.array([-141.0432, 87.994580, -109.7145]), 0.9531211, rotation_angles(conos[0][0], conos[0][1])],
    [np.array([-241.0494, 0, 0]), np.array([-140.9916, 32.530716, -92.98415]), 0.7776053, rotation_angles(conos[1][0], conos[1][1])],
    [np.array([-240.9422, 0, 0]), np.array([-140.9891, -133.9434, -38.89468]), 0.9489951, rotation_angles(conos[2][0], conos[2][1])],
    [np.array([-240.9378, 0, 0]), np.array([-141.0645, -10.31931, 135.40849]), 0.9366535, rotation_angles(conos[3][0], conos[3][1])],
    [np.array([-240.9442, 0, 0]), np.array([-141.0589, 53.872345, 16.822840]), 0.5143070, rotation_angles(conos[4][0], conos[4][1])]
    ])

pto_inicial = np.array([-340.0,0,0])

a = (eq_cono(conos_angles[0][0], conos_angles[0]))
print(a)

resultado = fsolve(sistema_eq_cono, pto_inicial, args=(conos_angles,))
print(resultado)
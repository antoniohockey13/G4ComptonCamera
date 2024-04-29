import ROOT
import numpy as np 
import matplotlib.pyplot as plt

import read_root

# Units
mm = 1
keV = 1
# Constants
M_ELECTRON = 510.99895000*keV
SOURCE_POSITION = np.array([-682/2*mm, 0*mm, 0*mm])

def rotate(phi, u, x):
    """
    Parameters
    ----------
    phi : float
        Rotation angle in radians.
    u : numpy.array
        Unitary vector in the direction of the cone axis.
    x : numpy.array
        Vector to be rotated.
    
    Returns
    -------
    numpy.array
        Rotated vector.
    """
    if np.linalg.norm(u) != 1:
        u = u/np.linalg.norm(u)
    r_phi = np.array([[np.cos(phi)+u[0]**2*(1-np.cos(phi)), u[0]*u[1]*(1-np.cos(phi))-u[2]*np.sin(phi), u[0]*u[2]*(1-np.cos(phi))+u[1]*np.sin(phi)],
                        [u[1]*u[0]*(1-np.cos(phi))+u[2]*np.sin(phi), np.cos(phi)+u[1]**2*(1-np.cos(phi)), u[1]*u[2]*(1-np.cos(phi))-u[0]*np.sin(phi)],
                        [u[2]*u[0]*(1-np.cos(phi))-u[1]*np.sin(phi), u[2]*u[1]*(1-np.cos(phi))+u[0]*np.sin(phi), np.cos(phi)+u[2]**2*(1-np.cos(phi))]])
    return np.dot(r_phi, x)

def min_distance(vertex, hit, theta, source):
    """
    Compute the minimum distance between a point and a straight line in 3d.
    
    Parameters
    ----------
    vertex : numpy.ndarray
        Numpy array containing the vertex coordinates (X1, Y1, Z1).
    hit : numpy.ndarray
        Numpy array containing the hit coordinates (X2, Y2, Z2).
    theta : float
        Compton angle in radians.
    source : numpy.ndarray
        Numpy array containing the source coordinates (X0, Y0, Z0).

    Returns
    -------
    float
        Minimum distance.
    """

    point = vertex-hit
    r = np.array([0,1,0])
    # Rotate in a perpendicular vector
    point_rot = rotate(theta, np.cross(point, r), point)

    min_dist = np.inf
    phi = np.linspace(0, 2*np.pi, 100)
    phi = np.append(phi, np.linspace(1.13488889, 1.13747475, 10)) # best_phi for event 0
    phi = np.append(phi, np.linspace(2.67777421, 2.68033851, 10)) # best_phi for event 1
    for p in phi:
        point_rot_phi = rotate(p, point, point_rot) + vertex
        dist = np.linalg.norm(np.cross(point_rot_phi, source))/np.linalg.norm(point_rot_phi)
        if dist < min_dist:
            min_dist = dist
            best_phi = p
    print(f"Best phi: {best_phi}")
    return min_dist
    

def plot_cone(vertex, hit, theta_m, distance):
    """
    Plot the photon trajectory in 3D.
    Parameters
    ----------
    vertex : numpy.ndarray
        Numpy array containing the vertex coordinates (X1, Y1, Z1).
    hit : numpy.ndarray
        Numpy array containing the hit coordinates (X2, Y2, Z2).
    theta_m : float
        Compton angle in radians, computed kinematically.
    distance : float
        Minimum distance between the cone and the source position.
    
    """
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')

    # Plot the source, vertex, hit
    ax.scatter(SOURCE_POSITION[0], SOURCE_POSITION[1], SOURCE_POSITION[2], c='r', marker='o', label='Source')
    ax.scatter(vertex[0], vertex[1], vertex[2], c='b', marker='o', label='Vertex')
    ax.scatter(hit[0], hit[1], hit[2], c='g', marker='o', label='Hit')

    # Plot the photon trajectory
    ax.plot([vertex[0], hit[0]], [vertex[1], hit[1]], [vertex[2], hit[2]], c='k', label='Photon trajectory')
    point = vertex-hit
    r = np.array([0,1,0])
    # Rotate in a perpendicular vector
    point_rot = rotate(theta_m, np.cross(point, r), point)
    
    phi = np.linspace(0, 2*np.pi, 100)
    phi = np.append(phi, np.linspace(1.13488889, 1.13747475, 10)) # best_phi for event 0
    phi = np.append(phi, np.linspace(2.67777421, 2.68033851, 10)) # best_phi for event 1
    for p in phi:
        point_rot_phi = rotate(p, point, point_rot) + vertex
        ax.plot([point_rot_phi[0], vertex[0]], [point_rot_phi[1], vertex[1]], [point_rot_phi[2], vertex[2]], c='y')

    ax.text(-300, 100, 0, f"Min distance {distance:.2f} mm")
    plt.legend(loc='best')
    plt.show()


vertex, hit, theta_m, E_1, E_2, event = read_root.extract_variables("Results/Validation/validation12.root")
vertex, hit, theta_m, E_1, E_2, event, theta_E = read_root.select_events(vertex, hit, theta_m, E_1, E_2, event, M_ELECTRON, energy_tol=1e-10)

# distance_m = []
# distance_E = []
# for i in range(len(E_1)):
#     distance_m.append(min_distance(vertex[i], hit[i], theta_m[i], SOURCE_POSITION))
#     distance_E.append(min_distance(vertex[i], hit[i], theta_E[i], SOURCE_POSITION))

plot_cone(vertex[0], hit[0], theta_m[0], min_distance(vertex[0], hit[0], theta_m[0], SOURCE_POSITION))
plot_cone(vertex[0], hit[0], theta_E[0], min_distance(vertex[0], hit[0], theta_E[0], SOURCE_POSITION))

plot_cone(vertex[1], hit[1], theta_m[1], min_distance(vertex[1], hit[1], theta_m[1], SOURCE_POSITION))
plot_cone(vertex[1], hit[1], theta_E[1], min_distance(vertex[1], hit[1], theta_E[1], SOURCE_POSITION))
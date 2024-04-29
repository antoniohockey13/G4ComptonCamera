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


vertex, hit, theta_m, E_1, E_2, event = read_root.extract_variables("Results/Validation/validation12.root")
vertex, hit, theta_m, E_1, E_2, event, theta_E = read_root.select_events(vertex, hit, theta_m, E_1, E_2, event, M_ELECTRON, energy_tol=1e-10)


def cone_in_event_coordinates(vertex, theta, y, z):
    """
    Cone equation in event coordinates
    WARNING 
    Need to duplicate results +- of sqrt
    Parameters
    ----------
    vertex : array-like
        Vertex of the cone
    theta : float
        Opening angle of the cone
    y : float
        y-coordinate of the point
    z : float
        z-coordinate of the point
    
    Returns
    -------
    x : float
        x-coordinate of the point
    """
    # ((y-y_0)**2+(z-z_0)**2)*np.cos(theta)**2-(x-x_0)**2*np.sin(theta)**2 # Cone implicit equation
    x_0 = vertex[0]
    y_0 = vertex[1]
    z_0 = vertex[2]
    return np.sqrt((y-y_0)**2+(z-z_0)**2)/np.tan(theta)+x_0


def rotate_in_not_origin(phi, u, v, x):
    """
    Rotate a point in the space around a vector u in a pouint v by an angle phi
    Parameters
    ----------
    phi : float
        Angle of rotation
    u : array-like
        Vector around which the rotation is performed
    v : array-like
        Point around which the rotation is performed
    x : array-like
        Point to rotate
    
    Returns
    -------
    x_rot : array-like
        Rotated point
    """
    v = v.reshape(-1,1)
    x = np.append(x, 0)
    if np.linalg.norm(u) != 1:
        u = u/np.linalg.norm(u)
    r_phi = np.array([[np.cos(phi)+u[0]**2*(1-np.cos(phi)), u[0]*u[1]*(1-np.cos(phi))-u[2]*np.sin(phi), u[0]*u[2]*(1-np.cos(phi))+u[1]*np.sin(phi)],
                        [u[1]*u[0]*(1-np.cos(phi))+u[2]*np.sin(phi), np.cos(phi)+u[1]**2*(1-np.cos(phi)), u[1]*u[2]*(1-np.cos(phi))-u[0]*np.sin(phi)],
                        [u[2]*u[0]*(1-np.cos(phi))-u[1]*np.sin(phi), u[2]*u[1]*(1-np.cos(phi))+u[0]*np.sin(phi), np.cos(phi)+u[2]**2*(1-np.cos(phi))]])
 
    rotation = np.block([[r_phi, v-np.dot(r_phi, v)],
                         [0, 0 ,0, 1]])
    return np.dot(rotation, x)
     


def plot_cone(vertex, hit, theta):

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.set_xlabel('X/mm')
    ax.set_ylabel('Y/mm')
    ax.set_zlabel('Z/mm')

    ax.scatter(SOURCE_POSITION[0], SOURCE_POSITION[1], SOURCE_POSITION[2], c='r', marker='o', label='Source')
    ax.scatter(vertex[0], vertex[1], vertex[2], c='b', marker='o', label='Vertex')
    ax.scatter(hit[0], hit[1], hit[2], c='g', marker='o', label='Hit')

    # Plot the photon trajectory
    ax.plot([vertex[0], hit[0]], [vertex[1], hit[1]], [vertex[2], hit[2]], c='k', label='Photon trajectory')
     
    # Compute angle between vertex-hit and x-axis
    phi = np.arccos((hit[0]-vertex[0])/np.linalg.norm(hit-vertex))
    point = vertex-hit
    r = np.array([0,1,0])
    u = np.cross(point, r)
    y = np.linspace(-100, 100, 100)
    z = np.linspace(-100, 100, 100)
    x_plot =[]
    for iy in y:
        for iz in z:
            x = cone_in_event_coordinates(vertex, theta, iy, iz)
            x_rot = rotate_in_not_origin(phi, u, vertex, np.array([x, iy, iz]))
            x_plot.append(x_rot)
    x_plot = np.array(x_plot)
    ax.plot_trisurf(x_plot[0], x_plot[1], x_plot[2], label='Cone')

    plt.legend(loc='best')  
    plt.show()

    ax = fig.add_subplot(111, projection='3d')
    ax.set_xlabel('X/mm')
    ax.set_ylabel('Y/mm')
    ax.set_zlabel('Z/mm')

    ax.scatter(SOURCE_POSITION[0], SOURCE_POSITION[1], SOURCE_POSITION[2], c='r', marker='o', label='Source')
    ax.scatter(vertex[0], vertex[1], vertex[2], c='b', marker='o', label='Vertex')
    ax.scatter(hit[0], hit[1], hit[2], c='g', marker='o', label='Hit')

    # Plot the photon trajectory
    ax.plot([vertex[0], hit[0]], [vertex[1], hit[1]], [vertex[2], hit[2]], c='k', label='Photon trajectory')
     
    # Compute angle between vertex-hit and x-axis
    phi = np.arccos((hit[0]-vertex[0])/np.linalg.norm(hit-vertex))
    point = vertex-hit
    r = np.array([0,1,0])
    u = np.cross(point, r)
    y = np.linspace(-100, 100, 100)
    z = np.linspace(-100, 100, 100)
    x_plot =[]
    for iy in y:
        for iz in z:
            x = cone_in_event_coordinates(vertex, theta, iy, iz)
            x_rot = rotate_in_not_origin(phi, u, vertex, np.array([x, iy, iz]))
            x_plot.append(x_rot)
    x_plot = np.array(x_plot)

    plt.legend(loc='best')  
    plt.show()

plot_cone(vertex[0], hit[0], theta_m[0])

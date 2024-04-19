import numpy as np
import matplotlib.pyplot as plt

def rotate_y_axis(theta, u):
    """
    Parameters
    ----------
    theta : float
        Rotation angle in radians.
    u : numpy.array
        Unitary vector in the direction of the cone axis. COLUMN VECTOR
    Returns
    -------
    numpy.array
        Rotated vector.
    """
    r_theta = np.array([[np.cos(theta), 0, np.sin(theta)],
                        [0, 1, 0],
                        [-np.sin(theta), 0, np.cos(theta)]])
    return np.dot(r_theta, u)

def rotate_in_cone_axis(phi, u, x):
    """
    Parameters
    ----------
    phi : float
        Rotation angle in radians.
    u : numpy.array
        Unitary vector in the direction of the cone axis. COLUMN VECTOR
    Returns
    -------
    numpy.array
        Rotated vector.
    """
    r_phi = np.array([[np.cos(phi)+u[0]**2*(1-np.cos(phi)), u[0]*u[1]*(1-np.cos(phi))-u[2]*np.sin(phi), u[0]*u[2]*(1-np.cos(phi))+u[1]*np.sin(phi)],
                        [u[1]*u[0]*(1-np.cos(phi))+u[2]*np.sin(phi), np.cos(phi)+u[1]**2*(1-np.cos(phi)), u[1]*u[2]*(1-np.cos(phi))-u[0]*np.sin(phi)],
                        [u[2]*u[0]*(1-np.cos(phi))-u[1]*np.sin(phi), u[2]*u[1]*(1-np.cos(phi))+u[0]*np.sin(phi), np.cos(phi)+u[2]**2*(1-np.cos(phi))]])
    return np.dot(r_phi, x)


def cone_surface(landa, phi, theta, u, vertex):
    """
    Cone surface

    Parameters
    ----------
    landa : float
        Parametric constant
    phi : float
        Azimuth angle in radians.
    theta : float
        Aperture angle in radians.
    u : float
        Unitary vector in the direction of the cone axis.
    Returns
    -------
    numpy.array(x,y,z)
        Point in the cone surface.
    """
    x_rot = rotate_y_axis(theta, u)
    x_cone = rotate_in_cone_axis(phi, u, x_rot)
    return landa*x_cone + vertex


def plot_cone(theta, u, vertex, hit):
    landa = np.linspace(-100, 100, 50)
    landa1 = np.linspace(-105, -90, 75)
    landa = np.concatenate((landa, landa1))
    phi = np.linspace(0, 2*np.pi, 50)
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.plot(vertex[0], vertex[1], vertex[2], 'ro')
    distance_min = np.inf
    for p in phi:
        for l in landa:
            x, y, z = cone_surface(l, p, theta, u, vertex)
            ax.plot(x, y, z, 'b.')
            distance = np.linalg.norm(np.array([x,y,z])-np.array([-340,0,0]))
            if distance < distance_min:
                distance_min = distance
                x_min = x
                y_min = y
                z_min = z
                landa_min = l
                phi_min = p

    print("Minimum distance", distance_min)
    print("Point", x_min, y_min, z_min)
    print(f"Minimum landa {landa_min} and phi {phi_min}")

    source_position = np.array([-682/2,0,0])
    ax.scatter(source_position[0], source_position[1], source_position [2], c='g', marker='o', label='Source')
    ax.scatter(x_min, y_min, z_min, c='r', marker='o', label='Minimum distance')
    # Line from source to vertex
    ax.plot([source_position[0], vertex[0]], [source_position[1], vertex[1]], [source_position[2], vertex[2]], 'r')
    # Line from vertex to hit
    ax.plot([vertex[0], hit[0]], [vertex[1], hit[1]], [vertex[2], hit[2]], 'r')
    ax.legend()
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    plt.show()



# Plot 1 cone

vertex1 = np.array([-240.9564, 0, 0])
hit1 = np.array([-141.0432, 87.994580, -109.7145])
v1 = hit1-vertex1
u1 = v1/np.linalg.norm(v1)
theta1 = 0.9531211
plot_cone(theta1, u1, vertex1, hit1)

# Cone 2
vertex2 = np.array([-240.9422, 0, 0])
hit2 = np.array([-140.9891, -133.9434, -38.89468])
v2 = hit2-vertex2
u2 = v2/np.linalg.norm(v2)
theta2 = 0.9489951
# plot_cone(theta2, u2, vertex2, hit2)

# Cone 3
vertex3 = np.array([-240.9378, 0, 0])
hit3 = np.array([-141.0645, -10.21931, 135.40849])
v3 = hit3-vertex3
u3 = v3/np.linalg.norm(v3)
theta3 = 0.9366535
#plot_cone(theta3, u3, vertex3, hit3)




"""
# Find intersection
tolerance = 25

# As the vector u is defined landa must be megative to found source position

intersection_points = []
intersection_point_high_accuracy = []
landa1_intersection = []
phi1_intersection = []

for landa1 in np.linspace(-100, -90,50):
    for phi1 in np.linspace(0, 2*np.pi, 50):
        point1 = cone_surface(landa1, phi1, theta1, u1, vertex1)

        for landa2 in np.linspace(-100, -90, 50):
            for phi2 in np.linspace(0, 2*np.pi, 10):
                point2 = cone_surface(landa2, phi2, theta2, u2, vertex2)

                if np.linalg.norm(point1 - point2) < tolerance:
                    for landa3 in np.linspace(-100, -90, 50):
                        for phi3 in np.linspace(0, 2*np.pi, 10):
                            point3 = cone_surface(landa3, phi3, theta3, u3, vertex3)
                            distance = min(np.linalg.norm(point1 - point3), np.linalg.norm(point2 - point3))
                            if distance < tolerance:
                                intersection_points.append(point1)
                                landa1_intersection.append(landa1)
                                phi1_intersection.append(phi1)
                                print("Intersection found", point1, point2, point3)
                                print(f"Distance with 3 {distance}")
                                print(f"Distance 1-2 {np.linalg.norm(point1 - point2)}")    
# mean of each of the components
intersection_points = np.array(intersection_points)
print(np.mean(intersection_points, axis=0))
print(tolerance)
"""
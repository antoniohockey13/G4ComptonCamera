import numpy as np 
import matplotlib.pyplot as plt

def cambio_base(v, u, d):
    """
    Cambio de base de coordenadas.

    Parameters
    ----------
    v : numpy.array(x,y,z)
        Vector en la base x,y,z canóanglenica.
    u : numpy.array(x,y,z)
        Vector en x,y,z del plano compton.
    d : numpy.array(x,y,z)
        Vector en x,y,z del plano compton.
    """
    # v = a*d + b*u
    # d = (1,0,0)
    if not np.array_equal(d, np.array([1,0,0])):
        print(" El vector d no es el esperado")
        return None
    b1 = v[1]/u[1]
    b2 = v[2]/u[2]
    if np.absolute(b1-b2)>1e-10:
        print("El vector introducido no es coplanar con el plano compton")
        return None
    a = v[0]-u[0]*b1
    return np.array([a, b1])

def rotate(phi, u, x):
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

def Angle_Energy(E_1,E_2):
    """
    Compute the compton angle

    Parameters
    ----------
    E_1 : float
        Energy lost in first detector.
    E_2 : float
        Energy lost in second detector.
    
    Returns
    -------
    float
        Compton angle in radians.
    """
    return np.arccos(1-511*E_1/(E_2*(E_1+E_2)))

def Compute_variables_to_plot(hit, vertex, source_position):
    """
    Compute the variables to plot the event in the compton plane.

    Parameters
    ----------
    hit : numpy.array(x,y,z)
        Hit position.
    vertex : numpy.array(x,y,z)
        Vertex position.
    E1 : float
        Energy lost in first detector.
    E2 : float
        Energy lost in second detector.
    """
    v = hit-vertex
    u = v/np.linalg.norm(v)
    base = np.array([0, u[1], u[2]])
    base = base/np.linalg.norm(base)

    source_vertex = vertex1- source_position
    d = source_vertex/np.linalg.norm(source_vertex)

    return vertex, hit, source_position, u, d, base


def plot_event(hit, vertex, source, theta, title=None):
    """
    Plot the event in the compton plane.

    Parameters
    ----------
    hit : numpy.array(x,y,z)
        Hit position.
    vertex : numpy.array(x,y,z)
        Vertex position.
    source : numpy.array(x,y,z)
        Source position.
    theta : float
        Compton angle in radians.
    title : str
        Title of the plot.
    """

    vertex, hit, source_position, u, d, base = Compute_variables_to_plot(hit, vertex, source)

    # Cambio de base
    source_position_compton = cambio_base(source, base, d)
    vertex_compton = cambio_base(vertex, base, d)
    hit_compton = cambio_base(hit, base, d)

    # Normal vector to the compton plane
    normal = np.cross(base, d)

    # Plot
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.scatter(source_position_compton[0], source_position_compton[1], c='g', marker='o', label='Source')
    ax.scatter(vertex_compton[0], vertex_compton[1], c='r', marker='o', label='Vertex')
    ax.scatter(hit_compton[0], hit_compton[1], c='b', marker='o', label='Hit')


    # Rotar recta angulo theta respecto de vector normal al plano compton
    vertex_hit = []
    source_hit = []
    lambda1 = np.linspace(-150, 200, 250)
    for l in lambda1:
        x = l*u
        x_v = x + vertex
        x_compton = cambio_base(x_v, base, d)
        vertex_hit.append(x_compton)
            
        x_rot = rotate(theta, normal, x)
        x_rot = x_rot + vertex
        x_rot_compton = cambio_base(x_rot, base, d) 
        source_hit.append(x_rot_compton)
    
    vertex_hit = np.array(vertex_hit)
    source_hit = np.array(source_hit)

    ax.plot(vertex_hit[:,0], vertex_hit[:,1], 'r', label='Vertex-Hit')
    ax.plot(source_hit[:,0], source_hit[:,1], 'b', label='Source-Hit')

    ax.legend(loc='best')
    ax.xaxis.set_label_text(f'd = x = {d}')
    ax.yaxis.set_label_text(f'base = {base}')
    plt.xlim(-400,-100)
    plt.ylim(-100,200)
    # Assuming photon propagates in straight line, compute minimum distance between source and source-hit line
    min_distance = np.linalg.norm(np.cross(source_hit[0]- source_hit[-1], source_hit[-1] - source_position_compton)) / np.linalg.norm(source_hit[0] - source_hit[-1])
    print(f"Minimum distance {min_distance}")
    # Show min distance in plot
    ax.text(-300, 100, f"Min distance: {min_distance:.8f}", fontsize=12)
    if title:
        plt.title(title)
    plt.show()

    return min_distance

source_position = np.array([-682/2,0,0])

distance_energy = {"Distance" : [],
                    "Energy" : []}

# Event 334739
vertex1 = np.array([-240.9564, 0, 0])
hit1 = np.array([-141.0432, 87.994580, -109.7145])

theta1_m = 0.9531211
E1 = 4.4095097
E2 = 65.590490
theta1_E = Angle_Energy(E1,E2)
print(theta1_E)

plot_event(hit1, vertex1, source_position, theta1_m, title="Event 334739, kinematics theta")
min_distance = plot_event(hit1, vertex1, source_position, theta1_E, title="Event 334739, energy theta")
distance_energy["Distance"].append(min_distance)
distance_energy["Energy"].append(70-(E1+E2))

# # Event 774945
# vertex2 = np.array([-240.9378, 0, 0])
# hit2 = np.array([-141.0645, -10.21931, 135.40849])

# theta2_m = 0.9366535
# E1 = 3.8420938
# E2 = 66.157906
# theta2_E = Angle_Energy(E1,E2)

# plot_event(hit2, vertex2, source_position, theta2_m, title="Event 774945, kinematics theta")
# min_distance = plot_event(hit2, vertex2, source_position, theta2_E, title="Event 774945, energy theta")

# distance_energy["Distance"].append(min_distance)
# distance_energy["Energy"].append(70-(E1+E2))


# # Event 1713126
# vertex3 = np.array([-240.9915, 0, 0])
# hit3 = np.array([-141.0205, -53.53393, -104.8608])

# theta3_m = 0.8668164
# E1 = 2.7505186
# E2 = 67.249481
# theta3_E = Angle_Energy(E1,E2)

# plot_event(hit3, vertex3, source_position, theta3_m, title="Event 1713126, kinematics theta")
# min_distance = plot_event(hit3, vertex3, source_position, theta3_E, title="Event 1713126, energy theta")
# distance_energy["Distance"].append(min_distance)
# distance_energy["Energy"].append(70-(E1+E2))

# # |E1+E2-70| < 
# # Event 36805102
# vertex4 = np.array([-241.0559,0,0])
# hit4 = np.array([-141.0087, 47.907277, -8.239250])

# theta4_m = 0.4522853
# E1 = 0.9374807
# E2 =  69.062519
# theta4_E = Angle_Energy(E1,E2)

# plot_event(hit4, vertex4, source_position, theta4_m, title="Event 36805102, kinematics theta")
# min_distance = plot_event(hit4, vertex4, source_position, theta4_E, title="Event 36805102, energy theta")
# distance_energy["Distance"].append(min_distance)
# distance_energy["Energy"].append(70-(E1+E2))


# # Event 48556354

# vertex5 = np.array([-241.0143,0,0])
# hit5 = np.array([-141.0231, 105.32865, 32.669562])

# theta5_m = 0.8342850
# E1 = 4.1300341
# E2 =  65.869965
# theta5_E = Angle_Energy(E1,E2)

# plot_event(hit5, vertex5, source_position, theta5_m, title="Event 48556354, kinematics theta")
# min_distance = plot_event(hit5, vertex5, source_position, theta5_E, title="Event 48556354, energy theta")
# distance_energy["Distance"].append(min_distance)
# distance_energy["Energy"].append(70-(E1+E2))

# # Plot distance vs energy 
# distance = np.array(distance_energy["Distance"])
# energy = np.array(distance_energy["Energy"])
# # Linear fit
# m, b= np.polyfit(energy, distance, 1)

# e = np.linspace(0, np.max(energy), 100)
# plt.figure()
# plt.plot(energy, distance, 'o')
# plt.plot(e, m*e + b)
# plt.xlabel("Energy")
# plt.ylabel("Distance")
# plt.show()

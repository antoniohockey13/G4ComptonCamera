import ROOT
import numpy as np 
import matplotlib.pyplot as plt
import read_root

# Constants
M_ELECTRON = 510.99895000
source_position = np.array([-682/2,0,0])


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

    source_vertex = vertex - source_position
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
    lambda1 = np.linspace(-150, 200, 2)
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

    ax.text(-300, 100, f"Min distance: {min_distance(hit, vertex, source, theta):.8f}", fontsize=12)
    if title:
        plt.title(title)
    plt.show()  

def min_distance(hit, vertex, source, theta):
    """
    Compute the minimum distance between the line source-hit and the source.

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
    """
    vertex, hit, source_position, u, d, base = Compute_variables_to_plot(hit, vertex, source)

    # Cambio de base
    source_position_compton = cambio_base(source, base, d)

    # Normal vector to the compton plane
    normal = np.cross(base, d)

    # Rotar recta 15.952385292692522 54.04761470730748angulo theta respecto de vector normal al plano compton
    vertex_hit = []
    source_hit = []
    lambda1 = np.linspace(-150, 200, 2)
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

    min_distance = np.linalg.norm(np.cross(source_hit[0]- source_hit[-1], source_hit[-1] - source_position_compton)) / np.linalg.norm(source_hit[0] - source_hit[-1])
    return min_distance

def mass(E1, E2, theta):
    """
    Compute the mass of the electron.

    Parameters
    ----------
    E1 : float
        Energy lost in first detector.
    E2 : float
        Energy lost in second detector.
    theta : float
        Compton angle in radians computed kinematic.
    """
    return (1-np.cos(theta))*E2*(E1+E2)/E1
                    



vertex, hit, theta_m, E1, E2, event = read_root.extract_variables("Results/Validation/validation12.root")
vertex, hit, theta_m, E1, E2, event, theta_E = read_root.select_events(vertex, hit, theta_m, E1, E2, event, M_ELECTRON, energy_tol=1e-10)

distance = []
energy = []
masa = []

for i in range(len(E1)):
    if theta_E[i] is not None:
        distance.append(min_distance(hit[i], vertex[i], source_position, theta_E[i]))
        energy.append(70.0-(E1[i]+E2[i]))
        masa.append(mass(E1[i], E2[i], theta_m[i]))

i = 0
plot_event(hit[i], vertex[i], source_position, theta_m[i], title=f"Event {event[i]}, kinematics theta, theta = {theta_m[i]}")
plot_event(hit[i], vertex[i], source_position, theta_E[i], title=f"Event {event[i]}, energy theta, theta = {theta_E[i]}, E1 = {E1[i]}, E2 = {E2[i]}")
    
#Plot mass histogram
masa = np.array(masa)
plt.figure()
plt.hist(masa, bins=100, color='r', alpha=0.7)
plt.axvline(x=M_ELECTRON, color='k', linestyle='--', label='Electron mass')
plt.legend(loc='best')
plt.yscale('log')
plt.xlabel("Mass/KeV")
plt.ylabel("Counts")
plt.show()

distance = np.array(distance)
energy = np.array(energy)
# Linear fit
m, b= np.polyfit(energy, distance, 1)
e = np.linspace(np.min(energy), np.max(energy), 100)
plt.figure()
plt.plot(energy, distance, 'o', c = 'r', label = 'Data')
plt.plot(e, m*e + b, '-', c = 'b', label = f'Linear fit: y = {m:.2e}x + {b:.2e}')
plt.xlabel("Energy 70-(E1+E2)/keV")
plt.ylabel("Distance/mm")
plt.legend(loc='best')
plt.show()


# Plot mass vs distance
plt.figure()
plt.plot(masa, distance, 'o', c = 'r')
plt.axvline(x=M_ELECTRON, color='k', linestyle='--', label='Electron mass')
plt.xlabel("Mass/keV")
plt.ylabel("Distance/mm")
plt.show()

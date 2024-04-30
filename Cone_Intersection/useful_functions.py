import numpy as np

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
    if np.linalg.norm(u) != 1:
        u = u/np.linalg.norm(u)
    r_phi = np.array([[np.cos(phi)+u[0]**2*(1-np.cos(phi)), u[0]*u[1]*(1-np.cos(phi))-u[2]*np.sin(phi), u[0]*u[2]*(1-np.cos(phi))+u[1]*np.sin(phi)],
                        [u[1]*u[0]*(1-np.cos(phi))+u[2]*np.sin(phi), np.cos(phi)+u[1]**2*(1-np.cos(phi)), u[1]*u[2]*(1-np.cos(phi))-u[0]*np.sin(phi)],
                        [u[2]*u[0]*(1-np.cos(phi))-u[1]*np.sin(phi), u[2]*u[1]*(1-np.cos(phi))+u[0]*np.sin(phi), np.cos(phi)+u[2]**2*(1-np.cos(phi))]])
    
    return np.dot(r_phi, x)
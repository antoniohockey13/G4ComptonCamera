import numpy as np 
import matplotlib.pyplot as plt

# Event 1
vertex1 = np.array([-240.9564, 0, 0])
hit1 = np.array([-141.0432, 87.994580, -109.7145])
v1 = hit1-vertex1
u1 = v1/np.linalg.norm(v1)
base = np.array([0, u1[1], u1[2]])
theta1 = 0.9531211
# theta1 = 0.9531211061511268

source_position = np.array([-682/2,0,0])

angle = np.arccos((np.dot((vertex1-source_position), v1))/(np.linalg.norm(v1)*np.linalg.norm(source_position-vertex1)))
print(angle)

# Plot the event in the compton plane
# Compton plane define by the vectors source-vertex and vertex-hit

source_vertex = vertex1 - source_position
d = source_vertex/np.linalg.norm(source_vertex)

# Normal vector to the compton plane
normal = np.cross(base, d)

# Vectores de la base son d (eje x) y u1
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
    # r_phi = np.array([[np.cos(phi), -np.sin(phi)],
    #                 [np.sin(phi), np.cos(phi)]])
    return np.dot(r_phi, x)

# Cambio de base
source_position_compton = cambio_base(source_position, base, d)
vertex_compton = cambio_base(vertex1, base, d)
hit_compton = cambio_base(hit1, base, d)
u1_compton = cambio_base(u1, base, d)


# Plot
fig = plt.figure()
ax = fig.add_subplot(111)
ax.scatter(source_position_compton[0], source_position_compton[1], c='g', marker='o', label='Source')
ax.scatter(vertex_compton[0], vertex_compton[1], c='r', marker='o', label='Vertex')
ax.scatter(hit_compton[0], hit_compton[1], c='b', marker='o', label='Hit')


# Rotar recta angulo theta respecto de vector normal al plano compton

lambda1 = np.linspace(-150, 200, 75)
for l in lambda1:
    x = l*u1
    x_v = x + vertex1
    x_compton = cambio_base(x_v, base, d)
    ax.scatter(x_compton[0], x_compton[1], c='k', marker='.')

    x_rot = rotate(theta1, normal, x)
    x_rot = x_rot + vertex1
    x_rot_compton = cambio_base(x_rot, base, d) 
    ax.scatter(x_rot_compton[0], x_rot_compton[1], c='y', marker='.')

ax.legend(loc='best')
ax.xaxis.set_label_text(f'd = x = {d}')
ax.yaxis.set_label_text(f'base = {base}')
plt.xlim(-400,-100)
plt.ylim(-100,200)
plt.show()

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

# Define parameters
x0, y0, z0 = vertex[0]
theta = theta_m[0]

# Generate cone surface points
Y = np.linspace(-100, 100, 2)
Z = np.linspace(-100, 100, 2)
Y, Z = np.meshgrid(Y, Z)
print(Y, Z)
# Solve for x
X = -np.sqrt((Y-y0)**2 + (Z-z0)**2) /np.tan(theta) + x0

# phi angle between x-axis and hit-vertex
phi = np.arccos((hit[0][0]-vertex[0][0])/np.linalg.norm(hit[0]-vertex[0]))

# u vector normal to hit-vertex
r = np.array([0,1,0])
u = np.cross(hit[0]-vertex[0], r)

x, y, z = [], [], []
for i in range(len(X)):
    X_i = X[i]
    Y_i = Y[i]
    Z_i = Z[i]
    x_rotated = []
    y_rotated = []
    z_rotated = []
    for j in range(len(X[i])):
        rotated = rotate_in_not_origin(phi, u, vertex[0], np.array([X_i[j], Y_i[j], Z_i[j]]))
        x_rotated.append([rotated[0]])
        y_rotated.append([rotated[1]])
        z_rotated.append([rotated[2]])
    x.append(x_rotated)
    y.append(y_rotated)
    z.append(z_rotated)


x = np.array(x)
y = np.array(y)
z = np.array(z)
print(x.shape, x)
print(y.shape, y)
print(z.shape, z)
# Plotting
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

ax.plot_surface(x, y, z, alpha=0.5)

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

plt.show()

import numpy as np
from scipy.optimize import fsolve
import matplotlib.pyplot as plt
import ROOT


file = ROOT.TFile.Open("validation9.root")

X1 = file.ComptonHits.X1

print(X1)


"""
def eq_cono(pto: np.array, cono: np.array):
    x0, y0, z0, theta = cono
    return ((pto[2]-z0)**2 + (pto[1]-y0)**2) * np.cos(theta)**2 - (pto[0]-x0)**2 * np.sin(theta)**2


def sistema_eq_cono(pto, conos):
    return [
        eq_cono(pto, conos[0]),
        eq_cono(pto, conos[1]),
        eq_cono(pto, conos[2])
    ]

conos = np.array([
    [-241.0410, 0, 0, 0.9770754],
    [-240.9511, 0, 0, 0.2952319],
    [-240.9642, 0, 0, 0.4579676]
])

def x_cono(z, y, cono):
    x0, y0, z0, theta = cono
    return x0 - np.sqrt((z-z0)**2 + (y-y0)**2) / np.tan(theta)


# Create a 3D plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot each cone
for cono in conos:
    z = np.linspace(-1 00, 100, 1000)
    y = np.linspace(-100, 100, 1000)
    z, y = np.meshgrid(z, y)
    x = x_cono(z, y, cono)
    ax.plot_surface(x, y, z, alpha=0.5)

# Set labels and title
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_title('Intersection of Three Cones')





x_inicial = np.array([-241., 0, 0])

resultado = fsolve(sistema_eq_cono, x_inicial, args=(conos,))
print(resultado)

# Add intersection point to the plot
ax.scatter(resultado[0], resultado[1], resultado[2], c='r', label = "Intersection Point")
ax.scatter(-341, 0, 0, c='g', label = "Fuente")
# Show the plot
plt.show()
"""
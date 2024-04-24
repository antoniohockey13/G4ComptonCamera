import ROOT
import numpy as np

def extract_variables(file_name: str, tree_name: str = "ComptonHits"):
    """
    Extract the vertex coordinates from the tree.
    
    Parameters
    ----------
    file_name : str
        Name of the ROOT file.
    tree_name : str
        Name of the tree in the ROOT file. Default is "ComptonHits".
    
    Returns
    -------
    numpy.ndarray
        Numpy array containing the vertex coordinates (X1, Y1, Z1).
    """
    file = ROOT.TFile(file_name)
    tree = file.Get(tree_name)

    vertex = []
    hit = []
    theta_m = []
    E_1 = []
    E_2 = []
    for i in range(tree.GetEntries()):
        tree.GetEntry(i)
        x_1 = tree.X1
        y_1 = tree.Y1
        z_1 = tree.Z1
        vertex.append([x_1, y_1, z_1])

        x_2 = tree.X2
        y_2 = tree.Y2
        z_2 = tree.Z2
        hit.append([x_2, y_2, z_2])

        theta_m.append(tree.ComptonAngle)
        E_1.append(tree.Elost1)
        E_2.append(tree.Elost2)
    
    vertex = np.array(vertex)
    hit = np.array(hit)
    theta_m = np.array(theta_m)
    E_1 = np.array(E_1)
    E_2 = np.array(E_2)
    return vertex, hit, theta_m, E_1, E_2


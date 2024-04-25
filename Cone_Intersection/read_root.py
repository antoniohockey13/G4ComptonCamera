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
    numpy.ndarray
        Numpy array containing the hit coordinates (X2, Y2, Z2).
    numpy.ndarray
        Numpy array containing the compton angle.
    numpy.ndarray
        Numpy array containing the energy lost in the first detector.
    numpy.ndarray   
        Numpy array containing the energy lost in the second detector.
    """
    file = ROOT.TFile(file_name)
    tree = file.Get(tree_name)

    vertex = []
    hit = []
    theta_m = []
    E_1 = []
    E_2 = []
    event = []
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
        event.append(tree.Event)
    
    vertex = np.array(vertex)
    hit = np.array(hit)
    theta_m = np.array(theta_m)
    E_1 = np.array(E_1)
    E_2 = np.array(E_2)
    event = np.array(event)

    return vertex, hit, theta_m, E_1, E_2, event

def select_events(vertex, hit, theta_m, E_1, E_2, event, mass, energy_tol = 1e-10):
    delete_index = []
    for ievent in range(len(event)):
        if vertex[ievent][1]!= 0 or vertex[ievent][2]!= 0 or np.abs(70-(E_1[ievent]+E_2[ievent]))>energy_tol:
            delete_index.append(ievent)
        elif (1-511*E_1[ievent]/(E_2[ievent]*(E_1[ievent]+E_2[ievent])))<-1:
            delete_index.append(ievent)
                
    vertex = np.delete(vertex, delete_index, 0)
    hit = np.delete(hit, delete_index, 0)
    theta_m = np.delete(theta_m, delete_index, 0)
    E_1 = np.delete(E_1, delete_index, 0)
    E_2 = np.delete(E_2, delete_index, 0)
    event = np.delete(event, delete_index, 0)

    theta_E = []
    for i in range(len(E_1)):
        theta = Angle_Energy(E_1[i],E_2[i], mass)
        theta_E.append(theta)
    return vertex, hit, theta_m, E_1, E_2, event, theta_E
    

def Angle_Energy(E_1,E_2, mass):
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
    
    try:
        theta = np.arccos(1-mass*E_1/(E_2*(E_1+E_2)))  
    except RuntimeWarning:
        theta = None
    return theta
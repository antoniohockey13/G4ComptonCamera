import ROOT
import numpy as np 
# Units
mm = 1
keV = 1
# Constants
M_ELECTRON = 510.99895000*keV
SOURCE_POSITION = np.array([-682/2*mm, 0*mm, 0*mm])


def extract_variables(file_name: str, tree_name: str = "ComptonHits", read_partially: bool = False):
    """
    Extract the vertex coordinates from the tree.
    
    Parameters
    ----------
    file_name : str
        Name of the ROOT file.
    tree_name : str
        Name of the tree in the ROOT file. Default is "ComptonHits".
    read_partially : bool
        If True, read only the first 30000 entries of the tree. Default is False.
    
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
    theta_E = []
    E_1 = []
    E_2 = []
    event = []
    reco_events = 0    
    len = tree.GetEntries()
    for i in range(len):
        tree.GetEntry(i)
        x_1 = tree.X1
        y_1 = tree.Y1
        z_1 = tree.Z1
    
        x_2 = tree.X2
        y_2 = tree.Y2
        z_2 = tree.Z2
        
        ret = select_events([x_1, y_1, z_1], [x_2, y_2, z_2], tree.ComptonAngle, tree.Elost1, tree.Elost2, energy_tol=1e-10)
        if ret is None:
            continue
        reco_events += 1
        ivertex, ihit, itheta_m, iE_1, iE_2, itheta_E = ret
        vertex.append(ivertex)
        hit.append(ihit)
        theta_m.append(itheta_m)
        E_1.append(iE_1)
        E_2.append(iE_2)
        event.append(tree.Event)
        theta_E.append(itheta_E)
        if read_partially and reco_events >= 4:
            break

    
    vertex = np.array(vertex)
    hit = np.array(hit)
    theta_m = np.array(theta_m)
    E_1 = np.array(E_1)
    E_2 = np.array(E_2)
    event = np.array(event)
    theta_E = np.array(theta_E)

    return vertex, hit, theta_m, E_1, E_2, event, theta_E

def select_events(vertex, hit, theta_m, E_1, E_2, energy_tol = 1e-10):
    """
    Select events that satisfy the conditions:
    - |70-(E1+E2)|<energy_tol
    - 1-511*E1/(E2*(E1+E2))>-1
    - Hit1 is in the (x,0,0)

    Parameters
    ----------
    vertex : numpy.ndarray
        Numpy array containing the vertex coordinates (X1, Y1, Z1).
    hit : numpy.ndarray
        Numpy array containing the hit coordinates (X2, Y2, Z2).
    theta_m : numpy.ndarray
        Numpy array containing the compton angle.
    E_1 : numpy.ndarray
        Numpy array containing the energy lost in the first detector.
    E_2 : numpy.ndarray
        Numpy array containing the energy lost in the second detector.
    energy_tol : float 
        Tolerance for the energy lost in the detectors. Default is 1e-10.
    
    Returns
    -------
    numpy.ndarray
        Numpy array containing the vertex coordinates (X1, Y1, Z1).
    numpy.ndarray
        Numpy array containing the hit coordinates (X2, Y2, Z2).
    numpy.ndarray
        Numpy array containing the compton angle computed kinematically.
    numpy.ndarray
        Numpy array containing the energy lost in the first detector.
    numpy.ndarray
        Numpy array containing the energy lost in the second detector.
    numpy.ndarray
        Numpy array containing the event number.
    numpy.ndarray
        Numpy array containing the compton angle computed with energies.
    """
    if vertex[1]!= 0 or vertex[2]!= 0 or np.abs(70-(E_1+E_2))>energy_tol:
            return None
    elif (1-511*E_1/(E_2*(E_1+E_2)))<-1:
        return None                
    
    
    theta_E = Angle_Energy(E_1,E_2, M_ELECTRON)

    return vertex, hit, theta_m, E_1, E_2, theta_E
    

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
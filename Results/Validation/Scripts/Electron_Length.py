import ROOT

name = input("Enter the name of the file (with the extension .root): ")
# Open the file
file = ROOT.TFile(name)
compton = input("Do you want compton effect (1) or photoelectic effect (2): ")

if compton == "1":
     name = f"electron_length_compt_{name}"
     process = "compt"
if compton == "2":
    name = f"electron_length_phot_{name}"
    process = "phot"

print_bool = False
# Create a new TFile for storing Compton events
electron_length = ROOT.TFile(name, "RECREATE")

pre_event = -1
stored = False
electron_track = -1
transport = False
pre_detector = -1
contador = 1

# Create a new TTree for storing Compton events
electron_length_tree = ROOT.TTree("Hits", "Hits")
event = ROOT.vector('int')()
X0 = ROOT.vector('double')()
Y0 = ROOT.vector('double')()
Z0 = ROOT.vector('double')()
X1 = ROOT.vector('double')()
Y1 = ROOT.vector('double')()
Z1 = ROOT.vector('double')()
electron_track_id = ROOT.vector('int')()
electron_number = ROOT.vector('int')()

# Define branches in the new TTree
electron_length_tree.Branch("Event", event)
electron_length_tree.Branch("X0", X0)
electron_length_tree.Branch("Y0", Y0)
electron_length_tree.Branch("Z0", Z0)
electron_length_tree.Branch("X1", X1)
electron_length_tree.Branch("Y1", Y1)
electron_length_tree.Branch("Z1", Z1)
electron_length_tree.Branch("TrackID", electron_track_id)
electron_length_tree.Branch("ElectronNumber", electron_number)


for line in file.Hits:
    # If new event and stored is True, fill the information to the new TTree
    if line.Event != pre_event and stored and not transport:
        if print_bool:
            print("Electron detected and stored due to new event") 
            print("Event: ", pre_event)
            print("X: ", pre_x)
            print("Y: ", pre_y)
            print("Z: ", pre_z)
            print("TrackID: ", electron_track)
        # Fill tree
        event.push_back(pre_event)
        X0.push_back(pre_x)
        Y0.push_back(pre_y)
        Z0.push_back(pre_z)
        X1.push_back(post_x)
        Y1.push_back(post_y)
        Z1.push_back(post_z)
        electron_track_id.push_back(electron_track)
        electron_number.push_back(contador)
        stored = False
        electron_track = -1
        pre_detector = -1
        contador = 1
    # If there is new event and transport reset parameters
    elif line.Event != pre_event and stored and transport:
        stored = False
        electron_track = -1
        pre_detector = -1
        transport = False
        contador = 1

    # If photon is detected, store the information
    if line.ParticleID == 22 and line.ProcessName == process:
        pre_event = line.Event
        pre_x = line.X
        pre_y = line.Y
        pre_z = line.Z
        pre_detector = line.DetectorID
        photon_track = line.TrackID
    # If electron is detected, store the information
    elif line.ParticleID == 11 and line.Event == pre_event and line.ParentID == photon_track and line.DetectorID == pre_detector:
        # If process is transport, set transport to True to discard the value
        if line.ProcessName == "Transportation": # Transport o Transportation
             transport = True
        # If new track id it means that the previous electron has ended
        if line.TrackID != electron_track and stored and not transport:
            # Fill tree
            event.push_back(pre_event)
            X0.push_back(pre_x)
            Y0.push_back(pre_y)
            Z0.push_back(pre_z)
            X1.push_back(post_x)
            Y1.push_back(post_y)
            Z1.push_back(post_z)
            electron_track_id.push_back(electron_track)
            stored = False
            electron_track = -1
            pre_detector = -1
            transport = False
            contador += 1
            electron_number.push_back(contador)
            
            if print_bool:
                print("Electron detected and stored due to new trackID") 
                print("Event: ", line.Event)
                print("X: ", line.X)
                print("Y: ", line.Y)
                print("Z: ", line.Z)
                print("TrackID: ", line.TrackID)
           
        # If there is new electron and transport reset parameters
        elif line.Event != pre_event and stored and transport:
            stored = False
            electron_track = -1
            pre_detector = -1
            transport = False

        # If the same track id or first electron, store the information
        else:
            post_x = line.X
            post_y = line.Y
            post_z = line.Z
            stored = True
            electron_track = line.TrackID
        


electron_length_tree.Fill()
electron_length_tree.Write()
electron_length.Write()
electron_length.Close()

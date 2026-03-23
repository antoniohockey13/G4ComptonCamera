import ROOT
import numpy as np
import argparse

e_charge = 1.602176634e-19
n_sim_elec = 500000       # /run/beamOn used for output_run_anode0.root
n_sim_photons = 100000000 # /run/beamOn used in detector simulation


def photons_from_tube(current_mA, time_min):
    """
    Estimate the number of photons emitted by a tungsten tube.
    """
    Z = 74
    V = 35000
    E_gamma_keV = 25

    I = current_mA * 1e-3
    t = time_min * 60

    eff = 9e-10 * Z * V
    n_gamma_per_e = eff * (V / 1000.0) / E_gamma_keV
    n_e = I * t / e_charge
    n_gamma_total = n_gamma_per_e * n_e

    r = 2.5e-3
    d = 0.1
    geom = (np.pi * r**2) / (4 * np.pi * d**2)

    return n_gamma_total * geom


def photons_from_anode(root_file, debug=True):
    """
    Compute the total photon weight from the PhantomHits tree.
    """
    f = ROOT.TFile.Open(root_file)
    if not f or f.IsZombie():
        raise OSError(f"Could not open ROOT file: {root_file}")

    phantom = f.Get("PhantomHits")
    if not phantom:
        f.Close()
        raise ValueError("Tree 'PhantomHits' not found")

    has_weight = phantom.GetBranch("Weight") is not None
    has_pid = phantom.GetBranch("ParticleID") is not None

    count_gamma = 0
    weight_sum = 0.0

    if debug:
        print("\n=== DEBUG PHANTOMHITS ===")
        print(f"Total entries: {phantom.GetEntries()}")
        print(f"Has Weight: {has_weight}")
        print(f"Has ParticleID: {has_pid}")

    for entry in phantom:
        if has_pid and entry.ParticleID != 22:
            continue

        count_gamma += 1
        weight_sum += float(entry.Weight) if has_weight else 1.0

    if debug:
        print("\n--- PhantomHits summary ---")
        print(f"Gamma rows: {count_gamma}")
        print(f"Sum of weights: {weight_sum:.6e}")

    f.Close()
    return weight_sum


def real_electrons(current_mA, time_min):
    """
    Compute the number of real electrons from current and acquisition time.
    """
    I = current_mA * 1e-3
    t = time_min * 60
    return I * t / e_charge


def real_gamma(root_file, current_mA, time_min, debug=True):
    """
    Compute the real number of emitted photons using the anode simulation.
    """
    n_e_real = real_electrons(current_mA, time_min)
    weight_sum = photons_from_anode(root_file, debug=debug)

    photons_per_e = weight_sum / n_sim_elec
    n_gamma_real = n_e_real * photons_per_e

    if debug:
        print("\n--- Anode ROOT result ---")
        print(f"Real electrons: {n_e_real:.3e}")
        print(f"Photons per electron: {photons_per_e:.3e}")
        print(f"Real photons: {n_gamma_real:.3e}")

    return n_gamma_real


def unique_event_weight_sum_rdf(root_file, tree_name, selection, weight_branch, debug_label=None):
    """
    Fast event-level aggregation using ROOT.RDataFrame.

    For each unique Event:
    - keep one associated weight
    - if the event appears multiple times, keep the first non-zero weight
    - if all weights are zero, keep zero

    Returns:
    - n_unique_events
    - sum_event_weights
    - n_selected_rows
    """
    rdf = ROOT.RDataFrame(tree_name, root_file)
    rdf_sel = rdf.Filter(selection)

    n_rows = int(rdf_sel.Count().GetValue())

    if n_rows == 0:
        if debug_label:
            print(f"\n=== {debug_label} ===")
            print("Selected rows: 0")
            print("Unique events: 0")
            print("Sum of event weights: 0.000000e+00")
        return 0, 0.0, 0

    events = np.array(rdf_sel.AsNumpy(["Event"])["Event"], dtype=np.int64)

    if weight_branch is not None:
        weights = np.array(rdf_sel.AsNumpy([weight_branch])[weight_branch], dtype=float)
    else:
        weights = np.ones(len(events), dtype=float)

    event_to_weight = {}

    for evt, wt in zip(events, weights):
        if evt not in event_to_weight:
            event_to_weight[evt] = float(wt)
        else:
            if event_to_weight[evt] == 0.0 and wt != 0.0:
                event_to_weight[evt] = float(wt)

    n_unique = len(event_to_weight)
    w_sum = float(sum(event_to_weight.values()))

    if debug_label:
        print(f"\n=== {debug_label} ===")
        print(f"Selected rows: {n_rows}")
        print(f"Unique events: {n_unique}")
        print(f"Sum of event weights: {w_sum:.6e}")

    return n_unique, w_sum, n_rows


def photons_interacting_k3(det_root, debug=True):
    label = "UNIQUE GAMMA EVENTS IN K3" if debug else None
    n_unique, w_sum, _ = unique_event_weight_sum_rdf(
        det_root,
        "Hits",
        "DetectorID == 1 && ParticleID == 22",
        "Weight",
        label
    )
    return n_unique, w_sum


def photons_interacting_k2(det_root, debug=True):
    label = "UNIQUE GAMMA EVENTS IN K2" if debug else None
    n_unique, w_sum, _ = unique_event_weight_sum_rdf(
        det_root,
        "Hits",
        "DetectorID == 2 && ParticleID == 22",
        "Weight",
        label
    )
    return n_unique, w_sum


def photons_compton_k3(det_root, debug=True):
    """
    Count unique Compton events in K3 using ROOT TTree selection.
    """

    f = ROOT.TFile.Open(det_root)
    if not f or f.IsZombie():
        raise OSError(f"Could not open ROOT file: {det_root}")

    hits = f.Get("Hits")
    if not hits:
        f.Close()
        raise ValueError("Tree 'Hits' not found")

    selection = 'DetectorID==1 && ParticleID==22 && ProcessName=="compt"'

    # Enable only needed branches
    hits.SetBranchStatus("*", 0)
    hits.SetBranchStatus("Event", 1)
    hits.SetBranchStatus("Weight", 1)
    hits.SetBranchStatus("DetectorID", 1)
    hits.SetBranchStatus("ParticleID", 1)
    hits.SetBranchStatus("ProcessName", 1)

    hits.Draw("Event:Weight", selection, "goff")
    n = hits.GetSelectedRows()

    if n <= 0:
        if debug:
            print("\n=== UNIQUE COMPTON EVENTS IN K3 ===")
            print(f"Selected rows: {n}")
            print("Unique events: 0")
            print("Sum of event weights: 0.000000e+00")
        f.Close()
        return 0, 0.0

    ev = hits.GetV1()
    wt = hits.GetV2()

    event_to_weight = {}

    for i in range(n):
        evt = int(ev[i])
        weight = float(wt[i])

        if evt not in event_to_weight:
            event_to_weight[evt] = weight
        else:
            if event_to_weight[evt] == 0.0 and weight != 0.0:
                event_to_weight[evt] = weight

    n_unique = len(event_to_weight)
    w_sum = sum(event_to_weight.values())

    if debug:
        print("\n=== UNIQUE COMPTON EVENTS IN K3 ===")
        print(f"Selected rows: {n}")
        print(f"Unique events: {n_unique}")
        print(f"Sum of event weights: {w_sum:.6e}")

    f.Close()
    return n_unique, w_sum

def photons_crossing_k2_plane(det_root, debug=True):
    """
    Count unique events crossing the K2 plane.
    """
    f = ROOT.TFile.Open(det_root)
    if not f or f.IsZombie():
        raise OSError(f"Could not open ROOT file: {det_root}")

    tree = f.Get("K2Plane")
    if not tree:
        f.Close()
        if debug:
            print("\n=== K2Plane ===")
            print("Tree 'K2Plane' not found in this ROOT file.")
        return 0, 0.0

    f.Close()

    label = "UNIQUE EVENTS CROSSING THE K2 PLANE" if debug else None
    n_unique, w_sum, _ = unique_event_weight_sum_rdf(
        det_root,
        "K2Plane",
        "1",
        "Weight",
        label
    )
    return n_unique, w_sum


def comptonhits_unique_events(det_root, debug=True):
    """
    Count unique coincidence events in ComptonHits using Weight2.
    """
    label = "UNIQUE EVENTS IN COMPTONHITS" if debug else None
    n_unique, w_sum, _ = unique_event_weight_sum_rdf(
        det_root,
        "ComptonHits",
        "1",
        "Weight2",
        label
    )
    return n_unique, w_sum


def k3k2_probability(det_root):
    """
    Compute coincidence probability from ComptonHits.
    """
    _, w_sum = comptonhits_unique_events(det_root, debug=False)
    return w_sum / n_sim_photons


def expected_real_events(n_gamma_real, weight_sum):
    """
    Convert simulated event weights into expected real counts.
    """
    return n_gamma_real * (weight_sum / n_sim_photons)


def estimate(anode_root, det_root, current_mA, time_min):
    """
    Compute expected number of real events in K3, K2, Compton in K3,
    and K3-K2 coincidences.
    """
    n_gamma_real = real_gamma(anode_root, current_mA, time_min, debug=False)

    _, w_gamma_k3 = photons_interacting_k3(det_root, debug=True)
    _, w_gamma_k2 = photons_interacting_k2(det_root, debug=True)
    _, w_compt_k3 = photons_compton_k3(det_root, debug=True)
    _, w_k3k2 = comptonhits_unique_events(det_root, debug=True)

    n_gamma_k3_real = expected_real_events(n_gamma_real, w_gamma_k3)
    n_gamma_k2_real = expected_real_events(n_gamma_real, w_gamma_k2)
    n_compt_k3_real = expected_real_events(n_gamma_real, w_compt_k3)
    n_k3k2_real = expected_real_events(n_gamma_real, w_k3k2)

    print("\n=== EXPECTED REAL EVENTS ===")
    print(f"Real photons:                {n_gamma_real:.3e}")
    print(f"Gamma events in K3:          {n_gamma_k3_real:.3e}")
    print(f"Gamma events in K2:          {n_gamma_k2_real:.3e}")
    print(f"Compton events in K3:        {n_compt_k3_real:.3e}")
    print(f"K3-K2 coincidences:          {n_k3k2_real:.3e}")

    return n_k3k2_real


def parse_args():
    parser = argparse.ArgumentParser(description="Photon analysis for K3/K2 detectors")
    parser.add_argument("anode_root", help="ROOT file from anode simulation (PhantomHits)")
    parser.add_argument("--det-root", required=True, help="ROOT file from detector simulation")
    parser.add_argument("--current", type=float, required=True, help="Tube current in mA")
    parser.add_argument("--time", type=float, required=True, help="Acquisition time in minutes")
    return parser.parse_args()


def main():
    args = parse_args()

    n_tube = photons_from_tube(args.current, args.time)
    n_root = real_gamma(args.anode_root, args.current, args.time, debug=True)

    print("\n=== PHOTON COUNTS ===")
    print(f"Tungsten tube estimate: {n_tube:.3e}")
    print(f"From ROOT simulation:   {n_root:.3e}")

    estimate(args.anode_root, args.det_root, args.current, args.time)


if __name__ == "__main__":
    main()

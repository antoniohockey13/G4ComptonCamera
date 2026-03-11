import sys
import ROOT
import sifca_utils
from collections import defaultdict

sifca_utils.plotting.set_sifca_style()
ROOT.gROOT.SetBatch(False)

if len(sys.argv) < 2:
    print("Usage: python3 analyze_hits.py input.root")
    sys.exit(1)

input_file = sys.argv[1]

f = ROOT.TFile.Open(input_file)
if not f or f.IsZombie():
    print(f"ERROR: cannot open file {input_file}")
    sys.exit(1)

t = f.Get("Hits")
if not t:
    print("ERROR: tree 'Hits' not found")
    sys.exit(1)

def read_process_name(x):
    try:
        return x.c_str()
    except Exception:
        pass
    try:
        return bytes(x).split(b"\x00", 1)[0].decode("utf-8", errors="ignore")
    except Exception:
        pass
    return str(x)

# ------------------------------------------------------------
# Geometry / pixelization
# ------------------------------------------------------------
sensor_size_y = 21.2   # mm
sensor_size_z = 21.2   # mm
npix_y = 16
npix_z = 16
half_y = sensor_size_y / 2.0
half_z = sensor_size_z / 2.0
pitch_y = sensor_size_y / npix_y
pitch_z = sensor_size_z / npix_z

# Approximate detector boxes for X-Y event display
# Replace if you know the exact global coordinates
det1_box_xy = (-38.0, -37.7, -10.6, 10.6)   # (xmin, xmax, ymin, ymax)
det2_box_xy = (49.8, 50.1, -10.6, 10.6)

# ------------------------------------------------------------
# Histograms
# ------------------------------------------------------------
h_toa_det1 = ROOT.TH1D("h_toa_det1", "Detector 1;ToA [ps];Number of hits", 200, 0, 1000)
h_toa_det2 = ROOT.TH1D("h_toa_det2", "Detector 2;ToA [ps];Number of hits", 200, 0, 1000)

h_elost_det1 = ROOT.TH1D("h_elost_det1", "Detector 1;Deposited energy [keV];Number of hits", 100, 0, 10)
h_elost_det2 = ROOT.TH1D("h_elost_det2", "Detector 2;Deposited energy [keV];Number of hits", 100, 0, 10)

h_toa_vs_elost_det1 = ROOT.TH2D(
    "h_toa_vs_elost_det1",
    "Detector 1;Deposited energy [keV];ToA [ps]",
    100, 0, 10, 200, 0, 1000
)
h_toa_vs_elost_det2 = ROOT.TH2D(
    "h_toa_vs_elost_det2",
    "Detector 2;Deposited energy [keV];ToA [ps]",
    100, 0, 10, 200, 0, 1000
)

h_pix_det1 = ROOT.TH2D("h_pix_det1", "Detector 1 hit map;Y pixel;Z pixel", 16, 0, 16, 16, 0, 16)
h_pix_det2 = ROOT.TH2D("h_pix_det2", "Detector 2 hit map;Y pixel;Z pixel", 16, 0, 16, 16, 0, 16)

h_gamma_category = ROOT.TH1D("h_gamma_category", "Primary photon category;Category;Photons", 4, 0.5, 4.5)
h_gamma_category.GetXaxis().SetBinLabel(1, "only det1")
h_gamma_category.GetXaxis().SetBinLabel(2, "only det2")
h_gamma_category.GetXaxis().SetBinLabel(3, "both")
h_gamma_category.GetXaxis().SetBinLabel(4, "none")

h_process = ROOT.TH1D("h_process", "Gamma interaction process;Process;Entries", 4, 0.5, 4.5)
h_process.GetXaxis().SetBinLabel(1, "phot")
h_process.GetXaxis().SetBinLabel(2, "compt")
h_process.GetXaxis().SetBinLabel(3, "other")
h_process.GetXaxis().SetBinLabel(4, "mixed")

# ------------------------------------------------------------
# Event and track accumulators
# ------------------------------------------------------------
event_summary = defaultdict(lambda: {
    "counts1": 0.0,
    "counts2": 0.0,
    "toa1": 1e99,
    "toa2": 1e99,
    "has1": False,
    "has2": False,
})

gamma_tracks = defaultdict(lambda: {
    "detectors": set(),
    "processes": set(),
})

# Keep a few events for event display
selected_display_events = []
seen_display_events = set()
event_hits_for_display = defaultdict(list)

# ------------------------------------------------------------
# Main loop
# ------------------------------------------------------------
for entry in t:
    ev = int(entry.Event)
    det = int(entry.DetectorID)
    x = float(entry.X)
    y = float(entry.Y)
    z = float(entry.Z)
    elost = float(entry.ELost)
    toa = float(entry.Time)
    pid = int(entry.ParticleID)
    parent = int(entry.ParentID)
    tid = int(entry.TrackID)
    proc = read_process_name(entry.ProcessName)

    # Per-hit histograms
    if det == 1:
        h_toa_det1.Fill(toa)
        h_elost_det1.Fill(elost)
        h_toa_vs_elost_det1.Fill(elost, toa)
    elif det == 2:
        h_toa_det2.Fill(toa)
        h_elost_det2.Fill(elost)
        h_toa_vs_elost_det2.Fill(elost, toa)

    # Pixel hit maps in Y,Z
    ipy = int((y + half_y) / pitch_y)
    ipz = int((z + half_z) / pitch_z)
    if 0 <= ipy < npix_y and 0 <= ipz < npix_z:
        if det == 1:
            h_pix_det1.Fill(ipy, ipz)
        elif det == 2:
            h_pix_det2.Fill(ipy, ipz)

    # Primary gamma bookkeeping
    if pid == 22 and parent == 0:
        key = (ev, tid)
        gamma_tracks[key]["detectors"].add(det)
        if proc:
            gamma_tracks[key]["processes"].add(proc)

        if det == 1:
            event_summary[ev]["counts1"] += elost
            event_summary[ev]["has1"] = True
            if toa < event_summary[ev]["toa1"]:
                event_summary[ev]["toa1"] = toa
        elif det == 2:
            event_summary[ev]["counts2"] += elost
            event_summary[ev]["has2"] = True
            if toa < event_summary[ev]["toa2"]:
                event_summary[ev]["toa2"] = toa

    # Keep a few events for display
    if pid == 22 and parent == 0 and ev not in seen_display_events and len(selected_display_events) < 3:
        selected_display_events.append(ev)
        seen_display_events.add(ev)

    if ev in seen_display_events:
        event_hits_for_display[ev].append({
            "DetectorID": det,
            "X": x,
            "Y": y,
            "Z": z,
            "ParticleID": pid,
            "ParentID": parent,
            "TrackID": tid,
            "Time": toa,
            "ELost": elost,
        })

# ------------------------------------------------------------
# Event-level counts
# ------------------------------------------------------------
n_events_only_det1 = 0
n_events_only_det2 = 0
n_events_both = 0

for ev, info in event_summary.items():
    if info["has1"] and not info["has2"]:
        n_events_only_det1 += 1
    elif info["has2"] and not info["has1"]:
        n_events_only_det2 += 1
    elif info["has1"] and info["has2"]:
        n_events_both += 1

# ------------------------------------------------------------
# Track-level photon classification
# ------------------------------------------------------------
n_gamma_only_det1 = 0
n_gamma_only_det2 = 0
n_gamma_both = 0
n_gamma_none = 0

n_phot = 0
n_compt = 0
n_other = 0
n_mixed = 0

for key, info in gamma_tracks.items():
    dets = info["detectors"]
    procs = info["processes"]

    in1 = 1 in dets
    in2 = 2 in dets

    if in1 and not in2:
        n_gamma_only_det1 += 1
        h_gamma_category.Fill(1)
    elif in2 and not in1:
        n_gamma_only_det2 += 1
        h_gamma_category.Fill(2)
    elif in1 and in2:
        n_gamma_both += 1
        h_gamma_category.Fill(3)
    else:
        n_gamma_none += 1
        h_gamma_category.Fill(4)

    has_phot = "phot" in procs
    has_compt = "compt" in procs

    if has_phot and not has_compt:
        n_phot += 1
        h_process.Fill(1)
    elif has_compt and not has_phot:
        n_compt += 1
        h_process.Fill(2)
    elif has_phot and has_compt:
        n_mixed += 1
        h_process.Fill(4)
    else:
        n_other += 1
        h_process.Fill(3)

# ------------------------------------------------------------
# Print summary
# ------------------------------------------------------------
print("=" * 80)
print("PRIMARY PHOTON SUMMARY")
print("=" * 80)
print(f"Primary photons only in detector 1 : {n_gamma_only_det1}")
print(f"Primary photons only in detector 2 : {n_gamma_only_det2}")
print(f"Primary photons in both detectors  : {n_gamma_both}")
print(f"Primary photons in none            : {n_gamma_none}")
print()

print("=" * 80)
print("EVENT SUMMARY")
print("=" * 80)
print(f"Events with photons only in detector 1 : {n_events_only_det1}")
print(f"Events with photons only in detector 2 : {n_events_only_det2}")
print(f"Events with photons in both detectors  : {n_events_both}")
print()

print("=" * 80)
print("PRIMARY GAMMA PROCESS SUMMARY")
print("=" * 80)
print(f"photoelectric only (phot): {n_phot}")
print(f"Compton only (compt)    : {n_compt}")
print(f"mixed phot+compt        : {n_mixed}")
print(f"other                   : {n_other}")
print()

# ------------------------------------------------------------
# Show canvases interactively
# ------------------------------------------------------------
canvases = []

c1 = ROOT.TCanvas("c1", "ToA det1", 900, 700)
h_toa_det1.Draw()
c1.Update()
canvases.append(c1)
input("Press Enter to continue...")

c2 = ROOT.TCanvas("c2", "ToA det2", 900, 700)
h_toa_det2.Draw()
c2.Update()
canvases.append(c2)
input("Press Enter to continue...")

c3 = ROOT.TCanvas("c3", "ELost det1", 900, 700)
h_elost_det1.Draw()
c3.Update()
canvases.append(c3)
input("Press Enter to continue...")

c4 = ROOT.TCanvas("c4", "ELost det2", 900, 700)
h_elost_det2.Draw()
c4.Update()
canvases.append(c4)
input("Press Enter to continue...")

c5 = ROOT.TCanvas("c5", "ToA vs ELost det1", 900, 700)
h_toa_vs_elost_det1.Draw("COLZ")
c5.Update()
canvases.append(c5)
input("Press Enter to continue...")

c6 = ROOT.TCanvas("c6", "ToA vs ELost det2", 900, 700)
h_toa_vs_elost_det2.Draw("COLZ")
c6.Update()
canvases.append(c6)
input("Press Enter to continue...")

c7 = ROOT.TCanvas("c7", "Pixel map det1", 900, 700)
h_pix_det1.Draw("COLZ TEXT")
c7.Update()
canvases.append(c7)
input("Press Enter to continue...")

c8 = ROOT.TCanvas("c8", "Pixel map det2", 900, 700)
h_pix_det2.Draw("COLZ TEXT")
c8.Update()
canvases.append(c8)
input("Press Enter to continue...")

c9 = ROOT.TCanvas("c9", "Primary photon category", 900, 700)
h_gamma_category.Draw()
c9.Update()
canvases.append(c9)
input("Press Enter to continue...")

c10 = ROOT.TCanvas("c10", "Primary gamma process", 900, 700)
h_process.Draw()
c10.Update()
canvases.append(c10)
input("Press Enter to continue...")

# ------------------------------------------------------------
# Event displays
# ------------------------------------------------------------
print("\nSelected events for event display:", selected_display_events)

event_display_objects = []

for ev_id in selected_display_events:
    hits = event_hits_for_display[ev_id]
    if not hits:
        continue

    gamma_x = []
    gamma_y = []
    ele_x = []
    ele_y = []
    gamma_labels = []
    ele_labels = []

    for h in hits:
        pid = h["ParticleID"]
        if pid == 22:
            gamma_x.append(h["X"])
            gamma_y.append(h["Y"])
            gamma_labels.append((h["X"], h["Y"], h["TrackID"]))
        elif pid == 11:
            ele_x.append(h["X"])
            ele_y.append(h["Y"])
            ele_labels.append((h["X"], h["Y"], h["TrackID"]))

    xs = [h["X"] for h in hits]
    ys = [h["Y"] for h in hits]

    xmin_ev = min(xs + [det1_box_xy[0], det2_box_xy[0]]) - 1.0
    xmax_ev = max(xs + [det1_box_xy[1], det2_box_xy[1]]) + 1.0
    ymin_ev = min(ys + [det1_box_xy[2], det2_box_xy[2]]) - 1.0
    ymax_ev = max(ys + [det1_box_xy[3], det2_box_xy[3]]) + 1.0

    c_ev = ROOT.TCanvas(f"c_event_{ev_id}", f"Event {ev_id}", 900, 700)
    frame = ROOT.TH2D(
        f"frame_{ev_id}",
        f"Event {ev_id};X [mm];Y [mm]",
        10, xmin_ev, xmax_ev, 10, ymin_ev, ymax_ev
    )
    frame.SetStats(0)
    frame.Draw()

    box1 = ROOT.TBox(det1_box_xy[0], det1_box_xy[2], det1_box_xy[1], det1_box_xy[3])
    box1.SetFillStyle(0)
    box1.SetLineColor(ROOT.kGreen + 2)
    box1.SetLineWidth(2)
    box1.Draw("same")

    box2 = ROOT.TBox(det2_box_xy[0], det2_box_xy[2], det2_box_xy[1], det2_box_xy[3])
    box2.SetFillStyle(0)
    box2.SetLineColor(ROOT.kMagenta + 2)
    box2.SetLineWidth(2)
    box2.Draw("same")

    keep = [frame, box1, box2]

    g_gamma = None
    g_ele = None

    if len(gamma_x) > 0:
        g_gamma = ROOT.TGraph(len(gamma_x))
        for i, (xx, yy) in enumerate(zip(gamma_x, gamma_y)):
            g_gamma.SetPoint(i, xx, yy)
        g_gamma.SetMarkerStyle(20)
        g_gamma.SetMarkerSize(1.2)
        g_gamma.SetMarkerColor(ROOT.kRed)
        g_gamma.Draw("P SAME")
        keep.append(g_gamma)

    if len(ele_x) > 0:
        g_ele = ROOT.TGraph(len(ele_x))
        for i, (xx, yy) in enumerate(zip(ele_x, ele_y)):
            g_ele.SetPoint(i, xx, yy)
        g_ele.SetMarkerStyle(21)
        g_ele.SetMarkerSize(1.0)
        g_ele.SetMarkerColor(ROOT.kBlue)
        g_ele.Draw("P SAME")
        keep.append(g_ele)

    for xx, yy, tid in gamma_labels:
        txt = ROOT.TLatex(xx, yy, f"#gamma T{tid}")
        txt.SetTextSize(0.02)
        txt.SetTextColor(ROOT.kRed + 1)
        txt.Draw()
        keep.append(txt)

    for xx, yy, tid in ele_labels:
        txt = ROOT.TLatex(xx, yy, f"e^{{-}} T{tid}")
        txt.SetTextSize(0.02)
        txt.SetTextColor(ROOT.kBlue + 1)
        txt.Draw()
        keep.append(txt)

    leg = ROOT.TLegend(0.70, 0.72, 0.90, 0.90)
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    if g_gamma:
        leg.AddEntry(g_gamma, "gamma", "p")
    if g_ele:
        leg.AddEntry(g_ele, "electron", "p")
    leg.AddEntry(box1, "Detector 1", "l")
    leg.AddEntry(box2, "Detector 2", "l")
    leg.Draw()
    keep.append(leg)

    c_ev.Update()
    canvases.append(c_ev)
    event_display_objects.append(keep)
    input(f"Showing event {ev_id}. Press Enter to continue...")

input("All canvases shown. Press Enter to exit...")
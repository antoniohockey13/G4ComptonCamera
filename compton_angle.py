import sys
import ROOT
import sifca_utils

sifca_utils.plotting.set_sifca_style()

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

f = ROOT.TFile.Open(sys.argv[1])
if not f or f.IsZombie():
    raise RuntimeError(f"Cannot open file: {sys.argv[1]}")

cp = f.Get("ComptonHits")

c = ROOT.TCanvas("c","Compton angle")
# cp.Draw("X2.X2-X1.X1")
# c.Update()
# input("Press Enter to continue...")

# cp.Draw("(Y2.Y2-Y1.Y1)")
# c.Update()
# input("Press Enter to continue...")

# cp.Draw("(Z2.Z2-Z1.Z1)")
# c.Update()
# input("Press Enter to continue...")

ch = f.Get("ComptonHits")


ct = ROOT.TCanvas("c","Compton angle")

# Histograma TH1F
h_angle = ROOT.TH1F("h_angle", "; #theta [deg];Entries", 60, 0, 15)

ch.Draw(
"acos((X2.X2-X1.X1)/sqrt((X2.X2-X1.X1)*(X2.X2-X1.X1)+(Y2.Y2-Y1.Y1)*(Y2.Y2-Y1.Y1)+(Z2.Z2-Z1.Z1)*(Z2.Z2-Z1.Z1)))*180/TMath::Pi()>>h_angle",
"",
"hist"
)

ct.Update()
input("Press Enter to continue...")


cm = ROOT.TCanvas("c_dt", "DeltaT")

h_true = ROOT.TH1F("compton_hits_dt", ";#Delta t [ps];Entries", 200, 200, 400)

# self_base = "Elost1>0 && Elost2>0"

cp.Draw("(Time2-Time1)>>compton_hits_dt")

h_true.Draw("hist")

cm.Update()
input("Press Enter to exit...")





t = f.Get("ComptonHits")
if not t:
    raise RuntimeError("Tree 'ComptonHits' not found in file")

c = ROOT.TCanvas("c_dt", "DeltaT", 900, 700)

h_true = ROOT.TH1F("h_true_dt", ";#Delta t [ps];Entries", 200, -1000, 1000)
h_false = ROOT.TH1F("h_false_dt", ";#Delta t [ps];Entries", 200, -1000, 1000)

sel_base = "Elost1>0 && Elost2>0"

t.Draw("DeltaT>>h_true_dt",  f"{sel_base} && samePhoton==1")
t.Draw("DeltaT>>h_false_dt", f"{sel_base} && samePhoton==0", "same")

h_true.SetLineWidth(2)
h_false.SetLineWidth(2)
h_false.SetLineStyle(2)

h_true.Draw("hist")
h_false.Draw("hist same")

leg = ROOT.TLegend(0.60, 0.75, 0.88, 0.88)
leg.AddEntry(h_true, "samePhoton = 1", "l")
leg.AddEntry(h_false, "samePhoton = 0", "l")
leg.Draw()

c.Update()
c.SaveAs("deltaT_true_vs_false.png")
input("Press Enter to exit...")

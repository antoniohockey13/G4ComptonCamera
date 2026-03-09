import sys
import ROOT
import sifca_utils

sifca_utils.plotting.set_sifca_style()

f = ROOT.TFile.Open(sys.argv[1])
if not f or f.IsZombie():
    raise RuntimeError(f"Cannot open file: {sys.argv[1]}")

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
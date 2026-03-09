import sys, ROOT, sifca_utils
sifca_utils.plotting.set_sifca_style()

f = ROOT.TFile.Open(sys.argv[1])
t = f.Get("ComptonHits")

c = ROOT.TCanvas("c","Compton angles",900,700)
h_true = ROOT.TH1F("h_true",";#theta [deg];Entries",180,0,180)
h_false = ROOT.TH1F("h_false",";#theta [deg];Entries",180,0,180)

geo = "acos((X2-X1)/sqrt((X2-X1)^2+(Y2-Y1)^2+(Z2-Z1)^2))*180/TMath::Pi()"

sel_base = "Elost1>0 && Elost2>0 && (X2-X1)^2+(Y2-Y1)^2+(Z2-Z1)^2>0"

t.Draw(f"{geo}>>h_true",  f"{sel_base} && samePhoton==1")
t.Draw(f"{geo}>>h_false", f"{sel_base} && samePhoton==0")

h_true.SetLineWidth(2)
h_false.SetLineWidth(2)
h_false.SetLineStyle(2)

h_true.Draw("hist")
h_false.Draw("hist same")

leg = ROOT.TLegend(0.60,0.75,0.88,0.88)
leg.AddEntry(h_true,"samePhoton = 1","l")
leg.AddEntry(h_false,"samePhoton = 0","l")
leg.Draw()

c.Update()
c.SaveAs("compton_angles_true_vs_false.png")
input("Press Enter to exit...")
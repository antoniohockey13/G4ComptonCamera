import ROOT
import click

# Units
MeV = 1
KeV = 1e-3*MeV

omit_plot = False
ROOT.gROOT.SetBatch(omit_plot)



def write_ascii(h, name):
    name = name.split(".")[0]
    entries = h.GetEntries()
    with open(f"{name}.txt", "w") as f:
        for i in range(h.GetNbinsX()):
            bin = h.GetBinCenter(i)
            if bin < 0:
                bin = 0
            f.write(f"{bin*KeV} {h.GetBinContent(i)/entries}\n")
    print(f"ASCII file written to {name}.txt")
    f.close()

@click.command()
@click.argument('input_file')
def main(input_file):
    f = ROOT.TFile(input_file)
    histo = ROOT.TH1F("energy", "", 100, 0, 35)
    f.PhantomHits.Project("energy", "KineticEnergy", "ParticleID == 22")
    histo.Draw()
    if not omit_plot:
        input("Press Enter to continue...")
    write_ascii(histo, input_file)
        
    f.Close()

if __name__ == '__main__':
    main()
    

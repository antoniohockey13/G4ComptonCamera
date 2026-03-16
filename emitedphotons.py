import uproot
import pandas as pd
import numpy as np

# --------------------------------
# CONFIG
# --------------------------------
root_file = "output.root"   # cambia esto por tu fichero
n_generated = 10_000_000

# --------------------------------
# READ TREES
# --------------------------------
with uproot.open(root_file) as f:
    hits = f["Hits"].arrays([
        "Event", "DetectorID", "ParticleID", "TrackID",
        "ELost", "PreKineticEnergy", "PostKineticEnergy", "ProcessName"
    ], library="pd")

    compton_hits = f["ComptonHits"].arrays(["Event"], library="pd")

    compton_pairs = f["ComptonPairs"].arrays(
        ["Event", "samePhoton", "ParticleID1", "ParticleID2"],
        library="pd"
    )

# --------------------------------
# UNIQUE EVENTS WITH ANY HIT
# --------------------------------
evt_det1 = hits.loc[hits["DetectorID"] == 1, "Event"].drop_duplicates()
evt_det2 = hits.loc[hits["DetectorID"] == 2, "Event"].drop_duplicates()

# --------------------------------
# UNIQUE EVENTS WITH GAMMA HIT
# --------------------------------
evt_gamma_det1 = hits.loc[
    (hits["DetectorID"] == 1) & (hits["ParticleID"] == 22),
    "Event"
].drop_duplicates()

evt_gamma_det2 = hits.loc[
    (hits["DetectorID"] == 2) & (hits["ParticleID"] == 22),
    "Event"
].drop_duplicates()

# --------------------------------
# UNIQUE EVENTS WITH COMPTON IN DET1 / DET2
# --------------------------------
evt_compt_det1 = hits.loc[
    (hits["DetectorID"] == 1) &
    (hits["ParticleID"] == 22) &
    (hits["ProcessName"] == "compt"),
    "Event"
].drop_duplicates()

evt_compt_det2 = hits.loc[
    (hits["DetectorID"] == 2) &
    (hits["ParticleID"] == 22) &
    (hits["ProcessName"] == "compt"),
    "Event"
].drop_duplicates()

# --------------------------------
# COINCIDENCE EVENTS
# --------------------------------
evt_comptonhits = compton_hits["Event"].drop_duplicates()

evt_samephoton_pairs = compton_pairs.loc[
    compton_pairs["samePhoton"] == 1,
    "Event"
].drop_duplicates()

# --------------------------------
# EFFICIENCIES
# --------------------------------
def eff_and_err(n, N):
    eps = n / N
    err = np.sqrt(eps * (1.0 - eps) / N)
    return eps, err

results = []

quantities = {
    "events_with_hit_det1": len(evt_det1),
    "events_with_hit_det2": len(evt_det2),
    "events_with_gamma_hit_det1": len(evt_gamma_det1),
    "events_with_gamma_hit_det2": len(evt_gamma_det2),
    "events_with_compton_det1": len(evt_compt_det1),
    "events_with_compton_det2": len(evt_compt_det2),
    "events_with_comptonhits": len(evt_comptonhits),
    "events_with_samephoton_pairs": len(evt_samephoton_pairs),
}

for name, n in quantities.items():
    eps, err = eff_and_err(n, n_generated)
    results.append({
        "quantity": name,
        "n_events": n,
        "efficiency": eps,
        "eff_error": err
    })

df_results = pd.DataFrame(results)

print(df_results.to_string(index=False))

df_results.to_csv("efficiencies_summary.csv", index=False)
print("\nSaved: efficiencies_summary.csv")
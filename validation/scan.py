import ROOT as r
import pandas as pd
from concurrent.futures import ProcessPoolExecutor, ThreadPoolExecutor
import itertools
import numpy as np

rdf_sig = r.RDataFrame("Events", "/data/userdata/aaarora/output/run2/sig_MVA_test.root").Filter("Hbbscore > 0.5 & HighestWjetScore > 0.7")
rdf_bkg = r.RDataFrame("Events", "/data/userdata/aaarora/output/run2/bkg_MVA_test.root").Filter("Hbbscore > 0.5 & HighestWjetScore > 0.7")

df_sig = pd.DataFrame(rdf_sig.AsNumpy(["event", "Hbbscore", "HighestWjetScore", "VBSBDTscore", "abcdnet_score", "weight"]))
df_bkg = pd.DataFrame(rdf_bkg.AsNumpy(["event", "Hbbscore", "HighestWjetScore", "VBSBDTscore", "abcdnet_score", "weight"]))

max_workers = 16

def calculate_significance(params):
    vbs, dnn = params
    s = df_sig.query(f"VBSBDTscore > {vbs} & abcdnet_score > {dnn}").weight.sum()
    b_a = df_bkg.query(f"VBSBDTscore > {vbs} & abcdnet_score > {dnn}").weight.sum()
    b_b = df_bkg.query(f"VBSBDTscore > {vbs} & abcdnet_score < {dnn}").weight.sum()
    b_c = df_bkg.query(f"VBSBDTscore < {vbs} & abcdnet_score > {dnn}").weight.sum()
    b_d = df_bkg.query(f"VBSBDTscore < {vbs} & abcdnet_score < {dnn}").weight.sum()

    if b_a <= 0:
        return None
    if (b_b * b_c / b_d) / b_a <= 0.95 or (b_b * b_c / b_d) / b_a >= 1.05:
        return None 
    return (vbs, dnn, s, np.sqrt(2 * ((s + b_a) * np.log(1 + s / b_a) - s)))

def process_chunk(chunk):
    # Use ThreadPoolExecutor to distribute the workload within the process
    with ThreadPoolExecutor(max_workers=8) as executor:
        return list(executor.map(calculate_significance, chunk))
    
# Split the parameters into chunks
params = list(itertools.product(
    np.linspace(0.5, 1.0, 51),
    np.linspace(0.5, 1.0, 51)
))

chunk_size = len(params) // max_workers
chunks = [params[i:i + chunk_size] for i in range(0, len(params), chunk_size)]

significance = []
with ProcessPoolExecutor(max_workers=max_workers) as executor:
    results = list(executor.map(process_chunk, chunks))

significance = [sig for sublist in results for sig in sublist if sig is not None]

max_significance = sorted(significance, key=lambda x: x[3], reverse=True)

print("Max significance:", max_significance[:10])
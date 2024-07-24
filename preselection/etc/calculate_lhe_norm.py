from glob import glob
import numpy as np
import uproot
import multiprocessing as mp

def get_pdf_norm(year):
    with uproot.open(year + "/merged.root") as f:
        df = f.get("Events").arrays(["LHEPdfWeight"], library="pd")
        df = df.map(lambda x : np.sqrt(np.sum((np.array(x) - 1) ** 2)))
        print(year, 1 + df.LHEPdfWeight.sum() / df.shape[0])
        return 1 + df.LHEPdfWeight.sum() / df.shape[0]

for sample in glob("/ceph/cms/store/user/mmasciov/vbs/*"):
    print(sample)
    with mp.Pool(4) as pool:
        pool.map(get_pdf_norm, glob(sample + "/*"))
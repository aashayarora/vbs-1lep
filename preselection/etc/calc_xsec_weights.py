#%%
import uproot
from glob import glob
# %%

def get_xsec_weight(file, sample, year):
    with uproot.open(file) as f:
        n_events = f.get("Runs")["genEventSumw"].array()[0]
    xsec = {"VBSOSWWH": 0.002805,
            "VBSWWH": 0.001517,
            "VBSWZH": 0.001631,
            "VBSZZH": 0.001126}
    lumi = {"2016APV": 19.52,
            "2016": 16.81,
            "2017": 41.529,
            "2018": 59.7}
    # print in non scientific form
    return ("{:.20f}".format(xsec[sample] * lumi[year] / n_events))


# %%
for file in glob('/data/userdata/aaarora/VBS_1lep_skims/sig_1lep_4ak4_or_1ak8_2ak4_v1/*/*/NANOAODSIM/*/*/skimmed/*.root'):
    if "20UL16" in file and "APV" in file:
        year = "2016APV"
    elif "20UL16" in file:
        year = "2016"
    elif "20UL17" in file:
        year = "2017"
    elif "20UL18" in file:
        year = "2018"
    else:
        raise ValueError("Year not found")
    
    if "VBSWWH_OS" in file:
        sample = "VBSOSWWH"
    elif "VBSWWH_SS" in file:
        sample = "VBSWWH"
    elif "VBSWZH" in file:
        sample = "VBSWZH"
    elif "VBSZZH" in file:
        sample = "VBSZZH"
    else:
        raise ValueError("Sample not found")
    
    print(sample, year, get_xsec_weight(file, sample, year))
# %%

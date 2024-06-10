#%%
import uproot
from glob import glob
# %%

def get_xsec_weight(file, sample, year):
    with uproot.open(file) as f:
        n_events = sum(f.get("Runs")["genEventSumw"].array())
    xsec = {"VBSWWH_OSWW": 0.002805,
            "VBSWWH_SSWW": 0.001517,
            "VBSWZH": 0.001631,
            "VBSZZH": 0.001126}
    lumi = {"2016preVFP": 19.52,
            "2016postVFP": 16.81,
            "2017": 41.529,
            "2018": 59.7}
    # print in non scientific form
    return ("{:.10f}".format(xsec[sample] * 1000 * lumi[year] / n_events))


# %%
for file in glob('/ceph/cms/store/user/aaarora/VBS_1lep_skims/sig_1lep_4ak4_or_1ak8_2ak4_v1/*/*/NANOAODSIM/*/*/skimmed/output_merged2.root'):
    if "20UL16" in file and "APV" in file:
        year = "2016preVFP"
    elif "20UL16" in file:
        year = "2016postVFP"
    elif "20UL17" in file:
        year = "2017"
    elif "20UL18" in file:
        year = "2018"
    else:
        raise ValueError("Year not found")
    
    if "VBSWWH_OS" in file:
        sample = "VBSWWH_OSWW"
    elif "VBSWWH_SS" in file:
        sample = "VBSWWH_SSWW"
    elif "VBSWZH" in file:
        sample = "VBSWZH"
    elif "VBSZZH" in file:
        sample = "VBSZZH"
    else:
        raise ValueError("Sample not found")
    
    print(f"""
        elif(InputYear == "{sample},{year}"):
          weight_xsec = {get_xsec_weight(file, sample, year)}
          """)

# %%

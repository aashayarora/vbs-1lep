# %%
from dataclasses import dataclass

import uproot
import numpy as np
import hist as hephist

import matplotlib.pyplot as plt

import mplhep
mplhep.style.use(mplhep.style.CMS)

# %%
@dataclass
class MyHist:
    var: str
    title: str
    xlabel: str
    binning: tuple
    hist: hephist.Hist = hephist.Hist

# %%
hists = [
    MyHist("VBSJet1_pt", "VBSJet1_pt", r"Leading Jet $p_T$", (100, 0, 1000)),
    MyHist("VBSJet2_pt", "VBSJet2_pt", r"Subleading Jet $p_T$", (100, 0, 1000))
]

# %%
with uproot.open("../preselections/output/data.root") as f:
    for myhist in hists:
        h = hephist.Hist(
                hephist.axis.Regular(
                    myhist.binning[0], myhist.binning[1], myhist.binning[2], name=myhist.title, label=myhist.xlabel, underflow=False, overflow=False
                )
            )
        data = f["Events"][myhist.var].array(library="np")
        h.fill(data)
        myhist.hist = h

for myhist in hists:
    fig, ax = plt.subplots()
    mplhep.histplot(myhist.hist)
    mplhep.cms.label("Preliminary", data=True, lumi=50)
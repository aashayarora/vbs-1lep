#!/usr/bin/env python
# %%
from dataclasses import dataclass, field

import matplotlib.pyplot as plt
import mplhep as hep
hep.style.use(hep.style.CMS)

import ROOT as r

# %%
@dataclass
class MyHist:
    var: str
    xlabel: str
    binning: tuple
    hist_data: list = field(default_factory=list)
    hist_bkg: list = field(default_factory=list)
    hist_sig: list = field(default_factory=list)

# %%

hists = [
    MyHist("GMuon_pt", r"Muon $p_T$ (GeV)", (50, 0, 200)),
    MyHist("GMuon_eta", r"Muon $\eta$", (50, -2.5, 2.5)),
    MyHist("GMuon_phi", r"Muon $\phi$", (50, -3.14, 3.14)),
    MyHist("GElectron_pt", r"Electron $p_T$ (GeV)", (50, 0, 200)),
    MyHist("GElectron_eta", r"Electron $\eta$", (50, -2.5, 2.5)),
    MyHist("GElectron_phi", r"Electron $\phi$", (50, -3.14, 3.14)),
    MyHist("GHiggs_pt", r"Higgs $p_T$ (GeV)", (50, 0, 1000)),
    MyHist("GHiggs_eta", r"Higgs $\eta$", (50, -2.5, 2.5)),
    MyHist("GHiggs_phi", r"Higgs $\phi$", (50, -3.14, 3.14)),
    MyHist("GHiggs_mass", r"Higgs FatJet Softdrop Mass (GeV)", (50, 0, 200)),
    MyHist("HighestHScore", r"Higgs Score", (50, 0, 1)),
    MyHist("GW_pt", r"W $p_T$ (GeV)", (50, 0, 1000)),
    MyHist("GW_eta", r"W $\eta$", (50, -2.5, 2.5)),
    MyHist("GW_phi", r"W $\phi$", (50, -3.14, 3.14)),
    MyHist("HighestWjetScore", r"W Score", (50, 0, 1)),
    MyHist("VBSjet1pt", r"Leading VBS Jet $p_T$ (GeV)", (50, 0, 200)),
    MyHist("VBSjet1eta", r"Leading VBS Jet $\eta$", (50, -2.5, 2.5)),
    MyHist("VBSjet1phi", r"Leading VBS Jet $\phi$", (50, -3.14, 3.14)),
    MyHist("VBSjet2pt", r"Subleading VBS Jet $p_T$ (GeV)", (50, 0, 200)),
    MyHist("VBSjet2eta", r"Subleading VBS Jet $\eta$", (50, -2.5, 2.5)),
    MyHist("VBSjet2phi", r"Subleading VBS Jet $\phi$", (50, -3.14, 3.14)),
    MyHist("VBSptjj", r"$p_{T_{jj}}$ (GeV)", (50, 0, 400)),
    MyHist("VBSMjj", r"$m_{jj}$ (GeV)", (80, 0, 800)),
    MyHist("VBSdetajj", r"$\Delta\eta_{jj}$", (50, 0, 10)),
    MyHist("ST", r"$S_T$ (GeV)", (50, 1000, 2000)),
    MyHist("Hbbmass", r"Higgs $b\bar{b}$ Mass (GeV)", (50, 0, 200)),
    MyHist("HbbPt", r"Higgs $b\bar{b}$ $p_T$ (GeV)", (50, 0, 2000)),
    MyHist("Wjetmass", r"W Jet Mass (GeV)", (50, 0, 200)),
    MyHist("WjetPt", r"W Jet $p_T$ (GeV)", (50, 0, 2000)),
    MyHist("MET", r"MET (GeV)", (50, 0, 200)),
    MyHist("Mlbminloose", r"Minimum $M_{lb}$ (GeV)", (50, 0, 200)),
    MyHist("GLepton_pt", r"Lepton $p_T$ (GeV)", (50, 0, 200)),
]

# %%
data = ["/data/userdata/aaarora/output/run2/data.root"]
bkg = ["/data/userdata/aaarora/output/run2/bkg.root"]
sig = ["/data/userdata/aaarora/output/run2/sig.root"]

df_data = r.RDataFrame("Events", data)
df_bkg = r.RDataFrame("Events", bkg)
df_sig = r.RDataFrame("Events", sig)

for histogram in hists:
    histogram.hist_data = [df_data.Histo1D((histogram.var, histogram.var, *histogram.binning), histogram.var, "weight")]
    bkg_samples = ["DY", "ttbar", "ST", "WJets", "ttx", "Other"]
    for sample in bkg_samples:
        histogram.hist_bkg.append(
            df_bkg.Filter(f"sample_type == \"{sample}\"").Histo1D((histogram.var, histogram.var, *histogram.binning), histogram.var, "weight")
        )
    histogram.hist_sig = [df_sig.Histo1D((histogram.var, histogram.var, *histogram.binning), histogram.var, "weight")]

# %%
def plot(histogram):
    try:
        hist_ratio = histogram.hist_data[0].GetValue().Clone()
        hist_bkg_total = histogram.hist_bkg[0].GetValue().Clone()
        [hist_bkg_total.Add(hist.GetValue()) for hist in histogram.hist_bkg[1:]]
        hist_bkg_total.Add(histogram.hist_sig[0].GetValue())
        hist_ratio.Divide(hist_bkg_total)

        histogram.hist_sig[0].Scale(1000)

        fig, ax = plt.subplots(2, 1, gridspec_kw={"height_ratios": (4, 1)})
        hep.cms.label("Preliminary", data=True, lumi=137, year="Run2", ax=ax[0])
        hep.histplot(histogram.hist_bkg, ax=ax[0], histtype="fill", stack=True, label=["DY", "ttbar", "ST", "WJets", "ttx", "Other"])
        hep.histplot(histogram.hist_sig, label="Signal x 1000", ax=ax[0], histtype="errorbar", color="white", markersize=5)
        hep.histplot(histogram.hist_data, label="Data", ax=ax[0], histtype="errorbar", color="black")
        hep.histplot(hist_ratio, color="black", ax=ax[1], histtype="errorbar")
        
        ax[0].legend()
        ax[0].set_xlabel("")
        ax[1].set_xlabel(histogram.xlabel)
        ax[0].set_ylabel("Events")
        ax[1].set_ylabel("Data / MC")
        ax[1].set_ylim(0.8, 1.2)
        ax[1].axhline(1, color="black", linestyle="--")

        plt.savefig(f"plots/{histogram.var}.png")
        print("Saved", histogram.var)
        plt.close()
    except Exception as e:
        print(f"Error {e} in", histogram.var)
        plt.close()

# %%
for hist in hists:
    plot(hist)
#!/usr/bin/env python
# %%
from dataclasses import dataclass, field

import matplotlib.pyplot as plt
import mplhep as hep
hep.style.use(hep.style.CMS)

import ROOT as r
r.EnableImplicitMT()
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
    MyHist("GMuon_pt", r"Muon $p_T$ (GeV)", (25, 0, 200)),
    MyHist("GMuon_eta", r"Muon $\eta$", (25, -2.5, 2.5)),
    MyHist("GMuon_phi", r"Muon $\phi$", (25, -3.14, 3.14)),
    MyHist("GElectron_pt", r"Electron $p_T$ (GeV)", (25, 0, 200)),
    MyHist("GElectron_eta", r"Electron $\eta$", (25, -2.5, 2.5)),
    MyHist("GElectron_phi", r"Electron $\phi$", (25, -3.14, 3.14)),
    MyHist("GHiggs_pt", r"Higgs $p_T$ (GeV)", (25, 0, 1000)),
    MyHist("GHiggs_eta", r"Higgs $\eta$", (25, -2.5, 2.5)),
    MyHist("GHiggs_phi", r"Higgs $\phi$", (25, -3.14, 3.14)),
    MyHist("GHiggs_mass", r"Higgs FatJet Softdrop Mass (GeV)", (25, 0, 200)),
    MyHist("HighestHScore", r"Higgs Score", (25, 0, 1)),
    MyHist("GW_pt", r"W $p_T$ (GeV)", (25, 0, 1000)),
    MyHist("GW_eta", r"W $\eta$", (25, -2.5, 2.5)),
    MyHist("GW_phi", r"W $\phi$", (25, -3.14, 3.14)),
    MyHist("HighestWjetScore", r"W Score", (25, 0, 1)),
    MyHist("VBSjet1pt", r"Leading VBS Jet $p_T$ (GeV)", (25, 0, 200)),
    MyHist("VBSjet1eta", r"Leading VBS Jet $\eta$", (25, -2.5, 2.5)),
    MyHist("VBSjet1phi", r"Leading VBS Jet $\phi$", (25, -3.14, 3.14)),
    MyHist("VBSjet2pt", r"Subleading VBS Jet $p_T$ (GeV)", (25, 0, 200)),
    MyHist("VBSjet2eta", r"Subleading VBS Jet $\eta$", (25, -2.5, 2.5)),
    MyHist("VBSjet2phi", r"Subleading VBS Jet $\phi$", (25, -3.14, 3.14)),
    MyHist("VBSptjj", r"$p_{T_{jj}}$ (GeV)", (25, 0, 400)),
    MyHist("VBSMjj", r"$m_{jj}$ (GeV)", (50, 0, 800)),
    MyHist("VBSdetajj", r"$\Delta\eta_{jj}$", (25, 0, 10)),
    MyHist("ST", r"$S_T$ (GeV)", (25, 1000, 2000)),
    MyHist("Hbbmass", r"Higgs $b\bar{b}$ Mass (GeV)", (25, 0, 200)),
    MyHist("HbbPt", r"Higgs $b\bar{b}$ $p_T$ (GeV)", (25, 0, 2000)),
    MyHist("Wjetmass", r"W Jet Mass (GeV)", (25, 0, 200)),
    MyHist("WjetPt", r"W Jet $p_T$ (GeV)", (25, 0, 2000)),
    MyHist("MET", r"MET (GeV)", (25, 0, 200)),
    MyHist("Mlbminloose", r"$M_{lb}$ (GeV)", (25, 0, 600)),
    MyHist("GLepton_pt", r"Lepton $p_T$ (GeV)", (25, 0, 200)),
]

# %%
data = ["/data/userdata/aaarora/output/run2/data.root"]
bkg = ["/data/userdata/aaarora/output/run2/bkg.root"]
sig = ["/data/userdata/aaarora/output/run2/sig.root"]

df_data = r.RDataFrame("Events", data)
df_bkg = r.RDataFrame("Events", bkg)
df_sig = r.RDataFrame("Events", sig)

bkg_samples_labels = {
    "WJets": "W + Jets",
    "ttbar": r"$t\bar t$",
    "Other": "Other",
    "DY": "Drell-Yan",
    "ST": "Single Top",
    "ttx": r"$t\bar t$ + X",
}

for histogram in hists:
    histogram.hist_data = [df_data.Histo1D((histogram.var, histogram.var, *histogram.binning), histogram.var, "weight")]
    for sample in bkg_samples_labels.keys():
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

        histogram.hist_sig[0].Scale(100)

        fig, ax = plt.subplots(2, 1, gridspec_kw={"height_ratios": (4, 1)})
        hep.cms.label("Preliminary", data=True, lumi=137, year="Run2", ax=ax[0])
        hep.histplot(histogram.hist_bkg, ax=ax[0], histtype="fill", stack=True, label=bkg_samples_labels.values())
        hep.histplot(histogram.hist_sig, label="Signal x 100", ax=ax[0], histtype="step", color="blue")
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
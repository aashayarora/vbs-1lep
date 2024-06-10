#!/usr/bin/env python
# %%
from dataclasses import dataclass
import multiprocessing as mp

import matplotlib.pyplot as plt
import mplhep as hep
hep.style.use(hep.style.CMS)

import ROOT as r
from uproot.pyroot import from_pyroot
import hist
# %%
@dataclass
class MyHist:
    var: str
    xlabel: str
    binning: tuple
    hist_data: hist.Hist = hist.Hist
    hist_bkg: hist.Stack = hist.Stack
    hist_sig: hist.Hist = hist.Hist
    hist_ratio: hist.Hist = hist.Hist

# %%
hists = [
    # MyHist("GMuon_pt", r"Muon $p_T$ (GeV)", (50, 0, 200)),
    # MyHist("GMuon_eta", r"Muon $\eta$", (50, -2.5, 2.5)),
    # MyHist("GMuon_phi", r"Muon $\phi$", (50, -3.14, 3.14)),
    # MyHist("GElectron_pt", r"Electron $p_T$ (GeV)", (50, 0, 200)),
    # MyHist("GElectron_eta", r"Electron $\eta$", (50, -2.5, 2.5)),
    # MyHist("GElectron_phi", r"Electron $\phi$", (50, -3.14, 3.14)),
    # MyHist("GHiggs_pt", r"Higgs $p_T$ (GeV)", (50, 0, 1000)),
    # MyHist("GHiggs_eta", r"Higgs $\eta$", (50, -2.5, 2.5)),
    # MyHist("GHiggs_phi", r"Higgs $\phi$", (50, -3.14, 3.14)),
    # MyHist("GHiggs_mass", r"Higgs FatJet Softdrop Mass (GeV)", (50, 0, 200)),
    # MyHist("HighestHScore", r"Higgs Score", (50, 0, 1)),
    # MyHist("GW_pt", r"W $p_T$ (GeV)", (50, 0, 1000)),
    # MyHist("GW_eta", r"W $\eta$", (50, -2.5, 2.5)),
    # MyHist("GW_phi", r"W $\phi$", (50, -3.14, 3.14)),
    # MyHist("HighestWjetScore", r"W Score", (50, 0, 1)),
    # MyHist("VBSjet1pt", r"Leading VBS Jet $p_T$ (GeV)", (50, 0, 200)),
    # MyHist("VBSjet1eta", r"Leading VBS Jet $\eta$", (50, -2.5, 2.5)),
    # MyHist("VBSjet1phi", r"Leading VBS Jet $\phi$", (50, -3.14, 3.14)),
    # MyHist("VBSjet2pt", r"Subleading VBS Jet $p_T$ (GeV)", (50, 0, 200)),
    # MyHist("VBSjet2eta", r"Subleading VBS Jet $\eta$", (50, -2.5, 2.5)),
    # MyHist("VBSjet2phi", r"Subleading VBS Jet $\phi$", (50, -3.14, 3.14)),
    # MyHist("VBSptjj", r"$p_{T_{jj}}$ (GeV)", (50, 0, 400)),
    # MyHist("VBSMjj", r"$m_{jj}$ (GeV)", (80, 0, 800)),
    # MyHist("VBSdetajj", r"$\Delta\eta_{jj}$", (50, 0, 10)),
    # MyHist("ST", r"$S_T$ (GeV)", (50, 1000, 2000)),
    # MyHist("Hbbmass", r"Higgs $b\bar{b}$ Mass (GeV)", (50, 0, 200)),
    # MyHist("HbbPt", r"Higgs $b\bar{b}$ $p_T$ (GeV)", (50, 0, 2000)),
    # MyHist("Wjetmass", r"W Jet Mass (GeV)", (50, 0, 200)),
    # MyHist("WjetPt", r"W Jet $p_T$ (GeV)", (50, 0, 2000)),
    # MyHist("MET", r"MET (GeV)", (50, 0, 200)),
    # MyHist("Mlbminloose", r"Minimum $M_{lb}$ (GeV)", (50, 0, 200)),
    MyHist("GLepton_pt", r"Lepton $p_T$ (GeV)", (50, 0, 200)),
]

# %%
data = ["/data/userdata/aaarora/output/run2/data.root"]
mc = ["/data/userdata/aaarora/output/run2/bkg.root", "/data/userdata/aaarora/output/run2/sig.root"]

df_data = r.RDataFrame("Events", data)
df_bkg = r.RDataFrame("Events", mc[0])
df_sig = r.RDataFrame("Events", mc[1])

# %%
def plot(histogram):
    try:
        hist_data = df_data.Histo1D((histogram.var, histogram.var, *histogram.binning), histogram.var).GetValue()
        hist_bkgs = [
            df_bkg.Filter("sample_type == \"DY\"").Histo1D((histogram.var, histogram.var, *histogram.binning), histogram.var, "weight").GetValue(),
            df_bkg.Filter("sample_type == \"ttbar\"").Histo1D((histogram.var, histogram.var, *histogram.binning), histogram.var, "weight").GetValue(),
            df_bkg.Filter("sample_type == \"ST\"").Histo1D((histogram.var, histogram.var, *histogram.binning), histogram.var, "weight").GetValue(),
            df_bkg.Filter("sample_type == \"WJets\"").Histo1D((histogram.var, histogram.var, *histogram.binning), histogram.var, "weight").GetValue(),
            df_bkg.Filter("sample_type == \"ttx\"").Histo1D((histogram.var, histogram.var, *histogram.binning), histogram.var, "weight").GetValue(),
            df_bkg.Filter("sample_type == \"Other\"").Histo1D((histogram.var, histogram.var, *histogram.binning), histogram.var, "weight").GetValue()
        ]
        hist_sig = df_sig.Histo1D((histogram.var, histogram.var, *histogram.binning), histogram.var, "weight").GetValue()

        hist_data.Sumw2()
        [hist.Sumw2() for hist in hist_bkgs]
        hist_sig.Sumw2()

        hist_ratio = hist_data.Clone()
        hist_bkg_total = hist_bkgs[0].Clone()
        [hist_bkg_total.Add(hist) for hist in hist_bkgs[1:]]
        hist_bkg_total.Add(hist_sig)
        hist_ratio.Divide(hist_bkg_total)

        hist_sig.Scale(1000)

        histogram.hist_data = from_pyroot(hist_data).to_hist()
        histogram.hist_bkg = hist.Stack.from_dict(
            {
                r"$t\bar{t}$": from_pyroot(hist_bkgs[1]).to_hist(),
                "W+Jets": from_pyroot(hist_bkgs[3]).to_hist(),
                "DY": from_pyroot(hist_bkgs[0]).to_hist(), 
                "ST": from_pyroot(hist_bkgs[2]).to_hist(),
                r"$t\bar{t} + X$": from_pyroot(hist_bkgs[4]).to_hist(),
                "Other": from_pyroot(hist_bkgs[5]).to_hist(),
            })
        histogram.hist_ratio = from_pyroot(hist_ratio).to_hist()
        histogram.hist_sig = from_pyroot(hist_sig).to_hist()

        fig, ax = plt.subplots(2, 1, gridspec_kw={"height_ratios": (4, 1)})
        hep.cms.label("Preliminary", data=True, lumi=137, year="Run2", ax=ax[0])
        hep.histplot(histogram.hist_data, label="Data", ax=ax[0], histtype="errorbar", color="black")
        histogram.hist_bkg.plot(ax=ax[0], stack=True, histtype="fill")
        hep.histplot(histogram.hist_sig, label="Signal x 1000", ax=ax[0], histtype="step", color="black", linestyle="--")
        hep.histplot(histogram.hist_ratio, color="black", ax=ax[1], histtype="errorbar")
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
        print("Error in", histogram.var)
        plt.close()


# %%
if __name__ == "__main__":
    mp.Pool(12).map(plot, hists)
# %%

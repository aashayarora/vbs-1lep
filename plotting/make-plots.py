#!/usr/bin/env python
# %%
from dataclasses import dataclass, field

import ROOT as r
r.EnableImplicitMT()

import matplotlib.pyplot as plt
import mplhep as hep
hep.style.use(hep.style.CMS)

# %%
@dataclass
class MyHist:
    var: str
    xlabel: str
    binning: tuple
    output_file: str
    hist_data: list = field(default_factory=list)
    hist_bkg: list = field(default_factory=list)
    hist_sig: list = field(default_factory=list)

# %%
def plot(histogram, output_tag, bkg_samples_labels):
    try:
        hist_ratio = histogram.hist_data[0].GetValue().Clone()
        hist_bkg_total = histogram.hist_bkg[0].GetValue().Clone()
        [hist_bkg_total.Add(hist.GetValue()) for hist in histogram.hist_bkg[1:]]

        hist_ratio.Divide(hist_bkg_total)

        histogram.hist_sig[0].Scale(1000)

        print("Data: ", histogram.hist_data[0].GetValue().Integral())

        print("ttbar: ", histogram.hist_bkg[0].Integral())
        print("WJets: ", histogram.hist_bkg[1].Integral())
        print("Other: ", histogram.hist_bkg[2].Integral())
        print("DY: ", histogram.hist_bkg[3].Integral())
        print("ST: ", histogram.hist_bkg[4].Integral())
        print("ttx: ", histogram.hist_bkg[5].Integral())
        print("EWK: ", histogram.hist_bkg[6].Integral())

        print("Signal: ", histogram.hist_sig[0].Integral())

        fig, ax = plt.subplots(2, 1, gridspec_kw={"height_ratios": (4, 1)})
        
        hep.histplot(histogram.hist_bkg, ax=ax[0], histtype="fill", stack=True, label=bkg_samples_labels.values())
        hep.histplot(histogram.hist_sig, label="Signal x 1000", ax=ax[0], histtype="step", color="red", linewidth=2, yerr=False)
        hep.histplot(histogram.hist_data, label="Data", ax=ax[0], histtype="errorbar", color="black")
        hep.histplot(hist_ratio, color="black", ax=ax[1], histtype="errorbar")
        
        hep.cms.label("Preliminary", data=True, lumi=137, year="Run2", ax=ax[0])
        ax[0].legend()
        ax[0].set_xlabel("")
        ax[1].set_xlabel(histogram.xlabel)
        ax[0].set_ylabel("Events")
        ax[1].set_ylabel("Data / MC")
        ax[1].set_ylim(0.8, 1.2)
        ax[1].axhline(1, color="black", linestyle="--")

        plt.savefig(f"plots/{output_tag}/plot_{output_tag}_{histogram.output_file}.png")
        print("Saved", histogram.var)
        plt.close()
    except Exception as e:
        print(f"Error {e} in", histogram.var)
        plt.close()

# %%
def main(input_tag, output_tag):
    hists = [
        MyHist("GMuon_pt", r"Muon $p_T$ (GeV)", (40, 40, 200), "muonpt"),
        MyHist("GMuon_eta", r"Muon $\eta$", (40, -2.5, 2.5), "muoneta"),
        MyHist("GMuon_phi", r"Muon $\phi$", (40, -3.14, 3.14), "muonphi"),
        MyHist("GElectron_pt", r"Electron $p_T$ (GeV)", (40, 40, 200), "elecpt"),
        MyHist("GElectron_eta", r"Electron $\eta$", (40, -2.5, 2.5), "eleceta"),
        MyHist("GElectron_phi", r"Electron $\phi$", (40, -3.14, 3.14), "elecphi"),
        MyHist("GHiggs_pt", r"Higgs $p_T$ (GeV)", (40, 250, 1000), "fatjetHpt"),
        MyHist("GHiggs_mass", r"Higgs FatJet Mass (GeV)", (40, 0, 200), "fatjetHmass"),
        MyHist("Hsoftdrop", r"Higgs FatJet Softdrop Mass (GeV)", (40, 0, 200), "fatjetHmsoftdrop"),
        MyHist("HighestHScore", r"Higgs Score", (40, 0, 1), "fatjetHscore"),
        MyHist("Htau21", r"Higgs $\tau_{21}$", (40, 0, 1), "fatjetHtau21"),
        MyHist("GW_pt", r"W $p_T$ (GeV)", (40, 250, 1000), "fatjetWpt"),
        MyHist("GW_mass", r"W FatJet Mass (GeV)", (40, 0, 200), "fatjetWmass"),
        MyHist("Wsoftdrop", r"W FatJet Softdrop Mass (GeV)", (40, 0, 200), "fatjetWmsoftdrop"),
        MyHist("HighestWjetScore", r"W Score", (40, 0, 1), "fatjetWscore"),
        MyHist("Wtau21", r"W $\tau_{21}$", (40, 0, 1), "fatjetWtau21"),
        MyHist("VBSjet1pt", r"Leading VBS Jet $p_T$ (GeV)", (40, 40, 200), "vbs1pt"),
        MyHist("VBSjet1eta", r"Leading VBS Jet $\eta$", (40, -2.5, 2.5), "vbs1eta"),
        MyHist("VBSjet1phi", r"Leading VBS Jet $\phi$", (40, -3.14, 3.14), "vbs1phi"),
        MyHist("VBSjet2pt", r"Subleading VBS Jet $p_T$ (GeV)", (40, 40, 200), "vbs2pt"),
        MyHist("VBSjet2eta", r"Subleading VBS Jet $\eta$", (40, -2.5, 2.5), "vbs2eta"),
        MyHist("VBSjet2phi", r"Subleading VBS Jet $\phi$", (40, -3.14, 3.14), "vbs2phi"),
        MyHist("MET", r"Missing $E_T$ (GeV)", (50, 0, 500), "MET"),
        MyHist("MT", r"$M_T$ (GeV)", (50, 0.001, 500), "MT"),
        MyHist("ST", r"$S_T$ (GeV)", (50, 0, 2000), "ST"),
        MyHist("Mlbminloose", r"$M_{lb}$ (GeV)", (40, 0, 600), "Mlb"),
        MyHist("bjet1pt", r"Leading b-tagging WP AK4 jet $p_T$ (GeV)", (40, 20, 200), "bjet1pt"),
        MyHist("bjet1eta", r"Leading b-tagging WP AK4 jet $\eta$", (40, -2.5, 2.5), "bjet1eta"),
        MyHist("bjet1phi", r"Leading b-tagging WP AK4 jet $\phi$", (40, -3.14, 3.14), "bjet1phi"),
        MyHist("bjet1score", r"Leading b-tagging WP AK4 jet score", (40, 0, 1), "bjet1score"),
        MyHist("bjet2pt", r"Subleading b-tagging WP AK4 jet $p_T$ (GeV)", (40, 20, 200), "bjet2pt"),
        MyHist("bjet2eta", r"Subleading b-tagging WP AK4 jet $\eta$", (40, -2.5, 2.5), "bjet2eta"),
        MyHist("bjet2phi", r"Subleading b-tagging WP AK4 jet $\phi$", (40, -3.14, 3.14), "bjet2phi"),
        MyHist("bjet2score", r"Subleading b-tagging WP AK4 jet score", (40, 0, 1), "bjet2score"),
    ]


    data = [f"/data/userdata/aaarora/output/run2/data.root"]
    bkg = [f"/data/userdata/aaarora/output/run2/bkg.root"]
    sig = [f"/data/userdata/aaarora/output/run2/sig.root"]

    df_data = r.RDataFrame("Events", data)
    df_bkg = r.RDataFrame("Events", bkg)
    df_sig = r.RDataFrame("Events", sig)

    if input_tag:
        df_data = df_data.Filter(input_tag)
        df_bkg = df_bkg.Filter(input_tag)
        df_sig = df_sig.Filter(input_tag)

    bkg_samples_labels = {
        "ttbar": r"$t\bar t$",
        "WJets": "W + Jets",
        "Other": "Other",
        "DY": "Drell-Yan",
        "ST": "Single Top",
        "ttx": r"$t\bar t$ + X",
        "EWK": "EWK",
    }

    for histogram in hists:
        histogram.hist_data = [df_data.Histo1D((histogram.var, histogram.var, *histogram.binning), histogram.var, "weight")]
        for sample in bkg_samples_labels.keys():
            histogram.hist_bkg.append(
                df_bkg.Filter(f"sample_type == \"{sample}\"").Histo1D((histogram.var, histogram.var, *histogram.binning), histogram.var, "weight")
            )
        histogram.hist_sig = [df_sig.Histo1D((histogram.var, histogram.var, *histogram.binning), histogram.var, "weight")]

    for hist in hists:
        plot(hist, output_tag, bkg_samples_labels)

if __name__ == "__main__":
    main(input_tag="passCut4", output_tag="fatjettag")
    main(input_tag="passCut5", output_tag="tightak4tag")
    main(input_tag="passCut6", output_tag="VBStag")
    main(input_tag="passCut7", output_tag="Vqq")
    
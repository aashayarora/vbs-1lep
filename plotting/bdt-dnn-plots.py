#!/usr/bin/env python
# %%
from dataclasses import dataclass, field

import ROOT as r

import matplotlib.pyplot as plt
import mplhep as hep
hep.style.use(hep.style.CMS)

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
def plot(histogram, scale=10):
    try:
        hist_ratio = histogram.hist_data[0].GetValue().Clone()
        hist_bkg_total = histogram.hist_bkg[0].GetValue().Clone()
        [hist_bkg_total.Add(hist.GetValue()) for hist in histogram.hist_bkg[1:]]
        hist_bkg_total.Add(histogram.hist_sig[0].GetValue())
        hist_ratio.Divide(hist_bkg_total)

        histogram.hist_sig[0].Scale(scale)

        fig, ax = plt.subplots(2, 1, gridspec_kw={"height_ratios": (4, 1)})
        hep.cms.label("Preliminary", data=True, lumi=137, year="Run2", ax=ax[0])
        hep.histplot(histogram.hist_bkg, ax=ax[0], histtype="fill", stack=True, label=bkg_samples_labels.values())
        if scale != 1:
            hep.histplot(histogram.hist_sig, label=f"Signal x {scale}", ax=ax[0], histtype="step", color="red")
        else:
            hep.histplot(histogram.hist_sig, label="Signal", ax=ax[0], histtype="step", color="red")
        hep.histplot(histogram.hist_data, label="Data", ax=ax[0], histtype="errorbar", color="black")
        hep.histplot(hist_ratio, color="black", ax=ax[1], histtype="errorbar")
        
        ax[0].legend()
        ax[0].set_xlabel("")
        ax[1].set_xlabel(histogram.xlabel)
        ax[0].set_ylabel("Events")
        ax[1].set_ylabel("Data / MC")
        ax[1].set_ylim(0.8, 1.2)
        ax[1].axhline(1, color="black", linestyle="--")

        plt.savefig(f"plots/Plot_{histogram.var}.png")
        print("Saved", histogram.var)
        plt.close()
    except Exception as e:
        print(f"Error {e} in", histogram.var)
        plt.close()

# %%
if __name__ == "__main__":
    hists = [
        MyHist("NNL_FVBSBDT", r"DNN Score (Fails BDT)", (10, 0, 1)),
        MyHist("NNL_PVBSBDT", r"DNN Score (Passes BDT)", (10, 0, 1)),
        MyHist("VBSBDT_PNNL", r"BDT Score (Passes DNN)", (10, 0, 1)),
        MyHist("VBSBDT_FNNL", r"BDT Score (Fails DNN)", (10, 0, 1)),
    ]

    r.EnableImplicitMT()

    data = [f"/data/userdata/aaarora/output/run2/ABCDNet_simpleDisco_VBSVVH1lep_30/output/data_MVA_abcdnet.root"]
    bkg = [f"/data/userdata/aaarora/output/run2/ABCDNet_simpleDisco_VBSVVH1lep_30/output/bkg_MVA_abcdnet.root"]
    sig = [f"/data/userdata/aaarora/output/run2/ABCDNet_simpleDisco_VBSVVH1lep_30/output/sig_MVA_abcdnet.root"]

    df_data = r.RDataFrame("Events", data)
    df_bkg = r.RDataFrame("Events", bkg)
    df_sig = r.RDataFrame("Events", sig)

    df_data = df_data.Define("fail_bdt", "VBSBDTscore < 0.56").Define("pass_bdt", "VBSBDTscore >= 0.56").Define("fail_dnn", "abcdnet_score < 0.92").Define("pass_dnn", "abcdnet_score >= 0.92")
    df_bkg = df_bkg.Define("fail_bdt", "VBSBDTscore < 0.56").Define("pass_bdt", "VBSBDTscore >= 0.56").Define("fail_dnn", "abcdnet_score < 0.92").Define("pass_dnn", "abcdnet_score >= 0.92")
    df_sig = df_sig.Define("fail_bdt", "VBSBDTscore < 0.56").Define("pass_bdt", "VBSBDTscore >= 0.56").Define("fail_dnn", "abcdnet_score < 0.92").Define("pass_dnn", "abcdnet_score >= 0.92")

    bkg_samples_labels = {
        "ttbar": r"$t\bar t$",
        "WJets": "W + Jets",
        "ST": "Single Top",
        "Other": "Other",
        "DY": "Drell-Yan",
        "ttx": r"$t\bar t$ + X",
    }

    sample_dict = {"ttbar": 0, "WJets": 1, "ttx": 2, "ST": 3, "DY": 4, "EWK": 5, "Other": 6}

    for histogram in hists:
        if histogram.var == "NNL_FVBSBDT":
            histogram.hist_data = [df_data.Filter("fail_bdt").Histo1D((histogram.var, histogram.var, *histogram.binning), "abcdnet_score", "weight")]
            for sample in bkg_samples_labels.keys():
                histogram.hist_bkg.append(
                    df_bkg.Filter(f"sample_type == {sample_dict.get(sample)}").Filter("fail_bdt").Histo1D((histogram.var, histogram.var, *histogram.binning), "abcdnet_score", "weight")
                )
            histogram.hist_sig = [df_sig.Filter("fail_bdt").Histo1D((histogram.var, histogram.var, *histogram.binning), "abcdnet_score", "weight")]
        
        elif histogram.var == "NNL_PVBSBDT":
            histogram.hist_data = [df_data.Filter("pass_bdt").Filter("abcdnet_score < 0.9").Histo1D((histogram.var, histogram.var, *histogram.binning), "abcdnet_score", "weight")]
            for sample in bkg_samples_labels.keys():
                histogram.hist_bkg.append(
                    df_bkg.Filter(f"sample_type == {sample_dict.get(sample)}").Filter("pass_bdt").Histo1D((histogram.var, histogram.var, *histogram.binning), "abcdnet_score", "weight")
                )
            histogram.hist_sig = [df_sig.Filter("pass_bdt").Histo1D((histogram.var, histogram.var, *histogram.binning), "abcdnet_score", "weight")]
        
        elif histogram.var == "VBSBDT_FNNL":
            histogram.hist_data = [df_data.Filter("fail_dnn").Histo1D((histogram.var, histogram.var, *histogram.binning), "VBSBDTscore", "weight")]
            for sample in bkg_samples_labels.keys():
                histogram.hist_bkg.append(
                    df_bkg.Filter(f"sample_type == {sample_dict.get(sample)}").Filter("fail_dnn").Histo1D((histogram.var, histogram.var, *histogram.binning), "VBSBDTscore", "weight")
                )
            histogram.hist_sig = [df_sig.Filter("fail_dnn").Histo1D((histogram.var, histogram.var, *histogram.binning), "VBSBDTscore", "weight")]
        
        elif histogram.var == "VBSBDT_PNNL":
            histogram.hist_data = [df_data.Filter("pass_dnn").Filter("VBSBDTscore < 0.5").Histo1D((histogram.var, histogram.var, *histogram.binning), "VBSBDTscore", "weight")]
            for sample in bkg_samples_labels.keys():
                histogram.hist_bkg.append(
                    df_bkg.Filter(f"sample_type == {sample_dict.get(sample)}").Filter("pass_dnn").Histo1D((histogram.var, histogram.var, *histogram.binning), "VBSBDTscore", "weight")
                )
            histogram.hist_sig = [df_sig.Filter("pass_dnn").Histo1D((histogram.var, histogram.var, *histogram.binning), "VBSBDTscore", "weight")]
            
    plot(hists[0], scale=100)
    plot(hists[1], scale=100)
    plot(hists[2], scale=100)
    plot(hists[3], scale=100)
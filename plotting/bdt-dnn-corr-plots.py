#!/usr/bin/env python

import hist
import numpy as np

import matplotlib.pyplot as plt
import mplhep as hep
hep.style.use(hep.style.CMS)

import ROOT as r

def plot(category):
    df = r.RDataFrame("Events", f"/data/userdata/aaarora/output/run2/plotting/{category}_MVA_abcdnet.root")
    cols = df.AsNumpy(["abcdnet_score", "VBSBDTscore", "weight"])

    h = hist.Hist.new.Reg(10, 0, 1, name="abcdnet_score").Reg(10, 0, 1, name="VBSBDTscore").Double()
    h.fill(cols["abcdnet_score"], cols["VBSBDTscore"], weight=cols["weight"])

    fig, ax = plt.subplots()
    hep.hist2dplot(h, cbar=True, ax=ax)
    hep.histplot(h.profile("VBSBDTscore"), ax=ax, histtype="errorbar", color="red", label="VBS BDT Score Profile")
    ax.errorbar(h.profile("abcdnet_score").to_numpy()[0], h.profile("abcdnet_score").to_numpy()[1][:-1] + 0.05, 
                xerr=np.sqrt(h.profile("abcdnet_score").variances()), 
                color="cyan", 
                marker="o", 
                linewidth=0, 
                elinewidth=1, 
                label="ABCDNet Score Profile")

    hep.cms.label(data=True, label="Preliminary", lumi=138, year="Run2", ax=ax)

    ax.set_ylabel("VBS BDT Score")
    ax.set_xlabel("ABCDNet Score")

    if category == "data":
        ax.legend(loc="lower right")

    elif category == "bkg":
        ax.legend(loc="lower right")

    elif category == "sig":
        ax.legend(loc="lower right")

    plt.savefig(f"plots/Plot_corr_{category}.png")
    print("Saved", f"plots/Plot_corr_{category}.png")


if __name__ == "__main__":
    r.EnableImplicitMT()
    plot("bkg")
    plot("sig")
    plot("data")
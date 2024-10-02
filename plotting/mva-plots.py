#!/usr/bin/env python
import ROOT as r
r.EnableImplicitMT()

import matplotlib.pyplot as plt
import mplhep as hep
hep.style.use(hep.style.CMS)

import hist
import pandas as pd


def makePlot(var, df_bkg, df_sig, xlabel, binning):
    h_bkg = hist.Hist(hist.axis.Regular(*binning))
    h_sig = hist.Hist(hist.axis.Regular(*binning))

    h_bkg.fill(df_bkg[var])
    h_sig.fill(df_sig[var])

    fig, ax = plt.subplots()
    hep.histplot(h_bkg, ax=ax, label="Background", density=True, histtype="step", linewidth=1.5)
    hep.histplot(h_sig, ax=ax, label="Signal", density=True, histtype="step", linewidth=1.5)

    hep.cms.label("Preliminary", year="Run2", ax=ax)
    ax.legend()
    ax.set_xlabel(xlabel)
    ax.set_ylabel("Density")

    plt.savefig(f"plots/bdt_vars/plot_{var}.png")


if __name__ == "__main__":
    bkg = [f"/data/userdata/aaarora/output/run2/bkg.root"]
    sig = [f"/data/userdata/aaarora/output/run2/sig.root"]

    rdf_bkg = r.RDataFrame("Events", bkg).Filter("passCut6")
    rdf_sig = r.RDataFrame("Events", sig).Filter("passCut6")

    vars = ["VBSjet1pt", 
            "VBSjet1eta", 
            "VBSjet2pt", 
            "VBSjet2eta", 
            "VBSMjj", 
            "VBSdetajj",
            "Hbbmass",
            "HbbPt",
            "Wjetmass",
            "WjetPt", 
            "leptonpt",
            "Mlbminloose",
            "MET"]
    
    xlabels = [r"Leading VBS jet $p_{T}$ [GeV]", 
               r"Leading VBS jet $\eta$", 
               r"Subleading VBS jet $p_{T}$ [GeV]", 
               r"Subleading VBS jet $\eta$", 
               r"$M_{jj}$ [GeV]", 
               r"$\Delta\eta_{jj}$",
               r"$M_{Hbb}$ [GeV]",
               r"$p_{T}^{Hbb}$ [GeV]",
               r"$M_{Wjet}$ [GeV]",
               r"$p_{T}^{Wjet}$ [GeV]",
               r"Lepton $p_{T}$ [GeV]",
               r"$M_{lb}$ [GeV]",
               r"MET [GeV]"]
    
    binnings = [(50, 30, 900), # vbsjet1pt
                 (50, -5, 5), # vbsjet1eta
                 (50, 30, 900), # vbsjet2pt
                 (50, -5, 5), # vbsjet2eta
                 (50, 0, 5000), # vbsmjj
                 (50, 0, 10), # vbsdetajj
                 (50, 0, 300), # hbbmass
                 (50, 250, 2000), # hbbpt
                 (50, 0, 300), # wjetmass
                 (50, 250, 2000), # wjetpt
                 (50, 40, 1000), # leptonpt
                 (50, 0, 1000), # mlbminloose
                 (50, 0, 1000)] # met

    df_bkg = pd.DataFrame(rdf_bkg.AsNumpy(vars))
    df_sig = pd.DataFrame(rdf_sig.AsNumpy(vars))

    for var, xlabel, binning in zip(vars, xlabels, binnings):
        makePlot(var, df_bkg, df_sig, xlabel, binning)
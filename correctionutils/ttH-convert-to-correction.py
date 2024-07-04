# %%
import uproot
import correctionlib.schemav2 as cs
import correctionlib.convert

# %%
def write_correction(paths, corrname, outname):
    hists = []
    for path in paths:
        with uproot.open(path) as h:
            hists.append(h.to_hist())

    tth = [correctionlib.convert.from_histogram(hist) for hist in hists]
    
    for corr_ in tth:
        corr_.data.flow = "clamp"

    corr = cs.Correction(
        name=outname,
        description=corrname,
        version=1,
        inputs=[
            cs.Variable(
                name="year",
                type="string",
                description="year",
            ),
            cs.Variable(
                name="xaxis",
                type="real",
                description="eta",
            ),
            cs.Variable(
                name="yaxis",
                type="real",
                description="pt",
            ),
        ],
        output=cs.Variable(name="weight", type="real", description="weight"),
        data=cs.Category(
            nodetype="category",
            input="year",
            content=[
                cs.CategoryItem(key="2016preVFP", value=tth[0].data),
                cs.CategoryItem(key="2016postVFP", value=tth[1].data),
                cs.CategoryItem(key="2017", value=tth[2].data),
                cs.CategoryItem(key="2018", value=tth[3].data),
            ],
        )
    )

    cset2 = cs.CorrectionSet(
        schema_version=2,
        description=corrname,
        corrections=[
            corr,
        ],
    )
    
    with open(f"{outname}.json", "w") as fout:
        fout.write(cset2.model_dump_json(indent=4, exclude_unset=True))

# %%
if __name__ == "__main__":
    # muon
    # corr_hists = ["../junk/root_sf/muon/egammaEffi2016APV_EGM2D.root:EGamma_SF2D", "../junk/root_sf/muon/egammaEffi2016_EGM2D.root:EGamma_SF2D", "../junk/root_sf/muon/egammaEffi2017_EGM2D.root:EGamma_SF2D", "../junk/root_sf/muon/egammaEffi2018_EGM2D.root:EGamma_SF2D"]
    # write_correction(corr_hists, "ttH SFs", "ttH_MuonID_SF")

    # corr_hists = ["../junk/root_sf/muon/egammaEffi2016APV_iso_EGM2D.root:EGamma_SF2D", "../junk/root_sf/muon/egammaEffi2016_iso_EGM2D.root:EGamma_SF2D", "../junk/root_sf/muon/egammaEffi2017_iso_EGM2D.root:EGamma_SF2D", "../junk/root_sf/muon/egammaEffi2018_iso_EGM2D.root:EGamma_SF2D"]
    # write_correction(corr_hists, "ttH SFs", "ttH_MuonISO_SF")

    # # electron
    # corr_hists = ["../junk/root_sf/elec/egammaEffi2016APV_2lss_EGM2D.root:EGamma_SF2D", "../junk/root_sf/elec/egammaEffi2016_2lss_EGM2D.root:EGamma_SF2D", "../junk/root_sf/elec/egammaEffi2017_2lss_EGM2D.root:EGamma_SF2D", "../junk/root_sf/elec/egammaEffi2018_2lss_EGM2D.root:EGamma_SF2D"]
    # write_correction(corr_hists, "ttH SFs", "ttH_ElectronID_SF")

    # corr_hists = ["../junk/root_sf/elec/egammaEffi2016APV_iso_EGM2D.root:EGamma_SF2D", "../junk/root_sf/elec/egammaEffi2016_iso_EGM2D.root:EGamma_SF2D", "../junk/root_sf/elec/egammaEffi2017_iso_EGM2D.root:EGamma_SF2D", "../junk/root_sf/elec/egammaEffi2018_iso_EGM2D.root:EGamma_SF2D"]
    # write_correction(corr_hists, "ttH SFs", "ttH_ElectronISO_SF")

    # corr_hists = ["../junk/root_sf/elec/electron_hlt_sfs_2016.root:EGamma_SF2D", "../junk/root_sf/elec/electron_hlt_sfs_2016.root:EGamma_SF2D", "../junk/root_sf/elec/electron_hlt_sfs_2017.root:EGamma_SF2D", "../junk/root_sf/elec/electron_hlt_sfs_2018.root:EGamma_SF2D"]
    # write_correction(corr_hists, "Electron Trigger SFs", "trigger")

    corr_hists = ["hist_beff.root:hist_btight", "hist_beff.root:hist_bloose", "hist_beff.root:hist_bmedium", "hist_beff.root:hist_ctight", "hist_beff.root:hist_cloose", "hist_beff.root:hist_cmedium", "hist_beff.root:hist_ltight", "hist_beff.root:hist_lloose", "hist_beff.root:hist_lmedium"]
    write_correction(corr_hists, "Btag Efficiency", "btag")
# %%

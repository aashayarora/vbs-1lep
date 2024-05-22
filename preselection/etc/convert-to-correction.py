# %%
import uproot
import correctionlib.schemav2 as cs
import correctionlib.convert

# %%
def json_from_root(path, outname):
    with uproot.open(path) as h:
        hist = h.to_hist()

    hist.name = outname
    hist.label= "ttH Scale Factors"
    tth = correctionlib.convert.from_histogram(hist)
    tth.description = "ttH Scale Factors"
    tth.data.flow = "clamp"

    corr = cs.Correction(
        name=outname,
        description="ttH Scale Factors",
        version=1,
        inputs=[
            cs.Variable(
                name="sf",
                type="string",
                description="sf",
            ),
            cs.Variable(
                name="xaxis",
                type="real",
                description="abseta",
            ),
            cs.Variable(
                name="yaxis",
                type="real",
                description="pt",
            ),
        ],
        output=cs.Variable(
            name="weight",
            type="real",
            description="weight",
        ),
        data=cs.Category(
            nodetype="category",
            input="sf",
            content=[
                cs.CategoryItem(
                    key="ttHSF",
                    value=tth.data,
                ),
            ],
        )
    )

    cset2 = cs.CorrectionSet(
        schema_version=2,
        description="ttH Scale Factors",
        corrections=[
            corr,
        ],
    )
    
    with open(f"{outname}.json", "w") as fout:
        fout.write(cset2.model_dump_json(indent=4, exclude_unset=True))

# %%
if __name__ == "__main__":
    # muon
    json_from_root("junk/root_sf/muon/egammaEffi2016APV_EGM2D.root:EGamma_SF2D", "2016preVFP_ttH_MuonID_SF")
    json_from_root("junk/root_sf/muon/egammaEffi2016APV_iso_EGM2D.root:EGamma_SF2D", "2016preVFP_ttH_MuonISO_SF")

    json_from_root("junk/root_sf/muon/egammaEffi2016_EGM2D.root:EGamma_SF2D", "2016postVFP_ttH_MuonID_SF")
    json_from_root("junk/root_sf/muon/egammaEffi2016_iso_EGM2D.root:EGamma_SF2D", "2016postVFP_ttH_MuonISO_SF")

    json_from_root("junk/root_sf/muon/egammaEffi2017_EGM2D.root:EGamma_SF2D", "2017_ttH_MuonID_SF")
    json_from_root("junk/root_sf/muon/egammaEffi2017_iso_EGM2D.root:EGamma_SF2D", "2017_ttH_MuonISO_SF")

    json_from_root("junk/root_sf/muon/egammaEffi2018_EGM2D.root:EGamma_SF2D", "2018_ttH_MuonID_SF")
    json_from_root("junk/root_sf/muon/egammaEffi2018_iso_EGM2D.root:EGamma_SF2D", "2018_ttH_MuonISO_SF")

    # electron
    json_from_root("junk/root_sf/elec/egammaEffi2016APV_2lss_EGM2D.root:EGamma_SF2D", "2016preVFP_ttH_ElectronID_SF")
    json_from_root("junk/root_sf/elec/egammaEffi2016APV_iso_EGM2D.root:EGamma_SF2D", "2016preVFP_ttH_ElectronISO_SF")

    json_from_root("junk/root_sf/elec/egammaEffi2016_2lss_EGM2D.root:EGamma_SF2D", "2016postVFP_ttH_ElectronID_SF")
    json_from_root("junk/root_sf/elec/egammaEffi2016_iso_EGM2D.root:EGamma_SF2D", "2016postVFP_ttH_ElectronISO_SF")

    json_from_root("junk/root_sf/elec/egammaEffi2017_2lss_EGM2D.root:EGamma_SF2D", "2017_ttH_ElectronID_SF")
    json_from_root("junk/root_sf/elec/egammaEffi2017_iso_EGM2D.root:EGamma_SF2D", "2017_ttH_ElectronISO_SF")

    json_from_root("junk/root_sf/elec/egammaEffi2018_2lss_EGM2D.root:EGamma_SF2D", "2018_ttH_ElectronID_SF")
    json_from_root("junk/root_sf/elec/egammaEffi2018_iso_EGM2D.root:EGamma_SF2D", "2018_ttH_ElectronISO_SF")
# %%
import correctionlib

ceval = correctionlib.CorrectionSet.from_file("2016preVFP_ttH_MuonID_SF.json")
ceval["2016preVFP_ttH_MuonID_SF"].evaluate("2016preVFP_ttH_MuonID_SF", 0.9, 20.0)
# %%

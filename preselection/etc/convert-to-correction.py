from correctionlib import convert
import json

def gen_tth_sfs(path, outname):
    corr = convert.from_uproot_THx(path)

    for idx, c in enumerate(corr):
        if c[0] == 'data':
            corr_ = c[1]

    corr_.__dict__["inputs"] = ["abs(eta)", "pt"]
    corr_.__dict__["flow"] = 1
    output_json = {}

    output_json["schema_version"] = 2
    output_json["description"] = "TTH Electron SFs"
    output_json["corrections"] = {}

    output_json["corrections"]["name"] = "EGamma_SF2D"
    output_json["corrections"]["description"] = "Electron SFs"
    output_json["corrections"]["version"] = 2
    output_json["corrections"]["inputs"] = [
        {"name": "abs(eta)", "type": "real", "description": "Electron eta"},
        {"name": "pt", "type": "real", "description": "Electron pT"},
    ]
    output_json["corrections"]["output"] = {"name": "weight", "type": "real", "description": "Electron SF"}


    output_json["corrections"]["data"] = corr_.__dict__

    with open(f"{outname}.json", "w") as f:
        json.dump(output_json, f, indent=4)

if __name__ == "__main__":
    gen_tth_sfs("/home/users/aaarora/phys/analysis/YifanPrivatePackage/AnalysisFile/AnalysisFile_MC/Include/efficiency/ttHAnalysis/TnP_loose_ele_2016.root:EGamma_SF2D", "TTH_Electron_SF_2016_loose")
    gen_tth_sfs("/home/users/aaarora/phys/analysis/YifanPrivatePackage/AnalysisFile/AnalysisFile_MC/Include/efficiency/ttHAnalysis/TnP_loose_ele_2017.root:EGamma_SF2D", "TTH_Electron_SF_2017_loose")
    gen_tth_sfs("/home/users/aaarora/phys/analysis/YifanPrivatePackage/AnalysisFile/AnalysisFile_MC/Include/efficiency/ttHAnalysis/TnP_loose_ele_2018.root:EGamma_SF2D", "TTH_Electron_SF_2018_loose")
    gen_tth_sfs("/home/users/aaarora/phys/analysis/YifanPrivatePackage/AnalysisFile/AnalysisFile_MC/Include/efficiency/ttHAnalysis/TnP_loose_muon_2016.root:EGamma_SF2D", "TTH_Muon_SF_2016_loose")
    gen_tth_sfs("/home/users/aaarora/phys/analysis/YifanPrivatePackage/AnalysisFile/AnalysisFile_MC/Include/efficiency/ttHAnalysis/TnP_loose_muon_2017.root:EGamma_SF2D", "TTH_Muon_SF_2017_loose")
    gen_tth_sfs("/home/users/aaarora/phys/analysis/YifanPrivatePackage/AnalysisFile/AnalysisFile_MC/Include/efficiency/ttHAnalysis/TnP_loose_muon_2018.root:EGamma_SF2D", "TTH_Muon_SF_2018_loose")
    gen_tth_sfs("/home/users/aaarora/phys/analysis/YifanPrivatePackage/AnalysisFile/AnalysisFile_MC/Include/efficiency/ttHAnalysis/TnP_ttH_ele_2016_2lss/passttH/egammaEffi.txt_EGM2D.root:EGamma_SF2D", "TTH_Electron_SF_2016_tight")
    gen_tth_sfs("/home/users/aaarora/phys/analysis/YifanPrivatePackage/AnalysisFile/AnalysisFile_MC/Include/efficiency/ttHAnalysis/TnP_ttH_ele_2017_2lss/passttH/egammaEffi.txt_EGM2D.root:EGamma_SF2D", "TTH_Electron_SF_2017_tight")
    gen_tth_sfs("/home/users/aaarora/phys/analysis/YifanPrivatePackage/AnalysisFile/AnalysisFile_MC/Include/efficiency/ttHAnalysis/TnP_ttH_ele_2018_2lss/passttH/egammaEffi.txt_EGM2D.root:EGamma_SF2D", "TTH_Electron_SF_2018_tight")
    gen_tth_sfs("/home/users/aaarora/phys/analysis/YifanPrivatePackage/AnalysisFile/AnalysisFile_MC/Include/efficiency/ttHAnalysis/TnP_ttH_muon_2016_2lss/passttH/egammaEffi.txt_EGM2D.root:EGamma_SF2D", "TTH_Muon_SF_2016_tight")
    gen_tth_sfs("/home/users/aaarora/phys/analysis/YifanPrivatePackage/AnalysisFile/AnalysisFile_MC/Include/efficiency/ttHAnalysis/TnP_ttH_muon_2017_2lss/passttH/egammaEffi.txt_EGM2D.root:EGamma_SF2D", "TTH_Muon_SF_2017_tight")
    gen_tth_sfs("/home/users/aaarora/phys/analysis/YifanPrivatePackage/AnalysisFile/AnalysisFile_MC/Include/efficiency/ttHAnalysis/TnP_ttH_muon_2018_2lss/passttH/egammaEffi.txt_EGM2D.root:EGamma_SF2D", "TTH_Muon_SF_2018_tight")

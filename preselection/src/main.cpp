// RDF Includes
#include "ROOT/RDataFrame.hxx"

#include "weights.h"
#include "selections.h"
#include "utils.h"

#include "argparser.hpp"

struct MyArgs : public argparse::Args {
    std::string &spec = kwarg("i,input", "spec.json path");
    bool &isData = flag("data", "is data");
    bool &isBkg = flag("bkg", "is bkg");
    bool &isSig = flag("sig", "is sig");
};

int main(int argc, char** argv) {
    auto args = argparse::parse<MyArgs>(argc, argv);
    std::string input_spec = args.spec;

    ROOT::EnableImplicitMT();
    ROOT::RDataFrame df_ = ROOT::RDF::Experimental::FromSpec(input_spec);
    ROOT::RDF::Experimental::AddProgressBar(df_);
    
    auto df = defineMetadata(df_);
    // selections
    auto df1 = flagSelections(df);
    auto df2 = triggerSelections(df1);
    auto df3 = leptonSelections(df2);
    auto df4 = higgsSelections(df3);
    auto df5 = WZSelections(df4);
    auto df6 = AK4Selections(df5);
    auto df7 = VBSJetsSelections(df6);
    auto df8 = finalSelections(df7);

    if (args.isData) {
        auto df_data_final = goodRun(LumiMask, df8);
        
        saveSnapshot(df_data_final, "data.root");
    }
    else {
        // pileup
        auto df_pileup = pileupCorrection(cset_pileup_2016preVFP, cset_pileup_2016postVFP, cset_pileup_2017, cset_pileup_2018, df8);
        // muon sf
        auto df_muon_ID = muonScaleFactors_ID(cset_muon_ID_2016preVFP, cset_muon_ID_2016postVFP, cset_muon_ID_2017, cset_muon_ID_2018, df_pileup);
        auto df_muon_ttHID = muonScaleFactors_ttH(cset_muon_ttHID_2016preVFP, cset_muon_ttHID_2016postVFP, cset_muon_ttHID_2017, cset_muon_ttHID_2018, df_muon_ID, "muon_scale_factors_ttHID");
        auto df_muon_ttHISO = muonScaleFactors_ttH(cset_muon_ttHISO_2016preVFP, cset_muon_ttHISO_2016postVFP, cset_muon_ttHISO_2017, cset_muon_ttHISO_2018, df_muon_ttHID, "muon_scale_factors_ttHISO");
        // elec sf
        auto df_elec_ID = electronScaleFactors_ID(cset_electron_ID_2016preVFP, cset_electron_ID_2016postVFP, cset_electron_ID_2017, cset_electron_ID_2018, df_muon_ttHISO);
        auto df_elec_ttHID = electronScaleFactors_ttH(cset_electron_ttHID_2016preVFP, cset_electron_ttHID_2016postVFP, cset_electron_ttHID_2017, cset_electron_ttHID_2018, df_elec_ID, "electron_scale_factors_ttHID");
        auto df_elec_ttHISO = electronScaleFactors_ttH(cset_electron_ttHISO_2016preVFP, cset_electron_ttHISO_2016postVFP, cset_electron_ttHISO_2017, cset_electron_ttHISO_2018, df_elec_ttHID, "electron_scale_factors_ttHISO");
        // final weight
        auto df_mc_final = finalMCWeight(df_elec_ttHISO); // remember to add weights to finalMCWeight everytime.

        if (args.isBkg) {
            saveSnapshot(df_mc_final, "bkg.root");
        }
        else if (args.isSig) {
            saveSnapshot(df_mc_final, "sig.root");
        }
    }

    return 0;
}

// RDF Includes
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RDFHelpers.hxx"

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

    ROOT::EnableImplicitMT(64);
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
    auto df8 = AddHEMCorrection(df7);
    auto df9 = finalSelections(df8);

    if (args.isData) {
        auto df_golden = goodRun(LumiMask, df9);
        auto df_data_final = finalDataWeight(df_golden);
        saveSnapshot(df_data_final, "data.root");
    }
    else {
        // pileup
        auto df_pileup = pileupCorrection(cset_pileup_2016preVFP, cset_pileup_2016postVFP, cset_pileup_2017, cset_pileup_2018, df9);
        // muon sf
        auto df_muon_ID = muonScaleFactors_ID(cset_muon_ID_2016preVFP, cset_muon_ID_2016postVFP, cset_muon_ID_2017, cset_muon_ID_2018, df_pileup);
        auto df_muon_ttHID = muonScaleFactors_ttH(cset_muon_ttHID, df_muon_ID, "muon_scale_factors_ttHID");
        auto df_muon_ttHISO = muonScaleFactors_ttH(cset_muon_ttHISO, df_muon_ttHID, "muon_scale_factors_ttHISO");
        // elec sf
        auto df_elec_ID = electronScaleFactors_ID(cset_electron_ID_2016preVFP, cset_electron_ID_2016postVFP, cset_electron_ID_2017, cset_electron_ID_2018, df_muon_ttHISO);
        auto df_elec_ttHID = electronScaleFactors_ttH(cset_electron_ttHID, df_elec_ID, "electron_scale_factors_ttHID");
        auto df_elec_ttHISO = electronScaleFactors_ttH(cset_electron_ttHISO, df_elec_ttHID, "electron_scale_factors_ttHISO");
        auto df_elec_trigger = electronScaleFactors_Trigger(cset_electron_trigger, df_elec_ttHISO);
        // particle net
        auto df_pnet_w = PNET_W_Corrections(cset_pnet_w, df_elec_trigger);
        auto df_pnet_h = PNET_H_Corrections(cset_pnet_h, df_pnet_w);
        auto df_jmr = JMR_Corrections(cset_jmr, df_pnet_h);
        auto df_jms = JMS_Corrections(cset_jms, df_jmr);
        auto df_mc_final = finalMCWeight(df_jms); // remember to add weights to finalMCWeight everytime.

        if (args.isBkg) {
            saveSnapshot(df_mc_final, "bkg.root");
        }
        else if (args.isSig) {
            saveSnapshot(df_mc_final, "sig.root");
        }
    }

    return 0;
}

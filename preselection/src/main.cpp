// RDF Includes
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RDFHelpers.hxx"

#include "weights.h"
#include "selections.h"
#include "corrections.h"
#include "utils.h"

#include "argparser.hpp"

struct MyArgs : public argparse::Args {
    std::string &spec = kwarg("i,input", "spec.json path");
    bool &isData = flag("data", "is data");
    bool &isBkg = flag("bkg", "is bkg");
    bool &isSig = flag("sig", "is sig");
    bool &METUnclustered = flag("met", "MET unclustered");
    bool &JEC = flag("jec", "JEC");
    bool &JMS = flag("jms", "JMS");
    bool &JMR = flag("jmr", "JMR");
    std::string &output = kwarg("o,output", "output root file").set_default("");
    std::string &variation = kwarg("var", "variation").set_default("nominal");
    std::string &JERvariation = kwarg("jervar", "JER variation").set_default("nominal");
    std::string &JEC_type = kwarg("jec_type", "JEC type").set_default("");
};

int main(int argc, char** argv) {
    auto args = argparse::parse<MyArgs>(argc, argv);
    std::string input_spec = args.spec;
    std::string output_file = args.output;

    if (output_file.empty()) {
        if (args.isData) {
            output_file = "data.root";
        }
        else if (args.isBkg) {
            output_file = "bkg.root";
        }
        else if (args.isSig) {
            output_file = "sig.root";
        }
    }

    ROOT::EnableImplicitMT(64);
    ROOT::RDataFrame df_ = ROOT::RDF::Experimental::FromSpec(input_spec);
    ROOT::RDF::Experimental::AddProgressBar(df_);
    
    // define metadata
    auto df = defineMetadata(df_);
    // corrections
    auto df0 = defineCorrectedCols(df);
    auto df1 = HEMCorrection(df0);
    auto df2 = JetEnergyResolution(cset_jerc_2016preVFP, cset_jerc_2016postVFP, cset_jerc_2017, cset_jerc_2018, cset_jer_smear, df1, args.JERvariation);

    auto applypreCorrections = [] (RNode df, MyArgs args) {
        if (args.METUnclustered) {
            return METUnclusteredCorrections(df, args.variation);
        }
        else if (args.JEC) {
            return JetEnergyCorrection(cset_jerc_2016preVFP, cset_jerc_2016postVFP, cset_jerc_2017, cset_jerc_2018, df, args.JEC_type, args.variation);
        }
        else {
            return df;
        }
    };

    auto df_correction = applypreCorrections(df2, args);

    // selections
    auto df_selected = finalSelections(df_correction);

    auto applypostCorrections = [](RNode df, MyArgs args) {
        if (args.JMS) {
            return JMS_Corrections(cset_jms, df, args.variation);
        }
        else if (args.JMR) {
            return JMR_Corrections(cset_jmr, df, args.variation);
        }
        else {
            return df;
        }
    };

    auto df_final = applypostCorrections(df_selected, args);

    //scale factors
    if (args.isData) {
        auto df_data_final = finalDataWeight(df_final);
        saveSnapshot(df_data_final, output_file);
    }
    else {
        auto df_mc_final = finalMCWeight(df_final);
        if (args.isBkg) {
            saveSnapshot(df_mc_final, output_file);
        }
        else if (args.isSig) {
            saveSnapshot(df_mc_final, output_file);
        }
    }

    return 0;
}


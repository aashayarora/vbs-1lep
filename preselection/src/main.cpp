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
    bool &JEC = flag("jec", "JEC");
    bool &JER = flag("jer", "JER");
    bool &JMS = flag("jms", "JMS");
    bool &JMR = flag("jmr", "JMR");
    bool &METUnclustered = flag("met", "MET unclustered");
    int &nthreads = kwarg("n,nthreads", "number of threads").set_default(1);
    int &c2v = kwarg("c2v", "c2v value").set_default(-1);
    std::string &output = kwarg("o,output", "output root file").set_default("");
    std::string &variation = kwarg("var", "variation").set_default("nominal");
    std::string &JERvariation = kwarg("jervar", "JER variation").set_default("nominal");
    std::string &JEC_type = kwarg("jec_type", "JEC type").set_default("");
    std::string &SFvariation = kwarg("sfvar", "SF variation").set_default("");
};

void runBkgAnalysis(RNode df, std::string output_file) {
    // corrections
    auto df1 = defineCorrectedCols(df);
    auto df2 = HEMCorrection(df1);
    // selections
    auto df3 = finalSelections(df2);
    // scale factors
    auto df4 = finalMCWeight(df3);
    saveSnapshot(df4, output_file);
}

void runDataAnalysis(RNode df, std::string output_file) {
    // corrections
    auto df1 = defineCorrectedCols(df);
    auto df2 = HEMCorrection(df1);
    // selections
    auto df3 = finalSelections(df2);
    // scale factors
    auto df4 = finalDataWeight(df3);
    saveSnapshot(df4, output_file);
}

void runSigAnalysis(RNode df, MyArgs args, std::string output_file) {
    // corrections
    auto df1 = defineCorrectedCols(df);
    auto df2 = HEMCorrection(df1);
    // selections
    auto applypreCorrections = [] (RNode df, MyArgs args) {
        if (args.JER) {return JetEnergyResolution(cset_jerc_2016preVFP, cset_jerc_2016postVFP, cset_jerc_2017, cset_jerc_2018, cset_jer_smear, df, args.JERvariation);}
        else if (args.METUnclustered) {return METUnclusteredCorrections(df, args.variation);}
        else if (args.JEC) {return JetEnergyCorrection(cset_jerc_2016preVFP, cset_jerc_2016postVFP, cset_jerc_2017, cset_jerc_2018, df, args.JEC_type, args.variation);}
        else {return df;}
    };
    auto df3 = applypreCorrections(df2, args);
    auto df4 = finalSelections(df3);
    auto applypostCorrections = [](RNode df, MyArgs args) { 
        if (args.JMS) {return JMS_Corrections(cset_jms, df, args.variation);} 
        else if (args.JMR) {return JMR_Corrections(cset_jmr, df, args.variation);} 
        else {return df;}
    };
    auto df5 = applypostCorrections(df4, args);
    // scale factors
    auto df6 = finalMCWeight(df5);
    auto applySFVariations = [](RNode df, MyArgs args) {
        if (args.SFvariation.empty()) { return df;}
        else {
            if (args.variation == "up") {return df.Redefine("weight", Form("weight * %s[1] / %s[0]", args.SFvariation.c_str(), args.SFvariation.c_str()));}
            else if (args.variation == "down") {return df.Redefine("weight", Form("weight * %s[2] / %s[0]", args.SFvariation.c_str(), args.SFvariation.c_str()));}
            else {return df;}
        }
    };
    auto df7 = applySFVariations(df6, args);
    auto C2VReweighting = [](RNode df, MyArgs args) {
        if (args.c2v == -1) {return df;}
        else {
            return df.Redefine("weight", Form("weight * LHEReweightingWeight[%d]", args.c2v));
        }
    };
    auto df8 = C2VReweighting(df7, args);
    saveSnapshot(df8, output_file);
}

int main(int argc, char** argv) {
    auto args = argparse::parse<MyArgs>(argc, argv);
    std::string input_spec = args.spec;
    std::string output_file = args.output;

    ROOT::EnableImplicitMT(args.nthreads);
    ROOT::RDataFrame df_ = ROOT::RDF::Experimental::FromSpec(input_spec);
    // ROOT::RDF::Experimental::AddProgressBar(df_);

    // define metadata
    auto df = defineMetadata(df_);
    // auto report = df.Report();
    // run analysis
    if (input_spec.find("data") != std::string::npos) {
        if (output_file.empty()) {
            output_file = "data/data.root";
        }
        runDataAnalysis(df, output_file);
    }
    else if (input_spec.find("bkg") != std::string::npos) {
        if (output_file.empty()) {
            output_file = "bkg/bkg.root";
        }
        runBkgAnalysis(df, output_file);
    }
    else if (input_spec.find("sig") != std::string::npos) {
        if (output_file.empty()) {
            output_file = "sig/sig.root";
        }
        runSigAnalysis(df, args, output_file);
    }
    else {
        std::cerr << "Invalid input file, the spec file name must contain sig, bkg or data" << std::endl;
        return 1;
    }
    // report->Print();
    return 0;
}
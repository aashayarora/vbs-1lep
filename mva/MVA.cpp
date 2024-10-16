#include "MVA.h"

struct MyArgs : public argparse::Args {
    std::string &input = kwarg("i,input", "input path");
    std::string &output = kwarg("o,output", "output path");
    std::string &year = kwarg("y,year", "year").set_default("");
    std::string &SFvariation = kwarg("sfvar", "SF variation").set_default("");
    std::string &variation = kwarg("var", "variation").set_default("nominal");
    std::string &cut = kwarg("cut", "cut on final snapshot").set_default("passCut7");
    int &nthreads = kwarg("n,nthreads", "number of threads").set_default(1);
    int &c2v = kwarg("c2v", "c2v value").set_default(-1);
};

int main(int argc, char** argv){
    auto args = argparse::parse<MyArgs>(argc, argv);
    std::string input_file = args.input;
    std::string output_file = args.output;

    TMVA::Experimental::RBDT bdt("VBSBDT", "/home/users/aaarora/phys/analysis/vbs-1lep/mva/weights/BDT/BDT_Weights.root");
    TMVA::Experimental::RSofieReader dnn("weights/DNN/model.pt", {{1, 7}});

    ROOT::EnableImplicitMT(args.nthreads);
    ROOT::RDataFrame df("Events", input_file);
    ROOT::RDF::Experimental::AddProgressBar(df);
    
    auto df1_ = df.Filter(args.cut)
            .Define("VBSBDTOutput", Compute<8, float>(bdt), {"VBSjet1pt", "VBSjet1eta", "VBSjet1phi", "VBSjet2pt","VBSjet2eta", "VBSjet2phi", "VBSMjj", "VBSdetajj"})
            .Define("VBSBDTscore", "VBSBDTOutput[0]")
            .Define("DNN_Hbbmass", "(Hbbmass - 50) / 200")
            .Define("DNN_Wjetmass", "Wjetmass / 200")
            .Define("DNN_HbbPt", "log(HbbPt)")
            .Define("DNN_WjetPt", "log(WjetPt)")
            .Define("DNN_leptonpt", "log(leptonpt)")
            .Define("DNN_Mlbminloose", "(Mlbminloose - 0) / 1000")
            .Define("DNN_MET", "log(MET)")
            .Define("abcdnet_output", Compute<7, float>(dnn), {"DNN_Hbbmass", "DNN_HbbPt", "DNN_Wjetmass", "DNN_WjetPt",  "DNN_leptonpt", "DNN_Mlbminloose", "DNN_MET"})
            .Define("abcdnet_score", "abcdnet_output[0]");

    auto df1 = RNode(df1_);
    
    if (!args.year.empty()) {
        df1 = df1.Filter(Form("sample_year == \"%s\"", args.year.c_str()));
    }

    if (!args.SFvariation.empty()) {
        if (args.variation == "up") {
            df1 = df1.Redefine("weight", Form("weight * %s[1] / %s[0]", args.SFvariation.c_str(), args.SFvariation.c_str()));
        }
        else if (args.variation == "down") {
            df1 = df1.Redefine("weight", Form("weight * %s[2] / %s[0]", args.SFvariation.c_str(), args.SFvariation.c_str()));
        }
    }

    if (args.c2v != -1) {
        df1 = df1.Redefine("weight", Form("weight * LHEReweightingWeight[%d]", args.c2v));
    }

    std::vector<std::string> final_vars = {"event", 
                                        "sample_type", 
                                        args.cut, 
                                        "Hbbscore", 
                                        "HighestWjetScore", 
                                        "HbbPt", 
                                        "Hbbmass", 
                                        "MET", 
                                        "Mlbminloose", 
                                        "VBSBDTscore",
                                        "abcdnet_score",
                                        "VBSMjj", 
                                        "VBSdetajj", 
                                        "VBSjet1eta", 
                                        "VBSjet1phi", 
                                        "VBSjet1pt", 
                                        "VBSjet2eta", 
                                        "VBSjet2phi", 
                                        "VBSjet2pt", 
                                        "WjetPt", 
                                        "Wjetmass", 
                                        "leptonpt", 
                                        "weight"};

    df1.Snapshot("Events", output_file, final_vars);

    return 0;
}
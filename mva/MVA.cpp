#include "MVA.h"

struct MyArgs : public argparse::Args {
    std::string &input = kwarg("i,input", "input path");
    std::string &output = kwarg("o,output", "output path");
    std::string &year = kwarg("y,year", "year").set_default("");
    std::string &SFvariation = kwarg("sfvar", "SF variation").set_default("");
    std::string &variation = kwarg("var", "variation").set_default("nominal");
    std::string &cut = kwarg("cut", "cut on final snapshot").set_default("passCut9");
    int &nthreads = kwarg("n,nthreads", "number of threads").set_default(1);
    int &c2v = kwarg("c2v", "c2v value").set_default(-1);
};

int main(int argc, char** argv){
    auto args = argparse::parse<MyArgs>(argc, argv);
    std::string input_file = args.input;
    std::string output_file = args.output;

    // TMVA::Experimental::RBDT bdt("VBSBDT", "/home/users/aaarora/phys/analysis/vbs-1lep/training/BDT/BDT_Weights.root");
    // TMVA::Experimental::RSofieReader dnn("/home/users/aaarora/phys/analysis/vbs-1lep/junk/model.onnx");

    TMVA::Experimental::RReader reader_AB("weights/BDT/BDT_AB/TMVAClassification_BDT.weights.xml");
    TMVA::Experimental::RReader reader_BA("weights/BDT/BDT_BA/TMVAClassification_BDT.weights.xml");

    auto predict = [&](unsigned long long event, float VBSjet1pt, float VBSjet1eta, float VBSjet1phi, float VBSjet2pt, float VBSjet2eta, float VBSjet2phi, float VBSMjj, float VBSdetajj){
        if(event % 2){
            auto score_AB = reader_AB.Compute({VBSjet1pt, VBSjet1eta, VBSjet1phi, VBSjet2pt, VBSjet2eta, VBSjet2phi, VBSMjj, VBSdetajj});
            return score_AB[0];
        }
        else{
            auto score_BA = reader_BA.Compute({VBSjet1pt, VBSjet1eta, VBSjet1phi, VBSjet2pt, VBSjet2eta, VBSjet2phi, VBSMjj, VBSdetajj});
            return score_BA[0];
        }
    };

    ROOT::EnableImplicitMT(args.nthreads);
    ROOT::RDataFrame df("Events", input_file);
    ROOT::RDF::Experimental::AddProgressBar(df);
    // auto df1_ = df.Filter(args.cut)
            // .Define("VBSBDTOutput", Compute<8, float>(bdt), {"VBSjet1pt", "VBSjet1eta", "VBSjet1phi", "VBSjet2pt","VBSjet2eta", "VBSjet2phi", "VBSMjj", "VBSdetajj"})
            // .Define("VBSBDTscore", "VBSBDTOutput[0]");

    auto df1_ = df.Filter(args.cut)
            .Define("VBSBDTscore", predict, {"event", "VBSjet1pt", "VBSjet1eta", "VBSjet1phi", "VBSjet2pt","VBSjet2eta", "VBSjet2phi", "VBSMjj", "VBSdetajj"});

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

    std::vector<std::string> final_vars = {"event", "sample_type", args.cut, "Hbbscore", "HighestWjetScore", "HbbPt", "Hbbmass", "MET", "Mlbminloose", "VBSBDTscore", "VBSMjj", "VBSdetajj", "VBSjet1eta", "VBSjet1phi", "VBSjet1pt", "VBSjet2eta", "VBSjet2phi", "VBSjet2pt", "WjetPt", "Wjetmass", "leptonpt", "weight"};
    df1.Snapshot("Events", output_file, final_vars);

    return 0;
}
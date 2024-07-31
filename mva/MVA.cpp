#include "MVA.h"
#include "argparser.hpp"

struct MyArgs : public argparse::Args {
    std::string &input = kwarg("i,input", "input path");
    std::string &output = kwarg("o,output", "output path");
    std::string &year = kwarg("y,year", "year").set_default("");
};

int main(int argc, char** argv){
    auto args = argparse::parse<MyArgs>(argc, argv);
    std::string input_file = args.input;
    std::string output_file = args.output;

    RReader reader_AB("weights/BDT/BDT_AB/TMVAClassification_BDT.weights.xml");
    RReader reader_BA("weights/BDT/BDT_BA/TMVAClassification_BDT.weights.xml");

    auto predict = [&](unsigned long long event, float VBSjet1pt, float VBSjet1eta, float VBSjet1phi, float VBSjet2pt, float VBSjet2eta, float VBSjet2phi, float VBSMjj, float VBSdetajj){
        // generate random number to decide which BDT to use
        if(event % 2){
            auto score_AB = reader_AB.Compute({VBSjet1pt, VBSjet1eta, VBSjet1phi, VBSjet2pt, VBSjet2eta, VBSjet2phi, VBSMjj, VBSdetajj});
            return score_AB[0];
        }
        else{
            auto score_BA = reader_BA.Compute({VBSjet1pt, VBSjet1eta, VBSjet1phi, VBSjet2pt, VBSjet2eta, VBSjet2phi, VBSMjj, VBSdetajj});
            return score_BA[0];
        }
    };

    // ROOT::EnableImplicitMT(4);
    ROOT::RDataFrame df("Events", input_file);
    ROOT::RDF::Experimental::AddProgressBar(df);
    // Make final cuts before running MVA

    auto df1 = df.Define("VBSBDTscore", predict, {"event", "VBSjet1pt", "VBSjet1eta", "VBSjet1phi", "VBSjet2pt","VBSjet2eta", "VBSjet2phi", "VBSMjj", "VBSdetajj"});

    auto filter_year = [](RNode df, MyArgs args){
        if (args.year.empty()) {return df.Filter("event > 0");}
        else {
            return df.Filter(Form("sample_year == \"%s\"", args.year.c_str()));
        } 
    };
    auto df2 = filter_year(df1, args);

    std::vector<std::string> final_vars = {"event", "Hbbscore", "HighestWjetScore", "HbbPt", "Hbbmass", "MET", "Mlbminloose", "VBSBDTscore", "VBSMjj", "VBSdetajj", "VBSjet1eta", "VBSjet1phi", "VBSjet1pt", "VBSjet2eta", "VBSjet2phi", "VBSjet2pt", "WjetPt", "Wjetmass", "leptonpt", "weight"};
    df2.Snapshot("Events", output_file, final_vars);

    return 0;
}
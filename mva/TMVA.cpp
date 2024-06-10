#include "TMVA.h"

int main(int argc, char** argv){
    std::string input_file = std::string(argv[1]);
    std::string output_file = std::string(argv[2]);

    RReader reader_AB("weights/BDT/BDT_AB/TMVAClassification_BDT.weights.xml");
    RReader reader_BA("weights/BDT/BDT_BA/TMVAClassification_BDT.weights.xml");

    TRandom3 rndm(1234);
    auto predict = [&](float VBSjet1pt, float VBSjet1eta, float VBSjet1phi, float VBSjet2pt, float VBSjet2eta, float VBSjet2phi, float VBSMjj, float VBSdetajj){
        // generate random number to decide which BDT to use
        float rnd = rndm.Uniform();
        if(rnd > 0.5){
            auto score_AB = reader_AB.Compute({VBSjet1pt, VBSjet1eta, VBSjet1phi, VBSjet2pt, VBSjet2eta, VBSjet2phi, VBSMjj, VBSdetajj});
            return score_AB[0];
        }
        else{
            auto score_BA = reader_BA.Compute({VBSjet1pt, VBSjet1eta, VBSjet1phi, VBSjet2pt, VBSjet2eta, VBSjet2phi, VBSMjj, VBSdetajj});
            return score_BA[0];
        }
    };

    ROOT::EnableImplicitMT(4);
    ROOT::RDataFrame df("Events", input_file);
    ROOT::RDF::Experimental::AddProgressBar(df);

    // Make final cuts before running MVA
    auto df1 = df.Filter("HighestHScore > 0.5 && HighestWjetScore > 0.7");
    auto df2 = df1.Define("VBSBDTscore", predict, {"VBSjet1pt", "VBSjet1eta", "VBSjet1phi", "VBSjet2pt","VBSjet2eta", "VBSjet2phi", "VBSMjj", "VBSdetajj"});

    std::vector<std::string> final_vars = {"Hbbscore", "HighestWjetScore", "HbbPt", "Hbbmass", "MET", "Mlbminloose", "VBSBDTscore", "VBSMjj", "VBSdetajj", "VBSjet1eta", "VBSjet1phi", "VBSjet1pt", "VBSjet2eta", "VBSjet2phi", "VBSjet2pt", "WjetPt", "Wjetmass", "leptonpt", "weight"};
    df2.Snapshot("Events", output_file, final_vars);

    return 0;
}
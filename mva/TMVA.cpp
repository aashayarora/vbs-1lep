#include "TMVA.h"

int main(int argc, char** argv){
    RReader reader_AB("weights/BDT_AB/TMVAClassification_BDT.weights.xml");
    RReader reader_BA("weights/BDT_BA/TMVAClassification_BDT.weights.xml");

    auto predict = [&](float VBSjet1pt, float VBSjet1eta, float VBSjet1phi, float VBSjet2pt, float VBSjet2eta, float VBSjet2phi, float VBSMjj, float VBSdetajj){
        // generate random number to decide which BDT to use
        TRandom3 rndm;
        if(rndm.Uniform() > 0.5){
            auto score_AB = reader_AB.Compute({VBSjet1pt, VBSjet1eta, VBSjet1phi, VBSjet2pt, VBSjet2eta, VBSjet2phi, VBSMjj, VBSdetajj});
            return score_AB;
        }
        else{
            auto score_BA = reader_BA.Compute({VBSjet1pt, VBSjet1eta, VBSjet1phi, VBSjet2pt, VBSjet2eta, VBSjet2phi, VBSMjj, VBSdetajj});
            return score_BA;
        }        
    };

    ROOT::RDataFrame df_data("Events", "/data/userdata/aaarora/output/run2/data.root");
    auto df1 = df_data.Define("BDT_Score", predict, {"VBSjet1pt", "VBSjet1eta", "VBSjet1phi", "VBSjet2pt","VBSjet2eta", "VBSjet2phi", "VBSMjj", "VBSdetajj"});

    auto h = df1.Histo1D({"BDT_Score", "BDT_Score", 20, 0, 1}, "BDT_Score");
    
    TCanvas c;
    h->Draw();
    c.SaveAs("BDT_Score.png");

    return 0;
}
#include "MVA.h"

#include <iostream>

RNode MVA(RNode df){
    static TMVA::Experimental::RReader reader_AB("weights/BDT_AB/TMVAClassification_BDT.weights.xml");
    static TMVA::Experimental::RReader reader_BA("weights/BDT_BA/TMVAClassification_BDT.weights.xml");

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

    return df.Filter("HighestHScore > 0.5 && HighestWjetScore > 0.7")
            .Define("VBSBDTscore", predict, {"VBSjet1pt", "VBSjet1eta", "VBSjet1phi", "VBSjet2pt","VBSjet2eta", "VBSjet2phi", "VBSMjj", "VBSdetajj"});
}
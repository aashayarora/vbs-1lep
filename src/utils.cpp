#include "utils.h"

double get_btag_workingpoint(const std::string& year, const std::string& wp){
    if(year == "2016preVFP"){
        if(wp == "loose") return 0.5426;
        if(wp == "medium") return 0.8484;
        if(wp == "tight") return 0.9535;
    }
    if(year == "2016postVFP"){
        if(wp == "loose") return 0.5426;
        if(wp == "medium") return 0.8484;
        if(wp == "tight") return 0.9535;
    }
    if(year == "2017"){
        if(wp == "loose") return 0.5803;
        if(wp == "medium") return 0.8838;
        if(wp == "tight") return 0.9693;
    }
    if(year == "2018"){
        if(wp == "loose") return 0.1241;
        if(wp == "medium") return 0.4184;
        if(wp == "tight") return 0.7527;
    }
    return -1;
}

void saveSnapshot(RNode df, const std::string& finalFile) {
    auto ColNames = df.GetColumnNames();
    std::vector<std::string> final_variables;
    for (auto &&ColName : ColNames)
        {
            TString colName = ColName;
            if(colName.Contains("P4")) continue;
            if(colName.Contains("LHE")) continue;
            if(colName.BeginsWith("FatJet")) continue;
            if(colName.BeginsWith("PPS")) continue;
            if(colName.BeginsWith("Gen")) continue;
            if(colName.BeginsWith("Sub")) continue;
            if(colName.BeginsWith("HLT")) continue;
            if(colName.BeginsWith("Flag")) continue;
            if(colName.BeginsWith("PS")) continue;
            if(colName.BeginsWith("nCorr")) continue;
            if(colName.BeginsWith("nGen")) continue;
            if(colName.BeginsWith("nOther")) continue;
            if(colName.BeginsWith("nSV")) continue;

            std::string name = colName.Data();
            final_variables.push_back(name);
        }
        df.Snapshot("Events", std::string("output/") + finalFile, final_variables);
}
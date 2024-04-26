// C++ Includes
#include <iostream>

// RDF Includes
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RDFHelpers.hxx"
#include "ROOT/RVec.hxx"

#include "weights.h"
#include "selections.h"
#include "utils.h"

using std::cout, std::endl;

int main(int argc, char** argv) {
    // Config
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <input_spec>" << endl;
        return 0;
    }
    std::string input_spec = argv[1];
    
    // golden json lumimask
    const auto LumiMask = lumiMask::fromJSON("corrections/goldenJson/Cert_271036-325175_13TeV_allRun2_JSON.json");

    ROOT::EnableImplicitMT();
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
    auto df8 = finalSelections(df7);

    // data only corrections
    auto df_data = df8.Filter("isData");
    auto df_data_final = goodRun(LumiMask, df_data);
    saveSnapshot(df_data_final, "data.root");

    // MC only corrections
    auto df_mc = df9.Filter("!isData");
    auto df_mc_final = pileupWeight(df_mc);
    
    saveSnapshot(df_mc_final, "mc.root");

    // print cutflow
    auto report = df_.Report();
    report->Print();
    

    return 0;
}

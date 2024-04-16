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
using ROOT::RDF::RSampleInfo;

int main(int argc, char** argv) {
    // Config
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <input_spec>" << endl;
        return 0;
    }
    std::string input_spec = argv[1];
    
    // golden json lumimask
    const auto LumiMask = lumiMask::fromJSON("corrections/goldenJson/Cert_271036-325175_13TeV_allRun2_JSON.json");
    // const auto LumiMask = lumiMask::fromJSON("corrections/goldenJson/Cert_271036-325175_13TeV_preLegacy_allRun2_JSON.json");

    ROOT::EnableImplicitMT();
    ROOT::RDataFrame df_ = ROOT::RDF::Experimental::FromSpec(input_spec);
    ROOT::RDF::Experimental::AddProgressBar(df_);

    std::cout << "Running dataframe" << std::endl;

    auto df = df_.DefinePerSample("xsec_weight", [](unsigned int slot, const RSampleInfo &id) { return id.GetD("xsec_weight");})
                .DefinePerSample("sample_category", [](unsigned int slot, const RSampleInfo &id) { return id.GetS("sample_category");})
                .DefinePerSample("sample_type", [](unsigned int slot, const RSampleInfo &id) { return id.GetS("sample_type");})
                .DefinePerSample("sample_year", [](unsigned int slot, const RSampleInfo &id) { return id.GetS("sample_year");})
                .Define("isData", "sample_category == \"data\"")
                .Define("is2016", "sample_year == \"2016preVFP\" || sample_year == \"2016postVFP\"")
                .Define("is2017", "sample_year == \"2017\"")
                .Define("is2018", "sample_year == \"2018\"");
    
    std::cout << "Defined meta columns, running selections" << std::endl;

    auto df1 = goodRun(LumiMask, df);
    auto df2 = analysisSelections(df1);

    // auto df3 = removeDuplicates(df2);

    auto report = df_.Report();
    report->Print();

    return 0;
}

// C++ Includes
#include <iostream>

// RDF Includes
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RDFHelpers.hxx"
#include "ROOT/RVec.hxx"

#include "weights.h"
#include "selections.h"
#include "lumimask.h"

using std::cout, std::endl;
using ROOT::RDF::RSampleInfo;

int main(int argc, char** argv) {
    // Config
    std::string input_spec = "configs/input.json";
    
    // golden json lumimask
    const auto LumiMask = lumiMask::fromJSON("corrections/goldenJson/Cert_271036-325175_13TeV_allRun2_JSON.json");

    ROOT::EnableImplicitMT(32); // enable multithreaded rdfs
    ROOT::RDataFrame df_ = ROOT::RDF::Experimental::FromSpec(input_spec);
    ROOT::RDF::Experimental::AddProgressBar(df_);

    auto df = df_.DefinePerSample("xsec_weights", [](unsigned int slot, const RSampleInfo &id) { return id.GetD("xsec_weights");})
                 .DefinePerSample("sample_category", [](unsigned int slot, const RSampleInfo &id) { return id.GetS("sample_category");});
    
    auto df1 = electronSelections(df);
    auto df2 = muonSelections(df1);
    auto df3 = eventSelections(df2);

    saveSnapshot(df3, "output.root");
    
    auto report = df_.Report();
    report->Print();

    return 0;
}

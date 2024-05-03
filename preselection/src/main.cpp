// RDF Includes
#include "ROOT/RDataFrame.hxx"

#include "weights.h"
#include "selections.h"
#include "utils.h"

#include "argparser.hpp"

struct MyArgs : public argparse::Args {
    std::string &spec = kwarg("i,input", "spec.json path");
    bool &isData      = flag("isData", "isData");
};

int main(int argc, char** argv) {
    // Config
    auto args = argparse::parse<MyArgs>(argc, argv);
    std::string input_spec = args.spec;

    ROOT::EnableImplicitMT();
    ROOT::RDataFrame df_ = ROOT::RDF::Experimental::FromSpec(input_spec);
    ROOT::RDF::Experimental::AddProgressBar(df_);

    auto df = defineMetadata(df_);
    // selections
    auto df0 = goodRun(LumiMask, df);
    auto df1 = flagSelections(df0);
    auto df2 = triggerSelections(df1);
    auto df3 = leptonSelections(df2);
    auto df4 = higgsSelections(df3);
    auto df5 = WZSelections(df4);
    auto df6 = AK4Selections(df5);
    auto df7 = VBSJetsSelections(df6);
    auto df8 = finalSelections(df7);

    // print cutflow
    auto report = df_.Report();
    report->Print();
    

    return 0;
}

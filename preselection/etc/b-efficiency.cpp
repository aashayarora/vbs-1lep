#include "ROOT/RDataFrame.hxx"
#include "ROOT/RDFHelpers.hxx"
#include "ROOT/RVec.hxx"
#include "TFile.h"

#include <vector>

using RNode = ROOT::RDF::RNode;
using ROOT::VecOps::RVec;
using ROOT::RDF::RSampleInfo;

float looseDFBtagWP(std::string year){
    if(year == "2016preVFP")
        return 0.0508;
    if(year == "2016postVFP")
        return 0.0480;
    if(year == "2017")
        return 0.0532;
    if(year == "2018")
        return 0.0490;
    return -1;
}

float mediumDFBtagWP(std::string year){
    if(year == "2016preVFP")
        return 0.2598;
    if(year == "2016postVFP")
        return 0.2489;
    if(year == "2017")
        return 0.3040;
    if(year == "2018")
        return 0.2783;
    return -1;
}

float tightDFBtagWP(std::string year){
    if(year == "2016preVFP")
        return 0.6502;
    if(year == "2016postVFP")
        return 0.6377;
    if(year == "2017")
        return 0.7476;
    if(year == "2018")
        return 0.7100;
    return -1;
}

int main(int arc, char** argv){
    ROOT::EnableImplicitMT(64);
    ROOT::RDataFrame df = ROOT::RDF::Experimental::FromSpec("../input/sig.json");
    ROOT::RDF::Experimental::AddProgressBar(df);

    auto df0 = df.DefinePerSample("xsec_weight", [](unsigned int slot, const RSampleInfo &id) { return id.GetD("xsec_weight");})
            .DefinePerSample("sample_category", [](unsigned int slot, const RSampleInfo &id) { return id.GetS("sample_category");})
            .DefinePerSample("sample_type", [](unsigned int slot, const RSampleInfo &id) { return id.GetS("sample_type");})
            .DefinePerSample("sample_year", [](unsigned int slot, const RSampleInfo &id) { return id.GetS("sample_year");})
            .Define("isData", "sample_category == \"data\"")
            .Define("is2016", "sample_year == \"2016preVFP\" || sample_year == \"2016postVFP\"")
            .Define("is2017", "sample_year == \"2017\"")
            .Define("is2018", "sample_year == \"2018\"");

    auto df1 = df0.Define("ak4tightBjetScore", tightDFBtagWP, {"sample_year"})
                .Define("ak4mediumBjetScore", mediumDFBtagWP, {"sample_year"})
                .Define("ak4looseBjetScore", looseDFBtagWP, {"sample_year"})
                .Define("tightBJets", "Jet_btagDeepFlavB > ak4tightBjetScore && Jet_hadronFlavour == 5") 
                .Define("tightbjet_pt", "Jet_pt[tightBJets]")
                .Define("tightbjet_eta", "Jet_eta[tightBJets]")
                .Define("looseBJets", "Jet_btagDeepFlavB > ak4looseBjetScore && Jet_btagDeepFlavB < ak4tightBjetScore && Jet_hadronFlavour == 5")
                .Define("loosebjet_pt", "Jet_pt[looseBJets]")
                .Define("loosebjet_eta", "Jet_eta[looseBJets]")
                .Define("notagBJets", "Jet_btagDeepFlavB < ak4looseBjetScore && Jet_hadronFlavour == 5")
                .Define("notagbjet_pt", "Jet_pt[notagBJets]")
                .Define("notagbjet_eta", "Jet_eta[notagBJets]")
                .Define("tightCJets", "Jet_btagDeepFlavB > ak4tightBjetScore && Jet_hadronFlavour == 4")
                .Define("tightcjet_pt", "Jet_pt[tightCJets]")
                .Define("tightcjet_eta", "Jet_eta[tightCJets]")
                .Define("looseCJets", "Jet_btagDeepFlavB > ak4looseBjetScore && Jet_btagDeepFlavB < ak4tightBjetScore && Jet_hadronFlavour == 4")
                .Define("loosecjet_pt", "Jet_pt[looseCJets]")
                .Define("loosecjet_eta", "Jet_eta[looseCJets]")
                .Define("notagCJets", "Jet_btagDeepFlavB < ak4looseBjetScore && Jet_hadronFlavour == 4")
                .Define("notagcjet_pt", "Jet_pt[notagCJets]")
                .Define("notagcjet_eta", "Jet_eta[notagCJets]")
                .Define("tightLJets", "Jet_btagDeepFlavB > ak4tightBjetScore && Jet_hadronFlavour == 0")
                .Define("tightljet_pt", "Jet_pt[tightLJets]")
                .Define("tightljet_eta", "Jet_eta[tightLJets]")
                .Define("looseLJets", "Jet_btagDeepFlavB > ak4looseBjetScore && Jet_btagDeepFlavB < ak4tightBjetScore && Jet_hadronFlavour == 0")
                .Define("looseljet_pt", "Jet_pt[looseLJets]")
                .Define("looseljet_eta", "Jet_eta[looseLJets]")
                .Define("notagLJets", "Jet_btagDeepFlavB < ak4looseBjetScore && Jet_hadronFlavour == 0")
                .Define("notagljet_pt", "Jet_pt[notagLJets]")
                .Define("notagljet_eta", "Jet_eta[notagLJets]");

    TFile* f = new TFile("hist_beff.root", "RECREATE");

    double pt_bins[11] = {15, 20, 30, 50, 70, 100, 140, 200, 300, 600, 1000};
    double eta_bins[5] = {-2.5, -0.9, 0, 0.9, 2.5};

    auto hist_btight_2016preVFP = df1.Filter("sample_year == \"2016preVFP\"").Histo2D({"hist_btight_2016preVFP", "hist_btight_2016preVFP;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "tightbjet_pt", "tightbjet_eta");    
    auto hist_bloose_2016preVFP = df1.Filter("sample_year == \"2016preVFP\"").Histo2D({"hist_bloose_2016preVFP", "hist_bloose_2016preVFP;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "loosebjet_pt", "loosebjet_eta");
    auto hist_bnotag_2016preVFP = df1.Filter("sample_year == \"2016preVFP\"").Histo2D({"hist_bnotag_2016preVFP", "hist_bnotag_2016preVFP;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "notagbjet_pt", "notagbjet_eta");

    auto hist_ctight_2016preVFP = df1.Filter("sample_year == \"2016preVFP\"").Histo2D({"hist_ctight_2016preVFP", "hist_ctight_2016preVFP;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "tightcjet_pt", "tightcjet_eta");
    auto hist_cloose_2016preVFP = df1.Filter("sample_year == \"2016preVFP\"").Histo2D({"hist_cloose_2016preVFP", "hist_cloose_2016preVFP;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "loosecjet_pt", "loosecjet_eta");
    auto hist_cnotag_2016preVFP = df1.Filter("sample_year == \"2016preVFP\"").Histo2D({"hist_cnotag_2016preVFP", "hist_cnotag_2016preVFP;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "notagcjet_pt", "notagcjet_eta");

    auto hist_ltight_2016preVFP = df1.Filter("sample_year == \"2016preVFP\"").Histo2D({"hist_ltight_2016preVFP", "hist_ltight_2016preVFP;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "tightljet_pt", "tightljet_eta");
    auto hist_lloose_2016preVFP = df1.Filter("sample_year == \"2016preVFP\"").Histo2D({"hist_lloose_2016preVFP", "hist_lloose_2016preVFP;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "looseljet_pt", "looseljet_eta");
    auto hist_lnotag_2016preVFP = df1.Filter("sample_year == \"2016preVFP\"").Histo2D({"hist_lnotag_2016preVFP", "hist_lnotag_2016preVFP;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "notagljet_pt", "notagljet_eta");

    auto hist_total_2016preVFP = df1.Filter("sample_year == \"2016preVFP\"").Histo2D({"hist_total_2016preVFP", "hist_total_2016preVFP;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "Jet_pt", "Jet_eta").GetValue();

    hist_btight_2016preVFP->Divide(&hist_total_2016preVFP);
    hist_bloose_2016preVFP->Divide(&hist_total_2016preVFP);
    hist_bnotag_2016preVFP->Divide(&hist_total_2016preVFP);

    hist_ctight_2016preVFP->Divide(&hist_total_2016preVFP);
    hist_cloose_2016preVFP->Divide(&hist_total_2016preVFP);
    hist_cnotag_2016preVFP->Divide(&hist_total_2016preVFP);

    hist_ltight_2016preVFP->Divide(&hist_total_2016preVFP);
    hist_lloose_2016preVFP->Divide(&hist_total_2016preVFP);
    hist_lnotag_2016preVFP->Divide(&hist_total_2016preVFP);

    hist_btight_2016preVFP->Write();
    hist_bloose_2016preVFP->Write();
    hist_bnotag_2016preVFP->Write();

    hist_ctight_2016preVFP->Write();
    hist_cloose_2016preVFP->Write();
    hist_cnotag_2016preVFP->Write();

    hist_ltight_2016preVFP->Write();
    hist_lloose_2016preVFP->Write();
    hist_lnotag_2016preVFP->Write();

    auto hist_btight_2016postVFP = df1.Filter("sample_year == \"2016postVFP\"").Histo2D({"hist_btight_2016postVFP", "hist_btight_2016postVFP;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "tightbjet_pt", "tightbjet_eta");    
    auto hist_bloose_2016postVFP = df1.Filter("sample_year == \"2016postVFP\"").Histo2D({"hist_bloose_2016postVFP", "hist_bloose_2016postVFP;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "loosebjet_pt", "loosebjet_eta");
    auto hist_bnotag_2016postVFP = df1.Filter("sample_year == \"2016postVFP\"").Histo2D({"hist_bnotag_2016postVFP", "hist_bnotag_2016postVFP;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "notagbjet_pt", "notagbjet_eta");

    auto hist_ctight_2016postVFP = df1.Filter("sample_year == \"2016postVFP\"").Histo2D({"hist_ctight_2016postVFP", "hist_ctight_2016postVFP;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "tightcjet_pt", "tightcjet_eta");
    auto hist_cloose_2016postVFP = df1.Filter("sample_year == \"2016postVFP\"").Histo2D({"hist_cloose_2016postVFP", "hist_cloose_2016postVFP;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "loosecjet_pt", "loosecjet_eta");
    auto hist_cnotag_2016postVFP = df1.Filter("sample_year == \"2016postVFP\"").Histo2D({"hist_cnotag_2016postVFP", "hist_cnotag_2016postVFP;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "notagcjet_pt", "notagcjet_eta");

    auto hist_ltight_2016postVFP = df1.Filter("sample_year == \"2016postVFP\"").Histo2D({"hist_ltight_2016postVFP", "hist_ltight_2016postVFP;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "tightljet_pt", "tightljet_eta");
    auto hist_lloose_2016postVFP = df1.Filter("sample_year == \"2016postVFP\"").Histo2D({"hist_lloose_2016postVFP", "hist_lloose_2016postVFP;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "looseljet_pt", "looseljet_eta");
    auto hist_lnotag_2016postVFP = df1.Filter("sample_year == \"2016postVFP\"").Histo2D({"hist_lnotag_2016postVFP", "hist_lnotag_2016postVFP;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "notagljet_pt", "notagljet_eta");
    
    auto hist_total_2016postVFP = df1.Filter("sample_year == \"2016postVFP\"").Histo2D({"hist_total_2016postVFP", "hist_total_2016postVFP;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "Jet_pt", "Jet_eta").GetValue();

    hist_btight_2016postVFP->Divide(&hist_total_2016postVFP);
    hist_bloose_2016postVFP->Divide(&hist_total_2016postVFP);
    hist_bnotag_2016postVFP->Divide(&hist_total_2016postVFP);

    hist_ctight_2016postVFP->Divide(&hist_total_2016postVFP);
    hist_cloose_2016postVFP->Divide(&hist_total_2016postVFP);
    hist_cnotag_2016postVFP->Divide(&hist_total_2016postVFP);

    hist_ltight_2016postVFP->Divide(&hist_total_2016postVFP);
    hist_lloose_2016postVFP->Divide(&hist_total_2016postVFP);
    hist_lnotag_2016postVFP->Divide(&hist_total_2016postVFP);

    hist_btight_2016postVFP->Write();
    hist_bloose_2016postVFP->Write();
    hist_bnotag_2016postVFP->Write();

    hist_ctight_2016postVFP->Write();
    hist_cloose_2016postVFP->Write();
    hist_cnotag_2016postVFP->Write();

    hist_ltight_2016postVFP->Write();
    hist_lloose_2016postVFP->Write();
    hist_lnotag_2016postVFP->Write();

    auto hist_btight_2017 = df1.Filter("sample_year == \"2017\"").Histo2D({"hist_btight_2017", "hist_btight_2017;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "tightbjet_pt", "tightbjet_eta");    
    auto hist_bloose_2017 = df1.Filter("sample_year == \"2017\"").Histo2D({"hist_bloose_2017", "hist_bloose_2017;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "loosebjet_pt", "loosebjet_eta");
    auto hist_bnotag_2017 = df1.Filter("sample_year == \"2017\"").Histo2D({"hist_bnotag_2017", "hist_bnotag_2017;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "notagbjet_pt", "notagbjet_eta");

    auto hist_ctight_2017 = df1.Filter("sample_year == \"2017\"").Histo2D({"hist_ctight_2017", "hist_ctight_2017;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "tightcjet_pt", "tightcjet_eta");
    auto hist_cloose_2017 = df1.Filter("sample_year == \"2017\"").Histo2D({"hist_cloose_2017", "hist_cloose_2017;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "loosecjet_pt", "loosecjet_eta");
    auto hist_cnotag_2017 = df1.Filter("sample_year == \"2017\"").Histo2D({"hist_cnotag_2017", "hist_cnotag_2017;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "notagcjet_pt", "notagcjet_eta");

    auto hist_ltight_2017 = df1.Filter("sample_year == \"2017\"").Histo2D({"hist_ltight_2017", "hist_ltight_2017;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "tightljet_pt", "tightljet_eta");
    auto hist_lloose_2017 = df1.Filter("sample_year == \"2017\"").Histo2D({"hist_lloose_2017", "hist_lloose_2017;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "looseljet_pt", "looseljet_eta");
    auto hist_lnotag_2017 = df1.Filter("sample_year == \"2017\"").Histo2D({"hist_lnotag_2017", "hist_lnotag_2017;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "notagljet_pt", "notagljet_eta");
    
    auto hist_total_2017 = df1.Filter("sample_year == \"2017\"").Histo2D({"hist_total_2017", "hist_total_2017;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "Jet_pt", "Jet_eta").GetValue();

    hist_btight_2017->Divide(&hist_total_2017);
    hist_bloose_2017->Divide(&hist_total_2017);
    hist_bnotag_2017->Divide(&hist_total_2017);

    hist_ctight_2017->Divide(&hist_total_2017);
    hist_cloose_2017->Divide(&hist_total_2017);
    hist_cnotag_2017->Divide(&hist_total_2017);

    hist_ltight_2017->Divide(&hist_total_2017);
    hist_lloose_2017->Divide(&hist_total_2017);
    hist_lnotag_2017->Divide(&hist_total_2017);

    hist_btight_2017->Write();
    hist_bloose_2017->Write();
    hist_bnotag_2017->Write();

    hist_ctight_2017->Write();
    hist_cloose_2017->Write();
    hist_cnotag_2017->Write();

    hist_ltight_2017->Write();
    hist_lloose_2017->Write();
    hist_lnotag_2017->Write();

    auto hist_btight_2018 = df1.Filter("sample_year == \"2018\"").Histo2D({"hist_btight_2018", "hist_btight_2018;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "tightbjet_pt", "tightbjet_eta");    
    auto hist_bloose_2018 = df1.Filter("sample_year == \"2018\"").Histo2D({"hist_bloose_2018", "hist_bloose_2018;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "loosebjet_pt", "loosebjet_eta");
    auto hist_bnotag_2018 = df1.Filter("sample_year == \"2018\"").Histo2D({"hist_bnotag_2018", "hist_bnotag_2018;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "notagbjet_pt", "notagbjet_eta");

    auto hist_ctight_2018 = df1.Filter("sample_year == \"2018\"").Histo2D({"hist_ctight_2018", "hist_ctight_2018;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "tightcjet_pt", "tightcjet_eta");
    auto hist_cloose_2018 = df1.Filter("sample_year == \"2018\"").Histo2D({"hist_cloose_2018", "hist_cloose_2018;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "loosecjet_pt", "loosecjet_eta");
    auto hist_cnotag_2018 = df1.Filter("sample_year == \"2018\"").Histo2D({"hist_cnotag_2018", "hist_cnotag_2018;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "notagcjet_pt", "notagcjet_eta");

    auto hist_ltight_2018 = df1.Filter("sample_year == \"2018\"").Histo2D({"hist_ltight_2018", "hist_ltight_2018;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "tightljet_pt", "tightljet_eta");
    auto hist_lloose_2018 = df1.Filter("sample_year == \"2018\"").Histo2D({"hist_lloose_2018", "hist_lloose_2018;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "looseljet_pt", "looseljet_eta");
    auto hist_lnotag_2018 = df1.Filter("sample_year == \"2018\"").Histo2D({"hist_lnotag_2018", "hist_lnotag_2018;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "notagljet_pt", "notagljet_eta");

    auto hist_total_2018 = df1.Filter("sample_year == \"2018\"").Histo2D({"hist_total_2018", "hist_total_2018;jet_pt;jet_eta", 10, pt_bins, 4, eta_bins}, "Jet_pt", "Jet_eta").GetValue();

    hist_btight_2018->Divide(&hist_total_2018);
    hist_bloose_2018->Divide(&hist_total_2018);
    hist_bnotag_2018->Divide(&hist_total_2018);

    hist_ctight_2018->Divide(&hist_total_2018);
    hist_cloose_2018->Divide(&hist_total_2018);
    hist_cnotag_2018->Divide(&hist_total_2018);

    hist_ltight_2018->Divide(&hist_total_2018);
    hist_lloose_2018->Divide(&hist_total_2018);
    hist_lnotag_2018->Divide(&hist_total_2018);

    hist_btight_2018->Write();
    hist_bloose_2018->Write();
    hist_bnotag_2018->Write();

    hist_ctight_2018->Write();
    hist_cloose_2018->Write();
    hist_cnotag_2018->Write();

    hist_ltight_2018->Write();
    hist_lloose_2018->Write();
    hist_lnotag_2018->Write();
    

    f->Close();
    return 0;
}
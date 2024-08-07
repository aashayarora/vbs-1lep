#include <iostream>
#include <stdio.h>

#include "ROOT/RDataFrame.hxx"
#include "ROOT/RDFHelpers.hxx"

#include "weights.h"
#include "selections.h"
#include "corrections.h"
#include "utils.h"

#include "argparser.hpp"

struct MyArgs : public argparse::Args {
    std::string &spec = kwarg("i,input", "spec.json path");
    bool &cutflow = flag("cutflow", "print cutflow");
    bool &JEC = flag("jec", "JEC");
    bool &JER = flag("jer", "JER");
    bool &JMS = flag("jms", "JMS");
    bool &JMR = flag("jmr", "JMR");
    bool &METUnclustered = flag("met", "MET unclustered");
    int &nthreads = kwarg("n,nthreads", "number of threads").set_default(1);
    std::string &output = kwarg("o,output", "output root file").set_default("");
    std::string &variation = kwarg("var", "variation").set_default("nominal");
    std::string &JERvariation = kwarg("jervar", "JER variation").set_default("nominal");
    std::string &JEC_type = kwarg("jec_type", "JEC type").set_default("");
};

void runDataAnalysis(RNode df, MyArgs args, std::string output_file) {
    auto df1 = defineCorrectedCols(df);
    auto df_flags = flagSelections(df1);
    auto df_trigger = triggerSelections(df_flags);
    auto df_lepton = leptonSelections(df_trigger);
    auto df_higgs = higgsSelections(df_lepton);
    auto df_w = WZSelections(df_higgs);
    auto df_ak4 = AK4Selections(df_w);
    auto df_vbs = VBSJetsSelections(df_ak4);
    
    auto df_hem = HEMCorrection(df_vbs);
    auto df_golden = goodRun(LumiMask, df_hem);
    auto df_weights = df_golden.Define("weight", "goldenJSON * HEMweight");
    
    auto df_cut1 = df_weights.Filter("passesFlags", "PASSES MET FILTERS");
    auto df_cut2 = df_cut1.Filter("passesTriggers", "PASSES TRIGGERS");
    auto df_cut3 = df_cut2.Filter("((nVetoMuons == 1 && nTightMuons == 1 && nVetoElectrons == 0 && nTightElectrons == 0) || "
            "(nVetoMuons == 0 && nTightMuons == 0 && nVetoElectrons == 1 && nTightElectrons == 1)) && "
            "(GLepton_pt > 40)", "ONLY ONE LEPTON PT > 40");
    auto df_cut4 = df_cut3.Filter("HighestHScore > 0", "HIGGS CANDIDATE EXISTS");
    auto df_cut5 = df_cut4.Filter("HighestWjetScore > 0", "W CANDIDATE EXISTS");
    auto df_cut6 = df_cut5.Filter("Sum(ak4FromBJet) == 0", "NO TIGHT B-TAGGED AK4 JETS");
    auto df_cut7 = df_cut6.Filter("Sum(goodVBSJets) >= 2", "AT LEAST TWO VBS JETS");
    auto df_cut8 = df_cut7.Filter("ST > 1000", "ST > 1000");
    auto df_cut9 = df_cut8.Filter("HighestHScore > 0.5", "HIGGS SCORE > 0.5");
    auto df_cut10 = df_cut9.Filter("HighestWjetScore > 0.7", "WJET SCORE > 0.7");
    auto df_cut9_cr = df_cut8.Filter("HighestHScore < 0.95", "HIGGS SCORE < 0.95");
    auto df_cut10_cr = df_cut9.Filter("HighestWjetScore < 0.7", "WJET SCORE < 0.7");

    auto cf_weights = df_weights.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_flags = df_cut1.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_trigger = df_cut2.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_lepton = df_cut3.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_higgs = df_cut4.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_w = df_cut5.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_ak4 = df_cut6.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_vbs = df_cut7.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_defs = df_cut8.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_hbb = df_cut9.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_hbb_cr = df_cut9_cr.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_vqq_cr = df_cut10_cr.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");

    auto df_snap = df_weights.Define("passCut1", "passesFlags && passesTriggers")
        .Define("passCut2", "passCut1 && ((nVetoMuons == 1 && nTightMuons == 1 && nVetoElectrons == 0 && nTightElectrons == 0) || "
            "(nVetoMuons == 0 && nTightMuons == 0 && nVetoElectrons == 1 && nTightElectrons == 1)) && "
            "(GLepton_pt > 40)")
        .Define("passCut3", "passCut2 && HighestHScore > 0")
        .Define("passCut4", "passCut3 && HighestWjetScore > 0")
        .Define("passCut5", "passCut4 && Sum(ak4FromBJet) == 0")
        .Define("passCut6", "passCut5 && Sum(goodVBSJets) >= 2")
        .Define("passCut7", "passCut6 && ST > 1000")
        .Define("passCut8", "passCut7 && HighestHScore > 0.5")
        .Define("passCut9", "passCut8 && HighestWjetScore > 0.7")
        .Define("passCut8_cr", "passCut7 && HighestHScore < 0.95")
        .Define("passCut9_cr", "passCut8_cr && HighestWjetScore < 0.7")    
        .Filter("passCut9");
    
    saveSnapshot(df_snap, std::string(output_file), true);

    if (args.cutflow){
        auto h_weight = cf_weights.GetValue(); h_weight.Sumw2();
        auto h_flags = cf_flags.GetValue(); h_flags.Sumw2();
        auto h_trigger = cf_trigger.GetValue(); h_trigger.Sumw2();
        auto h_lepton = cf_lepton.GetValue(); h_lepton.Sumw2();
        auto h_higgs = cf_higgs.GetValue(); h_higgs.Sumw2();
        auto h_w = cf_w.GetValue(); h_w.Sumw2();
        auto h_ak4 = cf_ak4.GetValue(); h_ak4.Sumw2();
        auto h_vbs = cf_vbs.GetValue(); h_vbs.Sumw2();
        auto h_defs = cf_defs.GetValue(); h_defs.Sumw2();
        auto h_hbb = cf_hbb.GetValue(); h_hbb.Sumw2();

        auto h_hbb_cr = cf_hbb_cr.GetValue(); h_hbb_cr.Sumw2();
        auto h_vqq_cr = cf_vqq_cr.GetValue(); h_vqq_cr.Sumw2();

        printf("AFTER GOLDENJSON: %.2f \\pm %.2f\n", h_weight.Integral(), h_weight.GetBinError(1));
        printf("AFTER MET FILTERS: %.2f \\pm %.2f\n", h_flags.Integral(), h_flags.GetBinError(1));
        printf("AFTER TRIGGER: %.2f \\pm %.2f\n", h_trigger.Integral(), h_trigger.GetBinError(1));
        printf("AFTER LEPTON: %.2f \\pm %.2f\n", h_lepton.Integral(), h_lepton.GetBinError(1));
        printf("AFTER HIGGS: %.2f \\pm %.2f\n", h_higgs.Integral(), h_higgs.GetBinError(1));
        printf("AFTER WZ: %.2f \\pm %.2f\n", h_w.Integral(), h_w.GetBinError(1));
        printf("AFTER AK4: %.2f \\pm %.2f\n", h_ak4.Integral(), h_ak4.GetBinError(1));
        printf("AFTER VBS: %.2f \\pm %.2f\n", h_vbs.Integral(), h_vbs.GetBinError(1));
        printf("AFTER ST: %.2f \\pm %.2f\n", h_defs.Integral(), h_defs.GetBinError(1));
        printf("AFTER HBB: %.2f \\pm %.2f\n", h_hbb.Integral(), h_hbb.GetBinError(1));

        printf("CR\n");
        printf("AFTER HBB: %.2f \\pm %.2f\n", h_hbb_cr.Integral(), h_hbb_cr.GetBinError(1));
        printf("AFTER VQQ: %.2f \\pm %.2f\n", h_vqq_cr.Integral(), h_vqq_cr.GetBinError(1));
    }
}

void runBkgAnalysis(RNode df, MyArgs args, std::string output_file) {
    auto df1 = defineCorrectedCols(df);
    auto df_flags = flagSelections(df1);
    auto df_trigger = triggerSelections(df_flags);
    auto df_lepton = leptonSelections(df_trigger);
    auto df_higgs = higgsSelections(df_lepton);
    auto df_w = WZSelections(df_higgs);
    auto df_ak4 = AK4Selections(df_w);
    auto df_vbs = VBSJetsSelections(df_ak4);
    
    auto df_hem = HEMCorrection(df_vbs);
    auto df_l1prefire = L1PreFiringWeight(df_hem);
    auto df_ewk = EWKCorrections(cset_ewk, df_l1prefire);
    auto df_pileup = pileupScaleFactors(cset_pileup_2016preVFP, cset_pileup_2016postVFP, cset_pileup_2017, cset_pileup_2018, df_ewk);
    auto df_pileupID = pileupIDScaleFactors(cset_pileupID_2016preVFP, cset_pileupID_2016postVFP, cset_pileupID_2017, cset_pileupID_2018, df_pileup);
    // muon sf
    auto df_muon_ID = muonScaleFactors_ID(cset_muon_ID_2016preVFP, cset_muon_ID_2016postVFP, cset_muon_ID_2017, cset_muon_ID_2018, df_pileupID);
    auto df_muon_trigger = muonScaleFactors_trigger(cset_muon_ID_2016preVFP, cset_muon_ID_2016postVFP, cset_muon_ID_2017, cset_muon_ID_2018, df_muon_ID);
    auto df_muon_ttHID = muonScaleFactors_ttHID(cset_muon_ttH, df_muon_trigger);
    auto df_muon_ttHISO = muonScaleFactors_ttHISO(cset_muon_ttH, df_muon_ttHID);
    // elec sf
    auto df_elec_Reco = electronScaleFactors_Reco(cset_electron_ID_2016preVFP, cset_electron_ID_2016postVFP, cset_electron_ID_2017, cset_electron_ID_2018, df_muon_ttHISO);
    auto df_elec_ID = electronScaleFactors_ID(cset_electron_ID_2016preVFP, cset_electron_ID_2016postVFP, cset_electron_ID_2017, cset_electron_ID_2018, df_elec_Reco);
    auto df_elec_ttHID = electronScaleFactors_ttHID(cset_electron_ttH, df_elec_ID);
    auto df_elec_ttHISO = electronScaleFactors_ttHISO(cset_electron_ttH, df_elec_ttHID);
    auto df_elec_trigger = electronScaleFactors_Trigger(cset_electron_trigger, df_elec_ttHISO);
    // particle net
    auto df_pnet_w_2016preVFP = PNET_W_ScaleFactors_2016preVFP(cset_pnet_w, df_elec_trigger);
    auto df_pnet_w_2016postVFP = PNET_W_ScaleFactors_2016postVFP(cset_pnet_w, df_pnet_w_2016preVFP);
    auto df_pnet_w_2017 = PNET_W_ScaleFactors_2017(cset_pnet_w, df_pnet_w_2016postVFP);
    auto df_pnet_w = PNET_W_ScaleFactors_2018(cset_pnet_w, df_pnet_w_2017);
    auto df_pnet_h_2016preVFP = PNET_H_ScaleFactors_2016preVFP(cset_pnet_h, df_pnet_w);
    auto df_pnet_h_2016postVFP = PNET_H_ScaleFactors_2016postVFP(cset_pnet_h, df_pnet_h_2016preVFP);
    auto df_pnet_h_2017 = PNET_H_ScaleFactors_2017(cset_pnet_h, df_pnet_h_2016postVFP);
    auto df_pnet_h = PNET_H_ScaleFactors_2018(cset_pnet_h, df_pnet_h_2017);
    // btagging sf
    auto df_btag = df_pnet_h.Define("LnTBJet_hadronFlavour", "Jet_hadronFlavour[LnTBJets]");
    auto df_btag_hf = bTaggingScaleFactors_HF(cset_btag_2016preVFP, cset_btag_2016postVFP, cset_btag_2017, cset_btag_2018, cset_btag_eff, df_btag);
    auto df_btag_lf = bTaggingScaleFactors_LF(cset_btag_2016preVFP, cset_btag_2016postVFP, cset_btag_2017, cset_btag_2018, cset_btag_eff, df_btag_hf);
    
    auto df_psweight_fsr = PSWeight_FSR(df_btag_lf);
    auto df_psweight_isr = PSWeight_ISR(df_psweight_fsr);
    auto df_lhescaleweight_muf = LHEScaleWeight_muF(df_psweight_isr);
    auto df_lhescaleweight_mur = LHEScaleWeight_muR(df_lhescaleweight_muf);

    auto df_pdf_lheweight = LHEWeights_pdf(df_lhescaleweight_mur);

    auto df_weights = df_pdf_lheweight.Define("weight", 
        "pileup_weight[0] * "
        "pileupid_weight[0] * "
        "L1PreFiringWeight[0] * "
        "muon_scale_factors_ID[0] * "
        "muon_scale_factors_trigger[0] * "
        "muon_scale_factors_ttHID[0] * "
        "muon_scale_factors_ttHISO[0] * "
        "electron_scale_factors_Reco[0] * "
        "electron_scale_factors_ID[0] * "
        "electron_scale_factors_ttHID[0] * "
        "electron_scale_factors_ttHISO[0] * "
        "electron_scale_factors_trigger[0] * "
        "particlenet_w_weight_2016preVFP[0] * "
        "particlenet_w_weight_2016postVFP[0] * "
        "particlenet_w_weight_2017[0] * "
        "particlenet_w_weight_2018[0] * "
        "particlenet_h_weight_2016preVFP[0] * "
        "particlenet_h_weight_2016postVFP[0] * "
        "particlenet_h_weight_2017[0] * "
        "particlenet_h_weight_2018[0] * "
        "btagging_scale_factors_HF[0] * "
        "btagging_scale_factors_LF[0] * "
        "PSWeight_ISR[0] * "
        "PSWeight_FSR[0] * "
        "LHEScaleWeight_muR[0] * "
        "LHEScaleWeight_muF[0] * "
        "LHEWeights_pdf[0] * " // contains genWeights
        "HEMweight * "
        "EWKCorrection *"
        "xsec_weight");

    auto df_cut1 = df_weights.Filter("passesFlags", "PASSES MET FILTERS");
    auto df_cut2 = df_cut1.Filter("passesTriggers", "PASSES TRIGGERS");
    auto df_cut3 = df_cut2.Filter("((nVetoMuons == 1 && nTightMuons == 1 && nVetoElectrons == 0 && nTightElectrons == 0) || "
            "(nVetoMuons == 0 && nTightMuons == 0 && nVetoElectrons == 1 && nTightElectrons == 1)) && "
            "(GLepton_pt > 40)", "ONLY ONE LEPTON PT > 40");
    auto df_cut4 = df_cut3.Filter("HighestHScore > 0", "HIGGS CANDIDATE EXISTS");
    auto df_cut5 = df_cut4.Filter("HighestWjetScore > 0", "W CANDIDATE EXISTS");
    auto df_cut6 = df_cut5.Filter("Sum(ak4FromBJet) == 0", "NO TIGHT B-TAGGED AK4 JETS");
    auto df_cut7 = df_cut6.Filter("Sum(goodVBSJets) >= 2", "AT LEAST TWO VBS JETS");
    auto df_cut8 = df_cut7.Filter("ST > 1000", "ST > 1000");
    auto df_cut9 = df_cut8.Filter("HighestHScore > 0.5", "HIGGS SCORE > 0.5");
    auto df_cut10 = df_cut9.Filter("HighestWjetScore > 0.7", "WJET SCORE > 0.7");

    auto cf_xsec = df_weights.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_flags = df_cut1.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_trigger = df_cut2.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_lepton = df_cut3.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_higgs = df_cut4.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_w = df_cut5.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_ak4 = df_cut6.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_vbs = df_cut7.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_defs = df_cut8.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_hbb = df_cut9.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_vqq = df_cut10.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");

    auto cf_xsec_ttbar = df_weights.Filter("sample_type == \"ttbar\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_flags_ttbar = df_cut1.Filter("sample_type == \"ttbar\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_trigger_ttbar = df_cut2.Filter("sample_type == \"ttbar\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_lepton_ttbar = df_cut3.Filter("sample_type == \"ttbar\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_higgs_ttbar = df_cut4.Filter("sample_type == \"ttbar\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_w_ttbar = df_cut5.Filter("sample_type == \"ttbar\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_ak4_ttbar = df_cut6.Filter("sample_type == \"ttbar\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_vbs_ttbar = df_cut7.Filter("sample_type == \"ttbar\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_defs_ttbar = df_cut8.Filter("sample_type == \"ttbar\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_hbb_ttbar = df_cut9.Filter("sample_type == \"ttbar\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_vqq_ttbar = df_cut10.Filter("sample_type == \"ttbar\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");

    auto cf_xsec_ttx = df_weights.Filter("sample_type == \"ttx\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_flags_ttx = df_cut1.Filter("sample_type == \"ttx\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_trigger_ttx = df_cut2.Filter("sample_type == \"ttx\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_lepton_ttx = df_cut3.Filter("sample_type == \"ttx\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_higgs_ttx = df_cut4.Filter("sample_type == \"ttx\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_w_ttx = df_cut5.Filter("sample_type == \"ttx\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_ak4_ttx = df_cut6.Filter("sample_type == \"ttx\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_vbs_ttx = df_cut7.Filter("sample_type == \"ttx\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_defs_ttx = df_cut8.Filter("sample_type == \"ttx\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_hbb_ttx = df_cut9.Filter("sample_type == \"ttx\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_vqq_ttx = df_cut10.Filter("sample_type == \"ttx\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");

    auto cf_xsec_wjet = df_weights.Filter("sample_type == \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_flags_wjet = df_cut1.Filter("sample_type == \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_trigger_wjet = df_cut2.Filter("sample_type == \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_lepton_wjet = df_cut3.Filter("sample_type == \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_higgs_wjet = df_cut4.Filter("sample_type == \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_w_wjet = df_cut5.Filter("sample_type == \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_ak4_wjet = df_cut6.Filter("sample_type == \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_vbs_wjet = df_cut7.Filter("sample_type == \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_defs_wjet = df_cut8.Filter("sample_type == \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_hbb_wjet = df_cut9.Filter("sample_type == \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_vqq_wjet = df_cut10.Filter("sample_type == \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");

    auto cf_xsec_other = df_weights.Filter("sample_type != \"ttbar\" && sample_type != \"ttx\" && sample_type != \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_flags_other = df_cut1.Filter("sample_type != \"ttbar\" && sample_type != \"ttx\" && sample_type != \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_trigger_other = df_cut2.Filter("sample_type != \"ttbar\" && sample_type != \"ttx\" && sample_type != \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_lepton_other = df_cut3.Filter("sample_type != \"ttbar\" && sample_type != \"ttx\" && sample_type != \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_higgs_other = df_cut4.Filter("sample_type != \"ttbar\" && sample_type != \"ttx\" && sample_type != \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_w_other = df_cut5.Filter("sample_type != \"ttbar\" && sample_type != \"ttx\" && sample_type != \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_ak4_other = df_cut6.Filter("sample_type != \"ttbar\" && sample_type != \"ttx\" && sample_type != \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_vbs_other = df_cut7.Filter("sample_type != \"ttbar\" && sample_type != \"ttx\" && sample_type != \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_defs_other = df_cut8.Filter("sample_type != \"ttbar\" && sample_type != \"ttx\" && sample_type != \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_hbb_other = df_cut9.Filter("sample_type != \"ttbar\" && sample_type != \"ttx\" && sample_type != \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_vqq_other = df_cut10.Filter("sample_type != \"ttbar\" && sample_type != \"ttx\" && sample_type != \"WJets\"").Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");

    auto df_snap = df_weights.Define("passCut1", "passesFlags && passesTriggers")
        .Define("passCut2", "passCut1 && ((nVetoMuons == 1 && nTightMuons == 1 && nVetoElectrons == 0 && nTightElectrons == 0) || "
            "(nVetoMuons == 0 && nTightMuons == 0 && nVetoElectrons == 1 && nTightElectrons == 1)) && "
            "(GLepton_pt > 40)")
        .Define("passCut3", "passCut2 && HighestHScore > 0")
        .Define("passCut4", "passCut3 && HighestWjetScore > 0")
        .Define("passCut5", "passCut4 && Sum(ak4FromBJet) == 0")
        .Define("passCut6", "passCut5 && Sum(goodVBSJets) >= 2")
        .Define("passCut7", "passCut6 && ST > 1000")
        .Define("passCut8", "passCut7 && HighestHScore > 0.5")
        .Define("passCut9", "passCut8 && HighestWjetScore > 0.7")
        .Define("passCut8_cr", "passCut7 && HighestHScore < 0.95")
        .Define("passCut9_cr", "passCut8_cr && HighestWjetScore < 0.7")    
        .Filter("passCut9");
    
    saveSnapshot(df_snap, std::string(output_file));

    if (args.cutflow){
        auto h_weight = cf_xsec.GetValue(); h_weight.Sumw2();
        auto h_trigger = cf_trigger.GetValue(); h_trigger.Sumw2();
        auto h_lepton = cf_lepton.GetValue(); h_lepton.Sumw2();
        auto h_higgs = cf_higgs.GetValue(); h_higgs.Sumw2();
        auto h_w = cf_w.GetValue(); h_w.Sumw2();
        auto h_ak4 = cf_ak4.GetValue(); h_ak4.Sumw2();
        auto h_vbs = cf_vbs.GetValue(); h_vbs.Sumw2();
        auto h_defs = cf_defs.GetValue(); h_defs.Sumw2();
        auto h_hbb = cf_hbb.GetValue(); h_hbb.Sumw2();
        auto h_vqq = cf_vqq.GetValue(); h_vqq.Sumw2();

        auto h_weight_ttbar = cf_xsec_ttbar.GetValue(); h_weight_ttbar.Sumw2();
        auto h_trigger_ttbar = cf_trigger_ttbar.GetValue(); h_trigger_ttbar.Sumw2();
        auto h_lepton_ttbar = cf_lepton_ttbar.GetValue(); h_lepton_ttbar.Sumw2();
        auto h_higgs_ttbar = cf_higgs_ttbar.GetValue(); h_higgs_ttbar.Sumw2();
        auto h_w_ttbar = cf_w_ttbar.GetValue(); h_w_ttbar.Sumw2();
        auto h_ak4_ttbar = cf_ak4_ttbar.GetValue(); h_ak4_ttbar.Sumw2();
        auto h_vbs_ttbar = cf_vbs_ttbar.GetValue(); h_vbs_ttbar.Sumw2();
        auto h_defs_ttbar = cf_defs_ttbar.GetValue(); h_defs_ttbar.Sumw2();
        auto h_hbb_ttbar = cf_hbb_ttbar.GetValue(); h_hbb_ttbar.Sumw2();
        auto h_vqq_ttbar = cf_vqq_ttbar.GetValue(); h_vqq_ttbar.Sumw2();

        auto h_weight_ttx = cf_xsec_ttx.GetValue(); h_weight_ttx.Sumw2();
        auto h_trigger_ttx = cf_trigger_ttx.GetValue(); h_trigger_ttx.Sumw2();
        auto h_lepton_ttx = cf_lepton_ttx.GetValue(); h_lepton_ttx.Sumw2();
        auto h_higgs_ttx = cf_higgs_ttx.GetValue(); h_higgs_ttx.Sumw2();
        auto h_w_ttx = cf_w_ttx.GetValue(); h_w_ttx.Sumw2();
        auto h_ak4_ttx = cf_ak4_ttx.GetValue(); h_ak4_ttx.Sumw2();
        auto h_vbs_ttx = cf_vbs_ttx.GetValue(); h_vbs_ttx.Sumw2();
        auto h_defs_ttx = cf_defs_ttx.GetValue(); h_defs_ttx.Sumw2();
        auto h_hbb_ttx = cf_hbb_ttx.GetValue(); h_hbb_ttx.Sumw2();
        auto h_vqq_ttx = cf_vqq_ttx.GetValue(); h_vqq_ttx.Sumw2();

        auto h_weight_wjet = cf_xsec_wjet.GetValue(); h_weight_wjet.Sumw2();
        auto h_trigger_wjet = cf_trigger_wjet.GetValue(); h_trigger_wjet.Sumw2();
        auto h_lepton_wjet = cf_lepton_wjet.GetValue(); h_lepton_wjet.Sumw2();
        auto h_higgs_wjet = cf_higgs_wjet.GetValue(); h_higgs_wjet.Sumw2();
        auto h_w_wjet = cf_w_wjet.GetValue(); h_w_wjet.Sumw2();
        auto h_ak4_wjet = cf_ak4_wjet.GetValue(); h_ak4_wjet.Sumw2();
        auto h_vbs_wjet = cf_vbs_wjet.GetValue(); h_vbs_wjet.Sumw2();
        auto h_defs_wjet = cf_defs_wjet.GetValue(); h_defs_wjet.Sumw2();
        auto h_hbb_wjet = cf_hbb_wjet.GetValue(); h_hbb_wjet.Sumw2();
        auto h_vqq_wjet = cf_vqq_wjet.GetValue(); h_vqq_wjet.Sumw2();

        auto h_weight_other = cf_xsec_other.GetValue(); h_weight_other.Sumw2();
        auto h_trigger_other = cf_trigger_other.GetValue(); h_trigger_other.Sumw2();
        auto h_lepton_other = cf_lepton_other.GetValue(); h_lepton_other.Sumw2();
        auto h_higgs_other = cf_higgs_other.GetValue(); h_higgs_other.Sumw2();
        auto h_w_other = cf_w_other.GetValue(); h_w_other.Sumw2();
        auto h_ak4_other = cf_ak4_other.GetValue(); h_ak4_other.Sumw2();
        auto h_vbs_other = cf_vbs_other.GetValue(); h_vbs_other.Sumw2();
        auto h_defs_other = cf_defs_other.GetValue(); h_defs_other.Sumw2();
        auto h_hbb_other = cf_hbb_other.GetValue(); h_hbb_other.Sumw2();
        auto h_vqq_other = cf_vqq_other.GetValue(); h_vqq_other.Sumw2();

        printf("AFTER XSEC: %.2f \\pm %.2f\n", h_weight.Integral(), h_weight.GetBinError(1));
        printf("AFTER TRIGGER: %.2f \\pm %.2f\n", h_trigger.Integral(), h_trigger.GetBinError(1));
        printf("AFTER LEPTON: %.2f \\pm %.2f\n", h_lepton.Integral(), h_lepton.GetBinError(1));
        printf("AFTER HIGGS: %.2f \\pm %.2f\n", h_higgs.Integral(), h_higgs.GetBinError(1));
        printf("AFTER WZ: %.2f \\pm %.2f\n", h_w.Integral(), h_w.GetBinError(1));
        printf("AFTER AK4: %.2f \\pm %.2f\n", h_ak4.Integral(), h_ak4.GetBinError(1));
        printf("AFTER VBS: %.2f \\pm %.2f\n", h_vbs.Integral(), h_vbs.GetBinError(1));
        printf("AFTER ST: %.2f \\pm %.2f\n", h_defs.Integral(), h_defs.GetBinError(1));
        printf("AFTER HBB: %.2f \\pm %.2f\n", h_hbb.Integral(), h_hbb.GetBinError(1));
        printf("AFTER VQQ: %.2f \\pm %.2f\n", h_vqq.Integral(), h_vqq.GetBinError(1));

        printf("TTBAR\n");
        printf("AFTER XSEC: %.2f \\pm %.2f\n", h_weight_ttbar.Integral(), h_weight_ttbar.GetBinError(1));
        printf("AFTER TRIGGER: %.2f \\pm %.2f\n", h_trigger_ttbar.Integral(), h_trigger_ttbar.GetBinError(1));
        printf("AFTER LEPTON: %.2f \\pm %.2f\n", h_lepton_ttbar.Integral(), h_lepton_ttbar.GetBinError(1));
        printf("AFTER HIGGS: %.2f \\pm %.2f\n", h_higgs_ttbar.Integral(), h_higgs_ttbar.GetBinError(1));
        printf("AFTER WZ: %.2f \\pm %.2f\n", h_w_ttbar.Integral(), h_w_ttbar.GetBinError(1));
        printf("AFTER AK4: %.2f \\pm %.2f\n", h_ak4_ttbar.Integral(), h_ak4_ttbar.GetBinError(1));
        printf("AFTER VBS: %.2f \\pm %.2f\n", h_vbs_ttbar.Integral(), h_vbs_ttbar.GetBinError(1));
        printf("AFTER ST: %.2f \\pm %.2f\n", h_defs_ttbar.Integral(), h_defs_ttbar.GetBinError(1));
        printf("AFTER HBB: %.2f \\pm %.2f\n", h_hbb_ttbar.Integral(), h_hbb_ttbar.GetBinError(1));
        printf("AFTER VQQ: %.2f \\pm %.2f\n", h_vqq_ttbar.Integral(), h_vqq_ttbar.GetBinError(1));

        printf("TTX\n");
        printf("AFTER XSEC: %.2f \\pm %.2f\n", h_weight_ttx.Integral(), h_weight_ttx.GetBinError(1));
        printf("AFTER TRIGGER: %.2f \\pm %.2f\n", h_trigger_ttx.Integral(), h_trigger_ttx.GetBinError(1));
        printf("AFTER LEPTON: %.2f \\pm %.2f\n", h_lepton_ttx.Integral(), h_lepton_ttx.GetBinError(1));
        printf("AFTER HIGGS: %.2f \\pm %.2f\n", h_higgs_ttx.Integral(), h_higgs_ttx.GetBinError(1));
        printf("AFTER WZ: %.2f \\pm %.2f\n", h_w_ttx.Integral(), h_w_ttx.GetBinError(1));
        printf("AFTER AK4: %.2f \\pm %.2f\n", h_ak4_ttx.Integral(), h_ak4_ttx.GetBinError(1));
        printf("AFTER VBS: %.2f \\pm %.2f\n", h_vbs_ttx.Integral(), h_vbs_ttx.GetBinError(1));
        printf("AFTER ST: %.2f \\pm %.2f\n", h_defs_ttx.Integral(), h_defs_ttx.GetBinError(1));
        printf("AFTER HBB: %.2f \\pm %.2f\n", h_hbb_ttx.Integral(), h_hbb_ttx.GetBinError(1));
        printf("AFTER VQQ: %.2f \\pm %.2f\n", h_vqq_ttx.Integral(), h_vqq_ttx.GetBinError(1));

        printf("WJETS\n");
        printf("AFTER XSEC: %.2f \\pm %.2f\n", h_weight_wjet.Integral(), h_weight_wjet.GetBinError(1));
        printf("AFTER TRIGGER: %.2f \\pm %.2f\n", h_trigger_wjet.Integral(), h_trigger_wjet.GetBinError(1));
        printf("AFTER LEPTON: %.2f \\pm %.2f\n", h_lepton_wjet.Integral(), h_lepton_wjet.GetBinError(1));
        printf("AFTER HIGGS: %.2f \\pm %.2f\n", h_higgs_wjet.Integral(), h_higgs_wjet.GetBinError(1));
        printf("AFTER WZ: %.2f \\pm %.2f\n", h_w_wjet.Integral(), h_w_wjet.GetBinError(1));
        printf("AFTER AK4: %.2f \\pm %.2f\n", h_ak4_wjet.Integral(), h_ak4_wjet.GetBinError(1));
        printf("AFTER VBS: %.2f \\pm %.2f\n", h_vbs_wjet.Integral(), h_vbs_wjet.GetBinError(1));
        printf("AFTER ST: %.2f \\pm %.2f\n", h_defs_wjet.Integral(), h_defs_wjet.GetBinError(1));
        printf("AFTER HBB: %.2f \\pm %.2f\n", h_hbb_wjet.Integral(), h_hbb_wjet.GetBinError(1));
        printf("AFTER VQQ: %.2f \\pm %.2f\n", h_vqq_wjet.Integral(), h_vqq_wjet.GetBinError(1));

        printf("OTHER\n");
        printf("AFTER XSEC: %.2f \\pm %.2f\n", h_weight_other.Integral(), h_weight_other.GetBinError(1));
        printf("AFTER TRIGGER: %.2f \\pm %.2f\n", h_trigger_other.Integral(), h_trigger_other.GetBinError(1));
        printf("AFTER LEPTON: %.2f \\pm %.2f\n", h_lepton_other.Integral(), h_lepton_other.GetBinError(1));
        printf("AFTER HIGGS: %.2f \\pm %.2f\n", h_higgs_other.Integral(), h_higgs_other.GetBinError(1));
        printf("AFTER WZ: %.2f \\pm %.2f\n", h_w_other.Integral(), h_w_other.GetBinError(1));
        printf("AFTER AK4: %.2f \\pm %.2f\n", h_ak4_other.Integral(), h_ak4_other.GetBinError(1));
        printf("AFTER VBS: %.2f \\pm %.2f\n", h_vbs_other.Integral(), h_vbs_other.GetBinError(1));
        printf("AFTER ST: %.2f \\pm %.2f\n", h_defs_other.Integral(), h_defs_other.GetBinError(1));
        printf("AFTER HBB: %.2f \\pm %.2f\n", h_hbb_other.Integral(), h_hbb_other.GetBinError(1));
        printf("AFTER VQQ: %.2f \\pm %.2f\n", h_vqq_other.Integral(), h_vqq_other.GetBinError(1));
    }
}

void runSigAnalysis(RNode df, MyArgs args, std::string output_file) {
    // corrections
    auto df1 = defineCorrectedCols(df);
    auto applypreCorrections = [] (RNode df, MyArgs args) {
        if (args.JER) {return JetEnergyResolution(cset_jerc_2016preVFP, cset_jerc_2016postVFP, cset_jerc_2017, cset_jerc_2018, cset_jer_smear, df, args.JERvariation);}
        else if (args.METUnclustered) {return METUnclusteredCorrections(df, args.variation);}
        else if (args.JEC) {return JetEnergyCorrection(cset_jerc_2016preVFP, cset_jerc_2016postVFP, cset_jerc_2017, cset_jerc_2018, df, args.JEC_type, args.variation);}
        else {return df;}
    };

    auto df_precorr = applypreCorrections(df1, args);
    auto df_flags = flagSelections(df_precorr);
    auto df_trigger = triggerSelections(df_flags);
    auto df_lepton = leptonSelections(df_trigger);
    auto df_higgs = higgsSelections(df_lepton);
    auto df_w = WZSelections(df_higgs);
    auto df_ak4 = AK4Selections(df_w);
    auto df_vbs = VBSJetsSelections(df_ak4);

    auto applypostCorrections = [](RNode df, MyArgs args) { 
        if (args.JMS) {return JMS_Corrections(cset_jms, df, args.variation);} 
        else if (args.JMR) {return JMR_Corrections(cset_jmr, df, args.variation);} 
        else {return df;}
    };

    auto df_postcorr = applypostCorrections(df_vbs, args);
    
    auto df_hem = HEMCorrection(df_postcorr);
    auto df_l1prefire = L1PreFiringWeight(df_hem);
    auto df_ewk = EWKCorrections(cset_ewk, df_l1prefire);
    auto df_pileup = pileupScaleFactors(cset_pileup_2016preVFP, cset_pileup_2016postVFP, cset_pileup_2017, cset_pileup_2018, df_ewk);
    auto df_pileupID = pileupIDScaleFactors(cset_pileupID_2016preVFP, cset_pileupID_2016postVFP, cset_pileupID_2017, cset_pileupID_2018, df_pileup);
    // muon sf
    auto df_muon_ID = muonScaleFactors_ID(cset_muon_ID_2016preVFP, cset_muon_ID_2016postVFP, cset_muon_ID_2017, cset_muon_ID_2018, df_pileupID);
    auto df_muon_trigger = muonScaleFactors_trigger(cset_muon_ID_2016preVFP, cset_muon_ID_2016postVFP, cset_muon_ID_2017, cset_muon_ID_2018, df_muon_ID);
    auto df_muon_ttHID = muonScaleFactors_ttHID(cset_muon_ttH, df_muon_trigger);
    auto df_muon_ttHISO = muonScaleFactors_ttHISO(cset_muon_ttH, df_muon_ttHID);
    // elec sf
    auto df_elec_Reco = electronScaleFactors_Reco(cset_electron_ID_2016preVFP, cset_electron_ID_2016postVFP, cset_electron_ID_2017, cset_electron_ID_2018, df_muon_ttHISO);
    auto df_elec_ID = electronScaleFactors_ID(cset_electron_ID_2016preVFP, cset_electron_ID_2016postVFP, cset_electron_ID_2017, cset_electron_ID_2018, df_elec_Reco);
    auto df_elec_ttHID = electronScaleFactors_ttHID(cset_electron_ttH, df_elec_ID);
    auto df_elec_ttHISO = electronScaleFactors_ttHISO(cset_electron_ttH, df_elec_ttHID);
    auto df_elec_trigger = electronScaleFactors_Trigger(cset_electron_trigger, df_elec_ttHISO);
    // particle net
    auto df_pnet_w_2016preVFP = PNET_W_ScaleFactors_2016preVFP(cset_pnet_w, df_elec_trigger);
    auto df_pnet_w_2016postVFP = PNET_W_ScaleFactors_2016postVFP(cset_pnet_w, df_pnet_w_2016preVFP);
    auto df_pnet_w_2017 = PNET_W_ScaleFactors_2017(cset_pnet_w, df_pnet_w_2016postVFP);
    auto df_pnet_w = PNET_W_ScaleFactors_2018(cset_pnet_w, df_pnet_w_2017);
    auto df_pnet_h_2016preVFP = PNET_H_ScaleFactors_2016preVFP(cset_pnet_h, df_pnet_w);
    auto df_pnet_h_2016postVFP = PNET_H_ScaleFactors_2016postVFP(cset_pnet_h, df_pnet_h_2016preVFP);
    auto df_pnet_h_2017 = PNET_H_ScaleFactors_2017(cset_pnet_h, df_pnet_h_2016postVFP);
    auto df_pnet_h = PNET_H_ScaleFactors_2018(cset_pnet_h, df_pnet_h_2017);
    // btagging sf
    auto df_btag = df_pnet_h.Define("LnTBJet_hadronFlavour", "Jet_hadronFlavour[LnTBJets]");
    auto df_btag_hf = bTaggingScaleFactors_HF(cset_btag_2016preVFP, cset_btag_2016postVFP, cset_btag_2017, cset_btag_2018, cset_btag_eff, df_btag);
    auto df_btag_lf = bTaggingScaleFactors_LF(cset_btag_2016preVFP, cset_btag_2016postVFP, cset_btag_2017, cset_btag_2018, cset_btag_eff, df_btag_hf);
    
    auto df_psweight_fsr = PSWeight_FSR(df_btag_lf);
    auto df_psweight_isr = PSWeight_ISR(df_psweight_fsr);
    auto df_lhescaleweight_muf = LHEScaleWeight_muF(df_psweight_isr);
    auto df_lhescaleweight_mur = LHEScaleWeight_muR(df_lhescaleweight_muf);

    auto df_pdf_lheweight = LHEWeights_pdf(df_lhescaleweight_mur);

    auto df_weights = df_pdf_lheweight.Define("weight", 
        "pileup_weight[0] * "
        "pileupid_weight[0] * "
        "L1PreFiringWeight[0] * "
        "muon_scale_factors_ID[0] * "
        "muon_scale_factors_trigger[0] * "
        "muon_scale_factors_ttHID[0] * "
        "muon_scale_factors_ttHISO[0] * "
        "electron_scale_factors_Reco[0] * "
        "electron_scale_factors_ID[0] * "
        "electron_scale_factors_ttHID[0] * "
        "electron_scale_factors_ttHISO[0] * "
        "electron_scale_factors_trigger[0] * "
        "particlenet_w_weight_2016preVFP[0] * "
        "particlenet_w_weight_2016postVFP[0] * "
        "particlenet_w_weight_2017[0] * "
        "particlenet_w_weight_2018[0] * "
        "particlenet_h_weight_2016preVFP[0] * "
        "particlenet_h_weight_2016postVFP[0] * "
        "particlenet_h_weight_2017[0] * "
        "particlenet_h_weight_2018[0] * "
        "btagging_scale_factors_HF[0] * "
        "btagging_scale_factors_LF[0] * "
        "PSWeight_ISR[0] * "
        "PSWeight_FSR[0] * "
        "LHEScaleWeight_muR[0] * "
        "LHEScaleWeight_muF[0] * "
        "LHEWeights_pdf[0] * " // contains genWeights
        "HEMweight * "
        "EWKCorrection *"
        "xsec_weight");

    auto df_cut1 = df_weights.Filter("passesFlags", "PASSES MET FILTERS");
    auto df_cut2 = df_cut1.Filter("passesTriggers", "PASSES TRIGGERS");
    auto df_cut3 = df_cut2.Filter("((nVetoMuons == 1 && nTightMuons == 1 && nVetoElectrons == 0 && nTightElectrons == 0) || "
            "(nVetoMuons == 0 && nTightMuons == 0 && nVetoElectrons == 1 && nTightElectrons == 1)) && "
            "(GLepton_pt > 40)", "ONLY ONE LEPTON PT > 40");
    auto df_cut4 = df_cut3.Filter("HighestHScore > 0", "HIGGS CANDIDATE EXISTS");
    auto df_cut5 = df_cut4.Filter("HighestWjetScore > 0", "W CANDIDATE EXISTS");
    auto df_cut6 = df_cut5.Filter("Sum(ak4FromBJet) == 0", "NO TIGHT B-TAGGED AK4 JETS");
    auto df_cut7 = df_cut6.Filter("Sum(goodVBSJets) >= 2", "AT LEAST TWO VBS JETS");
    auto df_cut8 = df_cut7.Filter("ST > 1000", "ST > 1000");
    auto df_cut9 = df_cut8.Filter("HighestHScore > 0.5", "HIGGS SCORE > 0.5");
    auto df_cut10 = df_cut9.Filter("HighestWjetScore > 0.7", "WJET SCORE > 0.7");

    auto cf_xsec = df_weights.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_flags = df_cut1.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_trigger = df_cut2.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_lepton = df_cut3.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_higgs = df_cut4.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_w = df_cut5.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_ak4 = df_cut6.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_vbs = df_cut7.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_defs = df_cut8.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_hbb = df_cut9.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");
    auto cf_vqq = df_cut10.Histo1D({"h", "h", 1,-100, 100}, "weight", "weight");        

    auto df_snap = df_cut10.Define("passCut1", "passesFlags && passesTriggers")
        .Define("passCut2", "passCut1 && ((nVetoMuons == 1 && nTightMuons == 1 && nVetoElectrons == 0 && nTightElectrons == 0) || "
            "(nVetoMuons == 0 && nTightMuons == 0 && nVetoElectrons == 1 && nTightElectrons == 1)) && "
            "(GLepton_pt > 40)")
        .Define("passCut3", "passCut2 && HighestHScore > 0")
        .Define("passCut4", "passCut3 && HighestWjetScore > 0")
        .Define("passCut5", "passCut4 && Sum(ak4FromBJet) == 0")
        .Define("passCut6", "passCut5 && Sum(goodVBSJets) >= 2")
        .Define("passCut7", "passCut6 && ST > 1000")
        .Define("passCut8", "passCut7 && HighestHScore > 0.5")
        .Define("passCut9", "passCut8 && HighestWjetScore > 0.7")
        .Define("passCut8_cr", "passCut7 && HighestHScore < 0.95")
        .Define("passCut9_cr", "passCut8_cr && HighestWjetScore < 0.7")        
        .Filter("passCut9");
    
    saveSnapshot(df_snap, std::string(output_file));

    if (args.cutflow){
        auto h_weight = cf_xsec.GetValue(); h_weight.Sumw2();
        auto h_flags = cf_flags.GetValue(); h_flags.Sumw2();
        auto h_trigger = cf_trigger.GetValue(); h_trigger.Sumw2();
        auto h_lepton = cf_lepton.GetValue(); h_lepton.Sumw2();
        auto h_higgs = cf_higgs.GetValue(); h_higgs.Sumw2();
        auto h_w = cf_w.GetValue(); h_w.Sumw2();
        auto h_ak4 = cf_ak4.GetValue(); h_ak4.Sumw2();
        auto h_vbs = cf_vbs.GetValue(); h_vbs.Sumw2();
        auto h_defs = cf_defs.GetValue(); h_defs.Sumw2();
        auto h_hbb = cf_hbb.GetValue(); h_hbb.Sumw2();
        auto h_vqq = cf_vqq.GetValue(); h_vqq.Sumw2();

        printf("AFTER XSEC: %.2f \\pm %.2f\n", h_weight.Integral(), h_weight.GetBinError(1));
        printf("AFTER MET FILTERS: %.2f \\pm %.2f\n", h_flags.Integral(), h_flags.GetBinError(1));
        printf("AFTER TRIGGER: %.2f \\pm %.2f\n", h_trigger.Integral(), h_trigger.GetBinError(1));
        printf("AFTER LEPTON: %.2f \\pm %.2f\n", h_lepton.Integral(), h_lepton.GetBinError(1));
        printf("AFTER HIGGS: %.2f \\pm %.2f\n", h_higgs.Integral(), h_higgs.GetBinError(1));
        printf("AFTER WZ: %.2f \\pm %.2f\n", h_w.Integral(), h_w.GetBinError(1));
        printf("AFTER AK4: %.2f \\pm %.2f\n", h_ak4.Integral(), h_ak4.GetBinError(1));
        printf("AFTER VBS: %.2f \\pm %.2f\n", h_vbs.Integral(), h_vbs.GetBinError(1));
        printf("AFTER ST: %.2f \\pm %.2f\n", h_defs.Integral(), h_defs.GetBinError(1));
        printf("AFTER HBB: %.2f \\pm %.2f\n", h_hbb.Integral(), h_hbb.GetBinError(1));
        printf("AFTER VQQ: %.2f \\pm %.2f\n", h_vqq.Integral(), h_vqq.GetBinError(1));
    }
}

int main(int argc, char** argv) {
    auto args = argparse::parse<MyArgs>(argc, argv);
    std::string input_spec = args.spec;
    std::string output_file = args.output;

    ROOT::EnableImplicitMT(args.nthreads);
    ROOT::RDataFrame df_ = ROOT::RDF::Experimental::FromSpec(input_spec);
    ROOT::RDF::Experimental::AddProgressBar(df_);

    // define metadata
    auto df = defineMetadata(df_);
    // run analysis
    if (input_spec.find("data") != std::string::npos) {
        if (output_file.empty()) {
            output_file = "data";
        }
        runDataAnalysis(df, args, output_file);
    }
    else if (input_spec.find("bkg") != std::string::npos) {
        if (output_file.empty()) {
            output_file = "bkg";
        }
        runBkgAnalysis(df, args, output_file);
    }
    else if (input_spec.find("sig") != std::string::npos) {
        if (output_file.empty()) {
            output_file = "sig";
        }
        runSigAnalysis(df, args, output_file);
    }
    else {
        std::cerr << "Invalid input file, the spec file name must contain sig, bkg or data" << std::endl;
        return 1;
    }
    return 0;
}
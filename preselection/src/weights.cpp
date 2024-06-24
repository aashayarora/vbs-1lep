#include "weights.h"

#include <iostream>
using namespace std;

RNode goodRun(lumiMask golden, RNode df){
    auto goldenjson = [golden](unsigned int &run, unsigned int &luminosityBlock){return golden.accept(run, luminosityBlock);};
    return df.Define("goldenJSON", goldenjson, {"run", "luminosityBlock"})
             .Filter("goldenJSON", "PASSES GOLDEN JSON");
}

RNode pileupCorrection(correction::Correction::Ref cset_pileup_2016preVFP, correction::Correction::Ref cset_pileup_2016postVFP, correction::Correction::Ref cset_pileup_2017, correction::Correction::Ref cset_pileup_2018, RNode df){
    auto eval_correction = [cset_pileup_2016preVFP, cset_pileup_2016postVFP, cset_pileup_2017, cset_pileup_2018] (std::string year, float ntrueint) {
        RVec<double> sfs;
        if (year == "2016preVFP") {
            sfs.push_back(cset_pileup_2016preVFP->evaluate({ntrueint, "nominal"}));
            sfs.push_back(cset_pileup_2016preVFP->evaluate({ntrueint, "up"}));
            sfs.push_back(cset_pileup_2016preVFP->evaluate({ntrueint, "down"}));
        }
        if (year == "2016postVFP") {
            sfs.push_back(cset_pileup_2016postVFP->evaluate({ntrueint, "nominal"}));
            sfs.push_back(cset_pileup_2016postVFP->evaluate({ntrueint, "up"}));
            sfs.push_back(cset_pileup_2016postVFP->evaluate({ntrueint, "down"}));
        }
        if (year == "2017") {
            sfs.push_back(cset_pileup_2017->evaluate({ntrueint, "nominal"}));
            sfs.push_back(cset_pileup_2017->evaluate({ntrueint, "up"}));
            sfs.push_back(cset_pileup_2017->evaluate({ntrueint, "down"}));
        }
        if (year == "2018") {
            sfs.push_back(cset_pileup_2018->evaluate({ntrueint, "nominal"}));
            sfs.push_back(cset_pileup_2018->evaluate({ntrueint, "up"}));
            sfs.push_back(cset_pileup_2018->evaluate({ntrueint, "down"}));
        }
        return sfs;
    };
    return df.Define("pileup_weight", [](){return 1.0;}).Vary("pileup_weight", eval_correction, {"sample_year", "Pileup_nTrueInt"}, {"nominal", "up", "down"});
}

RNode pileupIDCorrection(correction::Correction::Ref cset_pileup_2016preVFP, correction::Correction::Ref cset_pileup_2016postVFP, correction::Correction::Ref cset_pileup_2017, correction::Correction::Ref cset_pileup_2018, RNode df){
    auto eval_correction = [cset_pileup_2016preVFP, cset_pileup_2016postVFP, cset_pileup_2017, cset_pileup_2018] (std::string year, RVec<float> eta, RVec<float> pt) {
        double pileup_weight = 1.;
        if (eta.size() == 0) {
            return pileup_weight;
        }
        for (size_t i = 0; i < eta.size(); i++) {
            if (year == "2016preVFP" && pt[i] < 50) {
                pileup_weight *= cset_pileup_2016preVFP->evaluate({eta[i], pt[i], "nom", "L"});
            }
            if (year == "2016postVFP" && pt[i] < 50) {
                pileup_weight *= cset_pileup_2016postVFP->evaluate({eta[i], pt[i], "nom", "L"});
            }
            if (year == "2017" && pt[i] < 50) {
                pileup_weight *= cset_pileup_2017->evaluate({eta[i], pt[i], "nom", "L"});
            }
            if (year == "2018" && pt[i] < 50) {
                pileup_weight *= cset_pileup_2018->evaluate({eta[i], pt[i], "nom", "L"});
            }
        }
        return pileup_weight;
    };
    return df.Define("pileupid_weight", eval_correction, {"sample_year", "VBSJets_eta", "VBSJets_pt"});
}

RNode muonScaleFactors_ID(correction::Correction::Ref cset_muon_2016preVFP, correction::Correction::Ref cset_muon_2016postVFP, correction::Correction::Ref cset_muon_2017, correction::Correction::Ref cset_muon_2018, RNode df){
    auto eval_correction = [cset_muon_2016preVFP, cset_muon_2016postVFP, cset_muon_2017, cset_muon_2018] (std::string year, const RVec<float> eta, const RVec<float> pt) {
        double muon_sf_weight = 1.;
        if (eta.size() == 0) {
            return muon_sf_weight;
        }
        for (size_t i = 0; i < eta.size(); i++) {
            if (year == "2016preVFP") {
                muon_sf_weight *= cset_muon_2016preVFP->evaluate({abs(eta[i]), pt[i], "nominal"});
            }
            if (year == "2016postVFP") {
                muon_sf_weight *= cset_muon_2016postVFP->evaluate({abs(eta[i]), pt[i], "nominal"});
            }
            if (year == "2017") {
                muon_sf_weight *= cset_muon_2017->evaluate({abs(eta[i]), pt[i], "nominal"});
            }
            if (year == "2018") {
                muon_sf_weight *= cset_muon_2018->evaluate({abs(eta[i]), pt[i], "nominal"});
            }
        }
        return muon_sf_weight;
    };
    return df.Define("muon_scale_factors_ID", eval_correction, {"sample_year", "GMuon_eta", "GMuon_pt"});
}

RNode muonScaleFactors_ttH(correction::Correction::Ref cset_muon_tth, RNode df, std::string sf_type){
    auto eval_correction = [cset_muon_tth] (std::string year, const RVec<float> eta, const RVec<float> pt) {
        double muon_sf_weight = 1.;
        if (eta.size() == 0) {
            return muon_sf_weight;
        }
        for (size_t i = 0; i < eta.size(); i++) {
            muon_sf_weight *= cset_muon_tth->evaluate({year, abs(eta[i]), pt[i]});
        }
        return muon_sf_weight;
    };
    return df.Define(sf_type, eval_correction, {"sample_year", "GMuon_eta", "GMuon_pt"});
}

RNode electronScaleFactors_ID(correction::Correction::Ref cset_electron_2016preVFP, correction::Correction::Ref cset_electron_2016postVFP, correction::Correction::Ref cset_electron_2017, correction::Correction::Ref cset_electron_2018, RNode df){
    auto eval_correction = [cset_electron_2016preVFP, cset_electron_2016postVFP, cset_electron_2017, cset_electron_2018] (std::string year, const RVec<float> eta, const RVec<float> pt) {
        double electron_sf_weight = 1.;
        if (eta.size() == 0) {
            return electron_sf_weight;
        }
        for (size_t i = 0; i < eta.size(); i++) {
            if (year == "2016preVFP") {
                electron_sf_weight *= cset_electron_2016preVFP->evaluate({"2016preVFP", "sf", "RecoAbove20", eta[i], pt[i]});
                electron_sf_weight *= cset_electron_2016preVFP->evaluate({"2016preVFP", "sf", "Loose", eta[i], pt[i]});
            }
            if (year == "2016postVFP") {
                electron_sf_weight *= cset_electron_2016postVFP->evaluate({"2016postVFP", "sf", "RecoAbove20", eta[i], pt[i]});
                electron_sf_weight *= cset_electron_2016postVFP->evaluate({"2016postVFP", "sf", "Loose", eta[i], pt[i]});
            }
            if (year == "2017") {
                electron_sf_weight *= cset_electron_2017->evaluate({"2017", "sf", "RecoAbove20", eta[i], pt[i]});
                electron_sf_weight *= cset_electron_2017->evaluate({"2017", "sf", "Loose", eta[i], pt[i]});
            }
            if (year == "2018") {
                electron_sf_weight *= cset_electron_2018->evaluate({"2018", "sf", "RecoAbove20", eta[i], pt[i]});
                electron_sf_weight *= cset_electron_2018->evaluate({"2018", "sf", "Loose", eta[i], pt[i]});
            }
        }
        return electron_sf_weight;
    };
    return df.Define("electron_scale_factors_ID", eval_correction, {"sample_year", "GElectron_eta", "GElectron_pt"});
}

RNode electronScaleFactors_ttH(correction::Correction::Ref cset_electron_tth, RNode df, std::string sf_type){
    auto eval_correction = [cset_electron_tth] (std::string year, const RVec<float> eta, const RVec<float> pt) {
        double electron_sf_weight = 1.;
        if (eta.size() == 0) {
            return electron_sf_weight;
        }
        for (size_t i = 0; i < eta.size(); i++) {
            electron_sf_weight *= cset_electron_tth->evaluate({year, abs(eta[i]), pt[i]});
        }
        return electron_sf_weight;
    };
    return df.Define(sf_type, eval_correction, {"sample_year", "GElectron_eta", "GElectron_pt"});
}

RNode electronScaleFactors_Trigger(correction::Correction::Ref cset_electron_trigger, RNode df) {
    auto eval_correction = [cset_electron_trigger] (std::string year, const RVec<float> eta, const RVec<float> pt) {
        double electron_sf_weight = 1.;
        if (eta.size() == 0) {
            return electron_sf_weight;
        }
        for (size_t i = 0; i < eta.size(); i++) {
            electron_sf_weight *= cset_electron_trigger->evaluate({year, eta[i], pt[i]});
        }
        return electron_sf_weight;
    };
    return df.Define("electron_scale_factors_trigger", eval_correction, {"sample_year", "GElectron_eta", "GElectron_pt"});
}

/*
Summer19UL16APV_V7_MC_Regrouped_Absolute_AK4PFchs
Summer19UL16APV_V7_MC_Regrouped_Absolute_2016_AK4PFchs
Summer19UL16APV_V7_MC_Regrouped_BBEC1_AK4PFchs
Summer19UL16APV_V7_MC_Regrouped_BBEC1_2016_AK4PFchs
Summer19UL16APV_V7_MC_Regrouped_EC2_AK4PFchs
Summer19UL16APV_V7_MC_Regrouped_EC2_2016_AK4PFchs
Summer19UL16APV_V7_MC_Regrouped_FlavorQCD_AK4PFchs
Summer19UL16APV_V7_MC_Regrouped_HF_AK4PFchs
Summer19UL16APV_V7_MC_Regrouped_HF_2016_AK4PFchs
Summer19UL16APV_V7_MC_Regrouped_RelativeBal_AK4PFchs
Summer19UL16APV_V7_MC_Regrouped_RelativeSample_2016_AK4PFchs
Summer19UL16APV_V7_MC_RelativeSample_AK4PFchs
Summer20UL16APV_JRV3_MC_PtResolution_AK4PFchs
*/

// RNode JetEnergyCorrections(correction::Correction::Ref cset_jerc_2016preVFP, correction::Correction::Ref cset_jerc_2016postVFP, correction::Correction::Ref cset_jerc_2017, correction::Correction::Ref cset_jerc_2018, RNode df) {
//     auto eval_correction = [cset_jerc_2016preVFP, cset_jerc_2016postVFP, cset_jerc_2017, cset_jerc_2018] (std::string year, float pt, float eta) {
//         double jerc = 1.;
//         if (year == "2016preVFP") {
//             jerc *= cset_jerc_2016preVFP->at("Summer19UL16APV_V7_MC_Regrouped_Absolute_AK4PFchs")->evaluate({pt, eta});
//         }
//         if (year == "2016postVFP") {
//             jerc *= cset_jerc_2016postVFP->evaluate({pt, eta, "nominal"});
//         }
//         if (year == "2017") {
//             jerc *= cset_jerc_2017->evaluate({pt, eta, "nominal"});
//         }
//         if (year == "2018") {
//             jerc *= cset_jerc_2018->evaluate({pt, eta, "nominal"});
//         }
//         return jerc;
//     };

RNode PNET_W_Corrections(correction::Correction::Ref cset_pnet_w, RNode df) {
    auto eval_correction = [cset_pnet_w] (std::string year, float pt) {
        double pnet_w = 1.;
        pnet_w *= cset_pnet_w->evaluate({pt, year, "nominal"});
        return pnet_w;
    };
    return df.Define("particlenet_w_weight", eval_correction, {"sample_year", "GW_pt"});
}

RNode PNET_H_Corrections(correction::Correction::Ref cset_pnet_h, RNode df) {
    auto eval_correction = [cset_pnet_h] (std::string year, float pt) {
        double pnet_h = 1.;
        pnet_h *= cset_pnet_h->evaluate({pt, year, "nominal"});
        return pnet_h;
    };
    return df.Define("particlenet_h_weight", eval_correction, {"sample_year", "GHiggs_pt"});
}

RNode JMS_Corrections(correction::Correction::Ref cset_jet_mass_scale, RNode df) { 
    auto eval_correction = [cset_jet_mass_scale] (std::string year, float mass) {
        double scaleVal = 1. + 0.05 * cset_jet_mass_scale->evaluate({year, "nominal"});
        // https://docs.google.com/presentation/d/1C7CqO3Wv3-lYd7vw4IQXq69wmULesTsSniFXXM__ReU
        return mass * scaleVal;
    };
    return df.Define("JMS_SF_H", eval_correction, {"sample_year", "GHiggs_mass"})
             .Define("JMS_SF_W", eval_correction, {"sample_year", "GW_mass"})
             .Redefine("Hbbmass", "JMS_SF_H")
             .Redefine("Wjetmass", "JMS_SF_W");
}

RNode JMR_Corrections(correction::Correction::Ref cset_jet_mass_resolution, RNode df) {
    auto eval_correction = [cset_jet_mass_resolution] (std::string year, float mass, unsigned int lumi, unsigned long long event) {
        TRandom3 rnd((lumi << 10) + event);
        return rnd.Gaus(1, 0.1 * cset_jet_mass_resolution->evaluate({year, "nominal"})) * mass;
    };
    return df.Define("JMR_SF_H", eval_correction, {"sample_year", "GHiggs_mass", "luminosityBlock", "event"})
             .Define("JMR_SF_W", eval_correction, {"sample_year", "GW_mass", "luminosityBlock", "event"})
             .Redefine("Hbbmass", "JMR_SF_H")
             .Redefine("Wjetmass", "JMR_SF_W");
}

// RNode METphicorrection(RNode df){
//     return df.Define("CorrMET", METXYCorr_Met_MetPhi, {"MET_pt", "MET_phi", "run", "sample_year", "sample_category", "Pileup_nTrueInt"})
//              .Define("CorrMET_pt", "CorrMET.first")
//              .Define("CorrMET_phi", "CorrMET.second");
// }

RNode AddHEMCorrection(RNode df) {
    return df.Define("HEMweight", HEMCorrections, {"run", "event", "sample_year", "sample_category", "HEMJet_pt", "HEMJet_eta", "HEMJet_phi"});
}

RNode finalDataWeight(RNode df){
    
    return df.Define("weight", 
            "goldenJSON * "
            "HEMweight");
}

RNode finalMCWeight(RNode df){
    auto df_pileup = pileupCorrection(cset_pileup_2016preVFP, cset_pileup_2016postVFP, cset_pileup_2017, cset_pileup_2018, df);
    auto df_pileupID = pileupIDCorrection(cset_pileupID_2016preVFP, cset_pileupID_2016postVFP, cset_pileupID_2017, cset_pileupID_2018, df_pileup);
    // muon sf
    auto df_muon_ID = muonScaleFactors_ID(cset_muon_ID_2016preVFP, cset_muon_ID_2016postVFP, cset_muon_ID_2017, cset_muon_ID_2018, df_pileupID);
    auto df_muon_ttHID = muonScaleFactors_ttH(cset_muon_ttHID, df_muon_ID, "muon_scale_factors_ttHID");
    auto df_muon_ttHISO = muonScaleFactors_ttH(cset_muon_ttHISO, df_muon_ttHID, "muon_scale_factors_ttHISO");
    // elec sf
    auto df_elec_ID = electronScaleFactors_ID(cset_electron_ID_2016preVFP, cset_electron_ID_2016postVFP, cset_electron_ID_2017, cset_electron_ID_2018, df_muon_ttHISO);
    auto df_elec_ttHID = electronScaleFactors_ttH(cset_electron_ttHID, df_elec_ID, "electron_scale_factors_ttHID");
    auto df_elec_ttHISO = electronScaleFactors_ttH(cset_electron_ttHISO, df_elec_ttHID, "electron_scale_factors_ttHISO");
    auto df_elec_trigger = electronScaleFactors_Trigger(cset_electron_trigger, df_elec_ttHISO);
    // particle net
    auto df_pnet_w = PNET_W_Corrections(cset_pnet_w, df_elec_trigger);
    auto df_pnet_h = PNET_H_Corrections(cset_pnet_h, df_pnet_w);
    auto df_jmr = JMR_Corrections(cset_jmr, df_pnet_h);
    auto df_jms = JMS_Corrections(cset_jms, df_jmr);
    return df_jmr.Define("weight", 
            "pileup_weight * "
            "pileupid_weight * "
            "L1PreFiringWeight_Nom * "
            "muon_scale_factors_ID * "
            "muon_scale_factors_ttHID * "
            "muon_scale_factors_ttHISO * "
            "electron_scale_factors_ID * "
            "electron_scale_factors_ttHID * "
            "electron_scale_factors_ttHISO * "
            "electron_scale_factors_trigger * "
            "HEMweight * "
            "particlenet_w_weight * "
            "particlenet_h_weight * "
            "xsec_weight * "
            "genWeight");
}

// WEIGHT FUNCTION DEFINITIONS
int HEMCorrections(unsigned int run, unsigned long long event, std::string sample_year, std::string sample_category, RVec<float> pt, RVec<float> eta, RVec<float> phi) {
    bool isData = false;
    if (sample_category == "data") isData = true;
    if (sample_year == "2018" && ((isData && run >= 319077) || (!isData && event % 1961 < 1286))) {
        for (size_t i = 0; i < pt.size(); i++) {
            if (eta[i] > -3.2 && eta[i] < -1.3 && phi[i] > -1.57 && phi[i] < -0.87) {
                return 0;
            }
        }
    }
    return 1;
}

// PUID
// JEC

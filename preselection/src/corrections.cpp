#include "corrections.h"
// b WP

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

// CORRECTIONS
RNode defineCorrectedCols(RNode df) {
    return df.Define("CorrJet_pt", "Jet_pt")
            .Define("CorrJet_mass", "Jet_mass")
            .Define("CorrFatJet_pt", "FatJet_pt")
            .Define("CorrFatJet_mass", "FatJet_mass")
            .Define("CorrFatJet_msoftdrop", "FatJet_msoftdrop")
            .Define("CorrMET_pt", "MET_pt");
}

RNode HEMCorrection(RNode df) {
    auto df1 = df.Define("HEMJets", "CorrJet_pt > 15 && "
                "((is2016 && Jet_jetId >= 1) || (!is2016 && Jet_jetId >= 2)) && "
                "(CorrJet_pt >= 50 || (CorrJet_pt < 50 && Jet_puId != 0))")
            .Define("HEMJet_pt", "CorrJet_pt[HEMJets]")
            .Define("HEMJet_eta", "Jet_eta[HEMJets]")
            .Define("HEMJet_phi", "Jet_phi[HEMJets]");

    auto HEMCorrections = [](unsigned int run, unsigned long long event, std::string sample_year, std::string sample_category, RVec<float> pt, RVec<float> eta, RVec<float> phi) {
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
    };

    return df1.Define("HEMweight", HEMCorrections, {"run", "event", "sample_year", "sample_category", "HEMJet_pt", "HEMJet_eta", "HEMJet_phi"});
}

RNode JMS_Corrections(correction::Correction::Ref cset_jet_mass_scale, RNode df, std::string variation) { 
    auto eval_correction = [cset_jet_mass_scale, variation] (std::string year, float mass) {
        double scaleVal = 1. + 0.05 * cset_jet_mass_scale->evaluate({year, variation});
        // https://docs.google.com/presentation/d/1C7CqO3Wv3-lYd7vw4IQXq69wmULesTsSniFXXM__ReU
        return mass * scaleVal;
    };
    return df.Redefine("Hbbmass", eval_correction, {"sample_year", "GHiggs_mass"})
             .Redefine("Wjetmass", eval_correction, {"sample_year", "GW_mass"});
}

RNode JMR_Corrections(correction::Correction::Ref cset_jet_mass_resolution, RNode df, std::string variation) {
    auto eval_correction = [cset_jet_mass_resolution, variation] (std::string year, float mass, unsigned int lumi, unsigned long long event) {
        TRandom3 rnd((lumi << 10) + event);
        return rnd.Gaus(1, 0.1 * cset_jet_mass_resolution->evaluate({year, variation})) * mass;
    };
    return df.Redefine("Hbbmass", eval_correction, {"sample_year", "GHiggs_mass", "luminosityBlock", "event"})
             .Redefine("Wjetmass", eval_correction, {"sample_year", "GW_mass", "luminosityBlock", "event"});
}

RNode METUnclusteredCorrections(RNode df, std::string variation) {
    if (variation == "up") {
        return df.Define("MET_uncert_dx", "MET_pt * TMath::Cos(MET_phi) + MET_MetUnclustEnUpDeltaX")
                .Define("MET_uncert_dy", "MET_pt * TMath::Sin(MET_phi) + MET_MetUnclustEnUpDeltaY")
                .Redefine("CorrMET_pt", "TMath::Sqrt(MET_uncert_dx * MET_uncert_dx + MET_uncert_dy * MET_uncert_dy)");
    }
    else if (variation == "down") {
        return df.Define("MET_uncert_dx", "MET_pt * TMath::Cos(MET_phi) - MET_MetUnclustEnUpDeltaX")
                .Define("MET_uncert_dy", "MET_pt * TMath::Sin(MET_phi) - MET_MetUnclustEnUpDeltaY")
                .Redefine("CorrMET_pt", "TMath::Sqrt(MET_uncert_dx * MET_uncert_dx + MET_uncert_dy * MET_uncert_dy)");
    }
    return df;
}

RNode JetEnergyCorrection(correction::CorrectionSet cset_jerc_2016preVFP, correction::CorrectionSet cset_jerc_2016postVFP, correction::CorrectionSet cset_jerc_2017, correction::CorrectionSet cset_jerc_2018, RNode df, std::string JEC_type, std::string variation) {
    auto eval_correction = [cset_jerc_2016preVFP, cset_jerc_2016postVFP, cset_jerc_2017, cset_jerc_2018, JEC_type, variation] (std::string year, RVec<float> eta, RVec<float> pt, RVec<float> raw_factor, RVec<float> var) {
        RVec<float> jec_factors;
        RVec<float> uncorr_var;

        std::string JEC;

        if (var.size() == 0) {
            return var;
        }

        for (size_t i = 0; i < var.size(); i++){
            uncorr_var.push_back(var[i] * (1 - raw_factor[i]));
        }
        
        for (size_t i = 0; i < var.size(); i++) {
            if (year == "2016preVFP") {
                JEC = std::string("Summer19UL16APV_V7_MC_") + JEC_type + std::string("_AK4PFchs");
                if (JEC_type.find("2017") != std::string::npos || JEC_type.find("2018") != std::string::npos) {
                    jec_factors.push_back(var[i]);
                }
                else {
                    if (variation == "up") {
                        jec_factors.push_back(uncorr_var[i] * (1 + cset_jerc_2016preVFP.at(JEC)->evaluate({eta[i], pt[i]})));
                    }
                    else if (variation == "down") {
                        jec_factors.push_back(uncorr_var[i] * (1 - cset_jerc_2016preVFP.at(JEC)->evaluate({eta[i], pt[i]})));
                    }
                    else {
                        jec_factors.push_back(var[i]);
                    }
                }
            }
            else if (year == "2016postVFP") {
                JEC = std::string("Summer19UL16_V7_MC_") + JEC_type + std::string("_AK4PFchs");
                if (JEC_type.find("2017") != std::string::npos || JEC_type.find("2018") != std::string::npos) {
                    jec_factors.push_back(var[i]);
                }
                else {
                    if (variation == "up") {
                        jec_factors.push_back(uncorr_var[i] * (1 + cset_jerc_2016postVFP.at(JEC)->evaluate({eta[i], pt[i]})));
                    }
                    else if (variation == "down") {
                        jec_factors.push_back(uncorr_var[i] * (1 - cset_jerc_2016postVFP.at(JEC)->evaluate({eta[i], pt[i]})));
                    }
                    else {
                        jec_factors.push_back(var[i]);
                    }
                }
            }
            else if (year == "2017") {
                JEC = std::string("Summer19UL17_V5_MC_") + JEC_type + std::string("_AK4PFchs");
                if (JEC_type.find("2016") != std::string::npos || JEC_type.find("2018") != std::string::npos) {
                    jec_factors.push_back(var[i]);
                }
                else {
                    if (variation == "up") {
                        jec_factors.push_back(uncorr_var[i] * (1 + cset_jerc_2017.at(JEC)->evaluate({eta[i], pt[i]})));
                    }
                    else if (variation == "down") {
                        jec_factors.push_back(uncorr_var[i] * (1 - cset_jerc_2017.at(JEC)->evaluate({eta[i], pt[i]})));
                    }
                    else {
                        jec_factors.push_back(var[i]);
                    }
                }
            }
            else if (year == "2018") {
                JEC = std::string("Summer19UL18_V5_MC_") + JEC_type + std::string("_AK4PFchs");
                if (JEC_type.find("2016") != std::string::npos || JEC_type.find("2017") != std::string::npos) {
                    jec_factors.push_back(var[i]);
                }
                else {
                    if (variation == "up") {
                        jec_factors.push_back(uncorr_var[i] * (1 + cset_jerc_2018.at(JEC)->evaluate({eta[i], pt[i]})));
                    }
                    else if (variation == "down") {
                        jec_factors.push_back(uncorr_var[i] * (1 - cset_jerc_2018.at(JEC)->evaluate({eta[i], pt[i]})));
                    }
                    else {
                        jec_factors.push_back(var[i]);
                    }
                }
            }
            else {
                jec_factors.push_back(var[i]);
            }
        }
        return jec_factors;
    };
    auto df_jetcorr =  df.Redefine("CorrJet_pt", eval_correction, {"sample_year", "Jet_pt", "Jet_eta", "Jet_rawFactor", "Jet_pt"})
                        .Redefine("CorrJet_mass", eval_correction, {"sample_year", "Jet_pt", "Jet_eta", "Jet_rawFactor", "Jet_mass"})
                        .Redefine("CorrFatJet_pt", eval_correction, {"sample_year", "FatJet_pt", "FatJet_eta", "FatJet_rawFactor", "FatJet_pt"})
                        .Redefine("CorrFatJet_mass", eval_correction, {"sample_year", "FatJet_pt", "FatJet_eta", "FatJet_rawFactor", "FatJet_mass"})
                        .Redefine("CorrFatJet_msoftdrop", eval_correction, {"sample_year", "FatJet_pt", "FatJet_eta", "FatJet_rawFactor", "FatJet_msoftdrop"});

    auto correctmet = [](RVec<float> corrjet_pt, RVec<float> jet_phi, float MET_pt, float MET_phi) {
        float px = 0;
        float py = 0;
        for (size_t i = 0; i < corrjet_pt.size(); i++) {
            px += corrjet_pt[i] * TMath::Cos(jet_phi[i]);
            py += corrjet_pt[i] * TMath::Sin(jet_phi[i]);
        }
        return TMath::Sqrt(px * px + py * py);        
    };

    return df_jetcorr.Redefine("CorrMET_pt", correctmet, {"CorrJet_pt", "Jet_phi", "MET_pt", "MET_phi"});
}

RNode JetEnergyResolution(correction::CorrectionSet cset_jerc_2016preVFP, correction::CorrectionSet cset_jerc_2016postVFP, correction::CorrectionSet cset_jerc_2017, correction::CorrectionSet cset_jerc_2018, correction::CorrectionSet cset_jer_smear, RNode df, std::string variation) {
    auto eval_correction = [cset_jerc_2016preVFP, cset_jerc_2016postVFP, cset_jerc_2017, cset_jerc_2018, cset_jer_smear, variation] (std::string year, RVec<float> pt, RVec<float> eta, RVec<int> genJet_idx, RVec<float> genJet_pt, float rho, unsigned long long event, RVec<float> var) {
        RVec<float> jer_factors;

        std::string vary;
        // for jer json
        if (variation == "nominal") {
            vary = "nom";
        }
        else {
            vary = variation;
        }

        float jer_sf;
        float jer;

        if (var.size() == 0) {
            return var;
        }
        
        for (size_t i = 0; i < var.size(); i++) {
            if (year == "2016preVFP") {
                jer = cset_jerc_2016preVFP.at("Summer20UL16APV_JRV3_MC_PtResolution_AK4PFchs")->evaluate({eta[i], pt[i], rho});
                jer_sf = cset_jerc_2016preVFP.at("Summer20UL16APV_JRV3_MC_ScaleFactor_AK4PFchs")->evaluate({eta[i], vary});
                jer_factors.push_back(var[i] * cset_jer_smear.at("JERSmear")->evaluate({pt[i], eta[i], genJet_pt[genJet_idx[i]], rho, (int)event, jer, jer_sf}));
            }
            else if (year == "2016postVFP") {
                jer = cset_jerc_2016postVFP.at("Summer20UL16_JRV3_MC_PtResolution_AK4PFchs")->evaluate({eta[i], pt[i], rho});
                jer_sf = cset_jerc_2016postVFP.at("Summer20UL16_JRV3_MC_ScaleFactor_AK4PFchs")->evaluate({eta[i], vary});
                jer_factors.push_back(var[i] * cset_jer_smear.at("JERSmear")->evaluate({pt[i], eta[i], genJet_pt[genJet_idx[i]], rho, (int)event, jer, jer_sf}));
            }
            else if (year == "2017") {
                jer = cset_jerc_2017.at("Summer19UL17_JRV2_MC_PtResolution_AK4PFchs")->evaluate({eta[i], pt[i], rho});
                jer_sf = cset_jerc_2017.at("Summer19UL17_JRV2_MC_ScaleFactor_AK4PFchs")->evaluate({eta[i], vary});
                jer_factors.push_back(var[i] * cset_jer_smear.at("JERSmear")->evaluate({pt[i], eta[i], genJet_pt[genJet_idx[i]], rho, (int)event, jer, jer_sf}));
            }
            else if (year == "2018") {
                jer = cset_jerc_2018.at("Summer19UL18_JRV2_MC_PtResolution_AK4PFchs")->evaluate({eta[i], pt[i], rho});
                jer_sf = cset_jerc_2018.at("Summer19UL18_JRV2_MC_ScaleFactor_AK4PFchs")->evaluate({eta[i], vary});
                jer_factors.push_back(var[i] * cset_jer_smear.at("JERSmear")->evaluate({pt[i], eta[i], genJet_pt[genJet_idx[i]], rho, (int)event, jer, jer_sf}));
            }
            else {
                jer_factors.push_back(var[i]);
            }
        }
        return jer_factors;
    };
    return df.Redefine("CorrJet_pt", eval_correction, {"sample_year", "Jet_pt", "Jet_eta", "Jet_genJetIdx", "GenJet_pt", "fixedGridRhoFastjetAll", "event", "Jet_pt"})
            .Redefine("CorrJet_mass", eval_correction, {"sample_year", "Jet_pt", "Jet_eta", "Jet_genJetIdx", "GenJet_pt", "fixedGridRhoFastjetAll", "event", "Jet_mass"});
}   
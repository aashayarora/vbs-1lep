#include "weights.h"

RNode goodRun(lumiMask golden, RNode df){
    auto goldenjson = [golden](unsigned int &run, unsigned int &luminosityBlock){return golden.accept(run, luminosityBlock);};
    return df.Define("goodRun", goldenjson, {"run", "luminosityBlock"})
             .Filter("goodRun", "PASSES GOLDEN JSON");
}

RNode pileupCorrection(correction::Correction::Ref cset_pileup_2016preVFP, correction::Correction::Ref cset_pileup_2016postVFP, correction::Correction::Ref cset_pileup_2017, correction::Correction::Ref cset_pileup_2018, RNode df){
    auto eval_correction = [cset_pileup_2016preVFP, cset_pileup_2016postVFP, cset_pileup_2017, cset_pileup_2018] (std::string year, float ntrueint) {
        if (year == "2016preVFP") return cset_pileup_2016preVFP->evaluate({ntrueint, "nominal"});
        if (year == "2016postVFP") return cset_pileup_2016postVFP->evaluate({ntrueint, "nominal"});
        if (year == "2017") return cset_pileup_2017->evaluate({ntrueint, "nominal"});
        if (year == "2018") return cset_pileup_2018->evaluate({ntrueint, "nominal"});
        return 0.;
    };
    return df.Define("pileup_weight", eval_correction, {"sample_year", "Pileup_nTrueInt"});
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

RNode muonScaleFactors_ttH(correction::Correction::Ref cset_muon_2016preVFP, correction::Correction::Ref cset_muon_2016postVFP, correction::Correction::Ref cset_muon_2017, correction::Correction::Ref cset_muon_2018, RNode df, std::string sf_type){
    auto eval_correction = [cset_muon_2016preVFP, cset_muon_2016postVFP, cset_muon_2017, cset_muon_2018] (std::string year, const RVec<float> eta, const RVec<float> pt) {
        double muon_sf_weight = 1.;
        if (eta.size() == 0) {
            return muon_sf_weight;
        }
        for (size_t i = 0; i < eta.size(); i++) {
            if (year == "2016preVFP") {
                muon_sf_weight *= cset_muon_2016preVFP->evaluate({"ttHSF", abs(eta[i]), pt[i]});
            }
            if (year == "2016postVFP") {
                muon_sf_weight *= cset_muon_2016postVFP->evaluate({"ttHSF", abs(eta[i]), pt[i]});
            }
            if (year == "2017") {
                muon_sf_weight *= cset_muon_2017->evaluate({"ttHSF", abs(eta[i]), pt[i]});
            }
            if (year == "2018") {
                muon_sf_weight *= cset_muon_2018->evaluate({"ttHSF", abs(eta[i]), pt[i]});
            }
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
            }
            if (year == "2016postVFP") {
                electron_sf_weight *= cset_electron_2016postVFP->evaluate({"2016postVFP", "sf", "RecoAbove20", eta[i], pt[i]});
            }
            if (year == "2017") {
                electron_sf_weight *= cset_electron_2017->evaluate({"2017", "sf", "RecoAbove20", eta[i], pt[i]});
            }
            if (year == "2018") {
                electron_sf_weight *= cset_electron_2018->evaluate({"2018", "sf", "RecoAbove20", eta[i], pt[i]});
            }
        }
        return electron_sf_weight;
    };
    return df.Define("electron_scale_factors_ID", eval_correction, {"sample_year", "GElectron_eta", "GElectron_pt"});
}

RNode electronScaleFactors_ttH(correction::Correction::Ref cset_electron_2016preVFP, correction::Correction::Ref cset_electron_2016postVFP, correction::Correction::Ref cset_electron_2017, correction::Correction::Ref cset_electron_2018, RNode df, std::string sf_type){
    auto eval_correction = [cset_electron_2016preVFP, cset_electron_2016postVFP, cset_electron_2017, cset_electron_2018] (std::string year, const RVec<float> eta, const RVec<float> pt) {
        double electron_sf_weight = 1.;
        if (eta.size() == 0) {
            return electron_sf_weight;
        }
        for (size_t i = 0; i < eta.size(); i++) {
            if (year == "2016preVFP") {
                electron_sf_weight *= cset_electron_2016preVFP->evaluate({"ttHSF", abs(eta[i]), pt[i]});
            }
            if (year == "2016postVFP") {
                electron_sf_weight *= cset_electron_2016postVFP->evaluate({"ttHSF", abs(eta[i]), pt[i]});
            }
            if (year == "2017") {
                electron_sf_weight *= cset_electron_2017->evaluate({"ttHSF", abs(eta[i]), pt[i]});
            }
            if (year == "2018") {
                electron_sf_weight *= cset_electron_2018->evaluate({"ttHSF", abs(eta[i]), pt[i]});
            }
        }
        return electron_sf_weight;
    };
    return df.Define(sf_type, eval_correction, {"sample_year", "GElectron_eta", "GElectron_pt"});
}

RNode finalMCWeight(RNode df){
    return df.Define("weight", 
            "pileup_weight * "
            "muon_scale_factors_ID * "
            "muon_scale_factors_ttHID * "
            "muon_scale_factors_ttHISO * "
            "electron_scale_factors_ID * "
            "electron_scale_factors_ttHID * "
            "electron_scale_factors_ttHISO * "
            "xsec_weight * "
            "genWeight");
}
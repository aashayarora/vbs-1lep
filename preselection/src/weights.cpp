#include "weights.h"

RNode goodRun(lumiMask golden, RNode df){
    auto goldenjson = [golden](unsigned int &run, unsigned int &luminosityBlock){return golden.accept(run, luminosityBlock);};
    return df.Define("goldenJSON", goldenjson, {"run", "luminosityBlock"})
             .Filter("goldenJSON", "PASSES GOLDEN JSON");
}

RNode L1PreFiringWeight(RNode df){
    auto eval_correction = [] (float L1prefire, float L1prefireup, float L1prefiredown) {
        return RVec<float>{L1prefire, L1prefireup, L1prefiredown};
    };
    return df.Define("L1PreFiringWeight", eval_correction, {"L1PreFiringWeight_Nom", "L1PreFiringWeight_Up", "L1PreFiringWeight_Dn"});
}

RNode pileupScaleFactors(correction::Correction::Ref cset_pileup_2016preVFP, correction::Correction::Ref cset_pileup_2016postVFP, correction::Correction::Ref cset_pileup_2017, correction::Correction::Ref cset_pileup_2018, RNode df){
    auto eval_correction = [cset_pileup_2016preVFP, cset_pileup_2016postVFP, cset_pileup_2017, cset_pileup_2018] (std::string year, float ntrueint) {
        RVec<double> pileup_weights;
        if (year == "2016preVFP") {
            pileup_weights.push_back(cset_pileup_2016preVFP->evaluate({ntrueint, "nominal"}));
            pileup_weights.push_back(cset_pileup_2016preVFP->evaluate({ntrueint, "up"}));
            pileup_weights.push_back(cset_pileup_2016preVFP->evaluate({ntrueint, "down"}));
        }
        if (year == "2016postVFP") {
            pileup_weights.push_back(cset_pileup_2016postVFP->evaluate({ntrueint, "nominal"}));
            pileup_weights.push_back(cset_pileup_2016postVFP->evaluate({ntrueint, "up"}));
            pileup_weights.push_back(cset_pileup_2016postVFP->evaluate({ntrueint, "down"}));
        }
        if (year == "2017") {
            pileup_weights.push_back(cset_pileup_2017->evaluate({ntrueint, "nominal"}));
            pileup_weights.push_back(cset_pileup_2017->evaluate({ntrueint, "up"}));
            pileup_weights.push_back(cset_pileup_2017->evaluate({ntrueint, "down"}));
        }
        if (year == "2018") {
            pileup_weights.push_back(cset_pileup_2018->evaluate({ntrueint, "nominal"}));
            pileup_weights.push_back(cset_pileup_2018->evaluate({ntrueint, "up"}));
            pileup_weights.push_back(cset_pileup_2018->evaluate({ntrueint, "down"}));
        }
        return pileup_weights;
    };
    return df.Define("pileup_weight", eval_correction, {"sample_year", "Pileup_nTrueInt"});
}

RNode pileupIDScaleFactors(correction::Correction::Ref cset_pileup_2016preVFP, correction::Correction::Ref cset_pileup_2016postVFP, correction::Correction::Ref cset_pileup_2017, correction::Correction::Ref cset_pileup_2018, RNode df){
    auto eval_correction = [cset_pileup_2016preVFP, cset_pileup_2016postVFP, cset_pileup_2017, cset_pileup_2018] (std::string year, RVec<float> eta, RVec<float> pt) {
        RVec<double> pileup_weights = {1., 1., 1.};
        if (eta.size() == 0) {
            return pileup_weights;
        }
        for (size_t i = 0; i < eta.size(); i++) {
            if (year == "2016preVFP" && pt[i] < 50) {
                pileup_weights[0] *= cset_pileup_2016preVFP->evaluate({eta[i], pt[i], "nom", "L"});
                pileup_weights[1] *= cset_pileup_2016preVFP->evaluate({eta[i], pt[i], "up", "L"});
                pileup_weights[2] *= cset_pileup_2016preVFP->evaluate({eta[i], pt[i], "down", "L"});
            }
            if (year == "2016postVFP" && pt[i] < 50) {
                pileup_weights[0] *= cset_pileup_2016postVFP->evaluate({eta[i], pt[i], "nom", "L"});
                pileup_weights[1] *= cset_pileup_2016postVFP->evaluate({eta[i], pt[i], "up", "L"});
                pileup_weights[2] *= cset_pileup_2016postVFP->evaluate({eta[i], pt[i], "down", "L"});
            }
            if (year == "2017" && pt[i] < 50) {
                pileup_weights[0] *= cset_pileup_2017->evaluate({eta[i], pt[i], "nom", "L"});
                pileup_weights[1] *= cset_pileup_2017->evaluate({eta[i], pt[i], "up", "L"});
                pileup_weights[2] *= cset_pileup_2017->evaluate({eta[i], pt[i], "down", "L"});
            }
            if (year == "2018" && pt[i] < 50) {
                pileup_weights[0] *= cset_pileup_2018->evaluate({eta[i], pt[i], "nom", "L"});
                pileup_weights[1] *= cset_pileup_2018->evaluate({eta[i], pt[i], "up", "L"});
                pileup_weights[2] *= cset_pileup_2018->evaluate({eta[i], pt[i], "down", "L"});
            }
        }
        return pileup_weights;
    };
    return df.Define("pileupid_weight", eval_correction, {"sample_year", "VBSJets_eta", "VBSJets_pt"});
}

RNode muonScaleFactors_ID(correction::CorrectionSet cset_muon_2016preVFP, correction::CorrectionSet cset_muon_2016postVFP, correction::CorrectionSet cset_muon_2017, correction::CorrectionSet cset_muon_2018, RNode df){
    auto eval_correction = [cset_muon_2016preVFP, cset_muon_2016postVFP, cset_muon_2017, cset_muon_2018] (std::string year, const RVec<float> eta, const RVec<float> pt) {
        RVec<double> muon_sf_weights = {1., 1., 1.};
        if (eta.size() == 0) {
            return muon_sf_weights;
        }
        for (size_t i = 0; i < eta.size(); i++) {
            if (year == "2016preVFP") {
                muon_sf_weights[0] *= cset_muon_2016preVFP.at("NUM_TightID_DEN_TrackerMuons")->evaluate({abs(eta[i]), pt[i], "nominal"});
                muon_sf_weights[1] *= cset_muon_2016preVFP.at("NUM_TightID_DEN_TrackerMuons")->evaluate({abs(eta[i]), pt[i], "systup"});
                muon_sf_weights[2] *= cset_muon_2016preVFP.at("NUM_TightID_DEN_TrackerMuons")->evaluate({abs(eta[i]), pt[i], "systdown"});
            }
            if (year == "2016postVFP") {
                muon_sf_weights[0] *= cset_muon_2016postVFP.at("NUM_TightID_DEN_TrackerMuons")->evaluate({abs(eta[i]), pt[i], "nominal"});
                muon_sf_weights[1] *= cset_muon_2016postVFP.at("NUM_TightID_DEN_TrackerMuons")->evaluate({abs(eta[i]), pt[i], "systup"});
                muon_sf_weights[2] *= cset_muon_2016postVFP.at("NUM_TightID_DEN_TrackerMuons")->evaluate({abs(eta[i]), pt[i], "systdown"});
            }
            if (year == "2017") {
                muon_sf_weights[0] *= cset_muon_2017.at("NUM_TightID_DEN_TrackerMuons")->evaluate({abs(eta[i]), pt[i], "nominal"});
                muon_sf_weights[1] *= cset_muon_2017.at("NUM_TightID_DEN_TrackerMuons")->evaluate({abs(eta[i]), pt[i], "systup"});
                muon_sf_weights[2] *= cset_muon_2017.at("NUM_TightID_DEN_TrackerMuons")->evaluate({abs(eta[i]), pt[i], "systdown"});
            }
            if (year == "2018") {
                muon_sf_weights[0] *= cset_muon_2018.at("NUM_TightID_DEN_TrackerMuons")->evaluate({abs(eta[i]), pt[i], "nominal"});
                muon_sf_weights[1] *= cset_muon_2018.at("NUM_TightID_DEN_TrackerMuons")->evaluate({abs(eta[i]), pt[i], "systup"});
                muon_sf_weights[2] *= cset_muon_2018.at("NUM_TightID_DEN_TrackerMuons")->evaluate({abs(eta[i]), pt[i], "systdown"});
            }
        }
        return muon_sf_weights;
    };
    return df.Define("muon_scale_factors_ID", eval_correction, {"sample_year", "GMuon_eta", "GMuon_pt"});
}

RNode muonScaleFactors_trigger(correction::CorrectionSet cset_muon_2016preVFP, correction::CorrectionSet cset_muon_2016postVFP, correction::CorrectionSet cset_muon_2017, correction::CorrectionSet cset_muon_2018, RNode df){
    auto eval_correction = [cset_muon_2016preVFP, cset_muon_2016postVFP, cset_muon_2017, cset_muon_2018] (std::string year, const RVec<float> eta, const RVec<float> pt) {
        RVec<double> muon_sf_weights = {1., 1., 1.};
        if (eta.size() == 0) {
            return muon_sf_weights;
        }
        for (size_t i = 0; i < eta.size(); i++) {
            if (year == "2016preVFP") {
                muon_sf_weights[0] *= cset_muon_2016preVFP.at("NUM_IsoMu24_or_IsoTkMu24_DEN_CutBasedIdTight_and_PFIsoTight")->evaluate({abs(eta[i]), pt[i], "nominal"});
                muon_sf_weights[1] *= cset_muon_2016preVFP.at("NUM_IsoMu24_or_IsoTkMu24_DEN_CutBasedIdTight_and_PFIsoTight")->evaluate({abs(eta[i]), pt[i], "systup"});
                muon_sf_weights[2] *= cset_muon_2016preVFP.at("NUM_IsoMu24_or_IsoTkMu24_DEN_CutBasedIdTight_and_PFIsoTight")->evaluate({abs(eta[i]), pt[i], "systdown"});
            }
            if (year == "2016postVFP") {
                muon_sf_weights[0] *= cset_muon_2016postVFP.at("NUM_IsoMu24_or_IsoTkMu24_DEN_CutBasedIdTight_and_PFIsoTight")->evaluate({abs(eta[i]), pt[i], "nominal"});
                muon_sf_weights[1] *= cset_muon_2016postVFP.at("NUM_IsoMu24_or_IsoTkMu24_DEN_CutBasedIdTight_and_PFIsoTight")->evaluate({abs(eta[i]), pt[i], "systup"});
                muon_sf_weights[2] *= cset_muon_2016postVFP.at("NUM_IsoMu24_or_IsoTkMu24_DEN_CutBasedIdTight_and_PFIsoTight")->evaluate({abs(eta[i]), pt[i], "systdown"});
            }
            if (year == "2017") {
                muon_sf_weights[0] *= cset_muon_2017.at("NUM_IsoMu27_DEN_CutBasedIdTight_and_PFIsoTight")->evaluate({abs(eta[i]), pt[i], "nominal"});
                muon_sf_weights[1] *= cset_muon_2017.at("NUM_IsoMu27_DEN_CutBasedIdTight_and_PFIsoTight")->evaluate({abs(eta[i]), pt[i], "systup"});
                muon_sf_weights[2] *= cset_muon_2017.at("NUM_IsoMu27_DEN_CutBasedIdTight_and_PFIsoTight")->evaluate({abs(eta[i]), pt[i], "systdown"});
            }
            if (year == "2018") {
                muon_sf_weights[0] *= cset_muon_2018.at("NUM_IsoMu24_DEN_CutBasedIdTight_and_PFIsoTight")->evaluate({abs(eta[i]), pt[i], "nominal"});
                muon_sf_weights[1] *= cset_muon_2018.at("NUM_IsoMu24_DEN_CutBasedIdTight_and_PFIsoTight")->evaluate({abs(eta[i]), pt[i], "systup"});
                muon_sf_weights[2] *= cset_muon_2018.at("NUM_IsoMu24_DEN_CutBasedIdTight_and_PFIsoTight")->evaluate({abs(eta[i]), pt[i], "systdown"});
            }
        }
        return muon_sf_weights;
    };
    return df.Define("muon_scale_factors_trigger", eval_correction, {"sample_year", "GMuon_eta", "GMuon_pt"});
}

RNode muonScaleFactors_ttH(correction::Correction::Ref cset_muon_tth, RNode df, std::string sf_type){
    auto eval_correction = [cset_muon_tth] (std::string year, const RVec<float> eta, const RVec<float> pt) {
        RVec<double> muon_sf_weights = {1., 1., 1.};
        if (eta.size() == 0) {
            return muon_sf_weights;
        }
        for (size_t i = 0; i < eta.size(); i++) {
            float muon_sf_weight = cset_muon_tth->evaluate({year, abs(eta[i]), pt[i]});
            muon_sf_weights[0] *= muon_sf_weight;
            muon_sf_weights[1] *= (muon_sf_weight + sqrt(muon_sf_weight));
            muon_sf_weights[2] *= (muon_sf_weight - sqrt(muon_sf_weight));
        }
        return muon_sf_weights;
    };
    return df.Define(sf_type, eval_correction, {"sample_year", "GMuon_eta", "GMuon_pt"});
}

RNode electronScaleFactors_Reco(correction::Correction::Ref cset_electron_2016preVFP, correction::Correction::Ref cset_electron_2016postVFP, correction::Correction::Ref cset_electron_2017, correction::Correction::Ref cset_electron_2018, RNode df){
    auto eval_correction = [cset_electron_2016preVFP, cset_electron_2016postVFP, cset_electron_2017, cset_electron_2018] (std::string year, const RVec<float> eta, const RVec<float> pt) {
        RVec<double> electron_sf_weights = {1., 1., 1.};
        if (eta.size() == 0) {
            return electron_sf_weights;
        }
        for (size_t i = 0; i < eta.size(); i++) {
            if (year == "2016preVFP") {
                electron_sf_weights[0] *= cset_electron_2016preVFP->evaluate({"2016preVFP", "sf", "RecoAbove20", eta[i], pt[i]});
                electron_sf_weights[1] *= cset_electron_2016preVFP->evaluate({"2016preVFP", "sfup", "RecoAbove20", eta[i], pt[i]});
                electron_sf_weights[2] *= cset_electron_2016preVFP->evaluate({"2016preVFP", "sfdown", "RecoAbove20", eta[i], pt[i]});
            }
           if (year == "2016postVFP") {
                electron_sf_weights[0] *= cset_electron_2016postVFP->evaluate({"2016postVFP", "sf", "RecoAbove20", eta[i], pt[i]});
                electron_sf_weights[1] *= cset_electron_2016postVFP->evaluate({"2016postVFP", "sfup", "RecoAbove20", eta[i], pt[i]});
                electron_sf_weights[2] *= cset_electron_2016postVFP->evaluate({"2016postVFP", "sfdown", "RecoAbove20", eta[i], pt[i]});
            }
            if (year == "2017") {
                electron_sf_weights[0] *= cset_electron_2017->evaluate({"2017", "sf", "RecoAbove20", eta[i], pt[i]});
                electron_sf_weights[1] *= cset_electron_2017->evaluate({"2017", "sfup", "RecoAbove20", eta[i], pt[i]});
                electron_sf_weights[2] *= cset_electron_2017->evaluate({"2017", "sfdown", "RecoAbove20", eta[i], pt[i]});
            }
            if (year == "2018") {
                electron_sf_weights[0] *= cset_electron_2018->evaluate({"2018", "sf", "RecoAbove20", eta[i], pt[i]});
                electron_sf_weights[1] *= cset_electron_2018->evaluate({"2018", "sfup", "RecoAbove20", eta[i], pt[i]});
                electron_sf_weights[2] *= cset_electron_2018->evaluate({"2018", "sfdown", "RecoAbove20", eta[i], pt[i]});
            
           }
        }
        return electron_sf_weights;
    };
    return df.Define("electron_scale_factors_Reco", eval_correction, {"sample_year", "GElectron_eta", "GElectron_pt"});
}

RNode electronScaleFactors_ID(correction::Correction::Ref cset_electron_2016preVFP, correction::Correction::Ref cset_electron_2016postVFP, correction::Correction::Ref cset_electron_2017, correction::Correction::Ref cset_electron_2018, RNode df){
    auto eval_correction = [cset_electron_2016preVFP, cset_electron_2016postVFP, cset_electron_2017, cset_electron_2018] (std::string year, const RVec<float> eta, const RVec<float> pt) {
        RVec<double> electron_sf_weight = {1., 1., 1.};
        if (eta.size() == 0) {
            return electron_sf_weight;
        }
        for (size_t i = 0; i < eta.size(); i++) {
            if (year == "2016preVFP") {
                electron_sf_weight[0] *= cset_electron_2016preVFP->evaluate({"2016preVFP", "sf", "Loose", eta[i], pt[i]});
                electron_sf_weight[1] *= cset_electron_2016preVFP->evaluate({"2016preVFP", "sfup", "Loose", eta[i], pt[i]});
                electron_sf_weight[2] *= cset_electron_2016preVFP->evaluate({"2016preVFP", "sfdown", "Loose", eta[i], pt[i]});
            }
            if (year == "2016postVFP") {
                electron_sf_weight[0] *= cset_electron_2016postVFP->evaluate({"2016postVFP", "sf", "Loose", eta[i], pt[i]});
                electron_sf_weight[1] *= cset_electron_2016postVFP->evaluate({"2016postVFP", "sfup", "Loose", eta[i], pt[i]});
                electron_sf_weight[2] *= cset_electron_2016postVFP->evaluate({"2016postVFP", "sfdown", "Loose", eta[i], pt[i]});
            }
            if (year == "2017") {
                electron_sf_weight[0] *= cset_electron_2017->evaluate({"2017", "sf", "Loose", eta[i], pt[i]});
                electron_sf_weight[1] *= cset_electron_2017->evaluate({"2017", "sfup", "Loose", eta[i], pt[i]});
                electron_sf_weight[2] *= cset_electron_2017->evaluate({"2017", "sfdown", "Loose", eta[i], pt[i]});
            }
            if (year == "2018") {
                electron_sf_weight[0] *= cset_electron_2018->evaluate({"2018", "sf", "Loose", eta[i], pt[i]});
                electron_sf_weight[1] *= cset_electron_2018->evaluate({"2018", "sfup", "Loose", eta[i], pt[i]});
                electron_sf_weight[2] *= cset_electron_2018->evaluate({"2018", "sfdown", "Loose", eta[i], pt[i]});
            }
        }
        return electron_sf_weight;
    };
    return df.Define("electron_scale_factors_ID", eval_correction, {"sample_year", "GElectron_eta", "GElectron_pt"});
}

RNode electronScaleFactors_ttH(correction::Correction::Ref cset_electron_tth, RNode df, std::string sf_type){
    auto eval_correction = [cset_electron_tth] (std::string year, const RVec<float> eta, const RVec<float> pt) {
        RVec<double> electron_sf_weights = {1., 1., 1.};
        if (eta.size() == 0) {
            return electron_sf_weights;
        }
        for (size_t i = 0; i < eta.size(); i++) {
            float electron_sf_weight = cset_electron_tth->evaluate({year, abs(eta[i]), pt[i]});
            electron_sf_weights[0] *= electron_sf_weight;
            electron_sf_weights[1] *= (electron_sf_weight + sqrt(electron_sf_weight));
            electron_sf_weights[2] *= (electron_sf_weight - sqrt(electron_sf_weight));
        }
        return electron_sf_weights;
    };
    return df.Define(sf_type, eval_correction, {"sample_year", "GElectron_eta", "GElectron_pt"});
}

RNode electronScaleFactors_Trigger(correction::Correction::Ref cset_electron_trigger, RNode df) {
    auto eval_correction = [cset_electron_trigger] (std::string year, const RVec<float> eta, const RVec<float> pt) {
        RVec<double> electron_sf_weights = {1., 1., 1.};
        if (eta.size() == 0) {
            return electron_sf_weights;
        }
        for (size_t i = 0; i < eta.size(); i++) {
            float electron_sf_weight = cset_electron_trigger->evaluate({year, eta[i], pt[i]});
            electron_sf_weights[0] *= electron_sf_weight;
            electron_sf_weights[1] *= (electron_sf_weight + sqrt(electron_sf_weight));
            electron_sf_weights[2] *= (electron_sf_weight - sqrt(electron_sf_weight));
        }
        return electron_sf_weights;
    };
    return df.Define("electron_scale_factors_trigger", eval_correction, {"sample_year", "GElectron_eta", "GElectron_pt"});
}


RNode PNET_W_ScaleFactors_2016preVFP(correction::Correction::Ref cset_pnet_w, RNode df) {
    auto eval_correction = [cset_pnet_w] (std::string year, float pt) {
        RVec<double> pnet_w = {1., 1., 1.};
        if (year != "2016preVFP") {
            return pnet_w;
        }
        pnet_w[0] *= cset_pnet_w->evaluate({pt, year, "nominal"});
        pnet_w[1] *= cset_pnet_w->evaluate({pt, year, "up"});
        pnet_w[2] *= cset_pnet_w->evaluate({pt, year, "down"});
        return pnet_w;
    };
    return df.Define("particlenet_w_weight_2016preVFP", eval_correction, {"sample_year", "GW_pt"});
}

RNode PNET_W_ScaleFactors_2016postVFP(correction::Correction::Ref cset_pnet_w, RNode df) {
    auto eval_correction = [cset_pnet_w] (std::string year, float pt) {
        RVec<double> pnet_w = {1., 1., 1.};
        if (year != "2016postVFP") {
            return pnet_w;
        }
        pnet_w[0] *= cset_pnet_w->evaluate({pt, year, "nominal"});
        pnet_w[1] *= cset_pnet_w->evaluate({pt, year, "up"});
        pnet_w[2] *= cset_pnet_w->evaluate({pt, year, "down"});
        return pnet_w;
    };
    return df.Define("particlenet_w_weight_2016postVFP", eval_correction, {"sample_year", "GW_pt"});
}

RNode PNET_W_ScaleFactors_2017(correction::Correction::Ref cset_pnet_w, RNode df) {
    auto eval_correction = [cset_pnet_w] (std::string year, float pt) {
        RVec<double> pnet_w = {1., 1., 1.};
        if (year != "2017") {
            return pnet_w;
        }
        pnet_w[0] *= cset_pnet_w->evaluate({pt, year, "nominal"});
        pnet_w[1] *= cset_pnet_w->evaluate({pt, year, "up"});
        pnet_w[2] *= cset_pnet_w->evaluate({pt, year, "down"});
        return pnet_w;
    };
    return df.Define("particlenet_w_weight_2017", eval_correction, {"sample_year", "GW_pt"});
}

RNode PNET_W_ScaleFactors_2018(correction::Correction::Ref cset_pnet_w, RNode df) {
    auto eval_correction = [cset_pnet_w] (std::string year, float pt) {
        RVec<double> pnet_w = {1., 1., 1.};
        if (year != "2018") {
            return pnet_w;
        }
        pnet_w[0] *= cset_pnet_w->evaluate({pt, year, "nominal"});
        pnet_w[1] *= cset_pnet_w->evaluate({pt, year, "up"});
        pnet_w[2] *= cset_pnet_w->evaluate({pt, year, "down"});
        return pnet_w;
    };
    return df.Define("particlenet_w_weight_2018", eval_correction, {"sample_year", "GW_pt"});
}

RNode PNET_H_ScaleFactors_2016preVFP(correction::Correction::Ref cset_pnet_h, RNode df) {
    auto eval_correction = [cset_pnet_h] (std::string year, float pt) {
        RVec<double> pnet_h = {1., 1., 1.};
        if (year != "2016preVFP") {
            return pnet_h;
        }
        pnet_h[0] *= cset_pnet_h->evaluate({pt, year, "nominal"});
        pnet_h[1] *= cset_pnet_h->evaluate({pt, year, "up"});
        pnet_h[2] *= cset_pnet_h->evaluate({pt, year, "down"});
        return pnet_h;
    };
    return df.Define("particlenet_h_weight_2016preVFP", eval_correction, {"sample_year", "GHiggs_pt"});
}

RNode PNET_H_ScaleFactors_2016postVFP(correction::Correction::Ref cset_pnet_h, RNode df) {
    auto eval_correction = [cset_pnet_h] (std::string year, float pt) {
        RVec<double> pnet_h = {1., 1., 1.};
        if (year != "2016postVFP") {
            return pnet_h;
        }
        pnet_h[0] *= cset_pnet_h->evaluate({pt, year, "nominal"});
        pnet_h[1] *= cset_pnet_h->evaluate({pt, year, "up"});
        pnet_h[2] *= cset_pnet_h->evaluate({pt, year, "down"});
        return pnet_h;
    };
    return df.Define("particlenet_h_weight_2016postVFP", eval_correction, {"sample_year", "GHiggs_pt"});
}

RNode PNET_H_ScaleFactors_2017(correction::Correction::Ref cset_pnet_h, RNode df) {
    auto eval_correction = [cset_pnet_h] (std::string year, float pt) {
        RVec<double> pnet_h = {1., 1., 1.};
        if (year != "2017") {
            return pnet_h;
        }
        pnet_h[0] *= cset_pnet_h->evaluate({pt, year, "nominal"});
        pnet_h[1] *= cset_pnet_h->evaluate({pt, year, "up"});
        pnet_h[2] *= cset_pnet_h->evaluate({pt, year, "down"});
        return pnet_h;
    };
    return df.Define("particlenet_h_weight_2017", eval_correction, {"sample_year", "GHiggs_pt"});
}

RNode PNET_H_ScaleFactors_2018(correction::Correction::Ref cset_pnet_h, RNode df) {
    auto eval_correction = [cset_pnet_h] (std::string year, float pt) {
        RVec<double> pnet_h = {1., 1., 1.};
        if (year != "2018") {
            return pnet_h;
        }
        pnet_h[0] *= cset_pnet_h->evaluate({pt, year, "nominal"});
        pnet_h[1] *= cset_pnet_h->evaluate({pt, year, "up"});
        pnet_h[2] *= cset_pnet_h->evaluate({pt, year, "down"});
        return pnet_h;
    };
    return df.Define("particlenet_h_weight_2018", eval_correction, {"sample_year", "GHiggs_pt"});
}

RNode bTaggingScaleFactors(correction::CorrectionSet cset_btag_2016preVFP, correction::CorrectionSet cset_btag_2016postVFP, correction::CorrectionSet cset_btag_2017, correction::CorrectionSet cset_btag_2018, correction::CorrectionSet cset_btag_eff, RNode df) {
    auto eval_correction = [cset_btag_2016preVFP, cset_btag_2016postVFP, cset_btag_2017, cset_btag_2018, cset_btag_eff] (std::string year, const RVec<float> eta, const RVec<float> pt, const RVec<int> jetflavor) {
        // RVec<double> btag_sf_weights = {1., 1., 1.};
        double btag_sf_weight = 1.;
        if (eta.size() == 0) {
            // return btag_sf_weights;
            return btag_sf_weight;
        }
        float prod_tight = 1.;
        float prod_loose = 1.;
        float prod_notag = 1.;
        for (size_t i = 0; i < eta.size(); i++) {
            if (year == "2016preVFP") {
                float btag_sf_tight = -1.;
                float btag_sf_loose = -1.;
                float btag_eff_tight = -1.;
                float btag_eff_loose = -1.;
                if (jetflavor[i] == 5) {
                    btag_sf_tight = cset_btag_2016preVFP.at("deepCSV_comb")->evaluate({"central", "T", jetflavor[i], abs(eta[i]), pt[i]});
                    btag_sf_loose = cset_btag_2016preVFP.at("deepCSV_comb")->evaluate({"central", "L", jetflavor[i], abs(eta[i]), pt[i]});
                    btag_eff_tight = cset_btag_eff.at("btag_2016preVFP")->evaluate({"B", "T", pt[i], eta[i]});
                    btag_eff_loose = cset_btag_eff.at("btag_2016preVFP")->evaluate({"B", "L", pt[i], eta[i]});
                }
                else if (jetflavor[i] == 4) {
                    btag_sf_tight = cset_btag_2016preVFP.at("deepCSV_comb")->evaluate({"central", "T", jetflavor[i], abs(eta[i]), pt[i]});
                    btag_sf_loose = cset_btag_2016preVFP.at("deepCSV_comb")->evaluate({"central", "L", jetflavor[i], abs(eta[i]), pt[i]});
                    btag_eff_tight = cset_btag_eff.at("btag_2016preVFP")->evaluate({"C", "T", pt[i], eta[i]});
                    btag_eff_loose = cset_btag_eff.at("btag_2016preVFP")->evaluate({"C", "L", pt[i], eta[i]});
                }
                else {
                    btag_sf_tight = cset_btag_2016preVFP.at("deepCSV_incl")->evaluate({"central", "T", jetflavor[i], abs(eta[i]), pt[i]});
                    btag_sf_loose = cset_btag_2016preVFP.at("deepCSV_incl")->evaluate({"central", "L", jetflavor[i], abs(eta[i]), pt[i]});
                    btag_eff_tight = cset_btag_eff.at("btag_2016preVFP")->evaluate({"L", "T", pt[i], eta[i]});
                    btag_eff_loose = cset_btag_eff.at("btag_2016preVFP")->evaluate({"L", "L", pt[i], eta[i]});
                }
    }
    return df;
}

RNode finalMCWeight(RNode df){
    auto df_l1prefire = L1PreFiringWeight(df);
    auto df_pileup = pileupScaleFactors(cset_pileup_2016preVFP, cset_pileup_2016postVFP, cset_pileup_2017, cset_pileup_2018, df_l1prefire);
    auto df_pileupID = pileupIDScaleFactors(cset_pileupID_2016preVFP, cset_pileupID_2016postVFP, cset_pileupID_2017, cset_pileupID_2018, df_pileup);
    // muon sf
    auto df_muon_ID = muonScaleFactors_ID(cset_muon_ID_2016preVFP, cset_muon_ID_2016postVFP, cset_muon_ID_2017, cset_muon_ID_2018, df_pileupID);
    auto df_muon_trigger = muonScaleFactors_trigger(cset_muon_ID_2016preVFP, cset_muon_ID_2016postVFP, cset_muon_ID_2017, cset_muon_ID_2018, df_muon_ID);
    auto df_muon_ttHID = muonScaleFactors_ttH(cset_muon_ttHID, df_muon_trigger, "muon_scale_factors_ttHID");
    auto df_muon_ttHISO = muonScaleFactors_ttH(cset_muon_ttHISO, df_muon_ttHID, "muon_scale_factors_ttHISO");
    // elec sf
    auto df_elec_Reco = electronScaleFactors_Reco(cset_electron_ID_2016preVFP, cset_electron_ID_2016postVFP, cset_electron_ID_2017, cset_electron_ID_2018, df_muon_ttHISO);
    auto df_elec_ID = electronScaleFactors_ID(cset_electron_ID_2016preVFP, cset_electron_ID_2016postVFP, cset_electron_ID_2017, cset_electron_ID_2018, df_elec_Reco);
    auto df_elec_ttHID = electronScaleFactors_ttH(cset_electron_ttHID, df_elec_ID, "electron_scale_factors_ttHID");
    auto df_elec_ttHISO = electronScaleFactors_ttH(cset_electron_ttHISO, df_elec_ttHID, "electron_scale_factors_ttHISO");
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
    auto df_btag = bTaggingScaleFactors(cset_btag_2016preVFP, cset_btag_2016postVFP, cset_btag_2017, cset_btag_2018, df_pnet_h);
    
    return df_btag.Define("weight", 
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
            "HEMweight * "
            "xsec_weight * "
            "genWeight");
}

RNode finalDataWeight(RNode df){
    return df.Define("weight", 
            "goldenJSON * "
            "HEMweight");
}
#include "selections.h"

#include <iostream>

RNode analysisSelections(RNode df) {
    auto df_flags = df.Filter("Flag_goodVertices && "
                "(!isData || Flag_globalSuperTightHalo2016Filter) && "
                "Flag_HBHENoiseFilter && "
                "Flag_HBHENoiseIsoFilter && "
                "Flag_EcalDeadCellTriggerPrimitiveFilter && "
                "Flag_BadPFMuonFilter &&"
                "Flag_eeBadScFilter && "
                "Flag_BadPFMuonDzFilter && "
                "(!(is2017 || is2018) || Flag_ecalBadCalibFilter)", 
                "PASSED FLAGS");

    auto df_triggers = df_flags.Filter("(is2018 && (HLT_Ele32_WPTight_Gsf || HLT_IsoMu24)) || "
        "(is2017 && (HLT_Ele32_WPTight_Gsf_L1DoubleEG || HLT_IsoMu27)) ||"
        "(is2016 && (HLT_Ele27_eta2p1_WPTight_Gsf || HLT_IsoMu24 || HLT_IsoTkMu24))", 
        "PASSED TRIGGERS");

    auto df_el = df_triggers.Define("SC_absEta", "Electron_eta + Electron_deltaEtaSC")
            .Define("vetoElectrons", 
                "Electron_pt > 7 &&"
                "abs(SC_absEta) < 2.5 && "
                "abs(Electron_dxy) <= 0.05 && abs(Electron_dz) < 0.1 && "
                "abs(Electron_sip3d) < 8 && "
                "Electron_miniPFRelIso_all < 0.4 && "
                "Electron_lostHits <= 1 && "
                "Electron_mvaFall17V2noIso_WPL")
            .Define("nVetoElectrons", "Sum(vetoElectrons)")
            .Define("electronBjetScore", mediumDFBtagWP, {"sample_year"})
            .Define("electronNotFromBJet", "Jet_btagDeepFlavB[Electron_jetIdx[vetoElectrons][0]] < electronBjetScore")
            .Define("tightElectrons", "vetoElectrons &&" 
                "Electron_mvaTTHUL > 0.9 && "
                "Electron_pt > 35 && "
                "Electron_hoe < 0.1 && "
                "Electron_eInvMinusPInv > -0.04 && "
                "((SC_absEta <= 1.479 && Electron_sieie < 0.011) || ((SC_absEta > 1.479 && SC_absEta < 2.5) && Electron_sieie < 0.030)) && "
                "Electron_convVeto && "
                "Electron_tightCharge == 2 && "
                "Electron_lostHits == 0 && "
                "(Electron_jetIdx < 0 || (Electron_jetIdx >= 0 && Electron_jetIdx < nJet && electronNotFromBJet))")
            .Define("nTightElectrons", "Sum(tightElectrons)")
            .Define("GElectron_pt", "Electron_pt[tightElectrons][0]")
            .Define("GElectron_eta", "Electron_eta[tightElectrons][0]")
            .Define("GElectron_phi", "Electron_phi[tightElectrons][0]")
            .Define("GElectron_mass", "Electron_mass[tightElectrons][0]")
            .Define("isElectron", "nVetoElectrons == 1 && nTightElectrons == 1");
    
    auto df_mu = df_el.Define("vetoMuons", 
                "Muon_pt > 5 && "
                "abs(Muon_eta) < 2.4 && "
                "abs(Muon_dxy) < 0.05 && abs(Muon_dz) < 0.1 && "
                "abs(Muon_sip3d) < 8 && "
                "Muon_miniPFRelIso_all <= 0.4 && "
                "Muon_looseId")
            .Define("nVetoMuons", "Sum(vetoMuons)")
            .Define("muonBjetScore", mediumDFBtagWP, {"sample_year"})
            .Define("muNotFromBJet", "Jet_btagDeepFlavB[Muon_jetIdx[vetoMuons][0]] < muonBjetScore")
            .Define("tightMuons", "vetoMuons && "
                "Muon_mediumId && "
                "Muon_mvaTTHUL > 0.85 && "
                "Muon_pt > 35 && "
                "(Muon_jetIdx < 0 || (Muon_jetIdx >= 0 && Muon_jetIdx < nJet && muNotFromBJet))")
            .Define("nTightMuons", "Sum(tightMuons)")
            .Define("GMuon_pt", "Muon_pt[tightMuons][0]")
            .Define("GMuon_eta", "Muon_eta[tightMuons][0]")
            .Define("GMuon_phi", "Muon_phi[tightMuons][0]")
            .Define("GMuon_mass", "Muon_mass[tightMuons][0]")
            .Define("isMuon", "nVetoMuons == 1 && nTightMuons == 1");

    auto df_leps = df_mu.Define("GLepton_pt", "isElectron ? GElectron_pt : GMuon_pt")
            .Filter("(isElectron != isMuon) && (GLepton_pt > 40)", "ONLY ONE LEPTON PT > 40")
            .Define("GLepton_eta", "isElectron ? GElectron_eta : GMuon_eta")
            .Define("GLepton_phi", "isElectron ? GElectron_phi : GMuon_phi")
            .Define("GLepton_mass", "isElectron ? GElectron_mass : GMuon_mass");

    return df_leps;

    // auto df_higgs = df_leps.Define("HLepOverlap", fDeltaR, {"GLepton_eta", "GLepton_phi", "FatJet_eta", "FatJet_phi"})
    //         .Define("HCandidateJets", 
    //             "FatJet_pt > 250 && "
    //             "FatJet_mass > 50 && "
    //             "HLepOverlap >= 0.8 && "
    //             "abs(FatJet_eta) <= 2.5 && "
    //             "FatJet_msoftdrop > 40 && "
    //             "FatJet_jetId > 0")
    //         .Define("GHFatJet_particleNetMD_Xbb", "FatJet_particleNetMD_Xbb[HCandidateJets]")
    //         .Define("GHFatJet_particleNetMD_QCD", "FatJet_particleNetMD_QCD[HCandidateJets]")
    //         .Define("HScore", "GHFatJet_particleNetMD_Xbb / (GHFatJet_particleNetMD_Xbb + GHFatJet_particleNetMD_QCD)")
    //         .Define("HighestHScoreIdx", "HScore.size() != 0 ? ArgMax(HScore) : -1")
    //         .Define("HighestHScore", "HighestHScoreIdx != -1 ? HScore[HighestHScoreIdx] : -1")
    //         .Filter("HighestHScore > 0", "HIGGS CANDIDATE EXISTS")
    //         .Define("GHiggs_pt", "FatJet_pt[HCandidateJets][HighestHScoreIdx]")
    //         .Define("GHiggs_eta", "FatJet_eta[HCandidateJets][HighestHScoreIdx]")
    //         .Define("GHiggs_phi", "FatJet_phi[HCandidateJets][HighestHScoreIdx]")
    //         .Define("GHiggs_mass", "FatJet_mass[HCandidateJets][HighestHScoreIdx]");


    // auto df_wz = df_higgs.Define("QLepOverlap", fDeltaR, {"GLepton_eta", "GLepton_phi", "FatJet_eta", "FatJet_phi"})
    //         .Define("QHOverlap", fDeltaR, {"GHiggs_eta", "GHiggs_phi", "FatJet_eta", "FatJet_phi"})
    //         .Define("WZCandidateJets", "FatJet_pt > 250 && "
    //             "FatJet_mass > 50 && "
    //             "QLepOverlap >= 0.8 && "
    //             "QHOverlap >= 0.8 && "
    //             "abs(FatJet_eta) <= 2.5 && "
    //             "FatJet_msoftdrop > 40 && "
    //             "FatJet_jetId > 0")
    //         .Define("GWZFatJet_particleNetMD_Xbb", "FatJet_particleNetMD_Xbb[WZCandidateJets]")
    //         .Define("GWZFatJet_particleNetMD_Xcc", "FatJet_particleNetMD_Xcc[WZCandidateJets]")
    //         .Define("GWZFatJet_particleNetMD_Xqq", "FatJet_particleNetMD_Xqq[WZCandidateJets]")
    //         .Define("GWZFatJet_particleNetMD_QCD", "FatJet_particleNetMD_QCD[WZCandidateJets]")
    //         .Define("WScore", "(GWZFatJet_particleNetMD_Xqq + GWZFatJet_particleNetMD_Xcc) / (GWZFatJet_particleNetMD_Xqq + GWZFatJet_particleNetMD_Xcc + GWZFatJet_particleNetMD_QCD)")
    //         .Define("ZScore", "GWZFatJet_particleNetMD_Xbb / (GWZFatJet_particleNetMD_Xbb + GWZFatJet_particleNetMD_QCD)")
    //         .Define("HighestWjetScoreIdx", "WScore.size() != 0 ? ArgMax(WScore) : -1")
    //         .Define("HighestWjetScore", "HighestWjetScoreIdx != -1 ? WScore[HighestWjetScoreIdx] : -1")
    //         .Define("HighestZjetScoreIdx", "ZScore.size() != 0 ? ArgMax(ZScore) : -1")
    //         .Define("HighestZjetScore", "HighestZjetScoreIdx != -1 ? ZScore[HighestZjetScoreIdx] : -1")
    //         .Filter("HighestWjetScore > 0", "W CANDIDATE EXISTS")
    //         .Define("GW_pt", "FatJet_pt[WZCandidateJets][HighestWjetScoreIdx]")
    //         .Define("GW_eta", "FatJet_eta[WZCandidateJets][HighestWjetScoreIdx]")
    //         .Define("GW_phi", "FatJet_phi[WZCandidateJets][HighestWjetScoreIdx]")
    //         .Define("GW_mass", "FatJet_mass[WZCandidateJets][HighestWjetScoreIdx]");
    
    // auto df_ak4 = df_wz.Define("AK4LepOverlap", fDeltaR, {"GLepton_eta", "GLepton_phi", "Jet_eta", "Jet_phi"})
    //         .Define("AK4HOverlap", fDeltaR, {"GHiggs_eta", "GHiggs_phi", "Jet_eta", "Jet_phi"})
    //         .Define("AK4WOverlap", fDeltaR, {"GW_eta", "GW_phi", "Jet_eta", "Jet_phi"})
    //         .Define("ak4tightBjetScore", tightDFBtagWP, {"sample_year"})
    //         .Define("goodJets", "Jet_pt >= 20 && "
    //             "abs(Jet_eta) <= 2.5 && "
    //             "AK4LepOverlap >= 0.4 && "
    //             "AK4HOverlap >= 0.8 && "
    //             "AK4WOverlap >= 0.8 && "
    //             "((is2016 && Jet_jetId >= 1) || (!is2016 && Jet_jetId >= 2)) && "
    //             "(Jet_pt >= 50 || (Jet_pt < 50 && Jet_puId != 0))")
    //         .Define("ak4FromBJet", "goodJets && Jet_btagDeepFlavB > ak4tightBjetScore")
    //         .Filter("Sum(ak4FromBJet) == 0", "NO B-TAGGED AK4 JETS");
            
    // auto df_vbs = df_ak4.Define("goodVBSJets", "Jet_pt >= 30 && "
    //             "abs(Jet_eta) <= 4.7 && "
    //             "AK4LepOverlap >= 0.4 && "
    //             "AK4HOverlap >= 0.8 && "
    //             "AK4WOverlap >= 0.8 && "
    //             "((is2016 && Jet_jetId >= 1) || (!is2016 && Jet_jetId >= 2)) && "
    //             "(Jet_pt >= 50 || (Jet_pt < 50 && Jet_puId != 0))")
    //         .Filter("Sum(goodVBSJets) >= 2", "AT LEAST TWO VBS JETS")
    //         .Define("VBSJets_pt", "Jet_pt[goodVBSJets]")
    //         .Define("VBSJets_eta", "Jet_eta[goodVBSJets]")
    //         .Define("VBSJets_phi", "Jet_phi[goodVBSJets]")
    //         .Define("VBSJets_mass", "Jet_mass[goodVBSJets]")
    //         .Define("sortedVBSJets", "Argsort(-VBSJets_pt)")
    //         .Define("VBSJet1_pt", "VBSJets_pt[sortedVBSJets[0]]")
    //         .Define("VBSJet2_pt", "VBSJets_pt[sortedVBSJets[1]]")
    //         .Define("VBSJet1_eta", "VBSJets_eta[sortedVBSJets[0]]")
    //         .Define("VBSJet2_eta", "VBSJets_eta[sortedVBSJets[1]]")
    //         .Define("VBSJet1_phi", "VBSJets_phi[sortedVBSJets[0]]")
    //         .Define("VBSJet2_phi", "VBSJets_phi[sortedVBSJets[1]]")
    //         .Define("VBSJet1_mass", "VBSJets_mass[sortedVBSJets[0]]")
    //         .Define("VBSJet2_mass", "VBSJets_mass[sortedVBSJets[1]]");

    // auto df_st = df_vbs.Define("ST", "GLepton_pt + GHiggs_pt + GW_pt + MET_pt")
    //         .Filter("ST > 1000", "ST > 1000");

    // return df_st;
}
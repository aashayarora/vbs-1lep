#include "selections.h"

RNode analysisSelections(RNode df) {
    auto df_flags = df.Define("isData", "sample_category == \"data\"")
            .Define("is2016", "sample_year == \"2016preVFP\" || sample_year == \"2016postVFP\"")
            .Define("is2017", "sample_year == \"2017\"")
            .Define("is2018", "sample_year == \"2018\"")
            .Filter("Flag_goodVertices && "
                "(!isData || Flag_globalSuperTightHalo2016Filter) && "
                "Flag_HBHENoiseFilter && "
                "Flag_HBHENoiseIsoFilter && "
                "Flag_EcalDeadCellTriggerPrimitiveFilter && "
                "Flag_BadPFMuonFilter &&"
                "Flag_eeBadScFilter && "
                "Flag_BadPFMuonDzFilter && "
                "(!is2017 || Flag_ecalBadCalibFilter)", 
                "PASSED FLAGS");

    auto df_triggers = df_flags.Filter("(is2018 && (HLT_Ele32_WPTight_Gsf || HLT_IsoMu24)) || "
        "(is2017 && (HLT_Ele32_WPTight_Gsf_L1DoubleEG || HLT_IsoMu27)) ||"
        "(is2016 && (HLT_Ele27_eta2p1_WPTight_Gsf || HLT_IsoMu24 || HLT_IsoTkMu24))", 
        "PASSED TRIGGERS");

    auto df_el = df_triggers.Define("SC_absEta", "abs(Electron_eta + Electron_deltaEtaSC)")
            .Define("vetoElectrons", 
                "Electron_pt > 7 &&"
                "SC_absEta < 2.5 && "
                "abs(Electron_dxy) <= 0.05 && abs(Electron_dz) < 0.1 && "
                "abs(Electron_sip3d) < 8 && "
                "Electron_miniPFRelIso_all < 0.4 && "
                "Electron_lostHits <= 1 && "
                "Electron_mvaFall17V2noIso_WPL == 1")
            .Define("nVetoElectrons", "Sum(vetoElectrons)")
            .Define("electronBjetScore", mediumDFBtagWP, {"sample_year"})
            .Define("electronNotFromBJet", "Jet_btagDeepFlavB[Electron_jetIdx[vetoElectrons][0]] < electronBjetScore")
            .Define("tightElectrons", "vetoElectrons &&" 
                "Electron_pt > 40 && "
                "Electron_convVeto == 1 && "
                "Electron_tightCharge == 2 && "
                "Electron_lostHits == 0 && "
                "Electron_hoe < 0.1 && "
                "Electron_eInvMinusPInv > -0.04 && "
                "Electron_mvaTTHUL > 0.9 && "
                "((SC_absEta <= 1.479 && Electron_sieie < 0.011) || ((SC_absEta > 1.479 && SC_absEta < 2.5) && Electron_sieie < 0.030) || SC_absEta >= 2.5) && "
                "(Electron_jetIdx == 0 || (Electron_jetIdx >= 0 && Electron_jetIdx < nJet && electronNotFromBJet))")
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
                "Muon_miniPFRelIso_all < 0.4 && "
                "Muon_looseId == 1")
            .Define("nVetoMuons", "Sum(vetoMuons)")
            .Define("muonBjetScore", mediumDFBtagWP, {"sample_year"})
            .Define("muNotFromBJet", "Jet_btagDeepFlavB[Muon_jetIdx[vetoMuons][0]] < muonBjetScore")
            .Define("tightMuons", "vetoMuons && "
                "Muon_pt > 40 && "
                "Muon_jetIdx == 0 || (Muon_jetIdx >= 0 && Muon_jetIdx < nJet && muNotFromBJet)")
            .Define("nTightMuons", "Sum(tightMuons)")
            .Define("GMuon_pt", "Muon_pt[tightMuons][0]")
            .Define("GMuon_eta", "Muon_eta[tightMuons][0]")
            .Define("GMuon_phi", "Muon_phi[tightMuons][0]")
            .Define("GMuon_mass", "Muon_mass[tightMuons][0]")
            .Define("isMuon", "nVetoMuons == 1 && nTightMuons == 1");

    auto df_leps = df_mu.Filter("isElectron != isMuon", "EXACTLY ONE LEPTON WITH PT > 40")
            .Define("GLepton_pt", lepselect, {"isElectron", "GElectron_pt", "GMuon_pt"})
            .Define("GLepton_eta", lepselect, {"isElectron", "GElectron_eta", "GMuon_eta"})
            .Define("GLepton_phi", lepselect, {"isElectron", "GElectron_phi", "GMuon_phi"})
            .Define("GLepton_mass", lepselect, {"isElectron", "GElectron_mass", "GMuon_mass"});

    auto df_higgs = df_leps.Define("noHLepOverlap", noJetOverlap8, {"GLepton_eta", "GLepton_phi", "FatJet_eta", "FatJet_phi"})
            .Define("HCandidateJets", 
                "FatJet_pt > 250 && "
                "FatJet_mass > 50 && "
                "noHLepOverlap == 1 && "
                "abs(FatJet_eta) <= 2.5 && "
                "FatJet_msoftdrop > 40 && "
                "FatJet_jetId > 0")
            .Define("GHFatJet_particleNetMD_Xbb", "FatJet_particleNetMD_Xbb[HCandidateJets]")
            .Define("GHFatJet_particleNetMD_QCD", "FatJet_particleNetMD_QCD[HCandidateJets]")
            .Define("HScore", "GHFatJet_particleNetMD_Xbb / (GHFatJet_particleNetMD_Xbb + GHFatJet_particleNetMD_QCD)")
            .Define("HighestHScoreIdx", MyArgMax, {"HScore"})
            .Define("HighestHScore", MyMax, {"HScore"})
            .Filter("Sum(HighestHScore > 0) == 1", "HIGGS CANDIDATE EXISTS")
            .Define("GHiggs_pt", "FatJet_pt[HCandidateJets][HighestHScoreIdx[0]]")
            .Define("GHiggs_eta", "FatJet_eta[HCandidateJets][HighestHScoreIdx[0]]")
            .Define("GHiggs_phi", "FatJet_phi[HCandidateJets][HighestHScoreIdx[0]]")
            .Define("GHiggs_mass", "FatJet_mass[HCandidateJets][HighestHScoreIdx[0]]");

    auto df_wz = df_higgs.Define("noQLepOverlap", noJetOverlap8, {"GLepton_eta", "GLepton_phi", "FatJet_eta", "FatJet_phi"})
            .Define("noQHOverlap", noJetOverlap8, {"GHiggs_eta", "GHiggs_phi", "FatJet_eta", "FatJet_phi"})
            .Define("WZCandidateJets", "FatJet_pt > 250 && "
                "FatJet_mass > 50 && "
                "noQLepOverlap == 1 && "
                "noQHOverlap == 1 && "
                "abs(FatJet_eta) <= 2.5 && "
                "FatJet_msoftdrop > 40 && "
                "FatJet_jetId > 0")
            .Define("GWZFatJet_particleNetMD_Xbb", "FatJet_particleNetMD_Xbb[WZCandidateJets]")
            .Define("GWZFatJet_particleNetMD_Xcc", "FatJet_particleNetMD_Xcc[WZCandidateJets]")
            .Define("GWZFatJet_particleNetMD_Xqq", "FatJet_particleNetMD_Xqq[WZCandidateJets]")
            .Define("GWZFatJet_particleNetMD_QCD", "FatJet_particleNetMD_QCD[WZCandidateJets]")
            .Define("WScore", "(GWZFatJet_particleNetMD_Xqq + GWZFatJet_particleNetMD_Xcc) / (GWZFatJet_particleNetMD_Xqq + GWZFatJet_particleNetMD_Xcc + GWZFatJet_particleNetMD_QCD)")
            .Define("ZScore", "GWZFatJet_particleNetMD_Xbb / (GWZFatJet_particleNetMD_Xbb + GWZFatJet_particleNetMD_QCD)")
            .Define("HighestWjetScoreIdx", MyArgMax, {"WScore"})
            .Define("HighestWjetScore", MyMax, {"WScore"})
            .Define("HighestZjetScoreIdx", MyArgMax, {"ZScore"})
            .Define("HighestZjetScore", MyMax, {"ZScore"})
            .Filter("Sum(HighestWjetScore > 0) == 1", "W CANDIDATE EXISTS")
            .Define("GW_pt", "FatJet_pt[WZCandidateJets][HighestWjetScoreIdx[0]]")
            .Define("GW_eta", "FatJet_eta[WZCandidateJets][HighestWjetScoreIdx[0]]")
            .Define("GW_phi", "FatJet_phi[WZCandidateJets][HighestWjetScoreIdx[0]]")
            .Define("GW_mass", "FatJet_mass[WZCandidateJets][HighestWjetScoreIdx[0]]");
    
    auto df_ak4 = df_wz.Define("noAK4LepOverlap", noJetOverlap4, {"GLepton_eta", "GLepton_phi", "Jet_eta", "Jet_phi"})
            .Define("noAK4HOverlap", noJetOverlap8, {"GHiggs_eta", "GHiggs_phi", "Jet_eta", "Jet_phi"})
            .Define("noAK4WOverlap", noJetOverlap8, {"GW_eta", "GW_phi", "Jet_eta", "Jet_phi"})
            .Define("ak4tightBjetScore", tightDFBtagWP, {"sample_year"})
            .Define("goodJets", "Jet_pt >= 20 && "
                "abs(Jet_eta) <= 2.5 && "
                "noAK4LepOverlap == 1 && "
                "noAK4HOverlap == 1 && "
                "noAK4WOverlap == 1 && "
                "((is2016 && Jet_jetId >= 1) || (!is2016 && Jet_jetId >= 2)) && "
                "(Jet_pt >= 50 || (Jet_pt < 50 && Jet_puId != 0))")
            .Define("ak4FromBJet", "goodJets && Jet_btagDeepFlavB > ak4tightBjetScore")
            .Filter("Sum(ak4FromBJet) == 0", "NO B-TAGGED AK4 JETS");
            
    auto df_vbs = df_ak4.Define("goodVBSJets", "Jet_pt >= 30 && "
                "abs(Jet_eta) <= 4.7 && "
                "noAK4LepOverlap == 1 && "
                "noAK4HOverlap == 1 && "
                "noAK4WOverlap == 1 && "
                "((is2016 && Jet_jetId >= 1) || (!is2016 && Jet_jetId >= 2)) && "
                "(Jet_pt >= 50 || (Jet_pt < 50 && Jet_puId != 0))")
            .Filter("Sum(goodVBSJets) >= 2", "AT LEAST TWO VBS JETS")
            .Define("VBSJets_pt", "Jet_pt[goodVBSJets]")
            .Define("VBSJets_eta", "Jet_eta[goodVBSJets]")
            .Define("VBSJets_phi", "Jet_phi[goodVBSJets]")
            .Define("VBSJets_mass", "Jet_mass[goodVBSJets]")
            .Define("sortedVBSJets", "Argsort(-VBSJets_pt)")
            .Define("VBSJet1_pt", "VBSJets_pt[sortedVBSJets[0]]")
            .Define("VBSJet2_pt", "VBSJets_pt[sortedVBSJets[1]]")
            .Define("VBSJet1_eta", "VBSJets_eta[sortedVBSJets[0]]")
            .Define("VBSJet2_eta", "VBSJets_eta[sortedVBSJets[1]]")
            .Define("VBSJet1_phi", "VBSJets_phi[sortedVBSJets[0]]")
            .Define("VBSJet2_phi", "VBSJets_phi[sortedVBSJets[1]]")
            .Define("VBSJet1_mass", "VBSJets_mass[sortedVBSJets[0]]")
            .Define("VBSJet2_mass", "VBSJets_mass[sortedVBSJets[1]]");

    auto df_st = df_vbs.Filter("(GLepton_pt + GHiggs_pt + GW_pt + MET_pt) > 1000", "ST > 1000");

    auto df_final = df_st;
    
    return df_final;
}
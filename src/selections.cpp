#include "selections.h"

RNode analysisSelections(RNode df) {
    auto df_flags = df.Define("isData", "sample_category == \"data\"")
            .Define("is2017or2018", "sample_year == \"2017\" || sample_year == \"2018\"")
            .Filter("Flag_goodVertices && "
                "((isData && Flag_globalSuperTightHalo2016Filter) || !isData) && "
                "Flag_HBHENoiseFilter && "
                "Flag_HBHENoiseIsoFilter && "
                "Flag_EcalDeadCellTriggerPrimitiveFilter && "
                "Flag_BadPFMuonFilter &&"
                "Flag_eeBadScFilter && "
                "Flag_BadPFMuonDzFilter && "
                "(is2017or2018 && Flag_ecalBadCalibFilter) || !is2017or2018", "passes flags");

    auto df_triggers = df_flags.Filter("(sample_year == \"2018\" && (HLT_Ele32_WPTight_Gsf || HLT_IsoMu24)) || "
        "(sample_year == \"2017\" && (HLT_Ele32_WPTight_Gsf_L1DoubleEG || HLT_IsoMu27)) ||"
        "((sample_year == \"2016preVFP\" || sample_year == \"2016postVFP\") && (HLT_Ele27_eta2p1_WPTight_Gsf || HLT_IsoMu24 || HLT_IsoTkMu24))", "passes triggers");

    auto df_el = df_triggers.Define("vetoElectrons", 
                "Electron_pt > 7 &&"
                "abs(Electron_eta + Electron_deltaEtaSC) < 2.5 && "
                "abs(Electron_dxy) <= 0.05 && abs(Electron_dz) < 0.1 && "
                "abs(Electron_sip3d) < 8 && "
                "Electron_miniPFRelIso_all < 0.4 && "
                "Electron_lostHits <= 1 && "
                "Electron_mvaFall17V2noIso_WPL == 1")
            .Define("nVetoElectrons", "Sum(vetoElectrons)")
            .Define("SC_absEta", "abs(Electron_eta + Electron_deltaEtaSC)")
            .Define("electronBjetScore", mediumDFBtagWP, {"sample_year"})
            .Define("electronNotFromBJet", "Jet_btagDeepFlavB[Take(Electron_jetIdx, vetoElectrons)[0]] < electronBjetScore")
            .Define("tightElectrons", "vetoElectrons &&" 
                "Electron_pt > 10 && "
                "Electron_convVeto == 1 && "
                "Electron_tightCharge == 2 && "
                "Electron_lostHits == 0 && "
                "Electron_hoe < 0.1 && "
                "Electron_eInvMinusPInv > -0.04 && "
                "Electron_mvaTTHUL > 0.9 && "
                "(SC_absEta <= 1.479 && Electron_sieie < 0.011) || ((SC_absEta > 1.479 && SC_absEta < 2.5) && Electron_sieie < 0.030) &&"
                "Electron_jetIdx == 0 || (Electron_jetIdx >= 0 && Electron_jetIdx < nJet && electronNotFromBJet)")
            .Define("nTightElectrons", "Sum(tightElectrons)")
            .Define("GElectron_pt", "Take(Electron_pt, tightElectrons)")
            .Define("GElectron_eta", "Take(Electron_eta, tightElectrons)")
            .Define("GElectron_phi", "Take(Electron_phi, tightElectrons)")
            .Define("GElectron_mass", "Take(Electron_mass, tightElectrons)")
            .Define("isElectron", "nTightElectrons == 1");
    
    auto df_mu = df_el.Define("vetoMuons", "Muon_pt > 5 && "
                "abs(Muon_eta) < 2.4 && "
                "abs(Muon_dxy) <= 0.05 && abs(Muon_dz) < 0.1 && "
                "abs(Muon_sip3d) < 8 && "
                "Muon_miniPFRelIso_all < 0.4 && "
                "Muon_looseId == 1")
            .Define("nVetoMuons", "Sum(vetoMuons)")
            .Define("muonBjetScore", mediumDFBtagWP, {"sample_year"})
            .Define("muNotFromBJet", "Jet_btagDeepFlavB[Take(Muon_jetIdx, vetoMuons)[0]] < muonBjetScore")
            .Define("tightMuons", "vetoMuons && "
                "Muon_pt > 10 && "
                "Muon_jetIdx == 0 || (Muon_jetIdx >= 0 && Muon_jetIdx < nJet && muNotFromBJet)")
            .Define("nTightMuons", "Sum(tightMuons)")
            .Define("GMuon_pt", "Take(Muon_pt, tightMuons)")
            .Define("GMuon_eta", "Take(Muon_eta, tightMuons)")
            .Define("GMuon_phi", "Take(Muon_phi, tightMuons)")
            .Define("GMuon_mass", "Take(Muon_mass, tightMuons)")
            .Define("isMuon", "nTightMuons == 1");

    auto df_leps = df_mu.Define("nGLepton", "nTightElectrons + nTightMuons")
            .Define("GLepton_pt", lepselect, {"isElectron", "GElectron_pt", "GMuon_pt"})
            .Define("GLepton_eta", lepselect, {"isElectron", "GElectron_eta", "GMuon_eta"})
            .Define("GLepton_phi", lepselect, {"isElectron", "GElectron_phi", "GMuon_phi"})
            .Define("GLepton_mass", lepselect, {"isElectron", "GElectron_mass", "GMuon_mass"})
            .Filter("nGLepton == 1 && GLepton_pt[0] > 40", "only one lepton with pt > 40");

    auto df_higgs = df_leps.Define("noHLepOverlap", noJetOverlap8, {"GLepton_eta", "GLepton_phi", "FatJet_eta", "FatJet_phi"})
            .Define("HCandidateJets", "FatJet_pt > 250 && "
                "FatJet_mass > 50 && "
                "noHLepOverlap == 1&& "
                "abs(FatJet_eta) <= 2.5 && "
                "FatJet_msoftdrop > 40 && "
                "FatJet_jetId > 0")
            .Define("GHFatJet_particleNetMD_Xbb", "Take(FatJet_particleNetMD_Xbb, HCandidateJets)")
            .Define("GHFatJet_particleNetMD_QCD", "Take(FatJet_particleNetMD_QCD, HCandidateJets)")
            .Define("HScore", "GHFatJet_particleNetMD_Xbb / (GHFatJet_particleNetMD_Xbb + GHFatJet_particleNetMD_QCD)")
            .Define("HighestXbbScoreIdx", "ArgMax(HScore)")
            .Define("HighestXbbScore", "HScore[HighestXbbScoreIdx]")
            .Filter("HighestXbbScore > 0", "Higgs candidate exists")
            .Define("GHiggs_pt", "Take(Take(FatJet_pt, HCandidateJets), HighestXbbScoreIdx)")
            .Define("GHiggs_eta", "Take(Take(FatJet_eta, HCandidateJets), HighestXbbScoreIdx)")
            .Define("GHiggs_phi", "Take(Take(FatJet_phi, HCandidateJets), HighestXbbScoreIdx)")
            .Define("GHiggs_mass", "Take(Take(FatJet_mass, HCandidateJets), HighestXbbScoreIdx)");

    auto df_wz = df_higgs.Define("noQLepOverlap", noJetOverlap8, {"GLepton_eta", "GLepton_phi", "FatJet_eta", "FatJet_phi"})
            .Define("noQHOverlap", noJetOverlap8, {"GHiggs_eta", "GHiggs_phi", "FatJet_eta", "FatJet_phi"})
            .Define("WZCandidateJets", "FatJet_pt > 250 && "
                "FatJet_mass > 50 && "
                "noQLepOverlap == 1 && "
                "noQHOverlap == 1 && "
                "abs(FatJet_eta) <= 2.5 && "
                "FatJet_msoftdrop > 40 && "
                "FatJet_jetId > 0")
            .Define("GWZFatJet_particleNetMD_Xbb", "Take(FatJet_particleNetMD_Xbb, WZCandidateJets)")
            .Define("GWZFatJet_particleNetMD_Xqq", "Take(FatJet_particleNetMD_Xqq, WZCandidateJets)")
            .Define("GWZFatJet_particleNetMD_Xcc", "Take(FatJet_particleNetMD_Xcc, WZCandidateJets)")
            .Define("GWZFatJet_particleNetMD_QCD", "Take(FatJet_particleNetMD_QCD, WZCandidateJets)")
            .Define("WScore", "(GWZFatJet_particleNetMD_Xqq + GWZFatJet_particleNetMD_Xcc) / (GWZFatJet_particleNetMD_Xqq + GWZFatJet_particleNetMD_Xcc + GWZFatJet_particleNetMD_QCD)")
            .Define("ZScore", "GWZFatJet_particleNetMD_Xbb / (GWZFatJet_particleNetMD_Xbb + GWZFatJet_particleNetMD_QCD)")
            .Define("HighestWjetScoreIdx", "ArgMax(WScore)")
            .Define("HighestWjetScore", "WScore[HighestWjetScoreIdx]")
            .Define("HighestZjetScoreIdx", "ArgMax(ZScore)")
            .Define("HighestZjetScore", "ZScore[HighestZjetScoreIdx]")
            .Filter("HighestWjetScore > 0", "W candidate exists")
            .Define("GW_pt", "Take(Take(FatJet_pt, WZCandidateJets), HighestWjetScoreIdx)")
            .Define("GW_eta", "Take(Take(FatJet_eta, WZCandidateJets), HighestWjetScoreIdx)")
            .Define("GW_phi", "Take(Take(FatJet_phi, WZCandidateJets), HighestWjetScoreIdx)")
            .Define("GW_mass", "Take(Take(FatJet_mass, WZCandidateJets), HighestWjetScoreIdx)");
    
    // TODO: ak4Jet HEM15/16 correction

    auto df_ak4 = df_wz.Define("noAK4LepOverlap", noJetOverlap4, {"GLepton_eta", "GLepton_phi", "Jet_eta", "Jet_phi"})
            .Define("noAK4HOverlap", noJetOverlap8, {"GHiggs_eta", "GHiggs_phi", "Jet_eta", "Jet_phi"})
            .Define("noAK4WOverlap", noJetOverlap8, {"GW_eta", "GW_phi", "Jet_eta", "Jet_phi"})
            .Define("ak4tightBjetScore", tightDFBtagWP, {"sample_year"})
            .Define("goodJets", "Jet_pt >= 20 && "
                "abs(Jet_eta) <= 2.5 && "
                "noAK4LepOverlap == 1 && "
                "noAK4HOverlap == 1 && "
                "noAK4WOverlap == 1 && "
                "((sample_year == \"2016preVFP\" || sample_year == \"2016postVFP\") && Jet_jetId >= 1) || Jet_jetId >= 2 && "
                "Jet_pt >= 50 || (Jet_pt < 50 && Jet_puId > 0)")
            .Define("ak4FromBJet", "goodJets && Jet_btagDeepFlavB > ak4tightBjetScore")
            .Filter("Sum(ak4FromBJet) == 0", "no b-tagged AK4 jets");
            
    auto df_vbs = df_ak4.Define("goodVBSJets", "Jet_pt >= 30 && "
                "abs(Jet_eta) <= 4.7 && "
                "noAK4LepOverlap == 1 && "
                "noAK4HOverlap == 1 && "
                "noAK4WOverlap == 1 && "
                "((sample_year == \"2016preVFP\" || sample_year == \"2016postVFP\") && Jet_jetId >= 1) || Jet_jetId >= 2")
            .Filter("Sum(goodVBSJets) >= 2", "at least 2 VBS jets")
            .Define("VBSJets_pt", "Take(Jet_pt, goodVBSJets)")
            .Define("sortedVBSJets", "Argsort(VBSJets_pt)")
            .Define("VBSJet1_pt", "Take(Jet_pt, sortedVBSJets)[-1]")
            .Define("VBSJet1_eta", "Take(Jet_eta, sortedVBSJets)[-1]")
            .Define("VBSJet1_phi", "Take(Jet_phi, sortedVBSJets)[-1]")
            .Define("VBSJet1_mass", "Take(Jet_mass, sortedVBSJets)[-1]")
            .Define("VBSJet2_pt", "Take(Jet_pt, sortedVBSJets)[-2]")
            .Define("VBSJet2_eta", "Take(Jet_eta, sortedVBSJets)[-2]")
            .Define("VBSJet2_phi", "Take(Jet_phi, sortedVBSJets)[-2]")
            .Define("VBSJet2_mass", "Take(Jet_mass, sortedVBSJets)[-2]");

    auto df_st = df_vbs.Filter("(GLepton_pt[0] + GHiggs_pt[0] + GW_pt[0] + MET_pt) > 1000", "ST > 1000");

    auto df_final = df_st;
    
    return df_final;
}
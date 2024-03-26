#include "selections.h"

RNode electronSelections(RNode df) {
    double mediumDFBtagWP = get_btag_workingpoint("2016preVFP", "medium");
    return df.Define("vetoElectrons", 
                "Electron_pt > 7 &&"
                "abs(Electron_eta + Electron_deltaEtaSC) < 2.5 && "
                "abs(Electron_dxy) <= 0.05 && abs(Electron_dz) < 0.1 && "
                "abs(Electron_sip3d) < 8 && "
                "Electron_miniPFRelIso_all < 0.4 && "
                "Electron_lostHits <= 1 && "
                "Electron_mvaFall17V2noIso_WPL == 1")
            .Define("nVetoElectrons", "Sum(vetoElectrons)")
            .Define("SC_absEta", "abs(Electron_eta + Electron_deltaEtaSC)")
            .Define("tightElectrons", Form("vetoElectrons &&" 
                "Electron_pt > 10 && "
                "Electron_convVeto == 1 && "
                "Electron_tightCharge == 2 && "
                "Electron_lostHits == 0 && "
                "Electron_hoe < 0.1 && "
                "Electron_eInvMinusPInv > -0.04 && "
                "Electron_mvaTTHUL > 0.9 && "
                "(SC_absEta <= 1.479 && Electron_sieie < 0.011) || ((SC_absEta > 1.479 && SC_absEta < 2.5) && "
                "Electron_sieie < 0.030) && Electron_jetIdx == 0 || (Electron_jetIdx >= 0 && Electron_jetIdx < nJet && Jet_btagDeepFlavB[Electron_jetIdx[vetoElectrons][0]] < %f)", mediumDFBtagWP))
            .Define("nTightElectrons", "Sum(tightElectrons)");
}

RNode muonSelections(RNode df) {
    double mediumDFBtagWP = get_btag_workingpoint("2016preVFP", "medium");
    return df.Define("vetoMuons", "Muon_pt > 5 && "
                "abs(Muon_eta) < 2.4 && "
                "abs(Muon_dxy) <= 0.05 && abs(Muon_dz) < 0.1 && "
                "abs(Muon_sip3d) < 8 && "
                "Muon_miniPFRelIso_all < 0.4 && "
                "Muon_looseId == 1")
            .Define("nVetoMuons", "Sum(vetoMuons)")
            .Define("tightMuons", Form("vetoMuons && "
                "Muon_pt > 10 && "
                "Muon_jetIdx == 0 || (Muon_jetIdx >= 0 && Muon_jetIdx < nJet && Jet_btagDeepFlavB[Muon_jetIdx[vetoMuons][0]] < %f)", mediumDFBtagWP))
            .Define("nTightMuons", "Sum(tightMuons)");
}

RNode jetSelections(RNode df){
    
}

RNode eventSelections(RNode df) {
    return df.Filter("(nVetoElectrons == 1 && nTightElectrons == 1) || (nVetoMuons == 1 && nTightMuons == 1)", "only one lepton")
             .Filter("Electron_pt[tightElectrons][0] > 40 || Muon_pt[tightMuons][0] > 40", "lepton pt > 40 GeV");
}
#ifndef WEIGHTS_H
#define WEIGHTS_H

#pragma once

#include "ROOT/RDataFrame.hxx"
#include "ROOT/RDFHelpers.hxx"
#include "ROOT/RVec.hxx"

#include "utils.h"
#include "correction.h"

using correction::CorrectionSet;
using RNode = ROOT::RDF::RNode;
using ROOT::VecOps::RVec;

    
// golden json lumimask
const auto LumiMask = lumiMask::fromJSON("corrections/goldenJson/Cert_271036-325175_13TeV_allRun2_JSON.json");

// // pileup correction
const auto cset_pileup_2016preVFP = CorrectionSet::from_file("corrections/pileup/2016preVFP_puWeights.json")->at("Collisions16_UltraLegacy_goldenJSON");
const auto cset_pileup_2016postVFP = CorrectionSet::from_file("corrections/pileup/2016postVFP_puWeights.json")->at("Collisions16_UltraLegacy_goldenJSON");
const auto cset_pileup_2017 = CorrectionSet::from_file("corrections/pileup/2017_puWeights.json")->at("Collisions17_UltraLegacy_goldenJSON");
const auto cset_pileup_2018 = CorrectionSet::from_file("corrections/pileup/2018_puWeights.json")->at("Collisions18_UltraLegacy_goldenJSON");

// // muon scale factors
const auto cset_muon_ID_2016preVFP = CorrectionSet::from_file("corrections/scalefactors/muon/2016preVFP_muon_ID.json")->at("NUM_IsoMu24_or_IsoTkMu24_DEN_CutBasedIdTight_and_PFIsoTight");
const auto cset_muon_ID_2016postVFP = CorrectionSet::from_file("corrections/scalefactors/muon/2016postVFP_muon_ID.json")->at("NUM_IsoMu24_or_IsoTkMu24_DEN_CutBasedIdTight_and_PFIsoTight");
const auto cset_muon_ID_2017 = CorrectionSet::from_file("corrections/scalefactors/muon/2017_muon_ID.json")->at("NUM_IsoMu27_DEN_CutBasedIdTight_and_PFIsoTight");
const auto cset_muon_ID_2018 = CorrectionSet::from_file("corrections/scalefactors/muon/2018_muon_ID.json")->at("NUM_IsoMu24_DEN_CutBasedIdTight_and_PFIsoTight");

const auto cset_muon_ttHID_2016preVFP = CorrectionSet::from_file("corrections/scalefactors/muon/2016preVFP_ttH_MuonID_SF.json")->at("2016preVFP_ttH_MuonID_SF");
const auto cset_muon_ttHID_2016postVFP = CorrectionSet::from_file("corrections/scalefactors/muon/2016postVFP_ttH_MuonID_SF.json")->at("2016postVFP_ttH_MuonID_SF");
const auto cset_muon_ttHID_2017 = CorrectionSet::from_file("corrections/scalefactors/muon/2017_ttH_MuonID_SF.json")->at("2017_ttH_MuonID_SF");
const auto cset_muon_ttHID_2018 = CorrectionSet::from_file("corrections/scalefactors/muon/2018_ttH_MuonID_SF.json")->at("2018_ttH_MuonID_SF");

const auto cset_muon_ttHISO_2016preVFP = CorrectionSet::from_file("corrections/scalefactors/muon/2016preVFP_ttH_MuonISO_SF.json")->at("2016preVFP_ttH_MuonISO_SF");
const auto cset_muon_ttHISO_2016postVFP = CorrectionSet::from_file("corrections/scalefactors/muon/2016postVFP_ttH_MuonISO_SF.json")->at("2016postVFP_ttH_MuonISO_SF");
const auto cset_muon_ttHISO_2017 = CorrectionSet::from_file("corrections/scalefactors/muon/2017_ttH_MuonISO_SF.json")->at("2017_ttH_MuonISO_SF");
const auto cset_muon_ttHISO_2018 = CorrectionSet::from_file("corrections/scalefactors/muon/2018_ttH_MuonISO_SF.json")->at("2018_ttH_MuonISO_SF");

// electron scale factors
const auto cset_electron_ID_2016preVFP = CorrectionSet::from_file("corrections/scalefactors/electron/2016preVFP_electron_ID.json")->at("UL-Electron-ID-SF");
const auto cset_electron_ID_2016postVFP = CorrectionSet::from_file("corrections/scalefactors/electron/2016postVFP_electron_ID.json")->at("UL-Electron-ID-SF");
const auto cset_electron_ID_2017 = CorrectionSet::from_file("corrections/scalefactors/electron/2017_electron_ID.json")->at("UL-Electron-ID-SF");
const auto cset_electron_ID_2018 = CorrectionSet::from_file("corrections/scalefactors/electron/2018_electron_ID.json")->at("UL-Electron-ID-SF");

const auto cset_electron_ttHID_2016preVFP = CorrectionSet::from_file("corrections/scalefactors/electron/2016preVFP_ttH_ElectronID_SF.json")->at("2016preVFP_ttH_ElectronID_SF");
const auto cset_electron_ttHID_2016postVFP = CorrectionSet::from_file("corrections/scalefactors/electron/2016postVFP_ttH_ElectronID_SF.json")->at("2016postVFP_ttH_ElectronID_SF");
const auto cset_electron_ttHID_2017 = CorrectionSet::from_file("corrections/scalefactors/electron/2017_ttH_ElectronID_SF.json")->at("2017_ttH_ElectronID_SF");
const auto cset_electron_ttHID_2018 = CorrectionSet::from_file("corrections/scalefactors/electron/2018_ttH_ElectronID_SF.json")->at("2018_ttH_ElectronID_SF");

const auto cset_electron_ttHISO_2016preVFP = CorrectionSet::from_file("corrections/scalefactors/electron/2016preVFP_ttH_ElectronISO_SF.json")->at("2016preVFP_ttH_ElectronISO_SF");
const auto cset_electron_ttHISO_2016postVFP = CorrectionSet::from_file("corrections/scalefactors/electron/2016postVFP_ttH_ElectronISO_SF.json")->at("2016postVFP_ttH_ElectronISO_SF");
const auto cset_electron_ttHISO_2017 = CorrectionSet::from_file("corrections/scalefactors/electron/2017_ttH_ElectronISO_SF.json")->at("2017_ttH_ElectronISO_SF");
const auto cset_electron_ttHISO_2018 = CorrectionSet::from_file("corrections/scalefactors/electron/2018_ttH_ElectronISO_SF.json")->at("2018_ttH_ElectronISO_SF");


// function declarations
RNode goodRun(lumiMask golden, RNode df);
RNode pileupCorrection(correction::Correction::Ref cset_pileup_2016preVFP, correction::Correction::Ref cset_pileup_2016postVFP, correction::Correction::Ref cset_pileup_2017, correction::Correction::Ref cset_pileup_2018, RNode df);
RNode muonScaleFactors_ID(correction::Correction::Ref cset_muon_2016preVFP, correction::Correction::Ref cset_muon_2016postVFP, correction::Correction::Ref cset_muon_2017, correction::Correction::Ref cset_muon_2018, RNode df);
RNode muonScaleFactors_ttH(correction::Correction::Ref cset_muon_2016preVFP, correction::Correction::Ref cset_muon_2016postVFP, correction::Correction::Ref cset_muon_2017, correction::Correction::Ref cset_muon_2018, RNode df, std::string sf_type);
RNode electronScaleFactors_ID(correction::Correction::Ref cset_electron_2016preVFP, correction::Correction::Ref cset_electron_2016postVFP, correction::Correction::Ref cset_electron_2017, correction::Correction::Ref cset_electron_2018, RNode df);
RNode electronScaleFactors_ttH(correction::Correction::Ref cset_electron_2016preVFP, correction::Correction::Ref cset_electron_2016postVFP, correction::Correction::Ref cset_electron_2017, correction::Correction::Ref cset_electron_2018, RNode df, std::string sf_type);
RNode finalMCWeight(RNode df);


#endif
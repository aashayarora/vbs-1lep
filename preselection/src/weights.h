#ifndef WEIGHTS_H
#define WEIGHTS_H

#pragma once

#include "ROOT/RDataFrame.hxx"
#include "ROOT/RDFHelpers.hxx"
#include "ROOT/RVec.hxx"

#include "TRandom3.h"

#include "utils.h"
#include "correction.h"

using correction::CorrectionSet;
using RNode = ROOT::RDF::RNode;
using ROOT::VecOps::RVec;

// golden json
RNode goodRun(lumiMask golden, RNode df);
const auto LumiMask = lumiMask::fromJSON("corrections/goldenJson/Cert_271036-325175_13TeV_allRun2_JSON.json");

// pileup weights
RNode pileupCorrection(correction::Correction::Ref cset_pileup_2016preVFP, correction::Correction::Ref cset_pileup_2016postVFP, correction::Correction::Ref cset_pileup_2017, correction::Correction::Ref cset_pileup_2018, RNode df);
const auto cset_pileup_2016preVFP = CorrectionSet::from_file("corrections/pileup/2016preVFP_puWeights.json")->at("Collisions16_UltraLegacy_goldenJSON");
const auto cset_pileup_2016postVFP = CorrectionSet::from_file("corrections/pileup/2016postVFP_puWeights.json")->at("Collisions16_UltraLegacy_goldenJSON");
const auto cset_pileup_2017 = CorrectionSet::from_file("corrections/pileup/2017_puWeights.json")->at("Collisions17_UltraLegacy_goldenJSON");
const auto cset_pileup_2018 = CorrectionSet::from_file("corrections/pileup/2018_puWeights.json")->at("Collisions18_UltraLegacy_goldenJSON");

// muon sfs
RNode muonScaleFactors_ID(correction::Correction::Ref cset_muon_2016preVFP, correction::Correction::Ref cset_muon_2016postVFP, correction::Correction::Ref cset_muon_2017, correction::Correction::Ref cset_muon_2018, RNode df);
RNode muonScaleFactors_ttH(correction::Correction::Ref cset_muon_tth, RNode df, std::string sf_type);
const auto cset_muon_ID_2016preVFP = CorrectionSet::from_file("corrections/scalefactors/muon/2016preVFP_muon_ID.json")->at("NUM_IsoMu24_or_IsoTkMu24_DEN_CutBasedIdTight_and_PFIsoTight");
const auto cset_muon_ID_2016postVFP = CorrectionSet::from_file("corrections/scalefactors/muon/2016postVFP_muon_ID.json")->at("NUM_IsoMu24_or_IsoTkMu24_DEN_CutBasedIdTight_and_PFIsoTight");
const auto cset_muon_ID_2017 = CorrectionSet::from_file("corrections/scalefactors/muon/2017_muon_ID.json")->at("NUM_IsoMu27_DEN_CutBasedIdTight_and_PFIsoTight");
const auto cset_muon_ID_2018 = CorrectionSet::from_file("corrections/scalefactors/muon/2018_muon_ID.json")->at("NUM_IsoMu24_DEN_CutBasedIdTight_and_PFIsoTight");

const auto cset_muon_ttHID = CorrectionSet::from_file("corrections/scalefactors/muon/ttH_MuonID_SF.json")->at("ttH_MuonID_SF");
const auto cset_muon_ttHISO = CorrectionSet::from_file("corrections/scalefactors/muon/ttH_MuonISO_SF.json")->at("ttH_MuonISO_SF");

// electron sfs
RNode electronScaleFactors_ID(correction::Correction::Ref cset_electron_2016preVFP, correction::Correction::Ref cset_electron_2016postVFP, correction::Correction::Ref cset_electron_2017, correction::Correction::Ref cset_electron_2018, RNode df);
RNode electronScaleFactors_ttH(correction::Correction::Ref cset_electron_tth, RNode df, std::string sf_type);
RNode electronScaleFactors_Trigger(correction::Correction::Ref cset_electron_trigger, RNode df);
const auto cset_electron_ID_2016preVFP = CorrectionSet::from_file("corrections/scalefactors/electron/2016preVFP_electron_ID.json")->at("UL-Electron-ID-SF");
const auto cset_electron_ID_2016postVFP = CorrectionSet::from_file("corrections/scalefactors/electron/2016postVFP_electron_ID.json")->at("UL-Electron-ID-SF");
const auto cset_electron_ID_2017 = CorrectionSet::from_file("corrections/scalefactors/electron/2017_electron_ID.json")->at("UL-Electron-ID-SF");
const auto cset_electron_ID_2018 = CorrectionSet::from_file("corrections/scalefactors/electron/2018_electron_ID.json")->at("UL-Electron-ID-SF");

const auto cset_electron_ttHID = CorrectionSet::from_file("corrections/scalefactors/electron/ttH_ElectronID_SF.json")->at("ttH_ElectronID_SF");
const auto cset_electron_ttHISO = CorrectionSet::from_file("corrections/scalefactors/electron/ttH_ElectronISO_SF.json")->at("ttH_ElectronISO_SF");
const auto cset_electron_trigger = CorrectionSet::from_file("corrections/scalefactors/electron/trigger.json")->at("trigger");

// particle net sfs
RNode PNET_H_Corrections(correction::Correction::Ref cset_pnet_h, RNode df);
RNode PNET_W_Corrections(correction::Correction::Ref cset_pnet_w, RNode df);
const auto cset_pnet_w = CorrectionSet::from_file("corrections/scalefactors/particlenet/pnet.json")->at("PNET_W");
const auto cset_pnet_h = CorrectionSet::from_file("corrections/scalefactors/particlenet/pnet.json")->at("PNET_H");


// jet mass scale and resolution corrections
RNode JMS_Corrections(correction::Correction::Ref cset_jet_mass_scale, RNode df);
RNode JMR_Corrections(correction::Correction::Ref cset_jet_mass_scale, RNode df); 
const auto cset_jmr = CorrectionSet::from_file("corrections/scalefactors/jmar/jmar.json")->at("JMR");
const auto cset_jms = CorrectionSet::from_file("corrections/scalefactors/jmar/jmar.json")->at("JMS");

// other corrections
RNode METphicorrection(RNode df);
std::pair<double,double> METXYCorr_Met_MetPhi(float uncormet, float uncormet_phi, unsigned int runnb, std::string year_, std::string sample_category_, float npv);

RNode AddHEMCorrection(RNode df);
int HEMCorrections(unsigned int run, unsigned long long event, std::string sample_year, std::string sample_category, RVec<float> pt, RVec<float> eta, RVec<float> phi);

// final weights
RNode finalDataWeight(RNode df);
RNode finalMCWeight(RNode df);

enum TheRunEra{
  y2016B,y2016C,y2016D,y2016E,y2016F,y2016G,y2016H,
  y2017B,y2017C,y2017D,y2017E,y2017F,
  y2018A,y2018B,y2018C,y2018D,
  y2016MC,
  y2017MC,
  y2018MC,
  yUL2016B,yUL2016C,yUL2016D,yUL2016E,yUL2016F,yUL2016Flate,yUL2016G,yUL2016H,
  yUL2017B,yUL2017C,yUL2017D,yUL2017E,yUL2017F,
  yUL2018A,yUL2018B,yUL2018C,yUL2018D,
  yUL2016MCAPV,
  yUL2016MCnonAPV,
  yUL2017MC,
  yUL2018MC
};
#endif
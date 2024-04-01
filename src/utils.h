#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

#include "ROOT/RDataFrame.hxx"
#include "ROOT/RDFHelpers.hxx"
#include "ROOT/RVec.hxx"

#include "TString.h"

using RNode = ROOT::RDF::RNode;
using ROOT::VecOps::RVec;

double get_btag_workingpoint(const std::string& year, const std::string& wp);
double mediumDFBtagWP(std::string year);
double tightDFBtagWP(std::string year);
RVec<float> lepselect(bool is_el, RVec<float> el_var, RVec<float> mu_var);
RVec<int> noJetOverlap8 (RVec<float> obj_eta, RVec<float> obj_phi, RVec<float> jet_eta, RVec<float> jet_phi);
RVec<int> noJetOverlap4 (RVec<float> obj_eta, RVec<float> obj_phi, RVec<float> jet_eta, RVec<float> jet_phi);
RNode defineUndefinedHLTs(RNode df);
void saveSnapshot(RNode df, const std::string& finalFile);

#endif
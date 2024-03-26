#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

#include "ROOT/RDataFrame.hxx"
#include "ROOT/RDFHelpers.hxx"
#include "ROOT/RVec.hxx"

#include "TString.h"

using RNode = ROOT::RDF::RNode;

double get_btag_workingpoint(const std::string& year, const std::string& wp);
void saveSnapshot(RNode df, const std::string& finalFile);

#endif
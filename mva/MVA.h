#pragma once

#ifndef MVA_H
#define MVA_H


#include <string>
#include <vector>

#include <torch/script.h>

#include "argparser.hpp"

#include "ROOT/RDataFrame.hxx"
#include "ROOT/RDFHelpers.hxx"
#include "TMVA/RReader.hxx"

using namespace TMVA::Experimental;
using RNode = ROOT::RDF::RNode;

#endif // MVA_H
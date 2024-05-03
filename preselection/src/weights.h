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

RNode goodRun(lumiMask golden, RNode df);

#endif
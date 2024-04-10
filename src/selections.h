#ifndef SELECTIONS_H
#define SELECTIONS_H

#include "ROOT/RDataFrame.hxx"
#include "ROOT/RDFHelpers.hxx"
#include "ROOT/RVec.hxx"

#include "Math/Vector4D.h"
#include "Math/VectorUtil.h"

#include "utils.h"

using RNode = ROOT::RDF::RNode;
using ROOT::VecOps::RVec;

RNode analysisSelections(RNode df);

#endif
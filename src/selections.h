// RDF Includes
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RDFHelpers.hxx"
#include "ROOT/RVec.hxx"

#include "Math/Vector4D.h"

#include "utils.h"

using RNode = ROOT::RDF::RNode;
using ROOT::VecOps::RVec;

RNode electronSelections(RNode df);
RNode muonSelections(RNode df);
RNode eventSelections(RNode df);
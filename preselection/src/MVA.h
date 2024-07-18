#ifndef MVA_H
#define MVA_H

#pragma once

#include "ROOT/RDataFrame.hxx"
#include "ROOT/RDFHelpers.hxx"
#include "TMVA/RReader.hxx"

using RNode = ROOT::RDF::RNode;

RNode MVA(RNode df);

#endif // MVA_H
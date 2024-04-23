#include "utils.h"

/*
    LUMIMASK
*/

bool operator< ( const lumiMask::LumiBlockRange& lh, const lumiMask::LumiBlockRange& rh )
{
    return ( lh.run() == rh.run() ) ? ( lh.lastLumi() < rh.firstLumi() ) : lh.run() < rh.run();
}

lumiMask lumiMask::fromJSON(const std::string& file, lumiMask::Run firstRun, lumiMask::Run lastRun)
{
  const bool noRunFilter = ( firstRun == 0 ) && ( lastRun == 0 );
  boost::property_tree::ptree ptree;
  boost::property_tree::read_json(file, ptree);

  std::vector<lumiMask::LumiBlockRange> accept;
  for ( const auto& runEntry : ptree ) {
    const lumiMask::Run run = std::stoul(runEntry.first);
    if ( noRunFilter || ( ( firstRun <= run ) && ( run <= lastRun ) ) ) {
      for ( const auto& lrEntry : runEntry.second ) {
        const auto lrNd = lrEntry.second;
        const lumiMask::LumiBlock firstLumi = std::stoul(lrNd.begin()->second.data());
        const lumiMask::LumiBlock lastLumi  = std::stoul((++lrNd.begin())->second.data());
        accept.emplace_back(run, firstLumi, lastLumi);
      }
    }
  }
  return lumiMask(accept);
}

/*
    DUPLICATE REMOVAL
*/

RNode removeDuplicates(RNode df){
    return df.Filter(FilterOnePerKind(), {"run", "luminosityBlock", "event"}, "REMOVED DUPLICATES");
}

/*
    SELECTION UTILS
*/

float looseDFBtagWP(std::string year){
    if(year == "2016preVFP")
        return 0.0508;
    if(year == "2016postVFP")
        return 0.0480;
    if(year == "2017")
        return 0.0532;
    if(year == "2018")
        return 0.0490;
    return -1;
}

float mediumDFBtagWP(std::string year){
    if(year == "2016preVFP")
        return 0.2598;
    if(year == "2016postVFP")
        return 0.2489;
    if(year == "2017")
        return 0.3040;
    if(year == "2018")
        return 0.2783;
    return -1;
}

float tightDFBtagWP(std::string year){
    if(year == "2016preVFP")
        return 0.6502;
    if(year == "2016postVFP")
        return 0.6377;
    if(year == "2017")
        return 0.7476;
    if(year == "2018")
        return 0.7100;
    return -1;
}

RVec<float> fDeltaR (RVec<float> vec_eta, RVec<float> vec_phi, float obj_eta, float obj_phi) { 
    RVec<float> deltaR = {};
    for (size_t i = 0; i < vec_eta.size(); i++) {
        deltaR.push_back(ROOT::VecOps::DeltaR(vec_eta[i], obj_eta, vec_phi[i], obj_phi));
    }
    return deltaR;
}

void saveSnapshot(RNode df, const std::string& finalFile) {
    auto ColNames = df.GetDefinedColumnNames();
    std::vector<std::string> final_variables;
    for (auto &&ColName : ColNames)
        {
            TString colName = ColName;
            std::string name = colName.Data();
            final_variables.push_back(name);
        }
    // final_variables.push_back("nJet");
    df.Snapshot("Events", std::string("output/") + finalFile, final_variables);
}


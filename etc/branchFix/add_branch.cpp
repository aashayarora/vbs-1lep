#include "TFile.h"
#include "TTree.h"
#include "TMath.h"

#include <vector>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    TFile f(argv[1], "update");
 
    Bool_t new_v;
    auto events = f.Get<TTree>("Events");
    
    vector<const char*> branches = {"HLT_Ele32_WPTight_Gsf", "HLT_IsoMu24", "HLT_Ele32_WPTight_Gsf_L1DoubleEG", "HLT_IsoMu27", "HLT_Ele27_eta2p1_WPTight_Gsf", "HLT_IsoTkMu24"};
    vector<TBranch*> newBranches;
    for (auto branch : branches) {
        if (!events->GetBranch(branch)) {
            cout << "Adding branch " << branch << endl;
            newBranches.push_back(events->Branch(branch, &new_v));
        }
    }
    Long64_t nentries = events->GetEntries(); // read the number of entries in the t3
 
    for (Long64_t i = 0; i < nentries; i++) {
        new_v = 0;
        for (auto branch : newBranches) {
            branch->Fill();
        }
    }
 
    events->Write("", TObject::kOverwrite); // save only the new version of the tree
}
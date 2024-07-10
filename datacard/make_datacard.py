from dataclasses import dataclass
import uproot
import os.path

@dataclass
class Systematics():
    sig: list
    data: list
    ttH_elec_reco: list
    ttH_elec_recotoloose: list
    ttH_elec_trig: list
    ttH_elec_loosetoiso: list
    ttH_elec_isototight: list
    ttH_muon_recotoloose: list
    ttH_muon_trig: list
    ttH_muon_loosetoiso: list
    ttH_muon_isototight: list
    PrefireWeight_13TeV: list
    vbsvvh_puWeight: list
    vbsvvh_puJetID: list
    scale_j_Absolute_13TeV: list
    scale_j_Absolute_2016postVFP_13TeV: list
    scale_j_Absolute_2016preVFP_13TeV: list
    scale_j_Absolute_2017_13TeV: list
    scale_j_Absolute_2018_13TeV: list
    scale_j_BBEC1_13TeV: list
    scale_j_BBEC1_2016postVFP_13TeV: list
    scale_j_BBEC1_2016preVFP_13TeV: list
    scale_j_BBEC1_2017_13TeV: list
    scale_j_BBEC1_2018_13TeV: list
    scale_j_EC2_13TeV: list
    scale_j_EC2_2016postVFP_13TeV: list
    scale_j_EC2_2016preVFP_13TeV: list
    scale_j_EC2_2017_13TeV: list
    scale_j_EC2_2018_13TeV: list
    scale_j_FlavorQCD_13TeV: list
    scale_j_HF_13TeV: list
    scale_j_HF_2016postVFP_13TeV: list
    scale_j_HF_2016preVFP_13TeV: list
    scale_j_HF_2017_13TeV: list
    scale_j_HF_2018_13TeV: list
    scale_j_RelativeBal_13TeV: list
    scale_j_RelativeSample_2016postVFP_13TeV: list
    scale_j_RelativeSample_2016preVFP_13TeV: list
    scale_j_RelativeSample_2017_13TeV: list
    scale_j_RelativeSample_2018_13TeV: list
    res_j_13TeV: list
    metUncl_13Tev: list
    jms_pnetreg: list
    jmr_pnetreg: list
    btagWeightDeepJet_HF_13Tev: list
    btagWeightDeepJet_LF_13Tev: list
    vbsvvh1lep_bTagWeightXbb_13TeV_16preVFP: list
    vbsvvh1lep_bTagWeightXbb_13TeV_16postVFP: list
    vbsvvh1lep_bTagWeightXbb_13TeV_17: list
    vbsvvh1lep_bTagWeightXbb_13TeV_18: list
    vbsvvh1lep_qTagWeightXWqq_13TeV_16preVFP: list
    vbsvvh1lep_qTagWeightXWqq_13TeV_16postVFP: list
    vbsvvh1lep_qTagWeightXWqq_13TeV_17: list
    vbsvvh1lep_qTagWeightXWqq_13TeV_18: list

def max_var(variations):
    deviations = [abs(1 - x) for x in variations]
    return variations[deviations.index(max(deviations))]

def get_variation(correction=None, tree="Events"):
    print(f"Evaluating {correction}")
    sig_file = "/data/userdata/aaarora/output/run2/ABCDNet_simpleDisco_VBSVVH1lep_30/output/sig_MVA_abcdnet.root"
    
    BDT_CUT = 0.56
    DNN_CUT = 0.92
    
    with uproot.open(sig_file) as f:
        df = f.get(tree).arrays(["VBSBDTscore", "abcdnet_score", "weight"], library="pd")

    a = sum(df[(df.VBSBDTscore > BDT_CUT) & (df.abcdnet_score > DNN_CUT)].weight)
    b = sum(df[(df.VBSBDTscore < BDT_CUT) & (df.abcdnet_score > DNN_CUT)].weight)
    c = sum(df[(df.VBSBDTscore > BDT_CUT) & (df.abcdnet_score < DNN_CUT)].weight)
    d = sum(df[(df.VBSBDTscore < BDT_CUT) & (df.abcdnet_score < DNN_CUT)].weight)

    if correction is None:
        return ["{:.5f}".format(i) for i in [a, b, c, d]]

    variations = []

    if os.path.isfile(f"/data/userdata/aaarora/output/run2/ABCDNet_simpleDisco_VBSVVH1lep_30/output/sig_{correction}_up_MVA_abcdnet.root"):
        with uproot.open(f"/data/userdata/aaarora/output/run2/ABCDNet_simpleDisco_VBSVVH1lep_30/output/sig_{correction}_up_MVA_abcdnet.root") as g:
            df2 = g.get(tree).arrays(["VBSBDTscore", "abcdnet_score", "weight"], library="pd")

        a2 = sum(df2[(df2.VBSBDTscore > BDT_CUT) & (df2.abcdnet_score > DNN_CUT)].weight)
        b2 = sum(df2[(df2.VBSBDTscore < BDT_CUT) & (df2.abcdnet_score > DNN_CUT)].weight)
        c2 = sum(df2[(df2.VBSBDTscore > BDT_CUT) & (df2.abcdnet_score < DNN_CUT)].weight)
        d2 = sum(df2[(df2.VBSBDTscore < BDT_CUT) & (df2.abcdnet_score < DNN_CUT)].weight)

        variations.append([(a - a2) / a, (b - b2) / b, (c - c2) / c, (d - d2) / d])


    if os.path.isfile(f"/data/userdata/aaarora/output/run2/ABCDNet_simpleDisco_VBSVVH1lep_30/output/sig_{correction}_down_MVA_abcdnet.root"):
        with uproot.open(f"/data/userdata/aaarora/output/run2/ABCDNet_simpleDisco_VBSVVH1lep_30/output/sig_{correction}_down_MVA_abcdnet.root") as g:
            df3 = g.get(tree).arrays(["VBSBDTscore", "abcdnet_score", "weight"], library="pd")

        a2 = sum(df3[(df3.VBSBDTscore > BDT_CUT) & (df3.abcdnet_score > DNN_CUT)].weight)
        b2 = sum(df3[(df3.VBSBDTscore < BDT_CUT) & (df3.abcdnet_score > DNN_CUT)].weight)
        c2 = sum(df3[(df3.VBSBDTscore > BDT_CUT) & (df3.abcdnet_score < DNN_CUT)].weight)
        d2 = sum(df3[(df3.VBSBDTscore < BDT_CUT) & (df3.abcdnet_score < DNN_CUT)].weight)

        variations.append([(a - a2) / a, (b - b2) / b, (c - c2) / c, (d - d2) / d])

    if len(variations) == 2:
        return ["{:.5f}".format(1 + abs(max_var(x))) for x in zip(*variations)]
    else:
        return ["{:.5f}".format(1 + abs(x)) for x in variations[0]]

def get_data(tree="Events"):
    data_file = "/data/userdata/aaarora/output/run2/ABCDNet_simpleDisco_VBSVVH1lep_30/output/data_MVA_abcdnet.root"
    
    BDT_CUT = 0.56
    DNN_CUT = 0.92
    
    with uproot.open(data_file) as f:
        df = f.get(tree).arrays(["VBSBDTscore", "abcdnet_score", "weight"], library="pd")

    b = sum(df[(df.VBSBDTscore < BDT_CUT) & (df.abcdnet_score > DNN_CUT)].weight)
    c = sum(df[(df.VBSBDTscore > BDT_CUT) & (df.abcdnet_score < DNN_CUT)].weight)
    d = sum(df[(df.VBSBDTscore < BDT_CUT) & (df.abcdnet_score < DNN_CUT)].weight)

    return [b, c, d]

def write_datacard(sys):
    datacard = f"""imax 4 number of channels
jmax 1 number of backgrounds
kmax * number of nuisance parameters
--------------------------------------------------------------------------------------------------------------------------------
bin                                                     A                  B                  C                  D                  
observation                                             1                  78                 12                 1054               
--------------------------------------------------------------------------------------------------------------------------------
bin                                                     A                  B                  C                  D                  A           B           C           D           
process                                                 TotalBkg_OneLep    TotalBkg_OneLep    TotalBkg_OneLep    TotalBkg_OneLep    TotalSig    TotalSig    TotalSig    TotalSig    
process                                                 1                  1                  1                  1                  0           0           0           0           
rate                                                    1                  1                  1                  1                  {sys.sig[0]}     {sys.sig[1]}     {sys.sig[2]}     {sys.sig[3]}    
--------------------------------------------------------------------------------------------------------------------------------
CMS_vbsvvh1lep_control_abcd_syst                  lnN   1.2                -                  -                  -                  -           -           -           -           
lumi_13TeV_correlated                             lnN   -                  -                  -                  -                  1.016       1.016       1.016       1.016       
CMS_ttH_elec_reco                                 lnN   -                  -                  -                  -                  {sys.ttH_elec_reco[0]}     {sys.ttH_elec_reco[1]}     {sys.ttH_elec_reco[2]}     {sys.ttH_elec_reco[3]}
CMS_ttH_elec_recotoloose                          lnN   -                  -                  -                  -                  {sys.ttH_elec_recotoloose[0]}     {sys.ttH_elec_recotoloose[1]}     {sys.ttH_elec_recotoloose[2]}     {sys.ttH_elec_recotoloose[3]}
CMS_ttH_elec_trig                                 lnN   -                  -                  -                  -                  {sys.ttH_elec_trig[0]}     {sys.ttH_elec_trig[1]}     {sys.ttH_elec_trig[2]}     {sys.ttH_elec_trig[3]}
CMS_ttH_elec_loosetoiso                           lnN   -                  -                  -                  -                  {sys.ttH_elec_loosetoiso[0]}     {sys.ttH_elec_loosetoiso[1]}     {sys.ttH_elec_loosetoiso[2]}     {sys.ttH_elec_loosetoiso[3]}
CMS_ttH_elec_isototight                           lnN   -                  -                  -                  -                  {sys.ttH_elec_isototight[0]}     {sys.ttH_elec_isototight[1]}     {sys.ttH_elec_isototight[2]}     {sys.ttH_elec_isototight[3]}
CMS_ttH_muon_recotoloose                          lnN   -                  -                  -                  -                  {sys.ttH_muon_recotoloose[0]}     {sys.ttH_muon_recotoloose[1]}     {sys.ttH_muon_recotoloose[2]}     {sys.ttH_muon_recotoloose[3]}
CMS_ttH_muon_trig                                 lnN   -                  -                  -                  -                  {sys.ttH_muon_trig[0]}     {sys.ttH_muon_trig[1]}     {sys.ttH_muon_trig[2]}     {sys.ttH_muon_trig[3]}
CMS_ttH_muon_loosetoiso                           lnN   -                  -                  -                  -                  {sys.ttH_muon_loosetoiso[0]}     {sys.ttH_muon_loosetoiso[1]}     {sys.ttH_muon_loosetoiso[2]}     {sys.ttH_muon_loosetoiso[3]}
CMS_ttH_muon_isototight                           lnN   -                  -                  -                  -                  {sys.ttH_muon_isototight[0]}     {sys.ttH_muon_isototight[1]}     {sys.ttH_muon_isototight[2]}     {sys.ttH_muon_isototight[3]}
CMS_PrefireWeight_13TeV                           lnN   -                  -                  -                  -                  {sys.PrefireWeight_13TeV[0]}     {sys.PrefireWeight_13TeV[1]}     {sys.PrefireWeight_13TeV[2]}     {sys.PrefireWeight_13TeV[3]}
CMS_vbsvvh_puWeight                               lnN   -                  -                  -                  -                  {sys.vbsvvh_puWeight[0]}     {sys.vbsvvh_puWeight[1]}     {sys.vbsvvh_puWeight[2]}     {sys.vbsvvh_puWeight[3]}
CMS_vbsvvh_puJetID                                lnN   -                  -                  -                  -                  {sys.vbsvvh_puJetID[0]}     {sys.vbsvvh_puJetID[1]}     {sys.vbsvvh_puJetID[2]}     {sys.vbsvvh_puJetID[3]}
CMS_scale_j_Absolute_13TeV                        lnN   -                  -                  -                  -                  {sys.scale_j_Absolute_13TeV[0]}     {sys.scale_j_Absolute_13TeV[1]}     {sys.scale_j_Absolute_13TeV[2]}     {sys.scale_j_Absolute_13TeV[3]}
CMS_scale_j_Absolute_2016postVFP_13TeV            lnN   -                  -                  -                  -                  {sys.scale_j_Absolute_2016postVFP_13TeV[0]}     {sys.scale_j_Absolute_2016postVFP_13TeV[1]}     {sys.scale_j_Absolute_2016postVFP_13TeV[2]}     {sys.scale_j_Absolute_2016postVFP_13TeV[3]}
CMS_scale_j_Absolute_2016preVFP_13TeV             lnN   -                  -                  -                  -                  {sys.scale_j_Absolute_2016preVFP_13TeV[0]}     {sys.scale_j_Absolute_2016preVFP_13TeV[1]}     {sys.scale_j_Absolute_2016preVFP_13TeV[2]}     {sys.scale_j_Absolute_2016preVFP_13TeV[3]}
CMS_scale_j_Absolute_2017_13TeV                   lnN   -                  -                  -                  -                  {sys.scale_j_Absolute_2017_13TeV[0]}     {sys.scale_j_Absolute_2017_13TeV[1]}     {sys.scale_j_Absolute_2017_13TeV[2]}     {sys.scale_j_Absolute_2017_13TeV[3]}
CMS_scale_j_Absolute_2018_13TeV                   lnN   -                  -                  -                  -                  {sys.scale_j_Absolute_2018_13TeV[0]}     {sys.scale_j_Absolute_2018_13TeV[1]}     {sys.scale_j_Absolute_2018_13TeV[2]}     {sys.scale_j_Absolute_2018_13TeV[3]}
CMS_scale_j_BBEC1_13TeV                           lnN   -                  -                  -                  -                  {sys.scale_j_BBEC1_13TeV[0]}     {sys.scale_j_BBEC1_13TeV[1]}     {sys.scale_j_BBEC1_13TeV[2]}     {sys.scale_j_BBEC1_13TeV[3]}
CMS_scale_j_BBEC1_2016postVFP_13TeV               lnN   -                  -                  -                  -                  {sys.scale_j_BBEC1_2016postVFP_13TeV[0]}     {sys.scale_j_BBEC1_2016postVFP_13TeV[1]}     {sys.scale_j_BBEC1_2016postVFP_13TeV[2]}     {sys.scale_j_BBEC1_2016postVFP_13TeV[3]}
CMS_scale_j_BBEC1_2016preVFP_13TeV                lnN   -                  -                  -                  -                  {sys.scale_j_BBEC1_2016preVFP_13TeV[0]}     {sys.scale_j_BBEC1_2016preVFP_13TeV[1]}     {sys.scale_j_BBEC1_2016preVFP_13TeV[2]}     {sys.scale_j_BBEC1_2016preVFP_13TeV[3]}
CMS_scale_j_BBEC1_2017_13TeV                      lnN   -                  -                  -                  -                  {sys.scale_j_BBEC1_2017_13TeV[0]}     {sys.scale_j_BBEC1_2017_13TeV[1]}     {sys.scale_j_BBEC1_2017_13TeV[2]}     {sys.scale_j_BBEC1_2017_13TeV[3]}
CMS_scale_j_BBEC1_2018_13TeV                      lnN   -                  -                  -                  -                  {sys.scale_j_BBEC1_2018_13TeV[0]}     {sys.scale_j_BBEC1_2018_13TeV[1]}     {sys.scale_j_BBEC1_2018_13TeV[2]}     {sys.scale_j_BBEC1_2018_13TeV[3]}
CMS_scale_j_EC2_13TeV                             lnN   -                  -                  -                  -                  {sys.scale_j_EC2_13TeV[0]}     {sys.scale_j_EC2_13TeV[1]}     {sys.scale_j_EC2_13TeV[2]}     {sys.scale_j_EC2_13TeV[3]}
CMS_scale_j_EC2_2016postVFP_13TeV                 lnN   -                  -                  -                  -                  {sys.scale_j_EC2_2016postVFP_13TeV[0]}     {sys.scale_j_EC2_2016postVFP_13TeV[1]}     {sys.scale_j_EC2_2016postVFP_13TeV[2]}     {sys.scale_j_EC2_2016postVFP_13TeV[3]}
CMS_scale_j_EC2_2016preVFP_13TeV                  lnN   -                  -                  -                  -                  {sys.scale_j_EC2_2016preVFP_13TeV[0]}     {sys.scale_j_EC2_2016preVFP_13TeV[1]}     {sys.scale_j_EC2_2016preVFP_13TeV[2]}     {sys.scale_j_EC2_2016preVFP_13TeV[3]}
CMS_scale_j_EC2_2017_13TeV                        lnN   -                  -                  -                  -                  {sys.scale_j_EC2_2017_13TeV[0]}     {sys.scale_j_EC2_2017_13TeV[1]}     {sys.scale_j_EC2_2017_13TeV[2]}     {sys.scale_j_EC2_2017_13TeV[3]}
CMS_scale_j_EC2_2018_13TeV                        lnN   -                  -                  -                  -                  {sys.scale_j_EC2_2018_13TeV[0]}     {sys.scale_j_EC2_2018_13TeV[1]}     {sys.scale_j_EC2_2018_13TeV[2]}     {sys.scale_j_EC2_2018_13TeV[3]}
CMS_scale_j_FlavorQCD_13TeV                       lnN   -                  -                  -                  -                  {sys.scale_j_FlavorQCD_13TeV[0]}     {sys.scale_j_FlavorQCD_13TeV[1]}     {sys.scale_j_FlavorQCD_13TeV[2]}     {sys.scale_j_FlavorQCD_13TeV[3]}
CMS_scale_j_HF_13TeV                              lnN   -                  -                  -                  -                  {sys.scale_j_HF_13TeV[0]}     {sys.scale_j_HF_13TeV[1]}     {sys.scale_j_HF_13TeV[2]}     {sys.scale_j_HF_13TeV[3]}
CMS_scale_j_HF_2016postVFP_13TeV                  lnN   -                  -                  -                  -                  {sys.scale_j_HF_2016postVFP_13TeV[0]}     {sys.scale_j_HF_2016postVFP_13TeV[1]}     {sys.scale_j_HF_2016postVFP_13TeV[2]}     {sys.scale_j_HF_2016postVFP_13TeV[3]}
CMS_scale_j_HF_2016preVFP_13TeV                   lnN   -                  -                  -                  -                  {sys.scale_j_HF_2016preVFP_13TeV[0]}     {sys.scale_j_HF_2016preVFP_13TeV[1]}     {sys.scale_j_HF_2016preVFP_13TeV[2]}     {sys.scale_j_HF_2016preVFP_13TeV[3]}
CMS_scale_j_HF_2017_13TeV                         lnN   -                  -                  -                  -                  {sys.scale_j_HF_2017_13TeV[0]}     {sys.scale_j_HF_2017_13TeV[1]}     {sys.scale_j_HF_2017_13TeV[2]}     {sys.scale_j_HF_2017_13TeV[3]}
CMS_scale_j_HF_2018_13TeV                         lnN   -                  -                  -                  -                  {sys.scale_j_HF_2018_13TeV[0]}     {sys.scale_j_HF_2018_13TeV[1]}     {sys.scale_j_HF_2018_13TeV[2]}     {sys.scale_j_HF_2018_13TeV[3]}
CMS_scale_j_RelativeBal_13TeV                     lnN   -                  -                  -                  -                  {sys.scale_j_RelativeBal_13TeV[0]}     {sys.scale_j_RelativeBal_13TeV[1]}     {sys.scale_j_RelativeBal_13TeV[2]}     {sys.scale_j_RelativeBal_13TeV[3]}
CMS_scale_j_RelativeSample_2016postVFP_13TeV      lnN   -                  -                  -                  -                  {sys.scale_j_RelativeSample_2016postVFP_13TeV[0]}     {sys.scale_j_RelativeSample_2016postVFP_13TeV[1]}     {sys.scale_j_RelativeSample_2016postVFP_13TeV[2]}     {sys.scale_j_RelativeSample_2016postVFP_13TeV[3]}
CMS_scale_j_RelativeSample_2016preVFP_13TeV       lnN   -                  -                  -                  -                  {sys.scale_j_RelativeSample_2016preVFP_13TeV[0]}     {sys.scale_j_RelativeSample_2016preVFP_13TeV[1]}     {sys.scale_j_RelativeSample_2016preVFP_13TeV[2]}     {sys.scale_j_RelativeSample_2016preVFP_13TeV[3]}
CMS_scale_j_RelativeSample_2017_13TeV             lnN   -                  -                  -                  -                  {sys.scale_j_RelativeSample_2017_13TeV[0]}     {sys.scale_j_RelativeSample_2017_13TeV[1]}     {sys.scale_j_RelativeSample_2017_13TeV[2]}     {sys.scale_j_RelativeSample_2017_13TeV[3]}
CMS_scale_j_RelativeSample_2018_13TeV             lnN   -                  -                  -                  -                  {sys.scale_j_RelativeSample_2018_13TeV[0]}     {sys.scale_j_RelativeSample_2018_13TeV[1]}     {sys.scale_j_RelativeSample_2018_13TeV[2]}     {sys.scale_j_RelativeSample_2018_13TeV[3]}
CMS_res_j_13TeV                                   lnN   -                  -                  -                  -                  {sys.res_j_13TeV[0]}     {sys.res_j_13TeV[1]}     {sys.res_j_13TeV[2]}     {sys.res_j_13TeV[3]}
CMS_metUncl_13Tev                                 lnN   -                  -                  -                  -                  {sys.metUncl_13Tev[0]}     {sys.metUncl_13Tev[1]}     {sys.metUncl_13Tev[2]}     {sys.metUncl_13Tev[3]}
CMS_jms_pnetreg_13TeV                             lnN   -                  -                  -                  -                  {sys.jms_pnetreg[0]}     {sys.jms_pnetreg[1]}     {sys.jms_pnetreg[2]}     {sys.jms_pnetreg[3]}
CMS_jmr_pnetreg_13TeV                             lnN   -                  -                  -                  -                  {sys.jmr_pnetreg[0]}     {sys.jmr_pnetreg[1]}     {sys.jmr_pnetreg[2]}     {sys.jmr_pnetreg[3]}
CMS_btagWeightDeepJet_HF_13Tev                    lnN   -                  -                  -                  -                  {sys.btagWeightDeepJet_HF_13Tev[0]}     {sys.btagWeightDeepJet_HF_13Tev[1]}     {sys.btagWeightDeepJet_HF_13Tev[2]}     {sys.btagWeightDeepJet_HF_13Tev[3]}
CMS_btagWeightDeepJet_LF_13Tev                    lnN   -                  -                  -                  -                  {sys.btagWeightDeepJet_LF_13Tev[0]}     {sys.btagWeightDeepJet_LF_13Tev[1]}     {sys.btagWeightDeepJet_LF_13Tev[2]}     {sys.btagWeightDeepJet_LF_13Tev[3]}
CMS_vbsvvh1lep_bTagWeightXbb_13TeV_16preVFP       lnN   -                  -                  -                  -                  {sys.vbsvvh1lep_bTagWeightXbb_13TeV_16preVFP[0]}     {sys.vbsvvh1lep_bTagWeightXbb_13TeV_16preVFP[1]}     {sys.vbsvvh1lep_bTagWeightXbb_13TeV_16preVFP[2]}     {sys.vbsvvh1lep_bTagWeightXbb_13TeV_16preVFP[3]}
CMS_vbsvvh1lep_bTagWeightXbb_13TeV_16postVFP      lnN   -                  -                  -                  -                  {sys.vbsvvh1lep_bTagWeightXbb_13TeV_16postVFP[0]}     {sys.vbsvvh1lep_bTagWeightXbb_13TeV_16postVFP[1]}     {sys.vbsvvh1lep_bTagWeightXbb_13TeV_16postVFP[2]}     {sys.vbsvvh1lep_bTagWeightXbb_13TeV_16postVFP[3]}
CMS_vbsvvh1lep_bTagWeightXbb_13TeV_17             lnN   -                  -                  -                  -                  {sys.vbsvvh1lep_bTagWeightXbb_13TeV_17[0]}     {sys.vbsvvh1lep_bTagWeightXbb_13TeV_17[1]}     {sys.vbsvvh1lep_bTagWeightXbb_13TeV_17[2]}     {sys.vbsvvh1lep_bTagWeightXbb_13TeV_17[3]}
CMS_vbsvvh1lep_bTagWeightXbb_13TeV_18             lnN   -                  -                  -                  -                  {sys.vbsvvh1lep_bTagWeightXbb_13TeV_18[0]}     {sys.vbsvvh1lep_bTagWeightXbb_13TeV_18[1]}     {sys.vbsvvh1lep_bTagWeightXbb_13TeV_18[2]}     {sys.vbsvvh1lep_bTagWeightXbb_13TeV_18[3]}
CMS_vbsvvh1lep_qTagWeightXWqq_13TeV_16preVFP      lnN   -                  -                  -                  -                  {sys.vbsvvh1lep_qTagWeightXWqq_13TeV_16preVFP[0]}     {sys.vbsvvh1lep_qTagWeightXWqq_13TeV_16preVFP[1]}     {sys.vbsvvh1lep_qTagWeightXWqq_13TeV_16preVFP[2]}     {sys.vbsvvh1lep_qTagWeightXWqq_13TeV_16preVFP[3]}
CMS_vbsvvh1lep_qTagWeightXWqq_13TeV_16postVFP     lnN   -                  -                  -                  -                  {sys.vbsvvh1lep_qTagWeightXWqq_13TeV_16postVFP[0]}     {sys.vbsvvh1lep_qTagWeightXWqq_13TeV_16postVFP[1]}     {sys.vbsvvh1lep_qTagWeightXWqq_13TeV_16postVFP[2]}     {sys.vbsvvh1lep_qTagWeightXWqq_13TeV_16postVFP[3]}
CMS_vbsvvh1lep_qTagWeightXWqq_13TeV_17            lnN   -                  -                  -                  -                  {sys.vbsvvh1lep_qTagWeightXWqq_13TeV_17[0]}     {sys.vbsvvh1lep_qTagWeightXWqq_13TeV_17[1]}     {sys.vbsvvh1lep_qTagWeightXWqq_13TeV_17[2]}     {sys.vbsvvh1lep_qTagWeightXWqq_13TeV_17[3]}
CMS_vbsvvh1lep_qTagWeightXWqq_13TeV_18            lnN   -                  -                  -                  -                  {sys.vbsvvh1lep_qTagWeightXWqq_13TeV_18[0]}     {sys.vbsvvh1lep_qTagWeightXWqq_13TeV_18[1]}     {sys.vbsvvh1lep_qTagWeightXWqq_13TeV_18[2]}     {sys.vbsvvh1lep_qTagWeightXWqq_13TeV_18[3]}                  
--------------------------------------------------------------------------------------------------------------------------------
A_OneLep rateParam                  A  TotalBkg_OneLep    (@0*@1/@2) B_OneLep,C_OneLep,D_OneLep    
B_OneLep rateParam                  B  TotalBkg_OneLep    {sys.data[0]}                  
C_OneLep rateParam                  C  TotalBkg_OneLep    {sys.data[1]}                    
D_OneLep rateParam                  D  TotalBkg_OneLep    {sys.data[2]}                
"""
    with open("datacard.txt", "w") as f:
        f.write(datacard)

if __name__ == "__main__":
    sys = Systematics(
        sig=get_variation(),
        data=get_data(),
        ttH_elec_reco=get_variation("electron_scale_factors_Reco"),
        ttH_elec_recotoloose=get_variation("electron_scale_factors_ttHID"),
        ttH_elec_trig=get_variation("electron_scale_factors_trigger"),
        ttH_elec_loosetoiso=get_variation("electron_scale_factors_ttHISO"),
        ttH_elec_isototight=get_variation("electron_scale_factors_ID"),
        ttH_muon_recotoloose=get_variation("muon_scale_factors_ttHID"),
        ttH_muon_trig=get_variation("muon_scale_factors_trigger"),
        ttH_muon_loosetoiso=get_variation("muon_scale_factors_ttHISO"),
        ttH_muon_isototight=get_variation("muon_scale_factors_ID"),
        PrefireWeight_13TeV=get_variation("L1PreFiringWeight"),
        vbsvvh_puWeight=get_variation("pileup_weight"),
        vbsvvh_puJetID=get_variation("pileupid_weight"),
        scale_j_Absolute_13TeV=get_variation("jec_absolute"),
        scale_j_Absolute_2016postVFP_13TeV=get_variation("jec_absolute_2016"),
        scale_j_Absolute_2016preVFP_13TeV=get_variation("jec_absolute_2016"),
        scale_j_Absolute_2017_13TeV=get_variation("jec_absolute_2017"),
        scale_j_Absolute_2018_13TeV=get_variation("jec_absolute_2018"),
        scale_j_BBEC1_13TeV=get_variation("jec_bbec1"),
        scale_j_BBEC1_2016postVFP_13TeV=get_variation("jec_bbec1_2016"),
        scale_j_BBEC1_2016preVFP_13TeV=get_variation("jec_bbec1_2016"),
        scale_j_BBEC1_2017_13TeV=get_variation("jec_bbec1_2017"),
        scale_j_BBEC1_2018_13TeV=get_variation("jec_bbec1_2018"),
        scale_j_EC2_13TeV=get_variation("jec_ec2"),
        scale_j_EC2_2016postVFP_13TeV=get_variation("jec_ec2_2016"),
        scale_j_EC2_2016preVFP_13TeV=get_variation("jec_ec2_2016"),
        scale_j_EC2_2017_13TeV=get_variation("jec_ec2_2017"),
        scale_j_EC2_2018_13TeV=get_variation("jec_ec2_2018"),
        scale_j_FlavorQCD_13TeV=get_variation("jec_flavorqcd"),
        scale_j_HF_13TeV=get_variation("jec_hf"),
        scale_j_HF_2016postVFP_13TeV=get_variation("jec_hf_2016"),
        scale_j_HF_2016preVFP_13TeV=get_variation("jec_hf_2016"),
        scale_j_HF_2017_13TeV=get_variation("jec_hf_2017"),
        scale_j_HF_2018_13TeV=get_variation("jec_hf_2018"),
        scale_j_RelativeBal_13TeV=get_variation("jec_relativebal"),
        scale_j_RelativeSample_2016postVFP_13TeV=get_variation("jec_relativesample_2016"),
        scale_j_RelativeSample_2016preVFP_13TeV=get_variation("jec_relativesample_2016"),
        scale_j_RelativeSample_2017_13TeV=get_variation("jec_relativesample_2017"),
        scale_j_RelativeSample_2018_13TeV=get_variation("jec_relativesample_2018"),
        res_j_13TeV=get_variation("jer"),
        metUncl_13Tev=get_variation("met_unclustered"),
        jms_pnetreg=get_variation("jms"),
        jmr_pnetreg=get_variation("jmr"),
        btagWeightDeepJet_HF_13Tev=get_variation("btagging_scale_factors_HF"),
        btagWeightDeepJet_LF_13Tev=get_variation("btagging_scale_factors_LF"),
        vbsvvh1lep_bTagWeightXbb_13TeV_16preVFP=get_variation("particlenet_w_weight_2016preVFP"),
        vbsvvh1lep_bTagWeightXbb_13TeV_16postVFP=get_variation("particlenet_w_weight_2016postVFP"),
        vbsvvh1lep_bTagWeightXbb_13TeV_17=get_variation("particlenet_w_weight_2017"),
        vbsvvh1lep_bTagWeightXbb_13TeV_18=get_variation("particlenet_w_weight_2018"),
        vbsvvh1lep_qTagWeightXWqq_13TeV_16preVFP=get_variation("particlenet_h_weight_2016preVFP"),
        vbsvvh1lep_qTagWeightXWqq_13TeV_16postVFP=get_variation("particlenet_h_weight_2016postVFP"),
        vbsvvh1lep_qTagWeightXWqq_13TeV_17=get_variation("particlenet_h_weight_2017"),
        vbsvvh1lep_qTagWeightXWqq_13TeV_18=get_variation("particlenet_h_weight_2018")
    )

    write_datacard(sys)


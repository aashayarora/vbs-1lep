import uproot
import numpy as np
import argparse
import scipy
import os

def max_var(variations):
    deviations = [abs(1 - x) for x in variations]
    return variations[deviations.index(max(deviations))]

def get_variation(correction=None, tree="Events"):
    sig_file = "/data/userdata/aaarora/output/run2/ABCDNet_simpleDisco_VBSVVH1lep_30/output/sig_MVA_abcdnet.root"
    
    BDT_CUT = 0.56
    DNN_CUT = 0.92
    
    with uproot.open(sig_file) as f:
        df = f.get(tree).arrays(["VBSBDTscore", "abcdnet_score", "weight"], library="pd")
    
    df = df.drop(df.query("abs(weight) > 100").index)

    a = df.query(f"VBSBDTscore > {BDT_CUT} & abcdnet_score > {DNN_CUT}").weight.sum()
    b = df.query(f"VBSBDTscore < {BDT_CUT} & abcdnet_score > {DNN_CUT}").weight.sum()
    c = df.query(f"VBSBDTscore > {BDT_CUT} & abcdnet_score < {DNN_CUT}").weight.sum()
    d = df.query(f"VBSBDTscore < {BDT_CUT} & abcdnet_score < {DNN_CUT}").weight.sum()

    if correction is None:
        return ["{:.5f}".format(round(i, 5)) for i in [a, b, c, d]]

    variations = []

    with uproot.open(f"/data/userdata/aaarora/output/run2/ABCDNet_simpleDisco_VBSVVH1lep_30/output/sig_{correction}_up_MVA_abcdnet.root") as g:
        df2 = g.get(tree).arrays(["VBSBDTscore", "abcdnet_score", "weight"], library="pd")

    df2 = df2.drop(df2.query("abs(weight) > 100").index)

    a2 = df2.query(f"VBSBDTscore > {BDT_CUT} & abcdnet_score > {DNN_CUT}").weight.sum()
    b2 = df2.query(f"VBSBDTscore < {BDT_CUT} & abcdnet_score > {DNN_CUT}").weight.sum()
    c2 = df2.query(f"VBSBDTscore > {BDT_CUT} & abcdnet_score < {DNN_CUT}").weight.sum()
    d2 = df2.query(f"VBSBDTscore < {BDT_CUT} & abcdnet_score < {DNN_CUT}").weight.sum()

    variations.append([(a - a2) / a, (b - b2) / b, (c - c2) / c, (d - d2) / d])

    with uproot.open(f"/data/userdata/aaarora/output/run2/ABCDNet_simpleDisco_VBSVVH1lep_30/output/sig_{correction}_down_MVA_abcdnet.root") as g:
        df3 = g.get(tree).arrays(["VBSBDTscore", "abcdnet_score", "weight"], library="pd")

    df3 = df3.drop(df3.query("abs(weight) > 100").index)

    a3 = df3.query(f"VBSBDTscore > {BDT_CUT} & abcdnet_score > {DNN_CUT}").weight.sum()
    b3 = df3.query(f"VBSBDTscore < {BDT_CUT} & abcdnet_score > {DNN_CUT}").weight.sum()
    c3 = df3.query(f"VBSBDTscore > {BDT_CUT} & abcdnet_score < {DNN_CUT}").weight.sum()
    d3 = df3.query(f"VBSBDTscore < {BDT_CUT} & abcdnet_score < {DNN_CUT}").weight.sum()

    variations.append([(a - a3) / a, (b - b3) / b, (c - c3) / c, (d - d3) / d])

    return ["{:.5f}".format(round(1 + abs(max_var(x)), 5)) for x in zip(*variations)]

def get_stats(tree="Events"):
    sig_file = "/data/userdata/aaarora/output/run2/ABCDNet_simpleDisco_VBSVVH1lep_30/output/sig_MVA_abcdnet.root"
    
    BDT_CUT = 0.56
    DNN_CUT = 0.92
    
    with uproot.open(sig_file) as f:
        df = f.get(tree).arrays(["VBSBDTscore", "abcdnet_score", "weight"], library="pd")

    df_a = df.query(f"VBSBDTscore > {BDT_CUT} & abcdnet_score > {DNN_CUT}")
    df_b = df.query(f"VBSBDTscore < {BDT_CUT} & abcdnet_score > {DNN_CUT}")
    df_c = df.query(f"VBSBDTscore > {BDT_CUT} & abcdnet_score < {DNN_CUT}")
    df_d = df.query(f"VBSBDTscore < {BDT_CUT} & abcdnet_score < {DNN_CUT}")

    a = 1 + np.sqrt((df_a.weight ** 2).sum()) / df_a.weight.sum()
    b = 1 + np.sqrt((df_b.weight ** 2).sum()) / df_b.weight.sum()
    c = 1 + np.sqrt((df_c.weight ** 2).sum()) / df_c.weight.sum()
    d = 1 + np.sqrt((df_d.weight ** 2).sum()) / df_d.weight.sum()

    return ["{:.4f}".format(round(i, 4)) for i in [a, b, c, d]]

def get_data(tree="Events"):
    data_file = "/data/userdata/aaarora/output/run2/ABCDNet_simpleDisco_VBSVVH1lep_30/output/data_MVA_abcdnet.root"
    
    BDT_CUT = 0.56
    DNN_CUT = 0.92
    
    with uproot.open(data_file) as f:
        df = f.get(tree).arrays(["VBSBDTscore", "abcdnet_score", "weight"], library="pd")

    b = df.query(f"VBSBDTscore < {BDT_CUT} & abcdnet_score > {DNN_CUT}").weight.sum()
    c = df.query(f"VBSBDTscore > {BDT_CUT} & abcdnet_score < {DNN_CUT}").weight.sum()
    d = df.query(f"VBSBDTscore < {BDT_CUT} & abcdnet_score < {DNN_CUT}").weight.sum()

    up1 = round(scipy.stats.gamma.ppf(1- (1-0.9973) / 2, b+1)) #self.obs[1] + 3*(self.obs[1]**0.5)
    up2 = round(scipy.stats.gamma.ppf(1- (1-0.9973) / 2, c+1)) #self.obs[2] + 3*(self.obs[2]**0.5)
    up3 = round(scipy.stats.gamma.ppf(1- (1-0.9973) / 2, d+1)) #self.obs[3] + 3*(self.obs[3]**0.5)
    dn1 = round(scipy.stats.gamma.ppf((1 - 0.9973) / 2, b)) #self.obs[1] - 3*(self.obs[1]**0.5)
    dn2 = round(scipy.stats.gamma.ppf((1 - 0.9973) / 2, c)) #self.obs[2] - 3*(self.obs[2]**0.5)
    dn3 = round(scipy.stats.gamma.ppf((1 - 0.9973) / 2, d)) #self.obs[3] - 3*(self.obs[3]**0.5)

    return [b, c, d, up1, up2, up3, dn1, dn2, dn3]

def xbb_reweight(year):
    if (year == "2016preVFP"):
        return "CMS_vbsvvh1lep_bTagFitXbb_13TeV_16preVFP          lnN   -                  -                  -                  -                  1.03010     1.03010     1.03010     1.03010"
    elif (year == "2016postVFP"):
        return "CMS_vbsvvh1lep_bTagFitXbb_13TeV_16postVFP         lnN   -                  -                  -                  -                  1.01200     1.01200     1.01200     1.01200"
    elif (year == "2017"):
        return "CMS_vbsvvh1lep_bTagFitXbb_13TeV_17                lnN   -                  -                  -                  -                  1.02560     1.02560     1.02560     1.02560"
    elif (year == "2018"):
        return "CMS_vbsvvh1lep_bTagFitXbb_13TeV_18                lnN   -                  -                  -                  -                  1.07080     1.07080     1.07080     1.07080"
    else:
        return """CMS_vbsvvh1lep_bTagFitXbb_13TeV_16preVFP          lnN   -                  -                  -                  -                  1.03010     1.03010     1.03010     1.03010
CMS_vbsvvh1lep_bTagFitXbb_13TeV_16postVFP         lnN   -                  -                  -                  -                  1.01200     1.01200     1.01200     1.01200
CMS_vbsvvh1lep_bTagFitXbb_13TeV_17                lnN   -                  -                  -                  -                  1.02560     1.02560     1.02560     1.02560
CMS_vbsvvh1lep_bTagFitXbb_13TeV_18                lnN   -                  -                  -                  -                  1.07080     1.07080     1.07080     1.07080
"""

if __name__ == "__main__":
    argparser = argparse.ArgumentParser()
    argparser.add_argument("--output", type=str, default="-1", help="Output file name")
    argparser.add_argument("--year", type=str, default="Run2", help="Year of data taking")
    args = argparser.parse_args()

    sys = {
        "sig": get_variation(),
        "data": get_data(),
        "stat_unc": get_stats(),
        "ttH_elec_reco": get_variation("electron_scale_factors_Reco"),
        "ttH_elec_recotoloose": get_variation("electron_scale_factors_ttHID"),
        "ttH_elec_trig": get_variation("electron_scale_factors_trigger"),
        "ttH_elec_loosetoiso": get_variation("electron_scale_factors_ttHISO"),
        "ttH_elec_isototight": get_variation("electron_scale_factors_ID"),
        "ttH_muon_recotoloose": get_variation("muon_scale_factors_ttHID"),
        "ttH_muon_trig": get_variation("muon_scale_factors_trigger"),
        "ttH_muon_loosetoiso": get_variation("muon_scale_factors_ttHISO"),
        "ttH_muon_isototight": get_variation("muon_scale_factors_ID"),
        "PrefireWeight_13TeV": get_variation("L1PreFiringWeight"),
        "vbsvvh_puWeight": get_variation("pileup_weight"),
        "vbsvvh_puJetID": get_variation("pileupid_weight"),
        "scale_j_Absolute_13TeV": get_variation("jec_absolute"),
        "scale_j_Absolute_2016postVFP_13TeV": get_variation("jec_absolute_2016postVFP"),
        "scale_j_Absolute_2016preVFP_13TeV": get_variation("jec_absolute_2016preVFP"),
        "scale_j_Absolute_2017_13TeV": get_variation("jec_absolute_2017"),
        "scale_j_Absolute_2018_13TeV": get_variation("jec_absolute_2018"),
        "scale_j_BBEC1_13TeV": get_variation("jec_bbec1"),
        "scale_j_BBEC1_2016postVFP_13TeV": get_variation("jec_bbec1_2016postVFP"),
        "scale_j_BBEC1_2016preVFP_13TeV": get_variation("jec_bbec1_2016preVFP"),
        "scale_j_BBEC1_2017_13TeV": get_variation("jec_bbec1_2017"),
        "scale_j_BBEC1_2018_13TeV": get_variation("jec_bbec1_2018"),
        "scale_j_EC2_13TeV": get_variation("jec_ec2"),
        "scale_j_EC2_2016postVFP_13TeV": get_variation("jec_ec2_2016postVFP"),
        "scale_j_EC2_2016preVFP_13TeV": get_variation("jec_ec2_2016preVFP"),
        "scale_j_EC2_2017_13TeV": get_variation("jec_ec2_2017"),
        "scale_j_EC2_2018_13TeV": get_variation("jec_ec2_2018"),
        "scale_j_FlavorQCD_13TeV": get_variation("jec_flavorqcd"),
        "scale_j_HF_13TeV": get_variation("jec_hf"),
        "scale_j_HF_2016postVFP_13TeV": get_variation("jec_hf_2016postVFP"),
        "scale_j_HF_2016preVFP_13TeV": get_variation("jec_hf_2016preVFP"),
        "scale_j_HF_2017_13TeV": get_variation("jec_hf_2017"),
        "scale_j_HF_2018_13TeV": get_variation("jec_hf_2018"),
        "scale_j_RelativeBal_13TeV": get_variation("jec_relativebal"),
        "scale_j_RelativeSample_2016postVFP_13TeV": get_variation("jec_relativesample_2016postVFP"),
        "scale_j_RelativeSample_2016preVFP_13TeV": get_variation("jec_relativesample_2016preVFP"),
        "scale_j_RelativeSample_2017_13TeV": get_variation("jec_relativesample_2017"),
        "scale_j_RelativeSample_2018_13TeV": get_variation("jec_relativesample_2018"),
        "res_j_13TeV": get_variation("jer"),
        "metUncl_13Tev": get_variation("met_unclustered"),
        "jms_pnetreg": get_variation("jms"),
        "jmr_pnetreg": get_variation("jmr"),
        "btagWeightDeepJet_HF_13Tev": get_variation("btagging_scale_factors_HF"),
        "btagWeightDeepJet_LF_13Tev": get_variation("btagging_scale_factors_LF"),
        "vbsvvh1lep_bTagWeightXbb_13TeV_16preVFP": get_variation("particlenet_h_weight_2016preVFP"),
        "vbsvvh1lep_bTagWeightXbb_13TeV_16postVFP": get_variation("particlenet_h_weight_2016postVFP"),
        "vbsvvh1lep_bTagWeightXbb_13TeV_17": get_variation("particlenet_h_weight_2017"),
        "vbsvvh1lep_bTagWeightXbb_13TeV_18": get_variation("particlenet_h_weight_2018"),
        "vbsvvh1lep_qTagWeightXWqq_13TeV_16preVFP": get_variation("particlenet_w_weight_2016preVFP"),
        "vbsvvh1lep_qTagWeightXWqq_13TeV_16postVFP": get_variation("particlenet_w_weight_2016postVFP"),
        "vbsvvh1lep_qTagWeightXWqq_13TeV_17": get_variation("particlenet_w_weight_2017"),
        "vbsvvh1lep_qTagWeightXWqq_13TeV_18": get_variation("particlenet_w_weight_2018"),
        "PSWeight_FSR": get_variation("PSWeight_FSR"),
        "PSWeight_ISR": get_variation("PSWeight_ISR"),
        "LHEScaleWeight_muF": get_variation("LHEScaleWeight_muF"),
        "LHEScaleWeight_muR": get_variation("LHEScaleWeight_muR"),
        "LHEWeights_pdf": get_variation("LHEWeights_pdf")
    }

    datacard = f"""imax 4 number of channels
jmax 1 number of backgrounds
kmax * number of nuisance parameters
--------------------------------------------------------------------------------------------------------------------------------
bin                                                     A                  B                  C                  D                  
observation                                             1                  {sys["data"][0]}                 {sys["data"][1]}                 {sys["data"][2]}               
--------------------------------------------------------------------------------------------------------------------------------
bin                                                     A                  B                  C                  D                  A           B           C           D           
process                                                 TotalBkg_OneLep    TotalBkg_OneLep    TotalBkg_OneLep    TotalBkg_OneLep    TotalSig    TotalSig    TotalSig    TotalSig    
process                                                 1                  1                  1                  1                  0           0           0           0           
rate                                                    1                  1                  1                  1                  {sys["sig"][0]}     {sys["sig"][1]}     {sys["sig"][2]}     {sys["sig"][3]}    
--------------------------------------------------------------------------------------------------------------------------------
CMS_vbsvvh1lep_control_abcd_syst                  lnN   1.2                -                  -                  -                  -           -           -           -           
lumi_13TeV_correlated                             lnN   -                  -                  -                  -                  1.016       1.016       1.016       1.016       
CMS_vbsvvh1lep_signal_RegionA                     lnN   -                  -                  -                  -                  {sys["stat_unc"][0]}      -           -           -
CMS_vbsvvh1lep_signal_RegionB                     lnN   -                  -                  -                  -                  -           {sys["stat_unc"][1]}      -           -           
CMS_vbsvvh1lep_signal_RegionC                     lnN   -                  -                  -                  -                  -           -           {sys["stat_unc"][2]}      -           
CMS_vbsvvh1lep_signal_RegionD                     lnN   -                  -                  -                  -                  -           -           -           {sys["stat_unc"][3]}     
CMS_LHE_weights_pdf_vbsvvh                        lnN   -                  -                  -                  -                  {sys["LHEWeights_pdf"][0]}     {sys["LHEWeights_pdf"][1]}     {sys["LHEWeights_pdf"][2]}     {sys["LHEWeights_pdf"][3]}
CMS_ttH_elec_reco                                 lnN   -                  -                  -                  -                  {sys["ttH_elec_reco"][0]}     {sys["ttH_elec_reco"][1]}     {sys["ttH_elec_reco"][2]}     {sys["ttH_elec_reco"][3]}
CMS_ttH_elec_recotoloose                          lnN   -                  -                  -                  -                  {sys["ttH_elec_recotoloose"][0]}     {sys["ttH_elec_recotoloose"][1]}     {sys["ttH_elec_recotoloose"][2]}     {sys["ttH_elec_recotoloose"][3]}
CMS_ttH_elec_trig                                 lnN   -                  -                  -                  -                  {sys["ttH_elec_trig"][0]}     {sys["ttH_elec_trig"][1]}     {sys["ttH_elec_trig"][2]}     {sys["ttH_elec_trig"][3]}
CMS_ttH_elec_loosetoiso                           lnN   -                  -                  -                  -                  {sys["ttH_elec_loosetoiso"][0]}     {sys["ttH_elec_loosetoiso"][1]}     {sys["ttH_elec_loosetoiso"][2]}     {sys["ttH_elec_loosetoiso"][3]}
CMS_ttH_elec_isototight                           lnN   -                  -                  -                  -                  {sys["ttH_elec_isototight"][0]}     {sys["ttH_elec_isototight"][1]}     {sys["ttH_elec_isototight"][2]}     {sys["ttH_elec_isototight"][3]}
CMS_ttH_muon_recotoloose                          lnN   -                  -                  -                  -                  {sys["ttH_muon_recotoloose"][0]}     {sys["ttH_muon_recotoloose"][1]}     {sys["ttH_muon_recotoloose"][2]}     {sys["ttH_muon_recotoloose"][3]}
CMS_ttH_muon_trig                                 lnN   -                  -                  -                  -                  {sys["ttH_muon_trig"][0]}     {sys["ttH_muon_trig"][1]}     {sys["ttH_muon_trig"][2]}     {sys["ttH_muon_trig"][3]}
CMS_ttH_muon_loosetoiso                           lnN   -                  -                  -                  -                  {sys["ttH_muon_loosetoiso"][0]}     {sys["ttH_muon_loosetoiso"][1]}     {sys["ttH_muon_loosetoiso"][2]}     {sys["ttH_muon_loosetoiso"][3]}
CMS_ttH_muon_isototight                           lnN   -                  -                  -                  -                  {sys["ttH_muon_isototight"][0]}     {sys["ttH_muon_isototight"][1]}     {sys["ttH_muon_isototight"][2]}     {sys["ttH_muon_isototight"][3]}
CMS_PSWeight_FSR_vbsvvh                           lnN   -                  -                  -                  -                  {sys["PSWeight_FSR"][0]}     {sys["PSWeight_FSR"][1]}     {sys["PSWeight_FSR"][2]}     {sys["PSWeight_FSR"][3]}
CMS_PSWeight_ISR_vbsvvh                           lnN   -                  -                  -                  -                  {sys["PSWeight_ISR"][0]}     {sys["PSWeight_ISR"][1]}     {sys["PSWeight_ISR"][2]}     {sys["PSWeight_ISR"][3]}
CMS_PrefireWeight_13TeV                           lnN   -                  -                  -                  -                  {sys["PrefireWeight_13TeV"][0]}     {sys["PrefireWeight_13TeV"][1]}     {sys["PrefireWeight_13TeV"][2]}     {sys["PrefireWeight_13TeV"][3]}
CMS_vbsvvh_puWeight                               lnN   -                  -                  -                  -                  {sys["vbsvvh_puWeight"][0]}     {sys["vbsvvh_puWeight"][1]}     {sys["vbsvvh_puWeight"][2]}     {sys["vbsvvh_puWeight"][3]}
CMS_LHE_weights_scale_muF_vbsvvh                  lnN   -                  -                  -                  -                  {sys["LHEScaleWeight_muF"][0]}     {sys["LHEScaleWeight_muF"][1]}     {sys["LHEScaleWeight_muF"][2]}     {sys["LHEScaleWeight_muF"][3]}
CMS_LHE_weights_scale_muR_vbsvvh                  lnN   -                  -                  -                  -                  {sys["LHEScaleWeight_muR"][0]}     {sys["LHEScaleWeight_muR"][1]}     {sys["LHEScaleWeight_muR"][2]}     {sys["LHEScaleWeight_muR"][3]}
CMS_vbsvvh_puJetID                                lnN   -                  -                  -                  -                  {sys["vbsvvh_puJetID"][0]}     {sys["vbsvvh_puJetID"][1]}     {sys["vbsvvh_puJetID"][2]}     {sys["vbsvvh_puJetID"][3]}
CMS_scale_j_Absolute_13TeV                        lnN   -                  -                  -                  -                  {sys["scale_j_Absolute_13TeV"][0]}     {sys["scale_j_Absolute_13TeV"][1]}     {sys["scale_j_Absolute_13TeV"][2]}     {sys["scale_j_Absolute_13TeV"][3]}
CMS_scale_j_Absolute_2016postVFP_13TeV            lnN   -                  -                  -                  -                  {sys["scale_j_Absolute_2016postVFP_13TeV"][0]}     {sys["scale_j_Absolute_2016postVFP_13TeV"][1]}     {sys["scale_j_Absolute_2016postVFP_13TeV"][2]}     {sys["scale_j_Absolute_2016postVFP_13TeV"][3]}
CMS_scale_j_Absolute_2016preVFP_13TeV             lnN   -                  -                  -                  -                  {sys["scale_j_Absolute_2016preVFP_13TeV"][0]}     {sys["scale_j_Absolute_2016preVFP_13TeV"][1]}     {sys["scale_j_Absolute_2016preVFP_13TeV"][2]}     {sys["scale_j_Absolute_2016preVFP_13TeV"][3]}
CMS_scale_j_Absolute_2017_13TeV                   lnN   -                  -                  -                  -                  {sys["scale_j_Absolute_2017_13TeV"][0]}     {sys["scale_j_Absolute_2017_13TeV"][1]}     {sys["scale_j_Absolute_2017_13TeV"][2]}     {sys["scale_j_Absolute_2017_13TeV"][3]}
CMS_scale_j_Absolute_2018_13TeV                   lnN   -                  -                  -                  -                  {sys["scale_j_Absolute_2018_13TeV"][0]}     {sys["scale_j_Absolute_2018_13TeV"][1]}     {sys["scale_j_Absolute_2018_13TeV"][2]}     {sys["scale_j_Absolute_2018_13TeV"][3]}
CMS_scale_j_BBEC1_13TeV                           lnN   -                  -                  -                  -                  {sys["scale_j_BBEC1_13TeV"][0]}     {sys["scale_j_BBEC1_13TeV"][1]}     {sys["scale_j_BBEC1_13TeV"][2]}     {sys["scale_j_BBEC1_13TeV"][3]}
CMS_scale_j_BBEC1_2016postVFP_13TeV               lnN   -                  -                  -                  -                  {sys["scale_j_BBEC1_2016postVFP_13TeV"][0]}     {sys["scale_j_BBEC1_2016postVFP_13TeV"][1]}     {sys["scale_j_BBEC1_2016postVFP_13TeV"][2]}     {sys["scale_j_BBEC1_2016postVFP_13TeV"][3]}
CMS_scale_j_BBEC1_2016preVFP_13TeV                lnN   -                  -                  -                  -                  {sys["scale_j_BBEC1_2016preVFP_13TeV"][0]}     {sys["scale_j_BBEC1_2016preVFP_13TeV"][1]}     {sys["scale_j_BBEC1_2016preVFP_13TeV"][2]}     {sys["scale_j_BBEC1_2016preVFP_13TeV"][3]}
CMS_scale_j_BBEC1_2017_13TeV                      lnN   -                  -                  -                  -                  {sys["scale_j_BBEC1_2017_13TeV"][0]}     {sys["scale_j_BBEC1_2017_13TeV"][1]}     {sys["scale_j_BBEC1_2017_13TeV"][2]}     {sys["scale_j_BBEC1_2017_13TeV"][3]}
CMS_scale_j_BBEC1_2018_13TeV                      lnN   -                  -                  -                  -                  {sys["scale_j_BBEC1_2018_13TeV"][0]}     {sys["scale_j_BBEC1_2018_13TeV"][1]}     {sys["scale_j_BBEC1_2018_13TeV"][2]}     {sys["scale_j_BBEC1_2018_13TeV"][3]}
CMS_scale_j_EC2_13TeV                             lnN   -                  -                  -                  -                  {sys["scale_j_EC2_13TeV"][0]}     {sys["scale_j_EC2_13TeV"][1]}     {sys["scale_j_EC2_13TeV"][2]}     {sys["scale_j_EC2_13TeV"][3]}
CMS_scale_j_EC2_2016postVFP_13TeV                 lnN   -                  -                  -                  -                  {sys["scale_j_EC2_2016postVFP_13TeV"][0]}     {sys["scale_j_EC2_2016postVFP_13TeV"][1]}     {sys["scale_j_EC2_2016postVFP_13TeV"][2]}     {sys["scale_j_EC2_2016postVFP_13TeV"][3]}
CMS_scale_j_EC2_2016preVFP_13TeV                  lnN   -                  -                  -                  -                  {sys["scale_j_EC2_2016preVFP_13TeV"][0]}     {sys["scale_j_EC2_2016preVFP_13TeV"][1]}     {sys["scale_j_EC2_2016preVFP_13TeV"][2]}     {sys["scale_j_EC2_2016preVFP_13TeV"][3]}
CMS_scale_j_EC2_2017_13TeV                        lnN   -                  -                  -                  -                  {sys["scale_j_EC2_2017_13TeV"][0]}     {sys["scale_j_EC2_2017_13TeV"][1]}     {sys["scale_j_EC2_2017_13TeV"][2]}     {sys["scale_j_EC2_2017_13TeV"][3]}
CMS_scale_j_EC2_2018_13TeV                        lnN   -                  -                  -                  -                  {sys["scale_j_EC2_2018_13TeV"][0]}     {sys["scale_j_EC2_2018_13TeV"][1]}     {sys["scale_j_EC2_2018_13TeV"][2]}     {sys["scale_j_EC2_2018_13TeV"][3]}
CMS_scale_j_FlavorQCD_13TeV                       lnN   -                  -                  -                  -                  {sys["scale_j_FlavorQCD_13TeV"][0]}     {sys["scale_j_FlavorQCD_13TeV"][1]}     {sys["scale_j_FlavorQCD_13TeV"][2]}     {sys["scale_j_FlavorQCD_13TeV"][3]}
CMS_scale_j_HF_13TeV                              lnN   -                  -                  -                  -                  {sys["scale_j_HF_13TeV"][0]}     {sys["scale_j_HF_13TeV"][1]}     {sys["scale_j_HF_13TeV"][2]}     {sys["scale_j_HF_13TeV"][3]}
CMS_scale_j_HF_2016postVFP_13TeV                  lnN   -                  -                  -                  -                  {sys["scale_j_HF_2016postVFP_13TeV"][0]}     {sys["scale_j_HF_2016postVFP_13TeV"][1]}     {sys["scale_j_HF_2016postVFP_13TeV"][2]}     {sys["scale_j_HF_2016postVFP_13TeV"][3]}
CMS_scale_j_HF_2016preVFP_13TeV                   lnN   -                  -                  -                  -                  {sys["scale_j_HF_2016preVFP_13TeV"][0]}     {sys["scale_j_HF_2016preVFP_13TeV"][1]}     {sys["scale_j_HF_2016preVFP_13TeV"][2]}     {sys["scale_j_HF_2016preVFP_13TeV"][3]}
CMS_scale_j_HF_2017_13TeV                         lnN   -                  -                  -                  -                  {sys["scale_j_HF_2017_13TeV"][0]}     {sys["scale_j_HF_2017_13TeV"][1]}     {sys["scale_j_HF_2017_13TeV"][2]}     {sys["scale_j_HF_2017_13TeV"][3]}
CMS_scale_j_HF_2018_13TeV                         lnN   -                  -                  -                  -                  {sys["scale_j_HF_2018_13TeV"][0]}     {sys["scale_j_HF_2018_13TeV"][1]}     {sys["scale_j_HF_2018_13TeV"][2]}     {sys["scale_j_HF_2018_13TeV"][3]}
CMS_scale_j_RelativeBal_13TeV                     lnN   -                  -                  -                  -                  {sys["scale_j_RelativeBal_13TeV"][0]}     {sys["scale_j_RelativeBal_13TeV"][1]}     {sys["scale_j_RelativeBal_13TeV"][2]}     {sys["scale_j_RelativeBal_13TeV"][3]}
CMS_scale_j_RelativeSample_2016postVFP_13TeV      lnN   -                  -                  -                  -                  {sys["scale_j_RelativeSample_2016postVFP_13TeV"][0]}     {sys["scale_j_RelativeSample_2016postVFP_13TeV"][1]}     {sys["scale_j_RelativeSample_2016postVFP_13TeV"][2]}     {sys["scale_j_RelativeSample_2016postVFP_13TeV"][3]}
CMS_scale_j_RelativeSample_2016preVFP_13TeV       lnN   -                  -                  -                  -                  {sys["scale_j_RelativeSample_2016preVFP_13TeV"][0]}     {sys["scale_j_RelativeSample_2016preVFP_13TeV"][1]}     {sys["scale_j_RelativeSample_2016preVFP_13TeV"][2]}     {sys["scale_j_RelativeSample_2016preVFP_13TeV"][3]}
CMS_scale_j_RelativeSample_2017_13TeV             lnN   -                  -                  -                  -                  {sys["scale_j_RelativeSample_2017_13TeV"][0]}     {sys["scale_j_RelativeSample_2017_13TeV"][1]}     {sys["scale_j_RelativeSample_2017_13TeV"][2]}     {sys["scale_j_RelativeSample_2017_13TeV"][3]}
CMS_scale_j_RelativeSample_2018_13TeV             lnN   -                  -                  -                  -                  {sys["scale_j_RelativeSample_2018_13TeV"][0]}     {sys["scale_j_RelativeSample_2018_13TeV"][1]}     {sys["scale_j_RelativeSample_2018_13TeV"][2]}     {sys["scale_j_RelativeSample_2018_13TeV"][3]}
CMS_res_j_13TeV                                   lnN   -                  -                  -                  -                  {sys["res_j_13TeV"][0]}     {sys["res_j_13TeV"][1]}     {sys["res_j_13TeV"][2]}     {sys["res_j_13TeV"][3]}
CMS_metUncl_13Tev                                 lnN   -                  -                  -                  -                  {sys["metUncl_13Tev"][0]}     {sys["metUncl_13Tev"][1]}     {sys["metUncl_13Tev"][2]}     {sys["metUncl_13Tev"][3]}
CMS_jms_pnetreg_13TeV                             lnN   -                  -                  -                  -                  {sys["jms_pnetreg"][0]}     {sys["jms_pnetreg"][1]}     {sys["jms_pnetreg"][2]}     {sys["jms_pnetreg"][3]}
CMS_jmr_pnetreg_13TeV                             lnN   -                  -                  -                  -                  {sys["jmr_pnetreg"][0]}     {sys["jmr_pnetreg"][1]}     {sys["jmr_pnetreg"][2]}     {sys["jmr_pnetreg"][3]}
CMS_btagWeightDeepJet_HF_13Tev                    lnN   -                  -                  -                  -                  {sys["btagWeightDeepJet_HF_13Tev"][0]}     {sys["btagWeightDeepJet_HF_13Tev"][1]}     {sys["btagWeightDeepJet_HF_13Tev"][2]}     {sys["btagWeightDeepJet_HF_13Tev"][3]}
CMS_btagWeightDeepJet_LF_13Tev                    lnN   -                  -                  -                  -                  {sys["btagWeightDeepJet_LF_13Tev"][0]}     {sys["btagWeightDeepJet_LF_13Tev"][1]}     {sys["btagWeightDeepJet_LF_13Tev"][2]}     {sys["btagWeightDeepJet_LF_13Tev"][3]}
CMS_vbsvvh1lep_qTagWeightXWqq_13TeV_16preVFP      lnN   -                  -                  -                  -                  {sys["vbsvvh1lep_qTagWeightXWqq_13TeV_16preVFP"][0]}     {sys["vbsvvh1lep_qTagWeightXWqq_13TeV_16preVFP"][1]}     {sys["vbsvvh1lep_qTagWeightXWqq_13TeV_16preVFP"][2]}     {sys["vbsvvh1lep_qTagWeightXWqq_13TeV_16preVFP"][3]}
CMS_vbsvvh1lep_qTagWeightXWqq_13TeV_16postVFP     lnN   -                  -                  -                  -                  {sys["vbsvvh1lep_qTagWeightXWqq_13TeV_16postVFP"][0]}     {sys["vbsvvh1lep_qTagWeightXWqq_13TeV_16postVFP"][1]}     {sys["vbsvvh1lep_qTagWeightXWqq_13TeV_16postVFP"][2]}     {sys["vbsvvh1lep_qTagWeightXWqq_13TeV_16postVFP"][3]}
CMS_vbsvvh1lep_qTagWeightXWqq_13TeV_17            lnN   -                  -                  -                  -                  {sys["vbsvvh1lep_qTagWeightXWqq_13TeV_17"][0]}     {sys["vbsvvh1lep_qTagWeightXWqq_13TeV_17"][1]}     {sys["vbsvvh1lep_qTagWeightXWqq_13TeV_17"][2]}     {sys["vbsvvh1lep_qTagWeightXWqq_13TeV_17"][3]}
CMS_vbsvvh1lep_qTagWeightXWqq_13TeV_18            lnN   -                  -                  -                  -                  {sys["vbsvvh1lep_qTagWeightXWqq_13TeV_18"][0]}     {sys["vbsvvh1lep_qTagWeightXWqq_13TeV_18"][1]}     {sys["vbsvvh1lep_qTagWeightXWqq_13TeV_18"][2]}     {sys["vbsvvh1lep_qTagWeightXWqq_13TeV_18"][3]}                  
CMS_vbsvvh1lep_bTagWeightXbb_13TeV_16preVFP       lnN   -                  -                  -                  -                  {sys["vbsvvh1lep_bTagWeightXbb_13TeV_16preVFP"][0]}     {sys["vbsvvh1lep_bTagWeightXbb_13TeV_16preVFP"][1]}     {sys["vbsvvh1lep_bTagWeightXbb_13TeV_16preVFP"][2]}     {sys["vbsvvh1lep_bTagWeightXbb_13TeV_16preVFP"][3]}
CMS_vbsvvh1lep_bTagWeightXbb_13TeV_16postVFP      lnN   -                  -                  -                  -                  {sys["vbsvvh1lep_bTagWeightXbb_13TeV_16postVFP"][0]}     {sys["vbsvvh1lep_bTagWeightXbb_13TeV_16postVFP"][1]}     {sys["vbsvvh1lep_bTagWeightXbb_13TeV_16postVFP"][2]}     {sys["vbsvvh1lep_bTagWeightXbb_13TeV_16postVFP"][3]}
CMS_vbsvvh1lep_bTagWeightXbb_13TeV_17             lnN   -                  -                  -                  -                  {sys["vbsvvh1lep_bTagWeightXbb_13TeV_17"][0]}     {sys["vbsvvh1lep_bTagWeightXbb_13TeV_17"][1]}     {sys["vbsvvh1lep_bTagWeightXbb_13TeV_17"][2]}     {sys["vbsvvh1lep_bTagWeightXbb_13TeV_17"][3]}
CMS_vbsvvh1lep_bTagWeightXbb_13TeV_18             lnN   -                  -                  -                  -                  {sys["vbsvvh1lep_bTagWeightXbb_13TeV_18"][0]}     {sys["vbsvvh1lep_bTagWeightXbb_13TeV_18"][1]}     {sys["vbsvvh1lep_bTagWeightXbb_13TeV_18"][2]}     {sys["vbsvvh1lep_bTagWeightXbb_13TeV_18"][3]}
{xbb_reweight(args.year)}
--------------------------------------------------------------------------------------------------------------------------------
A_OneLep rateParam                  A  TotalBkg_OneLep    (@0*@1/@2) B_OneLep,C_OneLep,D_OneLep    
B_OneLep rateParam                  B  TotalBkg_OneLep    {sys["data"][0]} [{sys["data"][6]},{sys["data"][3]}]              
C_OneLep rateParam                  C  TotalBkg_OneLep    {sys["data"][1]} [{sys["data"][7]},{sys["data"][4]}]    
D_OneLep rateParam                  D  TotalBkg_OneLep    {sys["data"][2]} [{sys["data"][8]},{sys["data"][5]}]
"""

    names = [
        "scan_CV_1p0_C2V_m2p0_C3_1p0",
        "scan_CV_1p0_C2V_m1p75_C3_1p0",
        "scan_CV_1p0_C2V_m1p5_C3_1p0",
        "scan_CV_1p0_C2V_m1p25_C3_1p0",
        "scan_CV_1p0_C2V_m1p0_C3_1p0",
        "scan_CV_1p0_C2V_m0p75_C3_1p0",
        "scan_CV_1p0_C2V_m0p5_C3_1p0",
        "scan_CV_1p0_C2V_m0p25_C3_1p0",
        "scan_CV_1p0_C2V_0p0_C3_1p0",
        "scan_CV_1p0_C2V_0p1_C3_1p0",
        "scan_CV_1p0_C2V_0p2_C3_1p0",
        "scan_CV_1p0_C2V_0p3_C3_1p0",
        "scan_CV_1p0_C2V_0p4_C3_1p0",
        "scan_CV_1p0_C2V_0p5_C3_1p0",
        "scan_CV_1p0_C2V_0p6_C3_1p0",
        "scan_CV_1p0_C2V_0p7_C3_1p0",
        "scan_CV_1p0_C2V_0p8_C3_1p0",
        "scan_CV_1p0_C2V_0p9_C3_1p0",
        "scan_CV_1p0_C2V_1p0_C3_1p0",
        "scan_CV_1p0_C2V_1p1_C3_1p0",
        "scan_CV_1p0_C2V_1p2_C3_1p0",
        "scan_CV_1p0_C2V_1p3_C3_1p0",
        "scan_CV_1p0_C2V_1p4_C3_1p0",
        "scan_CV_1p0_C2V_1p5_C3_1p0",
        "scan_CV_1p0_C2V_1p6_C3_1p0",
        "scan_CV_1p0_C2V_1p7_C3_1p0",
        "scan_CV_1p0_C2V_1p8_C3_1p0",
        "scan_CV_1p0_C2V_1p9_C3_1p0",
        "scan_CV_1p0_C2V_2p0_C3_1p0",
        "scan_CV_1p0_C2V_2p25_C3_1p0",
        "scan_CV_1p0_C2V_2p5_C3_1p0",
        "scan_CV_1p0_C2V_2p75_C3_1p0",
        "scan_CV_1p0_C2V_3p0_C3_1p0",
        "scan_CV_1p0_C2V_3p25_C3_1p0",
        "scan_CV_1p0_C2V_3p5_C3_1p0",
        "scan_CV_1p0_C2V_3p75_C3_1p0",
        "scan_CV_1p0_C2V_4p0_C3_1p0"
    ]

    names = [ 
        "scan_C2W_m2p0_C2Z_m2p0", 
        "scan_C2W_m2p0_C2Z_m1p0", 
        "scan_C2W_m2p0_C2Z_m0p5", 
        "scan_C2W_m2p0_C2Z_0p0", 
        "scan_C2W_m2p0_C2Z_0p5", 
        "scan_C2W_m2p0_C2Z_1p0", 
        "scan_C2W_m2p0_C2Z_1p5", 
        "scan_C2W_m2p0_C2Z_2p0", 
        "scan_C2W_m2p0_C2Z_2p5", 
        "scan_C2W_m2p0_C2Z_3p0", 
        "scan_C2W_m2p0_C2Z_4p0", 
        "scan_C2W_m1p0_C2Z_m2p0", 
        "scan_C2W_m1p0_C2Z_m1p0", 
        "scan_C2W_m1p0_C2Z_m0p5", 
        "scan_C2W_m1p0_C2Z_0p0", 
        "scan_C2W_m1p0_C2Z_0p5", 
        "scan_C2W_m1p0_C2Z_1p0", 
        "scan_C2W_m1p0_C2Z_1p5", 
        "scan_C2W_m1p0_C2Z_2p0", 
        "scan_C2W_m1p0_C2Z_2p5", 
        "scan_C2W_m1p0_C2Z_3p0", 
        "scan_C2W_m1p0_C2Z_4p0", 
        "scan_C2W_m0p5_C2Z_m2p0", 
        "scan_C2W_m0p5_C2Z_m1p0", 
        "scan_C2W_m0p5_C2Z_m0p5", 
        "scan_C2W_m0p5_C2Z_0p0", 
        "scan_C2W_m0p5_C2Z_0p5", 
        "scan_C2W_m0p5_C2Z_1p0", 
        "scan_C2W_m0p5_C2Z_1p5", 
        "scan_C2W_m0p5_C2Z_2p0", 
        "scan_C2W_m0p5_C2Z_2p5", 
        "scan_C2W_m0p5_C2Z_3p0", 
        "scan_C2W_m0p5_C2Z_4p0", 
        "scan_C2W_0p0_C2Z_m2p0", 
        "scan_C2W_0p0_C2Z_m1p0", 
        "scan_C2W_0p0_C2Z_m0p5", 
        "scan_C2W_0p0_C2Z_0p0", 
        "scan_C2W_0p0_C2Z_0p5", 
        "scan_C2W_0p0_C2Z_1p0", 
        "scan_C2W_0p0_C2Z_1p5", 
        "scan_C2W_0p0_C2Z_2p0", 
        "scan_C2W_0p0_C2Z_2p5", 
        "scan_C2W_0p0_C2Z_3p0", 
        "scan_C2W_0p0_C2Z_4p0", 
        "scan_C2W_0p5_C2Z_m2p0", 
        "scan_C2W_0p5_C2Z_m1p0", 
        "scan_C2W_0p5_C2Z_m0p5", 
        "scan_C2W_0p5_C2Z_0p0", 
        "scan_C2W_0p5_C2Z_0p5", 
        "scan_C2W_0p5_C2Z_1p0", 
        "scan_C2W_0p5_C2Z_1p5", 
        "scan_C2W_0p5_C2Z_2p0", 
        "scan_C2W_0p5_C2Z_2p5", 
        "scan_C2W_0p5_C2Z_3p0", 
        "scan_C2W_0p5_C2Z_4p0", 
        "scan_C2W_1p0_C2Z_m2p0", 
        "scan_C2W_1p0_C2Z_m1p0", 
        "scan_C2W_1p0_C2Z_m0p5", 
        "scan_C2W_1p0_C2Z_0p0", 
        "scan_C2W_1p0_C2Z_0p5", 
        "scan_C2W_1p0_C2Z_1p0", 
        "scan_C2W_1p0_C2Z_1p5", 
        "scan_C2W_1p0_C2Z_2p0", 
        "scan_C2W_1p0_C2Z_2p5", 
        "scan_C2W_1p0_C2Z_3p0", 
        "scan_C2W_1p0_C2Z_4p0", 
        "scan_C2W_1p5_C2Z_m2p0", 
        "scan_C2W_1p5_C2Z_m1p0", 
        "scan_C2W_1p5_C2Z_m0p5", 
        "scan_C2W_1p5_C2Z_0p0", 
        "scan_C2W_1p5_C2Z_0p5", 
        "scan_C2W_1p5_C2Z_1p0", 
        "scan_C2W_1p5_C2Z_1p5", 
        "scan_C2W_1p5_C2Z_2p0", 
        "scan_C2W_1p5_C2Z_2p5", 
        "scan_C2W_1p5_C2Z_3p0", 
        "scan_C2W_1p5_C2Z_4p0", 
        "scan_C2W_2p0_C2Z_m2p0", 
        "scan_C2W_2p0_C2Z_m1p0", 
        "scan_C2W_2p0_C2Z_m0p5", 
        "scan_C2W_2p0_C2Z_0p0", 
        "scan_C2W_2p0_C2Z_0p5", 
        "scan_C2W_2p0_C2Z_1p0", 
        "scan_C2W_2p0_C2Z_1p5", 
        "scan_C2W_2p0_C2Z_2p0", 
        "scan_C2W_2p0_C2Z_2p5", 
        "scan_C2W_2p0_C2Z_3p0", 
        "scan_C2W_2p0_C2Z_4p0", 
        "scan_C2W_2p5_C2Z_m2p0", 
        "scan_C2W_2p5_C2Z_m1p0", 
        "scan_C2W_2p5_C2Z_m0p5", 
        "scan_C2W_2p5_C2Z_0p0", 
        "scan_C2W_2p5_C2Z_0p5", 
        "scan_C2W_2p5_C2Z_1p0", 
        "scan_C2W_2p5_C2Z_1p5", 
        "scan_C2W_2p5_C2Z_2p0", 
        "scan_C2W_2p5_C2Z_2p5", 
        "scan_C2W_2p5_C2Z_3p0", 
        "scan_C2W_2p5_C2Z_4p0", 
        "scan_C2W_3p0_C2Z_m2p0", 
        "scan_C2W_3p0_C2Z_m1p0", 
        "scan_C2W_3p0_C2Z_m0p5", 
        "scan_C2W_3p0_C2Z_0p0", 
        "scan_C2W_3p0_C2Z_0p5", 
        "scan_C2W_3p0_C2Z_1p0", 
        "scan_C2W_3p0_C2Z_1p5", 
        "scan_C2W_3p0_C2Z_2p0", 
        "scan_C2W_3p0_C2Z_2p5", 
        "scan_C2W_3p0_C2Z_3p0", 
        "scan_C2W_3p0_C2Z_4p0", 
        "scan_C2W_4p0_C2Z_m2p0", 
        "scan_C2W_4p0_C2Z_m1p0", 
        "scan_C2W_4p0_C2Z_m0p5", 
        "scan_C2W_4p0_C2Z_0p0", 
        "scan_C2W_4p0_C2Z_0p5", 
        "scan_C2W_4p0_C2Z_1p0", 
        "scan_C2W_4p0_C2Z_1p5", 
        "scan_C2W_4p0_C2Z_2p0", 
        "scan_C2W_4p0_C2Z_2p5", 
        "scan_C2W_4p0_C2Z_3p0", 
        "scan_C2W_4p0_C2Z_4p0",
    ]

    myrange=list(range(0, len(names)-1))
    myrange.insert(names.index("scan_C2W_2p0_C2Z_2p0"), -1)
    # myrange.insert(names.index("scan_CV_1p0_C2V_2p0_C3_1p0"), -1)

    myrange = [str(i) for i in myrange]

    datacard_name = names[myrange.index(args.output)]
    output_dir = f"datacards/{args.year}"

    # check if directory for year exists, if not create it
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    with open(f"{output_dir}/{datacard_name}.dat" , "w") as f:
        f.write(datacard)
from glob import glob
import json
import argparse
import uproot

def extract_sample_year(sample):
    if "UL16" in sample and "APV" in sample:
        return "2016preVFP"
    elif "UL16" in sample and not "APV" in sample:
        return "2016postVFP"
    elif "UL17" in sample or "UL2017" in sample:
        return "2017"
    elif "UL18" in sample or "UL2018" in sample:
        return "2018"
    elif ("Run2016B" in sample or "Run2016C" in sample or "Run2016D" in sample or "Run2016E" in sample or "Run2016F" in sample) and "HIPM" in sample:
        return "2016preVFP"
    elif ("Run2016F" in sample or "Run2016G" in sample or "Run2016H" in sample) and not "HIPM" in sample:
        return "2016postVFP"
    else:
        print("Error: year not found for " + sample)
        return None

def get_xsec_weight(sample):
    xsec = 0
    if(sample == "DYJetsToLL_M-10to50,2016preVFP"):
        xsec = 20657.0
    elif(sample == "DYJetsToLL_M-10to50,2016postVFP"):
        xsec = 20657.0
    elif(sample == "DYJetsToLL_M-10to50,2017"):
        xsec = 20657.0
    elif(sample == "DYJetsToLL_M-10to50,2018"):
        xsec = 20657.0
    elif(sample == "DYJetsToLL_M-50,2016preVFP"):
        xsec = 6198.0
    elif(sample == "DYJetsToLL_M-50,2016postVFP"):
        xsec = 6198.0
    elif(sample == "DYJetsToLL_M-50,2017"):
        xsec = 6198.0
    elif(sample == "DYJetsToLL_M-50,2018"):
        xsec = 6198.0
    elif(sample == "EWKWMinus2Jets_WToLNu_M-50,2016preVFP"):
        xsec = 32.26
    elif(sample == "EWKWMinus2Jets_WToLNu_M-50,2016postVFP"):
        xsec = 32.26
    elif(sample == "EWKWMinus2Jets_WToLNu_M-50,2017"):
        xsec = 32.26
    elif(sample == "EWKWMinus2Jets_WToLNu_M-50,2018"):
        xsec = 32.26
    elif(sample == "EWKWPlus2Jets_WToLNu_M-50,2016preVFP"):
        xsec = 39.33
    elif(sample == "EWKWPlus2Jets_WToLNu_M-50,2016postVFP"):
        xsec = 39.33
    elif(sample == "EWKWPlus2Jets_WToLNu_M-50,2017"):
        xsec = 39.33
    elif(sample == "EWKWPlus2Jets_WToLNu_M-50,2018"):
        xsec = 39.33
    elif(sample == "EWKWminus2Jets_WToQQ_dipoleRecoilOn,2016preVFP"):
        xsec = 10.67
    elif(sample == "EWKWminus2Jets_WToQQ_dipoleRecoilOn,2016postVFP"):
        xsec = 10.67
    elif(sample == "EWKWminus2Jets_WToQQ_dipoleRecoilOn,2017"):
        xsec = 10.67
    elif(sample == "EWKWminus2Jets_WToQQ_dipoleRecoilOn,2018"):
        xsec = 10.67
    elif(sample == "EWKWplus2Jets_WToQQ_dipoleRecoilOn,2016preVFP"):
        xsec = 10.67
    elif(sample == "EWKWplus2Jets_WToQQ_dipoleRecoilOn,2016postVFP"):
        xsec = 10.67
    elif(sample == "EWKWplus2Jets_WToQQ_dipoleRecoilOn,2017"):
        xsec = 10.67
    elif(sample == "EWKWplus2Jets_WToQQ_dipoleRecoilOn,2018"):
        xsec = 10.67
    elif(sample == "EWKZ2Jets_ZToLL_M-50,2016preVFP"):
        xsec = 6.22
    elif(sample == "EWKZ2Jets_ZToLL_M-50,2016postVFP"):
        xsec = 6.22
    elif(sample == "EWKZ2Jets_ZToLL_M-50,2017"):
        xsec = 6.22
    elif(sample == "EWKZ2Jets_ZToLL_M-50,2018"):
        xsec = 6.22
    elif(sample == "EWKZ2Jets_ZToNuNu_M-50,2016preVFP"):
        xsec = 10.72
    elif(sample == "EWKZ2Jets_ZToNuNu_M-50,2016postVFP"):
        xsec = 10.72
    elif(sample == "EWKZ2Jets_ZToNuNu_M-50,2017"):
        xsec = 10.72
    elif(sample == "EWKZ2Jets_ZToNuNu_M-50,2018"):
        xsec = 10.72
    elif(sample == "EWKZ2Jets_ZToQQ_dipoleRecoilOn,2016preVFP"):
        xsec = 10.67
    elif(sample == "EWKZ2Jets_ZToQQ_dipoleRecoilOn,2016postVFP"):
        xsec = 10.67
    elif(sample == "EWKZ2Jets_ZToQQ_dipoleRecoilOn,2017"):
        xsec = 10.67
    elif(sample == "EWKZ2Jets_ZToQQ_dipoleRecoilOn,2018"):
        xsec = 10.67
    elif(sample == "ST_t-channel_antitop_4f_InclusiveDecays,2016preVFP"):
        xsec = 80.95
    elif(sample == "ST_t-channel_antitop_4f_InclusiveDecays,2016postVFP"):
        xsec = 80.95
    elif(sample == "ST_t-channel_antitop_4f_InclusiveDecays,2017"):
        xsec = 80.95
    elif(sample == "ST_t-channel_antitop_4f_InclusiveDecays,2018"):
        xsec = 80.95
    elif(sample == "ST_t-channel_top_4f_InclusiveDecays,2016preVFP"):
        xsec = 136.02
    elif(sample == "ST_t-channel_top_4f_InclusiveDecays,2016postVFP"):
        xsec = 136.02
    elif(sample == "ST_t-channel_top_4f_InclusiveDecays,2017"):
        xsec = 136.02
    elif(sample == "ST_t-channel_top_4f_InclusiveDecays,2018"):
        xsec = 136.02
    elif(sample == "ST_tW_antitop_5f_inclusiveDecays,2016preVFP"):
        xsec = 19.559
    elif(sample == "ST_tW_antitop_5f_inclusiveDecays,2016postVFP"):
        xsec = 19.559
    elif(sample == "ST_tW_antitop_5f_inclusiveDecays,2017"):
        xsec = 19.559
    elif(sample == "ST_tW_antitop_5f_inclusiveDecays,2018"):
        xsec = 19.559
    elif(sample == "ST_tW_top_5f_inclusiveDecays,2016preVFP"):
        xsec = 19.559
    elif(sample == "ST_tW_top_5f_inclusiveDecays,2016postVFP"):
        xsec = 19.559
    elif(sample == "ST_tW_top_5f_inclusiveDecays,2017"):
        xsec = 19.559
    elif(sample == "ST_tW_top_5f_inclusiveDecays,2018"):
        xsec = 19.559
    elif(sample == "TTTo2L2Nu,2016preVFP"):
        xsec = 88.29
    elif(sample == "TTTo2L2Nu,2016postVFP"):
        xsec = 88.29
    elif(sample == "TTTo2L2Nu,2017"):
        xsec = 88.29
    elif(sample == "TTTo2L2Nu,2018"):
        xsec = 88.29
    elif(sample == "TTToHadronic,2016preVFP"):
        xsec = 377.96
    elif(sample == "TTToHadronic,2016postVFP"):
        xsec = 377.96
    elif(sample == "TTToHadronic,2017"):
        xsec = 377.96
    elif(sample == "TTToHadronic,2018"):
        xsec = 377.96
    elif(sample == "TTToSemiLeptonic,2016preVFP"):
        xsec = 365.34
    elif(sample == "TTToSemiLeptonic,2016postVFP"):
        xsec = 365.34
    elif(sample == "TTToSemiLeptonic,2017"):
        xsec = 365.34
    elif(sample == "TTToSemiLeptonic,2018"):
        xsec = 365.34
    elif(sample == "TTWJetsToLNu,2016preVFP"):
        xsec = 0.2043
    elif(sample == "TTWJetsToLNu,2016postVFP"):
        xsec = 0.2043
    elif(sample == "TTWJetsToLNu,2017"):
        xsec = 0.2043
    elif(sample == "TTWJetsToLNu,2018"):
        xsec = 0.2043
    elif(sample == "TTWW,2016preVFP"):
        xsec = 0.0115
    elif(sample == "TTWW,2016postVFP"):
        xsec = 0.0115
    elif(sample == "TTWW,2017"):
        xsec = 0.0115
    elif(sample == "TTWW,2018"):
        xsec = 0.0115
    elif(sample == "TTWZ,2016preVFP"):
        xsec = 0.003884
    elif(sample == "TTWZ,2016postVFP"):
        xsec = 0.003884
    elif(sample == "TTWZ,2017"):
        xsec = 0.003884
    elif(sample == "TTWZ,2018"):
        xsec = 0.003884
    elif(sample == "TTZToLLNuNu_M-10,2016preVFP"):
        xsec = 0.2529
    elif(sample == "TTZToLLNuNu_M-10,2016postVFP"):
        xsec = 0.2529
    elif(sample == "TTZToLLNuNu_M-10,2017"):
        xsec = 0.2529
    elif(sample == "TTZToLLNuNu_M-10,2018"):
        xsec = 0.2529
    elif(sample == "TTbb_4f_TTTo2L2Nu,2016preVFP"):
        xsec = 0.04
    elif(sample == "TTbb_4f_TTTo2L2Nu,2016postVFP"):
        xsec = 0.04
    elif(sample == "TTbb_4f_TTTo2L2Nu,2017"):
        xsec = 0.04
    elif(sample == "TTbb_4f_TTTo2L2Nu,2018"):
        xsec = 0.04
    elif(sample == "TTbb_4f_TTToSemiLeptonic,2016preVFP"):
        xsec = 0.62
    elif(sample == "TTbb_4f_TTToSemiLeptonic,2016postVFP"):
        xsec = 0.62
    elif(sample == "TTbb_4f_TTToSemiLeptonic,2017"):
        xsec = 0.62
    elif(sample == "TTbb_4f_TTToSemiLeptonic,2018"):
        xsec = 0.62
    elif(sample == "ttHToNonbb_M125,2016preVFP"):
        xsec = 0.215
    elif(sample == "ttHToNonbb_M125,2016postVFP"):
        xsec = 0.215
    elif(sample == "ttHToNonbb_M125,2017"):
        xsec = 0.215
    elif(sample == "ttHToNonbb_M125,2018"):
        xsec = 0.215
    elif(sample == "ttHTobb_M125,2016preVFP"):
        xsec = 0.1279
    elif(sample == "ttHTobb_M125,2016postVFP"):
        xsec = 0.1279
    elif(sample == "ttHTobb_M125,2017"):
        xsec = 0.1279
    elif(sample == "ttHTobb_M125,2018"):
        xsec = 0.1279
    elif(sample == "VBFWH_HToBB_WToLNu_M-125_dipoleRecoilOn,2016preVFP"):
        xsec = 0.02656
    elif(sample == "VBFWH_HToBB_WToLNu_M-125_dipoleRecoilOn,2016postVFP"):
        xsec = 0.02656
    elif(sample == "VBFWH_HToBB_WToLNu_M-125_dipoleRecoilOn,2017"):
        xsec = 0.02656
    elif(sample == "VBFWH_HToBB_WToLNu_M-125_dipoleRecoilOn,2018"):
        xsec = 0.02656
    elif(sample == "VHToNonbb_M125,2016preVFP"):
        xsec = 2.207
    elif(sample == "VHToNonbb_M125,2016postVFP"):
        xsec = 2.207
    elif(sample == "VHToNonbb_M125,2017"):
        xsec = 2.207
    elif(sample == "VHToNonbb_M125,2018"):
        xsec = 2.207
    elif(sample == "WJetsToLNu_HT-70To100,2016preVFP"):
        xsec = 1310.78
    elif(sample == "WJetsToLNu_HT-70To100,2016postVFP"):
        xsec = 1283.91
    elif(sample == "WJetsToLNu_HT-70To100,2017"):
        xsec = 1319.76
    elif(sample == "WJetsToLNu_HT-70To100,2018"):
        xsec = 1321.16
    elif(sample == "WJetsToLNu_HT-100To200,2016preVFP"):
        xsec = 1325.9
    elif(sample == "WJetsToLNu_HT-100To200,2016postVFP"):
        xsec = 1303.06
    elif(sample == "WJetsToLNu_HT-100To200,2017"):
        xsec = 1334.74
    elif(sample == "WJetsToLNu_HT-100To200,2018"):
        xsec = 1335.7
    elif(sample == "WJetsToLNu_HT-200To400,2016preVFP"):
        xsec = 348.57030
    elif(sample == "WJetsToLNu_HT-200To400,2016postVFP"):
        xsec = 341.046
    elif(sample == "WJetsToLNu_HT-200To400,2017"):
        xsec = 350.435
    elif(sample == "WJetsToLNu_HT-200To400,2018"):
        xsec = 351.689
    elif(sample == "WJetsToLNu_HT-400To600,2016preVFP"):
        xsec = 47.308275
    elif(sample == "WJetsToLNu_HT-400To600,2016postVFP"):
        xsec = 45.4362
    elif(sample == "WJetsToLNu_HT-400To600,2017"):
        xsec = 46.5726
    elif(sample == "WJetsToLNu_HT-400To600,2018"):
        xsec = 47.1663
    elif(sample == "WJetsToLNu_HT-600To800,2016preVFP"):
        xsec = 11.358487
    elif(sample == "WJetsToLNu_HT-600To800,2016postVFP"):
        xsec = 11.0051
    elif(sample == "WJetsToLNu_HT-600To800,2017"):
        xsec = 11.1485
    elif(sample == "WJetsToLNu_HT-600To800,2018"):
        xsec = 11.4196
    elif(sample == "WJetsToLNu_HT-800To1200,2016preVFP"):
        xsec = 5.2086934
    elif(sample == "WJetsToLNu_HT-800To1200,2016postVFP"):
        xsec = 4.94177
    elif(sample == "WJetsToLNu_HT-800To1200,2017"):
        xsec = 5.02246
    elif(sample == "WJetsToLNu_HT-800To1200,2018"):
        xsec = 5.12389
    elif(sample == "WJetsToLNu_HT-1200To2500,2016preVFP"):
        xsec = 1.1880809
    elif(sample == "WJetsToLNu_HT-1200To2500,2016postVFP"):
        xsec = 1.15544
    elif(sample == "WJetsToLNu_HT-1200To2500,2017"):
        xsec = 1.183
    elif(sample == "WJetsToLNu_HT-1200To2500,2018"):
        xsec = 1.18295
    elif(sample == "WJetsToLNu_HT-2500ToInf,2016preVFP"):
        xsec = 0.024098031
    elif(sample == "WJetsToLNu_HT-2500ToInf,2016postVFP"):
        xsec = 0.0216234
    elif(sample == "WJetsToLNu_HT-2500ToInf,2017"):
        xsec = 0.0258083
    elif(sample == "WJetsToLNu_HT-2500ToInf,2018"):
        xsec = 0.0255202
    elif(sample == "WWJJToLNuLNu_EWK_noTop,2016preVFP"):
        xsec = 0.284
    elif(sample == "WWJJToLNuLNu_EWK_noTop,2016postVFP"):
        xsec = 0.284
    elif(sample == "WWJJToLNuLNu_EWK_noTop,2017"):
        xsec = 0.284
    elif(sample == "WWJJToLNuLNu_EWK_noTop,2018"):
        xsec = 0.284
    elif(sample == "WWTo1L1Nu2Q_4f,2016preVFP"):
        xsec = 49.997
    elif(sample == "WWTo1L1Nu2Q_4f,2016postVFP"):
        xsec = 49.997
    elif(sample == "WWTo1L1Nu2Q_4f,2017"):
        xsec = 49.997
    elif(sample == "WWTo1L1Nu2Q_4f,2018"):
        xsec = 49.997
    elif(sample == "WWTo2L2Nu,2016preVFP"):
        xsec = 12.178
    elif(sample == "WWTo2L2Nu,2016postVFP"):
        xsec = 12.178
    elif(sample == "WWTo2L2Nu,2017"):
        xsec = 12.178
    elif(sample == "WWTo2L2Nu,2018"):
        xsec = 12.178
    elif(sample == "WWTolnulnu,2016preVFP"):
        xsec = 12.178
    elif(sample == "WWTolnulnu,2016postVFP"):
        xsec = 12.178
    elif(sample == "WWTolnulnu,2017"):
        xsec = 12.178
    elif(sample == "WWTolnulnu,2018"):
        xsec = 12.178
    elif(sample == "WWW_4F,2016preVFP"):
        xsec = 0.2086
    elif(sample == "WWW_4F,2016postVFP"):
        xsec = 0.2086
    elif(sample == "WWW_4F,2017"):
        xsec = 0.2086
    elif(sample == "WWW_4F,2018"):
        xsec = 0.2086
    elif(sample == "WWZ_4F,2016preVFP"):
        xsec = 0.1651
    elif(sample == "WWZ_4F,2016postVFP"):
        xsec = 0.1651
    elif(sample == "WWZ_4F,2017"):
        xsec = 0.1651
    elif(sample == "WWZ_4F,2018"):
        xsec = 0.1651
    elif(sample == "WZJJ_EWK_InclusivePolarization,2016preVFP"):
        xsec = 0.01701
    elif(sample == "WZJJ_EWK_InclusivePolarization,2016postVFP"):
        xsec = 0.01701
    elif(sample == "WZJJ_EWK_InclusivePolarization,2017"):
        xsec = 0.01701
    elif(sample == "WZJJ_EWK_InclusivePolarization,2018"):
        xsec = 0.01701
    elif(sample == "WZTo1L1Nu2Q_4f,2016preVFP"):
        xsec = 49.997
    elif(sample == "WZTo1L1Nu2Q_4f,2016postVFP"):
        xsec = 49.997
    elif(sample == "WZTo1L1Nu2Q_4f,2017"):
        xsec = 49.997
    elif(sample == "WZTo1L1Nu2Q_4f,2018"):
        xsec = 49.997
    elif(sample == "WZTo1L3Nu_4f,2016preVFP"):
        xsec = 3.05402
    elif(sample == "WZTo1L3Nu_4f,2016postVFP"):
        xsec = 3.05402
    elif(sample == "WZTo1L3Nu_4f,2017"):
        xsec = 3.05402
    elif(sample == "WZTo1L3Nu_4f,2018"):
        xsec = 3.05402
    elif(sample == "WZTo2Q2L_mllmin4p0,2016preVFP"):
        xsec = 5.6
    elif(sample == "WZTo2Q2L_mllmin4p0,2016postVFP"):
        xsec = 5.6
    elif(sample == "WZTo2Q2L_mllmin4p0,2017"):
        xsec = 5.6
    elif(sample == "WZTo2Q2L_mllmin4p0,2018"):
        xsec = 5.6
    elif(sample == "WZTo3LNu,2016preVFP"):
        xsec = 4.42965
    elif(sample == "WZTo3LNu,2016postVFP"):
        xsec = 4.42965
    elif(sample == "WZTo3LNu,2017"):
        xsec = 4.42965
    elif(sample == "WZTo3LNu,2018"):
        xsec = 4.42965
    elif(sample == "WZZ,2016preVFP"):
        xsec = 0.05565
    elif(sample == "WZZ,2016postVFP"):
        xsec = 0.05565
    elif(sample == "WZZ,2017"):
        xsec = 0.05565
    elif(sample == "WZZ,2018"):
        xsec = 0.05565
    elif(sample == "WminusH_HToBB_WToLNu_M-125,2016preVFP"):
        xsec = 0.0490124
    elif(sample == "WminusH_HToBB_WToLNu_M-125,2016postVFP"):
        xsec = 0.0490124
    elif(sample == "WminusH_HToBB_WToLNu_M-125,2017"):
        xsec = 0.0490124
    elif(sample == "WminusH_HToBB_WToLNu_M-125,2018"):
        xsec = 0.0490124
    elif(sample == "WplusH_HToBB_WToLNu_M-125,2016preVFP"):
        xsec = 0.084876
    elif(sample == "WplusH_HToBB_WToLNu_M-125,2016postVFP"):
        xsec = 0.084876
    elif(sample == "WplusH_HToBB_WToLNu_M-125,2017"):
        xsec = 0.084876
    elif(sample == "WplusH_HToBB_WToLNu_M-125,2018"):
        xsec = 0.084876
    elif(sample == "ZH_HToBB_ZToLL_M-125,2016preVFP"):
        xsec = 0.0262749
    elif(sample == "ZH_HToBB_ZToLL_M-125,2016postVFP"):
        xsec = 0.0262749
    elif(sample == "ZH_HToBB_ZToLL_M-125,2017"):
        xsec = 0.0262749
    elif(sample == "ZH_HToBB_ZToLL_M-125,2018"):
        xsec = 0.0262749
    elif(sample == "ZZJJTo4L,2016preVFP"):
        xsec = 0.00884
    elif(sample == "ZZJJTo4L,2016postVFP"):
        xsec = 0.00884
    elif(sample == "ZZJJTo4L,2017"):
        xsec = 0.00884
    elif(sample == "ZZJJTo4L,2018"):
        xsec = 0.00884
    elif(sample == "ZZTo2L2Nu,2016preVFP"):
        xsec = 0.564
    elif(sample == "ZZTo2L2Nu,2016postVFP"):
        xsec = 0.564
    elif(sample == "ZZTo2L2Nu,2017"):
        xsec = 0.564
    elif(sample == "ZZTo2L2Nu,2018"):
        xsec = 0.564
    elif(sample == "ZZTo2Q2L_mllmin4p0,2016preVFP"):
        xsec = 3.28
    elif(sample == "ZZTo2Q2L_mllmin4p0,2016postVFP"):
        xsec = 3.28
    elif(sample == "ZZTo2Q2L_mllmin4p0,2017"):
        xsec = 3.28
    elif(sample == "ZZTo2Q2L_mllmin4p0,2018"):
        xsec = 3.28
    elif(sample == "ZZTo4L_M-1toInf,2016preVFP"):
        xsec = 1.256
    elif(sample == "ZZTo4L_M-1toInf,2016postVFP"):
        xsec = 1.256
    elif(sample == "ZZTo4L_M-1toInf,2017"):
        xsec = 1.256
    elif(sample == "ZZTo4L_M-1toInf,2018"):
        xsec = 1.256
    elif(sample == "ZZZ,2016preVFP"):
        xsec = 0.01398
    elif(sample == "ZZZ,2016postVFP"):
        xsec = 0.01398
    elif(sample == "ZZZ,2017"):
        xsec = 0.01398
    elif(sample == "ZZZ,2018"):
        xsec = 0.01398
    elif(sample == "ggZH_HToBB_ZToLL_M-125,2016preVFP"):
        xsec = 0.0024614
    elif(sample == "ggZH_HToBB_ZToLL_M-125,2016postVFP"):
        xsec = 0.0024614
    elif(sample == "ggZH_HToBB_ZToLL_M-125,2017"):
        xsec = 0.0024614
    elif(sample == "ggZH_HToBB_ZToLL_M-125,2018"):
        xsec = 0.0024614
    # sig
    elif(sample == "VBSWWH_SSWW,2016preVFP"):
        xsec = 0.001517
    elif(sample == "VBSWWH_SSWW,2016postVFP"):
        xsec = 0.001517
    elif(sample == "VBSWWH_SSWW,2017"):
        xsec = 0.001517
    elif(sample == "VBSWWH_SSWW,2018"):
        xsec = 0.001517
    elif(sample == "VBSWWH_OSWW,2016preVFP"):
        xsec = 0.002805
    elif(sample == "VBSWWH_OSWW,2016postVFP"):
        xsec = 0.002805
    elif(sample == "VBSWWH_OSWW,2017"):
        xsec = 0.002805
    elif(sample == "VBSWWH_OSWW,2018"):
        xsec = 0.002805
    elif(sample == "VBSWZH,2016preVFP"):
        xsec = 0.001631
    elif(sample == "VBSWZH,2016postVFP"):
        xsec = 0.001631
    elif(sample == "VBSWZH,2017"):
        xsec = 0.001631
    elif(sample == "VBSWZH,2018"):
        xsec = 0.001631
    elif(sample == "VBSZZH,2016preVFP"):
        xsec = 0.001126
    elif(sample == "VBSZZH,2016postVFP"):
        xsec = 0.001126
    elif(sample == "VBSZZH,2017"):
        xsec = 0.001126
    elif(sample == "VBSZZH,2018"):
        xsec = 0.001126
    else:
        print("Error: xsec not found for " + sample)
    return xsec

def lhe_pdf_norms(sample):
    pdf_norm = 0
    if(sample == "VBSZZH,2017"):
        pdf_norm = 1.2980920185943707
    elif(sample == "VBSWWH_SSWW,2017"):
        pdf_norm = 1.2205882825144703
    elif(sample == "VBSWZH,2017"):
        pdf_norm = 1.2566551045188934
    elif(sample == "VBSWWH_OSWW,2017"):
        pdf_norm = 1.2507539374406431
    elif(sample == "VBSZZH,2018"):
        pdf_norm = 1.2985744198332958
    elif(sample == "VBSWWH_SSWW,2018"):
        pdf_norm = 1.2189417248705572
    elif(sample == "VBSWZH,2018"):
        pdf_norm = 1.2543030646199427
    elif(sample == "VBSWWH_OSWW,2018"):
        pdf_norm = 1.2486530419157191
    elif(sample == "VBSZZH,2016postVFP"):
        pdf_norm = 1.297558141585914
    elif(sample == "VBSWWH_SSWW,2016postVFP"):
        pdf_norm = 1.2196977423067406
    elif(sample == "VBSWZH,2016postVFP"):
        pdf_norm = 1.2576042114971981
    elif(sample == "VBSWWH_OSWW,2016postVFP"):
        pdf_norm = 1.2473127695051147
    elif(sample == "VBSZZH,2016preVFP"):
        pdf_norm = 1.2993650469217064
    elif(sample == "VBSWWH_SSWW,2016preVFP"):
        pdf_norm = 1.218872708226821
    elif(sample == "VBSWZH,2016preVFP"):
        pdf_norm = 1.2528598974228045
    elif(sample == "VBSWWH_OSWW,2016preVFP"):
        pdf_norm = 1.2507993077851738
    else:
        print("Error")
    return pdf_norm

def get_lumi(year):
    lumi = {
            "2016preVFP": 19.52,
            "2016postVFP": 16.81,
            "2017": 41.529,
            "2018": 59.7
        }
    return lumi[year]

def extract_mc_sample_type(sample_name):
    if "DY" in sample_name:
        return "DY"
    elif sample_name.startswith("TTTo"):
        return "ttbar"
    elif "TT" in sample_name or "tt" in sample_name:
        return "ttx"
    elif "ST" in sample_name:
        return "ST"
    elif "WJets" in sample_name:
        return "WJets"
    elif "EWK" in sample_name:
        return "EWK"
    else:
        return "Other"

def make_config(args):
    with open(args.output, "w+") as f:
        glob_n = 0
        try:
            config = json.load(f)
        except json.JSONDecodeError:
            config = None
        if config is None:
            config = {"samples": {}}
        # bkg
        if "bkg" in args.categories.split(","):
            sample_list_bkg = sorted(glob("/ceph/cms/store/user/aaarora/VBS_1lep_skims/bkg_1lep_4ak4_or_1ak8_2ak4_v1/*"))
            for sample in sample_list_bkg:
                sample_name = sample.split("/ceph/cms/store/user/aaarora/VBS_1lep_skims/bkg_1lep_4ak4_or_1ak8_2ak4_v1/")[1].split(",")[0].split("_Tune")[0]
                sample_year = extract_sample_year(sample)
                xsec = get_xsec_weight(sample_name + "," + sample_year)
                if xsec == 0:
                    continue
                if args.sample_year is not None and sample_year != args.sample_year:
                    continue
                num_events = 0
                files = glob(sample + "/output*.root")
                for file in files:
                    with uproot.open(file) as upf:
                        num_events += sum(upf.get("Runs")["genEventSumw"].array())
                config["samples"].update(
                    {
                        sample_name + "_" + sample_year: {
                            "trees": ["Events"],
                            "files": [sample + "/output*.root"],
                            "metadata": {
                                "sample_category": "bkg",
                                "sample_year" : sample_year,
                                "sample_type": extract_mc_sample_type(sample_name),
                                "xsec": xsec,
                                "lumi": get_lumi(sample_year),
                                "nevents": num_events
                            }
                        }
                    }
                )
        # data
        if "data" in args.categories.split(","):
            sample_list_data = sorted(glob("/ceph/cms/store/user/aaarora/VBS_1lep_skims/data_1lep_4ak4_or_1ak8_2ak4_v1/*"))
            for sample in sample_list_data:
                sample_name = sample.split("/ceph/cms/store/user/aaarora/VBS_1lep_skims/data_1lep_4ak4_or_1ak8_2ak4_v1/")[1].split(",")[0].split("_Tune")[0]
                sample_year = extract_sample_year(sample)
                if args.sample_year is not None and sample_year != args.sample_year:
                    continue
                data_sample_type = "SingleMuon" if "SingleMuon" in sample_name else "SingleElectron"
                config["samples"].update(
                    {
                        sample_name + "_" + sample_year: {
                            "trees": ["Events"],
                            "files": [sample + "/output*.root"],
                            "metadata": {
                                "sample_category": "data",
                                "sample_year" : sample_year,
                                "sample_type": data_sample_type,
                                "xsec": 1.0,
                                "lumi": 1.0,
                                "nevents": 1.0
                            }
                        }
                    }
                )
        # sig
        if "sig" in args.categories.split(","):
            # sample_list_sig = sorted(glob("/ceph/cms/store/user/aaarora/VBS_1lep_skims/sig_1lep_4ak4_or_1ak8_2ak4_v1/*/*/NANOAODSIM/*/*/skimmed/*.root"))
            sample_list_sig = sorted(glob("/ceph/cms/store/user/jguiang/VBSVHSkim/sig_1lep_4ak4_or_1ak8_2ak4_v1/*Inclusive*/*.root"))
            print(sample_list_sig)
            for sample in sample_list_sig:
                # sample_name = sample.split("/ceph/cms/store/user/aaarora/VBS_1lep_skims/sig_1lep_4ak4_or_1ak8_2ak4_v1/")[1].split(",")[0].split("_TuneCP5")[0]
                sample_name = sample.split("/ceph/cms/store/user/jguiang/VBSVHSkim/sig_1lep_4ak4_or_1ak8_2ak4_v1/")[1].split("_Inclusive")[0]
                if (sample_name == "VBSOSWWH"):
                    sample_name = "VBSWWH_OSWW"
                if (sample_name == "VBSWWH"):
                    sample_name = "VBSWWH_SSWW" 
                sample_year = extract_sample_year(sample)
                if args.sample_year is not None and sample_year != args.sample_year:
                    continue
                # xsec = get_xsec_weight(sample_name.split("/")[1].split("_MJJ")[0] + "," + sample_year)
                xsec = get_xsec_weight(sample_name + "," + sample_year)
                if xsec == 0:
                    print(sample)
                    continue
                num_events = 0
                
                with uproot.open(sample) as upf:
                    num_events += sum(upf.get("Runs")["genEventSumw"].array())

                config["samples"].update(
                    {
                        sample_name + "_" + sample_year: {
                            "trees": ["Events"],
                            "files": [sample],
                            "metadata": {
                                "sample_category": "sig",
                                "sample_year" : sample_year,
                                "sample_type": sample_name,
                                "xsec": xsec,
                                "lumi": get_lumi(sample_year),
                                "nevents": num_events
                            }
                        }
                    }
                )
        json.dump(config, f, indent=4)

if __name__ == "__main__":
    argparser = argparse.ArgumentParser()
    argparser.add_argument("--categories", help="categories", default="bkg,data,sig")
    argparser.add_argument("--sample_year", help="sample year", default=None)
    argparser.add_argument("--output", help="output config file", default="input.json")
    argparser.add_argument("--type", help="type", default=None)
    args = argparser.parse_args()

    config = make_config(args)
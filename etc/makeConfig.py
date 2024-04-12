from glob import glob
import json
import argparse

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

def get_xsec_weight(InputYear):
    weight_xsec = 0
    if(InputYear == "DYJetsToLL_M-10to50,2016preVFP"):
        weight_xsec = 15.6291
    elif(InputYear == "DYJetsToLL_M-10to50,2016postVFP"):
        weight_xsec = 15.5099
    elif(InputYear == "DYJetsToLL_M-10to50,2017"):
        weight_xsec = 12.5272
    elif(InputYear == "DYJetsToLL_M-10to50,2018"):
        weight_xsec = 13.0565
    elif(InputYear == "DYJetsToLL_M-50,2016preVFP"):
        weight_xsec = 1.27124
    elif(InputYear == "DYJetsToLL_M-50,2016postVFP"):
        weight_xsec = 1.26368
    elif(InputYear == "DYJetsToLL_M-50,2017"):
        weight_xsec = 2.5023
    elif(InputYear == "DYJetsToLL_M-50,2018"):
        weight_xsec = 3.84504
    elif(InputYear == "EWKWMinus2Jets_WToLNu_M-50,2016preVFP"):
        weight_xsec = 0.280122
    elif(InputYear == "EWKWMinus2Jets_WToLNu_M-50,2016postVFP"):
        weight_xsec = 0.246272
    elif(InputYear == "EWKWMinus2Jets_WToLNu_M-50,2017"):
        weight_xsec = 0.328606
    elif(InputYear == "EWKWMinus2Jets_WToLNu_M-50,2018"):
        weight_xsec = 0.429989
    elif(InputYear == "EWKWPlus2Jets_WToLNu_M-50,2016preVFP"):
        weight_xsec = 0.35136
    elif(InputYear == "EWKWPlus2Jets_WToLNu_M-50,2016postVFP"):
        weight_xsec = 0.325203
    elif(InputYear == "EWKWPlus2Jets_WToLNu_M-50,2017"):
        weight_xsec = 0.454335
    elif(InputYear == "EWKWPlus2Jets_WToLNu_M-50,2018"):
        weight_xsec = 0.523173
    elif(InputYear == "EWKWminus2Jets_WToQQ_dipoleRecoilOn,2016preVFP"):
        weight_xsec = 0.041664
    elif(InputYear == "EWKWminus2Jets_WToQQ_dipoleRecoilOn,2016postVFP"):
        weight_xsec = 0.035886
    elif(InputYear == "EWKWminus2Jets_WToQQ_dipoleRecoilOn,2017"):
        weight_xsec = 0.0562756
    elif(InputYear == "EWKWminus2Jets_WToQQ_dipoleRecoilOn,2018"):
        weight_xsec = 0.0659761
    elif(InputYear == "EWKWplus2Jets_WToQQ_dipoleRecoilOn,2016preVFP"):
        weight_xsec = 0.042136
    elif(InputYear == "EWKWplus2Jets_WToQQ_dipoleRecoilOn,2016postVFP"):
        weight_xsec = 0.035886
    elif(InputYear == "EWKWplus2Jets_WToQQ_dipoleRecoilOn,2017"):
        weight_xsec = 0.0554309
    elif(InputYear == "EWKWplus2Jets_WToQQ_dipoleRecoilOn,2018"):
        weight_xsec = 0.0637573
    elif(InputYear == "EWKZ2Jets_ZToLL_M-50,2016preVFP"):
        weight_xsec = 0.242829
    elif(InputYear == "EWKZ2Jets_ZToLL_M-50,2016postVFP"):
        weight_xsec = 0.230813
    elif(InputYear == "EWKZ2Jets_ZToLL_M-50,2017"):
        weight_xsec = 0.446132
    elif(InputYear == "EWKZ2Jets_ZToLL_M-50,2018"):
        weight_xsec = 0.371334
    elif(InputYear == "EWKZ2Jets_ZToNuNu_M-50,2016preVFP"):
        weight_xsec = 0.144313
    elif(InputYear == "EWKZ2Jets_ZToNuNu_M-50,2016postVFP"):
        weight_xsec = 0.120135
    elif(InputYear == "EWKZ2Jets_ZToNuNu_M-50,2017"):
        weight_xsec = 0.149594
    elif(InputYear == "EWKZ2Jets_ZToNuNu_M-50,2018"):
        weight_xsec = 0.215555
    elif(InputYear == "EWKZ2Jets_ZToQQ_dipoleRecoilOn,2016preVFP"):
        weight_xsec = 0.0418314
    elif(InputYear == "EWKZ2Jets_ZToQQ_dipoleRecoilOn,2016postVFP"):
        weight_xsec = 0.036396
    elif(InputYear == "EWKZ2Jets_ZToQQ_dipoleRecoilOn,2017"):
        weight_xsec = 0.0553893
    elif(InputYear == "EWKZ2Jets_ZToQQ_dipoleRecoilOn,2018"):
        weight_xsec = 0.0637956
    elif(InputYear == "TTTo2L2Nu,2016preVFP"):
        weight_xsec = 0.000637235
    elif(InputYear == "TTTo2L2Nu,2016postVFP"):
        weight_xsec = 0.000472642
    elif(InputYear == "TTTo2L2Nu,2017"):
        weight_xsec = 0.000476439
    elif(InputYear == "TTTo2L2Nu,2018"):
        weight_xsec = 0.000508988
    elif(InputYear == "TTToHadronic,2016preVFP"):
        weight_xsec = 0.000241649
    elif(InputYear == "TTToHadronic,2016postVFP"):
        weight_xsec = 0.000189043
    elif(InputYear == "TTToHadronic,2017"):
        weight_xsec = 0.000214604
    elif(InputYear == "TTToHadronic,2018"):
        weight_xsec = 0.000216954
    elif(InputYear == "TTToSemiLeptonic,2016preVFP"):
        weight_xsec = 0.000179307
    elif(InputYear == "TTToSemiLeptonic,2016postVFP"):
        weight_xsec = 0.000141024
    elif(InputYear == "TTToSemiLeptonic,2017"):
        weight_xsec = 0.000145705
    elif(InputYear == "TTToSemiLeptonic,2018"):
        weight_xsec = 0.00015504
    elif(InputYear == "TTWJetsToLNu,2016preVFP"):
        weight_xsec = 0.0041804
    elif(InputYear == "TTWJetsToLNu,2016postVFP"):
        weight_xsec = 0.00308519
    elif(InputYear == "TTWJetsToLNu,2017"):
        weight_xsec = 0.00354575
    elif(InputYear == "TTWJetsToLNu,2018"):
        weight_xsec = 0.00348217
    elif(InputYear == "TTWW,2016preVFP"):
        weight_xsec = 0.000807482
    elif(InputYear == "TTWW,2016postVFP"):
        weight_xsec = 0.000625615
    elif(InputYear == "TTWW,2017"):
        weight_xsec = 0.000684217
    elif(InputYear == "TTWW,2018"):
        weight_xsec = 0.000727278
    elif(InputYear == "TTWZ,2016preVFP"):
        weight_xsec = 0.000541541
    elif(InputYear == "TTWZ,2016postVFP"):
        weight_xsec = 0.000410629
    elif(InputYear == "TTWZ,2017"):
        weight_xsec = 0.000460853
    elif(InputYear == "TTWZ,2018"):
        weight_xsec = 0.000465612
    elif(InputYear == "TTZToLLNuNu_M-10,2016preVFP"):
        weight_xsec = 0.00348729
    elif(InputYear == "TTZToLLNuNu_M-10,2016postVFP"):
        weight_xsec = 0.00289547
    elif(InputYear == "TTZToLLNuNu_M-10,2017"):
        weight_xsec = 0.0030665
    elif(InputYear == "TTZToLLNuNu_M-10,2018"):
        weight_xsec = 0.00315665
    elif(InputYear == "TTbb_4f_TTTo2L2Nu,2016preVFP"):
        weight_xsec = 0.00012262
    elif(InputYear == "TTbb_4f_TTTo2L2Nu,2016postVFP"):
        weight_xsec = 9.32674e-05
    elif(InputYear == "TTbb_4f_TTTo2L2Nu,2017"):
        weight_xsec = 0.000104273
    elif(InputYear == "TTbb_4f_TTTo2L2Nu,2018"):
        weight_xsec = 0.000108612
    elif(InputYear == "TTbb_4f_TTToSemiLeptonic,2016preVFP"):
        weight_xsec = 0.000211376
    elif(InputYear == "TTbb_4f_TTToSemiLeptonic,2016postVFP"):
        weight_xsec = 0.000168898
    elif(InputYear == "TTbb_4f_TTToSemiLeptonic,2017"):
        weight_xsec = 0.000167415
    elif(InputYear == "TTbb_4f_TTToSemiLeptonic,2018"):
        weight_xsec = 0.000158203
    elif(InputYear == "VBFWH_HToBB_WToLNu_M-125_dipoleRecoilOn,2016preVFP"):
        weight_xsec = 0.000193669
    elif(InputYear == "VBFWH_HToBB_WToLNu_M-125_dipoleRecoilOn,2016postVFP"):
        weight_xsec = 0.000194278
    elif(InputYear == "VBFWH_HToBB_WToLNu_M-125_dipoleRecoilOn,2017"):
        weight_xsec = 0.000221399
    elif(InputYear == "VBFWH_HToBB_WToLNu_M-125_dipoleRecoilOn,2018"):
        weight_xsec = 0.000318081
    elif(InputYear == "VHToNonbb_M125,2016preVFP"):
        weight_xsec = 0.01682
    elif(InputYear == "VHToNonbb_M125,2016postVFP"):
        weight_xsec = 0.0193468
    elif(InputYear == "VHToNonbb_M125,2017"):
        weight_xsec = 0.018091
    elif(InputYear == "VHToNonbb_M125,2018"):
        weight_xsec = 0.0187589
    elif(InputYear == "WJetsToLNu_HT-100To200,2016preVFP"):
        weight_xsec = 1.1908
    elif(InputYear == "WJetsToLNu_HT-100To200,2016postVFP"):
        weight_xsec = 1.10886
    elif(InputYear == "WJetsToLNu_HT-100To200,2017"):
        weight_xsec = 1.16881
    elif(InputYear == "WJetsToLNu_HT-100To200,2018"):
        weight_xsec = 1.55112
    elif(InputYear == "WJetsToLNu_HT-1200To2500,2016preVFP"):
        weight_xsec = 0.0109394
    elif(InputYear == "WJetsToLNu_HT-1200To2500,2016postVFP"):
        weight_xsec = 0.00929078
    elif(InputYear == "WJetsToLNu_HT-1200To2500,2017"):
        weight_xsec = 0.0103383
    elif(InputYear == "WJetsToLNu_HT-1200To2500,2018"):
        weight_xsec = 0.0108959
    elif(InputYear == "WJetsToLNu_HT-200To400,2016preVFP"):
        weight_xsec = 0.380737
    elif(InputYear == "WJetsToLNu_HT-200To400,2016postVFP"):
        weight_xsec = 0.380484
    elif(InputYear == "WJetsToLNu_HT-200To400,2017"):
        weight_xsec = 0.344194
    elif(InputYear == "WJetsToLNu_HT-200To400,2018"):
        weight_xsec = 0.360594
    elif(InputYear == "WJetsToLNu_HT-2500ToInf,2016preVFP"):
        weight_xsec = 0.000581703
    elif(InputYear == "WJetsToLNu_HT-2500ToInf,2016postVFP"):
        weight_xsec = 0.000512308
    elif(InputYear == "WJetsToLNu_HT-2500ToInf,2017"):
        weight_xsec = 0.000903933
    elif(InputYear == "WJetsToLNu_HT-2500ToInf,2018"):
        weight_xsec = 0.000726316
    elif(InputYear == "WJetsToLNu_HT-400To600,2016preVFP"):
        weight_xsec = 0.374249
    elif(InputYear == "WJetsToLNu_HT-400To600,2016postVFP"):
        weight_xsec = 0.36104
    elif(InputYear == "WJetsToLNu_HT-400To600,2017"):
        weight_xsec = 0.353684
    elif(InputYear == "WJetsToLNu_HT-400To600,2018"):
        weight_xsec = 0.378267
    elif(InputYear == "WJetsToLNu_HT-600To800,2016preVFP"):
        weight_xsec = 0.0945842
    elif(InputYear == "WJetsToLNu_HT-600To800,2016postVFP"):
        weight_xsec = 0.0821543
    elif(InputYear == "WJetsToLNu_HT-600To800,2017"):
        weight_xsec = 0.0834916
    elif(InputYear == "WJetsToLNu_HT-600To800,2018"):
        weight_xsec = 0.0883237
    elif(InputYear == "WJetsToLNu_HT-70To100,2016preVFP"):
        weight_xsec = 1.51115
    elif(InputYear == "WJetsToLNu_HT-70To100,2016postVFP"):
        weight_xsec = 1.11022
    elif(InputYear == "WJetsToLNu_HT-70To100,2017"):
        weight_xsec = 1.22953
    elif(InputYear == "WJetsToLNu_HT-70To100,2018"):
        weight_xsec = 1.19107
    elif(InputYear == "WJetsToLNu_HT-800To1200,2016preVFP"):
        weight_xsec = 0.0404996
    elif(InputYear == "WJetsToLNu_HT-800To1200,2016postVFP"):
        weight_xsec = 0.0389598
    elif(InputYear == "WJetsToLNu_HT-800To1200,2017"):
        weight_xsec = 0.0409902
    elif(InputYear == "WJetsToLNu_HT-800To1200,2018"):
        weight_xsec = 0.0418681
    elif(InputYear == "WWJJToLNuLNu_EWK_noTop,2016preVFP"):
        weight_xsec = 0.0110874
    elif(InputYear == "WWJJToLNuLNu_EWK_noTop,2016postVFP"):
        weight_xsec = 0.00954808
    elif(InputYear == "WWJJToLNuLNu_EWK_noTop,2017"):
        weight_xsec = 0.0117942
    elif(InputYear == "WWJJToLNuLNu_EWK_noTop,2018"):
        weight_xsec = 0.0084774
    elif(InputYear == "WWTo1L1Nu2Q_4f,2016preVFP"):
        weight_xsec = 0.000577134
    elif(InputYear == "WWTo1L1Nu2Q_4f,2016postVFP"):
        weight_xsec = 0.000499039
    elif(InputYear == "WWTo1L1Nu2Q_4f,2017"):
        weight_xsec = 0.000617156
    elif(InputYear == "WWTo1L1Nu2Q_4f,2018"):
        weight_xsec = 0.000879532
    elif(InputYear == "WWTo2L2Nu,2016preVFP"):
        weight_xsec = 0.00710518
    elif(InputYear == "WWTo2L2Nu,2016postVFP"):
        weight_xsec = 0.00636799
    elif(InputYear == "WWTo2L2Nu,2017"):
        weight_xsec = 0.00642716
    elif(InputYear == "WWTo2L2Nu,2018"):
        weight_xsec = 0.00656189
    elif(InputYear == "WWTolnulnu,2016preVFP"):
        weight_xsec = 10.0838
    elif(InputYear == "WWTolnulnu,2016postVFP"):
        weight_xsec = 8.68625
    elif(InputYear == "WWTolnulnu,2017"):
        weight_xsec = 33.0717
    elif(InputYear == "WWTolnulnu,2018"):
        weight_xsec = 45.852
    elif(InputYear == "WWW_4F,2016preVFP"):
        weight_xsec = 0.00363464
    elif(InputYear == "WWW_4F,2016postVFP"):
        weight_xsec = 0.00390494
    elif(InputYear == "WWW_4F,2017"):
        weight_xsec = 0.00407266
    elif(InputYear == "WWW_4F,2018"):
        weight_xsec = 0.00583093
    elif(InputYear == "WWZ_4F,2016preVFP"):
        weight_xsec = 0.00372044
    elif(InputYear == "WWZ_4F,2016postVFP"):
        weight_xsec = 0.00353674
    elif(InputYear == "WWZ_4F,2017"):
        weight_xsec = 0.225642
    elif(InputYear == "WWZ_4F,2018"):
        weight_xsec = 0.0057934
    elif(InputYear == "WZJJ_EWK_InclusivePolarization,2016preVFP"):
        weight_xsec = 0.0107987
    elif(InputYear == "WZJJ_EWK_InclusivePolarization,2016postVFP"):
        weight_xsec = 0.0100226
    elif(InputYear == "WZJJ_EWK_InclusivePolarization,2017"):
        weight_xsec = 0.0226449
    elif(InputYear == "WZJJ_EWK_InclusivePolarization,2018"):
        weight_xsec = 0.0347331
    elif(InputYear == "WZTo1L1Nu2Q_4f,2016preVFP"):
        weight_xsec = 0.0174781
    elif(InputYear == "WZTo1L1Nu2Q_4f,2016postVFP"):
        weight_xsec = 0.0150804
    elif(InputYear == "WZTo1L1Nu2Q_4f,2017"):
        weight_xsec = 0.0187296
    elif(InputYear == "WZTo1L1Nu2Q_4f,2018"):
        weight_xsec = 0.0267372
    elif(InputYear == "WZTo1L3Nu_4f,2016preVFP"):
        weight_xsec = 0.00855474
    elif(InputYear == "WZTo1L3Nu_4f,2016postVFP"):
        weight_xsec = 0.00741616
    elif(InputYear == "WZTo1L3Nu_4f,2017"):
        weight_xsec = 0.00907468
    elif(InputYear == "WZTo1L3Nu_4f,2018"):
        weight_xsec = 0.0129612
    elif(InputYear == "WZTo2Q2L_mllmin4p0,2016preVFP"):
        weight_xsec = 0.000726511
    elif(InputYear == "WZTo2Q2L_mllmin4p0,2016postVFP"):
        weight_xsec = 0.000725481
    elif(InputYear == "WZTo2Q2L_mllmin4p0,2017"):
        weight_xsec = 0.000833521
    elif(InputYear == "WZTo2Q2L_mllmin4p0,2018"):
        weight_xsec = 0.0012195
    elif(InputYear == "WZTo3LNu,2016preVFP"):
        weight_xsec = 0.00106073
    elif(InputYear == "WZTo3LNu,2016postVFP"):
        weight_xsec = 0.000842635
    elif(InputYear == "WZTo3LNu,2017"):
        weight_xsec = 0.00210097
    elif(InputYear == "WZTo3LNu,2018"):
        weight_xsec = 0.00318055
    elif(InputYear == "WZZ,2016preVFP"):
        weight_xsec = 0.00352214
    elif(InputYear == "WZZ,2016postVFP"):
        weight_xsec = 0.00359348
    elif(InputYear == "WZZ,2017"):
        weight_xsec = 0.00408461
    elif(InputYear == "WZZ,2018"):
        weight_xsec = 0.00581363
    elif(InputYear == "WminusH_HToBB_WToLNu_M-125,2016preVFP"):
        weight_xsec = 0.00209656
    elif(InputYear == "WminusH_HToBB_WToLNu_M-125,2016postVFP"):
        weight_xsec = 0.00207706
    elif(InputYear == "WminusH_HToBB_WToLNu_M-125,2017"):
        weight_xsec = 0.00242768
    elif(InputYear == "WminusH_HToBB_WToLNu_M-125,2018"):
        weight_xsec = 0.00339087
    elif(InputYear == "WplusH_HToBB_WToLNu_M-125,2016preVFP"):
        weight_xsec = 0.00222174
    elif(InputYear == "WplusH_HToBB_WToLNu_M-125,2016postVFP"):
        weight_xsec = 0.00226259
    elif(InputYear == "WplusH_HToBB_WToLNu_M-125,2017"):
        weight_xsec = 0.00255594
    elif(InputYear == "WplusH_HToBB_WToLNu_M-125,2018"):
        weight_xsec = 0.0036796
    elif(InputYear == "ZH_HToBB_ZToLL_M-125,2016preVFP"):
        weight_xsec = 0.00241384
    elif(InputYear == "ZH_HToBB_ZToLL_M-125,2016postVFP"):
        weight_xsec = 0.00250719
    elif(InputYear == "ZH_HToBB_ZToLL_M-125,2017"):
        weight_xsec = 0.00296544
    elif(InputYear == "ZH_HToBB_ZToLL_M-125,2018"):
        weight_xsec = 0.00402611
    elif(InputYear == "ZZJJTo4L,2016preVFP"):
        weight_xsec = 8.63572e-05
    elif(InputYear == "ZZJJTo4L,2016postVFP"):
        weight_xsec = 7.46111e-05
    elif(InputYear == "ZZJJTo4L,2017"):
        weight_xsec = 9.1802e-05
    elif(InputYear == "ZZJJTo4L,2018"):
        weight_xsec = 0.000131937
    elif(InputYear == "ZZTo2L2Nu,2016preVFP"):
        weight_xsec = 0.000670512
    elif(InputYear == "ZZTo2L2Nu,2016postVFP"):
        weight_xsec = 0.000611274
    elif(InputYear == "ZZTo2L2Nu,2017"):
        weight_xsec = 0.000588983
    elif(InputYear == "ZZTo2L2Nu,2018"):
        weight_xsec = 0.000607852
    elif(InputYear == "ZZTo2Q2L_mllmin4p0,2016preVFP"):
        weight_xsec = 0.000276734
    elif(InputYear == "ZZTo2Q2L_mllmin4p0,2016postVFP"):
        weight_xsec = 0.000278632
    elif(InputYear == "ZZTo2Q2L_mllmin4p0,2017"):
        weight_xsec = 0.000320208
    elif(InputYear == "ZZTo2Q2L_mllmin4p0,2018"):
        weight_xsec = 0.000463075
    elif(InputYear == "ZZTo4L_M-1toInf,2016preVFP"):
        weight_xsec = 9.02688e-05
    elif(InputYear == "ZZTo4L_M-1toInf,2016postVFP"):
        weight_xsec = 8.40114e-05
    elif(InputYear == "ZZTo4L_M-1toInf,2017"):
        weight_xsec = 9.03515e-05
    elif(InputYear == "ZZTo4L_M-1toInf,2018"):
        weight_xsec = 8.44991e-05
    elif(InputYear == "ZZZ,2016preVFP"):
        weight_xsec = 0.00348442
    elif(InputYear == "ZZZ,2016postVFP"):
        weight_xsec = 0.00351073
    elif(InputYear == "ZZZ,2017"):
        weight_xsec = 0.00412841
    elif(InputYear == "ZZZ,2018"):
        weight_xsec = 0.0057154
    elif(InputYear == "ggZH_HToBB_ZToLL_M-125,2016preVFP"):
        weight_xsec = 0.0032155
    elif(InputYear == "ggZH_HToBB_ZToLL_M-125,2016postVFP"):
        weight_xsec = 0.00305625
    elif(InputYear == "ggZH_HToBB_ZToLL_M-125,2017"):
        weight_xsec = 0.00353689
    elif(InputYear == "ggZH_HToBB_ZToLL_M-125,2018"):
        weight_xsec = 0.00485784
    elif(InputYear == "ttHToNonbb_M125,2016preVFP"):
        weight_xsec = 0.00423661
    elif(InputYear == "ttHToNonbb_M125,2016postVFP"):
        weight_xsec = 0.00321883
    elif(InputYear == "ttHToNonbb_M125,2017"):
        weight_xsec = 0.00351483
    elif(InputYear == "ttHToNonbb_M125,2018"):
        weight_xsec = 0.00349592
    elif(InputYear == "ttHTobb_M125,2016preVFP"):
        weight_xsec = 0.00107828
    elif(InputYear == "ttHTobb_M125,2016postVFP"):
        weight_xsec = 0.000869231
    elif(InputYear == "ttHTobb_M125,2017"):
        weight_xsec = 0.00135507
    elif(InputYear == "ttHTobb_M125,2018"):
        weight_xsec = 0.00157649
    # sig
    elif(InputYear == "VBSOSWWH,2016postVFP"):
        weight_xsec = 0.000475387
    elif(InputYear == "VBSOSWWH,2016preVFP"):
        weight_xsec = 0.000552026
    elif(InputYear == "VBSOSWWH,2017"):
        weight_xsec = 0.00117444
    elif(InputYear == "VBSOSWWH,2018"):
        weight_xsec = 0.00168832
    elif(InputYear == "VBSWWH_SS,2016postVFP"):
        weight_xsec = 0.000256521
    elif(InputYear == "VBSWWH_SS,2016preVFP"):
        weight_xsec = 0.000297875
    elif(InputYear == "VBSWWH_SS,2017"):
        weight_xsec = 0.000633733
    elif(InputYear == "VBSWWH_SS,2018"):
        weight_xsec = 0.000911022
    elif(InputYear == "VBSWZH,2016postVFP"):
        weight_xsec = 0.00027518
    elif(InputYear == "VBSWZH,2016preVFP"):
        weight_xsec = 0.000319542
    elif(InputYear == "VBSWZH,2017"):
        weight_xsec = 0.00067983
    elif(InputYear == "VBSWZH,2018"):
        weight_xsec = 0.000977289
    elif(InputYear == "VBSZZH,2016postVFP"):
        weight_xsec = 0.000190289
    elif(InputYear == "VBSZZH,2016preVFP"):
        weight_xsec = 0.000220966
    elif(InputYear == "VBSZZH,2017"):
        weight_xsec = 0.000470108
    elif(InputYear == "VBSZZH,2018"):
        weight_xsec = 0.000675804
    else:
        print("Error: xsec not found for " + InputYear)
    return weight_xsec

def make_config(args):
    if args.sample_year is None:
        config_name = "input.json"
    else:
        config_name = f"config_{args.sample_year}.json"
    with open(config_name, "w+") as f:
        try:
            config = json.load(f)
        except json.JSONDecodeError:
            config = None
        if config is None:
            config = {"samples": {}}
        # bkg
        if "bkg" in args.categories.split(","):
            sample_list_bkg = sorted(glob("/data/userdata/aaarora/VBS_1lep_skims/bkg_1lep_4ak4_or_1ak8_2ak4_v1/*"))
            for sample in sample_list_bkg:
                sample_name = sample.split("/data/userdata/aaarora/VBS_1lep_skims/bkg_1lep_4ak4_or_1ak8_2ak4_v1/")[1].split(",")[0].split("_Tune")[0]
                sample_year = extract_sample_year(sample)
                xsec = get_xsec_weight(sample_name + "," + sample_year)
                if xsec == 0:
                    continue
                if args.sample_year is not None and sample_year != args.sample_year:
                    continue
                config["samples"].update(
                    {
                        sample_name + "_" + sample_year: {
                            "trees": ["Events"],
                            "files": [sample + "/output*.root"],
                            "metadata": {
                                "sample_category": "bkg",
                                "sample_year" : sample_year,
                                "xsec_weight": xsec
                            }
                        }
                    }
                )
        # data
        if "data" in args.categories.split(","):
            sample_list_data = sorted(glob("/data/userdata/aaarora/VBS_1lep_skims/data_1lep_4ak4_or_1ak8_2ak4_v1/*"))
            for sample in sample_list_data:
                sample_name = sample.split("/data/userdata/aaarora/VBS_1lep_skims/data_1lep_4ak4_or_1ak8_2ak4_v1/")[1].split(",")[0].split("_Tune")[0]
                sample_year = extract_sample_year(sample)
                if args.sample_year is not None and sample_year != args.sample_year:
                    continue
                config["samples"].update(
                    {
                        sample_name + "_" + sample_year: {
                            "trees": ["Events"],
                            "files": [sample + "/output*.root"],
                            "metadata": {
                                "sample_category": "data",
                                "sample_year" : sample_year,
                                "xsec_weight": 1.0
                            }
                        }
                    }
                )
        # sig
        if "sig" in args.categories.split(","):
            sample_list_sig = sorted(glob("/data/userdata/aaarora/VBS_1lep_skims/sig_1lep_4ak4_or_1ak8_2ak4_v1/*"))
            for sample in sample_list_sig:
                sample_name = sample.split("/data/userdata/aaarora/VBS_1lep_skims/sig_1lep_4ak4_or_1ak8_2ak4_v1/")[1].split(",")[0].split("_TuneCP5")[0]
                sample_year = extract_sample_year(sample)
                if args.sample_year is not None and sample_year != args.sample_year:
                    continue
                xsec = get_xsec_weight(sample_name + "," + sample_year)
                if xsec == 0:
                    print(sample)
                    continue
                config["samples"].update(
                    {
                        sample_name + "_" + sample_year: {
                            "trees": ["Events"],
                            "files": [sample + "/output*.root"],
                            "metadata": {
                                "sample_category": "sig",
                                "sample_year" : sample_year,
                                "xsec_weight": xsec
                            }
                        }
                    }
                )
        json.dump(config, f, indent=4)

if __name__ == "__main__":
    argparser = argparse.ArgumentParser()
    argparser.add_argument("--categories", help="categories", default="bkg,data,sig")
    argparser.add_argument("--sample_year", help="sample year", default=None)
    args = argparser.parse_args()

    config = make_config(args)
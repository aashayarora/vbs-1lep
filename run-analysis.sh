#!/bin/bash

function runPreselection() {
    c2v=$1
    # if c2v is -1 path prefix is sig/default, otherise sig/$c2v
    if [ $c2v -eq -1 ]; then
        path_prefix=sig/default
    else
        path_prefix=sig/$c2v
    fi

    rm -f $base_dir/$path_prefix/sig*.root
    cd ./preselection/

    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v -o $path_prefix/sig.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --met --var up -o $path_prefix/sig_met_unclustered_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --met --var down -o $path_prefix/sig_met_unclustered_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_Absolute --var up -o $path_prefix/sig_jec_absolute_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_Absolute --var down -o $path_prefix/sig_jec_absolute_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_Absolute_2016preVFP --var up -o $path_prefix/sig_jec_absolute_2016preVFP_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_Absolute_2016preVFP --var down -o $path_prefix/sig_jec_absolute_2016preVFP_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_Absolute_2016postVFP --var up -o $path_prefix/sig_jec_absolute_2016postVFP_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_Absolute_2016postVFP --var down -o $path_prefix/sig_jec_absolute_2016postVFP_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_Absolute_2017 --var up -o $path_prefix/sig_jec_absolute_2017_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_Absolute_2017 --var down -o $path_prefix/sig_jec_absolute_2017_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_Absolute_2018 --var up -o $path_prefix/sig_jec_absolute_2018_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_Absolute_2018 --var down -o $path_prefix/sig_jec_absolute_2018_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_BBEC1 --var up -o $path_prefix/sig_jec_bbec1_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_BBEC1 --var down -o $path_prefix/sig_jec_bbec1_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_BBEC1_2016preVFP --var up -o $path_prefix/sig_jec_bbec1_2016preVFP_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_BBEC1_2016preVFP --var down -o $path_prefix/sig_jec_bbec1_2016preVFP_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_BBEC1_2016postVFP --var up -o $path_prefix/sig_jec_bbec1_2016postVFP_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_BBEC1_2016postVFP --var down -o $path_prefix/sig_jec_bbec1_2016postVFP_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_BBEC1_2017 --var up -o $path_prefix/sig_jec_bbec1_2017_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_BBEC1_2017 --var down -o $path_prefix/sig_jec_bbec1_2017_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_BBEC1_2018 --var up -o $path_prefix/sig_jec_bbec1_2018_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_BBEC1_2018 --var down -o $path_prefix/sig_jec_bbec1_2018_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_EC2 --var up -o $path_prefix/sig_jec_ec2_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_EC2 --var down -o $path_prefix/sig_jec_ec2_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_EC2_2016preVFP --var up -o $path_prefix/sig_jec_ec2_2016preVFP_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_EC2_2016preVFP --var down -o $path_prefix/sig_jec_ec2_2016preVFP_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_EC2_2016postVFP --var up -o $path_prefix/sig_jec_ec2_2016postVFP_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_EC2_2016postVFP --var down -o $path_prefix/sig_jec_ec2_2016postVFP_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_EC2_2017 --var up -o $path_prefix/sig_jec_ec2_2017_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_EC2_2017 --var down -o $path_prefix/sig_jec_ec2_2017_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_EC2_2018 --var up -o $path_prefix/sig_jec_ec2_2018_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_EC2_2018 --var down -o $path_prefix/sig_jec_ec2_2018_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_FlavorQCD --var up -o $path_prefix/sig_jec_flavorqcd_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_FlavorQCD --var down -o $path_prefix/sig_jec_flavorqcd_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_HF --var up -o $path_prefix/sig_jec_hf_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_HF --var down -o $path_prefix/sig_jec_hf_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_HF_2016preVFP --var up -o $path_prefix/sig_jec_hf_2016preVFP_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_HF_2016preVFP --var down -o $path_prefix/sig_jec_hf_2016preVFP_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_HF_2016postVFP --var up -o $path_prefix/sig_jec_hf_2016postVFP_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_HF_2016postVFP --var down -o $path_prefix/sig_jec_hf_2016postVFP_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_HF_2017 --var up -o $path_prefix/sig_jec_hf_2017_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_HF_2017 --var down -o $path_prefix/sig_jec_hf_2017_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_HF_2018 --var up -o $path_prefix/sig_jec_hf_2018_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_HF_2018 --var down -o $path_prefix/sig_jec_hf_2018_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_RelativeBal --var up -o $path_prefix/sig_jec_relativebal_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_RelativeBal --var down -o $path_prefix/sig_jec_relativebal_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_RelativeSample_2016preVFP --var up -o $path_prefix/sig_jec_relativesample_2016preVFP_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_RelativeSample_2016preVFP --var down -o $path_prefix/sig_jec_relativesample_2016preVFP_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_RelativeSample_2016postVFP --var up -o $path_prefix/sig_jec_relativesample_2016postVFP_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_RelativeSample_2016postVFP --var down -o $path_prefix/sig_jec_relativesample_2016postVFP_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_RelativeSample_2017 --var up -o $path_prefix/sig_jec_relativesample_2017_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_RelativeSample_2017 --var down -o $path_prefix/sig_jec_relativesample_2017_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_RelativeSample_2018 --var up -o $path_prefix/sig_jec_relativesample_2018_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type Regrouped_RelativeSample_2018 --var down -o $path_prefix/sig_jec_relativesample_2018_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type RelativeSample --var up -o $path_prefix/sig_jec_relative_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jec --jec_type RelativeSample --var down -o $path_prefix/sig_jec_relative_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jer --var up -o $path_prefix/sig_jer_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jer --var down -o $path_prefix/sig_jer_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jms --var up -o $path_prefix/sig_jms_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jms --var down -o $path_prefix/sig_jms_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jmr --var up -o $path_prefix/sig_jmr_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --jmr --var down -o $path_prefix/sig_jmr_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar pileup_weight -o $path_prefix/sig_pileup_weight_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar pileup_weight -o $path_prefix/sig_pileup_weight_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar pileupid_weight -o $path_prefix/sig_pileupid_weight_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar pileupid_weight -o $path_prefix/sig_pileupid_weight_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar L1PreFiringWeight -o $path_prefix/sig_L1PreFiringWeight_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar L1PreFiringWeight -o $path_prefix/sig_L1PreFiringWeight_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar muon_scale_factors_ID -o $path_prefix/sig_muon_scale_factors_ID_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar muon_scale_factors_ID -o $path_prefix/sig_muon_scale_factors_ID_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar muon_scale_factors_trigger -o $path_prefix/sig_muon_scale_factors_trigger_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar muon_scale_factors_trigger -o $path_prefix/sig_muon_scale_factors_trigger_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar muon_scale_factors_ttHID -o $path_prefix/sig_muon_scale_factors_ttHID_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar muon_scale_factors_ttHID -o $path_prefix/sig_muon_scale_factors_ttHID_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar muon_scale_factors_ttHISO -o $path_prefix/sig_muon_scale_factors_ttHISO_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar muon_scale_factors_ttHISO -o $path_prefix/sig_muon_scale_factors_ttHISO_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar electron_scale_factors_Reco -o $path_prefix/sig_electron_scale_factors_Reco_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar electron_scale_factors_Reco -o $path_prefix/sig_electron_scale_factors_Reco_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar electron_scale_factors_ID -o $path_prefix/sig_electron_scale_factors_ID_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar electron_scale_factors_ID -o $path_prefix/sig_electron_scale_factors_ID_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar electron_scale_factors_ttHID -o $path_prefix/sig_electron_scale_factors_ttHID_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar electron_scale_factors_ttHID -o $path_prefix/sig_electron_scale_factors_ttHID_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar electron_scale_factors_ttHISO -o $path_prefix/sig_electron_scale_factors_ttHISO_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar electron_scale_factors_ttHISO -o $path_prefix/sig_electron_scale_factors_ttHISO_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar electron_scale_factors_trigger -o $path_prefix/sig_electron_scale_factors_trigger_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar electron_scale_factors_trigger -o $path_prefix/sig_electron_scale_factors_trigger_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar particlenet_w_weight_2016preVFP -o $path_prefix/sig_particlenet_w_weight_2016preVFP_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar particlenet_w_weight_2016preVFP -o $path_prefix/sig_particlenet_w_weight_2016preVFP_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar particlenet_w_weight_2016postVFP -o $path_prefix/sig_particlenet_w_weight_2016postVFP_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar particlenet_w_weight_2016postVFP -o $path_prefix/sig_particlenet_w_weight_2016postVFP_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar particlenet_w_weight_2017 -o $path_prefix/sig_particlenet_w_weight_2017_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar particlenet_w_weight_2017 -o $path_prefix/sig_particlenet_w_weight_2017_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar particlenet_w_weight_2018 -o $path_prefix/sig_particlenet_w_weight_2018_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar particlenet_w_weight_2018 -o $path_prefix/sig_particlenet_w_weight_2018_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar particlenet_h_weight_2016preVFP -o $path_prefix/sig_particlenet_h_weight_2016preVFP_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar particlenet_h_weight_2016preVFP -o $path_prefix/sig_particlenet_h_weight_2016preVFP_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar particlenet_h_weight_2016postVFP -o $path_prefix/sig_particlenet_h_weight_2016postVFP_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar particlenet_h_weight_2016postVFP -o $path_prefix/sig_particlenet_h_weight_2016postVFP_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar particlenet_h_weight_2017 -o $path_prefix/sig_particlenet_h_weight_2017_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar particlenet_h_weight_2017 -o $path_prefix/sig_particlenet_h_weight_2017_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar particlenet_h_weight_2018 -o $path_prefix/sig_particlenet_h_weight_2018_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar particlenet_h_weight_2018 -o $path_prefix/sig_particlenet_h_weight_2018_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar btagging_scale_factors_HF -o $path_prefix/sig_btagging_scale_factors_HF_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar btagging_scale_factors_HF -o $path_prefix/sig_btagging_scale_factors_HF_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar btagging_scale_factors_LF -o $path_prefix/sig_btagging_scale_factors_LF_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar btagging_scale_factors_LF -o $path_prefix/sig_btagging_scale_factors_LF_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar PSWeight_ISR -o $path_prefix/sig_PSWeight_ISR_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar PSWeight_ISR -o $path_prefix/sig_PSWeight_ISR_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar PSWeight_FSR -o $path_prefix/sig_PSWeight_FSR_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar PSWeight_FSR -o $path_prefix/sig_PSWeight_FSR_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar LHEScaleWeight_muR -o $path_prefix/sig_LHEScaleWeight_muR_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar LHEScaleWeight_muR -o $path_prefix/sig_LHEScaleWeight_muR_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar LHEScaleWeight_muF -o $path_prefix/sig_LHEScaleWeight_muF_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar LHEScaleWeight_muF -o $path_prefix/sig_LHEScaleWeight_muF_down.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var up --sfvar LHEWeights_pdf -o $path_prefix/sig_LHEWeights_pdf_up.root &
    ./bin/runAnalysis -n 2 -i input/sig.json --c2v $c2v --var down --sfvar LHEWeights_pdf -o $path_prefix/sig_LHEWeights_pdf_down.root &
    wait
    cd -
}

function runAnalysis() {
    c2v=$1
    year=$2

    if [ $c2v -eq -1 ]; then
        path_prefix=default
    else
        path_prefix=$c2v
    fi

    base_dir=/data/userdata/aaarora/output/run2

    rm -f $base_dir/sig*_MVA.root
    rm -f $base_dir/data_MVA.root
    rm -f $base_dir/ABCDNet_simpleDisco_VBSVVH1lep_30/output/*.root

    cd mva/

    if [ -z "$year" ]; then
            ./bin/runMVA -i $base_dir/data/data.root -o $base_dir/data_MVA.root &
        else
            ./bin/runMVA -i $base_dir/data/data.root --year $year -o $base_dir/data_MVA.root &
    fi

    for file in $(ls $base_dir/sig/$path_prefix/sig*.root)
    do
        base_name=$(basename $file .root)
        # check if year is defined
        if [ -z "$year" ]; then
            ./bin/runMVA -i $file -o $base_dir/${base_name}_MVA.root &
        else
            ./bin/runMVA -i $file --year $year -o $base_dir/${base_name}_MVA.root &
        fi
    done
    wait
    cd -

    cd dnn/
    python3 python/infer.py configs/ABCDNet_simpleDisco_VBSVVH1lep_30.json --export --epoch 500 --parallel
    cd -
    cd datacard/
    if [ -z "$year" ]; then
        python3 make_datacard.py --output run2.c2v=$c2v.txt
        else
        python3 make_datacard.py --year $year --output $year.c2v=$c2v.txt
    fi
    cd -
}

# for c2v in $(seq -1 35); do
#     runPreselection $c2v
# done

# for c2v in $(seq -1 35); do
#     runAnalysis $c2v 2016preVFP
#     runAnalysis $c2v 2016postVFP
#     runAnalysis $c2v 2017
#     runAnalysis $c2v 2018
# done

# for c2v in $(seq -1 35); do
#     runAnalysis $c2v
# done

runAnalysis 4 2016preVFP
runAnalysis 16 2016preVFP
runAnalysis 34 2016preVFP
runAnalysis 4
runAnalysis 16
runAnalysis 34

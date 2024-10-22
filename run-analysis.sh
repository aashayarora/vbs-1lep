#!/bin/bash

function runPreselection() {
    path_prefix='sig'
    cd ./preselection/
    ./bin/runAnalysis -n 2 -i input/sig.json -o $path_prefix/sig &
    ./bin/runAnalysis -n 2 -i input/sig.json --met --var up -o $path_prefix/sig_met_unclustered_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --met --var down -o $path_prefix/sig_met_unclustered_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_Absolute --var up -o $path_prefix/sig_jec_absolute_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_Absolute --var down -o $path_prefix/sig_jec_absolute_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_Absolute_2016preVFP --var up -o $path_prefix/sig_jec_absolute_2016preVFP_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_Absolute_2016preVFP --var down -o $path_prefix/sig_jec_absolute_2016preVFP_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_Absolute_2016postVFP --var up -o $path_prefix/sig_jec_absolute_2016postVFP_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_Absolute_2016postVFP --var down -o $path_prefix/sig_jec_absolute_2016postVFP_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_Absolute_2017 --var up -o $path_prefix/sig_jec_absolute_2017_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_Absolute_2017 --var down -o $path_prefix/sig_jec_absolute_2017_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_Absolute_2018 --var up -o $path_prefix/sig_jec_absolute_2018_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_Absolute_2018 --var down -o $path_prefix/sig_jec_absolute_2018_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_BBEC1 --var up -o $path_prefix/sig_jec_bbec1_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_BBEC1 --var down -o $path_prefix/sig_jec_bbec1_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_BBEC1_2016preVFP --var up -o $path_prefix/sig_jec_bbec1_2016preVFP_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_BBEC1_2016preVFP --var down -o $path_prefix/sig_jec_bbec1_2016preVFP_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_BBEC1_2016postVFP --var up -o $path_prefix/sig_jec_bbec1_2016postVFP_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_BBEC1_2016postVFP --var down -o $path_prefix/sig_jec_bbec1_2016postVFP_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_BBEC1_2017 --var up -o $path_prefix/sig_jec_bbec1_2017_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_BBEC1_2017 --var down -o $path_prefix/sig_jec_bbec1_2017_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_BBEC1_2018 --var up -o $path_prefix/sig_jec_bbec1_2018_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_BBEC1_2018 --var down -o $path_prefix/sig_jec_bbec1_2018_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_EC2 --var up -o $path_prefix/sig_jec_ec2_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_EC2 --var down -o $path_prefix/sig_jec_ec2_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_EC2_2016preVFP --var up -o $path_prefix/sig_jec_ec2_2016preVFP_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_EC2_2016preVFP --var down -o $path_prefix/sig_jec_ec2_2016preVFP_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_EC2_2016postVFP --var up -o $path_prefix/sig_jec_ec2_2016postVFP_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_EC2_2016postVFP --var down -o $path_prefix/sig_jec_ec2_2016postVFP_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_EC2_2017 --var up -o $path_prefix/sig_jec_ec2_2017_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_EC2_2017 --var down -o $path_prefix/sig_jec_ec2_2017_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_EC2_2018 --var up -o $path_prefix/sig_jec_ec2_2018_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_EC2_2018 --var down -o $path_prefix/sig_jec_ec2_2018_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_FlavorQCD --var up -o $path_prefix/sig_jec_flavorqcd_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_FlavorQCD --var down -o $path_prefix/sig_jec_flavorqcd_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_HF --var up -o $path_prefix/sig_jec_hf_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_HF --var down -o $path_prefix/sig_jec_hf_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_HF_2016preVFP --var up -o $path_prefix/sig_jec_hf_2016preVFP_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_HF_2016preVFP --var down -o $path_prefix/sig_jec_hf_2016preVFP_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_HF_2016postVFP --var up -o $path_prefix/sig_jec_hf_2016postVFP_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_HF_2016postVFP --var down -o $path_prefix/sig_jec_hf_2016postVFP_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_HF_2017 --var up -o $path_prefix/sig_jec_hf_2017_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_HF_2017 --var down -o $path_prefix/sig_jec_hf_2017_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_HF_2018 --var up -o $path_prefix/sig_jec_hf_2018_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_HF_2018 --var down -o $path_prefix/sig_jec_hf_2018_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_RelativeBal --var up -o $path_prefix/sig_jec_relativebal_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_RelativeBal --var down -o $path_prefix/sig_jec_relativebal_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_RelativeSample_2016preVFP --var up -o $path_prefix/sig_jec_relativesample_2016preVFP_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_RelativeSample_2016preVFP --var down -o $path_prefix/sig_jec_relativesample_2016preVFP_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_RelativeSample_2016postVFP --var up -o $path_prefix/sig_jec_relativesample_2016postVFP_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_RelativeSample_2016postVFP --var down -o $path_prefix/sig_jec_relativesample_2016postVFP_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_RelativeSample_2017 --var up -o $path_prefix/sig_jec_relativesample_2017_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_RelativeSample_2017 --var down -o $path_prefix/sig_jec_relativesample_2017_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_RelativeSample_2018 --var up -o $path_prefix/sig_jec_relativesample_2018_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type Regrouped_RelativeSample_2018 --var down -o $path_prefix/sig_jec_relativesample_2018_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type RelativeSample --var up -o $path_prefix/sig_jec_relative_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jec --jec_type RelativeSample --var down -o $path_prefix/sig_jec_relative_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jer --var up -o $path_prefix/sig_jer_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jer --var down -o $path_prefix/sig_jer_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jms --var up -o $path_prefix/sig_jms_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jms --var down -o $path_prefix/sig_jms_down &
    ./bin/runAnalysis -n 2 -i input/sig.json --jmr --var up -o $path_prefix/sig_jmr_up &
    ./bin/runAnalysis -n 2 -i input/sig.json --jmr --var down -o $path_prefix/sig_jmr_down &
    wait
    cd -
}

function runAnalysis() {
    c2v=$1
    bdt_cut=$2
    dnn_cut=$3
    year=$4

    if [ $c2v -eq -1 ]; then
        path_prefix=default
    else
        path_prefix=$c2v
    fi

    base_dir=/data/userdata/aaarora/output/run2

    rm -f $base_dir/si*_MVA.root
    rm -f $base_dir/data_MVA.root

    cd mva/

    if [ -z "$year" ]; then
            sleep 1; ./bin/runMVA -i $base_dir/data.root -o $base_dir/data_MVA.root &
        else
            sleep 1; ./bin/runMVA -i $base_dir/data.root --year $year -o $base_dir/data_MVA.root &
    fi

    for file in $(ls $base_dir/sig/si*.root)
    do
        base_name=$(basename $file .root)
        # check if year is defined
        if [ -z "$year" ]; then
            sleep 1; ./bin/runMVA --c2v $c2v -i $file -o $base_dir/${base_name}_MVA.root &
        else
            sleep 1; ./bin/runMVA --c2v $c2v -i $file --year $year -o $base_dir/${base_name}_MVA.root &
        fi
    done

    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar pileup_weight -o $base_dir/sig_pileup_weight_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar pileup_weight -o $base_dir/sig_pileup_weight_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar pileupid_weight -o $base_dir/sig_pileupid_weight_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar pileupid_weight -o $base_dir/sig_pileupid_weight_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar L1PreFiringWeight -o $base_dir/sig_L1PreFiringWeight_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar L1PreFiringWeight -o $base_dir/sig_L1PreFiringWeight_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar muon_scale_factors_ID -o $base_dir/sig_muon_scale_factors_ID_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar muon_scale_factors_ID -o $base_dir/sig_muon_scale_factors_ID_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar muon_scale_factors_trigger -o $base_dir/sig_muon_scale_factors_trigger_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar muon_scale_factors_trigger -o $base_dir/sig_muon_scale_factors_trigger_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar muon_scale_factors_ttHID -o $base_dir/sig_muon_scale_factors_ttHID_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar muon_scale_factors_ttHID -o $base_dir/sig_muon_scale_factors_ttHID_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar muon_scale_factors_ttHISO -o $base_dir/sig_muon_scale_factors_ttHISO_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar muon_scale_factors_ttHISO -o $base_dir/sig_muon_scale_factors_ttHISO_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar electron_scale_factors_Reco -o $base_dir/sig_electron_scale_factors_Reco_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar electron_scale_factors_Reco -o $base_dir/sig_electron_scale_factors_Reco_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar electron_scale_factors_ID -o $base_dir/sig_electron_scale_factors_ID_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar electron_scale_factors_ID -o $base_dir/sig_electron_scale_factors_ID_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar electron_scale_factors_ttHID -o $base_dir/sig_electron_scale_factors_ttHID_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar electron_scale_factors_ttHID -o $base_dir/sig_electron_scale_factors_ttHID_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar electron_scale_factors_ttHISO -o $base_dir/sig_electron_scale_factors_ttHISO_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar electron_scale_factors_ttHISO -o $base_dir/sig_electron_scale_factors_ttHISO_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar electron_scale_factors_trigger -o $base_dir/sig_electron_scale_factors_trigger_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar electron_scale_factors_trigger -o $base_dir/sig_electron_scale_factors_trigger_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar particlenet_w_weight_2016preVFP -o $base_dir/sig_particlenet_w_weight_2016preVFP_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar particlenet_w_weight_2016preVFP -o $base_dir/sig_particlenet_w_weight_2016preVFP_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar particlenet_w_weight_2016postVFP -o $base_dir/sig_particlenet_w_weight_2016postVFP_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar particlenet_w_weight_2016postVFP -o $base_dir/sig_particlenet_w_weight_2016postVFP_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar particlenet_w_weight_2017 -o $base_dir/sig_particlenet_w_weight_2017_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar particlenet_w_weight_2017 -o $base_dir/sig_particlenet_w_weight_2017_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar particlenet_w_weight_2018 -o $base_dir/sig_particlenet_w_weight_2018_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar particlenet_w_weight_2018 -o $base_dir/sig_particlenet_w_weight_2018_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar particlenet_h_weight_2016preVFP -o $base_dir/sig_particlenet_h_weight_2016preVFP_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar particlenet_h_weight_2016preVFP -o $base_dir/sig_particlenet_h_weight_2016preVFP_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar particlenet_h_weight_2016postVFP -o $base_dir/sig_particlenet_h_weight_2016postVFP_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar particlenet_h_weight_2016postVFP -o $base_dir/sig_particlenet_h_weight_2016postVFP_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar particlenet_h_weight_2017 -o $base_dir/sig_particlenet_h_weight_2017_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar particlenet_h_weight_2017 -o $base_dir/sig_particlenet_h_weight_2017_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar particlenet_h_weight_2018 -o $base_dir/sig_particlenet_h_weight_2018_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar particlenet_h_weight_2018 -o $base_dir/sig_particlenet_h_weight_2018_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar btagging_scale_factors_HF -o $base_dir/sig_btagging_scale_factors_HF_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar btagging_scale_factors_HF -o $base_dir/sig_btagging_scale_factors_HF_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar btagging_scale_factors_LF -o $base_dir/sig_btagging_scale_factors_LF_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar btagging_scale_factors_LF -o $base_dir/sig_btagging_scale_factors_LF_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar PSWeight_ISR -o $base_dir/sig_PSWeight_ISR_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar PSWeight_ISR -o $base_dir/sig_PSWeight_ISR_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar PSWeight_FSR -o $base_dir/sig_PSWeight_FSR_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar PSWeight_FSR -o $base_dir/sig_PSWeight_FSR_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar LHEScaleWeight_muR -o $base_dir/sig_LHEScaleWeight_muR_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar LHEScaleWeight_muR -o $base_dir/sig_LHEScaleWeight_muR_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar LHEScaleWeight_muF -o $base_dir/sig_LHEScaleWeight_muF_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar LHEScaleWeight_muF -o $base_dir/sig_LHEScaleWeight_muF_down_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var up --sfvar LHEWeights_pdf -o $base_dir/sig_LHEWeights_pdf_up_MVA.root &
    sleep 1; ./bin/runMVA -i $base_dir/sig/sig.root --c2v $c2v --var down --sfvar LHEWeights_pdf -o $base_dir/sig_LHEWeights_pdf_down_MVA.root &

    wait
    cd -

    cd datacard/
    mkdir -p datacards/$bdt_cut-$dnn_cut
    if [ -z "$year" ]; then
        python3 make_datacard.py --output $c2v --output_dir datacards/$bdt_cut-$dnn_cut
        else
        python3 make_datacard.py --year $year --output $c2v
    fi
    cd -
}

runPreselection

for c2v in $(seq 0 35); do
    runAnalysis $c2v $1 $2
done
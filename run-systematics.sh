#!/bin/bash

# Corrections

cd ./preselection/

./bin/runAnalysis -i input/sig.json --sig --met --var up -o sig_met_unclustered_up.root &
./bin/runAnalysis -i input/sig.json --sig --met --var down -o sig_met_unclustered_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_Absolute --var down -o sig_jec_absolute_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_Absolute_2016 --var up -o sig_jec_absolute_2016_up.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_Absolute_2016 --var down -o sig_jec_absolute_2016_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_Absolute_2017 --var up -o sig_jec_absolute_2017_up.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_Absolute_2017 --var down -o sig_jec_absolute_2017_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_Absolute_2018 --var up -o sig_jec_absolute_2018_up.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_Absolute_2018 --var down -o sig_jec_absolute_2018_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_BBEC1 --var down -o sig_jec_bbec1_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_BBEC1_2016 --var up -o sig_jec_bbec1_2016_up.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_BBEC1_2016 --var down -o sig_jec_bbec1_2016_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_BBEC1_2017 --var up -o sig_jec_bbec1_2017_up.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_BBEC1_2017 --var down -o sig_jec_bbec1_2017_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_BBEC1_2018 --var up -o sig_jec_bbec1_2018_up.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_BBEC1_2018 --var down -o sig_jec_bbec1_2018_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_EC2 --var down -o sig_jec_ec2_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_EC2_2016 --var up -o sig_jec_ec2_2016_up.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_EC2_2016 --var down -o sig_jec_ec2_2016_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_EC2_2017 --var up -o sig_jec_ec2_2017_up.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_EC2_2017 --var down -o sig_jec_ec2_2017_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_EC2_2018 --var up -o sig_jec_ec2_2018_up.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_EC2_2018 --var down -o sig_jec_ec2_2018_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_FlavorQCD --var down -o sig_jec_flavorqcd_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_HF --var down -o sig_jec_hf_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_HF_2016 --var up -o sig_jec_hf_2016_up.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_HF_2016 --var down -o sig_jec_hf_2016_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_HF_2017 --var up -o sig_jec_hf_2017_up.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_HF_2017 --var down -o sig_jec_hf_2017_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_HF_2018 --var up -o sig_jec_hf_2018_up.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_HF_2018 --var down -o sig_jec_hf_2018_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_RelativeBal --var down -o sig_jec_relativebal_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_RelativeSample_2016 --var up -o sig_jec_relativesample_2016_up.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_RelativeSample_2016 --var down -o sig_jec_relativesample_2016_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_RelativeSample_2017 --var up -o sig_jec_relativesample_2017_up.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_RelativeSample_2017 --var down -o sig_jec_relativesample_2017_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_RelativeSample_2018 --var up -o sig_jec_relativesample_2018_up.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_RelativeSample_2018 --var down -o sig_jec_relativesample_2018_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type RelativeSample --var down -o sig_jec_relative_down.root &
./bin/runAnalysis -i input/sig.json --sig --jervar up -o sig_jer_up.root &
./bin/runAnalysis -i input/sig.json --sig --jervar down -o sig_jer_down.root &
./bin/runAnalysis -i input/sig.json --sig --jms --var up -o sig_jms_up.root &
./bin/runAnalysis -i input/sig.json --sig --jms --var down -o sig_jms_down.root &
./bin/runAnalysis -i input/sig.json --sig --jmr --var up -o sig_jmr_up.root &
./bin/runAnalysis -i input/sig.json --sig --jmr --var down -o sig_jmr_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar pileup_weight -o sig_pileup_weight_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar pileup_weight -o sig_pileup_weight_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar pileupid_weight -o sig_pileupid_weight_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar pileupid_weight -o sig_pileupid_weight_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar L1PreFiringWeight -o sig_L1PreFiringWeight_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar L1PreFiringWeight -o sig_L1PreFiringWeight_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar muon_scale_factors_ID -o sig_muon_scale_factors_ID_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar muon_scale_factors_ID -o sig_muon_scale_factors_ID_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar muon_scale_factors_trigger -o sig_muon_scale_factors_trigger_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar muon_scale_factors_trigger -o sig_muon_scale_factors_trigger_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar muon_scale_factors_ttHID -o sig_muon_scale_factors_ttHID_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar muon_scale_factors_ttHID -o sig_muon_scale_factors_ttHID_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar muon_scale_factors_ttHISO -o sig_muon_scale_factors_ttHISO_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar muon_scale_factors_ttHISO -o sig_muon_scale_factors_ttHISO_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar electron_scale_factors_Reco -o sig_electron_scale_factors_Reco_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar electron_scale_factors_Reco -o sig_electron_scale_factors_Reco_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar electron_scale_factors_ID -o sig_electron_scale_factors_ID_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar electron_scale_factors_ID -o sig_electron_scale_factors_ID_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar electron_scale_factors_ttHID -o sig_electron_scale_factors_ttHID_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar electron_scale_factors_ttHID -o sig_electron_scale_factors_ttHID_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar electron_scale_factors_ttHISO -o sig_electron_scale_factors_ttHISO_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar electron_scale_factors_ttHISO -o sig_electron_scale_factors_ttHISO_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar electron_scale_factors_trigger -o sig_electron_scale_factors_trigger_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar electron_scale_factors_trigger -o sig_electron_scale_factors_trigger_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar particlenet_w_weight_2016preVFP -o sig_particlenet_w_weight_2016preVFP_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar particlenet_w_weight_2016preVFP -o sig_particlenet_w_weight_2016preVFP_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar particlenet_w_weight_2016postVFP -o sig_particlenet_w_weight_2016postVFP_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar particlenet_w_weight_2016postVFP -o sig_particlenet_w_weight_2016postVFP_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar particlenet_w_weight_2017 -o sig_particlenet_w_weight_2017_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar particlenet_w_weight_2017 -o sig_particlenet_w_weight_2017_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar particlenet_w_weight_2018 -o sig_particlenet_w_weight_2018_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar particlenet_w_weight_2018 -o sig_particlenet_w_weight_2018_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar particlenet_h_weight_2016preVFP -o sig_particlenet_h_weight_2016preVFP_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar particlenet_h_weight_2016preVFP -o sig_particlenet_h_weight_2016preVFP_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar particlenet_h_weight_2016postVFP -o sig_particlenet_h_weight_2016postVFP_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar particlenet_h_weight_2016postVFP -o sig_particlenet_h_weight_2016postVFP_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar particlenet_h_weight_2017 -o sig_particlenet_h_weight_2017_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar particlenet_h_weight_2017 -o sig_particlenet_h_weight_2017_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar particlenet_h_weight_2018 -o sig_particlenet_h_weight_2018_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar particlenet_h_weight_2018 -o sig_particlenet_h_weight_2018_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar btagging_scale_factors_HF -o sig_btagging_scale_factors_HF_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar btagging_scale_factors_HF -o sig_btagging_scale_factors_HF_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar btagging_scale_factors_LF -o sig_btagging_scale_factors_LF_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar btagging_scale_factors_LF -o sig_btagging_scale_factors_LF_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar PSWeight_ISR -o sig_PSWeight_ISR_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar PSWeight_ISR -o sig_PSWeight_ISR_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar PSWeight_FSR -o sig_PSWeight_FSR_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar PSWeight_FSR -o sig_PSWeight_FSR_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar LHEScaleWeight_muR -o sig_LHEScaleWeight_muR_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar LHEScaleWeight_muR -o sig_LHEScaleWeight_muR_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar LHEScaleWeight_muF -o sig_LHEScaleWeight_muF_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar LHEScaleWeight_muF -o sig_LHEScaleWeight_muF_down.root &
./bin/runAnalysis -i input/sig.json --sig --var up --sfvar LHEWeights_pdf -o sig_LHEWeights_pdf_up.root &
./bin/runAnalysis -i input/sig.json --sig --var down --sfvar LHEWeights_pdf -o sig_LHEWeights_pdf_down.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_Absolute --var up -o sig_jec_absolute_up.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_BBEC1 --var up -o sig_jec_bbec1_up.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_EC2 --var up -o sig_jec_ec2_up.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_HF --var up -o sig_jec_hf_up.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_FlavorQCD --var up -o sig_jec_flavorqcd_up.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type Regrouped_RelativeBal --var up -o sig_jec_relativebal_up.root &
./bin/runAnalysis -i input/sig.json --sig --jec --jec_type RelativeSample --var up -o sig_jec_relative_up.root &

wait
cd -

cd mva/

./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_met_unclustered_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_met_unclustered_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_absolute_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_absolute_2016_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_absolute_2016_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_absolute_2017_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_absolute_2017_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_absolute_2018_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_absolute_2018_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_bbec1_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_bbec1_2016_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_bbec1_2016_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_bbec1_2017_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_bbec1_2017_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_bbec1_2018_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_bbec1_2018_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_ec2_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_ec2_2016_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_ec2_2016_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_ec2_2017_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_ec2_2017_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_ec2_2018_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_ec2_2018_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_flavorqcd_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_hf_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_hf_2016_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_hf_2016_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_hf_2017_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_hf_2017_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_hf_2018_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_hf_2018_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_relativebal_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_relativesample_2016_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_relativesample_2016_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_relativesample_2017_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_relativesample_2017_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_relativesample_2018_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_relativesample_2018_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_relative_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jer_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jer_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jms_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jms_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jmr_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jmr_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_pileup_weight_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_pileup_weight_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_pileupid_weight_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_pileupid_weight_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_L1PreFiringWeight_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_L1PreFiringWeight_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_muon_scale_factors_ID_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_muon_scale_factors_ID_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_muon_scale_factors_trigger_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_muon_scale_factors_trigger_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_muon_scale_factors_ttHID_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_muon_scale_factors_ttHID_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_muon_scale_factors_ttHISO_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_muon_scale_factors_ttHISO_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_electron_scale_factors_Reco_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_electron_scale_factors_Reco_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_electron_scale_factors_ID_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_electron_scale_factors_ID_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_electron_scale_factors_ttHID_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_electron_scale_factors_ttHID_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_electron_scale_factors_ttHISO_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_electron_scale_factors_ttHISO_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_electron_scale_factors_trigger_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_electron_scale_factors_trigger_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_particlenet_w_weight_2016preVFP_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_particlenet_w_weight_2016preVFP_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_particlenet_w_weight_2016postVFP_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_particlenet_w_weight_2016postVFP_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_particlenet_w_weight_2017_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_particlenet_w_weight_2017_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_particlenet_w_weight_2018_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_particlenet_w_weight_2018_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_particlenet_h_weight_2016preVFP_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_particlenet_h_weight_2016preVFP_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_particlenet_h_weight_2016postVFP_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_particlenet_h_weight_2016postVFP_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_particlenet_h_weight_2017_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_particlenet_h_weight_2017_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_particlenet_h_weight_2018_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_particlenet_h_weight_2018_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_btagging_scale_factors_HF_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_btagging_scale_factors_HF_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_btagging_scale_factors_LF_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_btagging_scale_factors_LF_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_PSWeight_ISR_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_PSWeight_ISR_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_PSWeight_FSR_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_PSWeight_FSR_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_LHEScaleWeight_muR_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_LHEScaleWeight_muR_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_LHEScaleWeight_muF_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_LHEScaleWeight_muF_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_LHEWeights_pdf_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_LHEWeights_pdf_down.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_absolute_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_bbec1_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_ec2_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_flavorqcd_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_hf_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_relativebal_up.root &
./bin/runMVA -i /data/userdata/aaarora/output/run2/sig_jec_relative_up.root &

cd -
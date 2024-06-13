#!/bin/bash

# Run the MVA analysis
./bin/runMVA ~/nvme/output/run2/data.root ~/nvme/output/run2/data_bdt.root
./bin/runMVA ~/nvme/output/run2/sig.root ~/nvme/output/run2/sig_bdt.root
./bin/runMVA ~/nvme/output/run2/bkg.root ~/nvme/output/run2/bkg_bdt.root
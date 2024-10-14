#!/bin/bash

./bin/runAnalysis -n 16 -i input/bkg_dy.json --cut passCut7 -o dy --cutflow &
./bin/runAnalysis -n 16 -i input/bkg_ttbar.json --cut passCut7 -o ttbar --cutflow &
./bin/runAnalysis -n 16 -i input/bkg_ttx.json --cut passCut7 -o ttx --cutflow &
./bin/runAnalysis -n 16 -i input/bkg_wjets.json --cut passCut7 -o wjets --cutflow &
./bin/runAnalysis -n 16 -i input/bkg_singletop.json --cut passCut7 -o singletop --cutflow &
./bin/runAnalysis -n 16 -i input/bkg_other.json --cut passCut7 -o other --cutflow &
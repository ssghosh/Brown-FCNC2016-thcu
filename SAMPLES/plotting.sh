#!/bin/bash

./stackPlotter full_outputs/signal_output.root sig_plots/ lep_pt r
./stackPlotter full_outputs/signal_output.root sig_plots/ tau_pt r
./stackPlotter full_outputs/signal_output.root sig_plots/ lep_eta r
./stackPlotter full_outputs/signal_output.root sig_plots/ tau_eta r

cd sig_plots
sigpattern="sig*"
for f in *; do 
    if ! [[ ${f} =~ ${sigpattern} ]]; then
        mv "$f" "sig_$f"; 
    fi
done
cd ..

./stackPlotter full_outputs/bkg_output.root bkg_plots/ lep_pt r
./stackPlotter full_outputs/bkg_output.root bkg_plots/ tau_pt r
./stackPlotter full_outputs/bkg_output.root bkg_plots/ lep_eta r
./stackPlotter full_outputs/bkg_output.root bkg_plots/ tau_eta r

cd bkg_plots
bkgpattern="bkg*"
for f in *; do 
    if ! [[ ${f} =~ ${bkgpattern} ]]; then
        mv "$f" "bkg_$f"; 
    fi
done
cd ..

rsync -rv sig_plots soghosh@brux.hep.brown.edu:~/
rsync -rv bkg_plots soghosh@brux.hep.brown.edu:~/

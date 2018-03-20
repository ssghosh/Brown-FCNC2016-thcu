#!/bin/bash

python submitCondor.py --pileup 200 \
    --inputDir /eos/uscms/store/user/ssghosh/FCNC_signal_samples_LHE/ \
    --outDir /eos/uscms/store/user/ssghosh/FCNC_signal_samples_Delphes/ \
    --condorOutDir /afs/cern.ch/user/s/soghosh/WORKINGAREA/CMSSW_8_0_11/src/Brown-FCNC2016-thcu/SAMPLES/condor/LHEtoDelphes
    #--condorOutDir /uscms_data/d3/ssghosh/DelphesFromLHE_Logs/

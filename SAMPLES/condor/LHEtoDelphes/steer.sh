#!/bin/bash

python submitCondor.py --pileup 200 \
    --inputDir /eos/uscms/store/user/ssghosh/FCNC_signal_samples/ \
    --outDir /eos/uscms/store/user/ssghosh/FCNC_signal_samples/DelphesFromLHE/ \
    --condorOutDir /uscms_data/d3/ssghosh/DelphesFromLHE_Logs/

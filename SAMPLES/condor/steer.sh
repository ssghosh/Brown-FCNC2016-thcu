#!/bin/bash

curdir=`pwd`
# set up the environment
#cd /uscms_data/d3/ssghosh/CMSSW_7_6_4/src
#source /cvmfs/cms.cern.ch/cmsset_default.sh
#eval `scramv1 runtime -sh`

# go back to the original directory and submit jobs
echo $curdir
cd $curdir
python SubmitCondor.py --numEvents 5000 \
    --evPerJob 1000 \
    --decayMode eta_hut \
    --decayQk top \
    --numCPU 8 \
    --eosoutpath /FCNC_signal_samples_LHE/TT_topHadronicDecay_eta_hut_LO_14TEV/ \
    --homeoutpath /afs/cern.ch/user/s/soghosh/WORKINGAREA/CMSSW_8_0_11/src/Brown-FCNC2016-thcu/SAMPLES/condor/TT_topHadronicDecay_eta_hut_LO_14TEV_HE/ \
    --wdecay hadronic

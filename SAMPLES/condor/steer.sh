#!/bin/bash

curdir=`pwd`
# set up the environment
cd /home/soghosh/CMSSW_7_6_4/src
source /cvmfs/cms.cern.ch/cmsset_default.sh
eval `scramv1 runtime -sh`

# go back to the original directory and submit jobs
echo $curdir
cd $curdir

# topHadronicDecay_eta_hut
python SubmitCondor.py --numEvents 125000 \
    --evPerJob 125000 \
    --decayMode eta_hut \
    --decayQk top \
    --numCPU 8 \
    --eosoutpath /FCNC_signal_samples_LHE/TT_topHadronicDecay_eta_hut_LO_14TEV/ \
    --homeoutpath /home/soghosh/CMSSW_8_0_11/src/Brown-FCNC2016-thcu/SAMPLES/condor/TT_topHadronicDecay_eta_hut_LO_14TEV/ \
    --wdecay Hadronic

# topLeptonicDecay_eta_hut
python SubmitCondor.py --numEvents 125000 \
    --evPerJob 125000 \
    --decayMode eta_hut \
    --decayQk top \
    --numCPU 8 \
    --eosoutpath /FCNC_signal_samples_LHE/TT_topLeptonicDecay_eta_hut_LO_14TEV/ \
    --homeoutpath /home/soghosh/CMSSW_8_0_11/src/Brown-FCNC2016-thcu/SAMPLES/condor/TT_topLeptonicDecay_eta_hut_LO_14TEV/ \
    --wdecay Leptonic

# topHadronicDecay_eta_hct
python SubmitCondor.py --numEvents 125000 \
    --evPerJob 125000 \
    --decayMode eta_hct \
    --decayQk top \
    --numCPU 8 \
    --eosoutpath /FCNC_signal_samples_LHE/TT_topHadronicDecay_eta_hct_LO_14TEV/ \
    --homeoutpath /home/soghosh/CMSSW_8_0_11/src/Brown-FCNC2016-thcu/SAMPLES/condor/TT_topHadronicDecay_eta_hct_LO_14TEV/ \
    --wdecay Hadronic

# topLeptonicDecay_eta_hct
python SubmitCondor.py --numEvents 125000 \
    --evPerJob 125000 \
    --decayMode eta_hct \
    --decayQk top \
    --numCPU 8 \
    --eosoutpath /FCNC_signal_samples_LHE/TT_topLeptonicDecay_eta_hct_LO_14TEV/ \
    --homeoutpath /home/soghosh/CMSSW_8_0_11/src/Brown-FCNC2016-thcu/SAMPLES/condor/TT_topLeptonicDecay_eta_hct_LO_14TEV/ \
    --wdecay Leptonic

# antitopHadronicDecay_eta_hut
python SubmitCondor.py --numEvents 125000 \
    --evPerJob 125000 \
    --decayMode eta_hut \
    --decayQk antitop \
    --numCPU 8 \
    --eosoutpath /FCNC_signal_samples_LHE/TT_antitopHadronicDecay_eta_hut_LO_14TEV/ \
    --homeoutpath /home/soghosh/CMSSW_8_0_11/src/Brown-FCNC2016-thcu/SAMPLES/condor/TT_antitopHadronicDecay_eta_hut_LO_14TEV/ \
    --wdecay Hadronic

# antitopLeptonicDecay_eta_hut
python SubmitCondor.py --numEvents 125000 \
    --evPerJob 125000 \
    --decayMode eta_hut \
    --decayQk antitop \
    --numCPU 8 \
    --eosoutpath /FCNC_signal_samples_LHE/TT_antitopLeptonicDecay_eta_hut_LO_14TEV/ \
    --homeoutpath /home/soghosh/CMSSW_8_0_11/src/Brown-FCNC2016-thcu/SAMPLES/condor/TT_antitopLeptonicDecay_eta_hut_LO_14TEV/ \
    --wdecay Leptonic

# antitopHadronicDecay_eta_hct
python SubmitCondor.py --numEvents 125000 \
    --evPerJob 125000 \
    --decayMode eta_hct \
    --decayQk antitop \
    --numCPU 8 \
    --eosoutpath /FCNC_signal_samples_LHE/TT_antitopHadronicDecay_eta_hct_LO_14TEV/ \
    --homeoutpath /home/soghosh/CMSSW_8_0_11/src/Brown-FCNC2016-thcu/SAMPLES/condor/TT_antitopHadronicDecay_eta_hct_LO_14TEV/ \
    --wdecay Hadronic

# antitopLeptonicDecay_eta_hct
python SubmitCondor.py --numEvents 125000 \
    --evPerJob 125000 \
    --decayMode eta_hct \
    --decayQk antitop \
    --numCPU 8 \
    --eosoutpath /FCNC_signal_samples_LHE/TT_antitopLeptonicDecay_eta_hct_LO_14TEV/ \
    --homeoutpath /home/soghosh/CMSSW_8_0_11/src/Brown-FCNC2016-thcu/SAMPLES/condor/TT_antitopLeptonicDecay_eta_hct_LO_14TEV/ \
    --wdecay Leptonic

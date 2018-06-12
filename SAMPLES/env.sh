

OLDDIR=`pwd`

#DAnalysis
#DANALYSISPATH=/afs/cern.ch/user/j/jkiesele/public/DAnalysis_framework/v.1.0.0_rc0-beta.2/DAnalysis
#DANALYSISPATH=/afs/cern.ch/user/j/jkiesele/public/DAnalysis_framework/v.1.1_rc2_special_d3.4/DAnalysis
DANALYSISPATH=/uscms_data/d3/ssghosh/DAnalysis/DAnalysis


#CMSSW and Delphes
#DELPHESPATH=/afs/cern.ch/user/s/soghosh/WORKINGAREA/CMSSW_8_0_11/src/Brown-FCNC2016-thcu/SAMPLES/delphes
cd $DANALYSISPATH/../
cd CMSSW_8_0_4
eval `scramv1 runtime -sh`
cd ..
cd delphes
#cd $DELPHESPATH
source DelphesEnv.sh
export DELPHES_PATH=`pwd`
cd $OLDDIR


export PYTHIA8=$CMSSW_RELEASE_BASE/../../../external/pythia8/212-ikhhed3
export LD_LIBRARY_PATH=$PYTHIA8/lib:$LD_LIBRARY_PATH
export DANALYSISPATH=$DANALYSISPATH
export LD_LIBRARY_PATH=$DANALYSISPATH:$LD_LIBRARY_PATH
export PATH=$PATH:$DANALYSISPATH
#export LD_LIBRARY_PATH=/afs/cern.ch/user/${USER: 0 : 1}/${USER}/Brown-FCNC2016-thcu/SAMPLES:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/uscms_data/d3/${USER}/CMSSW_8_0_11/Brown-FCNC2016-thcu/SAMPLES:$LD_LIBRARY_PATH
#export PATH=/afs/cern.ch/user/${USER: 0 : 1}/${USER}/Brown-FCNC2016-thcu/SAMPLES:$PATH
export PATH=/uscms_data/d3/${USER}/CMSSW_8_0_11/src/Brown-FCNC2016-thcu/SAMPLES:$PATH

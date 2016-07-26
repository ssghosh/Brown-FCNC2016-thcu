#!/bin/tcsh -f

setenv DISPLAY 0
setenv PATH /bin:/usr/bin:/usr/local/bin:/usr/krb5/bin:/usr/afsws/bin:/usr/krb5/bin/aklog
cd /uscms_data/d3/${USER}/CMSSW_8_0_4/src/

#setenv SCRAM_ARCH slc6_amd64_gcc530
source /cvmfs/cms.cern.ch/cmsset_default.csh
eval `scramv1 runtime -csh`
rehash

root -l -b -q $CMSSW_BASE/../Brown-FCNC2016-thcu/src/SelectorExample.C'("OUTPUT_PATH/NAME.txt", "${_CONDOR_SCRATCH_DIR}/NAME.root","TREE_TITLE","SELECTION")'


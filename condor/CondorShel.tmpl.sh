#!/bin/tcsh -f
#date
#source /uscmst1/prod/sw/cms/setup/cshrc uaf
#setenv SCRAM_ARCH slc5_amd64_gcc434

#set MAIN_Dir = OUTPUT_PATH
#echo MAIN_Dir
#cd $MAIN_Dir

#eval `scram runtime -csh`

#echo "SCRAM_ARCH " $SCRAM_ARCH
#pwd

#cd ${_CONDOR_SCRATCH_DIR}

#echo Running
#date

#root -l -q $CMSSW_BASE/src/Delphes_Analysis/src/SelectorExample.C'("OUTPUT_PATH/NAME.txt", "OUTPUT_PATH/NAME.root","TREE_TITLE","SELECTION")'

#echo Done.
#date
setenv DISPLAY 0
setenv PATH /bin:/usr/bin:/usr/local/bin:/usr/krb5/bin:/usr/afsws/bin:/usr/krb5/bin/aklog
cd /uscms_data/d3/agarabed/Delphes_Analysis/CMSSW_8_0_4/src/

setenv SCRAM_ARCH slc6_amd64_gcc530
source /cvmfs/cms.cern.ch/cmsset_default.csh
eval `scramv1 runtime -csh`
rehash

root -l -q $CMSSW_BASE/src/Delphes_Analysis/src/SelectorExample.C'("OUTPUT_PATH/NAME.txt", "${_CONDOR_SCRATCH_DIR}/NAME.root","TREE_TITLE","SELECTION")'


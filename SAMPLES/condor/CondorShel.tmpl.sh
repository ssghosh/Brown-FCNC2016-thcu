#!/bin/bash 

# modify path and setup basic env
export DISPLAY=0
export PATH=/bin:/usr/bin:/usr/local/bin:/usr/krb5/bin:/usr/afsws/bin:/usr/krb5/bin/aklog
source /cvmfs/cms.cern.ch/cmsset_default.sh

# setup cmssw environment
#cmsrel CMSSWVER
xrdcp CMSSWLOC/CMSSWVER.tar.gz ${_CONDOR_SCRATCH_DIR}/cmssw.tar.gz
tar -xf cmssw.tar.gz
rm -rf cmssw.tar.gz
cd ${_CONDOR_SCRATCH_DIR}/CMSSWVER/src/
eval `scramv1 runtime -sh`

# setup lhapdf and fastjet
#scram setup fastjet
#scram setup lhapdf

export LHAPDF_BASE=/cvmfs/cms.cern.ch/slc6_amd64_gcc493/external/lhapdf/6.1.5-kpegke3
export FASTJET_BASE=/cvmfs/cms.cern.ch/slc6_amd64_gcc493/external/fastjet/3.1.0
#export LHAPDF_BASE=`scram build echo_LHAPDF_BASE | awk '/LHAPDF_BASE/{print $NF}'`
#export FASTJET_BASE=`scram build echo_FASTJET_BASE | awk '/FASTJET_BASE/{print $NF}'`

echo ">>> so LHAPDF_BASE is:"
echo $LHAPDF_BASE
echo ">>> so FASTJET_BASE is:"
echo $FASTJET_BASE

# unpack the gridpack and run it
tar -xf TT_no_top_decay_LO_xqcut20_tarball_14TeV.tar.xz
echo "after unpacking tarball, file list is:"
ls
./runcmsgrid.sh EVTS_PER_FILE RANDOM_SEED NUM_CPUS

# produce the desired decay
python madspin_fcnc.py cmsgrid_final.lhe tmp DECAY_MODE W_DECAY
# pick top or antitop
source DECAY_SCRIPT

#gunzip cmsgrid_final_decayed_DECAYED_QUARK.lhe.gz
xrdcp cmsgrid_final_decayed_DECAYED_QUARK.lhe.gz root://cmseos.fnal.gov:///store/user/USER/EOS_OUTDIRPATH/cmsgrid_final_decayed_DECAYED_QUARK_SUFFIX.lhe.gz

## copy analysis files
#xrdcp ANASUBLOC/anaSubstructure ${_CONDOR_SCRATCH_DIR}/anaSubstructure
#xrdcp INDIR/FILE.lhe ${_CONDOR_SCRATCH_DIR}/
#xrdcp ANASUBLOC/lhc14-pythia8-4C-minbias-nev100.pu14.gz ${_CONDOR_SCRATCH_DIR}/
#
## run analysis
#cd ${_CONDOR_SCRATCH_DIR}
#chmod 777 anaSubstructure
#./anaSubstructure FILE ./ MINEV MAXEV CFG TAG
#xrdcp processed-FILE-TAG.root root://cmseos.fnal.gov:///store/user/USER/OUTDIRFOLD/
#rm -rf CMSSWVER/
#rm *.lhe *.root *.pu14.gz anaSubstructure 

#!/bin/bash

#################################### Wrapper submit script for Upgrade production 
#Written by Alexis Kalogeropoulos - July 2014

source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc530

startTime=`date +%s`

# Condor arguments
DIRECTORY=$1
OUTPUT=$2
FILENAME=$3
QCUT=$4
JETS=$5
PILEUP=$6

echo "Starting job on " `date`
echo "Running on " `uname -a`
echo "System release " `cat /etc/redhat-release`

# Set variables
runEvents=-1
skipEvents=0
detCard=CMS_PhaseII_${PILEUP}PU_v03.tcl
energy=14
DelphesVersion=tags/3.4.2pre15
nPU=`echo $detCard | cut -d '_' -f 2`
process=`echo $FILENAME | cut -d '_' -f 1-2`
phase=`echo $detCard | cut -d '_' -f 1`
configuration=`echo $detCard | cut -d '_' -f 3 | cut -d '.' -f 1`
DelphesOutput=`echo $FILENAME | cut -d '.' -f 1`_${phase}_${configuration}_${nPU}.root
metaData=`echo $DelphesOutput | sed s/root/txt/`

# make the CMSSW release, compile, and copy
scram project CMSSW_9_1_0_pre3
cd CMSSW_9_1_0_pre3/src
eval `scram runtime -sh`
cd -

echo "xrdcp source tarball and pileup file"
#xrdcp -f root://cmseos.fnal.gov//store/user/snowmass/DelphesSubmissionLPCcondor//Delphes342pre15.tar .
xrdcp -f root://cmseos.fnal.gov//store/user/ssghosh/FCNC/Delphes342pre15.tar .
XRDEXIT=$?
if [[ $XRDEXIT -ne 0 ]]; then
    echo "exit code $XRDEXIT, failure in xrdcp of Delphes.tar"
    exit $XRDEXIT
fi

tar -xf Delphes342pre15.tar
cd delphes
#./configure
#make -j 4

xrdcp -f root://cmseos.fnal.gov//store/user/snowmass/DelphesSubmissionLPCcondor/MinBias_100k.pileup .
XRDEXIT=$?
if [[ $XRDEXIT -ne 0 ]]; then
    echo "exit code $XRDEXIT, failure in xrdcp of MinBias_100k.pileup"
    exit $XRDEXIT
fi

echo "xrdcp input LHE zip"
xrdcp -f root://cmseos.fnal.gov/${DIRECTORY}/${FILENAME} .
echo "Directory is ${DIRECTORY}"
echo "Filename is ${FILENAME}"
XRDEXIT=$?
if [[ $XRDEXIT -ne 0 ]]; then
    echo "exit code $XRDEXIT, failure in xrdcp of LHE file"
    exit $XRDEXIT
fi

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# Run DelphesPythia8

lhefile=events.lhe
echo "Swapping weight index in LHE file ${FILENAME}, unzipping to ${lhefile}"
noweightstring="2212     2212  0.70000000000E+04  0.70000000000E+04 0 0 10042 10042 3  3"
weightstring="2212     2212  0.70000000000E+04  0.70000000000E+04 0 0 10042 10042 4  3"

gunzip -c ${FILENAME} > ${lhefile}
sed -i "s/$noweightstring/$weightstring/" ${lhefile}
nEventsIn=`grep -c '<event>' ${lhefile}`
echo "=================== There are $nEventsIn events for $lhefile ============================="

rm ${FILENAME}

cp configLHE_jetmatching.cmnd hadronizer.cmnd

sed -i "s|RUNEVENTS|${nEventsIn}|g" hadronizer.cmnd
sed -i "s|SETQCUT|${QCUT}|g" hadronizer.cmnd
sed -i "s|SETMAXJETS|${JETS}|g" hadronizer.cmnd

setupTime=`date +%s`

./DelphesPythia8 cards/CMS_PhaseII/$detCard hadronizer.cmnd $DelphesOutput
if [ $? -ne 0 ]; then
    exit 16
fi

rm ${lhefile}

DelphesTime=`date +%s`

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#metadata

echo "User: " `eval whoami` > $metaData
echo "Date: " `date` >> $metaData
echo >> $metaData

echo "Process: " $process >> $metaData
echo "Pileup Conditions: " $nPU >> $metaData
echo "Phase: " $phase >> $metaData
echo "Configuration: " $configuration >> $metaData
echo "Energy: " $energy >> $metaData
echo >> $metaData

echo "Input LHE: " $FILENAME >> $metaData
echo "Input Events: " $nEventsIn >> $metaData
echo >> $metaData

echo "Delphes Output: " $DelphesOutput >> $metaData
echo "Delphes Version: " $DelphesVersion >> $metaData
echo "Detector Card: " $detCard >> $metaData
echo >> $metaData

echo "Minutes spent setting up job: " `expr $setupTime / 60 - $startTime / 60` >> $metaData
echo "Minutes spent running Delphes: " `expr $DelphesTime / 60 - $startTime / 60` >> $metaData
echo >> $metaData

echo "Pythia Card:" >> $metaData
cat hadronizer.cmnd >> $metaData
echo >> $metaData

echo "Delphes Card:" >> $metaData
cat cards/CMS_PhaseII/$detCard >> $metaData
echo >> $metaData

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# copy output to eos
echo "xrdcp -f ${DelphesOutput} ${eosOutDir}/${DelphesOutput}"
xrdcp -f ${DelphesOutput} root://cmseos.fnal.gov/${OUTPUT}/${DelphesOutput} 2>&1
XRDEXIT=$?
if [[ $XRDEXIT -ne 0 ]]; then
    echo "exit code $XRDEXIT, failure in xrdcp of ROOT"
    exit $XRDEXIT
fi

xrdcp -f ${metaData} root://cmseos.fnal.gov/${OUTPUT}/metaData/${metaData} 2>&1
XRDEXIT=$?
if [[ $XRDEXIT -ne 0 ]]; then
    echo "exit code $XRDEXIT, failure in xrdcp of metaData"
    exit $XRDEXIT
fi

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
endTime=`date +%s`
echo "Time spent copying output (s): " `expr $endTime - $DelphesTime`
echo "Total runtime (m): " `expr $endTime / 60 - $startTime / 60`

echo "removing inputs from condor"
rm -f ${DelphesOutput}
rm -f ${metaData}
rm -f ../Delphes342pre05.tar *.lhe *.gz hadronizer.cmnd MinBias_100k.pileup

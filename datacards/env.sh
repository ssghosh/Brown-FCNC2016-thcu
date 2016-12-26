#!/bin/sh

OLDDIR=`pwd`

SETUPDIR= ~/dcache/CMSSW_7_4_7/src

# set up CMSSW  
cd $SETUPDIR
cd CMSSW_8_0_4
eval `scramv1 runtime -sh`
cd $OLDDIR

# Brown-FCNC2016-thcu
Codebase for Brown's analysis of Flavor-Changing Neutral Currents: measuring the branching fraction for t->H(c/u) decays.

##Setup Area (LXPLUS):

Setting up only needs to be done once:

```
   $ cd ~
   $ cmsrel CMSSW_8_0_11
   $ cd CMSSW_8_0_11/src/
   $ cmsenv
   $ cd ~
   $ git clone https://github.com/ssghosh/Brown-FCNC2016-thcu.git
   $ source env.sh
```

For each fresh logon to LXPLUS, source the environment script:

```
   $ source env.sh
```

##Setup Area (LPC):

In order to run the analyzer on LPC, you must install the DAnalysis framework locally:

```
   $ cd ~
   $ mkdir DAnalysis_framework
   $ cd DAnalysis_framework
   $ cmsrel CMSSW_8_0_4
   $ scp <your_account>@lxplus6.cern.ch:/afs/cern.ch/user/j/jkiesele/public/DAnalysis_framework/v.1.1_rc2_special_d3.4/env_donttouch .
   $ rsync -rav <your_account>@lxplus6.cern.ch:/afs/cern.ch/user/j/jkiesele/public/DAnalysis_framework/v.1.1_rc2_special_d3.4/DAnalysis .
   $ rsync -rav <your_account>@lxplus6.cern.ch:/afs/cern.ch/user/j/jkiesele/public/DAnalysis_framework/v.1.1_rc2_special_d3.4/delphes .
   $ source env_donttouch
   $ cd delphes
   $ make clean
   $ make HAS_PYTHIA8=true
   $ make HAS_PYTHIA8=true DelphesPythia8
   $ cd ../DAnalysis
   $ source env.sh
   $ make
   $ make
```
Then,

```
   $ cd ~
   $ cmsrel CMSSW_8_0_11
   $ cd CMSSW_8_0_11/src
   $ cmsenv
   $ cd ~
   $ git clone https://github.com/ssghosh/Brown-FCNC2016-thcu.git
```

Before running ```source env.sh``` in this situation, you must open the file and change the variables DANALYSISPATH, LD_LIBRARY_PATH, and PATH to reflect the different home directory structure of LPC.

##Running Sample Generation:

Sample generation is performed with the DAnalysis framework (by Kieseler, J., Caillol, C., and E. Coleman). 

###Using LXBATCH



###Locally


##Running Analysis: 

The statistical analysis is performed using the Theta suite.

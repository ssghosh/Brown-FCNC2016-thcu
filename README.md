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
   $ git clone https://github.com/eacoleman/Brown-FCNC2016-thcu.git
   $ source env.sh
```

For each fresh logon to LXPLUS, source the environment script:

```
   $ source env.sh
```

##Running Sample Generation:

Sample generation is performed with the DAnalysis framework (by Kieseler, J., Caillol, C., and E. Coleman). 

###Using LXBATCH



###Locally


##Running Analysis: 

The statistical analysis is performed using the Theta suite.

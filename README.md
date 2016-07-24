# Brown-FCNC2016-thcu
Codebase for Brown's analysis of Flavor-Changing Neutral Currents: measuring the branching fraction for t->H(c/u) decays.

##Setup Area (LPC):
```
   $ cd ~/nobackup/
   $ git clone https://github.com/eacoleman/Brown-FCNC2016-thcu.git
   $ cmsrel CMSSW_8_0_4
   $ cd CMSSW_8_0_4/src/
   $ cmsenv

   $ git clone https://github.com/delphes/delphes 
   $ cd delphes
   $ git checkout tags/3.3.3pre11
   $ ./configure
   $ make -j 9
   $ make -j 9 # may have to run make twice because of DELPHES issues
```

##Running locally:

##Running via Condor 

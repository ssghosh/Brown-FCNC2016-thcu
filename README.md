# Brown-FCNC2016-thcu
Codebase for Brown's analysis of Flavor-Changing Neutral Currents: measuring the branching fraction for t->H(c/u) decays.

## Setup Area (LXPLUS):

Setting up only needs to be done once:

```
   $ cd ~
   $ cmsrel CMSSW_8_0_11
   $ cd CMSSW_8_0_11/src/
   $ cmsenv
   $ cd ~
   $ git clone https://github.com/ssghosh/Brown-FCNC2016-thcu.git
   $ cd Brown-FCNC2016-thcu/SAMPLES
```
Open the file ```env.sh``` and change the ```DANALYSISPATH```, ```LD_LIBRARY_PATH```, and ```PATH``` variables to reflect your home directory structure, DAnalysis installation path, and username. Then ```source env.sh``` to finish setting up the environment.

For each fresh logon to LXPLUS, run ```source env.sh``` before running anything in the repo.

## Setup Area (LPC):

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

Open the file ```env.sh``` and change the ```DANALYSISPATH```, ```LD_LIBRARY_PATH```, and ```PATH``` variables to reflect your home directory structure, DAnalysis installation path, and username. Then ```source env.sh``` to finish setting up the environment.

For each fresh logon to LPC, run ```source env.sh``` before running anything in the repo.

## Compiling and running the analyzer:
The analyzer is built using the DAnalysis framework (by Kieseler, J., Caillol, C., and E. Coleman). 

To compile and run the analyzer, do the following:
```
   $ cd <path_to_repo>/SAMPLES
   $ source env.sh
   $ rm preselection
   $ make preselection
   $ ./preselection config/<config_file>.txt
```
To run on signal samples, replace ```<config_file>``` with ```xrootd_signal```. To run on background samples, replace it with ```xrootd```. The existing config files in the directory provide examples of how to build your own if needed.
 
### Plotting analyzer results:
Use ```SAMPLES/plotter.py``` to plot histograms of interest that are in the output files from the analyzer. Open ```plotter.py``` in a text editor and change the settings in lines 29-68 as needed. This includes setting the directory in which the analyzer output files reside, the output directory for the plotter, the names of the signal and background analyzer output files, the names and titles of the background and signal histograms, and the names of the directories of the hct, hut, and background samples in the ROOT files.

After setting these values, simply run ```python plotter.py``` to produce the desired plots in the specified output directory.

## Running the statistical analysis:
You will need to create a master file containing both signal and background analyzer results. Edit ```SAMPLES/make_master_file.py``` to reflect the paths to your signal and background analyzer output files, then:
```
   $ python make_master_file.py
   $ cp master.root ../datacards/
   $ cd ../datacards
```

In order to run Higgs Combine, you will need CMSSW_7_4_7:

```
   $ cd ~
   $ cmsrel CMSSW_7_4_7
```

Then edit ```datacards/init.sh``` to reflect the path to your CMSSW_7_4_7 installation. Now:

```
   $ cd datacards
   $ source init.sh
   $ python makeDataCards.py
   $ ./run_analysis.sh
```
If you wish to change the statistical method used in the analysis, you can do so by editing ```run_analysis.sh```.

You can plot the results using ```datacards/plotting/PlotLimits.py```. Edit the value of ```lumi_dir_path``` in that file to point to your ```datacards``` directory and run with ```python PlotLimits.py```. 

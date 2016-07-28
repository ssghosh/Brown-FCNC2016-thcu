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

An example *selection* macro is available in `src/SelectorExample.C`. To run, use:

```
#   Argument 1: path to .txt list of input ntuples, or path to ntuples themselves
#            2: path to / filename of output sample (should have extension .root)
#            3: name of the tree to look for in the ntuples (should be "Delphes")
#            4: name of the selection to use (listed in src/Selections.h)
root -l -b -q src/SelectorExample.C'("OUTPUT_PATH/NAME.txt", "OUTPUT_PATH/NAME.root","TREE_TITLE","SELECTION")'
```

See `src/PlotExample.C` for a full plotting macro. It can be run with:

```
root -l -b -q src/PlotExample.C
```

A more comprehensive plotting script, complete with options, is available in `src/plotAll.C` .

##Running via Condor 

Running this analysis in Condor jobs is done on LPC with `python/SubmitCondor.py`:

```
python python/SubmitCondor.py --name  \ # name of output root files 
        --outdir       \ # absolute path to output directory 
        --sample_path  \ # absolute path to ntuples
        --nFiles       \ # number of output root files to produce 
        --selection    \ # name of the selection settings to use (see src/Selections.h) 
        --tree_name      # name of tree to use in ntuples (should be Delphes)
```

//root -l -q Delphes_Analysis/src/SelectorExample.C'("TChainArea/fileList.txt","out.root","Delphes")'

#include <iostream>
#include "Delphes_Analysis/interface/BrownDelphesSelector.h"

void SelectorExample(const char *inputFile, const char *outputFile, const char *treeName){   
    BrownDelphesSelector *Analysis = new BrownDelphesSelector;
    Analysis->Init(inputFile, outputFile, treeName, "SingleLepton");
    Analysis->Loop();
    
}

//root -l -q Delphes_Analysis/src/SelectorExample.C'("TChainArea/fileList.txt","out.root","Delphes")'

//#include <iostream>
#include "../interface/BrownDelphesSelector.h"

void SelectorExample(const char *inputFile, const char *outputFile, const char *treeName, const char *Selection="NoSelection"){
    BrownDelphesSelector *Analysis = new BrownDelphesSelector;
    Analysis->Init(inputFile, outputFile, treeName, Selection);
    Analysis->Loop();
    
}

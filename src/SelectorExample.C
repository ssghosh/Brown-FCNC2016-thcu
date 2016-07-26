//Usage: root -l -q Delphes_Analysis/src/SelectorExample.C'("TChainArea/fileList.txt","out.root","Delphes")'
#include "../interface/BrownDelphesSelector.h"

void SelectorExample(const char *inputFile,           // absolute input path to the selector (.txt,.root)
                     const char *outputFile,          // absolute path to output (.root) file 
                     const char *treeName ="Delphes", // input tree to use
                     const char *Selection="FCNC") {  // selection settings to use
    BrownDelphesSelector *Analysis = new BrownDelphesSelector;
    Analysis->Init(inputFile, outputFile, treeName, Selection);
    Analysis->Loop();
}

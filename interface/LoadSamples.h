#include <vector>
#include <map>

#include "TString.h"

map<TString, Sample> LoadSamples(TString type){
    map<TString, Sample> Samples;
    if(type == "SingleLepton"){
        Sample ttbar;
        ttbar.name = "TTbar";
        ttbar.path = "out.root";
        ttbar.type = "bkg";
        ttbar.color = 1;
        ttbar.xsec = 1.0;
        ttbar.genEvents = 37854.0;
        
        Samples[ttbar.name] = ttbar;
        
        Sample ttbar2;
        ttbar2.name = "TTbar2";
        ttbar2.path = "out.root";
        ttbar2.type = "bkg";
        ttbar2.color = 2;
        ttbar2.xsec = 2.0;
        ttbar2.genEvents = 37854.0;
        
        Samples[ttbar2.name] = ttbar2;
    }
    
    return Samples;
}
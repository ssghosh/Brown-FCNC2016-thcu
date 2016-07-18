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
    else if(type == "SnowMass_0PU"){
        Sample tt_0_600;
        tt_0_600.name = "tt_0_600";
        tt_0_600.path = "/eos/uscms/store/user/snowmass/agarabed/Test/tt-4p-0-600_0PU.root";
        tt_0_600.type = "bkg";
        tt_0_600.color = 1;
        tt_0_600.xsec = 530.89358;
        tt_0_600.genEvents = 6600943;
        Samples[tt_0_600.name] = tt_0_600;
        
        Sample tt_600_1100;
        tt_600_1100.name = "tt_600_1100";
        tt_600_1100.path = "/eos/uscms/store/user/snowmass/agarabed/Test/tt-4p-600-1100_0PU.root";
        tt_600_1100.type = "bkg";
        tt_600_1100.color = 2;
        tt_600_1100.xsec = 42.55351;
        tt_600_1100.genEvents = 4238879;
        Samples[tt_600_1100.name] = tt_600_1100;
        
        Sample tt_1100_1700;
        tt_1100_1700.name = "tt_1100_1700";
        tt_1100_1700.path = "/eos/uscms/store/user/snowmass/agarabed/Test/tt-4p-1100-1700_0PU.root";
        tt_1100_1700.type = "bkg";
        tt_1100_1700.color = 3;
        tt_1100_1700.xsec = 4.48209;
        tt_1100_1700.genEvents = 4148432;
        Samples[tt_1100_1700.name] = tt_1100_1700;
        
        Sample tt_1700_2500;
        tt_1700_2500.name = "tt_1700_2500";
        tt_1700_2500.path = "/eos/uscms/store/user/snowmass/agarabed/Test/tt-4p-1700-2500_0PU.root";
        tt_1700_2500.type = "bkg";
        tt_1700_2500.color = 4;
        tt_1700_2500.xsec = 0.52795;
        tt_1700_2500.genEvents = 4031722;
        Samples[tt_1700_2500.name] = tt_1700_2500;
        
        Sample tt_2500_100000;
        tt_2500_100000.name = "tt_2500_100000";
        tt_2500_100000.path = "/eos/uscms/store/user/snowmass/agarabed/Test/tt-4p-2500-100000_0PU.root";
        tt_2500_100000.type = "bkg";
        tt_2500_100000.color = 5;
        tt_2500_100000.xsec = 0.05449;
        tt_2500_100000.genEvents = 4238400;
        Samples[tt_2500_100000.name] = tt_2500_100000;
        
    }
    else if(type == "SnowMass_200PU"){
	Sample tt_0_600;
	tt_0_600.name = "tt_0_600";
	tt_0_600.path = "/eos/uscms/store/user/snowmass/agarabed/SnowMassSelection/tt-4p-0-600-v1510_14TEV_200PU.root";
	tt_0_600.type = "bkg";
	tt_0_600.color = 1;
	tt_0_600.xsec = 530.89358;
	tt_0_600.genEvents = 25050;
	Samples[tt_0_600.name] = tt_0_600;

	Sample tt_600_1100;
        tt_600_1100.name = "tt_600_1100";
        tt_600_1100.path = "/eos/uscms/store/user/snowmass/agarabed/SnowMassSelection/tt-4p-600-1100-v1510_14TEV_200PU.root";
        tt_600_1100.type = "bkg";
        tt_600_1100.color = 2;
        tt_600_1100.xsec = 42.55351;
        tt_600_1100.genEvents = 16166;
        Samples[tt_600_1100.name] = tt_600_1100;

	Sample tt_1100_1700;
        tt_1100_1700.name = "tt_1100_1700";
        tt_1100_1700.path = "/eos/uscms/store/user/snowmass/agarabed/SnowMassSelection/tt-4p-1100-1700-v1510_14TEV_200PU.root";
        tt_1100_1700.type = "bkg";
        tt_1100_1700.color = 3;
        tt_1100_1700.xsec = 4.48209;
        tt_1100_1700.genEvents = 16226;
        Samples[tt_1100_1700.name] = tt_1100_1700;

	Sample tt_1700_2500;
        tt_1700_2500.name = "tt_1700_2500";
        tt_1700_2500.path = "/eos/uscms/store/user/snowmass/agarabed/SnowMassSelection/tt-4p-1700-2500-v1510_14TEV_200PU.root";
        tt_1700_2500.type = "bkg";
        tt_1700_2500.color = 4;
        tt_1700_2500.xsec = 0.52795;
        tt_1700_2500.genEvents = 16226;
        Samples[tt_1700_2500.name] = tt_1700_2500;

	Sample tt_2500_100000;
        tt_2500_100000.name = "tt_2500_100000";
        tt_2500_100000.path = "/eos/uscms/store/user/snowmass/agarabed/SnowMassSelection/tt-4p-2500-100000-v1510_14TEV_200PU.root";
        tt_2500_100000.type = "bkg";
        tt_2500_100000.color = 5;
        tt_2500_100000.xsec = 0.05449;
        tt_2500_100000.genEvents = 16226;
        Samples[tt_2500_100000.name] = tt_2500_100000;

    }
    
    return Samples;

}

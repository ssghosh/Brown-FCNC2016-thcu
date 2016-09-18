
#!/bin/bash

export DISPLAY="0"
export PATH="/bin:/usr/bin:/usr/local/bin:/usr/krb5/bin:/usr/afsws/bin:/usr/krb5/bin/aklog"

cd CMSSWBASE/src/ 
source /cvmfs/cms.cern.ch/cmsset_default.sh
eval `scramv1 runtime -sh`
rehash

./preselection CONFIG

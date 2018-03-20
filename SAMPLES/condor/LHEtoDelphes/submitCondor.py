import os,datetime,time
from optparse import OptionParser
runDir=os.getcwd()

os.system('xrdcp -f root://cmseos.fnal.gov//store/user/snowmass/DelphesSubmissionLPCcondor/scripts/EOSSafeUtils.py '+runDir)
execfile(runDir+'/EOSSafeUtils.py')

start_time = time.time()

# get options
parser = OptionParser(description='Submit condor jobs to Delphes your signal samples.')
#parser.add_option('--numEvents',   action='store',  dest='numEvents',   default=50000,
#parser.add_option('--evPerJob',    action='store',  dest='evPerJob',    default=10000,
parser.add_option('--pileup',   
                    action='store',  
                    dest='pileup',   
                    default=200, 
                    help='how much pileup in each event')
parser.add_option('--inputDir',     
                    action='store',  
                    dest='inputDir',  
                    default='/eos/uscms/store/user/ssghosh/FCNC_signal_samples/', 
                    help='where the samples are stored, relative to the EOS root dir')
parser.add_option('--outDir',       
                    action='store',  
                    dest='outputDir', 
                    default='/eos/uscms/store/user/ssghosh/FCNC_signal_samples/DelphesFromLHE/', 
                    help='where to put the outputs, relative to the EOS root dir')
parser.add_option('--condorOutDir', 
                    action='store',  
                    dest='condorDir', 
                    default='/uscms_data/d3/ssghosh/DelphesFromLHE_Logs/', 
                    help='where to store the condor outputs, relative to root')
#parser.add_option('--anaSubLoc',  action='store', dest='anaSubLoc',  default="%s/anaSubs
#parser.add_option('--fastJetLoc', action='store', dest='fastJetLoc', default="%s/fastjet
#parser.add_option('--cfg',        action='store', dest='cfg',        default="rhe",   he
#parser.add_option('--indir',      action='store', dest='indir',      default="",    help

(options, args) = parser.parse_args()
#IO directories must be full paths
#pileup = str(sys.argv[1])

cTime=datetime.datetime.now()

inDir=cleanEOSpath(options.inputDir)
outDir=cleanEOSpath(options.outputDir)

print 'Getting proxy'
proxyPath=os.popen('voms-proxy-info -path')
proxyPath=proxyPath.readline().strip()

print 'Starting submission'
count=0

dirList = [  # CHOOSE SAMPLES
    'TT_topHadronicDecay_eta_hut_LO_14TEV' ]

for sample in dirList:
    os.system('eos root://cmseos.fnal.gov/ mkdir -p '+outDir+sample+'_'+options.pileup+'PU')
    os.system('eos root://cmseos.fnal.gov/ mkdir -p '+outDir+sample+'_'+options.pileup+'PU/metaData')
    os.system('mkdir -p '+options.condorDir+'/'+sample+'_'+options.pileup+'PU')
    relPath = sample
    #process = sample.split('-')[0]
    #if process == 'Bjj': process = 'Bjj-vbf'    

    xqcut = 40
    #if 't' in process: xqcut = 60
    #if 'tt' in process: xqcut = 80
    maxjets = 2
    #if process == 'BBB' or process == 'LLB' or process == 'ttB': maxjets = 1
    #if process == 'Bj' or process == 'Bjj-vbf' or process == 'H' or process =='tj': maxjets = 3

    qcut = xqcut*1.75 #guidance is between 1.5 and 2. Previously sliding scale 1.5 to 1.75, now use 1.75.

    #print 'Process '+process+', using xqcut '+str(xqcut)+' and max jets '+str(maxjets)

    lhefiles = EOSlist_lhe_files(options.inputDir+sample,None)
    if count+len(lhefiles) > 4000: 
        print 'SKIPPING '+sample+', more than 4K jobs'  # OPTIONAL TO SUBMIT IN CHUNKS
        continue

    tempcount = 0;
    for file in lhefiles:
        rawfile = file[:-7]

        count+=1
        tempcount+=1
        if tempcount > 1: continue   # OPTIONAL: RUN A 1 JOB TEST

        dict={'RUNDIR':runDir, 'RELPATH':relPath, 
              'PILEUP':options.pileup, 'QCUT':str(qcut), 
              'JETS':str(maxjets), 'INPUTDIR':inDir, 
              'FILENAME':rawfile, 'PROXY':proxyPath, 'OUTPUTDIR':outDir}
        jdfName=options.condorDir+'/%(RELPATH)s_%(PILEUP)sPU/%(FILENAME)s.jdl'%dict
        print jdfName
        jdf_current=open(jdfName,'w')
        jdf_tmpl=open('CondorConf.tmpl.condor')
        for line in jdf_tmpl:
            if 'PROXY' in line: line = line.replace('PROXY', dict['PROXY'])
            if 'RUNDIR' in line: line = line.replace('RUNDIR', dict['RUNDIR'])
            if 'FILENAME' in line: line = line.replace('FILENAME', dict['FILENAME'])
            if 'INPUTDIR' in line: line = line.replace('INPUTDIR', dict['INPUTDIR'])
            if 'RELPATH' in line: line = line.replace('RELPATH', dict['RELPATH'])
            if 'OUTPUTDIR' in line: line = line.replace('OUTPUTDIR', dict['OUTPUTDIR'])
            if 'PILEUP' in line: line = line.replace('PILEUP', dict['PILEUP'])
            if 'QCUT' in line: line = line.replace('QCUT', dict['QCUT'])
            if 'JETS' in line: line = line.replace('JETS', dict['JETS'])
            
            jdf_current.write(line)    

        jdf_current.close()
        jdf_tmpl.close()
        os.chdir('%s/%s_%sPU'%(options.condorDir,relPath,options.pileup))
        os.system('condor_submit %(FILENAME)s.jdl'%dict)
        os.system('sleep 0.5')                                
        os.chdir(runDir)
        print count, "jobs submitted!!!"

print("--- %s minutes ---" % (round(time.time() - start_time, 2)/60))

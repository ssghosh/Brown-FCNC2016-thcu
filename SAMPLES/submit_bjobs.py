import os,sys
from optparse import OptionParser
import math

pwd=os.environ['PWD']

# get options
parser = OptionParser(description='Submit bsubs for DAnalysis calls.')
parser.add_option('--indir',      action='store', dest='indir',      default="",    help='Input sample directory, complete with redirector')
parser.add_option('--outputdir',     action='store', dest='outputdir',     default="./",  help='Location of general output directory - each sample folder has an individual output dir within this')
parser.add_option('--lumi',   action='store', dest='lumi',   default=3000000, help='Luminosity in pb')
parser.add_option('--testmode', action='store', dest='testmode', default='true', help='If true only 1% of analysis will be performed.')
parser.add_option('--sampleinfo',  action='store', dest='sampleinfo',  default='', help='File containing comma separated properties of the sample: folder name, legend name, color, x-section, # of evts, legend order, and signal indicator')

(options, args) = parser.parse_args()
cmssw_base = os.environ['CMSSW_BASE']

# check that input directory is specified
if options.indir == "":
    print "ERROR: no --indir specified. Exiting..."
    quit()

# check that output directory exists
options.outdir = os.path.abspath(options.outdir)
if not os.path.exists(options.outdir):
    os.system('mkdir -p ' + options.outdir)

# prepare all configs
with open(sampleinfo) as samples:
   for config_line in samples:
       current_name = config_line.split(',')[0].strip('/')
       current_conf_name = 'batch_configs/' + current_name + '.conf'
       current_conf = open(current_conf_name,'w')
       current_script_name = 'batch_scripts/' + current_name + '.sh'
       current_shel = open(current_script_name,'w')

       # write analyzer config
       conf_tmpl = open('CondorConf.tmpl.condor')
       for line in conf_tmpl:
            if 'OUTPUTDIR'  in line: line = line.replace('OUTPUTDIR',  options.outputdir + current_name)       
            if 'OUTPUTFILE' in line: line = line.replace('OUTPUTFILE', current_name)
            if 'LUMINOSITY' in line: line = line.replace('LUMINOSITY', options.lumi)
            if 'TESTMODE' in line: line = line.replace('TESTMODE', options.testmode)  
            if 'SAMPLESDIR'       in line: line = line.replace('SAMPLESDIR',       options.indir)
            if 'INPUTFILE_INFO' in line: line = line.replace('INPUTFILE_INFO', config_line)

            current_conf.write(line)

       conf_tmpl.close()

       # write shell script
       shel_tmpl = open('CondorShel.tmpl.sh')
       for line in shel_tmpl:
           if 'CMSSWBASE' in line: line = line.replace('CMSSWBASE', cmssw_base)
           if 'CONFIG'  in line: line = line.replace('CONFIG',  current_conf_name)

           current_shel.write(line)

       shel_tmpl.close()

       current_conf.close()
       current_shel.close()

# run jobs
for i,j in [(i,j) for i in range(len(files)) for j in range(nFilesPerLHE)]:
    current_name = files[i].split('.')[0] + "_%i"%j
    os.chdir(options.outdir + '/')
    os.system('sh ' + current_name)

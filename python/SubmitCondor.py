import os,sys
import argparse
import math

parser = argparse.ArgumentParser(description='Submit condor jobs for selectors and plotting.')
parser.add_argument('--name')
parser.add_argument('--outdir')
parser.add_argument('--sample_path')
parser.add_argument('--nFiles')
parser.add_argument('--selection')
parser.add_argument('--tree_name')

args = parser.parse_args()
args.outdir = os.path.abspath(args.outdir)

if not os.path.exists(args.outdir):
    os.system('mkdir -p ' + args.outdir)


if not os.path.exists(args.sample_path):
    print "No Files Found. Quitting."
    exit()

cmssw_base = os.environ['CMSSW_BASE']

files = os.listdir(args.sample_path)
numjobs = int(math.ceil(float(len(files)) / float(args.nFiles)))

print len(files), numjobs
current = 0
index   = 0

for i in range(numjobs):
    current_name = args.outdir + '/' + args.name + '_' + str(i)
    current_list = open(current_name + '.txt','w')
    current_conf = open(current_name + '.condor','w')
    current_shel = open(current_name + '.sh','w')

    for j in range(int(args.nFiles)):

        file_no = i*10 + j
        if file_no >= int(len(files)): break

        if '.root' not in files[file_no]: continue
        toWrite = args.sample_path + '/' + files[file_no]+'\n'
        toWrite = 'root://cmseos.fnal.gov//' + toWrite[toWrite.find('store'):]
        current_list.write(toWrite+'\n')

    current_list.write('\n')

    conf_tmpl = open(cmssw_base + '/src/Brown-FCNC2016-thcu/condor/CondorConf.tmpl.condor')
    for line in conf_tmpl:
        if 'OUTPUT_PATH' in line: line = line.replace('OUTPUT_PATH', args.outdir)
        if 'NAME' in line: line = line.replace('NAME', args.name+'_'+str(i))

        current_conf.write(line)

    conf_tmpl.close()

    shel_tmpl = open(cmssw_base + '/src/Brown-FCNC2016-thcu/condor/CondorShel.tmpl.sh')
    for line in shel_tmpl:
        if 'OUTPUT_PATH' in line: line = line.replace('OUTPUT_PATH', args.outdir)
        if 'NAME' in line: line = line.replace('NAME', args.name+'_'+str(i))
        if 'TREE_TITLE' in line: line = line.replace('TREE_TITLE', args.tree_name)
        if 'SELECTION' in line: line = line.replace('SELECTION', args.selection)
        if 'CMSSWBASE' in line: line = line.replace('CMSSWBASE', cmssw_base)

        current_shel.write(line)

    shel_tmpl.close()

    current_list.close()
    current_conf.close()
    current_shel.close()


for i in range(numjobs):
    os.chdir(args.outdir + '/')
    os.system('condor_submit ' + args.name+'_'+str(i) + '.condor')


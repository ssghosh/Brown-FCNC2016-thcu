import os,sys
import argparse

parser = argparse.ArgumentParser(description='Process some integers.')
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
tmp_1 = len(files) / int(args.nFiles)
tmp_2 = len(files) % int(args.nFiles)

if tmp_2 != 0: tmp_1+=1

print len(files), tmp_1, tmp_2
current = 0
index = 0

for i in range(tmp_1):
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

    conf_tmpl = open(cmssw_base + '/src/Delphes_Analysis/condor/CondorConf.tmpl.condor')
    for line in conf_tmpl:
        if 'OUTPUT_PATH' in line: line = line.replace('OUTPUT_PATH', args.outdir)
        if 'NAME' in line: line = line.replace('NAME', args.name+'_'+str(i))

        current_conf.write(line)

    conf_tmpl.close()

    shel_tmpl = open(cmssw_base + '/src/Delphes_Analysis/condor/CondorShel.tmpl.sh')
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

    os.chdir(args.outdir + '/')
    os.system('condor_submit ' + args.name+'_'+str(i) + '.condor')


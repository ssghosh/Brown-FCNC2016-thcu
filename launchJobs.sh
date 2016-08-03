# look in this directory for folders corresponding to LHE filesets
snowmassdir=/eos/uscms/store/user/snowmass/DelphesFromLHE_2016June/

# scan through each folder in snowmassdir and run jobs for the LHE files 
for folder in ${snowmassdir}/*/ ; do
    dirs=(${folder//// })
    name="${dirs[@]: -1}"
    python python/SubmitCondor.py --name ${name} \
        --outdir /eos/uscms/store/user/${USER}/FCNC/samples/${name}/ \
        --sample_path ${folder}/ \
        --nFiles 2 \
        --tree_name "Delphes" \
        --selection "FCNC"
done

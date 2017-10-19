#!/bin/bash


for item in lumi{100,500,1000,1500,2000,2500,3000}
do
    cd $item
    combineCards.py ee4j=hut_ee4j.txt eegte5j=hut_eegte5j.txt em4j=hut_em4j.txt emgte5j=hut_emgte5j.txt lll=hut_lll.txt llt=hut_llt.txt mm4j=hut_mm4j.txt mmgte5j=hut_mmgte5j.txt > hut_combined.txt
    combineCards.py ee4j=hct_ee4j.txt eegte5j=hct_eegte5j.txt em4j=hct_em4j.txt emgte5j=hct_emgte5j.txt lll=hct_lll.txt llt=hct_llt.txt mm4j=hct_mm4j.txt mmgte5j=hct_mmgte5j.txt > hct_combined.txt
    combine -M Asymptotic hut_combined.txt | tee hut_asymp.out
    combine -M Asymptotic hct_combined.txt | tee hct_asymp.out
    #combine -M ProfileLikelihood --significance hut_combined.txt -t -1 --expectSignal=1 | tee hut_sig.txt
    #combine -M ProfileLikelihood --significance hct_combined.txt -t -1 --expectSignal=1 | tee hct_sig.txt
    echo
    echo "======= DONE WITH" $item "======="
    echo
    cd ..
done

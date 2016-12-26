#!/usr/bin/python

import os
from ROOT import TFile

bkgIn = TFile('full_outputs/bkg_output.root')
sigIn = TFile('full_outputs/signal_output.root')

histsInDirs = ['ee4j','eegte5j','em4j','emgte5j','mm4j','mmgte5j','lll','llt']

output = TFile('master.root','RECREATE')
for key in bkgIn.GetListOfKeys():
    name = key.GetName()
    for h in histsInDirs:
        hist = bkgIn.Get(name+'/'+h).Clone(h+'__'+name[2:])
        hist.Write()
for key in sigIn.GetListOfKeys():
    name = key.GetName()
    for h in histsInDirs:
        hist = sigIn.Get(name+'/'+h).Clone(h+'__'+name[2:])
        hist.Write()

data = bkgIn.Get(bkgIn.GetListOfKeys()[0].GetName()+'/'+'data_obs').Clone('data_obs')
for h in histsInDirs:
    data_h = data.Clone(h+'__data_obs')
    data_h.Write()
bkgIn.Close()
sigIn.Close()
output.Close()


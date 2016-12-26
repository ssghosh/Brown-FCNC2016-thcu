import CombineHarvester.CombineTools.ch as ch
from ROOT import TFile
import os,sys

cb = ch.CombineHarvester()
cb.SetVerbosity(3)

signal = 'TT_Tleptonic_eta_hut'
if len(sys.argv)>1: signal = sys.argv[1]

cats = [(0, 'ee4j'), 
		(1, 'eegte5j'), 
		(2, 'em4j'), 
		(3, 'emgte5j'),
		(4, 'mm4j'),
		(5, 'mmgte5j'),
		(6, 'lll'),
		(7, 'llt')]

obs_rates = {
    'ee4j': 0,
    'eegte5j': 0,
    'em4j': 0,
    'emgte5j': 0,
    'mm4j': 0,
    'mmgte5j': 0,
    'lll': 0,
    'llt': 0
}

cb.AddObservations( ['*'], [''], ['13TeV'], [''],             cats)
cb.AddProcesses(    ['*'], [''], ['13TeV'], [''], [signal],   cats, True)
cb.AddProcesses(    ['*'], [''], ['13TeV'], [''], ['B4p'],    cats, False)
cb.AddProcesses(    ['*'], [''], ['13TeV'], [''], ['BB4p'],   cats, False)
cb.AddProcesses(    ['*'], [''], ['13TeV'], [''], ['BBB4p'],  cats, False)
cb.AddProcesses(    ['*'], [''], ['13TeV'], [''], ['Bj4p'],   cats, False)
cb.AddProcesses(    ['*'], [''], ['13TeV'], [''], ['BjjVBF'], cats, False)
cb.AddProcesses(    ['*'], [''], ['13TeV'], [''], ['H4p'],    cats, False)
cb.AddProcesses(    ['*'], [''], ['13TeV'], [''], ['LL4p'],   cats, False)
cb.AddProcesses(    ['*'], [''], ['13TeV'], [''], ['LLB'],    cats, False)
cb.AddProcesses(    ['*'], [''], ['13TeV'], [''], ['tB4p'],   cats, False)
cb.AddProcesses(    ['*'], [''], ['13TeV'], [''], ['tj4p'],   cats, False)
cb.AddProcesses(    ['*'], [''], ['13TeV'], [''], ['tt4p'],   cats, False)
cb.AddProcesses(    ['*'], [''], ['13TeV'], [''], ['ttB4p'],  cats, False)

tfile = TFile(os.getcwd() + "/master.root")

cb.ForEachObs(lambda x: x.set_rate(obs_rates[x.bin()]))
cb.ForEachProc(lambda x: x.set_rate(tfile.Get(x.bin()+'__'+x.process()).Integral()))

cb.cp().bin_id([0,1]).AddSyst(cb, 'eerate', 'lnN', ch.SystMap()(1.05))
cb.cp().bin_id([2,3]).AddSyst(cb, 'emrate', 'lnN', ch.SystMap()(1.05))
cb.cp().bin_id([4,5]).AddSyst(cb, 'mmrate', 'lnN', ch.SystMap()(1.05))
cb.cp().bin_id([6]).AddSyst(cb, 'lllrate', 'lnN', ch.SystMap()(1.05))
cb.cp().bin_id([7]).AddSyst(cb, 'lltrate', 'lnN', ch.SystMap()(1.05))
cb.cp().bin_id(range(0,8)).AddSyst(cb, 'pileup', 'lnN', ch.SystMap()(1.05))
cb.cp().bin_id([0,1,2,3]).AddSyst(cb, 'elEscale', 'lnN', ch.SystMap()(1.05))
cb.cp().bin_id([2,3,4,5]).AddSyst(cb, 'muEscale', 'lnN', ch.SystMap()(1.05))
cb.cp().bin_id(range(0,8)).AddSyst(cb, 'jEscale', 'lnN', ch.SystMap()(1.05))
cb.cp().bin_id(range(0,8)).AddSyst(cb, 'jEres', 'lnN', ch.SystMap()(1.05))
cb.cp().bin_id(range(0,8)).AddSyst(cb, 'bID', 'lnN', ch.SystMap()(1.05))
cb.cp().bin_id(range(0,8)).AddSyst(cb, 'cID', 'lnN', ch.SystMap()(1.05))
cb.cp().bin_id(range(0,8)).AddSyst(cb, 'jID', 'lnN', ch.SystMap()(1.05))
cb.cp().bin_id(range(0,8)).AddSyst(cb, 'lumi', 'lnN', ch.SystMap()(1.05))

# dominant background cross section theoretical uncertainties
cb.cp().process(['B4p']).AddSyst(cb, 'B4pxs', 'lnN', ch.SystMap()(1.056)) # computed from w+jets, dy/z and gamma+jets in https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns
cb.cp().process(['Bj4p']).AddSyst(cb, 'Bj4pxs', 'lnN', ch.SystMap()(1.056)) # same as above
cb.cp().process(['BB4p']).AddSyst(cb, 'BB4pxs', 'lnN', ch.SystMap()(1.065)) # computed from WZ xs (arXiv:1607.06943) and ZZ xs (arXiv:1607.08834v2)
cb.cp().process(['tt4p']).AddSyst(cb, 'tt4pxs', 'lnN', ch.SystMap()(1.042)) # computed from ttbar in https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns
cb.cp().process(['tB4p']).AddSyst(cb, 'tB4pxs', 'lnN', ch.SystMap()(1.042)) # off-shell top pair, couldn't find a number so used the one for ttbar, above
cb.cp().process(['tj4p']).AddSyst(cb, 'tj4pxs', 'lnN', ch.SystMap()(1.06)) # computed from https://twiki.cern.ch/twiki/bin/viewauth/CMS/SingleTopSigma
cb.cp().process(['LL4p']).AddSyst(cb, 'LL4pxs', 'lnN', ch.SystMap()(1.056)) # off-shell boson, couldn't find a number so used the one for B4p, above

# subdominant backgrounds cross section theoretical uncertainties
cb.cp().process(['ttB4p']).AddSyst(cb, 'ttB4pxs', 'lnN', ch.SystMap()(1.0024)) # computed from TT+X in https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns
cb.cp().process(['LLB']).AddSyst(cb, 'LLBxs', 'lnN', ch.SystMap()(1.065)) # off-shell divector, couldn't find a number so used the one for BB-4p, above
cb.cp().process(['BBB4p']).AddSyst(cb, 'BBB4pxs', 'lnN', ch.SystMap()(1.065)) # trivector, no uncertainty given so used the one for BB-4p, above
cb.cp().process(['H4p']).AddSyst(cb, 'H4pxs', 'lnN', ch.SystMap()(1.147)) # computed from 14 TeV g-g fusion in https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt1314TeV2014
cb.cp().process(['BjjVBF']).AddSyst(cb, 'BjjVBFxs', 'lnN', ch.SystMap()(1.038)) # computed from 14 TeV VBF in https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt1314TeV2014

cb.PrintAll()
cb.WriteDatacard(signal+'.txt')


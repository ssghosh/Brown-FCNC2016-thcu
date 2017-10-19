import CombineHarvester.CombineTools.ch as ch
from ROOT import TFile
import os,sys

hut_sigs = ['TT_Tleptonic_eta_hut',
            'TT_aTleptonic_eta_hut',
            'TT_Thadronic_eta_hut',
            'TT_aThadronic_eta_hut']

hct_sigs = ['TT_Tleptonic_eta_hct',
            'TT_aTleptonic_eta_hct',
            'TT_Thadronic_eta_hct',
            'TT_aThadronic_eta_hct']

sig_types = {'hut': hut_sigs, 'hct': hct_sigs}

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

# what follows are numbers you may want to use in scaling/normalizing the counts

sig_xs = {
    'TT_aThadronic_eta_hct' : 65.65372,
    'TT_aThadronic_eta_hut' : 65.66524,
    'TT_aTleptonic_eta_hct' : 31.46717,
    'TT_aTleptonic_eta_hut' : 31.47269,
    'TT_Thadronic_eta_hct' : 65.65372,
    'TT_Thadronic_eta_hut' : 65.66524,
    'TT_Tleptonic_eta_hct' : 31.46717,
    'TT_Tleptonic_eta_hut' : 31.47269}

ttbar_xs = 984.5 #pb, from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/TtbarNNLO for 14 TeV
br_w_leptonic = 0.1086 # from http://pdg.lbl.gov/2014/listings/rpp2014-list-w-boson.pdf
br_w_hadronic = 0.6741 # ibid
hut_pw = 0.1904399 #GeV, from https://twiki.cern.ch/twiki/bin/view/CMS/TopFCNCxsection
hct_pw = 0.1904065 #GeV

# constructing some signal normalizations
# normalize everything to 1 pb:
sig_1pb_norm = {name : 1./sig_xs[name] for name in sig_xs} 
print sig_1pb_norm
# normalize everything to just the anomalous decay BR
sig_br_norm = {name : (ttbar_xs*br_w_leptonic)/sig_xs[name] if "leptonic" in name else (ttbar_xs*br_w_hadronic)/sig_xs[name] for name in sig_xs} 
print sig_br_norm
# normalize everything to the non-anomalous cross section, leaving anomalous decay as the free param
# the 2 in the numerator is there because analyzer used xs's for one top and we needed to account for 2
sig_anom_norm = {name : 2./hut_pw if "hut" in name else 2./hct_pw for name in sig_xs}

# want a list of the lumis we're interested in scanning through
# since the nominal lumi in the analyser is 3000 fb^-1 these scalings
# will all be < 1
nominal_lumi = 3000. #fb^-1
lumi_setpoints = [35., 100., 500., 1000., 1500., 2000., 2500., 3000.]

# inputs: output_dir, a string with a path to which the datacards will be written
#         sig_channel_normalizations, a dict with strings of the names of the signal
#           channels as keys and floats (multiplicative factors) as values. These 
#           values only affect the signal counts.
#         scaling, a float which is applied multiplicatively to all counts
# output: A set of datacards in output_dir
def produce_cards(output_dir, scaling, sig_channel_normalizations=None):
    for sig_type in sig_types:
        for cat in cats:
            cb = ch.CombineHarvester()
            cb.SetVerbosity(0)
            cb.AddObservations( ['*'], [''], ['14TeV'], [''],             [cat])
            for sig in sig_types[sig_type]:
                cb.AddProcesses(    ['*'], [''], ['14TeV'], [''], [sig],   [cat], True)
            cb.AddProcesses(    ['*'], [''], ['14TeV'], [''], ['B4p'],    [cat], False)
            cb.AddProcesses(    ['*'], [''], ['14TeV'], [''], ['BB4p'],   [cat], False)
            cb.AddProcesses(    ['*'], [''], ['14TeV'], [''], ['BBB4p'],  [cat], False)
            cb.AddProcesses(    ['*'], [''], ['14TeV'], [''], ['Bj4p'],   [cat], False)
            cb.AddProcesses(    ['*'], [''], ['14TeV'], [''], ['BjjVBF'], [cat], False)
            cb.AddProcesses(    ['*'], [''], ['14TeV'], [''], ['H4p'],    [cat], False)
            cb.AddProcesses(    ['*'], [''], ['14TeV'], [''], ['LL4p'],   [cat], False)
            cb.AddProcesses(    ['*'], [''], ['14TeV'], [''], ['LLB'],    [cat], False)
            cb.AddProcesses(    ['*'], [''], ['14TeV'], [''], ['tB4p'],   [cat], False)
            cb.AddProcesses(    ['*'], [''], ['14TeV'], [''], ['tj4p'],   [cat], False)
            cb.AddProcesses(    ['*'], [''], ['14TeV'], [''], ['tt4p'],   [cat], False)
            cb.AddProcesses(    ['*'], [''], ['14TeV'], [''], ['ttB4p'],  [cat], False)

            tfile = TFile(os.getcwd() + "/master.root")
            print("opened master.root")

            def set_rates(x):
                print "setting rate for", x.bin()+'__'+x.process() + "..."
                nominal = tfile.Get(x.bin()+'__'+x.process()).Integral() 
                if x.process() in sig_types[sig_type] and sig_channel_normalizations:
                    scaled = nominal * sig_channel_normalizations[x.process()] * scaling
                else:
                    scaled = nominal * scaling
                x.set_rate(scaled)

            cb.ForEachObs(lambda x: x.set_rate(obs_rates[x.bin()]))
            cb.ForEachProc(set_rates)


            bin_id = [cat[0]]
            if bin_id == 0 or bin_id == 1:
                cb.cp().bin_id(bin_id).AddSyst(cb, 'elEscale', 'lnN', ch.SystMap()(1.05))  
                cb.cp().bin_id(bin_id).AddSyst(cb, 'eerate', 'lnN', ch.SystMap()(1.05))
            if bin_id == 2 or bin_id == 3:
                cb.cp().bin_id(bin_id).AddSyst(cb, 'elEscale', 'lnN', ch.SystMap()(1.05))
                cb.cp().bin_id([bin_id]).AddSyst(cb, 'emrate', 'lnN', ch.SystMap()(1.05))
            if bin_id == 4 or bin_id == 5:
                cb.cp().bin_id(bin_id).AddSyst(cb, 'mmrate', 'lnN', ch.SystMap()(1.05))
            if bin_id == 6:
                cb.cp().bin_id(bin_id).AddSyst(cb, 'lllrate', 'lnN', ch.SystMap()(1.05))
            if bin_id == 7:
                cb.cp().bin_id(bin_id).AddSyst(cb, 'lltrate', 'lnN', ch.SystMap()(1.05))
            cb.cp().bin_id(bin_id).AddSyst(cb, 'leptonID', 'lnN', ch.SystMap()(1.01))
            cb.cp().bin_id(bin_id).AddSyst(cb, 'pileup', 'lnN', ch.SystMap()(1.05))
            cb.cp().bin_id(bin_id).AddSyst(cb, 'jEscale', 'lnN', ch.SystMap()(1.01))
            cb.cp().bin_id(bin_id).AddSyst(cb, 'jEres', 'lnN', ch.SystMap()(1.05))
            cb.cp().bin_id(bin_id).AddSyst(cb, 'btag', 'lnN', ch.SystMap()(1.01))
            cb.cp().bin_id(bin_id).AddSyst(cb, 'cID', 'lnN', ch.SystMap()(1.05))
            cb.cp().bin_id(bin_id).AddSyst(cb, 'jID', 'lnN', ch.SystMap()(1.05))
            cb.cp().bin_id(bin_id).AddSyst(cb, 'lumi', 'lnN', ch.SystMap()(1.015))

            ## dominant background cross section theoretical uncertainties
            cb.cp().process(['B4p']).AddSyst(cb, 'B4pxs', 'lnN', ch.SystMap()(1.056)) # computed from w+jets, dy/z and gamma+jets in https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns
            cb.cp().process(['Bj4p']).AddSyst(cb, 'Bj4pxs', 'lnN', ch.SystMap()(1.056)) # same as above
            cb.cp().process(['BB4p']).AddSyst(cb, 'BB4pxs', 'lnN', ch.SystMap()(1.065)) # computed from WZ xs (arXiv:1607.06943) and ZZ xs (arXiv:1607.08834v2)
            cb.cp().process(['tt4p']).AddSyst(cb, 'tt4pxs', 'lnN', ch.SystMap()(1.042)) # computed from ttbar in https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns
            cb.cp().process(['tB4p']).AddSyst(cb, 'tB4pxs', 'lnN', ch.SystMap()(1.042)) # off-shell top pair, couldn't find a number so used the one for ttbar, above
            cb.cp().process(['tj4p']).AddSyst(cb, 'tj4pxs', 'lnN', ch.SystMap()(1.06)) # computed from https://twiki.cern.ch/twiki/bin/viewauth/CMS/SingleTopSigma
            cb.cp().process(['LL4p']).AddSyst(cb, 'LL4pxs', 'lnN', ch.SystMap()(1.056)) # off-shell boson, couldn't find a number so used the one for B4p, above

            ## subdominant backgrounds cross section theoretical uncertainties
            cb.cp().process(['ttB4p']).AddSyst(cb, 'ttB4pxs', 'lnN', ch.SystMap()(1.0024)) # computed from TT+X in https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns
            cb.cp().process(['LLB']).AddSyst(cb, 'LLBxs', 'lnN', ch.SystMap()(1.065)) # off-shell divector, couldn't find a number so used the one for BB-4p, above
            cb.cp().process(['BBB4p']).AddSyst(cb, 'BBB4pxs', 'lnN', ch.SystMap()(1.065)) # trivector, no uncertainty given so used the one for BB-4p, above
            cb.cp().process(['H4p']).AddSyst(cb, 'H4pxs', 'lnN', ch.SystMap()(1.147)) # computed from 14 TeV g-g fusion in https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt1314TeV2014
            cb.cp().process(['BjjVBF']).AddSyst(cb, 'BjjVBFxs', 'lnN', ch.SystMap()(1.038)) # computed from 14 TeV VBF in https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt1314TeV2014

            if not os.path.exists(output_dir):
                os.makedirs(output_dir)
            #cb.PrintAll()
            cb.WriteDatacard(output_dir + '/' + sig_type + '_' + cat[1]+'.txt')

for setpoint in lumi_setpoints:
    produce_cards("lumi"+str(int(setpoint)), setpoint/nominal_lumi, sig_anom_norm)

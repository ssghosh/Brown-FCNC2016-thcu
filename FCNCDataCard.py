#!/usr/bin/env python

import CombineHarvester.CombineTools.ch as ch
import os


def add_processes_and_observations(cb, prefix="x53x53right"):
    print '>> Creating processes and observations...'
    if chiral=='LH': prefix="x53x53left"
    for chn in chns:
        cats_chn = cats[chn]
        if 'isCR' not in chn: 
			cb.AddObservations(  ['*'],  [prefix], [era], [chn],                 cats_chn      )
			cb.AddProcesses(     ['*'],  [prefix], [era], [chn], bkg_procs[chn], cats_chn, False  )
			cb.AddProcesses(     masses, [prefix], [era], [chn], sig_procs,      cats_chn, True   )
        else: 
			cb.AddObservations(  ['all'],  [prefix], [era], [chn],                 cats_chn      )
			cb.AddProcesses(     ['all'],  [prefix], [era], [chn], bkg_procs[chn], cats_chn, False  )



def rename_and_write(cb):
    print '>> Setting standardised bin names...'
    ch.SetStandardBinNames(cb)

    writer = ch.CardWriter('LIMITS_'+chiral+'/$TAG/$MASS/$ANALYSIS_$CHANNEL_$BINID_$ERA.txt',
                           'LIMITS_'+chiral+'/$TAG/common/$ANALYSIS_$CHANNEL.input.root')

    writer.SetVerbosity(1)
    writer.WriteCards('cmb', cb)
    for chn in chns:
        print chn
        writer.WriteCards(chn, cb.cp().channel([chn]))
    print '>> Done!'


def print_cb(cb):
    for s in ["Obs", "Procs", "Systs", "Params"]:
        print "* %s *" % s
        getattr(cb, "Print%s" % s)()
        print


def add_systematics(cb):
    print '>> Adding systematic uncertainties...'

    signal = cb.cp().signals().process_set()
    bkg = ['ewk', 'top', 'qcd']
    bkgNoQCD = ['ewk', 'top']

    cb.cp().process(signal + bkg).channel(['isSR_isE','isSR_isM','isCR_isE','isCR_isM']).AddSyst(cb, "lumi", "lnN", ch.SystMap()(1.062))
    cb.cp().process(signal + bkg).channel(['isSR_isE','isCR_isE']).AddSyst(cb, "elTrigSys", "lnN", ch.SystMap()(1.03))
    cb.cp().process(signal + bkg).channel(['isSR_isM','isCR_isM']).AddSyst(cb, "muTrigSys", "lnN", ch.SystMap()(1.011))
    cb.cp().process(signal + bkg).channel(['isSR_isE','isCR_isE']).AddSyst(cb, "elIdSys", "lnN", ch.SystMap()(1.01))
    cb.cp().process(signal + bkg).channel(['isSR_isM','isCR_isM']).AddSyst(cb, "muIdSys", "lnN", ch.SystMap()(1.011))
    cb.cp().process(signal + bkg).channel(['isSR_isE','isCR_isE']).AddSyst(cb, "elIsoSys", "lnN", ch.SystMap()(1.01))
    cb.cp().process(signal + bkg).channel(['isSR_isM','isCR_isM']).AddSyst(cb, "muIsoSys", "lnN", ch.SystMap()(1.03))
    cb.cp().process(signal + bkg).channel(['isSR_isE','isSR_isM']).AddSyst(cb, "trigeff", "shape", ch.SystMap()(1.0))
    cb.cp().process(signal + bkg).channel(['isSR_isE','isSR_isM']).AddSyst(cb, "pileup", "shape", ch.SystMap()(1.0))
    cb.cp().process(signal + bkg).channel(['isSR_isE','isSR_isM']).AddSyst(cb, "jec", "shape", ch.SystMap()(1.0))
    cb.cp().process(signal + bkgNoQCD).channel(['isSR_isE','isSR_isM']).AddSyst(cb, "jer", "shape", ch.SystMap()(1.0))
    cb.cp().process(signal + bkgNoQCD).channel(['isSR_isE','isSR_isM']).AddSyst(cb, "btag", "shape", ch.SystMap()(1.0))
    cb.cp().process(signal + bkgNoQCD).channel(['isSR_isE','isSR_isM']).AddSyst(cb, "mistag", "shape", ch.SystMap()(1.0))
    cb.cp().process(signal + bkgNoQCD).channel(['isSR_isE','isSR_isM']).AddSyst(cb, "tau21", "shape", ch.SystMap()(1.0))
    cb.cp().process(signal + bkgNoQCD).channel(['isSR_isE','isSR_isM']).AddSyst(cb, "topsf", "shape", ch.SystMap()(1.0))
    cb.cp().process(['top']).channel(['isSR_isE','isSR_isM']).AddSyst(cb, "toppt", "shape", ch.SystMap()(1.0))
    cb.cp().process(['top']).channel(['isSR_isE','isSR_isM']).AddSyst(cb, "q2", "shape", ch.SystMap()(1.0))
    cb.cp().process(signal + bkg).channel(['isSR_isE','isSR_isM','isCR_isE','isCR_isM']).AddSyst(cb, "muRFcorrdNew", "shape", ch.SystMap()(1.0))
    cb.cp().process(signal + bkg).channel(['isSR_isE','isSR_isM','isCR_isE','isCR_isM']).AddSyst(cb, "pdfNew", "shape", ch.SystMap()(1.0))
    cb.cp().process(['ewk']).channel(['isSR_isE','isCR_isE']).AddSyst(cb, "ewk_SF_el", "lnU", ch.SystMap()(100.0))
    cb.cp().process(['ewk']).channel(['isSR_isM','isCR_isM']).AddSyst(cb, "ewk_SF_mu", "lnU", ch.SystMap()(100.0))
    cb.cp().process(['top']).channel(['isSR_isE','isCR_isE']).AddSyst(cb, "top_SF_el", "lnU", ch.SystMap()(100.0))
    cb.cp().process(['top']).channel(['isSR_isM','isCR_isM']).AddSyst(cb, "top_SF_mu", "lnU", ch.SystMap()(100.0))


def go(cb):
    add_processes_and_observations(cb)
    add_systematics(cb)


    rename_and_write(cb)
    # print_cb(cb)


if __name__ == "__main__":
    cb = ch.CombineHarvester()
    # cb.SetVerbosity(20)

    era = '13TeV'
    chiral = 'RH'

    bkg_procs = ['B4p', 'BB4p', 'BBB4p', 'Bj4p', 'BjjVBF',
                  'H4p', 'LL4p', 'LLB', 'tB4p', 'tj4p',
                  'tt4p', 'ttB4p']
                 
    cats = ['ee4j', 'eegte5j', 'em4j', 'emgte5j', 'mm4j', 
            'mmgte5j', 'lll', 'llt']
	   
    signal_procs = [ 'TT_Tleptonic_eta_hut',
                     'TT_Tleptonic_eta_hct',
                     'TT_Thadronic_eta_hut',
                     'TT_aTleptonic_eta_hut',
                     'TT_aTleptonic_eta_hct',
                     'TT_aThadronic_eta_hut',
                     'TT_aThadronic_eta_hct']

    masses = ch.ValsFromRange('700:1600|100')
    go(cb)


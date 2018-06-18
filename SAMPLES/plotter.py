import ROOT as rt
import phase2tdrStyle

rt.gROOT.SetBatch(True)
# create color palette
colors8 = []
colors8.append(rt.TColor.GetColor(127,201,127)) # light green
colors8.append(rt.TColor.GetColor(190,174,212)) # purple
colors8.append(rt.TColor.GetColor(253,192,134)) # light orange
colors8.append(rt.TColor.GetColor(255,255,153)) # yellow
colors8.append(rt.TColor.GetColor(56,108,176))  # dark blue
colors8.append(rt.TColor.GetColor(240,2,127))   # magenta
colors8.append(rt.TColor.GetColor(191,91,23))   # brown
colors8.append(rt.TColor.GetColor(102,102,102)) # charcoal
colors12 = []
colors12.append(rt.TColor.GetColor(166,206,227)) # light blue
colors12.append(rt.TColor.GetColor(31,120,180)) # dark blue
colors12.append(rt.TColor.GetColor(178,223,138)) # light green
colors12.append(rt.TColor.GetColor(51,160,44)) # dark green
colors12.append(rt.TColor.GetColor(251,154,153)) # light red
colors12.append(rt.TColor.GetColor(227,26,28)) # dark red
colors12.append(rt.TColor.GetColor(253,191,111)) # light orange
colors12.append(rt.TColor.GetColor(255,127,0)) # dark orange
colors12.append(rt.TColor.GetColor(202,178,214)) # light purple
colors12.append(rt.TColor.GetColor(106,61,154)) # dark purple
colors12.append(rt.TColor.GetColor(255,255,153)) # yellow
colors12.append(rt.TColor.GetColor(177,89,40)) # brown

# user-specified settings
H = 600
W = 800
infiledir = 'full_outputs'
outdir = 'plotter_test'
siginfilename = 'signal_output_200PU.root'
bkginfilename = 'bkg_output.root'

# dict with hist names as keys for the title of each hist
bkg_hist_titles = {'lep_pt' : 'lepton pT',
                   'lep_eta' : 'lepton eta',
                   'bjet_sep12' : 'bjet 1 & 2 #Delta R'}
sig_hists = {'lep_pt' : 'lepton pT',
             'lep_eta' : 'lepton eta',
             'bjet_sep12' : 'bjet 1 & 2 #Delta R',
             'cutFlow_ee4j' : 'ee4j cut flow',
             'cutFlow_eegte5j' : 'eegte5j cut flow',
             'cutFlow_em4j' : 'em4j cut flow',
             'cutFlow_emgte5j' : 'emgte5j cut flow',
             'cutFlow_mm4j' : 'mm4j cut flow',
             'cutFlow_mmgte5j' : 'mmgte5j cut flow',
             'cutFlow_lll' : 'lll cut flow',
             'cutFlow_llt' : 'llt cut flow'}

hct_dirs = ['d_TT_aThadronic_eta_hct',
            'd_TT_Thadronic_eta_hct',
            'd_TT_aTleptonic_eta_hct',
            'd_TT_Tleptonic_eta_hct']

hut_dirs = ['d_TT_aThadronic_eta_hut',
            'd_TT_Thadronic_eta_hut',
            'd_TT_aTleptonic_eta_hut',
            'd_TT_Tleptonic_eta_hut']

bkg_dirs = ['d_B4p', 'd_BB4p',
            'd_BBB4p', 'd_Bj4p',
            'd_BjjVBF', 'd_H4p',
            'd_LL4p', 'd_LLB',
            'd_tB4p', 'd_tj4p',
            'd_tt4p', 'd_ttB4p']


siginfile = rt.TFile(infiledir+'/'+siginfilename)
bkginfile = rt.TFile(infiledir+'/'+bkginfilename)

# build dicts of the histograms from each directory
hct_hists = {name:[] for name in sig_hists}
hut_hists = {name:[] for name in sig_hists}
bkg_hists = {name:[] for name in bkg_hist_titles}
bkg_hist_dicts = {name:{} for name in bkg_hist_titles}
for key in siginfile.GetListOfKeys():
    name = key.GetName()
    if name in hct_dirs:
        for h in sig_hists.keys():
            hist = siginfile.Get(name+'/'+h)
            hct_hists[h].append(hist.Clone())
    elif name in hut_dirs:
        for h in sig_hists.keys():
            hist = siginfile.Get(name+'/'+h)
            hut_hists[h].append(hist.Clone())
    else:
        print ">>> not hct or hut, make sure that this is a signal file..."

for key in bkginfile.GetListOfKeys():
    name = key.GetName()
    if name in bkg_dirs:
        for h in bkg_hist_titles:
            hist = bkginfile.Get(name+'/'+h)
            bkg_hists[h].append(hist.Clone())
            bkg_hist_dicts[h][name[2:]] = hist.Clone()
    else:
        print ">>> Not in list of bkg dirs, make sure that this is a bkg file..."

#print hct_hists
#print hut_hists

# add the histograms
added_hct_hists = {}
added_hut_hists = {}
added_bkg_hists = {}
total_added_bkg_hists = {}
if all([item for item in hct_hists.values()]):
    for key in hct_hists.keys():
        hist1 = hct_hists[key][0].Clone()
        for hist2 in hct_hists[key][1:]:
            hist1.Add(hist2)
        hist1.SetTitle("Hct " + sig_hists[key])
        hist1.SetLineColor(1)
        hist1.SetLineStyle(7)
        hist1.SetLineWidth(2)
        # normalize:
        hist1.Scale(1./hist1.Integral())
        print "the type of hist1 is", type(hist1)
        print "is instance of TH1F?", isinstance(hist1,rt.TH1F)
        added_hct_hists[key] = hist1

if all([item for item in hut_hists.values()]):
    for key in hut_hists.keys():
        hist1 = hut_hists[key][0].Clone()
        for hist2 in hut_hists[key][1:]:
            hist1.Add(hist2)
        hist1.SetTitle("Hut " + sig_hists[key])
        hist1.SetLineColor(1)
        hist1.SetLineStyle(7)
        hist1.SetLineWidth(2)
        # normalize:
        hist1.Scale(1./hist1.Integral())
        added_hut_hists[key] = hist1
if all([item for item in bkg_hists.values()]):
    for key in bkg_hists.keys():
        hist1 = bkg_hists[key][0].Clone()
        for hist2 in bkg_hists[key][1:]:
            hist1.Add(hist2)
        hist1.SetTitle("Background " + bkg_hist_titles[key])
        # normalize:
        #print ">>> bkg hist integral was " + str(hist1.Integral())
        total_added_bkg_hists[key] = hist1.Clone()
        hist1.Scale(1./hist1.Integral())
        #print ">>> bkg hist integral is now " + str(hist1.Integral())
        added_bkg_hists[key] = hist1.Clone()

# create a background histogram stack
bkg_hist_stacks = {}
bkg_hist_legends = {}
if all([item for item in bkg_hists.values()]):
    for key in bkg_hists.keys():
        bkg_hist_legends[key] = rt.TLegend(0.75,0.2,1,1)
        # get the normalization
        norm = 1./total_added_bkg_hists[key].Integral()
        print "normalization is", str(norm)
        hist0 = bkg_hist_dicts[key][bkg_hist_dicts[key].keys()[0]]
        hist0_binwidth = hist0.GetXaxis().GetBinWidth(1)
        hist0_binwidth_str = "{:.2f}".format(hist0_binwidth)
        hist0_xtitle = hist0.GetXaxis().GetTitle()
        hist0_ytitle = hist0.GetYaxis().GetTitle()
        stack = rt.THStack(key,bkg_hist_titles[key]+";"+hist0_xtitle+";"
                +hist0_ytitle+" / "+hist0_binwidth_str)
        accumulator = 0.
        for idx, name in enumerate(bkg_hist_dicts[key].keys()):
            hist = bkg_hist_dicts[key][name].Clone()
            print "before norm hist integral was", str(hist.Integral())
            hist.SetFillColor(colors12[idx])
            bkg_hist_legends[key].AddEntry(hist,name,"f")
            hist.SetFillStyle(1001)
            hist.SetMarkerStyle()
            hist.SetMarkerColor(1)
            hist.SetLineColor(1)
            hist.Scale(norm)
            print "after norm hist integral was", str(hist.Integral())
            accumulator += hist.Integral()
            stack.Add(hist,"HIST")
        print "total stack integral should be", str(accumulator)
        print "the type of stack is", type(stack)
        print "is instance of thstack?", isinstance(stack,rt.THStack)
        #stack.Scale(1./added_bkg_hists[key].Integral())
        bkg_hist_stacks[key] = stack

# plot and output them
for key in added_hct_hists.keys():
    canvas, unc, aratios = phase2tdrStyle.draw([added_hct_hists[key]],True,False,True)
    phase2tdrStyle.drawCMS()
    phase2tdrStyle.drawEnPu()
    canvas.Print(outdir+'/hct_'+key+'.png')
for key in added_hut_hists.keys():
    canvas, unc, aratios = phase2tdrStyle.draw([added_hut_hists[key]],True,False,True)
    phase2tdrStyle.drawCMS()
    phase2tdrStyle.drawEnPu()
    canvas.Print(outdir+'/hut_'+key+'.png')
for key in added_bkg_hists.keys():
    canvas, unc, aratios = phase2tdrStyle.draw([added_bkg_hists[key]],True,False,False)
    phase2tdrStyle.drawCMS()
    phase2tdrStyle.drawEnPu()
    canvas.Print(outdir+'/bkg_'+key+'.png')

# plot hut and hct over bkg stacks (common hists only!)
# first we get a list of common keys between bkg_hist_titles and sig_hists:
common_keys = bkg_hist_titles.viewkeys() & sig_hists.viewkeys()
common_keys = list(common_keys)
common_hist_titles = {x : bkg_hist_titles[x] for x in common_keys}
# then loop over the keys in common_hist_titles to get and plot the hists
for key in common_hist_titles.keys():
    canvas, unc, aratios = phase2tdrStyle.draw([bkg_hist_stacks[key],added_hct_hists[key]],
            True,False,False)
    phase2tdrStyle.drawCMS()
    phase2tdrStyle.drawEnPu()
    bkg_hist_legends[key].Draw()
    canvas.Print(outdir+'/'+key+'_stack_hct.png')
for key in common_hist_titles.keys():
    canvas, unc, aratios = phase2tdrStyle.draw([bkg_hist_stacks[key],added_hut_hists[key]],
            True,False,False)
    phase2tdrStyle.drawCMS()
    phase2tdrStyle.drawEnPu()
    bkg_hist_legends[key].Draw()
    canvas.Print(outdir+'/'+key+'_stack_hut.png')


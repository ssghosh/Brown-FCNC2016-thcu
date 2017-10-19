import ROOT as rt

H = 600
W = 800
infiledir = 'full_outputs'
infilename = 'signal_output.root'
hists = ['lep_pt','lep_eta']
outdir = 'plotter_test'

hct_dirs = ['d_TT_aThadronic_eta_hct',
            'd_TT_Thadronic_eta_hct',
            'd_TT_aTleptonic_eta_hct',
            'd_TT_Tleptonic_eta_hct']

hut_dirs = ['d_TT_aThadronic_eta_hut',
            'd_TT_Thadronic_eta_hut',
            'd_TT_aTleptonic_eta_hut',
            'd_TT_Tleptonic_eta_hut']



infile = rt.TFile(infiledir+'/'+infilename)

# build dicts of the histograms from each directory
hct_hists = {name:[] for name in hists}
hut_hists = {name:[] for name in hists}
for key in infile.GetListOfKeys():
    name = key.GetName()
    if name in hct_dirs:
        for h in hists:
            hist = infile.Get(name+'/'+h)
            hct_hists[h].append(hist)
    elif name in hut_dirs:
        for h in hists:
            hist = infile.Get(name+'/'+h)
            hut_hists[h].append(hist)
    else:
        print ">>> dir name not recognized:", name

print hct_hists
print hut_hists

# add the histograms
added_hct_hists = {}
added_hut_hists = {}
for key in hct_hists.keys():
    hist1 = hct_hists[key][0]
    for hist2 in hct_hists[key][1:]:
        hist1.Add(hist2)
    added_hct_hists[key] = hist1
for key in hut_hists.keys():
    hist1 = hut_hists[key][0]
    for hist2 in hut_hists[key][1:]:
        hist1.Add(hist2)
    added_hut_hists[key] = hist1

print added_hct_hists
print added_hut_hists

# plot and output them
for key in added_hct_hists:
    c = rt.TCanvas("c1", "c1", 50, 50,W,H)
    added_hct_hists[key].SetStats(0)
    #added_hct_hists[key].SetFillColor(33)
    added_hct_hists[key].Draw("HIST")
    c.SaveAs(outdir+'/hct_'+key+'.png')
for key in added_hut_hists:
    c = rt.TCanvas("c1", "c1", 50, 50,W,H)
    #added_hut_hists[key].SetFillColor(33)
    added_hut_hists[key].SetStats(0)
    added_hut_hists[key].Draw("HIST")
    c.SaveAs(outdir+'/hut_'+key+'.png')

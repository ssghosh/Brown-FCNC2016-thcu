import ROOT as rt
from ROOT import TFile, TGraph, TCanvas
from array import array
import os,sys,math
from math import *
parent = os.path.dirname(os.getcwd())
sys.path.append(parent)
import CMS_lumi, tdrStyle
import re

rt.gROOT.SetBatch(1)

from tdrStyle import *
setTDRStyle()

#blind=False
#saveKey=''
#signal = 'X53'
#lumiPlot = '35.9'
#lumiStr = '35p867'
#
#mass_str = ['800','900','1000','1100','1200','1300','1400','1500','1600']
#theory_xsec = [0.196,0.0903,0.0440,0.0224,0.0118,0.00639,0.00354,0.00200,0.001148,0.000666,0.000391][:len(mass_str)]#pb
#scale_up = [1.9,1.9,1.9,1.8,1.8,1.8,1.7,1.8,1.7,1.6,1.7,1.7][:len(mass_str)]#%
#scale_dn = [1.9,1.8,1.7,1.6,1.6,1.5,1.5,1.5,1.5,1.5,1.5,1.5][:len(mass_str)]#%
#pdf_up   = [3.7,3.9,4.1,4.4,4.7,5.1,5.6,6.1,6.7,7.0,8.0,9.0][:len(mass_str)]#%
#pdf_dn   = [3.6,3.7,3.9,4.0,4.2,4.5,4.8,5.2,5.6,6.1,6.6,7.2][:len(mass_str)]#%
#
#mass   =array('d', [float(item) for item in mass_str])
#masserr=array('d',[0 for i in range(len(mass))])
#exp   =array('d',[0 for i in range(len(mass))])
#experr=array('d',[0 for i in range(len(mass))])
#obs   =array('d',[0 for i in range(len(mass))])
#obserr=array('d',[0 for i in range(len(mass))]) 
#exp68H=array('d',[0 for i in range(len(mass))])
#exp68L=array('d',[0 for i in range(len(mass))])
#exp95H=array('d',[0 for i in range(len(mass))])
#exp95L=array('d',[0 for i in range(len(mass))])
#
#theory_xsec_up = [math.sqrt(scale**2+pdf**2)*xsec/100 for xsec,scale,pdf in zip(theory_xsec,scale_up,pdf_up)]
#theory_xsec_dn = [math.sqrt(scale**2+pdf**2)*xsec/100 for xsec,scale,pdf in zip(theory_xsec,scale_dn,pdf_dn)]
#
#theory_xsec_v    = rt.TVectorD(len(mass),array('d',theory_xsec))
#theory_xsec_up_v = rt.TVectorD(len(mass),array('d',theory_xsec_up))
#theory_xsec_dn_v = rt.TVectorD(len(mass),array('d',theory_xsec_dn))      
#
#theory_xsec_gr = rt.TGraphAsymmErrors(rt.TVectorD(len(mass),mass),theory_xsec_v,rt.TVectorD(len(mass),masserr),rt.TVectorD(len(mass),masserr),theory_xsec_dn_v,theory_xsec_up_v)
#theory_xsec_gr.SetFillStyle(3001)
#theory_xsec_gr.SetFillColor(rt.kRed)
#			   
#theory = rt.TGraph(len(mass))
#for i in range(len(mass)):
#	theory.SetPoint(i, mass[i], theory_xsec[i])
#
#def getSensitivity(index, exp):
#	a1=mass[index]-mass[index-1]
#	b1=mass[index]-mass[index-1]
#	c1=0
#	a2=exp[index]-exp[index-1]
#	b2=theory_xsec[index]-theory_xsec[index-1]
#	c2=theory_xsec[index-1]-exp[index-1]
#	s = (c1*b2-c2*b1)/(a1*b2-a2*b1)
#	t = (a1*c2-a2*c1)/(a1*b2-a2*b1)
#	return mass[index-1]+s*(mass[index]-mass[index-1]), exp[index-1]+s*(exp[index]-exp[index-1])
#
#set the tdr style
#tdrstyle.setTDRStyle()

#change the CMS_lumi variables (see CMS_lumi.py)
CMS_lumi.writeExtraText = 1
CMS_lumi.extraText = "Preliminary"
CMS_lumi.lumi_sqrtS = "14 TeV" # used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)

iPos = 11
if( iPos==0 ): CMS_lumi.relPosX = 0.12

H_ref = 600; 
W_ref = 800; 
W = W_ref
H  = H_ref

# 
# Simple example of macro: plot with CMS name and lumi text
#  (this script does not pretend to work in all configurations)
# iPeriod = 1*(0/1 7 TeV) + 2*(0/1 8 TeV)  + 4*(0/1 13 TeV) 
# For instance: 
#               iPeriod = 3 means: 7 TeV + 8 TeV
#               iPeriod = 7 means: 7 TeV + 8 TeV + 13 TeV 
#               iPeriod = 0 means: free form (uses lumi_sqrtS)
# Initiated by: Gautier Hamel de Monchenault (Saclay)
# Translated in Python by: Joshua Hardenbrook (Princeton)
# Updated by:   Dinko Ferencek (Rutgers)
#

iPeriod = 0

# references for T, B, L, R
T = 0.08*H_ref
B = 0.12*H_ref 
L = 0.12*W_ref
R = 0.04*W_ref

lumi_dirs = ['lumi100',
            'lumi500',
            'lumi1000',
            'lumi1500',
            'lumi2000',  
            'lumi2500',  
            'lumi3000']

lumis = array('d',[100., 500., 1000., 1500., 2000., 2500., 3000.])

lumi_dir_path = '/uscms_data/d3/ssghosh/CMSSW_8_0_11/src/Brown-FCNC2016-thcu/datacards/test'

hut_lims = array('d',[0 for i in range(len(lumis))])
hut_95H = array('d',[0 for i in range(len(lumis))])
hut_95L = array('d',[0 for i in range(len(lumis))])
hut_68H = array('d',[0 for i in range(len(lumis))])
hut_68L = array('d',[0 for i in range(len(lumis))])

hct_lims = array('d',[0 for i in range(len(lumis))])
hct_95H = array('d',[0 for i in range(len(lumis))])
hct_95L = array('d',[0 for i in range(len(lumis))])
hct_68H = array('d',[0 for i in range(len(lumis))])
hct_68L = array('d',[0 for i in range(len(lumis))])

limerrs = array('d',[0 for i in range(len(lumis))])
lumierrs = array('d',[0 for i in range(len(lumis))])

def read_file(filename):
    with open(filename,"r") as f:
        contents = f.readlines()
        result_idx = [idx for idx, line in enumerate(contents) if "mean" in line][0]
        band68_idx = result_idx + 2
        band95_idx = result_idx + 3

        p = re.compile('\d+\.\d+')
        result_list = p.findall(contents[result_idx])
        band68_list = p.findall(contents[band68_idx])
        band95_list = p.findall(contents[band95_idx])
        result_val = float(result_list[0])
        band68_vals = [float(item) for item in band68_list]
        band95_vals = [float(item) for item in band95_list]

        return result_val, band68_vals, band95_vals

def read_single_asymptotic_file(filename):
    with open(filename,"r") as f:
        contents = f.readlines()
        result_header_idx = [idx for idx, line in enumerate(contents) if u"-- Asymptotic --" in line.encode('utf-8')][0]
        result_idx = result_header_idx + 4
        band68_idxs = (result_header_idx + 3, result_header_idx + 5) 
        band95_idxs = (result_header_idx + 2, result_header_idx + 6)

        p = re.compile('\d\.\d+[^%]')
        result_list = p.findall(contents[result_idx])
        band68_list = [p.findall(contents[band68_idxs[0]])[0], p.findall(contents[band68_idxs[1]])[0]]
        band95_list = [p.findall(contents[band95_idxs[0]])[0], p.findall(contents[band95_idxs[1]])[0]]
        result_val = float(result_list[0])
        band68_vals = [float(item) for item in band68_list]
        band95_vals = [float(item) for item in band95_list]

        return result_val, band68_vals, band95_vals
        
for jentry, lumi_dir in enumerate(lumi_dirs):
    #hut_val, hut_band68, hut_band95 = read_file(lumi_dir_path+'/'+lumi_dir+'/hut.out')
    hut_val, hut_band68, hut_band95 = read_single_asymptotic_file(lumi_dir_path+'/'+lumi_dir+'/hut_asymp.out')
    hut_lims[jentry] = hut_val
    hut_68L[jentry] = hut_band68[0]
    hut_68H[jentry] = hut_band68[1]
    hut_95L[jentry] = hut_band95[0]
    hut_95H[jentry] = hut_band95[1]

    #hct_val, hct_band68, hct_band95 = read_file(lumi_dir_path+'/'+lumi_dir+'/hct.out')
    hct_val, hct_band68, hct_band95 = read_single_asymptotic_file(lumi_dir_path+'/'+lumi_dir+'/hct_asymp.out')
    hct_lims[jentry] = hct_val
    hct_68L[jentry] = hct_band68[0]
    hct_68H[jentry] = hct_band68[1]
    hct_95L[jentry] = hct_band95[0]
    hct_95H[jentry] = hct_band95[1]

hut_plot_filename = 'hut_single_asymp_graph.png'
hct_plot_filename = 'hct_single_asymp_graph.png'
# vectors to be used by both plots
lumisv = rt.TVectorD(len(lumis),lumis)
limerrsv = rt.TVectorD(len(limerrs),limerrs)
lumierrsv = rt.TVectorD(len(lumierrs),lumierrs)

# hut plot
hut_limsv = rt.TVectorD(len(hut_lims),hut_lims)
hut_68Lv = rt.TVectorD(len(hut_68L),hut_68L)
hut_68Hv = rt.TVectorD(len(hut_68H),hut_68H)
hut_95Lv = rt.TVectorD(len(hut_95L),hut_95L)
hut_95Hv = rt.TVectorD(len(hut_95H),hut_95H)

hut_expected = rt.TGraphAsymmErrors(lumisv,hut_limsv,lumierrsv,lumierrsv,limerrsv,limerrsv)
hut_expected.SetLineColor(rt.kBlue)
hut_expected.SetLineWidth(2)
hut_expected.SetLineStyle(2)
hut_expected68 = rt.TGraphAsymmErrors(lumisv,hut_limsv,lumierrsv,lumierrsv,hut_68Lv,hut_68Hv)
hut_expected68.SetFillColor(rt.kGreen+1)
hut_expected95 = rt.TGraphAsymmErrors(lumisv,hut_limsv,lumierrsv,lumierrsv,hut_95Lv,hut_95Hv)
hut_expected95.SetFillColor(rt.kOrange)

XaxisTitle = "Integrated Luminosity (fb^{-1})"
YaxisTitle = "Signal Strength (\sigma_{tuh}/\sigma_{nom}"

hut_canvas = rt.TCanvas("c1","c1",50,50,W,H)
hut_canvas.SetFillColor(0)
hut_canvas.SetBorderMode(0)
hut_canvas.SetFrameFillStyle(0)
hut_canvas.SetFrameBorderMode(0)
hut_canvas.SetLeftMargin( L/W )
hut_canvas.SetRightMargin( R/W )
hut_canvas.SetTopMargin( T/H )
hut_canvas.SetBottomMargin( B/H )
hut_canvas.SetTickx(0)
hut_canvas.SetTicky(0)

hut_expected95.Draw("a3")
hut_expected68.Draw("3same")
hut_expected.Draw("same")
CMS_lumi.CMS_lumi(hut_canvas, iPeriod, iPos)
hut_expected.GetXaxis().SetTitle(XaxisTitle)
hut_expected.GetYaxis().SetTitle(YaxisTitle)

hut_legend = rt.TLegend(.6,.75,.9,.89) # top right
hut_legend.AddEntry(hut_expected, "95%CL upper lim", "l")
hut_legend.AddEntry(hut_expected68, "68%", "f")
hut_legend.AddEntry(hut_expected95, "95%", "f")
hut_legend.SetShadowColor(0)
hut_legend.SetFillStyle(0)
hut_legend.SetBorderSize(0)
hut_legend.SetFillColor(0)
hut_legend.SetLineColor(0)
hut_legend.SetNColumns(1)
hut_legend.SetTextSize(0.05)
hut_legend.Draw()

hut_canvas.Update()
hut_canvas.RedrawAxis()

hut_canvas.SaveAs(hut_plot_filename)

# hct plot
hct_limsv = rt.TVectorD(len(hct_lims),hct_lims)
hct_68Lv = rt.TVectorD(len(hct_68L),hct_68L)
hct_68Hv = rt.TVectorD(len(hct_68H),hct_68H)
hct_95Lv = rt.TVectorD(len(hct_95L),hct_95L)
hct_95Hv = rt.TVectorD(len(hct_95H),hct_95H)

hct_expected = rt.TGraphAsymmErrors(lumisv,hct_limsv,lumierrsv,lumierrsv,limerrsv,limerrsv)
hct_expected.SetLineColor(rt.kBlue)
hct_expected.SetLineWidth(2)
hct_expected.SetLineStyle(2)
hct_expected68 = rt.TGraphAsymmErrors(lumisv,hct_limsv,lumierrsv,lumierrsv,hct_68Lv,hct_68Hv)
hct_expected68.SetFillColor(rt.kGreen+1)
hct_expected95 = rt.TGraphAsymmErrors(lumisv,hct_limsv,lumierrsv,lumierrsv,hct_95Lv,hct_95Hv)
hct_expected95.SetFillColor(rt.kOrange)

XaxisTitle = "Integrated Luminosity (fb^{-1})"
YaxisTitle = "Signal Strength (\sigma_{tuh}/\sigma_{nom}"

hct_canvas = rt.TCanvas("c2","c2",50,50,W,H)
hct_canvas.SetFillColor(0)
hct_canvas.SetBorderMode(0)
hct_canvas.SetFrameFillStyle(0)
hct_canvas.SetFrameBorderMode(0)
hct_canvas.SetLeftMargin( L/W )
hct_canvas.SetRightMargin( R/W )
hct_canvas.SetTopMargin( T/H )
hct_canvas.SetBottomMargin( B/H )
hct_canvas.SetTickx(0)
hct_canvas.SetTicky(0)

hct_expected95.Draw("a3")
hct_expected68.Draw("3same")
hct_expected.Draw("same")
CMS_lumi.CMS_lumi(hct_canvas, iPeriod, iPos)
hct_expected.GetXaxis().SetTitle(XaxisTitle)
hct_expected.GetYaxis().SetTitle(YaxisTitle)

hct_legend = rt.TLegend(.6,.75,.9,.89) # top right
hct_legend.AddEntry(hct_expected, "95%CL upper lim", "l")
hct_legend.AddEntry(hct_expected68, "68%", "f")
hct_legend.AddEntry(hct_expected95, "95%", "f")
hct_legend.SetShadowColor(0)
hct_legend.SetFillStyle(0)
hct_legend.SetBorderSize(0)
hct_legend.SetFillColor(0)
hct_legend.SetLineColor(0)
hct_legend.SetNColumns(1)
hct_legend.SetTextSize(0.05)
hct_legend.Draw()

hct_canvas.Update()
hct_canvas.RedrawAxis()

hct_canvas.SaveAs(hct_plot_filename)
    
#def PlotLimits(limitDir,limitFile,tempKey):
#    histPrefix=discriminant+'_'+str(lumiStr)+'fb'
#    ljust_i = 10
#    print
#    print 'mass'.ljust(ljust_i), 'observed'.ljust(ljust_i), 'expected'.ljust(ljust_i), '-2 Sigma'.ljust(ljust_i), '-1 Sigma'.ljust(ljust_i), '+1 Sigma'.ljust(ljust_i), '+2 Sigma'.ljust(ljust_i)
#    
#    limExpected = 700
#    limObserved = 700
#    for i in range(len(mass)):
#        lims = {}
#        
#        try:
#        	if blind: fobs = open(limitDir+cutString+limitFile.replace(signal+signal+'M700',signal+signal+'M'+mass_str[i]), 'rU')
#        	if not blind: fobs = open(limitDir+cutString+limitFile.replace(signal+signal+'M700',signal+signal+'M'+mass_str[i]).replace('expected','observed'), 'rU')
#        	linesObs = fobs.readlines()
#        	fobs.close()
#        	
#        	fexp = open(limitDir+cutString+limitFile.replace(signal+signal+'M700',signal+signal+'M'+mass_str[i]), 'rU')
#        	linesExp = fexp.readlines()
#        	fexp.close()
#        except: 
#        	print "SKIPPING SIGNAL: "+mass_str[i]
#        	continue
#        
#        lims[-1] = float(linesObs[1].strip().split()[1])*1e3
#        obs[i] = float(linesObs[1].strip().split()[1])
#        obserr[i] = 0
#        
#        lims[.5] = float(linesExp[1].strip().split()[1])*1e3
#        exp[i] = float(linesExp[1].strip().split()[1])
#        experr[i] = 0
#        lims[.16] = float(linesExp[1].strip().split()[4])*1e3
#        exp68L[i] = float(linesExp[1].strip().split()[4])
#        lims[.84] = float(linesExp[1].strip().split()[5])*1e3
#        exp68H[i] = float(linesExp[1].strip().split()[5])
#        lims[.025] = float(linesExp[1].strip().split()[2])*1e3
#        exp95L[i] = float(linesExp[1].strip().split()[2])
#        lims[.975] = float(linesExp[1].strip().split()[3])*1e3
#        exp95H[i] = float(linesExp[1].strip().split()[3])
#    
#        if i!=0:
#        	if(exp[i]>theory_xsec[i] and exp[i-1]<theory_xsec[i-1]) or (exp[i]<theory_xsec[i] and exp[i-1]>theory_xsec[i-1]):
#        		limExpected,ycross = getSensitivity(i,exp)
#        	if(obs[i]>theory_xsec[i] and obs[i-1]<theory_xsec[i-1]) or (obs[i]<theory_xsec[i] and obs[i-1]>theory_xsec[i-1]):
#        		limObserved,ycross = getSensitivity(i,obs)
#        		
#        exp95L[i]=(exp[i]-exp95L[i])
#        exp95H[i]=abs(exp[i]-exp95H[i])
#        exp68L[i]=(exp[i]-exp68L[i])
#        exp68H[i]=abs(exp[i]-exp68H[i])
#
#        round_i = 2
#        print str(int(mass[i])).ljust(ljust_i), '& '+str(round(lims[-1],round_i)).ljust(ljust_i), '& '+str(round(lims[.5],round_i)).ljust(ljust_i), '& '+str(round(lims[.025],round_i)).ljust(ljust_i), '& '+str(round(lims[.16],round_i)).ljust(ljust_i), '& '+str(round(lims[.84],round_i)).ljust(ljust_i), '& '+str(round(lims[.975],round_i)).ljust(ljust_i)+' \\\\'
#    print
#    signExp = "="
#    signObs = "="
#    if limExpected==700: signExp = "<"
#    if limObserved==700: signObs = "<"
#    print "Expected lower limit "+signExp,int(round(limExpected)),"GeV"
#    print "Observed lower limit "+signObs,int(round(limObserved)),"GeV"
#    print
#
#    massv = rt.TVectorD(len(mass),mass)
#    expv  = rt.TVectorD(len(mass),exp)
#    exp68Hv = rt.TVectorD(len(mass),exp68H)
#    exp68Lv = rt.TVectorD(len(mass),exp68L)
#    exp95Hv = rt.TVectorD(len(mass),exp95H)
#    exp95Lv = rt.TVectorD(len(mass),exp95L)
#
#    obsv = rt.TVectorD(len(mass),obs)
#    masserrv = rt.TVectorD(len(mass),masserr)
#    obserrv = rt.TVectorD(len(mass),obserr)
#    experrv = rt.TVectorD(len(mass),experr)       
#
#
#    observed = rt.TGraphAsymmErrors(massv,obsv,masserrv,masserrv,obserrv,obserrv)
#    observed.SetLineColor(rt.kBlack)
#    observed.SetLineWidth(2)
#    observed.SetMarkerStyle(20)
#    expected = rt.TGraphAsymmErrors(massv,expv,masserrv,masserrv,experrv,experrv)
#    expected.SetLineColor(rt.kBlue)
#    expected.SetLineWidth(2)
#    expected.SetLineStyle(2)
#    expected68 = rt.TGraphAsymmErrors(massv,expv,masserrv,masserrv,exp68Lv,exp68Hv)
#    expected68.SetFillColor(rt.kGreen+1)
#    expected95 = rt.TGraphAsymmErrors(massv,expv,masserrv,masserrv,exp95Lv,exp95Hv)
#    expected95.SetFillColor(rt.kOrange)
#
#    canvas = rt.TCanvas("c4","c4",50,50,W,H)
#    canvas.SetFillColor(0)
#    canvas.SetBorderMode(0)
#    canvas.SetFrameFillStyle(0)
#    canvas.SetFrameBorderMode(0)
#    canvas.SetLeftMargin( L/W )
#    canvas.SetRightMargin( R/W )
#    canvas.SetTopMargin( T/H )
#    canvas.SetBottomMargin( B/H )
#    canvas.SetTickx(0)
#    canvas.SetTicky(0)
#    canvas.SetLogy()
#
#    if signal=='X53':
#    	XaxisTitle = "X_{5/3} mass [GeV]"
#    	#YaxisTitle = "#sigma(_{}X_{5/3}#bar{X}_{5/3}) [pb] - "+chiral.replace('left','LH').replace('right','RH')
#    	YaxisTitle = "#sigma(_{}X_{5/3}#bar{X}_{5/3}) [pb]"
#    else:
#		XaxisTitle = signal+" mass [GeV]"
#		YaxisTitle = "#sigma("+signal+"#bar{"+signal+"}) [pb]"
#
#    expected95.Draw("a3")
#    expected95.GetYaxis().SetRangeUser(.001+.00001,1.45)
#    expected95.GetXaxis().SetRangeUser(mass[0],mass[-1])
#    expected95.GetXaxis().SetTitle(XaxisTitle)
#    expected95.GetYaxis().SetTitle(YaxisTitle)
#    expected95.GetYaxis().SetTitleOffset(1)
#		
#    expected68.Draw("3same")
#    expected.Draw("same")
#
#    if not blind: observed.Draw("cpsame")
#    theory_xsec_gr.SetLineColor(2)
#    theory_xsec_gr.SetLineStyle(1)
#    theory_xsec_gr.SetLineWidth(2)
#    theory_xsec_gr.Draw("3same") 
#    theory.SetLineColor(2)
#    theory.SetLineStyle(1)
#    theory.SetLineWidth(2)
#    theory.Draw("same")                                                             
#        
#    #draw the lumi text on the canvas
#
#    legend = rt.TLegend(.37,.69,.94,.89) # top right
#    if tempKey=='nB0': legend = rt.TLegend(.32,.42,.92,.62)
#    if not blind: legend.AddEntry(observed, "95% CL observed", "lp")
#    legend.AddEntry(expected68, "68% expected", "f")
#    legend.AddEntry(expected, "Median expected", "l")
#    legend.AddEntry(expected95, "95% expected", "f")
#    legend.AddEntry(theory_xsec_gr, "Signal cross section", "lf")
#
#    legend.SetShadowColor(0)
#    legend.SetFillStyle(0)
#    legend.SetBorderSize(0)
#    legend.SetFillColor(0)
#    legend.SetLineColor(0)
#    legend.SetNColumns(2)
#    legend.Draw()
#    
#    chiralText = rt.TLatex()
#    chiralText.SetNDC()
#    chiralText.SetTextSize(0.06)
#    chiralText.SetTextAlign(21) # align center
#    thetext = "X_{5/3}#bar{X}_{5/3} - "+chiral.replace('left','LH').replace('right','RH')
#    chiralText.DrawLatex(0.76, 0.56, thetext)
#    
#    canvas.cd()
#    canvas.Update()
#    canvas.RedrawAxis()
#    frame = canvas.GetFrame()
#    frame.Draw()
#
#    folder = '.'
#    outDir=folder+'/'+limitDir.split('/')[-3]+'plots'
#    if not os.path.exists(outDir): os.system('mkdir '+outDir)
#    plotName = 'LimitPlot_'+histPrefix+'_rebinned_stat'+str(binning).replace('.','p')+saveKey+'_'+tempKey
#    if blind: plotName+='_blind'
#    canvas.SaveAs(outDir+'/'+plotName+'.eps')
#    canvas.SaveAs(outDir+'/'+plotName+'.pdf')
#    canvas.SaveAs(outDir+'/'+plotName+'.png')
#    return int(round(limExpected)), int(round(limObserved))
#
#doBRScan = False
#BRs={}
#BRs['BW']=[0.50,0.0,0.0,0.0,0.0,0.0,0.0,0.2,0.2,0.2,0.2,0.2,0.4,0.4,0.4,0.4,0.6,0.6,0.6,0.8,0.8,1.0]
#BRs['TH']=[0.25,0.0,0.2,0.4,0.6,0.8,1.0,0.0,0.2,0.4,0.6,0.8,0.0,0.2,0.4,0.6,0.0,0.2,0.4,0.0,0.2,0.0]
#BRs['TZ']=[0.25,1.0,0.8,0.6,0.4,0.2,0.0,0.8,0.6,0.4,0.2,0.0,0.6,0.4,0.2,0.0,0.4,0.2,0.0,0.2,0.0,0.0]
#nBRconf=len(BRs['BW'])
#if not doBRScan: nBRconf=1
#
#iPlotList=['minMlb']#,'ST','HT','YLD']
#tempKeys = ['all']#,'isE','isM','nW0','nW1p','nB1','nB2p','nT0','nT1p']
#cutString=''
#dirs = {
#		'SymWin':'templates_60Wmass100_2017_2_12',
#		'M17WtSF':'templates_M17WtSF_2017_3_31',
#		'M17WtSF_SRpCR':'templates_M17WtSF_2017_3_31_SRpCR',
#		'M17WtSF_SRpCRsimul':'templates_M17WtSF_2017_3_31_SRpCR_simulfit',
#		}
#dirKeyList = ['M17WtSF_SRpCR']#,'M17WtSF','M17WtSF_SRpCRsimul']
#binnings = ['0p3']
#
#expLimsL = {}
#obsLimsL = {}
#expLimsR = {}
#obsLimsR = {}
#for discriminant in iPlotList:
#	for dirKey in dirKeyList:
#		dir = dirs[dirKey]
#		#cutString=dir
#		expLimsL[dirKey+discriminant] = {}
#		obsLimsL[dirKey+discriminant] = {}
#		expLimsR[dirKey+discriminant] = {}
#		obsLimsR[dirKey+discriminant] = {}
#		for binning in binnings:
#			expLimsL[dirKey+discriminant][binning] = []
#			obsLimsL[dirKey+discriminant][binning] = []
#			expLimsR[dirKey+discriminant][binning] = []
#			obsLimsR[dirKey+discriminant][binning] = []
#			for tempKey in tempKeys:
#				for BRind in range(nBRconf):
#					BRconfStr=''
#					if doBRScan: BRconfStr='_bW'+str(BRs['BW'][BRind]).replace('.','p')+'_tZ'+str(BRs['TZ'][BRind]).replace('.','p')+'_tH'+str(BRs['TH'][BRind]).replace('.','p')
#					limitDir='/user_data/ssagir/x53x53_limits_2016/'+dir+'/'+tempKey+BRconfStr+'/'
#					limitFile='/limits_templates_'+discriminant+'_'+signal+signal+'M700left'+BRconfStr+'_'+str(lumiStr)+'fb_rebinned_stat'+str(binning).replace('.','p')+'_expected.txt'	
#					print limitDir+cutString+limitFile
#					expTemp,obsTemp = PlotLimits(limitDir,limitFile,'left',tempKey+BRconfStr)
#					expLimsL[dirKey+discriminant][binning].append(expTemp)
#					obsLimsL[dirKey+discriminant][binning].append(obsTemp)
#					limitFile='/limits_templates_'+discriminant+'_'+signal+signal+'M700right'+BRconfStr+'_'+str(lumiStr)+'fb_rebinned_stat'+str(binning).replace('.','p')+'_expected.txt'	
#					expTemp,obsTemp = PlotLimits(limitDir,limitFile,'right',tempKey+BRconfStr)
#					expLimsR[dirKey+discriminant][binning].append(expTemp)
#					obsLimsR[dirKey+discriminant][binning].append(obsTemp)
#if doBRScan:
#	print "BRs_bW:",BRs['BW']
#	print "BRs_tH:",BRs['TH']
#	print "BRs_tZ:",BRs['TZ']
## print "Configs :",tempKeys
## for dir in dirs:
## 	print dir
## 	for binning in binnings:
## 		print binning
## 		print "Expected:",expLims[dir][binning]
## 		print "Observed:",obsLims[dir][binning]
#for discriminant in iPlotList:
#	print discriminant
#	for dirKey in dirKeyList: print dirKey,
#	print
#	for ind in range(len(tempKeys)):
#		print "////////////////////////////////"
#		print "Channel Configuration: "+tempKeys[ind]
#		print "////////////////////////////////"
#		for binning in binnings:
#			for dirKey in dirKeyList:
#				print dirKey+'_'+binning,
#		print
#		print "Expected:"
#		print "LH:",
#		for binning in binnings:
#			for dirKey in dirKeyList: 
#				print expLimsL[dirKey+discriminant][binning][ind],
#		print
#		print "RH:",
#		for binning in binnings:
#			for dirKey in dirKeyList: 
#				print expLimsR[dirKey+discriminant][binning][ind],
#		print
#		print "Observed:"
#		print "LH:",
#		for binning in binnings:
#			for dirKey in dirKeyList: 
#				print obsLimsL[dirKey+discriminant][binning][ind],
#		print 
#		print "RH:",
#		for binning in binnings:
#			for dirKey in dirKeyList: 
#				print obsLimsR[dirKey+discriminant][binning][ind],
#		print
#

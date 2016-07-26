/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * plotAll.C                                                                   *
 *                                                                             *
 * Author: Alex Garabedian (2016)                                              *
 * Modified by Evan Coleman                                                    *
 *                                                                             *
 * ROOT Macro for control plots from DELPHES trees. Originally included in     *
 * the Delphes_Analysis package (github.com/agarabed/Delphes_Analysis/)        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <iostream>
#include <vector>
#include <map>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "THStack.h"

#include "Delphes_Analysis/interface/Sample.h"
#include "Delphes_Analysis/interface/LoadSamples.h"

using namespace std;

R__ADD_INCLUDE_PATH(delphes)


void PlotExample(TString outputBasedir,     // directory to place plots
                 TString outputBaseName,    // base name format for plots 
                 bool savePlots=false,      // if true, save all plots in pdf/png format 
                 bool logPlots=false,       // if true, and savePlots==true, save plots with log scales on the y axis
                 Int_t numEvents=-1,        // plot a given number of events for each sample (-1 for all)
                 Int_t numSamples=-1,       // plot a given number of samples (-1 for all)
                 Float_t lumi=1000.0*3000.0 // integrated luminosity of the sample (units?)
                                            // comma-separated list of sample names to open (order matters!)
                 const char* SampleOrderString="tt_0_600,tt_600_1100,tt_1100_1700,tt_1700_2500,tt_2500_100000")
{
    // set batchmode (for speed) and open output file
    gROOT->ProcessLine("gROOT->SetBatch()");
    TFile *outFile = new TFile(outputBasedir+"/"+outputBaseName+".root","RECREATE");
    outFile->mkdir("Stacks");
    outFile->mkdir("Hists");

    // set up useful maps / arrays
    map<TString, TLegend*> legs;
    map<TString, TCanvas*> canvases;
    map<TString, THStack*> stacks;
    map<TString, Sample> Samples = LoadSamples("SnowMass_0PU");
    vector<TString> SampleOrder  = strtok(SampeOrderString,",");
    //////////vector<TString> SampleOrder = {"TTbar","TTbar2"}; TODO understand this
    vector<aPlot> PlotInfo;

    // add in plot information
    PlotInfo.push_back( aPlot{"Missing_ET",         "MissingET", "MET",    "Missing ET",                  100,0,  1000 });
    PlotInfo.push_back( aPlot{"Scalar_HT",          "ScalarHT",  "HT",     "Scalar HT [GeV]",             100,0,  1000 });
    PlotInfo.push_back( aPlot{"HT",                 "Jet",       "HT",     "HT [GeV]",                    100,0,  1000 });
    PlotInfo.push_back( aPlot{"Leading_Jet_pT",     "Jet",       "PT",     "Leading Jet p_{T} [GeV]",     100,0,  1000 });
    PlotInfo.push_back( aPlot{"Jet_pT",             "Jet",       "PT",     "Jet p_{T} [GeV]",             100,0,  1000 });
    PlotInfo.push_back( aPlot{"SubLeading_Jet_pT",  "Jet",       "PT",     "Sub-Leading Jet p_{T} [GeV]", 100,0,  1000 });
    PlotInfo.push_back( aPlot{"Leading_Jet_Eta",    "Jet",       "Eta",    "Leading Jet Eta",             300,-3, 3    });
    PlotInfo.push_back( aPlot{"Jet_Eta",            "Jet",       "Eta",    "Jet Eta",                     300,-3, 3    });
    PlotInfo.push_back( aPlot{"SubLeading_Jet_Eta", "Jet",       "Eta",    "Sub-Leading Jet Eta",         300,-3, 3    });
    PlotInfo.push_back( aPlot{"nJets",              "Jet",       "nJets",  "Jet Multiplicity",            100,0,  100  }); 
    PlotInfo.push_back( aPlot{"Jet Tau 1",          "JetAK8",    "Tau[1]", "Jet Tau 1",                   120,0,  120  });
    PlotInfo.push_back( aPlot{"Jet Tau 2",          "JetAK8",    "Tau[2]", "Jet Tau 2",                   120,0,  120  });   
    PlotInfo.push_back( aPlot{"Electron_pT",        "Electron",  "PT",     "Electron pT [GeV]",           120,0,  120  });   
    PlotInfo.push_back( aPlot{"Electron_Eta",       "Electron",  "Eta",    "Electron Eta",                120,0,  120  });   
    
    // calculate the number of samples
    Int_t sampleSize = SampleOrder.size();
    if(numSamples >=0) sampleSize = min(sampleSize,numSamples);

    // prepare canvases, draw tools
    cout << "CREATING CANVASES AND STACKS" << endl;
    for(Int_t it = 0; it < PlotInfo.size(); it++){
        aPlot   thePlot    = PlotInfo[it];
        TString plotName   = thePlot.plotName;

        cout << "\t - preparing plot canvas: " << plotName << endl;

        canvases[plotName] = new TCanvas();
        legs[plotName]     = new TLegend(.65,.45,.85,.75);
        stacks[plotName]   = new THStack(plotName, plotName);
        
        canvases[plotName]->SetLogy(1);
        stacks[plotName]->GetYaxis()->SetTitle("Events");
    }

    // start sample loop, prepare to fill histograms
    cout << endl << "LOOPING OVER SAMPLES" << endl;
    for(Int_t iter = 0; iter < SampleOrder.size(); iter++) {
        cout << "\t - SAMPLE IS " << SampleOrder[iter] << endl;
        Sample *currentSample = &Samples[SampleOrder[iter]];
        currentSample->OpenSample();

        cout << "\t\t - OPENING NECESSARY BRANCHES IN SAMPLE & MAKING HISTS" << endl;

        currentSample->LoadBranch("Event");

        // initialize histograms to make for this sample
        for(Int_t it = 0; it < PlotInfo.size(); it++){
            aPlot   thePlot    = PlotInfo[it];
            TString plotName   = thePlot.plotName;
            TString branchName = thePlot.branchName;
            TString varName    = thePlot.varName;
            TString xAxisLabel = thePlot.xAxisLabel;
            Float_t xAxisMin   = thePlot.xAxisMin;
            Float_t xAxisMin   = thePlot.xAxisMax;
            Float_t nBins      = thePlot.nBins;

            cout << "\t\t - initializing histogram for " << plotName << endl;

            currentSample->LoadBranch(branchName);
            currentSample->plots[plotName] = new TH1F(plotName, xAxisLabel, nBins, xAxisMin, xAxisMax);
            currentSample->plots[plotName]->SetFillColor(currentSample->color);
            currentSample->plots[plotName]->SetLineColor(currentSample->color);
            legs[plotName]->AddEntry(currentSample->plots[plotName], currentSample->name, "f");
        }

        // calculate the number of events 
        Int_t eventsSize = currentSample->nEvents;
        if(numEvents >=0) eventsSize = min(eventsSize,numEvents);

        // read the tree data for one event, fill histos
        cout << "\n\t\t - LOOPING OVER " << eventsSize << " ENTRIES" << endl;
        for(Int_t entry = 0; entry < eventsSize; entry++) {

            currentSample->treeReader->ReadEntry(entry);
            for(Int_t it = 0; it < PlotInfo.size(); it++){
                aPlot thePlot = PlotInfo[it];
                currentSample->FillFloat(thePlot);
            }
        }

        // merge histos
        cout << "\n\t\t - ADDING TO STACKS" << endl;
        for(Int_t it = 0; it < PlotInfo.size(); it++){
            aPlot thePlot      = PlotInfo[it];
            TString plotName   = thePlot.plotName;
            TString branchName = thePlot.branchName;
            TString varName    = thePlot.varName;
            if(currentSample->type == "bkg"){
                currentSample->plots[plotName]->GetXaxis()->SetTitle(thePlot.xAxisLabel);
                currentSample->plots[plotName]->Scale(lumi * currentSample->xsec / currentSample->genEvents);

                cout << "\t\t - " << plotName << " " << currentSample->plots[plotName]->GetEntries() 
                     << " "       << currentSample->plots[plotName]->Integral() << endl;

                stacks[plotName]->Add(currentSample->plots[plotName]);
                outFile->cd("Hists");
                currentSample->plots[plotName]->Write(plotName + "__" + currentSample->name);
            }
        }
    }

    // save plots to outfile 
    cout << "WRITING PLOTS TO " << outFile->GetName() << endl;
    for(Int_t it = 0; it < PlotInfo.size(); it++){

        aPlot thePlot = PlotInfo[it];

        TString plotName   = thePlot.plotName;
        TString branchName = thePlot.branchName;
        TString varName    = thePlot.varName;
        TString xLabel     = thePlot.xAxisLabel;

        cout << "\t - writing plot to outfile: " << plotName << endl;
        canvases[plotName]->cd();
        canvases[plotName]->SetName(plotName);

        stacks[plotName]->Draw();
        legs[plotName]->Draw("same");

        outFile->cd("Stacks");

        canvases[plotName]->Write();

        if(savePlots) {
            canvases[plotName]->SetLogy(0);
            canvases[plotName]->SaveAs(outputBasedir + "/" + outputBaseName + plotName + ".png");
            canvases[plotName]->SaveAs(outputBasedir + "/" + outputBaseName + plotName + ".pdf");

            if(logPlots) {
                canvases[plotName]->SetLogy(1);
                canvases[plotName]->SaveAs(outputBasedir + "/" + outputBaseName + plotName + "_log.png");
                canvases[plotName]->SaveAs(outputBasedir + "/" + outputBaseName + plotName + "_log.pdf");
            }
        }
    }

    return;
}

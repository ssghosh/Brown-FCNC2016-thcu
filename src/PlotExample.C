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
void PlotExample(){
    Float_t lumi = 1000.0*300.0;
    
    map<TString, TLegend*> legs;
    
    gROOT->ProcessLine("gROOT->SetBatch()");
    TFile *outFile = new TFile("plots.root","RECREATE");
    outFile->mkdir("Stacks");
    outFile->mkdir("Hists");
    
    map<TString, Sample> Samples = LoadSamples("SnowMass_200PU");
    
    vector<TString> SampleOrder = {"tt_0_600","tt_600_1100","tt_1100_1700","tt_1700_2500","tt_2500_100000"};
    //vector<TString> SampleOrder = {"TTbar","TTbar2"};
    vector<aPlot> PlotInfo;
//    PlotInfo.push_back( aPlot{"Scalar_HT", "ScalarHT", "HT", "Scalar HT [GeV]"} );
    PlotInfo.push_back( aPlot{"HT", "Jet", "HT", "HT [GeV]"} );
    PlotInfo.push_back( aPlot{"Leading_Jet_pT", "Jet", "PT", "Leading Jet p_{T} [GeV]"} );
//    PlotInfo.push_back( aPlot{"Leading_Jet_Eta", "Jet", "Eta", "Leading Jet Eta"} );
//    PlotInfo.push_back( aPlot{"Jet_pT", "Jet", "PT", "Jet p_{T} [GeV]"} );
//    PlotInfo.push_back( aPlot{"Jet_Eta", "Jet", "Eta", "Jet Eta"} );
    PlotInfo.push_back( aPlot{"SubLeading_Jet_pT", "Jet", "PT", "Sub-Leading Jet p_{T} [GeV]"} );
    //    PlotInfo.push_back( aPlot{"SubLeading_Jet_Eta", "Jet", "Eta", "Sub-Leading Jet Eta"} );
    PlotInfo.push_back( aPlot{"Missing_ET", "MissingET", "MET", "Missing ET"} );
    PlotInfo.push_back( aPlot{"nJets", "Jet","nJets", "Jet Multiplicity"} ); 
    
    //    map<TString, map<TString, TString> > BranchPlotNames;
    //    BranchPlotNames["ScalarHT"] = {"ScalarHT","HT"};
    //    BranchPlotNames["ScalarHT"] = "HT";
    map<TString, TCanvas*> canvases;
    map<TString, THStack*> stacks;
    
    cout << "CREATING CANVASES AND STACKS" << endl;
    for(Int_t it = 0; it < PlotInfo.size(); it++){
        aPlot thePlot = PlotInfo[it];
        TString plotName = thePlot.plotName;
        TString branchName = thePlot.branchName;
        TString varName = thePlot.varName;
        cout << "\t" << plotName << endl;
        canvases[plotName] = new TCanvas();
        legs[plotName]  = new TLegend(.65,.45,.85,.75);
        canvases[plotName]->SetLogy(1);
        stacks[plotName] = new THStack(plotName, plotName);
//        stacks[plotName]->GetYaxis()->SetTitle("Events");
        //cout << thePlot.xAxisLabel << endl;
    }
    
    cout << endl << "LOOPING OVER SAMPLES" << endl;
    for(Int_t iter = 0; iter < SampleOrder.size(); iter++){
        cout << "\t" << SampleOrder[iter] << endl;
        Sample *currentSample = &Samples[SampleOrder[iter]];
        currentSample->OpenSample();
        
        cout << "\tOPENING NECESSARY BRANCHES IN SAMPLE & MAKING HISTS" << endl;
        for(Int_t it = 0; it < PlotInfo.size(); it++){
            aPlot thePlot = PlotInfo[it];
            TString plotName = thePlot.plotName;
            TString branchName = thePlot.branchName;
            TString varName = thePlot.varName;
            cout << "\t\t" << plotName << endl;
            currentSample->LoadBranch(branchName);
            if(varName == "PT" or varName == "HT" or varName == "MET") currentSample->plots[plotName] = new TH1F(plotName, plotName,100,0,1000);
            else if(varName == "Eta") currentSample->plots[plotName] = new TH1F(plotName, plotName,300,-3,3);
            else if(varName == "nJets") currentSample->plots[plotName] = new TH1F(plotName, plotName,100,0,100);
	    currentSample->plots[plotName]->SetFillColor(currentSample->color);
            currentSample->plots[plotName]->SetLineColor(currentSample->color);
            legs[plotName]->AddEntry(currentSample->plots[plotName], currentSample->name, "f");
        }
        
        Int_t alert = 0;
        cout << "\n\tLOOPING OVER ENTRIES" << endl;
        for(Int_t entry = 0; entry < currentSample->nEvents; entry++){
            if (entry >= (alert * .10 * currentSample->nEvents)){
                cout << "\t\t" << alert*10 << "%" << endl;
                alert++;
            }
            currentSample->treeReader->ReadEntry(entry);
            for(Int_t it = 0; it < PlotInfo.size(); it++){
                aPlot thePlot = PlotInfo[it];
                TString plotName = thePlot.plotName;
                TString branchName = thePlot.branchName;
                TString varName = thePlot.varName;
                currentSample->FillFloat(thePlot);
            }
            
        }
        cout << "\n\tADDING TO STACKS" << endl;
        for(Int_t it = 0; it < PlotInfo.size(); it++){
            aPlot thePlot = PlotInfo[it];
            TString plotName = thePlot.plotName;
            TString branchName = thePlot.branchName;
            TString varName = thePlot.varName;
            if(currentSample->type == "bkg"){
                currentSample->plots[plotName]->GetXaxis()->SetTitle(thePlot.xAxisLabel);
                currentSample->plots[plotName]->Scale(lumi * currentSample->xsec / currentSample->genEvents);
                cout << "\t\t" << plotName << " " <<  currentSample->plots[plotName]->GetEntries() << " " << currentSample->plots[plotName]->Integral() <<endl;
                stacks[plotName]->Add(currentSample->plots[plotName]);
                outFile->cd("Hists");
                currentSample->plots[plotName]->Write(plotName + "__" + currentSample->name);
            }
        }
    }
    cout << "WRITING PLOTS" << endl;
    for(Int_t it = 0; it < PlotInfo.size(); it++){

aPlot thePlot = PlotInfo[it];

        TString plotName = thePlot.plotName;

        TString branchName = thePlot.branchName;

TString varName = thePlot.varName;

TString xLabel = thePlot.xAxisLabel;

cout << "\t" << plotName << endl;
        
        canvases[plotName]->cd();

canvases[plotName]->SetName(plotName);

stacks[plotName]->Draw();

legs[plotName]->Draw("same");

outFile->cd("Stacks");

canvases[plotName]->Write();

//canvases[plotName]->SaveAs(plotName + ".pdf");
    }
    
    return;
}

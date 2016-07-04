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

//R__ADD_INCLUDE_PATH(delphes)
void PlotExample(TString RFileName){
    
    TFile *outFile = new TFile("plots.root","RECREATE");
    outFile->mkdir("Stacks");
    outFile->mkdir("Hists");
    
    map<TString, Sample> Samples = LoadSamples("SingleLepton");
    
    vector<TString> SampleOrder = {"TTbar", "TTbar2"};
    vector<aPlot> PlotInfo;
    PlotInfo.push_back( aPlot{"Scalar_HT", "ScalarHT", "HT"} );
    PlotInfo.push_back( aPlot{"Leading_Jet_pT", "Jet", "PT"} );
    PlotInfo.push_back( aPlot{"Leading_Jet_Eta", "Jet", "Eta"} );
    PlotInfo.push_back( aPlot{"Jet_pT", "Jet", "PT"} );
    PlotInfo.push_back( aPlot{"Jet_Eta", "Jet", "Eta"} );
    PlotInfo.push_back( aPlot{"SubLeading_Jet_pT", "Jet", "PT"} );
    PlotInfo.push_back( aPlot{"SubLeading_Jet_Eta", "Jet", "Eta"} );
    
    
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
        stacks[plotName] = new THStack();
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
            if(varName == "PT" or varName == "HT") currentSample->plots[plotName] = new TH1F(plotName, plotName,150,0,1500);
            if(varName == "Eta") currentSample->plots[plotName] = new TH1F(plotName, plotName,300,-3,3);
            currentSample->plots[plotName]->SetFillColor(currentSample->color);
            currentSample->plots[plotName]->SetLineColor(currentSample->color);
        }

        Int_t alert = 0;
        cout << "\tLOOPING OVER ENTRIES" << endl;
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
        cout << "\tADDING TO STACKS" << endl;
        for(Int_t it = 0; it < PlotInfo.size(); it++){
            aPlot thePlot = PlotInfo[it];
            TString plotName = thePlot.plotName;
            TString branchName = thePlot.branchName;
            TString varName = thePlot.varName;
            if(currentSample->type == "bkg"){
                currentSample->plots[plotName]->Scale(currentSample->xsec / currentSample->genEvents);
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
        cout << "\t" << plotName << endl;
        canvases[plotName]->cd();
        canvases[plotName]->SetName(plotName);
        stacks[plotName]->Draw();
        outFile->cd("Stacks");
        canvases[plotName]->Write();
    }
    
    return;
}
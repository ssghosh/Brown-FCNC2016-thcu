#include <vector>

#include "TString.h"
#include "TColor.h"

R__ADD_INCLUDE_PATH(delphes)
#ifdef __CLING__
R__LOAD_LIBRARY(delphes/libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootTreeWriter.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#include "external/ExRootAnalysis/ExRootTreeBranch.h"
#else
class ExRootTreeReader;
class ExRootResult;
#endif

struct aPlot {
    TString plotName;
    TString branchName;
    TString varName;
    TString xAxisLabel;
};

class Sample {
public:
    TString name;
    TString path;
    TString type;
    Color_t color;
    
    TFile *RFile;
    TTree *RTree;
    
    ExRootTreeReader *treeReader;
    map<TString, TClonesArray*> branches;
    
    Float_t xsec;
    Float_t genEvents;
    Int_t nEvents;
    
    map<TString,  TH1F*> plots;
    
    virtual void OpenSample();
    virtual void LoadBranch(TString branchName);
    virtual void FillFloat(aPlot thePlot);
};

void Sample::OpenSample(){
    RFile = new TFile(path, "READ");
    RTree = (TTree*)RFile->Get("Delphes");
    treeReader = new ExRootTreeReader(RTree);
    nEvents = treeReader->GetEntries();
    
    return;
}
void Sample::LoadBranch(TString branchName){
    map<TString, TClonesArray*>::iterator it = branches.find(branchName);
    if(it != branches.end()) return;
    
    branches[branchName] = treeReader->UseBranch(branchName);
    
}
void Sample::FillFloat(aPlot thePlot){
    
    TString plotName = thePlot.plotName;
    TString branchName = thePlot.branchName;
    TString varName = thePlot.varName;
    
    if(branchName == "ScalarHT"){
        
        ScalarHT *ht = (ScalarHT*)branches[branchName]->At(0);
        if(varName == "HT"){
            plots[plotName]->Fill(ht->HT);
        }
    }
    else if(branchName == "MissingET"){
        MissingET *met = (MissingET*)branches[branchName]->At(0);
        if(varName == "MET"){
            plots[plotName]->Fill(met->MET);
        }
    }
    else if(branchName == "Jet"){
        Float_t max_pt = 0.0;
        Float_t sub_pt = 0.0;
        
        Float_t ht = 0.0;
        
        Int_t max_pt_i = 0;
        Int_t sub_pt_i = 0;
        if(varName == "nJets"){
	    //cout << branches[branchName]->GetEntries() << endl;
	    plots[plotName]->Fill(branches[branchName]->GetEntries());
            return;
	}
        for(Int_t j = 0; j<branches[branchName]->GetEntries(); j++){
            Jet *jet = (Jet*)branches[branchName]->At(j);
            if (jet->PT > max_pt){
                sub_pt = max_pt;
                sub_pt_i = max_pt_i;
                
                max_pt = jet->PT;
                max_pt_i = j;
                
            }
            else if (jet->PT > sub_pt){
                sub_pt = jet->PT;
                sub_pt_i = j;
            }
            
            if(!plotName.Contains("Leading")){
                if(varName=="PT") plots[plotName]->Fill(jet->PT);
                else if(varName == "Eta") plots[plotName]->Fill(jet->Eta);
            }
            
            ht+=jet->PT;
            
        }
        if(plotName.Contains("Leading") && !plotName.Contains("Sub")){
            Jet *jet = (Jet*)branches[branchName]->At(max_pt_i);
            if(varName=="PT") plots[plotName]->Fill(jet->PT);
            else if(varName == "Eta") plots[plotName]->Fill(jet->Eta);
        }
        else if(plotName.Contains("SubLeading")){
            Jet *jet = (Jet*)branches[branchName]->At(sub_pt_i);
            if(varName=="PT") plots[plotName]->Fill(jet->PT);
            else if(varName == "Eta") plots[plotName]->Fill(jet->Eta);
        }
        else if(plotName == "HT"){
            plots[plotName]->Fill(ht);
        }
    }
    
    return;
    
}

#include <iostream>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector>
#include <TString.h>
#include <utility>
#include <fstream>

// Header file for the classes stored in the TTree if any.
#include "TClonesArray.h"
#include "TObject.h"
#include "TLorentzVector.h"
#include "TRef.h"
#include "TRefArray.h"
#include "TClass.h"
#include "TSystem.h"

#include "Selections.h"


using namespace std;

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

class BrownDelphesSelector {
public:
    
    TString AnalysisType;
    TString inputString;
    ifstream fileList;
    Selection *TheSelection = new Selection();
    
    TChain *inChain = new TChain();
    Long64_t allEntries;
    
    ExRootTreeReader *treeReader;
    ExRootTreeWriter *treeWriter;
    
    TFile *outFile;
    
    Long64_t entry;
    
    MissingET *met;
    Electron *elec;
    Muon *muon;
    Jet *jet;
    
    TClonesArray *branchEvent;
    TClonesArray *branchScalarHT;
    TClonesArray *branchMissingET;
    TClonesArray *branchElectron;
    TClonesArray *branchMuonTight;
    TClonesArray *branchJet;
    //    TClonesArray *branchParticle;
    
    ExRootTreeBranch *branchEvent_new;
    ExRootTreeBranch *branchScalarHT_new;
    ExRootTreeBranch *branchMissingET_new;
    ExRootTreeBranch *branchElectron_new;
    ExRootTreeBranch *branchMuonTight_new;
    ExRootTreeBranch *branchJet_new;
    //    ExRootTreeBranch *branchParticle_new;
    
    vector <Electron*> GoodElectrons;
    vector <Muon*> GoodMuons;
    vector <Jet*> GoodJets;
    
    map<TString, Int_t> cutFlow;
    
    BrownDelphesSelector();
    ~BrownDelphesSelector();
    virtual void Init(const char *inString, const char *outFileName, const char *treeName, TString type);
    virtual void LoadChain();
    virtual void Loop();
    virtual bool METCuts();
    virtual vector <Electron*> ElectronHandler();
    virtual vector <Muon*> MuonHandler();
    virtual vector <Jet*> JetHandler();
    virtual void PrintCutFlow(map<TString, Int_t> cFlow);
    virtual void Fill();
};


BrownDelphesSelector::BrownDelphesSelector(){};
BrownDelphesSelector::~BrownDelphesSelector(){};

void BrownDelphesSelector::Init(const char *inString, const char *outFileName, const char *treeName, TString type){
    
    AnalysisType = type;
    inputString = inString;
    inChain->SetName(treeName);
    
    LoadChain();
    
    //    fileList = ifstream(inputFile);
    //    std::string line;
    //    while(std::getline(fileList, line)){
    //        if (line.length()==0) continue;
    //        inChain->Add(TString(line));
    //    }
    
    treeReader = new ExRootTreeReader(inChain);
    allEntries = treeReader->GetEntries();
    cout << "** Chain contains " << allEntries << " events" << endl;
    
    branchEvent                 = treeReader->UseBranch("Event");
    branchElectron              = treeReader->UseBranch("Electron");
    branchMuonTight             = treeReader->UseBranch("MuonTight");
    branchJet                   = treeReader->UseBranch("Jet");
    branchMissingET             = treeReader->UseBranch("MissingET");
    branchScalarHT              = treeReader->UseBranch("ScalarHT");
    //    branchParticle              = treeReader->UseBranch("Particle");
    
    outFile = new TFile(outFileName, "RECREATE");
    treeWriter = new ExRootTreeWriter(outFile, "Delphes");
    
    
    branchEvent_new         = treeWriter->NewBranch("Event", Event::Class());
    branchMissingET_new     = treeWriter->NewBranch("MissingET", MissingET::Class());
    branchElectron_new      = treeWriter->NewBranch("Electron", Electron::Class());
    branchMuonTight_new     = treeWriter->NewBranch("MuonTight", Muon::Class());
    branchJet_new           = treeWriter->NewBranch("Jet", Jet::Class());
    branchScalarHT_new      = treeWriter->NewBranch("ScalarHT", ScalarHT::Class());
    //    branchParticle_new      = treeWriter->NewBranch("Particle", GenParticle::Class());
    
    cout << "INITIALIZED!" << endl;
    
    return;
}

void BrownDelphesSelector::LoadChain(){
    cout << endl << "ADDING FILES:" << endl;
    char* inString = (char*) inputString.Data();
    
    if(inputString.Contains(".root", TString::kIgnoreCase)){
        cout << inputString << endl;
        inChain->Add(inputString);
    }
    else if(inputString.Contains(".txt", TString::kIgnoreCase)){
        fileList = ifstream(inputString);
        std::string line;
        while(std::getline(fileList, line)){
            if (line.length()==0) continue;
            cout << line << endl;
            inChain->Add(TString(line));
        }
    }
    else if(gSystem->OpenDirectory(inputString)){
        
        char* dir = gSystem->ExpandPathName(inString);
        void* dirp = gSystem->OpenDirectory(dir);
        
        const char* entry;
        const char* filename;
        TString str;
        
        while((entry = (char*)gSystem->GetDirEntry(dirp))) {
            str = entry;
            if (str.Contains(".root", TString::kIgnoreCase)){
                cout << inputString + "/" + str << endl;
                inChain->Add(inputString + "/" + str);
            }
        }
    }
    cout << endl;
    return;
}

//================================================================================
//                              MAIN LOOP
//================================================================================
void BrownDelphesSelector::Loop(){
    cutFlow.clear();
    cutFlow["All Events"]=0;
    cutFlow["MET Cuts"]=0;
    cutFlow["Lepton Cuts"]=0;
    cutFlow["Jet Cuts"]=0;
    
    TheSelection = new Selection(AnalysisType);
    
    for(entry=0; entry < allEntries; ++entry){
        if(entry%1000 == 0) cout << entry << endl;
        cutFlow["All Events"]++;
        
        treeReader->ReadEntry(entry);
        
        
        if(!METCuts()) {continue;}
        cutFlow["MET Cuts"]++;
        
        
        GoodElectrons = ElectronHandler();
        GoodMuons = MuonHandler();
        
        if(!(TheSelection->LeptonCuts(GoodElectrons, GoodMuons))) {continue;}
        cutFlow["Lepton Cuts"]++;
        
        
        GoodJets = JetHandler();
        
        if(!(TheSelection->JetCuts(GoodJets))) {continue;}
        cutFlow["Jet Cuts"]++;
        
        Fill();
    }
    outFile->cd();
    treeWriter->Write();
    PrintCutFlow(cutFlow);
}
//================================================================================
//                                  MET
//================================================================================
bool BrownDelphesSelector::METCuts(){
    
    met = (MissingET*) branchMissingET->At(0);
    
    return TheSelection->METCuts(met);
}

//================================================================================
//                                  ELECTRONS
//================================================================================
vector <Electron*> BrownDelphesSelector::ElectronHandler(){
    bool isBad;
    vector <Electron*> elecVector;
    for(int i=0; i<branchElectron->GetEntriesFast(); ++i){
        
        elec = (Electron*)branchElectron->At(i);
        isBad = TheSelection->GoodElecCuts(elec);
        if(isBad) continue;
        elecVector.push_back(elec);
    }
    return elecVector;
}

//================================================================================
//                                  MUONS
//================================================================================
vector <Muon*> BrownDelphesSelector::MuonHandler(){
    bool isBad;
    vector <Muon*> muonVector;
    for(int i=0; i<branchMuonTight->GetEntriesFast(); ++i){
        muon = (Muon*) branchMuonTight->At(i);
        
        isBad = TheSelection->GoodMuonCuts(muon);
        if(isBad) continue;
        muonVector.push_back(muon);
    }
    return muonVector;
}

//================================================================================
//                                  JETS
//================================================================================
vector <Jet*> BrownDelphesSelector::JetHandler(){
    bool isBad;
    vector <Jet*> jetVector;
    for(int i=0; i<branchJet->GetEntriesFast(); ++i){
        jet = (Jet*) branchJet->At(i);
        
        isBad = TheSelection->GoodJetCuts(jet);
        if(isBad) continue;
        jetVector.push_back(jet);
    }
    return jetVector;
}

//================================================================================
//                                  OTHER
//================================================================================
void BrownDelphesSelector::Fill(){
    
    MissingET* met_new = static_cast<MissingET*>(branchMissingET_new->NewEntry());
    *met_new = *met;
    
    vector <Electron*> GoodElectrons_new;
    for(Int_t i = 0; i < GoodElectrons.size(); i++){
        GoodElectrons_new.push_back(static_cast<Electron*>(branchElectron_new->NewEntry()));
        *GoodElectrons_new[i] = *GoodElectrons[i];
    }
    
    vector <Muon*> GoodMuons_new;
    for(Int_t i = 0; i < GoodMuons.size(); i++){
        GoodMuons_new.push_back(static_cast<Muon*>(branchMuonTight_new->NewEntry()));
        *GoodMuons_new[i] = *GoodMuons[i];
    }
    
    vector <Jet*> GoodJets_new;
    for(Int_t i = 0; i < GoodJets.size(); i++){
        GoodJets_new.push_back(static_cast<Jet*>(branchJet_new->NewEntry()));
        *GoodJets_new[i] = *GoodJets[i];
    }
    
    Event *event = (Event*)branchEvent->At(0);
    Event *event_new = static_cast<Event*>(branchEvent_new->NewEntry());
    *event_new = *event;
    
    //    vector<GenParticle*> Particle_new;
    //    for(Int_t i = 0; i < branchParticle->GetEntries(); i++){
    //        Particle_new.push_back(static_cast<GenParticle*>(branchParticle_new->NewEntry()));
    //        GenParticle *particle = (GenParticle*)branchParticle->At(i);
    //        *Particle_new[i] = *particle;
    //    }
    
    ScalarHT *ht = (ScalarHT*)branchScalarHT->At(0);
    ScalarHT* ht_new = static_cast<ScalarHT*>(branchScalarHT_new->NewEntry());
    *ht_new = *ht;
    
    treeWriter->Fill();
    treeWriter->Clear();
}

void BrownDelphesSelector::PrintCutFlow(map<TString, Int_t> cFlow){
    vector<TString> order;
    order.push_back("All Events");
    order.push_back("MET Cuts");
    order.push_back("Lepton Cuts");
    order.push_back("Jet Cuts");
    
    cout << "===========" << endl;
    cout << "  CutFlow  " << endl;
    cout << "===========" << endl;
    for(int ii = 0; ii<order.size(); ii++){
        cout << order[ii] << ": " << cFlow[order[ii]] << endl;
    }
    cout << "===========" << endl;
}

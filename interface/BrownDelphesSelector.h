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

//R__ADD_INCLUDE_PATH(delphes)
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
    TClonesArray *branchElectron;
    TClonesArray *branchMuonTight;
    TClonesArray *branchMuonLoose;
    TClonesArray *branchMissingET;
    TClonesArray *branchGenMissingET;
    TClonesArray *branchPUPPIMissingET;
    TClonesArray *branchGenPileUpMissingET;
    TClonesArray *branchScalarHT;
    TClonesArray *branchJet;
    TClonesArray *branchGenJet;
    TClonesArray *branchGenJetAK8;
    TClonesArray *branchJetPUPPI;
    TClonesArray *branchJetAK8;
    TClonesArray *branchJetPUPPIAK8;
    TClonesArray *branchPhoton;
    //    TClonesArray *branchParticle;
    
    ExRootTreeBranch *branchEvent_new;
    ExRootTreeBranch *branchElectron_new;
    ExRootTreeBranch *branchMuonTight_new;
    ExRootTreeBranch *branchMuonLoose_new;
    ExRootTreeBranch *branchMissingET_new;
    ExRootTreeBranch *branchGenMissingET_new;
    ExRootTreeBranch *branchPUPPIMissingET_new;
    ExRootTreeBranch *branchGenPileUpMissingET_new;
    ExRootTreeBranch *branchScalarHT_new;
    ExRootTreeBranch *branchJet_new;
    ExRootTreeBranch *branchGenJet_new;
    ExRootTreeBranch *branchGenJetAK8_new;
    ExRootTreeBranch *branchJetPUPPI_new;
    ExRootTreeBranch *branchJetAK8_new;
    ExRootTreeBranch *branchJetPUPPIAK8_new;
    ExRootTreeBranch *branchPhoton_new;
    //    TClonesArray *branchParticle;
    
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
    branchMuonLoose             = treeReader->UseBranch("MuonLoose");
    branchPhoton                = treeReader->UseBranch("Photon");
    
    branchMissingET             = treeReader->UseBranch("MissingET");
    branchGenMissingET          = treeReader->UseBranch("GenMissingET");
    branchPUPPIMissingET        = treeReader->UseBranch("PuppiMissingET");
    branchGenPileUpMissingET    = treeReader->UseBranch("GenPileUpMissingET");
    
    
    branchScalarHT              = treeReader->UseBranch("ScalarHT");
    
    branchJet                   = treeReader->UseBranch("Jet");
    branchGenJet                = treeReader->UseBranch("GenJet");
    branchGenJetAK8             = treeReader->UseBranch("GenJetAK8");
    branchJetPUPPI              = treeReader->UseBranch("JetPUPPI");
    branchJetAK8                = treeReader->UseBranch("JetAK8");
    branchJetPUPPIAK8           = treeReader->UseBranch("JetPUPPIAK8");
    //    branchParticle              = treeReader->UseBranch("Particle");
    
    outFile = new TFile(outFileName, "RECREATE");
    treeWriter = new ExRootTreeWriter(outFile, "Delphes");
    
    branchEvent_new                 = treeWriter->NewBranch("Event", HepMCEvent::Class());
    branchElectron_new              = treeWriter->NewBranch("Electron", Electron::Class());
    branchMuonTight_new             = treeWriter->NewBranch("MuonTight", Muon::Class());
    branchMuonLoose_new             = treeWriter->NewBranch("MuonLoose", Muon::Class());
    
    branchPhoton_new             = treeWriter->NewBranch("Photon", Photon::Class());
    
    branchMissingET_new             = treeWriter->NewBranch("MissingET", MissingET::Class());
    branchGenMissingET_new          = treeWriter->NewBranch("GenMissingET", MissingET::Class());
    branchPUPPIMissingET_new        = treeWriter->NewBranch("PuppiMissingET", MissingET::Class());
    branchGenPileUpMissingET_new    = treeWriter->NewBranch("GenPileUpMissingET", MissingET::Class());
    
    
    branchScalarHT_new              = treeWriter->NewBranch("ScalarHT", ScalarHT::Class());
    
    branchJet_new                   = treeWriter->NewBranch("Jet", Jet::Class());
    branchGenJet_new                = treeWriter->NewBranch("GenJet", Jet::Class());
    branchGenJetAK8_new             = treeWriter->NewBranch("GenJetAK8", Jet::Class());
    branchJetPUPPI_new              = treeWriter->NewBranch("JetPUPPI", Jet::Class());
    branchJetAK8_new                = treeWriter->NewBranch("JetAK8", Jet::Class());
    branchJetPUPPIAK8_new           = treeWriter->NewBranch("JetPUPPIAK8", Jet::Class());
    //    branchParticle_new              = treeWriter->NewBranch("Particle");
    
    
//    branchEvent_new         = treeWriter->NewBranch("Event", Event::Class());
//    branchMissingET_new     = treeWriter->NewBranch("MissingET", MissingET::Class());
//    branchElectron_new      = treeWriter->NewBranch("Electron", Electron::Class());
//    branchMuonTight_new     = treeWriter->NewBranch("MuonTight", Muon::Class());
//    branchJet_new           = treeWriter->NewBranch("Jet", Jet::Class());
//    branchScalarHT_new      = treeWriter->NewBranch("ScalarHT", ScalarHT::Class());
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
    
    HepMCEvent *event = (HepMCEvent*)branchEvent->At(0);
    HepMCEvent *event_new = static_cast<HepMCEvent*>(branchEvent_new->NewEntry());
    *event_new = *event;
    
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
    vector <Muon*> MuonLoose_new;
    for(Int_t i = 0; i < branchMuonLoose->GetEntries(); i++){
        MuonLoose_new.push_back(static_cast<Muon*>(branchMuonLoose_new->NewEntry()));
        Muon *MuonLoose = (Muon*)branchMuonLoose->At(i);
        *MuonLoose_new[i] = *MuonLoose;
    }
    
    MissingET* met_new = static_cast<MissingET*>(branchMissingET_new->NewEntry());
    *met_new = *met;
    
    MissingET* gen_met = (MissingET*)branchGenMissingET->At(0);
    MissingET* gen_met_new = static_cast<MissingET*>(branchGenMissingET_new->NewEntry());
    *gen_met_new = *gen_met;
    
    MissingET* puppi_met = (MissingET*)branchPUPPIMissingET->At(0);
    MissingET* puppi_met_new = static_cast<MissingET*>(branchPUPPIMissingET_new->NewEntry());
    *puppi_met_new = *puppi_met;
    
    MissingET* gen_pileup_met = (MissingET*)branchGenPileUpMissingET->At(0);
    MissingET* gen_pileup_met_new = static_cast<MissingET*>(branchGenPileUpMissingET_new->NewEntry());
    *gen_pileup_met_new = *gen_pileup_met;
    
    ScalarHT *ht = (ScalarHT*)branchScalarHT->At(0);
    ScalarHT* ht_new = static_cast<ScalarHT*>(branchScalarHT_new->NewEntry());
    *ht_new = *ht;
    
    vector <Jet*> GoodJets_new;
    for(Int_t i = 0; i < GoodJets.size(); i++){
        GoodJets_new.push_back(static_cast<Jet*>(branchJet_new->NewEntry()));
        *GoodJets_new[i] = *GoodJets[i];
    }
    vector<Jet*> GenJet_new;
    for(Int_t i = 0; i < branchGenJet->GetEntries(); i++){
        GenJet_new.push_back(static_cast<Jet*>(branchGenJet_new->NewEntry()));
        Jet *GenJet = (Jet*)branchGenJet->At(i);
        *GenJet_new[i] = *GenJet;
    }
    vector<Jet*> GenJetAK8_new;
    for(Int_t i = 0; i < branchGenJetAK8->GetEntries(); i++){
        GenJetAK8_new.push_back(static_cast<Jet*>(branchGenJetAK8_new->NewEntry()));
        Jet *GenJetAK8 = (Jet*)branchGenJetAK8->At(i);
        *GenJetAK8_new[i] = *GenJetAK8;
    }
    vector<Jet*> JetPUPPI_new;
    for(Int_t i = 0; i < branchJetPUPPI->GetEntries(); i++){
        JetPUPPI_new.push_back(static_cast<Jet*>(branchJetPUPPI_new->NewEntry()));
        Jet *JetPUPPI = (Jet*)branchJetPUPPI->At(i);
        *JetPUPPI_new[i] = *JetPUPPI;
    }
    vector<Jet*> JetAK8_new;
    for(Int_t i = 0; i < branchJetAK8->GetEntries(); i++){
        JetAK8_new.push_back(static_cast<Jet*>(branchJetAK8_new->NewEntry()));
        Jet *JetAK8 = (Jet*)branchJetAK8->At(i);
        *JetAK8_new[i] = *JetAK8;
    }
    vector<Jet*> JetPUPPIAK8_new;
    for(Int_t i = 0; i < branchJetPUPPIAK8->GetEntries(); i++){
        JetPUPPIAK8_new.push_back(static_cast<Jet*>(branchJetPUPPIAK8_new->NewEntry()));
        Jet *JetPUPPIAK8 = (Jet*)branchJetPUPPIAK8->At(i);
        *JetPUPPIAK8_new[i] = *JetPUPPIAK8;
    }
    //    vector<GenParticle*> Particle_new;
    //    for(Int_t i = 0; i < branchParticle->GetEntries(); i++){
    //        Particle_new.push_back(static_cast<GenParticle*>(branchParticle_new->NewEntry()));
    //        GenParticle *particle = (GenParticle*)branchParticle->At(i);
    //        *Particle_new[i] = *particle;
    //    }
    vector<Photon*> Photon_new;
    for(Int_t i = 0; i < branchPhoton->GetEntries(); i++){
        Photon_new.push_back(static_cast<Photon*>(branchPhoton_new->NewEntry()));
        Photon *photon = (Photon*)branchPhoton->At(i);
        *Photon_new[i] = *photon;
    }
    
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

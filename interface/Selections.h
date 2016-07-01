#include <iostream>

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

class Selection {
public:
    
    TString analysisType;
    
    Float_t minMet = 0.0;
    
    Float_t minElec_pt = 0.0;
    Float_t maxElec_eta = 1000.0;
    
    Float_t minMuon_pt = 0.0;
    Float_t maxMuon_eta = 1000.0;
    
    Int_t minTightLeptons = 0;
    Int_t maxTightLeptons = 1000;
    
    Int_t minLooseLeptons = 0;
    Int_t maxLooseLeptons = 1000;
    
    Float_t minJet_pt = 0.0;
    Float_t maxJet_eta = 1000.0;
    
    Int_t minJets = 0;
    Int_t maxJets = 1000;
    
    
    Selection(){};
    Selection(TString typeString);
    ~Selection(){};
    virtual bool METCuts(MissingET* met);
    
    virtual bool GoodElecCuts(Electron *elec);
    virtual bool GoodMuonCuts(Muon *muon);
    virtual bool LeptonCuts(vector <Electron*> goodElectrons, vector <Muon*> goodMuons);
    virtual bool GoodJetCuts(Jet *jet);
    virtual bool JetCuts(vector<Jet*> goodJets);
};

Selection::Selection(TString typeString){
    analysisType = typeString;
    
    if(analysisType == "SingleLepton"){
        minMet = 100.0;
        
        minElec_pt = 50.0;
        maxElec_eta = 2.1;
        
        minMuon_pt = 50.0;
        maxMuon_eta = 2.1;
        
        minTightLeptons = 1;
        maxTightLeptons = 1;
        
        minLooseLeptons = 1;
        maxLooseLeptons = 1;
        
        minJet_pt = 100.0;
        maxJet_eta = 2.4;
        
        minJets = 2;
        maxJets = 1000;
        
    }
    else{ cout << "WARNING: No Selection Specified" << endl; }
}
bool Selection::METCuts(MissingET *met){
    if(met->MET < minMet) return false;
    return true;
}
bool Selection::GoodElecCuts(Electron *elec){
    bool badElec = false;
    
    if(elec->PT < minElec_pt) badElec = true;
    if(fabs(elec->Eta) > maxElec_eta) badElec = true;
    
    return badElec;
    
}
bool Selection::GoodMuonCuts(Muon *muon){
    bool badMuon = false;
    
    if(muon->PT < minMuon_pt) badMuon = true;
    if(fabs(muon->Eta) > maxMuon_eta) badMuon = true;
    
    return badMuon;
    
}
bool Selection::LeptonCuts(vector <Electron*> goodElectrons, vector <Muon*> goodMuons){
    Int_t total_tight_leptons = goodElectrons.size() + goodMuons.size();
    
    if (total_tight_leptons < minTightLeptons || total_tight_leptons > maxTightLeptons){ return false; }
    else{ return true; }
}
bool Selection::GoodJetCuts(Jet *jet){
    bool badJet = false;
    
    if(jet->PT < minJet_pt) badJet = true;
    if(fabs(jet->Eta) > maxJet_eta) badJet = true;
    
    return badJet;
}
bool Selection::JetCuts(vector <Jet*> goodJets){
    if (goodJets.size() < minJets || goodJets.size() > maxJets) return false;
    else{return true;}
}

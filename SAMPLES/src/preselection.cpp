/*
 * preselection.cpp
 *
 *  Created on: 24 Aug 2016
 *      Author: jkiesele
 */

#include "interface/preselection.h"
#include <cmath>

float DeltaR(float eta1, float phi1, float eta2, float phi2) {

    float dR=0.;
    float deta = std::fabs(eta1-eta2);
    float dphi = std::fabs(phi1-phi2);
    if(dphi>3.14159) dphi = 2.*3.14159-dphi;
    dR=std::sqrt(deta*deta+dphi*dphi);

    return dR;
}

void preselection::analyze(size_t childid /* this info can be used for printouts */){

    /*
     * This skeleton analyser runs directly on the Delphes output.
     * It can be used to create histograms directly or a skim.
     * If a skim is created, a new input configuration will be written automatically
     * and stored in the output directory together with the ntuples.
     * The skim can contain delphes objects again or can be flat. This is up
     * to the user.
     * Examples for both are given here.
     *
     * The same skeleton can be used to read the skim. Please refer to the comments
     * marked with "==SKIM=="
     *
     * These parts are commented, since the code is supposed to work as an example without
     * modifications on Delphes output directly.
     */



    /*
     * Define the branches that are to be considered for the analysis
     * This branch handler (notice the "d")
     * is used to run directly in Delphes output.
     * For skimmed ntuples, see below
     */
    d_ana::dBranchHandler<Electron>  elecs(tree(),"Electron");
    d_ana::dBranchHandler<Muon>      muons(tree(),"MuonTight");
    d_ana::dBranchHandler<Jet>       jets( tree(),"JetPUPPI");
    //d_ana::dBranchHandler<Jet>       taus( tree(),"Jet");
    //d_ana::dBranchHandler<Jet>       nonpuppijets(tree(),"Jet");
    d_ana::dBranchHandler<MissingET> mets( tree(),"PuppiMissingET");
    

    /* ==SKIM==
     *
     * If a skim of the Delphes outout was created in a way indicated
     * further below, use the tBranchHandler (please notive the "t")
     * to access vectors of objects...
     *
     */
    // d_ana::tBranchHandler<std::vector<Electron> > electrons(tree(),"Electrons");

    /*==SKIM==
     *
     * Or an object directly
     *
     */
    //d_ana::tBranchHandler<MissingET> met(tree(),"MET");



    /*
     * Always use this function to add a new histogram (can also be 2D)!
     * Histograms created this way are automatically added to the output file
     */
    //TH1* histo=addPlot(new TH1D("histoname1","histotitle1",100,0,100),"p_{T} [GeV]","N_{e}");


    /*
     * Helper variables 
     */
    Int_t    nElecs=0;
    Int_t   naElecs=0;
    Int_t    nMuons=0;
    Int_t   naMuons=0;
    Int_t     nTaus=0;
    Int_t     nJets=0;
    Int_t    nElecPt=0;
    Int_t nMuonPt=0;
    Int_t nTauPt=0;
    Double_t elecPt=0;
    Double_t muonPt=0;
    Double_t  tauPt=0;
    Double_t elecEta=0;
    Double_t muonEta=0;
    Double_t tauEta=0;
    Double_t elecCharge=0;
    Double_t muonCharge=0;


    size_t nevents=tree()->entries();
    if(isTestMode())
        nevents/=100;
    
    Int_t ee4jcounter   = 0;
    Int_t eegte5jcounter = 0;
    Int_t em4jcounter = 0;
    Int_t emgte5jcounter = 0;
    Int_t mm4jcounter = 0;
    Int_t mmgte5jcounter = 0;
    Int_t lllcounter = 0;
    Int_t lltcounter = 0;

    TH1 *data_obs = addPlot(new TH1I("data_obs", "Observed yields", 1, 0, 1), "Category", "Events");
    
    TH1 *ee4j = addPlot(new TH1F("ee4j", "2 electrons + 4 jets", 1, 0, 1), "Category", "Events");
    TH1 *eegte5j = addPlot(new TH1F("eegte5j", "2 electrons + #geq 5 jets", 1, 0, 1), "Category", "Events");
    TH1 *em4j = addPlot(new TH1F("em4j", "1 electron + 1 muon + 4 jets", 1, 0, 1), "Category", "Events");
    TH1 *emgte5j = addPlot(new TH1F("emgte5j", "1 electron + 1 muon + #geq 5 jets", 1, 0, 1), "Category", "Events");
    TH1 *mm4j = addPlot(new TH1F("mm4j", "2 muons + 4 jets", 1, 0, 1), "Category", "Events");
    TH1 *mmgte5j = addPlot(new TH1F("mmgte5j", "2 muons + #gte 5 jets", 1, 0, 1), "Category", "Events");
    TH1 *lll = addPlot(new TH1F("lll", "3 leptons", 1, 0, 1), "Category", "Events");
    TH1 *llt = addPlot(new TH1F("llt", "2 leptons + 1 #tau", 1, 0, 1), "Category", "Events");

    // produce an additional hist which contains all the events binned by category    
    TH1 *totals = addPlot(new TH1F("totals", "Events by Category", 8, 0, 8), "Category", "Events");

    // cut flow hist
    TH1 *cutFlow_ee4j = addPlot(new TH1F("cutFlow_ee4j", "ee4j Cut Flow", 0, 0, 0), "Cut", "Events");
    cutFlow_ee4j->SetCanExtend(TH1::kAllAxes);
    TH1 *cutFlow_llt = addPlot(new TH1F("cutFlow_llt", "llt Cut Flow", 0, 0, 0), "Cut", "Events");
    cutFlow_llt->SetCanExtend(TH1::kAllAxes);


    // kinematic variables plots
    TH1 *lepton_pt = addPlot(new TH1F("lep_pt", "Light lepton p_{T}", 30, 20, 100), "p_{T}", "Number");
    TH1 *lepton_eta = addPlot(new TH1F("lep_eta", "Light lepton #eta", 20, -2.5, 2.5), "#eta", "Number");
    TH1 *tau_pt = addPlot(new TH1F("tau_pt", "Tau p_{T}", 30, 20, 100), "p_{T}", "Number"); 
    TH1 *tau_eta = addPlot(new TH1F("tau_eta", "Tau #eta", 20, -2.5, 2.5), "#eta", "Number");
    TH1 *bjet_sep12 = addPlot(new TH1F("bjet_sep12", "DeltaR b/w bjet 1 and 2", 10, 0, 5), "#Delta R", "Number");
    TH1 *met = addPlot(new TH1F("met", "MET", 50, 0, 200), "MET (GeV)", "Number");
    //TH1 *tau_eta = addPlot(new TH1F("tau_eta", "Tau #eta", 20, -2.5, 2.5), "#eta", "Number");
    
    totals->GetXaxis()->SetBinLabel(1, "ee4j"        );          
    totals->GetXaxis()->SetBinLabel(2, "ee#geq5j"    );
    totals->GetXaxis()->SetBinLabel(3, "#mu#mu4j"    );
    totals->GetXaxis()->SetBinLabel(4, "#mu#mu#geq5j");
    totals->GetXaxis()->SetBinLabel(5, "e#mu4j"      );
    totals->GetXaxis()->SetBinLabel(6, "e#mu#geq5j"  );
    totals->GetXaxis()->SetBinLabel(7, "3l"          );
    totals->GetXaxis()->SetBinLabel(8, "2l1#tau"     );

    data_obs->Fill(0.5);
    //ee4j->Fill(0.5);
    //eegte5j->Fill(0.5);
    //em4j->Fill(0.5);
    //emgte5j->Fill(0.5);
    //mm4j->Fill(0.5);
    //mmgte5j->Fill(0.5);
    //lll->Fill(0.5);
    //llt->Fill(0.5);
    std::cout << "Number of events: " << nevents << std::endl;
    for(size_t eventno=0;eventno<nevents;eventno++){
        /*
         * The following two lines report the status and set the event link
         * Do not remove!
         */
        reportStatus(eventno,nevents);
        tree()->setEntry(eventno);

        nElecs=0;
        naElecs=0;
        nMuons=0;
        naMuons=0;
        nTaus =0; 
        nJets =0;
        nElecPt =0;
        nMuonPt=0;
        nTauPt=0;
        Double_t nBJets=0; //needs to be reset each loop

        /*
         * Perform pre-selection
         * Must have lepton Pts > 24
         */

        for(size_t i=0;i<elecs.size();i++){
            elecPt=elecs.at(i)->PT;
            elecEta=elecs.at(i)->Eta;
            elecCharge=elecs.at(i)->Charge;
            //std::cout << "electron charge is" << elecCharge << std::endl;
            if(elecPt < 25 || std::abs(elecEta) > 2.4) {
                continue;
            }
            nElecPt++;
            if (elecCharge == -1) nElecs++;
            if (elecCharge == 1) naElecs++;
            lepton_pt->Fill(elecPt);
            lepton_eta->Fill(elecEta);
        }

        for(size_t i=0;i<muons.size();i++){
            muonPt=muons.at(i)->PT;
            muonEta=muons.at(i)->Eta;
            muonCharge=muons.at(i)->Charge;
            if(muonPt < 25 || std::abs(muonEta) > 2.5) {
                continue;
            }
            nMuonPt++;
            if (muonCharge == -1) nMuons++;
            if (muonCharge == 1) naMuons++;
            lepton_pt->Fill(elecPt);
            lepton_eta->Fill(elecEta);
        }

        for(size_t i=0;i<mets.size();i++){
            MissingET *missinget = (MissingET*) mets.at(i);
            met->Fill(missinget->MET);
        }
            
            
        std::vector<Jet*> bjets;
        //std::cout << "jets has " << jets.size() << " elements" << std::endl;
        //std::cout << "nonpuppijets has " << nonpuppijets.size() << " elements" << std::endl;
        for(size_t i=0;i<jets.size();i++){
            Jet *jet = (Jet*) jets.at(i);
            //Jet *nonpuppijet = (Jet*) nonpuppijets.at(i);
            bool isTau = jet->TauTag > 0;
            bool isBJet = (jet->BTag) & 0x1;
            tauPt=jet->PT;
            tauEta=jet->Eta;
            if(!isTau) {
                if (tauPt > 25 && std::abs(tauEta) < 2.5){
                   if(isBJet){
                       nBJets += 1;
                       bjets.push_back(jet); }
                   nJets++;
                   continue;
                } 
                continue;
            }
            if (tauPt < 24 || tauEta > 2.5) continue; 
            nTauPt++;
            tau_pt->Fill(tauPt);
            tau_eta->Fill(tauEta);
            nTaus++;
        } 

        // use the regular jet collection as a tau collection
        //for (size_t i=0; i < taus.size(); i++) {
        //    Jet *jet = (Jet*) taus.at(i);
        //    bool isTau = jet->TauTag > 0;
        //    if (isTau) {
        //        tauPt = jet->PT;
        //        tauEta = jet->Eta;
        //        if (tauPt < 24 || tauEta > 2.5) continue;
        //        tau_pt->Fill(tauPt);
        //        tau_eta->Fill(tauEta);
        //        nTaus++;
        //    }
        //}



        // calculate deltaR between leading and subleading bjet
        for(size_t i=0;i<bjets.size();i++){
            if (bjets.size()>1) {
                //std::cout << "2 or more bjets!" << std::endl;
                Jet *bjet1 = bjets.at(0);
                Jet *bjet2 = bjets.at(1);
                float eta1 = bjet1->Eta;
                float phi1 = bjet1->Phi;
                float eta2 = bjet2->Eta;
                float phi2 = bjet2->Phi;
                float dr = DeltaR(eta1,phi1,eta2,phi2);
                bjet_sep12->Fill(dr);
            }
        }

        
                
                
        

        // PRESEL: Must have at least 1 lepton and 1 B jets
        if(nElecs + nMuons + nTaus < 1) continue;
        if (nBJets < 1) continue;

        // Cut flow for ee4j
        cutFlow_ee4j->Fill("total",1);
        if (nElecPt >= 2) {
            cutFlow_ee4j->Fill("nElec>=2,pT>25,eta<2.4",1);
            if (nJets >= 4) {
                cutFlow_ee4j->Fill("nJets>=4,pT>25,eta<2.5",1);
                if (nBJets >= 1) {
                    cutFlow_ee4j->Fill("nBJets>=1,pT>25,eta<2.5",1);
                    if (nTaus == 0) {
                        cutFlow_ee4j->Fill("no taus",1);
                        if (nElecs == 2 || naElecs == 2) {
                            cutFlow_ee4j->Fill("exactly 2 SS elecs",1);
                            if (nJets == 4) {
                                cutFlow_ee4j->Fill("exactly 4 jets",1);
                            }}}}}}
        cutFlow_ee4j->LabelsDeflate();
        cutFlow_ee4j->LabelsOption("u");

        // Cut flow for llt
        cutFlow_llt->Fill("total",1);
        if (nElecPt >= 2 || nMuonPt >= 2) {
            cutFlow_llt->Fill("nElec>=2,pT>25,eta<2.4||nMuon>=2,pT>25,eta<2.5",1);
            if (nJets >= 4) {
                cutFlow_llt->Fill("nJets>=4,pT>25,eta<2.5",1);
                if (nBJets >= 1) {
                    cutFlow_llt->Fill("nBJets>=1,pT>25,eta<2.5",1);
                    if (((nElecs + nMuons) == 2) ^ ((naMuons + naElecs) == 2)) {
                        cutFlow_llt->Fill("exactly 2 SS light leps",1);
                        if (nTaus >= 1) {
                            cutFlow_llt->Fill("exactly 1 tau",1);
                        }}}}}

        // Output channel counts and fills
        if ((nElecs == 2 || naElecs == 2) && nTaus == 0 && nJets == 4 && nBJets >= 1) {
           ee4jcounter++;
           ee4j->Fill(0.5);
           totals->Fill("ee4j", 1); }
        if ((nElecs == 2 || naElecs == 2) && nTaus == 0 && nJets >= 5 && nBJets >= 1) {
            eegte5jcounter++;
            eegte5j->Fill(0.5);
            totals->Fill("ee#geq5j", 1); }
        if (((nElecs + nMuons) == 2 || (naElecs + naMuons) == 2) && nJets == 4 && nBJets >= 1) {
            em4jcounter++;
            em4j->Fill(0.5);
            totals->Fill("e#mu4j", 1); }
        if (((nElecs + nMuons) == 2 || (naElecs + naMuons) == 2) && nJets >= 5 && nBJets >= 1) {
            emgte5jcounter++;
            emgte5j->Fill(0.5); 
            totals->Fill("e#mu#geq5j", 1);}
        if ((nMuons == 2 || naMuons == 2) && nJets == 4 && nBJets >= 1) {
            mm4jcounter++;
            mm4j->Fill(0.5); 
            totals->Fill("#mu#mu4j", 1);}
        if ((nMuons == 2 || naMuons == 2) && nJets >= 5 && nBJets >= 1) {
            mmgte5jcounter++;
            mmgte5j->Fill(0.5); 
            totals->Fill("#mu#mu#geq5j", 1);}
        if (((nMuons + nElecs + naMuons + naElecs) == 3) && ((nJets >= 3 && nBJets >= 2) || (nJets >= 4 && nBJets >= 1))) {
            lllcounter++;
            lll->Fill(0.5); 
            totals->Fill("3l", 1);}
        if (((nMuons + nElecs) == 2 || (naMuons + naElecs) == 2) && nTaus == 1 && nJets >= 4) {
            lltcounter++; 
            llt->Fill(0.5); 
            totals->Fill("2l1#tau", 1);}

    }                                        

    std::cout << "Number of ee>=5j: " << eegte5jcounter << std::endl;
    std::cout << "Number of emu4j: " << em4jcounter << std::endl;
    std::cout << "Number of emu>=5j: " << emgte5jcounter << std::endl;
    std::cout << "Number of mumu4j: " << mm4jcounter << std::endl;
    std::cout << "Number of mumu>=5j: " << mmgte5jcounter << std::endl; 
    std::cout << "Number of 3l: " << lllcounter << std::endl; 
    std::cout << "Number of llt: " << lltcounter << std::endl; 
    /*
     * Must be called in the end, takes care of thread-safe writeout and
     * call-back to the parent process
     */
    processEndFunction();
}

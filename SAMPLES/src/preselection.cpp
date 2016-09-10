/*
 * preselection.cpp
 *
 *  Created on: 24 Aug 2016
 *      Author: jkiesele
 */

#include "interface/preselection.h"


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
    //d_ana::dBranchHandler<MissingET> met(  tree(),"MissingET");

    //d_ana::dBranchHandler<Event>        events(      tree(),"Event");
    //d_ana::dBranchHandler<LHCOEvent>    lhcoevents(  tree(),"LHCOEvent");
    //d_ana::dBranchHandler<LHEFEvent>    lhefevents(  tree(),"LHEFEvent");
    //d_ana::dBranchHandler<LHEFWeight>   lhefweights( tree(),"LHEFWeight");
    //d_ana::dBranchHandler<HepMCEvent>   hepmcevents( tree(),"Event");
    //d_ana::dBranchHandler<GenParticle>  genparticles(tree(),"GenParticle");
    //d_ana::dBranchHandler<Vertex>       vertices(    tree(),"Vertex");
    //d_ana::dBranchHandler<ScalarHT>     scalarht(    tree(),"ScalarHT");
    //d_ana::dBranchHandler<Rho>          rho(         tree(),"Rho");
    //d_ana::dBranchHandler<Weight>       weights(     tree(),"Weight");
    //d_ana::dBranchHandler<Photon>       photons(     tree(),"Photon");
    d_ana::dBranchHandler<Jet>          jets(        tree(),"Jet");
    //d_ana::dBranchHandler<Track>        tracks(      tree(),"Track");
    //d_ana::dBranchHandler<Tower>        towers(      tree(),"Tower");
    //d_ana::dBranchHandler<HectorHit>    hectorhits(  tree(),"HectorHit");
    //d_ana::dBranchHandler<Candidate>    candidates(  tree(),"Candidate");

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
     * If (optionally) a skim or a flat ntuple is to be created, please use the following function to initialize
     * the tree.
     * The output files will be written automatically, and a config file will be created.
     */
    TTree* myskim=addTree();

    /*
     * Helper variables 
     */
    Int_t    nElecs=0;
    Int_t    nMuons=0;
    Int_t     nTaus=0;
    Int_t     nJets=0;
    Double_t elecPt=0;
    Double_t muonPt=0;
    Double_t  tauPt=0;

    /*
     * Or store a vector of objects (also possible to store only one object)
     */
    std::vector<Electron> skimmedelecs;
    std::vector<Muon>     skimmedmuons;
    std::vector<Jet>      skimmedtaus;

    myskim->Branch("Electrons", &skimmedelecs);
    myskim->Branch("Muons"    , &skimmedmuons);
    myskim->Branch("Taus"     , &skimmedtaus );

    /*
     * Additional branches to store other Delphes objects
     */
    //std::vector<Event>       skimmedevents;  
    //std::vector<LHCOEvent>   skimmedlhcoevents;
    //std::vector<LHEFEvent>   skimmedlhefevents;
    //std::vector<LHEFWeight>  skimmedlhefweights;
    //std::vector<HepMCEvent>  skimmedhepmcevents;
    //std::vector<GenParticle> skimmedgenparticles;
    //std::vector<Vertex>      skimmedvertices;
    //std::vector<ScalarHT>    skimmedscalarht;
    //std::vector<Rho>         skimmedrho;
    //std::vector<Weight>      skimmedweights;
    //std::vector<Photon>      skimmedphotons;
    std::vector<Jet>         skimmedjets;
    //std::vector<Track>       skimmedtracks;
    //std::vector<Tower>       skimmedtowers;
    //std::vector<HectorHit>   skimmedhectorhits;
    //std::vector<Candidate>   skimmedcandidates;
    //std::vector<MissingET>   skimmedmet;

    //myskim->Branch("Event"        , &skimmedevents);
    //myskim->Branch("LHCOEvent"    , &skimmedlhcoevents);
    //myskim->Branch("LHEFEvent"    , &skimmedlhefevents);
    //myskim->Branch("LHEFWeight"   , &skimmedlhefweights);
    //myskim->Branch("Event"        , &skimmedhepmcevents);
    //myskim->Branch("GenParticle"  , &skimmedgenparticles);
    //myskim->Branch("Vertex"       , &skimmedvertices);
    //myskim->Branch("ScalarHT"     , &skimmedscalarht);
    //myskim->Branch("Rho"          , &skimmedrho);
    //myskim->Branch("Weight"       , &skimmedweights);
    //myskim->Branch("Photon"       , &skimmedphotons);
    myskim->Branch("Jet"          , &skimmedjets);
    //myskim->Branch("Track"        , &skimmedtracks);
    //myskim->Branch("Tower"        , &skimmedtowers);
    //myskim->Branch("HectorHit"    , &skimmedhectorhits);
    //myskim->Branch("Candidate"    , &skimmedcandidates);
    //myskim->Branch("MissingET"          , &skimmedmet);



    size_t nevents=tree()->entries();
    if(isTestMode())
        nevents/=100;
    
    Int_t ee4jcounter   = 0;
    Int_t eegte5jcounter = 0;
    Int_t em4jcounter = 0;
    Int_t emgte5jcounter = 0;
    Int_t mm4jcounter = 0;
    Int_t mmgte5jcounter = 0;

    for(size_t eventno=0;eventno<nevents;eventno++){
        /*
         * The following two lines report the status and set the event link
         * Do not remove!
         */
        reportStatus(eventno,nevents);
        tree()->setEntry(eventno);

        nElecs=0;
        nMuons=0;
        nTaus =0; 
        nJets =0;

        /*
         * Perform pre-selection
         * Must have lepton Pts > 20
         */

        skimmedelecs.clear();
        for(size_t i=0;i<elecs.size();i++){
            elecPt=elecs.at(i)->PT;
            if(elecPt < 20) {
                continue;
            }
            nElecs++;
            skimmedelecs.push_back(*elecs.at(i));
        }

        skimmedmuons.clear();
        for(size_t i=0;i<muons.size();i++){
            muonPt=muons.at(i)->PT;
            if(muonPt < 20) {
                continue;
            }
            nMuons++;
            skimmedmuons.push_back(*muons.at(i));
        }

        skimmedtaus.clear();
        for(size_t i=0;i<jets.size();i++){
            bool isTau=(jets.at(i)->TauTag>>2) & 0x1;
            tauPt=jets.at(i)->PT;
            if(tauPt < 20 || !isTau) {
                continue;
            }
            nTaus++;
            skimmedtaus.push_back(*jets.at(i));
        }


        // PRESEL: Must have at least 1 lepton
        if(nElecs + nMuons + nTaus < 1) continue;

        /*
         * Begin the event-by-event storage 
         */

        //skimmedevents.clear();  
        //skimmedlhcoevents.clear();
        //skimmedlhefevents.clear();
        //skimmedlhefweights.clear();
        //skimmedhepmcevents.clear();
        //skimmedgenparticles.clear();
        //skimmedvertices.clear();
        //skimmedscalarht.clear();
        //skimmedrho.clear();
        //skimmedweights.clear();
        //skimmedphotons.clear();
        skimmedjets.clear();
        //skimmedtracks.clear();
        //skimmedtowers.clear();
        //skimmedhectorhits.clear();
        //skimmedcandidates.clear();
        //skimmedmet.clear();

        //for(size_t i=0; i<events.size(); i++)       skimmedevents.push_back(*events.at(i));
        //for(size_t i=0; i<lhcoevents.size(); i++)   skimmedlhcoevents.push_back(*lhcoevents.at(i));
        //for(size_t i=0; i<lhefevents.size(); i++)   skimmedlhefevents.push_back(*lhefevents.at(i));
        //for(size_t i=0; i<lhefweights.size(); i++)  skimmedlhefweights.push_back(*lhefweights.at(i));
        //for(size_t i=0; i<hepmcevents.size(); i++)  skimmedhepmcevents.push_back(*hepmcevents.at(i));
        //for(size_t i=0; i<genparticles.size(); i++) skimmedgenparticles.push_back(*genparticles.at(i));
        //for(size_t i=0; i<vertices.size(); i++)     skimmedvertices.push_back(*vertices.at(i));
        //for(size_t i=0; i<scalarht.size(); i++)     skimmedscalarht.push_back(*scalarht.at(i));
        //for(size_t i=0; i<rho.size(); i++)          skimmedrho.push_back(*rho.at(i));
        //for(size_t i=0; i<weights.size(); i++)      skimmedweights.push_back(*weights.at(i));
        //for(size_t i=0; i<photons.size(); i++)      skimmedphotons.push_back(*photons.at(i));
        for(size_t i=0; i<jets.size(); i++)         skimmedjets.push_back(*jets.at(i));
        //for(size_t i=0; i<tracks.size(); i++)       skimmedtracks.push_back(*tracks.at(i));
        //for(size_t i=0; i<towers.size(); i++)       skimmedtowers.push_back(*towers.at(i));
        //for(size_t i=0; i<hectorhits.size(); i++)   skimmedhectorhits.push_back(*hectorhits.at(i));
        //for(size_t i=0; i<candidates.size(); i++)   skimmedcandidates.push_back(*candidates.at(i));
        //for(size_t i=0; i<met.size(); i++)          skimmedmet.push_back(*met.at(i));

        // Probably not the correct way to get nJets
        nJets = jets.size() - nTaus;
        // Output channel counts
        if (nElecs == 2 && nJets == 4) ee4jcounter++;
        if (nElecs == 2 && nJets >= 5) eegte5jcounter++;
        if (nElecs == 1 && nMuons == 1 && nJets == 4) em4jcounter++;
        if (nElecs == 1 && nMuons == 1 && nJets >= 5) emgte5jcounter++;
        if (nMuons == 2 && nJets == 4) mm4jcounter++;
        if (nMuons == 2 && nJets >= 5) mmgte5jcounter++;

        myskim->Fill();                       
    }                                        
    
    std::cout << "Number of ee4j: " << std::to_string(ee4jcounter) << std::endl;
    std::cout << "Number of ee>=5j: " << std::to_string(eegte5jcounter) << std::endl;
    std::cout << "Number of emu4j: " << std::to_string(em4jcounter) << std::endl;
    std::cout << "Number of emu>=5j: " << std::to_string(emgte5jcounter) << std::endl;
    std::cout << "Number of mumu4j: " << std::to_string(mm4jcounter) << std::endl;
    std::cout << "Number of mumu>=5j: " << std::to_string(mmgte5jcounter) << std::endl; 

    /*
     * Must be called in the end, takes care of thread-safe writeout and
     * call-back to the parent process
     */
    processEndFunction();
}

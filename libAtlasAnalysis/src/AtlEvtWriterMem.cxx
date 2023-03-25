//____________________________________________________________________
//
// Event writer for MemNtupls used by AtlSelector
// 
//
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2013 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtWriterMem
#include <AtlEvtWriterMem.h>
#endif
#include <AtlBTag.h>

#ifndef __CINT__
ClassImp(AtlEvtWriterMem);
#endif

//____________________________________________________________________

AtlEvtWriterMem::AtlEvtWriterMem() {
    //
    // Default constructor
    //
    fElectrons = 0;
    fMuons     = 0;
    fJets      = 0;;
    fBJets     = 0;;
}

//____________________________________________________________________

AtlEvtWriterMem::~AtlEvtWriterMem() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlEvtWriterMem::BookTree(TTree *tree, AtlEvent *evt) {
    //
    // Book output MemNtuple
    //
    fTree  = tree;
    fEvent = evt;

    // ===============
    // Create branches
    // ===============

    // Evt header
    fTree->Branch("run_nr",     &fRunNr,       "run_nr/I");
    fTree->Branch("evt_nr",     &fEventNr,     "evt_nr/I");
    fTree->Branch("evt_weight", &fEventWeight, "evt_weight/F");

    // Missng Et
    fTree->Branch("met_et",  &fMET_Et,  "met_et/F");
    fTree->Branch("met_phi", &fMET_Phi, "met_phi/F");
    
    // Electrons
    fTree->Branch("el_n",  &fElectronN,   "el_n/I");
    fTree->Branch("el_e",   fElectronE,   "el_e[el_n]/F");
    fTree->Branch("el_pt",  fElectronPt,  "el_pt[el_n]/F");
    fTree->Branch("el_eta", fElectronEta, "el_eta[el_n]/F");
    fTree->Branch("el_phi", fElectronPhi, "el_phi[el_n]/F");
    fTree->Branch("el_chg", fElectronChg, "el_chg[el_n]/I");

    // Muons
    fTree->Branch("mu_n",  &fMuonN,   "mu_n/I");
    fTree->Branch("mu_e",   fMuonE,   "mu_e[mu_n]/F");
    fTree->Branch("mu_pt",  fMuonPt,  "mu_pt[mu_n]/F");
    fTree->Branch("mu_eta", fMuonEta, "mu_eta[mu_n]/F");
    fTree->Branch("mu_phi", fMuonPhi, "mu_phi[mu_n]/F");
    fTree->Branch("mu_chg", fMuonChg, "mu_chg[mu_n]/I");

    // Jets
    fTree->Branch("jet_n",      &fJetN,          "jet_n/I");
    fTree->Branch("jet_e",       fJetE,          "jet_e[jet_n]/F");
    fTree->Branch("jet_pt",      fJetPt,         "jet_pt[jet_n]/F");
    fTree->Branch("jet_eta",     fJetEta,        "jet_eta[jet_n]/F");
    fTree->Branch("jet_phi",     fJetPhi,        "jet_phi[jet_n]/F");
    fTree->Branch("jet_btagged", fJetBTagged,    "jet_btagged[jet_n]/O");
    fTree->Branch("jet_btagw",   fJetBTagWeight, "jet_btagw[jet_n]/F");
    fTree->Branch("jet_flav",    fJetFlav,       "jet_flav[jet_n]/B");
}

//____________________________________________________________________

void AtlEvtWriterMem::WriteEvent() {
    //
    // Write event
    //

    // =============
    // Fill branches
    // =============

    // Evt header
    fRunNr       = fEvent->RunNr();
    fEventNr     = fEvent->EventNr();
    fEventWeight = fEvent->GetTagEvtWeight();
    
    // Electrons
    TIter next_el(fElectrons);
    AtlElectron *el = 0;
    fElectronN = 0;
    while ( (el =  (AtlElectron*)next_el()) ) {
	fElectronE[fElectronN]   = el->E();
	fElectronPt[fElectronN]  = el->Pt();
	fElectronEta[fElectronN] = el->Eta();
	fElectronPhi[fElectronN] = el->Phi();
	fElectronChg[fElectronN] = ( el->IsPositive() ) ? 1 : -1;
	fElectronN++;
    }

    // Muons
    TIter next_mu(fMuons);
    AtlMuon *mu = 0;
    fMuonN = 0;
    while ( (mu =  (AtlMuon*)next_mu()) ) {
	fMuonE[fMuonN]   = mu->E();
	fMuonPt[fMuonN]  = mu->Pt();
	fMuonEta[fMuonN] = mu->Eta();
	fMuonPhi[fMuonN] = mu->Phi();
	fMuonChg[fMuonN] = ( mu->IsPositive() ) ? 1 : -1;
	fMuonN++;
    }

    // Jets
    TIter next_jet(fJets);
    AtlJet *jet = 0;
    AtlJet::EFlavour flavour = AtlJet::kUnknownFlavour;
    fJetN = 0;
    while ( (jet =  (AtlJet*)next_jet()) ) {
	fJetE[fJetN]          = jet->E();
	fJetPt[fJetN]         = jet->Pt();
	fJetEta[fJetN]        = jet->Eta();
	fJetPhi[fJetN]        = jet->Phi();
	fJetBTagWeight[fJetN] = jet->GetTag(fBTagger)->GetWeight();
	fJetBTagged[fJetN]    = ( fBJets->Contains(jet) );
	flavour = jet->GetTruthFlavour();
	switch ( flavour ) {
	    case AtlJet::kUnknownFlavour:
		fJetFlav[fJetN] = 'u';
		break;
	    case AtlJet::kLightFlavoured:
		fJetFlav[fJetN] = 'l';
		break;
	    case AtlJet::kCharm:
		fJetFlav[fJetN] = 'c';
		break;
	    case AtlJet::kBeauty:
		fJetFlav[fJetN] = 'b';
		break;
	    case AtlJet::kTau:
		fJetFlav[fJetN] = 't';
		break;
	    default:
		Error("WriteEvent", "Unknown jet truth flavour. Abort!");
		gSystem->Abort(1);
	}
	fJetN++;
    }
    
    // Missing Et
    fMET_Et  = fEvent->GetEnergySum()->MissingEt_Mag();
    fMET_Phi = fEvent->GetEnergySum()->MissingEt_Phi();
    
    // ===================
    // Write event to tree
    // ===================
    fTree->Fill();
}

//____________________________________________________________________

void AtlEvtWriterMem::SetElectrons(TList *electrons) {
    //
    // Set list of selected electrons
    //
    fElectrons = electrons;
}

//____________________________________________________________________

void AtlEvtWriterMem::SetMuons(TList *muons) {
    //
    // Set list selected muons
    //
    fMuons = muons;
}

//____________________________________________________________________

void AtlEvtWriterMem::SetJets(TList *jets, TList *bjets) {
    //
    // Set list of selected jets and b-jets
    //
    fJets  = jets;
    fBJets = bjets;
}

//____________________________________________________________________

void AtlEvtWriterMem::SetBTagger(AtlBTag::ETagger BTagger) {
    //
    // Set b-tagger type to be used for storing the b-tag weights for
    // each jet
    //
    fBTagger  = BTagger;
}

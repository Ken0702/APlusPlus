//____________________________________________________________________
//
// Single-Top Validation Analysis 
//
//
// Input: some A++ tree or chain
//
// Ouput: .root file containing validation histograms
//
// Usage:
// 
// {
//   TChain *ch = new TChain("MyInputTree");
//   ch->Add("MyInputFiles*.root");
//   AtlWtChallenge *sel = new AtlWtChallenge("MyOutputFile.root");
//   ch->Process(sel, "CTRL");
// }
//
//  
// Author: Michelangelo Giorgi <mailto: strider@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlWtChallenge
#include <AtlWtChallenge.h>
#endif
#include <TString.h>
#include <TH1F.h>
#include <TH2F.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlWtChallenge);
#endif

//____________________________________________________________________

AtlWtChallenge::AtlWtChallenge(const char* OutputFile) :
    AtlSelector(OutputFile) {
    //
    // Default constructor
    //

    // Init
    fJets    = 0;
    fBJets   = 0;
    fLeptons = 0;
    fElectrons   = 0;
    fMuons       = 0;
    fVetoLeptons = 0;

    fMET.Set(0., 0.);
    fRecoW = 0;
    fVertexTracksPresent = kFALSE; 
    fBadJetsPresent      = kFALSE;
    fElMuTrackMatched    = kFALSE;
	
    SetCutDefaults();

}

//____________________________________________________________________

AtlWtChallenge::~AtlWtChallenge() {
  //
  // Default destructor
  //
}

//____________________________________________________________________

void AtlWtChallenge::SetBranchStatus() {
    //
    // Switch on/off branches in order to gain speed
    //
    
    // Switch off all branches by default
    fTree->SetBranchStatus("*", kFALSE);
    
    // Switch on only branches we are going to use
    // This will speed up the analysis process considerably
    fTree->SetBranchStatus("fEventHeader*", kTRUE);
    fTree->SetBranchStatus("fTrigger*",     kTRUE);
}

//____________________________________________________________________

void AtlWtChallenge::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    fLeptonMode = kMuonMode;//kElectronMode, kMuonMode
    
    fEleTriggerChoice = "EF_e20_medium";
    fMuTriggerChoice  = "EF_mu18";
    fWmassT_min     = 60.;    
    fMET_Mag_min    = 25.;
    fElPt_Min       = 25.;
    fMuPt_Min       = 20.;
    fJets_Pt_min    = 25.;
    fJets_Eta_min   = -4.5;
    fJets_Eta_max   = 4.5;
    fBJets_Pt_min    = 35.;
    fBJets_Eta_min  = -2.5;
    fBJets_Eta_max  = 2.5;
}

//____________________________________________________________________

void AtlWtChallenge::BookHistograms() {
    //
    // Book histograms
    //
    // This example shows also how to create sub-folders inside a
    // .root file in order to group the large amount of histograms one
    // usually has
    //
    
    // Save present working directory
    // (only relevant when creating sub-directories)
    fOutputFile->cd();

    TDirectory *savdir = gDirectory;
    
    gDirectory->mkdir("AtlWtChallenge","Single-Top Validation Analysis");
    gDirectory->cd("AtlWtChallenge");
        
    // Cut-flow, electrons
    fHistCutFlow_e = new TH1F("h_cutflow_e", "Cut-Flow (Electrons)",
			      24, 0, 24);
    //common sgtop
    fHistCutFlow_e->GetXaxis()->SetBinLabel(1,  "(C0)  Total/GRL");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(2,  "(C1)  Trigger Passed");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(3,  "(C2)  Vtx Cut");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(4,  "Loose  Tree");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(5,  "(C3)  Tight Electron");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(6,  "(C4)  Veto Lepton");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(7,  "(C5)  Trigger Match");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(8,  "(C6)  e/#mu OR");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(9,  "(C7)  Bad Jets Cleaning");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(10, "(C8)  Etmiss");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(11, "(C9)  ElectronPt > 25 GeV");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(12, "(C10) +2 jets bin, P_{t} > 25 GeV, |#eta| < 4.5");

    // Wt Selection Challenge
    
    // C11 (Triangle cut)
    fHistCutFlow_e->GetXaxis()->SetBinLabel(13, "Triangle(C11)");
    // MTWs
    fHistCutFlow_e->GetXaxis()->SetBinLabel(14, "(Underflow) M_{T}(W) > 60 > GeV @ -1j bin");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(15, "(C11B2) M_{T}(W) > 60 GeV & 2 jets");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(16, "(C11B3) M_{T}(W) > 60 GeV & 3 jets");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(17, "(C11B4) M_{T}(W) > 60 GeV & 4 jets");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(18, "(OverFlow) M_{T}(W) > 60 GeV & +5 jets");

    // C12 (+1 b-tagging)
    // b-tag bins
    fHistCutFlow_e->GetXaxis()->SetBinLabel(19, "b-tag > 35 GeV, |#eta| <2.5 @ (C12)");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(20, "(Underflow) b-tag > 35 GeV, |#eta| <2.5 @ -1j bin");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(21, "(C12B2) b-tag > 35 GeV, |#eta| <2.5 @ 2j bin");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(22, "(C12B3) b-tag > 35 GeV, |#eta| <2.5 @ 3j bin");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(23, "(C12B4) b-tag > 35 GeV, |#eta| <2.5 @ 4j bin");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(24, "(OverFlow) b-tag > 35 GeV, |#eta| <2.5 @ +5j bin");
    
    fHistCutFlow_e->SetYTitle("Number of Events");

    
    // Cut-flow, muons
    fHistCutFlow_mu = new TH1F("h_cutflow_mu", "Cut-Flow (Muons)",
			      24, 0, 24);
    //common sgtop
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(1,  "Total/GRL (C0)");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(2,  "Trigger Passed (C1)");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(3,  "Vtx Cut (C2)");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(4,  "Loose Tree");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(5,  "Tight Electron (C3)");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(6,  "Veto Lepton (C4)");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(7,  "Trigger Match (C5)");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(8,  "e/#mu OR (C6)");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(9,  "Bad Jets Cleaning (C7)");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(10, "Etmiss  (C8)");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(11, "ElectronPt > 25 GeV (C9)");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(12, "+2 jets bin, P_{t} > 25 GeV, |#eta| < 4.5 (C10)");

    // Wt Selection Challenge
    
    // C11 (Triangle cut)
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(13, "Triangle(C11)");
    // MTWs
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(14, "(Underflow) M_{T}(W) > 60 > GeV @ -1j bin");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(15, "(C11B2) M_{T}(W) > 60 GeV & 2 jets");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(16, "(C11B3) M_{T}(W) > 60 GeV & 3 jets");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(17, "(C11B4) M_{T}(W) > 60 GeV & 4 jets");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(18, "(OverFlow) M_{T}(W) > 60 GeV & +5 jets");

    // C12 (+1 b-tagging)
    // b-tag bins
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(19, "b-tag > 35 GeV, |#eta| <2.5 @ (C12)");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(20, "(Underflow) b-tag > 35 GeV, |#eta| <2.5 @ -1j bin");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(21, "(C12B2) b-tag > 35 GeV, |#eta| <2.5 @ 2j bin");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(22, "(C12B3) b-tag > 35 GeV, |#eta| <2.5 @ 3j bin");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(23, "(C12B4) b-tag > 35 GeV, |#eta| <2.5 @ 4j bin");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(24, "(OverFlow) b-tag > 35 GeV, |#eta| <2.5 @ +5j bin");

    
    fHistCutFlow_mu->SetYTitle("Number of Events");

    // Restore pwd
    savdir->cd();
    
    // Open a file for output of event numbers
    gDirectory->cd("../../../tasks");
    if ( fLeptonMode == kElectronMode )
	fOut.open("logfile_e");
    if ( fLeptonMode == kMuonMode )
	fOut.open("logfile_mu");

    // Restore pwd
    savdir->cd();
}



//____________________________________________________________________

void AtlWtChallenge::InitEvent() {
    //
    // Prepare event analysis
    //

    //
    // Obtain Objects from Object Definitions Tool
    //
    // AtlAnalysisTool *tool = 0;
    // TIter next_tool(fListOfTools);    
    // next_tool.Reset();
    // while ( (tool = (AtlAnalysisTool*)next_tool()) ) {
    // 	if( ((TString)tool->ClassName()).Contains("AtlSgTopObjectsDefinitionTool") ){
    // 	    fMET      = ((AtlSgTopObjectsDefinitionTool*)tool)->GetEtmiss();
    // 	    fJets     = ((AtlSgTopObjectsDefinitionTool*)tool)->GetJets();
    // 	    fBJets    = ((AtlSgTopObjectsDefinitionTool*)tool)->GetBJets();
    // 	    fLeptons  = ((AtlSgTopObjectsDefinitionTool*)tool)->GetLeptons();
    // 	    fElectrons = ((AtlSgTopObjectsDefinitionTool*)tool)->GetElectrons();
    // 	    fMuons     = ((AtlSgTopObjectsDefinitionTool*)tool)->GetMuons();
    // 	    fVetoLeptons  = ((AtlSgTopObjectsDefinitionTool*)tool)->GetVetoLeptons();
    // 	    fVertexTracksPresent =
    // 		((AtlSgTopObjectsDefinitionTool*)tool)->GetVertexTracksPresent();
    // 	    fBadJetsPresent =
    // 		((AtlSgTopObjectsDefinitionTool*)tool)->GetBadJetsPresent();
    // 	    fElMuTrackMatched =
    // 		((AtlSgTopObjectsDefinitionTool*)tool)->GetElMuTrackMatched();
    // 	}
    // }


    //
    // Additional topological Cuts on Jets
    //

    // ===========================
    // Refine list of generic jets
    // ===========================
    TIter next_jet(fJets);
    AtlJet *jet = 0;

    // Remove low energetic jets and non-central jets from the list
    while ( (jet = (AtlJet*)next_jet()) ) {
	if ( 
	    ( jet->JES_Pt() <= fJets_Pt_min )
 	    || ( ( jet->EMScaleEta() + jet->EMJES_EtaCorr() ) < fJets_Eta_min ) 
 	    || ( ( jet->EMScaleEta() + jet->EMJES_EtaCorr() ) > fJets_Eta_max ) 
	    )
	    fJets->Remove(jet);
    }
    
    // Sort list
    fJets->Sort(kSortDescending);



    // ===========================
    // Get refined list of b-jets 
    // ===========================
    TIter next_bjet(fBJets);
    AtlJet *bjet = 0;

    // Remove low energetic jets and non-central jets from the list
    while ( (bjet = (AtlJet*)next_bjet()) ) {
	if ( 
	    ( bjet->JES_Pt() <= fBJets_Pt_min ) 
	    || ( ( bjet->EMScaleEta() + bjet->EMJES_EtaCorr() )  < fBJets_Eta_min )
	    || ( ( bjet->EMScaleEta() + bjet->EMJES_EtaCorr() )  > fBJets_Eta_max )
	    )
	    fBJets->Remove(bjet);
    }

    // Sort list
    fBJets->Sort(kSortDescending);

}

//____________________________________________________________________


void AtlWtChallenge::ReconstructW() {
    //
    // reconstruction of W transverse 4-momentum and mass
    //

    if ( fRecoW != 0 ) {
	delete fRecoW;
	fRecoW = 0;
    }
    
    // Require at least a leading lepton
    HepParticle *Lepton = (HepParticle*)fLeptons->At(0);
    if ( Lepton != 0 ) {
	// Get neutrino 4-vector by means of Et_miss	
	TLorentzVector p_nu(fMET.Px(),
			    fMET.Py(),
			    0.,fMET.Mod());
	
	// Get (transverse) leading lepton 4-momentum
	TLorentzVector p_lep = Lepton->P();
	if ( Lepton->IsElectron() ) {
	    // customized electron 4-momentum
	    Float_t cluster_e = ((AtlElectron*)Lepton)->ClusterE();
	    Float_t track_eta = ((AtlElectron*)Lepton)->GetIDTrack()->Eta();
	    Float_t track_phi = ((AtlElectron*)Lepton)->GetIDTrack()->Phi();
	    Float_t cosh_track_eta = TMath::CosH((Double_t)track_eta);
	    p_lep.SetPtEtaPhiE(
		cluster_e/cosh_track_eta, // pt
		track_eta,                // eta
		track_phi,                // phi
		cluster_e);               // e
	}
	p_lep.SetE(p_lep.Et());
	p_lep.SetPz(0.);
	
	// Reconstruct (transverse) W 4-momentum
	TLorentzVector p_W = p_nu + p_lep;

	// Which W?
	Int_t Wpdg = -24;
	if ( Lepton->IsPositive() ) Wpdg = 24;
	fRecoW = new HepParticle(1,p_W.Px(),p_W.Py(),p_W.Pz(),p_W.E(), Wpdg);

	//
	// Try to only reconstruct transverse W mass
	//
	fWmassT = TMath::Sqrt(
	    2*p_lep.Pt()*fMET.Mod()
	    *(1. -
	      TMath::Cos(TVector2::Phi_mpi_pi((Double_t) (p_lep.Phi() - fMET.Phi())))));

    }

}

//____________________________________________________________________

Bool_t AtlWtChallenge::ProcessCut() {
    //
    // Event selection
    //
    // In this example only events containing at least one muon with a
    // transverse momentum higher than 10 GeV are chosen
    //
    return kTRUE;
}


//____________________________________________________________________

void AtlWtChallenge::FillHistograms() {
    //
    // Fill histograms
    //
    Double_t MET_Mag = fMET.Mod();

//     Double_t met = fEvent->GetEnergySum()->GetRefFinalEM_etight_MissingEt().Mod();
//     cout << endl << "fEvent->GetEnergySum()->GetRefFinalEM_etight_MissingEt().Mod() = "
// 	 << met << endl << "MET_Mag = " << MET_Mag << endl;

    // ================
    // Electron Channel
    // ================
    
    if ( fLeptonMode == kElectronMode ) {
	
	// Total number of events (0)
	fHistCutFlow_e->Fill(0.5, 1.);

	// Trigger (1)
	if ( fEvent->HasPassedHLT(fEleTriggerChoice) == kFALSE ) return;
	fHistCutFlow_e->Fill(1.5, 1.);
	
	// Minimum number of tracks from primary or pile-up vertex? (C2)
	if ( !fVertexTracksPresent ) return;    
	fHistCutFlow_e->Fill(2.5, 1.);
	
	// Loose electron or muon
	if ( fVetoLeptons->GetEntries() == 0 ) return;
	fHistCutFlow_e->Fill(3.5, 1.);
	
	// Tight electron (3)
	if ( fElectrons->GetEntries() != 1 ) return;
	fHistCutFlow_e->Fill(4.5, 1.);
	
	// Veto on 2nd tight lepton (4)
	if ( fLeptons->GetEntries() > 1 ) return;
	fHistCutFlow_e->Fill(5.5, 1.);

	// Trigger Match (C5)
	if ( !fEvent->HasMatchedHLT((AtlElectron*)fElectrons->At(0), fEleTriggerChoice.Data()) && (fEvent->IsDATA()) ) return;
	fHistCutFlow_e->Fill(6.5, 1.);
	
	// No electron-muon overlaps (C6)
	if ( fElMuTrackMatched == kTRUE ) return; 
	fHistCutFlow_e->Fill(7.5, 1.);
	
	// Bad jets in event? (C7)
	if ( fBadJetsPresent ) return;
	fHistCutFlow_e->Fill(8.5, 1.);
		
	// Etmiss (C8)
	if ( MET_Mag <= fMET_Mag_min  ) return;  // correct
	fHistCutFlow_e->Fill(9.5, 1.);	
	
	// in Tree: other event selection criteria
	// ElectronPt
	AtlElectron *el = ((AtlElectron*)fElectrons->At(0));
	if ( ( el->ClusterE() / TMath::CosH((Double_t)el->GetIDTrack()->Eta()) )
	     <= fElPt_Min ) return;    
	fHistCutFlow_e->Fill(10.5, 1.);

	// At least 2 jet
	if ( fJets->GetEntries() < 2 ) return;  
	fHistCutFlow_e->Fill(11.5, 1.);    
	
		
	// -----------------------------------------------------
	//
	// C11
	//
	// Triangle cut: Mt(W) + Etmiss > 60 GeV?
	
	//
	// Simple W-boson reconstruction
	//
	ReconstructW();
	
	if ( (fWmassT + MET_Mag) <= fWmassT_min  ) return;

	fHistCutFlow_e->Fill(12.5, 1.);

              // Binwise working
	Int_t n_GoodJets=0;
	
	AtlJet *myjet =0;
	for (int i = 0; i < fJets->GetEntries(); i++) {
	    myjet =(AtlJet*)fJets->At(i);
		   
	    if ( (myjet->EMScaleEta() + myjet->EMJES_EtaCorr() < 2.5 )
		 &&
	         (myjet->EMScaleEta() + myjet->EMJES_EtaCorr() > -2.5 ) )

		n_GoodJets++;
	}
              // C11 B1-2-3
	
	if ( n_GoodJets  < 2) {fHistCutFlow_e->Fill(13.5, 1.); }
	if ( n_GoodJets == 2) {fHistCutFlow_e->Fill(14.5, 1.); }
	if ( n_GoodJets == 3) {fHistCutFlow_e->Fill(15.5, 1.); }
	if ( n_GoodJets == 4) {fHistCutFlow_e->Fill(16.5, 1.); }
	if ( n_GoodJets  > 4) {fHistCutFlow_e->Fill(17.5, 1.); }

	// -----------------------------------------------------
	//
	// C12
	//
	// b-tagging cut: existing fBjets list
	// (its existence means 2.5-eta range & 35 GeV
	// 
	
	if ( fBJets->GetEntries() != 1 ) return;
	fHistCutFlow_e->Fill(18.5, 1.); 
	
	// C12 B1-2-3
	if (n_GoodJets  < 2) {fHistCutFlow_e->Fill(19.5, 1.); }
	if (n_GoodJets == 2) {fHistCutFlow_e->Fill(20.5, 1.); }
	if (n_GoodJets == 3) {fHistCutFlow_e->Fill(21.5, 1.); }
	if (n_GoodJets == 4) {fHistCutFlow_e->Fill(22.5, 1.); }
	if (n_GoodJets  > 4) {fHistCutFlow_e->Fill(23.5, 1.); }	

    }



    
    // ============
    // Muon Channel
    // ============
    
    if ( fLeptonMode == kMuonMode ) {
	
	// Total number of events
	fHistCutFlow_mu->Fill(0.5, 1.);

	// Trigger
	if ( fEvent->HasPassedHLT(fMuTriggerChoice) == kFALSE ) return;
	fHistCutFlow_mu->Fill(1.5, 1.);

	// Minimum number of tracks from primary or pile-up vertex?
	if ( !fVertexTracksPresent ) return;    
	fHistCutFlow_mu->Fill(2.5, 1.);
	
	// Loose electron or muon 
	if ( fVetoLeptons->GetEntries() == 0 ) return;
	fHistCutFlow_mu->Fill(3.5, 1.);

	// Tight muon	
	if ( fMuons->GetEntries() != 1 ) return;
	fHistCutFlow_mu->Fill(4.5, 1.);

	// Veto on 2nd tight lepton
	if ( fLeptons->GetEntries() > 1 ) return;
	fHistCutFlow_mu->Fill(5.5, 1.);

 	// Trigger-Match
	if ( !fEvent->HasMatchedHLT((AtlMuon*)fMuons->At(0), fMuTriggerChoice.Data()) ) return;
	fHistCutFlow_mu->Fill(6.5, 1.);
	
	// No electron-muon overlaps
	if ( fElMuTrackMatched == kTRUE ) return;
	fHistCutFlow_mu->Fill(7.5, 1.);

	// Bad jets in event?
	if ( fBadJetsPresent ) return;
	fHistCutFlow_mu->Fill(8.5, 1.);
	
	// Etmiss
	if ( MET_Mag <= fMET_Mag_min  ) return;    
	fHistCutFlow_mu->Fill(9.5, 1.);
		
	// in Tree: other event selection criteria
	// ElectronPt
	AtlMuon *mu = ((AtlMuon*)fMuons->At(0));
	if ( mu->Pt() 
	     <= fMuPt_Min ) return;    
	fHistCutFlow_mu->Fill(10.5, 1.);
	
	// At least 1 jet
	if ( fJets->GetEntries() < 2 ) return;	
	fHistCutFlow_mu->Fill(11.5, 1.);    
		
	// Triangle cut: Mt(W) + Etmiss > 60 GeV?

	//
	// Simple W-boson reconstruction
	//
	ReconstructW();

	if ( (fWmassT + MET_Mag) <= 60.  ) return;
	fHistCutFlow_mu->Fill(12.5, 1.);
	
              // Binwise working
	Int_t n_GoodJets=0;
	
	AtlJet *myjet =0;
	for (int i = 0; i < fJets->GetEntries(); i++) {
	    myjet =(AtlJet*)fJets->At(i);
		   
	    if ( (myjet->EMScaleEta() + myjet->EMJES_EtaCorr() < 2.5 )
		 &&
	         (myjet->EMScaleEta() + myjet->EMJES_EtaCorr() > -2.5 ) )

		n_GoodJets++;
	}
              // C11 B1-2-3
              // C11 B1-2-3
	
	if ( n_GoodJets  < 2) {fHistCutFlow_mu->Fill(13.5, 1.); }
	if ( n_GoodJets == 2) {fHistCutFlow_mu->Fill(14.5, 1.); }
	if ( n_GoodJets == 3) {fHistCutFlow_mu->Fill(15.5, 1.); }
	if ( n_GoodJets == 4) {fHistCutFlow_mu->Fill(16.5, 1.); }
	if ( n_GoodJets  > 4) {fHistCutFlow_mu->Fill(17.5, 1.); }
	
	// -----------------------------------------------------
	//
	// C12
	//
	// b-tagging cut: existing fBjets list
	// (its existence means 2.5-eta range & 35 GeV
	// 
	
	if ( fBJets->GetEntries() != 1 ) return;
	fHistCutFlow_mu->Fill(18.5, 1.); 

      // C12 B1-2-3
	if (n_GoodJets  < 2) {fHistCutFlow_mu->Fill(19.5, 1.); }
	if (n_GoodJets == 2) {fHistCutFlow_mu->Fill(20.5, 1.); }
	if (n_GoodJets == 3) {fHistCutFlow_mu->Fill(21.5, 1.); }
	if (n_GoodJets == 4) {fHistCutFlow_mu->Fill(22.5, 1.); }
	if (n_GoodJets  > 4) {fHistCutFlow_mu->Fill(23.5, 1.); }	

}

}

//____________________________________________________________________

void AtlWtChallenge::Terminate() {
    //
    // Terminate job
    //

    // Close output text file
    fOut.close(); 

    fHistCutFlow_e->Print("all");
    fHistCutFlow_mu->Print("all");
    
    AtlSelector::Terminate();
}

//____________________________________________________________________

void AtlWtChallenge::Print() const {
    //
    // Prints chosen parameter settings for the validation analysis
    //

    cout << endl
	 << "========================================================" << endl
	 << "  Single-Top t-Channel Validation Analysis              " << endl
	 << "========================================================" << endl
	 << "  fLeptonMode                = ";
    if ( fLeptonMode == kElectronMode ) cout << "kElectronMode" << endl;
    if ( fLeptonMode == kMuonMode )     cout << "kMuonMode" << endl;
    cout << "--------------------------------------------------------" << endl 
	 << "  Specifications of physics objects:                    " << endl
	 << "--------------------------------------------------------" << endl
	 << "  fEleTriggerChoice          = " << fEleTriggerChoice << endl
	 << "  fMuTriggerChoice           = " << fMuTriggerChoice << endl
	 << "                                                        " << endl
	 << "  fElectron_Pt_min           = " << fElPt_Min << endl
	 << "--------------------------------------------------------" << endl
	 << "  Minimum Etmiss is  fMET_Mag_min = " << fMET_Mag_min << endl
	 << "                                                        " << endl
	 << "  Minimum Transverse fWmassT_min  = " << fWmassT_min  << endl
	 << "                                                        " << endl
	 << "  fJets_Pt_min               = " << fJets_Pt_min << endl
	 << "  fJets_Eta_min              = " << fJets_Eta_min << endl
	 << "  fJets_Eta_max              = " << fJets_Eta_max << endl   
	 << "  fBJets_Et_min              = " << fJets_Pt_min << endl
	 << "  fBJets_Pt_min              = " << fBJets_Pt_min << endl
	 << "  fBJets_Eta_min             = " << fBJets_Eta_min << endl
	 << "  fBJets_Eta_max             = " << fBJets_Eta_max << endl
	 << "========================================================" << endl
	 << endl;
}

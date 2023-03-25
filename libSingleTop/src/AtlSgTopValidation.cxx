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
//   AtlSgTopValidation *sel = new AtlSgTopValidation("MyOutputFile.root");
//   ch->Process(sel, "CTRL");
// }
//
//  
// Author: Ruth Herrberg <mailto: ruthh@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlSgTopValidation
#include <AtlSgTopValidation.h>
#endif
#include <TString.h>
#include <TH1F.h>
#include <TH2F.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlSgTopValidation);
#endif

//____________________________________________________________________

AtlSgTopValidation::AtlSgTopValidation(const char* OutputFile) :
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

AtlSgTopValidation::~AtlSgTopValidation() {
  //
  // Default destructor
  //
}

//____________________________________________________________________

void AtlSgTopValidation::SetBranchStatus() {
    //
    // Switch on/off branches in order to gain speed
    //
    
    // Switch off all branches by default
//    fTree->SetBranchStatus("*", kFALSE);
    fTree->SetBranchStatus("*", kTRUE);

    // Switch on only branches we are going to use
    // This will speed up the analysis process considerably
//     fTree->SetBranchStatus("fEventHeader*",         kTRUE);
// //    fTree->SetBranchStatus("fTrigger*",             kTRUE);
//     fTree->SetBranchStatus("fN_Electrons*",         kTRUE);
//     fTree->SetBranchStatus("fElectrons*",           kTRUE);
//     fTree->SetBranchStatus("fN_Muons*",             kTRUE);
//     fTree->SetBranchStatus("fMuons*",               kTRUE);
//     fTree->SetBranchStatus("fEnergySum*",           kTRUE);
//     fTree->SetBranchStatus("fN_AntiKt4TopoEMJets*", kTRUE);
//     fTree->SetBranchStatus("fAntiKt4TopoEMJets*",   kTRUE);
//     fTree->SetBranchStatus("fN_IDTracks*",          kTRUE);  
//     fTree->SetBranchStatus("fIDTracks*",            kTRUE);
//     fTree->SetBranchStatus("fN_Vertices*",          kTRUE);
//     fTree->SetBranchStatus("fVertices*",            kTRUE);
}

//____________________________________________________________________

void AtlSgTopValidation::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    fInputMode = kD3PDSgTop;
    
    fLeptonMode = kElectronMode;//kElectronMode, kMuonMode
    
    fEleTriggerChoice = "EF_e20_medium";
    fMuTriggerChoice  = "EF_mu18";
    
    fMET_Mag_min    = 25.;
    fElPt_Min       = 25.;
    fJets_Pt_min    = 25.;
    fJets_Eta_min   = -4.5;
    fJets_Eta_max   = 4.5;
    fBJets_Eta_min  = -2.5;
    fBJets_Eta_max  = 2.5;
}

//____________________________________________________________________

void AtlSgTopValidation::BookHistograms() {
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
    
    gDirectory->mkdir("AtlSgTopValidation","Single-Top Validation Analysis");
    gDirectory->cd("AtlSgTopValidation");
        
    // Cut-flow, electrons
    fHistCutFlow_e = new TH1F("h_cutflow_e", "Cut-Flow (Electrons)",
			      13, 0, 13);
    fHistCutFlow_e->GetXaxis()->SetBinLabel(1, "Total");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(2, "GRL");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(3, "LAr Error");
//    fHistCutFlow_e->GetXaxis()->SetBinLabel(2, "Bad Jets");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(4, "Vtx Cut");
//     fHistCutFlow_e->GetXaxis()->SetBinLabel(4, "Loose e");
//     fHistCutFlow_e->GetXaxis()->SetBinLabel(5, "Loose mu");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(5, "Loose Tree");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(6, "Tight Electron");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(7, "Veto Lepton");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(8, "Etmiss");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(9, "in Tree");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(10, "ElectronPt");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(11, "Trigger Passed");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(12, "Triangle");
    fHistCutFlow_e->GetXaxis()->SetBinLabel(13, "BTag");
    fHistCutFlow_e->SetYTitle("Number of Events");
    
    // Cut-flow, muons
    fHistCutFlow_mu = new TH1F("h_cutflow_mu", "Cut-Flow (Muons)",
			      12, 0, 12);
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(1, "Total");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(2, "Bad Jets");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(3, "Vtx Cut");
//     fHistCutFlow_mu->GetXaxis()->SetBinLabel(4, "Loose e");
//     fHistCutFlow_mu->GetXaxis()->SetBinLabel(5, "Loose mu");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(4, "Loose Tree");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(5, "Tight Muon");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(6, "Veto Lepton");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(7, "Etmiss");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(8, "in Tree");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(9, "Trigger Passed");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(10, "Trigger Matched");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(11, "Triangle");
    fHistCutFlow_mu->GetXaxis()->SetBinLabel(12, "BTag");
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

void AtlSgTopValidation::InitEvent() {
    //
    // Prepare event analysis
    //

    //
    // Obtain Objects from Object Definitions Tool
    //
//     AtlAnalysisTool *tool = 0;
//     TIter next_tool(fListOfTools);    
//     next_tool.Reset();
//     while ( (tool = (AtlAnalysisTool*)next_tool()) ) {
// 	if( ((TString)tool->ClassName()).Contains("AtlTopCommonObjectsTool") ){
// 	    fMET      = ((AtlTopCommonObjectsTool*)tool)->GetEtmiss();
// 	    fJets     = ((AtlTopCommonObjectsTool*)tool)->GetJets();
// 	    fBJets    = ((AtlTopCommonObjectsTool*)tool)->GetBJets();
// 	    fLeptons  = ((AtlTopCommonObjectsTool*)tool)->GetLeptons();
// 	    fElectrons = ((AtlTopCommonObjectsTool*)tool)->GetElectrons();
// 	    fMuons     = ((AtlTopCommonObjectsTool*)tool)->GetMuons();
// 	    fVetoLeptons  = ((AtlTopCommonObjectsTool*)tool)->GetVetoLeptons();
// 	    fVertexTracksPresent =
// 		((AtlTopCommonObjectsTool*)tool)->GetVertexTracksPresent();
// 	    fBadJetsPresent =
// 		((AtlTopCommonObjectsTool*)tool)->GetBadJetsPresent();
// 	    fElMuTrackMatched =
// 		((AtlTopCommonObjectsTool*)tool)->GetElMuTrackMatched();
//    	}
//     }


    //
    // Additional topological Cuts on Jets
    //

    // ===========================
    // Refine list of generic jets
    // ===========================
//     TIter next_jet(fJets);
//     AtlJet *jet = 0;

//     // Remove low energetic jets and non-central jets from the list
//     while ( (jet = (AtlJet*)next_jet()) ) {
// 	if ( 
// 	    ( jet->JES_Pt() <= fJets_Pt_min )
//  	    || ( ( jet->EMScaleEta() + jet->EMJES_EtaCorr() ) < fJets_Eta_min ) 
//  	    || ( ( jet->EMScaleEta() + jet->EMJES_EtaCorr() ) > fJets_Eta_max ) 
// 	    )
// 	    fJets->Remove(jet);
//     }
    
//     // Sort list
//     fJets->Sort(kSortDescending);



//     // ===========================
//     // Get refined list of b-jets 
//     // ===========================
//     TIter next_bjet(fBJets);
//     AtlJet *bjet = 0;

//     // Remove low energetic jets and non-central jets from the list
//     while ( (bjet = (AtlJet*)next_bjet()) ) {
// 	if ( 
// 	    ( bjet->JES_Pt() <= fJets_Pt_min ) 
// 	    || ( ( bjet->EMScaleEta() + bjet->EMJES_EtaCorr() )  < fBJets_Eta_min )
// 	    || ( ( bjet->EMScaleEta() + bjet->EMJES_EtaCorr() )  > fBJets_Eta_max )
// 	    )
// 	    fBJets->Remove(bjet);
//     }

//     // Sort list
//     fBJets->Sort(kSortDescending);

}

//____________________________________________________________________


void AtlSgTopValidation::ReconstructW() {
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

Bool_t AtlSgTopValidation::ProcessCut() {
    //
    // Event selection
    //
    // In this example only events containing at least one muon with a
    // transverse momentum higher than 10 GeV are chosen
    //
    return kTRUE;
}


//____________________________________________________________________

void AtlSgTopValidation::FillHistograms() {
    //
    // Fill histograms
    //

    // Etmiss
//     Double_t MET_Mag = fMET.Mod();
    Double_t MET_Mag = fEvent->GetEnergySum()->RefFinalEM_etight_MissingEt_Mag();
    
//     Double_t met = fEvent->GetEnergySum()->GetRefFinalEM_etight_MissingEt().Mod();
//     cout << endl << "fEvent->GetEnergySum()->GetRefFinalEM_etight_MissingEt().Mod() = "
// 	 << met << endl << "MET_Mag = " << MET_Mag << endl;

    // ================
    // Electron Channel
    // ================
    
    if ( fLeptonMode == kElectronMode ) {
	
	// Total number of events
	fHistCutFlow_e->Fill(0.5, 1.);

	// GRL
	if ( (fEvent->LumiBlock() < 61) || (fEvent->LumiBlock() > 80) ) return;
	fHistCutFlow_e->Fill(1.5, 1.);

	// HFOR
	
	
// 	// Bad jets in event?
// 	if ( fBadJetsPresent ) return;
// 	fHistCutFlow_e->Fill(1.5, 1.);
	
	// LArError in event?
	fHistCutFlow_e->Fill(2.5, 1.);
		
	// Minimum number of tracks from primary or pile-up vertex?
	HepVertex *vtx = (HepVertex*)fEvent->GetVertices()->At(0);
	if ( vtx == 0 ) return;
	if ( !( (vtx->IsPrimary() || vtx->IsPileUp()) && (vtx->GetNDaughters() > 4) ) )
	    return;    
// 	if ( !fVertexTracksPresent ) return;    
	fHistCutFlow_e->Fill(3.5, 1.);
	
	// Loose electron or muon
// 	if ( fVetoLeptons->GetEntries() == 0 ) return;
// 	fHistCutFlow_e->Fill(4.5, 1.);
	
	//***!hier weiter!***
	// Tight electron
// 	if ( fElectrons->GetEntries() != 1 ) return;
// 	fHistCutFlow_e->Fill(5.5, 1.);
	
// 	// Veto on 2nd tight lepton
// 	if ( fLeptons->GetEntries() > 1 ) return;
// 	fHistCutFlow_e->Fill(6.5, 1.);
	
// 	// Etmiss
// //    if ( MET_Mag < fMET_Mag_min  ) return;
// 	if ( fEvent->EventNr() == 61628 ) {
// 	    cout << "This event doesn't survive Etmiss cut" << endl;
// 	    fEvent->Print("ALL");
// 	    Double_t met = fMET.Mod();
// 	    cout << endl << "fEvent->GetEnergySum()->GetRefFinalEM_etight_MissingEt().Mod() = "
// 		 << met << endl;
// 	}
	    
// 	if ( MET_Mag <= fMET_Mag_min  ) return;  // correct
// 	fHistCutFlow_e->Fill(7.5, 1.);

// 	if ( fEvent->EventNr() == 61628 )
// 	    cout << "This event is not supposed to survive Etmiss cut...??" << endl;

// 	fOut << "EventNumber " << fEvent->EventNr() << endl;
	
	
// 	// in Tree: other event selection criteria
	
// 	// No electron-muon overlaps
// 	if ( fElMuTrackMatched == kTRUE ) return; 
// 	// At least 1 jet
// 	if ( fJets->GetEntries() < 1 ) return;  
// 	fHistCutFlow_e->Fill(8.5, 1.);    
	
// 	// ElectronPt
// 	AtlElectron *el = ((AtlElectron*)fElectrons->At(0));
// 	if ( ( el->ClusterE() / TMath::CosH((Double_t)el->GetIDTrack()->Eta()) )
// 	     <= fElPt_Min ) return;    
// 	fHistCutFlow_e->Fill(9.5, 1.);
	
// 	// Trigger
// 	if ( fEvent->HasPassedHLT(fEleTriggerChoice) == kFALSE ) return;
// 	fHistCutFlow_e->Fill(10.5, 1.);
	
	
// 	// Triangle cut: Mt(W) + Etmiss > 60 GeV?
	
// 	//
// 	// Simple W-boson reconstruction
// 	//
// 	ReconstructW();
	
// 	if ( (fWmassT + MET_Mag) <= 60.  ) return;
// 	// or
// //	if ( (fRecoW->Mperp() + MET_Mag) <= 60.  ) return;
// 	fHistCutFlow_e->Fill(11.5, 1.);
	

// 	// Exactly 1 B-tag
// 	if ( fBJets->GetEntries() != 1 ) return;
// //    if ( fBJets->GetEntries() == 0 ) return;
// //	// Exactly 2 jets (for t-channel) AND 1 B-tag
// //    if ( fJets->GetEntries() != 2 ) return;

// 	fHistCutFlow_e->Fill(12.5, 1.);
    }



    
    // ============
    // Muon Channel
    // ============
    
    if ( fLeptonMode == kMuonMode ) {
	
	// Total number of events
	fHistCutFlow_mu->Fill(0.5, 1.);
	
	// Bad jets in event?
	if ( fBadJetsPresent ) return;
	fHistCutFlow_mu->Fill(1.5, 1.);
	
    
	// Minimum number of tracks from primary or pile-up vertex?
	if ( !fVertexTracksPresent ) return;    
	fHistCutFlow_mu->Fill(2.5, 1.);
	
	// Loose electron or muon 
	if ( fVetoLeptons->GetEntries() == 0 ) return;
	fHistCutFlow_mu->Fill(3.5, 1.);
	
	// Tight muon

// 	//===========look at 2-jet-bin========
// 	if ( fJets->GetEntries() != 2 ) return;
//	//===========look at 2-jet-bin========
	
	if ( fMuons->GetEntries() != 1 ) return;
	fHistCutFlow_mu->Fill(4.5, 1.);

	fOut << "EventNumber " << fEvent->EventNr() << endl;
	
	// Veto on 2nd tight lepton
	if ( fLeptons->GetEntries() > 1 ) return;
	fHistCutFlow_mu->Fill(5.5, 1.);
	
	// Etmiss
	if ( MET_Mag <= fMET_Mag_min  ) return;    
	fHistCutFlow_mu->Fill(6.5, 1.);
	
	
	// in Tree: other event selection criteria
	
	// No electron-muon overlaps
	if ( fElMuTrackMatched == kTRUE ) return;
	// At least 1 jet
	if ( fJets->GetEntries() < 1 ) return;	
	fHistCutFlow_mu->Fill(7.5, 1.);    
	
	
	// Trigger
	if ( fEvent->HasPassedHLT(fMuTriggerChoice) == kFALSE ) return;
	fHistCutFlow_mu->Fill(8.5, 1.);
	
	// Trigger-Match
	if ( !fEvent->HasMatchedHLT((AtlMuon*)fMuons->At(0), fMuTriggerChoice.Data()) ) return;
	fHistCutFlow_mu->Fill(9.5, 1.);
	
	// Triangle cut: Mt(W) + Etmiss > 60 GeV?

	//
	// Simple W-boson reconstruction
	//
	ReconstructW();

	if ( (fWmassT + MET_Mag) <= 60.  ) return;
	// or
//	if ( (fRecoW->Mperp() + MET_Mag) <= 60.  ) return;
	fHistCutFlow_mu->Fill(10.5, 1.);
	
	// Exactly 1 B-tag
	if ( fBJets->GetEntries() != 1 ) return;    
	fHistCutFlow_mu->Fill(11.5, 1.);
    }

}

//____________________________________________________________________

void AtlSgTopValidation::Terminate() {
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

void AtlSgTopValidation::Print() const {
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
	 << "  fJets_Pt_min               = " << fJets_Pt_min << endl
	 << "  fJets_Eta_min              = " << fJets_Eta_min << endl
	 << "  fJets_Eta_max              = " << fJets_Eta_max << endl   
	 << "  fBJets_Et_min              = " << fJets_Pt_min << endl
	 << "  fBJets_Eta_min             = " << fBJets_Eta_min << endl
	 << "  fBJets_Eta_max             = " << fBJets_Eta_max << endl
	 << "========================================================" << endl
	 << endl;
}

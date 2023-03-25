 //____________________________________________________________________
//
// Phi decay finder class
//
// Phi will be reconstructed while looping over all two track combinations
// which are leaving the primary vertex. For the signal the leaving Kaon tracks
// have to be oppositely charged (added to Phi-List in AtlEvent), same
// charged tracks will be put in a TList called fBkgPhiDecays for the
// background.
// The selection can be done by a cut based method or a kinemativ fitter.
// Switching is possible by a Flag EMode (kCutBased, kKinFit) that can
// be set in the constructor via SetMode(EMode).
// Some Cut Values have to be set external, look at SetCutDefaults.
// You can also run in a Debug mode to get more information about your
// analysis.
//

//
// Author: Sophia Schade <mailto:schade@physik.hu-berlin.de>
// Copyright: 2010 (C) Sophia Schade
//
#ifndef ATLAS_AtlPhiFinder
#include <AtlPhiFinder.h>
#endif
#include <TH1F.h>
#include <AtlEvent.h>
#include <TTree.h>
#include <TString.h>
#include <iostream>
#include <TROOT.h>
#include <TFile.h>
#include <vector>
#include <iomanip>

using namespace std;

#ifndef __CINT__
ClassImp(AtlPhiFinder);
#endif

//____________________________________________________________________

AtlPhiFinder::AtlPhiFinder(const char* name, const char* title) :
    AtlKinFitterTool(name, title) {
    //
    // Default constructor
    //
    SetMode(kKinFit);           // kCutBased or kKinFit
    SetDebugOutput(kFALSE);     // Debug output
    fUseKaonPurity = kFALSE;
    fBkgPhiDecays = new TList;  // Bkg List
}

//____________________________________________________________________

AtlPhiFinder::~AtlPhiFinder() {
    //
    // Default destructor
    //
    fBkgPhiDecays->Delete(); delete fBkgPhiDecays;
    delete fBetheBloch;
    
    if (IsDebugRun() ) fDebugOutput.close();
}

//____________________________________________________________________

void AtlPhiFinder::SetBranchStatus() {
    //
    // Switch on needed branches
    //
    fTree->SetBranchStatus("fN_IDTracks*",         kTRUE);  
    fTree->SetBranchStatus("fIDTracks*",           kTRUE);
    fTree->SetBranchStatus("fN_Vertices*",         kTRUE);
    fTree->SetBranchStatus("fVertices*",           kTRUE);
    fTree->SetBranchStatus("fN_MCParticles*",      kTRUE);
    fTree->SetBranchStatus("fMCParticles*",        kTRUE);
    fTree->SetBranchStatus("fEventHeader*",        kTRUE);
}

//____________________________________________________________________

void AtlPhiFinder::BookHistograms() {
    //
    // Book histograms
    //
    
    // Debugging
    if ( IsDebugRun() ) fDebugOutput.open("DebugOutput.dat");
    
    // Book KinFitter Histograms
    AtlKinFitterTool::BookHistograms();
    AtlKinFitterTool::SetTruthMatchXTitle("M_{#Phi}");
    AtlKinFitterTool::SetTruthMatchBinning(30, 1, 30);
    
    PrintCutValues();
    
    // ======================
    // Book signal histograms
    // ======================
    gDirectory->mkdir("Signal", "Oppositely charged candidates");
    gDirectory->cd("Signal");

    // general tuning histograms
    fHistKaonMatchingProb = new TH1F("h_Kaon_MatchingProb",
				     "Matching probability (hitbased TruthMatching) of particles matching kaons",
				     50, 0, 1);
    fHistKaonMatchingProb->SetXTitle("MatchingProb");
    fHistKaonMatchingProb->SetYTitle("Number of Entries");
    
    fHistTrkChi2ovNDoF  = new TH1F("h_Trk_Chi2overNDoF",
				   "Chi2/NDoF of track fit",
				   50, 0, 5);
    fHistTrkChi2ovNDoF->SetXTitle("Chi2ovNDoF");
    fHistTrkChi2ovNDoF->SetYTitle("Number of Entries");
    fHistVtxChi2ovNDoF  = new TH1F("h_Vertex_Chi2overNDoF",
				   "Chi2/NDoF of vertex fit",
				   50, 0, 5);
    fHistVtxChi2ovNDoF->SetXTitle("Chi2ovNDoF");
    fHistVtxChi2ovNDoF->SetYTitle("Number of Entries");

    // Phi -> KK
    // ----------

    gDirectory->mkdir("KK", "Phi -> K+ + K-");
    gDirectory->cd("KK");

    if ( fMode == kKinFit ) {
	fHistPhireco_m_KK   = new TH1F("h_Phi_m_KK", "Invariant #Phi mass (#Phi->KK)",
				       1000, 1.018, 1.021);
    }else if (fMode == kCutBased){
	fHistPhireco_m_KK   = new TH1F("h_Phi_m_KK", "Invariant #Phi mass (#Phi->KK)",
				       100, 0.95, 1.1);
    }
    fHistPhireco_m_KK->SetXTitle("m_{#Phi} (GeV)");
    fHistPhireco_m_KK->SetYTitle("Number of Entries");
    fHistPhireco_pt_KK  = new TH1F("h_Phi_pt_KK", "#Phi-p_{#perp} (#Phi->KK)",
				   200, 0, 30);
    fHistPhireco_pt_KK->SetXTitle("p_{#perp} (GeV)");
    fHistPhireco_pt_KK->SetYTitle("Number of Entries");
    fHistPhireco_phi_KK = new TH1F("h_Phi_phi_KK", "#Phi #phi distribution (#Phi->KK)",
				   128, -3.2, 3.2);
    fHistPhireco_phi_KK->SetXTitle("#phi (rad)");
    fHistPhireco_phi_KK->SetYTitle("Number of Entries");
    fHistPhireco_eta_KK = new TH1F("h_Phi_eta_KK", "#Phi #eta distribution (#Phi->KK)",
				   50, -3, 3);
    fHistPhireco_eta_KK->SetXTitle("#eta");
    fHistPhireco_eta_KK->SetYTitle("Number of Entries");
    fHistPhireco_N_KK   = new TH1F("h_Phi_N_KK", "Reconstructed #Phi 's per event (#Phi->KK)",
				   10, 0, 10);
    fHistPhireco_N_KK->SetXTitle("N_{Phi}/evt");
    fHistPhireco_N_KK->SetYTitle("Number of Entries");

    // trks and their particle
    fHistPhireco_TRK   = new TH1F("h_Phi_TRK", "Particle of Signal Trks",
				   100, 0, 100);
    fHistPhireco_TRK->SetXTitle("Particle");
    fHistPhireco_TRK->SetYTitle("Number of Entries");

    // Kaon (K+) histograms in sub-folder
    gDirectory->mkdir("Kplus", "K+ from Phi -> KK");
    gDirectory->cd("Kplus");
    
    fHistPhireco_KPlus_pt = new TH1F("h_Phi_KPlus_pt", "K^{+}-p_{#perp} (#Phi->KK)",
				     200, 0, 20);
    fHistPhireco_KPlus_pt->SetXTitle("p_{#perp} (GeV)");
    fHistPhireco_KPlus_pt->SetYTitle("Number of Entries");
    fHistPhireco_KPlus_eta = new TH1F("h_Phi_KPlus_eta", "K^{+}:  #eta distribution (#Phi->KK)",
				      50, -3, 3);
    fHistPhireco_KPlus_eta->SetXTitle("#eta");
    fHistPhireco_KPlus_eta->SetYTitle("Number of Entries");
    fHistPhireco_KPlus_phi = new TH1F("h_Phi_KPlus_phi", "K^{+}:  #phi distribution (#Phi->KK)",
				      50, -3.14, 3.14);
    fHistPhireco_KPlus_phi->SetXTitle("#phi");
    fHistPhireco_KPlus_phi->SetYTitle("Number of Entries");
    
    gDirectory->cd("../"); 
    
    // Kaon (K-) histograms in sub-folder
    gDirectory->mkdir("KMinus", "K- from Phi -> KK");
    gDirectory->cd("KMinus");
    
    fHistPhireco_KMinus_pt = new TH1F("h_Phi_KMinus_pt", "K^{-}-p_{#perp} (#Phi->KK)",
				      200, 0, 20);
    fHistPhireco_KMinus_pt->SetXTitle("p_{#perp} (GeV)");
    fHistPhireco_KMinus_pt->SetYTitle("Number of Entries");
    fHistPhireco_KMinus_eta = new TH1F("h_Phi_KMinus_eta", "K^{-}:  #eta distribution (#Phi->KK)",
				       50, -5, 5);
    fHistPhireco_KMinus_eta->SetXTitle("#eta");
    fHistPhireco_KMinus_eta->SetYTitle("Number of Entries");
    fHistPhireco_KMinus_phi = new TH1F("h_Phi_KMinus_phi", "K^{-}:  #phi distribution (#Phi->KK)",
				       50, -3.14, 3.14);
    fHistPhireco_KMinus_phi->SetXTitle("#phi");
    fHistPhireco_KMinus_phi->SetYTitle("Number of Entries");

    // Angle between Kaon tracks in KK folder
    gDirectory->cd("../");

    fHistPhireco_KK_OpeningAngle = new TH1F("h_Phi_KK_opneingangle", "#alpha between  KK",
				     50, 0, 3.14);
    fHistPhireco_KK_OpeningAngle->SetXTitle("#alpha (rad)");
    fHistPhireco_KK_OpeningAngle->SetYTitle("Number of Entries");
    
    fHistPhireco_DecayAngle_KK = new TH1F("h_Phi_KK_decayangle", "#alpha between  KK",
				     50, 0, 3.14);
    fHistPhireco_DecayAngle_KK->SetXTitle("#alpha (rad)");
    fHistPhireco_DecayAngle_KK->SetYTitle("Number of Entries");

    // Difference between fore- and background
    fHistPhireco_m_KK_diff = new TH1F("h_Phi_M_diff", "Phi mass (fg-bg)", 100, 0.95, 1.1);
    fHistPhireco_m_KK_diff->SetXTitle("m_{#Phi} (GeV)");
    fHistPhireco_m_KK_diff->SetYTitle("Number of Entries");

    // dE/dx
    fHistdEdx_pmag_sig = new TH2F("h_dEdx_sig", "dE/dx as a function of momentum (sig)", 1000, GetLogBins(1000, -1.0, 3.0), 100, 0.0, 5.0);
    fHistdEdx_pmag_sig->SetXTitle("p (GeV)");
    fHistdEdx_pmag_sig->SetYTitle("dE/dx");

    fHistdEdx_diff_sig = new TH1F("h_dEdx_diff_sig", "difference between measured and calculated dEdx for signal", 1000, 0.0, 5.0);
    fHistdEdx_diff_sig->SetXTitle("#Delta dE/dx");
    fHistdEdx_diff_sig->SetYTitle("Number of Entries");
    
    gDirectory->cd("../../");

    // ===================
    // Book bkg histograms
    // ===================
 
    gDirectory->mkdir("Background", "Same charged candidates");
    gDirectory->cd("Background");
    

    // Phi -> KK 
    // ----------

    gDirectory->mkdir("KK", "Phi -> K+K+ / K-K-");
    gDirectory->cd("KK");

    if ( fMode == kKinFit ) {
	fHistPhireco_m_KK_bkg   = new TH1F("h_Phi_m_KK_bkg", "Invariant #Phi mass (#Phi->KK)",
					   1000, 1.018, 1.021);
    }else if (fMode == kCutBased){
	fHistPhireco_m_KK_bkg   = new TH1F("h_Phi_m_KK_bkg", "Invariant #Phi mass (#Phi->KK)",
					   100, 0.95, 1.1);
    }
    fHistPhireco_m_KK_bkg->SetXTitle("m_{#Phi} (GeV)");
    fHistPhireco_m_KK_bkg->SetYTitle("Number of Entries");
    fHistPhireco_pt_KK_bkg  = new TH1F("h_Phi_pt_KK_bkg", "#Phi-p_{#perp} (#Phi->KK)",
				       200, 0, 30);
    fHistPhireco_pt_KK_bkg->SetXTitle("p_{#perp} (GeV)");
    fHistPhireco_pt_KK_bkg->SetYTitle("Number of Entries");
    fHistPhireco_phi_KK_bkg = new TH1F("h_Phi_phi_KK_bkg", "#Phi #phi distribution (#Phi->KK)",
				       128, -3.2, 3.2);
    fHistPhireco_phi_KK_bkg->SetXTitle("#phi (rad)");
    fHistPhireco_phi_KK_bkg->SetYTitle("Number of Entries");
    fHistPhireco_eta_KK_bkg = new TH1F("h_Phi_eta_KK_bkg", "#Phi #eta distribution (#Phi->KK)",
				       50, -3, 3);
    fHistPhireco_eta_KK_bkg->SetXTitle("#eta");
    fHistPhireco_eta_KK_bkg->SetYTitle("Number of Entries");
    fHistPhireco_N_KK_bkg   = new TH1F("h_Phi_N_KK_bkg", "Reconstructed #Phi' per event (Phi->KK)",
				       10, 0, 10);
    fHistPhireco_N_KK_bkg->SetXTitle("N_{#Phi}/evt");
    fHistPhireco_N_KK_bkg->SetYTitle("Number of Entries");
    
    // Kaon (K+) histograms in sub-folder
    gDirectory->mkdir("Kplus", "K+ from Phi -> K+K+");
    gDirectory->cd("Kplus");
    
    fHistPhireco_KPlus_pt_bkg = new TH1F("h_Phi_KPlus_pt_bkg", "K^{+}-p_{#perp} (#Phi->KK)",
					 100, 0, 30);
    fHistPhireco_KPlus_pt_bkg->SetXTitle("p_{#perp} (GeV)");
    fHistPhireco_KPlus_pt_bkg->SetYTitle("Number of Entries");
    fHistPhireco_KPlus_eta_bkg = new TH1F("h_Phi_KPlus_eta_bkg", "K^{+}:  #eta distribution (#Phi->KK)",
					  50, -3, 3);
    fHistPhireco_KPlus_eta_bkg->SetXTitle("#eta");
    fHistPhireco_KPlus_eta_bkg->SetYTitle("Number of Entries");
    fHistPhireco_KPlus_phi_bkg = new TH1F("h_Phi_KPlus_phi_bkg", "K^{+}:  #phi distribution (#Phi->KK)",
					  50, -3.14, 3.14);
    fHistPhireco_KPlus_phi_bkg->SetXTitle("#phi");
    fHistPhireco_KPlus_phi_bkg->SetYTitle("Number of Entries");
    
    gDirectory->cd("../"); 
    
    // Kaon (K-) histograms in sub-folder
    gDirectory->mkdir("KMinus", "K- from Phi0 -> K-K-");
    gDirectory->cd("KMinus");
    
    fHistPhireco_KMinus_pt_bkg = new TH1F("h_Phi_KMinus_pt_bkg", "K^{-}-p_{#perp} (#Phi->KK)",
					  100, 0, 30);
    fHistPhireco_KMinus_pt_bkg->SetXTitle("p_{#perp} (GeV)");
    fHistPhireco_KMinus_pt_bkg->SetYTitle("Number of Entries");
    fHistPhireco_KMinus_eta_bkg = new TH1F("h_Phi_KMinus_eta_bkg", "K^{-}:  #eta distribution (#Phi->KK)",
					   50, -5, 5);
    fHistPhireco_KMinus_eta_bkg->SetXTitle("#eta");
    fHistPhireco_KMinus_eta_bkg->SetYTitle("Number of Entries");
    fHistPhireco_KMinus_phi_bkg = new TH1F("h_Phi_KMinus_phi_bkg", "K^{-}:  #phi distribution (#Phi->KK)",
					   50, -3.14, 3.14);
    fHistPhireco_KMinus_phi_bkg->SetXTitle("#phi");
    fHistPhireco_KMinus_phi_bkg->SetYTitle("Number of Entries");

    // Angle between Kaon tracks
    gDirectory->cd("../");

    fHistPhireco_KK_OpeningAngle_bkg = new TH1F("h_Phi_KK_OpeningAngle_bkg", "#alpha between KK Bkg",
					 50, 0, 3.14);
    fHistPhireco_KK_OpeningAngle_bkg->SetXTitle("#alpha (rad)");
    fHistPhireco_KK_OpeningAngle_bkg->SetYTitle("Number of Entries");
    
    fHistPhireco_KK_DecayAngle_bkg = new TH1F("h_Phi_KK_DecayAngle_bkg", "#alpha between K and Phi Bkg in Phis restframe",
					 50, 0, 3.14);
    fHistPhireco_KK_DecayAngle_bkg->SetXTitle("#alpha (rad)");
    fHistPhireco_KK_DecayAngle_bkg->SetYTitle("Number of Entries");

    fHistdEdx_pmag_bkg = new TH2F("h_dEdx_bkg", "dE/dx as a function of momentum (bkg)", 1000, GetLogBins(1000, -1.0, 3.0), 100, 0.0, 5.0);
    fHistdEdx_pmag_bkg->SetXTitle("p (GeV)");
    fHistdEdx_pmag_bkg->SetYTitle("dE/dx");

    fHistdEdx_diff_bkg = new TH1F("h_dEdx_diff_bkg", "difference between measured and calculated dEdx for bkg", 1000, 0.0, 5.0);
    fHistdEdx_diff_bkg->SetXTitle("#Delta dE/dx");
    fHistdEdx_diff_bkg->SetYTitle("Number of Entries");

    gDirectory->cd("../../");
    
    // Chi^2 from Track and Vertex with Track Fit
    fHistPhireco_Chi_TrkFit = new TH1F("h_Phi_TrkFit", "Chi^2 of Track Fit",
				       100, 0.0, 6.0);
    fHistPhireco_Chi_TrkFit->SetXTitle("Chi^2/NDoF");
    fHistPhireco_Chi_TrkFit->SetYTitle("Number of Entries");
    fHistPhireco_Chi_VtxFit = new TH1F("h_Phi_VtxTrkFit", "Chi^2 from Vtx with Trk Fit",
				       100, 0.0, 6.0);
    fHistPhireco_Chi_VtxFit->SetXTitle("Chi^2/NDoF");
    fHistPhireco_Chi_VtxFit->SetYTitle("Number of Entries");

    // ========================
    //     MC TRUTH STUDY
    // ========================

    gDirectory->mkdir("MCTruthStudy", "Small Truth Study with hitbased Truth-matching");
    gDirectory->cd("MCTruthStudy");

    // MC Truth Matching
    // only for KinFitter (later also for Cutbased)
    fHistPhi_MC_KKsame = new TH1F("h_Phi_KKsame", "MC-TM 2 Kaons same Mother", 2000, -1000, 1000);
    fHistPhi_MC_KKsame->SetXTitle("pdg code");
    fHistPhi_MC_KKsame->SetYTitle("Number of Entries");
    fHistPhi_MC_same = new TH1F("h_Phi_same", "MC-TM same Mother", 2000, -1000, 1000);
    fHistPhi_MC_same->SetXTitle("pdg code");
    fHistPhi_MC_same->SetYTitle("Number of Entries");
   
    fHistPhireco_MCParticle_Prob = new TH1F("h_Phi_MCProb", "MC Particle Prob",
					    200, 0.0, 1.0);
    fHistPhireco_MCParticle_Prob->SetXTitle("Prob");
    fHistPhireco_MCParticle_Prob->SetYTitle("Number of Entries");

    // =============================================================================
    // Book signal histograms for K0s-candidates that fulfill the MC Truth Matching
    // =============================================================================
     
    gDirectory->mkdir("Signal_MC", "Phi-candidates that fulfill the MC Truth Matching");
    gDirectory->cd("Signal_MC");

    if ( fMode == kKinFit ) {
	fHistPhireco_m_MCsignal   = new TH1F("h_Phi_m_MCsignal", "Invariant Phi mass (equaly signed kaons)",
					     50, 1.0, 1.05);
    }else if (fMode == kCutBased){
	fHistPhireco_m_MCsignal   = new TH1F("h_Phi_m_PiPi", "Invariant Phi mass (equaly signed kaons)",
					     50, 1.0, 1.6);
    }
    fHistPhireco_m_MCsignal->SetXTitle("m_{#Phi} (GeV)");
    fHistPhireco_m_MCsignal->SetYTitle("Number of Entries");
    fHistPhireco_pt_MCsignal  = new TH1F("h_Phi_pt_MCsignal", "Phi-p_{#perp} (equaly signed kaons)",
					 100, 0, 600);
    fHistPhireco_pt_MCsignal->SetXTitle("p_{#perp} (GeV)");
    fHistPhireco_pt_MCsignal->SetYTitle("Number of Entries");
    fHistPhireco_phi_MCsignal = new TH1F("h_Phi_phi_MCsignal", "Phi #phi distribution (equaly signed kaons)",
					 128, -3.2, 3.2);
    fHistPhireco_phi_MCsignal->SetXTitle("#phi (rad)");
    fHistPhireco_phi_MCsignal->SetYTitle("Number of Entries");
    fHistPhireco_eta_MCsignal = new TH1F("h_Phi_eta_MCsignal", "Phi #eta distribution (equaly signed kaons)",
					 50, -3, 3);
    fHistPhireco_eta_MCsignal->SetXTitle("#eta");
    fHistPhireco_eta_MCsignal->SetYTitle("Number of Entries");
    fHistPhireco_N_MCsignal   = new TH1F("h_Phi_N_MCsignal", "Reconstructed Phi' per event (equaly signed kaons)",
					 11,-0.5,10.5);
    fHistPhireco_N_MCsignal->SetXTitle("N_{Phi}/evt");
    fHistPhireco_N_MCsignal->SetYTitle("Number of Entries");
    fHistPhireco_OpeningAngle_MCsignal = new TH1F("h_Phi_OpeningAngle_MCsignal",
						  "opening angle between kaon tracks",
						  128, 0, 3.2);
    fHistPhireco_OpeningAngle_MCsignal->SetXTitle("#alpha (rad)");
    fHistPhireco_OpeningAngle_MCsignal->SetYTitle("Number of Entries");
    fHistPhireco_DecayAngle_MCsignal = new TH1F("h_Phi_DecayAngle_MCsignal",
						"decay angle between Phi and K+ in Phi' restframe",
						50, 0, 1);
    fHistPhireco_DecayAngle_MCsignal->SetXTitle("cos(#beta) (rad)");
    fHistPhireco_DecayAngle_MCsignal->SetYTitle("Number of Entries");

    fHistdEdx_pmag_MCsig = new TH2F("h_dEdx_MCsig", "dE/dx as a function of momentum (MC sig)", 1000, GetLogBins(1000, -1.0, 3.0), 100, 0.0, 5.0);
    fHistdEdx_pmag_MCsig->SetXTitle("p (GeV)");
    fHistdEdx_pmag_MCsig->SetYTitle("dE/dx");

    fHistdEdx_diff_MCsig = new TH1F("h_dEdx_diff_MCsig", "difference between measured and calculated dEdx for MC signal", 1000, 0.0, 5.0);
    fHistdEdx_diff_MCsig->SetXTitle("#Delta dE/dx");
    fHistdEdx_diff_MCsig->SetYTitle("Number of Entries");

    fHistPhireco_Number_Hits_MCsignal = new TH1F("h_Number_Hits_MCsignal", "Number of Hits in Detector of signal Tracks", 1000, 0.0, 1000);
    fHistPhireco_Number_Hits_MCsignal->SetXTitle("# Hits");
    fHistPhireco_Number_Hits_MCsignal->SetYTitle("Number of Entries");
    
    fHistPhireco_ChiNDoF_Trks_MCsignal = new TH1F("h_ChiNDoF_MCsignal", "Chi²over NDoF of signal Tracks", 1000, 0.0, 100);
    fHistPhireco_ChiNDoF_Trks_MCsignal->SetXTitle("Chi²over NDoF");
    fHistPhireco_ChiNDoF_Trks_MCsignal->SetYTitle("Number of Entries");
    
    gDirectory->cd("../");
    
    // ==========================================================================
    // Book bkg histograms for K0s-candidates that fail the MC Truth Matching
    // ==========================================================================

    gDirectory->mkdir("Bkg_MC", "Phi-candidates that fail the MC Truth Matching");
    gDirectory->cd("Bkg_MC");

    if ( fMode == kKinFit ) {
	fHistPhireco_m_MCbkg   = new TH1F("h_Phi_m_MCbkg", "Invariant Phi mass (equaly signed kaons)",
					  50, 1.0, 1.05);
    }else if (fMode == kCutBased){
	fHistPhireco_m_MCbkg   = new TH1F("h_Phi_m_PiPi", "Invariant Phi mass (equaly signed kaons)",
					  50, 1.0, 1.6);
    }
    fHistPhireco_m_MCbkg->SetXTitle("m_{#Phi} (GeV)");
    fHistPhireco_m_MCbkg->SetYTitle("Number of Entries");
    fHistPhireco_pt_MCbkg  = new TH1F("h_Phi_pt_MCbkg", "Phi-p_{#perp} (equaly signed kaons)",
				      100, 0, 600);
    fHistPhireco_pt_MCbkg->SetXTitle("p_{#perp} (GeV)");
    fHistPhireco_pt_MCbkg->SetYTitle("Number of Entries");
    fHistPhireco_phi_MCbkg = new TH1F("h_Phi_phi_MCbkg", "Phi #phi distribution (equaly signed kaons)",
				      128, -3.2, 3.2);
    fHistPhireco_phi_MCbkg->SetXTitle("#phi (rad)");
    fHistPhireco_phi_MCbkg->SetYTitle("Number of Entries");
    fHistPhireco_eta_MCbkg = new TH1F("h_Phi_eta_MCbkg", "Phi #eta distribution (equaly signed kaons)",
				      50, -3, 3);
    fHistPhireco_eta_MCbkg->SetXTitle("#eta");
    fHistPhireco_eta_MCbkg->SetYTitle("Number of Entries");
    fHistPhireco_N_MCbkg   = new TH1F("h_Phi_N_MCbkg", "Reconstructed Phi' per event (equaly signed kaons)",
				      11,-0.5,10.5);
    fHistPhireco_N_MCbkg->SetXTitle("N_{Phi}/evt");
    fHistPhireco_N_MCbkg->SetYTitle("Number of Entries");
    fHistPhireco_OpeningAngle_MCbkg = new TH1F("h_Phi_OpeningAngle_MCbkg",
					       "opening angle between kaon tracks",
					       128, 0, 3.2);
    fHistPhireco_OpeningAngle_MCbkg->SetXTitle("#alpha (rad)");
    fHistPhireco_OpeningAngle_MCbkg->SetYTitle("Number of Entries");
    fHistPhireco_DecayAngle_MCbkg = new TH1F("h_Phi_DecayAngle_MCbkg",
					     "decay angle between Phi and K+ in Phi's restframe",
					     50, 0, 1);
    fHistPhireco_DecayAngle_MCbkg->SetXTitle("cos(#beta) (rad)");
    fHistPhireco_DecayAngle_MCbkg->SetYTitle("Number of Entries");

    fHistdEdx_pmag_MCbkg = new TH2F("h_dEdx_MCbkg", "dE/dx as a function of momentum (MC bkg)", 1000, GetLogBins(1000, -1.0, 3.0), 100, 0.0, 5.0);
    fHistdEdx_pmag_MCbkg->SetXTitle("p (GeV)");
    fHistdEdx_pmag_MCbkg->SetYTitle("dE/dx");

    fHistdEdx_diff_MCbkg = new TH1F("h_dEdx_diff_MCbkg", "difference between measured and calculated dEdx for MC bkg", 1000, 0.0, 5.0);
    fHistdEdx_diff_MCbkg->SetXTitle("#Delta dE/dx");
    fHistdEdx_diff_MCbkg->SetYTitle("Number of Entries");

    fHistPhireco_Number_Hits_MCbkg = new TH1F("h_Number_Hits_MCbkg", "Number of Hits in Detector of bkg Tracks", 1000, 0.0, 1000);
    fHistPhireco_Number_Hits_MCbkg->SetXTitle("# Hits");
    fHistPhireco_Number_Hits_MCbkg->SetYTitle("Number of Entries");
    
    fHistPhireco_ChiNDoF_Trks_MCbkg = new TH1F("h_ChiNDoF_MCbkg", "Chi²over NDoF of bkg Tracks", 1000, 0.0, 100);
    fHistPhireco_ChiNDoF_Trks_MCbkg->SetXTitle("Chi²over NDoF");
    fHistPhireco_ChiNDoF_Trks_MCbkg->SetYTitle("Number of Entries");

    gDirectory->cd("../../../");

    // dE/dx Histograms
    fHistdEdx_pmag = new TH2F("h_dEdx", "dE/dx as a function of momentum", 1000, GetLogBins(1000, -1.0, 3.0), 100, 0.0, 5.0);
    fHistdEdx_pmag->SetXTitle("p (GeV)");
    fHistdEdx_pmag->SetYTitle("dE/dx");

    fHistdEdx_betagamma = new TH2F("h_dEdx_betagamma", "dE/dx as a function of #beta#gamma", 500, GetLogBins(500, -1.0, log10(300)), 100, 0.0, 5.0);
    fHistdEdx_betagamma->SetXTitle("#beta#gamma");
    fHistdEdx_betagamma->SetYTitle("dE/dx");
    
    gDirectory->cd("../");
}

//____________________________________________________________________

Double_t* AtlPhiFinder::GetLogBins(Int_t BinCount, Double_t MinBinExpo, Double_t MaxBinExpo) {
    // Logarithmic binning algorithm
    Double_t *bins = new Double_t[BinCount+1];
    for (Int_t i = 0; i < BinCount+1; i++)
	bins[i] = pow(10, (MaxBinExpo - MinBinExpo)/(BinCount+1) * i + MinBinExpo);
    return bins;
}
 
//____________________________________________________________________

Bool_t AtlPhiFinder::AnalyzeEvent() {
    //
    // Event analysis
    //
    
    // Clean-up bkg list from previous event
    for (Int_t i = 0; i < fBkgPhiDecays->GetEntries(); i++) {
	AtlPhiDecayKK *obj = (AtlPhiDecayKK*)fBkgPhiDecays->At(i);
	delete obj;
    }
    fBkgPhiDecays->GetEntries(); 
    fBkgPhiDecays->Clear("C");

    // GetPreTagEvtWeight() *= GetModifiedEventWeight();
    // Reconstruct Phi decays
    if ( fMode == kKinFit ) {
	ReconstructPhiKinFit();
    } else if ( fMode == kCutBased ) {
	ReconstructPhiCutBased();
    } else {
	Error("AnalyzeEvent",
	      "Invalid mode given. Abort!");
	gSystem->Abort(0);
    }

    if ( ! fUseKaonPurity && fEvent->IsMC() ) {
	// Count all phi decays in MC events to allow computation of the efficiency.
	Int_t NPhiTrue = 0;
	HepVertex *vtx = (HepVertex*)fEvent->GetPrimaryVtx();
	if (vtx == NULL) return kFALSE;
	for (Int_t i = 0; i < vtx->GetNDaughters(); i++) {
	    for (Int_t j = i+1; j < vtx->GetNDaughters(); j++) { 
		AtlIDTrack *trk1 = (AtlIDTrack*)vtx->GetDaughters()->At(i);
		AtlIDTrack *trk2 = (AtlIDTrack*)vtx->GetDaughters()->At(j);
		if(  trk1->HasValidTruthMatch(fMatchingProb_min)
		     && trk2->HasValidTruthMatch(fMatchingProb_min)) {
		    if( (trk1->GetMCParticle()->GetMother() == NULL) || (trk2->GetMCParticle()->GetMother() == NULL) ) {
			continue;}
		    if( (TMath::Abs(trk1->GetMCParticle()->GetPdgCode())== 321)
			&& (TMath::Abs(trk2->GetMCParticle()->GetPdgCode())== 321)
			&& (TMath::Abs(trk1->GetMCParticle()->GetMother()->GetPdgCode())== 333)
			&& (trk1->GetMCParticle()->GetMother() == trk2->GetMCParticle()->GetMother()) ) {
			NPhiTrue++;
		    }
		}
	    }
	}
	AddAbundanceTrue(NPhiTrue);
    } else if ( fUseKaonPurity && fEvent->IsMC() ) {
	// Kaon purity
	// Count all phi decays in MC events to allow computation of the efficiency.
	Int_t NPhiTrue = 0;
	HepVertex *vtx = (HepVertex*)fEvent->GetPrimaryVtx();
	if (vtx == NULL) return kFALSE;
	for (Int_t i = 0; i < vtx->GetNDaughters(); i++) {
	    AtlIDTrack *trk1 = (AtlIDTrack*)vtx->GetDaughters()->At(i);
	    if(  trk1->HasValidTruthMatch(fMatchingProb_min) ) {
		if (TMath::Abs(trk1->GetMCParticle()->GetPdgCode())== 321) {
		    NPhiTrue++;
		}
	    }
	}
	AddAbundanceTrue(NPhiTrue);
    }
    return kTRUE;
}

//____________________________________________________________________

void AtlPhiFinder::ReconstructPhiCutBased(){
    //
    // Cut Based Phi reconstruction
    // some cut criteria have to be set external
    // have a look at SetCutDefaults()
    // others can be changed in SetCutDefaults()
    // 
       
    // Looping over all trk-combinations at primary vertex for Phi reconstruction
    
    AtlIDTrack *trk1 = 0;
    AtlIDTrack *trk2 = 0;                    // compared trks
    TList  *Vtx1trk;                         // List of tracks from primary vertex
    HepVertex *PrimVtx;                      // associated primary Vertex
    
    Int_t i, j;                              // Vertex Loop iterators
    Int_t BgkCount = 1;                      // gives ID of background Phi (from same charged Kaons)
    
    TLorentzVector p_trk1, p_trk2, p_Phi;    // 4 Momentum
    Float_t m_Phi = 0;                       // Reconstructed Phi-mass

    // Get the primary vertex
    PrimVtx = fEvent->GetPrimaryVtx();
    // security request
    if (PrimVtx == NULL) return;
    
    // Get the primary vertex trk list
    Vtx1trk = fEvent->GetPrimVtxTracks();
    // security request
    if (Vtx1trk == NULL) return;
         
    // Track-Loop over all possible trk- combinations at primary Vertex for Phi reconstruction
    for(i=0; i < Vtx1trk->GetEntries() ; i++){
	for(j= i+1; j < Vtx1trk->GetEntries() ; j++ ){
	    trk1 = (AtlIDTrack*)(Vtx1trk->At(i));
	    trk2 = (AtlIDTrack*)(Vtx1trk->At(j));
	    
	    // Discard all tracks without reference to a truth particle for all
	    // MC-Runs to compute the purity correctly
	    if( fEvent->IsMC() ){
		if( !( trk1->HasValidTruthMatch(0.0001) &&
		       trk2->HasValidTruthMatch(0.0001)) )  continue;
	    }
	    
	    // Get 4 Momentum and compute inv. mass of Phi candidate
	    p_trk1.SetVectM(trk1->P(), fm_K);
	    p_trk2.SetVectM(trk2->P(), fm_K);
	    p_Phi = p_trk1 + p_trk2;
	    m_Phi = p_Phi.M();

            // Selection constraints
            // -------------------------
	    if( (m_Phi>fPhi_M_min) && (m_Phi<fPhi_M_max) ) {     // mass peak:
		
		//Quality of Track-Fit
		if (  ((trk1->GetChi2_Vtx())/(trk1->GetNDoF_Vtx()) > fTrackChi2ovNDoF_max) ||
		      ((trk2->GetChi2_Vtx())/(trk2->GetNDoF_Vtx()) > fTrackChi2ovNDoF_max)  ) continue;
		SetCutFlow("Quality Trk Fit");
		// Momenta of outgoing Kaon tracks
		if (  (p_trk1.Pt() > fSignalKaon_Pt_max) ||
		      (p_trk1.Pt() < fSignalKaon_Pt_min)  ) continue;
		if (  (p_trk2.Pt() > fSignalKaon_Pt_max) ||
		      (p_trk2.Pt() < fSignalKaon_Pt_min)  ) continue;
		SetCutFlow("Pt");
		fHistPhireco_KK_OpeningAngle->Fill(trk1->P().Angle(trk2->P()), GetPreTagEvtWeight());

		// Angle criterion (between the two Kaon (or what ever) tracks)
		if (trk1->P().Angle(trk2->P()) > fAngle_max) continue;
		SetCutFlow("oangle");
		
		if ((trk1->Pmag() < fPmag_min) || (trk1->Pmag() > fPmag_max) ||
		    (trk2->Pmag() < fPmag_min) || (trk2->Pmag() > fPmag_max)) continue;
		SetCutFlow("Pmag");
		
		if (Delta_dEdx(trk1) > fdEdx_Delta_max) continue;
		SetCutFlow("dEdx 1");
	    
		if (Delta_dEdx(trk2) > fdEdx_Delta_max) continue;
		SetCutFlow("dEdx 2");
		
		if ( trk1->GetQovP()*trk2->QovP() < 0.) {       // request opposite sign for signal
		SetCutFlow("opposite charge");    
		AtlPhiDecayKK *decay = 0;
		
		// Add PhiDecay to fEvents Decay-list
		// look for right order
		TLorentzVector P1,P2;
		P1.SetVectM(trk1->P(),fm_K);
		P2.SetVectM(trk2->P(),fm_K);
		
		if(trk1->GetQovP_Vtx()>0.){
		    HepParticle Fit_Daughter1(1,P1.Px(),P1.Py(),P2.Pz(),P1.E(),321);
		    HepParticle Fit_Daughter2(2,P2.Px(),P2.Py(),P2.Pz(),P2.E(),-321);
		    decay = fEvent->AddPhiDecayKK(p_Phi.Px(),p_Phi.Py(),p_Phi.Pz(),p_Phi.E(),
						  trk1,trk2,PrimVtx,
						  Fit_Daughter1,Fit_Daughter2);
		} else {
		    HepParticle Fit_Daughter1(1,P2.Px(),P2.Py(),P2.Pz(),P2.E(),321);
		    HepParticle Fit_Daughter2(2,P1.Px(),P1.Py(),P1.Pz(),P1.E(),-321);
		    decay = fEvent->AddPhiDecayKK(p_Phi.Px(),p_Phi.Py(),p_Phi.Pz(),p_Phi.E(),
						  trk2,trk1,PrimVtx,
					      Fit_Daughter1, Fit_Daughter2);
		}
		decay->SetChi2NDoF(0.0, 1);
		// DebugOutput
		if ( IsDebugRun() ) {
		    if( trk1->HasValidTruthMatch(0.0001) && trk2->HasValidTruthMatch(0.0001) ){
			fDebugOutput << endl
				     << "Event Nr. :" << fTree->GetReadEntry() << endl
				     << "Trk1: " << trk1->GetMCParticle()->GetPdgName() << " Mother: "
				     << trk1->GetMCParticle()->GetMother()->GetPdgName() << endl
				     << "Trk2: " << trk2->GetMCParticle()->GetPdgName() << " Mother: "
				     << trk2->GetMCParticle()->GetMother()->GetPdgName() << endl
				     << endl;
		    }
		}
		} else {
		    
		    // Add same charged combinations to the bkg list
		    TLorentzVector P1,P2;
		    P1.SetVectM(trk1->P(),fm_K);
		    P2.SetVectM(trk2->P(),fm_K);

		    Int_t pdgCode = 0;
		    if (trk1->QovP()>0.) pdgCode = 321;
		    else pdgCode = -321;

		    HepParticle Fit_Daughter1(1,P1.Px(),P1.Py(),P2.Pz(),P1.E(),pdgCode);
		    HepParticle Fit_Daughter2(2,P2.Px(),P2.Py(),P2.Pz(),P2.E(),pdgCode);
		    /* fEvent->AddPhiDecayKK(p_Phi.Px(),p_Phi.Py(),p_Phi.Pz(),p_Phi.E(),
					  trk1,trk2,
					  PrimVtx,PrimVtx,
					  Fit_Daughter1,Fit_Daughter2);*/
		    fBkgPhiDecays->Add(new AtlPhiDecayKK(BgkCount++, p_Phi.Px(), p_Phi.Py(),
							 p_Phi.Pz(), p_Phi.E(),
							 trk1,trk2,
							 PrimVtx,PrimVtx,
							 Fit_Daughter1,Fit_Daughter2));
		}
	    }
	}
    }
    delete Vtx1trk;
}

//____________________________________________________________________


void AtlPhiFinder::ReconstructPhiKinFit(){
    //
    // Phi reconstruction from track-pairs assuming being Kaons using KinFitter
    //

    //TList      *Vtx1trk;
    HepVertex  *PrimVtx;
    AtlIDTrack *trk1 = 0;  
    AtlIDTrack *trk2 = 0;
    Int_t j ;

    TLorentzVector p_trk1, p_trk2, p_Phi;    // 4 Momentum
    Float_t m_Phi = 0;                       // Reconstructed Phi-mas
    
    TMatrixD cov_trk1(3,3);
    TMatrixD cov_trk2(3,3);
   
    Int_t BgkCount = 1;                      // gives ID of background Phi (from same charged Kaons)
    Float_t PvalueKinFit = 0.0;

    // Get the primary vertex
    PrimVtx = fEvent->GetPrimaryVtx();
    // security request
    if (PrimVtx == NULL) return;
    
    /* Get the primary vertex trk list
    Vtx1trk = fEvent->GetPrimVtxTracks();
    // security request
    if (Vtx1trk == NULL) return;*/
        
    // Loop over all trk combination
    for(Int_t i = 0; i < PrimVtx->GetNDaughters(); i++) {
	trk1 = (AtlIDTrack*)PrimVtx->GetDaughters()->At(i);
 	if (trk1->GetPixeldEdx() < 0.1) continue;
	
	fHistdEdx_pmag->Fill(trk1->Pmag(), trk1->GetPixeldEdx(), GetPreTagEvtWeight());
	// MC Studies
        if (trk1->HasValidTruthMatch(0.9)) {
	    HepMCParticle *mcprt = trk1->GetMCParticle();
	    fHistdEdx_betagamma->Fill(mcprt->P().Beta()*mcprt->P().Gamma(), trk1->GetPixeldEdx(), GetPreTagEvtWeight());
	    
            if ((TMath::Abs(trk1->GetMCParticle()->GetPdgCode()) == 321) && (TMath::Abs(trk1->GetMCParticle()->GetMother()->GetPdgCode()) == 333)) {
	        fHistdEdx_pmag_MCsig->Fill(trk1->Pmag(), trk1->GetPixeldEdx(), GetPreTagEvtWeight());
		if ((trk1->Pmag() >= fPmag_min) && (trk1->Pmag() < fPmag_max))
		    fHistdEdx_diff_MCsig->Fill(Delta_dEdx(trk1), GetPreTagEvtWeight());
	    }
           else {
	        fHistdEdx_pmag_MCbkg->Fill(trk1->Pmag(), trk1->GetPixeldEdx(), GetPreTagEvtWeight());
		if ((trk1->Pmag() >= fPmag_min) && (trk1->Pmag() < fPmag_max))
		    fHistdEdx_diff_MCbkg->Fill(Delta_dEdx(trk1), GetPreTagEvtWeight());
	    }
        }
	
	for(j = i+1; j < PrimVtx->GetNDaughters(); j++) {

            trk2 = (AtlIDTrack*)PrimVtx->GetDaughters()->At(j);
	    if (trk2->GetPixeldEdx() < 0.1) continue;
	   
	    // Discard all tracks without reference to a truth particle for all
	    // MC-Runs to compute the purity correctly
	    if( fEvent->IsMC() ){
		if( !( trk1->HasValidTruthMatch(0.0001) &&
		       trk2->HasValidTruthMatch(0.0001)) ) {
		    continue;
		}
		SetCutFlow("MC exists");
	    }
	    
	    if ( (trk1->Chi2ovNDoF() > fTrackChi2ovNDoF_max) || (trk2->Chi2ovNDoF() > fTrackChi2ovNDoF_max)) continue;
	    SetCutFlow("Chi2ovNDoF cut");

            if ((trk1->Pmag() < fPmag_min) || (trk1->Pmag() > fPmag_max) ||
		(trk2->Pmag() < fPmag_min) || (trk2->Pmag() > fPmag_max)) continue;
	    SetCutFlow("Pmag");
	    
            if (Delta_dEdx(trk1) > fdEdx_Delta_max) continue;
            SetCutFlow("dEdx 1");
	    
	    if (Delta_dEdx(trk2) > fdEdx_Delta_max) continue;
	    SetCutFlow("dEdx 2");
	    
	    // Get 4 Momentum and compute inv. mass of Phi candidate
	    p_trk1.SetVectM(trk1->P(), fm_K);
	    p_trk2.SetVectM(trk2->P(), fm_K);
	    p_Phi = p_trk1 + p_trk2;
	    m_Phi = p_Phi.M();

	    // Selection constraints, regarding only interesting trk combinations
	    // -------------------------
	    if (! ((m_Phi > fPhi_M_min) && (m_Phi < fPhi_M_max)) ) continue;
	    SetCutFlow("mass cut");
	    /*
	    if ( trk1->P().Angle(trk2->P()) > fAngle_max ) continue; // improves purity in MC-TM, fAngle_max is set external
	    SetCutFlow("angle cut");
	    */

	    // cov matrices
	    trk1->GetCovMatrixPtEtaPhi(cov_trk1);
	    trk2->GetCovMatrixPtEtaPhi(cov_trk2);
	    
	    // define momentum used as output of the fit
	    TLorentzVector FitP_trk1;
	    TLorentzVector FitP_trk2;
	    
	    // define particels necessary for execution of the fit
	    // using pion mass hypothesis; fit variables are pt, eta, phi

	    TVector3 dummy1 = trk1->P(); // instead of const cast
	    TFitParticlePtEtaPhi FitExec_trk1("FitExec_trk1","FitExec_trk1 for PhiFit",
					      &dummy1,fm_K,&cov_trk1);
	    TVector3 dummy2 = trk2->P(); // instead of const cast
	    TFitParticlePtEtaPhi FitExec_trk2("FitExec_trk2","FitExec_trk2 for PhiFit",
					      &dummy2,fm_K,&cov_trk2);
	       
	    // definition of Phi mass constraint
	    TFitConstraintM MPhiCons("PhiMassConstraint","PhiMassConstraintGaus",0,0,fm_Phi);
	    MPhiCons.addParticles1(&FitExec_trk1,&FitExec_trk2);
	       
	    // fitting characteristics
	    TKinFitter fitter;
	    fitter.addMeasParticle(&FitExec_trk1);
	    fitter.addMeasParticle(&FitExec_trk2);
	    fitter.addConstraint(&MPhiCons);
 						   
	    fitter.setMaxNbIter(50);   // maximum number of iterations
	    fitter.setMaxDeltaS(5e-5); // maximum deviation of the minimum function within two iterations
	    fitter.setMaxF(1e-4);      // maximum value of constraints
	    fitter.setVerbosity(0);    // verbosity level
	       
	    // Kinematic Fitting
	    fitter.fit();

	    // Fill Histogram IF(!) the fit converged (fitter.getStatus==0)
	    if ( fitter.getS()<0. ) {
		Error("ReconstructPhiKinFit",
		      "fitter.getS()<0. Abort!");
		gSystem->Abort(0);
	    }
	    if( ! fitter.getStatus() ) {
		SetCutFlow("fit converged");
		
		// Copy fittet Vectors
		FitP_trk1=(*FitExec_trk1.getCurr4Vec());
		FitP_trk2=(*FitExec_trk2.getCurr4Vec());
		   
		// Reconstruct Phi 4-momentum
		p_Phi = FitP_trk1 + FitP_trk2;
				   
		//setting Chi2,NDoF,PvalueKinFit
		SetChi2(fitter.getS());
		SetNDoF(fitter.getNDF());
		PvalueKinFit = TMath::Prob(fChi2, fNDoF);

		// eta-cut from MC info
		if ( (p_Phi.Eta() < fPhi_Eta_min) || (p_Phi.Eta() > fPhi_Eta_max) ) continue;
		SetCutFlow("Eta");
				
		// Adding Phi to event
		if (PvalueKinFit >= fKinFitPvalue_min) {
		    AtlPhiDecayKK *decay;
		    SetCutFlow("Pvalue Cut");
		    if ( trk1->Charge()*trk2->Charge() < 0. ) { //require opposite sign
			SetCutFlow("opposite charge");

			AtlPhiDecayKK *decay;
			// look for right order
			if(trk1->GetQovP_Vtx()>0.){
			    HepParticle Fit_Daughter1(1, FitP_trk1.Px(), FitP_trk1.Py(),
						      FitP_trk1.Pz(), FitP_trk1.E(), 321);
			    HepParticle Fit_Daughter2(2, FitP_trk2.Px(), FitP_trk2.Py(),
						      FitP_trk2.Pz(), FitP_trk2.E(),-321);
			    decay = fEvent->AddPhiDecayKK(p_Phi.Px(),p_Phi.Py(),p_Phi.Pz(),p_Phi.E(),
							  trk1,trk2,PrimVtx,
							  Fit_Daughter1, Fit_Daughter2);
			}else{
			    HepParticle Fit_Daughter1(1,FitP_trk2.Px(),FitP_trk2.Py(),
						      FitP_trk2.Pz(), FitP_trk2.E(),321);
			    HepParticle Fit_Daughter2(2,FitP_trk1.Px(),FitP_trk1.Py(),
						      FitP_trk1.Pz(), FitP_trk1.E(),-321);
			    decay = fEvent->AddPhiDecayKK(p_Phi.Px(),p_Phi.Py(),p_Phi.Pz(),p_Phi.E(),
							  trk2,trk1,PrimVtx,
							  Fit_Daughter1, Fit_Daughter2);
			}
			decay->SetChi2NDoF(fChi2, fNDoF);
			
                        // DebugOuput
			if ( IsDebugRun() ) {
			    fDebugOutput
				<< "-------------------------"                                << endl
				<< "PDG trk1      : " << trk1->GetMCParticle()->GetPdgName()  << endl
				<< "PDG trk2      : " << trk2->GetMCParticle()->GetPdgName()  << endl;
			    if( trk1->GetMCParticle()->GetMother() != NULL ) {
				fDebugOutput << "Mother of trk1: " << trk1->GetMCParticle()->GetMother()->GetPdgName() << endl;
			    }
			    if( trk2->GetMCParticle()->GetMother() != NULL ) {
				fDebugOutput << "Mother of trk2: " << trk2->GetMCParticle()->GetMother()->GetPdgName() << endl;
			    }
			    fDebugOutput << "-------------------------"                       << endl;
			}
			
		    }
		    else {
			// Add same charged combinations to bkg list
			Int_t pdgCode = 0;
			if (trk1->QovP()>0.) pdgCode = 321;
			else pdgCode = -321;
			HepParticle Fit_Daughter1(1,FitP_trk1.Px(),FitP_trk1.Py(),
						  FitP_trk1.Pz(), FitP_trk1.E(),pdgCode);
			HepParticle Fit_Daughter2(2,FitP_trk2.Px(),FitP_trk2.Py(),
						  FitP_trk2.Pz(), FitP_trk2.E(),pdgCode);
			decay = new AtlPhiDecayKK(BgkCount++,
						  p_Phi.Px(), p_Phi.Py(),
						  p_Phi.Pz(), p_Phi.E(),
						  trk1,trk2,
						  PrimVtx,PrimVtx,
						  Fit_Daughter1,Fit_Daughter2);
			decay->SetChi2NDoF(fChi2, fNDoF);
			fBkgPhiDecays->Add(decay);
			

		    }
		}// End of Prob(Chi2ovNDoF) > x  request
	    }
	} // end of trk loop
    }
      FindSharedTracks();
}

//____________________________________________________________________    

void AtlPhiFinder::FindSharedTracks() {

    AtlPhiDecayKK *decay, *decay2;
    
    for ( Int_t i = 0; i < fEvent->GetN_PhiDecaysKK(); i++ ) {
      decay = (AtlPhiDecayKK*)fEvent->GetPhiDecaysKK()->At(i);

      for (Int_t j = i+1; j < fEvent->GetN_PhiDecaysKK(); j++) {
	  decay2 = (AtlPhiDecayKK*)fEvent->GetPhiDecaysKK()->At(j);
	  if (  (decay->GetKPlus() == decay2->GetKPlus()) ||
		(decay->GetKPlus() == decay2->GetKMinus()) ||
		(decay->GetKMinus() == decay2->GetKPlus()) ||
		(decay->GetKMinus() == decay2->GetKMinus()) ) {
	      decay->SetSharedTracks(kTRUE);
	      decay2->SetSharedTracks(kTRUE);
	  }
      }

/*      for (Int_t j = 0; j < fBkgLambdaDecays->GetEntries(); j++) {
	  decay2 = (AtlPhiDecayKPi*)fBkgLambdaDecays->At(j);
	  if (  (decay->GetKPlus() == decay2->GetKPlus()) ||
		(decay->GetKPlus() == decay2->GetKMinus()) ||
		(decay->GetKMinus() == decay2->GetKPlus()) ||
		(decay->GetKMinus() == decay2->GetKMinus()) ) {
	      decay->SetSharedTracks(kTRUE);
	      decay2->SetSharedTracks(kTRUE);
	  }
	  } */
      
    }
}

//____________________________________________________________________   

void AtlPhiFinder::FillHistograms() {
    //
    // Fill histograms (not all, some are filled in Reconstruct)
    //
    
    // Phi variables
    AtlPhiDecayKK *Decay   = 0 ;
    TLorentzVector P_Phi, P_KPlus, P_KMinus;
      
    // Kaon variables
    AtlIDTrack *Daughter1  = 0 ;
    AtlIDTrack *Daughter2  = 0 ;
    Float_t Daughter1_Pt   = 0.;
    Float_t Daughter2_Pt   = 0.;
    Float_t Daughter1_Eta  = 0.;
    Float_t Daughter2_Eta  = 0.;
    Float_t Daughter1_Phi  = 0.;
    Float_t Daughter2_Phi  = 0.;

    // Signal (opposite charged)
    // ________________________________________
    
    for ( Int_t i = 0; i < fEvent->GetN_PhiDecaysKK(); i++ ) {

	Decay = (AtlPhiDecayKK*)fEvent->GetPhiDecaysKK()->At(i);
	if (Decay->HasSharedTracks()) continue;
	
	SetChi2(Decay->GetChi2());
	SetNDoF(Decay->GetNDoF());
	
	// Hitbased TruthMatching
	if ( fEvent->IsMC() && fUseKaonPurity ) {
	    // K purity
	    switch ( DoTruthMatch(Decay->GetKPlus(), Decay->GetKMinus()) ) {
		case  1:
		    FillHistograms_MCTruthStudy(Decay,'s');
		    fTrueReco = kTRUE;
		    AtlKinFitterTool::FillHistograms("Signal");
		    fTrueReco = kFALSE;
		    AtlKinFitterTool::FillHistograms("Signal");
		    break;
		case  2:
		    FillHistograms_MCTruthStudy(Decay,'s');
		    fTrueReco = kTRUE;
		    AtlKinFitterTool::FillHistograms("Signal");
		    AtlKinFitterTool::FillHistograms("Signal");
		    break;
		case -1:
		    FillHistograms_MCTruthStudy(Decay,'b');
		    fTrueReco = kFALSE;
		    AtlKinFitterTool::FillHistograms("Signal");
		    AtlKinFitterTool::FillHistograms("Signal");
		    break;
	    }
	} else if ( fEvent->IsMC() && !fUseKaonPurity ) {
	    // Phi purity
	    switch ( DoTruthMatch(Decay->GetKPlus(), Decay->GetKMinus()) ) {
		case 1:
		    fTrueReco = kTRUE;
		    AtlKinFitterTool::FillHistograms("Signal");
		    FillHistograms_MCTruthStudy(Decay,'s');
		    break;
		case -1:
		    fTrueReco = kFALSE;
		    AtlKinFitterTool::FillHistograms("Signal");
		    AtlKinFitterTool::FillHistograms("Bkg");
		    FillHistograms_MCTruthStudy(Decay,'b');
		    break;
	    }
	} else {
	    AtlKinFitterTool::FillHistograms("Signal");
	}
	
	P_Phi    = Decay->P();
	P_KPlus  = Decay->GetReFitKPlus().P();
	P_KMinus = Decay->GetReFitKMinus().P();
	
	fHistPhireco_m_KK     ->Fill(Decay->M("Reco"),    GetPreTagEvtWeight());
	fHistPhireco_pt_KK    ->Fill(Decay->Pt(),         GetPreTagEvtWeight());  
	fHistPhireco_phi_KK   ->Fill(Decay->Phi(),        GetPreTagEvtWeight()); 
	fHistPhireco_eta_KK   ->Fill(Decay->Eta(),        GetPreTagEvtWeight());


	if ( fEvent->IsMC() ) {
	    if ( Decay->GetKPlus()->HasValidTruthMatch(fMatchingProb_min) ) {
		fHistPhireco_TRK->Fill(Decay->GetKPlus()->GetMCParticle()->GetPdgName(), GetPreTagEvtWeight());
	    }
	    if ( Decay->GetKMinus()->HasValidTruthMatch(fMatchingProb_min) ) {
		fHistPhireco_TRK->Fill(Decay->GetKMinus()->GetMCParticle()->GetPdgName(), GetPreTagEvtWeight());
	    }
	}
	
	// Opening Angle
	fHistPhireco_KK_OpeningAngle->Fill(Decay->GetOpeningAngle(), GetPreTagEvtWeight());

	// Decay Angle is the angle between Phis momentum and the momentum of the K+
	// in Phis restframe
	fHistPhireco_DecayAngle_KK    ->Fill(Decay->GetDecayAngle(),   GetPreTagEvtWeight());
	
	// Get Daughters: for Signal, first daughter is allways positive, second is negative
	// (see Reconstruct Phi cutbased/kinfit)
	Daughter1      = Decay->GetKPlus();
	Daughter2      = Decay->GetKMinus();
	Daughter1_Pt   = Daughter1->Pt();
	Daughter2_Pt   = Daughter2->Pt();
	Daughter1_Eta  = Daughter1->Eta();
	Daughter2_Eta  = Daughter2->Eta();
	Daughter1_Phi  = Daughter1->Phi();
	Daughter2_Phi  = Daughter2->Phi();
	
	fHistPhireco_KPlus_pt     ->Fill(Daughter1_Pt,    GetPreTagEvtWeight());
	fHistPhireco_KMinus_pt    ->Fill(Daughter2_Pt,    GetPreTagEvtWeight());
	fHistPhireco_KPlus_eta    ->Fill(Daughter1_Eta,   GetPreTagEvtWeight());
	fHistPhireco_KMinus_eta   ->Fill(Daughter2_Eta,   GetPreTagEvtWeight());
	fHistPhireco_KPlus_phi    ->Fill(Daughter1_Phi,   GetPreTagEvtWeight());
	fHistPhireco_KMinus_phi   ->Fill(Daughter2_Phi,   GetPreTagEvtWeight());

	// general tuning histograms:
	
	// Fill Histo of Vtx Fit
	fHistPhireco_Chi_VtxFit->Fill(Decay->GetVtx()->GetChi2overNDoF(), GetPreTagEvtWeight());

        // Fill Histo of Trk Fit   
	fHistPhireco_Chi_TrkFit->Fill(Daughter1->Chi2ovNDoF(), GetPreTagEvtWeight());
	fHistPhireco_Chi_TrkFit->Fill(Daughter2->Chi2ovNDoF(), GetPreTagEvtWeight());

        // dE/dx
	fHistdEdx_pmag_sig->Fill(Daughter1->Pmag(), Daughter1->GetPixeldEdx(), GetPreTagEvtWeight());
	fHistdEdx_pmag_sig->Fill(Daughter2->Pmag(), Daughter2->GetPixeldEdx(), GetPreTagEvtWeight());
	fHistdEdx_diff_sig->Fill(Delta_dEdx(Daughter1), GetPreTagEvtWeight());
	fHistdEdx_diff_sig->Fill(Delta_dEdx(Daughter2), GetPreTagEvtWeight());
    }
    fHistPhireco_N_KK ->Fill(fEvent->GetN_PhiDecaysKK(), GetPreTagEvtWeight());

    // Combinatorial Background (same charged)
    // ____________________________________________
    
    TIter next_bkg(fBkgPhiDecays);
    while ( (Decay = (AtlPhiDecayKK*)next_bkg()) ) {
	if (Decay->HasSharedTracks()) continue;
	
	TLorentzVector P_K1 = Decay->GetReFitKPlus().P();
	TLorentzVector P_K2 = Decay->GetReFitKMinus().P();

         // fill Chi2 and NDoF
	SetChi2(Decay->GetChi2());
	SetNDoF(Decay->GetNDoF());
//	AtlKinFitterTool::FillHistograms("Bkg");
	
	P_Phi = Decay->P();
	P_K1  = Decay->GetReFitKPlus().P();
	P_K2  = Decay->GetReFitKMinus().P();

	//filling the standard histograms
	//----------------------------------
	fHistPhireco_m_KK_bkg     ->Fill(Decay->M("REC"),    GetPreTagEvtWeight());
	fHistPhireco_pt_KK_bkg    ->Fill(Decay->Pt(),        GetPreTagEvtWeight());  
	fHistPhireco_phi_KK_bkg   ->Fill(Decay->Phi(),       GetPreTagEvtWeight()); 
	fHistPhireco_eta_KK_bkg   ->Fill(Decay->Eta(),       GetPreTagEvtWeight());

	// Opening Angle (bkg)
	fHistPhireco_KK_OpeningAngle_bkg  ->Fill(Decay->GetOpeningAngle(), GetPreTagEvtWeight());

	// Decay Angle is the angle between Phis momentum and the momentum of the K+
	// in Phis restframe (bkg)
	fHistPhireco_KK_DecayAngle_bkg->Fill(Decay->GetDecayAngle(),   GetPreTagEvtWeight());

        // Get Daughters: both are same charged -> fill either positive or
        // negative daughter histograms
	Daughter1     = Decay->GetKPlus();
	Daughter2     = Decay->GetKMinus();
	
	Daughter1_Pt  = Daughter1->Pt();
	Daughter2_Pt  = Daughter2->Pt();
	Daughter1_Eta = Daughter1->Eta();
	Daughter2_Eta = Daughter2->Eta();
	Daughter1_Phi = Daughter1->Phi();
	Daughter2_Phi = Daughter2->Phi();
	
	// dE/dx
	fHistdEdx_pmag_bkg->Fill(Daughter1->Pmag(), Daughter1->GetPixeldEdx(), GetPreTagEvtWeight());
	fHistdEdx_pmag_bkg->Fill(Daughter2->Pmag(), Daughter2->GetPixeldEdx(), GetPreTagEvtWeight());
	fHistdEdx_diff_bkg->Fill(Delta_dEdx(Daughter1), GetPreTagEvtWeight());
	fHistdEdx_diff_bkg->Fill(Delta_dEdx(Daughter2), GetPreTagEvtWeight());
    
	//Check if positive or negative charged daughters
	if( Daughter1->IsPositive() ) {
	    fHistPhireco_KPlus_pt_bkg     ->Fill(Daughter1_Pt,   GetPreTagEvtWeight());
	    fHistPhireco_KPlus_eta_bkg    ->Fill(Daughter1_Eta,  GetPreTagEvtWeight());
	    fHistPhireco_KPlus_phi_bkg    ->Fill(Daughter1_Phi,  GetPreTagEvtWeight());
	}else{
	    fHistPhireco_KMinus_pt_bkg    ->Fill(Daughter2_Pt,   GetPreTagEvtWeight());
	    fHistPhireco_KMinus_eta_bkg   ->Fill(Daughter2_Eta,  GetPreTagEvtWeight());
	    fHistPhireco_KMinus_phi_bkg   ->Fill(Daughter2_Phi,  GetPreTagEvtWeight());
	}
    }
    fHistPhireco_N_KK_bkg ->Fill(fBkgPhiDecays->GetEntries(), GetPreTagEvtWeight());
}

//____________________________________________________________________

void AtlPhiFinder::SetCutDefaults() {
    //
    // Selection cut defaults and Constraint parameters
    // -----------------------------------------------------------------
    // external cuts:
    // fPhi_M_min, fPhi_M_max:
    //                     e.g.:  for Cutbased: 0.950..1.250
    //                            for KinFit  : 1.015..1.025
    // Quality of VtxFit: fVertexChi2ovNDoF_max:
    //                     e.g.:  normal:   1.5
    //                            DebugRun: 5.0
    // Quality of TrkFit: fTrackChi2ovNDoF_max:
    //                     e.g.:  normal:   1.5
    //                            DebugRun: 3.8
    // Angle between Trks (only for KinFit): fAngle_max: e.g.: 0.3 (rad) (seems to less nice for purity)
    // Quality of kinematic Fit: fKinFitPvalue_min: e.g:   0.10
    // Quality of Truth Matching: fMatchingProb_min: e.g.: 0.95
    // -----------------------------------------------------------------
    //

    // Cuts for Tracks
    fSignalKaon_Pt_min       =  0.5;         // Min. Pt of signal electrons 
    fSignalKaon_Pt_max       =  1.e10;       // Max. Pt of signal electrons
    fSignalKaon_Eta_min      = -2.5;         // Min. eta of signal electrons
    fSignalKaon_Eta_max      =  2.5;         // Max. eta of signal electrons
    
    // mass hypotheses
    fm_K                     = 0.493677;     // Kaon mass hypothesis
    fm_Phi                   = 1.019455;     // Phi mass from PDG

    // Angle citerion
    if ( fMode == kCutBased ) {              // Angle criterion only for cutbased
	fAngle_max = 8.5 ;                   // for KinFit cut value is set external
    }

    // dEdx Fit values
    fdEdx_p0 = -2.375e-1;
    fdEdx_p1 = -3.09;
    fdEdx_p2 = 5.0e-1;
    fdEdx_p3 = 1.1389;
    fdEdx_p4 = -1.323e-1;

    // Bethe Bloch
    fBetheBloch = new TF1("Bethe-Bloch-like-Fkt","[0]/pow(x*x/(x*x + 1),[3]) * ([1] - pow(x*x/(x*x + 1),[3]) - log([2] + pow(1/x, [4])))", 0.0, 1e6);
    fBetheBloch->SetParameter(0, fdEdx_p0);
    fBetheBloch->SetParameter(1, fdEdx_p1);
    fBetheBloch->SetParameter(2, fdEdx_p2);
    fBetheBloch->SetParameter(3, fdEdx_p3);
    fBetheBloch->SetParameter(4, fdEdx_p4);
    
}

//____________________________________________________________________

void AtlPhiFinder::PrintCutValues(){

    cout << "----------------------------------------------" << endl;
    cout << "----------------------------------------------" << endl;
    if ( fMode == kKinFit ){
	cout << endl << " Used mode: kKinFit " << endl;
	cout << " ----------------------" << endl << endl
	     << " Prob. of MC Truth Matching Cut =  " << fMatchingProb_min << endl;
	cout << "-----------------------------------------------------" << endl;
	cout <<  " " << fPhi_M_min << " < M(Phi)(GeV)    < " << fPhi_M_max << endl;
	cout << "-----------------------------------------------------" << endl;
	cout << " max. Angle between Kaon Tracks (deg) : " << (fAngle_max * 180)/TMath::Pi() << endl;
	cout << "-----------------------------------------------------" << endl;
	cout << " maximum Chi2/NDoF of Vertex Fit: " << endl
	     << "  Chi2/NDoF_Vtx = "
	     << fVertexChi2ovNDoF_max << endl;
	cout << "------------------------------------------------------" << endl;
	cout << " maximum Chi2/NDoF of Track (to primary Vertex) Fit: " << endl
	     << "  Chi2_Vtx/NDoF_Vtx = "
	     << fTrackChi2ovNDoF_max << endl;
	cout << "------------------------------------------------------" << endl;
	cout << "fKinFitPvalue (Quality of kinematic fit): " << fKinFitPvalue_min << endl;
	cout << "------------------------------------------------------" << endl;
	cout << "Eta between " << fPhi_Eta_min << " and " << fPhi_Eta_max << endl;
	cout << "------------------------------------------------------" << endl;
	cout << "Pmag: " << fPmag_min << " < Pmag <  " << fPmag_max << endl;
	cout << "------------------------------------------------------" << endl;
	cout << "dEdx cut, max. Diff between measured and calculated dEdx: " << fdEdx_Delta_max << endl;
	cout << "------------------------------------------------------" << endl;
	cout << "------------------------------------------------------" << endl;
    }
    else if (fMode == kCutBased)  {
	cout << endl << " Used mode: kCutBased " << endl;  
	cout << " ----------------------------------------------" << endl;
	cout << " Used CutDefaults:" <<endl;
	cout << " ----------------------------------------------" << endl;
	
    	cout << scientific << setprecision(3) << " " << fSignalKaon_Pt_min <<
	    " < Pt(Pion)(GeV)  < " << fSignalKaon_Pt_max << endl;
	cout << scientific << fSignalKaon_Eta_min << " < Eta(Kaon)      < "
	     << fSignalKaon_Eta_max << endl;
	cout << scientific << " " << fPhi_M_min << " < M(Phi)(GeV)    < " << fPhi_M_max << endl;
	cout << "----------------------------------------------" << endl;
	cout << scientific << " Kaon mass hypothesis: " << endl
	     << "           M_K (GeV)  = " << fm_K << endl;
	cout << "----------------------------------------------" << endl;
	cout << " max. Angle between Kaon Tracks (deg) : " << (fAngle_max * 180)/TMath::Pi() << endl;
	cout << "----------------------------------------------" << endl;
	cout << scientific << " maximum Chi2/NDoF of Vertex Fit: " << endl
	     << "  Chi2/NDoF_Vtx = "
	     << fVertexChi2ovNDoF_max << endl;
	cout << "----------------------------------------------" << endl;
	cout << scientific << " maximum Chi2/NDoF of Track (to primary Vertex) Fit: " << endl
	     << "  Chi2_Vtx/NDoF_Vtx = "
	     << fTrackChi2ovNDoF_max << endl;
	cout << "----------------------------------------------" << endl;
	cout << "Pmag: " << fPmag_min << " < Pmag <  " << fPmag_max << endl;
	cout << "------------------------------------------------------" << endl;
	cout << "dEdx cut, max. Diff between measured and calculated dEdx: " << fdEdx_Delta_max << endl;
	cout << "----------------------------------------------" << endl;
	cout << "----------------------------------------------" << endl;
    }
}

//____________________________________________________________________

void AtlPhiFinder::Terminate() {
    cout << "Terminate begins" << endl;
    
    AtlKinFitterTool::Terminate();
    
    if ( fMode == kCutBased ) {
	// subtract back- from foreground (for phi mass histo) to see a mass peak
	fHistPhireco_m_KK_diff->Add(fHistPhireco_m_KK, 1.0);
	fHistPhireco_m_KK_diff->Add(fHistPhireco_m_KK_bkg, -1.0);
    } else {
	fHistdEdx_betagamma->ProfileX("h_dEdx_betagamma_pfx", 1, fHistdEdx_betagamma->GetXaxis()->GetLast());
    }
    
    // make nice Labels for pdg histos (in MC TM)
    fHistPhi_MC_same->LabelsDeflate("X");
    fHistPhi_MC_same->LabelsOption("v");
    fHistPhi_MC_KKsame->LabelsDeflate("X");
    fHistPhireco_TRK->LabelsDeflate("X");

    cout << endl << " ----------------------------------------" << endl;
    cout << endl << "INFO: Terminating AtlPhiFinder!" << endl;
}

//____________________________________________________________________


Int_t AtlPhiFinder::DoTruthMatch(AtlIDTrack *trk1, AtlIDTrack *trk2) {
    //
    // retuns 1 in case of true Phi, -1 if not
    // returns 0 if no decision possible (see fMatchingProb_min)
    //

    fHistPhireco_MCParticle_Prob->Fill(trk1->GetMCParticleProb(), GetPreTagEvtWeight());
    fHistPhireco_MCParticle_Prob->Fill(trk2->GetMCParticleProb(), GetPreTagEvtWeight());

    // remove "_bar" from particle name to collect antiparticle and particle in the same bin
    if (trk1->GetMCParticle()->GetMother() == NULL)
	return 0;
    const char* fullPdgName = trk1->GetMCParticle()->GetMother()->GetPdgName();
    char pdgName[strlen(fullPdgName)];
    if (strstr(fullPdgName, "_bar") == NULL) {
	strcpy(pdgName, fullPdgName);
    } else {
	strncpy(pdgName, fullPdgName, strlen(fullPdgName)-4);
	pdgName[strlen(fullPdgName)-4] = '\0';
    }
    
    // Hitbased TruthMatching

    if ( ! fUseKaonPurity ) {
	// Phi Purity
	if(  trk1->HasValidTruthMatch(fMatchingProb_min)
	     && trk2->HasValidTruthMatch(fMatchingProb_min)) {
	    if( (trk1->GetMCParticle()->GetMother() == NULL) || (trk2->GetMCParticle()->GetMother() == NULL) ) {
		return 0;}
	    if( (TMath::Abs(trk1->GetMCParticle()->GetPdgCode())== 321)
		&& (TMath::Abs(trk2->GetMCParticle()->GetPdgCode())== 321)
		&& (TMath::Abs(trk1->GetMCParticle()->GetMother()->GetPdgCode())== 333)
		&& (trk1->GetMCParticle()->GetMother() == trk2->GetMCParticle()->GetMother()) ) {
		fHistPhi_MC_same->Fill(pdgName, GetPreTagEvtWeight());
		fHistPhi_MC_KKsame->Fill(pdgName, GetPreTagEvtWeight());
		return 1;
	    } else {
		if ( (trk1->GetMCParticle()->GetMother() == trk2->GetMCParticle()->GetMother()) ) {
		    fHistPhi_MC_same->Fill(pdgName, GetPreTagEvtWeight());
		    if ( (TMath::Abs(trk1->GetMCParticle()->GetPdgCode()) == 321) && (TMath::Abs(trk2->GetMCParticle()->GetPdgCode()) == 321) ) {
			fHistPhi_MC_KKsame->Fill(pdgName, GetPreTagEvtWeight());
		    }
		}
		else {
		    fHistPhi_MC_same->Fill("Comb", GetPreTagEvtWeight());
		    if ( (TMath::Abs(trk1->GetMCParticle()->GetPdgCode()) == 321) && (TMath::Abs(trk2->GetMCParticle()->GetPdgCode()) == 321) ) {
			fHistPhi_MC_KKsame->Fill("Comb", GetPreTagEvtWeight());
		    }
		}
		return -1;
	    }  
	} else {
	    return 0;
	}
    } else if ( fUseKaonPurity ) {
	// Kaon Purity
	if( trk1->HasValidTruthMatch(fMatchingProb_min)
	    && trk2->HasValidTruthMatch(fMatchingProb_min) ) {
	    if( (TMath::Abs(trk1->GetMCParticle()->GetPdgCode())== 321)
		&& (TMath::Abs(trk2->GetMCParticle()->GetPdgCode())== 321) ){
		return 2;
	    }
	    if( (TMath::Abs(trk1->GetMCParticle()->GetPdgCode())== 321)
		|| (TMath::Abs(trk2->GetMCParticle()->GetPdgCode())== 321) ) {
		return 1;
	    } else {
		return -1;
	    }  
	} else {
	    return 0;
	}
    }
    return 0;
}

//____________________________________________________________________

void AtlPhiFinder::FillHistograms_MCTruthStudy(AtlPhiDecayKK *PhiCandidate, char type) {
    // !!! key 'type' must be "s" for signal or "b" for background !!!
    // !!! routine could only be processed in case of a mc sample  !!!
    //
    // fills several histograms like pointing angle, invariant mass,
    // decay angle, ... separately for bkg and signal (Phi candiates that fulfill
    // the monte-carlo truth-matching as well as those that fail)
    
    // filling histograms
    switch (type) {
	case 's':
	    fHistPhireco_m_MCsignal     ->Fill(PhiCandidate->M("REC")      ,GetPreTagEvtWeight());
	    fHistPhireco_pt_MCsignal    ->Fill(PhiCandidate->Pt()          ,GetPreTagEvtWeight());  
	    fHistPhireco_phi_MCsignal   ->Fill(PhiCandidate->Phi()         ,GetPreTagEvtWeight()); 
	    fHistPhireco_eta_MCsignal   ->Fill(PhiCandidate->Eta()         ,GetPreTagEvtWeight());
	    fHistPhireco_OpeningAngle_MCsignal  ->Fill(PhiCandidate->GetOpeningAngle() ,GetPreTagEvtWeight());
	    fHistPhireco_DecayAngle_MCsignal    ->Fill(PhiCandidate->GetDecayAngle()   ,GetPreTagEvtWeight());
	    fHistPhireco_Number_Hits_MCsignal->Fill(PhiCandidate->GetKPlus()->GetN_PixelHits() + PhiCandidate->GetKPlus()->GetN_SCTHits() + PhiCandidate->GetKPlus()->GetN_TRTHits(), GetPreTagEvtWeight());
	    fHistPhireco_Number_Hits_MCsignal->Fill(PhiCandidate->GetKMinus()->GetN_PixelHits() + PhiCandidate->GetKMinus()->GetN_SCTHits() + PhiCandidate->GetKMinus()->GetN_TRTHits(), GetPreTagEvtWeight());  
	    fHistPhireco_ChiNDoF_Trks_MCsignal->Fill(PhiCandidate->GetKPlus()->GetChi2()/PhiCandidate->GetKPlus()->GetNDoF(), GetPreTagEvtWeight());
	    fHistPhireco_ChiNDoF_Trks_MCsignal->Fill(PhiCandidate->GetKMinus()->GetChi2()/PhiCandidate->GetKMinus()->GetNDoF(), GetPreTagEvtWeight());
	    break;
	case 'b':
	    fHistPhireco_m_MCbkg     ->Fill(PhiCandidate->M("REC")      ,GetPreTagEvtWeight());
	    fHistPhireco_pt_MCbkg    ->Fill(PhiCandidate->Pt()          ,GetPreTagEvtWeight());  
	    fHistPhireco_phi_MCbkg   ->Fill(PhiCandidate->Phi()         ,GetPreTagEvtWeight()); 
	    fHistPhireco_eta_MCbkg   ->Fill(PhiCandidate->Eta()         ,GetPreTagEvtWeight());
	    fHistPhireco_OpeningAngle_MCbkg->Fill(PhiCandidate->GetOpeningAngle() ,GetPreTagEvtWeight());
	    fHistPhireco_DecayAngle_MCbkg->Fill(PhiCandidate->GetDecayAngle()     ,GetPreTagEvtWeight());
	    fHistPhireco_Number_Hits_MCbkg->Fill(PhiCandidate->GetKPlus()->GetN_PixelHits() + PhiCandidate->GetKPlus()->GetN_SCTHits() + PhiCandidate->GetKPlus()->GetN_TRTHits(), GetPreTagEvtWeight());
	    fHistPhireco_Number_Hits_MCbkg->Fill(PhiCandidate->GetKMinus()->GetN_PixelHits() + PhiCandidate->GetKMinus()->GetN_SCTHits() + PhiCandidate->GetKMinus()->GetN_TRTHits(), GetPreTagEvtWeight());  
	    fHistPhireco_ChiNDoF_Trks_MCbkg->Fill(PhiCandidate->GetKPlus()->GetChi2()/PhiCandidate->GetKPlus()->GetNDoF(), GetPreTagEvtWeight());
	    fHistPhireco_ChiNDoF_Trks_MCbkg->Fill(PhiCandidate->GetKMinus()->GetChi2()/PhiCandidate->GetKMinus()->GetNDoF(), GetPreTagEvtWeight());
	    break;
	default:
	    cout << endl << endl
		 <<"!!! Error using FillHistograms_MCTruthStudy(AtlPhiDecayKK*,char) in AtlPhiFinder"
		 << endl << "Select either 's' or 'b' as key !!!"
		 << endl << endl;
	    break;
    }
}

// ____________________________________________________________________

Float_t AtlPhiFinder::Delta_dEdx(AtlIDTrack *trk1) {

/*    TLorentzVector p_trk;
    p_trk.SetVectM(trk1->P(), fm_K);
    return TMath::Abs(fBetheBloch->Eval(p_trk.Beta()*p_trk.Gamma()) - trk1->GetPixeldEdx()) / trk1->GetPixeldEdx();
*/
    Float_t predict = fBetheBloch->Eval(trk1->Pmag()/fm_K);
    return TMath::Abs( (predict - trk1->GetPixeldEdx()) / predict );
}

// ____________________________________________________________________


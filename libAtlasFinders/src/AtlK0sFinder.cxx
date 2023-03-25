//____________________________________________________________________
//
// K0s decay finder class
//
// K0s will be reconstructed while looping over all secondary vertices
// with 2 daughter tracks. Oppositely charged tracks will be added to
// K0s-List in AtlEvent while same charged tracks will give a first
// first estimation for background K0s and will be stored in a TList
// called fKkgK0sDecays.
// The Decay decision will be done either by a cut bassed reconstruction
// or by using the kinemativ fitter. Switching is possible by a Flag
// EMode (kCutBased, kKinFit) that can be set in the constructor via
// SetMode(EMode)

//
// Author: Sebastian Beumler <mailto:beumler@physik.hu-berlin.de>
// Copyright: 2010 (C) Sebastian Beumler
//
#ifndef ATLAS_AtlK0sFinder
#include <AtlK0sFinder.h>
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
ClassImp(AtlK0sFinder);
#endif

//____________________________________________________________________

AtlK0sFinder::AtlK0sFinder(const char* name, const char* title) :
    AtlKinFitterTool(name, title) {
    //
    // Default constructor
    //

    // set default mode
    SetMode(kKinFit);
    SetDebugMode(kFALSE);
    
    fBkgK0sDecays = new TList;
    fNK0s = 0;

    // Bkg Decay Angle analysis
    NLambdapos           = 0;
    NLambdaBarpos        = 0;
    NPiPluspos           = 0;
    NProtonpos           = 0;

    NLambdaneg           = 0;
    NLambdaBarneg        = 0;
    NPiPlusneg           = 0;
    NProtonneg           = 0;

    
}

//____________________________________________________________________

AtlK0sFinder::~AtlK0sFinder() {
    //
    // Default destructor
    //
    fBkgK0sDecays->Delete();
    delete fBkgK0sDecays;

    if ( IsDebugRun() )    fDebugOutput.close();
    
}

//____________________________________________________________________

void AtlK0sFinder::SetBranchStatus() {
    //
    // Switch on needed branches
    //
    fTree->SetBranchStatus("fN_IDTracks*",         kTRUE);  
    fTree->SetBranchStatus("fIDTracks*",           kTRUE);
    fTree->SetBranchStatus("fN_Vertices*",         kTRUE);
    fTree->SetBranchStatus("fVertices*",           kTRUE);
    fTree->SetBranchStatus("fN_MCParticles*",      kTRUE);
    fTree->SetBranchStatus("fMCParticles*",        kTRUE);
}

//____________________________________________________________________

void AtlK0sFinder::BookHistograms() {
    //
    // Book histograms
    //
    
    
    //Debugging
    if (IsDebugRun() ){
	fDebugOutput.open("DebugOutput.dat");
    }
    
    // Book KinFitter Histograms
    AtlKinFitterTool::BookHistograms();
    // Set X-Variable for Efficiency Computation depending on R_Vtx
    AtlKinFitterTool::SetTruthMatchXTitle("cos_{#Theta*} (rad)");
    AtlKinFitterTool::SetTruthMatchBinning(20, 0.0, 1.);
    //Outout std::out
    PrintCutValues();

    
    // ======================
    //       SIGNAL
    // ======================
    gDirectory->mkdir("Signal", "Oppositely charged candidates");
    gDirectory->cd("Signal");

    // general tuning histograms
    fHistPionMatchingProb = new TH1F("h_Pion_MatchingProb",
				     "Matching probability (hitbased TruthMatching) of particles matching pions",
				     50, 0, 1);
    fHistPionMatchingProb->SetXTitle("MatchingProb");
    fHistPionMatchingProb->SetYTitle("Number of Entries");
    
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
    
    
    // K0s
    // ---
    gDirectory->mkdir("PiPi", "K0s -> pi+pi-");
    gDirectory->cd("PiPi");

    if ( fMode == kKinFit ) {
	fHistK0sreco_m_PiPi   = new TH1F("h_K0s_m_PiPi", "Invariant K0s mass (K0s->PiPi)",
					 50, 0.49, 0.45);
    }else if (fMode == kCutBased){
	fHistK0sreco_m_PiPi   = new TH1F("h_K0s_m_PiPi", "Invariant K0s mass (K0s->PiPi)",
					 50, 0.4, 0.6);
    }
    fHistK0sreco_m_PiPi->SetXTitle("m_{K0s} (GeV)");
    fHistK0sreco_m_PiPi->SetYTitle("Number of Entries");
    fHistK0sreco_pt_PiPi  = new TH1F("h_K0s_pt_PiPi", "K0s-p_{#perp} (K0s->PiPi)",
				     400, 0, 400);
    fHistK0sreco_pt_PiPi->SetXTitle("p_{#perp} (GeV)");
    fHistK0sreco_pt_PiPi->SetYTitle("Number of Entries");
    fHistK0sreco_phi_PiPi = new TH1F("h_K0s_phi_PiPi", "K0s #phi distribution (K0s->PiPi)",
				     128, -3.2, 3.2);
    fHistK0sreco_phi_PiPi->SetXTitle("#phi (rad)");
    fHistK0sreco_phi_PiPi->SetYTitle("Number of Entries");
    fHistK0sreco_eta_PiPi = new TH1F("h_K0s_eta_PiPi", "K0s #eta distribution (K0s->PiPi)",
				 50, -3, 3);
    fHistK0sreco_eta_PiPi->SetXTitle("#eta");
    fHistK0sreco_eta_PiPi->SetYTitle("Number of Entries");
    fHistK0sreco_N_PiPi   = new TH1F("h_K0s_N_PiPi", "Reconstructed K0s' per event (K0s->PiPi)",
				     11, -0.5, 10.5);
    fHistK0sreco_N_PiPi->SetXTitle("N_{K0s}/evt");
    fHistK0sreco_N_PiPi->SetYTitle("Number of Entries");
    fHistK0sreco_tau_PiPi = new TH1F("h_K0s_tau_PiPi", "Reconstructed K0s' lifetime (K0s->PPi)", 
				       100, 0, 300);
    fHistK0sreco_tau_PiPi->SetXTitle("#tau (ps)");
    fHistK0sreco_tau_PiPi->SetYTitle("Number of Entries");
    fHistK0sreco_DecayLength_PiPi = new TH1F("h_K0s_DecayLength_PiPi",
					     "Reconstructed K0s' decay length (K0s->PiPi)", 
				       100, 0, 300);
    fHistK0sreco_DecayLength_PiPi->SetXTitle("#DecayLength ");
    fHistK0sreco_DecayLength_PiPi->SetYTitle("Number of Entries");
    fHistK0sreco_OpeningAngle_PiPi = new TH1F("h_K0s_OpeningAngle_PiPi",
					      "opening angle between pion tracks",
				     128, 0, 3.2);
    fHistK0sreco_OpeningAngle_PiPi->SetXTitle("#alpha (rad)");
    fHistK0sreco_OpeningAngle_PiPi->SetYTitle("Number of Entries");
    fHistK0sreco_DecayAngle_PiPi = new TH1F("h_K0s_DecayAngle_PiPi",
					    "decay angle between K0s and pi+ in K0s' restframe (reconstructed K0s)",
				     50, -1, 1);
    fHistK0sreco_DecayAngle_PiPi->SetXTitle("cos(#beta)");
    fHistK0sreco_DecayAngle_PiPi->SetYTitle("Number of Entries");
    fHistK0sreco_PointingAngle_PiPi = new TH1F("h_K0s_PointingAngle_PiPi",
					       "Cos of angle between K0s' momentum and vec(PV)-vec(SecVtx)",
					       150, 0.8, 1);
    fHistK0sreco_PointingAngle_PiPi->SetXTitle("cos(#delta)");
    fHistK0sreco_PointingAngle_PiPi->SetYTitle("Number of Entries");
    fHistK0sreco_R_vtx = new TH1F("h_K0s_R_vtx", "Radial distance of secondary vertices", 120, 0.0, 60.0);
    fHistK0sreco_R_vtx->SetXTitle("r_{#perp} (cm)");
    fHistK0sreco_R_vtx->SetYTitle("Number of Entries");
    fHistK0sreco_RvtxMee = new TH2F("h_K0sreco_RvtxMee",
					  "Radial dust of sec vtx and Mee M_ee mass hypothesis",
					  60, 0.0, 60.0, 10, 0.0, 1.5);
    fHistK0sreco_RvtxMee    ->SetXTitle("R_{#perp} (cm)");
    fHistK0sreco_RvtxMee    ->SetYTitle("M_{ee}");
    fHistK0sreco_RvtxMee    ->SetZTitle("Number of Entries");
    fHistK0sreco_RvtxMee    ->SetOption("COLZ");

    fHistK0sreco_Pions_PtEta = new TH2F("h_K0s_Pions_PtEta", "#pion Pt and eta",
					400, 0, 400, 50, -5, 5);
    fHistK0sreco_Pions_PtEta->SetXTitle("p_{T} (GeV)");
    fHistK0sreco_Pions_PtEta->SetYTitle("#eta");
    fHistK0sreco_Pions_PtEta->SetZTitle("Number of Entries");
        
    // Pion (Pi+) histograms in sub-folder
    gDirectory->mkdir("Piplus", "Pi+ from K0s -> PiPi");
    gDirectory->cd("Piplus");
    
    fHistK0sreco_PiPlus_pt = new TH1F("h_K0s_PiPlus_pt", "#pi^{+}-p_{#perp} (K0s->#pi#pi)",
				      400, 0, 400);
    fHistK0sreco_PiPlus_pt->SetXTitle("p_{#perp} (GeV)");
    fHistK0sreco_PiPlus_pt->SetYTitle("Number of Entries");
    fHistK0sreco_PiPlus_eta = new TH1F("h_K0s_PiPlus_eta", "#pi^{+}:  #eta distribution (K0s->#pi#pi)",
				       50, -3, 3);
    fHistK0sreco_PiPlus_eta->SetXTitle("#eta");
    fHistK0sreco_PiPlus_eta->SetYTitle("Number of Entries");
    fHistK0sreco_PiPlus_phi = new TH1F("h_K0s_PiPlus_phi", "#pi^{+}:  #phi distribution (K0s->#pi#pi)",
				    50, -3.14, 3.14);
    fHistK0sreco_PiPlus_phi->SetXTitle("#phi");
    fHistK0sreco_PiPlus_phi->SetYTitle("Number of Entries");
    
    gDirectory->cd("../"); 
    
    // Pion (Pi-) histograms in sub-folder
    gDirectory->mkdir("PiMinus", "Pi- from K0s0 -> PiPi");
    gDirectory->cd("PiMinus");
    
    fHistK0sreco_PiMinus_pt = new TH1F("h_K0s_PiMinus_pt", "#pi^{-}-p_{#perp} (K0s->#pi#pi)",
				       400, 0, 400);
    fHistK0sreco_PiMinus_pt->SetXTitle("p_{#perp} (GeV)");
    fHistK0sreco_PiMinus_pt->SetYTitle("Number of Entries");
    fHistK0sreco_PiMinus_eta = new TH1F("h_K0s_PiMinus_eta", "#pi^{-}:  #eta distribution (K0s->#pi#pi)",
					50, -5, 5);
    fHistK0sreco_PiMinus_eta->SetXTitle("#eta");
    fHistK0sreco_PiMinus_eta->SetYTitle("Number of Entries");
    fHistK0sreco_PiMinus_phi = new TH1F("h_K0s_PiMinus_phi", "#pi^{-}:  #phi distribution (K0s->#pi#pi)",
					50, -3.14, 3.14);
    fHistK0sreco_PiMinus_phi->SetXTitle("#phi");
    fHistK0sreco_PiMinus_phi->SetYTitle("Number of Entries");
    
    
    

    gDirectory->cd("../../../");
    // =================
    //    BACKGROUND
    // =================
     gDirectory->mkdir("Background", "Same charged candidates");
     gDirectory->cd("Background");



    // ===========================================
    // Book bkg histograms for like-sign approach
    // ===========================================
           
    gDirectory->mkdir("Like-sign", "K0s -> pi+pi+ / pi-pi-");
    gDirectory->cd("Like-sign");

    if ( fMode == kKinFit ) {
	fHistK0sreco_m_LS_bkg   = new TH1F("h_K0s_m_LS_bkg", "Invariant K0s mass (equaly signed pions)",
					     50, 0.496, 0.4975);
     }else if (fMode == kCutBased){
	fHistK0sreco_m_LS_bkg   = new TH1F("h_K0s_m_PiPi", "Invariant K0s mass (equaly signed pions)",
					     50, 0.4, 0.6);
    }
    fHistK0sreco_m_LS_bkg->SetXTitle("m_{K0s} (GeV)");
    fHistK0sreco_m_LS_bkg->SetYTitle("Number of Entries");
    fHistK0sreco_pt_LS_bkg  = new TH1F("h_K0s_pt_LS_bkg", "K0s-p_{#perp} (equaly signed pions)",
				       400, 0, 400);
    fHistK0sreco_pt_LS_bkg->SetXTitle("p_{#perp} (GeV)");
    fHistK0sreco_pt_LS_bkg->SetYTitle("Number of Entries");
    fHistK0sreco_phi_LS_bkg = new TH1F("h_K0s_phi_LS_bkg", "K0s #phi distribution (equaly signed pions)",
				       128, -3.2, 3.2);
    fHistK0sreco_phi_LS_bkg->SetXTitle("#phi (rad)");
    fHistK0sreco_phi_LS_bkg->SetYTitle("Number of Entries");
    fHistK0sreco_eta_LS_bkg = new TH1F("h_K0s_eta_LS_bkg", "K0s #eta distribution (equaly signed pions)",
				       50, -3, 3);
    fHistK0sreco_eta_LS_bkg->SetXTitle("#eta");
    fHistK0sreco_eta_LS_bkg->SetYTitle("Number of Entries");
    fHistK0sreco_N_LS_bkg   = new TH1F("h_K0s_N_LS_bkg", "Reconstructed K0s' per event (equaly signed pions)",
				       11, -0.5, 10.5);
    fHistK0sreco_N_LS_bkg->SetXTitle("N_{K0s}/evt");
    fHistK0sreco_N_LS_bkg->SetYTitle("Number of Entries");
    fHistK0sreco_tau_LS_bkg = new TH1F("h_K0s_tau_LS_bkg", "Reconstructed K0s' lifetime (equaly signed pions)", 
					 100, 0, 300);
    fHistK0sreco_tau_LS_bkg->SetXTitle("#tau (ps)");
    fHistK0sreco_tau_LS_bkg->SetYTitle("Number of Entries");
    fHistK0sreco_DecayLength_LS_bkg = new TH1F("h_K0s_DecayLength_LS_bkg",
					    "Reconstructed K0s' decay length (equaly signed pions)", 
					    100, 0, 300);
    fHistK0sreco_DecayLength_LS_bkg->SetXTitle("#DecayLength ");
    fHistK0sreco_DecayLength_LS_bkg->SetYTitle("Number of Entries");

    // Pion (Pi+) histograms in sub-folder
    gDirectory->mkdir("Piplus", "Pi+ from K0s -> Pi+Pi+");
    gDirectory->cd("Piplus");

    fHistK0sreco_PiPlus_pt_LS_bkg = new TH1F("h_K0s_PiPlus_pt_LS_bkg",
					   "#pi^{+}-p_{#perp} (K0s->#pi#pi)",
					   400, 0, 400);
    fHistK0sreco_PiPlus_pt_LS_bkg->SetXTitle("p_{#perp} (GeV)");
    fHistK0sreco_PiPlus_pt_LS_bkg->SetYTitle("Number of Entries");
    fHistK0sreco_PiPlus_eta_LS_bkg = new TH1F("h_K0s_PiPlus_eta_LS_bkg",
					      "#pi^{+}:  #eta distribution (K0s->#pi#pi)",
					      50, -3, 3);
    fHistK0sreco_PiPlus_eta_LS_bkg->SetXTitle("#eta");
    fHistK0sreco_PiPlus_eta_LS_bkg->SetYTitle("Number of Entries");
    fHistK0sreco_PiPlus_phi_LS_bkg = new TH1F("h_K0s_PiPlus_phi_LS_bkg",
					      "#pi^{+}:  #phi distribution (K0s->#pi#pi)",
					      50, -3.14, 3.14);
    fHistK0sreco_PiPlus_phi_LS_bkg->SetXTitle("#phi");
    fHistK0sreco_PiPlus_phi_LS_bkg->SetYTitle("Number of Entries");
    
    gDirectory->cd("../"); 
    // Pion (Pi-) histograms in sub-folder
    gDirectory->mkdir("PiMinus", "Pi- from K0s0 -> Pi-Pi-");
    gDirectory->cd("PiMinus");
    
    fHistK0sreco_PiMinus_pt_LS_bkg = new TH1F("h_K0s_PiMinus_pt_LS_bkg",
					      "#pi^{-}-p_{#perp} (K0s->#pi#pi)",
					      400, 0, 400);
    fHistK0sreco_PiMinus_pt_LS_bkg->SetXTitle("p_{#perp} (GeV)");
    fHistK0sreco_PiMinus_pt_LS_bkg->SetYTitle("Number of Entries");
    fHistK0sreco_PiMinus_eta_LS_bkg = new TH1F("h_K0s_PiMinus_eta_LS_bkg",
					     "#pi^{-}:  #eta distribution (K0s->#pi#pi)",
					       50, -5, 5);
    fHistK0sreco_PiMinus_eta_LS_bkg->SetXTitle("#eta");
    fHistK0sreco_PiMinus_eta_LS_bkg->SetYTitle("Number of Entries");
    fHistK0sreco_PiMinus_phi_LS_bkg = new TH1F("h_K0s_PiMinus_phi_LS_bkg",
					       "#pi^{-}:  #phi distribution (K0s->#pi#pi)",
					       50, -3.14, 3.14);
    fHistK0sreco_PiMinus_phi_LS_bkg->SetXTitle("#phi");
    fHistK0sreco_PiMinus_phi_LS_bkg->SetYTitle("Number of Entries");
     


    gDirectory->cd("../../../");


    
    // ========================
    //     MC TRUTH STUDY
    // ========================

    gDirectory->mkdir("MCTruthStudy", "Small Truth Study with hitbased Truth-matching");
    gDirectory->cd("MCTruthStudy");


    fHistK0sreco_truthtype = new TH2F("h_K0sreco_truthtype", "truth type of K0s candidates", 6260, -3130, 3130, 50, 0.0, 1.0);
    fHistK0sreco_truthtype->SetXTitle("");
    fHistK0sreco_truthtype->SetYTitle("P(#Chi^{2})");
    fHistK0sreco_truthtype->SetZTitle("Number of Entries");
    fHistK0sreco_truthtype->SetOption("COLZ");
    

    
    // =============================================================================
    // Book signal histograms for K0s-candidates that fulfill the MC Truth Matching
    // =============================================================================
    
    gDirectory->mkdir("Signal_MC", "K0s-candidates that fulfill the MC Truth Matching");
    gDirectory->cd("Signal_MC");

    if ( fMode == kKinFit ) {
	fHistK0sreco_m_MCsignal   = new TH1F("h_K0s_m_MCsignal", "Invariant K0s mass (equaly signed pions)",
					    50, 0.496, 0.4975);
    }else if (fMode == kCutBased){
	fHistK0sreco_m_MCsignal   = new TH1F("h_K0s_m_PiPi", "Invariant K0s mass (equaly signed pions)",
					50, 0.4, 0.6);
    }
    fHistK0sreco_m_MCsignal->SetXTitle("m_{K0s} (GeV)");
    fHistK0sreco_m_MCsignal->SetYTitle("Number of Entries");
    fHistK0sreco_pt_MCsignal  = new TH1F("h_K0s_pt_MCsignal", "K0s-p_{#perp} (equaly signed pions)",
				    400, 0, 400);
    fHistK0sreco_pt_MCsignal->SetXTitle("p_{#perp} (GeV)");
    fHistK0sreco_pt_MCsignal->SetYTitle("Number of Entries");
    fHistK0sreco_phi_MCsignal = new TH1F("h_K0s_phi_MCsignal", "K0s #phi distribution (equaly signed pions)",
				    128, -3.2, 3.2);
    fHistK0sreco_phi_MCsignal->SetXTitle("#phi (rad)");
    fHistK0sreco_phi_MCsignal->SetYTitle("Number of Entries");
    fHistK0sreco_eta_MCsignal = new TH1F("h_K0s_eta_MCsignal", "K0s #eta distribution (equaly signed pions)",
				    50, -3, 3);
    fHistK0sreco_eta_MCsignal->SetXTitle("#eta");
    fHistK0sreco_eta_MCsignal->SetYTitle("Number of Entries");
    fHistK0sreco_N_MCsignal   = new TH1F("h_K0s_N_MCsignal", "Reconstructed K0s' per event (equaly signed pions)",
				    11,-0.5,10.5);
    fHistK0sreco_N_MCsignal->SetXTitle("N_{K0s}/evt");
    fHistK0sreco_N_MCsignal->SetYTitle("Number of Entries");
    fHistK0sreco_tau_MCsignal = new TH1F("h_K0s_tau_MCsignal", "Reconstructed K0s' lifetime (equaly signed pions)", 
				    100, 0, 300);
    fHistK0sreco_tau_MCsignal->SetXTitle("#tau (ps)");
    fHistK0sreco_tau_MCsignal->SetYTitle("Number of Entries");
    fHistK0sreco_DecayLength_MCsignal = new TH1F("h_K0s_DecayLength_MCsignal",
					    "Reconstructed K0s' decay length (equaly signed pions)", 
					    100, 0, 50);
    fHistK0sreco_DecayLength_MCsignal->SetXTitle("DecayLength ");
    fHistK0sreco_DecayLength_MCsignal->SetYTitle("Number of Entries");
    fHistK0sreco_OpeningAngle_MCsignal = new TH1F("h_K0s_OpeningAngle_MCsignal",
					     "opening angle between pion tracks",
					     128, 0, 3.2);
    fHistK0sreco_OpeningAngle_MCsignal->SetXTitle("#alpha (rad)");
    fHistK0sreco_OpeningAngle_MCsignal->SetYTitle("Number of Entries");
    fHistK0sreco_DecayAngle_MCsignal = new TH1F("h_K0s_DecayAngle_MCsignal",
					   "decay angle for reconstructed K0s that fulfill truthmatching",
					   50, -1, 1);
    fHistK0sreco_DecayAngle_MCsignal->SetXTitle("cos(#beta) (rad)");
    fHistK0sreco_DecayAngle_MCsignal->SetYTitle("Number of Entries");
    fHistK0sreco_PointingAngle_MCsignal = new TH1F("h_K0s_PointingAngle_MCsignal",
					      "cos of angle between K0s' momentum and vec(PV)-vec(SecVtx)",
					      1000, 0.99, 1);
    fHistK0sreco_PointingAngle_MCsignal->SetXTitle("cos(#delta)");
    fHistK0sreco_PointingAngle_MCsignal->SetYTitle("Number of Entries");
    fHistK0sreco_R_vtx_MCsignal = new TH1F("h_K0s_R_vtx_MCsignal",
					   "Radial distance of secondary vertices",
					   120, 0.0, 60.0);
    fHistK0sreco_R_vtx_MCsignal->SetXTitle("r_{#perp} (cm)");
    fHistK0sreco_R_vtx_MCsignal->SetYTitle("Number of Entries");
    fHistK0sreco_RvtxMee_MCsignal = new TH2F("h_K0sreco_RvtxMee",
					  "Radial dust of sec vtx and Mee M_ee mass hypothesis",
					  60, 0.0, 60.0, 10, 0.0, 1.5);
    fHistK0sreco_RvtxMee_MCsignal    ->SetXTitle("R_{#perp} (cm)");
    fHistK0sreco_RvtxMee_MCsignal    ->SetYTitle("M_{ee}");
    fHistK0sreco_RvtxMee_MCsignal    ->SetZTitle("Number of Entries");
    fHistK0sreco_RvtxMee_MCsignal    ->SetOption("COLZ");

    fHistK0sreco_Chi2_MCsignal = new TH1F("h_fHistK0sreco_Chi2_MCsignal",
					  "chi2 signal",
					  300, 0, 100);
    fHistK0sreco_Chi2_MCsignal->SetXTitle("#chi^{2} / N_{DoF}");
    fHistK0sreco_Chi2_MCsignal->SetYTitle("Number of Entries");
    fHistK0sreco_pValue_MCsignal = new TH1F("h_fHistK0sreco_pValue_MCsignal",
					    "pValue signal",
					    100, 0, 1);
    fHistK0sreco_pValue_MCsignal->SetXTitle("p-Value");
    fHistK0sreco_pValue_MCsignal->SetYTitle("Number of Entries");
    fHistK0sreco_Chi2Trk_MCsignal = new TH1F("h_fHistK0sreco_Chi2Trk_MCsignal",
					  "chi2 signal",
					  300, 0, 100);
    fHistK0sreco_Chi2Trk_MCsignal->SetXTitle("#chi^{2} / N_{DoF}");
    fHistK0sreco_Chi2Trk_MCsignal->SetYTitle("Number of Entries");
    fHistK0sreco_Chi2Vtx_MCsignal = new TH1F("h_fHistK0sreco_Chi2Vtx_MCsignal",
					  "chi2 signal",
					  300, 0, 100);
    fHistK0sreco_Chi2Vtx_MCsignal->SetXTitle("#chi^{2} / N_{DoF}");
    fHistK0sreco_Chi2Vtx_MCsignal->SetYTitle("Number of Entries");

    
    
    gDirectory->cd("../");

    // ==========================================================================
    // Book bkg histograms for K0s-candidates that fail the MC Truth Matching
    // ==========================================================================

    gDirectory->mkdir("Bkg_MC", "K0s-candidates that fail the MC Truth Matching");
    gDirectory->cd("Bkg_MC");

     if ( fMode == kKinFit ) {
	fHistK0sreco_m_MCbkg   = new TH1F("h_K0s_m_MCbkg", "Invariant K0s mass (equaly signed pions)",
					50, 0.496, 0.4975);
    }else if (fMode == kCutBased){
	fHistK0sreco_m_MCbkg   = new TH1F("h_K0s_m_PiPi", "Invariant K0s mass (equaly signed pions)",
					50, 0.4, 0.6);
    }
    fHistK0sreco_m_MCbkg->SetXTitle("m_{K0s} (GeV)");
    fHistK0sreco_m_MCbkg->SetYTitle("Number of Entries");
    fHistK0sreco_pt_MCbkg  = new TH1F("h_K0s_pt_MCbkg", "K0s-p_{#perp} (equaly signed pions)",
				    400, 0, 400);
    fHistK0sreco_pt_MCbkg->SetXTitle("p_{#perp} (GeV)");
    fHistK0sreco_pt_MCbkg->SetYTitle("Number of Entries");
    fHistK0sreco_phi_MCbkg = new TH1F("h_K0s_phi_MCbkg", "K0s #phi distribution (equaly signed pions)",
				    128, -3.2, 3.2);
    fHistK0sreco_phi_MCbkg->SetXTitle("#phi (rad)");
    fHistK0sreco_phi_MCbkg->SetYTitle("Number of Entries");
    fHistK0sreco_eta_MCbkg = new TH1F("h_K0s_eta_MCbkg", "K0s #eta distribution (equaly signed pions)",
				    50, -3, 3);
    fHistK0sreco_eta_MCbkg->SetXTitle("#eta");
    fHistK0sreco_eta_MCbkg->SetYTitle("Number of Entries");
    fHistK0sreco_N_MCbkg   = new TH1F("h_K0s_N_MCbkg", "Reconstructed K0s' per event (equaly signed pions)",
				    11,-0.5,10.5);
    fHistK0sreco_N_MCbkg->SetXTitle("N_{K0s}/evt");
    fHistK0sreco_N_MCbkg->SetYTitle("Number of Entries");
    fHistK0sreco_tau_MCbkg = new TH1F("h_K0s_tau_MCbkg", "Reconstructed K0s' lifetime (equaly signed pions)", 
				    100, 0, 300);
    fHistK0sreco_tau_MCbkg->SetXTitle("#tau (ps)");
    fHistK0sreco_tau_MCbkg->SetYTitle("Number of Entries");
    fHistK0sreco_DecayLength_MCbkg = new TH1F("h_K0s_DecayLength_MCbkg",
					    "Reconstructed K0s' decay length (equaly signed pions)", 
					    100, 0, 50);
    fHistK0sreco_DecayLength_MCbkg->SetXTitle("#DecayLength ");
    fHistK0sreco_DecayLength_MCbkg->SetYTitle("Number of Entries");
    fHistK0sreco_OpeningAngle_MCbkg = new TH1F("h_K0s_OpeningAngle_MCbkg",
					     "opening angle between pion tracks",
					     128, 0, 3.2);
    fHistK0sreco_OpeningAngle_MCbkg->SetXTitle("#alpha (rad)");
    fHistK0sreco_OpeningAngle_MCbkg->SetYTitle("Number of Entries");
    fHistK0sreco_DecayAngle_MCbkg = new TH1F("h_K0s_DecayAngle_MCbkg",
					   "decay angle of reconstructed K0s that fail truthmatching",
					   50, -1, 1);
    fHistK0sreco_DecayAngle_MCbkg->SetXTitle("cos(#beta) (rad)");
    fHistK0sreco_DecayAngle_MCbkg->SetYTitle("Number of Entries");
    fHistK0sreco_PointingAngle_MCbkg = new TH1F("h_K0s_PointingAngle_MCbkg",
					      "cos of angle between K0s' momentum and vec(PV)-vec(SecVtx)",
					      1000, 0.99, 1);
    fHistK0sreco_PointingAngle_MCbkg->SetXTitle("cos(#delta)");
    fHistK0sreco_PointingAngle_MCbkg->SetYTitle("Number of Entries");
    fHistK0sreco_R_vtx_MCbkg = new TH1F("h_K0s_R_vtx_MCbkg",
					   "Radial distance of secondary vertices",
					   120, 0.0, 60.0);
    fHistK0sreco_R_vtx_MCbkg->SetXTitle("r_{#perp} (cm)");
    fHistK0sreco_R_vtx_MCbkg->SetYTitle("Number of Entries");
    fHistK0sreco_R_vtx_MCbkg_gamma = new TH1F("h_K0s_R_vtx_MCbkg_gamma",
					      "Radial distance of secondary vertices that came from photo-conversions",
					      120, 0.0, 60.0);
    fHistK0sreco_R_vtx_MCbkg_gamma->SetXTitle("r_{#perp} (cm)");
    fHistK0sreco_R_vtx_MCbkg_gamma->SetYTitle("Number of Entries");
    fHistK0sreco_RvtxMee_MCbkg = new TH2F("h_K0sreco_RvtxMee",
					  "Radial dust of sec vtx and Mee M_ee mass hypothesis",
					  60, 0.0, 60.0, 10, 0.0, 1.5);
    fHistK0sreco_RvtxMee_MCbkg    ->SetXTitle("R_{#perp} (cm)");
    fHistK0sreco_RvtxMee_MCbkg    ->SetYTitle("M_{ee}");
    fHistK0sreco_RvtxMee_MCbkg    ->SetZTitle("Number of Entries");
    fHistK0sreco_RvtxMee_MCbkg    ->SetOption("COLZ");

    fHistK0sreco_Chi2_MCbkg = new TH1F("h_fHistK0sreco_Chi2_MCbkg",
					  "chi2 bkg",
					  300, 0, 100);
    fHistK0sreco_Chi2_MCbkg->SetXTitle("#chi^{2} / N_{DoF}");
    fHistK0sreco_Chi2_MCbkg->SetYTitle("Number of Entries");
    fHistK0sreco_pValue_MCbkg = new TH1F("h_fHistK0sreco_pValue_MCbkg",
					    "pValue bkg",
					    100, 0, 1);
    fHistK0sreco_pValue_MCbkg->SetXTitle("p-Value");
    fHistK0sreco_pValue_MCbkg->SetYTitle("Number of Entries");
    fHistK0sreco_Chi2Trk_MCbkg = new TH1F("h_fHistK0sreco_Chi2Trk_MCbkg",
					  "chi2 bkg",
					  300, 0, 100);
    fHistK0sreco_Chi2Trk_MCbkg->SetXTitle("#chi^{2} / N_{DoF}");
    fHistK0sreco_Chi2Trk_MCbkg->SetYTitle("Number of Entries");
    fHistK0sreco_Chi2Vtx_MCbkg = new TH1F("h_fHistK0sreco_Chi2Vtx_MCbkg",
					  "chi2 bkg",
					  300, 0, 100);
    fHistK0sreco_Chi2Vtx_MCbkg->SetXTitle("#chi^{2} / N_{DoF}");
    fHistK0sreco_Chi2Vtx_MCbkg->SetYTitle("Number of Entries");
    
    
    gDirectory->cd("../");

    // Decay Angle MC study
    
    gDirectory->mkdir("DecayAngle_Analysis",
		      "Analyzing the 3 areas in bkg decay angle distribution");
    gDirectory->cd("DecayAngle_Analysis");

    // DecayAngle areas
    fHist_DecayAngle_0to06 = new TH1F("h_DecayAngle_0to06",
				      "truth type with DecayAngle from 0 to 0.6",
				      6230, -3130,3130);
    fHist_DecayAngle_06to094 = new TH1F("h_DecayAngle_06to094",
					"truth type with DecayAngle from 0.6 to 0.94",
				      6230, -3130,3130);
    fHist_DecayAngle_094to1 = new TH1F("h_DecayAngle_094to1",
				       "truth type with DecayAngle from 0.94 to 1.0",
				       6230, -3130,3130);
    fHist_DecayAngleNeg_0to06 = new TH1F("h_DecayAngleNeg_0to06",
				      "truth type with DecayAngle from 0 to -0.6",
				      6230, -3130,3130);
    fHist_DecayAngleNeg_06to094 = new TH1F("h_DecayAngleNeg_06to094",
					"truth type with DecayAngle from -0.6 to -0.94",
				      6230, -3130,3130);
    fHist_DecayAngleNeg_094to1 = new TH1F("h_DecayAngleNeg_094to1",
				       "truth type with DecayAngle from -0.94 to -1.0",
				       6230, -3130,3130);
    
    fHist_MCK0s_DecayAngle = new TH1F("h_MCK0s_DecayAngle",
				      "decay angle of reconstructable mc K0s",
				      50, -1, 1);
    fHist_MCK0s_DecayAngle->SetXTitle("cos(#beta) (rad)");
    fHist_MCK0s_DecayAngle->SetYTitle("Number of Entries");
    fHistAllMCK0s_DecayAngle = new TH1F("h_AllMCK0s_DecayAngle",
					"decay angle of all mc K0s",
					50, -1, 1);
    fHistAllMCK0s_DecayAngle->SetXTitle("cos(#beta) (rad)");
    fHistAllMCK0s_DecayAngle->SetYTitle("Number of Entries");

    
    fHist_DA_Comb = new TH1F("h_DA_Comb", "decay angle of Comb Bkg",
			     50, -1, 1);
    fHist_DA_Comb->SetXTitle("Decay Angle cos(#theta*) (rad)");
    fHist_DA_Comb->SetYTitle("Number of Entries");
    fHist_DA_Gamma = new TH1F("h_DA_Gamma", "decay angle of PhotonConv",
			      50, -1, 1);
    fHist_DA_Gamma->SetXTitle("Decay Angle cos(#theta*) (rad)");
    fHist_DA_Gamma->SetYTitle("Number of Entries");
    fHist_DA_Lambda = new TH1F("h_DA_Lambda", "decay angle of Lambda Decays",
			       50, -1, 1);
    fHist_DA_Lambda->SetXTitle("Decay Angle cos(#theta*) (rad)");
    fHist_DA_Lambda->SetYTitle("Number of Entries");
    
    
    fHist_SubView = new TH1F("h_SubView", "SuvView", 1000, -1, -0.5);
    fHist_SubView->SetXTitle("cos(#beta) (rad)");
    fHist_SubView->SetYTitle("Number of Entries");

    fHist_InvMass = new TH1F("h_InvMass", "Invariant K0s mass (RootBoost)",
			     100, 0.2, 0.8);
    fHist_InvMass->SetXTitle("m_{K0s} (GeV)");
    fHist_InvMass->SetYTitle("Number of Entries");
    fHist_InvMass_OwnBoost = new TH1F("h_InvMass_OwnBoost", "Invariant K0s mass (Own Boost)",
				      100, 0.2, 0.8);
    fHist_InvMass_OwnBoost->SetXTitle("m_{K0s} (GeV)");
    fHist_InvMass_OwnBoost->SetYTitle("Number of Entries");
    
    fHist_OwnBoost = new TH1F("h_AllOwnBoost",
			      "Own LorentzTrafo",
			      50, -1, 1);
    fHist_OwnBoost->SetXTitle("cos(#beta) (rad)");
    fHist_OwnBoost->SetYTitle("Number of Entries");


    // hier
    fHist_PionsRight_Pt = new TH1F("h_PionsRight_Pt",
				  "Pt of Pions in DecayAngle Area 06 to 094",
				  60, 0, 20);
    fHist_PionsRight_Pt->SetXTitle("Pt");
    fHist_PionsRight_Pt->SetYTitle("Number of Entries");
    fHist_PionsLeft_Pt = new TH1F("h_PionsLeft_Pt",
				 "Pt of Pions in DecayAngle Area -06 to -094",
				 60, 0, 20);
    fHist_PionsLeft_Pt->SetXTitle("Pt");
    fHist_PionsLeft_Pt->SetYTitle("Number of Entries");
    

    
    gDirectory->cd("../../../");
}


//____________________________________________________________________

Bool_t AtlK0sFinder::AnalyzeEvent() {
    //
    // Event analysis
    //

    // Clean-up bkg list from previous event
    for (Int_t i = 0; i < fBkgK0sDecays->GetEntries(); i++) {
	AtlK0sDecayPiPi *obj = (AtlK0sDecayPiPi*)fBkgK0sDecays->At(i);
	delete obj;
    }
    fBkgK0sDecays->Clear("C");
    
    // Reconstruct K0s decays
    if ( fMode == kKinFit ) {
	
	// Count real K0s to compute efficiency and do DecayAngle Analysis
	if (fEvent->IsMC() ){
	    TList *K0s_list = fEvent->GetMCParticles("K_S0");
	    Int_t Nr_K0sMC = 0;
	    
	    for( Int_t i = 0; i < K0s_list->GetEntries(); i++ ){
		HepMCParticle *prt = (HepMCParticle*)(K0s_list->At(i));
		if (prt->GetN_Daughters() != 2) continue;
		// compute Decay Angle for all mc K0s
		TLorentzVector P_PiPlus_RFK0s,P_other;
		if( ((HepMCParticle*)(prt->GetDaughters()->At(0)))->IsPiPlus() ){
		    P_PiPlus_RFK0s = ((HepMCParticle*)(prt->GetDaughters()->At(0)))->P();
		    P_other = ((HepMCParticle*)(prt->GetDaughters()->At(1)))->P();
		}
		else if ( ((HepMCParticle*)(prt->GetDaughters()->At(1)))->IsPiPlus() ){
		    P_PiPlus_RFK0s = ((HepMCParticle*)(prt->GetDaughters()->At(1)))->P();
		    P_other = ((HepMCParticle*)(prt->GetDaughters()->At(0)))->P();
		}
		else continue;
		TVector3 boost = -prt->P().BoostVector();
		TLorentzVector P_Own = P_PiPlus_RFK0s;
		TLorentzVector Pother_Own = P_other;
		P_PiPlus_RFK0s.Boost(boost); // boosting trks momentum in K0s restframe
		P_other.Boost(boost);
		Float_t DecayAngle = TMath::Cos(prt->P().Angle(P_PiPlus_RFK0s.Vect()));
		fHistAllMCK0s_DecayAngle->Fill(DecayAngle, GetPreTagEvtWeight());
		
		if(DecayAngle < -0.8){
		    fHist_InvMass->Fill((P_PiPlus_RFK0s + P_other).M(),GetPreTagEvtWeight());
		    fHist_SubView->Fill(DecayAngle,GetPreTagEvtWeight());
		}

		// own boost:
		TVector3 p = P_Own.Vect();
		Float_t  E = P_Own.E();
		TVector3 pother = Pother_Own.Vect();
		Float_t Eother = Pother_Own.E();
		Float_t  Eprime = 0;
		Double_t pprime[3];
		Float_t  Eotherprime = 0;
		Double_t potherprime[3];
		Float_t gamma = prt->E()/prt->M();
		Float_t dummy = 0;
		Float_t dummy2 = 0;
		
		for( Int_t b = 0; b < 3; b++) {
		    dummy  += -boost[b]*p[b];
		    dummy2 += -boost[b]*pother[b];
		}
		for( Int_t a = 0; a < 3; a++){
		    pprime[a] = p[a] + boost[a]*gamma*(E - gamma*dummy/(gamma + 1));
		    potherprime[a] = pother[a] + boost[a]*gamma*(Eother - gamma*dummy2/(gamma + 1));
		}
		Eprime = gamma*(E - dummy);
		Eotherprime = gamma*(Eother - dummy2);
		// Abspeichern und eigenen DecayWinkel fuellen
		TVector3 pprime_save(pprime[0],pprime[1],pprime[2]);
		TVector3 pprimeother_save(potherprime[0],potherprime[1],potherprime[2]);
		P_Own.SetE(Eprime);
		P_Own.SetVect(pprime_save);
		Pother_Own.SetE(Eotherprime);
		Pother_Own.SetVect(pprimeother_save);

		fHist_OwnBoost->Fill( TMath::Cos(prt->P().Angle(P_Own.Vect())) , GetPreTagEvtWeight());
		if(DecayAngle < -0.8)
		    fHist_InvMass_OwnBoost->Fill((P_Own + Pother_Own).M(),GetPreTagEvtWeight());
		

		
	        // Fill computated Decay Angle for reconstructable mc K0s  
		AtlIDTrack *daughter1 =
		       fEvent->GetAssociatedIDTrack((HepMCParticle*)prt->GetDaughters()->At(0));
		AtlIDTrack *daughter2 =
		    fEvent->GetAssociatedIDTrack((HepMCParticle*)prt->GetDaughters()->At(1));
		if( (daughter1 == 0) || (daughter2 == 0) ) continue;
		if( (TMath::Abs(prt->Eta()) < 2.5) && (prt->Pt() > 0.5) ){
		    if (daughter1->HasValidTruthMatch(fMatchingProb_min) &&
			daughter2->HasValidTruthMatch(fMatchingProb_min) ){
			fHist_MCK0s_DecayAngle->Fill( DecayAngle,GetPreTagEvtWeight());
			Nr_K0sMC++;
		    }
		}
	    }
	    AddAbundanceTrue( Nr_K0sMC );
	}
	ReconstructK0sKinFit();
    } else if ( fMode == kCutBased ) {
	ReconstructK0sCutBased();
    } else {
	Error("AnalyzeEvent",
	      "Invalid mode given. Abort!");
	gSystem->Abort(0);
    }
    return kTRUE;
}

//____________________________________________________________________


void AtlK0sFinder::ReconstructK0sCutBased(){
    //
    // Cut Based K0s reconstruction  
    // 
    
    // Looping over all trk-combinations at a sec Vertex for K0s reconstruction
    AtlIDTrack *trk1 = 0;
    AtlIDTrack *trk2 = 0;                    // compared trks
    HepVertex *Vtx=0;                        // associated DecayVertex 
    HepVertex *PrimVtx=0;                    // Primary Vertex

    Int_t i;                                 // Vertex Loop iterator
    Int_t BgkCount = 1;                      // gives ID of background K0s (from same charged pions) 

    TLorentzVector p_trk1, p_trk2, p_K0s;    // 4 Momentum
    Float_t m_K0s = 0;                       // Reconstructed K0s-mass


    //storing primary Vtx
    PrimVtx = fEvent->GetPrimaryVtx();
    
    // Vertex-Loop
    for ( i = 0; i < fEvent->GetN_Vertices(); i++ ) {
	
	Vtx = (HepVertex*)fEvent->GetVertices()->At(i);

	
	// regarding only sec vtx containing 2 outgoing tracks
	if ( !( (Vtx->IsSecondary()) && (Vtx->GetNDaughters() == 2) )) continue;
	SetCutFlow("Sec Vtx & 2 Dght");

	trk1 = (AtlIDTrack*)Vtx->GetDaughters()->At(0);
	trk2 = (AtlIDTrack*)Vtx->GetDaughters()->At(1);
	
	// Momenta of outgoing pion tracks
	if ( (trk1->Pt() > fSignalPion_Pt_max) ||
	     (trk1->Pt() < fSignalPion_Pt_min) ) continue;
	if ( (trk2->Pt() > fSignalPion_Pt_max) ||
	     (trk2->Pt() < fSignalPion_Pt_min) ) continue;
	SetCutFlow("#pi p_{#perp}");
	// Eta of outgoing pion tracks
	if ( (trk1->Eta() > fSignalPion_Eta_max) ||
	     (trk1->Eta() < fSignalPion_Eta_min) ) continue;
	if ( (trk2->Eta() > fSignalPion_Eta_max) ||
	     (trk2->Eta() < fSignalPion_Eta_min) ) continue;
	SetCutFlow("#pi #eta");

	// Quality of sec. Vertex fit
	if (Vtx->GetChi2overNDoF() > fVertexChi2ovNDoF_max) continue;
	SetCutFlow("Vtx #chi^{2}");
	// Cut on Transverse Decay Length
	if( (PrimVtx->GetPos() - Vtx->GetPos()).Perp() < fTransvDecayLength_min) continue;
	SetCutFlow("Transv. Dcy. Len.");
	
		
       // Quality of Track-Fit
	if (  (trk1->Chi2ovNDoF() > fTrackChi2ovNDoF_max) ||
	      (trk2->Chi2ovNDoF() > fTrackChi2ovNDoF_max)  ) continue;
	SetCutFlow("Trk #chi^{2}");
	
	
	// Discard all tracks without reference to a truth particle for all
	// MC-Runs to compute the purity correctly
	if( fEvent->IsMC() ){
	    if( !( trk1->HasValidTruthMatch(0.0001) &&
		   trk2->HasValidTruthMatch(0.0001)) )  continue;
	}
	SetCutFlow("MC exists");
	
	// Get 4 Momentum and compute inv. mass of K0s candidate
	p_trk1.SetVectM(trk1->P(), fm_pi);
	p_trk2.SetVectM(trk2->P(), fm_pi);
	p_K0s = p_trk1 + p_trk2;
	m_K0s = p_K0s.M();   

	// Cut on cosine of pointing angle
	if( TMath::Abs( TMath::Cos( p_K0s.Angle( (Vtx->GetPos() - PrimVtx->GetPos()) ))) < fPointingAngle_min)
	    continue;
	SetCutFlow("Pointing Angle)");

	// Cut on cos of Decay Angle
	TVector3 boost = -p_K0s.BoostVector();
	TLorentzVector P_trk1_RFK0s = p_trk1;
	TLorentzVector P_trk2_RFK0s = p_trk2;
	P_trk1_RFK0s.Boost(boost); // boosting trks momentum in K0s restframe
	P_trk2_RFK0s.Boost(boost); // boosting trks momentum in K0s restframe
	if ( (TMath::Cos(p_K0s.Angle(P_trk1_RFK0s.Vect())) > fDecayAngle_max)
	     || (TMath::Cos(p_K0s.Angle(P_trk2_RFK0s.Vect())) > fDecayAngle_max) )
	    continue;
	SetCutFlow("Decay Angle)");

	// Cut on Opening Angle
	if ( p_trk1.Angle(p_trk2.Vect()) < fOpeningAngle_min)
	    continue;
	SetCutFlow("Opening Angle");
	
       // Invariant Mass
	// -------------------------
	if( (m_K0s>fK0s_M_min) && (m_K0s<fK0s_M_max) ){     // mass peak:
	    SetCutFlow("inv. mass window)");
	    if ( trk1->GetQovP()*trk2->QovP() < 0.) {       // request opposite sign for signal
		SetCutFlow("opp. sign");
		// Add K0sDecay to fEvents Decay-list
		TLorentzVector P1,P2;
		P1.SetVectM(trk1->P(),fm_pi);
		P2.SetVectM(trk2->P(),fm_pi);

		AtlK0sDecayPiPi *decay = 0;
                // look for right order:
		if(trk1->GetQovP_Vtx()>0.){
		    HepParticle Fit_Daughter1(1,P1.Px(),P1.Py(),P2.Pz(),P1.E(),211);
		    HepParticle Fit_Daughter2(2,P2.Px(),P2.Py(),P2.Pz(),P2.E(),-211);
		    decay = fEvent->AddK0sDecayPiPi(p_K0s.Px(),p_K0s.Py(),p_K0s.Pz(),p_K0s.E(),
					    trk1,trk2,Vtx,
					    Fit_Daughter1,Fit_Daughter2);\
		    fNK0s++;
		} else {
		    HepParticle Fit_Daughter1(1,P2.Px(),P2.Py(),P2.Pz(),P2.E(),211);
		    HepParticle Fit_Daughter2(2,P1.Px(),P1.Py(),P1.Pz(),P1.E(),-211);
		    decay = fEvent->AddK0sDecayPiPi(p_K0s.Px(),p_K0s.Py(),p_K0s.Pz(),p_K0s.E(),
					    trk2,trk1,Vtx,
					    Fit_Daughter1, Fit_Daughter2);
		    fNK0s++;
		}
		
		// Hitbased TruthMatching
		if ( fEvent->IsMC() ){
		    switch ( DoTruthMatch(decay) ){
			case  1:
			    fTrueReco = kTRUE;
			    FillHistograms_MCTruthStudy(decay,'s');
			    break;
			case -1:
			    fTrueReco = kFALSE;
			    FillHistograms_MCTruthStudy(decay,'b');
			    break;
			case  0:
			    continue;
		    }
		}
		
		// DebugOutput
		if ( IsDebugRun() ) {
		    if( trk1->HasValidTruthMatch(0.0001) && trk2->HasValidTruthMatch(0.0001) ){
			fDebugOutput << endl
				     << "Event Nr. :" << fTree->GetReadEntry() << "Vertex ID: "
				     << Vtx->GetId() << endl
				     << "Trk1: " << trk1->GetMCParticle()->GetPdgName() << " Mother: "
				     << trk1->GetMCParticle()->GetMother()->GetPdgName() << endl
				     << "Trk2: " << trk2->GetMCParticle()->GetPdgName() << " Mother: "
				     << trk2->GetMCParticle()->GetMother()->GetPdgName() << endl
				     << endl;
		    }
		    } 
		
	    } else { // Add same charged combinations to the bkg list (like-sign)
		TLorentzVector P1,P2;
		P1.SetVectM(trk1->P(),fm_pi);
		P2.SetVectM(trk2->P(),fm_pi);
		
		Int_t pdgCode = 0;
		if (trk1->QovP()>0.) pdgCode = 211;
		else pdgCode = -211;

		HepParticle Fit_Daughter1(1,P1.Px(),P1.Py(),P2.Pz(),P1.E(),pdgCode);
		HepParticle Fit_Daughter2(2,P2.Px(),P2.Py(),P2.Pz(),P2.E(),pdgCode);
		fEvent->AddK0sDecayPiPi(p_K0s.Px(),p_K0s.Py(),p_K0s.Pz(),p_K0s.E(),
					trk1,trk2,Vtx,
					Fit_Daughter1,Fit_Daughter2);
		fBkgK0sDecays->Add(new AtlK0sDecayPiPi(BgkCount++,p_K0s.Px(), p_K0s.Py(),
						       p_K0s.Pz(), p_K0s.E(),
						       trk1,trk2,
						       Vtx, PrimVtx, Fit_Daughter1,Fit_Daughter2));
	    }
	}
    } // end of vertex loop
}


//____________________________________________________________________

void AtlK0sFinder::ReconstructK0sKinFit(){
    //
    // K0s reconstruction from track-pairs assuming being pions using KinFitter
    //


    
    HepVertex *Vtx = 0;
    HepVertex *PrimVtx = 0;
    AtlIDTrack *trk1 = 0;  
    AtlIDTrack *trk2 = 0;
    
    TLorentzVector p_K0s;
    
    TMatrixD cov_trk1(3,3);
    TMatrixD cov_trk2(3,3);

    Int_t BgkCount = 1;  // gives ID of background K0s (from same charged pions)
    Float_t PvalueKinFit = 0.0;

     //storing primary Vtx
    PrimVtx = fEvent->GetPrimaryVtx();
        
    // Vertex-Loop
    for ( Int_t i = 0; i < fEvent->GetN_Vertices(); i++){
	
	Vtx = (HepVertex*)fEvent->GetVertices()->At(i);
	
	// regarding only sec vtx containing 2 outgoing tracks
	if ( !( (Vtx->IsSecondary()) && (Vtx->GetNDaughters() == 2) )) continue;
	SetCutFlow("Sec. Vtx & 2 Dght.");

	trk1 = (AtlIDTrack*)Vtx->GetDaughters()->At(0);
	trk2 = (AtlIDTrack*)Vtx->GetDaughters()->At(1);
	
	// Momenta of outgoing pion tracks
	if ( (trk1->Pt() > fSignalPion_Pt_max) ||
	     (trk1->Pt() < fSignalPion_Pt_min) ) continue;
	if ( (trk2->Pt() > fSignalPion_Pt_max) ||
	     (trk2->Pt() < fSignalPion_Pt_min) ) continue;
	SetCutFlow("#pi p_{#perp}");
	// Eta of outgoing pion tracks
	if ( (trk1->Eta() > fSignalPion_Eta_max) ||
	     (trk1->Eta() < fSignalPion_Eta_min) ) continue;
	if ( (trk2->Eta() > fSignalPion_Eta_max) ||
	     (trk2->Eta() < fSignalPion_Eta_min) ) continue;
	SetCutFlow("#eta (pions)");

       // Quality of sec. Vertex fit
	if (Vtx->GetChi2overNDoF() > fVertexChi2ovNDoF_max) continue;
	SetCutFlow("Vtx #chi^{2}");
	// Cut on Transverse Decay Length
	if( (PrimVtx->GetPos() - Vtx->GetPos()).Perp() < fTransvDecayLength_min) continue;
	SetCutFlow("Decay Len.");
	
      // Quality of Track-Fit
	if (  (trk1->Chi2ovNDoF() > fTrackChi2ovNDoF_max) ||
	      (trk2->Chi2ovNDoF() > fTrackChi2ovNDoF_max)  ) continue;
	SetCutFlow("Trk #chi^{2}");
	
	// Discard all tracks without reference to a truth particle for all
	// MC-Runs to compute the purity correctly
	if( fEvent->IsMC() ){
	    if( !( trk1->HasValidTruthMatch(0.0001) &&
		   trk2->HasValidTruthMatch(0.0001)) )  continue;
	}
	SetCutFlow("MC exists");
		
	// covariance matrices, taken from track fit
	trk1->GetCovMatrixPtEtaPhi(cov_trk1);
	trk2->GetCovMatrixPtEtaPhi(cov_trk2);
	
	// define momentum used as output of the fit
	TLorentzVector FitP_trk1;
	TLorentzVector FitP_trk2;
	
	// define particels necessary for execution of the fit
	// using pion mass hypothesis; fit variables are pt, eta, phi
	TVector3 dummy1 = trk1->P(); // instead of const cast
	TFitParticlePtEtaPhi FitExec_trk1("FitExec_trk1","FitExec_trk1 for K0sFit",
					  &dummy1,fm_pi,&cov_trk1);
	TVector3 dummy2 = trk2->P(); // instead of const cast
	TFitParticlePtEtaPhi FitExec_trk2("FitExec_trk2","FitExec_trk2 for K0sFit",
					  &dummy2,fm_pi,&cov_trk2);
	
	// definition of K0s mass constraint
	TFitConstraintM MK0sCons("K0sMassConstraint","K0sMassConstraintGaus",0,0,0.497);
	MK0sCons.addParticles1(&FitExec_trk1,&FitExec_trk2);
	
	// fitting characteristics
	TKinFitter fitter;
	fitter.addMeasParticle(&FitExec_trk1);
	fitter.addMeasParticle(&FitExec_trk2);
	fitter.addConstraint(&MK0sCons);
	
	fitter.setMaxNbIter(50);   // maximum number of iterations
	fitter.setMaxDeltaS(5e-5); // maximum deviation of the minimum function within two iterations
	fitter.setMaxF(1e-6);      // maximum value of constraints
	fitter.setVerbosity(0);    // verbosity level

	// Kinematic Fitting
	fitter.fit();
	
	// Fill Histogram IF(!) the fit converged (fitter.getStatus==0)
	if ( fitter.getS()<0. ) {
	    continue;
	}
	if ( !fitter.getStatus() ) {
	    SetCutFlow("Fit converged");
	    // Copy fittet Vectors
	    FitP_trk1=(*FitExec_trk1.getCurr4Vec());
	    FitP_trk2=(*FitExec_trk2.getCurr4Vec());
	    
	    // Reconstruct K0s 4-momentum
	    p_K0s = FitP_trk1 + FitP_trk2;

	    // Cut on cosine of pointing angle
	    if( TMath::Abs( TMath::Cos(p_K0s.Angle( (Vtx->GetPos() - PrimVtx->GetPos() ) ))) < fPointingAngle_min)
		continue;
	    SetCutFlow("Pointing Angle");
	    
	    // Cut on cos of Decay Angle
	    TVector3 boost = -p_K0s.BoostVector();
	    TLorentzVector P_trk1_RFK0s = FitP_trk1;
	    TLorentzVector P_trk2_RFK0s = FitP_trk2;
	    P_trk1_RFK0s.Boost(boost); // boosting trks momentum in K0s restframe
	    P_trk2_RFK0s.Boost(boost); // boosting trks momentum in K0s restframe
	    if ( (TMath::Abs(TMath::Cos(p_K0s.Angle(P_trk1_RFK0s.Vect()))) > fDecayAngle_max)
		 || TMath::Abs((TMath::Cos(p_K0s.Angle(P_trk2_RFK0s.Vect()))) > fDecayAngle_max) )
		continue;
	    SetCutFlow("Decay Angle");


	    // Cut on Opening Angle
	    if ( FitP_trk1.Angle(FitP_trk2.Vect()) < fOpeningAngle_min)
		continue;
	    SetCutFlow("Opening Angle");
	    
	    //setting Chi2,NDoF
	    SetChi2(fitter.getS());
	    SetNDoF(fitter.getNDF());
	    PvalueKinFit = TMath::Prob(fChi2, fNDoF);

	    // Adding K0s to event
	    if (PvalueKinFit >= fKinFitPvalue_min) {
		SetCutFlow("KinFit p-value");

		// Discard all Candidates that are Lambda Candidates as well
		if( IsLambdaCandidate(Vtx) ) continue;
		SetCutFlow("#Lambda Veto");


		//Fill Signal or Background
		AtlKinFitterTool::SetX_TruthHistograms(Vtx->GetPos().Perp());
		
                AtlK0sDecayPiPi *decay;
		if ( trk1->Charge()*trk2->Charge() < 0. ) { //require opposite sign
		    SetCutFlow("opp. sign");

                      // look for right order
		    if ( trk1->QovP()>0. ) {
			HepParticle Fit_Daughter1(1,FitP_trk1.Px(),FitP_trk1.Py(),
						  FitP_trk1.Pz(), FitP_trk1.E(),211);
			HepParticle Fit_Daughter2(2,FitP_trk2.Px(),FitP_trk2.Py(),
						  FitP_trk2.Pz(), FitP_trk2.E(),-211);
			decay = fEvent->AddK0sDecayPiPi(p_K0s.Px(),p_K0s.Py(),p_K0s.Pz(),p_K0s.E(),
							trk1,trk2,Vtx,
							Fit_Daughter1,Fit_Daughter2);
			fNK0s++;
		    } else {
			HepParticle Fit_Daughter1(1,FitP_trk2.Px(),FitP_trk2.Py(),
						  FitP_trk2.Pz(), FitP_trk2.E(),211);
			HepParticle Fit_Daughter2(2,FitP_trk1.Px(),FitP_trk1.Py(),
						  FitP_trk1.Pz(), FitP_trk1.E(),-211);
			decay = fEvent->AddK0sDecayPiPi(p_K0s.Px(),p_K0s.Py(),p_K0s.Pz(),p_K0s.E(),
						trk2,trk1,Vtx,
						Fit_Daughter1, Fit_Daughter2);
			fNK0s++;
		    }
		    decay->SetChi2NDoF(fChi2, fNDoF);

		    // Hitbased TruthMatching
		    if ( fEvent->IsMC() ){
			switch ( DoTruthMatch(decay) ){
			    case  1:
				fTrueReco = kTRUE;
				FillHistograms_MCTruthStudy(decay,'s');
				break;
			    case -1:
				fTrueReco = kFALSE;
				FillHistograms_MCTruthStudy(decay,'b');
				break;
			    case  0:
				continue;
			}
		    }
		    
		    // DebugOutput
		      if ( IsDebugRun() ) {
			if( trk1->HasValidTruthMatch(0.0001) && trk2->HasValidTruthMatch(0.0001) ){
			    //fDebugOutput
			    cout << endl
				 << "Event Nr. :" << fTree->GetReadEntry() << "Vertex ID: "
				 << Vtx->GetId() << endl
				 << "Trk1: " << trk1->GetMCParticle()->GetPdgName() << " Mother: "
				 << trk1->GetMCParticle()->GetMother()->GetPdgName() << endl
				 << "Trk2: " << trk2->GetMCParticle()->GetPdgName() << " Mother: "
				 << trk2->GetMCParticle()->GetMother()->GetPdgName() << endl
				 << endl;
			}
			}
		    // fill fitter histograms depending on Chi2 and NDoF
		    // AtlKinFitterTool::SetX_TruthHistograms( decay->GetDecayAngle() ); 
		    AtlKinFitterTool::FillHistograms("Signal"); 
		} else {
		    
		    // Add same charged combinations to the bkg list
		    Int_t pdgCode = 0;
		    if (trk1->QovP()>0.) pdgCode = 211;
		    else pdgCode = -211;
		    HepParticle Fit_Daughter1(1,FitP_trk1.Px(),FitP_trk1.Py(),
					      FitP_trk1.Pz(), FitP_trk1.E(),pdgCode);
		    HepParticle Fit_Daughter2(2,FitP_trk2.Px(),FitP_trk2.Py(),
					      FitP_trk2.Pz(), FitP_trk2.E(),pdgCode);
		    decay = new AtlK0sDecayPiPi(BgkCount++,p_K0s.Px(), p_K0s.Py(),
						p_K0s.Pz(), p_K0s.E(),
						trk1, trk2,
						Vtx, PrimVtx,
						Fit_Daughter1,Fit_Daughter2);
		    decay->SetChi2NDoF(fChi2, fNDoF);
		    fBkgK0sDecays->Add(decay);
		    // fill Chi2 and NDoFa
		    AtlKinFitterTool::FillHistograms("Bkg");
		}

	    }// End of Prob(Chi2ovNDoF) > x  request
	}
    } //end of vertex loop
}

//____________________________________________________________________    

void AtlK0sFinder::FillHistograms() {
    //
    // Fill histograms
    //

    AtlK0sDecayPiPi *K0s = 0;
    TLorentzVector P_K0s,P_PiPlus,P_PiMinus;
    Float_t OpeningAngle = 0.;
    Float_t PointingAngle = 0.;
    

    // Signal (opposite charged)
    // ________________________________________
    
    for ( Int_t i = 0; i < fEvent->GetN_K0sDecaysPiPi(); i++ ) {

       	K0s = (AtlK0sDecayPiPi*)fEvent->GetK0sDecaysPiPi()->At(i);
	P_K0s     = K0s->P();
	P_PiPlus  = K0s->GetReFitPiPlus().P();
	P_PiMinus = K0s->GetReFitPiMinus().P();

        //filling the standard histograms
	//----------------------------------
	fHistK0sreco_m_PiPi     ->Fill(K0s->M("REC")      ,GetPreTagEvtWeight());
	fHistK0sreco_pt_PiPi    ->Fill(K0s->Pt()          ,GetPreTagEvtWeight());  
	fHistK0sreco_phi_PiPi   ->Fill(K0s->Phi()         ,GetPreTagEvtWeight()); 
	fHistK0sreco_eta_PiPi   ->Fill(K0s->Eta()         ,GetPreTagEvtWeight());
	fHistK0sreco_tau_PiPi   ->Fill(K0s->GetLifeTime() ,GetPreTagEvtWeight());
	fHistK0sreco_DecayLength_PiPi   ->Fill(K0s->GetDecayLength(),  GetPreTagEvtWeight());
	fHistK0sreco_R_vtx->Fill(K0s->GetVtx()->GetPos().Perp(),       GetPreTagEvtWeight());
	
	// FlightDirection
	TVector3 FlightDirection = (K0s->GetVtx())->GetPos();
	FlightDirection -= (fEvent->GetPrimaryVtx())->GetPos();
	PointingAngle = P_K0s.Angle(FlightDirection);
	fHistK0sreco_PointingAngle_PiPi ->Fill(TMath::Abs(TMath::Cos(PointingAngle)),GetPreTagEvtWeight());

        // Opening Angle
	OpeningAngle = P_PiPlus.Angle(P_PiMinus.Vect());
	fHistK0sreco_OpeningAngle_PiPi  ->Fill(OpeningAngle, GetPreTagEvtWeight());

        // Decay Angle is the angle between K0s' momentum and the momentum of the pi+
	// in K0s' restframe
	fHistK0sreco_DecayAngle_PiPi    ->Fill(TMath::Cos(K0s->GetDecayAngle()),  GetPreTagEvtWeight());
	
	
       // filling pion histograms
	// --------------------------
	fHistK0sreco_PiPlus_pt     ->Fill(P_PiPlus.Pt()   ,GetPreTagEvtWeight());
	fHistK0sreco_PiMinus_pt    ->Fill(P_PiMinus.Pt()  ,GetPreTagEvtWeight());
	fHistK0sreco_PiPlus_eta    ->Fill(P_PiPlus.Eta()  ,GetPreTagEvtWeight());
	fHistK0sreco_PiMinus_eta   ->Fill(P_PiMinus.Eta() ,GetPreTagEvtWeight());
	fHistK0sreco_PiPlus_phi    ->Fill(P_PiPlus.Phi()  ,GetPreTagEvtWeight());
	fHistK0sreco_PiMinus_phi   ->Fill(P_PiMinus.Phi() ,GetPreTagEvtWeight());

	fHistK0sreco_Pions_PtEta   ->Fill(P_PiMinus.Pt() ,P_PiMinus.Eta(), GetPreTagEvtWeight());
	fHistK0sreco_Pions_PtEta   ->Fill(P_PiPlus.Pt() , P_PiPlus.Eta(),  GetPreTagEvtWeight());
	
	// general tuning histograms
	fHistVtxChi2ovNDoF->Fill(K0s->GetVtx()->GetChi2overNDoF(),GetPreTagEvtWeight());
	fHistTrkChi2ovNDoF->Fill(K0s->GetPiPlus()->Chi2ovNDoF(),  GetPreTagEvtWeight());
	fHistTrkChi2ovNDoF->Fill(K0s->GetPiMinus()->Chi2ovNDoF(), GetPreTagEvtWeight());


	// filing 2d Histogram (R_Vtx and M_ee), therefore the invariant mass of the
	// 2track-system will be computed while asuming a electron mass hypothesis
	P_PiPlus.SetVectM(K0s->GetReFitPiPlus().P3(),0.000510998910);
	P_PiMinus.SetVectM(K0s->GetReFitPiMinus().P3(),0.000510998910);
	fHistK0sreco_RvtxMee->Fill(K0s->GetVtx()->GetPos().Perp(),(P_PiPlus+P_PiMinus).M(),GetPreTagEvtWeight());
	
    }
    fHistK0sreco_N_PiPi ->Fill(fEvent->GetN_K0sDecaysPiPi(), GetPreTagEvtWeight());
    
    
    // Combinatorial Background (Like-sign approach)
    // ____________________________________________

     TIter next(fBkgK0sDecays);
    while ( (K0s = (AtlK0sDecayPiPi*)next()) ) {
	
	TLorentzVector P_Pi1 = K0s->GetReFitPiPlus().P();
	TLorentzVector P_Pi2 = K0s->GetReFitPiMinus().P();
	
	P_K0s  = K0s->P();
	P_Pi1  = K0s->GetReFitPiPlus().P();
	P_Pi2  = K0s->GetReFitPiMinus().P();

        //filling the standard histograms
	//----------------------------------
	fHistK0sreco_m_LS_bkg     ->Fill(K0s->M("REC")      ,GetPreTagEvtWeight());
	fHistK0sreco_pt_LS_bkg    ->Fill(K0s->Pt()          ,GetPreTagEvtWeight());  
	fHistK0sreco_phi_LS_bkg   ->Fill(K0s->Phi()         ,GetPreTagEvtWeight()); 
	fHistK0sreco_eta_LS_bkg   ->Fill(K0s->Eta()         ,GetPreTagEvtWeight());
	fHistK0sreco_tau_LS_bkg   ->Fill(K0s->GetLifeTime() ,GetPreTagEvtWeight());
	fHistK0sreco_DecayLength_LS_bkg   ->Fill(K0s->GetDecayLength(),  GetPreTagEvtWeight());
	
        //filling pion histograms
	//-----------------------------
        // Get Daughters: both are same charged -> fill either positive or
	// negative daughter histograms
	
	// Check if positive or negative charged daughters   
	if( K0s->GetPiPlus()->IsPositive() ) {    
	    fHistK0sreco_PiPlus_pt_LS_bkg     ->Fill(P_Pi1.Pt(),   GetPreTagEvtWeight());
	    fHistK0sreco_PiPlus_eta_LS_bkg    ->Fill(P_Pi1.Eta(),  GetPreTagEvtWeight());
	    fHistK0sreco_PiPlus_phi_LS_bkg    ->Fill(P_Pi1.Phi(),  GetPreTagEvtWeight());
	} else {
	    fHistK0sreco_PiMinus_pt_LS_bkg    ->Fill(P_Pi2.Pt(),   GetPreTagEvtWeight());
	    fHistK0sreco_PiMinus_eta_LS_bkg   ->Fill(P_Pi2.Eta(),  GetPreTagEvtWeight());
	    fHistK0sreco_PiMinus_phi_LS_bkg   ->Fill(P_Pi2.Phi(),  GetPreTagEvtWeight());
	}
    }
    fHistK0sreco_N_LS_bkg ->Fill(fBkgK0sDecays->GetEntries(), GetPreTagEvtWeight());
}

//____________________________________________________________________

void AtlK0sFinder::SetCutDefaults() {
    //
    // Selection cut defaults and Constraint parameters
    //
    
    // Cuts for Tracks  
    fSignalPion_Pt_min       = 0.5;      // Min. Pt of signal electrons
    fSignalPion_Pt_max       = 1.e10;    // Max. Pt of signal electrons
    fSignalPion_Eta_min      = -2.5;     // Min. eta of signal electrons
    fSignalPion_Eta_max      =  2.5;     // Max. eta of signal electrons
    
    fm_pi                    = 0.13957;  // Pion mass hypothesis
}


//____________________________________________________________________


void AtlK0sFinder::PrintCutValues(){
        
    if ( fMode == kKinFit ){
	cout << endl << " Used mode: kKinFit";
	cout << endl << " ----------------------------------------------" << endl;
	cout << " Used CutDefaults:" <<endl;
	cout << " ----------------------------------------------" << endl;
	cout << scientific << setprecision(3) << " Prob(Chi2/NDoF) > "
	     << fKinFitPvalue_min << endl;
	cout << scientific << setprecision(3) << " Min. of Matching Probability > "
	     << fMatchingProb_min << endl;
    }
    else if (fMode == kCutBased)  {
	cout << endl << " Used mode: kCutBased " << endl;  
	cout << " ----------------------------------------------" << endl;
	cout << " Used CutDefaults:" <<endl;
	cout << " ----------------------------------------------" << endl;
	
	cout << scientific << setprecision(3) << " " << fSignalPion_Pt_min <<
	    " < Pt(Pion)  < " << fSignalPion_Pt_max << endl;
	cout << scientific << fSignalPion_Eta_min << " < Eta(Pion) < "
	     << fSignalPion_Eta_max << endl;
	cout << scientific << " " << fK0s_M_min << " < M(K0s)    < " << fK0s_M_max << endl;
	cout << scientific << " Pion mass hypothesis: " << endl
	     << "           M_Pi = " << fm_pi << endl;
    }
    cout << scientific << " maximum Chi2/NDoF of Vertex Fit: " << endl
	 << "  Chi2/NDoF_Vtx = "
	 << fVertexChi2ovNDoF_max << endl
         << scientific << " maximum Chi2/NDoF of Track Fit: " << endl
	 << "  Chi2/NDoF_Vtx = "
	 << fTrackChi2ovNDoF_max << endl
	 << "  Minimum (abs) cos of Pointing Angle  = "
	 << fPointingAngle_min << endl
	 << "  Minimum of  transverse decay length  = "
	 << fTransvDecayLength_min << endl
	 << "  Maximum of  Decay Angle  = "
	 << fDecayAngle_max << endl
	 << "  Minimum of  Opening Angle  = "
	 << fOpeningAngle_min << endl;
    cout << "----------------------------------------------" << endl;




    
    
}

//____________________________________________________________________

Int_t AtlK0sFinder::DoTruthMatch(AtlK0sDecayPiPi *K0sCandidate) {
    //
    // retuns 1 in case of true K0s, -1 if not
    // returns 0 if no decision possible (see fMatchingProb_min)
    //

    AtlIDTrack *trk1 = K0sCandidate->GetPiPlus();
    AtlIDTrack *trk2 = K0sCandidate->GetPiMinus();
    
    fHistPionMatchingProb->Fill(trk1->GetMCParticleProb(), GetPreTagEvtWeight());
    fHistPionMatchingProb->Fill(trk2->GetMCParticleProb(), GetPreTagEvtWeight());


   
    // remove "_bar" from particle name to collect antiparticle and particle in the same bin
    if( trk1->HasValidTruthMatch(0.0001) ){
	const char* fullPdgName = trk1->GetMCParticle()->GetMother()->GetPdgName();
	char pdgName[strlen(fullPdgName)];
	if (strstr(fullPdgName, "_bar") == NULL) {
	    strcpy(pdgName, fullPdgName);
	} else {
	    strncpy(pdgName, fullPdgName, strlen(fullPdgName)-4);
	    pdgName[strlen(fullPdgName)-4] = '\0';
	}
    
    
    // Hitbased TruthMatching
    if( trk1->HasValidTruthMatch(fMatchingProb_min)
	&& trk2->HasValidTruthMatch(fMatchingProb_min)) {
	if( TMath::Abs(trk1->GetMCParticle()->GetPdgCode())== 211
	    && TMath::Abs(trk2->GetMCParticle()->GetPdgCode())== 211
	    && TMath::Abs(trk1->GetMCParticle()->GetMother()->GetPdgCode())== 310
	    && trk1->GetMCParticle()->GetMother() == trk2->GetMCParticle()->GetMother()){
	    fHistK0sreco_truthtype->Fill(pdgName, K0sCandidate->GetProb(), GetPreTagEvtWeight());
	    return 1;
	}else{
	    if (trk1->GetMCParticle()->GetMother() != trk2->GetMCParticle()->GetMother()) {
		fHistK0sreco_truthtype->Fill("Comb", K0sCandidate->GetProb(), GetPreTagEvtWeight());
	    } else {
		fHistK0sreco_truthtype->Fill(pdgName, K0sCandidate->GetProb(), GetPreTagEvtWeight());
	    }
	    return -1;
	}
    }else{
	return 0;
    }


    }

    return 0;
}

//____________________________________________________________________

Bool_t AtlK0sFinder::IsLambdaCandidate(HepVertex *vtx) {
    //
    // Return true, if the given vertex is already assigned
    // to a lambda decay. Return false otherwise or if
    // there are no  lambda decays (eg. the Lambda finder didn't run).
    //
    if (fEvent->GetN_LambdaDecaysPiPi() < 1)
	return kFALSE;

    TClonesArray *Lambdas = fEvent->GetLambdaDecaysPiPi();
    for (Int_t i = 0; i < fEvent->GetN_LambdaDecaysPiPi(); i++) {
	if (((AtlLambdaDecayPPi*)Lambdas->At(i))->GetVtx() == vtx)
	    return kTRUE;
    }
    return kFALSE;
}


//____________________________________________________________________


void AtlK0sFinder::FillHistograms_MCTruthStudy(AtlK0sDecayPiPi *K0sCandidate, char type) {
    // !!! key 'type' must be "s" for signal or "b" for background !!!
    // !!! routine could only be processed in case of a mc sample  !!!
    //
    // fills several histograms like pointing angle, invariant mass,
    // decay angle, ... separately for bkg and signal (K0s candiates that fulfill
    // the monte-carlo truth-matching as well as those that fail)
    
    TLorentzVector P_K0s,P_PiPlus,P_PiMinus;
    Float_t OpeningAngle = 0.;
    Float_t DecayAngle = 0.;
    Float_t PointingAngle = 0.;
    
    P_K0s     = K0sCandidate->P();
    P_PiPlus  = K0sCandidate->GetReFitPiPlus().P();
    P_PiMinus = K0sCandidate->GetReFitPiMinus().P();

    // Opening Angle
    OpeningAngle = P_PiPlus.Angle(P_PiMinus.Vect());
    // FlightDirection
    TVector3 FlightDirection = (K0sCandidate->GetVtx())->GetPos();
    FlightDirection -= (fEvent->GetPrimaryVtx())->GetPos();
    PointingAngle = P_K0s.Angle(FlightDirection);
    // Decay Angle is the angle between K0s' momentum and the momentum of the pi+
    // in K0s' restframe
    DecayAngle = TMath::Cos(K0sCandidate->GetDecayAngle());

    // filing 2d Histogram (R_Vtx and M_ee), therefore the invariant mass of the
    // 2track-system will be computed while asuming a electron mass hypothesis
    // !!! Originaly momenta of Pi+ and pi- candidates are now overwritten !!!
    P_PiPlus.SetVectM(K0sCandidate->GetReFitPiPlus().P3(),0.000510998910);
    P_PiMinus.SetVectM(K0sCandidate->GetReFitPiMinus().P3(),0.000510998910);
    
    // filling histograms // decay angle, N,  
    switch (type) {
	case 's':
	    fHistK0sreco_m_MCsignal     ->Fill(K0sCandidate->M("REC")  ,GetPreTagEvtWeight());
	    fHistK0sreco_pt_MCsignal    ->Fill(K0sCandidate->Pt()          ,GetPreTagEvtWeight());  
	    fHistK0sreco_phi_MCsignal   ->Fill(K0sCandidate->Phi()         ,GetPreTagEvtWeight()); 
	    fHistK0sreco_eta_MCsignal   ->Fill(K0sCandidate->Eta()         ,GetPreTagEvtWeight());
	    fHistK0sreco_tau_MCsignal   ->Fill(K0sCandidate->GetLifeTime() ,GetPreTagEvtWeight());
	    fHistK0sreco_DecayLength_MCsignal   ->Fill(K0sCandidate->GetTransvDecayLength(),  GetPreTagEvtWeight());
	    fHistK0sreco_PointingAngle_MCsignal ->Fill(TMath::Abs(TMath::Cos(PointingAngle)),GetPreTagEvtWeight());
	    fHistK0sreco_OpeningAngle_MCsignal  ->Fill(OpeningAngle, GetPreTagEvtWeight());
	    fHistK0sreco_DecayAngle_MCsignal    ->Fill(DecayAngle, GetPreTagEvtWeight());
	    fHistK0sreco_N_MCsignal             ->Fill(1, GetPreTagEvtWeight());
	    fHistK0sreco_R_vtx_MCsignal ->Fill(K0sCandidate->GetVtx()->GetPos().Perp(), GetPreTagEvtWeight());
	    fHistK0sreco_RvtxMee_MCsignal->Fill(K0sCandidate->GetVtx()->GetPos().Perp(),
						(P_PiPlus+P_PiMinus).M(), GetPreTagEvtWeight());
	    fHistK0sreco_Chi2_MCsignal   ->Fill(K0sCandidate->GetChi2overNDoF(), GetPreTagEvtWeight());
	    fHistK0sreco_pValue_MCsignal ->Fill(K0sCandidate->GetProb(),         GetPreTagEvtWeight());
	    fHistK0sreco_Chi2Trk_MCsignal   ->Fill(K0sCandidate->GetPiPlus()->Chi2ovNDoF(), GetPreTagEvtWeight());
	    fHistK0sreco_Chi2Trk_MCsignal   ->Fill(K0sCandidate->GetPiMinus()->Chi2ovNDoF(), GetPreTagEvtWeight());
	    fHistK0sreco_Chi2Vtx_MCsignal   ->Fill(K0sCandidate->GetVtx()->GetChi2overNDoF(), GetPreTagEvtWeight());
	    break;
	case 'b':
	    fHistK0sreco_m_MCbkg     ->Fill(K0sCandidate->M("REC")  ,GetPreTagEvtWeight());
	    fHistK0sreco_pt_MCbkg    ->Fill(K0sCandidate->Pt()          ,GetPreTagEvtWeight());  
	    fHistK0sreco_phi_MCbkg   ->Fill(K0sCandidate->Phi()         ,GetPreTagEvtWeight()); 
	    fHistK0sreco_eta_MCbkg   ->Fill(K0sCandidate->Eta()         ,GetPreTagEvtWeight());
	    fHistK0sreco_tau_MCbkg   ->Fill(K0sCandidate->GetLifeTime() ,GetPreTagEvtWeight());
	    fHistK0sreco_DecayLength_MCbkg   ->Fill(K0sCandidate->GetTransvDecayLength(),  GetPreTagEvtWeight());
	    fHistK0sreco_PointingAngle_MCbkg ->Fill(TMath::Abs(TMath::Cos(PointingAngle)),GetPreTagEvtWeight());
	    fHistK0sreco_OpeningAngle_MCbkg  ->Fill(OpeningAngle, GetPreTagEvtWeight());
	    fHistK0sreco_DecayAngle_MCbkg    ->Fill(DecayAngle, GetPreTagEvtWeight());
	    fHistK0sreco_N_MCbkg             ->Fill(1, GetPreTagEvtWeight());
	    fHistK0sreco_R_vtx_MCbkg ->Fill(K0sCandidate->GetVtx()->GetPos().Perp(), GetPreTagEvtWeight());
	    fHistK0sreco_Chi2_MCbkg   ->Fill(K0sCandidate->GetChi2overNDoF(), GetPreTagEvtWeight());
	    fHistK0sreco_pValue_MCbkg ->Fill(K0sCandidate->GetProb(),         GetPreTagEvtWeight());
	    fHistK0sreco_Chi2Trk_MCbkg   ->Fill(K0sCandidate->GetPiPlus()->Chi2ovNDoF(), GetPreTagEvtWeight());
	    fHistK0sreco_Chi2Trk_MCbkg   ->Fill(K0sCandidate->GetPiMinus()->Chi2ovNDoF(), GetPreTagEvtWeight());
	    fHistK0sreco_Chi2Vtx_MCbkg   ->Fill(K0sCandidate->GetVtx()->GetChi2overNDoF(), GetPreTagEvtWeight());
	    // Analyzing tBkg Type
	    // Photon Conv
	    if( (K0sCandidate->GetPiMinus()->GetMCParticle()->GetMother()
		 == K0sCandidate->GetPiPlus()->GetMCParticle()->GetMother())
		&& (K0sCandidate->GetPiPlus()->GetMCParticle()->GetMother()->GetPdgCode() == 22))
	    {
		fHistK0sreco_R_vtx_MCbkg_gamma->Fill(K0sCandidate->GetVtx()->GetPos().Perp(), GetPreTagEvtWeight());
		fHistK0sreco_RvtxMee_MCbkg->Fill(K0sCandidate->GetVtx()->GetPos().Perp(),
						 (P_PiPlus+P_PiMinus).M(), GetPreTagEvtWeight());
		fHist_DA_Gamma->Fill(DecayAngle, GetPreTagEvtWeight());
	    }
	    // Lambda
	    if( (K0sCandidate->GetPiMinus()->GetMCParticle()->GetMother()
		 == K0sCandidate->GetPiPlus()->GetMCParticle()->GetMother())
		&& TMath::Abs( (K0sCandidate->GetPiPlus()->GetMCParticle()->GetMother()->GetPdgCode()) ) == 3122)
	    {
		fHist_DA_Lambda->Fill(DecayAngle, GetPreTagEvtWeight());
	    }
	    // Comb Bkg
	    if( (K0sCandidate->GetPiMinus()->GetMCParticle()->GetMother()
		 != K0sCandidate->GetPiPlus()->GetMCParticle()->GetMother()) )
	    {
		fHist_DA_Comb->Fill(DecayAngle, GetPreTagEvtWeight());
	    }
		
	    // Decay Angle Bkg Analysis
	    if( TMath::Abs(DecayAngle) < 0.94){
		if( TMath::Abs(DecayAngle) < 0.6){
		    if ( K0sCandidate->GetPiPlus()->GetMCParticle()->GetMother() ==
			 K0sCandidate->GetPiMinus()->GetMCParticle()->GetMother() )
		    {
			if( DecayAngle > 0 ) fHist_DecayAngle_0to06->Fill(K0sCandidate->GetPiPlus()->GetMCParticle()
									  ->GetMother()->GetPdgName(),GetPreTagEvtWeight());
			else                 fHist_DecayAngleNeg_0to06->Fill(K0sCandidate->GetPiPlus()->GetMCParticle()
									     ->GetMother()->GetPdgName(),GetPreTagEvtWeight());
		    }else{
			if( DecayAngle > 0 ) fHist_DecayAngle_0to06->Fill("Comb", GetPreTagEvtWeight());
			else                 fHist_DecayAngleNeg_0to06->Fill("Comb", GetPreTagEvtWeight());
		    }
		}else{ // 0.6 < DecayAngle < 0.94
		    if ( K0sCandidate->GetPiPlus()->GetMCParticle()->GetMother() ==
			 K0sCandidate->GetPiMinus()->GetMCParticle()->GetMother() )
		    {
			if( DecayAngle > 0 )  {
			    fHist_DecayAngle_06to094->Fill(K0sCandidate->GetPiPlus()->GetMCParticle()
									     ->GetMother()->GetPdgName(),GetPreTagEvtWeight());
			    fHist_PionsRight_Pt->Fill(K0sCandidate->GetPiPlus()->Pt(),GetPreTagEvtWeight());
			    
                       // Control Output: Bkg should be from Lambda-Decays, that means Protons that make the fake
			    // Peak!
			    AtlIDTrack *trkpos = K0sCandidate->GetPiPlus();
			    			    
			    if (trkpos->HasValidTruthMatch(0.9) ) {
				HepMCParticle *mcpos = trkpos->GetMCParticle();
				if( strcmp( mcpos->GetPdgName(), "pi+" ) == 0 ) {
				    if( strcmp( mcpos->GetMother()->GetPdgName(), "Lambda0_bar" ) != 0 ) {
					//cout << endl << " pi+ von Nicht Lambda_bar !!!! -> "
					//   << mcpos->GetMother()->GetPdgName()<< endl;
				    }else{
					NLambdaBarpos++;
					NPiPluspos++;
				    }
				}
				if( strcmp( mcpos->GetPdgName(), "proton" ) == 0 ) {
				    if( strcmp( mcpos->GetMother()->GetPdgName(), "Lambda0" ) != 0 ) {
					//cout << endl << " proton nicht von Nicht Lambda !!!!  -> "
					//   << mcpos->GetMother()->GetPdgName() << endl;
				    }else{
					NLambdapos++;
					NProtonpos++;
				    }
				}
			    }
			}
			else{
			    fHist_DecayAngleNeg_06to094->Fill(K0sCandidate->GetPiPlus()->GetMCParticle()
							      ->GetMother()->GetPdgName(),GetPreTagEvtWeight());
			    fHist_PionsLeft_Pt->Fill(K0sCandidate->GetPiPlus()->Pt(),GetPreTagEvtWeight());


                            // Control Output: Bkg should be from LambdaBar-Decays, that means piPlus that make the fake
			    // Peak!
			    AtlIDTrack *trkpos = K0sCandidate->GetPiPlus();

			    if (trkpos->HasValidTruthMatch(0.9) ) {
				HepMCParticle *mcpos = trkpos->GetMCParticle();
				if( strcmp( mcpos->GetPdgName(), "pi+" ) == 0 ) {
				    if( strcmp( mcpos->GetMother()->GetPdgName(), "Lambda0_bar" ) != 0 ) {
					//cout << endl << " pi+ von Nicht Lambda_bar !!!!  -> "
					//   << mcpos->GetMother()->GetPdgName() << endl;
				    }else{
					NLambdaBarneg++;
					NPiPlusneg++;
				    }
				}
				if( strcmp( mcpos->GetPdgName(), "proton" ) == 0 ) {
				    if( strcmp( mcpos->GetMother()->GetPdgName(), "Lambda0" ) != 0 ) {
					//cout << endl << " proton nicht von Nicht Lambda !!!!  -> "
					//   << mcpos->GetMother()->GetPdgName() << endl;
				    }else{
					NLambdaneg++;
					NProtonneg++;
				    }
				}
			    }
			    
			}
		    }else{
			if (DecayAngle > 0) fHist_DecayAngle_06to094->Fill("Comb", GetPreTagEvtWeight());
			else                fHist_DecayAngleNeg_06to094->Fill("Comb", GetPreTagEvtWeight());
		    }
		}
	    }else{ // DecayAngle > 0.94
		if ( K0sCandidate->GetPiPlus()->GetMCParticle()->GetMother() ==
		     K0sCandidate->GetPiMinus()->GetMCParticle()->GetMother() )
		{
		    if (DecayAngle > 0)  fHist_DecayAngle_094to1->Fill(K0sCandidate->GetPiPlus()->GetMCParticle()
								       ->GetMother()->GetPdgName(),GetPreTagEvtWeight());
		    else                 fHist_DecayAngleNeg_094to1->Fill(K0sCandidate->GetPiPlus()->GetMCParticle()
									  ->GetMother()->GetPdgName(),GetPreTagEvtWeight());
		}else{
		    if (DecayAngle > 0)  fHist_DecayAngle_094to1->Fill("Comb", GetPreTagEvtWeight());
		    else                 fHist_DecayAngleNeg_094to1->Fill("Comb", GetPreTagEvtWeight());
		} 
		} 
	    
	    break;
	default:
	    cout << endl << endl
		 <<"!!! Error using FillHistograms_MCTruthStudy(AtlK0sDecayPiPi*,char) in AtlK0sFinder"
		 << endl << "Select either 's' or 'b' as key !!!"
		 << endl << endl;
	    break;
    }
}

//____________________________________________________________________

void AtlK0sFinder::Terminate() {

    AtlKinFitterTool::Terminate();
    // print truth matching statistics
    fHistK0sreco_truthtype->LabelsDeflate("X");
    fHistK0sreco_truthtype->SetStats(kFALSE);
    fHistK0sreco_truthtype->GetXaxis()->SetLabelSize(0.06);
    fHistK0sreco_truthtype->GetXaxis()->LabelsOption("v");

    // Decay Angle Analysis
    fHist_DecayAngle_0to06->LabelsDeflate();
    fHist_DecayAngle_06to094->LabelsDeflate();
    fHist_DecayAngle_094to1->LabelsDeflate();
    fHist_DecayAngleNeg_0to06->LabelsDeflate();
    fHist_DecayAngleNeg_06to094->LabelsDeflate();
    fHist_DecayAngleNeg_094to1->LabelsDeflate();
    
    cout << endl << endl << "NO. OF RECONSTRUCTED K0s':  " << fNK0s << endl;


    cout << endl << "NLambdapos  " << NLambdapos
	 << endl << "NLambdaBarpos  " << NLambdaBarpos
	 << endl << "NPiPluspos  " << NPiPluspos
	 << endl << "NProtonpos  " << NProtonpos
	 << endl
	 << endl << "NLambdaneg  " << NLambdaneg
	 << endl << "NLambdaBarneg  " << NLambdaBarneg
	 << endl << "NPiPlusneg  " << NPiPlusneg
	 << endl << "NProtonneg  " << NProtonneg;
	
}



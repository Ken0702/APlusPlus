//____________________________________________________________________
//
// Photoconversion finder class
//
// Conversions will be reconstructed while looping over all secondary vertices
// with 2 oppositly charged daughter tracks. 
// The Decay decision will be done either by a cut bassed reconstruction
// or by using the kinemativ fitter. Switching is possible by a Flag
// EMode (kCutBased, kKinFit) that can be set in the constructor via
// SetMode(EMode)

//
// Author: Sebastian Beumler <mailto:beumler@physik.hu-berlin.de>
// Copyright: 2010 (C) Sebastian Beumler
//
#ifndef ATLAS_AtlConvFinder
#include <AtlConvFinder.h>
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
ClassImp(AtlConvFinder);
#endif

//____________________________________________________________________

AtlConvFinder::AtlConvFinder(const char* name, const char* title) :
    AtlKinFitterTool(name, title) {
    //
    // Default constructor
    //

    // set default mode
    SetMode(kKinFit);
    SetDebugMode(kFALSE);

    fBkgPhotonConv = new TList;
    fN_RecConv     = 0;
    fN_TrueRecConv = 0;
    fN_NoDecision  = 0;
}

//____________________________________________________________________

AtlConvFinder::~AtlConvFinder() {
    //
    // Default destructor
    //
    fBkgPhotonConv->Delete();
    delete fBkgPhotonConv;

    if ( IsDebugRun() )    fDebugOutput.close();
    
}

//____________________________________________________________________

void AtlConvFinder::SetBranchStatus() {
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

void AtlConvFinder::BookHistograms() {
    //
    // Book histograms
    //
    
    //Debugging
    if (IsDebugRun() ){
	fDebugOutput.open("DebugOutput.dat");
    }
    
    // Book KinFitter Histograms
    AtlKinFitterTool::BookHistograms();
    // Show adapted Cuts
    PrintCutValues();

    
    // ======================
    //       SIGNAL
    // ======================
    gDirectory->mkdir("Signal", "Oppositely charged candidates");
    gDirectory->cd("Signal");


    fHisteMatchingProb = new TH1F("h_e_MatchingProb",
				     "Matching probability (hitbased TruthMatching) of particles matching electrons",
				     50, 0, 1.2);
    fHisteMatchingProb->SetXTitle("MatchingProb");
    fHisteMatchingProb->SetYTitle("Number of Entries");

    
       
    // Conversions
    // ---

    if ( fMode == kKinFit ) {
	fHistConvreco_m_ee   = new TH1F("h_Conv_m_ee", "Invariant Conv mass (Conv->ee)",
					 50, 0.0, 0.01);
    }else if (fMode == kCutBased){
	fHistConvreco_m_ee   = new TH1F("h_Conv_m_ee", "Invariant Conv mass (Conv->ee)",
					 50, 0.0, 0.01);
    }
    fHistConvreco_m_ee->SetXTitle("m_{Conv} (GeV)");
    fHistConvreco_m_ee->SetYTitle("Number of Entries");
    fHistConvreco_pt_ee  = new TH1F("h_Conv_pt_ee", "Conv-p_{#perp} (Conv->ee)",
				     400, 0, 400);
    fHistConvreco_pt_ee->SetXTitle("p_{#perp} (GeV)");
    fHistConvreco_pt_ee->SetYTitle("Number of Entries");
    fHistConvreco_phi_ee = new TH1F("h_Conv_phi_ee", "Conv #phi distribution (Conv->ee)",
				     128, -3.2, 3.2);
    fHistConvreco_phi_ee->SetXTitle("#phi (rad)");
    fHistConvreco_phi_ee->SetYTitle("Number of Entries");
    fHistConvreco_eta_ee = new TH1F("h_Conv_eta_ee", "Conv #eta distribution (Conv->ee)",
				 50, -3, 3);
    fHistConvreco_eta_ee->SetXTitle("#eta");
    fHistConvreco_eta_ee->SetYTitle("Number of Entries");
    fHistConvreco_N_ee   = new TH1F("h_Conv_N_ee", "Reconstructed Conv' per event (Conv->ee)",
				     11, -0.5, 10.5);
    fHistConvreco_N_ee->SetXTitle("N_{Conv}/evt");
    fHistConvreco_N_ee->SetYTitle("Number of Entries");
    fHistConvreco_tau_ee = new TH1F("h_Conv_tau_ee", "Reconstructed Conv' lifetime (Conv->PPi)", 
				       100, 0, 0.2);
    fHistConvreco_tau_ee->SetXTitle("#tau (ps)");
    fHistConvreco_tau_ee->SetYTitle("Number of Entries");
    fHistConvreco_DecayLength_ee = new TH1F("h_Conv_DecayLength_ee",
					     "Reconstructed Conv' decay length (Conv->ee)", 
				       100, 0, 300);
    fHistConvreco_DecayLength_ee->SetXTitle("#DecayLength ");
    fHistConvreco_DecayLength_ee->SetYTitle("Number of Entries");
    fHistConvreco_OpeningAngle_ee = new TH1F("h_Conv_OpeningAngle_ee",
					      "opening angle between pion tracks",
				     128, 0, 0.01);
    fHistConvreco_OpeningAngle_ee->SetXTitle("#alpha (rad)");
    fHistConvreco_OpeningAngle_ee->SetYTitle("Number of Entries");
    fHistConvreco_DecayAngle_ee = new TH1F("h_Conv_DecayAngle_ee",
					    "decay angle between Conv and e+ in Conv' restframe (reconstructed Conv)",
				     50, -1, 1);
    fHistConvreco_DecayAngle_ee->SetXTitle("cos(#beta)");
    fHistConvreco_DecayAngle_ee->SetYTitle("Number of Entries");
    fHistConvreco_PointingAngle_ee = new TH1F("h_Conv_PointingAngle_ee",
					       "Cos of angle between Conv' momentum and vec(PV)-vec(SecVtx)",
					       50, 0.995, 1);
    fHistConvreco_PointingAngle_ee->SetXTitle("cos(#delta)");
    fHistConvreco_PointingAngle_ee->SetYTitle("Number of Entries");
    
    fHistConvreco_R_vtx = new TH1F("h_Conv_R_vtx", "Radial distance of secondary vertices", 120, 0.0, 60.0);
    fHistConvreco_R_vtx->SetXTitle("r_{#perp} (cm)");
    fHistConvreco_R_vtx->SetYTitle("Number of Entries");
    
    // Electron (e+) histograms in sub-folder
    gDirectory->mkdir("eplus", "e+ from Conv -> ee");
    gDirectory->cd("eplus");
    
    fHistConvreco_ePlus_pt = new TH1F("h_Conv_ePlus_pt", "#e^{+}-p_{#perp} (Conv->#e#e)",
				      400, 0, 400);
    fHistConvreco_ePlus_pt->SetXTitle("p_{#perp} (GeV)");
    fHistConvreco_ePlus_pt->SetYTitle("Number of Entries");
    fHistConvreco_ePlus_eta = new TH1F("h_Conv_ePlus_eta", "#e^{+}:  #eta distribution (Conv->#e#e)",
				       50, -3, 3);
    fHistConvreco_ePlus_eta->SetXTitle("#eta");
    fHistConvreco_ePlus_eta->SetYTitle("Number of Entries");
    fHistConvreco_ePlus_phi = new TH1F("h_Conv_ePlus_phi", "#e^{+}:  #phi distribution (Conv->#e#pi)",
				    50, -3.14, 3.14);
    fHistConvreco_ePlus_phi->SetXTitle("#phi");
    fHistConvreco_ePlus_phi->SetYTitle("Number of Entries");
    
    gDirectory->cd("../"); 
    
    // Electron (e-) histograms in sub-folder
    gDirectory->mkdir("eMinus", "e- from Conv0 -> ee");
    gDirectory->cd("eMinus");
    
    fHistConvreco_eMinus_pt = new TH1F("h_Conv_eMinus_pt", "#e^{-}-p_{#perp} (Conv->#e#e)",
				       400, 0, 400);
    fHistConvreco_eMinus_pt->SetXTitle("p_{#perp} (GeV)");
    fHistConvreco_eMinus_pt->SetYTitle("Number of Entries");
    fHistConvreco_eMinus_eta = new TH1F("h_Conv_eMinus_eta", "#e^{-}:  #eta distribution (Conv->#e#e)",
					50, -5, 5);
    fHistConvreco_eMinus_eta->SetXTitle("#eta");
    fHistConvreco_eMinus_eta->SetYTitle("Number of Entries");
    fHistConvreco_eMinus_phi = new TH1F("h_Conv_eMinus_phi", "#e^{-}:  #phi distribution (Conv->#e#e)",
					50, -3.14, 3.14);
    fHistConvreco_eMinus_phi->SetXTitle("#phi");
    fHistConvreco_eMinus_phi->SetYTitle("Number of Entries");


    gDirectory->cd("../../");

    // ======================
    //       BACKGROUND
    // ======================
    gDirectory->mkdir("Bkg", "Equally charged candidates");
    gDirectory->cd("Bkg");

       
    // Conversions
    // ---

    if ( fMode == kKinFit ) {
	fHistConvreco_m_LS_bkg   = new TH1F("h_Conv_m_LS_bkg", "Invariant Conv mass (Conv->ee)",
					 50, 0.0, 0.01);
    }else if (fMode == kCutBased){
	fHistConvreco_m_LS_bkg   = new TH1F("h_Conv_m_LS_bkg", "Invariant Conv mass (Conv->ee)",
					 50, 0.0, 0.01);
    }
    fHistConvreco_m_LS_bkg->SetXTitle("m_{Conv} (GeV)");
    fHistConvreco_m_LS_bkg->SetYTitle("Number of Entries");
    fHistConvreco_pt_LS_bkg  = new TH1F("h_Conv_pt_LS_bkg", "Conv-p_{#perp} (Conv->ee)",
				     400, 0, 400);
    fHistConvreco_pt_LS_bkg->SetXTitle("p_{#perp} (GeV)");
    fHistConvreco_pt_LS_bkg->SetYTitle("Number of Entries");
    fHistConvreco_phi_LS_bkg = new TH1F("h_Conv_phi_LS_bkg", "Conv #phi distribution (Conv->ee)",
				     128, -3.2, 3.2);
    fHistConvreco_phi_LS_bkg->SetXTitle("#phi (rad)");
    fHistConvreco_phi_LS_bkg->SetYTitle("Number of Entries");
    fHistConvreco_eta_LS_bkg = new TH1F("h_Conv_eta_LS_bkg", "Conv #eta distribution (Conv->ee)",
				 50, -3, 3);
    fHistConvreco_eta_LS_bkg->SetXTitle("#eta");
    fHistConvreco_eta_LS_bkg->SetYTitle("Number of Entries");
    fHistConvreco_N_LS_bkg   = new TH1F("h_Conv_N_LS_bkg", "Reconstructed Conv' per event (Conv->ee)",
				     11, -0.5, 10.5);
    fHistConvreco_N_LS_bkg->SetXTitle("N_{Conv}/evt");
    fHistConvreco_N_LS_bkg->SetYTitle("Number of Entries");
    fHistConvreco_tau_LS_bkg = new TH1F("h_Conv_tau_LS_bkg", "Reconstructed Conv' lifetime (Conv->PPi)", 
				       100, 0, 0.2);
    fHistConvreco_tau_LS_bkg->SetXTitle("#tau (ps)");
    fHistConvreco_tau_LS_bkg->SetYTitle("Number of Entries");
    fHistConvreco_DecayLength_LS_bkg = new TH1F("h_Conv_DecayLength_LS_bkg",
					     "Reconstructed Conv' decay length (Conv->ee)", 
				       100, 0, 300);
    fHistConvreco_DecayLength_LS_bkg->SetXTitle("#DecayLength ");
    fHistConvreco_DecayLength_LS_bkg->SetYTitle("Number of Entries");
    
    // Electron (e+) histograms in sub-folder
    gDirectory->mkdir("eplus", "e+ from Conv -> e+e+");
    gDirectory->cd("eplus");
    
    fHistConvreco_ePlus_pt_LS_bkg = new TH1F("h_Conv_ePlus_pt_LS_bkg", "#e^{+}-p_{#perp} (Conv->#e#e)",
				      400, 0, 400);
    fHistConvreco_ePlus_pt_LS_bkg->SetXTitle("p_{#perp} (GeV)");
    fHistConvreco_ePlus_pt_LS_bkg->SetYTitle("Number of Entries");
    fHistConvreco_ePlus_eta_LS_bkg = new TH1F("h_Conv_ePlus_eta_LS_bkg", "#e^{+}:  #eta distribution (Conv->#e#e)",
				       50, -3, 3);
    fHistConvreco_ePlus_eta_LS_bkg->SetXTitle("#eta");
    fHistConvreco_ePlus_eta_LS_bkg->SetYTitle("Number of Entries");
    fHistConvreco_ePlus_phi_LS_bkg = new TH1F("h_Conv_ePlus_phi_LS_bkg", "#e^{+}:  #phi distribution (Conv->#e#pi)",
				    50, -3.14, 3.14);
    fHistConvreco_ePlus_phi_LS_bkg->SetXTitle("#phi");
    fHistConvreco_ePlus_phi_LS_bkg->SetYTitle("Number of Entries");
    
    gDirectory->cd("../"); 
    
    // Electron (e-) histograms in sub-folder
    gDirectory->mkdir("eMinus", "e- from Conv0 -> e-e-");
    gDirectory->cd("eMinus");
    
    fHistConvreco_eMinus_pt_LS_bkg = new TH1F("h_Conv_eMinus_pt_LS_bkg", "#e^{-}-p_{#perp} (Conv->#e#e)",
				       400, 0, 400);
    fHistConvreco_eMinus_pt_LS_bkg->SetXTitle("p_{#perp} (GeV)");
    fHistConvreco_eMinus_pt_LS_bkg->SetYTitle("Number of Entries");
    fHistConvreco_eMinus_eta_LS_bkg  = new TH1F("h_Conv_eMinus_eta_LS_bkg", "#e^{-}:  #eta distribution (Conv->#e#e)",
					50, -5, 5);
    fHistConvreco_eMinus_eta_LS_bkg ->SetXTitle("#eta");
    fHistConvreco_eMinus_eta_LS_bkg->SetYTitle("Number of Entries");
    fHistConvreco_eMinus_phi_LS_bkg = new TH1F("h_Conv_eMinus_phi_LS_bkg", "#e^{-}:  #phi distribution (Conv->#e#e)",
					50, -3.14, 3.14);
    fHistConvreco_eMinus_phi_LS_bkg->SetXTitle("#phi");
    fHistConvreco_eMinus_phi_LS_bkg->SetYTitle("Number of Entries");


    gDirectory->cd("../../../");

    
    // ========================
    //     MC TRUTH STUDY
    // ========================

    gDirectory->mkdir("MCTruthStudy", "Small Truth Study with hitbased Truth-matching");
    gDirectory->cd("MCTruthStudy");

    fHistConvreco_truthtype = new TH2F("h_Convreco_truthtype", "truth type of Conv candidates",
				       6260, -3130, 3130, 50, 0.0, 1.0);
    fHistConvreco_truthtype->SetXTitle("");
    fHistConvreco_truthtype->SetYTitle("P(#Chi^{2})");
    fHistConvreco_truthtype->SetZTitle("Number of Entries");
    fHistConvreco_truthtype->SetOption("COLZ");
    
    // =============================================================================
    // Book signal histograms for Conv-candidates that fulfill the MC Truth Matching
    // =============================================================================
    
    gDirectory->mkdir("Signal_MC", "Conv-candidates that fulfill the MC Truth Matching");
    gDirectory->cd("Signal_MC");

    if ( fMode == kKinFit ) {
	fHistConvreco_m_MCsignal   = new TH1F("h_Conv_m_MCsignal",
					      "Invariant Conv mass (equaly signed electrons)",
					      50, 0.0, 0.01);
    }else if (fMode == kCutBased){
	fHistConvreco_m_MCsignal   = new TH1F("h_Conv_m_PiPi",
					      "Invariant Conv mass (equaly signed electrons)",
					      50, 0.0, 0.01);
    }
    fHistConvreco_m_MCsignal->SetXTitle("m_{Conv} (GeV)");
    fHistConvreco_m_MCsignal->SetYTitle("Number of Entries");
    fHistConvreco_pt_MCsignal  = new TH1F("h_Conv_pt_MCsignal", "Conv-p_{#perp} (equaly signed electrons)",
				    400, 0, 400);
    fHistConvreco_pt_MCsignal->SetXTitle("p_{#perp} (GeV)");
    fHistConvreco_pt_MCsignal->SetYTitle("Number of Entries");
    fHistConvreco_phi_MCsignal = new TH1F("h_Conv_phi_MCsignal", "Conv #phi distribution (equaly signed electrons)",
				    128, -3.2, 3.2);
    fHistConvreco_phi_MCsignal->SetXTitle("#phi (rad)");
    fHistConvreco_phi_MCsignal->SetYTitle("Number of Entries");
    fHistConvreco_eta_MCsignal = new TH1F("h_Conv_eta_MCsignal", "Conv #eta distribution (equaly signed electrons)",
				    50, -3, 3);
    fHistConvreco_eta_MCsignal->SetXTitle("#eta");
    fHistConvreco_eta_MCsignal->SetYTitle("Number of Entries");
    fHistConvreco_N_MCsignal   = new TH1F("h_Conv_N_MCsignal", "Reconstructed Conv' per event (equaly signed electrons)",
				    11,-0.5,10.5);
    fHistConvreco_N_MCsignal->SetXTitle("N_{Conv}/evt");
    fHistConvreco_N_MCsignal->SetYTitle("Number of Entries");
    fHistConvreco_tau_MCsignal = new TH1F("h_Conv_tau_MCsignal", "Reconstructed Conv' lifetime (equaly signed electrons)", 
				    100, 0, 0.2);
    fHistConvreco_tau_MCsignal->SetXTitle("#tau (ps)");
    fHistConvreco_tau_MCsignal->SetYTitle("Number of Entries");
    fHistConvreco_DecayLength_MCsignal = new TH1F("h_Conv_DecayLength_MCsignal",
					    "Reconstructed Conv' decay length (equaly signed electrons)", 
					    100, 0, 50);
    fHistConvreco_DecayLength_MCsignal->SetXTitle("DecayLength ");
    fHistConvreco_DecayLength_MCsignal->SetYTitle("Number of Entries");
    fHistConvreco_OpeningAngle_MCsignal = new TH1F("h_Conv_OpeningAngle_MCsignal",
					     "opening angle between electron tracks",
					     128, 0, 0.01);
    fHistConvreco_OpeningAngle_MCsignal->SetXTitle("#alpha (rad)");
    fHistConvreco_OpeningAngle_MCsignal->SetYTitle("Number of Entries");
    fHistConvreco_DecayAngle_MCsignal = new TH1F("h_Conv_DecayAngle_MCsignal",
					   "decay angle for reconstructed Conv that fulfill truthmatching",
					   50, -1, 1);
    fHistConvreco_DecayAngle_MCsignal->SetXTitle("cos(#beta) (rad)");
    fHistConvreco_DecayAngle_MCsignal->SetYTitle("Number of Entries");
    fHistConvreco_PointingAngle_MCsignal = new TH1F("h_Conv_PointingAngle_MCsignal",
					      "cos of angle between Conv' momentum and vec(PV)-vec(SecVtx)",
					      50, 0.995, 1);
    fHistConvreco_PointingAngle_MCsignal->SetXTitle("cos(#delta)");
    fHistConvreco_PointingAngle_MCsignal->SetYTitle("Number of Entries");
    fHistConvreco_R_vtx_MCsignal = new TH1F("h_Conv_R_vtx_MCsignal",
					   "Radial distance of secondary vertices",
					   120, 0.0, 60.0);
    fHistConvreco_R_vtx_MCsignal->SetXTitle("r_{#perp} (cm)");
    fHistConvreco_R_vtx_MCsignal->SetYTitle("Number of Entries");
        
    gDirectory->cd("../");

    // ==========================================================================
    // Book bkg histograms for Conv-candidates that fail the MC Truth Matching
    // ==========================================================================

    gDirectory->mkdir("Bkg_MC", "Conv-candidates that fail the MC Truth Matching");
    gDirectory->cd("Bkg_MC");

     if ( fMode == kKinFit ) {
	fHistConvreco_m_MCbkg   = new TH1F("h_Conv_m_MCbkg", "Invariant Conv mass (equaly signed electrons)",
					50, 0.0, 0.01);
    }else if (fMode == kCutBased){
	fHistConvreco_m_MCbkg   = new TH1F("h_Conv_m_PiPi", "Invariant Conv mass (equaly signed electrons)",
					50, 0.0, 0.01);
    }
    fHistConvreco_m_MCbkg->SetXTitle("m_{Conv} (GeV)");
    fHistConvreco_m_MCbkg->SetYTitle("Number of Entries");
    fHistConvreco_pt_MCbkg  = new TH1F("h_Conv_pt_MCbkg", "Conv-p_{#perp} (equaly signed electrons)",
				    400, 0, 400);
    fHistConvreco_pt_MCbkg->SetXTitle("p_{#perp} (GeV)");
    fHistConvreco_pt_MCbkg->SetYTitle("Number of Entries");
    fHistConvreco_phi_MCbkg = new TH1F("h_Conv_phi_MCbkg", "Conv #phi distribution (equaly signed electrons)",
				    128, -3.2, 3.2);
    fHistConvreco_phi_MCbkg->SetXTitle("#phi (rad)");
    fHistConvreco_phi_MCbkg->SetYTitle("Number of Entries");
    fHistConvreco_eta_MCbkg = new TH1F("h_Conv_eta_MCbkg", "Conv #eta distribution (equaly signed electrons)",
				    50, -3, 3);
    fHistConvreco_eta_MCbkg->SetXTitle("#eta");
    fHistConvreco_eta_MCbkg->SetYTitle("Number of Entries");
    fHistConvreco_N_MCbkg   = new TH1F("h_Conv_N_MCbkg", "Reconstructed Conv' per event (equaly signed electrons)",
				    11,-0.5,10.5);
    fHistConvreco_N_MCbkg->SetXTitle("N_{Conv}/evt");
    fHistConvreco_N_MCbkg->SetYTitle("Number of Entries");
    fHistConvreco_tau_MCbkg = new TH1F("h_Conv_tau_MCbkg", "Reconstructed Conv' lifetime (equaly signed electrons)", 
				    100, 0, 0.2);
    fHistConvreco_tau_MCbkg->SetXTitle("#tau (ps)");
    fHistConvreco_tau_MCbkg->SetYTitle("Number of Entries");
    fHistConvreco_DecayLength_MCbkg = new TH1F("h_Conv_DecayLength_MCbkg",
					    "Reconstructed Conv' decay length (equaly signed electrons)", 
					    100, 0, 50);
    fHistConvreco_DecayLength_MCbkg->SetXTitle("#DecayLength ");
    fHistConvreco_DecayLength_MCbkg->SetYTitle("Number of Entries");
    fHistConvreco_OpeningAngle_MCbkg = new TH1F("h_Conv_OpeningAngle_MCbkg",
					     "opening angle between electron tracks",
					     128, 0, 0.01);
    fHistConvreco_OpeningAngle_MCbkg->SetXTitle("#alpha (rad)");
    fHistConvreco_OpeningAngle_MCbkg->SetYTitle("Number of Entries");
    fHistConvreco_DecayAngle_MCbkg = new TH1F("h_Conv_DecayAngle_MCbkg",
					   "decay angle of reconstructed Conv that fail truthmatching",
					   50, -1, 1);
    fHistConvreco_DecayAngle_MCbkg->SetXTitle("cos(#beta) (rad)");
    fHistConvreco_DecayAngle_MCbkg->SetYTitle("Number of Entries");
    fHistConvreco_PointingAngle_MCbkg = new TH1F("h_Conv_PointingAngle_MCbkg",
					      "cos of angle between Conv' momentum and vec(PV)-vec(SecVtx)",
					      50, 0.995, 1);
    fHistConvreco_PointingAngle_MCbkg->SetXTitle("cos(#delta)");
    fHistConvreco_PointingAngle_MCbkg->SetYTitle("Number of Entries");
    fHistConvreco_R_vtx_MCbkg = new TH1F("h_Conv_R_vtx_MCbkg",
					   "Radial distance of secondary vertices",
					   120, 0.0, 60.0);
    fHistConvreco_R_vtx_MCbkg->SetXTitle("r_{#perp} (cm)");
    fHistConvreco_R_vtx_MCbkg->SetYTitle("Number of Entries");
         
    gDirectory->cd("../../../");
}


//____________________________________________________________________

Bool_t AtlConvFinder::AnalyzeEvent() {
    //
    // Event analysis
    //

    // Count real Conv to compute efficiency
    if (fEvent->IsMC() ) {
	TList *Conv_list = fEvent->GetMCParticles("gamma");
	Int_t Nr_ConvMC = 0;
	
	for( Int_t i = 0; i < Conv_list->GetEntries(); i++ ){
	    
	    HepMCParticle *prt = (HepMCParticle*)(Conv_list->At(i));
	    if (! prt->IsConversion() ) continue;
	    if (prt->GetN_Daughters() != 2) continue;
	    // daughter-trks
	    AtlIDTrack *daughter1 =
		fEvent->GetAssociatedIDTrack((HepMCParticle*)prt->GetDaughters()->At(0));
	    AtlIDTrack *daughter2 =
		fEvent->GetAssociatedIDTrack((HepMCParticle*)prt->GetDaughters()->At(1));
	    if( (daughter1 == 0) || (daughter2 == 0) ) continue;
	    if( (TMath::Abs(prt->Eta()) < 2.5) && (prt->Pt() > 0.5) ){
		if (daughter1->HasValidTruthMatch(fMatchingProb_min) &&
		    daughter2->HasValidTruthMatch(fMatchingProb_min) ){
		    Nr_ConvMC++;
		}
	    }
	}
	AddAbundanceTrue( Nr_ConvMC );
    }
    
    // Reconstruct Conv decays
    if ( fMode == kKinFit ) {
	ReconstructConvKinFit();
    } else if (fMode == kCutBased){
	ReconstructConvCutBased();
    } else {
	Error("AnalyzeEvent",
	      "Invalid mode given. Abort!");
	gSystem->Abort(0);
    }
    return kTRUE;
}

//____________________________________________________________________

void AtlConvFinder::ReconstructConvCutBased(){

    HepVertex  *Vtx  = 0;
    HepVertex *PrimVtx = 0;
    AtlIDTrack *trk1 = 0;
    AtlIDTrack *trk2 = 0;

    TLorentzVector pConv;
    
    TLorentzVector p1,p2;
    AtlPhotonConv *Conv = 0;

    // Storing Prim. Vtx
    PrimVtx = fEvent->GetPrimaryVtx();
    
    // Vertex Loop
    for(Int_t i = 0; i < fEvent->GetN_Vertices(); i++){

	// Get Vtx and associated Daughters
	Vtx = (HepVertex*)fEvent->GetVertices()->At(i);
	if(Vtx->GetNDaughters() != 2) continue;
	SetCutFlow("Sec Vtx with 2 Daughters");
	trk1 = (AtlIDTrack*)Vtx->GetDaughters()->At(0);
	trk2 = (AtlIDTrack*)Vtx->GetDaughters()->At(1);
		
	// Cut on R_Vtx
	if ( Vtx->GetPos().Perp() < fR_Vtx_min ) continue;
	SetCutFlow("R_Vtx");

	// Cut on Transverse Decay Length
	if( (PrimVtx->GetPos() - Vtx->GetPos()).Perp() < fTransvDecayLength_min)
	    continue;
	SetCutFlow("Decay Len.");

	// Get Momenta of Daughter Tracks
	p1.SetVectM(trk1->P(),fm_e);
	p2.SetVectM(trk2->P(),fm_e);
	pConv = p1+p2;
	
        // Momenta of outgoing electron tracks
	if ( (trk1->Pt() > fSignale_Pt_max) ||
	     (trk1->Pt() < fSignale_Pt_min) ) continue;
	if ( (trk2->Pt() > fSignale_Pt_max) ||
	     (trk2->Pt() < fSignale_Pt_min) ) continue;
	SetCutFlow("electron p_{#perp}");

        // Eta of outgoing electron tracks
	if ( (trk1->Eta() > fSignale_Eta_max) ||
	     (trk1->Eta() < fSignale_Eta_min) ) continue;
	if ( (trk2->Eta() > fSignale_Eta_max) ||
	     (trk2->Eta() < fSignale_Eta_min) ) continue;
	SetCutFlow("electron #eta");
	
	// Discard all tracks without reference to a truth particle for all
	// MC-Runs to compute the purity correctly
	if( fEvent->IsMC() ){
	    if( !( trk1->HasValidTruthMatch(0.0001) &&
		   trk2->HasValidTruthMatch(0.0001)) )  continue;
	}
	SetCutFlow("MC exists");


	// Cut on cosine of pointing angle
	if( TMath::Cos(pConv.Angle((Vtx->GetPos() - PrimVtx->GetPos())))
	    < fPointingAngle_min )
	    continue;
	SetCutFlow("Pointing Angle");
	
	// Cut on cos of Decay Angle
	TLorentzVector RF;
	RF.SetVectM(pConv.Vect(),0.497); // K0s mass considered to get a well defined rest frame
	TVector3 boost = -RF.BoostVector();
	TLorentzVector P_trk1_RFK0s = p1;
	TLorentzVector P_trk2_RFK0s = p2;
	P_trk1_RFK0s.Boost(boost); // boosting trks momentum in K0s restframe
	P_trk2_RFK0s.Boost(boost); // boosting trks momentum in K0s restframe
	if ( (TMath::Abs(TMath::Cos(pConv.Angle(P_trk1_RFK0s.Vect()))) < fDecayAngle_min)
	     || TMath::Abs((TMath::Cos(pConv.Angle(P_trk2_RFK0s.Vect()))) < fDecayAngle_min) )
	    continue;
	SetCutFlow("Decay Angle");

	// Mass request
	if(TMath::Abs(pConv.M()) > fConv_M_max) continue;
	
	//require opposite sign
	if ( trk1->Charge()*trk2->Charge() < 0. ) { 
	    SetCutFlow("opp. sign");
	    
	    // look for right order
	    if ( trk1->QovP()>0. ) {
		HepParticle Fit_Daughter1(1,p1.Px(),p1.Py(),p1.Pz(), p1.E(),-11);
		HepParticle Fit_Daughter2(2,p2.Px(),p2.Py(),p2.Pz(), p2.E(),11);
		Conv = fEvent->AddPhotonConv(pConv.Px(),pConv.Py(),pConv.Pz(),pConv.E(),
					    trk1,trk2,Vtx,
					    Fit_Daughter1,Fit_Daughter2);
	    } else {
		HepParticle Fit_Daughter2(1,p1.Px(),p1.Py(),p1.Pz(), p1.E(),-11);
		HepParticle Fit_Daughter1(2,p2.Px(),p2.Py(), p2.Pz(), p2.E(),11);
		Conv = fEvent->AddPhotonConv(pConv.Px(),pConv.Py(),pConv.Pz(),pConv.E(),
					    trk2,trk1,Vtx,
					    Fit_Daughter1, Fit_Daughter2);
	    }
	    fN_RecConv++;
	    
	    // Hitbased TruthMatching
	    if ( fEvent->IsMC() ){
		switch ( DoTruthMatch(Conv) ){
		    case  1:
			fTrueReco = kTRUE;
			FillHistograms_MCTruthStudy(Conv,'s');
			fN_TrueRecConv++;
			break;
		    case -1:
			fTrueReco = kFALSE;
			FillHistograms_MCTruthStudy(Conv,'b');
			break;
		    case  0:
			fN_NoDecision++;
			continue;
		}
	    }
	    
	    
	    // fill fitter histograms depending on Chi2 and NDoF
	    AtlKinFitterTool::FillHistograms("Signal"); 
	    
	}// same charged
	// If there are like signed electrons, fill BkgDecay-List later
    }
}

//____________________________________________________________________

void AtlConvFinder::ReconstructConvKinFit(){
    //
    // Conversion reconstruction from track-pairs assuming being electrons using KinFitter
    //
    
    HepVertex  *Vtx     = 0;
    HepVertex  *PrimVtx = 0;
    AtlIDTrack *trk1    = 0;  
    AtlIDTrack *trk2    = 0;
    
    TLorentzVector pConv;
    
    TMatrixD cov_trk1(3,3);
    TMatrixD cov_trk2(3,3);

    Int_t   BgkCount     = 1;  // gives ID of background Conv (from same charged electrons)
    Float_t PvalueKinFit = 0.0;

    //storing primary Vtx
    PrimVtx = fEvent->GetPrimaryVtx();
    
    // Vertex-Loop
    for ( Int_t i = 0; i < fEvent->GetN_Vertices(); i++){
	
	Vtx = (HepVertex*)fEvent->GetVertices()->At(i);
	if ( !(Vtx->GetNDaughters() == 2) ) continue;
	SetCutFlow("Sec. Vtx with 2 Dght.");

	// Cut on R_Vtx
	if ( Vtx->GetPos().Perp() < fR_Vtx_min ) continue;
	SetCutFlow("R_Vtx");
	
	// Cut on Transverse Decay Length
	if( (PrimVtx->GetPos() - Vtx->GetPos()).Perp() < fTransvDecayLength_min)
	    continue;
	SetCutFlow("Decay Len.");

	// Get Momenta of Daughter Tracks
	trk1 = (AtlIDTrack*)Vtx->GetDaughters()->At(0);
	trk2 = (AtlIDTrack*)Vtx->GetDaughters()->At(1);
	// Momenta of outgoing pion tracks
	if ( (trk1->Pt() > fSignale_Pt_max) ||
	     (trk1->Pt() < fSignale_Pt_min) ) continue;
	if ( (trk2->Pt() > fSignale_Pt_max) ||
	     (trk2->Pt() < fSignale_Pt_min) ) continue;
	SetCutFlow("#pi p_{#perp}");
	// Eta of outgoing pion tracks
	if ( (trk1->Eta() > fSignale_Eta_max) ||
	     (trk1->Eta() < fSignale_Eta_min) ) continue;
	if ( (trk2->Eta() > fSignale_Eta_max) ||
	     (trk2->Eta() < fSignale_Eta_min) ) continue;
	SetCutFlow("#eta (pions)");
	
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
	TFitParticlePtEtaPhi FitExec_trk1("FitExec_trk1","FitExec_trk1 for ConvFit",
					  &dummy1,fm_e,&cov_trk1);
	TVector3 dummy2 = trk2->P(); // instead of const cast
	TFitParticlePtEtaPhi FitExec_trk2("FitExec_trk2","FitExec_trk2 for ConvFit",
					  &dummy2,fm_e,&cov_trk2);

	// definition of Conv mass constraint
	TFitConstraintMGaus MConvCons("ConvMassConstraint","ConvMassConstraintGaus",0,0,fm_PolMass, fm_Width);
	MConvCons.addParticles1(&FitExec_trk1,&FitExec_trk2);
	
	// fitting characteristics
	TKinFitter fitter;
	fitter.addMeasParticle(&FitExec_trk1);
	fitter.addMeasParticle(&FitExec_trk2);
	fitter.addConstraint(&MConvCons);
	
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

            // Copy fitted Vectors
	    FitP_trk1=*(FitExec_trk1.getCurr4Vec());
	    FitP_trk2=*(FitExec_trk2.getCurr4Vec());

	    // Reconstruct Conv 4-momentum
	    pConv = FitP_trk1 + FitP_trk2;

	    // Cut on cosine of pointing angle
	    if( TMath::Cos(pConv.Angle((Vtx->GetPos() - PrimVtx->GetPos())))
		< fPointingAngle_min )
	    	continue;
	    SetCutFlow("Pointing Angle");

	    // Cut on cos of Decay Angle
	    TLorentzVector RF;
	    RF.SetVectM(pConv.Vect(),0.497); // K0s mass considered to get a well defined rest frame
	    TVector3 boost = -RF.BoostVector();
	    TLorentzVector P_trk1_RFK0s = FitP_trk1;
	    TLorentzVector P_trk2_RFK0s = FitP_trk2;
	    P_trk1_RFK0s.Boost(boost); // boosting trks momentum in K0s restframe
	    P_trk2_RFK0s.Boost(boost); // boosting trks momentum in K0s restframe
	    if ( (TMath::Abs(TMath::Cos(pConv.Angle(P_trk1_RFK0s.Vect()))) < fDecayAngle_min)
		 || TMath::Abs((TMath::Cos(pConv.Angle(P_trk2_RFK0s.Vect()))) < fDecayAngle_min) )
		continue;
	    SetCutFlow("Decay Angle");
	    
	    //setting Chi2,NDoF
	    SetChi2(fitter.getS());
	    SetNDoF(fitter.getNDF());
	    PvalueKinFit = TMath::Prob(fChi2, fNDoF);

	    // Cut on KinFit p-Value
	    if (PvalueKinFit >= fKinFitPvalue_min) {
		SetCutFlow("KinFit p-value");

		// Adding Conversion Candidate to Event
		AtlPhotonConv *Conv;
		if ( trk1->Charge()*trk2->Charge() < 0. ) { //require opposite sign
		    SetCutFlow("opp. sign");

                      // look for right order
		    if ( trk1->QovP()>0. ) {
			HepParticle Fit_Daughter1(1,FitP_trk1.Px(),FitP_trk1.Py(),
						  FitP_trk1.Pz(), FitP_trk1.E(),211);
			HepParticle Fit_Daughter2(2,FitP_trk2.Px(),FitP_trk2.Py(),
						  FitP_trk2.Pz(), FitP_trk2.E(),-211);
			Conv = fEvent->AddPhotonConv(pConv.Px(),pConv.Py(),pConv.Pz(),pConv.E(),
						     trk1,trk2,Vtx,
						     Fit_Daughter1,Fit_Daughter2);
		    } else {
			HepParticle Fit_Daughter1(1,FitP_trk2.Px(),FitP_trk2.Py(),
						  FitP_trk2.Pz(), FitP_trk2.E(),211);
			HepParticle Fit_Daughter2(2,FitP_trk1.Px(),FitP_trk1.Py(),
						  FitP_trk1.Pz(), FitP_trk1.E(),-211);
			Conv = fEvent->AddPhotonConv(pConv.Px(),pConv.Py(),pConv.Pz(),pConv.E(),
						     trk2,trk1,Vtx,
						     Fit_Daughter1, Fit_Daughter2);
		    }
		    fN_RecConv++;
                    // Set Chi2, NDoF
		    Conv->SetChi2NDoF(fChi2, fNDoF);

		    // Hitbased TruthMatching
		    if ( fEvent->IsMC() ){
			switch ( DoTruthMatch(Conv) ){
			    case  1:
				fTrueReco = kTRUE;
				FillHistograms_MCTruthStudy(Conv,'s');
				fN_TrueRecConv++;
				break;
			    case -1:
				fTrueReco = kFALSE;
				FillHistograms_MCTruthStudy(Conv,'b');
				break;
			    case  0:
				fN_NoDecision++;
				continue;
			}
		    }
		    
		    
		    // fill fitter histograms depending on Chi2 and NDoF
		    AtlKinFitterTool::FillHistograms("Signal"); 
		    
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
		} else { // end: opp. sign request
		    
		    // Add same charged combinations to the bkg list
		    Int_t pdgCode = 22;
		    HepParticle Fit_Daughter1(1,FitP_trk1.Px(),FitP_trk1.Py(),
					      FitP_trk1.Pz(), FitP_trk1.E(),pdgCode);
		    HepParticle Fit_Daughter2(2,FitP_trk2.Px(),FitP_trk2.Py(),
					      FitP_trk2.Pz(), FitP_trk2.E(),pdgCode);
		    Conv = new AtlPhotonConv(BgkCount++,pConv.Px(), pConv.Py(),
					    pConv.Pz(), pConv.E(),
					    trk1, trk2,
					    Vtx, PrimVtx,
					    Fit_Daughter1,Fit_Daughter2);

		    Conv->SetChi2NDoF(fChi2, fNDoF);
		    fBkgPhotonConv->Add(Conv);
		    
		    // fill Chi2 and NDoFa
		    AtlKinFitterTool::FillHistograms("Bkg");
		}
		
	    }// End of Prob(Chi2ovNDoF) > x  request
	}
    } //end of vertex loop
}

//____________________________________________________________________    

void AtlConvFinder::FillHistograms() {
    //
    // Fill histograms
    //

    AtlPhotonConv *Conv   = 0;
    TLorentzVector PConv,P_ePlus,P_eMinus;
    Float_t OpeningAngle  = 0.;
    Float_t PointingAngle = 0.;
    

    // Signal (opposite charged)
    // ________________________________________

    for ( Int_t i = 0; i < fEvent->GetN_PhotonConv(); i++ ) {

       	Conv = (AtlPhotonConv*)fEvent->GetPhotonConv()->At(i);
	
	PConv     = Conv->P();
	P_ePlus    = Conv->GetReFitePlus().P();
	P_eMinus   = Conv->GetReFiteMinus().P();

        //filling the standard histograms
	//----------------------------------
	fHistConvreco_m_ee     ->Fill(Conv->M("REC")      ,GetPreTagEvtWeight());
	fHistConvreco_pt_ee    ->Fill(Conv->Pt()          ,GetPreTagEvtWeight());  
	fHistConvreco_phi_ee   ->Fill(Conv->Phi()         ,GetPreTagEvtWeight()); 
	fHistConvreco_eta_ee   ->Fill(Conv->Eta()         ,GetPreTagEvtWeight());
	fHistConvreco_tau_ee   ->Fill(Conv->GetLifeTime() ,GetPreTagEvtWeight());
	fHistConvreco_DecayLength_ee ->Fill(Conv->GetDecayLength(),            GetPreTagEvtWeight());
	fHistConvreco_R_vtx          ->Fill(Conv->GetVtx()->GetPos().Perp(),   GetPreTagEvtWeight());
	
	// FlightDirection
	TVector3 FlightDirection = (Conv->GetVtx())->GetPos();
	FlightDirection -= (fEvent->GetPrimaryVtx())->GetPos();
	PointingAngle = PConv.Angle(FlightDirection);
	fHistConvreco_PointingAngle_ee ->Fill(TMath::Cos(PointingAngle),GetPreTagEvtWeight());

        // Opening Angle
	OpeningAngle = P_ePlus.Angle(P_eMinus.Vect());
	fHistConvreco_OpeningAngle_ee  ->Fill(OpeningAngle, GetPreTagEvtWeight());

	// Decay Angle is the angle between Conv' momentum and the momentum of the e+
	// in K0s' restframe (since the restframe of gamma is not well defined)
	TLorentzVector RF;
	RF.SetVectM(PConv.Vect(),0.497);
	TVector3 boost = -RF.BoostVector();
	TLorentzVector P_trk1_RFK0s = P_ePlus; // looks like a bug? S. Stamm  (was P_ePlus.P());
	P_trk1_RFK0s.Boost(boost); // boosting trks momentum in K0s restframe
	Float_t DecayAngle = TMath::Cos( PConv.Angle(P_trk1_RFK0s.Vect()) );
	fHistConvreco_DecayAngle_ee ->Fill(DecayAngle,  GetPreTagEvtWeight());
	
	
        //filling pion histograms
	//--------------------------
	fHistConvreco_ePlus_pt     ->Fill(P_ePlus.Pt()   ,GetPreTagEvtWeight());
	fHistConvreco_eMinus_pt    ->Fill(P_eMinus.Pt()  ,GetPreTagEvtWeight());
	fHistConvreco_ePlus_eta    ->Fill(P_ePlus.Eta()  ,GetPreTagEvtWeight());
	fHistConvreco_eMinus_eta   ->Fill(P_eMinus.Eta() ,GetPreTagEvtWeight());
	fHistConvreco_ePlus_phi    ->Fill(P_ePlus.Phi()  ,GetPreTagEvtWeight());
	fHistConvreco_eMinus_phi   ->Fill(P_eMinus.Phi() ,GetPreTagEvtWeight());
			
    }
    fHistConvreco_N_ee ->Fill(fEvent->GetN_PhotonConv(), GetPreTagEvtWeight());
    
    
    // Combinatorial Background (Like-sign approach)
    // ____________________________________________

    TIter next(fBkgPhotonConv);
    while ( (Conv = (AtlPhotonConv*)next()) ) {
	
	TLorentzVector P_e1 = Conv->GetReFitePlus().P();
	TLorentzVector P_e2 = Conv->GetReFiteMinus().P();
	
	PConv  = Conv->P();
	P_e1  = Conv->GetReFitePlus().P();
	P_e2  = Conv->GetReFiteMinus().P();

        //filling the standard histograms
	//----------------------------------
	fHistConvreco_m_LS_bkg     ->Fill(Conv->M("REC")      ,GetPreTagEvtWeight());
	fHistConvreco_pt_LS_bkg    ->Fill(Conv->Pt()          ,GetPreTagEvtWeight());  
	fHistConvreco_phi_LS_bkg   ->Fill(Conv->Phi()         ,GetPreTagEvtWeight()); 
	fHistConvreco_eta_LS_bkg   ->Fill(Conv->Eta()         ,GetPreTagEvtWeight());
	fHistConvreco_tau_LS_bkg   ->Fill(Conv->GetLifeTime() ,GetPreTagEvtWeight());
	fHistConvreco_DecayLength_LS_bkg   ->Fill(Conv->GetDecayLength(),  GetPreTagEvtWeight());
	
        //filling pion histograms
	//-----------------------------
        // Get Daughters: both are same charged -> fill either positive or
	// negative daughter histograms
	
	// Check if positive or negative charged daughters


	// still buggy !!!
/*	if( Conv->GetePlus()->GetQovP() > 0. ) {
	    cout << fHistConvreco_ePlus_pt_LS_bkg << endl;
	    fHistConvreco_ePlus_pt_LS_bkg     ->Fill(P_e1.Pt(),   GetPreTagEvtWeight());
	    fHistConvreco_ePlus_eta_LS_bkg    ->Fill(P_e1.Eta(),  GetPreTagEvtWeight());
	    fHistConvreco_ePlus_phi_LS_bkg    ->Fill(P_e1.Phi(),  GetPreTagEvtWeight());
	} else {
	    fHistConvreco_eMinus_pt_LS_bkg    ->Fill(P_e2.Pt(),   GetPreTagEvtWeight());
	    fHistConvreco_eMinus_eta_LS_bkg   ->Fill(P_e2.Eta(),  GetPreTagEvtWeight());
	    fHistConvreco_eMinus_phi_LS_bkg   ->Fill(P_e2.Phi(),  GetPreTagEvtWeight());
	    } */
    } 
    fHistConvreco_N_LS_bkg ->Fill(fBkgPhotonConv->GetEntries(), GetPreTagEvtWeight()); 


    
}

//____________________________________________________________________

void AtlConvFinder::SetCutDefaults() {
    //
    // Selection cut defaults and Constraint parameters
    //
    
    // Cuts for Tracks  
    fSignale_Pt_min       =  0.0;      // Min. Pt of signal electrons
    fSignale_Pt_max       =  1.e10;    // Max. Pt of signal electrons
    fSignale_Eta_min      = -2.5;      // Min. eta of signal electrons
    fSignale_Eta_max      =  2.5;      // Max. eta of signal electrons

    fm_e                    = 0.000510998910;  // Electron mass hypothesis
}


//____________________________________________________________________


void AtlConvFinder::PrintCutValues(){
        
    if ( fMode == kKinFit ){
	cout << endl << " Used mode: kKinFit";
	cout << endl << " ----------------------------------------------" << endl;
	cout << " Used CutDefaults:" <<endl;
	cout << " ----------------------------------------------" << endl;
	cout << scientific << setprecision(3) << " Prob(Chi2/NDoF) > "
	     << fKinFitPvalue_min << endl;
	cout << scientific << setprecision(3) << " Pol Mass:  "
	     << fm_PolMass << endl;
	
    }
    else if (fMode == kCutBased)  {
	cout << endl << " Used mode: kCutBased " << endl;  
	cout << " ----------------------------------------------" << endl;
	cout << " Used CutDefaults:" <<endl;
	cout << " ----------------------------------------------" << endl;
	cout << scientific << "  M(Conv)   <  " << fConv_M_max << endl;
    }
    cout << scientific << "  e mass hypothesis: " 
	 << "  M_e = " << fm_e << endl;
    cout << scientific << setprecision(3) << "  " << fSignale_Pt_min <<
	" <  Pt(e)  < " << fSignale_Pt_max << endl;
    cout << scientific << " " << fSignale_Eta_min << " < Eta(e) < "
	 << fSignale_Eta_max << endl;
    cout << scientific << setprecision(3) << "  Min. of Matching Probability > "
	 << fMatchingProb_min << endl;
    cout << scientific
	 << "  Minimum of  Pointing Angle  = "
	 << fPointingAngle_min << endl
	 << "  Minimum of  transverse decay length  = "
	 << fTransvDecayLength_min << endl
	 << "  Minimumum of  Decay Angle (in K0s associated restframe  = "
	 << fDecayAngle_min << endl
	 << "  Minimum of R_Vtx = "
	 << fR_Vtx_min << endl;
    cout << "----------------------------------------------" << endl;

    
}

//____________________________________________________________________

Int_t AtlConvFinder::DoTruthMatch(AtlPhotonConv *ConvCandidate) {
    //
    // returns 1 in case of true Conv, -1 if not
    // returns 0 if no decision possible (see fMatchingProb_min)
    //

    AtlIDTrack *trk1 = ConvCandidate->GetePlus();
    AtlIDTrack *trk2 = ConvCandidate->GeteMinus();

    fHisteMatchingProb->Fill(trk1->GetMCParticleProb(), GetPreTagEvtWeight());
    fHisteMatchingProb->Fill(trk2->GetMCParticleProb(), GetPreTagEvtWeight());

    // Hitbased TruthMatching
    if( trk1->HasValidTruthMatch(fMatchingProb_min)
	&& trk2->HasValidTruthMatch(fMatchingProb_min)) {

	// remove "_bar" from particle name to collect antiparticle and particle in the same bin
	const char* fullPdgName = trk1->GetMCParticle()->GetMother()->GetPdgName();
	char pdgName[strlen(fullPdgName)];
	if (strstr(fullPdgName, "_bar") == NULL) {
	    strcpy(pdgName, fullPdgName);
	} else {
	    strncpy(pdgName, fullPdgName, strlen(fullPdgName)-4);
	    pdgName[strlen(fullPdgName)-4] = '\0';
	}

	// Comparison with truth content
	if( (trk1->GetMCParticle()->GetPdgCode() == -11)
	    && (trk2->GetMCParticle()->GetPdgCode()== 11)
	    && (trk1->GetMCParticle()->GetMother()->GetPdgCode()== 22)
	    && (trk1->GetMCParticle()->GetMother() == trk2->GetMCParticle()->GetMother()) ){
	    fHistConvreco_truthtype->Fill(pdgName, ConvCandidate->GetProb(), GetPreTagEvtWeight());
	    return 1;
	}else{
	    if (trk1->GetMCParticle()->GetMother() != trk2->GetMCParticle()->GetMother()) {
		fHistConvreco_truthtype->Fill("Comb", ConvCandidate->GetProb(), GetPreTagEvtWeight());
	    } else {
		fHistConvreco_truthtype->Fill(pdgName, ConvCandidate->GetProb(), GetPreTagEvtWeight());
	    }
	    return -1;
	}
    }else{
	return 0;
    }

    return 0;
}


//____________________________________________________________________


void AtlConvFinder::FillHistograms_MCTruthStudy(AtlPhotonConv *ConvCandidate, char type) {
    // !!! key 'type' must be "s" for signal or "b" for background !!!
    // !!! routine could only be processed in case of a mc sample  !!!
    //
    // fills several histograms like pointing angle, invariant mass,
    // decay angle, ... separately for bkg and signal (Conv candiates that fulfill
    // the monte-carlo truth-matching as well as those that fail)
    
    TLorentzVector PConv,P_ePlus,P_eMinus;
    Float_t OpeningAngle  = 0.;
    Float_t DecayAngle    = 0.;
    Float_t PointingAngle = 0.;
    
    PConv   = ConvCandidate->P();
    P_ePlus  = ConvCandidate->GetReFitePlus().P();
    P_eMinus = ConvCandidate->GetReFiteMinus().P();

    // Opening Angle
    OpeningAngle = P_ePlus.Angle(P_eMinus.Vect());
    // FlightDirection
    TVector3 FlightDirection = (ConvCandidate->GetVtx())->GetPos();
    FlightDirection -= (fEvent->GetPrimaryVtx())->GetPos();
    PointingAngle = PConv.Angle(FlightDirection);
    // Decay Angle is the angle between Conv' momentum and the momentum of the e+
    // in K0s' restframe (since the restframe of gamma is not well defined)
    TLorentzVector RF;
    RF.SetVectM(PConv.Vect(),0.497);
    TVector3 boost = -RF.BoostVector();
    TLorentzVector P_trk1_RFK0s = P_ePlus; // looks like a bug? S. Stamm (was P_ePlus.P());
    P_trk1_RFK0s.Boost(boost); // boosting trks momentum in K0s restframe
    DecayAngle = TMath::Cos( PConv.Angle(P_trk1_RFK0s.Vect()) );

    // filling histograms // decay angle, N,  
    switch (type) {
	case 's':
	    fHistConvreco_m_MCsignal     ->Fill(ConvCandidate->M("REC")      ,GetPreTagEvtWeight());
	    fHistConvreco_pt_MCsignal    ->Fill(ConvCandidate->Pt()          ,GetPreTagEvtWeight());  
	    fHistConvreco_phi_MCsignal   ->Fill(ConvCandidate->Phi()         ,GetPreTagEvtWeight()); 
	    fHistConvreco_eta_MCsignal   ->Fill(ConvCandidate->Eta()         ,GetPreTagEvtWeight());
	    fHistConvreco_tau_MCsignal   ->Fill(ConvCandidate->GetLifeTime() ,GetPreTagEvtWeight());
	    fHistConvreco_DecayLength_MCsignal   ->Fill(ConvCandidate->GetTransvDecayLength(), GetPreTagEvtWeight());
	    fHistConvreco_PointingAngle_MCsignal ->Fill(TMath::Cos(PointingAngle),             GetPreTagEvtWeight());
	    fHistConvreco_OpeningAngle_MCsignal  ->Fill(OpeningAngle, GetPreTagEvtWeight());
	    fHistConvreco_DecayAngle_MCsignal    ->Fill(DecayAngle,   GetPreTagEvtWeight());
	    fHistConvreco_N_MCsignal             ->Fill(1,            GetPreTagEvtWeight());
	    fHistConvreco_R_vtx_MCsignal ->Fill(ConvCandidate->GetVtx()->GetPos().Perp(),      GetPreTagEvtWeight());
	    break;
	case 'b':
	    fHistConvreco_m_MCbkg     ->Fill(ConvCandidate->M("REC")      ,GetPreTagEvtWeight());
	    fHistConvreco_pt_MCbkg    ->Fill(ConvCandidate->Pt()          ,GetPreTagEvtWeight());  
	    fHistConvreco_phi_MCbkg   ->Fill(ConvCandidate->Phi()         ,GetPreTagEvtWeight()); 
	    fHistConvreco_eta_MCbkg   ->Fill(ConvCandidate->Eta()         ,GetPreTagEvtWeight());
	    fHistConvreco_tau_MCbkg   ->Fill(ConvCandidate->GetLifeTime() ,GetPreTagEvtWeight());
	    fHistConvreco_DecayLength_MCbkg   ->Fill(ConvCandidate->GetTransvDecayLength(),  GetPreTagEvtWeight());
	    fHistConvreco_PointingAngle_MCbkg ->Fill(TMath::Cos(PointingAngle),              GetPreTagEvtWeight());
	    fHistConvreco_OpeningAngle_MCbkg  ->Fill(OpeningAngle,  GetPreTagEvtWeight());
	    fHistConvreco_DecayAngle_MCbkg    ->Fill(DecayAngle,    GetPreTagEvtWeight());
	    fHistConvreco_N_MCbkg             ->Fill(1,             GetPreTagEvtWeight());
	    fHistConvreco_R_vtx_MCbkg ->Fill(ConvCandidate->GetVtx()->GetPos().Perp(),       GetPreTagEvtWeight());
	    break;
	default:
	    cout << endl << endl
		 <<"!!! Error using FillHistograms_MCTruthStudy(AtlConvDecayee*,char) in AtlConvFinder"
		 << endl << "Select either 's' or 'b' as key !!!"
		 << endl << endl;
	    break;
    }
}

//____________________________________________________________________

void AtlConvFinder::Terminate() {

    // print truth matching statistics
    fHistConvreco_truthtype->LabelsDeflate("X");
    fHistConvreco_truthtype->SetStats(kFALSE);
    fHistConvreco_truthtype->GetXaxis()->SetLabelSize(0.06);
    fHistConvreco_truthtype->GetXaxis()->LabelsOption("v");

    // Number of reconstructed Conversions and General Purity
    cout << endl << endl
	 << "NO. OF RECONSTRUCTED Conv':  " << fN_RecConv
	 << ", GENERAL PURITY:  " << ((Float_t)fN_TrueRecConv)/(Float_t)(fN_RecConv - fN_NoDecision)
	 << endl;

    AtlKinFitterTool::Terminate();
}


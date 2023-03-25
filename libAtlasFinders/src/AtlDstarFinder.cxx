//____________________________________________________________________
//
// Dstar decay finder class
//

//
// Authors: Thomas Kintscher <mailto:kintscher@physik.hu-berlin.de>
//          Sophia Schade    <mailto:schade@physik.hu-berlin.de>
// Copyright: 2010 (C) Thomas Kintscher
//
#ifndef ATLAS_AtlDstarFinder
#include <AtlDstarFinder.h>
#endif
#include <AtlEvent.h>
#include <TTree.h>
#include <TString.h>
#include <iostream>
#include <TROOT.h>
#include <TFile.h>
#include <vector>
#include <iomanip>
#include <string>

using namespace std;

#ifndef __CINT__
ClassImp(AtlDstarFinder);
#endif

//____________________________________________________________________

AtlDstarFinder::AtlDstarFinder(const char* name, const char* title) :
    AtlKinFitterTool(name, title) {
    //
    // Default constructor
    //
    SetMode(kKinFit);
    SetDebugOutput(kFALSE);
    fBkgLambdaDecays = new TList;
}

//____________________________________________________________________

AtlDstarFinder::~AtlDstarFinder() {
    //
    // Default destructor
    //
    fBkgLambdaDecays->Delete(); delete fBkgLambdaDecays;
    if (IsDebugRun() ) fDebugStream.close();
}

//____________________________________________________________________

void AtlDstarFinder::SetBranchStatus() {
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

void AtlDstarFinder::BookHistograms() {
    //
    // Book histograms
    //
    if ( IsDebugRun() ) fDebugStream.open("DebugOutputDstar.dat");
    fN_Fits = 0;
    fN_MCFail = 0;
    fN_MCProbMiss = 0;
    fN_SameOriginMiss = 0;

    // Book KinFitter Histograms
    AtlKinFitterTool::BookHistograms();
    SetTruthMatchBinning(30, 1, 30);
    PrintCutValues();

    // ======================
    // Book signal histograms
    // ======================
    gDirectory->mkdir("Signal", "Oppositely charged candidates");
    gDirectory->cd("Signal");
    

    // Lambda -> PPi
    // ----------
    gDirectory->mkdir("PPi", "Lambda -> proton+pion");
    gDirectory->cd("PPi");
    
    if ( fMode == kCutBased ) {
      fHistLambdareco_m_PPi   = new TH1F("h_Lambda_m_PPi", "Invariant Lambda mass (Lambda->PPi)", 100, 0.6, 1.6);
    } else if ( fMode == kKinFit ) {
      fHistLambdareco_m_PPi   = new TH1F("h_Lambda_m_PPi", "Invariant Lambda mass (Lambda->PPi)", 1000, 0.6, 2.5);
      fHistD0reco_m_kaon_pi   = new TH1F("h_D0_m_kaon_pi", "Invariant mass (something->K pi", 1000, 0.0, 2.5);
    }
    fHistLambdareco_m_PPi->SetXTitle("m_{Lambda} (GeV)"); fHistLambdareco_m_PPi->SetYTitle("Number of Entries");
    fHistD0reco_m_kaon_pi->SetXTitle("m_{Something} (GeV)"); fHistD0reco_m_kaon_pi->SetYTitle("Number of Entries");
    
    fHistLambdareco_m_PiPi  = new TH1F("h_Lambda_m_PiPi", "Invariant mass (* #rightarrow #pi#pi)", 480, 0.0, 2.5);
    fHistLambdareco_m_PiPi->SetXTitle("m (Gev)");         fHistLambdareco_m_PiPi->SetYTitle("Number of Entries");
    fHistLambdareco_m_ee  = new TH1F("h_Lambda_m_ee", "Invariant mass (* #rightarrow e e)", 480, 0.0, 2.5);
    fHistLambdareco_m_ee->SetXTitle("m (Gev)");         fHistLambdareco_m_ee->SetYTitle("Number of Entries");
    fHistLambdareco_pt_PPi  = new TH1F("h_Lambda_pt_PPi", "Lambda-p_{#perp} (Lambda->PPi)", 60, 0, 30);
    fHistLambdareco_pt_PPi->SetXTitle("p_{#perp} (GeV)"); fHistLambdareco_pt_PPi->SetYTitle("Number of Entries");
    fHistLambdareco_phi_PPi = new TH1F("h_Lambda_phi_PPi", "Lambda #phi distribution (Lambda->PPi)", 128, -3.2, 3.2);
    fHistLambdareco_phi_PPi->SetXTitle("#phi (rad)");     fHistLambdareco_phi_PPi->SetYTitle("Number of Entries");
    fHistLambdareco_eta_PPi = new TH1F("h_Lambda_eta_PPi", "Lambda #eta distribution (Lambda->PPi)", 100, -3, 3);
    fHistLambdareco_eta_PPi->SetXTitle("#eta");           fHistLambdareco_eta_PPi->SetYTitle("Number of Entries");
    fHistLambdareco_N_PPi   = new TH1F("h_Lambda_N_PPi", "Reconstructed Lambdas per event (Lambda->PPi)", 10, 0, 10);
    fHistLambdareco_N_PPi->SetXTitle("N_{Lambda}/evt");   fHistLambdareco_N_PPi->SetYTitle("Number of Entries");
    fHistLambdareco_tau_PPi = new TH1F("h_Lambda_tau_PPi", "Reconstructed Lambda lifetime (Lambda->PPi)", 100, 0, 300);
    fHistLambdareco_tau_PPi->SetXTitle("#tau (ps)");      fHistLambdareco_tau_PPi->SetYTitle("Number of Entries");
    fHistLambdareco_truth_PPi = new TH2F("h_Lambda_truth_PPi", "Pdg Code of truth-matched, reconstructed particles", 6260, -3130, 3130, 50, 0.0, 1.0);
    fHistLambdareco_truth_PPi->SetXTitle("Pdg Code");     fHistLambdareco_truth_PPi->SetYTitle("#Chi^{2} Probability");
    fHistLambdareco_truth_PPi->SetZTitle("Number of Entries");
    fHistLambdareco_R_vtx = new TH1F("h_Lambda_r_vtx", "Radial distance of secondary vertices", 400, 0.0, 100.0);
    fHistLambdareco_R_vtx->SetXTitle("r_{#perp} (cm)");   fHistLambdareco_R_vtx->SetYTitle("Number of Entries");
    fHistLambdareco_tdcy_len = new TH1F("h_Lambda_tdcy_len", "Transverse #Lambda decay length", 400, 0.0, 100.0);
    fHistLambdareco_tdcy_len->SetXTitle("s_{#perp} (cm)");fHistLambdareco_tdcy_len->SetYTitle("Number of Entries");
    fHistLambdareco_oangle = new TH1F("h_Lambda_oangle", "Opening angle of p/pi", 256, 0.0, 3.2);
    fHistLambdareco_oangle->SetXTitle("#alpha (rad)");    fHistLambdareco_oangle->SetYTitle("Number of Entries");
    fHistLambdareco_dangle = new TH1F("h_Lambda_dangle", "Decay angle of p/pi", 256, -1.0, 1.0);
    fHistLambdareco_dangle->SetXTitle("cos(#alpha)");     fHistLambdareco_dangle->SetYTitle("Number of Entries");
    
    // Pion histograms in sub-folder
    gDirectory->mkdir("Pion", "Pi from Lambda -> PPi");
    gDirectory->cd("Pion");
    fHistLambdareco_Pion_pt = new TH1F("h_Lambda_Pion_pt",   "#pi:  p_{#perp} (Lambda->P#Pi)", 60, 0, 30);
    fHistLambdareco_Pion_pt->SetXTitle("p_{#perp} (GeV)"); fHistLambdareco_Pion_pt->SetYTitle("Number of Entries");
    fHistLambdareco_Pion_eta = new TH1F("h_Lambda_Pion_eta", "#pi:  #eta distribution (Lambda->P#Pi)", 100, -3, 3);
    fHistLambdareco_Pion_eta->SetXTitle("#eta");           fHistLambdareco_Pion_eta->SetYTitle("Number of Entries");
    fHistLambdareco_Pion_phi = new TH1F("h_Lambda_Pion_phi", "#pi:  #phi distribution (Lambda->P#Pi)", 50, -3.14, 3.14);
    fHistLambdareco_Pion_phi->SetXTitle("#phi");           fHistLambdareco_Pion_phi->SetYTitle("Number of Entries");

    // Fine grained statistics about +/- charged pions
    gDirectory->mkdir("Pi_Plus", "Pion(+) from Lambda -> PPi");
    gDirectory->cd("Pi_Plus");
    fHistLambdareco_Piplus_pt = new TH1F("h_Lambda_Piplus_pt",   "#pi^{+}: p_{#perp} (Lambda->P#Pi)", 60, 0, 30);
    fHistLambdareco_Piplus_pt->SetXTitle("p_{#perp} (GeV)");  fHistLambdareco_Piplus_pt->SetYTitle("Number of Entries");
    fHistLambdareco_Piplus_eta = new TH1F("h_Lambda_Piplus_eta", "#pi^{+}: #eta distribution (Lambda->P#Pi)",100, -3, 3);
    fHistLambdareco_Piplus_eta->SetXTitle("#eta");            fHistLambdareco_Piplus_eta->SetYTitle("Number of Entries");
    fHistLambdareco_Piplus_phi = new TH1F("h_Lambda_Piplus_phi", "#pi^{+}: #phi distribution (Lambda->P#Pi)",50, -3.14, 3.14);
    fHistLambdareco_Piplus_phi->SetXTitle("#phi");            fHistLambdareco_Piplus_phi->SetYTitle("Number of Entries");

    gDirectory->cd("../");
    gDirectory->mkdir("Pi_Minus", "Pion(-) from Lambda -> PPi");
    gDirectory->cd("Pi_Minus");
    fHistLambdareco_Piminus_pt = new TH1F("h_Lambda_Piminus_pt",   "#pi^{-}: p_{#perp} (Lambda->P#Pi)", 60, 0, 30);
    fHistLambdareco_Piminus_pt->SetXTitle("p_{#perp} (GeV)"); fHistLambdareco_Piminus_pt->SetYTitle("Number of Entries");
    fHistLambdareco_Piminus_eta = new TH1F("h_Lambda_Piminus_eta", "#pi^{-}: #eta distribution (Lambda->P#Pi)", 100, -3, 3);
    fHistLambdareco_Piminus_eta->SetXTitle("#eta");           fHistLambdareco_Piminus_eta->SetYTitle("Number of Entries");
    fHistLambdareco_Piminus_phi = new TH1F("h_Lambda_Piminus_phi", "#pi^{-}: #phi distribution (Lambda->P#Pi)", 50, -3.14, 3.14);
    fHistLambdareco_Piminus_phi->SetXTitle("#phi");           fHistLambdareco_Piminus_phi->SetYTitle("Number of Entries");    
    
    gDirectory->cd("../../"); 

    // Proton histograms in sub-folder
    gDirectory->mkdir("Proton", "Proton from Lambda -> PPi");
    gDirectory->cd("Proton");
    fHistLambdareco_Proton_pt = new TH1F("h_Lambda_Proton_pt",   "Proton:  p_{#perp} (Lambda->P#Pi)", 60, 0, 30);
    fHistLambdareco_Proton_pt->SetXTitle("p_{#perp} (GeV)"); fHistLambdareco_Proton_pt->SetYTitle("Number of Entries");
    fHistLambdareco_Proton_eta = new TH1F("h_Lambda_Proton_eta", "Proton:  #eta distribution (Lambda->P#Pi)", 100, -5, 5);
    fHistLambdareco_Proton_eta->SetXTitle("#eta");           fHistLambdareco_Proton_eta->SetYTitle("Number of Entries");
    fHistLambdareco_Proton_phi = new TH1F("h_Lambda_Proton_phi", "Proton:  #phi distribution (Lambda->P#Pi)", 50, -3.14, 3.14);
    fHistLambdareco_Proton_phi->SetXTitle("#phi");           fHistLambdareco_Proton_phi->SetYTitle("Number of Entries");

    // fine grained statistics about proton/antiproton
    gDirectory->mkdir("Proton_Plus", "Proton(+) from Lambda -> PPi");
    gDirectory->cd("Proton_Plus");
    fHistLambdareco_Prplus_pt = new TH1F("h_Lambda_Prplus_pt",   "Pr^{+}   p_{#perp} (Lambda->P#Pi)", 60, 0, 30);
    fHistLambdareco_Prplus_pt->SetXTitle("p_{#perp} (GeV)"); fHistLambdareco_Prplus_pt->SetYTitle("Number of Entries");
    fHistLambdareco_Prplus_eta = new TH1F("h_Lambda_Prplus_eta", "Pr^{+}:  #eta distribution (Lambda->P#Pi)", 100, -5, 5);
    fHistLambdareco_Prplus_eta->SetXTitle("#eta");           fHistLambdareco_Prplus_eta->SetYTitle("Number of Entries");
    fHistLambdareco_Prplus_phi = new TH1F("h_Lambda_Prplus_phi", "Pr^{+}:  #phi distribution (Lambda->P#Pi)", 50, -3.14, 3.14);
    fHistLambdareco_Prplus_phi->SetXTitle("#phi");           fHistLambdareco_Prplus_phi->SetYTitle("Number of Entries");

    gDirectory->cd("../");
    gDirectory->mkdir("Proton_Minus", "Antiproton from Lambda -> PPi");
    gDirectory->cd("Proton_Minus");
    fHistLambdareco_Prminus_pt = new TH1F("h_Lambda_Prminus_pt",   "Pr^{-}   p_{#perp} (Lambda->P#Pi)", 60, 0, 30);
    fHistLambdareco_Prminus_pt->SetXTitle("p_{#perp} (GeV)"); fHistLambdareco_Prminus_pt->SetYTitle("Number of Entries");
    fHistLambdareco_Prminus_eta = new TH1F("h_Lambda_Prminus_eta", "Pr^{-}:  #eta distribution (Lambda->P#Pi)", 100, -5, 5);
    fHistLambdareco_Prminus_eta->SetXTitle("#eta");           fHistLambdareco_Prminus_eta->SetYTitle("Number of Entries");
    fHistLambdareco_Prminus_phi = new TH1F("h_Lambda_Prminus_phi", "Pr^{-}:  #phi distribution (Lambda->P#Pi)", 50, -3.14, 3.14);
    fHistLambdareco_Prminus_phi->SetXTitle("#phi");           fHistLambdareco_Prminus_phi->SetYTitle("Number of Entries");

    gDirectory->cd("../../../../");

    
    // ===================
    // Book bkg histograms
    // ===================
 
    gDirectory->mkdir("Background", "Same charged candidates");
    gDirectory->cd("Background");
    

    // Lambda -> PPi
    // ----------

    gDirectory->mkdir("PPi", "Lambda' -> proton pion");
    gDirectory->cd("PPi");

    // wide inv mass window to see the Lambda resonance for the cutbased reconstruction
    if ( fMode == kCutBased ) {
      fHistLambdareco_m_PPi_bkg   = new TH1F("h_Lambda_m_PPi_bkg", "Invariant Lambda' mass (Lambda->PPi)", 2400, 0.0, 1.2);
    } else {
      fHistLambdareco_m_PPi_bkg   = new TH1F("h_Lambda_m_PPi_bkg", "Invariant Lambda' mass (Lambda->PPi)", 50, 1.1, 2.5);
    }
    fHistLambdareco_m_PPi_bkg->SetXTitle("m_{Lambda} (GeV)"); fHistLambdareco_m_PPi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_pt_PPi_bkg  = new TH1F("h_Lambda_pt_PPi_bkg", "Lambda'-p_{#perp} (Lambda->PPi)", 60, 0, 30);
    fHistLambdareco_pt_PPi_bkg->SetXTitle("p_{#perp} (GeV)"); fHistLambdareco_pt_PPi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_phi_PPi_bkg = new TH1F("h_Lambda_phi_PPi_bkg", "Lambda' #phi distribution (Lambda->PPi)", 128, -3.2, 3.2);
    fHistLambdareco_phi_PPi_bkg->SetXTitle("#phi (rad)");     fHistLambdareco_phi_PPi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_eta_PPi_bkg = new TH1F("h_Lambda_eta_PPi_bkg", "Lambda' #eta distribution (Lambda->PPi)", 100, -3, 3);
    fHistLambdareco_eta_PPi_bkg->SetXTitle("#eta");           fHistLambdareco_eta_PPi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_N_PPi_bkg   = new TH1F("h_Lambda_N_PPi_bkg", "Reconstructed Lambda' per event (Lambda->PPi)", 10, 0, 10);
    fHistLambdareco_N_PPi_bkg->SetXTitle("N_{Lambda}/evt");   fHistLambdareco_N_PPi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_tau_PPi_bkg = new TH1F("h_Lambda_tau_PPi_bkg", "Reconstructed Lambda' lifetime (Lambda->PPi)", 100, 0, 300);
    fHistLambdareco_tau_PPi_bkg->SetXTitle("#tau (ps)");      fHistLambdareco_tau_PPi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_truth_PPi_bkg = new TH1F("h_Lambda_truth_PPi_bkg", "Pdg Code of truth-matched, reconstructed particles", 6260, -3130, 3130);
    fHistLambdareco_truth_PPi_bkg->SetXTitle("Pdg Code");     fHistLambdareco_truth_PPi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_R_vtx_bkg = new TH1F("h_Lambda_r_vtx_bkg", "Radial distance of secondary vertices", 400, 0.0, 100.0);
    fHistLambdareco_R_vtx_bkg->SetXTitle("r_{#perp} (cm)");   fHistLambdareco_R_vtx_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_tdcy_len_bkg = new TH1F("h_Lambda_tdcy_len_bkg", "Transverse #Lambda decay length", 400, 0.0, 100.0);
    fHistLambdareco_tdcy_len_bkg->SetXTitle("s_{#perp} (cm)");fHistLambdareco_tdcy_len_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_oangle_bkg = new TH1F("h_Lambda_oangle_bkg ", "Opening angle of p/pi", 256, 0.0, 3.2);
    fHistLambdareco_oangle_bkg ->SetXTitle("#alpha (rad)");   fHistLambdareco_oangle_bkg ->SetYTitle("Number of Entries");
    fHistLambdareco_dangle_bkg  = new TH1F("h_Lambda_dangle_bkg ", "Decay angle of p/pi", 256, -1.0, 1.0);
    fHistLambdareco_dangle_bkg ->SetXTitle("cos(#alpha)");    fHistLambdareco_dangle_bkg ->SetYTitle("Number of Entries");

    // Pion histograms in sub-folder
    gDirectory->mkdir("Pion", "Pion from Lambda' -> PPi");
    gDirectory->cd("Pion");
    fHistLambdareco_Pion_pt_bkg = new TH1F("h_Lambda_Pion_pt_bkg",   "#pi:  p_{#perp} (Lambda->P#Pi)", 60, 0, 30);
    fHistLambdareco_Pion_pt_bkg->SetXTitle("p_{#perp} (GeV)"); fHistLambdareco_Pion_pt_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_Pion_eta_bkg = new TH1F("h_Lambda_Pion_eta_bkg", "#pi:  #eta distribution (Lambda->P#Pi)", 100, -3, 3);
    fHistLambdareco_Pion_eta_bkg->SetXTitle("#eta");           fHistLambdareco_Pion_eta_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_Pion_phi_bkg = new TH1F("h_Lambda_Pion_phi_bkg", "#pi:  #phi distribution (Lambda->P#Pi)", 50, -3.14, 3.14);
    fHistLambdareco_Pion_phi_bkg->SetXTitle("#phi");           fHistLambdareco_Pion_phi_bkg->SetYTitle("Number of Entries");
    
    gDirectory->mkdir("Pi_Plus", "Pion(+) from Lambda' -> PPi");
    gDirectory->cd("Pi_Plus");
    fHistLambdareco_Piplus_pt_bkg = new TH1F("h_Lambda_Piplus_pt",   "#pi^{+}:  p_{#perp} (Lambda'->P#Pi)", 60, 0, 30);
    fHistLambdareco_Piplus_pt_bkg->SetXTitle("p_{#perp} (GeV)");  fHistLambdareco_Piplus_pt_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_Piplus_eta_bkg = new TH1F("h_Lambda_Piplus_eta", "#pi^{+}:  #eta distribution (Lambda'->P#Pi)", 100, -3, 3);
    fHistLambdareco_Piplus_eta_bkg->SetXTitle("#eta");            fHistLambdareco_Piplus_eta_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_Piplus_phi_bkg = new TH1F("h_Lambda_Piplus_phi", "#pi^{+}:  #phi distribution (Lambda'->P#Pi)", 50, -3.14, 3.14);
    fHistLambdareco_Piplus_phi_bkg->SetXTitle("#phi");            fHistLambdareco_Piplus_phi_bkg->SetYTitle("Number of Entries");

    gDirectory->cd("../");
    gDirectory->mkdir("Pi_Minus", "Pion(-) from Lambda' -> PPi");
    gDirectory->cd("Pi_Minus");
    fHistLambdareco_Piminus_pt_bkg = new TH1F("h_Lambda_Piminus_pt",   "#pi^{-}:  p_{#perp} (Lambda'->P#Pi)", 60, 0, 30);
    fHistLambdareco_Piminus_pt_bkg->SetXTitle("p_{#perp} (GeV)"); fHistLambdareco_Piminus_pt_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_Piminus_eta_bkg = new TH1F("h_Lambda_Piminus_eta", "#pi^{-}:  #eta distribution (Lambda'->P#Pi)", 100, -3, 3);
    fHistLambdareco_Piminus_eta_bkg->SetXTitle("#eta");           fHistLambdareco_Piminus_eta_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_Piminus_phi_bkg = new TH1F("h_Lambda_Piminus_phi", "#pi^{-}:  #phi distribution (Lambda'->P#Pi)", 50, -3.14, 3.14);
    fHistLambdareco_Piminus_phi_bkg->SetXTitle("#phi");           fHistLambdareco_Piminus_phi_bkg->SetYTitle("Number of Entries");

    gDirectory->cd("../../"); 
    
    // Proton histograms in sub-folder
    gDirectory->mkdir("Proton", "Proton from Lambda' -> PPi");
    gDirectory->cd("Proton");
    fHistLambdareco_Proton_pt_bkg = new TH1F("h_Lambda_Proton_pt_bkg",   "Proton:  p_{#perp} (Lambda'->#Pi#Pi)", 60, 0, 30);
    fHistLambdareco_Proton_pt_bkg->SetXTitle("p_{#perp} (GeV)"); fHistLambdareco_Proton_pt_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_Proton_eta_bkg = new TH1F("h_Lambda_Proton_eta_bkg", "Proton:  #eta distribution (Lambda'->#Pi#Pi)", 100, -5, 5);
    fHistLambdareco_Proton_eta_bkg->SetXTitle("#eta");           fHistLambdareco_Proton_eta_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_Proton_phi_bkg = new TH1F("h_Lambda_Proton_phi_bkg", "Proton:  #phi distribution (Lambda'->#Pi#Pi)", 50, -3.14, 3.14);
    fHistLambdareco_Proton_phi_bkg->SetXTitle("#phi");           fHistLambdareco_Proton_phi_bkg->SetYTitle("Number of Entries");

    gDirectory->mkdir("Proton_Plus", "Proton(+) from Lambda' -> PPi");
    gDirectory->cd("Proton_Plus");

    fHistLambdareco_Prplus_pt_bkg = new TH1F("h_Lambda_Prplus_pt",   "Pr^{+}:  p_{#perp} (Lambda'->P#Pi)", 60, 0, 30);
    fHistLambdareco_Prplus_pt_bkg->SetXTitle("p_{#perp} (GeV)"); fHistLambdareco_Prplus_pt->SetYTitle("Number of Entries");
    fHistLambdareco_Prplus_eta_bkg = new TH1F("h_Lambda_Prplus_eta", "Pr^{+}:  #eta distribution (Lambda'->P#Pi)", 100, -5, 5);
    fHistLambdareco_Prplus_eta_bkg->SetXTitle("#eta");           fHistLambdareco_Prplus_eta->SetYTitle("Number of Entries");
    fHistLambdareco_Prplus_phi_bkg = new TH1F("h_Lambda_Prplus_phi", "Pr^{+}:  #phi distribution (Lambda'->P#Pi)", 50, -3.14, 3.14);
    fHistLambdareco_Prplus_phi_bkg->SetXTitle("#phi");           fHistLambdareco_Prplus_phi_bkg->SetYTitle("Number of Entries");

    gDirectory->cd("../");
    gDirectory->mkdir("Proton_Minus", "Antiproton from Lambda' -> PPi");
    gDirectory->cd("Proton_Minus");
    fHistLambdareco_Prminus_pt_bkg = new TH1F("h_Lambda_Prminus_pt",   "Pr^{-}:  p_{#perp} (Lambda'->P#Pi)", 60, 0, 30);
    fHistLambdareco_Prminus_pt_bkg->SetXTitle("p_{#perp} (GeV)"); fHistLambdareco_Prminus_pt_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_Prminus_eta_bkg = new TH1F("h_Lambda_Prminus_eta", "Pr^{-}:  #eta distribution (Lambda'->P#Pi)", 100, -5, 5);
    fHistLambdareco_Prminus_eta_bkg->SetXTitle("#eta");           fHistLambdareco_Prminus_eta_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_Prminus_phi_bkg = new TH1F("h_Lambda_Prminus_phi", "Pr^{-}:  #phi distribution (Lambda'->P#Pi)", 50, -3.14, 3.14);
    fHistLambdareco_Prminus_phi_bkg->SetXTitle("#phi");           fHistLambdareco_Prminus_phi_bkg->SetYTitle("Number of Entries");

    gDirectory->cd("../../../..");

    //
    // Histograms for MC truth matching (signal and background)
    gDirectory->mkdir("MC_Signal");
    gDirectory->cd("MC_Signal");
    if ( fMode == kCutBased ) {
      fHistLambdareco_MC_m   = new TH1F("h_Lambda_MC_m", "Invariant #Lambda mass (MC signal)", 2400, 0.0, 1.2);
    } else {
      fHistLambdareco_MC_m   = new TH1F("h_Lambda_MC_m", "Invariant #Lambda mass (MC signal)", 1000, 0.6, 2.5);
    }
    fHistLambdareco_MC_m->SetXTitle("m_{#Lambda} (GeV)");     fHistLambdareco_MC_m->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ppi = new TH1F("h_Lambda_MC_m_ppi", "Invariant mass w/ proton/pion hypothesis (MC signal)", 480, 0.0, 2.5);
    fHistLambdareco_MC_m_ppi->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_ppi->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_pipi = new TH1F("h_Lambda_MC_m_pipi", "Invariant mass w/ pion/pion hypothesis (MC signal)", 480, 0.0, 2.5);
    fHistLambdareco_MC_m_pipi->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_pipi->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ee = new TH1F("h_Lambda_MC_m_ee", "Invariant mass w/ ee hypothesis (MC signal)", 480, 0.0, 2.5);
    fHistLambdareco_MC_m_ee->SetXTitle("m_{#Lambda} (GeV)");  fHistLambdareco_MC_m_ee->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ppi_o = new TH1F("h_Lambda_MC_m_ppi_o", "Invariant mass w/ proton/pion hypothesis (MC signal)", 480, 0.0, 2.5);
    fHistLambdareco_MC_m_ppi_o->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_ppi_o->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_pipi_o = new TH1F("h_Lambda_MC_m_pipi_o", "Invariant mass w/ pion/pion hypothesis (MC signal)", 480, 0.0, 2.5);
    fHistLambdareco_MC_m_pipi_o->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_pipi_o->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ee_o = new TH1F("h_Lambda_MC_m_ee_o", "Invariant mass w/ ee hypothesis (MC signal)", 480, 0.0, 2.5);
    fHistLambdareco_MC_m_ee_o->SetXTitle("m_{#Lambda} (GeV)");  fHistLambdareco_MC_m_ee_o->SetYTitle("Number of Entries");
    fHistLambdareco_MC_pt  = new TH1F("h_Lambda_MC_pt", "Lambda-p_{#perp} (MC signal)", 120, 0, 60);
    fHistLambdareco_MC_pt->SetXTitle("p_{#perp} (GeV)");      fHistLambdareco_MC_pt->SetYTitle("Number of Entries");
    fHistLambdareco_MC_phi = new TH1F("h_Lambda_MC_phi", "Lambda #phi distribution (MC signal)", 128, -3.2, 3.2);
    fHistLambdareco_MC_phi->SetXTitle("#phi (rad)");          fHistLambdareco_MC_phi->SetYTitle("Number of Entries");
    fHistLambdareco_MC_eta = new TH1F("h_Lambda_MC_eta", "Lambda #eta distribution (MC signal)", 100, -3, 3);
    fHistLambdareco_MC_eta->SetXTitle("#eta");                fHistLambdareco_MC_eta->SetYTitle("Number of Entries");
    fHistLambdareco_MC_tau = new TH1F("h_Lambda_MC_tau", "Reconstructed Lambda lifetime (MC signal)", 100, 0, 3);
    fHistLambdareco_MC_tau->SetXTitle("#tau (ps)");           fHistLambdareco_MC_tau->SetYTitle("Number of Entries");
    fHistLambdareco_MC_src_div = new TH1F("h_Lambda_MC_src_div", "Angular distance of #Lambda origin and primary vertex", 300, 0.0, 0.015);
    fHistLambdareco_MC_src_div->SetXTitle("#alpha (rad)");    fHistLambdareco_MC_src_div->SetYTitle("Number of Entries");
    fHistLambdareco_MC_R_vtx = new TH1F("h_Lambda_MC_r_vtx", "Radial distance of secondary vertices", 500, 0.0, 0.3);
    fHistLambdareco_MC_R_vtx->SetXTitle("r_{#perp} (cm)");    fHistLambdareco_MC_R_vtx->SetYTitle("Number of Entries");
    fHistLambdareco_MC_tdcy_len = new TH1F("h_Lambda_MC_tdcy_len", "Transverse decay length (MC signal)", 500, 0.0, 0.3);
    fHistLambdareco_MC_tdcy_len->SetXTitle("s_{#perp} (cm)"); fHistLambdareco_MC_tdcy_len->SetYTitle("Number of Entries");
    fHistLambdareco_MC_oangle = new TH1F("h_Lambda_MC_oangle", "Opening angle of p/pi (MC signal)", 256, 0.0, 3.2);
    fHistLambdareco_MC_oangle->SetXTitle("#alpha (rad)");     fHistLambdareco_MC_oangle->SetYTitle("Number of Entries");
    fHistLambdareco_MC_dangle = new TH1F("h_Lambda_MC_dangle", "Decay angle of p/pi (MC signal)", 256, -1.0, 1.0);
    fHistLambdareco_MC_dangle->SetXTitle("cos(#alpha)");      fHistLambdareco_MC_dangle->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_dangle = new TH2F("h_lambda_m_dangle", "M vs. Decay Angle", 40, -1.0, 1.0, 480, 0.0, 1.2);
    fHistLambdareco_MC_m_dangle->SetXTitle("cos #alpha");     fHistLambdareco_MC_m_dangle->SetYTitle("m (GeV)");
    fHistLambdareco_MC_d0_Pi = new TH1F("h_Lambda_MC_d0_Pi", "D0 (track param) of #pi", 200, -1.0, 1.0);
    fHistLambdareco_MC_d0_Pi->SetXTitle("d0 (cm)");           fHistLambdareco_MC_d0_Pi->SetYTitle("Number of Entries");
    fHistLambdareco_MC_z0_Pi = new TH1F("h_Lambda_MC_z0_Pi", "Z0 (track param) of #pi", 200, -1.0, 1.0);
    fHistLambdareco_MC_z0_Pi->SetXTitle("z0 (cm)");           fHistLambdareco_MC_z0_Pi->SetYTitle("Number of Entries");
    fHistLambdareco_MC_trk_chi2 = new TH1F("h_Lambda_MC_trk_chi2", "#Chi^{2}/ndf of #pi tracks", 50, 0.0, 5.0);
    fHistLambdareco_MC_trk_chi2->SetXTitle("#Chi^{2}/ndf");   fHistLambdareco_MC_trk_chi2->SetYTitle("Number of Entries");
    fHistLambdareco_MC_pt_Pi = new TH1F("h_Lambda_MC_pt_Pi", "p_#perp of #pi", 50, 0.0, 20.0);
    fHistLambdareco_MC_pt_Pi->SetXTitle("p_#perp (GeV)");     fHistLambdareco_MC_pt_Pi->SetYTitle("Number of Entries");
    fHistLambdareco_MC_Pi_angle = new TH1F("h_Lambda_MC_Pi_angle", "Angle between the two #pi", 120, 0.0, 3.5);
    fHistLambdareco_MC_Pi_angle->SetXTitle("angle (rad)");    fHistLambdareco_MC_Pi_angle->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    gDirectory->mkdir("MC_Background");
    gDirectory->cd("MC_Background");
    if ( fMode == kCutBased ) {
      fHistLambdareco_MC_m_bkg   = new TH1F("h_Lambda_MC_m_bkg", "Invariant #Lambda mass (MC background)", 2400, 0.0, 1.2);
    } else {
      fHistLambdareco_MC_m_bkg   = new TH1F("h_Lambda_MC_m_bkg", "Invariant #Lambda mass (MC background)", 1000, 0.6, 2.5);
    }
    fHistLambdareco_MC_m_bkg->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ppi_bkg = new TH1F("h_Lambda_MC_m_ppi_bkg", "Invariant mass w/ p#pi hypothesis (MC bkg)", 480, 0.0, 2.5);
    fHistLambdareco_MC_m_ppi_bkg->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_ppi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_pipi_bkg = new TH1F("h_Lambda_MC_m_pipi_bkg", "Invariant mass w/ #pi#pi hypothesis (MC bkg)", 480, 0.0, 2.5);
    fHistLambdareco_MC_m_pipi_bkg->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_pipi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ee_bkg = new TH1F("h_Lambda_MC_m_ee_bkg", "Invariant mass w/ ee hypothesis (MC bkg)", 480, 0.0, 2.5);
    fHistLambdareco_MC_m_ee_bkg->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_ee_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ppi_o_bkg = new TH1F("h_Lambda_MC_m_ppi_o_bkg", "Invariant mass w/ p#pi hypothesis (MC bkg)", 480, 0.0, 2.5);
    fHistLambdareco_MC_m_ppi_o_bkg->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_ppi_o_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_pipi_o_bkg = new TH1F("h_Lambda_MC_m_pipi_o_bkg", "Invariant mass w/ #pi#pi hypothesis (MC bkg)", 480, 0.0, 2.5);
    fHistLambdareco_MC_m_pipi_o_bkg->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_pipi_o_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ee_o_bkg = new TH1F("h_Lambda_MC_m_ee_o_bkg", "Invariant mass w/ ee hypothesis (MC bkg)", 480, 0.0, 2.5);
    fHistLambdareco_MC_m_ee_o_bkg->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_ee_o_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_pt_bkg  = new TH1F("h_Lambda_MC_pt_bkg", "Lambda-p_{#perp} (MC background)", 120, 0, 60);
    fHistLambdareco_MC_pt_bkg->SetXTitle("p_{#perp} (GeV)");   fHistLambdareco_MC_pt_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_phi_bkg = new TH1F("h_Lambda_MC_phi_bkg", "Lambda #phi distribution (MC background)", 128, -3.2, 3.2);
    fHistLambdareco_MC_phi_bkg->SetXTitle("#phi (rad)");       fHistLambdareco_MC_phi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_eta_bkg = new TH1F("h_Lambda_MC_eta_bkg", "Lambda #eta distribution (MC background)", 100, -3, 3);
    fHistLambdareco_MC_eta_bkg->SetXTitle("#eta");             fHistLambdareco_MC_eta_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_tau_bkg = new TH1F("h_Lambda_MC_tau_bkg", "Reconstructed Lambda lifetime (MC background)", 100, 0, 3);
    fHistLambdareco_MC_tau_bkg->SetXTitle("#tau (ps)");        fHistLambdareco_MC_tau_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_src_div_bkg = new TH1F("h_Lambda_MC_src_div_bkg", "Angular distance of #Lambda origin and primary vertex", 300, 0.0, 0.015);
    fHistLambdareco_MC_src_div_bkg->SetXTitle("#alpha (rad)"); fHistLambdareco_MC_src_div_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_R_vtx_bkg = new TH1F("h_Lambda_MC_r_vtx_bkg", "Radial distance of secondary vertices", 500, 0.0, 0.3);
    fHistLambdareco_MC_R_vtx_bkg->SetXTitle("r_{#perp} (cm)"); fHistLambdareco_MC_R_vtx_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_tdcy_len_bkg = new TH1F("h_Lambda_MC_tdcy_len_bkg", "Transverse decay length (MC bkg)", 500, 0.0, 0.3);
    fHistLambdareco_MC_tdcy_len_bkg->SetXTitle("s_{#perp} (cm)"); fHistLambdareco_MC_tdcy_len_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_oangle_bkg = new TH1F("h_Lambda_MC_oangle_bkg", "Opening angle of p/pi (MC bkg)", 256, 0.0, 3.2);
    fHistLambdareco_MC_oangle_bkg->SetXTitle("#alpha (rad)");  fHistLambdareco_MC_oangle_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_dangle_bkg = new TH1F("h_Lambda_MC_dangle_bkg", "Decay angle of p/pi (MC bkg)", 256, -1.0, 1.0);
    fHistLambdareco_MC_dangle_bkg->SetXTitle("cos(#alpha)");   fHistLambdareco_MC_dangle_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_dangle_bkg = new TH2F("h_lambda_m_dangle_bkg", "M vs. Decay Angle", 40, -1.0, 1.0, 480, 0.0, 1.2);
    fHistLambdareco_MC_m_dangle_bkg->SetXTitle("cos #alpha");  fHistLambdareco_MC_m_dangle_bkg->SetYTitle("m (GeV)");
    fHistLambdareco_MC_d0_Pi_bkg = new TH1F("h_Lambda_MC_d0_Pi_bkg", "D0 (track param) of #pi", 200, -1.0, 1.0);
    fHistLambdareco_MC_d0_Pi_bkg->SetXTitle("d0 (cm)");        fHistLambdareco_MC_d0_Pi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_z0_Pi_bkg = new TH1F("h_Lambda_MC_z0_Pi_bkg", "Z0 (track param) of #pi", 200, -1.0, 1.0);
    fHistLambdareco_MC_z0_Pi_bkg->SetXTitle("z0 (cm)");        fHistLambdareco_MC_z0_Pi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_trk_chi2_bkg = new TH1F("h_Lambda_MC_trk_chi2_bkg", "#Chi^{2}/ndf of #pi tracks", 50, 0.0, 5.0);
    fHistLambdareco_MC_trk_chi2_bkg->SetXTitle("#Chi^{2}/ndf");fHistLambdareco_MC_trk_chi2_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_pt_Pi_bkg = new TH1F("h_Lambda_MC_pt_Pi_bkg", "p_#perp of #pi", 50, 0.0, 20.0);
    fHistLambdareco_MC_pt_Pi_bkg->SetXTitle("p_#perp (GeV)");  fHistLambdareco_MC_pt_Pi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_Pi_angle_bkg = new TH1F("h_Lambda_MC_Pi_angle_bkg", "Angle between the two #pi", 120, 0.0, 3.5);
    fHistLambdareco_MC_Pi_angle_bkg->SetXTitle("angle (rad)"); fHistLambdareco_MC_Pi_angle_bkg->SetYTitle("Number of Entries");

    gDirectory->cd("../Signal");

    //  Misc stuff
    // --------------
    // chi2 over ndof plots
    fHistLambdareco_chi2_vtx = new TH1F("h_Lambda_chi2_vtx", "Quality of vertex fits", 50, 0.0, 5.0);
    fHistLambdareco_chi2_vtx->SetXTitle("Chi^{2} of vertex fits"); fHistLambdareco_chi2_vtx->SetYTitle("Number of Entries");
    fHistLambdareco_chi2_vtx->GetXaxis()->SetLabelSize(0.03);
    fHistLambdareco_chi2_trk = new TH1F("h_Lambda_chi2_trk", "Quality of track fits", 50, 0.0, 5.0);
    fHistLambdareco_chi2_trk->SetXTitle("Chi^{2} of track fits");  fHistLambdareco_chi2_trk->SetYTitle("Number of Entries");
    fHistLambdareco_chi2_trk->GetXaxis()->SetLabelSize(0.03);
    fHistLambdareco_prob_mc = new TH1F("h_Lambda_prob_mc", "Quality of truth matching", 50, 0.0, 1.0);
    fHistLambdareco_prob_mc->SetXTitle("Truth matching performance"); fHistLambdareco_prob_mc->SetYTitle("Number of Entries");

}


//____________________________________________________________________

Bool_t AtlDstarFinder::AnalyzeEvent() {
    //
    // Event analysis
    //

    // Clean-up bkg list from previous event
    for (Int_t i = 0; i < fBkgLambdaDecays->GetEntries(); i++) {
	AtlDstarDecayDPi *obj = (AtlDstarDecayDPi*)fBkgLambdaDecays->At(i);
	delete obj;
    }
    fBkgLambdaDecays->Clear("C");

    //GetPreTagEvtWeight() *= GetModifiedEventWeight();
    
    if (fEvent->IsMC()) {
	// Count all dstar decays in MC events to allow computation of the efficiency.
	Int_t NDstarTrue = 0;
	for (Int_t i = 0; i < fEvent->GetN_IDTracks(); i++) {
	    AtlIDTrack *trk1 = (AtlIDTrack*)fEvent->GetIDTracks()->At(i);
	    if(  trk1->HasValidTruthMatch(fMatchingProb_min) ) {
		if( (trk1->GetMCParticle()->GetMother() != NULL) && (TMath::Abs(trk1->GetMCParticle()->GetPdgCode())== 211) 
		    && (TMath::Abs(trk1->GetMCParticle()->GetMother()->GetPdgCode())== 413) ) {
		    HepMCParticle *MCMom = trk1->GetMCParticle()->GetMother();
		    if ( (MCMom->GetN_Daughters() == 2) && (
			     (TMath::Abs(((HepMCParticle*)MCMom->GetDaughters()->At(0))->GetPdgCode()) == 421) ||
			     (TMath::Abs(((HepMCParticle*)MCMom->GetDaughters()->At(1))->GetPdgCode()) == 421) ))
			NDstarTrue++;
		}
	    }
	}
	AddAbundanceTrue(NDstarTrue);
    }

    // Do analysis
    if ( fMode == kKinFit ) {
      ReconstructLambdaKinFit();
    } else {
      Error("AnalyzeEvent", "Invalid mode given. Abort!");
      gSystem->Abort(0);
    }
    return kTRUE;
}

//____________________________________________________________________

Bool_t AtlDstarFinder::IsKaonVertex(HepVertex *vtx) {
    //
    // Return true, if the given vertex is already assigned
    // to a kaon decay. Return false otherwise or if
    // there are no kaon decays (eg. the K0s finder didn't run).
    //
    
    TClonesArray *k0s = fEvent->GetK0sDecaysPiPi();
    for (Int_t i = 0; i < fEvent->GetN_K0sDecaysPiPi(); i++) {
	if (((AtlK0sDecayPiPi*)k0s->At(i))->GetVtx() == vtx)
	    return kTRUE;
    }
    TLorentzVector p1, p2, p;
    p1.SetVectM(((AtlIDTrack*)vtx->GetDaughters()->At(0))->P(), fm_pi);
    p2.SetVectM(((AtlIDTrack*)vtx->GetDaughters()->At(1))->P(), fm_pi);
    p = p1 + p2;
    if ((p.M() < 0.45) || ((p.M() > 0.492) && (p.M() < 0.503)))
	return kTRUE;
    return kFALSE;
}

//____________________________________________________________________

Bool_t AtlDstarFinder::IsLambdaVertex(HepVertex *vtx) {
    //
    // Return true, if the given vertex is already assigned
    // to a kaon decay. Return false otherwise or if
    // there are no kaon decays (eg. the K0s finder didn't run).
    //
    
    if (fEvent->GetN_LambdaDecaysPiPi() < 1) {
	return kFALSE;
    }

    TClonesArray *lambda = fEvent->GetLambdaDecaysPiPi();
    for (Int_t i = 0; i < fEvent->GetN_LambdaDecaysPiPi(); i++) {
	if (((AtlLambdaDecayPPi*)lambda->At(i))->GetVtx() == vtx)
	    return kTRUE;
    }
    return kFALSE;
}

//____________________________________________________________________

Bool_t AtlDstarFinder::IsConversionVertex(HepVertex *vtx) {
    //
    // Return true, if the given vertex was already being tagged
    // as a photo conversion vertex. Return false otherwise or if
    // there are no conversion vertices.
    //
   
    AtlIDTrack *trk1 = (AtlIDTrack*)vtx->GetDaughters()->At(0);
    AtlIDTrack *trk2 = (AtlIDTrack*)vtx->GetDaughters()->At(1);
    TLorentzVector p_e1, p_e2, p_g;
    p_e1.SetVectM(trk1->P(), fm_e);
    p_e2.SetVectM(trk2->P(), fm_e);
    p_g = p_e1 + p_e2;
    return (p_g.M() < fPhotonMass_max);
}


//____________________________________________________________________

void AtlDstarFinder::ReconstructLambdaKinFit() {
  //
  // Lambda reconstruction from track-pairs assuming being pions using KinFitter
  //
  HepVertex  *PrimVtx = 0;
  AtlD0DecayKPi *D0 = 0, *D02 = 0;
  AtlIDTrack *trk1 = 0;  
  AtlIDTrack *trk2 = 0;
  AtlIDTrack *trk3 = 0;
    
  TLorentzVector p_Dstar;
  TLorentzVector p_D0;
    
  TMatrixD cov_trk1(3,3);
  TMatrixD cov_trk2(3,3);
  TMatrixD cov_trk3(3,3);
  
  Double_t fChi2;
  Int_t fNDoF;

//  Int_t BkgCount = 1;  // Counts background lambdas (with equally charged decay products)
  Float_t PvalueKinFit = 0.0;

  // Get primary vertex
  PrimVtx = fEvent->GetPrimaryVtx();
  if (PrimVtx == NULL) return;

  // Vertex-Loop
  for ( Int_t i = 0; i < fEvent->GetN_D0DecaysKPi(); i++) {
    
      D0   = (AtlD0DecayKPi*)fEvent->GetD0DecaysKPi()->At(i);
      if (D0->HasSharedTracks()) continue;
      trk1 = D0->GetKaon();
      trk2 = D0->GetPion();

      for ( Int_t j = 0; j < fEvent->GetN_IDTracks(); j++) {

	trk3 = (AtlIDTrack*)fEvent->GetIDTracks()->At(j);
	Bool_t isSharedPion = kFALSE;
	for ( Int_t k = 0; k < fEvent->GetN_D0DecaysKPi(); k++) {
	    D02 = (AtlD0DecayKPi*)fEvent->GetD0DecaysKPi()->At(i);
	    if ((D02->GetKaon() == trk3) || (D02->GetPion() == trk3)) {
		isSharedPion = kTRUE;
		break;
	    }
	}
	if (isSharedPion) continue;

	if ( (trk3->GetD0() < fD0_Pi_min) || (trk3->GetD0() > fD0_Pi_max) ) continue;
	
	if ( trk3->GetVertex() != PrimVtx ) continue;
	if ( trk3->Chi2ovNDoF() > fTrackChi2ovNDoF_max ) continue;
	SetCutFlow("Trk #chi^{2}");
	if ( ! trk3->HasValidTruthMatch(fMatchingProb_min) ) continue;
	SetCutFlow("MC exists");
	if ( trk3->QovP() * trk2->QovP() < 0.0 ) continue;
	SetCutFlow("Pions' charge");

	TLorentzVector p1, p2, p3, p;
	p1.SetVectM(D0->GetKaon()->P(), fm_kaon);
	p2.SetVectM(D0->GetPion()->P(), fm_pi);
	p3.SetVectM(trk3->P(),          fm_pi);
	p = p1 + p2 + p3;

	if ( (p.M() < fDstar_M_min) || (p.M() > fDstar_M_max)) continue;
	SetCutFlow("Mass wnd");

	// Definition of covariance matrices, taken from track fit
	D0->GetKaon()->GetCovMatrixPtEtaPhi(cov_trk1);
	D0->GetPion()->GetCovMatrixPtEtaPhi(cov_trk2);
	trk3->GetCovMatrixPtEtaPhi(cov_trk3);

	// Define momentum used as output of the fit
	TLorentzVector FitP_trk1k;
	TLorentzVector FitP_trk2pi;
	TLorentzVector FitP_trk3pi;
	TKinFitter fitter = PerformFit(D0->GetReFitKaon(), D0->GetReFitPion(), trk3, FitP_trk1k, FitP_trk2pi, FitP_trk3pi, &cov_trk1, &cov_trk2, &cov_trk3);
	
	// abort, if a fit failed severely (chi2 < 0)
	if ( fitter.getS() < 0. ) {
	    Error("ReconstructLambdaKinFit", "fitter.getS()<0. Abort!");
	    gSystem->Abort(0);
	}
	if ( fitter.getStatus() != 0 ) continue;
	SetCutFlow("Fit ok");
	fN_Fits++;

	p_D0    = FitP_trk1k + FitP_trk2pi;
	p_Dstar = p_D0 + FitP_trk3pi;

	fChi2 = fitter.getS();
	fNDoF = fitter.getNDF();
	PvalueKinFit = TMath::Prob(fChi2, fNDoF);

	HepParticle Fit_Daughter1(1, FitP_trk1k.Px(), FitP_trk1k.Py(), FitP_trk1k.Pz(), 
				 FitP_trk1k.E(),  (trk1->GetQovP() < 0.) ? -321 : 321);
	HepParticle Fit_Daughter2(2, FitP_trk2pi.Px(), FitP_trk2pi.Py(), FitP_trk2pi.Pz(), 
				 FitP_trk2pi.E(), (trk2->GetQovP() < 0.) ? -211 : 211);  
	HepParticle Fit_Daughter3(3, FitP_trk3pi.Px(), FitP_trk3pi.Py(), FitP_trk3pi.Pz(), 
				 FitP_trk3pi.E(), (trk3->GetQovP() < 0.) ? -211 : 211);

	if ((p_Dstar.Eta() < fSignal_Eta_min) || (p_Dstar.Eta() > fSignal_Eta_max))
	    continue;
	SetCutFlow("Dstar Eta");

	if ( TMath::Abs( p_D0.Angle(FitP_trk3pi.Vect())  ) > fOAngle_max )
	    continue;
	SetCutFlow("OAngle");

	if ((p_Dstar.Pt() < fSignal_Pt_min) || (p_Dstar.Pt() > fSignal_Pt_max))
	    continue;
	SetCutFlow("Dstar Pt");

	if ( FitP_trk3pi.Pt() < fPi3_pt_max ) continue;
	SetCutFlow("pt cut");

	// Add Lambda to event
	if (PvalueKinFit >= fKinFitPvalue_min) {
	    SetCutFlow("KinFit P-value");
	    
	    AtlDstarDecayDPi *decay;    
	    decay = fEvent->AddDstarDecayDPi(p_Dstar.Px(), p_Dstar.Py(), p_Dstar.Pz(), p_Dstar.E(),
					     D0, trk3, PrimVtx, PrimVtx, Fit_Daughter1, Fit_Daughter3);
	    decay->SetChi2NDoF(fChi2, fNDoF);
		
	    if ( IsDebugRun() ) {
		fDebugStream  << endl << "=========================================="
			      << endl << "Entry: " << fTree->GetReadEvent() << " Prob: " << PvalueKinFit;
		if (trk1->GetMCParticle()->GetMother() != NULL)
		    fDebugStream << endl << "Mother1: " << trk1->GetMCParticle()->GetMother()->GetPdgName()
				 << " (" << trk1->GetMCParticle()->GetMother()->GetPdgCode() << ")";
		if (trk2->GetMCParticle()->GetMother() != NULL)
		    fDebugStream << endl << "Mother2: " << trk2->GetMCParticle()->GetMother()->GetPdgName()
				 << " (" << trk2->GetMCParticle()->GetMother()->GetPdgCode() << ") ";
		if (trk3->GetMCParticle()->GetMother() != NULL)
		    fDebugStream << endl << "Mother3: " << trk3->GetMCParticle()->GetMother()->GetPdgName()
				 << " (" << trk3->GetMCParticle()->GetMother()->GetPdgCode() << ")";
		fDebugStream  << endl << "Track1: " << trk1->GetMCParticle()->GetPdgName() << " (" << trk1->GetMCParticle()->GetPdgCode() << ")"
			      << endl << "Track2: " << trk2->GetMCParticle()->GetPdgName() << " (" << trk2->GetMCParticle()->GetPdgCode() << ")"
			      << endl << "Track3: " << trk3->GetMCParticle()->GetPdgName() << " (" << trk3->GetMCParticle()->GetPdgCode() << ")";
	    }
	    
	   
	}
      }
  } //end of vertex loop

  FindSharedTracks();
}

//____________________________________________________________________    

void AtlDstarFinder::FindSharedTracks() {

    AtlDstarDecayDPi *decay, *decay2;
    
    for ( Int_t i = 0; i < fEvent->GetN_DstarDecaysDPi(); i++ ) {
	decay = (AtlDstarDecayDPi*)fEvent->GetDstarDecaysDPi()->At(i);

	for (Int_t j = i+1; j < fEvent->GetN_DstarDecaysDPi(); j++) {
	    decay2 = (AtlDstarDecayDPi*)fEvent->GetDstarDecaysDPi()->At(j);
	    if (decay->GetPion() == decay2->GetPion()) {
		decay->SetSharedTracks(kTRUE);
		decay2->SetSharedTracks(kTRUE);
	    }
	}
    }
}


//____________________________________________________________________    

TKinFitter AtlDstarFinder::PerformFit(HepParticle trk1, HepParticle trk2, AtlIDTrack *trk3,
				      TLorentzVector& FitP_trk1, TLorentzVector& FitP_trk2, TLorentzVector& FitP_trk3,
				      TMatrixD *cov_trk1, TMatrixD *cov_trk2, TMatrixD *cov_trk3) {
  //
  // Perform a kinematic fit with the given tracks to test the lambda mass hyptothesis
  // assuming that track1 is the proton and track2 is the pion.
  //

  // Define particles necessary for execution of the fit
  // Using proton/pion mass hypothesis; fit variables are pt, eta, phi
  TVector3 dummy1 = trk1.P3(); // instead of const cast
  TFitParticlePtEtaPhi FitExec_trk1("FitExec_trk1", "FitExec_trk1 for LambdaFit", &dummy1, fm_kaon, cov_trk1);
  TVector3 dummy2 = trk2.P3(); // instead of const cast
  TFitParticlePtEtaPhi FitExec_trk2("FitExec_trk2", "FitExec_trk2 for LambdaFit", &dummy2, fm_pi, cov_trk2);
  TVector3 dummy3 = trk3->P(); // instead of const cast
  TFitParticlePtEtaPhi FitExec_trk3("FitExec_trk3", "FitExec_trk3 for LambdaFit", &dummy3, fm_pi, cov_trk3);
  
  // Definition of D* mass constraint
  TFitConstraintM MDstarCons("DstarMassConstraint","DstarMassConstraint", 0, 0, fm_Dstar);
  MDstarCons.addParticles1(&FitExec_trk1, &FitExec_trk2, &FitExec_trk3);
  // Definition of D0 mass constraint
  TFitConstraintM MD0Cons("D0MassConstraint", "D0MassConstrant", 0, 0, fm_D0);
  MD0Cons.addParticles1(&FitExec_trk1, &FitExec_trk2);
    
  // fitting characteristics
  TKinFitter fitter;
  fitter.addMeasParticle(&FitExec_trk1);
  fitter.addMeasParticle(&FitExec_trk2);
  fitter.addMeasParticle(&FitExec_trk3);
  fitter.addConstraint(&MD0Cons);
  fitter.addConstraint(&MDstarCons);
    
  fitter.setMaxNbIter(50);   // maximum number of iterations
  fitter.setMaxDeltaS(5e-5); // maximum deviation of the minimum function within two iterations
  fitter.setMaxF(1e-4);      // maximum value of constraints
  fitter.setVerbosity(0);    // verbosity level
    
  // Kinematic Fitting
  fitter.fit();

  // Copy fitted 4-vectors (circumvent const cast)
  FitP_trk1 = *FitExec_trk1.getCurr4Vec();
  FitP_trk2 = *FitExec_trk2.getCurr4Vec();
  FitP_trk3 = *FitExec_trk3.getCurr4Vec();

  return fitter;
}


//____________________________________________________________________    

void AtlDstarFinder::FillMCHistograms(AtlDstarDecayDPi *decay, Bool_t signal) {
    //
    // Fill histograms for MC signal/background comparison.
    // Set signal to false to fill background histograms.
    //

/*    TLorentzVector p_trk1, p_trk2, p_ee, p_ppi, p_pipi, p_ee_o, p_ppi_o, p_pipi_o;
    p_trk1.SetVectM(decay->GetReFitProton().P().Vect(), fm_e);
    p_trk2.SetVectM(decay->GetReFitPion().P().Vect(),   fm_e);
    p_ee = p_trk1 + p_trk2;
    p_trk1.SetVectM(decay->GetReFitProton().P().Vect(), fm_pi);
    p_trk2.SetVectM(decay->GetReFitPion().P().Vect(),   fm_pi);
    p_pipi = p_trk1 + p_trk2;
    p_trk1.SetVectM(decay->GetReFitProton().P().Vect(), fm_proton);
    p_ppi = p_trk1 + p_trk2;

    p_trk1.SetVectM(decay->GetProton()->P(), fm_e);
    p_trk2.SetVectM(decay->GetPion()->P(), fm_e);
    p_ee_o = p_trk1 + p_trk2;
    p_trk1.SetVectM(decay->GetProton()->P(), fm_pi);
    p_trk2.SetVectM(decay->GetPion()->P(),   fm_pi);
    p_pipi_o = p_trk1 + p_trk2;
    p_trk1.SetVectM(decay->GetProton()->P(), fm_proton);
    p_ppi_o = p_trk1 + p_trk2;*/

    if (signal) {
	fHistLambdareco_MC_m       ->Fill(decay->M("Reco"),     GetPreTagEvtWeight());
	fHistLambdareco_MC_pt      ->Fill(decay->Pt(),          GetPreTagEvtWeight());
	fHistLambdareco_MC_phi     ->Fill(decay->Phi(),         GetPreTagEvtWeight());
	fHistLambdareco_MC_eta     ->Fill(decay->Eta(),         GetPreTagEvtWeight());
	fHistLambdareco_MC_tau     ->Fill(decay->GetLifeTime(), GetPreTagEvtWeight());
/*   	fHistLambdareco_MC_m_ee    ->Fill(p_ee.M(),             GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_pipi  ->Fill(p_pipi.M(),           GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_ppi   ->Fill(p_ppi.M(),            GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_ee_o  ->Fill(p_ee_o.M(),           GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_pipi_o->Fill(p_pipi_o.M(),         GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_ppi_o ->Fill(p_ppi_o.M(),          GetPreTagEvtWeight());*/
	fHistLambdareco_MC_R_vtx   ->Fill(decay->GetVtx()->GetPos().Perp(),   GetPreTagEvtWeight());
	fHistLambdareco_MC_src_div ->Fill(decay->GetAngleToPrimary(),         GetPreTagEvtWeight());
	fHistLambdareco_MC_tdcy_len->Fill(decay->GetTransvDecayLength(),      GetPreTagEvtWeight());
	fHistLambdareco_MC_oangle  ->Fill(decay->GetOpeningAngle(),           GetPreTagEvtWeight());
	fHistLambdareco_MC_dangle  ->Fill(TMath::Cos(decay->GetDecayAngle()), GetPreTagEvtWeight());
	fHistLambdareco_MC_m_dangle->Fill(TMath::Cos(decay->GetDecayAngle()), decay->M("Reco"), GetPreTagEvtWeight());
	fHistLambdareco_MC_d0_Pi   ->Fill(decay->GetPion()->GetD0(),          GetPreTagEvtWeight());
	fHistLambdareco_MC_z0_Pi   ->Fill(decay->GetPion()->GetZ0(),          GetPreTagEvtWeight());
	fHistLambdareco_MC_trk_chi2->Fill(decay->GetPion()->GetChi2() / decay->GetPion()->GetNDoF(), GetPreTagEvtWeight());
	fHistLambdareco_MC_pt_Pi   ->Fill(decay->GetPion()->Pt(),             GetPreTagEvtWeight());
	fHistLambdareco_MC_Pi_angle->Fill(decay->GetPion()->P().Angle(decay->GetPion()->P()), GetPreTagEvtWeight());
    } else {
	fHistLambdareco_MC_m_bkg       ->Fill(decay->M("Reco"),     GetPreTagEvtWeight());
	fHistLambdareco_MC_pt_bkg      ->Fill(decay->Pt(),          GetPreTagEvtWeight());
	fHistLambdareco_MC_phi_bkg     ->Fill(decay->Phi(),         GetPreTagEvtWeight());
	fHistLambdareco_MC_eta_bkg     ->Fill(decay->Eta(),         GetPreTagEvtWeight());
	fHistLambdareco_MC_tau_bkg     ->Fill(decay->GetLifeTime(), GetPreTagEvtWeight());
/*   	fHistLambdareco_MC_m_ee_o_bkg  ->Fill(p_ee_o.M(),           GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_pipi_o_bkg->Fill(p_pipi_o.M(),         GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_ppi_o_bkg ->Fill(p_ppi_o.M(),          GetPreTagEvtWeight());*/
	fHistLambdareco_MC_R_vtx_bkg   ->Fill(decay->GetVtx()->GetPos().Perp(),   GetPreTagEvtWeight());
	fHistLambdareco_MC_src_div_bkg ->Fill(decay->GetAngleToPrimary(),         GetPreTagEvtWeight());
	fHistLambdareco_MC_tdcy_len_bkg->Fill(decay->GetTransvDecayLength(),      GetPreTagEvtWeight());
	fHistLambdareco_MC_oangle_bkg  ->Fill(decay->GetOpeningAngle(),           GetPreTagEvtWeight());
	fHistLambdareco_MC_dangle_bkg  ->Fill(TMath::Cos(decay->GetDecayAngle()), GetPreTagEvtWeight());
	fHistLambdareco_MC_m_dangle_bkg->Fill(TMath::Cos(decay->GetDecayAngle()), decay->M("Reco"), GetPreTagEvtWeight());
	fHistLambdareco_MC_d0_Pi_bkg   ->Fill(decay->GetPion()->GetD0(),          GetPreTagEvtWeight());
	fHistLambdareco_MC_z0_Pi_bkg   ->Fill(decay->GetPion()->GetZ0(),          GetPreTagEvtWeight());
	fHistLambdareco_MC_trk_chi2_bkg->Fill(decay->GetPion()->GetChi2() / decay->GetPion()->GetNDoF(), GetPreTagEvtWeight());
	fHistLambdareco_MC_pt_Pi_bkg   ->Fill(decay->GetPion()->Pt(),             GetPreTagEvtWeight());
	fHistLambdareco_MC_Pi_angle_bkg->Fill(decay->GetPion()->P().Angle(decay->GetPion()->P()), GetPreTagEvtWeight());
    }    
}


//____________________________________________________________________    


void AtlDstarFinder::FillHistograms() {
    //
    // Fill histograms
    //

    AtlDstarDecayDPi *Lambda = 0;
    
    // Proton/Pion variables
//    AtlIDTrack *Proton =0;
    AtlIDTrack *Pion   =0;
    TLorentzVector p_trk1, p_trk2, p_Lambda;

    // Signal (opposite charged)
    // ________________________________________
    
    for ( Int_t i = 0; i < fEvent->GetDstarDecaysDPi()->GetEntriesFast(); i++ ) {
      Lambda = (AtlDstarDecayDPi*)fEvent->GetDstarDecaysDPi()->At(i);
      if (Lambda->HasSharedTracks()) continue;
      SetCutFlow("Shared tracks");

      // Hitbased TruthMatching
      SetChi2( Lambda->GetChi2() );
      SetNDoF( Lambda->GetNDoF() );
      if (fEvent->IsMC()) {
	  switch ( DoTruthMatch(Lambda->GetD0()->GetKaon(), Lambda->GetD0()->GetPion(), Lambda->GetPion()) ){
	      case 1:   fTrueReco = kTRUE;  FillMCHistograms(Lambda, kTRUE);  AtlKinFitterTool::FillHistograms("Signal"); break;
	      case -1:  fTrueReco = kFALSE; FillMCHistograms(Lambda, kFALSE); AtlKinFitterTool::FillHistograms("Signal"); break;
	  }
      } else {
	  AtlKinFitterTool::FillHistograms("Signal");
      }
      
      fHistLambdareco_m_PPi     ->Fill(Lambda->M("Reco"),    GetPreTagEvtWeight());
      fHistLambdareco_pt_PPi    ->Fill(Lambda->Pt(),         GetPreTagEvtWeight());
      fHistLambdareco_phi_PPi   ->Fill(Lambda->Phi(),        GetPreTagEvtWeight());
      fHistLambdareco_eta_PPi   ->Fill(Lambda->Eta(),        GetPreTagEvtWeight());
      fHistLambdareco_tau_PPi   ->Fill(Lambda->GetLifeTime(),             GetPreTagEvtWeight());
      fHistLambdareco_R_vtx     ->Fill(Lambda->GetVtx()->GetPos().Perp(), GetPreTagEvtWeight());
      fHistLambdareco_tdcy_len  ->Fill(Lambda->GetTransvDecayLength(), GetPreTagEvtWeight());
      fHistLambdareco_oangle    ->Fill(Lambda->GetOpeningAngle(),      GetPreTagEvtWeight());
      fHistLambdareco_dangle    ->Fill(Lambda->GetDecayAngle(),        GetPreTagEvtWeight());

//      Proton = Lambda->GetProton();
      Pion  =  Lambda->GetPion();

/*      // test pi/pi hypothesis
      p_trk1.SetVectM(Proton->P(), fm_pi);
      p_trk2.SetVectM(Pion->P(),   fm_pi);
      p_Lambda = p_trk1 + p_trk2;
      fHistLambdareco_m_PiPi->Fill(p_Lambda.M(), GetPreTagEvtWeight());

      // test e/e hypothesis
      p_trk1.SetVectM(Proton->P(), fm_e);
      p_trk2.SetVectM(Pion->P(),   fm_e);
      p_Lambda = p_trk1 + p_trk2;
      fHistLambdareco_m_ee->Fill(p_Lambda.M(), GetPreTagEvtWeight());*/

      fHistLambdareco_chi2_vtx->Fill(Lambda->GetVtx()->GetChi2overNDoF(), GetPreTagEvtWeight());
//      fHistLambdareco_chi2_trk->Fill(Proton->Chi2ovNDoF(), GetPreTagEvtWeight());
      fHistLambdareco_chi2_trk->Fill(Pion->Chi2ovNDoF(), GetPreTagEvtWeight());

//      fHistLambdareco_Pion_pt     ->Fill(Proton->Pt(),  GetPreTagEvtWeight());
      fHistLambdareco_Proton_pt   ->Fill(Pion->Pt(),    GetPreTagEvtWeight());
//      fHistLambdareco_Pion_eta    ->Fill(Proton->Eta(), GetPreTagEvtWeight());
      fHistLambdareco_Proton_eta  ->Fill(Pion->Eta(),   GetPreTagEvtWeight());
//      fHistLambdareco_Pion_phi    ->Fill(Proton->Phi(), GetPreTagEvtWeight());
      fHistLambdareco_Proton_phi  ->Fill(Pion->Phi(),   GetPreTagEvtWeight());
      // charge separated histograms
      if (Pion->QovP() < 0.) {
        fHistLambdareco_Piminus_pt  ->Fill(Pion->Pt(),    GetPreTagEvtWeight());
        fHistLambdareco_Piminus_eta ->Fill(Pion->Eta(),   GetPreTagEvtWeight());
        fHistLambdareco_Piminus_phi ->Fill(Pion->Phi(),   GetPreTagEvtWeight());
//        fHistLambdareco_Prplus_pt   ->Fill(Proton->Pt(),  GetPreTagEvtWeight());
//        fHistLambdareco_Prplus_eta  ->Fill(Proton->Eta(), GetPreTagEvtWeight());
//        fHistLambdareco_Prplus_phi  ->Fill(Proton->Phi(), GetPreTagEvtWeight());
      } else {
        fHistLambdareco_Piplus_pt   ->Fill(Pion->Pt(),    GetPreTagEvtWeight());
        fHistLambdareco_Piplus_eta  ->Fill(Pion->Eta(),   GetPreTagEvtWeight());
        fHistLambdareco_Piplus_phi  ->Fill(Pion->Phi(),   GetPreTagEvtWeight());
//        fHistLambdareco_Prminus_pt  ->Fill(Proton->Pt(),  GetPreTagEvtWeight());
//        fHistLambdareco_Prminus_eta ->Fill(Proton->Eta(), GetPreTagEvtWeight());
//        fHistLambdareco_Prminus_phi ->Fill(Proton->Phi(), GetPreTagEvtWeight());
      }
    }

    fHistLambdareco_N_PPi ->Fill(fEvent->GetN_DstarDecaysDPi(), GetPreTagEvtWeight());
    

    // Combinatorial Background (same charged)
    // ____________________________________________

    TIter next_bkg(fBkgLambdaDecays);
    while ( ( Lambda = (AtlDstarDecayDPi*)next_bkg() ) ) {
      fHistLambdareco_m_PPi_bkg     ->Fill(Lambda->M("Reco"),      GetPreTagEvtWeight());
      fHistLambdareco_pt_PPi_bkg    ->Fill(Lambda->Pt(),           GetPreTagEvtWeight());
      fHistLambdareco_phi_PPi_bkg   ->Fill(Lambda->Phi(),          GetPreTagEvtWeight());
      fHistLambdareco_eta_PPi_bkg   ->Fill(Lambda->Eta(),          GetPreTagEvtWeight());
      fHistLambdareco_tau_PPi_bkg   ->Fill(Lambda->GetLifeTime(),  GetPreTagEvtWeight());
      fHistLambdareco_R_vtx_bkg     ->Fill(Lambda->GetVtx()->GetPos().Perp(), GetPreTagEvtWeight());
      fHistLambdareco_tdcy_len_bkg  ->Fill(Lambda->GetTransvDecayLength(),    GetPreTagEvtWeight());
      fHistLambdareco_oangle_bkg    ->Fill(Lambda->GetOpeningAngle(),         GetPreTagEvtWeight());
      fHistLambdareco_dangle_bkg    ->Fill(Lambda->GetDecayAngle(),           GetPreTagEvtWeight());

//      Proton = Lambda->GetProton();
      Pion  =  Lambda->GetPion();
      
//      fHistLambdareco_Pion_pt_bkg     ->Fill(Proton->Pt(),  GetPreTagEvtWeight());
      fHistLambdareco_Proton_pt_bkg   ->Fill(Pion->Pt(),    GetPreTagEvtWeight());
      //    fHistLambdareco_Pion_eta_bkg    ->Fill(Proton->Eta(), GetPreTagEvtWeight());
      fHistLambdareco_Proton_eta_bkg  ->Fill(Pion->Eta(),   GetPreTagEvtWeight());
//      fHistLambdareco_Pion_phi_bkg    ->Fill(Proton->Phi(), GetPreTagEvtWeight());
      fHistLambdareco_Proton_phi_bkg  ->Fill(Pion->Phi(),   GetPreTagEvtWeight());
      // charge separated histograms
      if (Pion->QovP() < 0.) {
        fHistLambdareco_Piminus_pt_bkg  ->Fill(Pion->Pt(),    GetPreTagEvtWeight());
        fHistLambdareco_Piminus_eta_bkg ->Fill(Pion->Eta(),   GetPreTagEvtWeight());
        fHistLambdareco_Piminus_phi_bkg ->Fill(Pion->Phi(),   GetPreTagEvtWeight());
//        fHistLambdareco_Prminus_pt_bkg  ->Fill(Proton->Pt(),  GetPreTagEvtWeight());
//        fHistLambdareco_Prminus_eta_bkg ->Fill(Proton->Eta(), GetPreTagEvtWeight());
//        fHistLambdareco_Prminus_phi_bkg ->Fill(Proton->Phi(), GetPreTagEvtWeight());
      } else {
        fHistLambdareco_Piplus_pt_bkg   ->Fill(Pion->Pt(),    GetPreTagEvtWeight());
        fHistLambdareco_Piplus_eta_bkg  ->Fill(Pion->Eta(),   GetPreTagEvtWeight());
        fHistLambdareco_Piplus_phi_bkg  ->Fill(Pion->Phi(),   GetPreTagEvtWeight());
//        fHistLambdareco_Prplus_pt_bkg   ->Fill(Proton->Pt(),  GetPreTagEvtWeight());
//        fHistLambdareco_Prplus_eta_bkg  ->Fill(Proton->Eta(), GetPreTagEvtWeight());
//        fHistLambdareco_Prplus_phi_bkg  ->Fill(Proton->Phi(), GetPreTagEvtWeight());
      }
    }

    fHistLambdareco_N_PPi_bkg ->Fill(fBkgLambdaDecays->GetEntries(), GetPreTagEvtWeight());
}

//____________________________________________________________________

void AtlDstarFinder::SetCutDefaults() {
    //
    // Selection cut defaults and Constraint parameters
    //

    fm_lambda            = 1.12;           // Lambda mass hypothesis
    fm_pi                = 0.13957;        // Pion mass hypothesis
    fm_proton            = 0.935;          // Proton mass hypothesis
    fm_e                 = 0.000510998910; // Electron mass hypothesis
    fm_kaon              = 0.493667;
    fm_D0                = 1.86484;
    fm_Dstar             = 2.01027;
}

//____________________________________________________________________


void AtlDstarFinder::PrintCutValues() {
        
  if ( fMode == kKinFit ){
    cout << endl 
         << " Used mode: kKinFit " << endl
         << " ----------------------" << endl;
    cout << " Maximum inv. mass of e+/e- test:            " << fPhotonMass_max << endl;
    cout << " Minimum probability value of kinematic fit: " << fKinFitPvalue_min << endl;
    cout << " Minimum truth matching probability:         " << fMatchingProb_min << endl;
    cout << scientific << " maximum Chi2/NDoF of Vertex Fit: " << endl
         << "  Chi2/NDoF_Vtx = " << fVertexChi2ovNDoF_max << endl;
    cout << scientific << " maximum Chi2/NDoF of Track Fit: " << endl
         << "  Chi2/NDoF_Trk =" << fTrackChi2ovNDoF_max << endl;
    cout << " ----------------------" << endl << endl;
  } else if (fMode == kCutBased)  {
    cout << endl 
         << " Used mode: kCutBased " << endl;  
    cout << " ----------------------------------------------" << endl;
    cout << " Used CutDefaults:" <<endl;
    cout << " ----------------------------------------------" << endl;
    
    cout << scientific << setprecision(3) << " " << fSignal_Pt_min << " <   Pt   < " << fSignal_Pt_max << endl;
    cout << scientific << fSignal_Eta_min << " <   Eta  < " << fSignal_Eta_max << endl;
    cout << scientific << " " << fDstar_M_min << " < M(Lbd) < " << fDstar_M_max << endl;
    cout << scientific << " Pion mass hypothesis: " << endl
         << "           M_Pi = " << fm_pi << endl;
    cout << scientific << " Proton mass hypothesis: " << endl
         << "       M_Proton = " << fm_proton << endl;
    cout << scientific << " maximum Chi2/NDoF of Vertex Fit: " << endl
         << "  Chi2/NDoF_Vtx = " << fVertexChi2ovNDoF_max << endl;
    cout << "----------------------------------------------" << endl;
  }
}


//____________________________________________________________________


void AtlDstarFinder::Terminate() {

  AtlKinFitterTool::Terminate();

  // print truth matching statistics
  fHistLambdareco_truth_PPi->LabelsDeflate("X");
  fHistLambdareco_truth_PPi->SetStats(kFALSE);
  fHistLambdareco_truth_PPi->GetXaxis()->SetLabelSize(0.03);
  fHistLambdareco_truth_PPi->GetXaxis()->LabelsOption("v");

  cout << endl << " ----------------------------------------" << endl;
  for (int i = 1; i < fHistLambdareco_truth_PPi->GetNbinsX(); i++) {
      Double_t binContent = 0;
      for (int j = 1; j < fHistLambdareco_truth_PPi->GetNbinsY(); j++)
	  binContent += fHistLambdareco_truth_PPi->GetBinContent(i, j);
      if ((binContent > 0) && (strncmp(fHistLambdareco_truth_PPi->GetXaxis()->GetBinLabel(i), "", 2) != 0)) {
          cout.width(15);
          cout << fHistLambdareco_truth_PPi->GetXaxis()->GetBinLabel(i);
          cout << ": " << TMath::Nint(binContent) << endl;
      }
  }

  cout << endl << "INFO: Terminating AtlDstarFinder!";
  cout << endl << "        # Successful fits: " << fN_Fits;
  cout << endl << "        # Truth match bad: " << fN_MCProbMiss;
  cout << endl << "   # TM with diff. origin: " << fN_SameOriginMiss;
  cout << endl << "     # TM => wrong tracks: " << fN_MCFail;
  cout << endl << " # Possible Lambdas found: " << TMath::Nint(fHistLambdareco_m_PPi->GetEntries());
    
}

//____________________________________________________________________

Int_t AtlDstarFinder::DoTruthMatch(AtlIDTrack *trk1, AtlIDTrack *trk2, AtlIDTrack *trk3) {
    //
    // retuns 1 in case of true Lambdas, -1 if not
    // returns 0 if no decision possible (see fMatchingProb_min)
    //

    fHistLambdareco_prob_mc->Fill(trk1->GetMCParticleProb(), GetPreTagEvtWeight());
    fHistLambdareco_prob_mc->Fill(trk2->GetMCParticleProb(), GetPreTagEvtWeight());
    fHistLambdareco_prob_mc->Fill(trk3->GetMCParticleProb(), GetPreTagEvtWeight());

    // perform truth matching when analyzing MC samples
    if ( trk1->HasValidTruthMatch(fMatchingProb_min) && trk2->HasValidTruthMatch(fMatchingProb_min) && trk3->HasValidTruthMatch(fMatchingProb_min)) {
	
	HepMCParticle *MC1 = trk1->GetMCParticle();
	HepMCParticle *MC2 = trk2->GetMCParticle();
	HepMCParticle *MC3 = trk3->GetMCParticle();

	// Perform null pointer check on mother particle. Yes, there are events with MC particles without a mother!
	if ((MC1->GetMother() == NULL) || (MC2->GetMother() == NULL) || (MC3->GetMother() == NULL) ) {
	    return 0;
	}

	// remove "_bar" from particle name to collect antiparticle and particle in the same bin
	const char* fullPdgName = MC3->GetMother()->GetPdgName();
	char pdgName[strlen(fullPdgName)];
	if (strstr(fullPdgName, "_bar") == NULL) {
	    strcpy(pdgName, fullPdgName);
	} else {
	    strncpy(pdgName, fullPdgName, strlen(fullPdgName)-4);
	    pdgName[strlen(fullPdgName)-4] = '\0';
	}

	// decide truth match
	Float_t prob = ((AtlDstarDecayDPi*)fEvent->GetDstarDecaysDPi()->Last())->GetProb();
	if ( (TMath::Abs(MC1->GetMother()->GetPdgCode()) == 421) &&
	     (MC1->GetMother() == MC2->GetMother()) &&
	     (TMath::Abs(MC1->GetPdgCode()) == 321) &&
	     (TMath::Abs(MC2->GetPdgCode()) == 211) &&
	     (MC1->GetMother()->GetMother() == MC3->GetMother()) &&
	     (TMath::Abs(MC3->GetPdgCode()) == 211) &&
	     (TMath::Abs(MC3->GetMother()->GetPdgCode()) == 413)) {

	    fHistLambdareco_truth_PPi->Fill(pdgName, prob, GetPreTagEvtWeight());
	    return 1;
	} else {
	    
	    if (MC1->GetMother()->GetMother() != MC3->GetMother()) {
		fHistLambdareco_truth_PPi->Fill("Comb", prob, GetPreTagEvtWeight());
		fN_SameOriginMiss++;
	    } else {
		fHistLambdareco_truth_PPi->Fill(pdgName, prob, GetPreTagEvtWeight());
		fN_MCFail++;
	    }

	    return -1;
	}
    } else {
	return 0;
    }
      
}

//____________________________________________________________________

Double_t AtlDstarFinder::GetModifiedEventWeight() {
    UInt_t run[] = {107680, 107681, 107682, 107683, 107684, 107685, 107690, 107691, 107692, 107693, 107694, 107695, 107700, 107701, 107702, 107703, 107704, 107705, 106280, 106281, 106283, 107650, 107651, 107652, 107653, 107654, 107655, 107660, 107661, 107662, 107663, 107664, 107665, 107670, 107671, 107672, 107673, 107674, 107675, 109300, 109301, 109302, 109303, 109305, 109306, 109307, 109308, 109310, 109311, 109312, 109313, 105860, 107941, 108340, 108341, 108342, 108343, 108344, 108345, 108346, 107100, 107101, 107102, 107103, 107104, 107105, 107106, 107107, 107108, 107109, 107110, 107111, 108320, 108319, 113159, 113160, 113161, 113162, 113163, 113164, 113165, 113166, 113167, 113168, 113169, 113170, 113171, 113172, 113173, 113174, 113175, 113176, 113177, 113178, 113179, 113180, 113181, 113182, 113183};
    Double_t mod[] = {0.00505753, 0.00500372, 0.00199634, 0.00199893, 0.0019475, 0.00200058, 0.00502187, 0.00500647, 0.00199776, 0.00198675, 0.00197829, 0.00200114, 0.00500611, 0.00501191, 0.00199845, 0.00199715, 0.00197753, 0.00175088, 0.000492383, 0.000472727, 0.0004, 0.00221211, 0.0021012, 0.00206698, 0.00203673, 0.0018012, 0.0016, 0.00216386, 0.00210807, 0.00208498, 0.00201928, 0.00186791, 0.00160321, 0.00216707, 0.00209512, 0.00207265, 0.00200109, 0.00193462, 0.00140281, 4.20697e-05, 2.43636e-05, 2.11408e-05, 3.83355e-05, 4.23016e-05, 2.45061e-05, 2.11413e-05, 3.77813e-05, 4.25241e-05, 2.42642e-05, 2.16372e-05, 3.81252e-05, 0.000395824, 0.000444825, 0.000715701, 0.000717815, 0.0007128, 4.70854e-05, 4.68587e-05, 4.70188e-05, 0.000972391, 4.11099e-05, 3.94879e-05, 2.94078e-05, 1.78053e-05, 4.42696e-05, 3.9916e-05, 4.42442e-05, 1.86112e-05, 4.94148e-05, 5e-05, 1.76247e-05, 1.1209e-05, 0.00122778, 0.00127425, 170.341, 5.9222, 0.214886, 0.00483829, 0.000488728, 30.7848, 20.7877, 1.33516, 0.0397393, 0.00346116, 6.53177, 7.77513, 1.35395, 0.0715945, 0.00846341, 0.987771, 2.3384, 0.750828, 0.0564942, 0.00962421, 0.139078, 0.680058, 0.400836, 0.0575774, 0.0267182};
    Int_t nRuns = 21 + 78;

    for (Int_t i = 0; i < nRuns; i++) {
	if (run[i] == fEvent->RunNr()) {
	    return mod[i];
	}
    }
    return 1.0;
}
//____________________________________________________________________


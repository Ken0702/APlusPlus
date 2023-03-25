//____________________________________________________________________
//
// Lambda decay finder class
//

//
// Author: Thomas Kintscher <mailto:kintscher@physik.hu-berlin.de>
// Copyright: 2010 (C) Thomas Kintscher
//
#ifndef ATLAS_AtlD0Finder
#include <AtlD0Finder.h>
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
#include <string>

using namespace std;

#ifndef __CINT__
ClassImp(AtlD0Finder);
#endif

//____________________________________________________________________

AtlD0Finder::AtlD0Finder(const char* name, const char* title) :
    AtlKinFitterTool(name, title) {
    //
    // Default constructor
    //
    SetMode(kKinFit);
    SetDebugOutput(kFALSE);
    fBkgLambdaDecays = new TList;
}

//____________________________________________________________________

AtlD0Finder::~AtlD0Finder() {
    //
    // Default destructor
    //
    fBkgLambdaDecays->Delete(); delete fBkgLambdaDecays;
    if (IsDebugRun() ) fDebugStream.close();
}

//____________________________________________________________________

void AtlD0Finder::SetBranchStatus() {
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

void AtlD0Finder::BookHistograms() {
    //
    // Book histograms
    //
    if ( IsDebugRun() ) fDebugStream.open("DebugOutputD0.dat");
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
      fHistD0reco_m_kaon_pi->SetXTitle("m_{Something} (GeV)"); fHistD0reco_m_kaon_pi->SetYTitle("Number of Entries");
    }
    fHistLambdareco_m_PPi->SetXTitle("m_{Lambda} (GeV)"); fHistLambdareco_m_PPi->SetYTitle("Number of Entries");
    
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
    fHistLambdareco_tau_PPi = new TH1F("h_Lambda_tau_PPi", "Reconstructed Lambda lifetime (Lambda->PPi)", 100, 0, 0.05);
    fHistLambdareco_tau_PPi->SetXTitle("#tau (ps)");      fHistLambdareco_tau_PPi->SetYTitle("Number of Entries");
    fHistLambdareco_truth_PPi = new TH2F("h_Lambda_truth_PPi", "Pdg Code of truth-matched, reconstructed particles", 6260, -3130, 3130, 50, 0.0, 1.0);
    fHistLambdareco_truth_PPi->SetXTitle("Pdg Code");     fHistLambdareco_truth_PPi->SetYTitle("#Chi^{2} Probability");
    fHistLambdareco_truth_PPi->SetZTitle("Number of Entries");
    fHistLambdareco_R_vtx = new TH1F("h_Lambda_r_vtx", "Radial distance of secondary vertices", 400, 0.0, 0.2);
    fHistLambdareco_R_vtx->SetXTitle("r_{#perp} (cm)");   fHistLambdareco_R_vtx->SetYTitle("Number of Entrs");
    fHistLambdareco_tdcy_len = new TH1F("h_Lambda_tdcy_len", "Transverse #Lambda decay length", 400, 0.0, 0.001);
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
    fHistLambdareco_tau_PPi_bkg = new TH1F("h_Lambda_tau_PPi_bkg", "Reconstructed Lambda' lifetime (Lambda->PPi)", 100, 0, 0.05);
    fHistLambdareco_tau_PPi_bkg->SetXTitle("#tau (ps)");      fHistLambdareco_tau_PPi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_truth_PPi_bkg = new TH1F("h_Lambda_truth_PPi_bkg", "Pdg Code of truth-matched, reconstructed particles", 6260, -3130, 3130);
    fHistLambdareco_truth_PPi_bkg->SetXTitle("Pdg Code");     fHistLambdareco_truth_PPi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_R_vtx_bkg = new TH1F("h_Lambda_r_vtx_bkg", "Radial distance of secondary vertices", 400, 0.0, 0.2);
    fHistLambdareco_R_vtx_bkg->SetXTitle("r_{#perp} (cm)");   fHistLambdareco_R_vtx_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_tdcy_len_bkg = new TH1F("h_Lambda_tdcy_len_bkg", "Transverse #Lambda decay length", 400, 0.0, 0.001);
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
    fHistLambdareco_MC_m_pipi = new TH1F("h_Lambda_MC_m_pipi", "Invariant mass w/ pion/pion hypothesis (MC signal)", 480, 0.0, 6.0);
    fHistLambdareco_MC_m_pipi->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_pipi->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ee = new TH1F("h_Lambda_MC_m_ee", "Invariant mass w/ ee hypothesis (MC signal)", 480, 0.0, 6.0);
    fHistLambdareco_MC_m_ee->SetXTitle("m_{#Lambda} (GeV)");  fHistLambdareco_MC_m_ee->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ppi_o = new TH1F("h_Lambda_MC_m_ppi_o", "Invariant mass w/ proton/pion hypothesis (MC signal)", 480, 0.0, 2.5);
    fHistLambdareco_MC_m_ppi_o->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_ppi_o->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_pipi_o = new TH1F("h_Lambda_MC_m_pipi_o", "Invariant mass w/ pion/pion hypothesis (MC signal)", 480, 0.0, 6.0);
    fHistLambdareco_MC_m_pipi_o->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_pipi_o->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ee_o = new TH1F("h_Lambda_MC_m_ee_o", "Invariant mass w/ ee hypothesis (MC signal)", 480, 0.0, 6.0);
    fHistLambdareco_MC_m_ee_o->SetXTitle("m_{#Lambda} (GeV)");  fHistLambdareco_MC_m_ee_o->SetYTitle("Number of Entries");
    fHistLambdareco_MC_pt  = new TH1F("h_Lambda_MC_pt", "Lambda-p_{#perp} (MC signal)", 320, 0, 60);
    fHistLambdareco_MC_pt->SetXTitle("p_{#perp} (GeV)");      fHistLambdareco_MC_pt->SetYTitle("Number of Entries");
    fHistLambdareco_MC_phi = new TH1F("h_Lambda_MC_phi", "Lambda #phi distribution (MC signal)", 128, -3.2, 3.2);
    fHistLambdareco_MC_phi->SetXTitle("#phi (rad)");          fHistLambdareco_MC_phi->SetYTitle("Number of Entries");
    fHistLambdareco_MC_eta = new TH1F("h_Lambda_MC_eta", "Lambda #eta distribution (MC signal)", 100, -3, 3);
    fHistLambdareco_MC_eta->SetXTitle("#eta");                fHistLambdareco_MC_eta->SetYTitle("Number of Entries");
    fHistLambdareco_MC_tau = new TH1F("h_Lambda_MC_tau", "Reconstructed Lambda lifetime (MC signal)", 100, 0, 0.05);
    fHistLambdareco_MC_tau->SetXTitle("#tau (ps)");           fHistLambdareco_MC_tau->SetYTitle("Number of Entries");
    fHistLambdareco_MC_src_div = new TH1F("h_Lambda_MC_src_div", "Angular distance of #Lambda origin and primary vertex", 300, 0.0, 0.012);
    fHistLambdareco_MC_src_div->SetXTitle("#alpha (rad)");    fHistLambdareco_MC_src_div->SetYTitle("Number of Entries");
    fHistLambdareco_MC_R_vtx = new TH1F("h_Lambda_MC_r_vtx", "Radial distance of secondary vertices", 400, 0.0, 0.2);
    fHistLambdareco_MC_R_vtx->SetXTitle("r_{#perp} (cm)");    fHistLambdareco_MC_R_vtx->SetYTitle("Number of Entries");
    fHistLambdareco_MC_tdcy_len = new TH1F("h_Lambda_MC_tdcy_len", "Transverse decay length (MC signal)", 400, 0.0, 0.001);
    fHistLambdareco_MC_tdcy_len->SetXTitle("s_{#perp} (cm)"); fHistLambdareco_MC_tdcy_len->SetYTitle("Number of Entries");
    fHistLambdareco_MC_oangle = new TH1F("h_Lambda_MC_oangle", "Opening angle of p/pi (MC signal)", 256, 0.0, 3.2);
    fHistLambdareco_MC_oangle->SetXTitle("#alpha (rad)");     fHistLambdareco_MC_oangle->SetYTitle("Number of Entries");
    fHistLambdareco_MC_coangle = new TH1F("h_Lambda_MC_coangle", "Opening angle of p/pi (MC bkg)", 256, -1.0, 1.0);
    fHistLambdareco_MC_coangle->SetXTitle("cos(#alpha)");     fHistLambdareco_MC_coangle->SetYTitle("Number of Entries");
    fHistLambdareco_MC_dangle = new TH1F("h_Lambda_MC_dangle", "Decay angle of p/pi (MC signal)", 256, -1.0, 1.0);
    fHistLambdareco_MC_dangle->SetXTitle("cos(#alpha)");      fHistLambdareco_MC_dangle->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_dangle = new TH2F("h_lambda_m_dangle", "M vs. Decay Angle", 40, -1.0, 1.0, 500, 1.85, 1.88);
    fHistLambdareco_MC_m_dangle->SetXTitle("cos #alpha");     fHistLambdareco_MC_m_dangle->SetYTitle("m (GeV)");
    fHistLambdareco_MC_d0_K = new TH1F("h_Lambda_MC_d0_K",  "D0 of Kaon", 200, -1.0, 1.0);
    fHistLambdareco_MC_d0_K->SetXTitle("d0 (cm)");            fHistLambdareco_MC_d0_K->SetYTitle("Number of Entries");
    fHistLambdareco_MC_d0_Pi = new TH1F("h_Lambda_MC_d0_Pi","D0 of Pion", 200, -1.0, 1.0);
    fHistLambdareco_MC_d0_Pi->SetXTitle("d0 (cm)");           fHistLambdareco_MC_d0_Pi->SetYTitle("Number of Entries");
    fHistLambdareco_MC_d0ovE_K = new TH1F("h_Lambda_MC_d0ovE_K",   "D0/err of Kaon", 400, -40.0, 40.0);
    fHistLambdareco_MC_d0ovE_K->SetXTitle("d0 (cm)");         fHistLambdareco_MC_d0ovE_K->SetYTitle("Number of Entries");
    fHistLambdareco_MC_d0ovE_Pi = new TH1F("h_Lambda_MC_d0ovE_Pi", "D0/err of Pion", 400, -40.0, 40.0);
    fHistLambdareco_MC_d0ovE_Pi->SetXTitle("d0 (cm)");        fHistLambdareco_MC_d0ovE_Pi->SetYTitle("Number of Entries");
    fHistLambdareco_MC_z0_Pi = new TH1F("h_Lambda_MC_z0_Pi", "Z0 of #pi", 200, -3.0, 3.0);
    fHistLambdareco_MC_z0_Pi->SetXTitle("d0 (cm)");           fHistLambdareco_MC_z0_Pi->SetYTitle("Number of Entries");
    fHistLambdareco_MC_z0_K = new TH1F("h_Lambda_MC_z0_K", "Z0 of K", 200, -3.0, 3.0);
    fHistLambdareco_MC_z0_K->SetXTitle("z0 (cm)");            fHistLambdareco_MC_z0_K->SetYTitle("Number of Entries");
    fHistLambdareco_MC_pi_tchi2 = new TH1F("h_Lambda_MC_pi_tchi2", "#Chi^{2}/ndf of #pi tracks", 50, 0.0, 5.0);
    fHistLambdareco_MC_pi_tchi2->SetXTitle("#Chi^{2}/ndf");   fHistLambdareco_MC_pi_tchi2->SetYTitle("Number of Entries");
    fHistLambdareco_MC_k_tchi2 = new TH1F("h_Lambda_MC_k_tchi2", "#Chi^{2}/ndf of K tracks", 50, 0.0, 5.0);
    fHistLambdareco_MC_k_tchi2->SetXTitle("#Chi^{2}/ndf");    fHistLambdareco_MC_k_tchi2->SetYTitle("Number of Entries");
    fHistLambdareco_MC_z0_diff = new TH1F("h_Lambda_MC_z0_diff", "Z0 of K", 100, 0.0, 1.0);
    fHistLambdareco_MC_z0_diff->SetXTitle("#Delta z0 (cm)");  fHistLambdareco_MC_z0_diff->SetYTitle("Number of Entries");
    
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
    fHistLambdareco_MC_m_pipi_bkg = new TH1F("h_Lambda_MC_m_pipi_bkg", "Invariant mass w/ #pi#pi hypothesis (MC bkg)", 480, 0.0, 6.0);
    fHistLambdareco_MC_m_pipi_bkg->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_pipi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ee_bkg = new TH1F("h_Lambda_MC_m_ee_bkg", "Invariant mass w/ ee hypothesis (MC bkg)", 480, 0.0, 6.0);
    fHistLambdareco_MC_m_ee_bkg->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_ee_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ppi_o_bkg = new TH1F("h_Lambda_MC_m_ppi_o_bkg", "Invariant mass w/ p#pi hypothesis (MC bkg)", 480, 0.0, 2.5);
    fHistLambdareco_MC_m_ppi_o_bkg->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_ppi_o_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_pipi_o_bkg = new TH1F("h_Lambda_MC_m_pipi_o_bkg", "Invariant mass w/ #pi#pi hypothesis (MC bkg)", 480, 0.0, 6.0);
    fHistLambdareco_MC_m_pipi_o_bkg->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_pipi_o_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ee_o_bkg = new TH1F("h_Lambda_MC_m_ee_o_bkg", "Invariant mass w/ ee hypothesis (MC bkg)", 480, 0.0, 6.0);
    fHistLambdareco_MC_m_ee_o_bkg->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_ee_o_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_pt_bkg  = new TH1F("h_Lambda_MC_pt_bkg", "Lambda-p_{#perp} (MC background)", 320, 0, 60);
    fHistLambdareco_MC_pt_bkg->SetXTitle("p_{#perp} (GeV)");   fHistLambdareco_MC_pt_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_phi_bkg = new TH1F("h_Lambda_MC_phi_bkg", "Lambda #phi distribution (MC background)", 128, -3.2, 3.2);
    fHistLambdareco_MC_phi_bkg->SetXTitle("#phi (rad)");       fHistLambdareco_MC_phi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_eta_bkg = new TH1F("h_Lambda_MC_eta_bkg", "Lambda #eta distribution (MC background)", 100, -3, 3);
    fHistLambdareco_MC_eta_bkg->SetXTitle("#eta");             fHistLambdareco_MC_eta_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_tau_bkg = new TH1F("h_Lambda_MC_tau_bkg", "Reconstructed Lambda lifetime (MC background)", 100, 0, 0.05);
    fHistLambdareco_MC_tau_bkg->SetXTitle("#tau (ps)");        fHistLambdareco_MC_tau_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_src_div_bkg = new TH1F("h_Lambda_MC_src_div_bkg", "Angular distance of #Lambda origin and primary vertex", 300, 0.0, 0.012);
    fHistLambdareco_MC_src_div_bkg->SetXTitle("#alpha (rad)"); fHistLambdareco_MC_src_div_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_R_vtx_bkg = new TH1F("h_Lambda_MC_r_vtx_bkg", "Radial distance of secondary vertices", 400, 0.0, 0.2);
    fHistLambdareco_MC_R_vtx_bkg->SetXTitle("r_{#perp} (cm)"); fHistLambdareco_MC_R_vtx_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_tdcy_len_bkg = new TH1F("h_Lambda_MC_tdcy_len_bkg", "Transverse decay length (MC bkg)", 400, 0.0, 0.001);
    fHistLambdareco_MC_tdcy_len_bkg->SetXTitle("s_{#perp} (cm)"); fHistLambdareco_MC_tdcy_len_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_oangle_bkg = new TH1F("h_Lambda_MC_oangle_bkg", "Opening angle of p/pi (MC bkg)", 256, 0.0, 3.2);
    fHistLambdareco_MC_oangle_bkg->SetXTitle("#alpha (rad)");  fHistLambdareco_MC_oangle_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_coangle_bkg = new TH1F("h_Lambda_MC_coangle_bkg", "Opening angle of p/pi (MC bkg)", 256, -1.0, 1.0);
    fHistLambdareco_MC_coangle_bkg->SetXTitle("cos(#alpha)");  fHistLambdareco_MC_coangle_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_dangle_bkg = new TH1F("h_Lambda_MC_dangle_bkg", "Decay angle of p/pi (MC bkg)", 256, -1.0, 1.0);
    fHistLambdareco_MC_dangle_bkg->SetXTitle("cos(#alpha)");   fHistLambdareco_MC_dangle_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_dangle_bkg = new TH2F("h_lambda_m_dangle_bkg", "M vs. Decay Angle", 40, -1.0, 1.0, 500, 1.85, 1.88);
    fHistLambdareco_MC_m_dangle_bkg->SetXTitle("cos #alpha");  fHistLambdareco_MC_m_dangle_bkg->SetYTitle("m (GeV)");
    fHistLambdareco_MC_d0_K_bkg = new TH1F("h_Lambda_MC_d0_K_bkg",   "D0 of Kaon", 200, -1.0, 1.0);
    fHistLambdareco_MC_d0_K_bkg->SetXTitle("d0 (cm)");         fHistLambdareco_MC_d0_K_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_d0_Pi_bkg = new TH1F("h_Lambda_MC_d0_Pi_bkg", "D0 of Pion", 200, -1.0, 1.0);
    fHistLambdareco_MC_d0_Pi_bkg->SetXTitle("d0 (cm)");        fHistLambdareco_MC_d0_Pi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_d0ovE_K_bkg = new TH1F("h_Lambda_MC_d0ovE_K_bkg",   "D0/err of Kaon", 400, -40.0, 40.0);
    fHistLambdareco_MC_d0ovE_K_bkg->SetXTitle("d0 (cm)");      fHistLambdareco_MC_d0ovE_K_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_d0ovE_Pi_bkg = new TH1F("h_Lambda_MC_d0ovE_Pi_bkg", "D0/err of Pion", 400, -40.0, 40.0);
    fHistLambdareco_MC_d0ovE_Pi_bkg->SetXTitle("d0 (cm)");     fHistLambdareco_MC_d0ovE_Pi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_z0_Pi_bkg = new TH1F("h_Lambda_MC_z0_Pi_bkg", "Z0 of #pi", 200, -3.0, 3.0);
    fHistLambdareco_MC_z0_Pi_bkg->SetXTitle("d0 (cm)");           fHistLambdareco_MC_z0_Pi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_z0_K_bkg = new TH1F("h_Lambda_MC_z0_K_bkg", "Z0 of K", 200, -3.0, 3.0);
    fHistLambdareco_MC_z0_K_bkg->SetXTitle("z0 (cm)");            fHistLambdareco_MC_z0_K_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_pi_tchi2_bkg = new TH1F("h_Lambda_MC_pi_tchi2_bkg", "#Chi^{2}/ndf of #pi tracks", 50, 0.0, 5.0);
    fHistLambdareco_MC_pi_tchi2_bkg->SetXTitle("#Chi^{2}/ndf");   fHistLambdareco_MC_pi_tchi2_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_k_tchi2_bkg = new TH1F("h_Lambda_MC_k_tchi2_bkg", "#Chi^{2}/ndf of K tracks", 50, 0.0, 5.0);
    fHistLambdareco_MC_k_tchi2_bkg->SetXTitle("#Chi^{2}/ndf");    fHistLambdareco_MC_k_tchi2_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_z0_diff_bkg = new TH1F("h_Lambda_MC_z0_diff_bkg", "Z0 of K", 200, 0.0, 1.0);
    fHistLambdareco_MC_z0_diff_bkg->SetXTitle("#Delta z0 (cm)");  fHistLambdareco_MC_z0_diff_bkg->SetYTitle("Number of Entries");

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

    fHist_MC_KPi_vtx = new TH1F("h_MC_KPi_vtx", "Vertices of Kaon/Pion", 12, 0.0, 12.0);
    fHist_MC_KPi_vtx->SetYTitle("Number of Entries");
}


//____________________________________________________________________

Bool_t AtlD0Finder::AnalyzeEvent() {
    //
    // Event analysis
    //

    // Clean-up bkg list from previous event
    for (Int_t i = 0; i < fBkgLambdaDecays->GetEntries(); i++) {
	AtlD0DecayKPi*obj = (AtlD0DecayKPi*)fBkgLambdaDecays->At(i);
	delete obj;
    }
    fBkgLambdaDecays->Clear("C");


    if (fEvent->IsMC()) {
	// Count all d0 decays in MC events to allow computation of the efficiency.
	Int_t ND0True = 0;
	for (Int_t i = 0; i < fEvent->GetN_IDTracks(); i++) {
	    for (Int_t j = i+1; j < fEvent->GetN_IDTracks(); j++) {
		AtlIDTrack *trk1 = (AtlIDTrack*)fEvent->GetIDTracks()->At(i);
		AtlIDTrack *trk2 = (AtlIDTrack*)fEvent->GetIDTracks()->At(j);
		if( (trk1->HasValidTruthMatch(fMatchingProb_min)
		     && trk2->HasValidTruthMatch(fMatchingProb_min))
		    &&( (trk1->GetMCParticle()->GetMother() != NULL)
			&&(trk2->GetMCParticle()->GetMother() != NULL) )  ) {
		    HepMCParticle *MCMom = trk1->GetMCParticle()->GetMother();
		    if ( (MCMom->GetN_Daughters() == 2)
			 && (TMath::Abs(trk1->GetMCParticle()->GetMother()->GetPdgCode()) == 421)
			 && (trk1->GetMCParticle()->GetMother() == trk2->GetMCParticle()->GetMother())
			 && (((TMath::Abs(trk1->GetMCParticle()->GetPdgCode()) == 211)
			      && (TMath::Abs(trk2->GetMCParticle()->GetPdgCode()) == 321) )
			     || ((TMath::Abs(trk1->GetMCParticle()->GetPdgCode()) == 321)
				  && (TMath::Abs(trk2->GetMCParticle()->GetPdgCode()) == 211) )) ) {
			ND0True++;
		    }
		}
	    }
	}
	AddAbundanceTrue(ND0True);
    }
    
    if (fEvent->IsMC()) {
	HepVertex *PrimVtx = fEvent->GetPrimaryVtx();
	if (PrimVtx == NULL) return kFALSE;
	
	TList *mc = fEvent->FindMCParticlesByName("D0");
	TList *mc2 = fEvent->FindMCParticlesByName("D0_bar");
	for (Int_t i = 0; i < mc->GetEntries() + mc2->GetEntries(); i++) {
	    HepMCParticle *prt;
	    if (i < mc->GetEntries())
		prt = (HepMCParticle*)mc->At(i);
	    else
		prt = (HepMCParticle*)mc2->At(i-mc->GetEntries());
	    if (prt->GetN_Daughters() != 2) continue;

	    HepMCParticle *d1, *d2;
	    if ( TMath::Abs(((HepMCParticle*)prt->GetDaughters()->At(0))->GetPdgCode()) == 321 ) {
		d1 = (HepMCParticle*)prt->GetDaughters()->At(0);
		d2 = (HepMCParticle*)prt->GetDaughters()->At(1);
	    } else {
		d1 = (HepMCParticle*)prt->GetDaughters()->At(1);
		d2 = (HepMCParticle*)prt->GetDaughters()->At(0);
	    }
	    if (!( (TMath::Abs(d1->GetPdgCode()) == 321) && (TMath::Abs(d2->GetPdgCode()) == 211) ) ) continue;
	    
	    AtlIDTrack *t1 = fEvent->GetAssociatedIDTrack(d1);
	    AtlIDTrack *t2 = fEvent->GetAssociatedIDTrack(d2);
	    if ((t1 == NULL) && (t2 == NULL)) {
		fHist_MC_KPi_vtx->Fill("KPi invisible", GetPreTagEvtWeight());
	    } else if (t1 == NULL) {
		fHist_MC_KPi_vtx->Fill("K invisible", GetPreTagEvtWeight());
	    } else if (t2 == NULL) {
		fHist_MC_KPi_vtx->Fill("Pi invisible", GetPreTagEvtWeight());
	    } else if (t1->GetVertex() == NULL) {
		if (t2->GetVertex() == NULL) {
		    fHist_MC_KPi_vtx->Fill("No vtx/no vtx", GetPreTagEvtWeight());
		} else if (t2->GetVertex() == PrimVtx) {
		    fHist_MC_KPi_vtx->Fill("K no vtx/Pi prim vtx", GetPreTagEvtWeight());
		} else {
		    fHist_MC_KPi_vtx->Fill("K no vtx/Pi sec vtx", GetPreTagEvtWeight());
		}
	    } else if (t1->GetVertex() == PrimVtx) {
		if (t2->GetVertex() == NULL) {
		    fHist_MC_KPi_vtx->Fill("K prim vtx/no vtx", GetPreTagEvtWeight());
		} else if (t2->GetVertex() == PrimVtx) {
		    fHist_MC_KPi_vtx->Fill("K prim vtx/Pi prim vtx", GetPreTagEvtWeight());
		} else {
		    fHist_MC_KPi_vtx->Fill("K prim vtx/Pi sec vtx", GetPreTagEvtWeight());
		}
	    } else {
		if (t2->GetVertex() == NULL) {
		    fHist_MC_KPi_vtx->Fill("K sec vtx/no vtx", GetPreTagEvtWeight());
		} else if (t2->GetVertex() == PrimVtx) {
		    fHist_MC_KPi_vtx->Fill("K sec vtx/Pi prim vtx", GetPreTagEvtWeight());
		} else {
		    fHist_MC_KPi_vtx->Fill("K sec vtx/Pi sec vtx", GetPreTagEvtWeight());
		}
	    }

	}
	delete mc;
	delete mc2;
    }

    // Do analysis
    if ( fMode == kCutBased ) {
      ReconstructLambdaCutBased();
    } else if ( fMode == kKinFit ) {
      ReconstructLambdaKinFit();
    } else {
      Error("AnalyzeEvent", "Invalid mode given. Abort!");
      gSystem->Abort(0);
    }
    return kTRUE;
}

//____________________________________________________________________


void AtlD0Finder::ReconstructLambdaCutBased(){
    //
    // Cut Based Lambda reconstruction  
    //

    // Looping over all trk-combinations at a sec Vertex for Lambda reconstruction

    return;

    AtlIDTrack *trk1 = 0;
    AtlIDTrack *trk2 = 0;                    // Compared trks
    HepVertex *Vtx;                          // Associated secondary vertex
    HepVertex *PrimaryVtx;                   // Primary vertex

    Int_t i;                                 // Vertex Loop iterator
    Int_t BkgCount = 0;                      // Background events counter

    TLorentzVector p_trk1, p_trk2, p_Lambda; // 4 Momentum
    Float_t m_Lambda = 0;                    // Reconstructed Lambda-mass

    PrimaryVtx = fEvent->GetPrimaryVtx();

    // Vertex-Loop
    for (i = 0; i < fEvent->GetN_Vertices(); i++) {

      Vtx = (HepVertex*)fEvent->GetVertices()->At(i);
      // regard only high-quality secondary vertices containing 2 outgoing tracks
      if ( !( (Vtx->IsSecondary()) && (Vtx->GetNDaughters()==2) ))  continue;
      if ( Vtx->GetChi2overNDoF() > fVertexChi2ovNDoF_max )  continue;

      // get outgoing tracks and test their quality
      trk1 = (AtlIDTrack*)Vtx->GetDaughters()->At(0);
      trk2 = (AtlIDTrack*)Vtx->GetDaughters()->At(1);
      if ( trk1->GetChi2_Vtx() / trk1->GetNDoF_Vtx() > fTrackChi2ovNDoF_max ) continue;
      if ( trk2->GetChi2_Vtx() / trk2->GetNDoF_Vtx() > fTrackChi2ovNDoF_max ) continue;

      // Get 4 Momentum and compute inv. mass of Lambda candidate
      p_trk1.SetVectM(trk1->P(), fm_proton);
      p_trk2.SetVectM(trk2->P(), fm_pi);
      p_Lambda = p_trk1 + p_trk2;
      m_Lambda = p_Lambda.M();

      // Selection constraints
      // -------------------------
    
      if( (m_Lambda > fD0_M_min) && (m_Lambda < fD0_M_max) ) { // cut: invariant mass
        if (trk1->GetQovP() * trk2->QovP() < 0.)    {                  // cut: opposite sign for signal

	  if ( (p_trk1.Pt()  > fSignal_Pt_max)  || (p_trk1.Pt()  < fSignal_Pt_min)  ) continue;
	  if ( (p_trk2.Pt()  > fSignal_Pt_max)  || (p_trk2.Pt()  < fSignal_Pt_min)  ) continue;
	  if ( (p_trk1.Eta() > fSignal_Eta_max) || (p_trk1.Eta() < fSignal_Eta_min) ) continue;
	  if ( (p_trk2.Eta() > fSignal_Eta_max) || (p_trk2.Eta() < fSignal_Eta_min) ) continue;

	  // guess which one is the proton (the one with the larger momentum?)
	  // and add LambdaDecay to fEvents Decay-list
	  if ( p_trk1.P() > p_trk2.P() ) {
	    // fitted daughter tracks are just clones, because fitting only happens in the kinfitter method
	    HepParticle Fit_Daughter1(1, p_trk1.Px(), p_trk1.Py(), p_trk1.Pz(), p_trk1.E(),  
				      (trk1->GetQovP() < 0.) ? -2212 : 2212);
	    HepParticle Fit_Daughter2(2, p_trk2.Px(), p_trk2.Py(), p_trk2.Pz(), p_trk2.E(), 
				      (trk2->GetQovP() < 0.) ?  -211 :  211);
	    fEvent->AddLambdaDecayPiPi(p_Lambda.Px(), p_Lambda.Py(), p_Lambda.Pz(), p_Lambda.E(),
				       trk1, trk2, Vtx, PrimaryVtx, Fit_Daughter1, Fit_Daughter2);
	  } else {
	    HepParticle Fit_Daughter1(1, p_trk1.Px(), p_trk1.Py(), p_trk1.Pz(), p_trk1.E(), 
				      (trk1->GetQovP() < 0.) ?  -211 :  211);
	    HepParticle Fit_Daughter2(2, p_trk2.Px(), p_trk2.Py(), p_trk2.Pz(), p_trk2.E(), 
				      (trk2->GetQovP() < 0.) ? -2212 : 2212);
	    fEvent->AddLambdaDecayPiPi(p_Lambda.Px(), p_Lambda.Py(), p_Lambda.Pz(), p_Lambda.E(),
				       trk2, trk1, Vtx, PrimaryVtx, Fit_Daughter1, Fit_Daughter2);
	  }
        } else {
	  // both particles have same charge; add to background list
	  HepParticle Fit_Daughter1(1, p_trk1.Px(), p_trk1.Py(), p_trk1.Pz(), p_trk1.E(),
				    (trk1->GetQovP() < 0.) ? -2212 : 2212);
	  HepParticle Fit_Daughter2(2, p_trk2.Px(), p_trk2.Py(), p_trk2.Pz(), p_trk2.E(),
				    (trk2->GetQovP() < 0.) ?  -211 :  211);
	  fBkgLambdaDecays->Add(new AtlD0DecayKPi(BkgCount++, p_Lambda.Px(), p_Lambda.Py(), p_Lambda.Pz(),
						      p_Lambda.E(), trk1, trk2, Vtx, PrimaryVtx, 
						      Fit_Daughter1, Fit_Daughter2));
        }
      }
    }
}


//____________________________________________________________________

Bool_t AtlD0Finder::IsKaon(AtlIDTrack *trk1, AtlIDTrack *trk2) {
    //
    // Return true, if the given vertex is already assigned
    // to a kaon decay. Return false otherwise or if
    // there are no kaon decays (eg. the K0s finder didn't run).
    //

    AtlK0sDecayPiPi *decay = 0;
    TClonesArray *k0s = fEvent->GetK0sDecaysPiPi();
    for (Int_t i = 0; i < fEvent->GetN_K0sDecaysPiPi(); i++) {
	decay = (AtlK0sDecayPiPi*)k0s->At(i);
	if ((decay->GetPiPlus() == trk1)  || (decay->GetPiPlus() == trk2) ||
	    (decay->GetPiMinus() == trk1) || (decay->GetPiMinus() == trk2))
	    return kTRUE;
    }
    
    TLorentzVector p1, p2, p;
    p1.SetVectM(trk1->P(), fm_pi);
    p2.SetVectM(trk2->P(), fm_pi);
    p = p1 + p2;
    if ((p.M() > 0.492) && (p.M() < 0.503))
	return kTRUE;
    return kFALSE;
}

//____________________________________________________________________

Bool_t AtlD0Finder::IsLambda(AtlIDTrack *trk1, AtlIDTrack *trk2) {
    //
    // Return true, if the given vertex is already assigned
    // to a kaon decay. Return false otherwise or if
    // there are no kaon decays (eg. the K0s finder didn't run).
    //
    
    if (fEvent->GetN_LambdaDecaysPiPi() < 1) {
	return kFALSE;
    }

    AtlLambdaDecayPPi *decay;
    TClonesArray *lambda = fEvent->GetLambdaDecaysPiPi();
    for (Int_t i = 0; i < fEvent->GetN_LambdaDecaysPiPi(); i++) {
	decay = (AtlLambdaDecayPPi*)lambda->At(i);
	if ((decay->GetProton() == trk1) || (decay->GetProton() == trk2) ||
	    (decay->GetPion()   == trk1) || (decay->GetPion()   == trk2))
	    return kTRUE;
    }

    TLorentzVector p1, p2, p_ppi, p_pip;
    p1.SetVectM(trk1->P(), fm_proton);
    p2.SetVectM(trk2->P(), fm_pi);
    p_ppi = p1 + p2;
    if ((p_ppi.M() > 1.11) && (p_ppi.M() < 1.12))
	return kTRUE;
    p1.SetVectM(trk1->P(), fm_pi);
    p2.SetVectM(trk2->P(), fm_proton);
    p_pip = p1 + p2;
    if ((p_pip.M() > 1.11) && (p_pip.M() < 1.12))
	return kTRUE;
    return kFALSE;
}

//____________________________________________________________________

Bool_t AtlD0Finder::IsConversion(AtlIDTrack *trk1, AtlIDTrack *trk2) {
    //
    // Return true, if the given vertex was already being tagged
    // as a photo conversion vertex. Return false otherwise or if
    // there are no conversion vertices.
    //
   
    TLorentzVector p_e1, p_e2, p_g;
    p_e1.SetVectM(trk1->P(), fm_lambda);
    p_e2.SetVectM(trk2->P(), fm_e);
    p_g = p_e1 + p_e2;
    return (p_g.M() < fPhotonMass_max);
}


//____________________________________________________________________

void AtlD0Finder::ReconstructLambdaKinFit() {
  //
  // Lambda reconstruction from track-pairs assuming being pions using KinFitter
  //
  HepVertex  *PrimVtx = 0;
  AtlIDTrack *trk1 = 0;  
  AtlIDTrack *trk2 = 0;
    
  TLorentzVector p_D0;
    
  TMatrixD cov_trk1(3,3);
  TMatrixD cov_trk2(3,3);
  
  Double_t fChi2;
  Int_t fNDoF;

  Int_t BkgCount = 1;  // Counts background lambdas (with equally charged decay products)
  Float_t PvalueKinFit = 0.0;

  // Get primary vertex
  PrimVtx = fEvent->GetPrimaryVtx();
  if (PrimVtx == NULL) return;

  // Vertex-Loop
  for ( Int_t i = 0; i < fEvent->GetN_IDTracks(); i++){
    
    trk1 = (AtlIDTrack*)fEvent->GetIDTracks()->At(i);
    if ( trk1->Chi2ovNDoF() > fTrackChi2ovNDoF_max ) continue;
    if ( ! trk1->HasValidTruthMatch(fMatchingProb_min) ) continue;
    /*if ( ! (trk1->GetVertex() == PrimVtx) ) continue;*/

    for ( Int_t j = i+1; j < fEvent->GetN_IDTracks(); j++) {
	trk2 = (AtlIDTrack*)fEvent->GetIDTracks()->At(j);
	/*if ( ! (trk2->GetVertex() == PrimVtx) ) continue;*/
	if ( trk2->Chi2ovNDoF() > fTrackChi2ovNDoF_max ) continue;
	SetCutFlow("Trk #chi^{2}");

	if ( ! trk2->HasValidTruthMatch(fMatchingProb_min) ) continue;
	SetCutFlow("MC exists");

	if ( (trk1->GetD0() < fD0_K_min) || (trk1->GetD0() > fD0_K_max) )
	    continue;
	SetCutFlow("Kaon D0");
	
	if ( (trk2->GetD0() < fD0_Pi_min) || (trk2->GetD0() > fD0_Pi_max) )
	    continue;
	SetCutFlow("Pion D0");

	if ( TMath::Abs(trk1->GetZ0()-trk2->GetZ0()) >= fZDiff_max ) continue;
	SetCutFlow("Z0_Diff");
	
	if (IsConversion(trk1, trk2))
	    continue;
	SetCutFlow("No #gamma");
	
	// reject event, if it was also found by the K0s finder
	if (IsKaon(trk1, trk2))
	    continue;
	SetCutFlow("No K0s");
	
	if (IsLambda(trk1, trk2))
	    continue;
	SetCutFlow("No Lambdas");

	TLorentzVector p1, p2, p12, p21;
	if (trk1->Pt() > trk2->Pt()) {
	    p1.SetVectM(trk1->P(), fm_pi);
	    p2.SetVectM(trk2->P(), fm_kaon);
	    p12 = p1 + p2;
	    fHistD0reco_m_kaon_pi->Fill(p12.M(), GetPreTagEvtWeight());
	} else {
	    p1.SetVectM(trk2->P(), fm_kaon);
	    p2.SetVectM(trk1->P(), fm_pi);
	    p21 = p1 + p2;
	    fHistD0reco_m_kaon_pi->Fill(p21.M(), GetPreTagEvtWeight());
	}

	if ( ((p12.M() < fD0_M_min) || (p12.M() > fD0_M_max)) &&
	     ((p21.M() < fD0_M_min) || (p21.M() > fD0_M_max)) ) continue;
	SetCutFlow("D0 mass wnd");

	// Definition of covariance matrices, taken from track fit
	trk1->GetCovMatrixPtEtaPhi(cov_trk1);
	trk2->GetCovMatrixPtEtaPhi(cov_trk2);

	// Define momentum used as output of the fit
	TLorentzVector FitP_trk1k;
	TLorentzVector FitP_trk2pi;
	TKinFitter fitterKPi = PerformFit(trk1, trk2, FitP_trk1k, FitP_trk2pi, &cov_trk1, &cov_trk2);
	
	TLorentzVector FitP_trk1pi;
	TLorentzVector FitP_trk2k;
	TKinFitter fitterPiK = PerformFit(trk2, trk1, FitP_trk1pi, FitP_trk2k, &cov_trk2, &cov_trk1);
          
	// abort, if a fit failed severely (chi2 < 0)
	if ( (fitterKPi.getS() < 0.) || (fitterPiK.getS() < 0.) ) {
	    Error("ReconstructLambdaKinFit", "fitter.getS()<0. Abort!");
	    gSystem->Abort(0);
	}
	// skip event, if neither fit converged
	Bool_t convKPi = (fitterKPi.getStatus() == 0);
	Bool_t convPiK = (fitterPiK.getStatus() == 0);
	if ( (!convKPi) && (!convPiK) ) continue;
	SetCutFlow("#geq 1 Fit ok");
	fN_Fits++;

	// Reconstruct Lambda 4-momentum
	// and set Chi2 and NDoF from the converged fit.
	// If both fits converged, prefer the lower chi2/ndof.
	Float_t Chi2overNDoF_KPi = fitterKPi.getS()/fitterKPi.getNDF();
	Float_t Chi2overNDoF_PiK = fitterPiK.getS()/fitterPiK.getNDF();
	HepParticle Fit_Daughter1; // save momenta from the better fit
	HepParticle Fit_Daughter2; // is filled in the following if-condition
	if ( convKPi && ( (!convPiK) || ( Chi2overNDoF_KPi <= Chi2overNDoF_PiK ) ) ) {
	    p_D0 = FitP_trk1k + FitP_trk2pi;
	    fChi2 = fitterKPi.getS();
	    fNDoF = fitterKPi.getNDF();
	    HepParticle FitDaughter1(1, FitP_trk1k.Px(), FitP_trk1k.Py(), FitP_trk1k.Pz(), 
				     FitP_trk1k.E(),  (trk1->GetQovP() < 0.) ? -321 : 321);
	    HepParticle FitDaughter2(2, FitP_trk2pi.Px(), FitP_trk2pi.Py(), FitP_trk2pi.Pz(), 
				     FitP_trk2pi.E(), (trk2->GetQovP() < 0.) ? -211 : 211);  
	    Fit_Daughter1 = FitDaughter1;
	    Fit_Daughter2 = FitDaughter2;
	} else {
	    p_D0 = FitP_trk1pi + FitP_trk2k;
	    fChi2 = fitterPiK.getS();
	    fNDoF = fitterPiK.getNDF();
	    HepParticle FitDaughter1(1, FitP_trk1pi.Px(), FitP_trk1pi.Py(), FitP_trk1pi.Pz(), 
				     FitP_trk1pi.E(), (trk1->GetQovP() < 0.) ? -211 : 211);
	    HepParticle FitDaughter2(2, FitP_trk2k.Px(), FitP_trk2k.Py(), FitP_trk2k.Pz(), 
				     FitP_trk2k.E(),  (trk2->GetQovP() < 0.) ? -321 : 321);
	    Fit_Daughter1 = FitDaughter1;
	    Fit_Daughter2 = FitDaughter2;
	    // save Kaon track as first track always
	    AtlIDTrack *tmp = trk2;
	    trk2 = trk1;
	    trk1 = tmp;
	}
	PvalueKinFit = TMath::Prob(fChi2, fNDoF);

	if ((p_D0.Eta() < fSignal_Eta_min) || (p_D0.Eta() > fSignal_Eta_max))
	    continue;
	SetCutFlow("D0 eta");

	if ( TMath::Abs(Fit_Daughter1.P().Angle(Fit_Daughter2.P3())) > fOAngle_max )
	    continue;
	SetCutFlow("OAngle");

	if ((p_D0.Pt() < fSignal_Pt_min) || (p_D0.Pt() > fSignal_Pt_max))
	    continue;
	SetCutFlow("D0 Pt");
	
	// Add Lambda to event
	if (PvalueKinFit >= fKinFitPvalue_min) {
	    SetCutFlow("KinFit P-value");
	    
	    AtlD0DecayKPi *decay;
	    if ( trk1->Charge()*trk2->Charge() < 0. ) { // require opposite sign
		SetCutFlow("Charge ok");

		decay = fEvent->AddD0DecayKPi(p_D0.Px(), p_D0.Py(), p_D0.Pz(), p_D0.E(),
					      trk1, trk2, PrimVtx, PrimVtx, Fit_Daughter1, Fit_Daughter2);
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
		    if ((trk1->GetMCParticle()->GetMother() != NULL) && (trk2->GetMCParticle()->GetMother() != NULL))
			fDebugStream << ((trk1->GetMCParticle()->GetMother() == trk2->GetMCParticle()->GetMother()) ? "SAME" : "DIFF");
		    fDebugStream  << endl << "Track1: " << trk1->GetMCParticle()->GetPdgName() << " (" << trk1->GetMCParticle()->GetPdgCode() << ")"
				  << endl << "Track2: " << trk2->GetMCParticle()->GetPdgName() << " (" << trk2->GetMCParticle()->GetPdgCode() << ")";
		}
	    } else {
		// Add equally charged or mc-failed combinations to the bkg list
		decay = new AtlD0DecayKPi(BkgCount++, p_D0.Px(), p_D0.Py(), p_D0.Pz(), p_D0.E(),
					  trk1, trk2, PrimVtx, PrimVtx, 
					  Fit_Daughter1, Fit_Daughter2);
		decay->SetChi2NDoF(fChi2, fNDoF);
		fBkgLambdaDecays->Add(decay);
	    }
	}
    }
  } //end of vertex loop

  FindSharedTracks();
}

//____________________________________________________________________    

void AtlD0Finder::FindSharedTracks() {

    AtlD0DecayKPi *decay, *decay2;
    
    for ( Int_t i = 0; i < fEvent->GetN_D0DecaysKPi(); i++ ) {
      decay = (AtlD0DecayKPi*)fEvent->GetD0DecaysKPi()->At(i);

      for (Int_t j = i+1; j < fEvent->GetN_D0DecaysKPi(); j++) {
	  decay2 = (AtlD0DecayKPi*)fEvent->GetD0DecaysKPi()->At(j);
	  if (  (decay->GetKaon() == decay2->GetKaon()) ||
		(decay->GetKaon() == decay2->GetPion()) ||
		(decay->GetPion() == decay2->GetKaon()) ||
		(decay->GetPion() == decay2->GetPion()) ) {
	      decay->SetSharedTracks(kTRUE);
	      decay2->SetSharedTracks(kTRUE);
	  }
      }

/*      for (Int_t j = 0; j < fBkgLambdaDecays->GetEntries(); j++) {
	  decay2 = (AtlD0DecayKPi*)fBkgLambdaDecays->At(j);
	  if (  (decay->GetKaon() == decay2->GetKaon()) ||
		(decay->GetKaon() == decay2->GetPion()) ||
		(decay->GetPion() == decay2->GetKaon()) ||
		(decay->GetPion() == decay2->GetPion()) ) {
	      decay->SetSharedTracks(kTRUE);
	      decay2->SetSharedTracks(kTRUE);
	  }
	  } */
      
    }
}

//____________________________________________________________________    

TKinFitter AtlD0Finder::PerformFit(AtlIDTrack *trk1, AtlIDTrack *trk2, TLorentzVector& FitP_trk1, TLorentzVector& FitP_trk2,
				   TMatrixD *cov_trk1, TMatrixD *cov_trk2) {
  //
  // Perform a kinematic fit with the given tracks to test the lambda mass hyptothesis
  // assuming that track1 is the proton and track2 is the pion.
  //

  // Define particles necessary for execution of the fit
  // Using proton/pion mass hypothesis; fit variables are pt, eta, phi
  TVector3 dummy1 = trk1->P(); // instead of const cast
  TFitParticlePtEtaPhi FitExec_trk1("FitExec_trk1", "FitExec_trk1 for D0Fit", &dummy1, fm_kaon, cov_trk1);
  TVector3 dummy2 = trk2->P(); // instead of const cast
  TFitParticlePtEtaPhi FitExec_trk2("FitExec_trk2", "FitExec_trk2 for D0Fit", &dummy2, fm_pi, cov_trk2);
  
  // Definition of Lambda mass constraint
  TFitConstraintM MD0Cons("D0MassConstraint","D0MassConstraintGaus", 0, 0, fm_D0);
  MD0Cons.addParticles1(&FitExec_trk1, &FitExec_trk2);
    
  // fitting characteristics
  TKinFitter fitter;
  fitter.addMeasParticle(&FitExec_trk1);
  fitter.addMeasParticle(&FitExec_trk2);
  fitter.addConstraint(&MD0Cons);
    
  fitter.setMaxNbIter(50);   // maximum number of iterations
  fitter.setMaxDeltaS(5e-5); // maximum deviation of the minimum function within two iterations
  fitter.setMaxF(1e-4);      // maximum value of constraints
  fitter.setVerbosity(0);    // verbosity level
    
  // Kinematic Fitting
  fitter.fit();

  // Copy fitted 4-vectors (circumvent const cast)
  FitP_trk1 = *FitExec_trk1.getCurr4Vec();
  FitP_trk2 = *FitExec_trk2.getCurr4Vec();

  return fitter;
}


//____________________________________________________________________    

void AtlD0Finder::FillMCHistograms(AtlD0DecayKPi *decay, Bool_t signal) {
    //
    // Fill histograms for MC signal/background comparison.
    // Set signal to false to fill background histograms.
    //

    TLorentzVector p_trk1, p_trk2, p_ee, p_kpi, p_pipi, p_ee_o, p_kpi_o, p_pipi_o;
    p_trk1.SetVectM(decay->GetReFitKaon().P().Vect(), fm_lambda);
    p_trk2.SetVectM(decay->GetReFitPion().P().Vect(), fm_e);
    p_ee = p_trk1 + p_trk2;
    p_trk1.SetVectM(decay->GetReFitKaon().P().Vect(), fm_lambda);
    p_trk2.SetVectM(decay->GetReFitPion().P().Vect(), fm_pi);
    p_pipi = p_trk1 + p_trk2;
    p_trk1.SetVectM(decay->GetReFitKaon().P().Vect(), fm_kaon);
    p_kpi = p_trk1 + p_trk2;

    p_trk1.SetVectM(decay->GetKaon()->P(), fm_lambda);
    p_trk2.SetVectM(decay->GetPion()->P(), fm_e);
    p_ee_o = p_trk1 + p_trk2;
    p_trk1.SetVectM(decay->GetKaon()->P(), fm_lambda);
    p_trk2.SetVectM(decay->GetPion()->P(), fm_pi);
    p_pipi_o = p_trk1 + p_trk2;
    p_trk1.SetVectM(decay->GetKaon()->P(), fm_kaon);
    p_kpi_o = p_trk1 + p_trk2;

    if (signal) {
	fHistLambdareco_MC_m       ->Fill(decay->M("Reco"),     GetPreTagEvtWeight());
	fHistLambdareco_MC_pt      ->Fill(decay->Pt(),          GetPreTagEvtWeight());
	fHistLambdareco_MC_phi     ->Fill(decay->Phi(),         GetPreTagEvtWeight());
	fHistLambdareco_MC_eta     ->Fill(decay->Eta(),         GetPreTagEvtWeight());
	fHistLambdareco_MC_tau     ->Fill(decay->GetLifeTime(), GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_ee    ->Fill(p_ee.M(),             GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_pipi  ->Fill(p_pipi.M(),           GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_ppi   ->Fill(p_kpi.M(),            GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_ee_o  ->Fill(p_ee_o.M(),           GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_pipi_o->Fill(p_pipi_o.M(),         GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_ppi_o ->Fill(p_kpi_o.M(),          GetPreTagEvtWeight());
	fHistLambdareco_MC_R_vtx   ->Fill(decay->GetVtx()->GetPos().Perp(), GetPreTagEvtWeight());
	fHistLambdareco_MC_src_div ->Fill(decay->GetAngleToPrimary(),       GetPreTagEvtWeight());
	fHistLambdareco_MC_tdcy_len->Fill(decay->GetTransvDecayLength(),    GetPreTagEvtWeight());
	fHistLambdareco_MC_oangle  ->Fill(decay->GetOpeningAngle(),         GetPreTagEvtWeight());
	fHistLambdareco_MC_coangle ->Fill(TMath::Cos(decay->GetOpeningAngle()), GetPreTagEvtWeight());
	fHistLambdareco_MC_dangle  ->Fill(TMath::Cos(decay->GetDecayAngle()),   GetPreTagEvtWeight());
	fHistLambdareco_MC_m_dangle->Fill(TMath::Cos(decay->GetDecayAngle()), decay->M("Reco"), GetPreTagEvtWeight());
	
 	fHistLambdareco_MC_d0_K    ->Fill(decay->GetKaon()->GetD0(), GetPreTagEvtWeight());
	fHistLambdareco_MC_d0_Pi   ->Fill(decay->GetPion()->GetD0(), GetPreTagEvtWeight());
	fHistLambdareco_MC_z0_K    ->Fill(decay->GetKaon()->GetZ0(), GetPreTagEvtWeight());
	fHistLambdareco_MC_z0_Pi   ->Fill(decay->GetPion()->GetZ0(), GetPreTagEvtWeight());
	fHistLambdareco_MC_z0_diff ->Fill(TMath::Abs(decay->GetKaon()->GetZ0() - decay->GetPion()->GetZ0()), GetPreTagEvtWeight());
	fHistLambdareco_MC_k_tchi2 ->Fill(decay->GetKaon()->GetChi2() / decay->GetKaon()->GetNDoF(), GetPreTagEvtWeight());
	fHistLambdareco_MC_pi_tchi2->Fill(decay->GetPion()->GetChi2() / decay->GetPion()->GetNDoF(), GetPreTagEvtWeight());
 	fHistLambdareco_MC_d0ovE_K ->Fill(decay->GetKaon()->GetD0() / TMath::Sqrt(decay->GetKaon()->GetCovMatrix()[0]), GetPreTagEvtWeight());
	fHistLambdareco_MC_d0ovE_Pi->Fill(decay->GetPion()->GetD0() / TMath::Sqrt(decay->GetPion()->GetCovMatrix()[0]), GetPreTagEvtWeight());
    } else {
	fHistLambdareco_MC_m_bkg       ->Fill(decay->M("Reco"),     GetPreTagEvtWeight());
	fHistLambdareco_MC_pt_bkg      ->Fill(decay->Pt(),          GetPreTagEvtWeight());
	fHistLambdareco_MC_phi_bkg     ->Fill(decay->Phi(),         GetPreTagEvtWeight());
	fHistLambdareco_MC_eta_bkg     ->Fill(decay->Eta(),         GetPreTagEvtWeight());
	fHistLambdareco_MC_tau_bkg     ->Fill(decay->GetLifeTime(), GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_ee_bkg    ->Fill(p_ee.M(),             GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_pipi_bkg  ->Fill(p_pipi.M(),           GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_ppi_bkg   ->Fill(p_kpi.M(),            GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_ee_o_bkg  ->Fill(p_ee_o.M(),           GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_pipi_o_bkg->Fill(p_pipi_o.M(),         GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_ppi_o_bkg ->Fill(p_kpi_o.M(),          GetPreTagEvtWeight());
	fHistLambdareco_MC_R_vtx_bkg   ->Fill(decay->GetVtx()->GetPos().Perp(), GetPreTagEvtWeight());
	fHistLambdareco_MC_src_div_bkg ->Fill(decay->GetAngleToPrimary(),       GetPreTagEvtWeight());
	fHistLambdareco_MC_tdcy_len_bkg->Fill(decay->GetTransvDecayLength(),    GetPreTagEvtWeight());
	fHistLambdareco_MC_oangle_bkg  ->Fill(decay->GetOpeningAngle(),         GetPreTagEvtWeight());
	fHistLambdareco_MC_coangle_bkg ->Fill(TMath::Cos(decay->GetOpeningAngle()), GetPreTagEvtWeight());
	fHistLambdareco_MC_dangle_bkg  ->Fill(TMath::Cos(decay->GetDecayAngle()),   GetPreTagEvtWeight());
	fHistLambdareco_MC_m_dangle_bkg->Fill(TMath::Cos(decay->GetDecayAngle()), decay->M("Reco"), GetPreTagEvtWeight());

     	fHistLambdareco_MC_d0_K_bkg    ->Fill(decay->GetKaon()->GetD0(), GetPreTagEvtWeight());
	fHistLambdareco_MC_d0_Pi_bkg   ->Fill(decay->GetPion()->GetD0(), GetPreTagEvtWeight());
	fHistLambdareco_MC_z0_K_bkg    ->Fill(decay->GetKaon()->GetZ0(), GetPreTagEvtWeight());
	fHistLambdareco_MC_z0_Pi_bkg   ->Fill(decay->GetPion()->GetZ0(), GetPreTagEvtWeight());
	fHistLambdareco_MC_z0_diff_bkg ->Fill(TMath::Abs(decay->GetKaon()->GetZ0() - decay->GetPion()->GetZ0()), GetPreTagEvtWeight());
	fHistLambdareco_MC_k_tchi2_bkg ->Fill(decay->GetKaon()->GetChi2() / decay->GetKaon()->GetNDoF(), GetPreTagEvtWeight());
	fHistLambdareco_MC_pi_tchi2_bkg->Fill(decay->GetPion()->GetChi2() / decay->GetPion()->GetNDoF(), GetPreTagEvtWeight());
 	fHistLambdareco_MC_d0ovE_K_bkg ->Fill(decay->GetKaon()->GetD0() / TMath::Sqrt(decay->GetKaon()->GetCovMatrix()[0]), GetPreTagEvtWeight());
	fHistLambdareco_MC_d0ovE_Pi_bkg->Fill(decay->GetPion()->GetD0() / TMath::Sqrt(decay->GetPion()->GetCovMatrix()[0]), GetPreTagEvtWeight());
    }    
}


//____________________________________________________________________    


void AtlD0Finder::FillHistograms() {
    //
    // Fill histograms
    //

    AtlD0DecayKPi *Lambda = 0;
    
    // Proton/Pion variables
    AtlIDTrack *Proton =0;
    AtlIDTrack *Pion   =0;
    TLorentzVector p_trk1, p_trk2, p_Lambda;

    // Signal (opposite charged)
    // ________________________________________

/*    Int_t shareSig;
      Int_t shareBkg;*/
    for ( Int_t i = 0; i < fEvent->GetN_D0DecaysKPi(); i++ ) {
      Lambda = (AtlD0DecayKPi*)fEvent->GetD0DecaysKPi()->At(i);

      if (Lambda->HasSharedTracks())
	  continue;
      SetCutFlow("Shared tracks");

      // Hitbased TruthMatching
      SetChi2( Lambda->GetChi2() );
      SetNDoF( Lambda->GetNDoF() );
      if (fEvent->IsMC()) {
	  switch ( DoTruthMatch(Lambda->GetKaon(), Lambda->GetPion(), NULL) ) {
	      case 1:   fTrueReco = kTRUE;  FillMCHistograms(Lambda, kTRUE);  AtlKinFitterTool::FillHistograms("Signal"); break;
	      case -1:  fTrueReco = kFALSE; FillMCHistograms(Lambda, kFALSE); AtlKinFitterTool::FillHistograms("Signal"); break;
	  }
      } else {
	  AtlKinFitterTool::FillHistograms("Signal");
      }
		
/*
      DEBUG_OUTPUT

      shareSig = 0;
      shareBkg = 0;
      for (Int_t j = 0; j < fEvent->GetN_D0DecaysKPi(); j++)
	  if (i != j) {
	      Lambda2 = (AtlD0DecayKPi*)fEvent->GetD0DecaysKPi()->At(j);
	      if (Lambda->GetKaon() == Lambda2->GetKaon()) if (Lambda2->GetMCDecay() != NULL) shareSig++; else shareBkg++;
	      if (Lambda->GetKaon() == Lambda2->GetPion()) if (Lambda2->GetMCDecay() != NULL) shareSig++; else shareBkg++;
	      if (Lambda->GetPion() == Lambda2->GetKaon()) if (Lambda2->GetMCDecay() != NULL) shareSig++; else shareBkg++;
	      if (Lambda->GetPion() == Lambda2->GetPion()) if (Lambda2->GetMCDecay() != NULL) shareSig++; else shareBkg++;
	  }
      if (shareSig > 0)
	  cout << endl << "MC " << (Lambda->GetMCDecay() != NULL ? "Signal" : "Background") << " decay shares a track with " << shareSig << " other signal decays!";
      if (shareBkg > 0)
	  cout << endl << "MC " << (Lambda->GetMCDecay() != NULL ? "Signal" : "Background") << " decay shares a track with " << shareBkg << " other background decays!";
*/
      fHistLambdareco_m_PPi     ->Fill(Lambda->M("Reco"),    GetPreTagEvtWeight());
      fHistLambdareco_pt_PPi    ->Fill(Lambda->Pt(),         GetPreTagEvtWeight());
      fHistLambdareco_phi_PPi   ->Fill(Lambda->Phi(),        GetPreTagEvtWeight());
      fHistLambdareco_eta_PPi   ->Fill(Lambda->Eta(),        GetPreTagEvtWeight());
      fHistLambdareco_tau_PPi   ->Fill(Lambda->GetLifeTime(),               GetPreTagEvtWeight());
      fHistLambdareco_R_vtx     ->Fill(Lambda->GetVtx()->GetPos().Perp(),   GetPreTagEvtWeight());
      fHistLambdareco_tdcy_len  ->Fill(Lambda->GetTransvDecayLength(),      GetPreTagEvtWeight());
      fHistLambdareco_oangle    ->Fill(Lambda->GetOpeningAngle(),           GetPreTagEvtWeight());
      fHistLambdareco_dangle    ->Fill(TMath::Cos(Lambda->GetDecayAngle()), GetPreTagEvtWeight());

      Proton = Lambda->GetKaon();
      Pion  =  Lambda->GetPion();

      // test pi/pi hypothesis
      p_trk1.SetVectM(Proton->P(), fm_kaon);
      p_trk2.SetVectM(Pion->P(),   fm_pi);
      p_Lambda = p_trk1 + p_trk2;
      fHistLambdareco_m_PiPi->Fill(p_Lambda.M(), GetPreTagEvtWeight());

      // test e/e hypothesis
      p_trk1.SetVectM(Proton->P(), fm_e);
      p_trk2.SetVectM(Pion->P(),   fm_e);
      p_Lambda = p_trk1 + p_trk2;
      fHistLambdareco_m_ee->Fill(p_Lambda.M(), GetPreTagEvtWeight());

      fHistLambdareco_chi2_vtx->Fill(Lambda->GetVtx()->GetChi2overNDoF(), GetPreTagEvtWeight());
      fHistLambdareco_chi2_trk->Fill(Proton->Chi2ovNDoF(), GetPreTagEvtWeight());
      fHistLambdareco_chi2_trk->Fill(Pion->Chi2ovNDoF(), GetPreTagEvtWeight());

      fHistLambdareco_Pion_pt     ->Fill(Proton->Pt(),  GetPreTagEvtWeight());
      fHistLambdareco_Proton_pt   ->Fill(Pion->Pt(),    GetPreTagEvtWeight());
      fHistLambdareco_Pion_eta    ->Fill(Proton->Eta(), GetPreTagEvtWeight());
      fHistLambdareco_Proton_eta  ->Fill(Pion->Eta(),   GetPreTagEvtWeight());
      fHistLambdareco_Pion_phi    ->Fill(Proton->Phi(), GetPreTagEvtWeight());
      fHistLambdareco_Proton_phi  ->Fill(Pion->Phi(),   GetPreTagEvtWeight());
      // charge separated histograms
      if (Pion->QovP() < 0.) {
        fHistLambdareco_Piminus_pt  ->Fill(Pion->Pt(),    GetPreTagEvtWeight());
        fHistLambdareco_Piminus_eta ->Fill(Pion->Eta(),   GetPreTagEvtWeight());
        fHistLambdareco_Piminus_phi ->Fill(Pion->Phi(),   GetPreTagEvtWeight());
        fHistLambdareco_Prplus_pt   ->Fill(Proton->Pt(),  GetPreTagEvtWeight());
        fHistLambdareco_Prplus_eta  ->Fill(Proton->Eta(), GetPreTagEvtWeight());
        fHistLambdareco_Prplus_phi  ->Fill(Proton->Phi(), GetPreTagEvtWeight());
      } else {
        fHistLambdareco_Piplus_pt   ->Fill(Pion->Pt(),    GetPreTagEvtWeight());
        fHistLambdareco_Piplus_eta  ->Fill(Pion->Eta(),   GetPreTagEvtWeight());
        fHistLambdareco_Piplus_phi  ->Fill(Pion->Phi(),   GetPreTagEvtWeight());
        fHistLambdareco_Prminus_pt  ->Fill(Proton->Pt(),  GetPreTagEvtWeight());
        fHistLambdareco_Prminus_eta ->Fill(Proton->Eta(), GetPreTagEvtWeight());
        fHistLambdareco_Prminus_phi ->Fill(Proton->Phi(), GetPreTagEvtWeight());
      }
    }

    fHistLambdareco_N_PPi ->Fill(fEvent->GetN_D0DecaysKPi(), GetPreTagEvtWeight());
    

    // Combinatorial Background (same charged)
    // ____________________________________________

    TIter next_bkg(fBkgLambdaDecays);
    while ( ( Lambda = (AtlD0DecayKPi*)next_bkg() ) ) {
      if (Lambda->HasSharedTracks())
	  continue;
      SetChi2( Lambda->GetChi2() );
      SetNDoF( Lambda->GetNDoF() );
      AtlKinFitterTool::FillHistograms("Bkg");
	
      fHistLambdareco_m_PPi_bkg     ->Fill(Lambda->M("Reco"),      GetPreTagEvtWeight());
      fHistLambdareco_pt_PPi_bkg    ->Fill(Lambda->Pt(),           GetPreTagEvtWeight());
      fHistLambdareco_phi_PPi_bkg   ->Fill(Lambda->Phi(),          GetPreTagEvtWeight());
      fHistLambdareco_eta_PPi_bkg   ->Fill(Lambda->Eta(),          GetPreTagEvtWeight());
      fHistLambdareco_tau_PPi_bkg   ->Fill(Lambda->GetLifeTime(),  GetPreTagEvtWeight());
      fHistLambdareco_R_vtx_bkg     ->Fill(Lambda->GetVtx()->GetPos().Perp(),   GetPreTagEvtWeight());
      fHistLambdareco_tdcy_len_bkg  ->Fill(Lambda->GetTransvDecayLength(),      GetPreTagEvtWeight());
      fHistLambdareco_oangle_bkg    ->Fill(Lambda->GetOpeningAngle(),           GetPreTagEvtWeight());
      fHistLambdareco_dangle_bkg    ->Fill(TMath::Cos(Lambda->GetDecayAngle()), GetPreTagEvtWeight());

      Proton = Lambda->GetKaon();
      Pion  =  Lambda->GetPion();
      
      fHistLambdareco_Pion_pt_bkg     ->Fill(Proton->Pt(),  GetPreTagEvtWeight());
      fHistLambdareco_Proton_pt_bkg   ->Fill(Pion->Pt(),    GetPreTagEvtWeight());
      fHistLambdareco_Pion_eta_bkg    ->Fill(Proton->Eta(), GetPreTagEvtWeight());
      fHistLambdareco_Proton_eta_bkg  ->Fill(Pion->Eta(),   GetPreTagEvtWeight());
      fHistLambdareco_Pion_phi_bkg    ->Fill(Proton->Phi(), GetPreTagEvtWeight());
      fHistLambdareco_Proton_phi_bkg  ->Fill(Pion->Phi(),   GetPreTagEvtWeight());
      // charge separated histograms
      if (Pion->QovP() < 0.) {
        fHistLambdareco_Piminus_pt_bkg  ->Fill(Pion->Pt(),    GetPreTagEvtWeight());
        fHistLambdareco_Piminus_eta_bkg ->Fill(Pion->Eta(),   GetPreTagEvtWeight());
        fHistLambdareco_Piminus_phi_bkg ->Fill(Pion->Phi(),   GetPreTagEvtWeight());
        fHistLambdareco_Prminus_pt_bkg  ->Fill(Proton->Pt(),  GetPreTagEvtWeight());
        fHistLambdareco_Prminus_eta_bkg ->Fill(Proton->Eta(), GetPreTagEvtWeight());
        fHistLambdareco_Prminus_phi_bkg ->Fill(Proton->Phi(), GetPreTagEvtWeight());
      } else {
        fHistLambdareco_Piplus_pt_bkg   ->Fill(Pion->Pt(),    GetPreTagEvtWeight());
        fHistLambdareco_Piplus_eta_bkg  ->Fill(Pion->Eta(),   GetPreTagEvtWeight());
        fHistLambdareco_Piplus_phi_bkg  ->Fill(Pion->Phi(),   GetPreTagEvtWeight());
        fHistLambdareco_Prplus_pt_bkg   ->Fill(Proton->Pt(),  GetPreTagEvtWeight());
        fHistLambdareco_Prplus_eta_bkg  ->Fill(Proton->Eta(), GetPreTagEvtWeight());
        fHistLambdareco_Prplus_phi_bkg  ->Fill(Proton->Phi(), GetPreTagEvtWeight());
      }
    }

    fHistLambdareco_N_PPi_bkg ->Fill(fBkgLambdaDecays->GetEntries(), GetPreTagEvtWeight());
}

//____________________________________________________________________

void AtlD0Finder::SetCutDefaults() {
    //
    // Selection cut defaults and Constraint parameters
    //

    fSignal_Pt_min       =  0.5;     // Min. Pt of signal pion/proton
    fSignal_Pt_max       =  1.e10;   // Max. Pt of signal pion/proton
    
    fm_lambda            = 1.115683;       // Lambda mass hypothesis
    fm_pi                = 0.13957;        // Pion mass hypothesis
    fm_proton            = 0.935;          // Proton mass hypothesis
    fm_e                 = 0.000510998910; // Electron mass hypothesis
    fm_kaon              = 0.493667;
    fm_D0                = 1.86484;
    fm_Dstar             = 2.01027;
}

//____________________________________________________________________


void AtlD0Finder::PrintCutValues() {
        
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
    cout << scientific << " " << fD0_M_min << " < M(Lbd) < " << fD0_M_max << endl;
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


void AtlD0Finder::Terminate() {

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

  cout << endl << "INFO: Terminating AtlD0Finder!";
  cout << endl << "        # Successful fits: " << fN_Fits;
  cout << endl << "        # Truth match bad: " << fN_MCProbMiss;
  cout << endl << "   # TM with diff. origin: " << fN_SameOriginMiss;
  cout << endl << "     # TM => wrong tracks: " << fN_MCFail;
  cout << endl << " # Possible Lambdas found: " << TMath::Nint(fHistLambdareco_m_PPi->GetEntries());
    
}

//____________________________________________________________________

Int_t AtlD0Finder::DoTruthMatch(AtlIDTrack *trk1, AtlIDTrack *trk2, HepVertex *Vtx) {
    //
    // retuns 1 in case of true Lambdas, -1 if not
    // returns 0 if no decision possible (see fMatchingProb_min)
    //

    fHistLambdareco_prob_mc->Fill(trk1->GetMCParticleProb(), GetPreTagEvtWeight());
    fHistLambdareco_prob_mc->Fill(trk2->GetMCParticleProb(), GetPreTagEvtWeight());

    // perform truth matching when analyzing MC samples
    if ( trk1->HasValidTruthMatch(fMatchingProb_min) && trk2->HasValidTruthMatch(fMatchingProb_min)) {
	
	HepMCParticle *MC1 = trk1->GetMCParticle();
	HepMCParticle *MC2 = trk2->GetMCParticle();

	// Perform null pointer check on mother particle. Yes, there are events with MC particles without a mother!
	if ((MC1->GetMother() == NULL) || (MC2->GetMother() == NULL)) {
	    return 0;
	}

	// remove "_bar" from particle name to collect antiparticle and particle in the same bin
	const char* fullPdgName = MC1->GetMother()->GetPdgName();
	char pdgName[strlen(fullPdgName)];
	if (strstr(fullPdgName, "_bar") == NULL) {
	    strcpy(pdgName, fullPdgName);
	} else {
	    strncpy(pdgName, fullPdgName, strlen(fullPdgName)-4);
	    pdgName[strlen(fullPdgName)-4] = '\0';
	}

	// decide truth match
	AtlD0DecayKPi *decay = (AtlD0DecayKPi*)fEvent->GetD0DecaysKPi()->Last();
	Float_t prob = decay->GetProb();
	if ( (TMath::Abs(MC1->GetMother()->GetPdgCode()) == 421) &&
	     (MC1->GetMother() == MC2->GetMother()) &&
	     (TMath::Abs(MC1->GetPdgCode()) == 321) &&
	     (TMath::Abs(MC2->GetPdgCode()) == 211) ) {

	    fHistLambdareco_truth_PPi->Fill(pdgName, prob, GetPreTagEvtWeight());
	    return 1;
	} else {
	    
	    if (MC1->GetMother() != MC2->GetMother()) {
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

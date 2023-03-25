//____________________________________________________________________
//
// Lambda decay finder class
//

//
// Author: Thomas Kintscher <mailto:kintscher@physik.hu-berlin.de>
// Copyright: 2010 (C) Thomas Kintscher
//
#ifndef ATLAS_AtlKaonHistos
#include <AtlKaonHistos.h>
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
ClassImp(AtlKaonHistos);
#endif

//____________________________________________________________________

AtlKaonHistos::AtlKaonHistos(const char* name, const char* title) :
    AtlKinFitterTool(name, title) {
    //
    // Default constructor
    //
    SetMode(kKinFit);
    SetDebugOutput(kFALSE);
    fBkgLambdaDecays = new TList;
}

//____________________________________________________________________

AtlKaonHistos::~AtlKaonHistos() {
    //
    // Default destructor
    //
    fBkgLambdaDecays->Delete(); delete fBkgLambdaDecays;
    if (IsDebugRun() ) fDebugStream.close();
}

//____________________________________________________________________

void AtlKaonHistos::SetBranchStatus() {
    //
    // Switch on needed branches
    //
    fTree->SetBranchStatus("fN_IDTracks*",         kTRUE);  
    fTree->SetBranchStatus("fIDTracks*",           kTRUE);
    fTree->SetBranchStatus("fN_Vertices*",         kTRUE);
    fTree->SetBranchStatus("fVertices*",           kTRUE);
}

//____________________________________________________________________

void AtlKaonHistos::BookHistograms() {
    //
    // Book histograms
    //
    if ( IsDebugRun() ) fDebugStream.open("DebugOutput.dat");
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
      fHistLambdareco_m_PPi   = new TH1F("h_Lambda_m_PPi", "Invariant Lambda mass (Lambda->PPi)", 100, 1.0, 1.2);
    } else if ( fMode == kKinFit ) {
      fHistLambdareco_m_PPi   = new TH1F("h_Lambda_m_PPi", "Invariant Lambda mass (Lambda->PPi)", 1800, 0.0, 1.81571);
    }
    fHistLambdareco_m_PPi->SetXTitle("m_{Lambda} (GeV)"); fHistLambdareco_m_PPi->SetYTitle("Number of Entries");
    fHistLambdareco_m_PiPi  = new TH1F("h_Lambda_m_PiPi", "Invariant mass (* #rightarrow #pi#pi)", 480, 0.0, 1.2);
    fHistLambdareco_m_PiPi->SetXTitle("m (Gev)");         fHistLambdareco_m_PiPi->SetYTitle("Number of Entries");
    fHistLambdareco_m_ee  = new TH1F("h_Lambda_m_ee", "Invariant mass (* #rightarrow e e)", 480, 0.0, 1.2);
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
      fHistLambdareco_m_PPi_bkg   = new TH1F("h_Lambda_m_PPi_bkg", "Invariant Lambda' mass (Lambda->PPi)", 50, 1.1, 1.14);
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
      fHistLambdareco_MC_m   = new TH1F("h_Lambda_MC_m", "Invariant #Lambda mass (MC signal)", 50, 1.1, 1.14);
    }
    fHistLambdareco_MC_m->SetXTitle("m_{#Lambda} (GeV)");     fHistLambdareco_MC_m->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ppi = new TH1F("h_Lambda_MC_m_ppi", "Invariant mass w/ proton/pion hypothesis (MC signal)", 480, 0.0, 1.2);
    fHistLambdareco_MC_m_ppi->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_ppi->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_pipi = new TH1F("h_Lambda_MC_m_pipi", "Invariant mass w/ pion/pion hypothesis (MC signal)", 480, 0.0, 1.2);
    fHistLambdareco_MC_m_pipi->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_pipi->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ee = new TH1F("h_Lambda_MC_m_ee", "Invariant mass w/ ee hypothesis (MC signal)", 480, 0.0, 1.2);
    fHistLambdareco_MC_m_ee->SetXTitle("m_{#Lambda} (GeV)");  fHistLambdareco_MC_m_ee->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ppi_o = new TH1F("h_Lambda_MC_m_ppi_o", "Invariant mass w/ proton/pion hypothesis (MC signal)", 480, 0.0, 1.2);
    fHistLambdareco_MC_m_ppi_o->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_ppi_o->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_pipi_o = new TH1F("h_Lambda_MC_m_pipi_o", "Invariant mass w/ pion/pion hypothesis (MC signal)", 480, 0.0, 1.2);
    fHistLambdareco_MC_m_pipi_o->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_pipi_o->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ee_o = new TH1F("h_Lambda_MC_m_ee_o", "Invariant mass w/ ee hypothesis (MC signal)", 480, 0.0, 1.2);
    fHistLambdareco_MC_m_ee_o->SetXTitle("m_{#Lambda} (GeV)");  fHistLambdareco_MC_m_ee_o->SetYTitle("Number of Entries");
    fHistLambdareco_MC_pt  = new TH1F("h_Lambda_MC_pt", "Lambda-p_{#perp} (MC signal)", 60, 0, 30);
    fHistLambdareco_MC_pt->SetXTitle("p_{#perp} (GeV)");      fHistLambdareco_MC_pt->SetYTitle("Number of Entries");
    fHistLambdareco_MC_phi = new TH1F("h_Lambda_MC_phi", "Lambda #phi distribution (MC signal)", 128, -3.2, 3.2);
    fHistLambdareco_MC_phi->SetXTitle("#phi (rad)");          fHistLambdareco_MC_phi->SetYTitle("Number of Entries");
    fHistLambdareco_MC_eta = new TH1F("h_Lambda_MC_eta", "Lambda #eta distribution (MC signal)", 100, -3, 3);
    fHistLambdareco_MC_eta->SetXTitle("#eta");                fHistLambdareco_MC_eta->SetYTitle("Number of Entries");
    fHistLambdareco_MC_tau = new TH1F("h_Lambda_MC_tau", "Reconstructed Lambda lifetime (MC signal)", 100, 0, 300);
    fHistLambdareco_MC_tau->SetXTitle("#tau (ps)");           fHistLambdareco_MC_tau->SetYTitle("Number of Entries");
    fHistLambdareco_MC_src_div = new TH1F("h_Lambda_MC_src_div", "Angular distance of #Lambda origin and primary vertex", 300, 0.0, 3.2);
    fHistLambdareco_MC_src_div->SetXTitle("#alpha (rad)");    fHistLambdareco_MC_src_div->SetYTitle("Number of Entries");
    fHistLambdareco_MC_R_vtx = new TH1F("h_Lambda_MC_r_vtx", "Radial distance of secondary vertices", 400, 0.0, 100.0);
    fHistLambdareco_MC_R_vtx->SetXTitle("r_{#perp} (cm)");    fHistLambdareco_MC_R_vtx->SetYTitle("Number of Entries");
    fHistLambdareco_MC_tdcy_len = new TH1F("h_Lambda_MC_tdcy_len", "Transverse decay length (MC signal)", 400, 0.0, 100.0);
    fHistLambdareco_MC_tdcy_len->SetXTitle("s_{#perp} (cm)"); fHistLambdareco_MC_tdcy_len->SetYTitle("Number of Entries");
    fHistLambdareco_MC_oangle = new TH1F("h_Lambda_MC_oangle", "Opening angle of p/pi (MC signal)", 256, 0.0, 3.2);
    fHistLambdareco_MC_oangle->SetXTitle("#alpha (rad)");     fHistLambdareco_MC_oangle->SetYTitle("Number of Entries");
    fHistLambdareco_MC_dangle = new TH1F("h_Lambda_MC_dangle", "Decay angle of p/pi (MC signal)", 256, -1.0, 1.0);
    fHistLambdareco_MC_dangle->SetXTitle("cos(#alpha)");      fHistLambdareco_MC_dangle->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    gDirectory->mkdir("MC_Background");
    gDirectory->cd("MC_Background");
    if ( fMode == kCutBased ) {
      fHistLambdareco_MC_m_bkg   = new TH1F("h_Lambda_MC_m_bkg", "Invariant #Lambda mass (MC background)", 2400, 0.0, 1.2);
    } else {
      fHistLambdareco_MC_m_bkg   = new TH1F("h_Lambda_MC_m_bkg", "Invariant #Lambda mass (MC background)", 50, 1.1, 1.14);
    }
    fHistLambdareco_MC_m_bkg->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ppi_bkg = new TH1F("h_Lambda_MC_m_ppi_bkg", "Invariant mass w/ p#pi hypothesis (MC bkg)", 480, 0.0, 1.2);
    fHistLambdareco_MC_m_ppi_bkg->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_ppi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_pipi_bkg = new TH1F("h_Lambda_MC_m_pipi_bkg", "Invariant mass w/ #pi#pi hypothesis (MC bkg)", 480, 0.0, 1.2);
    fHistLambdareco_MC_m_pipi_bkg->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_pipi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ee_bkg = new TH1F("h_Lambda_MC_m_ee_bkg", "Invariant mass w/ ee hypothesis (MC bkg)", 480, 0.0, 1.2);
    fHistLambdareco_MC_m_ee_bkg->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_ee_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ppi_o_bkg = new TH1F("h_Lambda_MC_m_ppi_o_bkg", "Invariant mass w/ p#pi hypothesis (MC bkg)", 480, 0.0, 1.2);
    fHistLambdareco_MC_m_ppi_o_bkg->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_ppi_o_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_pipi_o_bkg = new TH1F("h_Lambda_MC_m_pipi_o_bkg", "Invariant mass w/ #pi#pi hypothesis (MC bkg)", 480, 0.0, 1.2);
    fHistLambdareco_MC_m_pipi_o_bkg->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_pipi_o_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_m_ee_o_bkg = new TH1F("h_Lambda_MC_m_ee_o_bkg", "Invariant mass w/ ee hypothesis (MC bkg)", 480, 0.0, 1.2);
    fHistLambdareco_MC_m_ee_o_bkg->SetXTitle("m_{#Lambda} (GeV)"); fHistLambdareco_MC_m_ee_o_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_pt_bkg  = new TH1F("h_Lambda_MC_pt_bkg", "Lambda-p_{#perp} (MC background)", 60, 0, 30);
    fHistLambdareco_MC_pt_bkg->SetXTitle("p_{#perp} (GeV)");   fHistLambdareco_MC_pt_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_phi_bkg = new TH1F("h_Lambda_MC_phi_bkg", "Lambda #phi distribution (MC background)", 128, -3.2, 3.2);
    fHistLambdareco_MC_phi_bkg->SetXTitle("#phi (rad)");       fHistLambdareco_MC_phi_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_eta_bkg = new TH1F("h_Lambda_MC_eta_bkg", "Lambda #eta distribution (MC background)", 100, -3, 3);
    fHistLambdareco_MC_eta_bkg->SetXTitle("#eta");             fHistLambdareco_MC_eta_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_tau_bkg = new TH1F("h_Lambda_MC_tau_bkg", "Reconstructed Lambda lifetime (MC background)", 100, 0, 300);
    fHistLambdareco_MC_tau_bkg->SetXTitle("#tau (ps)");        fHistLambdareco_MC_tau_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_src_div_bkg = new TH1F("h_Lambda_MC_src_div_bkg", "Angular distance of #Lambda origin and primary vertex", 300, 0.0, 3.2);
    fHistLambdareco_MC_src_div_bkg->SetXTitle("#alpha (rad)"); fHistLambdareco_MC_src_div_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_R_vtx_bkg = new TH1F("h_Lambda_MC_r_vtx_bkg", "Radial distance of secondary vertices", 400, 0.0, 100.0);
    fHistLambdareco_MC_R_vtx_bkg->SetXTitle("r_{#perp} (cm)"); fHistLambdareco_MC_R_vtx_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_tdcy_len_bkg = new TH1F("h_Lambda_MC_tdcy_len_bkg", "Transverse decay length (MC bkg)", 400, 0.0, 100.0);
    fHistLambdareco_MC_tdcy_len_bkg->SetXTitle("s_{#perp} (cm)"); fHistLambdareco_MC_tdcy_len_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_oangle_bkg = new TH1F("h_Lambda_MC_oangle_bkg", "Opening angle of p/pi (MC bkg)", 256, 0.0, 3.2);
    fHistLambdareco_MC_oangle_bkg->SetXTitle("#alpha (rad)");  fHistLambdareco_MC_oangle_bkg->SetYTitle("Number of Entries");
    fHistLambdareco_MC_dangle_bkg = new TH1F("h_Lambda_MC_dangle_bkg", "Decay angle of p/pi (MC bkg)", 256, -1.0, 1.0);
    fHistLambdareco_MC_dangle_bkg->SetXTitle("cos(#alpha)");   fHistLambdareco_MC_dangle_bkg->SetYTitle("Number of Entries");

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

    gDirectory->cd("/");
    fHistPrimKaon_Mother = new TH1F("h_PrimVtx_K_Mother", "Kaon mothers (at primary vtx)", 1000, 0.0, 1000.0);
    fHistPrimKaon_Mother->SetXTitle(""); fHistPrimKaon_Mother->SetYTitle("");
    fHistSecKaon_Mother  = new TH1F("h_SecVtx_K_Mother",  "Kaon mothers (at secondary vtx)", 1000, 0.0, 1000.0);
    fHistSecKaon_Mother->SetXTitle("");  fHistSecKaon_Mother->SetYTitle("");
    fHistSecKaon_Mother2Body  = new TH1F("h_SecVtx_K_Mother2Body",  "Kaon mothers (at secondary vtx)", 1000, 0.0, 1000.0);
    fHistSecKaon_Mother2Body->SetXTitle(""); fHistSecKaon_Mother2Body->SetYTitle("");
    
}


//____________________________________________________________________

Bool_t AtlKaonHistos::AnalyzeEvent() {
    //
    // Event analysis
    //

    if (fEvent->IsMC()) {

	HepVertex *vtx;
	AtlIDTrack *trk, *trk1, *trk2;
	HepMCParticle *m, *t1, *t2, *d;
	TLorentzVector v, v1, v2;
	
	for (Int_t i = 0; i < fEvent->GetN_Vertices(); i++) {
	    vtx = (HepVertex*)fEvent->GetVertices()->At(i);
	    if (!vtx->IsSecondary()) continue;

	    for (Int_t j = 0; j < vtx->GetNDaughters(); j++) {
		trk = (AtlIDTrack*)vtx->GetDaughters()->At(j);
		if ( trk->HasValidTruthMatch(0.9) && (trk->GetMCParticle()->GetMother() != NULL) &&
		     (TMath::Abs(trk->GetMCParticle()->GetPdgCode()) == 321)) {
		    fHistSecKaon_Mother->Fill(trk->GetMCParticle()->GetMother()->GetPdgName(), GetPreTagEvtWeight());
		}
	    }
	}

	for (Int_t i = 0; i < fEvent->GetN_Vertices(); i++) {
	    vtx = (HepVertex*)fEvent->GetVertices()->At(i);
	    if (!vtx->IsSecondary() || (vtx->GetNDaughters() != 2)) continue;
	    
	    trk1 = (AtlIDTrack*)vtx->GetDaughters()->At(0);
	    trk2 = (AtlIDTrack*)vtx->GetDaughters()->At(1);

	    if (! (trk1->HasValidTruthMatch(0.9) && trk2->HasValidTruthMatch(0.9)) ) continue;
	    if ((trk1->GetMCParticle()->GetMother() == NULL) || (trk2->GetMCParticle()->GetMother() == NULL)) continue;
	    if (trk1->GetMCParticle()->GetMother() != trk2->GetMCParticle()->GetMother()) continue;

	    t1  = trk1->GetMCParticle();
	    t2  = trk2->GetMCParticle();
	    m  = t1->GetMother();
	    
	    if ((TMath::Abs(t1->GetPdgCode()) == 321) || (TMath::Abs(t2->GetPdgCode()) == 321)) {
		if ( (TMath::Abs(t1->GetPdgCode()) == 321)) {
		    v1.SetVectM(trk1->P(),fm_k);
		    v2.SetVectM(trk2->P(),fm_pi);
		    v = v1 + v2;
		    fHistLambdareco_m_PPi->Fill(v.M(), GetPreTagEvtWeight());
		}
		else {
		    v1.SetVectM(trk2->P(),fm_k);
		    v2.SetVectM(trk1->P(),fm_pi);
		    v = v1 + v2;
		    fHistLambdareco_m_PPi->Fill(v.M(), GetPreTagEvtWeight());
		}
		fHistSecKaon_Mother2Body->Fill(m->GetPdgName(), GetPreTagEvtWeight());
		fDebugStream << endl << "---------------------------------------" << endl;
		fDebugStream << "Evt:    " << fTree->GetReadEvent() << " Prt: " << m->GetId() << " Vtx: " << vtx->GetId() << endl;
		if (m->GetMother() != NULL)
		    fDebugStream << "Mother²:" << m->GetMother()->GetPdgName() << endl;
		fDebugStream << "Mother: " << m->GetPdgName() << endl;
		fDebugStream << "Track1: " << t1->GetPdgName() << endl;
		fDebugStream << "Track2: " << t2->GetPdgName() << endl;
		for (Int_t j = 0; j < m->GetN_Daughters(); j++) {
		    d = (HepMCParticle*)m->GetDaughters()->At(j);
		    fDebugStream << "Daughter " << j << ": " << d->GetPdgName() << endl;
		}
	    }
	}

	

	vtx = fEvent->GetPrimaryVtx();
	if (vtx == NULL) return kFALSE;
	for (Int_t j = 0; j < vtx->GetNDaughters(); j++) {
	    trk = (AtlIDTrack*)vtx->GetDaughters()->At(j);
	    if ( trk->HasValidTruthMatch(0.9) && (trk->GetMCParticle()->GetMother() != NULL) &&
		 (TMath::Abs(trk->GetMCParticle()->GetPdgCode()) == 321)) {
		fHistPrimKaon_Mother->Fill(trk->GetMCParticle()->GetMother()->GetPdgName(), GetPreTagEvtWeight());
	    }
	}
    }
    
    /*
    fBkgLambdaDecays->Clear("C");

    // count true Lambda decays to compute efficiency
    if (fEvent->IsMC()) {
	Int_t NLambdaTrue = 0;
	for (Int_t i = 0; i < fEvent->GetN_Vertices(); i++) {
	    HepVertex *vtx = (HepVertex*)fEvent->GetVertices()->At(i);
	    if ( !( (vtx->IsSecondary()) && (vtx->GetNDaughters()==2) ))  continue;
	    AtlIDTrack *trk1 = (AtlIDTrack*)vtx->GetDaughters()->At(0);
	    AtlIDTrack *trk2 = (AtlIDTrack*)vtx->GetDaughters()->At(1);

	    if ((trk1->HasValidTruthMatch(0.0001) && (trk1->GetMCParticle()->GetMother() == NULL)) ||
		(trk2->HasValidTruthMatch(0.0001) && (trk2->GetMCParticle()->GetMother() == NULL)) ) {
		cout << endl << "In Event " << fTree->GetReadEvent() << " at vtx " << i << ": "
		     << "A daughter has an MC Particle, but no MC Mother!";
	    }

	    if (trk1->HasValidTruthMatch(0.0001) && (trk1->GetMCParticle()->GetMother() != NULL) &&
		(TMath::Abs(trk1->GetMCParticle()->GetMother()->GetPdgCode()) == 421)) {
		NLambdaTrue++;
	    } else if (trk2->HasValidTruthMatch(0.0001) && (trk2->GetMCParticle()->GetMother() != NULL) &&
		       (TMath::Abs(trk2->GetMCParticle()->GetMother()->GetPdgCode()) == 421)) {
		NLambdaTrue++;
	    }
	}
	AddAbundanceTrue(NLambdaTrue);
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
    */
    return kTRUE;
}

//____________________________________________________________________


void AtlKaonHistos::ReconstructLambdaCutBased(){
    //
    // Cut Based Lambda reconstruction  
    //

    // Looping over all trk-combinations at a sec Vertex for Lambda reconstruction

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
      p_trk1.SetVectM(trk1->P(), fm_k);
      p_trk2.SetVectM(trk2->P(), fm_pi);
      p_Lambda = p_trk1 + p_trk2;
      m_Lambda = p_Lambda.M();

      // Selection constraints
      // -------------------------
    
      if( (m_Lambda > fLambda_M_min) && (m_Lambda < fLambda_M_max) ) { // cut: invariant mass
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
	  fBkgLambdaDecays->Add(new AtlLambdaDecayPPi(BkgCount++, p_Lambda.Px(), p_Lambda.Py(), p_Lambda.Pz(),
						      p_Lambda.E(), trk1, trk2, Vtx, PrimaryVtx, 
						      Fit_Daughter1, Fit_Daughter2));
        }
      }
    }
}


//____________________________________________________________________

Bool_t AtlKaonHistos::IsKaonVertex(HepVertex *vtx) {
    //
    // Return true, if the given vertex is already assigned
    // to a kaon decay. Return false otherwise or if
    // there are no kaon decays (eg. the K0s finder didn't run).
    //

    /*
    if (fEvent->GetN_K0sDecaysPiPi() < 1) {
	return kFALSE;
    }

    TClonesArray *k0s = fEvent->GetK0sDecaysPiPi();
    for (Int_t i = 0; i < fEvent->GetN_K0sDecaysPiPi(); i++) {
	if (((AtlK0sDecayPiPi*)k0s->At(i))->GetVtx() == vtx)
	    return kTRUE;
    }
    return kFALSE;
    */
    
    AtlIDTrack *trk1 = (AtlIDTrack*)vtx->GetDaughters()->At(0);
    AtlIDTrack *trk2 = (AtlIDTrack*)vtx->GetDaughters()->At(1);
    TLorentzVector p_e1, p_e2, p_k;
    p_e1.SetVectM(trk1->P(), fm_e);
    p_e2.SetVectM(trk2->P(), fm_e);
    p_k = p_e1 + p_e2;
    return ((p_k.M() >= fK0s_M_min) && (p_k.M() < fK0s_M_max));
}


//____________________________________________________________________

Bool_t AtlKaonHistos::IsConversionVertex(HepVertex *vtx) {
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

void AtlKaonHistos::ReconstructLambdaKinFit() {
  //
  // Lambda reconstruction from track-pairs assuming being pions using KinFitter
  //
  HepVertex  *Vtx = 0;
  HepVertex  *PrimVtx = 0;
  AtlIDTrack *trk1 = 0;  
  AtlIDTrack *trk2 = 0;
    
  TLorentzVector p_Lambda;
    
  TMatrixD cov_trk1(3,3);
  TMatrixD cov_trk2(3,3);
  
  Double_t fChi2;
  Int_t fNDoF;

  Int_t BkgCount = 1;  // Counts background lambdas (with equally charged decay products)
  Float_t PvalueKinFit = 0.0;

  // Get primary vertex
  PrimVtx = fEvent->GetPrimaryVtx();

  // Vertex-Loop
  for ( Int_t i = 0; i < fEvent->GetN_Vertices(); i++){
    
    Vtx = (HepVertex*)fEvent->GetVertices()->At(i);

    // Analyse only secondary vertices containing 2 outgoing tracks
    if ( !( (Vtx->IsSecondary()) && (Vtx->GetNDaughters() == 2) )) continue;
    SetCutFlow("Sec. Vtx w/ 2 Dght.");
    // cut vertex fit's chi2/ndof
    if ( Vtx->GetChi2overNDoF() > fVertexChi2ovNDoF_max ) continue;
    SetCutFlow("Vtx #chi^{2}");

    trk1 = (AtlIDTrack*)Vtx->GetDaughters()->At(0);
    trk2 = (AtlIDTrack*)Vtx->GetDaughters()->At(1);
    // cut track fit's chi2/ndof
    if (  (trk1->Chi2ovNDoF() > fTrackChi2ovNDoF_max) ||
	 (trk2->Chi2ovNDoF() > fTrackChi2ovNDoF_max) ) continue;
    SetCutFlow("Trk #chi^{2}");
    
    // Discard all tracks without reference to a truth particle for all
    // MC-Runs to compute the purity correctly
    if( fEvent->IsMC() ) {
	if( !( trk1->HasValidTruthMatch(0.0001) &&
	       trk2->HasValidTruthMatch(0.0001)) )  continue;
    }
    SetCutFlow("MC exists");

    // Definition of covariance matrices, taken from track fit
    trk1->GetCovMatrixPtEtaPhi(cov_trk1);
    trk2->GetCovMatrixPtEtaPhi(cov_trk2);

    // Define momentum used as output of the fit
    TLorentzVector FitP_trk1pr;
    TLorentzVector FitP_trk2pi;
    TKinFitter fitterPPi = PerformFit(trk1, trk2, FitP_trk1pr, FitP_trk2pi, &cov_trk1, &cov_trk2);

    TLorentzVector FitP_trk1pi;
    TLorentzVector FitP_trk2pr;
    TKinFitter fitterPiP = PerformFit(trk2, trk1, FitP_trk1pi, FitP_trk2pr, &cov_trk2, &cov_trk1);
          
    // abort, if a fit failed severely (chi2 < 0)
    if ( (fitterPPi.getS() < 0.) || (fitterPiP.getS() < 0.) ) {
      Error("ReconstructLambdaKinFit", "fitter.getS()<0. Abort!");
      gSystem->Abort(0);
    }
    // skip event, if neither fit converged
    Bool_t convPPi = (fitterPPi.getStatus() == 0);
    Bool_t convPiP = (fitterPiP.getStatus() == 0);
    if ( (!convPPi) && (!convPiP) ) continue;
    SetCutFlow("#geq 1 Fit ok");
    fN_Fits++;

    // Reconstruct Lambda 4-momentum
    // and set Chi2 and NDoF from the converged fit.
    // If both fits converged, prefer the lower chi2/ndof.
    Float_t Chi2overNDoF_PPi = fitterPPi.getS()/fitterPPi.getNDF();
    Float_t Chi2overNDoF_PiP = fitterPiP.getS()/fitterPiP.getNDF();
    HepParticle Fit_Daughter1; // save momenta from the better fit
    HepParticle Fit_Daughter2; // is filled in the following if-condition
    if ( convPPi && ( (!convPiP) || ( Chi2overNDoF_PPi <= Chi2overNDoF_PiP ) ) ) {
      p_Lambda = FitP_trk1pr + FitP_trk2pi;
      fChi2 = fitterPPi.getS();
      fNDoF = fitterPPi.getNDF();
      HepParticle FitDaughter1(1, FitP_trk1pr.Px(), FitP_trk1pr.Py(), FitP_trk1pr.Pz(), 
			       FitP_trk1pr.E(), (trk1->GetQovP() < 0.) ? -2212 : 2212);
      HepParticle FitDaughter2(2, FitP_trk2pi.Px(), FitP_trk2pi.Py(), FitP_trk2pi.Pz(), 
			       FitP_trk2pi.E(), (trk2->GetQovP() < 0.) ?  -211 :  211);  
      Fit_Daughter1 = FitDaughter1;
      Fit_Daughter2 = FitDaughter2;
    } else {
      p_Lambda = FitP_trk1pi + FitP_trk2pr;
      fChi2 = fitterPiP.getS();
      fNDoF = fitterPiP.getNDF();
      HepParticle FitDaughter1(1,FitP_trk1pi.Px(), FitP_trk1pi.Py(), FitP_trk1pi.Pz(), 
			       FitP_trk1pi.E(), (trk1->GetQovP() < 0.) ?  -211 :  211);
      HepParticle FitDaughter2(2, FitP_trk2pr.Px(), FitP_trk2pr.Py(), FitP_trk2pr.Pz(), 
			       FitP_trk2pr.E(), (trk2->GetQovP() < 0.) ? -2212 : 2212);
      Fit_Daughter1 = FitDaughter1;
      Fit_Daughter2 = FitDaughter2;
      // save proton track as first track always
      AtlIDTrack *tmp = trk2;
      trk2 = trk1;
      trk1 = tmp;
    }
    SetChi2(fChi2);
    SetNDoF(fNDoF);
    PvalueKinFit = TMath::Prob(fChi2, fNDoF);

    // calculate angular divergence to primary vertex
    TLorentzVector p_Refit = Fit_Daughter1.P() + Fit_Daughter2.P();
    if (p_Refit.Angle( Vtx->GetPos() - PrimVtx->GetPos() ) > fAngleToPrimary_max)
        continue;
    SetCutFlow("Prim vtx origin");

    // reject event, if it was also found by the K0s finder
    if (IsKaonVertex(Vtx))
	continue;
    SetCutFlow("No K0s");

    // reject event, if it was generated by photo conversion
    if (IsConversionVertex(Vtx))
        continue;
    SetCutFlow("No #gamma conv");

    // Add Lambda to event
    if (PvalueKinFit >= fKinFitPvalue_min) {
	SetCutFlow("KinFit P-value");

	AtlLambdaDecayPPi *decay;    
	if ( trk1->Charge()*trk2->Charge() < 0. ) { // require opposite sign

	    SetCutFlow("Charge ok");

	    decay = fEvent->AddLambdaDecayPiPi(p_Lambda.Px(), p_Lambda.Py(), p_Lambda.Pz(), p_Lambda.E(),
					       trk1, trk2, Vtx, PrimVtx, Fit_Daughter1, Fit_Daughter2);
	    decay->SetChi2NDoF(fChi2, fNDoF);

	    // Hitbased TruthMatching
	    if (fEvent->IsMC()) {
		switch ( DoTruthMatch(trk1, trk2, Vtx) ){
		    case 1:   fTrueReco = kTRUE;  FillMCHistograms(decay, kTRUE);  break;
		    case -1:  fTrueReco = kFALSE; FillMCHistograms(decay, kFALSE); break;
		    case 0:   continue;
		}
	    }

	    if ( IsDebugRun() ) {
		fDebugStream  << endl << "=========================================="
                                << endl << "Entry: " << fTree->GetReadEvent() << " Vtx: " << Vtx->GetId() << " Prob: " << PvalueKinFit;
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
	    
	    AtlKinFitterTool::FillHistograms("Signal");
	} else {
	    // Add equally charged or mc-failed combinations to the bkg list
	    decay = new AtlLambdaDecayPPi(BkgCount++, p_Lambda.Px(), p_Lambda.Py(), p_Lambda.Pz(), 
					  p_Lambda .E(),trk1, trk2,Vtx, PrimVtx, 
					  Fit_Daughter1, Fit_Daughter2);
	    decay->SetChi2NDoF(fChi2, fNDoF);
	    fBkgLambdaDecays->Add(decay);
	    AtlKinFitterTool::FillHistograms("Bkg");
	}
    }

  } //end of vertex loop
}

//____________________________________________________________________    

TKinFitter AtlKaonHistos::PerformFit(AtlIDTrack *trk1, AtlIDTrack *trk2, TLorentzVector& FitP_trk1, TLorentzVector& FitP_trk2,
				       TMatrixD *cov_trk1, TMatrixD *cov_trk2) {
  //
  // Perform a kinematic fit with the given tracks to test the lambda mass hyptothesis
  // assuming that track1 is the proton and track2 is the pion.
  //

  // Define particles necessary for execution of the fit
  // Using proton/pion mass hypothesis; fit variables are pt, eta, phi
  TVector3 dummy1 = trk1->P(); // instead of const cast
  TFitParticlePtEtaPhi FitExec_trk1("FitExec_trk1", "FitExec_trk1 for LambdaFit", &dummy1, fm_k, cov_trk1);
  TVector3 dummy2 = trk2->P(); // instead of const cast
  TFitParticlePtEtaPhi FitExec_trk2("FitExec_trk2", "FitExec_trk2 for LambdaFit", &dummy2, fm_pi, cov_trk2);
  
  // Definition of Lambda mass constraint
  TFitConstraintM MLambdaCons("LambdaMassConstraint","LambdaMassConstraintGaus", 0, 0, fm_d0);
  MLambdaCons.addParticles1(&FitExec_trk1, &FitExec_trk2);
    
  // fitting characteristics
  TKinFitter fitter;
  fitter.addMeasParticle(&FitExec_trk1);
  fitter.addMeasParticle(&FitExec_trk2);
  fitter.addConstraint(&MLambdaCons);
    
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

void AtlKaonHistos::FillMCHistograms(AtlLambdaDecayPPi *decay, Bool_t signal) {
    //
    // Fill histograms for MC signal/background comparison.
    // Set signal to false to fill background histograms.
    //

    TLorentzVector p_trk1, p_trk2, p_ee, p_ppi, p_pipi, p_ee_o, p_ppi_o, p_pipi_o;
    p_trk1.SetVectM(decay->GetReFitProton().P().Vect(), fm_e);
    p_trk2.SetVectM(decay->GetReFitPion().P().Vect(),   fm_e);
    p_ee = p_trk1 + p_trk2;
    p_trk1.SetVectM(decay->GetReFitProton().P().Vect(), fm_pi);
    p_trk2.SetVectM(decay->GetReFitPion().P().Vect(),   fm_pi);
    p_pipi = p_trk1 + p_trk2;
    p_trk1.SetVectM(decay->GetReFitProton().P().Vect(), fm_k);
    p_ppi = p_trk1 + p_trk2;

    p_trk1.SetVectM(decay->GetProton()->P(), fm_e);
    p_trk2.SetVectM(decay->GetPion()->P(), fm_e);
    p_ee_o = p_trk1 + p_trk2;
    p_trk1.SetVectM(decay->GetProton()->P(), fm_pi);
    p_trk2.SetVectM(decay->GetPion()->P(),   fm_pi);
    p_pipi_o = p_trk1 + p_trk2;
    p_trk1.SetVectM(decay->GetProton()->P(), fm_k);
    p_ppi_o = p_trk1 + p_trk2;

    if (signal) {
	fHistLambdareco_MC_m       ->Fill(decay->M("Reco"),     GetPreTagEvtWeight());
	fHistLambdareco_MC_pt      ->Fill(decay->Pt(),          GetPreTagEvtWeight());
	fHistLambdareco_MC_phi     ->Fill(decay->Phi(),         GetPreTagEvtWeight());
	fHistLambdareco_MC_eta     ->Fill(decay->Eta(),         GetPreTagEvtWeight());
	fHistLambdareco_MC_tau     ->Fill(decay->GetLifeTime(), GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_ee    ->Fill(p_ee.M(),             GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_pipi  ->Fill(p_pipi.M(),           GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_ppi   ->Fill(p_ppi.M(),            GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_ee_o  ->Fill(p_ee_o.M(),           GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_pipi_o->Fill(p_pipi_o.M(),         GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_ppi_o ->Fill(p_ppi_o.M(),          GetPreTagEvtWeight());
	fHistLambdareco_MC_R_vtx   ->Fill(decay->GetVtx()->GetPos().Perp(), GetPreTagEvtWeight());
	fHistLambdareco_MC_src_div ->Fill(decay->GetAngleToPrimary(),       GetPreTagEvtWeight());
	fHistLambdareco_MC_tdcy_len->Fill(decay->GetTransvDecayLength(),    GetPreTagEvtWeight());
	fHistLambdareco_MC_oangle  ->Fill(decay->GetOpeningAngle(),         GetPreTagEvtWeight());
	fHistLambdareco_MC_dangle  ->Fill(TMath::Cos(decay->GetDecayAngle()),GetPreTagEvtWeight());
    } else {
	fHistLambdareco_MC_m_bkg       ->Fill(decay->M("Reco"),     GetPreTagEvtWeight());
	fHistLambdareco_MC_pt_bkg      ->Fill(decay->Pt(),          GetPreTagEvtWeight());
	fHistLambdareco_MC_phi_bkg     ->Fill(decay->Phi(),         GetPreTagEvtWeight());
	fHistLambdareco_MC_eta_bkg     ->Fill(decay->Eta(),         GetPreTagEvtWeight());
	fHistLambdareco_MC_tau_bkg     ->Fill(decay->GetLifeTime(), GetPreTagEvtWeight());
        fHistLambdareco_MC_m_ee_bkg    ->Fill(p_ee_o.M(),           GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_pipi_bkg  ->Fill(p_pipi_o.M(),         GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_ppi_bkg   ->Fill(p_ppi_o.M(),          GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_ee_o_bkg  ->Fill(p_ee_o.M(),           GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_pipi_o_bkg->Fill(p_pipi_o.M(),         GetPreTagEvtWeight());
   	fHistLambdareco_MC_m_ppi_o_bkg ->Fill(p_ppi_o.M(),          GetPreTagEvtWeight());
	fHistLambdareco_MC_R_vtx_bkg   ->Fill(decay->GetVtx()->GetPos().Perp(), GetPreTagEvtWeight());
	fHistLambdareco_MC_src_div_bkg ->Fill(decay->GetAngleToPrimary(),       GetPreTagEvtWeight());
	fHistLambdareco_MC_tdcy_len_bkg->Fill(decay->GetTransvDecayLength(),    GetPreTagEvtWeight());
	fHistLambdareco_MC_oangle_bkg  ->Fill(decay->GetOpeningAngle(),         GetPreTagEvtWeight());
	fHistLambdareco_MC_dangle_bkg  ->Fill(TMath::Cos(decay->GetDecayAngle()),GetPreTagEvtWeight());
    }    
}


//____________________________________________________________________    


void AtlKaonHistos::FillHistograms() {
    //
    // Fill histograms
    //

    return;

    AtlLambdaDecayPPi *Lambda = 0;
    
    // Proton/Pion variables
    AtlIDTrack *Proton =0;
    AtlIDTrack *Pion   =0;
    TLorentzVector p_trk1, p_trk2, p_Lambda;

    // Signal (opposite charged)
    // ________________________________________
    
    for ( Int_t i = 0; i < fEvent->GetN_LambdaDecaysPiPi(); i++ ) {
      Lambda = (AtlLambdaDecayPPi*)fEvent->GetLambdaDecaysPiPi()->At(i);

      fHistLambdareco_m_PPi     ->Fill(Lambda->M("Reco"),    GetPreTagEvtWeight());
      fHistLambdareco_pt_PPi    ->Fill(Lambda->Pt(),         GetPreTagEvtWeight());
      fHistLambdareco_phi_PPi   ->Fill(Lambda->Phi(),        GetPreTagEvtWeight());
      fHistLambdareco_eta_PPi   ->Fill(Lambda->Eta(),        GetPreTagEvtWeight());
      fHistLambdareco_tau_PPi   ->Fill(Lambda->GetLifeTime(),             GetPreTagEvtWeight());
      fHistLambdareco_R_vtx     ->Fill(Lambda->GetVtx()->GetPos().Perp(), GetPreTagEvtWeight());
      fHistLambdareco_tdcy_len  ->Fill(Lambda->GetTransvDecayLength(), GetPreTagEvtWeight());
      fHistLambdareco_oangle    ->Fill(Lambda->GetOpeningAngle(),      GetPreTagEvtWeight());
      fHistLambdareco_dangle    ->Fill(Lambda->GetDecayAngle(),        GetPreTagEvtWeight());

      Proton = Lambda->GetProton();
      Pion  =  Lambda->GetPion();

      // test pi/pi hypothesis
      p_trk1.SetVectM(Proton->P(), fm_pi);
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

    fHistLambdareco_N_PPi ->Fill(fEvent->GetN_LambdaDecaysPiPi(), GetPreTagEvtWeight());
    

    // Combinatorial Background (same charged)
    // ____________________________________________

    TIter next_bkg(fBkgLambdaDecays);
    while ( ( Lambda = (AtlLambdaDecayPPi*)next_bkg() ) ) {
      fHistLambdareco_m_PPi_bkg     ->Fill(Lambda->M("Reco"),      GetPreTagEvtWeight());
      fHistLambdareco_pt_PPi_bkg    ->Fill(Lambda->Pt(),           GetPreTagEvtWeight());
      fHistLambdareco_phi_PPi_bkg   ->Fill(Lambda->Phi(),          GetPreTagEvtWeight());
      fHistLambdareco_eta_PPi_bkg   ->Fill(Lambda->Eta(),          GetPreTagEvtWeight());
      fHistLambdareco_tau_PPi_bkg   ->Fill(Lambda->GetLifeTime(),  GetPreTagEvtWeight());
      fHistLambdareco_R_vtx_bkg     ->Fill(Lambda->GetVtx()->GetPos().Perp(), GetPreTagEvtWeight());
      fHistLambdareco_tdcy_len_bkg  ->Fill(Lambda->GetTransvDecayLength(),    GetPreTagEvtWeight());
      fHistLambdareco_oangle_bkg    ->Fill(Lambda->GetOpeningAngle(),         GetPreTagEvtWeight());
      fHistLambdareco_dangle_bkg    ->Fill(Lambda->GetDecayAngle(),           GetPreTagEvtWeight());

      Proton = Lambda->GetProton();
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

void AtlKaonHistos::SetCutDefaults() {
    //
    // Selection cut defaults and Constraint parameters
    //

    fSignal_Pt_min       =  0.5;     // Min. Pt of signal pion/proton
    fSignal_Pt_max       =  1.e10;   // Max. Pt of signal pion/proton
    fSignal_Eta_min      = -2.5;     // Min. eta of signal pion/proton
    fSignal_Eta_max      =  2.5;     // Max. eta of signal pion/proton
    
    fm_lambda            = 1.115683;       // Lambda mass hypothesis
    fm_d0                = 1.86484;        // D0 mass hypothesis
    fm_k                 = 0.493667;       // Kaon mass hypothesis
    fm_pi                = 0.13957;        // Pion mass hypothesis
    fm_proton            = 0.938272013;    // Proton mass hypothesis
    fm_e                 = 0.000510998910; // Electron mass hypothesis
}

//____________________________________________________________________


void AtlKaonHistos::PrintCutValues() {
        
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
    cout << scientific << " " << fLambda_M_min << " < M(Lbd) < " << fLambda_M_max << endl;
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


void AtlKaonHistos::Terminate() {
    fHistPrimKaon_Mother->SetStats(kFALSE);
    fHistPrimKaon_Mother->LabelsDeflate("x");
    fHistSecKaon_Mother->SetStats(kFALSE);
    fHistSecKaon_Mother->LabelsDeflate("x");
    fHistSecKaon_Mother2Body->SetStats(kFALSE);
    fHistSecKaon_Mother2Body->LabelsDeflate("x");
/*
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
      if ((binContent > 0) && (fHistLambdareco_truth_PPi->GetXaxis()->GetBinLabel(i) != "")) {
          cout.width(15);
          cout << fHistLambdareco_truth_PPi->GetXaxis()->GetBinLabel(i);
          cout << ": " << TMath::Nint(binContent) << endl;
      }
  }

  cout << endl << "INFO: Terminating AtlKaonHistos!";
  cout << endl << "        # Successful fits: " << fN_Fits;
  cout << endl << "        # Truth match bad: " << fN_MCProbMiss;
  cout << endl << "   # TM with diff. origin: " << fN_SameOriginMiss;
  cout << endl << "     # TM => wrong tracks: " << fN_MCFail;
  cout << endl << " # Possible Lambdas found: " << TMath::Nint(fHistLambdareco_m_PPi->GetEntries());
*/  
}

//____________________________________________________________________

Int_t AtlKaonHistos::DoTruthMatch(AtlIDTrack *trk1, AtlIDTrack *trk2, HepVertex *Vtx) {
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
	Float_t prob = ((AtlLambdaDecayPPi*)fEvent->GetLambdaDecaysPiPi()->Last())->GetProb();
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

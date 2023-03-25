
//____________________________________________________________________
//
// Z0 decay finder class
// 

//  
// Author: Holger Schulz <mailto:holger.schulz@physik.hu-berlin.de>, Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Holger Schulz, Oliver Maria Kind
//
#ifndef ATLAS_AtlZ0Finder
#include <AtlZ0Finder.h>
#endif
#include <TH1F.h>
#include <AtlEvent.h>
#include <TTree.h>
#include <TString.h>
#include <iostream>
#include <TROOT.h>
#include <TFile.h>
#include <iostream>
#include <vector>

using namespace std;

#ifndef __CINT__
ClassImp(AtlZ0Finder);
#endif

//____________________________________________________________________

AtlZ0Finder::AtlZ0Finder(const char* name, const char* title,
			 Bool_t chargetest, EModeLepton ModeLepton) :
    AtlKinFitterTool(name, title) {
    //
    // Default constructor
    //
    SetMode();
    fchargetest=chargetest;
    fSignalElectrons = 0;
    fSignalMuons     = 0;
    fSignalTaus      = 0;
    fModeLepton=ModeLepton;
    c11=0;
    c22=0;
    c33=0;
    c12=0;
    c13=0;
    c23=0;
    TruthMatchCounter=0;

    fBkgZ0Decays = new TList;
}

//____________________________________________________________________

AtlZ0Finder::~AtlZ0Finder() {
    //
    // Default destructor
    //
    if ( fSignalElectrons != 0 ) delete fSignalElectrons;
    if ( fSignalMuons     != 0 ) delete fSignalMuons;
    if ( fSignalTaus      != 0 ) delete fSignalTaus;
    delete fBkgZ0Decays;
}

//____________________________________________________________________

void AtlZ0Finder::SetBranchStatus() {
    //
    // Switch on needed branches
    //
    fTree->SetBranchStatus("fN_Cone4H1TowerJets*",  kTRUE);
    fTree->SetBranchStatus("fCone4H1TowerJets*",    kTRUE);
    fTree->SetBranchStatus("fN_Electrons*",         kTRUE);
    fTree->SetBranchStatus("fElectrons*",           kTRUE);
    fTree->SetBranchStatus("fN_Muons*",             kTRUE);
    fTree->SetBranchStatus("fMuons*",               kTRUE);
    fTree->SetBranchStatus("fN_Taus*",              kTRUE);
    fTree->SetBranchStatus("fTaus*",                kTRUE);
    fTree->SetBranchStatus("fN_Z0Decays*",          kTRUE);
    fTree->SetBranchStatus("fZ0Decays*",            kTRUE);
    fTree->SetBranchStatus("fN_IDTracks*",          kTRUE);  
    fTree->SetBranchStatus("fIDTracks*",            kTRUE);
}

//____________________________________________________________________

void AtlZ0Finder::BookHistograms() {
    //
    // Book histograms
    //

    // Book KinFitter histograms
    if ( fMode == kKinFit ) {
	AtlKinFitterTool::BookHistograms();
	AtlKinFitterTool::SetTruthMatchXTitle("M_{Z}^{True}");
	AtlKinFitterTool::SetTruthMatchBinning(50,65.,115.);   // use invariant mass of true lepton pair (Z mass)
	                                                       // to bin Truth Matching histograms
	
    }

    // small resolution study

    gDirectory->mkdir("Resolution", "Electrons");
    gDirectory->cd("Resolution");
    
    fHist_Zee_PtRes = new TH1F("h_eRes_pt", "Electron Resolution Pt", 70, -0.3, 0.2);
    fHist_Zee_PtRes->SetXTitle("(p_{T}^{Reco}-p_{T}^{True})/p_{T}^{True}");
    fHist_Zee_PtRes->SetYTitle("# Entries");
    
    fHist_Zee_EtaRes = new TH1F("h_eRes_eta", "Electron Resolution Eta", 100, -0.012, 0.012);
    fHist_Zee_EtaRes->SetXTitle("(#eta^{Reco}-#eta^{True})/#eta^{True}");
    fHist_Zee_EtaRes->SetYTitle("# Entries");
    
    fHist_Zee_PhiRes = new TH1F("h_eRes_phi", "Electron Resolution Phi", 100, -0.01, 0.01);
    fHist_Zee_PhiRes->SetXTitle("(#varphi^{Reco}-#varphi^{True})/#varphi^{True}");
    fHist_Zee_PhiRes->SetYTitle("# Entries");

    fHist_Zee_PtRes_KinFit  = new TH1F("h_eRes_pt_KinFit", "Electron Resolution Pt, KinFitter Output", 70, -0.3, 0.2);
    fHist_Zee_PtRes_KinFit->SetXTitle("(p_{T}^{Reco}-p_{T}^{True})/p_{T}^{True}");
    fHist_Zee_PtRes_KinFit->SetYTitle("# Entries");

    fHist_Zee_EtaRes_KinFit = new TH1F("h_eRes_eta_KinFit", "Electron Resolution Eta, KinFitter Output",
				       50, -0.025, 0.025);
    fHist_Zee_EtaRes_KinFit->SetXTitle("(#eta^{Reco}-#eta^{True})/#eta^{True}");
    fHist_Zee_EtaRes_KinFit->SetYTitle("# Entries");

    fHist_Zee_PhiRes_KinFit = new TH1F("h_eRes_phi_KinFit", "Electron Resolution Phi, KinFitter Output", 50, -0.02, 0.02);
    fHist_Zee_PhiRes_KinFit->SetXTitle("(#varphi^{Reco}-#varphi^{True})/#varphi^{True}");
    fHist_Zee_PhiRes_KinFit->SetYTitle("# Entries");

    fHist_Zee_BremslossProb = new TH2F("h_Z_Brems","e^{+}e^{-} #gamma -Energy loss vs. #chi^{2} probability",
				       50,0.,1.,40,0.,6.);
    fHist_Zee_BremslossProb->SetXTitle("#chi^{2} probability");
    fHist_Zee_BremslossProb->SetYTitle("#gamma -Energy loss");
    fHist_Zee_BremslossProb->SetZTitle("# events");
    fHist_Zee_BremslossProb->SetOption("lego");

    fHist_Zee_RadProb = new TH1F("h_Zee_RadProb","# Z#rightarrow ee#gamma events vs. #chi^{2} probability",50,0.,1.);
    fHist_Zee_RadProb->SetXTitle("#chi^{2} probability");
    fHist_Zee_RadProb->SetYTitle("# Z#rightarrow ee#gamma events");

    fHist_Zee_ZPtProb = new TH2F("h_Zee_PtProb","Z p_{t}^{True} and #chi^{2} probability",50,0.,40.,50,0.,1.);
    fHist_Zee_ZPtProb->SetXTitle("Z p_{t}^{True}");
    fHist_Zee_ZPtProb->SetYTitle("#chi^{2} probability");
    fHist_Zee_ZPtProb->SetZTitle("# events");
    fHist_Zee_ZPtProb->SetOption("lego");

    fHist_Zee_ZEProb = new TH2F("h_Zee_EProb","Z E^{True} and #chi^{2} probability",70,50.,230.,50,0.,1.);
    fHist_Zee_ZEProb->SetXTitle("Z E^{True}");
    fHist_Zee_ZEProb->SetYTitle("#chi^{2} probability");
    fHist_Zee_ZEProb->SetZTitle("# events");
    fHist_Zee_ZEProb->SetOption("lego");

    fHist_Zee_ZMProb = new TH2F("h_Zee_MProb","relative Z Mass and #chi^{2} probability",50,0.,0.5,50,0.,1.);
    fHist_Zee_ZMProb->SetXTitle("|({p_{Z}^{2}-M_{Z})/M_{Z}|");
    fHist_Zee_ZMProb->SetYTitle("#chi^{2} probability");
    fHist_Zee_ZMProb->SetZTitle("# events");
    fHist_Zee_ZMProb->SetOption("lego");
    
    fHist_e_px_true = new TH1F("h_e_px_true","MC Electron px",80,-25.,25.);
    fHist_e_px_true->SetXTitle("p_{e-_{x}}^{True}+p_{e+_{x}}^{True}");
    fHist_e_px_true->SetYTitle("# Events");
				       
    gDirectory->cd("../");
    
    // ======================
    // Book signal histograms
    // ======================
    gDirectory->mkdir("Signal", "Oppositely charged candidates");
    gDirectory->cd("Signal");

    //
    // Z -> ee
    // -------
    gDirectory->mkdir("ee", "Z0 -> ee");
    gDirectory->cd("ee");
    fHistZreco_m_ee   = new TH1F("h_Z_m_ee", "Invariant Z mass (Z->ee)",
				 50, 65, 115);
    fHistZreco_m_ee->SetXTitle("m_{Z} (GeV)");
    fHistZreco_m_ee->SetYTitle("Number of Entries");
    fHistZreco_pt_ee  = new TH1F("h_Z_pt_ee", "Z-p_{#perp} (Z->ee)",
				 100, 0, 200);
    fHistZreco_pt_ee->SetXTitle("p_{#perp} (GeV)");
    fHistZreco_pt_ee->SetYTitle("Number of Entries");
    fHistZreco_phi_ee = new TH1F("h_Z_phi_ee", "Z boson #phi distribution (Z->ee)",
				 128, -3.2, 3.2);
    fHistZreco_phi_ee->SetXTitle("#phi (rad)");
    fHistZreco_phi_ee->SetYTitle("Number of Entries");
    fHistZreco_eta_ee = new TH1F("h_Z_eta_ee", "Z boson #eta distribution (Z->ee)",
				 50, -3, 3);
    fHistZreco_eta_ee->SetXTitle("#eta");
    fHistZreco_eta_ee->SetYTitle("Number of Entries");
    fHistZreco_n_ee   = new TH1F("h_Z_n_ee", "Reconstructed Z bosons per event (Z->ee)",
				 10, 0, 10);
    fHistZreco_n_ee->SetXTitle("N_{Z}/evt");
    fHistZreco_n_ee->SetYTitle("Number of Entries");

    fHistZreco_DeltaPhi_ee = new TH1F("h_Z_DeltaPhi_ee", "Abs. Value of Phi(e+)-Phi(e-) (Z->ee)",
				      30,2,4);
    fHistZreco_DeltaPhi_ee->SetXTitle("|#Delta #phi_{ee}|");
    fHistZreco_DeltaPhi_ee->SetYTitle("# Events");
    
    fHistZreco_prob_ee = new TH1F("h_Z_prob_ee", "Probability of Z->ee",
				      20,0,1.1);
    fHistZreco_prob_ee->SetXTitle("Probability");
    fHistZreco_prob_ee->SetYTitle("# Events");
    
    //Histrogramm Z->ee with probability cut
    
    gDirectory->mkdir("with_probability_cut","Z->ee after probability cut");
    gDirectory->cd("with_probability_cut");

    fHistZreco_m_ee_with_prob   = new TH1F("h_Z_m_ee_with_prob", "Invariant Z mass (Z->ee) with_prob",
				 50, 65, 115);
    fHistZreco_m_ee_with_prob->SetXTitle("m_{Z} (GeV)");
    fHistZreco_m_ee_with_prob->SetYTitle("Number of Entries");
    fHistZreco_pt_ee_with_prob  = new TH1F("h_Z_pt_ee_with_prob", "Z-p_{#perp} (Z->ee)",
				 100, 0, 200);
    fHistZreco_pt_ee_with_prob->SetXTitle("p_{#perp} (GeV)");
    fHistZreco_pt_ee_with_prob->SetYTitle("Number of Entries");
    fHistZreco_phi_ee_with_prob = new TH1F("h_Z_phi_ee_with_prob", "Z boson #phi distribution (Z->ee)",
				 128, -3.2, 3.2);
    fHistZreco_phi_ee_with_prob->SetXTitle("#phi (rad)");
    fHistZreco_phi_ee_with_prob->SetYTitle("Number of Entries");
    fHistZreco_eta_ee_with_prob = new TH1F("h_Z_eta_ee_with_prob", "Z boson #eta distribution (Z->ee)",
				 50, -3, 3);
    fHistZreco_eta_ee_with_prob->SetXTitle("#eta");
    fHistZreco_eta_ee_with_prob->SetYTitle("Number of Entries");
    fHistZreco_n_ee_with_prob   = new TH1F("h_Z_n_ee_with_prob", "Reconstructed Z bosons per event (Z->ee)",
				 10, 0, 10);
    fHistZreco_n_ee_with_prob->SetXTitle("N_{Z}/evt");
    fHistZreco_n_ee_with_prob->SetYTitle("Number of Entries");

    fHistZreco_DeltaPhi_ee_with_prob = new TH1F("h_Z_DeltaPhi_ee", "Abs. Value of Phi(e+)-Phi(e-) (Z->ee)",
				      30,2,4);
    fHistZreco_DeltaPhi_ee_with_prob->SetXTitle("|#Delta #phi_{ee}|");
    fHistZreco_DeltaPhi_ee_with_prob->SetYTitle("# Events");
    
    gDirectory->cd("../");

    // electron (e+) histograms in sub-folder
    gDirectory->mkdir("eplus", "e+ from Z0 -> ee");
    gDirectory->cd("eplus");
    
    fHistZreco_eplus_pt = new TH1F("h_Z_eplus_pt", "e^{+}-p_{#perp} (Z->ee)",
				   100, 0, 100);
    fHistZreco_eplus_pt->SetXTitle("p_{#perp} (GeV)");
    fHistZreco_eplus_pt->SetYTitle("Number of Entries");
    fHistZreco_eplus_eta = new TH1F("h_Z_eplus_eta", "e^{+}:  #eta distribution (Z->ee)",
				    50, -3, 3);
    fHistZreco_eplus_eta->SetXTitle("#eta");
    fHistZreco_eplus_eta->SetYTitle("Number of Entries");
    fHistZreco_eplus_phi = new TH1F("h_Z_eplus_phi", "e^{+}:  #phi distribution (Z->ee)",
				    50, -3.14, 3.14);
    fHistZreco_eplus_phi->SetXTitle("#phi");
    fHistZreco_eplus_phi->SetYTitle("Number of Entries");
    
    gDirectory->cd("../"); 
    
    // electron (e-) histograms in sub-folder
    gDirectory->mkdir("eminus", "e- from Z0 -> ee");
    gDirectory->cd("eminus");
    
    fHistZreco_eminus_pt = new TH1F("h_Z_eminus_pt", "e^{-}-p_{#perp} (Z->ee)",
				    100, 0, 100);
    fHistZreco_eminus_pt->SetXTitle("p_{#perp} (GeV)");
    fHistZreco_eminus_pt->SetYTitle("Number of Entries");
    fHistZreco_eminus_eta = new TH1F("h_Z_eminus_eta", "e^{-}:  #eta distribution (Z->ee)",
				     50, -5, 5);
    fHistZreco_eminus_eta->SetXTitle("#eta");
    fHistZreco_eminus_eta->SetYTitle("Number of Entries");
    fHistZreco_eminus_phi = new TH1F("h_Z_eminus_phi", "e^{-}:  #phi distribution (Z->ee)",
				     50, -3.14, 3.14);
    fHistZreco_eminus_phi->SetXTitle("#phi");
    fHistZreco_eminus_phi->SetYTitle("Number of Entries");

    gDirectory->cd("../../");

    //
    // Z -> mumu
    // ---------
    gDirectory->mkdir("mumu", "Z0 -> mumu");
    gDirectory->cd("mumu");
    fHistZreco_m_mumu   = new TH1F("h_Z_m_mumu", "Invariant Z mass (Z->#mu#mu)",
				   50, 65, 115);
    fHistZreco_m_mumu->SetXTitle("m_{Z} (GeV)");
    fHistZreco_m_mumu->SetYTitle("Number of Entries");
    fHistZreco_pt_mumu  = new TH1F("h_Z_pt_mumu", "Z-p_{#perp} (Z->#mu#mu)",
				   100, 0, 200);
    fHistZreco_pt_mumu->SetXTitle("p_{#perp} (GeV)");
    fHistZreco_pt_mumu->SetYTitle("Number of Entries");
    fHistZreco_phi_mumu = new TH1F("h_Z_phi_mumu", "Z boson #phi distribution (Z->#mu#mu)",
				   128, -3.2, 3.2);
    fHistZreco_phi_mumu->SetXTitle("#phi (rad)");
    fHistZreco_phi_mumu->SetYTitle("Number of Entries");
    fHistZreco_eta_mumu = new TH1F("h_Z_eta_mumu", "Z boson #eta distribution (Z->#mu#mu)", 50, -5, 5);
    fHistZreco_eta_mumu->SetXTitle("#eta");
    fHistZreco_eta_mumu->SetYTitle("Number of Entries");
    fHistZreco_n_mumu   = new TH1F("h_Z_n_mumu", "Reconstructed Z bosons per event (Z->#mu#mu)",
				   10, 0, 10);
    fHistZreco_n_mumu->SetXTitle("N_{Z}/evt");
    fHistZreco_n_mumu->SetYTitle("Number of Entries");
    fHistZreco_DeltaPhi_mumu = new TH1F("h_Z_DeltaPhi_mumu", "Abs. Value of Phi(#mu+)-Phi(#mu-) (Z->#mu#mu)",
				 30,2,4);
    fHistZreco_DeltaPhi_mumu->SetXTitle("|#Delta #phi_{#mu#mu}|");
    fHistZreco_DeltaPhi_mumu->SetYTitle("# Events");
    
    fHistZreco_prob_mumu = new TH1F("h_Z_prob_mumu", "Probability of Z->mumu",
				      100,0,1);
    fHistZreco_prob_mumu->SetXTitle("Probability");
    fHistZreco_prob_mumu->SetYTitle("# Events");

    //Z->mumu after probability cut
    gDirectory->mkdir("with_probability_cut","Z->mumu after probability cut");
    gDirectory->cd("with_probability_cut");
    fHistZreco_m_mumu_with_prob   = new TH1F("h_Z_m_mumu_with_prob", "Invariant Z mass (Z->#mu#mu)_with_prob",
				   50, 65, 115);
    fHistZreco_m_mumu_with_prob->SetXTitle("m_{Z} (GeV)");
    fHistZreco_m_mumu_with_prob->SetYTitle("Number of Entries");
    fHistZreco_pt_mumu_with_prob  = new TH1F("h_Z_pt_mumu", "Z-p_{#perp} (Z->#mu#mu)",
				   100, 0, 200);
    fHistZreco_pt_mumu_with_prob->SetXTitle("p_{#perp} (GeV)");
    fHistZreco_pt_mumu_with_prob->SetYTitle("Number of Entries");
    fHistZreco_phi_mumu_with_prob = new TH1F("h_Z_phi_mumu", "Z boson #phi distribution (Z->#mu#mu)",
				   128, -3.2, 3.2);
    fHistZreco_phi_mumu_with_prob->SetXTitle("#phi (rad)");
    fHistZreco_phi_mumu_with_prob->SetYTitle("Number of Entries");
    fHistZreco_eta_mumu_with_prob = new TH1F("h_Z_eta_mumu", "Z boson #eta distribution (Z->#mu#mu)", 50, -5, 5);
    fHistZreco_eta_mumu_with_prob->SetXTitle("#eta");
    fHistZreco_eta_mumu_with_prob->SetYTitle("Number of Entries");
    fHistZreco_n_mumu_with_prob   = new TH1F("h_Z_n_mumu", "Reconstructed Z bosons per event (Z->#mu#mu)",
				   10, 0, 10);
    fHistZreco_n_mumu_with_prob->SetXTitle("N_{Z}/evt");
    fHistZreco_n_mumu_with_prob->SetYTitle("Number of Entries");
    fHistZreco_DeltaPhi_mumu_with_prob = new TH1F("h_Z_DeltaPhi_mumu", "Abs. Value of Phi(#mu+)-Phi(#mu-) (Z->#mu#mu)",
				 30,2,4);
    fHistZreco_DeltaPhi_mumu_with_prob->SetXTitle("|#Delta #phi_{#mu#mu}|");
    fHistZreco_DeltaPhi_mumu_with_prob->SetYTitle("# Events");
    
    gDirectory->cd("../");

    // muon (mu+) histograms in sub-folder
    gDirectory->mkdir("muplus", "mu+ from Z0 -> mumu");
    gDirectory->cd("muplus");
    
    fHistZreco_muplus_pt = new TH1F("h_Z_muplus_pt", "#mu^{+}-p_{#perp} (Z->#mu#mu)",
				   100, 0, 100);
    fHistZreco_muplus_pt->SetXTitle("p_{#perp} (GeV)");
    fHistZreco_muplus_pt->SetYTitle("Number of Entries");
    fHistZreco_muplus_eta = new TH1F("h_Z_muplus_eta", "mu^{+}:  #eta distribution (Z->#mu#mu)",
				    50, -3, 3);
    fHistZreco_muplus_eta->SetXTitle("#eta");
    fHistZreco_muplus_eta->SetYTitle("Number of Entries");
    fHistZreco_muplus_phi = new TH1F("h_Z_muplus_phi", "mu^{+}:  #phi distribution (Z->#mu#mu)",
				    50, -3.14, 3.14);
    fHistZreco_muplus_phi->SetXTitle("#phi");
    fHistZreco_muplus_phi->SetYTitle("Number of Entries");
    
    gDirectory->cd("../"); 
    
    // muon (mu-) histograms in sub-folder
    gDirectory->mkdir("muminus", "mu- from Z0 -> mumu");
    gDirectory->cd("muminus");
    
    fHistZreco_muminus_pt = new TH1F("h_Z_muminus_pt", "#mu^{-}-p_{#perp} (Z->#mu#mu)",
				    100, 0, 100);
    fHistZreco_muminus_pt->SetXTitle("p_{#perp} (GeV)");
    fHistZreco_muminus_pt->SetYTitle("Number of Entries");
    fHistZreco_muminus_eta = new TH1F("h_Z_muminus_eta", "mu^{-}:  #eta distribution (Z->#mu#mu)",
				     50, -3, 3);
    fHistZreco_muminus_eta->SetXTitle("#eta");
    fHistZreco_muminus_eta->SetYTitle("Number of Entries");
    fHistZreco_muminus_phi = new TH1F("h_Z_muminus_phi", "mu^{-}:  #phi distribution (Z->#mu#mu)",
				     50, -3.14, 3.14);
    fHistZreco_muminus_phi->SetXTitle("#phi");
    fHistZreco_muminus_phi->SetYTitle("Number of Entries");

    gDirectory->cd("../../");


    //
    // Z -> tautau
    // -----------
    gDirectory->mkdir("tautau", "Z0 -> tautau");
    gDirectory->cd("tautau");
    fHistZreco_m_tautau   = new TH1F("h_Z_m_tautau", "Invariant Z mass (Z->#tau#tau)",
				     50, 65, 115);
    fHistZreco_m_tautau->SetXTitle("m_{Z} (GeV)");
    fHistZreco_m_tautau->SetYTitle("Number of Entries");
    fHistZreco_pt_tautau  = new TH1F("h_Z_pt_tautau", "Z-p_{#perp} (Z->#tau#tau)",
				     100, 0, 200);
    fHistZreco_pt_tautau->SetXTitle("p_{#perp} (GeV)");
    fHistZreco_pt_tautau->SetYTitle("Number of Entries");
    fHistZreco_phi_tautau = new TH1F("h_Z_phi_tautau", "Z boson #phi distribution (Z->#tau#tau)",
				     128, -3.2, 3.2);
    fHistZreco_phi_tautau->SetXTitle("#phi (rad)");
    fHistZreco_phi_tautau->SetYTitle("Number of Entries");
    fHistZreco_eta_tautau = new TH1F("h_Z_eta_tautau", "Z boson #eta distribution (Z->#tau#tau)",
				     50, -5, 5);
    fHistZreco_eta_tautau->SetXTitle("#eta");
    fHistZreco_eta_tautau->SetYTitle("Number of Entries");
    fHistZreco_n_tautau   = new TH1F("h_Z_n_tautau", "Reconstructed Z bosons per event (Z->#tau#tau)",
				     10, 0, 10);
    fHistZreco_n_tautau->SetXTitle("N_{Z}/evt");
    fHistZreco_n_tautau->SetYTitle("Number of Entries");
    fHistZreco_DeltaPhi_tautau = new TH1F("h_Z_DeltaPhi_tautau", "Abs. Value of Phi(tau+)-Phi(tau-) (Z->#tau#tau)",
				 30,2,4);
    fHistZreco_DeltaPhi_tautau->SetXTitle("|#Delta #phi_{#tau#tau}|");
    fHistZreco_DeltaPhi_tautau->SetYTitle("# Events");
    gDirectory->cd("../");

    //
    // Z -> ll
    // -------
    gDirectory->mkdir("leptons", "Z0 -> ll");
    gDirectory->cd("leptons");
    fHistZreco_m_ll   = new TH1F("h_Z_m_ll", "Invariant Z mass (Z -> ll)",
				      50, 65, 115);
    fHistZreco_m_ll->SetXTitle("m_{Z} (GeV)");
    fHistZreco_m_ll->SetYTitle("Number of Entries");
    fHistZreco_pt_ll  = new TH1F("h_Z_pt_ll", "Z-p_{#perp} (Z -> ll)",
				      100, 0, 200);
    fHistZreco_pt_ll->SetXTitle("p_{#perp} (GeV)");
    fHistZreco_pt_ll->SetYTitle("Number of Entries");
    fHistZreco_phi_ll = new TH1F("h_Z_phi_ll", "Z boson #phi distribution (Z -> ll)",
				      128, -3.2, 3.2);
    fHistZreco_phi_ll->SetXTitle("#phi (rad)");
    fHistZreco_phi_ll->SetYTitle("Number of Entries");
    fHistZreco_eta_ll = new TH1F("h_Z_eta_ll", "Z boson #eta distribution (Z -> ll)",
				      50, -5, 5);
    fHistZreco_eta_ll->SetXTitle("#eta");
    fHistZreco_eta_ll->SetYTitle("Number of Entries");
    fHistZreco_n_ll   = new TH1F("h_Z_n_ll", "Reconstructed Z bosons per event (Z -> ll)",
				      10, 0, 10);
    fHistZreco_n_ll->SetXTitle("N_{Z}/evt");
    fHistZreco_n_ll->SetYTitle("Number of Entries");
    fHistZreco_DeltaPhi_ll = new TH1F("h_Z_DeltaPhi_ll", "Abs. Value of Phi(l+)-Phi(l-) (Z->ll)",
				 30,2,4);
    fHistZreco_DeltaPhi_ll->SetXTitle("|#Delta #phi_{ll}|");
    fHistZreco_DeltaPhi_ll->SetYTitle("# Events");
    
    gDirectory->cd("../../");
    
    // ===================
    // Book bkg histograms
    // ===================
    gDirectory->mkdir("Bkg", "Same charged candidates");
    gDirectory->cd("Bkg");

    //
    // Z -> ee
    // -------
    gDirectory->mkdir("ee", "Z0 -> ee");
    gDirectory->cd("ee");
    fHistZreco_m_ee_bkg   = new TH1F("h_Z_m_ee_bkg", "Bkg. Invariant Z mass (Z->ee)",
				     50, 65, 115);
    fHistZreco_m_ee_bkg->SetXTitle("m_{Z} (GeV)");
    fHistZreco_m_ee_bkg->SetYTitle("Number of Entries");
    fHistZreco_pt_ee_bkg  = new TH1F("h_Z_pt_ee_bkg", "Bkg. Z-p_{#perp} (Z->ee)",
				     100, 0, 200);
    fHistZreco_pt_ee_bkg->SetXTitle("p_{#perp} (GeV)");
    fHistZreco_pt_ee_bkg->SetYTitle("Number of Entries");
    fHistZreco_phi_ee_bkg = new TH1F("h_Z_phi_ee_bkg", "Bkg. Z boson #phi distribution (Z->ee)",
				     128, -3.2, 3.2);
    fHistZreco_phi_ee_bkg->SetXTitle("#phi (rad)");
    fHistZreco_phi_ee_bkg->SetYTitle("Number of Entries");
    fHistZreco_eta_ee_bkg = new TH1F("h_Z_eta_ee_bkg", "Bkg. Z boson #eta distribution (Z->ee)",
				     50, -5, 5);
    fHistZreco_eta_ee_bkg->SetXTitle("#eta");
    fHistZreco_eta_ee_bkg->SetYTitle("Number of Entries");
    fHistZreco_DeltaPhi_ee_bkg = new TH1F("h_Z_DeltaPhi_ee_bkg", "Abs. Value of Phi(e+)-Phi(e-) (Z->ee), Bkg.",
				     30,2,4);
    fHistZreco_DeltaPhi_ee_bkg->SetXTitle("|#Delta #phi_{ee}| Bkg.");
    fHistZreco_DeltaPhi_ee_bkg->SetYTitle("# Events");
    gDirectory->cd("../");

    //
    // Z -> mumu
    // ---------
    gDirectory->mkdir("mumu", "Z0 -> mumu");
    gDirectory->cd("mumu");
    fHistZreco_m_mumu_bkg   = new TH1F("h_Z_m_mumu_bkg", "Bkg. Invariant Z mass (Z->#mu#mu)",
				       50, 65, 115);
    fHistZreco_m_mumu_bkg->SetXTitle("m_{Z} (GeV)");
    fHistZreco_m_mumu_bkg->SetYTitle("Number of Entries");
    fHistZreco_pt_mumu_bkg  = new TH1F("h_Z_pt_mumu_bkg", "Bkg. Z-p_{#perp} (Z->#mu#mu)",
				       100, 0, 200);
    fHistZreco_pt_mumu_bkg->SetXTitle("p_{#perp} (GeV)");
    fHistZreco_pt_mumu_bkg->SetYTitle("Number of Entries");
    fHistZreco_phi_mumu_bkg = new TH1F("h_Z_phi_mumu_bkg", "Bkg. Z boson #phi distribution (Z->#mu#mu)",
				       128, -3.2, 3.2);
    fHistZreco_phi_mumu_bkg->SetXTitle("#phi (rad)");
    fHistZreco_phi_mumu_bkg->SetYTitle("Number of Entries");
    fHistZreco_eta_mumu_bkg = new TH1F("h_Z_eta_mumu_bkg", "Bkg. Z boson #eta distribution (Z->#mu#mu)",
				       50, -5, 5);
    fHistZreco_eta_mumu_bkg->SetXTitle("#eta");
    fHistZreco_eta_mumu_bkg->SetYTitle("Number of Entries");
    fHistZreco_DeltaPhi_mumu_bkg = new TH1F("h_Z_DeltaPhi_mumu_bkg", "Abs. Value of Phi(#mu+)-Phi(#mu-) (Z->#mu#mu), Bkg.",
				 30,2,4);
    fHistZreco_DeltaPhi_mumu_bkg->SetXTitle("|#Delta #phi_{#mu#mu}| Bkg.");
    fHistZreco_DeltaPhi_mumu_bkg->SetYTitle("# Events");
    gDirectory->cd("../");

    //
    // Z -> tautau
    // -----------
    gDirectory->mkdir("tautau", "Z0 -> tautau");
    gDirectory->cd("tautau");
    fHistZreco_m_tautau_bkg   = new TH1F("h_Z_m_tautau_bkg", "Invariant Z mass (Z->#tau#tau)",
					 50, 65, 115);
    fHistZreco_m_tautau_bkg->SetXTitle("m_{Z} (GeV)");
    fHistZreco_m_tautau_bkg->SetYTitle("Number of Entries");
    fHistZreco_pt_tautau_bkg  = new TH1F("h_Z_pt_tautau_bkg", "Z-p_{#perp} (Z->#tau#tau)",
					 100, 0, 200);
    fHistZreco_pt_tautau_bkg->SetXTitle("p_{#perp} (GeV)");
    fHistZreco_pt_tautau_bkg->SetYTitle("Number of Entries");
    fHistZreco_phi_tautau_bkg = new TH1F("h_Z_phi_tautau_bkg",
					 "Z boson #phi distribution (Z->#tau#tau)", 128, -3.2, 3.2);
    fHistZreco_phi_tautau_bkg->SetXTitle("#phi (rad)");
    fHistZreco_phi_tautau_bkg->SetYTitle("Number of Entries");
    fHistZreco_eta_tautau_bkg = new TH1F("h_Z_eta_tautau_bkg", "Z boson #eta distribution (Z->#tau#tau)",
					 50, -5, 5);
    fHistZreco_eta_tautau_bkg->SetXTitle("#eta");
    fHistZreco_eta_tautau_bkg->SetYTitle("Number of Entries");
    fHistZreco_DeltaPhi_tautau_bkg = new TH1F("h_Z_DeltaPhi_tautau_bkg",
					      "Abs. Value of Phi(#tau+)-Phi(#tau-) (Z->#tau#tau), Bkg.", 30,2,4);
    fHistZreco_DeltaPhi_tautau_bkg->SetXTitle("|#Delta #phi_{#tau#tau}| Bkg.");
    fHistZreco_DeltaPhi_tautau_bkg->SetYTitle("# Events");
    gDirectory->cd("../");

    //
    // Z -> ll
    // -------
    gDirectory->mkdir("leptons", "Z0 -> ll");
    gDirectory->cd("leptons");
    fHistZreco_m_ll_bkg   = new TH1F("h_Z_m_ll_bkg", "Invariant Z mass (Z -> ll)",
					  50, 65, 115);
    fHistZreco_m_ll_bkg->SetXTitle("m_{Z} (GeV)");
    fHistZreco_m_ll_bkg->SetYTitle("Number of Entries");
    fHistZreco_pt_ll_bkg  = new TH1F("h_Z_pt_ll_bkg", "Z-p_{#perp} (Z -> ll)",
					  100, 0, 200);
    fHistZreco_pt_ll_bkg->SetXTitle("p_{#perp} (GeV)");
    fHistZreco_pt_ll_bkg->SetYTitle("Number of Entries");
    fHistZreco_phi_ll_bkg = new TH1F("h_Z_phi_ll_bkg", "Z boson #phi distribution (Z -> ll)",
					  128, -3.2, 3.2);
    fHistZreco_phi_ll_bkg->SetXTitle("#phi (rad)");
    fHistZreco_phi_ll_bkg->SetYTitle("Number of Entries");
    fHistZreco_eta_ll_bkg = new TH1F("h_Z_eta_ll_bkg", "Z boson #eta distribution (Z -> ll)",
					  50, -5, 5);
    fHistZreco_eta_ll_bkg->SetXTitle("#eta");
    fHistZreco_eta_ll_bkg->SetYTitle("Number of Entries");
    fHistZreco_DeltaPhi_ll_bkg = new TH1F("h_Z_DeltaPhi_ll_bkg",
					      "Abs. Value of Phi(l+)-Phi(l-) (Z->ll), Bkg.", 30,2,4);
    fHistZreco_DeltaPhi_ll_bkg->SetXTitle("|#Delta #phi_{ll}| Bkg.");
    fHistZreco_DeltaPhi_ll_bkg->SetYTitle("# Events");

    gDirectory->cd("../../");

    
    
}


//____________________________________________________________________

Bool_t AtlZ0Finder::AnalyzeEvent() {
    //
    // Event analysis
    //

    /*  for(Int_t i=0; i<fEvent->GetN_Electrons(); i++){
	
	AtlElectron *elec = (AtlElectron*)fEvent->GetElectrons()->At(i);
	cout << "success" << endl;

	AtlIDTrack *trk = (AtlIDTrack*)elec->GetIDTrack();
	cout << "elec " << elec << " trk "<< trk << endl;
	elec->Print();

	}*/



    // Clean-up bkg list from previous event
    fBkgZ0Decays->Delete();
    
    // InitEvent
    InitEvent();

    // Fill MC histograms for true Z bosons, do it only if List is filled with MoneCarlo Particles
    if (fEvent->IsMC()){
	FillTruthHistograms();
    }

    // Require at least 2 leptons
    if ( !(( fModeLepton & kElectron && fSignalElectrons->GetEntries() >= 2 ) ||
	   ( fModeLepton & kMuon     && fSignalMuons->GetEntries()     >= 2 ) ||
	   ( fModeLepton & kTau      && fSignalTaus->GetEntries()      >= 2 )) ) return kFALSE;

    // Reconstruct Z0 decays
    if ( fMode == kKinFit ) {
            
	ReconstructZ0KinFit(fSignalElectrons);
	ReconstructZ0KinFit(fSignalMuons);
	//ReconstructZ0KinFit(fSignalTaus); No Tau Leptons used, Tau-Electron Fake Rate too high
    } else if ( fMode == kCutBased ) {
	ReconstructZ0CutBased(fSignalElectrons);
	ReconstructZ0CutBased(fSignalMuons);
	// ReconstructZ0CutBased(fSignalTaus); No Tau Leptons used, Tau-Electron fake rate too high
    } else {
	Error("AnalyzeEvent",
	      "Invalid mode given. Abort!");
	gSystem->Abort(0);
    }

    // Clean-up
    delete fSignalElectrons;
    delete fSignalMuons;
    delete fSignalTaus; 
    
    return kTRUE;
}

//____________________________________________________________________

void AtlZ0Finder::InitEvent() {
    //
    // Prepare event analysis
    //

    // Fill CutOverview Histogramm for electrons
    // Cut of Electrons in tight,high_pt,pt_max,pt_min,eta 
    //--------------------------------------------------------------------
    AtlElectron *et=0;
    Int_t tight=0;
    Int_t eta=0;
    Int_t etcone20=0;
    Int_t pt=0;
    Int_t highpt=0;
    for (Int_t i=0;i<fEvent->GetN_Electrons();i++){
	et=(AtlElectron*)fEvent->GetElectrons()->At(i);
	if (et->IsTight()){
	    tight++;
	    if ((et->Eta()>=-1.9) && (et->Eta()<=1.9)){
		if ((TMath::Abs(et->Eta())<=1.37)||(TMath::Abs(et->Eta())>=1.52) ){
	            eta++;
	            if (et->GetEtCone20()<=100.){
		       etcone20++;
		       if ((et->Pt()>=10.)&&(et->Pt()<=10.e10)){
		   	    pt++;
			    if (et->IsHighPtElectron()){
				    highpt++;
			    }
		       }
		    }    
		}
	}
	}
     }   
    //because you need 2 electrons for one Z0
    for (Int_t i=0; i<tight/2;i++){
	SetCutFlow("Cut of Tight electrons");//# of electrons divided 2
    }
    for (Int_t i=0; i<eta/2;i++){//# of electrons divided 2
	 SetCutFlow("Cut of Eta");
    }
    for (Int_t i=0; i<etcone20/2;i++){//# of electrons divided 2
 	 SetCutFlow("Cut of EtCone20");
    }
    for (Int_t i=0; i<pt/2;i++){//# of electrons divided 2
 	 SetCutFlow("Cut of Pt");
    }
    for (Int_t i=0;i<highpt/2;i++){
	SetCutFlow("Cut of HighPtElectron");
    }
    //end of filling histogramm-------------------------------------------------
        //Get Leptons for analyze event
    if(fModeLepton & kElectron){
        fSignalElectrons = fEvent->GetElectrons(fSignalElectronAuthor,
	                         	        fSignalElectronIsEM,
					        fSignalElectron_Pt_min,
					        fSignalElectron_Pt_max,
					        fSignalElectron_Eta_min,
					        fSignalElectron_Eta_max,
					        fSignalElectron_EtCone20_max);
         }
    if (fModeLepton & kMuon) {
        fSignalMuons = fEvent->GetMuons(fSignalMuonAuthor,
				        fSignalMuon_Pt_min,
				        fSignalMuon_Pt_max,
				        fSignalMuon_Eta_min,
				        fSignalMuon_Eta_max,
				        fSignalMuon_Chi2_max,
				        fSignalMuon_EtCone20_max);
       }
    // if (fModeLepton & kTau) {
    //     fSignalTaus = fEvent->GetTaus(fSignalTauAuthor,
    //                                   fSignalTauAuthorFlag,
    // 				      fSignalTau_Pt_min,
    // 				      fSignalTau_Pt_max,
    // 				      fSignalTau_Eta_min,
    // 				      fSignalTau_Eta_max);
    // 	}
    // Tau lists are being filled :)ss
}

//____________________________________________________________________

void AtlZ0Finder::ReconstructZ0CutBased(TList *SignalLeptons) {
    //
    // Cut Based Z-boson reconstruction from lepton-pairs
    //
    
    // Iterators over particles
    TIter next_l1(SignalLeptons);

    HepParticle *l1 = 0;
    HepParticle *l2 = 0;
    TLorentzVector p_l1;
    TLorentzVector p_l2;
    TLorentzVector p_Z;
    
    while ( (l1 = (HepParticle*)next_l1()) ) {
	TIter next_l2(SignalLeptons);
        while ( (l2 = (HepParticle*) next_l2()) ) {
	    if ( l1 == l2 ) continue;
	    
            // Get lepton 4-momentum
            p_l1 = l1->P();
            p_l2 = l2->P();
            
            // Reconstruct Z0 4-momentum
            p_Z = p_l1 + p_l2;
            Float_t m_Z = p_Z.M();
            
            // Use only m_in in Z Mass peak
            if ( ( m_Z > fZBoson_M_min ) && ( m_Z < fZBoson_M_max ) ) {
		if (fchargetest && (l1->Charge()*l2->Charge() > 0.) ) {  // Add oppositely charged combinations to the event
		    HepParticle Fit_Daughter1(1,p_l1.Px(),p_l1.Py(),
					      p_l1.Pz(), p_l1.E(),l1->GetPdgCode());
		    HepParticle Fit_Daughter2(2,p_l2.Px(),p_l2.Py(),
					      p_l2.Pz(), p_l2.E(),l2->GetPdgCode());
		    fBkgZ0Decays->Add(new HepZ0Decay(1, p_Z.Px(), p_Z.Py(), p_Z.Pz(), p_Z.E(),
						     (TObject*)l1, (TObject*)l2,
						     Fit_Daughter1, Fit_Daughter2));
		    
		} else { // Add same charged combinations to the bkg list
		    HepParticle Fit_Daughter1(1,p_l1.Px(),p_l1.Py(),
					      p_l1.Pz(), p_l1.E(),l1->GetPdgCode());
		    HepParticle Fit_Daughter2(2,p_l2.Px(),p_l2.Py(),
					      p_l2.Pz(), p_l2.E(),l2->GetPdgCode());
		    fEvent->AddZ0Decay(p_Z.Px(), p_Z.Py(), p_Z.Pz(), p_Z.E(),
				       (TObject*)l1, (TObject*)l2,
 				       Fit_Daughter1, Fit_Daughter2);
		}
	    }
            SignalLeptons->Remove(l1);
        }
	
    }
}

//____________________________________________________________________

void AtlZ0Finder::ReconstructZ0KinFit(TList *SignalLeptons) {
    //
    // Z-boson reconstruction from lepton-pairs using KinFitter
    //
    if (SignalLeptons!=0){
	for (Int_t i=0;i< (SignalLeptons->GetEntries()/2);i++){
	    SetCutFlow("# Cut of all at same time");
	}
	// Iterators over particles
	TIter next_l1(SignalLeptons);
	
	HepParticle *l1 = 0;    
	HepParticle *l2 = 0;    
	TLorentzVector p_Z;   

	TVector3 p_l1;   
	TVector3 p_l2;   

	Double_t pt_true;
	Double_t eta_true;
	Double_t phi_true;
	Double_t pt_reco;
	Double_t eta_reco;
	Double_t phi_reco;
	
	HepMCParticle *l1True = 0;
	HepMCParticle *l2True = 0;
	
	Double_t cp, ceta,cphi;
	// -------------------------------------------
	
	while ( (l1 = (HepParticle*)next_l1()) ) {
	    TIter next_l2(SignalLeptons);
	    while ( (l2 = (HepParticle*) next_l2()) ) {
		if ( l1 == l2 ) continue;
		
		// Get lepton 3-momentum
		p_l1 = l1->P3();
		p_l2 = l2->P3();
		
		// Get Covariance Matrices from TrackFit (inner detector track)
		TMatrixD cov_l1(3,3);
		TMatrixD cov_l2(3,3);
		
		//still not working because of failing covariance matrices in Zee samples
		l1->GetCovMatrixPtEtaPhi(cov_l1);
		l2->GetCovMatrixPtEtaPhi(cov_l2);
		
		// define particels necessary for execution of the fit
		TFitParticlePtEtaPhi FitExec_l1("FitExec_l1","FitExec_l1",&p_l1,l1->Mass(),&cov_l1);
		TFitParticlePtEtaPhi FitExec_l2("FitExec_l2","FitExec_l2",&p_l2,l2->Mass(),&cov_l2);
		
		// definition of Z-Boson mass constraint
		TFitConstraintMGaus MZCons("ZMassConstraint","ZMassConstraintGaus", 0, 0, 91.2, 2.5);
		MZCons.addParticles1(&FitExec_l1,&FitExec_l2);
		
		// definition of transverse momentum-conservation
		vector<TAbsFitParticle*> vec_constraint;
		vec_constraint.push_back(&FitExec_l1);
		vec_constraint.push_back(&FitExec_l2);
		
		TFitConstraintEpGaus PxCons(&vec_constraint, TFitConstraintEp::pX,0.0001,8.1);
		TFitConstraintEpGaus PyCons(&vec_constraint, TFitConstraintEp::pY,0.0001,8.1);
		
		// fitting characteristics
		TKinFitter fitter;
		
		fitter.addMeasParticle(&FitExec_l1);
		fitter.addMeasParticle(&FitExec_l2);
		
		fitter.addConstraint(&MZCons);
		fitter.setMaxNbIter(2000);   // maximum number of iterations
		fitter.setMaxDeltaS(5e-5); // maximum deviation of the minimum function within two iterations
		fitter.setMaxF(1e-4);      // maximum value of constraints
		fitter.setVerbosity(0);    // verbosity level
		
		// Kinematic Fitting
		fitter.fit();
		
		// Fill Histogram IF(!) the fit converged (fitter.getStatus==0)
		if ( fitter.getS()<0. ) {
		    //fitter.printMatrices();
		    Error("ReconstructZ0KinFit",
			  "fitter.getS()<0. Abort!");
		    gSystem->Abort(0);
		}
		if(  fitter.getStatus() ) { cout << endl << "DIVERGENCE" << endl; }
		if( ! fitter.getStatus() ) {
		cout<<endl<<"geht doch weiter"<<endl;
		// Copy fittet Vectors
		// define momentum used as output of the fit
		TLorentzVector FitP_l1;
		TLorentzVector FitP_l2;
		
		FitP_l1=(*FitExec_l1.getCurr4Vec());
		FitP_l2=(*FitExec_l2.getCurr4Vec());
		// Reconstruct Z0 4-momentum
		p_Z = FitP_l1 + FitP_l2;
		SetChi2(fitter.getS());
		
	        
		SetNDoF(fitter.getNDF());
		SetNbIter(fitter.getNbIter());
		SetConsValue(fitter.getF());
		
		
		// Add Z0 to event fill the signal list
		// the user can decide, if the Z0-Finder should do the charge test or not
		SetCutFlow("#Z0 without chargetest");
		
		
		
		if (fchargetest && (l1->Charge()*l2->Charge() > 0.) ) {//fill Bkg list
		    HepParticle Fit_Daughter1(1,FitP_l1.Px(),FitP_l1.Py(),
					      FitP_l1.Pz(), FitP_l1.E(),l1->GetPdgCode());
		    HepParticle Fit_Daughter2(2,FitP_l2.Px(),FitP_l2.Py(),
					      FitP_l2.Pz(), FitP_l2.E(),l2->GetPdgCode());
		    fBkgZ0Decays->Add(new HepZ0Decay(1, p_Z.Px(), p_Z.Py(), p_Z.Pz(), p_Z.E(),
						     (TObject*)l1, (TObject*)l2,
						     Fit_Daughter1, Fit_Daughter2));
		}else{
		    HepParticle Fit_Daughter1(1,FitP_l1.Px(),FitP_l1.Py(),
					      FitP_l1.Pz(), FitP_l1.E(),l1->GetPdgCode());
		    HepParticle Fit_Daughter2(2,FitP_l2.Px(),FitP_l2.Py(),
					      FitP_l2.Pz(), FitP_l2.E(),l2->GetPdgCode());
		    fEvent->AddZ0Decay(p_Z.Px(), p_Z.Py(), p_Z.Pz(), p_Z.E(),
				       (TObject*)l1, (TObject*)l2,
 				       Fit_Daughter1, Fit_Daughter2);
		    SetCutFlow("#Z0 after chargetest");
		}
		
		
			
		
		if (fEvent->IsMC()){
		    // small resolution study ---------------------------------------------
		    
		    l1True=fEvent->FindMatchedMCParticle((HepParticle*)l1,false,0.1,0.1);
		    l2True=fEvent->FindMatchedMCParticle((HepParticle*)l2,false,0.1,0.1);
		    if (l1True!=0 && l1True->IsElectron()) {
			pt_true=l1True->Pt();
			eta_true=l1True->Eta();
			phi_true=l1True->Phi();
			pt_reco=l1->Pt();
			eta_reco=l1->Eta();
			phi_reco=l1->Phi();
			cp= (pt_reco-pt_true)/pt_true;
			ceta=(eta_reco-eta_true)/eta_true;
			cphi=(phi_reco-phi_true)/phi_true;
			fHist_Zee_PtRes->Fill( cp, GetPreTagEvtWeight() );
			fHist_Zee_EtaRes->Fill(  ceta, GetPreTagEvtWeight() );
			fHist_Zee_PhiRes->Fill(  cphi, GetPreTagEvtWeight() );
			if ( cp*cp<0.09 && TMath::Abs(ceta)<0.01 && TMath::Abs(cphi)<0.005 ) {
			    // prevent from very bad matching distorting the result
			    c11+=cp*cp;
			    c22+=ceta*ceta;
			    c33+=cphi*cphi;
			    c12+=cp*ceta;
			    c13+=cp*cphi;
			    c23+=ceta*cphi;
			    TruthMatchCounter++;
			}
		   }
		    
		   // resolution study finished --------------------------------------------





		    
		   // computation of efficiency

		    fTrueReco = false;
		    HepZ0Decay    *ZBoson = 0;              // reconstructed Z Boson
		    HepMCParticle *MCprt  = 0;              // true Z Boson if ZBoson is matched


		    if( fEvent->GetN_Z0Decays() != 0 ){
			ZBoson = (HepZ0Decay*)fEvent->GetZ0Decays()->At(fEvent->GetN_Z0Decays()-1); // Get latest reconstructed


			// Z Boson
			ZBoson->SetChi2NDoF(fitter.getS(),fitter.getNDF());
			
			MCprt = fEvent->FindMatchedMCParticle((HepParticle*)ZBoson,false,0.3,0.8);  // Z Boson truth matched?
			if ( (MCprt != 0) && (MCprt->IsZ0Boson()) ) {
			fTrueReco = true;
			}
			
			// Use Mass of true Z boson (MZ_true)
			// MZ_true already set in FillTruthHistograms
			// attention: this is done for Z+jets MC samples where you always have
			// exactly one Z boson

			Double_t prob;
			Double_t loss;
			prob=TMath::Prob(fitter.getS(),fitter.getNDF());

			
			if ( l1True!=0 && l1True->IsElectron() && l2True!=0 && l2True->IsElectron() ) {
			loss=l1True->GetBremsLoss()+l2True->GetBremsLoss();
			fHist_Zee_BremslossProb->Fill( prob ,loss , GetPreTagEvtWeight() );
			}
			
			// histograms to compare prob with true Z pt, E and final state radiation
			
			TList *Z0BosonsTrue = fEvent->GetMCParticles("Z0");
			HepMCParticle *Z0True = 0;
			TIter nextZ0(Z0BosonsTrue);
			
			while ( (Z0True = (HepMCParticle*)nextZ0()) ) {
			    if ( Z0True->IsGoodMother() ) {
				fHist_Zee_ZPtProb->Fill(Z0True->Pt(),prob,GetPreTagEvtWeight());
				fHist_Zee_ZEProb->Fill(Z0True->E(),prob,GetPreTagEvtWeight());
				fHist_Zee_ZMProb->Fill(TMath::Abs((Z0True->M("REC")-91.2)/91.2),prob,GetPreTagEvtWeight());
				if ( Z0True->GetN_Daughters()>2 ) {
				    fHist_Zee_RadProb->Fill(prob,GetPreTagEvtWeight());
				}
			    }
			}
			
		    }
		    AtlKinFitterTool::FillHistograms("Signal");
		} // is mc
		
		SignalLeptons->Remove(l1);
		
	    } // end get status
	    } // loop l2
	} // loop l1
	

    } // signal leptons
}


//____________________________________________________________________

void AtlZ0Finder::FillTruthHistograms() {
    
    // build histograms for all true Z Bosons (#Events vs. M_Z)
    
    TList *Z0BosonsTrue = fEvent->GetMCParticles("Z0");
    HepMCParticle *Z0True = 0;
    TIter nextZ0(Z0BosonsTrue);
    
    while ( (Z0True = (HepMCParticle*)nextZ0()) ) {

	if ( Z0True->IsGoodMother() ) {
	    
	    if (fMode==kKinFit) {
		SetX_TruthHistograms(Z0True->Mass("REC"));
		AtlKinFitterTool::FillHistograms("Truth");
	    
	    }
	
    }
    }
    
    // Clean-up
    delete Z0BosonsTrue;
    
    }

//____________________________________________________________________

void AtlZ0Finder::FillHistograms() {
    //
    // Fill histograms
    //
    // This method will be called only for events surviving the
    // ProcessCut() routine
    //
    SetCutFlow("# of Events");
    if ( fEvent->GetN_Z0Decays() < 1 ) return;
    
    HepZ0Decay *Z0 = 0;
    Float_t M   = 0.;
    Float_t Pt  = 0.;
    Float_t Phi = 0.;
    Float_t Eta = 0.;
    Int_t n_ee = 0;
    Int_t n_mumu = 0;
    Int_t n_tautau = 0;

    // electron variables
    HepParticle *DaughterPlus =0;
    HepParticle *DaughterMinus=0;
    HepParticle *DaughterDummy=0;
    Float_t DaughterPlus_Pt   =0.;
    Float_t DaughterMinus_Pt  =0.;
    Float_t DaughterPlus_Eta  =0.;
    Float_t DaughterMinus_Eta =0.;
    Float_t DaughterPlus_Phi  =0.;
    Float_t DaughterMinus_Phi =0.;    
    
    // TODO: jets

    // Signal (opposite charged)
    for ( Int_t i = 0; i < fEvent->GetN_Z0Decays(); i++ ) {
	Z0 = (HepZ0Decay*)fEvent->GetZ0Decays()->At(i);
       	M   = Z0->M("Reco");
	Pt  = Z0->Pt();
	Phi = Z0->Phi();
	Eta = Z0->Eta();
		
	if ( ((HepParticle*)Z0->GetDaughter1())->IsElectron() ) {
	    SetCutFlow("Z->ee");
	    if (Z0->GetProb()>0.2){
		SetCutFlow("Cut of Probability");
		fHistZreco_m_ee_with_prob ->Fill(M,GetPreTagEvtWeight());
	    }
	    // =======
	    // Z -> ee
	    // =======
	    fHistZreco_m_ee     ->Fill(M,    GetPreTagEvtWeight());
	    fHistZreco_pt_ee    ->Fill(Pt ,  GetPreTagEvtWeight());  
	    fHistZreco_phi_ee   ->Fill(Phi,  GetPreTagEvtWeight()); 
	    fHistZreco_eta_ee   ->Fill(Eta,  GetPreTagEvtWeight());
	    fHistZreco_prob_ee  ->Fill(Z0->GetProb(),GetPreTagEvtWeight());
	    n_ee++;
	    
	    // e+/e- daughters
	    DaughterDummy =(HepParticle*)Z0->GetDaughter1();
	    if(DaughterDummy->Charge() < 0){
		DaughterMinus = DaughterDummy;
	        DaughterPlus  =(HepParticle*)Z0->GetDaughter2();
	    }else{
		DaughterPlus  = DaughterDummy;
	        DaughterMinus =(HepParticle*)Z0->GetDaughter2();
	    }
	    DaughterPlus_Pt  = DaughterPlus ->Pt();  DaughterMinus_Pt  = DaughterMinus->Pt();
	    DaughterPlus_Eta = DaughterPlus ->Eta(); DaughterMinus_Eta = DaughterMinus ->Eta();
	    DaughterPlus_Phi = DaughterPlus ->Phi(); DaughterMinus_Phi = DaughterMinus ->Phi();
	    fHistZreco_eplus_pt  ->Fill(DaughterPlus_Pt,  GetPreTagEvtWeight());
	    fHistZreco_eplus_eta ->Fill(DaughterPlus_Eta, GetPreTagEvtWeight());
	    fHistZreco_eplus_phi ->Fill(DaughterPlus_Phi, GetPreTagEvtWeight());
	    fHistZreco_eminus_pt ->Fill(DaughterMinus_Pt, GetPreTagEvtWeight());
	    fHistZreco_eminus_eta->Fill(DaughterMinus_Eta,GetPreTagEvtWeight());
	    fHistZreco_eminus_phi->Fill(DaughterMinus_Phi,GetPreTagEvtWeight());

	    fHistZreco_DeltaPhi_ee->Fill(TMath::Abs(DaughterPlus_Phi-DaughterMinus_Phi),GetPreTagEvtWeight());
	    fHistZreco_DeltaPhi_ll->Fill(TMath::Abs(DaughterPlus_Phi-DaughterMinus_Phi),GetPreTagEvtWeight());
	    
	} else if ( ((HepParticle*)Z0->GetDaughter1())->IsMuon() ) {
	    SetCutFlow("Z->MuMu");
	    if (Z0->GetProb()>0.2){
		SetCutFlow("Cut of Probability");
		fHistZreco_m_mumu_with_prob->Fill(M, GetPreTagEvtWeight());
	    }


	    // =========
	    // Z -> mumu
	    // =========
	    fHistZreco_m_mumu     ->Fill(M,    GetPreTagEvtWeight());
	    fHistZreco_pt_mumu    ->Fill(Pt ,  GetPreTagEvtWeight());  
	    fHistZreco_phi_mumu   ->Fill(Phi,  GetPreTagEvtWeight()); 
	    fHistZreco_eta_mumu   ->Fill(Eta,  GetPreTagEvtWeight());
	    fHistZreco_prob_mumu  ->Fill(Z0->GetProb(),GetPreTagEvtWeight());
	    n_mumu++;

	     // mu+/mu- daughters
	    DaughterDummy =(HepParticle*)Z0->GetDaughter1();
	    if(DaughterDummy->Charge() < 0){
		DaughterMinus = DaughterDummy;
	        DaughterPlus  =(HepParticle*)Z0->GetDaughter2();
	    }else{
		DaughterPlus  = DaughterDummy;
	        DaughterMinus =(HepParticle*)Z0->GetDaughter2();
	    }
	    DaughterPlus_Pt  = DaughterPlus ->Pt();  DaughterMinus_Pt  = DaughterMinus->Pt();
	    DaughterPlus_Eta = DaughterPlus ->Eta(); DaughterMinus_Eta = DaughterMinus ->Eta();
	    DaughterPlus_Phi = DaughterPlus ->Phi(); DaughterMinus_Phi = DaughterMinus ->Phi();
	    fHistZreco_muplus_pt  ->Fill(DaughterPlus_Pt,  GetPreTagEvtWeight());
	    fHistZreco_muplus_eta ->Fill(DaughterPlus_Eta, GetPreTagEvtWeight());
	    fHistZreco_muplus_phi ->Fill(DaughterPlus_Phi, GetPreTagEvtWeight());
	    fHistZreco_muminus_pt ->Fill(DaughterMinus_Pt, GetPreTagEvtWeight());
	    fHistZreco_muminus_eta->Fill(DaughterMinus_Eta,GetPreTagEvtWeight());
	    fHistZreco_muminus_phi->Fill(DaughterMinus_Phi,GetPreTagEvtWeight());

	    fHistZreco_DeltaPhi_mumu->Fill(TMath::Abs(DaughterPlus_Phi-DaughterMinus_Phi),GetPreTagEvtWeight());
	    fHistZreco_DeltaPhi_ll->Fill(TMath::Abs(DaughterPlus_Phi-DaughterMinus_Phi),GetPreTagEvtWeight());
	   
	} // else if ( ((HepParticle*)Z0->GetDaughter1())->IsTau() ) {
// 	    SetCutFlow("Z->TauTau");
// 	    ===========
// 	    Z -> tautau
// 	    ===========
// 	    fHistZreco_m_tautau     ->Fill(M,    GetPreTagEvtWeight());
// 	    fHistZreco_pt_tautau    ->Fill(Pt ,  GetPreTagEvtWeight());  
// 	    fHistZreco_phi_tautau   ->Fill(Phi,  GetPreTagEvtWeight()); 
// 	    fHistZreco_eta_tautau   ->Fill(Eta,  GetPreTagEvtWeight());
// 	    n_tautau++;

// 	    tau+/tau- daughters
// 	    DaughterDummy =(HepParticle*)Z0->GetDaughter1();
// 	    if(DaughterDummy->Charge() < 0){
// 		DaughterMinus = DaughterDummy;
// 	        DaughterPlus  =(HepParticle*)Z0->GetDaughter2();
// 	    }else{
// 		DaughterPlus  = DaughterDummy;
// 	        DaughterMinus =(HepParticle*)Z0->GetDaughter2();
// 	    }
// 	    DaughterPlus_Phi = DaughterPlus ->Phi(); DaughterMinus_Phi = DaughterMinus ->Phi();
// 	    fHistZreco_DeltaPhi_tautau->Fill(TMath::Abs(DaughterPlus_Phi-DaughterMinus_Phi),GetPreTagEvtWeight());
// 	    fHistZreco_DeltaPhi_ll->Fill(TMath::Abs(DaughterPlus_Phi-DaughterMinus_Phi),GetPreTagEvtWeight());
	    
// 	}
	
	// =======
	// Z -> ll
	// =======
        fHistZreco_m_ll   ->Fill(M,    GetPreTagEvtWeight());
        fHistZreco_pt_ll  ->Fill(Pt ,  GetPreTagEvtWeight());  
        fHistZreco_phi_ll ->Fill(Phi,  GetPreTagEvtWeight()); 
        fHistZreco_eta_ll ->Fill(Eta,  GetPreTagEvtWeight()); 
    }
    for (Int_t i=0;i<TruthMatchCounter;i++){
	if (Z0->GetProb()>0.2){
		SetCutFlow("# of Z matched true");
	    }
    }
	
    // Z0 numbers / evt
    fHistZreco_n_ee     ->Fill(n_ee, GetPreTagEvtWeight());
    fHistZreco_n_mumu   ->Fill(n_mumu, GetPreTagEvtWeight());
    fHistZreco_n_tautau ->Fill(n_tautau, GetPreTagEvtWeight());
    fHistZreco_n_ll->Fill(n_ee+n_mumu+n_tautau, GetPreTagEvtWeight());    

    // Combinatorial bkg. (same charged)
    TIter next(fBkgZ0Decays);

    while ( (Z0 = (HepZ0Decay*)next()) ) {    
      	
      M   = Z0->M("Reco");
      Pt  = Z0->Pt();
      Phi = Z0->Phi();
      Eta = Z0->Eta();
      
	if ( ((HepParticle*)Z0->GetDaughter1())->IsElectron() ) {
	    // =======
	    // Z -> ee
	    // =======
	    fHistZreco_m_ee_bkg  ->Fill(M,   GetPreTagEvtWeight());
	    fHistZreco_pt_ee_bkg ->Fill(Pt,  GetPreTagEvtWeight());  
	    fHistZreco_phi_ee_bkg->Fill(Phi, GetPreTagEvtWeight()); 
	    fHistZreco_eta_ee_bkg->Fill(Eta, GetPreTagEvtWeight());

	    // e+/e- daughters
	    DaughterDummy =(HepParticle*)Z0->GetDaughter1();
	    if(DaughterDummy->Charge() < 0){
		DaughterMinus = DaughterDummy;
	        DaughterPlus  =(HepParticle*)Z0->GetDaughter2();
	    }else{
		DaughterPlus  = DaughterDummy;
	        DaughterMinus =(HepParticle*)Z0->GetDaughter2();
	    }
	    DaughterPlus_Phi = DaughterPlus ->Phi(); DaughterMinus_Phi = DaughterMinus ->Phi();
	    fHistZreco_DeltaPhi_ee_bkg->Fill(TMath::Abs(DaughterPlus_Phi-DaughterMinus_Phi),GetPreTagEvtWeight());
	    fHistZreco_DeltaPhi_ll_bkg->Fill(TMath::Abs(DaughterPlus_Phi-DaughterMinus_Phi),GetPreTagEvtWeight());
	    
	    
	} else if ( ((HepParticle*)Z0->GetDaughter1())->IsMuon() ) {
	    // =========
	    // Z -> mumu
	    // =========
	    fHistZreco_m_mumu_bkg  ->Fill(M,   GetPreTagEvtWeight());
	    fHistZreco_pt_mumu_bkg ->Fill(Pt , GetPreTagEvtWeight());  
	    fHistZreco_phi_mumu_bkg->Fill(Phi, GetPreTagEvtWeight()); 
	    fHistZreco_eta_mumu_bkg->Fill(Eta, GetPreTagEvtWeight());

	    // mu+/mu- daughters
	    DaughterDummy =(HepParticle*)Z0->GetDaughter1();
	    if(DaughterDummy->Charge() < 0){
		DaughterMinus = DaughterDummy;
	        DaughterPlus  =(HepParticle*)Z0->GetDaughter2();
	    }else{
		DaughterPlus  = DaughterDummy;
	        DaughterMinus =(HepParticle*)Z0->GetDaughter2();
	    }
	    DaughterPlus_Phi = DaughterPlus ->Phi(); DaughterMinus_Phi = DaughterMinus ->Phi();
	    fHistZreco_DeltaPhi_mumu_bkg->Fill(TMath::Abs(DaughterPlus_Phi-DaughterMinus_Phi),GetPreTagEvtWeight());
	    fHistZreco_DeltaPhi_ll_bkg->Fill(TMath::Abs(DaughterPlus_Phi-DaughterMinus_Phi),GetPreTagEvtWeight());
	    
	} else if ( ((HepParticle*)Z0->GetDaughter1())->IsTau() ) {
	    // ===========
	    // Z -> tautau
	    // ===========
	    fHistZreco_m_tautau_bkg  ->Fill(M,   GetPreTagEvtWeight());
	    fHistZreco_pt_tautau_bkg ->Fill(Pt , GetPreTagEvtWeight());  
	    fHistZreco_phi_tautau_bkg->Fill(Phi, GetPreTagEvtWeight()); 
	    fHistZreco_eta_tautau_bkg->Fill(Eta, GetPreTagEvtWeight());

	    //tau+/tau- daughters
	    DaughterDummy =(HepParticle*)Z0->GetDaughter1();
	    if(DaughterDummy->Charge() < 0){
		DaughterMinus = DaughterDummy;
	        DaughterPlus  =(HepParticle*)Z0->GetDaughter2();
	    }else{
		DaughterPlus  = DaughterDummy;
	        DaughterMinus =(HepParticle*)Z0->GetDaughter2();
	    }
	    DaughterPlus_Phi = DaughterPlus ->Phi(); DaughterMinus_Phi = DaughterMinus ->Phi();
	    fHistZreco_DeltaPhi_tautau_bkg->Fill(TMath::Abs(DaughterPlus_Phi-DaughterMinus_Phi),GetPreTagEvtWeight());
	    fHistZreco_DeltaPhi_ll_bkg->Fill(TMath::Abs(DaughterPlus_Phi-DaughterMinus_Phi),GetPreTagEvtWeight());
	    
	}
	// =======
	// Z -> ll
	// =======
        fHistZreco_m_ll_bkg  ->Fill(M,   GetPreTagEvtWeight());
        fHistZreco_pt_ll_bkg ->Fill(Pt , GetPreTagEvtWeight());
        fHistZreco_phi_ll_bkg->Fill(Phi, GetPreTagEvtWeight()); 
        fHistZreco_eta_ll_bkg->Fill(Eta, GetPreTagEvtWeight()); 
    } 
}

//____________________________________________________________________

void AtlZ0Finder::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    
    // =======
    // Leptons
    // =======
    
    // Electrons
    fSignalElectronAuthor        = AtlEMShower::kHighPtElectron;
    fSignalElectronIsEM          = AtlEMShower::kElectronTight;
    fSignalElectron_Pt_min       = 10.0;
    fSignalElectron_Pt_max       = 10.e10;
    fSignalElectron_Eta_min      = -1.9;
    fSignalElectron_Eta_max      = 1.9;
    fSignalElectron_EtCone20_max = 100.;
    
    // Muons
    fSignalMuonAuthor        = AtlMuon::kSTACO;
    fSignalMuon_Pt_min       = 10.0;
    fSignalMuon_Pt_max       = 10.e10;
    fSignalMuon_Eta_min      = -1.9;
    fSignalMuon_Eta_max      = 1.9;
    fSignalMuon_Chi2_max     = 4.;
    fSignalMuon_EtCone20_max = 100.;

    // Taus
    // fSignalTauAuthor        = AtlTau::kTauRec;
    // fSignalTauAuthorFlag    = AtlTau::kTauCutTight;
    fSignalTau_Pt_min       = 10.0;
    fSignalTau_Pt_max       = 10.e10;
    fSignalTau_Eta_min      = -1.9;
    fSignalTau_Eta_max      = 1.9;

    // ============
    // Z mass range
    // ============

    // Z Boson
    fZBoson_M_min       = 65.0;
    fZBoson_M_max       = 115.0;
    
}

//____________________________________________________________________

void AtlZ0Finder::Terminate() {

    // small resolution study - electron covariance matrix
    // all matrix elements represent relative quantities
    // e.g. mean value of [(pt_true-pt_reco)/pt_true]^2
   
	c11=c11/TruthMatchCounter;
	c22=c22/TruthMatchCounter;
	c33=c33/TruthMatchCounter;
	c12=c12/TruthMatchCounter;
	c13=c13/TruthMatchCounter;
	c23=c23/TruthMatchCounter;

	//cout << "\n\nCovariance Matrix of Electrons" << endl;
	//cout << "--------------------------------" << endl;
	//cout << c11 << " " << c12 << " " << c13 << endl;
	//cout << c12 << " " << c22 << " " << c23 << endl;
	//cout << c13 << " " << c23 << " " << c33 << endl;
	//cout << endl << endl;

	//cout << "TruthMatchCounter: " << TruthMatchCounter << endl;
	
    if ( fMode==kKinFit ) {
    
	// gDirectory->cd("Z0Reco/Fitter/TruthMatching");
    AtlKinFitterTool::Terminate();
    gDirectory->cd("../../");

    }
    
}

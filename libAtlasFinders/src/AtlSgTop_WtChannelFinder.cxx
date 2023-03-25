//____________________________________________________________________
//
// Single top Wt-channel event finder
//
// Helper class for conducting a kinematic fit analysis of the Wt-channel
// of single-top.
//
// Used by AtlSelector.
//
//
//
// Use this tool by adding it to the task class of your analysis job
// like this:
//
//  //************************************************************
//
//  class YourTask : public AtlAppAnalysisTask {
//  .
//  .
//  .
//  }
//
//  void YourTask::InitSelector() {
//  //
//  // Initialise the analysis selector of your analysis
//  //
//  fSelector = new YourAnalysis(fOutputFileName->Data());
//
//  // ===============================================================================    
//  // Definition of selection cuts for your (dummy or alternative cut-based) analysis
//  // ===============================================================================
//  SetCut(...)
//  .
//  .
//  .
//  //
//  // Add kinematic fitting procedure
//  //
//  AtlSgTop_WtChannelFinder *kinfit =
//  new AtlSgTop_WtChannelFinder("sgtop_finder_kinfitter",
// 	    "Single-Top t-Channel Reconstruction by KinFitter");
//  SetToolCut("sgtop_finder_kinfitter", "fMode", "AtlSgTop_WtChannelFinder::kKinFit");
//  SetToolCut("sgtop_finder_kinfitter", "fModeMass", "AtlSgTop_WtChannelFinder::kGauss");
//  SetToolCut("sgtop_finder_kinfitter", "fBJetTagger",    "AtlBTag::kBaseline");
//  SetToolCut("sgtop_finder_kinfitter", "fBJetWeightMin", "4.0");
//  .
//  .
//  .
//  .
//  fSelector->AddTool(kinfit);
//
//  
//
// When included in the analysis task this class appends an additional
// directory structure to the output file in which the KinFitter
// histograms are stored.
//
// Author: Patrick Rieck <mailto:rieck@physik.hu-berlin.de>
// Copyright: 2009 (C) Patrick Rieck
//
#ifndef ATLAS_AtlSgTop_WtChannelFinder
#include <AtlSgTop_WtChannelFinder.h>
#endif
#include <TH1F.h>
#include <AtlEvent.h>
#include <TTree.h>
#include <TBranch.h>
#include <TString.h>
#include <iostream>
#include <TROOT.h>
#include <TFile.h>
#include <TMath.h>
#include <vector>

using namespace std;

#ifndef __CINT__
ClassImp(AtlSgTop_WtChannelFinder);
#endif
const Int_t AtlSgTop_WtChannelFinder::fgNSystNames = 13;//9;
const char* AtlSgTop_WtChannelFinder::fgSystNames[] = {
    "nominal",
    "el_sf_up",
    "el_sf_down",
    "mu_sf_up",
    "mu_sf_down",
    "jet_btag_sf_up",
    "jet_btag_sf_down",
    "jet_ctag_sf_up",
    "jet_ctag_sf_down",
    "jet_mistag_sf_up",
    "jet_mistag_sf_down",
    "jvf_weight_up",
    "jvf_weight_down"
};


//____________________________________________________________________

AtlSgTop_WtChannelFinder::AtlSgTop_WtChannelFinder(const char* name, const char* title) :
    AtlKinFitterTool(name, title) {
    //
    // Default constructor
    //
    fKinFitter_HadronicTop = new TKinFitter;

    fLeptons      = 0;
    fJets         = 0;
    fBJets        = new TList;
    fWhadJets     = new TList;
    fLepton       = 0;
    fNeutrino     = 0;
    fBJet         = 0;
    fBJet_had     = 0;
    fJet1         = 0;
    fJet2         = 0;
    fJet1_had     = 0;
    fJet2_had     = 0;
    fBestLeptonicBJet = 0;
    fBestLeptonicJet1 = 0;
    fBestLeptonicJet2 = 0;
    fJet1LeptonicKinFit = 0;
    fJet2LeptonicKinFit = 0;    
    
    fBestHadronicBJet = 0;
    fBestHadronicJet1 = 0;
    fBestHadronicJet2 = 0;
    fBJetHadronicKinFit = 0;
    fJet1HadronicKinFit = 0;
    fJet2HadronicKinFit = 0;    
    fCovLepton.ResizeTo(3,3);
    fCovNeutrino.ResizeTo(3,3);
    fCovBJet.ResizeTo(3,3);
    fCovWJet1.ResizeTo(3,3);
    fCovWJet2.ResizeTo(3,3);
    fCovLepton.Zero();
    fCovNeutrino.Zero();
    fCovBJet.Zero();
    fCovWJet1.Zero();
    fCovWJet2.Zero();
    
    fCovLepton_had.ResizeTo(3,3);
    fCovBJet_had.ResizeTo(3,3);
    fCovWJet1_had.ResizeTo(3,3);
    fCovWJet2_had.ResizeTo(3,3);
    fCovLepton_had.Zero();
    fCovBJet_had.Zero();
    fCovWJet1_had.Zero();
    fCovWJet2_had.Zero();

    fCutflow_tool    = 0;
    fCutflow_tool_AC = 0;
    fRecoTriangularW = 0;
    SetCutDefaults();    
}

//____________________________________________________________________

AtlSgTop_WtChannelFinder::~AtlSgTop_WtChannelFinder() {
  //
  // Default destructor
  //
  if ( fWhadJets   != 0 ) delete fWhadJets;
  if ( fBJets      != 0 ) delete fBJets;
  if ( fNeutrino   != 0 ) delete fNeutrino;
  fLepton     = 0;
  fNeutrino   = 0;
  fBJet       = 0;
  fJet1       = 0;
  fJet2       = 0;
  fBJet_had   = 0;
  fJet1_had   = 0;
  fJet2_had   = 0;
  fBestLeptonicBJet = 0;
  fBestLeptonicJet1 = 0;
  fBestLeptonicJet2 = 0;
  fBestHadronicBJet = 0;
  fBestHadronicJet1 = 0;
  fBestHadronicJet2 = 0;

  delete fKinFitter_HadronicTop;
}

//____________________________________________________________________

void AtlSgTop_WtChannelFinder::SetBranchStatus() {
    //
    // Switch on needed branches
    //
    fTree->SetBranchStatus("fEventHeader*",         kTRUE);
    fTree->SetBranchStatus("fN_Electrons*",         kTRUE);
    fTree->SetBranchStatus("fElectrons*",           kTRUE);
    fTree->SetBranchStatus("fN_Muons*",             kTRUE);
    fTree->SetBranchStatus("fMuons*",               kTRUE);
    fTree->SetBranchStatus("fEnergySum*",           kTRUE);
    fTree->SetBranchStatus("fWDecaysLNu*",          kTRUE);
    fTree->SetBranchStatus("fN_WDecaysLNu*",        kTRUE);
    fTree->SetBranchStatus("fWDecaysJJ*",           kTRUE);
    fTree->SetBranchStatus("fN_WDecaysJJ*",         kTRUE);
    fTree->SetBranchStatus("fTopDecays*",           kTRUE);
    fTree->SetBranchStatus("fN_TopDecays*",         kTRUE);
    fTree->SetBranchStatus("fIDTracks*",            kTRUE);
    fTree->SetBranchStatus("fN_IDTracks*",          kTRUE);
    
}


//____________________________________________________________________

void AtlSgTop_WtChannelFinder::BookHistograms() {
    //
    // Book reconstruction histograms
    //
    // Initialize text file for acceptance challenge output
    gDirectory->cd("$LIBSINGLETOP/tasks");
    //    fOut("logfile_AcceptanceChallenge");
    fOut.open("logfile_AcceptanceChallenge_C10");
    AtlKinFitterTool::BookHistograms();
    gDirectory->cd();
    gDirectory->mkdir("WtFinderPerformance");
    gDirectory->cd("WtFinderPerformance");

    fHistPerformanceComparison_before = new TH1F("h_PerformanceComparison_beforeCut",
						 "Lept vs Hadr converged fits - before cut",
						 2,0.,2.);
    fHistPerformanceComparison_before->SetXTitle("Fit Type");
    fHistPerformanceComparison_before->SetYTitle("Number of Entries");
    TAxis *axis = fHistPerformanceComparison_before->GetXaxis();
    axis->SetBinLabel(1,"Leptonic Top");
    axis->SetBinLabel(2,"Hadronic Top");
    
    fHistPerformanceComparison_after = new TH1F("h_PerformanceComparison_afterCut",
						"Lept vs Hadr converged fits - after cut",
						2,0.,2.);
    fHistPerformanceComparison_after->SetXTitle("Fit Type");
    fHistPerformanceComparison_after->SetYTitle("Number of Entries");
    axis = fHistPerformanceComparison_after->GetXaxis();
    axis->SetBinLabel(1,"Leptonic Top");
    axis->SetBinLabel(2,"Hadronic Top");
    
    fHist_Chi2_LeptHadrComp = new TH2F("h_Chi2_LeptHadrComp",
					 "Lept vs Hadr p-Value comparison",
					 50, 0, 100, 50, 0, 100);
    fHist_Chi2_LeptHadrComp->SetXTitle("#Chi^{2}(Wt_{Lept})");
    fHist_Chi2_LeptHadrComp->SetYTitle("#Chi^{2}(Wt_{Hadr})");
    fHist_Chi2_LeptHadrComp->SetZTitle("Number of Entries");

    fHist_PValue_LeptHadrComp = new TH2F("h_PValue_LeptHadrComp",
					 "Lept vs Hadr p-Value comparison",
					 50, 0, 1, 50, 0, 1);
    fHist_PValue_LeptHadrComp->SetXTitle("P(Wt_{Lept})");
    fHist_PValue_LeptHadrComp->SetYTitle("P(Wt_{Hadr})");
    fHist_PValue_LeptHadrComp->SetZTitle("Number of Entries");

    
    gDirectory->mkdir("Wt-LeptonicTop");
    gDirectory->cd("Wt-LeptonicTop");

    // KinFitter status counts for fits
    fHist_KinFit_KinFitterStatus_LeptonicTop = new TH1F("h_KinFitterStatus_LeptonicTop",
					    "Status of KinFitter",
					    2,0,2);
    fHist_KinFit_KinFitterStatus_LeptonicTop->SetXTitle("Status of Fit");
    fHist_KinFit_KinFitterStatus_LeptonicTop->SetYTitle("No. of Fits");
    axis = fHist_KinFit_KinFitterStatus_LeptonicTop->GetXaxis();
    for ( Int_t k = 0; k < 2; k++ ) {
	axis->SetBinLabel(k+1, AtlKinFitterTool::fgStatusNames[k]);
    }
    
    fHist_KinFit_NbIterLeptonicTop = new TH1F("h_NbIter_LeptonicTop",
					      "No. of Iterations for Leptonic Top fitting procedure",
					      1200,0,1200);
    fHist_KinFit_NbIterLeptonicTop->SetXTitle("No. of Iterations");
    fHist_KinFit_NbIterLeptonicTop->SetYTitle("No. of fitting procedures");
    
    // Number of fits that DID converge (after pre-selection)
    fHist_KinFit_NbFitsConverged_LeptonicTop = new TH1F("h_NbFitsConv_LeptonicTop",
					    "No. of Fitting Procedures converged per event (after pre-selection)",
					    4,-0.5,3.5);

    
    // Number of fits that DID NOT converge (after pre-selection)
    fHist_KinFit_NbFitsNotConverged_LeptonicTop = new TH1F("h_NbFitsNotConv_LeptonicTop",
			       "No. of Fitting Procedures not converged per event (after pre-selection)",
			       20,0,20);
    fHist_KinFit_NbFitsNotConverged_LeptonicTop->SetXTitle("No. of Fits_{not conv.} / Evt");
    fHist_KinFit_NbFitsNotConverged_LeptonicTop->SetYTitle("No. of Events");

    // Number of fits that failed numerically (after pre-selection)
    fHist_KinFit_NbFitsFailedNumerically_LeptonicTop = new TH1F("h_NbFitsFailNumeric_LeptonicTop",
			       "No. of Fitting Procedures failing numerically per event (after pre-selection)",
			       20,0,20);
    fHist_KinFit_NbFitsFailedNumerically_LeptonicTop->SetXTitle("No. of Fits_{numeric fail} / Evt");
    fHist_KinFit_NbFitsFailedNumerically_LeptonicTop->SetYTitle("No. of Events");

    gDirectory->cd("..");
    
    gDirectory->mkdir("Wt-HadronicTop");
    gDirectory->cd("Wt-HadronicTop");

    // KinFitter status counts for fits
    fHist_KinFit_KinFitterStatus_HadronicTop = new TH1F("h_KinFitterStatus_HadronicTop",
					    "Status of KinFitter",
					    2,0,2);
    fHist_KinFit_KinFitterStatus_HadronicTop->SetXTitle("Status of Fit");
    fHist_KinFit_KinFitterStatus_HadronicTop->SetYTitle("No. of Fits");
    axis = fHist_KinFit_KinFitterStatus_HadronicTop->GetXaxis();
    for ( Int_t k = 0; k < 2; k++ ) {
	axis->SetBinLabel(k+1, AtlKinFitterTool::fgStatusNames[k]);
    }
    
    fHist_KinFit_NbIterHadronicTop = new TH1F("h_NbIter_HadronicTop",
					      "No. of Iterations for Hadronic Top fitting procedure",
					      1200,0,1200);
    fHist_KinFit_NbIterHadronicTop->SetXTitle("No. of Iterations");
    fHist_KinFit_NbIterHadronicTop->SetYTitle("No. of fitting procedures");
    
    // Number of fits that DID converge (after pre-selection)
    fHist_KinFit_NbFitsConverged_HadronicTop = new TH1F("h_NbFitsConv_HadronicTop",
					    "No. of Fitting Procedures converged per event (after pre-selection)",
					    4,-0.5,3.5);

    
    // Number of fits that DID NOT converge (after pre-selection)
    fHist_KinFit_NbFitsNotConverged_HadronicTop = new TH1F("h_NbFitsNotConv_HadronicTop",
			       "No. of Fitting Procedures not converged per event (after pre-selection)",
			       20,0,20);
    fHist_KinFit_NbFitsNotConverged_HadronicTop->SetXTitle("No. of Fits_{not conv.} / Evt");
    fHist_KinFit_NbFitsNotConverged_HadronicTop->SetYTitle("No. of Events");

    // Number of fits that failed numerically (after pre-selection)
    fHist_KinFit_NbFitsFailedNumerically_HadronicTop = new TH1F("h_NbFitsFailNumeric_HadronicTop",
			       "No. of Fitting Procedures failing numerically per event (after pre-selection)",
			       20,0,20);
    fHist_KinFit_NbFitsFailedNumerically_HadronicTop->SetXTitle("No. of Fits_{numeric fail} / Evt");
    fHist_KinFit_NbFitsFailedNumerically_HadronicTop->SetYTitle("No. of Events");

     
    // Mass-chi2 comparison histograms
    // ------------------------------>before the p-Value cut
    fHist_W_hadr_top_reco_comp_Mptbjet = new TH2F("h_WMass_vs_ptbjet_beforecut",
						  "",
						  300, 0, 300, 400, -100, 300);
    fHist_W_hadr_top_reco_comp_Mptbjet->SetXTitle("p_{t, bjet}");
    fHist_W_hadr_top_reco_comp_Mptbjet->SetYTitle("W mass");
    fHist_W_hadr_top_reco_comp_Mptbjet->SetZTitle("Number of Entries");
    fHist_W_hadr_top_reco_comp_Metabjet = new TH2F("h_WMass_vs_etabjet_beforecut",
 						   "",
						   50, -2.5, 2.5, 400, -100, 300);
    fHist_W_hadr_top_reco_comp_Metabjet->SetXTitle("#eta_{bjet}");
    fHist_W_hadr_top_reco_comp_Metabjet->SetYTitle("W mass");
    fHist_W_hadr_top_reco_comp_Metabjet->SetZTitle("Number of Entries");

    
    fHist_W_hadr_top_reco_comp_Mptjet1 = new TH2F("h_WMass_vs_ptjet1_beforecut",
						  "",
						  300, 0, 300, 400, -100, 300);
    fHist_W_hadr_top_reco_comp_Mptjet1->SetXTitle("p_{t, jet1}");
    fHist_W_hadr_top_reco_comp_Mptjet1->SetYTitle("W mass");
    fHist_W_hadr_top_reco_comp_Mptjet1->SetZTitle("Number of Entries");

    fHist_W_hadr_top_reco_comp_Mptjet2 = new TH2F("h_WMass_vs_ptjet2_beforecut",
						  "",
						  300, 0, 300,400, -100, 300);
    fHist_W_hadr_top_reco_comp_Mptjet2->SetXTitle("p_{t, jet2}");
    fHist_W_hadr_top_reco_comp_Mptjet2->SetYTitle("W mass");
    fHist_W_hadr_top_reco_comp_Mptjet2->SetZTitle("Number of Entries");
    fHist_W_hadr_top_reco_comp_Metajet1 = new TH2F("h_WMass_vs_etajet1_beforecut",
 						   "",
						   50, -2.5, 2.5, 400, -100, 300);
    fHist_W_hadr_top_reco_comp_Metajet1->SetXTitle("#eta_{jet1}");
    fHist_W_hadr_top_reco_comp_Metajet1->SetYTitle("W mass");
    fHist_W_hadr_top_reco_comp_Metajet1->SetZTitle("Number of Entries");
    
    fHist_W_hadr_top_reco_comp_Metajet2 = new TH2F("h_WMass_vs_etajet2_beforecut",
						   "",
						   50, -2.5, 2.5, 400, -100, 300 );
    fHist_W_hadr_top_reco_comp_Metajet2->SetXTitle("#eta{jet2}");
    fHist_W_hadr_top_reco_comp_Metajet2->SetYTitle("W mass");
    fHist_W_hadr_top_reco_comp_Metajet2->SetZTitle("Number of Entries");

    fHist_W_hadr_top_reco_comp_chi2ptbjet = new TH2F("h_chi2_vs_ptbjet_beforecut",
						     "",
						     300, 0, 300,400, -100, 300 );
    fHist_W_hadr_top_reco_comp_chi2ptbjet->SetXTitle("p_{t, bjet}");
    fHist_W_hadr_top_reco_comp_chi2ptbjet->SetYTitle("#chi^{2}_{Wt-hadr}");
    fHist_W_hadr_top_reco_comp_chi2ptbjet->SetZTitle("Number of Entries");

    fHist_W_hadr_top_reco_comp_chi2etabjet = new TH2F("h_chi2_vs_etabjet_beforecut",
						      "",
						      50, -2.5, 2.5,400, -100, 300 );
    fHist_W_hadr_top_reco_comp_chi2etabjet->SetXTitle("#eta_{bjet}");
    fHist_W_hadr_top_reco_comp_chi2etabjet->SetYTitle("#chi^{2}_{Wt-hadr}");
    fHist_W_hadr_top_reco_comp_chi2etabjet->SetZTitle("Number of Entries");    

    fHist_W_hadr_top_reco_comp_chi2ptjet1 = new TH2F("h_chi2_vs_ptjet1_beforecut",
						     "",
						     300, 0, 300,400, -100, 300 );
    fHist_W_hadr_top_reco_comp_chi2ptjet1->SetXTitle("p_{t, jet1}");
    fHist_W_hadr_top_reco_comp_chi2ptjet1->SetYTitle("#chi^{2}_{Wt-hadr}");
    fHist_W_hadr_top_reco_comp_chi2ptjet1->SetZTitle("Number of Entries");

    fHist_W_hadr_top_reco_comp_chi2ptjet2 = new TH2F("h_chi2_vs_ptjet2_beforecut",
						     "",
						     300, 0, 300,400, -100, 300 );
    fHist_W_hadr_top_reco_comp_chi2ptjet2->SetXTitle("p_{t, jet2}");
    fHist_W_hadr_top_reco_comp_chi2ptjet2->SetYTitle("#chi^{2}_{Wt-hadr}");
    fHist_W_hadr_top_reco_comp_chi2ptjet2->SetZTitle("Number of Entries");

    fHist_W_hadr_top_reco_comp_chi2etajet1 = new TH2F("h_chi2_vs_etajet1_beforecut",
						      "",
						      50, -2.5, 2.5,400, -100, 300 );
    fHist_W_hadr_top_reco_comp_chi2etajet1->SetXTitle("#eta_{jet1}");
    fHist_W_hadr_top_reco_comp_chi2etajet1->SetYTitle("#chi^{2}_{Wt-hadr}");
    fHist_W_hadr_top_reco_comp_chi2etajet1->SetZTitle("Number of Entries");
    
    fHist_W_hadr_top_reco_comp_chi2etajet2 = new TH2F("h_chi2_vs_etajet2_beforecut",
						      "",
						      50, -2.5, 2.5,400, -100, 300 );
    fHist_W_hadr_top_reco_comp_chi2etajet2->SetXTitle("#eta_{jet2}");
    fHist_W_hadr_top_reco_comp_chi2etajet2->SetYTitle("#chi^{2}_{Wt-hadr}");
    fHist_W_hadr_top_reco_comp_chi2etajet2->SetZTitle("Number of Entries");

    
    fHist_W_hadr_top_reco_comp_MChi2 = new TH2F("h_WMass_Chi2HadrWt_beforecut",
						"",
						400, -100, 300, 100, 0, 100 );
    fHist_W_hadr_top_reco_comp_MChi2->SetXTitle("W mass");
    fHist_W_hadr_top_reco_comp_MChi2->SetYTitle("#chi^{2}_{Wt-hadr}");
    fHist_W_hadr_top_reco_comp_MChi2->SetZTitle("Number of Entries");

    fHist_W_hadr_top_reco_comp_MPChi2 = new TH2F("h_WMass_ProbChi2HadrWt_beforecut",
						 "",
						 400, -100, 300, 100, 0, 1 );
    fHist_W_hadr_top_reco_comp_MPChi2->SetXTitle("W mass");
    fHist_W_hadr_top_reco_comp_MPChi2->SetYTitle("P(#chi^{2})_{Wt-hadr}");
    fHist_W_hadr_top_reco_comp_MPChi2->SetZTitle("Number of Entries");

    fHist_t_hadr_top_reco_comp_MChi2 = new TH2F("h_topMass_Chi2HadrWt_beforecut",
						"",
						400, -100, 300, 100, 0, 100 );
    fHist_t_hadr_top_reco_comp_MChi2->SetXTitle("top-quark mass");
    fHist_t_hadr_top_reco_comp_MChi2->SetYTitle("#chi^{2}_{Wt-hadr}");
    fHist_t_hadr_top_reco_comp_MChi2->SetZTitle("Number of Entries");

    fHist_t_hadr_top_reco_comp_MPChi2 = new TH2F("h_topMass_ProbChi2HadrWt_beforecut",
						 "",
						 400, -100, 300, 100, 0, 1 );
    fHist_t_hadr_top_reco_comp_MPChi2->SetXTitle("top-quark mass");
    fHist_t_hadr_top_reco_comp_MPChi2->SetYTitle("P(#chi^{2})_{Wt-hadr}");
    fHist_t_hadr_top_reco_comp_MPChi2->SetZTitle("Number of Entries");

    
    fHist_comp_MM = new TH2F("h_WMassTopMass_beforecut",
					     "",
					     400, -100, 300, 100, 100, 200 );
    fHist_comp_MM->SetXTitle("W mass");
    fHist_comp_MM->SetYTitle("top-quark mass");
    fHist_comp_MM->SetZTitle("Number of Entries");

        // ------------------------------>after the p-Value cut
    gDirectory->mkdir("AfterTheCut");
    gDirectory->cd("AfterTheCut");
    fHist_W_hadr_top_reco_comp_MChi2_after = new TH2F("h_WMass_Chi2HadrWt_aftercut",
						"",
						400, -100, 300, 100, 0, 100 );
    fHist_W_hadr_top_reco_comp_MChi2_after->SetXTitle("W mass");
    fHist_W_hadr_top_reco_comp_MChi2_after->SetYTitle("#chi^{2}_{Wt-hadr}");
    fHist_W_hadr_top_reco_comp_MChi2_after->SetZTitle("Number of Entries");

    fHist_W_hadr_top_reco_comp_MPChi2_after = new TH2F("h_WMass_ProbChi2HadrWt_aftercut",
						 "",
						 400, -100, 300, 100, 0, 1 );
    fHist_W_hadr_top_reco_comp_MPChi2_after->SetXTitle("W mass");
    fHist_W_hadr_top_reco_comp_MPChi2_after->SetYTitle("P(#chi^{2})_{Wt-hadr}");
    fHist_W_hadr_top_reco_comp_MPChi2_after->SetZTitle("Number of Entries");

    fHist_t_hadr_top_reco_comp_MChi2_after = new TH2F("h_topMass_Chi2HadrWt_aftercut",
						"",
						400, -100, 300, 100, 0, 100 );
    fHist_t_hadr_top_reco_comp_MChi2_after->SetXTitle("top-quark mass");
    fHist_t_hadr_top_reco_comp_MChi2_after->SetYTitle("#chi^{2}_{Wt-hadr}");
    fHist_t_hadr_top_reco_comp_MChi2_after->SetZTitle("Number of Entries");

    fHist_t_hadr_top_reco_comp_MPChi2_after = new TH2F("h_topMass_ProbChi2HadrWt_aftercut",
						 "",
						 400, -100, 300, 100, 0, 1 );
    fHist_t_hadr_top_reco_comp_MPChi2_after->SetXTitle("top-quark mass");
    fHist_t_hadr_top_reco_comp_MPChi2_after->SetYTitle("P(#chi^{2})_{Wt-hadr}");
    fHist_t_hadr_top_reco_comp_MPChi2_after->SetZTitle("Number of Entries");

    
    fHist_comp_MM_after = new TH2F("h_WMassTopMass_aftercut",
					     "",
					     400, -100, 300, 100, 100, 200 );
    fHist_comp_MM_after->SetXTitle("W mass");
    fHist_comp_MM_after->SetYTitle("top-quark mass");
    fHist_comp_MM_after->SetZTitle("Number of Entries");

    gDirectory->cd("..");
    
    
    gDirectory->cd("..");

    gDirectory->cd("..");
    
    // Book tree for combined analysis
    if (fProduceCommonTree) {
      gDirectory->cd("..");
      fCommonTree = new TTree("Mu3Jet","Mu3Jet");
      fCommonTree->Branch("RunNumber",  &fRunNumber,  "RunNumber/I");
      fCommonTree->Branch("LumiBlock",  &fLumiBlock,  "LumiBlock/I");
      fCommonTree->Branch("EventNumber",&fEventNumber,"EventNumber/I");
      fCommonTree->Branch("mc_channel_number",&fmc_channel_number,"mc_channel_number/I");
      fCommonTree->Branch("top_hfor_type",&ftop_hfor_type,"top_hfor_type/F");

      //fCommonTree->Branch("fchi2ttbarBkg",   &fchi2ttbarBkg,   "fchi2ttbarBkg/F");
      //      fCommonTree->Branch("fChi2ProbTTBarBkg",   &fChi2ProbTTBarBkg,   "fChi2ProbTTBarBkg/F");
      fCommonTree->Branch("fChi2ProbTTBarBkg",&fChi2ProbTTBarBkg,"fChi2ProbTTBarBkg/D");

      fCommonTree->Branch("fchi2WTop",    &fchi2WTop,    "fchi2WTop/D");
      fCommonTree->Branch("fchi2ProbWTop",&fchi2ProbWTop,"fchi2ProbWTop/D");
      //fCommonTree->Branch("fNDoFWTop",    &fNDoFWTop,    "fNDoFWTop/D");

      fCommonTree->Branch("fchi2RedWTop",    &fchi2RedWTop,    "fchi2RedWTop/D");

      fCommonTree->Branch("pT_System",&pT_System,"pT_System/F");
      fCommonTree->Branch("fRapTop",&fRapTop,"fRapTop/F");
      fCommonTree->Branch("fEtaTop",&fEtaTop,"fEtaTop/F");
      fCommonTree->Branch("fEtaWTop",&fEtaWTop,"fEtaWTop/F");
      fCommonTree->Branch("fCosPhiAngleWTop",&fCosPhiAngleWTop,"fCosPhiAngleWTop/D");
      fCommonTree->Branch("fCosPhiAngleAssociateWTop",&fCosPhiAngleAssociateWTop,"fCosPhiAngleAssociateWTop/D");
      fCommonTree->Branch("fCosPhiAngleAssociateWW",&fCosPhiAngleAssociateWW,"fCosPhiAngleAssociateWW/D");
      fCommonTree->Branch("fTopFrame_DThetaStarAssociateWTop",
			  &fTopFrame_DThetaStarAssociateWTop,"fTopFrame_DThetaStarAssociateWTop/D");
      fCommonTree->Branch("fWFrameCosThetaStar_DeltaWtop_bW",
			  &fWFrameCosThetaStar_DeltaWtop_bW,"fWFrameCosThetaStar_DeltaWtop_bW/D");
      fCommonTree->Branch("fTopFrameCosThetaStar_DeltaWtop_btop",
			  &fTopFrameCosThetaStar_DeltaWtop_btop,"fTopFrameCosThetaStar_DeltaWtop_btop/D");
      //fCommonTree->Branch("BerlinWeight",&GetPreTagEvtWeight(),"BerlinWeight/F");
      fCommonTree->Branch("fEtaW_LeptTop",&fEtaW_LeptTop,"fEtaW_LeptTop/F");
      fCommonTree->Branch("fCosThetaStarLeptNeutrino_LeptTop", &fCosThetaStarLeptNeutrino_LeptTop,
			  "fCosThetaStarLeptNeutrino_LeptTop/D");
      fCommonTree->Branch("fTopFrameCosThetaStarWb_LeptTop", &fTopFrameCosThetaStarWb_LeptTop,
			  "fTopFrameCosThetaStarWb_LeptTop/D");
      fCommonTree->Branch("fCosThetaStar1_LeptTop", &fCosThetaStar1_LeptTop,"fCosThetaStar1_LeptTop/D");
      fCommonTree->Branch("fCosThetaStar2_LeptTop", &fCosThetaStar2_LeptTop,"fCosThetaStar29_LeptTop/D");
      fCommonTree->Branch("fLogPWtBest",     &fLogPWtBest,     "fLogPWtBest/D");
      fCommonTree->Branch("fLogPWtHadronic", &fLogPWtHadronic, "fLogPWtHadronic/D");
      fCommonTree->Branch("fLogPWtLeptonic", &fLogPWtLeptonic, "fLogPWtLeptonic/D");
      fCommonTree->Branch("fPWtRatio",    &fPWtRatio,    "fPWtRatio/D");
      fCommonTree->Branch("fPWtSum",      &fPWtSum,      "fPWtSum/D");
      fCommonTree->Branch("fPWtDiff",     &fPWtDiff,     "fPWtDiff/D");
      fCommonTree->Branch("fPWtAsymmetry",&fPWtAsymmetry,"fPWtAsymmetry/D");

      fCommonTree->Branch("fLogPWtRatio",    &fLogPWtRatio,    "fLogPWtRatio/D");
      fCommonTree->Branch("fLogPWtSum",      &fLogPWtSum,      "fLogPWtSum/D");
      fCommonTree->Branch("fLogPWtDiff",     &fLogPWtDiff,     "fLogPWtDiff/D");
      fCommonTree->Branch("fLogPWtAsymmetry",&fLogPWtAsymmetry,"fLogPWtAsymmetry/D");



      gDirectory->mkdir("NtupleStuff");
      gDirectory->cd("NtupleStuff");
      // The above modulus vs eta of the 2ndLeadJet (fine binning)

      nbins_pt_fine = 25;
      n_eta_slices = 5;
      n_eta_slices_more = 7;
      fHistWtSystem_vs_TopEta_sliced_fine = new TH2F(
	  "h_WtSystem_vs_EtaTop_fine",
	  "Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of W. jet vs #eta of top-quark",
	  nbins_pt_fine,   0., 150.,
	  n_eta_slices, -6., 6.);
      //	100, 0., 100.,
      //	100, -5., 5.);
      fHistWtSystem_vs_TopEta_sliced_fine->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{W}|");
      fHistWtSystem_vs_TopEta_sliced_fine->SetYTitle("#eta_{top}");
      fHistWtSystem_vs_TopEta_sliced_fine->SetZTitle("Number of Entries");
      
      nbins_pt_coarse = 15;
      
      // The above modulus vs eta of the 2ndLeadJet (coarse binning)
      fHistWtSystem_vs_TopEta_sliced = new TH2F(
	  "h_WtSystem_vs_EtaTop",
	  "Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of W. jet vs #eta of top-quark",
	  nbins_pt_coarse, 0., 150.,
	  n_eta_slices, -6., 6.);
      //	100, 0., 100.,
      //	100, -5., 5.);
      fHistWtSystem_vs_TopEta_sliced->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{W}|");
      fHistWtSystem_vs_TopEta_sliced->SetYTitle("#eta_{top}");
      fHistWtSystem_vs_TopEta_sliced->SetZTitle("Number of Entries");


      fHistWtSystem_vs_TopEta_sliced_fine_more = new TH2F(
	  "h_WtSystem_vs_EtaTop_fine_more",
	  "Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of W. jet vs #eta of top-quark",
	  nbins_pt_fine,   0., 150.,
	  n_eta_slices_more, -6., 6.);
      //	100, 0., 100.,
      //	100, -5., 5.);
      fHistWtSystem_vs_TopEta_sliced_fine_more->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{W}|");
      fHistWtSystem_vs_TopEta_sliced_fine_more->SetYTitle("#eta_{top}");
      fHistWtSystem_vs_TopEta_sliced_fine_more->SetZTitle("Number of Entries");
      
      nbins_pt_coarse = 15;
      
      // The above modulus vs eta of the 2ndLeadJet (coarse binning)
      fHistWtSystem_vs_TopEta_sliced_more = new TH2F(
	  "h_WtSystem_vs_EtaTop_more",
	  "Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of W. jet vs #eta of top-quark",
	  nbins_pt_coarse, 0., 150.,
	  n_eta_slices_more, -6., 6.);
      //	100, 0., 100.,
      //	100, -5., 5.);
      fHistWtSystem_vs_TopEta_sliced_more->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{W}|");
      fHistWtSystem_vs_TopEta_sliced_more->SetYTitle("#eta_{top}");
      fHistWtSystem_vs_TopEta_sliced_more->SetZTitle("Number of Entries");

      Double_t EtaRange[6]; 
      EtaRange[0]=-6.; 
      EtaRange[1]=-3.; 
      EtaRange[2]=-1.;
      EtaRange[3]=1.;
      EtaRange[4]=3.;
      EtaRange[5]=6;
      // The above modulus vs eta of the 2ndLeadJet (coarse binning)
      fHistWtSystem_vs_TopEta_sliced_less = new TH2F(
	  "h_WtSystem_vs_EtaTop_less",
	  "Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of W. jet vs #eta of top-quark",
	  nbins_pt_coarse, 0., 150.,
	  5, EtaRange);
      //	100, 0., 100.,
      //	100, -5., 5.);
      fHistWtSystem_vs_TopEta_sliced_less->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{W}|");
      fHistWtSystem_vs_TopEta_sliced_less->SetYTitle("#eta_{top}");
      fHistWtSystem_vs_TopEta_sliced_less->SetZTitle("Number of Entries");

      
      
      fhist_fCosPhiAngleWTop = new TH1F("fhist_fCosPhiAngleWTop",
				     "Cos of Phi Angle between Top and W from top decay",
				     20,-1,1);
      fhist_fCosPhiAngleWTop->SetXTitle("cos(#Delta#phi)_{Top - W_{TopDecay}}");
      fhist_fCosPhiAngleWTop->SetYTitle("Number of Entries");
      
      fhist_fCosPhiAngleAssociateWTop = new TH1F("fhist_fCosPhiAngleAssociateWTop",
				     "Cos of Phi Angle between Top and associate W",
				     20,-1,1);
      fhist_fCosPhiAngleAssociateWTop->SetXTitle("cos(#Delta#phi)_{Top - W_{Associate}}");
      fhist_fCosPhiAngleAssociateWTop->SetYTitle("Number of Entries");

      fhist_fCosPhiAngleAssociateWW = new TH1F("fhist_fCosPhiAngleAssociateWW",
				     "Cos of Phi Angle between W-TopDEcay and associate W",
				     20,-1,1);
      fhist_fCosPhiAngleAssociateWW->SetXTitle("cos(#Delta#phi)_{W_{TopDecay} - W_{Associate}}");
      fhist_fCosPhiAngleAssociateWW->SetYTitle("Number of Entries");      
      
      fhist_fWFrameCosThetaStar_DeltaWtop_bW = new TH1F("fhist_fWFrameCosThetaStar_DeltaWtop_bW",
							"Cos of ThetaStar Angle between W_TopFrame and b_WFrame",
							20,-1,1);
      fhist_fWFrameCosThetaStar_DeltaWtop_bW->SetXTitle("cos(#Delta#theta^{*})_{W_{Top} - b_{W}}");
      fhist_fWFrameCosThetaStar_DeltaWtop_bW->SetYTitle("Number of Entries");
      fhist_fTopFrameCosThetaStar_DeltaWtop_btop = new TH1F("fhist_fTopFrameCosThetaStar_DeltaWtop_btop",
						      "Cos of ThetaStar Angle between W_TopFrame and b_TopFrame",
						      20,-1,1);
      fhist_fTopFrameCosThetaStar_DeltaWtop_btop->SetXTitle("cos(#Delta#theta^{*})_{W_{Top} - b_{Top}}");
      fhist_fTopFrameCosThetaStar_DeltaWtop_btop->SetYTitle("Number of Entries");

      fhist_fCosThetaStarLeptNeutrino_LeptTop = new TH1F("fhist_fCosThetaStarLeptNeutrino_LeptTop",
						      "Cos of ThetaStar Angle between lepton and neutrino in WFrame",
						      20,-1,1);
      fhist_fCosThetaStarLeptNeutrino_LeptTop->SetXTitle("cos(#Delta#theta^{*})_{l,#nu,TopFrame}");
      fhist_fCosThetaStarLeptNeutrino_LeptTop->SetYTitle("Number of Entries");

      fhist_fTopFrameCosThetaStarWb_LeptTop = new TH1F("fhist_fTopFrameCosThetaStarWb_LeptTop",
						      "Cos of ThetaStar Angle between b_TopFrame and W_TopFrame",
						      20,-1,1);
      fhist_fTopFrameCosThetaStarWb_LeptTop->SetXTitle("cos(#Delta#theta^{*})_{b-W TopFrame}");
      fhist_fTopFrameCosThetaStarWb_LeptTop->SetYTitle("Number of Entries");
      
      fhist_fCosThetaStar1_LeptTop = new TH1F("fhist_fCosThetaStar1_LeptTop",
					      "Cos of ThetaStar Angle between lept_WFrame and W_TopFrame",
					      20,-1,1);
      fhist_fCosThetaStar1_LeptTop->SetXTitle("cos(#Delta#theta^{*}_1)_{lept_W - W_Top}");
      fhist_fCosThetaStar1_LeptTop->SetYTitle("Number of Entries");
      
      fhist_fCosThetaStar2_LeptTop = new TH1F("fhist_fCosThetaStar2_LeptTop",
					      "Cos of ThetaStar Angle between lept_WFrame and b_WFrame",
					      20,-1,1);
      fhist_fCosThetaStar2_LeptTop->SetXTitle("cos(#Delta#theta^{*}_2)_{lept_W - b_W}");
      fhist_fCosThetaStar2_LeptTop->SetYTitle("Number of Entries");

//trying binning 1
      const Int_t nbinsH = 6;
      Double_t xbinsH[nbinsH+1];
      xbinsH[0] = 0;
      xbinsH[1] = 0.001;
      xbinsH[2] = 0.01;
      xbinsH[3] = 0.05;
      xbinsH[4] = 0.1;
      xbinsH[5] = 0.5;
      xbinsH[6] = 1.;
//trying binning 2
      const Int_t nbins = 20;
      Double_t xmax = 1;
      Double_t xmin2 = 0.5e-2;
      Double_t logxmin2 = TMath::Log10(xmin2);
      Double_t logxmax2 = TMath::Log10(xmax);
      Double_t binwidth2 = (logxmax2-logxmin2)/nbins;
      Double_t xbins2[nbins+1];
      xbins2[0] = 0;
      xbins2[1] = xmin2;
      for (Int_t i=2;i<=nbins;i++) {
	  xbins2[i] = xmin2 + TMath::Power(10,logxmin2+i*binwidth2);
      }
      
      fhist_fLogBest1 = new TH1F("fhist_fLogBin_1",
				"p-Value",
				    nbinsH,xbinsH);
      fhist_fLogBest1->SetXTitle("p-Value");
      fhist_fLogBest1->SetYTitle("Number of Entries");
      fhist_fLogBest2 = new TH1F("fhist_fLogBin_2",
				"p-Value",
				    nbins,xbins2);
      fhist_fLogBest2->SetXTitle("p-Value");
      fhist_fLogBest2->SetYTitle("Number of Entries");
      
      fhist_fLogBest = new TH1F("fhist_fLogBest",
				"Log of Best p-Value",
				50,0,400);
      fhist_fLogBest->SetXTitle("-log(p-Value_{best})");
      fhist_fLogBest->SetYTitle("Number of Entries");
      
      fhist_fLogLept = new TH1F("fhist_fLogLept",
			       "Log of Lept p-Value",
			       50,0,400);
      fhist_fLogLept->SetXTitle("-log(p-Value_{lept})");
      fhist_fLogLept->SetYTitle("Number of Entries");

      fhist_fLogHadr = new TH1F("fhist_fLogHadr",
			       "Log of Hadr p-Value",
			       50,0,400);
      fhist_fLogHadr->SetXTitle("-log(p-Value_{Hadr})");
      fhist_fLogHadr->SetYTitle("Number of Entries");

      fhist_fPWtRatio = new TH1F("fhist_fPWtRatio",
			       "fhist_fPWtRatio",
			       50,0,1);
      fhist_fPWtRatio->SetXTitle("p-Value Ratio (Best/Sum)");
      fhist_fPWtRatio->SetYTitle("Number of Entries");

      fhist_fPWtSum = new TH1F("fhist_fPWtSum",
			       "fhist_fPWtSum",
			       50,0,2);
      fhist_fPWtSum->SetXTitle("p-Value Sum");
      fhist_fPWtSum->SetYTitle("Number of Entries");

      fhist_fPWtDiff = new TH1F("fhist_fPWtDiff",
			       "fhist_fPWtDiff",
			       100,0,1);
      fhist_fPWtDiff->SetXTitle("p-Value Diff");
      fhist_fPWtDiff->SetYTitle("Number of Entries");

      fhist_fPWtAsymmetry = new TH1F("fhist_fPWtAsymmetry",
			       "fhist_fPWtAsymmetry",
			       80,0,1);
      fhist_fPWtAsymmetry->SetXTitle("p-Value Asymmetry");
      fhist_fPWtAsymmetry->SetYTitle("Number of Entries");

      fhist_fLogPWtRatio = new TH1F("fhist_fLogPWtRatio",
			       "fhist_fLogPWtRatio",
			       100,0,1);
      fhist_fLogPWtRatio->SetXTitle("-log(p-Value Ratio(Best/Sum))");
      fhist_fLogPWtRatio->SetYTitle("Number of Entries");

      fhist_fLogPWtSum = new TH1F("fhist_fLogPWtSum",
			       "",
			       80,0,400);
      fhist_fLogPWtSum->SetXTitle("-log(p-Value Sum)");
      fhist_fLogPWtSum->SetYTitle("Number of Entries");

      fhist_fLogPWtDiff = new TH1F("fhist_fLogPWtDiff",
			       "fhist_fLogPWtDiff",
			       80,0,400);
      fhist_fLogPWtDiff->SetXTitle("-log(p-Value Diff)");
      fhist_fLogPWtDiff->SetYTitle("Number of Entries");

      fhist_fLogPWtAsymmetry = new TH1F("fhist_fLogPWtAsymmetry",
			       "fhist_fLogPWtAsymmetry",
			       100,0,10);
      fhist_fLogPWtAsymmetry->SetXTitle("-log(p-Value Asymmetry)");
      fhist_fLogPWtAsymmetry->SetYTitle("Number of Entries");
      
      gDirectory->cd("..");
      
      gDirectory->cd("AtlSgTop_WtChannelFinder");
    }
    //
    // Book reconstruction histograms
    //
    fHistW_Mt_QCDValidation_pretag = new TH1F("h_w_mt_qcd_valid_pretag", "W Transverse Mass after common SgTop selection",
				    100, 0, 500);
    fHistW_Mt_QCDValidation_pretag->SetXTitle("m_{t}^{W} (GeV)");
    fHistW_Mt_QCDValidation_pretag->SetYTitle("Number of Entries");
    fHistW_Mt_QCDValidation_tag = new TH1F("h_w_mt_qcd_valid_tag", "W Transverse Mass after common SgTop selection",
				    100, 0, 500);
    fHistW_Mt_QCDValidation_tag->SetXTitle("m_{t}^{W} (GeV)");
    fHistW_Mt_QCDValidation_tag->SetYTitle("Number of Entries");

    fHist_WtSystem_Pt = new TH1F("h_WtSystem_Pt", "Global Transverse Momentum of the fitted W+top system",
				    12, 0, 100);
    fHist_WtSystem_Pt->SetXTitle("p_{t} (Wt)(GeV)");
    fHist_WtSystem_Pt->SetYTitle("Number of Entries");
    fHist_WtSystem_Pt_nocut = new TH1F("h_WtSystem_Pt_nocut", "Global Transverse Momentum of the fitted W+top system",
				    20, 0, 200);
    fHist_WtSystem_Pt_nocut->SetXTitle("p_{t} (Wt) (GeV)");
    fHist_WtSystem_Pt_nocut->SetYTitle("Number of Entries");

    fHist_EtaTop_nocut = new TH1F("h_topeta", "Top Pseudo-Rapidity",
	 	20, -6.0, 6.0);
    fHist_EtaTop_nocut->SetXTitle("#eta");
    fHist_EtaTop_nocut->SetYTitle("Number of Entries");

    fHist_PtTop_nocut = new TH1F("h_toppt", "Top Transverse Momentum",
				 20, 0., 200.);
    fHist_PtTop_nocut->SetXTitle("p_{t}");
    fHist_PtTop_nocut->SetYTitle("Number of Entries");

    
    
    fHistEtMissMag = new TH1F("h_etmiss_mag_NoCutsWSE",
				     "Magnitude of missing E_{T} before fit/tag",
				     40,0.,200);
    fHistEtMissPhi = new TH1F("h_etmiss_phi_NoCutsWSE",
				  "#varphi of missing E_{T} before fit/tag",
				  40,-3.2,3.2);
    fHistEtMissPhi->SetXTitle("#varphi");
    fHistEtMissPhi->SetYTitle("Number of Entries");

    fHistSumEt = new TH1F("h_sum_et", "SumEt before fit/tag",
				    100, 0, 1000);
    fHistSumEt->SetXTitle("#sum E_{t} (GeV)");
    fHistSumEt->SetYTitle("Number of Entries");

    fHistEtMissMag_pretag = new TH1F("h_etmiss_mag_before_pretag",
				     "Magnitude of missing E_{T} before fit",
				     40,0.,200);
    fHistEtMissPhi_pretag = new TH1F("h_etmiss_phi_before_pretag",
				  "#varphi of missing E_{T} before fit",
				  40,-3.2,3.2);
    fHistEtMissPhi_pretag->SetXTitle("#varphi");
    fHistEtMissPhi_pretag->SetYTitle("Number of Entries");
    fHistSumEt_pretag = new TH1F("h_sum_et_pretag", "SumEt before B-Tag requirement (tag)",
				    40, 0, 1000);
    fHistSumEt_pretag->SetXTitle("#sum E_{t} (GeV)");
    fHistSumEt_pretag->SetYTitle("Number of Entries");

    fHistEtMissMag_tag = new TH1F("h_etmiss_mag_before_tag",
				     "Magnitude of missing E_{T} before fit",
				     40,0.,200);
    fHistEtMissPhi_tag = new TH1F("h_etmiss_phi_before_tag",
				  "#varphi of missing E_{T} before fit",
				  40,-3.2,3.2);
    fHistEtMissPhi_tag->SetXTitle("#varphi");
    fHistEtMissPhi_tag->SetYTitle("Number of Entries");

    fHistSumEt_tag = new TH1F("h_sum_et_tag", "SumEt after B-Tag requirement (tag)",
				    40, 0, 1000);
    fHistSumEt_tag->SetXTitle("#sum E_{t} (GeV)");
    fHistSumEt_tag->SetYTitle("Number of Entries");

//
    // histograms of pretag and tag counts by lepton flavour for standalone tool 
    // determining  W reweighting factors
    //
    
    // ------
    // Pretag
    // ------
    gDirectory->mkdir("EventVariables");
    gDirectory->cd("EventVariables");
    // pretag, lepton +
    fHistCountLepPlus_pretag = new TH1F("h_count_lep_plus_pretag", 
					"Number of events (weighted) with positive lepton (pretag)",
					1, 0, 1);
    fHistCountLepPlus_pretag->SetXTitle("passed pretag");
    fHistCountLepPlus_pretag->SetYTitle("Number of Entries");
    TAxis *axis1 = fHistCountLepPlus_pretag->GetXaxis();
    axis1->SetBinLabel(1,"True");
    // pretag, lepton -
    fHistCountLepMinus_pretag = new TH1F("h_count_lep_minus_pretag", 
					 "Number of events (weighted) with negative lepton (pretag)",
					 1, 0, 1);
    fHistCountLepMinus_pretag->SetXTitle("passed pretag");
    fHistCountLepMinus_pretag->SetYTitle("Number of Entries");
    axis1 = fHistCountLepMinus_pretag->GetXaxis();
    axis1->SetBinLabel(1,"True");
    
    // ------
    // Tag
    // ------
    // tag, lepton all
    fHistCountLepAll_tag = new TH1F("h_count_lep_all_tag", 
				     "Total number of events (weighted, tag)",
				     1, 0, 1);
    fHistCountLepAll_tag->SetXTitle("passed tag");
    fHistCountLepAll_tag->SetYTitle("Number of Entries");
    axis1 = fHistCountLepAll_tag->GetXaxis();
    axis1->SetBinLabel(1,"True");
    
    // tag, lepton +
    fHistCountLepPlus_tag = new TH1F("h_count_lep_plus_tag", 
				     "Number of events (weighted) with positive lepton (tag)",
				     1, 0, 1);
    fHistCountLepPlus_tag->SetXTitle("passed tag");
    fHistCountLepPlus_tag->SetYTitle("Number of Entries");
    axis1 = fHistCountLepPlus_tag->GetXaxis();
    axis1->SetBinLabel(1,"True");
    // tag, lepton -
    fHistCountLepMinus_tag = new TH1F("h_count_lep_minus_tag", 
				      "Number of events (weighted) with negative lepton (tag)",
				      1, 0, 1);
    fHistCountLepMinus_tag->SetXTitle("passed tag");
    fHistCountLepMinus_tag->SetYTitle("Number of Entries");
    axis1 = fHistCountLepMinus_tag->GetXaxis();
    axis1->SetBinLabel(1,"True");
    
        
    // top_hfor_flag for use by wjets flavour reweighting tool
    fHist_HFOR_flag_pretag = new TH1I("h_hfor_flag_pretag", "HFOR flag after common SgTop selection (pretag)",
				      4, 0, 4);
    fHist_HFOR_flag_pretag->SetXTitle("HFOR flag (pretag)");
    fHist_HFOR_flag_pretag->SetYTitle("Number of Entries");

    //
    // Histograms of pretag and tag counts of central and forward electrons in e-channel
    //
    
    // ------
    // Pretag
    // ------    
    fHistCountFwdElectrons_pretag = new TH1F("h_count_fwd_electrons_pretag", 
				      "Weighted number of events with forward electrons (pretag)",
				      1, 0, 1);
    fHistCountFwdElectrons_pretag->SetXTitle("passed pretag");
    fHistCountFwdElectrons_pretag->SetYTitle("Number of Entries");
    TAxis *axis_qcd = fHistCountFwdElectrons_pretag->GetXaxis();
    axis_qcd->SetBinLabel(1,"True");

    fHistCountCentralElectrons_pretag = new TH1F("h_count_central_electrons_pretag", 
				      "Weighted number of events with central electrons (pretag)",
				      1, 0, 1);
    fHistCountCentralElectrons_pretag->SetXTitle("passed pretag");
    fHistCountCentralElectrons_pretag->SetYTitle("Number of Entries");
    axis_qcd = fHistCountCentralElectrons_pretag->GetXaxis();
    axis_qcd->SetBinLabel(1,"True");

    
    // ---
    // Tag
    // ---
    fHistCountFwdElectrons_tag = new TH1F("h_count_fwd_electrons_tag", 
				      "Weighted number of events with forward electrons (tag)",
				      1, 0, 1);
    fHistCountFwdElectrons_tag->SetXTitle("passed tag");
    fHistCountFwdElectrons_tag->SetYTitle("Number of Entries");
    axis_qcd = fHistCountFwdElectrons_tag->GetXaxis();
    axis_qcd->SetBinLabel(1,"True");

    fHistCountCentralElectrons_tag = new TH1F("h_count_central_electrons_tag", 
				      "Weighted number of events with central electrons (tag)",
				      1, 0, 1);
    fHistCountCentralElectrons_tag->SetXTitle("passed tag");
    fHistCountCentralElectrons_tag->SetYTitle("Number of Entries");
    axis_qcd = fHistCountCentralElectrons_tag->GetXaxis();
    axis_qcd->SetBinLabel(1,"True");


    gDirectory->cd("..");
    
    CommonHistograms();
    LeptonicTopHistograms();
    HadronicTopHistograms();

    BookComparisonHistogramsLeptTopFit();
    BookComparisonHistogramsHadrTopFit();
    // Initialize pointers to tools needed in event loop
    fCutflow_tool    = (AtlCutFlowTool*)GetTool("AtlCutFlowTool","AtlCutFlowTool",kTRUE);
    fCutflow_tool_AC = (AtlCutFlowTool*)GetTool("AtlCutFlowTool","AtlCutFlowTool_AC",kTRUE);
    
}

void AtlSgTop_WtChannelFinder::CommonHistograms() {
    BookLeptonHistograms();
    BookJetHistograms();
    BookLeptonHistograms_BTag();
    BookJetHistograms_BTag();

    // Cut Flow Histograms
    fHistCutFlowWeighted = new TH1F("h_CutFlowWeighted", "Effects of different Cuts, weighted",
				1000, 0., 1000.);
    fHistCutFlowWeighted->GetYaxis()->SetTitle("Number of Entries");
    fHistCutFlowUnweighted = new TH1F("h_CutFlowUnweighted", "Effects of different Cuts, unweighted",
				1000, 0., 1000.);
    fHistCutFlowUnweighted->GetYaxis()->SetTitle("Number of Entries");

    // chi2 and chi2-probability histograms for both semileptonic decay modes
    //
    
    fHistChi2Prob_both = new TH1F("h_chi2prob_both_inclusive",
				  "#chi^{2} prob. distribution of sgtop_WtChannel reco (both W decays), both semileptonic modes",
				  40,0,1);
    fHistChi2Prob_both->SetXTitle("P(#chi^{2})");
    fHistChi2Prob_both->SetYTitle("Number of Entries");
    axis = fHistChi2Prob_both->GetXaxis();
    //axis->Set( 3, Chi2ProbBins );
    
    fHistChi2_both = new TH1F("h_chi2_both_inclusive",
			      "#chi^{2} distribution of sgtop_WtChannel reco (both W decays), both semileptonic modes",
			      60,0,30);
    fHistChi2_both->SetXTitle("#chi^{2}");
    fHistChi2_both->SetYTitle("Number of Entries");

    fHistChi2Red_both = new TH1F("h_chi2Red_both_inclusive",
			      "#chi^{2} distribution of sgtop_WtChannel reco (both W decays), both semileptonic modes",
			      60,0,30);
    fHistChi2Red_both->SetXTitle("#chi^{2}");
    fHistChi2Red_both->SetYTitle("Number of Entries");

    fHistChi2NDOF_both = new TH1F("h_chi2NDOF_both_inclusive",
			      "#chi^{2} distribution of sgtop_WtChannel reco (both W decays), both semileptonic modes",
			      150,0,30);
    fHistChi2NDOF_both->SetXTitle("#chi^{2}/NDoF");
    fHistChi2NDOF_both->SetYTitle("Number of Entries");

    
    fHistChi2Prob_both_after = new TH1F("h_chi2prob_both_inclusive_after",
				  "#chi^{2} prob. distribution of sgtop_WtChannel reco (both W decays), both semileptonic modes",
				  40,0,1);
    fHistChi2Prob_both_after->SetXTitle("P(#chi^{2})");
    fHistChi2Prob_both_after->SetYTitle("Number of Entries");
    axis = fHistChi2Prob_both_after->GetXaxis();
    //axis->Set( 3, Chi2ProbBins );
    
    fHistChi2_both_after = new TH1F("h_chi2_both_inclusive_after",
			      "#chi^{2} distribution of sgtop_WtChannel reco (both W decays), both semileptonic modes",
			      150,0,30);
    fHistChi2_both_after->SetXTitle("#chi^{2}");
    fHistChi2_both_after->SetYTitle("Number of Entries");

    fHistChi2NDOF_both_after = new TH1F("h_chi2NDOF_both_inclusive_after",
			      "#chi^{2} distribution of sgtop_WtChannel reco (both W decays), both semileptonic modes",
			      150,0,30);
    fHistChi2NDOF_both_after->SetXTitle("#chi^{2}/NDoF");
    fHistChi2NDOF_both_after->SetYTitle("Number of Entries");

    
    //
    // Jet bin counts
    //
    gDirectory->mkdir("JetBins","Counts per jet bin");
    gDirectory->cd("JetBins");

    fHistJetBins_pretag = new TH1F("h_JetBins_pretag","Counts in jet bins (pretag)",
				   3,0.,3.);
    fHistJetBins_pretag->SetXTitle("Jet Bin");
    fHistJetBins_pretag->SetYTitle("Number of Entries");
    TAxis *axis = fHistJetBins_pretag->GetXaxis();
    axis->SetBinLabel(1,"1");
    axis->SetBinLabel(2,"2");
    axis->SetBinLabel(3,"3");
    axis->SetBinLabel(4,"4");
    axis->SetBinLabel(5,"5");

    fHistJetBins_tag = new TH1F("h_JetBins_tag","Counts in jet bins (tag)",
				   3,0.,3.);
    fHistJetBins_tag->SetXTitle("Jet Bin");
    fHistJetBins_tag->SetYTitle("Number of Entries");
    axis = fHistJetBins_tag->GetXaxis();
    axis->SetBinLabel(1,"1");
    axis->SetBinLabel(2,"2");
    axis->SetBinLabel(3,"3");
    axis->SetBinLabel(4,"4");
    axis->SetBinLabel(5,"5");

    fHistJetBins_fit = new TH1F("h_JetBins_fit","Counts in jet bins (after full fit selection)",
				   3,0.,3.);
    fHistJetBins_fit->SetXTitle("Jet Bin");
    fHistJetBins_fit->SetYTitle("Number of Entries");
    axis = fHistJetBins_fit->GetXaxis();
    axis->SetBinLabel(1,"1");
    axis->SetBinLabel(2,"2");
    axis->SetBinLabel(3,"3");
    axis->SetBinLabel(4,"4");
    axis->SetBinLabel(5,"5");
    gDirectory->cd("..");
}
void AtlSgTop_WtChannelFinder::LeptonicTopHistograms() {

     // ==================================
    // histograms for leptonic top decays
    // ==================================
    gDirectory->mkdir("LeptonicTOP", "Leptonic t/tbar reconstruction");
    gDirectory->cd("LeptonicTOP");
    // =========================
    // Leptonic W reconstruction
    // =========================
    gDirectory->mkdir("LeptonicW-reco", "LeptonicW reconstruction");
    gDirectory->cd("LeptonicW-reco");
    gDirectory->mkdir("Both", "W+/W- reconstruction");
    gDirectory->cd("Both");
    
    // All leptons
    fHist_W_lept_top_reco_Mt = new TH1F("h_W_lept_top_reco_Mt", "W Invariant Transverse Mass (all leptons)",
			      200, 0, 130);
    fHist_W_lept_top_reco_Mt->SetXTitle("M_{t,W} (GeV)");
    fHist_W_lept_top_reco_Mt->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_M = new TH1F("h_W_lept_top_reco_M", "W Invariant Mass (all leptons)",
			      200, 40, 120);
    fHist_W_lept_top_reco_M->SetXTitle("M_{W} (GeV)");
    fHist_W_lept_top_reco_M->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_phi = new TH1F("h_W_lept_top_reco_phi", "W Azimuth Angle (all leptons)",
				128, -3.2, 3.2);
    fHist_W_lept_top_reco_phi->SetXTitle("#phi (rad)");
    fHist_W_lept_top_reco_phi->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_pt = new TH1F("h_W_lept_top_reco_pt", "W Transverse Momentum (all leptons)",
			       100, 0, 300);
    fHist_W_lept_top_reco_pt->SetXTitle("P_{t} (GeV)");
    fHist_W_lept_top_reco_pt->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_eta = new TH1F("h_W_lept_top_reco_eta", "W Pseudo-Rapidity (all leptons)",
				     50, -5., 5.);
    fHist_W_lept_top_reco_eta->SetXTitle("#eta");
    fHist_W_lept_top_reco_eta->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_rap = new TH1F("h_W_lept_top_reco_rap", "W Rapidity (all leptons)",
				     50, -5., 5.);
    fHist_W_lept_top_reco_rap->SetXTitle("y (rapidity)");
    fHist_W_lept_top_reco_rap->SetYTitle("Number of Entries");

    // Electron channel
    gDirectory->mkdir("Electron", "Electron channel");
    gDirectory->cd("Electron");
    fHist_W_lept_top_reco_Mt_e = new TH1F("h_W_lept_top_reco_Mt_e", "W Invariant Transverse Mass (e channel)",
			      200, 0, 130);
    fHist_W_lept_top_reco_Mt_e->SetXTitle("M_{t,W} (GeV)");
    fHist_W_lept_top_reco_Mt_e->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_M_e = new TH1F("h_W_lept_top_reco_M_e", "W Invariant Mass (e channel)",
			      200, 40, 120);
    fHist_W_lept_top_reco_M_e->SetXTitle("M_{W} (GeV)");
    fHist_W_lept_top_reco_M_e->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_phi_e = new TH1F("h_W_lept_top_reco_phi_e", "W Azimuth Angle (e channel)",
				128, -3.2, 3.2);
    fHist_W_lept_top_reco_phi_e->SetXTitle("#phi (rad)");
    fHist_W_lept_top_reco_phi_e->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_pt_e = new TH1F("h_W_lept_top_reco_pt_e", "W Transverse Momentum (e channel)",
			       200, 0, 200);
    fHist_W_lept_top_reco_pt_e->SetXTitle("P_{t} (GeV)");
    fHist_W_lept_top_reco_pt_e->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_eta_e = new TH1F("h_W_lept_top_reco_eta_e", "W Pseudo-Rapidity (e channel)",
				     50, -5., 5.);
    fHist_W_lept_top_reco_eta_e->SetXTitle("#eta");
    fHist_W_lept_top_reco_eta_e->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_rap_e = new TH1F("h_W_lept_top_reco_rap_e", "W Rapidity (e channel)",
				     50, -5.0, 5.0);
    fHist_W_lept_top_reco_rap_e->SetXTitle("y (rapidity)");
    fHist_W_lept_top_reco_rap_e->SetYTitle("Number of Entries");
    gDirectory->cd("..");    
    // Muon channel
    gDirectory->mkdir("Muon", "Muon channel");
    gDirectory->cd("Muon");
    fHist_W_lept_top_reco_Mt_mu = new TH1F("h_W_lept_top_reco_Mt_mu", "W Invariant Transverse Mass (#mu channel)",
			       200, 0, 130);
    fHist_W_lept_top_reco_Mt_mu->SetXTitle("M_{t,W} (GeV)");
    fHist_W_lept_top_reco_Mt_mu->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_M_mu = new TH1F("h_W_lept_top_reco_M_mu", "W Invariant Mass (#mu channel)",
			       200, 40, 120);
    fHist_W_lept_top_reco_M_mu->SetXTitle("M_{W} (GeV)");
    fHist_W_lept_top_reco_M_mu->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_phi_mu = new TH1F("h_W_lept_top_reco_phi_mu", "W Azimuth Angle (#mu channel)",
				 128, -3.2, 3.2);
    fHist_W_lept_top_reco_phi_mu->SetXTitle("#phi (rad)");
    fHist_W_lept_top_reco_phi_mu->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_pt_mu = new TH1F("h_W_lept_top_reco_pt_mu", "W Transverse Momentum (#mu channel)",
				100, 0, 300);
    fHist_W_lept_top_reco_pt_mu->SetXTitle("P_{t} (GeV)");
    fHist_W_lept_top_reco_pt_mu->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_eta_mu = new TH1F("h_W_lept_top_reco_eta_mu", "W Pseudo-Rapidity (#mu channel)",
				     50, -5.0, 5.0);
    fHist_W_lept_top_reco_eta_mu->SetXTitle("#eta");
    fHist_W_lept_top_reco_eta_mu->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_rap_mu = new TH1F("h_W_lept_top_reco_rap_mu", "W Rapidity (#mu channel)",
				     50, -5.0, 5.0);
    fHist_W_lept_top_reco_rap_mu->SetXTitle("y (rapidity)");
    fHist_W_lept_top_reco_rap_mu->SetYTitle("Number of Entries");
    gDirectory->cd("..");    
    
    gDirectory->cd("..");
    gDirectory->mkdir("W-", "W- reconstruction");
    gDirectory->cd("W-");
    
    // All negative sign leptons
    fHist_W_lept_top_reco_Mt_minus = new TH1F("h_W_lept_top_reco_Mt_minus",
				   "W^{-} Invariant Transverse Mass (all negative sign leptons)",
				   65, 0, 130);
    fHist_W_lept_top_reco_Mt_minus->SetXTitle("M_{t,W} (GeV)");
    fHist_W_lept_top_reco_Mt_minus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_M_minus = new TH1F("h_W_lept_top_reco_M_minus",
				   "W^{-} Invariant Mass (all negative sign leptons)",
				   80, 40, 120);
    fHist_W_lept_top_reco_M_minus->SetXTitle("M_{W} (GeV)");
    fHist_W_lept_top_reco_M_minus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_phi_minus = new TH1F("h_W_lept_top_reco_phi_minus",
				    "W^{-} Azimuth Angle (all negative sign leptons)",
				    128, -3.2, 3.2);
    fHist_W_lept_top_reco_phi_minus->SetXTitle("#phi (rad)");
    fHist_W_lept_top_reco_phi_minus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_pt_minus = new TH1F("h_W_lept_top_reco_pt_minus",
				   "W^{-} Transverse Momentum (all negative sign leptons)",
				   100, 0, 300);
    fHist_W_lept_top_reco_pt_minus->SetXTitle("P_{t} (GeV)");
    fHist_W_lept_top_reco_pt_minus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_eta_minus = new TH1F("h_W_lept_top_reco_eta_minus",
				    "W^{-} Pseudo-Rapidity (all negative sign leptons)",
				    50, -5.0, 5.0);
    fHist_W_lept_top_reco_eta_minus->SetXTitle("#eta");
    fHist_W_lept_top_reco_eta_minus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_rap_minus = new TH1F("h_W_lept_top_reco_rap_minus",
				    "W^{-} Rapidity (all negative sign leptons)",
				    50, -5.0, 5.0);
    fHist_W_lept_top_reco_rap_minus->SetXTitle("y (rapidity)");
    fHist_W_lept_top_reco_rap_minus->SetYTitle("Number of Entries");
        
    // Electron e- channel
    gDirectory->mkdir("Electron", "Electron channel");
    gDirectory->cd("Electron");
    fHist_W_lept_top_reco_Mt_e_minus = new TH1F("h_W_lept_top_reco_Mt_e_minus",
				     "W^{-} Invariant Transverse Mass (e^{-} channel)",
				     65, 0, 130);
    fHist_W_lept_top_reco_Mt_e_minus->SetXTitle("M_{t,W} (GeV)");
    fHist_W_lept_top_reco_Mt_e_minus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_M_e_minus = new TH1F("h_W_lept_top_reco_M_e_minus",
				     "W^{-} Invariant Mass (e^{-} channel)",
				     80, 40, 120);
    fHist_W_lept_top_reco_M_e_minus->SetXTitle("M_{W} (GeV)");
    fHist_W_lept_top_reco_M_e_minus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_phi_e_minus = new TH1F("h_W_lept_top_reco_phi_e_minus",
				      "W^{-} Azimuth Angle (e^{-} channel)",
				      128, -3.2, 3.2);
    fHist_W_lept_top_reco_phi_e_minus->SetXTitle("#phi (rad)");
    fHist_W_lept_top_reco_phi_e_minus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_pt_e_minus = new TH1F("h_W_lept_top_reco_pt_e_minus",
				     "W^{-} Transverse Momentum (e^{-} channel)",
				     100, 0, 300);
    fHist_W_lept_top_reco_pt_e_minus->SetXTitle("P_{t} (GeV)");
    fHist_W_lept_top_reco_pt_e_minus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_eta_e_minus = new TH1F("h_W_lept_top_reco_eta_e_minus",
				      "W^{-} Pseudo-Rapidity (e^{-} channel)",
				      50, -5.0, 5.0);
    fHist_W_lept_top_reco_eta_e_minus->SetXTitle("#eta");
    fHist_W_lept_top_reco_eta_e_minus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_rap_e_minus = new TH1F("h_W_lept_top_reco_rap_e_minus",
				      "W^{-} Rapidity (e^{-} channel)",
				      50, -5.0, 5.0);
    fHist_W_lept_top_reco_rap_e_minus->SetXTitle("y (rapidity)");
    fHist_W_lept_top_reco_rap_e_minus->SetYTitle("Number of Entries");
    gDirectory->cd("..");

    // Muon mu- channel
    gDirectory->mkdir("Muon", "Muon channel");
    gDirectory->cd("Muon");
    
    fHist_W_lept_top_reco_Mt_mu_minus = new TH1F("h_W_lept_top_reco_Mt_mu_minus",
				      "W^{-} Invariant Transverse Mass (#mu^{-} channel)",
				      65, 0, 130);
    fHist_W_lept_top_reco_Mt_mu_minus->SetXTitle("M_{t,W} (GeV)");
    fHist_W_lept_top_reco_Mt_mu_minus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_M_mu_minus = new TH1F("h_W_lept_top_reco_M_mu_minus",
				      "W^{-} Invariant Mass (#mu^{-} channel)",
				      80, 40, 120);
    fHist_W_lept_top_reco_M_mu_minus->SetXTitle("M_{W} (GeV)");
    fHist_W_lept_top_reco_M_mu_minus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_phi_mu_minus = new TH1F("h_W_lept_top_reco_phi_mu_minus",
				       "W^{-} Azimuth Angle (#mu^{-} channel)",
				       128, -3.2, 3.2);
    fHist_W_lept_top_reco_phi_mu_minus->SetXTitle("#phi (rad)");
    fHist_W_lept_top_reco_phi_mu_minus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_pt_mu_minus = new TH1F("h_W_lept_top_reco_pt_mu_minus",
				      "W^{-} Transverse Momentum (#mu^{-} channel)",
				      100, 0, 300);
    fHist_W_lept_top_reco_pt_mu_minus->SetXTitle("P_{t} (GeV)");
    fHist_W_lept_top_reco_pt_mu_minus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_eta_mu_minus = new TH1F("h_W_lept_top_reco_eta_mu_minus",
				       "W^{-} Pseudo-Rapidity (#mu^{-} channel)",
				       50, -5.0, 5.0);
    fHist_W_lept_top_reco_eta_mu_minus->SetXTitle("#eta");
    fHist_W_lept_top_reco_eta_mu_minus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_rap_mu_minus = new TH1F("h_W_lept_top_reco_rap_mu_minus",
				       "W^{-} Rapidity (#mu^{-} channel)",
				       50, -5.0, 5.0);
    fHist_W_lept_top_reco_rap_mu_minus->SetXTitle("y (rapidity)");
    fHist_W_lept_top_reco_rap_mu_minus->SetYTitle("Number of Entries");
    gDirectory->cd("..");

    gDirectory->cd("..");
    gDirectory->mkdir("W+", "W+ reconstruction");
    gDirectory->cd("W+");
    
    // All positive sign leptons
    fHist_W_lept_top_reco_Mt_plus = new TH1F("h_W_lept_top_reco_Mt_plus",
				  "W^{+} Invariant Transverse Mass (all positive sign leptons)",
				  65, 0, 130);
    fHist_W_lept_top_reco_Mt_plus->SetXTitle("M_{t,W} (GeV)");
    fHist_W_lept_top_reco_Mt_plus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_M_plus = new TH1F("h_W_lept_top_reco_M_plus",
				  "W^{+} Invariant Mass (all positive sign leptons)",
				  80, 40, 120);
    fHist_W_lept_top_reco_M_plus->SetXTitle("M_{W} (GeV)");
    fHist_W_lept_top_reco_M_plus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_phi_plus = new TH1F("h_W_lept_top_reco_phi_plus",
				   "W^{+} Azimuth Angle (all positive sign leptons)",
				   128, -3.2, 3.2);
    fHist_W_lept_top_reco_phi_plus->SetXTitle("#phi (rad)");
    fHist_W_lept_top_reco_phi_plus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_pt_plus = new TH1F("h_W_lept_top_reco_pt_plus",
				  "W^{+} Transverse Momentum (all positive sign leptons)",
				  100, 0, 300);
    fHist_W_lept_top_reco_pt_plus->SetXTitle("P_{t} (GeV)");
    fHist_W_lept_top_reco_pt_plus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_eta_plus = new TH1F("h_W_lept_top_reco_eta_plus",
				   "W^{+} Pseudo-Rapidity (all positive sign leptons)",
				   50, -5.0, 5.0);
    fHist_W_lept_top_reco_eta_plus->SetXTitle("#eta");
    fHist_W_lept_top_reco_eta_plus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_rap_plus = new TH1F("h_W_lept_top_reco_rap_plus",
				   "W^{+} Rapidity (all positive sign leptons)",
				   50, -5.0, 5.0);
    fHist_W_lept_top_reco_rap_plus->SetXTitle("y (rapidity)");
    fHist_W_lept_top_reco_rap_plus->SetYTitle("Number of Entries");
    
    // Electron e+ channel
    gDirectory->mkdir("Electron", "Electron channel");
    gDirectory->cd("Electron");
    fHist_W_lept_top_reco_Mt_e_plus = new TH1F("h_W_lept_top_reco_Mt_e_plus",
				    "W^{+} Invariant Transverse Mass (e^{+} channel)",
				    65, 0, 130);
    fHist_W_lept_top_reco_Mt_e_plus->SetXTitle("M_{t,W} (GeV)");
    fHist_W_lept_top_reco_Mt_e_plus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_M_e_plus = new TH1F("h_W_lept_top_reco_M_e_plus",
				    "W^{+} Invariant Mass (e^{+} channel)",
				    80, 40, 120);
    fHist_W_lept_top_reco_M_e_plus->SetXTitle("M_{W} (GeV)");
    fHist_W_lept_top_reco_M_e_plus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_phi_e_plus = new TH1F("h_W_lept_top_reco_phi_e_plus",
				     "W^{+} Azimuth Angle (e^{+} channel)",
				     128, -3.2, 3.2);
    fHist_W_lept_top_reco_phi_e_plus->SetXTitle("#phi (rad)");
    fHist_W_lept_top_reco_phi_e_plus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_pt_e_plus = new TH1F("h_W_lept_top_reco_pt_e_plus",
				    "W^{+} Transverse Momentum (e^{+} channel)",
				    100, 0, 300);
    fHist_W_lept_top_reco_pt_e_plus->SetXTitle("P_{t} (GeV)");
    fHist_W_lept_top_reco_pt_e_plus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_eta_e_plus = new TH1F("h_W_lept_top_reco_eta_e_plus",
				     "W^{+} Pseudo-Rapidity (e^{+} channel)",
				     50, -5.0, 5.0);
    fHist_W_lept_top_reco_eta_e_plus->SetXTitle("#eta");
    fHist_W_lept_top_reco_eta_e_plus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_rap_e_plus = new TH1F("h_W_lept_top_reco_rap_e_plus", "W^{+} Rapidity (e^{+} channel)",
				     50, -5.0, 5.0);
    fHist_W_lept_top_reco_rap_e_plus->SetXTitle("y (rapidity)");
    fHist_W_lept_top_reco_rap_e_plus->SetYTitle("Number of Entries");
    gDirectory->cd("..");

    // Muon mu+ channel
    gDirectory->mkdir("Muon", "Muon channel");
    gDirectory->cd("Muon");
    fHist_W_lept_top_reco_Mt_mu_plus = new TH1F("h_W_lept_top_reco_Mt_mu_plus",
				     "W^{+} Invariant Transverse Mass (#mu^{+} channel)",
				     65, 0, 130);
    fHist_W_lept_top_reco_Mt_mu_plus->SetXTitle("M_{t,W} (GeV)");
    fHist_W_lept_top_reco_Mt_mu_plus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_M_mu_plus = new TH1F("h_W_lept_top_reco_M_mu_plus",
				     "W^{+} Invariant Mass (#mu^{+} channel)",
				     80, 40, 120);
    fHist_W_lept_top_reco_M_mu_plus->SetXTitle("M_{W} (GeV)");
    fHist_W_lept_top_reco_M_mu_plus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_phi_mu_plus = new TH1F("h_W_lept_top_reco_phi_mu_plus",
				      "W^{+} Azimuth Angle (#mu^{+} channel)",
				      128, -3.2, 3.2);
    fHist_W_lept_top_reco_phi_mu_plus->SetXTitle("#phi (rad)");
    fHist_W_lept_top_reco_phi_mu_plus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_pt_mu_plus = new TH1F("h_W_lept_top_reco_pt_mu_plus",
				     "W^{+} Transverse Momentum (#mu^{+} channel)",
				     100, 0, 300);
    fHist_W_lept_top_reco_pt_mu_plus->SetXTitle("P_{t} (GeV)");
    fHist_W_lept_top_reco_pt_mu_plus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_eta_mu_plus = new TH1F("h_W_lept_top_reco_eta_mu_plus",
				      "W^{+} Pseudo-Rapidity (#mu^{+} channel)",
				      50, -5.0, 5.0);
    fHist_W_lept_top_reco_eta_mu_plus->SetXTitle("#eta");
    fHist_W_lept_top_reco_eta_mu_plus->SetYTitle("Number of Entries");
    fHist_W_lept_top_reco_rap_mu_plus = new TH1F("h_W_lept_top_reco_rap_mu_plus",
				      "W^{+} Rapidity (#mu^{+} channel)",
				      50, -5.0, 5.0);
    fHist_W_lept_top_reco_rap_mu_plus->SetXTitle("y (rapidity)");
    fHist_W_lept_top_reco_rap_mu_plus->SetYTitle("Number of Entries");
    gDirectory->cd("..");
    gDirectory->cd("../..");
    
    // ========================
    // Top-quark reconstruction
    // ========================
    gDirectory->mkdir("Top-reco", "Top reconstruction");
    gDirectory->cd("Top-reco");

    gDirectory->mkdir("Both", "t/#bar{t} reconstruction");
    gDirectory->cd("Both");

    // All leptons
    fHistLeptonicTop_Mt = new TH1F("h_treco_Mt", "Top Transverse Invariant Mass (all leptons)",
			       75, 0, 300);
    fHistLeptonicTop_Mt->SetXTitle("M_{t,top} (GeV)");
    fHistLeptonicTop_Mt->SetYTitle("Number of Entries");
    fHistLeptonicTop_M = new TH1F("h_treco_M", "Top Invariant Mass (all leptons)",
			       120, 165, 185);
    fHistLeptonicTop_M->SetXTitle("M_{top} (GeV)");
    fHistLeptonicTop_M->SetYTitle("Number of Entries");
    fHistLeptonicTop_phi = new TH1F("h_treco_phi", "Top Azimuth Angle (all leptons)",
				64, -3.2, 3.2);
    fHistLeptonicTop_phi->SetXTitle("#phi (rad)");
    fHistLeptonicTop_phi->SetYTitle("Number of Entries");
    fHistLeptonicTop_pt = new TH1F("h_treco_pt", "Top Transverse Momentum (all leptons)",
			       100, 0, 300);
    fHistLeptonicTop_pt->SetXTitle("P_{t} (GeV)");
    fHistLeptonicTop_pt->SetYTitle("Number of Entries");
    fHistLeptonicTop_eta = new TH1F("h_treco_eta", "Top Pseudo-Rapidity (all leptons)",
				50, -5.0, 5.0);
    fHistLeptonicTop_eta->SetXTitle("#eta");
    fHistLeptonicTop_eta->SetYTitle("Number of Entries");
    fHistLeptonicTop_rap = new TH1F("h_treco_rap", "Top Rapidity (all leptons)",
				50, -5.0, 5.0);
    fHistLeptonicTop_rap->SetXTitle("y (rapidity)");
    fHistLeptonicTop_rap->SetYTitle("Number of Entries");
    
    // Electron channel
    gDirectory->mkdir("Electron", "Electron channel");
    gDirectory->cd("Electron");

    fHistLeptonicTop_Mt_e = new TH1F("h_treco_Mt_e", "Top Transverse Invariant Mass (e channel)",
				 60, 0, 210);
    fHistLeptonicTop_Mt_e->SetXTitle("M_{t,top} (GeV)");
    fHistLeptonicTop_Mt_e->SetYTitle("Number of Entries");
    fHistLeptonicTop_M_e = new TH1F("h_treco_M_e", "Top Invariant Mass (e channel)",
				 120, 165, 185);
    fHistLeptonicTop_M_e->SetXTitle("M_{top} (GeV)");
    fHistLeptonicTop_M_e->SetYTitle("Number of Entries");
    fHistLeptonicTop_phi_e = new TH1F("h_treco_phi_e", "Top Azimuth Angle (e channel)",
				  64, -3.2, 3.2);
    fHistLeptonicTop_phi_e->SetXTitle("#phi (rad)");
    fHistLeptonicTop_phi_e->SetYTitle("Number of Entries");
    fHistLeptonicTop_pt_e = new TH1F("h_treco_pt_e", "Top Transverse Momentum (e channel)",
				 100, 0, 300);
    fHistLeptonicTop_pt_e->SetXTitle("P_{t} (GeV)");
    fHistLeptonicTop_pt_e->SetYTitle("Number of Entries");
    fHistLeptonicTop_eta_e = new TH1F("h_treco_eta_e", "Top Pseudo-Rapidity (e channel)",
				  50, -5.0, 5.0);
    fHistLeptonicTop_eta_e->SetXTitle("#eta");
    fHistLeptonicTop_eta_e->SetYTitle("Number of Entries");
    fHistLeptonicTop_rap_e = new TH1F("h_treco_rap_e", "Top Rapidity (e channel)",
				  50, -5.0, 5.0);
    fHistLeptonicTop_rap_e->SetXTitle("y (rapidity)");
    fHistLeptonicTop_rap_e->SetYTitle("Number of Entries");
    gDirectory->cd("..");
    
    // Muon channel
    gDirectory->mkdir("Muon", "Muon channel");
    gDirectory->cd("Muon");
    fHistLeptonicTop_Mt_mu = new TH1F("h_treco_Mt_mu", "Top Transverse Invariant Mass (#mu channel)",
				  60, 0, 210);
    fHistLeptonicTop_Mt_mu->SetXTitle("M_{t,top} (GeV)");
    fHistLeptonicTop_Mt_mu->SetYTitle("Number of Entries");
    fHistLeptonicTop_M_mu = new TH1F("h_treco_M_mu", "Top Invariant Mass (#mu channel)",
				  120, 165, 185);
    fHistLeptonicTop_M_mu->SetXTitle("M_{top} (GeV)");
    fHistLeptonicTop_M_mu->SetYTitle("Number of Entries");
    fHistLeptonicTop_phi_mu = new TH1F("h_treco_phi_mu", "Top Azimuth Angle (#mu channel)",
				   64, -3.2, 3.2);
    fHistLeptonicTop_phi_mu->SetXTitle("#phi (rad)");
    fHistLeptonicTop_phi_mu->SetYTitle("Number of Entries");
    fHistLeptonicTop_pt_mu = new TH1F("h_treco_pt_mu", "Top Transverse Momentum (#mu channel)",
				  100, 0, 300);
    fHistLeptonicTop_pt_mu->SetXTitle("P_{t} (GeV)");
    fHistLeptonicTop_pt_mu->SetYTitle("Number of Entries");
    fHistLeptonicTop_eta_mu = new TH1F("h_treco_eta_mu", "Top Pseudo-Rapidity (#mu channel)",
				   50, -5.0, 5.0);
    fHistLeptonicTop_eta_mu->SetXTitle("#eta");
    fHistLeptonicTop_eta_mu->SetYTitle("Number of Entries");
    fHistLeptonicTop_rap_mu = new TH1F("h_treco_rap_mu", "Top Rapidity (#mu channel)",
				   50, -5.0, 5.0);
    fHistLeptonicTop_rap_mu->SetXTitle("y (rapidity)");
    fHistLeptonicTop_rap_mu->SetYTitle("Number of Entries");
    gDirectory->cd("..");

    gDirectory->cd("..");
    gDirectory->mkdir("t", "t-quark reconstruction");
    gDirectory->cd("t");

    // All positive sign leptons
    fHistLeptonicTop_Mt_plus = new TH1F("h_treco_Mt_plus",
				    "t-Quark Transverse Invariant Mass (all positive sign leptons)",
				    60, 0, 210);
    fHistLeptonicTop_Mt_plus->SetXTitle("M_{t,top} (GeV)");
    fHistLeptonicTop_Mt_plus->SetYTitle("Number of Entries");
    fHistLeptonicTop_M_plus = new TH1F("h_treco_M_plus",
				    "t-Quark Invariant Mass (all positive sign leptons)",
				    120, 165, 185);
    fHistLeptonicTop_M_plus->SetXTitle("M_{top} (GeV)");
    fHistLeptonicTop_M_plus->SetYTitle("Number of Entries");
    fHistLeptonicTop_phi_plus = new TH1F("h_treco_phi_plus",
				     "t-Quark Azimuth Angle (all positive sign leptons)",
				     64, -3.2, 3.2);
    fHistLeptonicTop_phi_plus->SetXTitle("#phi (rad)");
    fHistLeptonicTop_phi_plus->SetYTitle("Number of Entries");
    fHistLeptonicTop_pt_plus = new TH1F("h_treco_pt_plus",
				    "t-Quark Transverse Momentum (all positive sign leptons)",
				    100, 0, 300);
    fHistLeptonicTop_pt_plus->SetXTitle("P_{t} (GeV)");
    fHistLeptonicTop_pt_plus->SetYTitle("Number of Entries");
    fHistLeptonicTop_eta_plus = new TH1F("h_treco_eta_plus",
				     "t-Quark Pseudo-Rapidity (all positive sign leptons)",
				     50, -5.0, 5.0);
    fHistLeptonicTop_eta_plus->SetXTitle("#eta");
    fHistLeptonicTop_eta_plus->SetYTitle("Number of Entries");
    fHistLeptonicTop_rap_plus = new TH1F("h_treco_rap_plus",
				     "t-Quark Rapidity (all positive sign leptons)",
				     50, -5.0, 5.0);
    fHistLeptonicTop_rap_plus->SetXTitle("y (rapidity)");
    fHistLeptonicTop_rap_plus->SetYTitle("Number of Entries");

    // Electron e+ channel
    gDirectory->mkdir("Electron", "Electron channel");
    gDirectory->cd("Electron");
    fHistLeptonicTop_Mt_e_plus = new TH1F("h_treco_Mt_e_plus",
				      "t-Quark Transverse Invariant Mass (e^{+} channel)",
				      60, 0, 210);
    fHistLeptonicTop_Mt_e_plus->SetXTitle("M_{t,top} (GeV)");
    fHistLeptonicTop_Mt_e_plus->SetYTitle("Number of Entries");
    fHistLeptonicTop_M_e_plus = new TH1F("h_treco_M_e_plus",
				      "t-Quark Invariant Mass (e^{+} channel)",
				      120, 165, 185);
    fHistLeptonicTop_M_e_plus->SetXTitle("M_{top} (GeV)");
    fHistLeptonicTop_M_e_plus->SetYTitle("Number of Entries");
    fHistLeptonicTop_phi_e_plus = new TH1F("h_treco_phi_e_plus",
				       "t-Quark Azimuth Angle (e^{+} channel)",
				       64, -3.2, 3.2);
    fHistLeptonicTop_phi_e_plus->SetXTitle("#phi (rad)");
    fHistLeptonicTop_phi_e_plus->SetYTitle("Number of Entries");
    fHistLeptonicTop_pt_e_plus = new TH1F("h_treco_pt_e_plus",
				      "t-Quark Transverse Momentum (e^{+} channel)",
				      100, 0, 300);
    fHistLeptonicTop_pt_e_plus->SetXTitle("P_{t} (GeV)");
    fHistLeptonicTop_pt_e_plus->SetYTitle("Number of Entries");
    fHistLeptonicTop_eta_e_plus = new TH1F("h_treco_eta_e_plus",
				       "t-Quark Pseudo-Rapidity (e^{+} channel)",
				       50, -5.0, 5.0);
    fHistLeptonicTop_eta_e_plus->SetXTitle("#eta");
    fHistLeptonicTop_eta_e_plus->SetYTitle("Number of Entries");
    fHistLeptonicTop_rap_e_plus = new TH1F("h_treco_rap_e_plus",
				       "t-Quark Rapidity (e^{+} channel)",
				       50, -5.0, 5.0);
    fHistLeptonicTop_rap_e_plus->SetXTitle("y (rapidity)");
    fHistLeptonicTop_rap_e_plus->SetYTitle("Number of Entries");
    gDirectory->cd("..");
    
    // Muon mu+ channel
    gDirectory->mkdir("Muon", "Muon channel");
    gDirectory->cd("Muon");
    fHistLeptonicTop_Mt_mu_plus = new TH1F("h_treco_Mt_mu_plus",
				       "t-Quark Transverse Invariant Mass (#mu^{+} channel)",
				       60, 0, 210);
    fHistLeptonicTop_Mt_mu_plus->SetXTitle("M_{t,top} (GeV)");
    fHistLeptonicTop_Mt_mu_plus->SetYTitle("Number of Entries");
    fHistLeptonicTop_M_mu_plus = new TH1F("h_treco_M_mu_plus",
				       "t-Quark Invariant Mass (#mu^{+} channel)",
				       120, 165, 185);
    fHistLeptonicTop_M_mu_plus->SetXTitle("M_{top} (GeV)");
    fHistLeptonicTop_M_mu_plus->SetYTitle("Number of Entries");
    fHistLeptonicTop_phi_mu_plus = new TH1F("h_treco_phi_mu_plus",
					"t-Quark Azimuth Angle (#mu^{+} channel)",
					64, -3.2, 3.2);
    fHistLeptonicTop_phi_mu_plus->SetXTitle("#phi (rad)");
    fHistLeptonicTop_phi_mu_plus->SetYTitle("Number of Entries");
    fHistLeptonicTop_pt_mu_plus = new TH1F("h_treco_pt_mu_plus",
				       "t-Quark Transverse Momentum (#mu^{+} channel)",
				       100, 0, 300);
    fHistLeptonicTop_pt_mu_plus->SetXTitle("P_{t} (GeV)");
    fHistLeptonicTop_pt_mu_plus->SetYTitle("Number of Entries");
    fHistLeptonicTop_eta_mu_plus = new TH1F("h_treco_eta_mu_plus",
					"t-Quark Pseudo-Rapidity (#mu^{+} channel)",
					50, -5.0, 5.0);
    fHistLeptonicTop_eta_mu_plus->SetXTitle("#eta");
    fHistLeptonicTop_eta_mu_plus->SetYTitle("Number of Entries");
    fHistLeptonicTop_rap_mu_plus = new TH1F("h_treco_rap_mu_plus",
					"Top-Quark Rapidity (#mu^{+} channel)",
					50, -5.0, 5.0);
    fHistLeptonicTop_rap_mu_plus->SetXTitle("y (rapidity)");
    fHistLeptonicTop_rap_mu_plus->SetYTitle("Number of Entries");
    gDirectory->cd("..");
    
    gDirectory->cd("..");
    gDirectory->mkdir("tbar", "#bar{t}-quark reconstruction");
    gDirectory->cd("tbar");
    
    // All negative sign leptons
    fHistLeptonicTop_Mt_minus = new TH1F("h_treco_Mt_minus",
				     "#bar{t}-Quark Transverse Invariant Mass (all negative sign leptons)",
				     60, 0, 210);
    fHistLeptonicTop_Mt_minus->SetXTitle("M_{t,top} (GeV)");
    fHistLeptonicTop_Mt_minus->SetYTitle("Number of Entries");
    fHistLeptonicTop_M_minus = new TH1F("h_treco_M_minus",
				     "#bar{t}-Quark Invariant Mass (all negative sign leptons)",
				     120, 165, 185);
    fHistLeptonicTop_M_minus->SetXTitle("M_{top} (GeV)");
    fHistLeptonicTop_M_minus->SetYTitle("Number of Entries");
    fHistLeptonicTop_phi_minus = new TH1F("h_treco_phi_minus",
				      "#bar{t}-Quark Azimuth Angle (all negative sign leptons)",
				      64, -3.2, 3.2);
    fHistLeptonicTop_phi_minus->SetXTitle("#phi (rad)");
    fHistLeptonicTop_phi_minus->SetYTitle("Number of Entries");
    fHistLeptonicTop_pt_minus = new TH1F("h_treco_pt_minus",
				     "#bar{t}-Quark Transverse Momentum (all negative sign leptons)",
				     100, 0, 300);
    fHistLeptonicTop_pt_minus->SetXTitle("P_{t} (GeV)");
    fHistLeptonicTop_pt_minus->SetYTitle("Number of Entries");
    fHistLeptonicTop_eta_minus = new TH1F("h_treco_eta_minus",
				      "t-Quark Pseudo-Rapidity (all negative sign leptons)",
				      50, -5.0, 5.0);
    fHistLeptonicTop_eta_minus->SetXTitle("#eta");
    fHistLeptonicTop_eta_minus->SetYTitle("Number of Entries");
    fHistLeptonicTop_rap_minus = new TH1F("h_treco_rap_minus",
				      "t-Quark Rapidity (all negative sign leptons)",
				      50, -5.0, 5.0);
    fHistLeptonicTop_rap_minus->SetXTitle("y (rapidity)");
    fHistLeptonicTop_rap_minus->SetYTitle("Number of Entries");

    // Electron e- channel
    gDirectory->mkdir("Electron", "Electron channel");
    gDirectory->cd("Electron");
    fHistLeptonicTop_Mt_e_minus = new TH1F("h_treco_Mt_e_minus",
				       "#bar{t}-Quark Transverse Invariant Mass (e^{-} channel)",
				       60, 0, 210);
    fHistLeptonicTop_Mt_e_minus->SetXTitle("M_{t,top} (GeV)");
    fHistLeptonicTop_Mt_e_minus->SetYTitle("Number of Entries");
    fHistLeptonicTop_M_e_minus = new TH1F("h_treco_M_e_minus",
				       "#bar{t}-Quark Invariant Mass (e^{-} channel)",
				       120, 165, 185);
    fHistLeptonicTop_M_e_minus->SetXTitle("M_{top} (GeV)");
    fHistLeptonicTop_M_e_minus->SetYTitle("Number of Entries");
    fHistLeptonicTop_phi_e_minus = new TH1F("h_treco_phi_e_minus",
					"#bar{t}-Quark Azimuth Angle (e^{-} channel)",
					64, -3.2, 3.2);
    fHistLeptonicTop_phi_e_minus->SetXTitle("#phi (rad)");
    fHistLeptonicTop_phi_e_minus->SetYTitle("Number of Entries");
    fHistLeptonicTop_pt_e_minus = new TH1F("h_treco_pt_e_minus",
				       "#bar{t}-Quark Transverse Momentum (e^{-} channel)",
				       100, 0, 300);
    fHistLeptonicTop_pt_e_minus->SetXTitle("P_{t} (GeV)");
    fHistLeptonicTop_pt_e_minus->SetYTitle("Number of Entries");
    fHistLeptonicTop_eta_e_minus = new TH1F("h_treco_eta_e_minus",
					"#bar{t}-Quark Pseudo-Rapidity (e^{-} channel)",
					50, -5.0, 5.0);
    fHistLeptonicTop_eta_e_minus->SetXTitle("#eta");
    fHistLeptonicTop_eta_e_minus->SetYTitle("Number of Entries");
    fHistLeptonicTop_rap_e_minus = new TH1F("h_treco_rap_e_minus",
					"#bar{t}-Quark Rapidity (e^{-} channel)",
					50, -5.0, 5.0);
    fHistLeptonicTop_rap_e_minus->SetXTitle("y (rapidity)");
    fHistLeptonicTop_rap_e_minus->SetYTitle("Number of Entries");
    gDirectory->cd("..");

    // Muon mu- channel
    gDirectory->mkdir("Muon", "Muon channel");
    gDirectory->cd("Muon");
    fHistLeptonicTop_Mt_mu_minus = new TH1F("h_treco_Mt_mu_minus",
					"#bar{t}-Quark Transverse Invariant Mass (#mu^{-} channel)",
					60, 0, 210);
    fHistLeptonicTop_Mt_mu_minus->SetXTitle("M_{t,top} (GeV)");
    fHistLeptonicTop_Mt_mu_minus->SetYTitle("Number of Entries");
    fHistLeptonicTop_M_mu_minus = new TH1F("h_treco_M_mu_minus",
					"#bar{t}-Quark Invariant Mass (#mu^{-} channel)",
					120, 165, 185);
    fHistLeptonicTop_M_mu_minus->SetXTitle("M_{top} (GeV)");
    fHistLeptonicTop_M_mu_minus->SetYTitle("Number of Entries");
    fHistLeptonicTop_phi_mu_minus = new TH1F("h_treco_phi_mu_minus",
					 "#bar{t}-Quark Azimuth Angle (#mu^{-} channel)",
					 64, -3.2, 3.2);
    fHistLeptonicTop_phi_mu_minus->SetXTitle("#phi (rad)");
    fHistLeptonicTop_phi_mu_minus->SetYTitle("Number of Entries");
    fHistLeptonicTop_pt_mu_minus = new TH1F("h_treco_pt_mu_minus",
					"#bar{t}-Quark Transverse Momentum (#mu^{-} channel)",
					100, 0, 300);
    fHistLeptonicTop_pt_mu_minus->SetXTitle("P_{t} (GeV)");
    fHistLeptonicTop_pt_mu_minus->SetYTitle("Number of Entries");
    fHistLeptonicTop_eta_mu_minus = new TH1F("h_treco_eta_mu_minus",
					 "#bar{t}-Quark Pseudo-Rapidity (#mu^{-} channel)",
					 50, -5.0, 5.0);
    fHistLeptonicTop_eta_mu_minus->SetXTitle("#eta");
    fHistLeptonicTop_eta_mu_minus->SetYTitle("Number of Entries");
    fHistLeptonicTop_rap_mu_minus = new TH1F("h_treco_rap_mu_minus",
					 "#bar{t}-Quark Rapidity (#mu^{-} channel)",
					 50, -5.0, 5.0);
    fHistLeptonicTop_rap_mu_minus->SetXTitle("y (rapidity)");
    fHistLeptonicTop_rap_mu_minus->SetYTitle("Number of Entries");
    gDirectory->cd("..");
    
    gDirectory->cd("../..");
    
    // ========================
    // Neutrino reconstruction
    // ========================
    gDirectory->mkdir("Neutrino-reco", "Neutrino reconstruction");
    gDirectory->cd("Neutrino-reco");

    gDirectory->mkdir("Both", "#nu/#bar{#nu} reconstruction");
    gDirectory->cd("Both");

    // All leptons
    fHistLeptonicTopNuReco_phi = new TH1F("h_nureco_phi", "#nu Azimuth Angle (all leptons)",
			       64, -3.2, 3.2);
    fHistLeptonicTopNuReco_phi->SetXTitle("#phi (rad)");
    fHistLeptonicTopNuReco_phi->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_pt = new TH1F("h_nureco_pt", "#nu Transverse Momentum (all leptons)",
			      100, 0, 300);
    fHistLeptonicTopNuReco_pt->SetXTitle("P_{t} (GeV)");
    fHistLeptonicTopNuReco_pt->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_eta = new TH1F("h_nureco_eta", "#nu Pseudo-Rapidity (all leptons)",
			       50, -5.0, 5.0);
    fHistLeptonicTopNuReco_eta->SetXTitle("#eta");
    fHistLeptonicTopNuReco_eta->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_E = new TH1F("h_nureco_E", "#nu Energy (all leptons)",
				     75, 0., 300);
    fHistLeptonicTopNuReco_E->SetXTitle("E (GeV)");
    fHistLeptonicTopNuReco_E->SetYTitle("Number of Entries");
    
    // Electron channel
    gDirectory->mkdir("Electron", "Electron channel");
    gDirectory->cd("Electron");
    fHistLeptonicTopNuReco_phi_e = new TH1F("h_nureco_phi_e", "#nu Azimuth Angle (e channel)",
				 64, -3.2, 3.2);
    fHistLeptonicTopNuReco_phi_e->SetXTitle("#phi (rad)");
    fHistLeptonicTopNuReco_phi_e->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_pt_e = new TH1F("h_nureco_pt_e", "#nu Transverse Momentum (e channel)",
				100, 0, 300);
    fHistLeptonicTopNuReco_pt_e->SetXTitle("P_{t} (GeV)");
    fHistLeptonicTopNuReco_pt_e->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_eta_e = new TH1F("h_nureco_eta_e", "#nu Pseudo-Rapidity (e channel)",
				 50, -5.0, 5.0);
    fHistLeptonicTopNuReco_eta_e->SetXTitle("#eta");
    fHistLeptonicTopNuReco_eta_e->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_E_e = new TH1F("h_nureco_E_e", "#nu Energy (e channel)",
				     75, 0., 300);
    fHistLeptonicTopNuReco_E_e->SetXTitle("E (GeV)");
    fHistLeptonicTopNuReco_E_e->SetYTitle("Number of Entries");
    gDirectory->cd("..");
    
    // Muon channel
    gDirectory->mkdir("Muon", "Muon channel");
    gDirectory->cd("Muon");
    fHistLeptonicTopNuReco_phi_mu = new TH1F("h_nureco_phi_mu", "#nu Azimuth Angle (#mu channel)",
				  64, -3.2, 3.2);
    fHistLeptonicTopNuReco_phi_mu->SetXTitle("#phi (rad)");
    fHistLeptonicTopNuReco_phi_mu->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_pt_mu = new TH1F("h_nureco_pt_mu", "#nu Transverse Momentum (#mu channel)",
				 100, 0, 300);
    fHistLeptonicTopNuReco_pt_mu->SetXTitle("P_{t} (GeV)");
    fHistLeptonicTopNuReco_pt_mu->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_eta_mu = new TH1F("h_nureco_eta_mu", "#nu Pseudo-Rapidity (#mu channel)",
				  50, -5.0, 5.0);
    fHistLeptonicTopNuReco_eta_mu->SetXTitle("#eta");
    fHistLeptonicTopNuReco_eta_mu->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_E_mu = new TH1F("h_nureco_E_mu", "#nu Energy (#mu channel)",
				     75, 0., 300);
    fHistLeptonicTopNuReco_E_mu->SetXTitle("E (GeV)");
    fHistLeptonicTopNuReco_E_mu->SetYTitle("Number of Entries");
    gDirectory->cd("..");
   
    
    gDirectory->cd("..");
    gDirectory->mkdir("neutrino", "neutrino reconstruction");
    gDirectory->cd("neutrino");

    // All positive sign leptons
    fHistLeptonicTopNuReco_phi_plus = new TH1F("h_nureco_phi_plus",
				    "#nu Azimuth Angle (all positive sign leptons)",
				    128, -3.2, 3.2);
    fHistLeptonicTopNuReco_phi_plus->SetXTitle("#phi (rad)");
    fHistLeptonicTopNuReco_phi_plus->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_pt_plus = new TH1F("h_nureco_pt_plus",
				   "#nu Transverse Momentum (all positive sign leptons)",
				   100, 0, 300);
    fHistLeptonicTopNuReco_pt_plus->SetXTitle("P_{t} (GeV)");
    fHistLeptonicTopNuReco_pt_plus->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_eta_plus = new TH1F("h_nureco_eta_plus",
				    "#nu Pseudo-Rapidity (all positive sign leptons)",
				    50, -5.0, 5.0);
    fHistLeptonicTopNuReco_eta_plus->SetXTitle("#eta");
    fHistLeptonicTopNuReco_eta_plus->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_E_plus = new TH1F("h_nureco_E_plus","#nu Energy (all positive sign leptons)",
				     75, 0., 300);
    fHistLeptonicTopNuReco_E_plus->SetXTitle("E (GeV)");
    fHistLeptonicTopNuReco_E_plus->SetYTitle("Number of Entries");
    

    // Electron e+ channel
    gDirectory->mkdir("Electron", "Electron channel");
    gDirectory->cd("Electron");
    fHistLeptonicTopNuReco_phi_e_plus = new TH1F("h_nureco_phi_e_plus",
				      "#nu Azimuth Angle (e^{+} channel)",
				      64, -3.2, 3.2);
    fHistLeptonicTopNuReco_phi_e_plus->SetXTitle("#phi (rad)");
    fHistLeptonicTopNuReco_phi_e_plus->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_pt_e_plus = new TH1F("h_nureco_pt_e_plus", "#nu Transverse Momentum (e^{+} channel)",
				     100, 0, 300);
    fHistLeptonicTopNuReco_pt_e_plus->SetXTitle("P_{t} (GeV)");
    fHistLeptonicTopNuReco_pt_e_plus->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_eta_e_plus = new TH1F("h_nureco_eta_e_plus", "#nu Pseudo-Rapidity (e^{+} channel)",
				      50, -5.0, 5.0);
    fHistLeptonicTopNuReco_eta_e_plus->SetXTitle("#eta");
    fHistLeptonicTopNuReco_eta_e_plus->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_E_e_plus = new TH1F("h_nureco_E_e_plus", "#nu Energy (e^{+} channel)",
				     75, 0., 300);
    fHistLeptonicTopNuReco_E_e_plus->SetXTitle("E (GeV)");
    fHistLeptonicTopNuReco_E_e_plus->SetYTitle("Number of Entries");
    gDirectory->cd("..");
    
    // Muon mu+ channel
    gDirectory->mkdir("Muon", "Muon channel");
    gDirectory->cd("Muon");
    fHistLeptonicTopNuReco_phi_mu_plus = new TH1F("h_nureco_phi_mu_plus", "#nu Azimuth Angle (#mu^{+} channel)",
				       64, -3.2, 3.2);
    fHistLeptonicTopNuReco_phi_mu_plus->SetXTitle("#phi (rad)");
    fHistLeptonicTopNuReco_phi_mu_plus->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_pt_mu_plus = new TH1F("h_nureco_pt_mu_plus", "#nu Transverse Momentum (#mu^{+} channel)",
				      100, 0, 300);
    fHistLeptonicTopNuReco_pt_mu_plus->SetXTitle("P_{t} (GeV)");
    fHistLeptonicTopNuReco_pt_mu_plus->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_eta_mu_plus = new TH1F("h_nureco_eta_mu_plus", "#nu Pseudo-Rapidity (#mu^{+} channel)",
				       50, -5.0, 5.0);
    fHistLeptonicTopNuReco_eta_mu_plus->SetXTitle("#eta");
    fHistLeptonicTopNuReco_eta_mu_plus->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_E_mu_plus = new TH1F("h_nureco_E_mu_plus", "#nu Energy (#mu^{+} channel)",
				     75, 0, 300);
    fHistLeptonicTopNuReco_E_mu_plus->SetXTitle("E (GeV)");
    fHistLeptonicTopNuReco_E_mu_plus->SetYTitle("Number of Entries");
    gDirectory->cd("..");
    
    gDirectory->cd("..");
    gDirectory->mkdir("anti-neutrino", "#bar{#nu} reconstruction");
    gDirectory->cd("anti-neutrino");

    // All negative sign leptons
    fHistLeptonicTopNuReco_phi_minus = new TH1F("h_nureco_phi_minus",
				     "#bar{#nu} Azimuth Angle (all negative sign leptons)",
				     64, -3.2, 3.2);
    fHistLeptonicTopNuReco_phi_minus->SetXTitle("#phi (rad)");
    fHistLeptonicTopNuReco_phi_minus->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_pt_minus = new TH1F("h_nureco_pt_minus",
				    "#bar{#nu} Transverse Momentum (all negative sign leptons)",
				    100, 0, 300);
    fHistLeptonicTopNuReco_pt_minus->SetXTitle("P_{t} (GeV)");
    fHistLeptonicTopNuReco_pt_minus->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_eta_minus = new TH1F("h_nureco_eta_minus",
				     "#bar{#nu} Pseudo-Rapidity (all negative sign leptons)",
				     50, -5.0, 5.0);
    fHistLeptonicTopNuReco_eta_minus->SetXTitle("#eta");
    fHistLeptonicTopNuReco_eta_minus->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_E_minus = new TH1F("h_nureco_E_minus", "#bar{#nu} Energy (all negative sign leptons)",
				     75, 0, 300);
    fHistLeptonicTopNuReco_E_minus->SetXTitle("E (GeV)");
    fHistLeptonicTopNuReco_E_minus->SetYTitle("Number of Entries");

    // Electron e- channel
    gDirectory->mkdir("Electron", "Electron channel");
    gDirectory->cd("Electron");
    fHistLeptonicTopNuReco_phi_e_minus = new TH1F("h_nureco_phi_e_minus",
				       "#bar{#nu} Azimuth Angle (e^{-} channel)",
				       64, -3.2, 3.2);
    fHistLeptonicTopNuReco_phi_e_minus->SetXTitle("#phi (rad)");
    fHistLeptonicTopNuReco_phi_e_minus->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_pt_e_minus = new TH1F("h_nureco_pt_e_minus",
				      "#bar{#nu} Transverse Momentum (e^{-} channel)",
				      75, 0, 300);
    fHistLeptonicTopNuReco_pt_e_minus->SetXTitle("P_{t} (GeV)");
    fHistLeptonicTopNuReco_pt_e_minus->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_eta_e_minus = new TH1F("h_nureco_eta_e_minus",
				       "#bar{#nu} Pseudo-Rapidity (e^{-} channel)",
				       50, -5.0, 5.0);
    fHistLeptonicTopNuReco_eta_e_minus->SetXTitle("#eta");
    fHistLeptonicTopNuReco_eta_e_minus->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_E_e_minus = new TH1F("h_nureco_E_e_minus", "#bar{#nu} Energy (e^{-} channel)",
				     75, 0, 300);
    fHistLeptonicTopNuReco_E_e_minus->SetXTitle("E (GeV)");
    fHistLeptonicTopNuReco_E_e_minus->SetYTitle("Number of Entries");
    gDirectory->cd("..");
    
    // Muon mu- channel
    gDirectory->mkdir("Muon", "Muon channel");
    gDirectory->cd("Muon");
    fHistLeptonicTopNuReco_phi_mu_minus = new TH1F("h_nureco_phi_mu_minus",
					"#bar{#nu} Azimuth Angle (#mu^{-} channel)",
					64, -3.2, 3.2);
    fHistLeptonicTopNuReco_phi_mu_minus->SetXTitle("#phi (rad)");
    fHistLeptonicTopNuReco_phi_mu_minus->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_pt_mu_minus = new TH1F("h_nureco_pt_mu_minus",
				       "#bar{#nu} Transverse Momentum (#mu^{-} channel)",
				       75, 0, 300);
    fHistLeptonicTopNuReco_pt_mu_minus->SetXTitle("P_{t} (GeV)");
    fHistLeptonicTopNuReco_pt_mu_minus->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_eta_mu_minus = new TH1F("h_nureco_eta_mu_minus",
					"#bar{#nu} Pseudo-Rapidity (#mu^{-} channel)",
					50, -5.0, 5.0);
    fHistLeptonicTopNuReco_eta_mu_minus->SetXTitle("#eta");
    fHistLeptonicTopNuReco_eta_mu_minus->SetYTitle("Number of Entries");
    fHistLeptonicTopNuReco_E_mu_minus = new TH1F("h_nureco_E_mu_minus","#bar{#nu} Energy (#mu^{-} channel)",
						 75, 0, 300);
    fHistLeptonicTopNuReco_E_mu_minus->SetXTitle("E (GeV)");
    fHistLeptonicTopNuReco_E_mu_minus->SetYTitle("Number of Entries");
    gDirectory->cd("..");

    gDirectory->cd("../..");
    gDirectory->mkdir("Associated_HadronicW-reco", "Associated-W reconstruction");
    gDirectory->cd("Associated_HadronicW-reco");
    gDirectory->mkdir("both", "electron- and muon-channel");
    gDirectory->cd("both");
    // electron and muon channel
    fHist_W_hadr_notop_reco_Mt = new TH1F("h_W_hadr_notop_reco_Mt",
				  "associated hadronic W Invariant Transverse Mass, all leptons",
				  65, 0, 130);
    fHist_W_hadr_notop_reco_Mt->SetXTitle("M_{t,W} (GeV)");
    fHist_W_hadr_notop_reco_Mt->SetYTitle("Number of Entries");
    fHist_W_hadr_notop_reco_M = new TH1F("h_W_hadr_notop_reco_M",
				  "associated hadronic W Invariant Mass, all leptons",
				  200, 0, 130);
    fHist_W_hadr_notop_reco_M->SetXTitle("M_{W} (GeV)");
    fHist_W_hadr_notop_reco_M->SetYTitle("Number of Entries");
    fHist_W_hadr_notop_reco_pt = new TH1F("h_W_hadr_notop_reco_pt",
				  "associated hadronic W p_{T}, all leptons",
				  100, 0, 300);
    fHist_W_hadr_notop_reco_pt->SetXTitle("p_{T} (GeV)");
    fHist_W_hadr_notop_reco_pt->SetYTitle("Number of Entries");
    fHist_W_hadr_notop_reco_phi = new TH1F("h_W_hadr_notop_reco_phi",
				  "associated hadronic W #varphi, all leptons",
				  64, -3.2, 3.2);
    fHist_W_hadr_notop_reco_phi->SetXTitle("#varphi");
    fHist_W_hadr_notop_reco_phi->SetYTitle("Number of Entries");
    fHist_W_hadr_notop_reco_rap = new TH1F("h_W_hadr_notop_reco_y",
				  "associated hadronic W rapidity, all leptons",
				  50, -5., 5.);
    fHist_W_hadr_notop_reco_rap->SetXTitle("y");
    fHist_W_hadr_notop_reco_rap->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");

    // electron channel
    gDirectory->mkdir("Electron", "Electron-channel");
    gDirectory->cd("Electron");
    fHist_W_hadr_notop_reco_Mt_e = new TH1F("h_W_hadr_notop_reco_Mt_e",
				  "associated hadronic W Invariant Transverse Mass, e-channel",
				  75, 0, 130);
    fHist_W_hadr_notop_reco_Mt_e->SetXTitle("M_{t,W} (GeV)");
    fHist_W_hadr_notop_reco_Mt_e->SetYTitle("Number of Entries");
    fHist_W_hadr_notop_reco_M_e = new TH1F("h_W_hadr_notop_reco_M_e",
				  "associated hadronic W Invariant Mass, e-channel",
				  200, 0, 130);
    fHist_W_hadr_notop_reco_M_e->SetXTitle("M_{W} (GeV)");
    fHist_W_hadr_notop_reco_M_e->SetYTitle("Number of Entries");
    fHist_W_hadr_notop_reco_pt_e = new TH1F("h_W_hadr_notop_reco_pt_e",
				  "associated hadronic W p_{T}, e-channel",
				  100, 0, 300);
    fHist_W_hadr_notop_reco_pt_e->SetXTitle("p_{T} (GeV)");
    fHist_W_hadr_notop_reco_pt_e->SetYTitle("Number of Entries");
    fHist_W_hadr_notop_reco_phi_e = new TH1F("h_W_hadr_notop_reco_phi_e",
				  "associated hadronic W #varphi, e-channel",
				  64, -3.2, 3.2);
    fHist_W_hadr_notop_reco_phi_e->SetXTitle("#varphi");
    fHist_W_hadr_notop_reco_phi_e->SetYTitle("Number of Entries");
    fHist_W_hadr_notop_reco_rap_e = new TH1F("h_W_hadr_notop_reco_y_e",
				  "associated hadronic W rapidity, e-channel",
				  50, -5., 5.);
    fHist_W_hadr_notop_reco_rap_e->SetXTitle("y");
    fHist_W_hadr_notop_reco_rap_e->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // muon channel
    gDirectory->mkdir("Muon", "muon-channel");
    gDirectory->cd("Muon");
    fHist_W_hadr_notop_reco_Mt_mu = new TH1F("h_W_hadr_notop_reco_Mt_mu",
				  "associated hadronic W Invariant Transverse Mass, e-channel",
				  65, 0, 130);
    fHist_W_hadr_notop_reco_Mt_mu->SetXTitle("M_{t,W} (GeV)");
    fHist_W_hadr_notop_reco_Mt_mu->SetYTitle("Number of Entries");
    fHist_W_hadr_notop_reco_M_mu = new TH1F("h_W_hadr_notop_reco_M_mu",
				  "associated hadronic W Invariant Mass, e-channel",
				  200, 0, 130);
    fHist_W_hadr_notop_reco_M_mu->SetXTitle("M_{W} (GeV)");
    fHist_W_hadr_notop_reco_M_mu->SetYTitle("Number of Entries");
    fHist_W_hadr_notop_reco_pt_mu = new TH1F("h_W_hadr_notop_reco_pt_mu",
				  "associated hadronic W p_{T}, e-channel",
				  100, 0, 300);
    fHist_W_hadr_notop_reco_pt_mu->SetXTitle("p_{T} (GeV)");
    fHist_W_hadr_notop_reco_pt_mu->SetYTitle("Number of Entries");
    fHist_W_hadr_notop_reco_phi_mu = new TH1F("h_W_hadr_notop_reco_phi_mu",
				  "associated hadronic W #varphi, e-channel",
				  64, -3.2, 3.2);
    fHist_W_hadr_notop_reco_phi_mu->SetXTitle("#varphi");
    fHist_W_hadr_notop_reco_phi_mu->SetYTitle("Number of Entries");
    fHist_W_hadr_notop_reco_rap_mu = new TH1F("h_W_hadr_notop_reco_y_mu",
				  "associated hadronic W rapidity, e-channel",
				  50, -5., 5.);
    fHist_W_hadr_notop_reco_rap_mu->SetXTitle("y");
    fHist_W_hadr_notop_reco_rap_mu->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // ================================
    // Chi2 of KinFitter reconstruction
    // ================================
    gDirectory->cd("..");
    gDirectory->mkdir("KinFitter","Chi-square and chi-square probability distributions");
    gDirectory->cd("KinFitter");
    // PULLS
    gDirectory->mkdir("Pulls","Pull Distributions");
    gDirectory->cd("Pulls");

    gDirectory->mkdir("Electron","Electron");
    gDirectory->cd("Electron");

    gDirectory->mkdir("Residuals","Residuals");
    gDirectory->cd("Residuals");
    fHistResidualElecPt = new TH1F("h_ResidualElecPt","Residual of Electron Pt",
				 1000,-0.5,0.5);
    fHistResidualElecPt->SetXTitle("#frac{#Delta p_{T}}{#sigma_{p_{T}}}");
    fHistResidualElecPt->SetYTitle("Number of Entries");
    fHistResidualElecEta = new TH1F("h_ResidualElecEta","Residual of Electron #eta",
				  1000,-0.5,0.5);
    fHistResidualElecEta->SetXTitle("#frac{#Delta#eta}{#sigma_{#eta}}");
    fHistResidualElecEta->SetYTitle("Number of Entries");
    fHistResidualElecPhi = new TH1F("h_ResidualElecPhi","Residual of Electron #varphi",
				  1000,-0.5,0.5);
    fHistResidualElecPhi->SetXTitle("#frac{#Delta#varphi}{#sigma_{#varphi}}");
    fHistResidualElecPhi->SetYTitle("Number of Entries");
    gDirectory->cd("..");

    fHistPullElecPt = new TH1F("h_PullElecPt","Pull of Electron Pt",
				  31,-8.,8.);
    fHistPullElecPt->SetXTitle("#frac{#Delta p_{T}}{#sigma_{p_{T}}}");
    fHistPullElecPt->SetYTitle("Number of Entries");
    fHistPullElecEta = new TH1F("h_PullElecEta","Pull of Electron #eta",
				  31,-8.,8.);
    fHistPullElecEta->SetXTitle("#frac{#Delta#eta}{#sigma_{#eta}}");
    fHistPullElecEta->SetYTitle("Number of Entries");
    fHistPullElecPhi = new TH1F("h_PullElecPhi","Pull of Electron #varphi",
				  31,-8.,8.);
    fHistPullElecPhi->SetXTitle("#frac{#Delta#varphi}{#sigma_{#varphi}}");
    fHistPullElecPhi->SetYTitle("Number of Entries");
    gDirectory->cd("..");



    gDirectory->mkdir("Muon","Muon");
    gDirectory->cd("Muon");

    gDirectory->mkdir("Residuals","Residuals");
    gDirectory->cd("Residuals");
    fHistResidualMuonPt = new TH1F("h_ResidualMuonPt","Residual of Electron Pt",
				 1000,-0.5,0.5);
    fHistResidualMuonPt->SetXTitle("#frac{#Delta p_{T}}{#sigma_{p_{T}}}");
    fHistResidualMuonPt->SetYTitle("Number of Entries");
    fHistResidualMuonEta = new TH1F("h_ResidualMuonEta","Residual of Electron #eta",
				  1000,-0.5,0.5);
    fHistResidualMuonEta->SetXTitle("#frac{#Delta#eta}{#sigma_{#eta}}");
    fHistResidualMuonEta->SetYTitle("Number of Entries");
    fHistResidualMuonPhi = new TH1F("h_ResidualMuonPhi","Residual of Electron #varphi",
				  1000,-0.5,0.5);
    fHistResidualMuonPhi->SetXTitle("#frac{#Delta#varphi}{#sigma_{#varphi}}");
    fHistResidualMuonPhi->SetYTitle("Number of Entries");
    gDirectory->cd("..");

    fHistPullMuonPt = new TH1F("h_PullMuonPt","Pull of Electron Pt",
				  31,-8.,8.);
    fHistPullMuonPt->SetXTitle("#frac{#Delta p_{T}}{#sigma_{p_{T}}}");
    fHistPullMuonPt->SetYTitle("Number of Entries");
    fHistPullMuonEta = new TH1F("h_PullMuonEta","Pull of Electron #eta",
				  31,-8.,8.);
    fHistPullMuonEta->SetXTitle("#frac{#Delta#eta}{#sigma_{#eta}}");
    fHistPullMuonEta->SetYTitle("Number of Entries");
    fHistPullMuonPhi = new TH1F("h_PullMuonPhi","Pull of Electron #varphi",
				  31,-8.,8.);
    fHistPullMuonPhi->SetXTitle("#frac{#Delta#varphi}{#sigma_{#varphi}}");
    fHistPullMuonPhi->SetYTitle("Number of Entries");
    gDirectory->cd("..");



    
    gDirectory->mkdir("EtMiss","EtMiss");
    gDirectory->cd("EtMiss");

    fHistPullEtMissMag = new TH1F("h_PullEtMag","Pull of E_{T}^{miss}",
				  31,-8.,8.);
    fHistPullEtMissMag->SetXTitle("#frac{#Delta E_{T}^{miss}}{#sigma_{E_{T}^{miss}}}");
    fHistPullEtMissMag->SetYTitle("Number of Entries");
    fHistPullEtMissPhi = new TH1F("h_PullEtMPhi","Pull #varphi_{E_{T}^{miss}}",
				  31,-8.,8.);
    fHistPullEtMissPhi->SetXTitle("#frac{#Delta#varphi_{E_{T}^{miss}}}{#sigma_{#varphi_{E_{T}^{miss}}}}");
    fHistPullEtMissPhi->SetYTitle("Number of Entries");
    gDirectory->cd("..");
    
    gDirectory->mkdir("BJet","BJet");
    gDirectory->cd("BJet");
    
    fHistPullBJetPt = new TH1F("h_PullBJetPt","Pull of BJet Pt",
				  31,-8.,8.);
    fHistPullBJetPt->SetXTitle("#frac{#Delta p_{T}}{#sigma_{p_{T}}}");
    fHistPullBJetPt->SetYTitle("Number of Entries");
    fHistPullBJetEta = new TH1F("h_PullBJetEta","Pull of BJet #eta",
				  31,-8.,8.);
    fHistPullBJetEta->SetXTitle("#frac{#Delta#eta}{#sigma_{#eta}}");
    fHistPullBJetEta->SetYTitle("Number of Entries");
    fHistPullBJetPhi = new TH1F("h_PullBJetPhi","Pull of BJet #varphi",
				  31,-8.,8.);
    fHistPullBJetPhi->SetXTitle("#frac{#Delta#varphi}{#sigma_{#varphi}}");
    fHistPullBJetPhi->SetYTitle("Number of Entries");

    gDirectory->cd("..");

    gDirectory->mkdir("Jet1","Jet1");
    gDirectory->cd("Jet1");
    gDirectory->mkdir("Residuals","Residuals");
    gDirectory->cd("Residuals");
    fHistResidualJet1Pt = new TH1F("h_ResidualJet1Pt","Residual of Jet1 Pt",
				  1000,-05.,05.);
    fHistResidualJet1Pt->SetXTitle("#frac{#Delta p_{T}}{#sigma_{p_{T}}}");
    fHistResidualJet1Pt->SetYTitle("Number of Entries");
    fHistResidualJet1Eta = new TH1F("h_ResidualJet1Eta","Residual of Jet1 #eta",
				  1000,-0.5,0.5);
    fHistResidualJet1Eta->SetXTitle("#frac{#Delta#eta}{#sigma_{#eta}}");
    fHistResidualJet1Eta->SetYTitle("Number of Entries");
    fHistResidualJet1Phi = new TH1F("h_ResidualJet1Phi","Residual of Jet1 #varphi",
				  1000,-0.5,0.5);
    fHistResidualJet1Phi->SetXTitle("#frac{#Delta#varphi}{#sigma_{#varphi}}");
    fHistResidualJet1Phi->SetYTitle("Number of Entries");
    gDirectory->cd("..");
    fHistPullJet1Pt = new TH1F("h_PullJet1Pt","Pull of Jet1 Pt",
			       31,-8.,8.);
    fHistPullJet1Pt->SetXTitle("#frac{#Delta p_{T}}{#sigma_{p_{T}}}");
    fHistPullJet1Pt->SetYTitle("Number of Entries");
    fHistPullJet1Eta = new TH1F("h_PullJet1Eta","Pull of Jet1 #eta",
				31,-8.,8.);
    fHistPullJet1Eta->SetXTitle("#frac{#Delta#eta}{#sigma_{#eta}}");
    fHistPullJet1Eta->SetYTitle("Number of Entries");
    fHistPullJet1Phi = new TH1F("h_PullJet1Phi","Pull of Jet1 #varphi",
				31,-8.,8.);
    fHistPullJet1Phi->SetXTitle("#frac{#Delta#varphi}{#sigma_{#varphi}}");
    fHistPullJet1Phi->SetYTitle("Number of Entries");
    gDirectory->cd("..");

    gDirectory->mkdir("Jet2","Jet2");
    gDirectory->cd("Jet2");
    gDirectory->mkdir("Residuals","Residuals");
    gDirectory->cd("Residuals");
    fHistResidualJet2Pt = new TH1F("h_ResidualJet2Pt","Residual of Jet2 Pt",
				   31,-8.,8.);
    fHistResidualJet2Pt->SetXTitle("#frac{#Delta p_{T}}{#sigma_{p_{T}}}");
    fHistResidualJet2Pt->SetYTitle("Number of Entries");
    fHistResidualJet2Eta = new TH1F("h_ResidualJet2Eta","Residual of Jet2 #eta",
				    31,-8.,8.);
    fHistResidualJet2Eta->SetXTitle("#frac{#Delta#eta}{#sigma_{#eta}}");
    fHistResidualJet2Eta->SetYTitle("Number of Entries");
    fHistResidualJet2Phi = new TH1F("h_ResidualJet2Phi","Residual of Jet2 #varphi",
				    31,-8.,8.);
    fHistResidualJet2Phi->SetXTitle("#frac{#Delta#varphi}{#sigma_{#varphi}}");
    fHistResidualJet2Phi->SetYTitle("Number of Entries");
    gDirectory->cd("..");
    
    fHistPullJet2Pt = new TH1F("h_PullJet2Pt","Pull of Jet2 Pt",
			       31,-8.,8.);
    fHistPullJet2Pt->SetXTitle("#frac{#Delta p_{T}}{#sigma_{p_{T}}}");
    fHistPullJet2Pt->SetYTitle("Number of Entries");
    fHistPullJet2Eta = new TH1F("h_PullJet2Eta","Pull of Jet2 #eta",
				31,-8.,8.);
    fHistPullJet2Eta->SetXTitle("#frac{#Delta#eta}{#sigma_{#eta}}");
    fHistPullJet2Eta->SetYTitle("Number of Entries");
    fHistPullJet2Phi = new TH1F("h_PullJet2Phi","Pull of Jet2 #varphi",
				31,-8.,8.);
    fHistPullJet2Phi->SetXTitle("#frac{#Delta#varphi}{#sigma_{#varphi}}");
    fHistPullJet2Phi->SetYTitle("Number of Entries");

    gDirectory->cd("../..");

    // both W decays
    fHistLeptonicTopChi2_both = new TH1F("h_leptchi2_both",
			      "#chi^{2} distribution of sgtop_WtChannel reco (both W decays)",
			      150,0,30);
    fHistLeptonicTopChi2_both->SetXTitle("#chi^{2}");
    fHistLeptonicTopChi2_both->SetYTitle("Number of Entries");

    fHistLeptonicTopChi2Prob_both = new TH1F("h_chi2prob_both",
				  "#chi^{2} prob. distribution of sgtop_WtChannel reco (both W decays)",
					     40,0,1);
    fHistLeptonicTopChi2Prob_both->SetXTitle("P(#chi^{2})");
    fHistLeptonicTopChi2Prob_both->SetYTitle("Number of Entries");

    fHistLeptonicTopChi2NDOF_both = new TH1F("h_leptchi2NDOF_both",
			      "#chi^{2} distribution of sgtop_WtChannel reco (both W decays)",
			      150,0,30);
    fHistLeptonicTopChi2NDOF_both->SetXTitle("#chi^{2}");
    fHistLeptonicTopChi2NDOF_both->SetYTitle("Number of Entries");    
    
    fHistLeptonicTopChi2_both_after = new TH1F("h_chi2_both_after",
			      "#chi^{2} distribution of sgtop_WtChannel reco (both W decays)",
			      150,0,30);
    fHistLeptonicTopChi2_both_after->SetXTitle("#chi^{2}");
    fHistLeptonicTopChi2_both_after->SetYTitle("Number of Entries");
    fHistLeptonicTopChi2Prob_both_after = new TH1F("h_chi2prob_both_after",
				  "#chi^{2} prob. distribution of sgtop_WtChannel reco (both W decays)",
					     40,0,1);
    fHistLeptonicTopChi2Prob_both_after->SetXTitle("P(#chi^{2})");
    fHistLeptonicTopChi2Prob_both_after->SetYTitle("Number of Entries");

    fHistLeptonicTopChi2NDOF_both_after = new TH1F("h_leptchi2NDOF_both_after",
			      "#chi^{2} distribution of sgtop_WtChannel reco (both W decays)",
			      150,0,30);
    fHistLeptonicTopChi2NDOF_both_after->SetXTitle("#chi^{2}");
    fHistLeptonicTopChi2NDOF_both_after->SetYTitle("Number of Entries");    

    
//axis->Set( 3, Chi2ProbBins );
    
    fHistLeptonicTopChi2ProbTTBarVeto = new TH1F("h_chi2prob_TTBarVeto_LeptonicTop",
				  "#chi^{2} prob. distribution of TTBar Veto with Wt reconstruction for leptonic top decay",
					     1000,0,1);
    fHistLeptonicTopChi2ProbTTBarVeto->SetXTitle("P(#chi^{2})");
    fHistLeptonicTopChi2ProbTTBarVeto->SetYTitle("Number of Entries");    
    fHistLeptonicTopChi2ProbWtAndTTBar = new TH2F("h_chi2prob_WtAndTTBar_LeptonicTop",
				  "#chi^{2} prob. distribution of sgtop_WtChannel (leptonic top decay) and t#bar{t}  reco",
				  1000, 0, 1, 1000, 0, 1 );
    fHistLeptonicTopChi2ProbWtAndTTBar->SetXTitle("P(#chi^{2})_{Wt}");
    fHistLeptonicTopChi2ProbWtAndTTBar->SetYTitle("P(#chi^{2})_{t#bar{t}}");
    fHistLeptonicTopChi2ProbWtAndTTBar->SetZTitle("Number of Entries");
    
    // W -> e nu decays
    gDirectory->mkdir("Electron","Electron");
    gDirectory->cd("Electron");
    fHistLeptonicTopChi2_e = new TH1F("h_chi2_e",
			      "#chi^{2} distribution of sgtop_WtChannel reco (e channel)",
			      150,0,30);
    fHistLeptonicTopChi2_e->SetXTitle("#chi^{2}");
    fHistLeptonicTopChi2_e->SetYTitle("Number of Entries");
    
    fHistLeptonicTopChi2Prob_e = new TH1F("h_chi2prob_e",
				  "#chi^{2} prob. distribution of sgtop_WtChannel reco (e channel)",
				 40,0,1);
    fHistLeptonicTopChi2Prob_e->SetXTitle("P(#chi^{2})");
    fHistLeptonicTopChi2Prob_e->SetYTitle("Number of Entries");
    axis = fHistLeptonicTopChi2Prob_e->GetXaxis();
    //axis->Set( 3, Chi2ProbBins );
    gDirectory->cd("..");
    
    // W -> mu nu decays
    gDirectory->mkdir("Muon","Muon");
    gDirectory->cd("Muon");
    fHistLeptonicTopChi2_mu = new TH1F("h_chi2_mu",
			       "#chi^{2} distribution of sgtop_WtChannel reco (#mu channel)",
			       150,0,30);
    fHistLeptonicTopChi2_mu->SetXTitle("#chi^{2}");
    fHistLeptonicTopChi2_mu->SetYTitle("Number of Entries");
    
    fHistLeptonicTopChi2Prob_mu = new TH1F("h_chi2prob_mu",
				   "#chi^{2} prob. distr. of sgtop_WtChannel reco (#mu channel)",
					   40,0,1);
    fHistLeptonicTopChi2Prob_mu->SetXTitle("P(#chi^{2})");
    fHistLeptonicTopChi2Prob_mu->SetYTitle("Number of Entries");
    axis = fHistLeptonicTopChi2Prob_mu->GetXaxis();
    //axis->Set( 3, Chi2ProbBins );
    gDirectory->cd("..");

    //
    // 2D plots: correlation of P(chi2) and M_invar
    //

    // 2D plots: correlation of P(chi2) and M_invar_W
    fHist_LeptonicTopChi2Prob_vs_M_invar_W = new TH2F("h_chi2prob_vs_m_invar_W",
					   "#chi^{2} prob. vs invariant mass of W in sgtop_WtChannel reco",
					   25,0.,1.,
					   80,50.,130.);
    fHist_LeptonicTopChi2Prob_vs_M_invar_W->SetXTitle("P(#chi^{2})");
    fHist_LeptonicTopChi2Prob_vs_M_invar_W->SetYTitle("M_{W} (GeV)");
    fHist_LeptonicTopChi2Prob_vs_M_invar_W->SetZTitle("Number of Entries");
  

    // 2D plots: correlation of P(chi2) and M_invar_top
    fHist_LeptonicTopChi2Prob_vs_M_invar_top = new TH2F("h_chi2prob_vs_m_invar_top",
					   "#chi^{2} prob. vs invariant mass of top in sgtop_WtChannel reco",
					   25,0.,1.,
					     60,150.,210.);
    fHist_LeptonicTopChi2Prob_vs_M_invar_top->SetXTitle("P(#chi^{2})");
    fHist_LeptonicTopChi2Prob_vs_M_invar_top->SetYTitle("M_{top} (GeV)");
    fHist_LeptonicTopChi2Prob_vs_M_invar_top->SetZTitle("Number of Entries");
    
    
//     // All positive sign leptons
//     fHist_W_hadr_notop_reco_Mt_plus = new TH1F("h_W_hadr_notop_reco_Mt_plus",
// 				  "W^{-} Invariant Transverse Mass (all positive sign leptons)",
// 				  200, 0, 130);
//     fHist_W_hadr_notop_reco_Mt_plus->SetXTitle("M_{t,W} (GeV)");
//     fHist_W_hadr_notop_reco_Mt_plus->SetYTitle("Number of Entries");
//     fHist_W_hadr_notop_reco_M_plus = new TH1F("h_W_hadr_notop_reco_M_plus",
// 				  "W^{-} Invariant Mass (all positive sign leptons)",
// 				  200, 40, 120);
//     fHist_W_hadr_notop_reco_M_plus->SetXTitle("M_{W} (GeV)");
//     fHist_W_hadr_notop_reco_M_plus->SetYTitle("Number of Entries");
//     fHist_W_hadr_notop_reco_phi_plus = new TH1F("h_W_hadr_notop_reco_phi_plus",
// 				   "W^{-} Azimuth Angle (all positive sign leptons)",
// 				   128, -3.2, 3.2);
//     fHist_W_hadr_notop_reco_phi_plus->SetXTitle("#phi (rad)");
//     fHist_W_hadr_notop_reco_phi_plus->SetYTitle("Number of Entries");
//     fHist_W_hadr_notop_reco_pt_plus = new TH1F("h_W_hadr_notop_reco_pt_plus",
// 				  "W^{-} Transverse Momentum (all positive sign leptons)",
// 				  300, 0, 300);
//     fHist_W_hadr_notop_reco_pt_plus->SetXTitle("P_{t} (GeV)");
//     fHist_W_hadr_notop_reco_pt_plus->SetYTitle("Number of Entries");
//     fHist_W_hadr_notop_reco_eta_plus = new TH1F("h_W_hadr_notop_reco_eta_plus",
// 				   "W^{-} Pseudo-Rapidity (all positive sign leptons)",
// 				   50, -5.0, 5.0);
//     fHist_W_hadr_notop_reco_eta_plus->SetXTitle("#eta");
//     fHist_W_hadr_notop_reco_eta_plus->SetYTitle("Number of Entries");
//     fHist_W_hadr_notop_reco_rap_plus = new TH1F("h_W_hadr_notop_reco_rap_plus",
// 				   "W^{-} Rapidity (all positive sign leptons)",
// 				   50, -5.0, 5.0);
//     fHist_W_hadr_notop_reco_rap_plus->SetXTitle("y (rapidity)");
//     fHist_W_hadr_notop_reco_rap_plus->SetYTitle("Number of Entries");
//    // All negative sign leptons
//     fHist_W_hadr_notop_reco_Mt_minus = new TH1F("h_W_hadr_notop_reco_Mt_minus",
// 				   "W^{+} Invariant Transverse Mass (all negative sign leptons)",
// 				   200, 0, 130);
//     fHist_W_hadr_notop_reco_Mt_minus->SetXTitle("M_{t,W} (GeV)");
//     fHist_W_hadr_notop_reco_Mt_minus->SetYTitle("Number of Entries");
//     fHist_W_hadr_notop_reco_M_minus = new TH1F("h_W_hadr_notop_reco_M_minus",
// 				   "W^{+} Invariant Mass (all negative sign leptons)",
// 				   200, 40, 120);
//     fHist_W_hadr_notop_reco_M_minus->SetXTitle("M_{W} (GeV)");
//     fHist_W_hadr_notop_reco_M_minus->SetYTitle("Number of Entries");
//     fHist_W_hadr_notop_reco_phi_minus = new TH1F("h_W_hadr_notop_reco_phi_minus",
// 				    "W^{+} Azimuth Angle (all negative sign leptons)",
// 				    128, -3.2, 3.2);
//     fHist_W_hadr_notop_reco_phi_minus->SetXTitle("#phi (rad)");
//     fHist_W_hadr_notop_reco_phi_minus->SetYTitle("Number of Entries");
//     fHist_W_hadr_notop_reco_pt_minus = new TH1F("h_W_hadr_notop_reco_pt_minus",
// 				   "W^{+} Transverse Momentum (all negative sign leptons)",
// 				   300, 0, 300);
//     fHist_W_hadr_notop_reco_pt_minus->SetXTitle("P_{t} (GeV)");
//     fHist_W_hadr_notop_reco_pt_minus->SetYTitle("Number of Entries");
//     fHist_W_hadr_notop_reco_eta_minus = new TH1F("h_W_hadr_notop_reco_eta_minus",
// 				    "W^{+} Pseudo-Rapidity (all negative sign leptons)",
// 				    50, -5.0, 5.0);
//     fHist_W_hadr_notop_reco_eta_minus->SetXTitle("#eta");
//     fHist_W_hadr_notop_reco_eta_minus->SetYTitle("Number of Entries");
//     fHist_W_hadr_notop_reco_rap_minus = new TH1F("h_W_hadr_notop_reco_rap_minus",
// 				    "W^{+} Rapidity (all negative sign leptons)",
// 				    50, -5.0, 5.0);
//     fHist_W_hadr_notop_reco_rap_minus->SetXTitle("y (rapidity)");
//     fHist_W_hadr_notop_reco_rap_minus->SetYTitle("Number of Entries");

    // ======================================
    // Truth Matching for Leptonic Top Decays
    // ======================================
    gDirectory->cd("..");
    gDirectory->mkdir("Truth Matching","Truth Matching for leptonic top decays");
    gDirectory->cd("Truth Matching");

    fHistLeptonicTopRecoTrueChannel = new TH1I("h_TMLeptonicTopChannel",
					  "True Channel in Wt events for reconstruction of leptonic top decay",
					  3, 0, 2);
    fHistLeptonicTopRecoTrueChannel->SetXTitle("true channel");
    fHistLeptonicTopRecoTrueChannel->SetYTitle("# Events");
    fHistLeptonicTopRecoTrueChannel->SetAxisRange( 0., fHistLeptonicTopRecoTrueChannel->GetMaximumStored(), "Y" );
    
    // charged lepton truth matching
    gDirectory->mkdir("ChargedLepton","TruthMathching histograms for electrons and muons");
    gDirectory->cd("ChargedLepton");
    
    // electron
    gDirectory->mkdir("electron","TruthMathching histograms for electrons");
    gDirectory->cd("electron");
    
    // Electron pt
    fHistLeptonicTopElecTruthMatch_pt = new TH1F("h_TMLeptonicTopElec_pt",
					  "Truth Matching of Electron p_{T}",
					  30, -1.5, 1.5);
    fHistLeptonicTopElecTruthMatch_pt->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistLeptonicTopElecTruthMatch_pt->SetYTitle("# Events");
    
    // Electron phi
    fHistLeptonicTopElecTruthMatch_phi = new TH1F("h_TMLeptonicTopElec_phi",
					   "Truth Matching of Electron #varphi",
					   30, -1.5, 1.5);
    fHistLeptonicTopElecTruthMatch_phi->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistLeptonicTopElecTruthMatch_phi->SetYTitle("# Events");

    // Electron eta
    fHistLeptonicTopElecTruthMatch_eta = new TH1F("h_TMLeptonicTopElec_eta",
					   "Truth Matching of Electron #eta",
					   30, -1.5, 1.5);
    fHistLeptonicTopElecTruthMatch_eta->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistLeptonicTopElecTruthMatch_eta->SetYTitle("# Events");
    
    gDirectory->cd("..");

    // muon    
    gDirectory->mkdir("muon","TruthMathching histograms for muons");
    gDirectory->cd("muon");
    
    // Muon pt
    fHistLeptonicTopMuTruthMatch_pt = new TH1F("h_TMLeptonicTopMu_pt",
				      "Truth Matching of Muon p_{T}",
				      30, -1.5, 1.5);
    fHistLeptonicTopMuTruthMatch_pt->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistLeptonicTopMuTruthMatch_pt->SetYTitle("# Events");
    
    // Muon phi
    fHistLeptonicTopMuTruthMatch_phi = new TH1F("h_TMLeptonicTopMu_phi",
				       "Truth Matching of Muon #varphi",
				       30, -1.5, 1.5);
    fHistLeptonicTopMuTruthMatch_phi->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistLeptonicTopMuTruthMatch_phi->SetYTitle("# Events");

    // Muon eta
    fHistLeptonicTopMuTruthMatch_eta = new TH1F("h_TMLeptonicTopMu_eta",
				       "Truth Matching of Muon #eta",
				       30, -1.5, 1.5);
    fHistLeptonicTopMuTruthMatch_eta->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistLeptonicTopMuTruthMatch_eta->SetYTitle("# Events");
    
    gDirectory->cd("../../");
    
    // Neutrino Truth Matching
    gDirectory->mkdir("Neutrino","TruthMathching histograms for Neutrino");
    gDirectory->cd("Neutrino");

    // Neutrino pt
    fHistLeptonicTopNuTruthMatch_pt = new TH1F("h_TMLeptonicTopNu_pt",
				    "Truth Matching of Neutrino p_{T}",
				    30, -1.5, 1.5);
    fHistLeptonicTopNuTruthMatch_pt->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistLeptonicTopNuTruthMatch_pt->SetYTitle("# Events");
    
    // Neutrino phi
    fHistLeptonicTopNuTruthMatch_phi = new TH1F("h_TMLeptonicTopNu_phi",
				     "Truth Matching of Neutrino #varphi",
				     30, -1.5, 1.5);
    fHistLeptonicTopNuTruthMatch_phi->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistLeptonicTopNuTruthMatch_phi->SetYTitle("# Events");

    // Neutrino theta
    fHistLeptonicTopNuTruthMatch_theta = new TH1F("h_TMLeptonicTopNu_theta",
				       "Truth Matching of Neutrino #theta",
				       30, -1.5, 1.5);
    fHistLeptonicTopNuTruthMatch_theta->SetXTitle("#frac{#theta^{rec}-#theta^{MC}}{#theta^{MC}}");
    fHistLeptonicTopNuTruthMatch_theta->SetYTitle("# Events");

    // Neutrino theta
    fHistLeptonicTopNuTruthMatch_eta = new TH1F("h_TMLeptonicTopNu_eta",
				       "Truth Matching of Neutrino #eta",
				       30, -1.5, 1.5);
    fHistLeptonicTopNuTruthMatch_eta->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistLeptonicTopNuTruthMatch_eta->SetYTitle("# Events");
    
    gDirectory->cd("..");
    
    // b-jet truth matching w.r.t. the true b-quark
    gDirectory->mkdir("b-jet","TruthMathching histograms for b-jet");
    gDirectory->cd("b-jet");
    gDirectory->mkdir("b-quark","TruthMathching histograms for b-quark w.r.t. b-quark");
    gDirectory->cd("b-quark");

    // b-jet pt
    fHistLeptonicTopBottomTruthMatch_pt = new TH1F("h_TMLeptonicTopb_pt",
				      "Truth Matching of b-jet p_{T} w.r.t. MC b quark",
				      30, -1.5, 1.5);
    fHistLeptonicTopBottomTruthMatch_pt->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistLeptonicTopBottomTruthMatch_pt->SetYTitle("# Events");
    
    // b-jet phi
    fHistLeptonicTopBottomTruthMatch_phi = new TH1F("h_TMLeptonicTopb_phi",
				       "Truth Matching of b-jet #varphi w.r.t. MC b quark",
				       30, -1.5, 1.5);
    fHistLeptonicTopBottomTruthMatch_phi->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistLeptonicTopBottomTruthMatch_phi->SetYTitle("# Events");

    // b-jet eta
    fHistLeptonicTopBottomTruthMatch_eta = new TH1F("h_TMLeptonicTopb_eta",
				       "Truth Matching of b-jet #eta",
				       30, -1.5, 1.5);
    fHistLeptonicTopBottomTruthMatch_eta->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistLeptonicTopBottomTruthMatch_eta->SetYTitle("# Events");
    
    gDirectory->cd("..");

    gDirectory->mkdir("Hadron Level Jet","TruthMathching histograms w.r.t. hadron level jet");
    gDirectory->cd("Hadron Level Jet");

    // b-jet pt
    fHistLeptonicTopBHJetTruthMatch_pt = new TH1F("h_TMLeptonicTopBHJet_pt",
				      "Truth Matching of b-jet p_{T} w.r.t. Hadron Level jet",
				      30, -1.5, 1.5);
    fHistLeptonicTopBHJetTruthMatch_pt->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistLeptonicTopBHJetTruthMatch_pt->SetYTitle("# Events");
    
    // b-jet phi
    fHistLeptonicTopBHJetTruthMatch_phi = new TH1F("h_TMLeptonicTopBHJet_phi",
				       "Truth Matching of b-jet #varph w.r.t. Hadron Level jeti",
				       30, -1.5, 1.5);
    fHistLeptonicTopBHJetTruthMatch_phi->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistLeptonicTopBHJetTruthMatch_phi->SetYTitle("# Events");
    
    // b-jet eta
    fHistLeptonicTopBHJetTruthMatch_eta = new TH1F("h_TMLeptonicTopBHJet_eta",
				       "Truth Matching of b-jet #eta w.r.t. Hadron Level jet",
				       30, -1.5, 1.5);
    fHistLeptonicTopBHJetTruthMatch_eta->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistLeptonicTopBHJetTruthMatch_eta->SetYTitle("# Events");

    gDirectory->cd("../..");

    // hadronic W truth matching w.r.t. the true quarks form W decay
    gDirectory->mkdir("Hadronic W","TruthMathching histograms for hadronic W");
    gDirectory->cd("Hadronic W");
    gDirectory->mkdir("Quarks","TruthMathching histograms for jets from W decay w.r.t. quarks");
    gDirectory->cd("Quarks");

    // hadronic W quark pt
    fHistLeptonicTopWQuarkTruthMatch_pt = new TH1F("h_TMLeptonicTopWQuark_pt",
				      "Truth Matching of jets p_{T} w.r.t. quarks",
				      30, -1.5, 1.5);
    fHistLeptonicTopWQuarkTruthMatch_pt->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistLeptonicTopWQuarkTruthMatch_pt->SetYTitle("# Events");
    
    // hadronic W quark phi
    fHistLeptonicTopWQuarkTruthMatch_phi = new TH1F("h_TMLeptonicTopWQuark_phi",
				       "Truth Matching of jets #varph w.r.t. quarks",
				       30, -1.5, 1.5);
    fHistLeptonicTopWQuarkTruthMatch_phi->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistLeptonicTopWQuarkTruthMatch_phi->SetYTitle("# Events");
    
    // hadronic W quark eta
    fHistLeptonicTopWQuarkTruthMatch_eta = new TH1F("h_TMLeptonicTopWQuark_eta",
				       "Truth Matching of jets #eta w.r.t. quarks",
				       30, -1.5, 1.5);
    fHistLeptonicTopWQuarkTruthMatch_eta->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistLeptonicTopWQuarkTruthMatch_eta->SetYTitle("# Events");

    gDirectory->cd("..");
    gDirectory->mkdir("Hadron Level Jets","TruthMathching histograms for jets from W decay w.r.t. HL jets");
    gDirectory->cd("Hadron Level Jets");

    // hadronic W jet pt
    fHistLeptonicTopWHLJetTruthMatch_pt = new TH1F("h_TMLeptonicTopWHLJet_pt",
				      "Truth Matching of jets p_{T} w.r.t. HL jets",
				      30, -1.5, 1.5);
    fHistLeptonicTopWHLJetTruthMatch_pt->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistLeptonicTopWHLJetTruthMatch_pt->SetYTitle("# Events");
    
    // hadronic W jet phi
    fHistLeptonicTopWHLJetTruthMatch_phi = new TH1F("h_TMLeptonicTopWHLJet_phi",
				       "Truth Matching of jets #varph w.r.t. HL jets",
				       30, -1.5, 1.5);
    fHistLeptonicTopWHLJetTruthMatch_phi->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistLeptonicTopWHLJetTruthMatch_phi->SetYTitle("# Events");
    
    // hadronic W jet eta
    fHistLeptonicTopWHLJetTruthMatch_eta = new TH1F("h_TMLeptonicTopWHLJet_eta",
				       "Truth Matching of jets #eta w.r.t. HL jets",
				       30, -1.5, 1.5);
    fHistLeptonicTopWHLJetTruthMatch_eta->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistLeptonicTopWHLJetTruthMatch_eta->SetYTitle("# Events");

    gDirectory->cd("..");
    
    // ==================================
    // histograms for hadronic top decays
    // ==================================
    gDirectory->cd("../../..");
}
void AtlSgTop_WtChannelFinder::HadronicTopHistograms() {

    gDirectory->mkdir("HadronicTOP", "Hadronic t/tbar reconstruction");
    gDirectory->cd("HadronicTOP");
    // ========================
    // Top-quark reconstruction
    // ========================
    gDirectory->mkdir("Top-reco", "Top reconstruction");
    gDirectory->cd("Top-reco");

    gDirectory->mkdir("Both", "t/#bar{t} reconstruction");
    gDirectory->cd("Both");

    fHistHadronicTop_Mt = new TH1F("h_treco_Mt", "Top Transverse Invariant Mass (all leptons)",
			       60, 0, 210);
    fHistHadronicTop_Mt->SetXTitle("M_{t,top} (GeV)");
    fHistHadronicTop_Mt->SetYTitle("Number of Entries");
    fHistHadronicTop_M = new TH1F("h_treco_M", "Top Invariant Mass (all leptons)",
			       120, 165, 185);
    fHistHadronicTop_M->SetXTitle("M_{top} (GeV)");
    fHistHadronicTop_M->SetYTitle("Number of Entries");
    fHistHadronicTop_phi = new TH1F("h_treco_phi", "Top Azimuth Angle (all leptons)",
				64, -3.2, 3.2);
    fHistHadronicTop_phi->SetXTitle("#phi (rad)");
    fHistHadronicTop_phi->SetYTitle("Number of Entries");
    fHistHadronicTop_pt = new TH1F("h_treco_pt", "Top Transverse Momentum (all leptons)",
			       100, 0, 300);
    fHistHadronicTop_pt->SetXTitle("P_{t} (GeV)");
    fHistHadronicTop_pt->SetYTitle("Number of Entries");
    fHistHadronicTop_rap = new TH1F("h_treco_rap", "Top Rapidity (all leptons)",
				50, -5.0, 5.0);
    fHistHadronicTop_rap->SetXTitle("y (rapidity)");
    fHistHadronicTop_rap->SetYTitle("Number of Entries");

    fHistHadronicTop_eta = new TH1F("h_treco_eta", "Top PseudoRapidity (all leptons)",
				50, -5.0, 5.0);
    fHistHadronicTop_eta->SetXTitle("#eta (pseudorapidity)");
    fHistHadronicTop_eta->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    gDirectory->mkdir("e-channel", "hadronic top quark, e-channel");
    gDirectory->cd("e-channel");
    
    fHistHadronicTop_Mt_e = new TH1F("h_treco_e_Mt", "Top Transverse Invariant Mass, electrons",
			       60, 0, 210);
    fHistHadronicTop_Mt_e->SetXTitle("M_{t,top} (GeV)");
    fHistHadronicTop_Mt->SetYTitle("Number of Entries");
    fHistHadronicTop_M_e = new TH1F("h_treco_M", "Top Invariant Mass, electrons",
			       50, 168, 182);
    fHistHadronicTop_M_e->SetXTitle("M_{top} (GeV)");
    fHistHadronicTop_M_e->SetYTitle("Number of Entries");
    fHistHadronicTop_phi_e = new TH1F("h_treco_e_phi", "Top Azimuth Angle, electrons",
				128, -3.2, 3.2);
    fHistHadronicTop_phi_e->SetXTitle("#phi (rad)");
    fHistHadronicTop_phi_e->SetYTitle("Number of Entries");
    fHistHadronicTop_pt_e = new TH1F("h_treco_e_pt", "Top Transverse Momentum, electrons",
			       100, 0, 300);
    fHistHadronicTop_pt_e->SetXTitle("P_{t} (GeV)");
    fHistHadronicTop_pt_e->SetYTitle("Number of Entries");
    fHistHadronicTop_rap_e = new TH1F("h_treco_e_rap", "Top Rapidity, electrons",
				50, -5.0, 5.0);
    fHistHadronicTop_rap_e->SetXTitle("y (rapidity)");
    fHistHadronicTop_rap_e->SetYTitle("Number of Entries");
    fHistHadronicTop_eta_e = new TH1F("h_treco_e_eta", "Top PseudoRapidity, electrons",
				50, -5.0, 5.0);
    fHistHadronicTop_eta_e->SetXTitle("#eta (pseudorapidity)");
    fHistHadronicTop_eta_e->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    gDirectory->mkdir("mu-channel", "hadronic top quark, #mu-channel");
    gDirectory->cd("mu-channel");

    fHistHadronicTop_Mt_mu = new TH1F("h_treco_mu_Mt", "Top Transverse Invariant Mass, muons",
			       60, 0, 210);
    fHistHadronicTop_Mt_mu->SetXTitle("M_{t,top} (GeV)");
    fHistHadronicTop_Mt->SetYTitle("Number of Entries");
    fHistHadronicTop_M_mu = new TH1F("h_treco_M", "Top Invariant Mass, muons",
			       50, 168, 182);
    fHistHadronicTop_M_mu->SetXTitle("M_{top} (GeV)");
    fHistHadronicTop_M_mu->SetYTitle("Number of Entries");
    fHistHadronicTop_phi_mu = new TH1F("h_treco_mu_phi", "Top Azimuth Angle, muons",
				128, -3.2, 3.2);
    fHistHadronicTop_phi_mu->SetXTitle("#phi (rad)");
    fHistHadronicTop_phi_mu->SetYTitle("Number of Entries");
    fHistHadronicTop_pt_mu = new TH1F("h_treco_mu_pt", "Top Transverse Momentum, muons",
			       100, 0, 300);
    fHistHadronicTop_pt_mu->SetXTitle("P_{t} (GeV)");
    fHistHadronicTop_pt_mu->SetYTitle("Number of Entries");
    fHistHadronicTop_rap_mu = new TH1F("h_treco_mu_rap", "Top Rapidity, muons",
				50, -5.0, 5.0);
    fHistHadronicTop_rap_mu->SetXTitle("y (rapidity)");
    fHistHadronicTop_rap_mu->SetYTitle("Number of Entries");
    fHistHadronicTop_eta_mu = new TH1F("h_treco_mu_eta", "Top PseudoRapidity, muons",
				50, -5.0, 5.0);
    fHistHadronicTop_eta_mu->SetXTitle("#eta (pseudorapidity)");
    fHistHadronicTop_eta_mu->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
//     gDirectory->mkdir("tbar", "#bar{t}-quark reconstruction");
//     gDirectory->cd("tbar");
//     // All positive sign leptons
//     fHistHadronicTop_Mt_plus = new TH1F("h_treco_Mt_plus",
// 				    "t-Quark Transverse Invariant Mass (all positive sign leptons)",
// 				    60, 0, 210);
//     fHistHadronicTop_Mt_plus->SetXTitle("M_{t,top} (GeV)");
//     fHistHadronicTop_Mt_plus->SetYTitle("Number of Entries");
//     fHistHadronicTop_M_plus = new TH1F("h_treco_M_plus",
// 				    "t-Quark Invariant Mass (all positive sign leptons)",
// 				    50, 168, 182);
//     fHistHadronicTop_M_plus->SetXTitle("M_{top} (GeV)");
//     fHistHadronicTop_M_plus->SetYTitle("Number of Entries");
//     fHistHadronicTop_phi_plus = new TH1F("h_treco_phi_plus",
// 				     "t-Quark Azimuth Angle (all positive sign leptons)",
// 				     128, -3.2, 3.2);
//     fHistHadronicTop_phi_plus->SetXTitle("#phi (rad)");
//     fHistHadronicTop_phi_plus->SetYTitle("Number of Entries");
//     fHistHadronicTop_pt_plus = new TH1F("h_treco_pt_plus",
// 				    "t-Quark Transverse Momentum (all positive sign leptons)",
// 				    100, 0, 300);
//     fHistHadronicTop_pt_plus->SetXTitle("P_{t} (GeV)");
//     fHistHadronicTop_pt_plus->SetYTitle("Number of Entries");
//     fHistHadronicTop_eta_plus = new TH1F("h_treco_eta_plus",
// 				     "t-Quark Pseudo-Rapidity (all positive sign leptons)",
// 				     50, -5.0, 5.0);
//     fHistHadronicTop_eta_plus->SetXTitle("#eta");
//     fHistHadronicTop_eta_plus->SetYTitle("Number of Entries");
//     fHistHadronicTop_rap_plus = new TH1F("h_treco_rap_plus",
// 				     "t-Quark Rapidity (all positive sign leptons)",
// 				     50, -5.0, 5.0);
//     fHistHadronicTop_rap_plus->SetXTitle("y (rapidity)");
//     fHistHadronicTop_rap_plus->SetYTitle("Number of Entries");


    
//     gDirectory->cd("..");
//     gDirectory->mkdir("t", "t-quark reconstruction");
//     gDirectory->cd("t");
    
//     // All negative sign leptons
//     fHistHadronicTop_Mt_minus = new TH1F("h_treco_Mt_minus",
// 				     "#bar{t}-Quark Transverse Invariant Mass (all negative sign leptons)",
// 				     60, 0, 210);
//     fHistHadronicTop_Mt_minus->SetXTitle("M_{t,top} (GeV)");
//     fHistHadronicTop_Mt_minus->SetYTitle("Number of Entries");
//     fHistHadronicTop_M_minus = new TH1F("h_treco_M_minus",
// 				     "#bar{t}-Quark Invariant Mass (all negative sign leptons)",
// 				     50, 168, 182);
//     fHistHadronicTop_M_minus->SetXTitle("M_{top} (GeV)");
//     fHistHadronicTop_M_minus->SetYTitle("Number of Entries");
//     fHistHadronicTop_phi_minus = new TH1F("h_treco_phi_minus",
// 				      "#bar{t}-Quark Azimuth Angle (all negative sign leptons)",
// 				      128, -3.2, 3.2);
//     fHistHadronicTop_phi_minus->SetXTitle("#phi (rad)");
//     fHistHadronicTop_phi_minus->SetYTitle("Number of Entries");
//     fHistHadronicTop_pt_minus = new TH1F("h_treco_pt_minus",
// 				     "#bar{t}-Quark Transverse Momentum (all negative sign leptons)",
// 				     100, 0, 300);
//     fHistHadronicTop_pt_minus->SetXTitle("P_{t} (GeV)");
//     fHistHadronicTop_pt_minus->SetYTitle("Number of Entries");
//     fHistHadronicTop_eta_minus = new TH1F("h_treco_eta_minus",
// 				      "t-Quark Pseudo-Rapidity (all negative sign leptons)",
// 				      50, -5.0, 5.0);
//     fHistHadronicTop_eta_minus->SetXTitle("#eta");
//     fHistHadronicTop_eta_minus->SetYTitle("Number of Entries");
//     fHistHadronicTop_rap_minus = new TH1F("h_treco_rap_minus",
// 				      "t-Quark Rapidity (all negative sign leptons)",
// 				      50, -5.0, 5.0);
//     fHistHadronicTop_rap_minus->SetXTitle("y (rapidity)");
//     fHistHadronicTop_rap_minus->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");

    // hadronic W reconstruction
    gDirectory->mkdir("Hadronic W-reco", "Hadronic W reconstruction");
    gDirectory->cd("Hadronic W-reco");
    gDirectory->mkdir("both", "electron- and muon-channel");
    gDirectory->cd("both");
    // electron and muon channel
    fHist_W_hadr_top_reco_Mt = new TH1F("h_W_hadr_top_reco_Mt",
				  "hadronic W Invariant Transverse Mass, all leptons",
				  65, 0, 130);
    fHist_W_hadr_top_reco_Mt->SetXTitle("M_{t,W} (GeV)");
    fHist_W_hadr_top_reco_Mt->SetYTitle("Number of Entries");
    fHist_W_hadr_top_reco_M = new TH1F("h_W_hadr_top_reco_M",
				  "hadronic W Invariant Mass, all leptons",
				  100, 0, 300);
    fHist_W_hadr_top_reco_M->SetXTitle("M_{W} (GeV)");
    fHist_W_hadr_top_reco_M->SetYTitle("Number of Entries");
    fHist_W_hadr_top_reco_pt = new TH1F("h_W_hadr_top_reco_pt",
				  "hadronic W p_{T}, all leptons",
				  100, 0, 300);
    fHist_W_hadr_top_reco_pt->SetXTitle("p_{T} (GeV)");
    fHist_W_hadr_top_reco_pt->SetYTitle("Number of Entries");
    fHist_W_hadr_top_reco_phi = new TH1F("h_W_hadr_top_reco_phi",
				  "hadronic W #varphi, all leptons",
				  64, -3.2, 3.2);
    fHist_W_hadr_top_reco_phi->SetXTitle("#varphi");
    fHist_W_hadr_top_reco_phi->SetYTitle("Number of Entries");
    fHist_W_hadr_top_reco_rap = new TH1F("h_W_hadr_top_reco_y",
				  "hadronic W rapidity, all leptons",
				  50, -5., 5.);
    fHist_W_hadr_top_reco_rap->SetXTitle("y");
    fHist_W_hadr_top_reco_rap->SetYTitle("Number of Entries");

    gDirectory->cd("..");

    // electron channel

    gDirectory->mkdir("Electron", "electron-channel");
    gDirectory->cd("Electron");
    
    fHist_W_hadr_top_reco_Mt_e = new TH1F("h_W_hadr_top_reco_Mt_e",
				  "hadronic W Invariant Transverse Mass, e-channel",
				  200, 0, 130);
    fHist_W_hadr_top_reco_Mt_e->SetXTitle("M_{t,W} (GeV)");
    fHist_W_hadr_top_reco_Mt_e->SetYTitle("Number of Entries");
    fHist_W_hadr_top_reco_M_e = new TH1F("h_W_hadr_top_reco_M_e",
				  "hadronic W Invariant Mass, e-channel",
				  100, 0, 300);
    fHist_W_hadr_top_reco_M_e->SetXTitle("M_{W} (GeV)");
    fHist_W_hadr_top_reco_M_e->SetYTitle("Number of Entries");
    fHist_W_hadr_top_reco_pt_e = new TH1F("h_W_hadr_top_reco_pt_e",
				  "hadronic W p_{T}, e-channel",
				  100, 0, 300);
    fHist_W_hadr_top_reco_pt_e->SetXTitle("p_{T} (GeV)");
    fHist_W_hadr_top_reco_pt_e->SetYTitle("Number of Entries");
    fHist_W_hadr_top_reco_phi_e = new TH1F("h_W_hadr_top_reco_phi_e",
				  "hadronic W #varphi, e-channel",
				  64, -3.2, 3.2);
    fHist_W_hadr_top_reco_phi_e->SetXTitle("#varphi");
    fHist_W_hadr_top_reco_phi_e->SetYTitle("Number of Entries");
    fHist_W_hadr_top_reco_rap_e = new TH1F("h_W_hadr_top_reco_y_e",
				  "hadronic W rapidity, e-channel",
				  50, -5., 5.);
    fHist_W_hadr_top_reco_rap_e->SetXTitle("y");
    fHist_W_hadr_top_reco_rap_e->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // muon channel

    gDirectory->mkdir("Muon", "Muon-channel");
    gDirectory->cd("Muon");

    fHist_W_hadr_top_reco_Mt_mu = new TH1F("h_W_hadr_top_reco_Mt_mu",
				  "hadronic W Invariant Transverse Mass, e-channel",
				  130, 0, 130);
    fHist_W_hadr_top_reco_Mt_mu->SetXTitle("M_{t,W} (GeV)");
    fHist_W_hadr_top_reco_Mt_mu->SetYTitle("Number of Entries");
    fHist_W_hadr_top_reco_M_mu = new TH1F("h_W_hadr_top_reco_M_mu",
				  "hadronic W Invariant Mass, e-channel",
				  100, 0, 300);
    fHist_W_hadr_top_reco_M_mu->SetXTitle("M_{W} (GeV)");
    fHist_W_hadr_top_reco_M_mu->SetYTitle("Number of Entries");
    fHist_W_hadr_top_reco_pt_mu = new TH1F("h_W_hadr_top_reco_pt_mu",
				  "hadronic W p_{T}, e-channel",
				  100, 0, 300);
    fHist_W_hadr_top_reco_pt_mu->SetXTitle("p_{T} (GeV)");
    fHist_W_hadr_top_reco_pt_mu->SetYTitle("Number of Entries");
    fHist_W_hadr_top_reco_phi_mu = new TH1F("h_W_hadr_top_reco_phi_mu",
				  "hadronic W #varphi, e-channel",
				  64, -3.2, 3.2);
    fHist_W_hadr_top_reco_phi_mu->SetXTitle("#varphi");
    fHist_W_hadr_top_reco_phi_mu->SetYTitle("Number of Entries");
    fHist_W_hadr_top_reco_rap_mu = new TH1F("h_W_hadr_top_reco_y_mu",
				  "hadronic W rapidity, e-channel",
				  50, -5., 5.);
    fHist_W_hadr_top_reco_rap_mu->SetXTitle("y");
    fHist_W_hadr_top_reco_rap_mu->SetYTitle("Number of Entries");

    gDirectory->cd("..");    
    // ================================
    // Chi2 of KinFitter reconstruction
    // ================================
    gDirectory->cd("..");

    gDirectory->mkdir("Associated_LeptonicW-reco", "information regarding LeptonicW");
    gDirectory->cd("Associated_LeptonicW-reco");
    
//     gDirectory->mkdir("Both", "W+/W- reconstruction");
//     gDirectory->cd("Both");
    
//     // All leptons
//     fHist_W_lept_notop_reco_Mt = new TH1F("h_W_lept_notop_reco_Mt", "W Invariant Transverse Mass (all leptons)",
// 			      200, 0, 130);
//     fHist_W_lept_notop_reco_Mt->SetXTitle("M_{t,W} (GeV)");
//     fHist_W_lept_notop_reco_Mt->SetYTitle("Number of Entries");
//     fHist_W_lept_notop_reco_phi = new TH1F("h_W_lept_notop_reco_phi", "W Azimuth Angle (all leptons)",
// 				64, -3.2, 3.2);
//     fHist_W_lept_notop_reco_phi->SetXTitle("#phi (rad)");
//     fHist_W_lept_notop_reco_phi->SetYTitle("Number of Entries");
//     fHist_W_lept_notop_reco_pt = new TH1F("h_W_lept_notop_reco_pt", "W Transverse Momentum (all leptons)",
// 			       100, 0, 300);
//     fHist_W_lept_notop_reco_pt->SetXTitle("P_{t} (GeV)");
//     fHist_W_lept_notop_reco_pt->SetYTitle("Number of Entries");

//     // Electron channel
//     fHist_W_lept_notop_reco_Mt_e = new TH1F("h_W_lept_notop_reco_Mt_e", "W Invariant Transverse Mass (e channel)",
// 			      200, 0, 130);
//     fHist_W_lept_notop_reco_Mt_e->SetXTitle("M_{t,W} (GeV)");
//     fHist_W_lept_notop_reco_Mt_e->SetYTitle("Number of Entries");
//     fHist_W_lept_notop_reco_phi_e = new TH1F("h_W_lept_notop_reco_phi_e", "W Azimuth Angle (e channel)",
// 				64, -3.2, 3.2);
//     fHist_W_lept_notop_reco_phi_e->SetXTitle("#phi (rad)");
//     fHist_W_lept_notop_reco_phi_e->SetYTitle("Number of Entries");
//     fHist_W_lept_notop_reco_pt_e = new TH1F("h_W_lept_notop_reco_pt_e", "W Transverse Momentum (e channel)",
// 			       100, 0, 300);
//     fHist_W_lept_notop_reco_pt_e->SetXTitle("P_{t} (GeV)");
//     fHist_W_lept_notop_reco_pt_e->SetYTitle("Number of Entries");
    
//     // Muon channel
//     fHist_W_lept_notop_reco_Mt_mu = new TH1F("h_W_lept_notop_reco_Mt_mu", "W Invariant Transverse Mass (#mu channel)",
// 			       200, 0, 130);
//     fHist_W_lept_notop_reco_Mt_mu->SetXTitle("M_{t,W} (GeV)");
//     fHist_W_lept_notop_reco_Mt_mu->SetYTitle("Number of Entries");
//     fHist_W_lept_notop_reco_phi_mu = new TH1F("h_W_lept_notop_reco_phi_mu", "W Azimuth Angle (#mu channel)",
// 				 64, -3.2, 3.2);
//     fHist_W_lept_notop_reco_phi_mu->SetXTitle("#phi (rad)");
//     fHist_W_lept_notop_reco_phi_mu->SetYTitle("Number of Entries");
//     fHist_W_lept_notop_reco_pt_mu = new TH1F("h_W_lept_notop_reco_pt_mu", "W Transverse Momentum (#mu channel)",
// 				100, 0, 300);
//     fHist_W_lept_notop_reco_pt_mu->SetXTitle("P_{t} (GeV)");
//     fHist_W_lept_notop_reco_pt_mu->SetYTitle("Number of Entries");

//     gDirectory->cd("..");
//     gDirectory->mkdir("W-", "W- reconstruction");
//     gDirectory->cd("W-");
    
//     // All negative sign leptons
//     fHist_W_lept_notop_reco_Mt_minus = new TH1F("h_W_lept_notop_reco_Mt_minus",
// 				   "W^{-} Invariant Transverse Mass (all negative sign leptons)",
// 				   200, 0, 130);
//     fHist_W_lept_notop_reco_Mt_minus->SetXTitle("M_{t,W} (GeV)");
//     fHist_W_lept_notop_reco_Mt_minus->SetYTitle("Number of Entries");
//     fHist_W_lept_notop_reco_phi_minus = new TH1F("h_W_lept_notop_reco_phi_minus",
// 				    "W^{-} Azimuth Angle (all negative sign leptons)",
// 				    64, -3.2, 3.2);
//     fHist_W_lept_notop_reco_phi_minus->SetXTitle("#phi (rad)");
//     fHist_W_lept_notop_reco_phi_minus->SetYTitle("Number of Entries");
//     fHist_W_lept_notop_reco_pt_minus = new TH1F("h_W_lept_notop_reco_pt_minus",
// 				   "W^{-} Transverse Momentum (all negative sign leptons)",
// 				   100, 0, 300);
//     fHist_W_lept_notop_reco_pt_minus->SetXTitle("P_{t} (GeV)");
//     fHist_W_lept_notop_reco_pt_minus->SetYTitle("Number of Entries");
        
//     // Electron e- channel
//     fHist_W_lept_notop_reco_Mt_e_minus = new TH1F("h_W_lept_notop_reco_Mt_e_minus",
// 				     "W^{-} Invariant Transverse Mass (e^{-} channel)",
// 				     200, 0, 130);
//     fHist_W_lept_notop_reco_Mt_e_minus->SetXTitle("M_{t,W} (GeV)");
//     fHist_W_lept_notop_reco_Mt_e_minus->SetYTitle("Number of Entries");
//     fHist_W_lept_notop_reco_phi_e_minus = new TH1F("h_W_lept_notop_reco_phi_e_minus",
// 				      "W^{-} Azimuth Angle (e^{-} channel)",
// 				      64, -3.2, 3.2);
//     fHist_W_lept_notop_reco_phi_e_minus->SetXTitle("#phi (rad)");
//     fHist_W_lept_notop_reco_phi_e_minus->SetYTitle("Number of Entries");
//     fHist_W_lept_notop_reco_pt_e_minus = new TH1F("h_W_lept_notop_reco_pt_e_minus",
// 				     "W^{-} Transverse Momentum (e^{-} channel)",
// 				     100, 0, 300);
//     fHist_W_lept_notop_reco_pt_e_minus->SetXTitle("P_{t} (GeV)");
//     fHist_W_lept_notop_reco_pt_e_minus->SetYTitle("Number of Entries");

//     // Muon mu- channel
//     fHist_W_lept_notop_reco_Mt_mu_minus = new TH1F("h_W_lept_notop_reco_Mt_mu_minus",
// 				      "W^{-} Invariant Transverse Mass (#mu^{-} channel)",
// 				      200, 0, 130);
//     fHist_W_lept_notop_reco_Mt_mu_minus->SetXTitle("M_{t,W} (GeV)");
//     fHist_W_lept_notop_reco_Mt_mu_minus->SetYTitle("Number of Entries");
//     fHist_W_lept_notop_reco_phi_mu_minus = new TH1F("h_W_lept_notop_reco_phi_mu_minus",
// 				       "W^{-} Azimuth Angle (#mu^{-} channel)",
// 				       64, -3.2, 3.2);
//     fHist_W_lept_notop_reco_phi_mu_minus->SetXTitle("#phi (rad)");
//     fHist_W_lept_notop_reco_phi_mu_minus->SetYTitle("Number of Entries");
//     fHist_W_lept_notop_reco_pt_mu_minus = new TH1F("h_W_lept_notop_reco_pt_mu_minus",
// 				      "W^{-} Transverse Momentum (#mu^{-} channel)",
// 				      100, 0, 300);
//     fHist_W_lept_notop_reco_pt_mu_minus->SetXTitle("P_{t} (GeV)");
//     fHist_W_lept_notop_reco_pt_mu_minus->SetYTitle("Number of Entries");

//     gDirectory->cd("..");
//     gDirectory->mkdir("W+", "W+ reconstruction");
//     gDirectory->cd("W+");
    
//     // All positive sign leptons
//     fHist_W_lept_notop_reco_Mt_plus = new TH1F("h_W_lept_notop_reco_Mt_plus",
// 				  "W^{+} Invariant Transverse Mass (all positive sign leptons)",
// 				  200, 0, 130);
//     fHist_W_lept_notop_reco_Mt_plus->SetXTitle("M_{t,W} (GeV)");
//     fHist_W_lept_notop_reco_Mt_plus->SetYTitle("Number of Entries");
//     fHist_W_lept_notop_reco_phi_plus = new TH1F("h_W_lept_notop_reco_phi_plus",
// 				   "W^{+} Azimuth Angle (all positive sign leptons)",
// 				   64, -3.2, 3.2);
//     fHist_W_lept_notop_reco_phi_plus->SetXTitle("#phi (rad)");
//     fHist_W_lept_notop_reco_phi_plus->SetYTitle("Number of Entries");
//     fHist_W_lept_notop_reco_pt_plus = new TH1F("h_W_lept_notop_reco_pt_plus",
// 				  "W^{+} Transverse Momentum (all positive sign leptons)",
// 				  100, 0, 300);
//     fHist_W_lept_notop_reco_pt_plus->SetXTitle("P_{t} (GeV)");
//     fHist_W_lept_notop_reco_pt_plus->SetYTitle("Number of Entries");
    
//     // Electron e+ channel
//     fHist_W_lept_notop_reco_Mt_e_plus = new TH1F("h_W_lept_notop_reco_Mt_e_plus",
// 				    "W^{+} Invariant Transverse Mass (e^{+} channel)",
// 				    200, 0, 130);
//     fHist_W_lept_notop_reco_Mt_e_plus->SetXTitle("M_{t,W} (GeV)");
//     fHist_W_lept_notop_reco_Mt_e_plus->SetYTitle("Number of Entries");
//     fHist_W_lept_notop_reco_phi_e_plus = new TH1F("h_W_lept_notop_reco_phi_e_plus",
// 				     "W^{+} Azimuth Angle (e^{+} channel)",
// 				     64, -3.2, 3.2);
//     fHist_W_lept_notop_reco_phi_e_plus->SetXTitle("#phi (rad)");
//     fHist_W_lept_notop_reco_phi_e_plus->SetYTitle("Number of Entries");
//     fHist_W_lept_notop_reco_pt_e_plus = new TH1F("h_W_lept_notop_reco_pt_e_plus",
// 				    "W^{+} Transverse Momentum (e^{+} channel)",
// 				    100, 0, 300);
//     fHist_W_lept_notop_reco_pt_e_plus->SetXTitle("P_{t} (GeV)");
//     fHist_W_lept_notop_reco_pt_e_plus->SetYTitle("Number of Entries");

//     // Muon mu+ channel
//     fHist_W_lept_notop_reco_Mt_mu_plus = new TH1F("h_W_lept_notop_reco_Mt_mu_plus",
// 				     "W^{+} Invariant Transverse Mass (#mu^{+} channel)",
// 				     200, 0, 130);
//     fHist_W_lept_notop_reco_Mt_mu_plus->SetXTitle("M_{t,W} (GeV)");
//     fHist_W_lept_notop_reco_Mt_mu_plus->SetYTitle("Number of Entries");
//     fHist_W_lept_notop_reco_phi_mu_plus = new TH1F("h_W_lept_notop_reco_phi_mu_plus",
// 				      "W^{+} Azimuth Angle (#mu^{+} channel)",
// 				      64, -3.2, 3.2);
//     fHist_W_lept_notop_reco_phi_mu_plus->SetXTitle("#phi (rad)");
//     fHist_W_lept_notop_reco_phi_mu_plus->SetYTitle("Number of Entries");
//     fHist_W_lept_notop_reco_pt_mu_plus = new TH1F("h_W_lept_notop_reco_pt_mu_plus",
// 				     "W^{+} Transverse Momentum (#mu^{+} channel)",
// 				     100, 0, 300);
//     fHist_W_lept_notop_reco_pt_mu_plus->SetXTitle("P_{t} (GeV)");
//     fHist_W_lept_notop_reco_pt_mu_plus->SetYTitle("Number of Entries");

    gDirectory->mkdir("ChargedLepton", "reconstructed charged lepton");
    gDirectory->cd("ChargedLepton");
    gDirectory->mkdir("electron", "reconstructed electron");
    gDirectory->cd("electron");
    fHist_HadronicTop_electron_pt = new TH1F("h_hadr_top_e_pt",
				  "electron pt in hadronic top events",
				  50, 9., 150.);
    fHist_HadronicTop_electron_pt->SetXTitle("p_{T} (GeV)");
    fHist_HadronicTop_electron_pt->SetYTitle("Number of Entries");
    fHist_HadronicTop_electron_eta = new TH1F("h_hadr_top_e_eta",
				  "electron #eta in hadronic top events",
				  50,-2.6, 2.6);
    fHist_HadronicTop_electron_eta->SetXTitle("#eta");
    fHist_HadronicTop_electron_eta->SetYTitle("Number of Entries");
    fHist_HadronicTop_electron_phi = new TH1F("h_hadr_top_e_phi",
				  "electron #varphi in hadronic top events",
				  50, -3.2, 3.2);
    fHist_HadronicTop_electron_phi->SetXTitle("#varphi");
    fHist_HadronicTop_electron_phi->SetYTitle("Number of Entries");
    gDirectory->cd("..");
    
    gDirectory->mkdir("muon", "reconstructed muon");
    gDirectory->cd("muon");
    fHist_HadronicTop_muon_pt = new TH1F("h_hadr_top_mu_pt",
					 "muon pt in hadronic top events",
					 50, 9., 150.);
    fHist_HadronicTop_muon_pt->SetXTitle("p_{T} (GeV)");
    fHist_HadronicTop_muon_pt->SetYTitle("Number of Entries");
    fHist_HadronicTop_muon_eta = new TH1F("h_hadr_top_mu_eta",
					  "muon #eta in hadronic top events",
					  50,-2.6, 2.6);
    fHist_HadronicTop_muon_eta->SetXTitle("#eta");
    fHist_HadronicTop_muon_eta->SetYTitle("Number of Entries");
    fHist_HadronicTop_muon_phi = new TH1F("h_hadr_top_mu_phi",
					  "muon #varphi in hadronic top events",
					  50,-3.2, 3.2);
    fHist_HadronicTop_muon_phi->SetXTitle("#varphi");
    fHist_HadronicTop_muon_phi->SetYTitle("Number of Entries");
    gDirectory->cd("..");
    
    gDirectory->mkdir("both", "reconstructed charged lepton");
    gDirectory->cd("both");
    fHist_HadronicTop_ChargedLepton_pt = new TH1F("h_hadr_top_lep_pt",
						  "ChargedLepton pt in hadronic top events",
						  50, 9., 150.);
    fHist_HadronicTop_ChargedLepton_pt->SetXTitle("p_{T} (GeV)");
    fHist_HadronicTop_ChargedLepton_pt->SetYTitle("Number of Entries");
    fHist_HadronicTop_ChargedLepton_eta = new TH1F("h_hadr_top_lep_eta",
						   "ChargedLepton #eta in hadronic top events",
						   50,-2.6, 2.6);
    fHist_HadronicTop_ChargedLepton_eta->SetXTitle("#eta");
    fHist_HadronicTop_ChargedLepton_eta->SetYTitle("Number of Entries");
    fHist_HadronicTop_ChargedLepton_phi = new TH1F("h_hadr_top_lep_phi",
						   "ChargedLepton #varphi in hadronic top events",
						   50,-3.2, 3.2);
    fHist_HadronicTop_ChargedLepton_phi->SetXTitle("#varphi");
    fHist_HadronicTop_ChargedLepton_phi->SetYTitle("Number of Entries");
    gDirectory->cd("../..");
    gDirectory->mkdir("EtMiss", "reconstructed missing Et");
    gDirectory->cd("EtMiss");
    fHist_HadronicTop_EtMiss_mag = new TH1F("h_hadr_top_etmiss_mag",
					    "EtMiss in hadronic top events",
					    50, 9., 150.);
    fHist_HadronicTop_EtMiss_mag->SetXTitle("E_{T}^{Miss}");
    fHist_HadronicTop_EtMiss_mag->SetYTitle("Number of Entries");
    fHist_HadronicTop_EtMiss_phi = new TH1F("h_hadr_top_etmiss_phi",
					    "#varphi of EtMiss in hadronic top events",
					    50, 0., 4.);
    fHist_HadronicTop_EtMiss_phi->SetXTitle("#varphi");
    fHist_HadronicTop_EtMiss_phi->SetYTitle("Number of Entries");
    gDirectory->cd("..");
    
    // histograms for transverse mass of leptonic W
    // computation with the help of the charged lepton
    // and EtMiss, the associated leptonic W of Wt events
    // is not reconstructed with a kinematic fit

    fHist_W_lept_notop_reco_Mt = new TH1F("h_W_lept_notop_reco_Mt", "W Invariant Transverse Mass (all leptons)",
					  200, 0, 130);
    fHist_W_lept_notop_reco_Mt->SetXTitle("M_{t,W} (GeV)");
    fHist_W_lept_notop_reco_Mt->SetYTitle("Number of Entries");
    fHist_W_lept_notop_reco_pt = new TH1F("h_W_lept_notop_reco_pt",
				  "associated leptonic W p_{T}, all leptons",
				  100, 0, 300);
    fHist_W_lept_notop_reco_pt->SetXTitle("p_{T} (GeV)");
    fHist_W_lept_notop_reco_pt->SetYTitle("Number of Entries");
    fHist_W_lept_notop_reco_phi = new TH1F("h_W_lept_notop_reco_phi",
				  "associated leptonic W #varphi, all leptons",
				  64, -3.2, 3.2);
    fHist_W_lept_notop_reco_phi->SetXTitle("#varphi");
    fHist_W_lept_notop_reco_phi->SetYTitle("Number of Entries");
//     fHist_W_lept_notop_reco_rap = new TH1F("h_W_lept_notop_reco_y",
// 				  "associated leptonic W rapidity, all leptons",
// 				  50, -5., 5.);
//     fHist_W_lept_notop_reco_rap->SetXTitle("y");
//     fHist_W_lept_notop_reco_rap->SetYTitle("Number of Entries");

    fHist_W_lept_notop_reco_Mt_e = new TH1F("h_W_lept_notop_reco_Mt_e", "W Invariant Transverse Mass (all leptons)",
					  200, 0, 130);
    fHist_W_lept_notop_reco_Mt_e->SetXTitle("M_{t,W} (GeV)");
    fHist_W_lept_notop_reco_Mt_e->SetYTitle("Number of Entries");
    fHist_W_lept_notop_reco_Mt_mu = new TH1F("h_W_lept_notop_reco_Mt_mu", "W Invariant Transverse Mass (all leptons)",
					  200, 0, 130);
    fHist_W_lept_notop_reco_Mt_mu->SetXTitle("M_{t,W} (GeV)");
    fHist_W_lept_notop_reco_Mt_mu->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    

    
    gDirectory->mkdir("KinFitter","Chi-square and chi-square probability distributions");
    gDirectory->cd("KinFitter");
    gDirectory->mkdir("Pulls","Pull Distributions");
    gDirectory->cd("Pulls");

    gDirectory->mkdir("BJet","BJet");
    gDirectory->cd("BJet");
    gDirectory->mkdir("Residuals","Residuals");
    gDirectory->cd("Residuals");
    fHistResidualBJetPt_had = new TH1F("h_ResidualBJetPt","Residual of BJet Pt",
				  1000,-0.5,0.5);
    fHistResidualBJetPt_had->SetXTitle("#frac{#Delta p_{T}}{#sigma_{p_{T}}}");
    fHistResidualBJetPt_had->SetYTitle("Number of Entries");
    fHistResidualBJetEta_had = new TH1F("h_ResidualBJetEta","Residual of BJet #eta",
				  1000,-0.5,0.5);
    fHistResidualBJetEta_had->SetXTitle("#frac{#Delta#eta}{#sigma_{#eta}}");
    fHistResidualBJetEta_had->SetYTitle("Number of Entries");
    fHistResidualBJetPhi_had = new TH1F("h_ResidualBJetPhi","Residual of BJet #varphi",
				  1000,-0.5,0.5);
    fHistResidualBJetPhi_had->SetXTitle("#frac{#Delta#varphi}{#sigma_{#varphi}}");
    fHistResidualBJetPhi_had->SetYTitle("Number of Entries");
    gDirectory->cd("..");
    
    fHistPullBJetPt_had = new TH1F("h_PullBJetPt_had","Pull of BJet Pt",
				   31,-8.,8.);
    fHistPullBJetPt_had->SetXTitle("#frac{#Delta p_{T}}{#sigma_{p_{T}}}");
    fHistPullBJetPt_had->SetYTitle("Number of Entries");
    fHistPullBJetEta_had = new TH1F("h_PullBJetEta_had","Pull of BJet #eta",
				    31,-8.,8.);
    fHistPullBJetEta_had->SetXTitle("#frac{#Delta#eta}{#sigma_{#eta}}");
    fHistPullBJetEta_had->SetYTitle("Number of Entries");
    fHistPullBJetPhi_had = new TH1F("h_PullBJetPhi_had","Pull of BJet #varphi",
				    31,-8.,8.);
    fHistPullBJetPhi_had->SetXTitle("#frac{#Delta#varphi}{#sigma_{#varphi}}");
    fHistPullBJetPhi_had->SetYTitle("Number of Entries");
    gDirectory->cd("..");

    gDirectory->mkdir("Jet1","Jet1");
    gDirectory->cd("Jet1");
    gDirectory->mkdir("Residuals","Residuals");
    gDirectory->cd("Residuals");
    fHistResidualJet1Pt_had = new TH1F("h_ResidualJet1Pt","Residual of Jet1 Pt",
				  1000,-0.5,0.5);
    fHistResidualJet1Pt_had->SetXTitle("#frac{#Delta p_{T}}{#sigma_{p_{T}}}");
    fHistResidualJet1Pt_had->SetYTitle("Number of Entries");
    fHistResidualJet1Eta_had = new TH1F("h_ResidualJet1Eta","Residual of Jet1 #eta",
				  1000,-0.5,0.5);
    fHistResidualJet1Eta_had->SetXTitle("#frac{#Delta#eta}{#sigma_{#eta}}");
    fHistResidualJet1Eta_had->SetYTitle("Number of Entries");
    fHistResidualJet1Phi_had = new TH1F("h_ResidualJet1Phi","Residual of Jet1 #varphi",
				  1000,-0.5,0.5);
    fHistResidualJet1Phi_had->SetXTitle("#frac{#Delta#varphi}{#sigma_{#varphi}}");
    fHistResidualJet1Phi_had->SetYTitle("Number of Entries");
    gDirectory->cd("..");
    
    fHistPullJet1Pt_had = new TH1F("h_PullJet1Pt_had","Pull of Jet1 Pt",
				   31,-8.,8.);
    fHistPullJet1Pt_had->SetXTitle("#frac{#Delta p_{T}}{#sigma_{p_{T}}}");
    fHistPullJet1Pt_had->SetYTitle("Number of Entries");
    fHistPullJet1Eta_had = new TH1F("h_PullJet1Eta_had","Pull of Jet1 #eta",
				    31,-8.,8.);
    fHistPullJet1Eta_had->SetXTitle("#frac{#Delta#eta}{#sigma_{#eta}}");
    fHistPullJet1Eta_had->SetYTitle("Number of Entries");
    fHistPullJet1Phi_had = new TH1F("h_PullJet1Phi_had","Pull of Jet1 #varphi",
				    31,-8.,8.);
    fHistPullJet1Phi_had->SetXTitle("#frac{#Delta#varphi}{#sigma_{#varphi}}");
    fHistPullJet1Phi_had->SetYTitle("Number of Entries");
    gDirectory->cd("..");

    gDirectory->mkdir("Jet2","Jet2");
    gDirectory->cd("Jet2");
    gDirectory->mkdir("Residuals","Residuals");    
    gDirectory->cd("Residuals");
    fHistResidualJet2Pt_had = new TH1F("h_ResidualJet2Pt_had","Residual of Jet2 Pt",
				  1000,-0.5,0.5);
    fHistResidualJet2Pt_had->SetXTitle("#frac{#Delta p_{T}}{#sigma_{p_{T}}}");
    fHistResidualJet2Pt_had->SetYTitle("Number of Entries");
    fHistResidualJet2Eta_had = new TH1F("h_ResidualJet2Eta_had","Residual of Jet2 #eta",
				  1000,-0.5,0.5);
    fHistResidualJet2Eta_had->SetXTitle("#frac{#Delta#eta}{#sigma_{#eta}}");
    fHistResidualJet2Eta_had->SetYTitle("Number of Entries");
    fHistResidualJet2Phi_had = new TH1F("h_ResidualJet2Phi_had","Residual of Jet2 #varphi",
				  1000,-05.,05.);
    fHistResidualJet2Phi_had->SetXTitle("#frac{#Delta#varphi}{#sigma_{#varphi}}");
    fHistResidualJet2Phi_had->SetYTitle("Number of Entries");
    gDirectory->cd("..");    
    fHistPullJet2Pt_had = new TH1F("h_PullJet2Pt_had","Pull of Jet2 Pt",
				   31,-8.,8.);
    fHistPullJet2Pt_had->SetXTitle("#frac{#Delta p_{T}}{#sigma_{p_{T}}}");
    fHistPullJet2Pt_had->SetYTitle("Number of Entries");
    fHistPullJet2Eta_had = new TH1F("h_PullJet2Eta_had","Pull of Jet2 #eta",
				    31,-8.,8.);
    fHistPullJet2Eta_had->SetXTitle("#frac{#Delta#eta}{#sigma_{#eta}}");
    fHistPullJet2Eta_had->SetYTitle("Number of Entries");
    fHistPullJet2Phi_had = new TH1F("h_PullJet2Phi_had","Pull of Jet2 #varphi",
				    31,-8.,8.);
    fHistPullJet2Phi_had->SetXTitle("#frac{#Delta#varphi}{#sigma_{#varphi}}");
    fHistPullJet2Phi_had->SetYTitle("Number of Entries");

    gDirectory->cd("../..");

    // both Wlep decays
    fHistHadronicTopChi2_both = new TH1F("h_hadrchi2_both",
			      "#chi^{2} distribution of sgtop_WtChannel reco (both W decays)",
			      150,0,30);
    fHistHadronicTopChi2_both->SetXTitle("#chi^{2}");
    fHistHadronicTopChi2_both->SetYTitle("Number of Entries");
    fHistHadronicTopChi2Prob_both = new TH1F("h_hadrchi2prob_both",
				  "#chi^{2} prob. distribution of sgtop_WtChannel reco (both W decays)",
				  40,0,1);
    fHistHadronicTopChi2Prob_both->SetXTitle("P(#chi^{2})");
    fHistHadronicTopChi2Prob_both->SetYTitle("Number of Entries");
    fHistHadronicTopChi2NDOF_both = new TH1F("h_hadrchi2NDOF_both",
			      "#chi^{2} distribution of sgtop_WtChannel reco (both W decays)",
			      150,0,30);
    fHistHadronicTopChi2NDOF_both->SetXTitle("#chi^{2}/NDOF");
    fHistHadronicTopChi2NDOF_both->SetYTitle("Number of Entries");

    fHistHadronicTopChi2_both_after = new TH1F("h_hadrchi2_both_after",
			      "#chi^{2} distribution of sgtop_WtChannel reco (both W decays)",
			      150,0,30);
    fHistHadronicTopChi2_both_after->SetXTitle("#chi^{2}");
    fHistHadronicTopChi2_both_after->SetYTitle("Number of Entries");
    fHistHadronicTopChi2Prob_both_after = new TH1F("h_hadrchi2prob_both_after",
				  "#chi^{2} prob. distribution of sgtop_WtChannel reco (both W decays)",
				  40,0,1);
    fHistHadronicTopChi2Prob_both_after->SetXTitle("P(#chi^{2})");
    fHistHadronicTopChi2Prob_both_after->SetYTitle("Number of Entries");
    fHistHadronicTopChi2NDOF_both_after = new TH1F("h_hadrchi2NDOF_both_after",
			      "#chi^{2} distribution of sgtop_WtChannel reco (both W decays)",
			      150,0,30);
    fHistHadronicTopChi2NDOF_both_after->SetXTitle("#chi^{2}/NDOF");
    fHistHadronicTopChi2NDOF_both_after->SetYTitle("Number of Entries");

    
    axis = fHistHadronicTopChi2Prob_both->GetXaxis();
    //axis->Set( 3, Chi2ProbBins );

    fHistHadronicTopChi2ProbTTBarVeto = new TH1F("h_chi2prob_TTBarVeto_HadronicTop",
				  "#chi^{2} prob. distribution of TTBar Veto with Wt reconstruction for hadronic top decay",
					     1000,0,1);
    fHistHadronicTopChi2ProbTTBarVeto->SetXTitle("P(#chi^{2})");
    fHistHadronicTopChi2ProbTTBarVeto->SetYTitle("Number of Entries");
    fHistHadronicTopChi2ProbWtAndTTBar = new TH2F("h_chi2prob_WtAndTTBar_HadronicTop",
				  "#chi^{2} prob. distribution of sgtop_WtChannel (hadronic top decay) and t#bar{t}  reco",
				  1000, 0, 1, 1000, 0, 1 );
    fHistHadronicTopChi2ProbWtAndTTBar->SetXTitle("P(#chi^{2})_{Wt}");
    fHistHadronicTopChi2ProbWtAndTTBar->SetYTitle("P(#chi^{2})_{t#bar{t}}");
    fHistHadronicTopChi2ProbWtAndTTBar->SetZTitle("Number of Entries");
    
    // Wlep -> e nu decays
    gDirectory->mkdir("Electron","Electron");
    gDirectory->cd("Electron");
    fHistHadronicTopChi2_e = new TH1F("h_chi2_e",
			      "#chi^{2} distribution of sgtop_WtChannel reco (e channel)",
			      150,0,30);
    fHistHadronicTopChi2_e->SetXTitle("#chi^{2}");
    fHistHadronicTopChi2_e->SetYTitle("Number of Entries");
    
    fHistHadronicTopChi2Prob_e = new TH1F("h_chi2prob_e",
				  "#chi^{2} prob. distribution of sgtop_WtChannel reco (e channel)",
				  40,0,1);
    fHistHadronicTopChi2Prob_e->SetXTitle("P(#chi^{2})");
    fHistHadronicTopChi2Prob_e->SetYTitle("Number of Entries");
    axis = fHistHadronicTopChi2Prob_e->GetXaxis();
    //axis->Set( 3, Chi2ProbBins );
    gDirectory->cd("..");
    
    // Wlep -> mu nu decays
    gDirectory->mkdir("Muon","Electron");
    gDirectory->cd("Muon");
    fHistHadronicTopChi2_mu = new TH1F("h_chi2_mu",
			       "#chi^{2} distribution of sgtop_WtChannel reco (#mu channel)",
			       35,0,10);
    fHistHadronicTopChi2_mu->SetXTitle("#chi^{2}");
    fHistHadronicTopChi2_mu->SetYTitle("Number of Entries");
    
    fHistHadronicTopChi2Prob_mu = new TH1F("h_chi2prob_mu",
				   "#chi^{2} prob. distr. of sgtop_WtChannel reco (#mu channel)",
				   40,0,1);
    fHistHadronicTopChi2Prob_mu->SetXTitle("P(#chi^{2})");
    fHistHadronicTopChi2Prob_mu->SetYTitle("Number of Entries");
    axis = fHistHadronicTopChi2Prob_mu->GetXaxis();
    //axis->Set( 3, Chi2ProbBins );
    gDirectory->cd("..");
    
    //
    // 2D plots: correlation of P(chi2) and M_invar
    //

    // 2D plots: correlation of P(chi2) and M_invar_W
    fHist_HadronicTopChi2Prob_vs_M_invar_W = new TH2F("h_chi2prob_vs_m_invar_W",
					   "#chi^{2} prob. vs invariant mass of W in sgtop_WtChannel reco",
					   25,0.,1.,
					   80,50.,130.);
    fHist_HadronicTopChi2Prob_vs_M_invar_W->SetXTitle("P(#chi^{2})");
    fHist_HadronicTopChi2Prob_vs_M_invar_W->SetYTitle("M_{W} (GeV)");
    fHist_HadronicTopChi2Prob_vs_M_invar_W->SetZTitle("Number of Entries");
  

    // 2D plots: correlation of P(chi2) and M_invar_top
    fHist_HadronicTopChi2Prob_vs_M_invar_top = new TH2F("h_chi2prob_vs_m_invar_top",
					   "#chi^{2} prob. vs invariant mass of top in sgtop_WtChannel reco",
					   25,0.,1.,
					     60,150.,210.);
    fHist_HadronicTopChi2Prob_vs_M_invar_top->SetXTitle("P(#chi^{2})");
    fHist_HadronicTopChi2Prob_vs_M_invar_top->SetYTitle("M_{top} (GeV)");
    fHist_HadronicTopChi2Prob_vs_M_invar_top->SetZTitle("Number of Entries");

    // ======================================
    // Truth Matching for Hadronic Top Decays
    // ======================================
    gDirectory->cd("..");
    gDirectory->mkdir("Truth Matching","Truth Matching for hadronic top decays");
    gDirectory->cd("Truth Matching");

    fHistHadronicTopRecoTrueChannel = new TH1I("h_TMHadronicTopChannel",
					  "True Channel in Wt events for reconstruction of hadronic top decay",
					  3, 0, 2);
    fHistHadronicTopRecoTrueChannel->SetXTitle("true channel");
    fHistHadronicTopRecoTrueChannel->SetYTitle("# Events");
    fHistHadronicTopRecoTrueChannel->SetAxisRange( 0., fHistHadronicTopRecoTrueChannel->GetMaximumStored(), "Y" );
    
    // b-jet truth matching w.r.t. the true b-quark
    gDirectory->mkdir("b-jet","TruthMathching histograms for b-jet");
    gDirectory->cd("b-jet");
    gDirectory->mkdir("b-quark","TruthMathching histograms for b-quark w.r.t. b-quark");
    gDirectory->cd("b-quark");

    // b-jet pt
    fHistHadronicTopBottomTruthMatch_pt = new TH1F("h_TMHadronicTopb_pt",
				      "Truth Matching of b-jet p_{T} w.r.t. MC b quark",
				      30, -1.5, 1.5);
    fHistHadronicTopBottomTruthMatch_pt->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistHadronicTopBottomTruthMatch_pt->SetYTitle("# Events");
    
    // b-jet phi
    fHistHadronicTopBottomTruthMatch_phi = new TH1F("h_TMHadronicTopb_phi",
				       "Truth Matching of b-jet #varphi w.r.t. MC b quark",
				       30, -1.5, 1.5);
    fHistHadronicTopBottomTruthMatch_phi->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistHadronicTopBottomTruthMatch_phi->SetYTitle("# Events");

    // b-jet eta
    fHistHadronicTopBottomTruthMatch_eta = new TH1F("h_TMHadronicTopb_eta",
				       "Truth Matching of b-jet #eta",
				       30, -1.5, 1.5);
    fHistHadronicTopBottomTruthMatch_eta->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistHadronicTopBottomTruthMatch_eta->SetYTitle("# Events");
    
    gDirectory->cd("..");

    gDirectory->mkdir("Hadron Level Jet","TruthMathching histograms w.r.t. hadron level jet");
    gDirectory->cd("Hadron Level Jet");

    // b-jet pt
    fHistHadronicTopBHJetTruthMatch_pt = new TH1F("h_TMHadronicTopBHJet_pt",
				      "Truth Matching of b-jet p_{T} w.r.t. Hadron Level jet",
				      30, -1.5, 1.5);
    fHistHadronicTopBHJetTruthMatch_pt->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistHadronicTopBHJetTruthMatch_pt->SetYTitle("# Events");
    
    // b-jet phi
    fHistHadronicTopBHJetTruthMatch_phi = new TH1F("h_TMHadronicTopBHJet_phi",
				       "Truth Matching of b-jet #varph w.r.t. Hadron Level jeti",
				       30, -1.5, 1.5);
    fHistHadronicTopBHJetTruthMatch_phi->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistHadronicTopBHJetTruthMatch_phi->SetYTitle("# Events");
    
    // b-jet eta
    fHistHadronicTopBHJetTruthMatch_eta = new TH1F("h_TMHadronicTopBHJet_eta",
				       "Truth Matching of b-jet #eta w.r.t. Hadron Level jet",
				       30, -1.5, 1.5);
    fHistHadronicTopBHJetTruthMatch_eta->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistHadronicTopBHJetTruthMatch_eta->SetYTitle("# Events");

    gDirectory->cd("../..");

    // hadronic W truth matching w.r.t. the true quarks form W decay
    gDirectory->mkdir("Hadronic W","TruthMathching histograms for hadronic W");
    gDirectory->cd("Hadronic W");
    gDirectory->mkdir("quarks","TruthMathching histograms for jets from W decay w.r.t. quarks");
    gDirectory->cd("quarks");

    // hadronic W quark pt
    fHistHadronicTopWQuarkTruthMatch_pt = new TH1F("h_TMHadronicTopWQuark_pt",
				      "Truth Matching of jets p_{T} w.r.t. quarks",
				      30, -1.5, 1.5);
    fHistHadronicTopWQuarkTruthMatch_pt->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistHadronicTopWQuarkTruthMatch_pt->SetYTitle("# Events");
    
    // hadronic W quark phi
    fHistHadronicTopWQuarkTruthMatch_phi = new TH1F("h_TMHadronicTopWQuark_phi",
				       "Truth Matching of jets #varph w.r.t. quarks",
				       30, -1.5, 1.5);
    fHistHadronicTopWQuarkTruthMatch_phi->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistHadronicTopWQuarkTruthMatch_phi->SetYTitle("# Events");
    
    // hadronic W quark eta
    fHistHadronicTopWQuarkTruthMatch_eta = new TH1F("h_TMHadronicTopWQuark_eta",
				       "Truth Matching of jets #eta w.r.t. quarks",
				       30, -1.5, 1.5);
    fHistHadronicTopWQuarkTruthMatch_eta->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistHadronicTopWQuarkTruthMatch_eta->SetYTitle("# Events");

    gDirectory->cd("..");
    gDirectory->mkdir("Hadron Level Jets","TruthMathching histograms for jets from W decay w.r.t. HL jets");
    gDirectory->cd("Hadron Level Jets");

    // hadronic W jet pt
    fHistHadronicTopWHLJetTruthMatch_pt = new TH1F("h_TMHadronicTopWHLJet_pt",
				      "Truth Matching of jets p_{T} w.r.t. HL jets",
				      30, -1.5, 1.5);
    fHistHadronicTopWHLJetTruthMatch_pt->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistHadronicTopWHLJetTruthMatch_pt->SetYTitle("# Events");
    
    // hadronic W jet phi
    fHistHadronicTopWHLJetTruthMatch_phi = new TH1F("h_TMHadronicTopWHLJet_phi",
				       "Truth Matching of jets #varph w.r.t. HL jets",
				       30, -1.5, 1.5);
    fHistHadronicTopWHLJetTruthMatch_phi->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistHadronicTopWHLJetTruthMatch_phi->SetYTitle("# Events");
    
    // hadronic W jet eta
    fHistHadronicTopWHLJetTruthMatch_eta = new TH1F("h_TMHadronicTopWHLJet_eta",
				       "Truth Matching of jets #eta w.r.t. HL jets",
				       30, -1.5, 1.5);
    fHistHadronicTopWHLJetTruthMatch_eta->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistHadronicTopWHLJetTruthMatch_eta->SetYTitle("# Events");

    gDirectory->cd("../..");

    // back to top level
    gDirectory->cd("../..");
 
}

//____________________________________________________________________

Bool_t AtlSgTop_WtChannelFinder::AnalyzeEvent() {
    //
    // Event analysis
    //

    // Clean-up old lists from the previous event first
    fBJets->Clear();
    fWhadJets->Clear();

    // Initialize the current event
    InitEvent();

    fMode = kKinFit; // problem with WtChannel task, got kCutBased for reasons unknown
    
    // Require exactly one lepton,
    // no Veto Lepton, missing Et,
    // three jets, exactly one b jet,
    // no veto b-jet
    // caution: a b-jet could contain a lepton
    // therefore ask for isolated leptons

    // ================
    // Event level cuts
    // ================
    
//     fCutflow_tool->Fill("No Cuts (D3PD)", GetPreTagEvtWeight());
//     fCutflow_tool_AC->Fill("No Cuts (D3PD)", GetPreTagEvtWeight());

    if ( fLepton == 0 ) return kFALSE;
    fCutflow_tool->Fill("Signal Lepton", GetPreTagEvtWeight());
    fCutflow_tool_AC->Fill("Signal Lepton", GetPreTagEvtWeight());

    Bool_t IsFwdElectron = kTRUE;
    if ( ( fLepton->IsElectron() )
	 && ( TMath::Abs(fLepton->Eta()) < 1.5 ) ) IsFwdElectron = kFALSE;
    
    if ( fMET_Mag < fMET_Mag_min  ) return kFALSE;
    fCutflow_tool   ->Fill(Form("Etmiss > %d GeV",(Int_t)fMET_Mag_min), GetPreTagEvtWeight());
    fCutflow_tool_AC->Fill(Form("Etmiss > %d GeV",(Int_t)fMET_Mag_min), GetPreTagEvtWeight());
     
    if ( fNJets < 1 ) return kFALSE;
    fCutflow_tool->Fill("No. of Jets #geq 1", GetPreTagEvtWeight());
    fCutflow_tool_AC->Fill("No. of Jets #geq 1", GetPreTagEvtWeight());

    // Triangle cut: Mt(W) + Etmiss > 60 GeV?
    //
    // Simple W-boson reconstruction
    //
    ReconstructW();

    if ( fLepton->IsElectron() ) {
	// W transverse mass cut to reduce QCD:
	// Mt(W) > 30 GeV?    
	if ( fWmassT <= 30  ) return kFALSE;
	fCutflow_tool->Fill("Mt(W) > 30 GeV",GetPreTagEvtWeight());
	fCutflow_tool_AC->Fill("Mt(W) > 30 GeV",GetPreTagEvtWeight());
    } else if ( fLepton->IsMuon() ) {
	// Triangular cut to reduce QCD:
	// Mt(W) + Etmiss > 60 GeV?    
	if ( (fWmassT + fMET_Mag) <= 60  ) return kFALSE;
	fCutflow_tool->Fill("Mt(W) + Etmiss > 60 GeV",GetPreTagEvtWeight());
	fCutflow_tool_AC->Fill("Mt(W) + Etmiss > 60 GeV",GetPreTagEvtWeight());
    }
        // For acceptance challenges:
    // write out text file with event information!
    if ( fIsAcceptanceChallenge ) {
	fOut
	    << "Run " << fEvent->RunNr()
	    << " " << fEvent->EventNr()
	    << " " << fLepton->Pt()
	    << " " << fLepton->Eta()
	    << " " << fNJets
	    << " " << ((AtlJet*)fJets->At(0))->Pt()
	    << " " << ((AtlJet*)fJets->At(0))->Eta()
	    << " " << ((AtlJet*)fJets->At(1))->Pt()
	    << " " << ((AtlJet*)fJets->At(1))->Eta()
	    << " " << fMET_Mag
	    << " " << fWmassT
	    << endl;
    }

    // START C10 Acceptance challenge - NB analysis is untouched
    if(fNJets == 2){fCutflow_tool_AC->Fill("PreTag-No. of Jets = 2");}
    else if(fNJets == 3){fCutflow_tool_AC->Fill("PreTag-No. of Jets = 3");}
    else if(fNJets == 4){fCutflow_tool_AC->Fill("PreTag-No. of Jets = 4");}
    if ( fBJets->GetEntries()  == 1 ) {
	fCutflow_tool_AC->Fill("Exactly 1 b-Jet");
	if(fNJets == 2){fCutflow_tool_AC->Fill("BTag-No. of Jets = 2");}
	else if(fNJets == 3){fCutflow_tool_AC->Fill("BTag-No. of Jets = 3");}
	else if(fNJets == 4){fCutflow_tool_AC->Fill("BTag-No. of Jets = 4");}
    }
    // END   C10 Acceptance challenge - NB analysis is untouched



    
    if ( fNJets != fNJetsExcl ) return kFALSE;
    fCutflow_tool->Fill(Form("No. of Jets = %d", fNJetsExcl), GetPreTagEvtWeight());

    
    // Fill All PreTag Histograms                 <----------------------------- PRETAG HISTOS
    fHistEtMissMag->Fill(fMET_Mag, GetPreTagEvtWeight());
    fHistEtMissPhi->Fill(fMET_Phi, GetPreTagEvtWeight());
    fHistSumEt->Fill(fEvent->GetEnergySum()->GetRefFinalEM_etightpp_SumEt(),
		     GetPreTagEvtWeight());
    FillJetHistograms();
    FillNonBJetHistograms();
    FillBJetHistograms();
    FillLeptonHistograms();

    fHistEtMissMag_pretag->Fill(fMET_Mag,GetPreTagEvtWeight());
    fHistEtMissPhi_pretag->Fill(fMET_Phi,GetPreTagEvtWeight());
    fHistSumEt_pretag->Fill(fEvent->GetEnergySum()->GetRefFinalEM_etightpp_SumEt(),GetPreTagEvtWeight());
    fHist_HFOR_flag_pretag->Fill((Float_t)(fEvent->GetEventHeader()->GetHFORType()),GetPreTagEvtWeight());
    // Pretag counts for positive and negative leptons
    if ( fLepton->IsPositive() )
	fHistCountLepPlus_pretag->AddBinContent(1,fEvent->GetPreTagEvtWeight());
    else if ( fLepton->IsNegative() )
	fHistCountLepMinus_pretag->AddBinContent(1,fEvent->GetPreTagEvtWeight());

    // Keep track of counts of central and forward electrons in data
    if ( fLepton->IsElectron() ) {
	if ( IsFwdElectron )
	    fHistCountFwdElectrons_pretag->AddBinContent(1,fEvent->GetPreTagEvtWeight());
	else
	    fHistCountCentralElectrons_pretag->AddBinContent(1,fEvent->GetPreTagEvtWeight());
    }
    
    // Fill transverse W mass histograms for validation of QCD estimate
    fHistW_Mt_QCDValidation_pretag->Fill(fWmassT, GetPreTagEvtWeight());
    //
    // Jet bin counts for pretag sample
    //
    if ( fNJets == 1 )
	fHistJetBins_pretag->Fill("1",GetPreTagEvtWeight());
    else if ( fNJets == 2 )
	fHistJetBins_pretag->Fill("2",GetPreTagEvtWeight());
    else if ( fNJets == 3 )
	fHistJetBins_pretag->Fill("3",GetPreTagEvtWeight());
    else if ( fNJets == 4 )
	fHistJetBins_pretag->Fill("4",GetPreTagEvtWeight());
    else if ( fNJets >= 5 )
	fHistJetBins_pretag->Fill("5",GetPreTagEvtWeight());
    else {
	Error("AnalyzeEvent","Wrong # of jets selected. Abort!");
	gSystem->Abort();
    }

    // -----------------
    // B-tag requirement
    // -----------------

     // at least 1 b-jet
    if ( fBJets->GetEntries()  == 0 ) return kFALSE;
    
    fCutflow_tool->Fill("b-Jet", GetPreTagEvtWeight());
    
    // NB:    // NB:    // NB:    // NB:    // NB:    // NB:
    // btag histograms are filled if AT LEAST 1 jet is tagged   <---------------- TAG HISTOS
    FillJetHistograms_BTag();
    FillBJetHistograms_BTag();
    FillNonBJetHistograms_BTag();
    FillLeptonHistograms_BTag();
    
    fHistEtMissMag_tag->Fill(fMET_Mag,GetPreTagEvtWeight());
    fHistEtMissPhi_tag->Fill(fMET_Phi,GetPreTagEvtWeight());
    fHistSumEt_tag->Fill(fEvent->GetEnergySum()->GetRefFinalEM_etightpp_SumEt(),GetPreTagEvtWeight());
    // Fill transverse W mass histograms for validation of QCD estimate
    fHistW_Mt_QCDValidation_tag->Fill(fWmassT, GetPreTagEvtWeight());
    // NB:    // NB:    // NB:    // NB:    // NB:    // NB:
    
    // exactly 1 b-jet
    if ( fBJets->GetEntries()  != 1 ) return kFALSE;
    fCutflow_tool->Fill("Exactly 1 b-Jet", GetPreTagEvtWeight());

    // Tag counts for positive and negative leptons
    if ( fLepton->IsPositive() ) {
	fHistCountLepPlus_tag->AddBinContent(1,fEvent->GetPreTagEvtWeight());
	fHistCountLepAll_tag->AddBinContent(1,fEvent->GetPreTagEvtWeight());
    } else if ( fLepton->IsNegative() ) {
	fHistCountLepMinus_tag->AddBinContent(1,fEvent->GetPreTagEvtWeight());
    	fHistCountLepAll_tag->AddBinContent(1,fEvent->GetPreTagEvtWeight());
    }
    // Keep track of counts of central and forward electrons in data
    if ( fLepton->IsElectron() ) {
	if ( IsFwdElectron )
	    fHistCountFwdElectrons_tag->AddBinContent(1,fEvent->GetPreTagEvtWeight());
	else
	    fHistCountCentralElectrons_tag->AddBinContent(1,fEvent->GetPreTagEvtWeight());
    }

    //
    // Jet bin counts for tag sample
    //
    if ( fNJets == 1 )
	fHistJetBins_tag->Fill("1",GetPreTagEvtWeight());
    else if ( fNJets == 2 )
	fHistJetBins_tag->Fill("2",GetPreTagEvtWeight());
    else if ( fNJets == 3 )
	fHistJetBins_tag->Fill("3",GetPreTagEvtWeight());
    else if ( fNJets == 4 )
	fHistJetBins_tag->Fill("4",GetPreTagEvtWeight());
    else if ( fNJets >= 5 )
	fHistJetBins_tag->Fill("5",GetPreTagEvtWeight());
    else {
	Error("AnalyzeEvent","Wrong # of jets selected. Abort!");
	gSystem->Abort();
    }

    // NNBB NNBB NNBB NNBB NNBB NNBB NNBB NNBB NNBB NNBB NNBB NNBB
    // Passing event selection done here to the parent selector
    // so that only events passing this part of the tool are filled
    // in thistool::FillHistograms()
    // NB: this is done via using the fPassedSelection public
    // data member in AtlSelector::ProcessCut routine
    
    fParent->SetPassedSelection(kTRUE);
    
    // Reconstruct now
    if ( fMode == kKinFit ) {
	ReconstructionKinFit();
    } else if ( fMode == kCutBased ) {
	cout << endl << endl << "Cut Based !!!!!!!!!!!!!!!!!!!" << endl << endl;
	ReconstructionCutBased();
	//gSystem->Abort(0);
    }
    
    return kTRUE;
}

//____________________________________________________________________

void AtlSgTop_WtChannelFinder::InitEvent() {
    //
    // Prepare event analysis
    //



    //---------
    // Leptons
    //---------
    fLepton = 0;

    
    // =============
    // Signal lepton
    // =============
    HepParticle *lep = 0;
    
    // In case no lepton was found leave the function here
    
    if ( fLeptons->GetEntries() > 0 ) {
	
	lep = (HepParticle*)fLeptons->At(0);

	// Electron
	if ( lep->IsElectron()
	     // below implies for SgTop D3PDs: cluster energy over CosH(IDTrack eta) (see TopSlim reader!)
	     && ((AtlElectron*)lep)->Pt() > fElPt_Min ) {
	    fLepton = lep;
	}
	
	// Muon
	if ( lep->IsMuon()
	     && ( lep->Pt() > fMuPt_Min )) {
	    fLepton = lep;
	}
	
	if ( fLepton == 0 ) return;

	
	// Split analysis on jet-electrons into events with signal electrons that
	// are forward and central
	if ( (fLepton->IsElectron()) && fUseLeptonCovRand ) {
	    Float_t lep_eta_abs = TMath::Abs(fLepton->Eta());
	    if ( ( ( lep_eta_abs < 1.5) && fIsFwdElectronMode ) ||
		 ( ( lep_eta_abs >= 1.5) && !fIsFwdElectronMode ) )
		fLepton = 0;
	}
	
	if ( fLepton == 0 ) return;
    } else return;
    
    // Jet-electrons?
    
    if ( fUseLeptonCovRand ) {
	// Get its covariance matrix
	AtlKinFitterTool::GetLeptonCovMatRand(fLepton,fCovLepton);
	AtlKinFitterTool::GetLeptonCovMatRand(fLepton,fCovLepton_had);
    }
    else{
	// Get its covariance matrix
	fLepton->GetCovMatrixPtEtaPhi(fCovLepton);
	fLepton->GetCovMatrixPtEtaPhi(fCovLepton_had);
    }

    // ======
    // Etmiss
    // ======
    fMET_Mag   = fMET.Mod();
    fMET_Phi   = TVector2::Phi_mpi_pi((Double_t)fMET.Phi());

    
    // Set neutrino from missing Et. Since its z component cannot be
    // measured set eta = 0 and E = Pt. The correct PDG code will be
    // given inside the loop over all leptons
    if ( fNeutrino != 0 ) delete fNeutrino;
    fNeutrino = new HepParticle(1, 0., 0., 1., 0., 12);
    fNeutrino->SetPtEtaPhiE(fMET_Mag,
 			    0.,
 			    fMET_Phi,
 			    fMET_Mag);
    


    //------
    // Jets
    //------


    // ==============================================
    // Get list of jets for hadronic W reconstruction
    // ==============================================
    fWhadJets->AddAll(fJets); // alternatively: take them from inverse list NonBJets
    
    
    TIter next_wjet(fWhadJets);
    AtlJet *wjet = 0;

    // Remove low energetic jets, non-central jets and b-jets from the list
    while ( (wjet = (AtlJet*)next_wjet()) ) {
	if ( 
	    (wjet->Pt() <= fWhadJets_Et_min )
	    || ( ( TMath::Abs(wjet->EMScaleEta() + wjet->EMJES_EtaCorr()) ) < fWhadJets_Eta_min )
	    || ( ( TMath::Abs(wjet->EMScaleEta() + wjet->EMJES_EtaCorr()) ) > fWhadJets_Eta_max )
	    || ( wjet->GetTag(fWhadJetTagger)->GetWeight() > fWhadJetWeightMax )
	    )
	    fWhadJets->Remove(wjet);
    }

    
    // Sort list
    fWhadJets->Sort(kSortDescending);

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
	    ( jet->Pt() <= fJets_Et_min )
	    || ( ( TMath::Abs(jet->EMScaleEta() + jet->EMJES_EtaCorr()) ) < fJets_Eta_min )
	    || ( ( TMath::Abs(jet->EMScaleEta() + jet->EMJES_EtaCorr()) ) > fJets_Eta_max )
	    )
	    fJets->Remove(jet);
    }
    
    // Sort list
    fJets->Sort(kSortDescending);

    // Keep track of total number of jets in generic list
    fNJets = fJets->GetEntries();
 
    // ===========================
    // Get refined list of b-jets 
    // ===========================
    fBJets->AddAll(fJets);
    
    TIter next_bjet(fBJets);
    AtlJet *bjet = 0;
    
    // Remove low energetic jets and non-central jets from the list
    while ( (bjet = (AtlJet*)next_bjet()) ) {
      if (
	  ( bjet->GetTag(fBJetTagger)->GetWeight() <= fBJetWeightMin )
	  || ( bjet->Pt() <= fBJets_Et_min )
	  || ( ( TMath::Abs(bjet->EMScaleEta() + bjet->EMJES_EtaCorr()) ) < fBJets_Eta_min )
	  || ( ( TMath::Abs(bjet->EMScaleEta() + bjet->EMJES_EtaCorr()) ) > fBJets_Eta_max )
	  )
      fBJets->Remove(bjet);
    }
    // Sort list
    fBJets->Sort(kSortDescending);

}
//____________________________________________________________________

void AtlSgTop_WtChannelFinder::SetNeutrinoStartingValues() {
    //
    // Provide reasonable starting values for eta and E of the
    // neutrino with the help of the currently set lepton and the
    // b-jet, and TKinFitter::GetEtaNuRoot()
    //
    // !!! Note that fLepton, fBJet and fNeutrino must be set before !!!
    // !!! calling this function                                     !!!
    //
    Double_t EtaStart = 0.;
//    Double_t EStart   = 0.;
    Double_t EStart   = fNeutrino->Et();
    if ( fGuessNeutrinoEtaE ) GuessTopDecayNeutrinoEtaE(EtaStart, EStart, fMET, fLepton, fBJet);

    // Re-set eta and E of neutrino
    fNeutrino->SetPtEtaPhiE(fNeutrino->Pt(),
			    EtaStart,
			    fNeutrino->Phi(),
			    EStart);
}

//____________________________________________________________________

void AtlSgTop_WtChannelFinder::ReconstructionKinFit() {    
    //
    // Single-top Wt-channel event reconstruction (semi-leptonic),
    // no additional jets so far, LO process
    // 
    // Actually this means two channels:
    //   1) leptonic top decay, associated W decays hadronically
    //   2) hadronic top decay, associated W decays leptonically
    //
    // in general:
    //   
    // --> loop over all particle and jet combinations
    // --> perform the fit
    // --> select the combination with the smallest chi^2 as candidate
    //     for a signal event
    //
    // regarding the 1st case:
    // 
    //    - loop over all b jets and perform the reconstruction of the leptonic
    //      top decay -> chi^2_top
    //    - for every such combination loop over all pairs of further jets
    //      and try to reconstruct the associated W -> chi^2_W
    //    - try to reconstruct a hadronic top decay from the jet list except
    //      the already used b jet, for successfull reconstruction: don't choose
    //      this event (ttbar background)
    //    - if ttbar hypothesis is not accepted:
    //      chi^2 = chi^2_top + chi^2_W, choose combination with smallest
    //      chi^2 as a candidate
    //
    // regarding the 2nd case:
    // 
    //    - reconstruct hadronic top decay from one b jet
    //      and two other jets -> chi^2
    //    - again use a ttbar veto:
    //      try to reconstruct a leptonic ttbar decay from
    //      the lepton, the neutrino (missing ET) and one further jet,
    //      again for successfull reconstruction: don't choose
    //      this event
    //    - if ttbar hypothesis is not accepted:
    //      choose combination with the best chi^2
    //
    // then: choose combination with the overall best
    // chi^2 probability as a candidate
    //
    
    // Use 1.e10 as starting value for chi^2,
    // reconstruction successfull if chi^2 gets smaller    
    fchi2LeptonicTop = 1.e10;
    fchi2HadronicTop = 1.e10;
    
    fchi2ttbarBkg = 1.e10;

    fChi2ProbTTBarBkg=-1;
	
    if( fEvent->GetN_TopDecays() > 1 )
    {
	// Find top pair with best (highest) P(Chi2)
	//
	TIter next_top(fEvent->GetTopDecays());
	HepTopDecay* top = 0;
	Double_t  p_chi2 = 0;
  	
	// Loop over all top-quarks
	while ( ( top = (HepTopDecay*)next_top() ) ) {
	    p_chi2 = top->GetProb();
	    if ( p_chi2 > fChi2ProbTTBarBkg ) fChi2ProbTTBarBkg = p_chi2;
	}
	// Speed up interactive
	//Info("ReconstructionKinFit","Found ttbar candidate with P(Chi2) = %f. In run %d evt %d lb %d",fChi2ProbTTBarBkg,fEvent->RunNr(),fEvent->EventNr(),fEvent->LumiBlock());
    }
    
    // Set the correct neutrino type w.r.t. the lepton
    if ( fLepton->IsEPlus() ) {
	fNeutrino->SetPdgCode(12);
    } else if ( fLepton->IsEMinus()   ) {
	fNeutrino->SetPdgCode(-12);
    } else if ( fLepton->IsMuPlus()   ) {
	fNeutrino->SetPdgCode(14);
    } else if ( fLepton->IsMuMinus()  ) {
	fNeutrino->SetPdgCode(-14);
    } else if ( fLepton->IsTauPlus()  ) {
	fNeutrino->SetPdgCode(16);
    } else if ( fLepton->IsTauMinus() ) {
	fNeutrino->SetPdgCode(-16);
	}
    
    // Improve starting values for eta and E of the neutrino and set
    // its covariance matrix	
    SetNeutrinoStartingValues();
    SetCovMatrix(fCovNeutrino, fNeutrino);
    
    // Perform fit for leptonic top decay
    LeptonicTopKinFitReco();
    
    // Perform fit for hadronic top decay
    HadronicTopKinFitReco();

	// now decide whether it was a leptonic or hadronic top decay
	// ttbar veto still to be included
	// use GetDecaychannel() to decide
	// then add the corresponding particles and jets to the event
	// as W decays, top decays and neutrinos

	Int_t channel = GetDecayChannel();
	//if (channel==3) cout << "channel = " << channel <<endl;
	// in case of leptonic top Wt-channel event
	if ( channel == 1 ){
	    // Add neutrino
	    HepParticle *nu_refit = fEvent->AddNeutrino(fNeutrinoLeptonicKinFit->Px(),
							fNeutrinoLeptonicKinFit->Py(),
							fNeutrinoLeptonicKinFit->Z(),
							fNeutrinoLeptonicKinFit->E(),
							fNeutrino->GetPdgCode());
	    // Add leptonic W decay
	    TLorentzVector P_Wlep = *fNeutrinoLeptonicKinFit + *fLeptonLeptonicKinFit;
	    HepWDecay *WdecayLep = fEvent->AddWDecayLNu(P_Wlep.Px(), P_Wlep.Py(),
							P_Wlep.Pz(), P_Wlep.E(),
							fLepton,
							fLeptonLeptonicKinFit->Px(),
							fLeptonLeptonicKinFit->Py(),
							fLeptonLeptonicKinFit->Pz(),
							fLeptonLeptonicKinFit->E(),
							nu_refit,
							HepWDecay::kSgTop_WtChannel);
	    // Add top decay
	    TLorentzVector P_top = P_Wlep + *fBJetLeptonicKinFit;
	    fEvent->AddTopDecay(P_top.Px(), P_top.Py(), P_top.Pz(),
				P_top.E(), WdecayLep, fBestLeptonicBJet,
				fBJetLeptonicKinFit->Px(), fBJetLeptonicKinFit->Py(),
				fBJetLeptonicKinFit->Pz(), fBJetLeptonicKinFit->E(),
				HepTopDecay::kSgTop_WtChannel);
	    // Add hadronic W decay
	    TLorentzVector P_Whad = *fJet1LeptonicKinFit + *fJet2LeptonicKinFit;
	    fEvent->AddWDecayJJ(P_Whad.Px(), P_Whad.Py(),
				P_Whad.Pz(), P_Whad.E(),
				fBestLeptonicJet1, fBestLeptonicJet2,
				fJet1LeptonicKinFit->Px(),
				fJet1LeptonicKinFit->Py(),
				fJet1LeptonicKinFit->Pz(),
				fJet1LeptonicKinFit->E(),
				fJet2LeptonicKinFit->Px(),
				fJet2LeptonicKinFit->Py(),
				fJet2LeptonicKinFit->Pz(),
				fJet2LeptonicKinFit->E(),
				HepWDecay::kSgTop_WtChannel);
	   
	}
	// in case of hadronic top Wt-channel event
	if ( channel == 2 ){
	    // Add hadronic W decay
	    TLorentzVector P_Whad = *fJet1HadronicKinFit + *fJet2HadronicKinFit;
//	    P_Whad.Print();
	    HepWDecay *WdecayHad = fEvent->AddWDecayJJ(P_Whad.Px(), P_Whad.Py(),
						       P_Whad.Pz(), P_Whad.E(),
						       fBestHadronicJet1, fBestHadronicJet2,
						       fJet1HadronicKinFit->Px(),
						       fJet1HadronicKinFit->Py(),
						       fJet1HadronicKinFit->Pz(),
						       fJet1HadronicKinFit->E(),
						       fJet2HadronicKinFit->Px(),
						       fJet2HadronicKinFit->Py(),
						       fJet2HadronicKinFit->Pz(),
						       fJet2HadronicKinFit->E(),
						       HepWDecay::kSgTop_WtChannel);
	    // Add top decay
	    TLorentzVector P_top = P_Whad + *fBJetHadronicKinFit;
// 	    cout<<"P_Whad.M() = " << P_Whad.M() << endl;
// 	    cout<<"P_top.M()  = " << P_top.M() << endl;
	    fEvent->AddTopDecay(P_top.Px(), P_top.Py(), P_top.Pz(),
				P_top.E(), WdecayHad, fBestHadronicBJet,
				fBJetHadronicKinFit->Px(), fBJetHadronicKinFit->Py(),
				fBJetHadronicKinFit->Pz(), fBJetHadronicKinFit->E(),
				HepTopDecay::kSgTop_WtChannel);
//	    P_top.Print();
	}
}

//____________________________________________________________________

void AtlSgTop_WtChannelFinder::LeptonicTopKinFitReco() {

    // Use 1.e10 as starting value for chi^2,
    // reconstruction successful if chi^2 gets smaller
    // Number of combinations that led to convergence

    fN_Converged_lept    = 0;
    fN_NotConverged_lept = 0;
    fN_FailNumeric_lept  = 0;

    
    fChi2 = 1.e10;
    
    // ==================================
    // Loop over all combinations of jets
    // ==================================
 
    TIter next_bjet(fBJets);

    // Loop over all b-jets
    //next_bjet.Reset();
    while( (fBJet = (AtlJet*)next_bjet()) ) {	

      // Set b-jet covariance matrix
	SetCovMatrix(fCovBJet, fBJet);
	
	for( Int_t i=0; i<fWhadJets->GetEntries()-1; i++){
	    fJet1 = (AtlJet*)fWhadJets->At(i);
	    if ( fJet1 == fBJet) continue;
	    SetCovMatrix(fCovWJet1, fJet1);

	    for ( Int_t j=i+1; j<fWhadJets->GetEntries(); j++){
		fJet2 = (AtlJet*)fWhadJets->At(j);
		if ( fJet2 == fBJet) continue;
		SetCovMatrix(fCovWJet2, fJet2);

		// ===============
		// Perform the fit
		// ===============

		DoLeptonicTopFit();

		// Did KinFitter converge ? ("0" means "yes")
		if ( fKinFitter->getStatus() != 0 ) {
		    fN_NotConverged_lept++;
		    continue;
		}
		fN_Converged_lept++;

		fHist_KinFit_NbIterAll        ->Fill(fKinFitter->getNbIter());
		fHist_KinFit_NbIterLeptonicTop->Fill(fKinFitter->getNbIter());
		
		if ( fKinFitter->getS() < fchi2LeptonicTop ){
		    fchi2LeptonicTop = fKinFitter->getS();
		    fNDoFLeptonicTop = fKinFitter->getNDF();
		    fBestLeptonicBJet = fBJet;
		    fBestLeptonicJet1 = fJet1;
		    fBestLeptonicJet2 = fJet2;
	
		    // Get the improved 4-momenta of the outgoing particles
		    const TLorentzVector P_lep_refit  = *(((TAbsFitParticle*)fKinFitter
							   ->getParticle(0))->getCurr4Vec());
		    const TLorentzVector P_nu_refit   = *(((TAbsFitParticle*)fKinFitter
							   ->getParticle(1))->getCurr4Vec());
		    const TLorentzVector P_bjet_refit = *(((TAbsFitParticle*)fKinFitter
							   ->getParticle(2))->getCurr4Vec());
		    const TLorentzVector P_jet1_refit = *(((TAbsFitParticle*)fKinFitter
							   ->getParticle(3))->getCurr4Vec());
		    const TLorentzVector P_jet2_refit = *(((TAbsFitParticle*)fKinFitter
							   ->getParticle(4))->getCurr4Vec());

		    fLeptonLeptonicKinFit   = new TLorentzVector(P_lep_refit);
		    fNeutrinoLeptonicKinFit = new TLorentzVector(P_nu_refit);
		    fBJetLeptonicKinFit     = new TLorentzVector(P_bjet_refit);
		    fJet1LeptonicKinFit     = new TLorentzVector(P_jet1_refit);
		    fJet2LeptonicKinFit     = new TLorentzVector(P_jet2_refit);

		    // Lepton
		    if(fLepton->Pt () !=0) {
			fBeforeLeptonPt   = fLepton->Pt();
			fAfterLeptonPt    = P_lep_refit.Pt();
			fResidualLeptonPt = ( P_lep_refit.Pt()  - fLepton->Pt()  )/fLepton->Pt();
		    }
		    if(fLepton->Eta () !=0) {
			fBeforeLeptonEta   = fLepton->Eta();
			fAfterLeptonEta    = P_lep_refit.Eta();
			fResidualLeptonEta = ( P_lep_refit.Eta()  - fLepton->Eta()  )/fLepton->Eta();
		    }
		    if(fLepton->Phi () !=0) {
			fBeforeLeptonPhi   = fLepton->Phi();
			fAfterLeptonPhi    = P_lep_refit.Phi();
			fResidualLeptonPhi = ( P_lep_refit.Phi()  - fLepton->Phi()  )/fLepton->Phi();
		    }
                    // neutrino

		    if(fNeutrino->Pt () !=0) {
			fBeforeEtMissMag  = fNeutrino->Pt();
			fAfterEtMissMag   = P_nu_refit.Pt();
			fResidualLeptonPt = ( P_nu_refit.Pt()  - fNeutrino->Pt()  )/fNeutrino->Pt();
		    }
		    if(fNeutrino->Eta () !=0) {
			fBeforeEtMissEta = fNeutrino->Eta();
			fAfterEtMissEta  = P_nu_refit.Eta();
		    }
		    if(fNeutrino->Phi () !=0) {
			fBeforeEtMissPhi   = fNeutrino->Phi();
			fAfterEtMissPhi    = P_nu_refit.Phi();
			fResidualEtMissPhi = ( P_nu_refit.Phi()  - fNeutrino->Phi()  )/fNeutrino->Phi();
		    }		    
                    // BJet
		    
		    if(fBJet->Pt () !=0) {
			fBeforeBJetPt = fBJet->Pt();
			fAfterBJetPt  = P_bjet_refit.Pt();
			fResidualBJetPt  = ( P_bjet_refit.Pt()  - fBJet->Pt()  )/fBJet->Pt();
		    }
		    if(fBJet->Eta() !=0) {
			fBeforeBJetEta = fBJet->Eta();
			fAfterBJetEta  = P_bjet_refit.Eta();
			fResidualBJetEta = ( P_bjet_refit.Eta() - fBJet->Eta() )/fBJet->Eta();
		    }
		    if(fBJet->Phi() !=0) {
			fBeforeBJetPhi = fBJet->Phi();
			fAfterBJetPhi  = P_bjet_refit.Phi();
			fResidualBJetPhi = ( P_bjet_refit.Phi() - fBJet->Phi() )/fBJet->Phi();
		    }
		    // Jet 1
		    if(fJet1->Pt () !=0) {
			fBeforeJet1Pt = fJet1->Pt();
			fAfterJet1Pt  = P_jet1_refit.Pt();
			fResidualJet1Pt  = ( P_jet1_refit.Pt()  - fJet1->Pt()  )/fJet1->Pt();
		    }
		    if(fJet1->Eta() !=0) {
			fBeforeJet1Eta = fJet1->Eta();
			fAfterJet1Eta  = P_jet1_refit.Eta();
			fResidualJet1Eta = ( P_jet1_refit.Eta() - fJet1->Eta() )/fJet1->Eta();
		    }
		    if(fJet1->Phi() !=0) {
			fBeforeJet1Phi = fJet1->Phi();
			fAfterJet1Phi  = P_jet1_refit.Phi();
			fResidualJet1Phi = ( P_jet1_refit.Phi() - fJet1->Phi() )/fJet1->Phi();
		    }
		    // Jet 2
		    if(fJet2->Pt () !=0) {
			fBeforeJet2Pt = fJet2->Pt();
			fAfterJet2Pt  = P_jet2_refit.Pt();
			fResidualJet1Pt  = ( P_jet2_refit.Pt()  - fJet2->Pt()  )/fJet2->Pt();
		    }
		    if(fJet2->Eta() !=0) {
			fBeforeJet2Eta = fJet2->Eta();
			fAfterJet2Eta  = P_jet2_refit.Eta();
			fResidualJet2Eta = ( P_jet2_refit.Eta() - fJet2->Eta() )/fJet2->Eta();
		    }
		    if(fJet2->Phi() !=0) {
			fBeforeJet2Phi = fJet2->Phi();
			fAfterJet2Phi  = P_jet2_refit.Phi();
			fResidualJet2Phi = ( P_jet2_refit.Phi() - fJet2->Phi() )/fJet2->Phi();
		    }
		}
	    }
	}
    }
    // ================================
    // KinFitter performance histograms
    // ================================

    fHist_KinFit_NbFitsConverged_LeptonicTop->Fill(fN_Converged_lept, GetPreTagEvtWeight());
    
    // No. of main fits that DID NOT converge in this event
    fHist_KinFit_NbFitsNotConverged->Fill(fN_NotConverged_lept);
    fHist_KinFit_NbFitsNotConverged_LeptonicTop->Fill(fN_NotConverged_lept);

    // No. of main fits that failed numerically in this event
    fHist_KinFit_NbFitsFailedNumerically->Fill(fN_FailNumeric_lept);
    fHist_KinFit_NbFitsFailedNumerically_LeptonicTop->Fill(fN_FailNumeric_lept);
}

//____________________________________________________________________

void AtlSgTop_WtChannelFinder::HadronicTopKinFitReco() {

    // ==================================
    // Loop over all combinations of jets
    // ==================================
    //TIter next_jet(fJets);

    fN_Converged_hadr    = 0;
    fN_NotConverged_hadr = 0;
    fN_FailNumeric_hadr  = 0;

    fChi2 = 1.e10;
	
    TIter next_bjet(fBJets);

    // Loop over all b-jets
    next_bjet.Reset();
    while( (fBJet_had = (AtlJet*)next_bjet()) ) {

	// Set b-jet covariance matrix
	SetCovMatrix(fCovBJet_had, fBJet_had);

	for( Int_t i=0; i<fWhadJets->GetEntries()-1; i++){
	    fJet1_had = (AtlJet*)fWhadJets->At(i);
	    if ( fJet1_had == fBJet_had) continue;
	    SetCovMatrix(fCovWJet1_had, fJet1_had);
	    for ( Int_t j=i+1; j < fWhadJets->GetEntries(); j++){
		fJet2_had = (AtlJet*)fWhadJets->At(j);
		if ( fJet2_had == fBJet_had) continue;
		SetCovMatrix(fCovWJet2_had, fJet2_had);

		// ===============
		// Perform the fit
		// ===============
		
		DoHadronicTopFit();
		// Did KinFitter converge ? ("0" means "yes")
		if ( fKinFitter_HadronicTop->getStatus() != 0 ) {
		    fN_NotConverged_hadr++;
		    continue;
		}
		fN_Converged_hadr++;

		// Debug output of complete covariance matrix
		if ( fVerbosityLevel > 1 ) {
		    // Before fit
		    cout << endl << "Complete covariance matrix before fit:" << endl;
		    cout<<"(?) 0 - 2 = Pt, Eta, Phi (b-jet )" <<endl;
		    cout<<"(?) 3 - 5 = Pt, Eta, Phi (W-jet 1)"<<endl;
		    cout<<"(?) 6 - 8 = Pt, Eta, Phi (W-jet 2)"<<endl;
		}
		fHist_KinFit_NbIterAll->Fill(fKinFitter_HadronicTop->getNbIter());
		fHist_KinFit_NbIterHadronicTop->Fill(fKinFitter_HadronicTop->getNbIter());
		
		if ( fKinFitter_HadronicTop->getS() < fchi2HadronicTop ){
		    fchi2HadronicTop = fKinFitter_HadronicTop->getS();
		    fNDoFHadronicTop = fKinFitter_HadronicTop->getNDF();
		    fBestHadronicBJet = fBJet_had;
		    fBestHadronicJet1 = fJet1_had;
		    fBestHadronicJet2 = fJet2_had;
		       
		    // Get the improved 4-momenta of the outgoing particles
		    const TLorentzVector P_jet1_refit_had = *(((TAbsFitParticle*)fKinFitter_HadronicTop
							       ->getParticle(0))->getCurr4Vec());
		    const TLorentzVector P_jet2_refit_had = *(((TAbsFitParticle*)fKinFitter_HadronicTop
							       ->getParticle(1))->getCurr4Vec());
		    const TLorentzVector P_bjet_refit_had = *(((TAbsFitParticle*)fKinFitter_HadronicTop
							       ->getParticle(2))->getCurr4Vec());

		    fBJetHadronicKinFit     = new TLorentzVector(P_bjet_refit_had);
		    fJet1HadronicKinFit     = new TLorentzVector(P_jet1_refit_had);
		    fJet2HadronicKinFit     = new TLorentzVector(P_jet2_refit_had);
		    if(fBJet_had->Pt()  !=0) {
			fBeforeBJetPt_hadr   = fBJet_had->Pt();
			fAfterBJetPt_hadr    = P_bjet_refit_had.Pt();
			fResidualBJetPt_hadr = ( P_bjet_refit_had.Pt()   - fBJet_had->Pt()  )/fBJet_had->Pt();
		    }
		    if(fBJet_had->Eta() !=0) {
			fBeforeBJetEta_hadr   = fBJet_had->Eta();
			fAfterBJetEta_hadr    = P_bjet_refit_had.Eta();
			fResidualBJetEta_hadr = ( P_bjet_refit_had.Eta()  - fBJet_had->Eta()  )/fBJet_had->Eta();
		    }
		    if(fBJet_had->Phi() !=0) {
			fBeforeBJetPhi_hadr   = fBJet_had->Phi();
			fAfterBJetPhi_hadr    = P_bjet_refit_had.Phi();
			fResidualBJetPhi_hadr = ( P_bjet_refit_had.Phi() - fBJet_had->Phi()  )/fBJet_had->Phi();
		    }
		    if(fJet1_had->Pt()  !=0) {
			fBeforeJet1Pt_hadr   = fJet1_had->Pt();
			fAfterJet1Pt_hadr    = P_jet1_refit_had.Pt();
			fResidualJet1Pt_hadr = ( P_jet1_refit_had.Pt()  - fJet1_had->Pt()  )/fJet1_had->Pt();
		    }
		    if(fJet1_had->Eta() !=0) {
			fBeforeJet1Eta_hadr   = fJet1_had->Eta();
			fAfterJet1Eta_hadr    = P_jet1_refit_had.Eta();
			fResidualJet1Eta_hadr = ( P_jet1_refit_had.Eta() - fJet1_had->Eta() )/fJet1_had->Eta();
		    }
		    if(fJet1_had->Phi() !=0) {
			fBeforeJet1Phi_hadr   = fJet1_had->Phi();
			fAfterJet1Phi_hadr    = P_jet1_refit_had.Phi();
			fResidualJet1Phi_hadr = ( P_jet1_refit_had.Phi() - fJet1_had->Phi() )/fJet1_had->Phi();
		    }
		    if(fJet2_had->Pt()  !=0) {
			fBeforeJet2Pt_hadr   = fJet2_had->Pt();
			fAfterJet2Pt_hadr    = P_jet2_refit_had.Pt();
			fResidualJet2Pt_hadr = ( P_jet2_refit_had.Pt()  - fJet2_had->Pt()  )/fJet2_had->Pt();
		    }
		    if(fJet2_had->Eta() !=0) {
			fBeforeJet2Eta_hadr   = fJet2_had->Eta();
			fAfterJet2Eta_hadr    = P_jet2_refit_had.Eta();
			fResidualJet2Eta_hadr = ( P_jet2_refit_had.Eta() - fJet2_had->Eta() )/fJet2_had->Eta();
		    }
		    if(fJet2_had->Phi() !=0) {
			fBeforeJet2Phi_hadr   = fJet2_had->Phi();
			fAfterJet2Phi_hadr    = P_jet2_refit_had.Phi();
			fResidualJet2Phi_hadr = ( P_jet2_refit_had.Phi() - fJet2_had->Phi() )/fJet2_had->Phi();
		    }
		}
	    }
	}
    }
    // ================================
    // KinFitter performance histograms
    // ================================

    fHist_KinFit_NbFitsConverged_HadronicTop->Fill(fN_Converged_hadr, GetPreTagEvtWeight());

    // No. of main fits that DID NOT converge in this event
    fHist_KinFit_NbFitsNotConverged->Fill(fN_NotConverged_hadr);
    fHist_KinFit_NbFitsNotConverged_HadronicTop->Fill(fN_NotConverged_hadr);
    
    // No. of main fits that failed numerically in this event
    fHist_KinFit_NbFitsFailedNumerically->Fill(fN_FailNumeric_hadr);
    fHist_KinFit_NbFitsFailedNumerically_HadronicTop->Fill(fN_FailNumeric_hadr);
}
//____________________________________________________________________

void AtlSgTop_WtChannelFinder::DoLeptonicTopFit() {
    //
    // Perform the kinematic fit for the current charged lepton,
    // neutrino, b-jet, jet1 and jet2 combination
    //
    // the hypothesis is t -> lepton, neutrino, b-jet
    // and associated W -> jet1, jet2
    //
        
    // Re-set and init fitter
    fKinFitter->reset();
    fKinFitter->setMaxNbIter(fMaxNbIter);    // maximum number of iterations
    fKinFitter->setMaxDeltaS(5.e-5); // maximum deviation of the minimum function within two iterations
    fKinFitter->setMaxF(1.e-4);      // maximum value of constraints
    fKinFitter->setVerbosity(0);     // verbosity level

    // ===========================
    // Get covariance matrices
    // ===========================

    // Lepton
    // =============
    // Get 3-momenta
    // =============
    TVector3 p3_lepton   = TVector3(fLepton->P3());
    TVector3 p3_neutrino = TVector3(fNeutrino->P3());
    TVector3 p3_bjet     = TVector3(fBJet->P3());
    TVector3 p3_jet1     = TVector3(fJet1->P3());
    TVector3 p3_jet2     = TVector3(fJet2->P3());
    
    // ================
    // Define particles
    // ================
    // Necessary for execution of the fit
    TFitParticlePtEtaPhi   KinFit_lepton("KinFit_lepton", "Charged lepton",
					 &p3_lepton, fLepton->Mass("PDG"), &fCovLepton);
    TFitParticlePtThetaPhi KinFit_nu("KinFit_nu", "Neutrino",
				     &p3_neutrino, 0., &fCovNeutrino);
    TFitParticlePtEtaPhi   KinFit_bjet("KinFit_bjet", "BJet",
				       &p3_bjet, fB_Mass, &fCovBJet);
    TFitParticlePtEtaPhi   KinFit_jet1("KinFit_jet1", "Jet1",
				       &p3_jet1, 0., &fCovWJet1);
    TFitParticlePtEtaPhi   KinFit_jet2("KinFit_jet2", "Jet2",
				       &p3_jet2, 0., &fCovWJet2);
    // Add particles to fitter
    fKinFitter->addMeasParticle(&KinFit_lepton);
    fKinFitter->addMeasParticle(&KinFit_nu);
    fKinFitter->setParamUnmeas(&KinFit_nu, 1); // eta (component 1) of neutrino unmeasured
    fKinFitter->addMeasParticle(&KinFit_bjet);
    fKinFitter->addMeasParticle(&KinFit_jet1);
    fKinFitter->addMeasParticle(&KinFit_jet2);
    
 
    
    // ==================
    // Define Constraints
    // ==================

    TFitConstraintM *ConstraintM_Wtop = 0;
    TFitConstraintM *ConstraintM_Wnotop = 0;
    TFitConstraintM *ConstraintM_t = 0;
    
    // Definition of top-quark and W-boson mass constraints
     if ( fModeMass == kGauss ) {

	 ConstraintM_Wtop = new TFitConstraintMGaus("WMassConstraintTop",
				   "WMassConstraintGauss from top",
						    0, 0, fW_Mass, fW_Width);
	 ConstraintM_t = new TFitConstraintMGaus("TopMassConstraint",
						 "TopMassConstraintGauss",
						 0, 0, fTop_Mass, fTop_Width);
	 ConstraintM_Wnotop = new TFitConstraintMGaus("WMassConstraintNoTop",
						      "WMassConstraintGauss not from top",
						      0, 0, fW_Mass, fW_Width);

     }	 else if ( fModeMass == kBW ) {
	 
	 ConstraintM_Wtop = new TFitConstraintMBW2("WMassConstraintTop",
						   "WMassConstraintGauss from top",
						   0, fW_Mass, fW_Width);
	 ConstraintM_t  = new TFitConstraintMBW2("TopMassConstraint",
						 "TopMassConstraintGauss",
						 0, fTop_Mass, fTop_Width);
	 ConstraintM_Wnotop = new TFitConstraintMBW2("WMassConstraintNoTop",
						     "WMassConstraintGauss not from top",
						     0, fW_Mass, fW_Width);
     }     else {
	 cout<<endl;
	 cout<<" fModeMass not set properly. Options are: kBW, kGauss."<<endl;
	 //gSystem->Abort(0);
     }
     
	 
     ConstraintM_Wtop  ->addParticles1(&KinFit_lepton, &KinFit_nu);
     ConstraintM_t     ->addParticles1(&KinFit_lepton, &KinFit_nu, &KinFit_bjet);
     ConstraintM_Wnotop->addParticles1(&KinFit_jet1,   &KinFit_jet2);
     
     fKinFitter->addConstraint(ConstraintM_Wtop);
     fKinFitter->addConstraint(ConstraintM_Wnotop);
     fKinFitter->addConstraint(ConstraintM_t);
     
     
     // =========================
     // Perform Kinematic Fitting
     // =========================
     fKinFitter->fit();
     if ( fKinFitter->getS() < fChi2 ){
	 //
	 // Get mass terms
	 //
	 const TMatrixD *MassParameterWtop   = ConstraintM_Wtop->getParCurr();
	 const TMatrixD *MassParameterTop    = ConstraintM_t->getParCurr();
	 const TMatrixD *MassParameterWnoTop = ConstraintM_Wnotop->getParCurr();
	 
	 Double_t mu_Wtop   = (*MassParameterWtop)  (0, 0); // *** hier geht das???
	 Double_t mu_Top    = (*MassParameterTop)   (0, 0);
	 Double_t mu_WnoTop = (*MassParameterWnoTop)(0, 0);
	 
	 if ( fModeMass == kBW ){
	     fMassConstraintParameterTop    = mu_Top;
	     fMassConstraintParameterWtop   = mu_Wtop;
	     fMassConstraintParameterWnoTop = mu_WnoTop;
	     fMassConstraintParameterAll =
		 fMassConstraintParameterTop*fMassConstraintParameterTop +
		 fMassConstraintParameterWtop*fMassConstraintParameterWtop +
		 fMassConstraintParameterWnoTop*fMassConstraintParameterWnoTop;
	 }
	 else {
	     fMassConstraintParameterTop  = (mu_Top-1)/(fTop_Width)*fTop_Mass;// *** wie wird das verrechnet? Korrekt?
	     fMassConstraintParameterWtop = (mu_Wtop-1)/(fW_Width)*fW_Mass;
	     fMassConstraintParameterWtop = (mu_WnoTop-1)/(fW_Width)*fW_Mass;
	     fMassConstraintParameterAll =
		 fMassConstraintParameterTop*fMassConstraintParameterTop +
		 fMassConstraintParameterWtop*fMassConstraintParameterWtop +
		 fMassConstraintParameterWnoTop*fMassConstraintParameterWnoTop;
	 }

	 //
	 // Get pull values ( pull := [value(fit) - value(initial)] / sqrt[variance of corrections] )
	 //
     
	 const TMatrixD ChargedLeptonPull = *(((TAbsFitParticle*)fKinFitter->getParticle(0))->getPull());
	 const TMatrixD NeutrinoPull      = *(((TAbsFitParticle*)fKinFitter->getParticle(1))->getPull());
	 const TMatrixD BJetPull          = *(((TAbsFitParticle*)fKinFitter->getParticle(2))->getPull());
	 const TMatrixD Jet1Pull          = *(((TAbsFitParticle*)fKinFitter->getParticle(3))->getPull());
	 const TMatrixD Jet2Pull          = *(((TAbsFitParticle*)fKinFitter->getParticle(4))->getPull());

	 fPullLeptonPt  = ChargedLeptonPull(0,0); //cout<<"        fPullLeptonPt  = "<<fPullLeptonPt<<endl;
	 fPullLeptonEta = ChargedLeptonPull(1,0); //cout<<"        fPullLeptonEta = "<<fPullLeptonEta<<endl;
	 fPullLeptonPhi = ChargedLeptonPull(2,0); //cout<<"        fPullLeptonPhi = "<<fPullLeptonPhi<<endl;
	 fPullEtMissMag = NeutrinoPull(0,0);      //cout<<"        fPullEtMissMag = "<<fPullEtMissMag<<endl;
	 fPullEtMissPhi = NeutrinoPull(2,0);      //cout<<"        fPullEtMissPhi = "<<fPullEtMissPhi<<endl;
	 fPullBJetPt  = BJetPull(0,0);            //cout<<"        fPullBJetPt    = "<<fPullBJetPt<<endl;
	 fPullBJetEta = BJetPull(1,0);            //cout<<"        fPullBJetEta   = "<<fPullBJetEta<<endl;
	 fPullBJetPhi = BJetPull(2,0);            //cout<<"        fPullBJetPhi   = "<<fPullBJetPhi<<endl;
	 fPullJet1Pt  = Jet1Pull(0,0);            //cout<<"        fPullJet1Pt    = "<<fPullJet1Pt<<endl;
	 fPullJet1Eta = Jet1Pull(1,0);            //cout<<"        fPullJet1Eta   = "<<fPullJet1Eta<<endl;
	 fPullJet1Phi = Jet1Pull(2,0);            //cout<<"        fPullJet1Phi   = "<<fPullJet1Phi<<endl;
	 fPullJet2Pt  = Jet2Pull(0,0);            //cout<<"        fPullJet2Pt    = "<<fPullJet2Pt<<endl;
	 fPullJet2Eta = Jet2Pull(1,0);            //cout<<"        fPullJet2Eta   = "<<fPullJet2Eta<<endl;
	 fPullJet2Phi = Jet2Pull(2,0);            //cout<<"        fPullJet2Phi   = "<<fPullJet2Phi<<endl;
     }


     if ( fKinFitter->matrixInvFailed() ) fN_FailNumeric_lept++;

     switch ( fKinFitter->getStatus() ) {
	 case 0: {
	     fHist_KinFit_KinFitterStatus->AddBinContent(1);
	     fHist_KinFit_KinFitterStatus_LeptonicTop->AddBinContent(1);
	     break;
	 }
	 case 1: {
	     fHist_KinFit_KinFitterStatus->AddBinContent(2);
	     fHist_KinFit_KinFitterStatus_LeptonicTop->AddBinContent(2);
	     break;
	 }
	 default:{
	     Error("DoLeptonicTopFit","Current KinFitter status not defined for this histogram.");
	 }
     }

    
     // Fitting okay ?
     if ( fKinFitter->getS() < 0. ) {
	 Error("DoLeptonicTopFit",
	       "Chi2 negative!!! Possibly bad input of covariance matrices. Abort!");
	 
	 //gSystem->Abort(0);
	 fLepton->Print();
	 fNeutrino->Print();
	 fBJet->Print();
	 fJet1->Print();
	 fJet2->Print();
     }
     
     delete ConstraintM_Wtop;
     delete ConstraintM_t;
     delete ConstraintM_Wnotop;
}

//____________________________________________________________________

void AtlSgTop_WtChannelFinder::DoHadronicTopFit() {
    //
    // Perform the kinematic fit for the current
    // b-jet, jet1 and jet2 combination
    //
    // the hypothesis is t -> bjet, Whad and
    // Whad -> jet1, jet2
    //

    // Re-set and init fitter
    fKinFitter_HadronicTop->reset();
    fKinFitter_HadronicTop->setMaxNbIter(fMaxNbIter);    // maximum number of iterations
    fKinFitter_HadronicTop->setMaxDeltaS(5.e-5); // maximum deviation of the minimum function within two iterations
    fKinFitter_HadronicTop->setMaxF(1.e-4);      // maximum value of constraints
    fKinFitter_HadronicTop->setVerbosity(0);     // verbosity level

    // =============
    // Get 3-momenta
    // =============
    TVector3 p3_bjet_had = TVector3(fBJet_had->P3());
    TVector3 p3_jet1_had = TVector3(fJet1_had->P3());
    TVector3 p3_jet2_had = TVector3(fJet2_had->P3());
    // ================
    // Define particles
    // ================
    // Necessary for execution of the fit
    TFitParticlePtEtaPhi   KinFit_had_bjet("KinFit_had_bjet", "BJet_had",
					   &p3_bjet_had, fB_Mass, &fCovBJet_had);
    TFitParticlePtEtaPhi   KinFit_had_jet1("KinFit_had_jet1", "Jet1_had",
					   &p3_jet1_had, 0., &fCovWJet1_had);
    TFitParticlePtEtaPhi   KinFit_had_jet2("KinFit_had_jet2", "Jet2_had",
					   &p3_jet2_had, 0., &fCovWJet2_had);
 

    fKinFitter_HadronicTop->addMeasParticle(&KinFit_had_jet1);
    fKinFitter_HadronicTop->addMeasParticle(&KinFit_had_jet2);
    fKinFitter_HadronicTop->addMeasParticle(&KinFit_had_bjet);
    
    // ==================
    // Define Constraints
    // ==================

//      TFitConstraintM *Constraint_had_M_Wtop = 0;
//      TFitConstraintM *Constraint_had_M_t = 0;
    
    
    // Definition of top-quark and W-boson mass constraints

    TFitConstraintMGaus *Constraint_had_M_Wtop =
	new TFitConstraintMGaus("WMassConstraintTop_had",
				"WMassConstraintGauss from hadronic W boson",
				0, 0, fW_Mass, fW_Width);
    TFitConstraintMGaus *Constraint_had_M_t
	= new TFitConstraintMGaus("TopMassConstraint_had",
				  "TopMassConstraintGauss from hadronic top quark",
				  0, 0,  fTop_Mass, fTop_Width);
    
    Constraint_had_M_Wtop->addParticles1(&KinFit_had_jet1, &KinFit_had_jet2);
    Constraint_had_M_t   ->addParticles1(&KinFit_had_jet1, &KinFit_had_jet2, &KinFit_had_bjet);

    fKinFitter_HadronicTop->addConstraint(Constraint_had_M_Wtop);
    fKinFitter_HadronicTop->addConstraint(Constraint_had_M_t);    
    
     // =========================
     // Perform Kinematic Fitting
     // =========================
     fKinFitter_HadronicTop->fit();
     
     if ( fKinFitter_HadronicTop->getS() < fChi2 ){
	 //
	 // Get mass terms
	 //
	 const TMatrixD *MassParameterWtop_had   = Constraint_had_M_Wtop->getParCurr();
	 const TMatrixD *MassParameterTop_had    = Constraint_had_M_t->getParCurr();
	 Double_t mu_Wtop   = (*MassParameterWtop_had)(0, 0); // *** hier geht das???
	 Double_t mu_Top    = (*MassParameterTop_had)(0, 0);
	 
	 if ( fModeMass == kBW ){
	     fMassConstraintParameterTop    = mu_Top;
	     fMassConstraintParameterWtop   = mu_Wtop;
	     fMassConstraintParameterAll =
		 fMassConstraintParameterTop*fMassConstraintParameterTop +
		 fMassConstraintParameterWtop*fMassConstraintParameterWtop;
	}
	else {
	    fMassConstraintParameterTop  = (mu_Top-1)/(fTop_Width)*fTop_Mass;// *** wie wird das verrechnet? Korrekt?
	    fMassConstraintParameterWtop = (mu_Wtop-1)/(fW_Width)*fW_Mass;
	    fMassConstraintParameterAll  =
		fMassConstraintParameterTop*fMassConstraintParameterTop +
		fMassConstraintParameterWtop*fMassConstraintParameterWtop;
	}

	//
	// Get pull values ( pull := [value(fit) - value(initial)] / sqrt[variance of corrections] )
	//
	 
	 const TMatrixD Jet1Pull_had          = *(((TAbsFitParticle*)fKinFitter_HadronicTop->getParticle(0))->getPull());
	 const TMatrixD Jet2Pull_had          = *(((TAbsFitParticle*)fKinFitter_HadronicTop->getParticle(1))->getPull());
	 const TMatrixD BJetPull_had          = *(((TAbsFitParticle*)fKinFitter_HadronicTop->getParticle(2))->getPull());
	 
	fPullBJetPt_had  = BJetPull_had(0,0);           //cout<<"        fPullBJetPt_had    = "<<fPullBJetPt_had<<endl;
	fPullBJetEta_had = BJetPull_had(1,0);           //cout<<"        fPullBJetEta_had   = "<<fPullBJetEta_had<<endl;
	fPullBJetPhi_had = BJetPull_had(2,0);           //cout<<"        fPullBJetPhi_had   = "<<fPullBJetPhi_had<<endl;
	fPullJet1Pt_had  = Jet1Pull_had(0,0);           //cout<<"        fPullJet1Pt_had    = "<<fPullJet1Pt_had<<endl;
	fPullJet1Eta_had = Jet1Pull_had(1,0);           //cout<<"        fPullJet1Eta_had   = "<<fPullJet1Eta_had<<endl;
	fPullJet1Phi_had = Jet1Pull_had(2,0);           //cout<<"        fPullJet1Phi_had   = "<<fPullJet1Phi_had<<endl;
	fPullJet2Pt_had  = Jet2Pull_had(0,0);           //cout<<"        fPullJet2Pt_had    = "<<fPullJet2Pt_had<<endl;
	fPullJet2Eta_had = Jet2Pull_had(1,0);           //cout<<"        fPullJet2Eta_had   = "<<fPullJet2Eta_had<<endl;
	fPullJet2Phi_had = Jet2Pull_had(2,0);           //cout<<"        fPullJet2Phi_had   = "<<fPullJet2Phi_had<<endl;

       }
       
    if ( fKinFitter_HadronicTop->matrixInvFailed() ) fN_FailNumeric_hadr++;

    switch ( fKinFitter_HadronicTop->getStatus() ) {
	case 0: {
	    fHist_KinFit_KinFitterStatus->AddBinContent(1);
	    fHist_KinFit_KinFitterStatus_HadronicTop->AddBinContent(1);
	    break;
	}
	case 1: {
	    fHist_KinFit_KinFitterStatus->AddBinContent(2);
	    fHist_KinFit_KinFitterStatus_HadronicTop->AddBinContent(2);
	    break;
	}
	default:{
	    Error("DoHadronicTopFit","Current KinFitter status not defined for this histogram.");
	}
    }
        
    // Fitting okay ?
    if ( fKinFitter_HadronicTop->getS() < 0. ) {
	Error("DoHadronicTopFit",
	      "Chi2 negative!!! Possibly bad input of covariance matrices. Abort!");
	fBJet_had->Print();
	fJet1_had->Print();
	fJet2_had->Print();
	//gSystem->Abort(0);
    }
    
    delete Constraint_had_M_Wtop;
    delete Constraint_had_M_t;
}

//____________________________________________________________________

Int_t AtlSgTop_WtChannelFinder::GetDecayChannel() {
    // function to decide whether
    // the reconstruction results in a Wt-event
    // with leptonic top decay -> return 1
    // with hadronic top decay -> return 2
    // with no successfull Wt reconstruction
    // or a better reconstruction of the ttbar
    // hypothesis -> return 0
    //
    // decision is made with the help of
    // chi^2 probability
    //

    if ( fchi2LeptonicTop == 1.e10 && fchi2HadronicTop == 1.e10 ) return 0;
    
    fchi2ProbLeptonicTop     = TMath::Prob(fchi2LeptonicTop,
					 fNDoFLeptonicTop);
    fchi2RedLeptonicTop = fchi2LeptonicTop/fNDoFLeptonicTop;
    fchi2ProbHadronicTop     = TMath::Prob(fchi2HadronicTop,
					   fNDoFHadronicTop);
    fchi2RedHadronicTop = fchi2HadronicTop/fNDoFHadronicTop;
    if ( fchi2LeptonicTop != 1.e10 && fchi2HadronicTop != 1.e10 ) 
    fHist_PValue_LeptHadrComp->Fill(fchi2ProbLeptonicTop,fchi2ProbHadronicTop,GetPreTagEvtWeight());
    fHist_Chi2_LeptHadrComp  ->Fill(fchi2LeptonicTop,fchi2HadronicTop,GetPreTagEvtWeight());

    //
    // Cut here is the "ttbar veto"
    // it cuts the event _ONLY_ if the P-value from toppairfinder
    // is _AT_THE_SAME_TIME_ higher than _BOTH_ Wt leptonic && hadronic P-values
    // 
    // -> the decision whether P(chi2_Wt lept) >/< P(chi2_Wt hadr)
    //    is made right afterwards
    //
    //   used to be via simple p-Value comparison
    //if ( fChi2ProbTTBarBkg > fchi2ProbLeptonicTop && fChi2ProbTTBarBkg > fchi2ProbHadronicTop ) return 0;
    
    if (fChi2ProbTTBarBkg > fVetoTTbarProbMin) return 3;
    // resolving W-ambiguity here: leptonic(1) or hadronic(2) top decay ?
    if ( fchi2ProbLeptonicTop < fchi2ProbHadronicTop  ) return 2;
    return 1;
}

//____________________________________________________________________

void AtlSgTop_WtChannelFinder::ReconstructionCutBased() {
    //
    // SgTop_WtChannel event reconstruction (cut-based)
    //
    Error("ReconstructionCutBased",
	  "Not yet implemented. Abort!");
    gSystem->Abort(0);
}

//____________________________________________________________________

void AtlSgTop_WtChannelFinder::FillHistograms() {
    //
    // Fill histograms
    //
    // This method will be called only for events surviving the
    // AnalyzeEvent() routine
    //
    
    // Initialize final step of cut flow (common for Leptonic and Hadronic Top case
    Int_t pchi2_min = (Int_t)(fChi2ProbMin*100);
    TString pchi2_min_str = Form("P(#chi^{2}) > %d",pchi2_min);
    pchi2_min_str.Append("%");

    fCosPhiAngleWTop                     = -999;
    fCosPhiAngleAssociateWTop            = -999;
    fCosPhiAngleAssociateWW              = -999;
    fTopFrame_DThetaStarAssociateWTop    = -999;
    fWFrameCosThetaStar_DeltaWtop_bW     = -999;
    fTopFrameCosThetaStar_DeltaWtop_btop = -999;
    fNDoFWTop     = -999;
    fchi2WTop     = -999; 
    fchi2RedWTop  = -999; 
    fchi2ProbWTop = -999; 
    fRapTop       = -999;
    fEtaTop       = -999;
    fEtaWTop      = -999;
    fEtaNeutrino  = -999.;
    fEtaW_LeptTop = -999.;
    fCosThetaStarLeptNeutrino_LeptTop = -999.;
    fTopFrameCosThetaStarWb_LeptTop   = -999.;
    fCosThetaStar1_LeptTop            = -999.;
    fCosThetaStar2_LeptTop            = -999.;

    fLogPWtBest      = -999.;
    fLogPWtWorst     = -999.;
    fLogPWtHadronic  = -999.;
    fLogPWtLeptonic  = -999.;
    fPWtRatio        = -999.;
    fPWtSum          = -999.;
    fPWtDiff         = -999.;
    fPWtAsymmetry    = -999.;
    fLogPWtRatio     = -999.;
    fLogPWtSum       = -999.;
    fLogPWtDiff      = -999.;
    fLogPWtAsymmetry = -999.;
    
    // Get Top Decay in Wt event if existing
    HepTopDecay* t = 0;

    for ( Int_t i = 0;  i<fEvent->GetN_TopDecays(); i++ ){
	if ( ((HepTopDecay*)fEvent->GetTopDecays()->At(i))->IsSgTop_WtChannel() ){
	    t=(HepTopDecay*)fEvent->GetTopDecays()->At(i);
	}
    }

//    if ( GetDecayChannel() == 3 ) cout << "ttbar veto ? == " << GetDecayChannel() << endl;
    if ( GetDecayChannel() != 3 ) {
	//return;
	//
	// (!)-> Notice that the decision taken in GetDecayChannel() ( i.e. t!=0)
	//       guarantees that the cut above works for both
	//       leptonic and hadronic Wt decays detected.
	//
	
	//SetCutFlow("t#bar{t} Veto (P(#chi^{2}) < 10%)");
	fCutflow_tool->Fill("t#bar{t} Veto", GetPreTagEvtWeight());
	
	
	if ( t ) { //return;x
	    
	    fCutflow_tool->Fill("Convergence", GetPreTagEvtWeight());
	    
	    // No. of main fits that DID converge in this event
	    fHist_KinFit_NbFitsConverged->Fill(fN_Converged_lept+fN_Converged_hadr, GetPreTagEvtWeight());
	    //if ( fChi2ProbTTBarBkg < fVetoTTbarProbMin  ) {
	    

	    TLorentzVector p_BJet = t->GetP_BJet();
	    AtlWDecayJJ* Whad = 0;
	    for ( Int_t i = 0;  i<fEvent->GetN_WDecaysJJ(); i++ ){
		if ( ((AtlWDecayJJ*)fEvent->GetWDecaysJJ()->At(i))->IsSgTop_WtChannel() ){
		    Whad=(AtlWDecayJJ*)fEvent->GetWDecaysJJ()->At(i);
		}
	    }
	    
	    fLogPWtLeptonic = -TMath::Log(fchi2ProbLeptonicTop);
	    fLogPWtHadronic = -TMath::Log(fchi2ProbHadronicTop);
	    fPWtSum         =             fchi2ProbLeptonicTop + fchi2ProbHadronicTop;
	    fPWtDiff        =  TMath::Abs(fchi2ProbLeptonicTop - fchi2ProbHadronicTop);
	    fLogPWtSum      = -TMath::Log(fPWtSum);
	    fLogPWtDiff     = -TMath::Log(fPWtDiff);
	    
	    if (fPWtSum > 0) {
		fPWtAsymmetry    = fPWtDiff/fPWtSum;
		fLogPWtAsymmetry = -TMath::Log(fPWtAsymmetry);
	    } else {
		fPWtAsymmetry    = -999;
		fLogPWtAsymmetry = -999;
	    }

////////////// ========================================================
// !!!!!!	    // Histograms for Wt-channel events with leptonic top decay
////////////// ========================================================
	    
	    if ( GetDecayChannel() == 1 ) {
		fHistPerformanceComparison_before->Fill("Leptonic Top",GetPreTagEvtWeight());
		AtlWDecayLNu* Wlep = (AtlWDecayLNu*)t->GetWDecay();
		
		HepParticle* nu    = Wlep->GetNeutrino();
				
		Float_t mt_Wlep = Wlep->Mperp();
		Float_t mt_top  = t->Mperp();
		Float_t mt_Whad = Whad->Mperp();
		Float_t RapTop  = (t->P()).Rapidity();
		Float_t EtaTop  = (t->P()).Eta();
		Float_t RapWlep = (Wlep->P()).Rapidity();
		Float_t RapWhad = (Whad->P()).Rapidity();
		Float_t EtaWTop = (Wlep->P()).Eta();
		fRapTop = RapTop;
		fEtaTop = EtaTop;
		fEtaWTop = EtaWTop;

		fEtaNeutrino = nu->Eta();
		
		p_System = TVector2((t->P()).Px() + (Whad->P()).Px(),
				    (t->P()).Py() + (Whad->P()).Py());

		pT_System = p_System.Mod();

		fCosPhiAngleWTop          = TMath::Cos((t->P()).Phi()    - (Wlep->P()).Phi());
		fCosPhiAngleAssociateWTop = TMath::Cos((t->P()).Phi()    - (Whad->P()).Phi());
		fCosPhiAngleAssociateWW   = TMath::Cos((Whad->P()).Phi() - (Wlep->P()).Phi());

		// =============
		// Rest Frame Plots
		// =============
		
		//compute boost vectors
		TVector3 boost_w   = -(Wlep->P()).BoostVector();
		TVector3 boost_top = -(t->P()).BoostVector();
		
		TLorentzVector P_W_RF_Top    = Wlep->P();
		TLorentzVector P_BJet_RF_W   = *fBJetLeptonicKinFit;
		TLorentzVector P_BJet_RF_Top = *fBJetLeptonicKinFit;
		TLorentzVector P_lep_RF_W    = *fLeptonLeptonicKinFit;
		TLorentzVector P_nu_RF_W     = *fNeutrinoLeptonicKinFit;

		//boost
		P_W_RF_Top   .Boost(boost_top);
		P_BJet_RF_W  .Boost(boost_w);
		P_BJet_RF_Top.Boost(boost_top);
		P_lep_RF_W   .Boost(boost_w);
		P_nu_RF_W    .Boost(boost_w);
		
		//Decay angle Theta*s
		fWFrameCosThetaStar_DeltaWtop_bW     = TMath::Cos(P_W_RF_Top.   Angle(P_BJet_RF_W.Vect()));
		fTopFrameCosThetaStar_DeltaWtop_btop = TMath::Cos(P_W_RF_Top.   Angle(P_BJet_RF_Top.Vect()));
		fTopFrameCosThetaStarWb_LeptTop      = TMath::Cos(P_BJet_RF_Top.Angle(P_W_RF_Top.Vect()));
		fCosThetaStar1_LeptTop               = TMath::Cos(P_lep_RF_W.   Angle(P_W_RF_Top.Vect()));
		fCosThetaStar2_LeptTop               = TMath::Cos(P_lep_RF_W.   Angle(P_BJet_RF_W.Vect()));
		fCosThetaStarLeptNeutrino_LeptTop    = TMath::Cos(P_lep_RF_W.   Angle(P_nu_RF_W.Vect()));

		// Fill histograms for chi2 and chi2 probability
		fHistLeptonicTopChi2_both    ->Fill(fchi2LeptonicTop,     GetPreTagEvtWeight());
		fHistLeptonicTopChi2Prob_both->Fill(fchi2ProbLeptonicTop, GetPreTagEvtWeight());
		fHistLeptonicTopChi2NDOF_both->Fill(fNDoFLeptonicTop,     GetPreTagEvtWeight());
		fHistChi2_both               ->Fill(fchi2LeptonicTop,     GetPreTagEvtWeight());
		fHistChi2Red_both            ->Fill(fchi2RedLeptonicTop,  GetPreTagEvtWeight());
		fHistChi2Prob_both           ->Fill(fchi2ProbLeptonicTop, GetPreTagEvtWeight());
		fhist_fLogBest1              ->Fill(fchi2ProbLeptonicTop, GetPreTagEvtWeight());
		fhist_fLogBest2              ->Fill(fchi2ProbLeptonicTop, GetPreTagEvtWeight());
		//		fhist_fLogBest3              ->Fill(fchi2ProbLeptonicTop, GetPreTagEvtWeight());
		fHistChi2NDOF_both           ->Fill(fNDoFLeptonicTop,     GetPreTagEvtWeight());

		fNDoFWTop     = fNDoFLeptonicTop; 
		fchi2WTop     = fchi2LeptonicTop;
		fchi2RedWTop  = fchi2RedLeptonicTop;
		fchi2ProbWTop = fchi2ProbLeptonicTop;
		fLogPWtBest   = -TMath::Log(fchi2ProbWTop);		
		if (fPWtSum) {
		    fPWtRatio    = fchi2ProbWTop/fPWtSum;
		    fLogPWtRatio = -TMath::Log(fPWtRatio);
		} else {
		    fPWtRatio    = -999;
		    fLogPWtRatio = -999;
		}
// --> !!		
// P-Value Cut
// Final cut decision: P-Value of the fit mst be higher than X%
		
		if ( fchi2ProbLeptonicTop > fChi2ProbMin ) {
		    fHistPerformanceComparison_after->Fill("Leptonic Top",GetPreTagEvtWeight());
		    //
		    // Jet bin counts for successful reco
		    //
		    if ( fNJets == 1 )
			fHistJetBins_fit->Fill("1",GetPreTagEvtWeight());
		    else if ( fNJets == 2 )
			fHistJetBins_fit->Fill("2",GetPreTagEvtWeight());
		    else if ( fNJets == 3 )
			fHistJetBins_fit->Fill("3",GetPreTagEvtWeight());
		    else if ( fNJets == 4 )
			fHistJetBins_fit->Fill("4",GetPreTagEvtWeight());
		    else if ( fNJets >= 5 )
			fHistJetBins_fit->Fill("5",GetPreTagEvtWeight());
		    else {
			Error("FillKinFitHistograms","Wrong # of jets selected. Abort!");
			gSystem->Abort();
		    }

		    // fill system pt


		    //SetCutFlow("P(#chi^{2}) > X%");
		    fCutflow_tool->Fill(pchi2_min_str, GetPreTagEvtWeight());
		    // Total yield histogram
		    fHistTotalYield_allCuts->AddBinContent(1,GetPreTagEvtWeight());
		    if ( p_System.Mod() < 35) { fHistTotalYield_p10WtPt->AddBinContent(1,GetPreTagEvtWeight());}
		    if ( fchi2ProbLeptonicTop > 0.2 ) {fHistTotalYield_allCuts_cut20->AddBinContent(1,GetPreTagEvtWeight());}
		    if ( fchi2ProbLeptonicTop > 0.3 ) {fHistTotalYield_allCuts_cut30->AddBinContent(1,GetPreTagEvtWeight());}
		    if ( fchi2ProbLeptonicTop > 0.4 ) {fHistTotalYield_allCuts_cut40->AddBinContent(1,GetPreTagEvtWeight());}
		    if ( fchi2ProbLeptonicTop > 0.5 ) {fHistTotalYield_allCuts_cut50->AddBinContent(1,GetPreTagEvtWeight());}
		    if ( fchi2ProbLeptonicTop > 0.6 ) {fHistTotalYield_allCuts_cut60->AddBinContent(1,GetPreTagEvtWeight());}
                    // Speed up interactive
//		    Info("FillHistograms","Found Wt(leptonic top) candidate with P(Chi2) = %f. In run %d evt %d lb %d",fchi2ProbLeptonicTop,fEvent->RunNr(),fEvent->EventNr(),fEvent->LumiBlock());		    
		    
                    // fill chi2 quantities AFTER cut
		    fHistLeptonicTopChi2_both_after     ->Fill(fchi2LeptonicTop,     GetPreTagEvtWeight());
		    fHistLeptonicTopChi2Prob_both_after ->Fill(fchi2ProbLeptonicTop, GetPreTagEvtWeight());
		    fHistLeptonicTopChi2NDOF_both_after ->Fill(fNDoFLeptonicTop,     GetPreTagEvtWeight());
		    fHistChi2_both_after                ->Fill(fchi2LeptonicTop,     GetPreTagEvtWeight());
		    fHistChi2Prob_both_after            ->Fill(fchi2ProbLeptonicTop, GetPreTagEvtWeight());
		    fHistChi2NDOF_both_after            ->Fill(fNDoFLeptonicTop,     GetPreTagEvtWeight());

		    // Fill histograms for electron and muon channel
		    fHistLeptonicTop_Mt  ->Fill(TMath::Abs(mt_top), GetPreTagEvtWeight());
		    fHistLeptonicTop_M   ->Fill(t->M("REC"),        GetPreTagEvtWeight());
		    fHistLeptonicTop_phi ->Fill(t->Phi(),           GetPreTagEvtWeight());
		    fHist_PtTop_nocut    ->Fill(t->Pt(),            GetPreTagEvtWeight());
		    fHistLeptonicTop_pt  ->Fill(t->Pt(),            GetPreTagEvtWeight());
		    fHistLeptonicTop_rap ->Fill(RapTop,             GetPreTagEvtWeight());
		    fHistLeptonicTop_eta ->Fill(EtaTop,             GetPreTagEvtWeight());
		    
		    // Fill neutrino histograms
		    fHistLeptonicTopNuReco_pt  ->Fill(nu->Pt(),  GetPreTagEvtWeight());
		    fHistLeptonicTopNuReco_eta ->Fill(nu->Eta(), GetPreTagEvtWeight());
		    fHistLeptonicTopNuReco_phi ->Fill(nu->Phi(), GetPreTagEvtWeight());
		    fHistLeptonicTopNuReco_E   ->Fill(nu->E(),   GetPreTagEvtWeight());
		    
		    // Fill  histograms of reconstructed leptonic W
		    // coming from a top decay, e and mu channel
		    fHist_W_lept_top_reco_Mt  ->Fill(TMath::Abs(mt_Wlep), GetPreTagEvtWeight());
		    fHist_W_lept_top_reco_M   ->Fill(Wlep->M("REC"),      GetPreTagEvtWeight());
		    fHist_W_lept_top_reco_phi ->Fill(Wlep->Phi(),         GetPreTagEvtWeight());
		    fHist_W_lept_top_reco_pt  ->Fill(Wlep->Pt(),          GetPreTagEvtWeight());
		    fHist_W_lept_top_reco_rap ->Fill(RapWlep,             GetPreTagEvtWeight());
		    fHist_W_lept_top_reco_eta ->Fill(EtaWTop,             GetPreTagEvtWeight());
		    
		    // Fill  histograms of reconstructed hadronic W
		    // that means associated production, e and mu channel
		    fHist_W_hadr_notop_reco_Mt  ->Fill(TMath::Abs(mt_Whad), GetPreTagEvtWeight());
		    fHist_W_hadr_notop_reco_M   ->Fill(Whad->M("REC"),      GetPreTagEvtWeight());
		    fHist_W_hadr_notop_reco_phi ->Fill(Whad->Phi(),         GetPreTagEvtWeight());
		    fHist_W_hadr_notop_reco_pt  ->Fill(Whad->Pt(),          GetPreTagEvtWeight());
		    fHist_W_hadr_notop_reco_rap ->Fill(RapWhad,             GetPreTagEvtWeight());
		    
// 		    // Fill histograms for chi2 and chi2 probability
		    if ( fChi2ProbTTBarBkg != -1 ) {
			fHistLeptonicTopChi2ProbTTBarVeto  ->Fill(fChi2ProbTTBarBkg, GetPreTagEvtWeight());
			fHistLeptonicTopChi2ProbWtAndTTBar ->Fill(fchi2ProbLeptonicTop, fChi2ProbTTBarBkg, GetPreTagEvtWeight());
		    }
		    
		    // Wlep -> e nu
		    if ( Wlep->GetChargedLepton()->IsElectron() ) {
			
			//  Fill electron histograms of reconstructed top-quark
			fHistLeptonicTop_Mt_e  ->Fill(TMath::Abs(mt_top), GetPreTagEvtWeight());
			fHistLeptonicTop_M_e   ->Fill(t->M("REC"),        GetPreTagEvtWeight());
			fHistLeptonicTop_phi_e ->Fill(t->Phi(),           GetPreTagEvtWeight());
			fHistLeptonicTop_pt_e  ->Fill(t->Pt(),            GetPreTagEvtWeight());
			fHistLeptonicTop_rap_e ->Fill(RapTop,             GetPreTagEvtWeight());
			fHistLeptonicTop_eta_e ->Fill(EtaTop,             GetPreTagEvtWeight());
			// Fill neutrino histograms
			fHistLeptonicTopNuReco_pt_e  ->Fill(nu->Pt(),  GetPreTagEvtWeight());
			fHistLeptonicTopNuReco_eta_e ->Fill(nu->Eta(), GetPreTagEvtWeight());
			fHistLeptonicTopNuReco_phi_e ->Fill(nu->Phi(), GetPreTagEvtWeight());
			fHistLeptonicTopNuReco_E_e   ->Fill(nu->E(),   GetPreTagEvtWeight());
			
			// Fill  histograms of reconstructed leptonic W
			// coming from a top decay
			fHist_W_lept_top_reco_Mt_e  ->Fill(TMath::Abs(mt_Wlep), GetPreTagEvtWeight());
			fHist_W_lept_top_reco_M_e   ->Fill(Wlep->M("REC"),      GetPreTagEvtWeight());
			fHist_W_lept_top_reco_phi_e ->Fill(Wlep->Phi(),         GetPreTagEvtWeight());
			fHist_W_lept_top_reco_pt_e  ->Fill(Wlep->Pt(),          GetPreTagEvtWeight());
			fHist_W_lept_top_reco_rap_e ->Fill(RapWlep,             GetPreTagEvtWeight());
			
			// Fill electron histograms of reconstructed hadronic W
			// that means associated production
			fHist_W_hadr_notop_reco_Mt_e  ->Fill(TMath::Abs(mt_Whad), GetPreTagEvtWeight());
			fHist_W_hadr_notop_reco_M_e   ->Fill(Whad->M("REC"),      GetPreTagEvtWeight());
			fHist_W_hadr_notop_reco_phi_e ->Fill(Whad->Phi(),         GetPreTagEvtWeight());
			fHist_W_hadr_notop_reco_pt_e  ->Fill(Whad->Pt(),          GetPreTagEvtWeight());
			fHist_W_hadr_notop_reco_rap_e ->Fill(RapWhad,             GetPreTagEvtWeight());
			
			// Fill histograms for chi2 and chi2 probability
			fHistLeptonicTopChi2_e    ->Fill(fchi2LeptonicTop,     GetPreTagEvtWeight());
			fHistLeptonicTopChi2Prob_e->Fill(fchi2ProbLeptonicTop, GetPreTagEvtWeight());
			
			
			// =======================
			// Fill pull distributions
			// =======================
			// Be aware of the fact that the assignment pt, eta, phi
			// must be changed here when another particle parameterization
			// is used
			
			// electron
			fHistPullElecPt                ->Fill(fPullLeptonPt,   GetPreTagEvtWeight());
			fHistSignalLeptonPt_before_lept->Fill(fBeforeLeptonPt, GetPreTagEvtWeight());
			fHistSignalLeptonPt_after_lept ->Fill(fAfterLeptonPt,  GetPreTagEvtWeight());

			fHistPullElecEta                      ->Fill(fPullLeptonEta,   GetPreTagEvtWeight());
			fHistSignalLeptonEta_before_lept      ->Fill(fBeforeLeptonEta, GetPreTagEvtWeight());
			fHistSignalLeptonEta_before_cells_lept->Fill(fBeforeLeptonEta, GetPreTagEvtWeight());
			fHistSignalLeptonEta_after_lept       ->Fill(fAfterLeptonEta,  GetPreTagEvtWeight());

			fHistPullElecPhi                ->Fill(fPullLeptonPhi,   GetPreTagEvtWeight());
			fHistSignalLeptonPhi_before_lept->Fill(fBeforeLeptonPhi, GetPreTagEvtWeight());
			fHistSignalLeptonPhi_after_lept ->Fill(fAfterLeptonPhi,  GetPreTagEvtWeight());
			// et miss
			fHistPullEtMissMag        ->Fill(fPullEtMissMag,   GetPreTagEvtWeight());
			fHistEtMissMag_before_lept->Fill(fBeforeEtMissMag, GetPreTagEvtWeight());
			fHistEtMissMag_after_lept ->Fill(fAfterEtMissMag,  GetPreTagEvtWeight());
			fHistPullEtMissPhi        ->Fill(fPullEtMissPhi,   GetPreTagEvtWeight());
			fHistEtMissPhi_before_lept->Fill(fBeforeEtMissPhi, GetPreTagEvtWeight());
			fHistEtMissPhi_after_lept ->Fill(fAfterEtMissPhi,  GetPreTagEvtWeight());
			
			// B-jet
			fHistPullBJetPt          ->Fill(fPullBJetPt,    GetPreTagEvtWeight());
			fHistPullBJetEta         ->Fill(fPullBJetEta,   GetPreTagEvtWeight());
			fHistPullBJetPhi         ->Fill(fPullBJetPhi,   GetPreTagEvtWeight());
			fHistBJetEt_before_lept  ->Fill(fBeforeBJetPt,  GetPreTagEvtWeight());
			fHistBJetEta_before_lept ->Fill(fBeforeBJetEta, GetPreTagEvtWeight());
			fHistBJetPhi_before_lept ->Fill(fBeforeBJetPhi, GetPreTagEvtWeight());
			fHistBJetEt_after_lept   ->Fill(fAfterBJetPt,   GetPreTagEvtWeight());
			fHistBJetEta_after_lept  ->Fill(fAfterBJetEta,  GetPreTagEvtWeight());
			fHistBJetPhi_after_lept  ->Fill(fAfterBJetPhi,  GetPreTagEvtWeight());


			
			// jet 1
			fHistPullJet1Pt          ->Fill(fPullJet1Pt,     GetPreTagEvtWeight());
			fHistPullJet1Eta         ->Fill(fPullJet1Eta,    GetPreTagEvtWeight());
			fHistPullJet1Phi         ->Fill(fPullJet1Phi,    GetPreTagEvtWeight());
			fHistResidualJet1Pt      ->Fill(fResidualJet1Pt, GetPreTagEvtWeight());
			fHistResidualJet1Eta     ->Fill(fResidualJet1Eta,GetPreTagEvtWeight());
			fHistResidualJet1Phi     ->Fill(fResidualJet1Phi,GetPreTagEvtWeight());
			fHistJet1Et_before_lept  ->Fill(fBeforeJet1Pt,   GetPreTagEvtWeight());
			fHistJet1Eta_before_lept ->Fill(fBeforeJet1Eta,  GetPreTagEvtWeight());
			fHistJet1Phi_before_lept ->Fill(fBeforeJet1Phi,  GetPreTagEvtWeight());
			fHistJet1Et_after_lept   ->Fill(fAfterJet1Pt,    GetPreTagEvtWeight());
			fHistJet1Eta_after_lept  ->Fill(fAfterJet1Eta,   GetPreTagEvtWeight());
			fHistJet1Phi_after_lept  ->Fill(fAfterJet1Phi,   GetPreTagEvtWeight());

			
			// jet 2
			fHistPullJet2Pt     ->Fill(fPullJet2Pt,        GetPreTagEvtWeight());
			fHistPullJet2Eta    ->Fill(fPullJet2Eta,       GetPreTagEvtWeight());
			fHistPullJet2Phi    ->Fill(fPullJet2Phi,       GetPreTagEvtWeight());
			fHistResidualJet2Pt ->Fill(fResidualJet2Pt,    GetPreTagEvtWeight());
			fHistResidualJet2Eta->Fill(fResidualJet2Eta,   GetPreTagEvtWeight());
			fHistResidualJet2Phi->Fill(fResidualJet2Phi,   GetPreTagEvtWeight());
			fHistJet2Et_before_lept ->Fill(fBeforeJet2Pt,  GetPreTagEvtWeight());
			fHistJet2Eta_before_lept->Fill(fBeforeJet2Eta, GetPreTagEvtWeight());
			fHistJet2Phi_before_lept->Fill(fBeforeJet2Phi, GetPreTagEvtWeight());
			fHistJet2Et_after_lept  ->Fill(fAfterJet2Pt,   GetPreTagEvtWeight());
			fHistJet2Eta_after_lept ->Fill(fAfterJet2Eta,  GetPreTagEvtWeight());
			fHistJet2Phi_after_lept ->Fill(fAfterJet2Phi,  GetPreTagEvtWeight());
		    }
		    
		    // Wlep -> mu nu
		    if ( Wlep->GetChargedLepton()->IsMuon() ) {

			// =======================
			// Fill pull distributions
			// =======================
			// Be aware of the fact that the assignment pt, eta, phi
			// must be changed here when another particle parameterization
			// is used
			
			// muon
			fHistPullMuonPt                 ->Fill(fPullLeptonPt,   GetPreTagEvtWeight());
			fHistSignalLeptonPt_before_lept ->Fill(fBeforeLeptonPt, GetPreTagEvtWeight());
			fHistSignalLeptonPt_after_lept  ->Fill(fAfterLeptonPt,  GetPreTagEvtWeight());

			fHistPullMuonEta                       ->Fill(fPullLeptonEta,   GetPreTagEvtWeight());
			fHistSignalLeptonEta_before_lept       ->Fill(fBeforeLeptonEta, GetPreTagEvtWeight());
			fHistSignalLeptonEta_before_cells_lept ->Fill(fBeforeLeptonEta, GetPreTagEvtWeight());
			fHistSignalLeptonEta_after_lept        ->Fill(fAfterLeptonEta,  GetPreTagEvtWeight());

			fHistPullMuonPhi                 ->Fill(fPullLeptonPhi,   GetPreTagEvtWeight());
			fHistSignalLeptonPhi_before_lept ->Fill(fBeforeLeptonPhi, GetPreTagEvtWeight());
			fHistSignalLeptonPhi_after_lept  ->Fill(fAfterLeptonPhi,  GetPreTagEvtWeight());
			// et miss
			fHistPullEtMissMag         ->Fill(fPullEtMissMag,   GetPreTagEvtWeight());
			fHistEtMissMag_before_lept ->Fill(fBeforeEtMissMag, GetPreTagEvtWeight());
			fHistEtMissMag_after_lept  ->Fill(fAfterEtMissMag,  GetPreTagEvtWeight());
			fHistPullEtMissPhi         ->Fill(fPullEtMissPhi,   GetPreTagEvtWeight());
			fHistEtMissPhi_before_lept ->Fill(fBeforeEtMissPhi, GetPreTagEvtWeight());
			fHistEtMissPhi_after_lept  ->Fill(fAfterEtMissPhi,  GetPreTagEvtWeight());
			
			// B-jet
			fHistPullBJetPt          ->Fill(fPullBJetPt,    GetPreTagEvtWeight());
			fHistPullBJetEta         ->Fill(fPullBJetEta,   GetPreTagEvtWeight());
			fHistPullBJetPhi         ->Fill(fPullBJetPhi,   GetPreTagEvtWeight());
			fHistBJetEt_before_lept  ->Fill(fBeforeBJetPt,  GetPreTagEvtWeight());
			fHistBJetEta_before_lept ->Fill(fBeforeBJetEta, GetPreTagEvtWeight());
			fHistBJetPhi_before_lept ->Fill(fBeforeBJetPhi, GetPreTagEvtWeight());
			fHistBJetEt_after_lept   ->Fill(fAfterBJetPt,   GetPreTagEvtWeight());
			fHistBJetEta_after_lept  ->Fill(fAfterBJetEta,  GetPreTagEvtWeight());
			fHistBJetPhi_after_lept  ->Fill(fAfterBJetPhi,  GetPreTagEvtWeight());
			
			// jet 1
			fHistPullJet1Pt         ->Fill(fPullJet1Pt,      GetPreTagEvtWeight());
			fHistPullJet1Eta        ->Fill(fPullJet1Eta,     GetPreTagEvtWeight());
			fHistPullJet1Phi        ->Fill(fPullJet1Phi,     GetPreTagEvtWeight());
			fHistResidualJet1Pt     ->Fill(fResidualJet1Pt,  GetPreTagEvtWeight());
			fHistResidualJet1Eta    ->Fill(fResidualJet1Eta, GetPreTagEvtWeight());
			fHistResidualJet1Phi    ->Fill(fResidualJet1Phi, GetPreTagEvtWeight());
			fHistJet1Et_before_lept ->Fill(fBeforeJet1Pt,    GetPreTagEvtWeight());
			fHistJet1Eta_before_lept->Fill(fBeforeJet1Eta,   GetPreTagEvtWeight());
			fHistJet1Phi_before_lept->Fill(fBeforeJet1Phi,   GetPreTagEvtWeight());
			fHistJet1Et_after_lept  ->Fill(fAfterJet1Pt,     GetPreTagEvtWeight());
			fHistJet1Eta_after_lept ->Fill(fAfterJet1Eta,    GetPreTagEvtWeight());
			fHistJet1Phi_after_lept ->Fill(fAfterJet1Phi,    GetPreTagEvtWeight());

			// jet 2
			fHistPullJet2Pt         ->Fill(fPullJet2Pt,      GetPreTagEvtWeight());
			fHistPullJet2Eta        ->Fill(fPullJet2Eta,     GetPreTagEvtWeight());
			fHistPullJet2Phi        ->Fill(fPullJet2Phi,     GetPreTagEvtWeight());
			fHistResidualJet2Pt     ->Fill(fResidualJet2Pt,  GetPreTagEvtWeight());
			fHistResidualJet2Eta    ->Fill(fResidualJet2Eta, GetPreTagEvtWeight());
			fHistResidualJet2Phi    ->Fill(fResidualJet2Phi, GetPreTagEvtWeight());
			fHistJet2Et_before_lept ->Fill(fBeforeJet2Pt,    GetPreTagEvtWeight());
			fHistJet2Eta_before_lept->Fill(fBeforeJet2Eta,   GetPreTagEvtWeight());
			fHistJet2Phi_before_lept->Fill(fBeforeJet2Phi,   GetPreTagEvtWeight());
			fHistJet2Et_after_lept  ->Fill(fAfterJet2Pt,     GetPreTagEvtWeight());
			fHistJet2Eta_after_lept ->Fill(fAfterJet2Eta,    GetPreTagEvtWeight());
			fHistJet2Phi_after_lept ->Fill(fAfterJet2Phi,    GetPreTagEvtWeight());

			//  Fill electron histograms of reconstructed top-quark
			fHistLeptonicTop_Mt_mu  ->Fill(TMath::Abs(mt_top), GetPreTagEvtWeight());
			fHistLeptonicTop_M_mu   ->Fill(t->M("REC"),        GetPreTagEvtWeight());
			fHistLeptonicTop_phi_mu ->Fill(t->Phi(),           GetPreTagEvtWeight());
			fHistLeptonicTop_pt_mu  ->Fill(t->Pt(),            GetPreTagEvtWeight());
			fHistLeptonicTop_rap_mu ->Fill(RapTop,             GetPreTagEvtWeight());
			fHistLeptonicTop_eta_mu ->Fill(EtaTop,             GetPreTagEvtWeight());
			// Fill neutrino histograms
			fHistLeptonicTopNuReco_pt_mu  ->Fill(nu->Pt(),  GetPreTagEvtWeight());
			fHistLeptonicTopNuReco_eta_mu ->Fill(nu->Eta(), GetPreTagEvtWeight());
			fHistLeptonicTopNuReco_phi_mu ->Fill(nu->Phi(), GetPreTagEvtWeight());
			fHistLeptonicTopNuReco_E_mu   ->Fill(nu->E(),   GetPreTagEvtWeight());
			
			// Fill  histograms of reconstructed leptonic W
			// coming from a top decay
			fHist_W_lept_top_reco_Mt_mu  ->Fill(TMath::Abs(mt_Wlep), GetPreTagEvtWeight());
			fHist_W_lept_top_reco_M_mu   ->Fill(Wlep->M("REC"),      GetPreTagEvtWeight());
			fHist_W_lept_top_reco_phi_mu ->Fill(Wlep->Phi(),         GetPreTagEvtWeight());
			fHist_W_lept_top_reco_pt_mu  ->Fill(Wlep->Pt(),          GetPreTagEvtWeight());
			fHist_W_lept_top_reco_rap_mu ->Fill(RapWlep,             GetPreTagEvtWeight());
			
			// Fill electron histograms of reconstructed hadronic W
			// that means associated production
			fHist_W_hadr_notop_reco_Mt_mu  ->Fill(TMath::Abs(mt_Whad), GetPreTagEvtWeight());
			fHist_W_hadr_notop_reco_M_mu   ->Fill(Whad->M("REC"),      GetPreTagEvtWeight());
			fHist_W_hadr_notop_reco_phi_mu ->Fill(Whad->Phi(),         GetPreTagEvtWeight());
			fHist_W_hadr_notop_reco_pt_mu  ->Fill(Whad->Pt(),          GetPreTagEvtWeight());
			fHist_W_hadr_notop_reco_rap_mu ->Fill(RapWhad,             GetPreTagEvtWeight());
			
			// Fill histograms for chi2 and chi2 probability
			fHistLeptonicTopChi2_mu     ->Fill(fchi2LeptonicTop,     GetPreTagEvtWeight());
			fHistLeptonicTopChi2Prob_mu ->Fill(fchi2ProbLeptonicTop, GetPreTagEvtWeight());
		    }
		}
	    }
////////////// ========================================================
// !!!!!!      Histograms for Wt-channel events with hadronic top decay
////////////// ========================================================


	    
	    if ( GetDecayChannel() == 2) {
		fHistPerformanceComparison_before->Fill("Hadronic Top",GetPreTagEvtWeight());
		
		Float_t mt_top   = t->Mperp();
		Float_t mt_Whad  = Whad->Mperp();		
		Float_t  RapTop  = (t->P()).Rapidity();
		Float_t  EtaTop  = (t->P()).Eta();
		Float_t  RapWhad = (Whad->P()).Rapidity();
		Float_t  EtaWTop = (Whad->P()).Eta();
		fRapTop  = RapTop;
		fEtaTop  = EtaTop;
		fEtaWTop = EtaWTop;
		Float_t mt_Wlep = 2*fLepton->Pt()*fMET_Mag;
		mt_Wlep *= (1-TMath::Cos(fLepton->Phi()-fMET_Phi));
		mt_Wlep = TMath::Sqrt(mt_Wlep);
		Float_t  px_Wlep  = fLepton->Px() + fMET.Px();
		Float_t  py_Wlep  = fLepton->Py() + fMET.Py();
		TVector2 p_Wlep   = TVector2(px_Wlep, py_Wlep);
		Float_t  pt_Wlep  = p_Wlep.Mod();
		Float_t  phi_Wlep = p_Wlep.Phi();

	    
		p_System = TVector2((t->P()).Px() + px_Wlep,
				    (t->P()).Py() + py_Wlep);
		fCosPhiAngleWTop          = TMath::Cos((t->P()).Phi()    - (Whad->P()).Phi());
		fCosPhiAngleAssociateWTop = TMath::Cos((t->P()).Phi()    - phi_Wlep);
		fCosPhiAngleAssociateWW   = TMath::Cos((Whad->P()).Phi() - phi_Wlep);

		// =============
		// Rest Frame Plots
		// =============
		
		//compute boost vectors
		TVector3 boost_w   = -(Whad->P()).BoostVector();
		TVector3 boost_top = -(t->P()).BoostVector();
		
		TLorentzVector P_W_RF_Top    = Whad->P() ;
		TLorentzVector P_BJet_RF_W   = *fBJetHadronicKinFit;
		TLorentzVector P_BJet_RF_Top = *fBJetHadronicKinFit;
		
		//boost
		P_W_RF_Top.Boost(boost_top);
		P_BJet_RF_W.Boost(boost_w);
		P_BJet_RF_Top.Boost(boost_top);
		
		//Decay angle Theta1* and Theta2*
		fWFrameCosThetaStar_DeltaWtop_bW     = TMath::Cos(P_W_RF_Top.Angle(P_BJet_RF_W.Vect()));
		fTopFrameCosThetaStar_DeltaWtop_btop = TMath::Cos(P_W_RF_Top.Angle(P_BJet_RF_Top.Vect()));

		// Fill histograms for chi2 and chi2 probability
		fHistHadronicTopChi2_both    ->Fill(fchi2HadronicTop, GetPreTagEvtWeight());
		fHistHadronicTopChi2Prob_both->Fill(fchi2ProbHadronicTop, GetPreTagEvtWeight());
		fHistHadronicTopChi2NDOF_both->Fill(fNDoFHadronicTop, GetPreTagEvtWeight());

		fHistChi2_both    ->Fill(fchi2HadronicTop,    GetPreTagEvtWeight());
		fHistChi2Red_both ->Fill(fchi2RedHadronicTop, GetPreTagEvtWeight());
		fHistChi2Prob_both->Fill(fchi2ProbHadronicTop,GetPreTagEvtWeight());
		fhist_fLogBest1->Fill(fchi2ProbHadronicTop, GetPreTagEvtWeight());
		fhist_fLogBest2->Fill(fchi2ProbHadronicTop, GetPreTagEvtWeight());
		//fhist_fLogBest3->Fill(fchi2ProbHadronicTop, GetPreTagEvtWeight());
		fHistChi2NDOF_both->Fill(fNDoFHadronicTop,    GetPreTagEvtWeight());

		fNDoFWTop     = fNDoFHadronicTop;
		fchi2WTop     = fchi2HadronicTop;
		fchi2RedWTop  = fchi2RedHadronicTop;
		fchi2ProbWTop = fchi2ProbHadronicTop;
		fLogPWtBest   = -TMath::Log(fchi2ProbWTop);
		
		if (fPWtSum) {
		    fPWtRatio    = fchi2ProbWTop/fPWtSum;
		    fLogPWtRatio = -TMath::Log(fPWtRatio);
		} else {
		    fPWtRatio    = -999;
		    fLogPWtRatio = -999;
		}
                // fill mass-chi2 comparison histograms
		fHist_W_hadr_top_reco_comp_MChi2 ->Fill(Whad->M("REC"),fchi2HadronicTop,     GetPreTagEvtWeight());
		fHist_W_hadr_top_reco_comp_MPChi2->Fill(Whad->M("REC"),fchi2ProbHadronicTop, GetPreTagEvtWeight());
		fHist_t_hadr_top_reco_comp_MChi2 ->Fill(t->M("REC"),fchi2HadronicTop,        GetPreTagEvtWeight());
		fHist_t_hadr_top_reco_comp_MPChi2->Fill(t->M("REC"),fchi2ProbHadronicTop,    GetPreTagEvtWeight());
		fHist_comp_MM                    ->Fill(Whad->M("REC"),t->M("REC"),          GetPreTagEvtWeight());

		fHist_W_hadr_top_reco_comp_Mptbjet    ->Fill(fBeforeBJetPt_hadr, Whad->M("REC"),   GetPreTagEvtWeight());
		fHist_W_hadr_top_reco_comp_Metabjet   ->Fill(fBeforeBJetEta_hadr,Whad->M("REC"),   GetPreTagEvtWeight());
		fHist_W_hadr_top_reco_comp_chi2ptbjet ->Fill(fBeforeBJetPt_hadr, fchi2HadronicTop, GetPreTagEvtWeight());
		fHist_W_hadr_top_reco_comp_chi2etabjet->Fill(fBeforeBJetEta_hadr,fchi2HadronicTop, GetPreTagEvtWeight());

		fHist_W_hadr_top_reco_comp_Mptjet1 ->Fill(fBeforeJet1Pt_hadr, Whad->M("REC"), GetPreTagEvtWeight());
		fHist_W_hadr_top_reco_comp_Metajet1->Fill(fBeforeJet1Eta_hadr,Whad->M("REC"), GetPreTagEvtWeight());
		fHist_W_hadr_top_reco_comp_Mptjet2 ->Fill(fBeforeJet2Pt_hadr, Whad->M("REC"), GetPreTagEvtWeight());
		fHist_W_hadr_top_reco_comp_Metajet2->Fill(fBeforeJet2Eta_hadr,Whad->M("REC"), GetPreTagEvtWeight());

		fHist_W_hadr_top_reco_comp_chi2ptjet1 ->Fill(fBeforeJet1Pt_hadr, fchi2HadronicTop, GetPreTagEvtWeight());
		fHist_W_hadr_top_reco_comp_chi2etajet1->Fill(fBeforeJet1Eta_hadr,fchi2HadronicTop, GetPreTagEvtWeight());
		fHist_W_hadr_top_reco_comp_chi2ptjet2 ->Fill(fBeforeJet2Pt_hadr, fchi2HadronicTop, GetPreTagEvtWeight());
		fHist_W_hadr_top_reco_comp_chi2etajet2->Fill(fBeforeJet2Eta_hadr,fchi2HadronicTop, GetPreTagEvtWeight());

		
// --> !!		
// P-Value Cut
// Final cut decision: P-Value of the fit mst be higher than X%
		
		if ( fchi2ProbHadronicTop > fChi2ProbMin ) {
		    fHistPerformanceComparison_after->Fill("Hadronic Top",GetPreTagEvtWeight());
		    //
		    // Jet bin counts for successful reco
		    //
		    if ( fNJets == 1 )
			fHistJetBins_fit->Fill("1",GetPreTagEvtWeight());
		    else if ( fNJets == 2 )
			fHistJetBins_fit->Fill("2",GetPreTagEvtWeight());
		    else if ( fNJets == 3 )
			fHistJetBins_fit->Fill("3",GetPreTagEvtWeight());
		    else if ( fNJets == 4 )
			fHistJetBins_fit->Fill("4",GetPreTagEvtWeight());
		    else if ( fNJets >= 5 )
			fHistJetBins_fit->Fill("5",GetPreTagEvtWeight());
		    else {
			Error("FillKinFitHistograms","Less than 2 jets selected. Abort!");
			gSystem->Abort();
		    }


		    //SetCutFlow("P(#chi^{2}) > X%");
		    fCutflow_tool->Fill(pchi2_min_str, GetPreTagEvtWeight());
		    // Total yield histogram
		    fHistTotalYield_allCuts->AddBinContent(1,GetPreTagEvtWeight());
		    if ( p_System.Mod() < 35) { fHistTotalYield_p10WtPt->AddBinContent(1,GetPreTagEvtWeight());}
		    if ( fchi2ProbHadronicTop > 0.2 ) {fHistTotalYield_allCuts_cut20->AddBinContent(1,GetPreTagEvtWeight());}
		    if ( fchi2ProbHadronicTop > 0.3 ) {fHistTotalYield_allCuts_cut30->AddBinContent(1,GetPreTagEvtWeight());}
		    if ( fchi2ProbHadronicTop > 0.4 ) {fHistTotalYield_allCuts_cut40->AddBinContent(1,GetPreTagEvtWeight());}
		    if ( fchi2ProbHadronicTop > 0.5 ) {fHistTotalYield_allCuts_cut50->AddBinContent(1,GetPreTagEvtWeight());}
		    if ( fchi2ProbHadronicTop > 0.6 ) {fHistTotalYield_allCuts_cut60->AddBinContent(1,GetPreTagEvtWeight());}

		    // Speed up interactive
                    // Info("FillHistograms","Found Wt(hadronic top) candidate with P(Chi2) = %f. In run %d evt %d lb %d",fchi2ProbHadronicTop,fEvent->RunNr(),fEvent->EventNr(),fEvent->LumiBlock());

                    // fill chi2 quantities AFTER cut
		    fHistHadronicTopChi2_both_after     ->Fill(fchi2HadronicTop,     GetPreTagEvtWeight());
		    fHistHadronicTopChi2Prob_both_after ->Fill(fchi2ProbHadronicTop, GetPreTagEvtWeight());
		    fHistHadronicTopChi2NDOF_both_after ->Fill(fNDoFHadronicTop,     GetPreTagEvtWeight());
		    fHistChi2_both_after                ->Fill(fchi2HadronicTop,     GetPreTagEvtWeight());
		    fHistChi2Prob_both_after            ->Fill(fchi2ProbHadronicTop, GetPreTagEvtWeight());
		    fHistChi2NDOF_both_after            ->Fill(fNDoFHadronicTop,     GetPreTagEvtWeight());
		
		    // Fill histograms for electron and muon channel
		    // Fill electron histograms of reconstructed top-quark
		    fHistHadronicTop_Mt  ->Fill(TMath::Abs(mt_top), GetPreTagEvtWeight());
		    fHistHadronicTop_M   ->Fill(t->M("REC"),        GetPreTagEvtWeight());
		    fHistHadronicTop_phi ->Fill(t->Phi(),           GetPreTagEvtWeight());
		    fHist_PtTop_nocut->Fill(t->Pt(),            GetPreTagEvtWeight());
		    fHistHadronicTop_pt  ->Fill(t->Pt(),            GetPreTagEvtWeight());
		    fHistHadronicTop_rap ->Fill(RapTop,             GetPreTagEvtWeight());
		    fHistHadronicTop_eta ->Fill(EtaTop,             GetPreTagEvtWeight());
		    // Fill  histograms of reconstructed hadronic W
		    // coming from a top decay, e and mu channel
		    fHist_W_hadr_top_reco_Mt  ->Fill(TMath::Abs(mt_Whad), GetPreTagEvtWeight());
		    fHist_W_hadr_top_reco_M   ->Fill(Whad->M("REC"),      GetPreTagEvtWeight());
		    fHist_W_hadr_top_reco_phi ->Fill(Whad->Phi(),         GetPreTagEvtWeight());
		    fHist_W_hadr_top_reco_pt  ->Fill(Whad->Pt(),          GetPreTagEvtWeight());
		    fHist_W_hadr_top_reco_rap ->Fill(RapWhad,             GetPreTagEvtWeight());

		    fHist_W_hadr_top_reco_comp_MChi2_after ->Fill(Whad->M("REC"),fchi2HadronicTop,     GetPreTagEvtWeight());
		    fHist_W_hadr_top_reco_comp_MPChi2_after->Fill(Whad->M("REC"),fchi2ProbHadronicTop, GetPreTagEvtWeight());
		    fHist_t_hadr_top_reco_comp_MChi2_after ->Fill(t->M("REC"),   fchi2HadronicTop,     GetPreTagEvtWeight());
		    fHist_t_hadr_top_reco_comp_MPChi2_after->Fill(t->M("REC"),   fchi2ProbHadronicTop, GetPreTagEvtWeight());

		    fHist_comp_MM_after->Fill(Whad->M("REC"),t->M("REC"), GetPreTagEvtWeight());

                    // fill histograms with information regarding the leptonic
		    // decay of the associated W
		    fHist_HadronicTop_ChargedLepton_pt ->Fill(fLepton->Pt(),  GetPreTagEvtWeight());
		    fHist_HadronicTop_ChargedLepton_eta->Fill(fLepton->Eta(), GetPreTagEvtWeight());
		    fHist_HadronicTop_ChargedLepton_phi->Fill(fLepton->Phi(), GetPreTagEvtWeight());

		    fHist_HadronicTop_EtMiss_mag->Fill(fMET_Mag, GetPreTagEvtWeight());
		    fHist_HadronicTop_EtMiss_phi->Fill(fMET_Phi, GetPreTagEvtWeight());
		    
		    fHist_W_lept_notop_reco_Mt ->Fill(mt_Wlep,  GetPreTagEvtWeight());
 		    fHist_W_lept_notop_reco_pt ->Fill(pt_Wlep,  GetPreTagEvtWeight());
 		    fHist_W_lept_notop_reco_phi->Fill(phi_Wlep, GetPreTagEvtWeight());

		    fHistPullBJetPt_had ->Fill(fPullBJetPt_had,  GetPreTagEvtWeight());
		    fHistPullBJetEta_had->Fill(fPullBJetEta_had, GetPreTagEvtWeight());
		    fHistPullBJetPhi_had->Fill(fPullBJetPhi_had, GetPreTagEvtWeight());
		    
		    fHistPullJet1Pt_had ->Fill(fPullJet1Pt_had,  GetPreTagEvtWeight());
		    fHistPullJet1Eta_had->Fill(fPullJet1Eta_had, GetPreTagEvtWeight());
		    fHistPullJet1Phi_had->Fill(fPullJet1Phi_had, GetPreTagEvtWeight());
		    
		    fHistPullJet2Pt_had ->Fill(fPullJet2Pt_had,  GetPreTagEvtWeight());
		    fHistPullJet2Eta_had->Fill(fPullJet2Eta_had, GetPreTagEvtWeight());
		    fHistPullJet2Phi_had->Fill(fPullJet2Phi_had, GetPreTagEvtWeight());
		    
		    fHistResidualBJetPt_had ->Fill(fResidualBJetPt_hadr,  GetPreTagEvtWeight());
		    fHistResidualBJetEta_had->Fill(fResidualBJetEta_hadr, GetPreTagEvtWeight());
		    fHistResidualBJetPhi_had->Fill(fResidualBJetPhi_hadr, GetPreTagEvtWeight());
		    fHistResidualJet1Pt_had ->Fill(fResidualJet1Pt_hadr,  GetPreTagEvtWeight());
		    fHistResidualJet1Eta_had->Fill(fResidualJet1Eta_hadr, GetPreTagEvtWeight());
		    fHistResidualJet1Phi_had->Fill(fResidualJet1Phi_hadr, GetPreTagEvtWeight());
		    fHistResidualJet2Pt_had ->Fill(fResidualJet2Pt_hadr,  GetPreTagEvtWeight());
		    fHistResidualJet2Eta_had->Fill(fResidualJet2Eta_hadr, GetPreTagEvtWeight());
		    fHistResidualJet2Phi_had->Fill(fResidualJet2Phi_hadr, GetPreTagEvtWeight());
		    
		    fHistBJetEt_before_hadr ->Fill(fBeforeBJetPt_hadr,   GetPreTagEvtWeight());
		    fHistBJetEta_before_hadr->Fill(fBeforeBJetEta_hadr,  GetPreTagEvtWeight());
		    fHistBJetPhi_before_hadr->Fill(fBeforeBJetPhi_hadr,  GetPreTagEvtWeight());
		    fHistBJetEt_after_hadr  ->Fill(fAfterBJetPt_hadr,    GetPreTagEvtWeight());
		    fHistBJetEta_after_hadr ->Fill(fAfterBJetEta_hadr,   GetPreTagEvtWeight());
		    fHistBJetPhi_after_hadr ->Fill(fAfterBJetPhi_hadr,   GetPreTagEvtWeight());
		    
		    fHistJet1Et_before_hadr ->Fill(fBeforeJet1Pt_hadr,   GetPreTagEvtWeight());
		    fHistJet1Eta_before_hadr->Fill(fBeforeJet1Eta_hadr,  GetPreTagEvtWeight());
		    fHistJet1Phi_before_hadr->Fill(fBeforeJet1Phi_hadr,  GetPreTagEvtWeight());
		    fHistJet1Et_after_hadr  ->Fill(fAfterJet1Pt_hadr,    GetPreTagEvtWeight());
		    fHistJet1Eta_after_hadr ->Fill(fAfterJet1Eta_hadr,   GetPreTagEvtWeight());
		    fHistJet1Phi_after_hadr ->Fill(fAfterJet1Phi_hadr,   GetPreTagEvtWeight());
		    
		    fHistJet2Et_before_hadr ->Fill(fBeforeJet2Pt_hadr,   GetPreTagEvtWeight());
		    fHistJet2Eta_before_hadr->Fill(fBeforeJet2Eta_hadr,  GetPreTagEvtWeight());
		    fHistJet2Phi_before_hadr->Fill(fBeforeJet2Phi_hadr,  GetPreTagEvtWeight());
		    fHistJet2Et_after_hadr  ->Fill(fAfterJet2Pt_hadr,    GetPreTagEvtWeight());
		    fHistJet2Eta_after_hadr ->Fill(fAfterJet2Eta_hadr,   GetPreTagEvtWeight()); 
		    fHistJet2Phi_after_hadr ->Fill(fAfterJet2Phi_hadr,   GetPreTagEvtWeight());
		    
		    
		    // Wlep -> e nu
		    if ( fLepton->IsElectron() ) {
			
			//  Fill electron histograms of reconstructed top-quark
			fHistHadronicTop_Mt_e  ->Fill(TMath::Abs(mt_top), GetPreTagEvtWeight());
			fHistHadronicTop_M_e   ->Fill(t->M("REC"),        GetPreTagEvtWeight());
			fHistHadronicTop_phi_e ->Fill(t->Phi(),           GetPreTagEvtWeight());
			fHistHadronicTop_pt_e  ->Fill(t->Pt(),            GetPreTagEvtWeight());
			fHistHadronicTop_rap_e ->Fill(RapTop,             GetPreTagEvtWeight());
			fHistHadronicTop_eta_e ->Fill(EtaTop,             GetPreTagEvtWeight());
			// Fill  histograms of reconstructed hadronic W
			// coming from a top decay
			fHist_W_hadr_top_reco_Mt_e  ->Fill(TMath::Abs(mt_Whad), GetPreTagEvtWeight());
			fHist_W_hadr_top_reco_M_e   ->Fill(Whad->M("REC"),      GetPreTagEvtWeight());
			fHist_W_hadr_top_reco_phi_e ->Fill(Whad->Phi(),         GetPreTagEvtWeight());
			fHist_W_hadr_top_reco_pt_e  ->Fill(Whad->Pt(),          GetPreTagEvtWeight());
			fHist_W_hadr_top_reco_rap_e ->Fill(RapWhad,             GetPreTagEvtWeight());
			
			// fill histograms with information regarding the electronic
			// decay of the associated W
			fHist_HadronicTop_electron_pt ->Fill(fLepton->Pt(),  GetPreTagEvtWeight());
			fHist_HadronicTop_electron_eta->Fill(fLepton->Eta(), GetPreTagEvtWeight());
			fHist_HadronicTop_electron_phi->Fill(fLepton->Phi(), GetPreTagEvtWeight());
			fHist_W_lept_notop_reco_Mt_e  ->Fill(mt_Wlep,        GetPreTagEvtWeight());
			
			// Fill histograms for chi2 and chi2 probability
			fHistHadronicTopChi2_e     ->Fill(fchi2HadronicTop,     GetPreTagEvtWeight());
			fHistHadronicTopChi2Prob_e ->Fill(fchi2ProbHadronicTop, GetPreTagEvtWeight());
			
		    }
		    // Wlep -> mu nu
		    if ( fLepton->IsMuon() ) {
					    
			//  Fill muon histograms of reconstructed top-quark
			fHistHadronicTop_Mt_mu  ->Fill(TMath::Abs(mt_top), GetPreTagEvtWeight());
			fHistHadronicTop_M_mu   ->Fill(t->M("REC"),        GetPreTagEvtWeight());
			fHistHadronicTop_phi_mu ->Fill(t->Phi(),           GetPreTagEvtWeight());
			fHistHadronicTop_pt_mu  ->Fill(t->Pt(),            GetPreTagEvtWeight());
			fHistHadronicTop_rap_mu ->Fill(RapTop,             GetPreTagEvtWeight());
			fHistHadronicTop_eta_mu ->Fill(EtaTop,             GetPreTagEvtWeight());
			// Fill  histograms of reconstructed hadronic W
			// coming from a top decay
			fHist_W_hadr_top_reco_Mt_mu  ->Fill(TMath::Abs(mt_Whad), GetPreTagEvtWeight());
			fHist_W_hadr_top_reco_M_mu   ->Fill(Whad->M("REC"),      GetPreTagEvtWeight());
			fHist_W_hadr_top_reco_phi_mu ->Fill(Whad->Phi(),         GetPreTagEvtWeight());
			fHist_W_hadr_top_reco_pt_mu  ->Fill(Whad->Pt(),          GetPreTagEvtWeight());
			fHist_W_hadr_top_reco_rap_mu ->Fill(RapWhad,             GetPreTagEvtWeight());
			
			// fill histograms with information regarding the muonic
			// decay of the associated W
			fHist_HadronicTop_muon_pt    ->Fill(fLepton->Pt(),  GetPreTagEvtWeight());
			fHist_HadronicTop_muon_eta   ->Fill(fLepton->Eta(), GetPreTagEvtWeight());
			fHist_HadronicTop_muon_phi   ->Fill(fLepton->Phi(), GetPreTagEvtWeight());
			fHist_W_lept_notop_reco_Mt_mu->Fill(mt_Wlep,        GetPreTagEvtWeight());
			
			// Fill histograms for chi2 and chi2 probability
			fHistHadronicTopChi2_mu     ->Fill(fchi2HadronicTop,     GetPreTagEvtWeight());
			fHistHadronicTopChi2Prob_mu ->Fill(fchi2ProbHadronicTop, GetPreTagEvtWeight());	    

		    } // close IsMuon
		}     // close prob cut
	    }         // close getdecaychan(2)
	}             // close ttbar veto
    }                 // close Exist Wt TopDecay
    
    fRunNumber   = fEvent->RunNr();
    fEventNumber = fEvent->EventNr();
    fLumiBlock   = fEvent->LumiBlock();
    fmc_channel_number = fEvent->GetChannelNumber();
    ftop_hfor_type = fEvent->GetHFORType();
    
    fhist_fCosPhiAngleWTop                     ->Fill(fCosPhiAngleWTop,                     GetPreTagEvtWeight());
    fhist_fCosPhiAngleAssociateWTop            ->Fill(fCosPhiAngleAssociateWTop,            GetPreTagEvtWeight());
    fhist_fCosPhiAngleAssociateWW              ->Fill(fCosPhiAngleAssociateWW,              GetPreTagEvtWeight());
    fhist_fWFrameCosThetaStar_DeltaWtop_bW     ->Fill(fWFrameCosThetaStar_DeltaWtop_bW,     GetPreTagEvtWeight());
    fhist_fTopFrameCosThetaStar_DeltaWtop_btop ->Fill(fTopFrameCosThetaStar_DeltaWtop_btop, GetPreTagEvtWeight());

// Filling ntuple for combined analysis and histograms related
    
    if (fProduceCommonTree) {
	
	fhist_fCosThetaStar1_LeptTop           ->Fill(fCosThetaStar1_LeptTop,           GetPreTagEvtWeight());
	fhist_fCosThetaStar2_LeptTop           ->Fill(fCosThetaStar2_LeptTop,           GetPreTagEvtWeight());
	fhist_fCosThetaStarLeptNeutrino_LeptTop->Fill(fCosThetaStarLeptNeutrino_LeptTop,GetPreTagEvtWeight());
	fhist_fLogBest        ->Fill(fLogPWtBest,     GetPreTagEvtWeight());
	fhist_fLogLept        ->Fill(fLogPWtLeptonic, GetPreTagEvtWeight());
	fhist_fLogHadr        ->Fill(fLogPWtHadronic, GetPreTagEvtWeight());
	fhist_fPWtRatio       ->Fill(fPWtRatio,       GetPreTagEvtWeight());
	fhist_fPWtSum         ->Fill(fPWtSum,         GetPreTagEvtWeight());
	fhist_fPWtDiff        ->Fill(fPWtDiff,        GetPreTagEvtWeight());
	fhist_fPWtAsymmetry   ->Fill(fPWtAsymmetry,   GetPreTagEvtWeight());
	fhist_fLogPWtRatio    ->Fill(fLogPWtRatio,    GetPreTagEvtWeight());
	fhist_fLogPWtSum      ->Fill(fLogPWtSum,      GetPreTagEvtWeight());
	fhist_fLogPWtDiff     ->Fill(fLogPWtDiff,     GetPreTagEvtWeight());
	fhist_fLogPWtAsymmetry->Fill(fLogPWtAsymmetry,GetPreTagEvtWeight());

	if ((p_System.Mod() > 0. ) && (p_System.Mod() < 100. )) {fHist_WtSystem_Pt->Fill(pT_System,GetPreTagEvtWeight());}
	if ((p_System.Mod() > 0. ) && (p_System.Mod() < 200. )) {fHist_WtSystem_Pt_nocut ->Fill(pT_System,GetPreTagEvtWeight());}
	if( (fEtaTop > -6.) && (fEtaTop < 6.)) { fHist_EtaTop_nocut->Fill(fEtaTop,GetPreTagEvtWeight());}

	if ((p_System.Mod() > 0. ) && (p_System.Mod() < 150. ) &&
	    (fEtaTop > -6.) && (fEtaTop < 6.)){
	    fHistWtSystem_vs_TopEta_sliced_fine     ->Fill(pT_System,fEtaTop, GetPreTagEvtWeight());
	    fHistWtSystem_vs_TopEta_sliced          ->Fill(pT_System,fEtaTop, GetPreTagEvtWeight());
	    fHistWtSystem_vs_TopEta_sliced_fine_more->Fill(pT_System,fEtaTop, GetPreTagEvtWeight());
	    fHistWtSystem_vs_TopEta_sliced_more     ->Fill(pT_System,fEtaTop, GetPreTagEvtWeight());
	    fHistWtSystem_vs_TopEta_sliced_less     ->Fill(pT_System,fEtaTop, GetPreTagEvtWeight());

	}
	// fhist_fLogBest_logscale->Fill(fLogPWtBest,GetPreTagEvtWeight());
	// fhist_fLogLept_logscale->Fill(fLogPWtLeptonic,GetPreTagEvtWeight());
	// fhist_fLogHadr_logscale->Fill(fLogPWtHadronic,GetPreTagEvtWeight());
	// fhist_fPWtRatio_logscale       ->Fill(fPWtRatio,       GetPreTagEvtWeight());
	// fhist_fPWtSum_logscale         ->Fill(fPWtSum,         GetPreTagEvtWeight());
	// fhist_fPWtDiff_logscale        ->Fill(fPWtDiff,        GetPreTagEvtWeight());
	// fhist_fPWtAsymmetry_logscale   ->Fill(fPWtAsymmetry,   GetPreTagEvtWeight());
	// fhist_fLogPWtRatio_logscale    ->Fill(fLogPWtRatio,    GetPreTagEvtWeight());
	// fhist_fLogPWtSum_logscale      ->Fill(fLogPWtSum,      GetPreTagEvtWeight());
	// fhist_fLogPWtDiff_logscale     ->Fill(fLogPWtDiff,     GetPreTagEvtWeight());
	// fhist_fLogPWtAsymmetry_logscale->Fill(fLogPWtAsymmetry,GetPreTagEvtWeight());

	fCommonTree->Fill();
    }
}
//____________________________________________________________________

void AtlSgTop_WtChannelFinder::SetCutDefaults() {
    //
    // Selection cut defaults
    //

    // channel of events for which the analysis is performed
    fDecayChannel = 3;
    
    fMaxNbIter         = 100;

    fSFSystMode        = kNone;

    fIsAcceptanceChallenge = kFALSE;

    // Mode of finder
    fMode     = kKinFit;
    fModeMass = kGauss;
    fLeptonMode        = kElectronMode;
    fIsFwdElectronMode = kFALSE;
    fVerbosityLevel    = 0;
    fGuessNeutrinoEtaE = kFALSE;
    fProduceCommonTree = kTRUE;

    // P(chi2) cut
    fChi2ProbMin = 0.1;
    
    // Make sure all cuts are correctly set:
    fSignalSampleMode=kTRUE;
    fIsAcceptanceChallenge=kFALSE;
    fVerbosityLevel=0;
    fMode=kKinFit;
    fMaxNbIter=100;
    fTruthMatch=kFALSE;
//    fNJetsMin=n_jets_excl;
//    fNJetsMax=n_jets_excl;
//    fNJetsExcl=n_jets_excl;
    fBJetTagger=AtlBTag::kMV1;
    fBJets_Et_min=25.0;
    fBJets_Eta_min=-2.5;
    fBJets_Eta_max=2.5;


    // ======================
    // Pole masses and widths
    // ======================

    // Triangle Cut

    fMET_Mag_min = 25.;
    fW_Mass   = 80.4; 
    fW_Width  = 2.14;
    fTop_Mass = 172.9;
    fTop_Width = 1.99; // quite uncertain, not measured, depends on mass
        



    // =======
    // Leptons
    // =======
    fElPt_Min = 30.;   
    fMuPt_Min = 30.;   

    
    // ====
    // Jets
    // ====
    
    // Jets
    fNJetsMin        = 3;
    fNJetsMax        = 5;
    fJets_Et_min     = 25.0;
    fJets_Eta_min    = -2.5;
    fJets_Eta_max    = 2.5;
    
    // BJets
    fBJets_Et_min    = 25.0;
    fBJets_Eta_min   = -2.5;
    fBJets_Eta_max   = 2.5;
    fBJetTagger      = AtlBTag::kMV1;
    fBJetWeightMin   = 0.602; // 2.2 - 57% w.p // 0.35 - 70% w.p.

    // Jets from hadronic W
    fWhadJets_Et_min  = 25;
    fWhadJets_Eta_min = -2.5; 
    fWhadJets_Eta_max = 2.5;
    fWhadJetTagger    = AtlBTag::kMV1;
    fWhadJetWeightMax = fBJetWeightMin;


    // ==========
    // ttbar veto
    // ==========
    fVetoTTbarProbMin = 0.001; // -------> Not used since ttbar veto is done by p(chi2) comparison

}

//____________________________________________________________________

void AtlSgTop_WtChannelFinder::Terminate() {
    //
    // Terminate Single-top Wt-channel finder
    //
    // Close output text file for acceptance challenges
    fOut.close(); 
    // Kill empty bins of CutFlow Histo
    fHistCutFlowWeighted->LabelsDeflate("x");
    fHistCutFlowUnweighted->LabelsDeflate("x");
    // use reasonable Y axis range to prevent confusion
    fHistCutFlowWeighted->SetAxisRange(0, fHistCutFlowWeighted->GetMaximumStored(), "Y" );
    fHistCutFlowUnweighted->SetAxisRange(0, fHistCutFlowUnweighted->GetMaximumStored(), "Y" );

    //
    // Fill histogram with slices of fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet in eta direction
    // for better signal discrimination:
    //
    fOutputFile->cd();
    gDirectory->cd();
    gDirectory->cd("NtupleStuff");


    // Fine binning
    Int_t nbins =  nbins_pt_fine*n_eta_slices; // 25 pt X 11 eta
    fHistWtSystem_rolled_TopEta_sliced_fine = 
      new TH1F("h_WtSystem_rolled_EtaTop_fine",
	       "Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of W. jet (Range 0. to 150.), in 11 slices of #eta of top",
	       nbins, 0., 1500.);
    fHistWtSystem_rolled_TopEta_sliced_fine->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{W}|");
    fHistWtSystem_rolled_TopEta_sliced_fine->SetZTitle("Number of Entries");

    Int_t offset = 0;
    Int_t curr_bin = 0;
    Double_t curr_err = 0;
    Float_t curr_value = 0;

    for ( Int_t i = 1; i <= n_eta_slices; i++ ) {
      for ( Int_t j = 1; j <= nbins_pt_fine; j++ ) {
	curr_bin = offset+j;
	curr_value = fHistWtSystem_vs_TopEta_sliced_fine->GetBinContent(j,i);
	curr_err   = fHistWtSystem_vs_TopEta_sliced_fine->GetBinError(j,i);
	fHistWtSystem_rolled_TopEta_sliced_fine->SetBinContent(curr_bin, curr_value);
	fHistWtSystem_rolled_TopEta_sliced_fine->SetBinError(curr_bin, curr_err);
      }
      offset += nbins_pt_fine;
    }


    // Coarse binning
    nbins =  nbins_pt_coarse*n_eta_slices; // 15 pt X 11 eta
    fHistWtSystem_rolled_TopEta_sliced = 
	new TH1F("h_WtSystem_rolled_EtaTop",
		 "Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of top (Range 0. to 150.), in 11 slices of #eta of top",
		 nbins, 0., 1500.);
    fHistWtSystem_rolled_TopEta_sliced->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{W}|");
    fHistWtSystem_rolled_TopEta_sliced->SetZTitle("Number of Entries");

    offset = 0;
    curr_bin = 0;
    curr_err = 0;
    curr_value = 0;

    for ( Int_t i = 1; i <= n_eta_slices; i++ ) {
      for ( Int_t j = 1; j <= nbins_pt_coarse; j++ ) {
	curr_bin = offset+j;
	curr_value = fHistWtSystem_vs_TopEta_sliced->GetBinContent(j,i);
	curr_err = fHistWtSystem_vs_TopEta_sliced->GetBinError(j,i);
	fHistWtSystem_rolled_TopEta_sliced->SetBinContent(curr_bin, curr_value);
	fHistWtSystem_rolled_TopEta_sliced->SetBinError(curr_bin, curr_err);
      }
      offset += nbins_pt_coarse;
    }

    fHistWtSystem_rolled_TopEta_sliced_less = 
	new TH1F("h_WtSystem_rolled_EtaTop_less",
		 "Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of top (Range 0. to 150.), in 11 slices of #eta of top",
		 nbins, 0., 1500.);
    fHistWtSystem_rolled_TopEta_sliced_less->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{W}|");
    fHistWtSystem_rolled_TopEta_sliced_less->SetZTitle("Number of Entries");
    
    offset = 0;
    curr_bin = 0;
    curr_err = 0;
    curr_value = 0;

    for ( Int_t i = 1; i <= 5; i++ ) {
      for ( Int_t j = 1; j <= nbins_pt_coarse; j++ ) {
	curr_bin = offset+j;
	curr_value = fHistWtSystem_vs_TopEta_sliced_less->GetBinContent(j,i);
	curr_err = fHistWtSystem_vs_TopEta_sliced_less->GetBinError(j,i);
	fHistWtSystem_rolled_TopEta_sliced_less->SetBinContent(curr_bin, curr_value);
	fHistWtSystem_rolled_TopEta_sliced_less->SetBinError(curr_bin, curr_err);
      }
      offset += nbins_pt_coarse;
    }


    
    // REPLICATE WITH DIFFERENT ETA SLICING RANGE

    // Fine binning
    nbins =  nbins_pt_fine*n_eta_slices_more; // 25 pt X 11 eta
    fHistWtSystem_rolled_TopEta_sliced_fine_more = 
	new TH1F("h_WtSystem_rolled_EtaTop_fine_more",
		 "Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of W. jet (Range 0. to 150.), in 11 slices of #eta of top",
		 nbins, 0., 1500.);
    fHistWtSystem_rolled_TopEta_sliced_fine_more->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{W}|");
    fHistWtSystem_rolled_TopEta_sliced_fine_more->SetZTitle("Number of Entries");
    
    offset = 0;
    curr_bin = 0;
    curr_err = 0;
    curr_value = 0;

    for ( Int_t i = 1; i <= n_eta_slices_more; i++ ) {
      for ( Int_t j = 1; j <= nbins_pt_fine; j++ ) {
	curr_bin = offset+j;
	curr_value = fHistWtSystem_vs_TopEta_sliced_fine_more->GetBinContent(j,i);
	curr_err   = fHistWtSystem_vs_TopEta_sliced_fine_more->GetBinError(j,i);
	fHistWtSystem_rolled_TopEta_sliced_fine_more->SetBinContent(curr_bin, curr_value);
	fHistWtSystem_rolled_TopEta_sliced_fine_more->SetBinError(curr_bin, curr_err);
      }
      offset += nbins_pt_fine;
    }


    // Coarse binning
    nbins =  nbins_pt_coarse*n_eta_slices_more; // 15 pt X 11 eta
    fHistWtSystem_rolled_TopEta_sliced_more = 
	new TH1F("h_WtSystem_rolled_EtaTop_more",
		 "Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of top (Range 0. to 150.), in 11 slices of #eta of top",
		 nbins, 0., 1500.);
    fHistWtSystem_rolled_TopEta_sliced_more->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{W}|");
    fHistWtSystem_rolled_TopEta_sliced_more->SetZTitle("Number of Entries");

    offset = 0;
    curr_bin = 0;
    curr_err = 0;
    curr_value = 0;

    for ( Int_t i = 1; i <= n_eta_slices_more; i++ ) {
      for ( Int_t j = 1; j <= nbins_pt_coarse; j++ ) {
	curr_bin = offset+j;
	curr_value = fHistWtSystem_vs_TopEta_sliced_more->GetBinContent(j,i);
	curr_err = fHistWtSystem_vs_TopEta_sliced_more->GetBinError(j,i);
	fHistWtSystem_rolled_TopEta_sliced_more->SetBinContent(curr_bin, curr_value);
	fHistWtSystem_rolled_TopEta_sliced_more->SetBinError(curr_bin, curr_err);
      }
      offset += nbins_pt_coarse;
    }


    
    fOutputFile->cd();

    AtlKinFitterTool::Terminate();
    

}


//--------------------------------------------------------------------

void AtlSgTop_WtChannelFinder::Print() const {
    //
    // Prints chosen parameter settings for the finder tool to
    // standard output
    //
    
    cout << endl
	 << "========================================================" << endl
	 << "  Single-Top Wt-Channel Finder Tool                      " << endl
	 << "========================================================" << endl
	 << "  Chosen run mode is           ";
	switch ( fMode ) {
	    case kAll :
		cout << "kAll" << endl;
		break;
	    case kKinFit :
		cout << "kKinFit" << endl;
		break;
	    case kCutBased :
		cout << "kCutBased" << endl;
		break;
	}
	if ( fUseLeptonCovRand )
	  cout
	      << "  fUseLeptonCovRand                              = kTRUE"<<endl
	      << "  fIsFwdElectronMode                             = " << ( (fIsFwdElectronMode) ? "kTRUE" : "kFALSE" )
	      << endl;	else cout
	       << "  fUseLeptonCovRand                              = kFALSE"<<endl;
	
	cout << "  fSFSystMode                                    = " << fgSystNames[fSFSystMode] << endl;	
	cout << "  Chosen mass constraint is    "
	     << ( (fModeMass == kBW) ? "kBW" : "kGauss" ) << endl
	     << "  Is GuessNeutrinoEtaE used?   "
	     << ( (fGuessNeutrinoEtaE) ? "kTRUE" : "kFALSE" ) << endl
	     << "  Is truth-matching applied?   "
	     << "  Chosen main P(chi2) cut is   fChi2ProbMin      = "
	     << fChi2ProbMin << endl
	     << "  fMaxNbIter   = " << fMaxNbIter << endl
	     << endl
	     << "--------------------------------------------------------" << endl	
	     << endl
	     << "  Minimum jet multiplicity is  fNJetsMin         = "
	     << fNJetsMin << endl
	     << "  Maximum jet multiplicity is  fNJetsMax         = "
	     << fNJetsMax                                                  << endl
	     << "  Maximum jet multiplicity is  fNJetsExcl        = "
	     << fNJetsExcl                                                  << endl

	     << "--------------------------------------------------------" << endl	
	     << "  W-boson and top-quark:                                " << endl
	     << "--------------------------------------------------------" << endl
	     << "  fW_Mass                    = " << fW_Mass << endl
	     << "  fW_Width                   = " << fW_Width << endl
	     << "  fTop_Mass                  = " << fTop_Mass << endl
	     << "  fTop_Width                 = " << fTop_Width << endl
	     << "--------------------------------------------------------" << endl 
	     << "  Specifications of physics objects:                    " << endl
	     << "--------------------------------------------------------" << endl
	     << "  fElPt_Min                  = " << fElPt_Min << endl
	     << "  fMuPt_Min                  = " << fMuPt_Min << endl
	     << "--------------------------------------------------------" << endl
	     << "  fJets_Et_min               = " << fJets_Et_min      << endl
	     << "  fJets_Eta_min              = " << fJets_Eta_min     << endl
	     << "  fJets_Eta_max              = " << fJets_Eta_max     << endl   
	     << "  fBJets_Et_min              = " << fBJets_Et_min     << endl
	     << "  fBJets_Eta_min             = " << fBJets_Eta_min    << endl
	     << "  fBJets_Eta_max             = " << fBJets_Eta_max    << endl
	     << "  fBJetTagger                = " << AtlBTag::GetTaggerName(fBJetTagger) << endl
	     << "  fBJetWeightMin             = " << fBJetWeightMin    << endl
	     << "  fWhadJets_Et_min           = " << fWhadJets_Et_min  << endl
	     << "  fWhadJets_Eta_min          = " << fWhadJets_Eta_min << endl
	     << "  fWhaddJets_Eta_max         = " << fWhadJets_Eta_max << endl
	     << "  fWhadJetTagger             = " << AtlBTag::GetTaggerName(fWhadJetTagger) << endl
	     << "  fWhadJetWeightMax          = " << fWhadJetWeightMax << endl
	     << "========================================================" << endl
	     << " ttbar veto (only cut flow) "                             << endl
	     << "  fVetoTTbarProbMin          = " << fVetoTTbarProbMin     << endl
	     << "========================================================" << endl
	     << "  fProduceCommonTree         = " << //fProduceCommonTree << endl
	    ( (fProduceCommonTree) ? "kTRUE" : "kFALSE" ) << endl
	     << "  fIsAcceptanceChallenge         = " <<
	    ( (fIsAcceptanceChallenge) ? "kTRUE" : "kFALSE" ) << endl
	     << "========================================================" << endl
	     << endl;
	
}

//____________________________________________________________________
    
void AtlSgTop_WtChannelFinder::BookLeptonHistograms() {
    //
   // Book histograms for the leptons used in the reconstruction
    //
    gDirectory->mkdir("TotalYield", "Total yield after all selection cuts");
    gDirectory->cd("TotalYield");
    
    fHistTotalYield_allCuts
	= new TH1F("h_total_yield_all_cuts", "Total weighted yield after all analysis selection cuts",
		   1, 0, 1);
    fHistTotalYield_allCuts->SetXTitle("passed all cuts");
    fHistTotalYield_allCuts->SetYTitle("Number of Entries");
    TAxis *axe = fHistTotalYield_allCuts->GetXaxis();
    axe->SetBinLabel(1,"True");

    fHistTotalYield_allCuts_cut20
	= new TH1F("h_total_yield_all_cuts_cut20", "Total weighted yield after all analysis selection cuts",
		   1, 0, 1);
    fHistTotalYield_allCuts_cut20->SetXTitle("passed all cuts");
    fHistTotalYield_allCuts_cut20->SetYTitle("Number of Entries");
    axe = fHistTotalYield_allCuts_cut20->GetXaxis();
    axe->SetBinLabel(1,"True");

    fHistTotalYield_allCuts_cut30
	= new TH1F("h_total_yield_all_cuts_cut30", "Total weighted yield after all analysis selection cuts",
		   1, 0, 1);
    fHistTotalYield_allCuts_cut30->SetXTitle("passed all cuts");
    fHistTotalYield_allCuts_cut30->SetYTitle("Number of Entries");
    axe = fHistTotalYield_allCuts_cut30->GetXaxis();
    axe->SetBinLabel(1,"True");

    fHistTotalYield_allCuts_cut40
	= new TH1F("h_total_yield_all_cuts_cut40", "Total weighted yield after all analysis selection cuts",
		   1, 0, 1);
    fHistTotalYield_allCuts_cut40->SetXTitle("passed all cuts");
    fHistTotalYield_allCuts_cut40->SetYTitle("Number of Entries");
    axe = fHistTotalYield_allCuts_cut40->GetXaxis();
    axe->SetBinLabel(1,"True");

    fHistTotalYield_allCuts_cut50
	= new TH1F("h_total_yield_all_cuts_cut50", "Total weighted yield after all analysis selection cuts",
		   1, 0, 1);
    fHistTotalYield_allCuts_cut50->SetXTitle("passed all cuts");
    fHistTotalYield_allCuts_cut50->SetYTitle("Number of Entries");
    axe = fHistTotalYield_allCuts_cut50->GetXaxis();
    axe->SetBinLabel(1,"True");

    fHistTotalYield_allCuts_cut60
	= new TH1F("h_total_yield_all_cuts_cut60", "Total weighted yield after all analysis selection cuts",
		   1, 0, 1);
    fHistTotalYield_allCuts_cut60->SetXTitle("passed all cuts");
    fHistTotalYield_allCuts_cut60->SetYTitle("Number of Entries");
    axe = fHistTotalYield_allCuts_cut60->GetXaxis();
    axe->SetBinLabel(1,"True");
    fHistTotalYield_p10WtPt
	= new TH1F("h_total_yield_p10WtPt", "Total weighted yield after all analysis selection cuts",
		   1, 0, 1);
    fHistTotalYield_p10WtPt->SetXTitle("passed all cuts");
    fHistTotalYield_p10WtPt->SetYTitle("Number of Entries");
    axe = fHistTotalYield_p10WtPt->GetXaxis();
    axe->SetBinLabel(1,"True");

    gDirectory->cd("..");

    gDirectory->mkdir("CommonEventSelection");
    gDirectory->cd("CommonEventSelection");

    gDirectory->mkdir("SignalLepton","Signal lepton");
    gDirectory->cd("SignalLepton");

    // ===========
    // All leptons
    // ===========
    gDirectory->mkdir("AllSignalLeptons","Inclusive signal leptons");
    gDirectory->cd("AllSignalLeptons");

    
    fHistSignalLepton_pt = new TH1F("h_sig_lepton_pt", "Signal Lepton Transverse Momentum",
				    40, 0, 200);
    fHistSignalLepton_pt->SetXTitle("P_{t} (GeV)");
    fHistSignalLepton_pt->SetYTitle("Number of Entries");
    fHistSignalLepton_eta = new TH1F("h_sig_lepton_eta", "Signal Lepton Pseudo-Rapidity",
				     40, -2.5, 2.5);
    fHistSignalLepton_eta->SetXTitle("#eta");
    fHistSignalLepton_eta->SetYTitle("Number of Entries");
    fHistSignalLepton_eta_cells = new TH1F("h_sig_lepton_eta_cells", "Signal Lepton Pseudo-Rapidity (cell granularity)",
  	                                      100, -2.5, 2.5);
    fHistSignalLepton_eta_cells->SetXTitle("#eta");
    fHistSignalLepton_eta_cells->SetYTitle("Number of Entries");
    
    fHistSignalLepton_phi = new TH1F("h_sig_lepton_phi", "Signal Lepton Azimuth",
				     40, -3.2, 3.2);
    fHistSignalLepton_phi->SetXTitle("#phi (rad)");
    fHistSignalLepton_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // =========
    // Electrons
    // =========
    gDirectory->mkdir("SignalElectron","Signal electron");
    gDirectory->cd("SignalElectron");


    
    //****************************************
    // Number of signal-electrons by author
    //****************************************
    fHistSignalElectron_author = new TH1F("h_signal_electron_author",
		  "Signal Electron Author",
		  (AtlElectron::fgNAuthors + 1), 0, (AtlElectron::fgNAuthors + 1 ));
    fHistSignalElectron_author->SetYTitle("Number of Entries");
    TAxis *axis = fHistSignalElectron_author->GetXaxis();
    for ( Int_t i = 0; i < AtlElectron::fgNAuthors; i++ )
	axis->SetBinLabel(i+1, AtlElectron::fgAuthorNames[i]);
    axis->SetBinLabel(6,"HighPt and Soft");

    
    // Both charges
    gDirectory->mkdir("both");
    gDirectory->cd("both");

    fHistSignalElectron_pt = new TH1F("h_sig_electron_pt", "Signal e Transverse Momentum",
				      200, 0, 200);
    fHistSignalElectron_pt->SetXTitle("P_{t} (GeV)");
    fHistSignalElectron_pt->SetYTitle("Number of Entries");
    fHistSignalElectron_eta = new TH1F("h_sig_electron_eta", "Signal e Pseudo-Rapidity",
				       50, -2.5, 2.5);
    fHistSignalElectron_eta->SetXTitle("#eta");
    fHistSignalElectron_eta->SetYTitle("Number of Entries");
    fHistSignalElectron_phi = new TH1F("h_sig_electron_phi", "Signal e Azimuth",
				       128, -3.2, 3.2);
    fHistSignalElectron_phi->SetXTitle("#phi (rad)");
    fHistSignalElectron_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // e+
    gDirectory->mkdir("e+");
    gDirectory->cd("e+");

    fHistSignalEplus_pt = new TH1F("h_sig_eplus_pt", "Signal e^{+} Transverse Momentum",
				   200, 0, 200);
    fHistSignalEplus_pt->SetXTitle("P_{t} (GeV)");
    fHistSignalEplus_pt->SetYTitle("Number of Entries");
    fHistSignalEplus_eta = new TH1F("h_sig_eplus_eta", "Signal e^{+} Pseudo-Rapidity",
				    50, -2.5, 2.5);
    fHistSignalEplus_eta->SetXTitle("#eta");
    fHistSignalEplus_eta->SetYTitle("Number of Entries");
    fHistSignalEplus_phi = new TH1F("h_sig_eplus_phi", "Signal e^{+} Azimuth",
				    128, -3.2, 3.2);
    fHistSignalEplus_phi->SetXTitle("#phi (rad)");
    fHistSignalEplus_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // e-
    gDirectory->mkdir("e-");
    gDirectory->cd("e-");

    fHistSignalEminus_pt = new TH1F("h_sig_eminus_pt", "Signal e^{-} Transverse Momentum",
				   200, 0, 200);
    fHistSignalEminus_pt->SetXTitle("P_{t} (GeV)");
    fHistSignalEminus_pt->SetYTitle("Number of Entries");
    fHistSignalEminus_eta = new TH1F("h_sig_eminus_eta", "Signal e^{-} Pseudo-Rapidity",
				     50, -2.5, 2.5);
    fHistSignalEminus_eta->SetXTitle("#eta");
    fHistSignalEminus_eta->SetYTitle("Number of Entries");
    fHistSignalEminus_phi = new TH1F("h_sig_eminus_phi", "Signal e^{-} Azimuth",
				    128, -3.2, 3.2);
    fHistSignalEminus_phi->SetXTitle("#phi (rad)");
    fHistSignalEminus_phi->SetYTitle("Number of Entries");

    gDirectory->cd("../..");

    // =====
    // Muons
    // =====
    gDirectory->mkdir("SignalMuon","Signal muon");
    gDirectory->cd("SignalMuon");
    
    // Both charges
    gDirectory->mkdir("both");
    gDirectory->cd("both");

    fHistSignalMuon_pt = new TH1F("h_sig_muon_pt", "Signal #mu Transverse Momentum",
				  200, 0, 200);
    fHistSignalMuon_pt->SetXTitle("P_{t} (GeV)");
    fHistSignalMuon_pt->SetYTitle("Number of Entries");
    fHistSignalMuon_eta = new TH1F("h_sig_muon_eta", "Signal #mu Pseudo-Rapidity",
				   50, -2.5, 2.5);
    fHistSignalMuon_eta->SetXTitle("#eta");
    fHistSignalMuon_eta->SetYTitle("Number of Entries");
    fHistSignalMuon_phi = new TH1F("h_sig_muon_phi", "Signal #mu Azimuth",
				   128, -3.2, 3.2);
    fHistSignalMuon_phi->SetXTitle("#phi (rad)");
    fHistSignalMuon_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // mu+
    gDirectory->mkdir("mu+");
    gDirectory->cd("mu+");

    fHistSignalMuplus_pt = new TH1F("h_sig_muplus_pt", "Signal #mu^{+} Transverse Momentum",
				    200, 0, 200);
    fHistSignalMuplus_pt->SetXTitle("P_{t} (GeV)");
    fHistSignalMuplus_pt->SetYTitle("Number of Entries");
    fHistSignalMuplus_eta = new TH1F("h_sig_muplus_eta", "Signal #mu^{+} Pseudo-Rapidity",
				     50, -2.5, 2.5);
    fHistSignalMuplus_eta->SetXTitle("#eta");
    fHistSignalMuplus_eta->SetYTitle("Number of Entries");
    fHistSignalMuplus_phi = new TH1F("h_sig_muplus_phi", "Signal #mu^{+} Azimuth",
				     128, -3.2, 3.2);
    fHistSignalMuplus_phi->SetXTitle("#phi (rad)");
    fHistSignalMuplus_phi->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    // mu-
    gDirectory->mkdir("mu-");
    gDirectory->cd("mu-");

    fHistSignalMuminus_pt = new TH1F("h_sig_muminus_pt", "Signal #mu^{-} Transverse Momentum",
				   200, 0, 200);
    fHistSignalMuminus_pt->SetXTitle("P_{t} (GeV)");
    fHistSignalMuminus_pt->SetYTitle("Number of Entries");
    fHistSignalMuminus_eta = new TH1F("h_sig_muminus_eta", "Signal #mu^{-} Pseudo-Rapidity",
				     50, -2.5, 2.5);
    fHistSignalMuminus_eta->SetXTitle("#eta");
    fHistSignalMuminus_eta->SetYTitle("Number of Entries");
    fHistSignalMuminus_phi = new TH1F("h_sig_muminus_phi", "Signal #mu^{-} Azimuth",
				    128, -3.2, 3.2);
    fHistSignalMuminus_phi->SetXTitle("#phi (rad)");
    fHistSignalMuminus_phi->SetYTitle("Number of Entries");

    gDirectory->cd("../..");

    // ===
    // Tau
    // ===
    gDirectory->mkdir("SignalTau","Signal tau");
    gDirectory->cd("SignalTau");
    
    // Both charges
    gDirectory->mkdir("both");
    gDirectory->cd("both");

    fHistSignalTau_pt = new TH1F("h_sig_tau_pt", "Signal #tau Transverse Momentum",
				  200, 0, 200);
    fHistSignalTau_pt->SetXTitle("P_{t} (GeV)");
    fHistSignalTau_pt->SetYTitle("Number of Entries");
    fHistSignalTau_eta = new TH1F("h_sig_tau_eta", "Signal #tau Pseudo-Rapidity",
				   50, -2.5, 2.5);
    fHistSignalTau_eta->SetXTitle("#eta");
    fHistSignalTau_eta->SetYTitle("Number of Entries");
    fHistSignalTau_phi = new TH1F("h_sig_tau_phi", "Signal #tau Azimuth",
				   128, -3.2, 3.2);
    fHistSignalTau_phi->SetXTitle("#phi (rad)");
    fHistSignalTau_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // tau+
    gDirectory->mkdir("tau+");
    gDirectory->cd("tau+");

    fHistSignalTauplus_pt = new TH1F("h_sig_tauplus_pt", "Signal #tau^{+} Transverse Momentum",
				    200, 0, 200);
    fHistSignalTauplus_pt->SetXTitle("P_{t} (GeV)");
    fHistSignalTauplus_pt->SetYTitle("Number of Entries");
    fHistSignalTauplus_eta = new TH1F("h_sig_tauplus_eta", "Signal #tau^{+} Pseudo-Rapidity",
				     50, -2.5, 2.5);
    fHistSignalTauplus_eta->SetXTitle("#eta");
    fHistSignalTauplus_eta->SetYTitle("Number of Entries");
    fHistSignalTauplus_phi = new TH1F("h_sig_tauplus_phi", "Signal #tau^{+} Azimuth",
				     128, -3.2, 3.2);
    fHistSignalTauplus_phi->SetXTitle("#phi (rad)");
    fHistSignalTauplus_phi->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    // tau-
    gDirectory->mkdir("tau-");
    gDirectory->cd("tau-");

    fHistSignalTauminus_pt = new TH1F("h_sig_tauminus_pt", "Signal #tau^{-} Transverse Momentum",
				   200, 0, 200);
    fHistSignalTauminus_pt->SetXTitle("P_{t} (GeV)");
    fHistSignalTauminus_pt->SetYTitle("Number of Entries");
    fHistSignalTauminus_eta = new TH1F("h_sig_tauminus_eta", "Signal #tau^{-} Pseudo-Rapidity",
				     50, -2.5, 2.5);
    fHistSignalTauminus_eta->SetXTitle("#eta");
    fHistSignalTauminus_eta->SetYTitle("Number of Entries");
    fHistSignalTauminus_phi = new TH1F("h_sig_tauminus_phi", "Signal #tau^{-} Azimuth",
				    128, -3.2, 3.2);
    fHistSignalTauminus_phi->SetXTitle("#phi (rad)");
    fHistSignalTauminus_phi->SetYTitle("Number of Entries");
    
    gDirectory->cd("../../..");
}

//____________________________________________________________________

void AtlSgTop_WtChannelFinder::BookJetHistograms() {
    //
    // Book jet histograms
    //
    gDirectory->mkdir("SignalJets","Signal jets");
    gDirectory->cd("SignalJets");

    // ======
    // Jets
    // ======
    gDirectory->mkdir("all_jets","All selected signal jets");
    gDirectory->cd("all_jets");

    // All jets
    fHistJet_n = new TH1F("h_jet_n", "No. of all jets",
			   10, 0, 10);
    fHistJet_n->SetXTitle("N_{jet}");
    fHistJet_n->SetYTitle("Number of Entries");
    fHistJet_Et = new TH1F("h_jet_Et", "Transverse Energy of all jets",
			    200, 0, 200);
    fHistJet_Et->SetXTitle("E_{t} (GeV)");
    fHistJet_Et->SetYTitle("Number of Entries");
    fHistJet_eta = new TH1F("h_jet_eta", "Pseudo-Rapidity of all jets",
			     50, -2.5, 2.5);
    fHistJet_eta->SetXTitle("#eta");
    fHistJet_eta->SetYTitle("Number of Entries");
    fHistJet_phi = new TH1F("h_jet_phi", "Azimuth of all jets",
			     128, -3.2, 3.2);
    fHistJet_phi->SetXTitle("#phi (rad)");
    fHistJet_phi->SetYTitle("Number of Entries");

    // Leading jet
    fHistJet1_Et = new TH1F("h_jet1_Et", "Transverse Energy of leading jet",
			    200, 0, 200);
    fHistJet1_Et->SetXTitle("E_{t} (GeV)");
    fHistJet1_Et->SetYTitle("Number of Entries");
    fHistJet1_eta = new TH1F("h_jet1_eta", "Pseudo-Rapidity of leading jet",
			     50, -2.5, 2.5);
    fHistJet1_eta->SetXTitle("#eta");
    fHistJet1_eta->SetYTitle("Number of Entries");
    fHistJet1_phi = new TH1F("h_jet1_phi", "Azimuth of leading jet",
			     128, -3.2, 3.2);
    fHistJet1_phi->SetXTitle("#phi (rad)");
    fHistJet1_phi->SetYTitle("Number of Entries");

    // 2nd Leading jet
    fHistJet2_Et = new TH1F("h_jet2_Et", "Transverse Energy of 2nd leading jet",
			    200, 0, 200);
    fHistJet2_Et->SetXTitle("E_{t} (GeV)");
    fHistJet2_Et->SetYTitle("Number of Entries");
    fHistJet2_eta = new TH1F("h_jet2_eta", "Pseudo-Rapidity of 2nd leading jet",
			     50, -2.5, 2.5);
    fHistJet2_eta->SetXTitle("#eta");
    fHistJet2_eta->SetYTitle("Number of Entries");
    fHistJet2_phi = new TH1F("h_jet2_phi", "Azimuth of 2nd leading jet",
			     128, -3.2, 3.2);
    fHistJet2_phi->SetXTitle("#phi (rad)");
    fHistJet2_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");


    // ======
    // B-Jets
    // ======
    gDirectory->mkdir("b-jet","Signal b-jet");
    gDirectory->cd("b-jet");

    // All b-jets
    fHistBJet_n = new TH1F("h_bjet_n", "No. of all b-jets",
			   10, 0, 10);
    fHistBJet_n->SetXTitle("N_{b-jet}");
    fHistBJet_n->SetYTitle("Number of Entries");
    fHistBJet_Et = new TH1F("h_bjet_Et", "Transverse Energy of all b-jets",
			    200, 0, 200);
    fHistBJet_Et->SetXTitle("E_{t} (GeV)");
    fHistBJet_Et->SetYTitle("Number of Entries");
    fHistBJet_eta = new TH1F("h_bjet_eta", "Pseudo-Rapidity of all b-jets",
			     50, -2.5, 2.5);
    fHistBJet_eta->SetXTitle("#eta");
    fHistBJet_eta->SetYTitle("Number of Entries");
    fHistBJet_phi = new TH1F("h_bjet_phi", "Azimuth of all b-jets",
			     128, -3.2, 3.2);
    fHistBJet_phi->SetXTitle("#phi (rad)");
    fHistBJet_phi->SetYTitle("Number of Entries");
    fHistBJet_weight = new TH1F("h_bjet_weight", "B-tag weights of all b-jets",
			     50, -10, 12);
    fHistBJet_weight->SetXTitle("B-Tag Weight");
    fHistBJet_weight->SetYTitle("Number of Entries");

    // Leading b-jet
    fHistBJet1_Et = new TH1F("h_bjet1_Et", "Transverse Energy of leading b-jet",
			    200, 0, 200);
    fHistBJet1_Et->SetXTitle("E_{t} (GeV)");
    fHistBJet1_Et->SetYTitle("Number of Entries");
    fHistBJet1_eta = new TH1F("h_bjet1_eta", "Pseudo-Rapidity of leading b-jet",
			     50, -2.5, 2.5);
    fHistBJet1_eta->SetXTitle("#eta");
    fHistBJet1_eta->SetYTitle("Number of Entries");
    fHistBJet1_phi = new TH1F("h_bjet1_phi", "Azimuth of leading b-jet",
			     128, -3.2, 3.2);
    fHistBJet1_phi->SetXTitle("#phi (rad)");
    fHistBJet1_phi->SetYTitle("Number of Entries");
    fHistBJet1_weight = new TH1F("h_bjet1_weight", "B-tag weight of leading b-jets",
			     50, -10, 12);
    fHistBJet1_weight->SetXTitle("B-Tag Weight");
    fHistBJet1_weight->SetYTitle("Number of Entries");

    // 2nd Leading b-jet
    fHistBJet2_Et = new TH1F("h_bjet2_Et", "Transverse Energy of 2nd leading b-jet",
			    200, 0, 200);
    fHistBJet2_Et->SetXTitle("E_{t} (GeV)");
    fHistBJet2_Et->SetYTitle("Number of Entries");
    fHistBJet2_eta = new TH1F("h_bjet2_eta", "Pseudo-Rapidity of 2nd leading b-jet",
			     50, -2.5, 2.5);
    fHistBJet2_eta->SetXTitle("#eta");
    fHistBJet2_eta->SetYTitle("Number of Entries");
    fHistBJet2_phi = new TH1F("h_bjet2_phi", "Azimuth of 2nd leading b-jet",
			     128, -3.2, 3.2);
    fHistBJet2_phi->SetXTitle("#phi (rad)");
    fHistBJet2_phi->SetYTitle("Number of Entries");
    fHistBJet2_weight = new TH1F("h_bjet2_weight", "B-tag weight of 2nd leading b-jet",
			     50, -10, 12);
    fHistBJet2_weight->SetXTitle("B-Tag Weight");
    fHistBJet2_weight->SetYTitle("Number of Entries");

    gDirectory->cd("..");


    // ======
    // Non-B-Jets
    // ======
    gDirectory->mkdir("non-b-jet","Signal non-b-jet");
    gDirectory->cd("non-b-jet");

    // All non-b-jets
    fHistNonBJet_n = new TH1F("h_nonbjet_n", "No. of all non-b-jets",
			   10, 0, 10);
    fHistNonBJet_n->SetXTitle("N_{non-b-jet}");
    fHistNonBJet_n->SetYTitle("Number of Entries");
    fHistNonBJet_Et = new TH1F("h_nonbjet_Et", "Transverse Energy of all non-b-jets",
			    200, 0, 200);
    fHistNonBJet_Et->SetXTitle("E_{t} (GeV)");
    fHistNonBJet_Et->SetYTitle("Number of Entries");
    fHistNonBJet_eta = new TH1F("h_nonbjet_eta", "Pseudo-Rapidity of all non-b-jets",
			     50, -2.5, 2.5);
    fHistNonBJet_eta->SetXTitle("#eta");
    fHistNonBJet_eta->SetYTitle("Number of Entries");
    fHistNonBJet_phi = new TH1F("h_nonbjet_phi", "Azimuth of all non-b-jets",
			     128, -3.2, 3.2);
    fHistNonBJet_phi->SetXTitle("#phi (rad)");
    fHistNonBJet_phi->SetYTitle("Number of Entries");

    // Leading non-b-jet
    fHistNonBJet1_Et = new TH1F("h_nonbjet1_Et", "Transverse Energy of leading non-b-jet",
			    200, 0, 200);
    fHistNonBJet1_Et->SetXTitle("E_{t} (GeV)");
    fHistNonBJet1_Et->SetYTitle("Number of Entries");
    fHistNonBJet1_eta = new TH1F("h_nonbjet1_eta", "Pseudo-Rapidity of leading non-b-jet",
			     50, -2.5, 2.5);
    fHistNonBJet1_eta->SetXTitle("#eta");
    fHistNonBJet1_eta->SetYTitle("Number of Entries");
    fHistNonBJet1_phi = new TH1F("h_nonbjet1_phi", "Azimuth of leading non-b-jet",
			     128, -3.2, 3.2);
    fHistNonBJet1_phi->SetXTitle("#phi (rad)");
    fHistNonBJet1_phi->SetYTitle("Number of Entries");

    // 2nd Leading non-b-jet
    fHistNonBJet2_Et = new TH1F("h_nonbjet2_Et", "Transverse Energy of 2nd leading non-b-jet",
			    200, 0, 200);
    fHistNonBJet2_Et->SetXTitle("E_{t} (GeV)");
    fHistNonBJet2_Et->SetYTitle("Number of Entries");
    fHistNonBJet2_eta = new TH1F("h_nonbjet2_eta", "Pseudo-Rapidity of 2nd leading non-b-jet",
			     50, -2.5, 2.5);
    fHistNonBJet2_eta->SetXTitle("#eta");
    fHistNonBJet2_eta->SetYTitle("Number of Entries");
    fHistNonBJet2_phi = new TH1F("h_nonbjet2_phi", "Azimuth of 2nd leading non-b-jet",
			     128, -3.2, 3.2);
    fHistNonBJet2_phi->SetXTitle("#phi (rad)");
    fHistNonBJet2_phi->SetYTitle("Number of Entries");

    gDirectory->cd("../../..");
}

//____________________________________________________________________

void AtlSgTop_WtChannelFinder::FillJetHistograms() {
    //
    // Fill jet histograms
    //
    Float_t Et  = 0.;
    Float_t eta = 0.;
    Float_t phi = 0.;
    AtlJet *jet = 0;
    
    TIter next_jet(fJets);
    Int_t njet = 0;
    while ( (jet = (AtlJet*)next_jet()) ) {
	Et  = jet->Et();
	eta = jet->Eta();
	phi = jet->Phi();
	fHistJet_Et ->Fill(Et, GetPreTagEvtWeight());
	fHistJet_eta->Fill(eta, GetPreTagEvtWeight());
	fHistJet_phi->Fill(phi, GetPreTagEvtWeight());
	if ( njet == 0 ) {
	    fHistJet1_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistJet1_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistJet1_phi->Fill(phi, GetPreTagEvtWeight());
	}
	if ( njet == 1 ) {
	    fHistJet2_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistJet2_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistJet2_phi->Fill(phi, GetPreTagEvtWeight());
	}
	njet++;
    }
    fHistJet_n->Fill(njet, GetPreTagEvtWeight());
}

//____________________________________________________________________

void AtlSgTop_WtChannelFinder::FillBJetHistograms() {
    //
    // Fill b-jet histograms
    //
    Float_t Et  = 0.;
    Float_t eta = 0.;
    Float_t phi = 0.;
    Float_t weight = 0;

    AtlJet *jet = 0;
    
    TIter next_bjet(fBJets);
    Int_t njet = 0;
    while ( (jet = (AtlJet*)next_bjet()) ) {
	Et  = jet->Et();
	eta = jet->Eta();
	phi = jet->Phi();
	weight = jet->GetTag(fBJetTagger)->GetWeight();

	fHistBJet_Et ->Fill(Et, GetPreTagEvtWeight());
	fHistBJet_eta->Fill(eta, GetPreTagEvtWeight());
	fHistBJet_phi->Fill(phi, GetPreTagEvtWeight());
	fHistBJet_weight->Fill(weight, GetPreTagEvtWeight());
	if ( njet == 0 ) {
	    fHistBJet1_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistBJet1_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistBJet1_phi->Fill(phi, GetPreTagEvtWeight());
	    fHistBJet1_weight->Fill(weight, GetPreTagEvtWeight());
	}
	if ( njet == 1 ) {
	    fHistBJet2_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistBJet2_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistBJet2_phi->Fill(phi, GetPreTagEvtWeight());
	    fHistBJet2_weight->Fill(weight, GetPreTagEvtWeight());
	}
	njet++;
    }
    fHistBJet_n->Fill(njet, GetPreTagEvtWeight());
}

//____________________________________________________________________

void AtlSgTop_WtChannelFinder::FillNonBJetHistograms() {
    //
    // Fill non-b-jet histograms
    //
    Float_t Et  = 0.;
    Float_t eta = 0.;
    Float_t phi = 0.;
    AtlJet *jet = 0;
    
    TIter next_nonbjet(fWhadJets);
    Int_t njet = 0;
    while ( (jet = (AtlJet*)next_nonbjet()) ) {
	Et  = jet->Et();
	eta = jet->Eta();
	phi = jet->Phi();
	fHistNonBJet_Et ->Fill(Et, GetPreTagEvtWeight());
	fHistNonBJet_eta->Fill(eta, GetPreTagEvtWeight());
	fHistNonBJet_phi->Fill(phi, GetPreTagEvtWeight());
	if ( njet == 0 ) {
	    fHistNonBJet1_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistNonBJet1_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistNonBJet1_phi->Fill(phi, GetPreTagEvtWeight());
	}
	if ( njet == 1 ) {
	    fHistNonBJet2_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistNonBJet2_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistNonBJet2_phi->Fill(phi, GetPreTagEvtWeight());
	}
	njet++;
    }
    fHistNonBJet_n->Fill(njet, GetPreTagEvtWeight());
}

//____________________________________________________________________
void AtlSgTop_WtChannelFinder::FillLeptonHistograms() {
    //
    // Fill lepton histograms
    //

    // =============
    // Signal lepton
    // =============
    if ( fLepton != 0 ) {
	Float_t Pt_sig  = fLepton->Pt(); 	// *** still okay? ***
	Float_t Eta_sig = fLepton->Eta();
	Float_t Phi_sig = fLepton->Phi();

	fHistSignalLepton_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
	fHistSignalLepton_eta->Fill(Eta_sig, GetPreTagEvtWeight());
	fHistSignalLepton_eta_cells->Fill(Eta_sig, GetPreTagEvtWeight());
	fHistSignalLepton_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	
	// e
	if ( fLepton->IsElectron() ) {
	    //*************************************
	    // Number of signal electrons by author
	    //*************************************
	    // Mind event weights at low statistics!!!
	    AtlElectron::EAuthor author = ((AtlElectron*)fLepton)->GetAuthor();

	    for ( Int_t j = 0; j < AtlElectron::fgNAuthors; j++ ) {
		if ( author & (1<<j) ) fHistSignalElectron_author->AddBinContent(j+1, GetPreTagEvtWeight());
	    }
	    // set of electrons that are highPt AND soft
	    if ( (author & 2) && (author & 4) )
		fHistSignalElectron_author->AddBinContent(6, GetPreTagEvtWeight());
//	    fHistSignalElectron_author->Sumw2();
	   
	    fHistSignalElectron_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
	    fHistSignalElectron_eta->Fill(Eta_sig, GetPreTagEvtWeight());
	    fHistSignalElectron_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    if ( fLepton->IsPositive() ) {
		fHistSignalEplus_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistSignalEplus_eta->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistSignalEplus_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    } else {
		fHistSignalEminus_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistSignalEminus_eta->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistSignalEminus_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    }
	}
	
	// mu
	if ( fLepton->IsMuon() ) {
	    fHistSignalMuon_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
	    fHistSignalMuon_eta->Fill(Eta_sig, GetPreTagEvtWeight());
	    fHistSignalMuon_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    if ( fLepton->IsPositive() ) {
		fHistSignalMuplus_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistSignalMuplus_eta->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistSignalMuplus_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    } else {
		fHistSignalMuminus_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistSignalMuminus_eta->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistSignalMuminus_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    }
	}
    }
}

//____________________________________________________________________
void AtlSgTop_WtChannelFinder::FillLeptonHistograms_BTag() {
    //
    // Fill lepton histograms
    //

    // =============
    // Signal lepton
    // =============
    if ( fLepton != 0 ) {
	Float_t Pt_sig  = fLepton->Pt(); 	// *** still okay? ***
	Float_t Eta_sig = fLepton->Eta();
	Float_t Phi_sig = fLepton->Phi();

	fHistSignalLepton_pt_btag ->Fill(Pt_sig,  GetPreTagEvtWeight());
	fHistSignalLepton_eta_btag->Fill(Eta_sig, GetPreTagEvtWeight());
	fHistSignalLepton_eta_btag_cells->Fill(Eta_sig, GetPreTagEvtWeight());
	fHistSignalLepton_phi_btag->Fill(Phi_sig, GetPreTagEvtWeight());
	
	// e
	if ( fLepton->IsElectron() ) {
	    //*************************************
	    // Number of signal electrons by author
	    //*************************************
	    // Mind event weights at low statistics!!!
	    AtlElectron::EAuthor author = ((AtlElectron*)fLepton)->GetAuthor();

	    for ( Int_t j = 0; j < AtlElectron::fgNAuthors; j++ ) {
		if ( author & (1<<j) ) fHistSignalElectron_author_btag->AddBinContent(j+1, GetPreTagEvtWeight());
	    }
	    // set of electrons that are highPt AND soft
	    if ( (author & 2) && (author & 4) )
		fHistSignalElectron_author_btag->AddBinContent(6, GetPreTagEvtWeight());
//	    fHistSignalElectron_author->Sumw2();
	   
	    fHistSignalElectron_pt_btag ->Fill(Pt_sig,  GetPreTagEvtWeight());
	    fHistSignalElectron_eta_btag->Fill(Eta_sig, GetPreTagEvtWeight());
	    fHistSignalElectron_phi_btag->Fill(Phi_sig, GetPreTagEvtWeight());
	    if ( fLepton->IsPositive() ) {
		fHistSignalEplus_pt_btag ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistSignalEplus_eta_btag->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistSignalEplus_phi_btag->Fill(Phi_sig, GetPreTagEvtWeight());
	    } else {
		fHistSignalEminus_pt_btag ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistSignalEminus_eta_btag->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistSignalEminus_phi_btag->Fill(Phi_sig, GetPreTagEvtWeight());
	    }
	}
	
	// mu
	if ( fLepton->IsMuon() ) {
	    fHistSignalMuon_pt_btag ->Fill(Pt_sig,  GetPreTagEvtWeight());
	    fHistSignalMuon_eta_btag->Fill(Eta_sig, GetPreTagEvtWeight());
	    fHistSignalMuon_phi_btag->Fill(Phi_sig, GetPreTagEvtWeight());
	    if ( fLepton->IsPositive() ) {
		fHistSignalMuplus_pt_btag ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistSignalMuplus_eta_btag->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistSignalMuplus_phi_btag->Fill(Phi_sig, GetPreTagEvtWeight());
	    } else {
		fHistSignalMuminus_pt_btag ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistSignalMuminus_eta_btag->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistSignalMuminus_phi_btag->Fill(Phi_sig, GetPreTagEvtWeight());
	    }
	}
    }
}
//____________________________________________________________________

void AtlSgTop_WtChannelFinder::FillJetHistograms_BTag() {
    //
    // Fill jet histograms
    //
    Float_t Et  = 0.;
    Float_t eta = 0.;
    Float_t phi = 0.;
    AtlJet *jet = 0;
    
    TIter next_jet(fJets);
    Int_t njet = 0;
    while ( (jet = (AtlJet*)next_jet()) ) {
	Et  = jet->Et();
	eta = jet->Eta();
	phi = jet->Phi();
	fHistJet_Et_btag ->Fill(Et, GetPreTagEvtWeight());
	fHistJet_eta_btag->Fill(eta, GetPreTagEvtWeight());
	fHistJet_phi_btag->Fill(phi, GetPreTagEvtWeight());
	if ( njet == 0 ) {
	    fHistJet1_Et_btag ->Fill(Et, GetPreTagEvtWeight());
	    fHistJet1_eta_btag->Fill(eta, GetPreTagEvtWeight());
	    fHistJet1_phi_btag->Fill(phi, GetPreTagEvtWeight());
	}
	if ( njet == 1 ) {
	    fHistJet2_Et_btag ->Fill(Et, GetPreTagEvtWeight());
	    fHistJet2_eta_btag->Fill(eta, GetPreTagEvtWeight());
	    fHistJet2_phi_btag->Fill(phi, GetPreTagEvtWeight());
	}
	njet++;
    }
    fHistJet_n_btag->Fill(njet, GetPreTagEvtWeight());
}

//____________________________________________________________________

void AtlSgTop_WtChannelFinder::FillBJetHistograms_BTag() {
    //
    // Fill b-jet histograms
    //
    Float_t Et  = 0.;
    Float_t eta = 0.;
    Float_t phi = 0.;
    Float_t weight = 0;

    AtlJet *jet = 0;
    
    TIter next_bjet(fBJets);
    Int_t njet = 0;
    while ( (jet = (AtlJet*)next_bjet()) ) {
	Et  = jet->Et();
	eta = jet->Eta();
	phi = jet->Phi();
	weight = jet->GetTag(fBJetTagger)->GetWeight();

	fHistBJet_Et_btag ->Fill(Et, GetPreTagEvtWeight());
	fHistBJet_eta_btag->Fill(eta, GetPreTagEvtWeight());
	fHistBJet_phi_btag->Fill(phi, GetPreTagEvtWeight());
	fHistBJet_weight_btag->Fill(weight, GetPreTagEvtWeight());
	if ( njet == 0 ) {
	    fHistBJet1_Et_btag ->Fill(Et, GetPreTagEvtWeight());
	    fHistBJet1_eta_btag->Fill(eta, GetPreTagEvtWeight());
	    fHistBJet1_phi_btag->Fill(phi, GetPreTagEvtWeight());
	    fHistBJet1_weight_btag->Fill(weight, GetPreTagEvtWeight());
	}
	if ( njet == 1 ) {
	    fHistBJet2_Et_btag ->Fill(Et, GetPreTagEvtWeight());
	    fHistBJet2_eta_btag->Fill(eta, GetPreTagEvtWeight());
	    fHistBJet2_phi_btag->Fill(phi, GetPreTagEvtWeight());
	    fHistBJet2_weight_btag->Fill(weight, GetPreTagEvtWeight());
	}
	njet++;
    }
    fHistBJet_n_btag->Fill(njet, GetPreTagEvtWeight());
}

//____________________________________________________________________

void AtlSgTop_WtChannelFinder::FillNonBJetHistograms_BTag() {
    //
    // Fill non-b-jet histograms
    //
    Float_t Et  = 0.;
    Float_t eta = 0.;
    Float_t phi = 0.;
    AtlJet *jet = 0;
    
    TIter next_nonbjet(fWhadJets);
    Int_t njet = 0;
    while ( (jet = (AtlJet*)next_nonbjet()) ) {
	Et  = jet->Et();
	eta = jet->Eta();
	phi = jet->Phi();
	fHistNonBJet_Et_btag ->Fill(Et, GetPreTagEvtWeight());
	fHistNonBJet_eta_btag->Fill(eta, GetPreTagEvtWeight());
	fHistNonBJet_phi_btag->Fill(phi, GetPreTagEvtWeight());
	if ( njet == 0 ) {
	    fHistNonBJet1_Et_btag ->Fill(Et, GetPreTagEvtWeight());
	    fHistNonBJet1_eta_btag->Fill(eta, GetPreTagEvtWeight());
	    fHistNonBJet1_phi_btag->Fill(phi, GetPreTagEvtWeight());
	}
	if ( njet == 1 ) {
	    fHistNonBJet2_Et_btag ->Fill(Et, GetPreTagEvtWeight());
	    fHistNonBJet2_eta_btag->Fill(eta, GetPreTagEvtWeight());
	    fHistNonBJet2_phi_btag->Fill(phi, GetPreTagEvtWeight());
	}
	njet++;
    }
    fHistNonBJet_n_btag->Fill(njet, GetPreTagEvtWeight());
}

//____________________________________________________________________

void AtlSgTop_WtChannelFinder::BookComparisonHistogramsLeptTopFit() {
    //
    // Book histograms for comparison of distributions before and after
    // KinFitter selection
    //
    gDirectory->mkdir("Comparison_LeptonicTop",
		      "Before/after fit comparison histograms");
    gDirectory->cd("Comparison_LeptonicTop");

    gDirectory->mkdir("Before",
		      "Histograms before fit ");
    gDirectory->cd("Before");

    // signal lepton
    gDirectory->mkdir("Lepton",
		      "Histograms before fit ");
    gDirectory->cd("Lepton");

    fHistSignalLeptonPt_before_lept = new TH1F("h_signal_lepton_pt_before_lept",
				  "p_{T} of signal lepton before fit",
				  50,0.,200);
    fHistSignalLeptonPt_before_lept->SetXTitle("p_{T}");
    fHistSignalLeptonPt_before_lept->SetYTitle("Number of Entries");

    fHistSignalLeptonEta_before_lept = new TH1F("h_signal_lepton_eta_before_lept",
				  "#eta of signal lepton before_lept fit",
				  50,-5.,5.);
    fHistSignalLeptonEta_before_lept->SetXTitle("#eta");
    fHistSignalLeptonEta_before_lept->SetYTitle("Number of Entries");
    
    fHistSignalLeptonEta_before_cells_lept = new TH1F("h_signal_lepton_eta_before_cells_lept",
						 "#eta of signal lepton before fit (cell granularity)",
						 100,-2.5,2.5);
    fHistSignalLeptonEta_before_cells_lept->SetXTitle("#eta");
    fHistSignalLeptonEta_before_cells_lept->SetYTitle("Number of Entries");

    fHistSignalLeptonPhi_before_lept = new TH1F("h_signal_lepton_phi_before_lept",
				  "#varphi of signal lepton before_lept fit",
				  50,-3.2,3.2);
    fHistSignalLeptonPhi_before_lept->SetXTitle("#varphi");
    fHistSignalLeptonPhi_before_lept->SetYTitle("Number of Entries");
    gDirectory->cd("..");
    // b-jet
    gDirectory->mkdir("jets",
		      "Histograms before fit ");
    gDirectory->cd("jets");

    fHistBJetEt_before_lept = new TH1F("h_bjet_et_before_lept",
				  "E_{T} of b-jet before_lept fit",
				  50,0.,200);
    fHistBJetEt_before_lept->SetXTitle("E_{T}");
    fHistBJetEt_before_lept->SetYTitle("Number of Entries");

    fHistBJetEta_before_lept = new TH1F("h_bjet_eta_before_lept",
				  "#eta of b-jet before_lept fit",
				  50,-5.,5.0);
    fHistBJetEta_before_lept->SetXTitle("#eta");
    fHistBJetEta_before_lept->SetYTitle("Number of Entries");
    
    fHistBJetPhi_before_lept = new TH1F("h_bjet_phi_before_lept",
				  "#varphi of b-jet before_lept fit",
				  50,-3.2,3.2);
    fHistBJetPhi_before_lept->SetXTitle("#varphi");
    fHistBJetPhi_before_lept->SetYTitle("Number of Entries");

    fHistBJetWeight_before_lept = new TH1F("h_bjet_weight_before_lept",
				  "b-tag weight of b-jet used in fit",
				  50,-10,12);
    fHistBJetWeight_before_lept->SetXTitle("B-Tag Weight");
    fHistBJetWeight_before_lept->SetYTitle("Number of Entries");

        // light-jet1
    fHistJet1Et_before_lept = new TH1F("h_jet1_et_before_lept",
				  "E_{T} of jet1 before_lept fit",
				  50,0.,200);
    fHistJet1Et_before_lept->SetXTitle("E_{T}");
    fHistJet1Et_before_lept->SetYTitle("Number of Entries");

    fHistJet1Eta_before_lept = new TH1F("h_jet1_eta_before_lept",
				  "#eta of jet1 before_lept fit",
				  50,-5.,5.0);
    fHistJet1Eta_before_lept->SetXTitle("#eta");
    fHistJet1Eta_before_lept->SetYTitle("Number of Entries");
    
    fHistJet1Phi_before_lept = new TH1F("h_jet1_phi_before_lept",
				  "#varphi of jet1 before_lept fit",
				  50,-3.2,3.2);
    fHistJet1Phi_before_lept->SetXTitle("#varphi");
    fHistJet1Phi_before_lept->SetYTitle("Number of Entries");

        // light-jet2
    fHistJet2Et_before_lept = new TH1F("h_jet2_et_before_lept",
				  "E_{T} of jet2 before_lept fit",
				  50,0.,200);
    fHistJet2Et_before_lept->SetXTitle("E_{T}");
    fHistJet2Et_before_lept->SetYTitle("Number of Entries");

    fHistJet2Eta_before_lept = new TH1F("h_jet2_eta_before_lept",
				  "#eta of jet2 before_lept fit",
				  50,-5.,5.0);
    fHistJet2Eta_before_lept->SetXTitle("#eta");
    fHistJet2Eta_before_lept->SetYTitle("Number of Entries");
    
    fHistJet2Phi_before_lept = new TH1F("h_je2t_phi_before_lept",
				  "#varphi of jet2 before_lept fit",
				  50,-3.2,3.2);
    fHistJet2Phi_before_lept->SetXTitle("#varphi");
    fHistJet2Phi_before_lept->SetYTitle("Number of Entries");
    gDirectory->cd("..");

    gDirectory->mkdir("EtMiss",
		      "Histograms before fit ");
    gDirectory->cd("EtMiss");
    
    // Et_miss
    fHistEtMissMag_before_lept = new TH1F("h_etmiss_mag_before_lept",
				  "Magnitude of missing E_{T} before_lept fit",
				  100,0.,300);
    fHistEtMissMag_before_lept->SetXTitle("E_{T}^{miss}");
    fHistEtMissMag_before_lept->SetYTitle("Number of Entries");
    
    fHistEtMissPhi_before_lept = new TH1F("h_etmiss_phi_before_lept",
				  "#varphi of missing E_{T} before_lept fit",
				  50,-3.2,3.2);
    fHistEtMissPhi_before_lept->SetXTitle("#varphi");
    fHistEtMissPhi_before_lept->SetYTitle("Number of Entries");

    // SumEt
    fHistSumEt_before_lept = new TH1F("h_sumet_before_lept",
				  "SumEt in events used for fit",
				  100,0,1000);
    fHistSumEt_before_lept->SetXTitle("#sum E_{t} (GeV)");
    fHistSumEt_before_lept->SetYTitle("Number of Entries");

    gDirectory->cd("..");

    // back to top
    gDirectory->cd("..");

    gDirectory->mkdir("After",
		      "Histograms after fit ");
    gDirectory->cd("After");

    // signal lepton
    gDirectory->mkdir("Lepton",
		      "Histograms before fit ");
    gDirectory->cd("Lepton");

    fHistSignalLeptonPt_after_lept = new TH1F("h_signal_lepton_pt_after_lept",
				  "p_{T} of signal lepton after_lept fit",
				  50,0.,200);
    fHistSignalLeptonPt_after_lept->SetXTitle("p_{T}");
    fHistSignalLeptonPt_after_lept->SetYTitle("Number of Entries");

    fHistSignalLeptonEta_after_lept = new TH1F("h_signal_lepton_eta_after_lept",
				  "#eta of signal lepton after_lept fit",
				  50,-5.,5.);
    fHistSignalLeptonEta_after_lept->SetXTitle("#eta");
    fHistSignalLeptonEta_after_lept->SetYTitle("Number of Entries");
    
    fHistSignalLeptonPhi_after_lept = new TH1F("h_signal_lepton_phi_after_lept",
				  "#varphi of signal lepton after_lept fit",
				  50,-3.2,3.2);
    fHistSignalLeptonPhi_after_lept->SetXTitle("#varphi");
    fHistSignalLeptonPhi_after_lept->SetYTitle("Number of Entries");

    gDirectory->cd("..");

    gDirectory->mkdir("jets",
		      "Histograms before fit ");
    gDirectory->cd("jets");

    // b-jet

    fHistBJetEt_after_lept = new TH1F("h_bjet_et_after_lept",
				  "E_{T} of b-jet after_lept fit",
				  50,0.,200);
    fHistBJetEt_after_lept->SetXTitle("E_{T}");
    fHistBJetEt_after_lept->SetYTitle("Number of Entries");

    fHistBJetEta_after_lept = new TH1F("h_bjet_eta_after_lept",
				  "#eta of b-jet after_lept fit",
				  50,-5.,5.0);
    fHistBJetEta_after_lept->SetXTitle("#eta");
    fHistBJetEta_after_lept->SetYTitle("Number of Entries");
    
    fHistBJetPhi_after_lept = new TH1F("h_bjet_phi_after_lept",
				  "#varphi of b-jet after_lept fit",
				  50,-3.2,3.2);
    fHistBJetPhi_after_lept->SetXTitle("#varphi");
    fHistBJetPhi_after_lept->SetYTitle("Number of Entries");

    // light-jet1
    fHistJet1Et_after_lept = new TH1F("h_jet1_et_after_lept",
				  "E_{T} of jet1 after_lept fit",
				  50,0.,200);
    fHistJet1Et_after_lept->SetXTitle("E_{T}");
    fHistJet1Et_after_lept->SetYTitle("Number of Entries");

    fHistJet1Eta_after_lept = new TH1F("h_jet1_eta_after_lept",
				  "#eta of jet1 after_lept fit",
				  50,-5.,5.0);
    fHistJet1Eta_after_lept->SetXTitle("#eta");
    fHistJet1Eta_after_lept->SetYTitle("Number of Entries");
    
    fHistJet1Phi_after_lept = new TH1F("h_jet1_phi_after_lept",
				  "#varphi of jet1 after_lept fit",
				  50,-3.2,3.2);
    fHistJet1Phi_after_lept->SetXTitle("#varphi");
    fHistJet1Phi_after_lept->SetYTitle("Number of Entries");

        // light-jet2
    fHistJet2Et_after_lept = new TH1F("h_jet2_et_after_lept",
				  "E_{T} of jet2 after_lept fit",
				  50,0.,200);
    fHistJet2Et_after_lept->SetXTitle("E_{T}");
    fHistJet2Et_after_lept->SetYTitle("Number of Entries");

    fHistJet2Eta_after_lept = new TH1F("h_jet2_eta_after_lept",
				  "#eta of jet2 after_lept fit",
				  50,-5.,5.0);
    fHistJet2Eta_after_lept->SetXTitle("#eta");
    fHistJet2Eta_after_lept->SetYTitle("Number of Entries");
    
    fHistJet2Phi_after_lept = new TH1F("h_je2t_phi_after_lept",
				  "#varphi of jet2 after_lept fit",
				  50,-3.2,3.2);
    fHistJet2Phi_after_lept->SetXTitle("#varphi");
    fHistJet2Phi_after_lept->SetYTitle("Number of Entries");

    gDirectory->cd("..");

    gDirectory->mkdir("EtMiss",
		      "Histograms before fit ");
    gDirectory->cd("EtMiss");
    
    // Et_miss
    fHistEtMissMag_after_lept = new TH1F("h_etmiss_mag_after_lept",
				    "Magnitude of missing E_{T} after_lept fit",
				  50,0.,200);
    fHistEtMissMag_after_lept->SetXTitle("E_{T}^{miss}");
    fHistEtMissMag_after_lept->SetYTitle("Number of Entries");
    
    fHistEtMissPhi_after_lept = new TH1F("h_etmiss_phi_after_lept",
				  "#varphi of missing E_{T} after_lept fit",
				  50,-3.2,3.2);
    fHistEtMissPhi_after_lept->SetXTitle("#varphi");
    fHistEtMissPhi_after_lept->SetYTitle("Number of Entries");    
    gDirectory->cd("..");
    // back to the top
    gDirectory->cd("../..");
}

//____________________________________________________________________

void AtlSgTop_WtChannelFinder::BookComparisonHistogramsHadrTopFit() {
    //
    // Book histograms for comparison of distributions before and after
    // KinFitter selection
    //
    gDirectory->mkdir("Comparison_HadronicTop",
		      "Before/after fit comparison histograms");
    gDirectory->cd("Comparison_HadronicTop");

    gDirectory->mkdir("Before",
		      "Histograms before fit ");
    gDirectory->cd("Before");

        // signal lepton
    // signal lepton
    gDirectory->mkdir("Lepton",
		      "Histograms before fit ");
    gDirectory->cd("Lepton");
    fHistSignalLeptonPt_before_hadr = new TH1F("h_signal_hadron_pt_before_hadr",
				  "p_{T} of signal lepton before fit",
				  50,0.,200);
    fHistSignalLeptonPt_before_hadr->SetXTitle("p_{T}");
    fHistSignalLeptonPt_before_hadr->SetYTitle("Number of Entries");

    fHistSignalLeptonEta_before_hadr = new TH1F("h_signal_hadron_eta_before_hadr",
				  "#eta of signal lepton before_hadr fit",
				  50,-5.,5.);
    fHistSignalLeptonEta_before_hadr->SetXTitle("#eta");
    fHistSignalLeptonEta_before_hadr->SetYTitle("Number of Entries");
    
    fHistSignalLeptonPhi_before_hadr = new TH1F("h_signal_hadron_phi_before_hadr",
				  "#varphi of signal lepton before_hadr fit",
				  50,-3.2,3.2);
    fHistSignalLeptonPhi_before_hadr->SetXTitle("#varphi");
    fHistSignalLeptonPhi_before_hadr->SetYTitle("Number of Entries");

    gDirectory->cd("..");

    gDirectory->mkdir("jets",
		      "Histograms before fit ");
    gDirectory->cd("jets");

    // b-jet
    fHistBJetEt_before_hadr = new TH1F("h_bjet_et_before_hadr",
				  "E_{T} of b-jet before_hadr fit",
				  50,0.,200);
    fHistBJetEt_before_hadr->SetXTitle("E_{T}");
    fHistBJetEt_before_hadr->SetYTitle("Number of Entries");

    fHistBJetEta_before_hadr = new TH1F("h_bjet_eta_before_hadr",
				  "#eta of b-jet before_hadr fit",
				  50,-5.,5.0);
    fHistBJetEta_before_hadr->SetXTitle("#eta");
    fHistBJetEta_before_hadr->SetYTitle("Number of Entries");
    
    fHistBJetPhi_before_hadr = new TH1F("h_bjet_phi_before_hadr",
				  "#varphi of b-jet before_hadr fit",
				  50,-3.2,3.2);
    fHistBJetPhi_before_hadr->SetXTitle("#varphi");
    fHistBJetPhi_before_hadr->SetYTitle("Number of Entries");
    
    fHistSignalLeptonEta_before_cells_hadr = new TH1F("h_signal_lepton_eta_before_cells_hadr",
						      "#eta of signal lepton before fit (cell granularity)",
						      100,-2.5,2.5);
    fHistSignalLeptonEta_before_cells_hadr->SetXTitle("#eta");
    fHistSignalLeptonEta_before_cells_hadr->SetYTitle("Number of Entries");


    fHistBJetWeight_before_hadr = new TH1F("h_bjet_weight_before_hadr",
				  "b-tag weight of b-jet used in fit",
				  50,-10,12);
    fHistBJetWeight_before_hadr->SetXTitle("B-Tag Weight");
    fHistBJetWeight_before_hadr->SetYTitle("Number of Entries");

        // light-jet1
    fHistJet1Et_before_hadr = new TH1F("h_jet1_et_before_hadr",
				  "E_{T} of jet1 before_hadr fit",
				  50,0.,200);
    fHistJet1Et_before_hadr->SetXTitle("E_{T}");
    fHistJet1Et_before_hadr->SetYTitle("Number of Entries");

    fHistJet1Eta_before_hadr = new TH1F("h_jet1_eta_before_hadr",
				  "#eta of jet1 before_hadr fit",
				  50,-5.,5.0);
    fHistJet1Eta_before_hadr->SetXTitle("#eta");
    fHistJet1Eta_before_hadr->SetYTitle("Number of Entries");
    
    fHistJet1Phi_before_hadr = new TH1F("h_jet1_phi_before_hadr",
				  "#varphi of jet1 before_hadr fit",
				  50,-3.2,3.2);
    fHistJet1Phi_before_hadr->SetXTitle("#varphi");
    fHistJet1Phi_before_hadr->SetYTitle("Number of Entries");

        // light-jet2
    fHistJet2Et_before_hadr = new TH1F("h_jet2_et_before_hadr",
				  "E_{T} of jet2 before_hadr fit",
				  50,0.,200);
    fHistJet2Et_before_hadr->SetXTitle("E_{T}");
    fHistJet2Et_before_hadr->SetYTitle("Number of Entries");

    fHistJet2Eta_before_hadr = new TH1F("h_jet2_eta_before_hadr",
				  "#eta of jet2 before_hadr fit",
				  50,-5.,5.0);
    fHistJet2Eta_before_hadr->SetXTitle("#eta");
    fHistJet2Eta_before_hadr->SetYTitle("Number of Entries");
    
    fHistJet2Phi_before_hadr = new TH1F("h_je2t_phi_before_hadr",
				  "#varphi of jet2 before_hadr fit",
				  50,-3.2,3.2);
    fHistJet2Phi_before_hadr->SetXTitle("#varphi");
    fHistJet2Phi_before_hadr->SetYTitle("Number of Entries");

    gDirectory->cd("..");

    gDirectory->mkdir("EtMiss",
		      "Histograms before fit ");
    gDirectory->cd("EtMiss");
      
    // Et_miss
    fHistEtMissMag_before_hadr = new TH1F("h_etmiss_mag_before_hadr",
				  "Magnitude of missing E_{T} before_hadr fit",
				  100,0.,300);
    fHistEtMissMag_before_hadr->SetXTitle("E_{T}^{miss}");
    fHistEtMissMag_before_hadr->SetYTitle("Number of Entries");
    
    fHistEtMissPhi_before_hadr = new TH1F("h_etmiss_phi_before_hadr",
				  "#varphi of missing E_{T} before_hadr fit",
				  50,-3.2,3.2);
    fHistEtMissPhi_before_hadr->SetXTitle("#varphi");
    fHistEtMissPhi_before_hadr->SetYTitle("Number of Entries");

    // SumEt
    fHistSumEt_before_hadr = new TH1F("h_sumet_before_hadr",
				  "SumEt in events used for fit",
				  100,0,1000);
    fHistSumEt_before_hadr->SetXTitle("#sum E_{t} (GeV)");
    fHistSumEt_before_hadr->SetYTitle("Number of Entries");
    gDirectory->cd("..");

    //back to the top
    gDirectory->cd("..");

    gDirectory->mkdir("After",
		      "Histograms after fit ");
    gDirectory->cd("After");
    gDirectory->mkdir("jets",
		      "Histograms before fit ");
    gDirectory->cd("jets");

    // b-jet
    fHistBJetEt_after_hadr = new TH1F("h_bjet_et_after_hadr",
				  "E_{T} of b-jet after_hadr fit",
				  50,0.,200);
    fHistBJetEt_after_hadr->SetXTitle("E_{T}");
    fHistBJetEt_after_hadr->SetYTitle("Number of Entries");

    fHistBJetEta_after_hadr = new TH1F("h_bjet_eta_after_hadr",
				  "#eta of b-jet after_hadr fit",
				  50,-5.,5.0);
    fHistBJetEta_after_hadr->SetXTitle("#eta");
    fHistBJetEta_after_hadr->SetYTitle("Number of Entries");
    
    fHistBJetPhi_after_hadr = new TH1F("h_bjet_phi_after_hadr",
				  "#varphi of b-jet after_hadr fit",
				  50,-3.2,3.2);
    fHistBJetPhi_after_hadr->SetXTitle("#varphi");
    fHistBJetPhi_after_hadr->SetYTitle("Number of Entries");

        // light-jet1
    fHistJet1Et_after_hadr = new TH1F("h_jet1_et_after_hadr",
				  "E_{T} of jet1 after_hadr fit",
				  50,0.,200);
    fHistJet1Et_after_hadr->SetXTitle("E_{T}");
    fHistJet1Et_after_hadr->SetYTitle("Number of Entries");

    fHistJet1Eta_after_hadr = new TH1F("h_jet1_eta_after_hadr",
				  "#eta of jet1 after_hadr fit",
				  50,-5.,5.0);
    fHistJet1Eta_after_hadr->SetXTitle("#eta");
    fHistJet1Eta_after_hadr->SetYTitle("Number of Entries");
    
    fHistJet1Phi_after_hadr = new TH1F("h_jet1_phi_after_hadr",
				  "#varphi of jet1 after_hadr fit",
				  50,-3.2,3.2);
    fHistJet1Phi_after_hadr->SetXTitle("#varphi");
    fHistJet1Phi_after_hadr->SetYTitle("Number of Entries");

        // light-jet2
    fHistJet2Et_after_hadr = new TH1F("h_jet2_et_after_hadr",
				  "E_{T} of jet2 after_hadr fit",
				  50,0.,200);
    fHistJet2Et_after_hadr->SetXTitle("E_{T}");
    fHistJet2Et_after_hadr->SetYTitle("Number of Entries");

    fHistJet2Eta_after_hadr = new TH1F("h_jet2_eta_after_hadr",
				  "#eta of jet2 after_hadr fit",
				  50,-5.,5.0);
    fHistJet2Eta_after_hadr->SetXTitle("#eta");
    fHistJet2Eta_after_hadr->SetYTitle("Number of Entries");
    
    fHistJet2Phi_after_hadr = new TH1F("h_je2t_phi_after_hadr",
				  "#varphi of jet2 after_hadr fit",
				  50,-3.2,3.2);
    fHistJet2Phi_after_hadr->SetXTitle("#varphi");
    fHistJet2Phi_after_hadr->SetYTitle("Number of Entries");
    gDirectory->cd("..");
    
    gDirectory->mkdir("EtMiss",
		      "Histograms before fit ");
    gDirectory->cd("EtMiss");

    // Et_miss
    fHistEtMissMag_after_hadr = new TH1F("h_etmiss_mag_after_hadr",
				    "Magnitude of missing E_{T} after_hadr fit",
				  50,0.,200);
    fHistEtMissMag_after_hadr->SetXTitle("E_{T}^{miss}");
    fHistEtMissMag_after_hadr->SetYTitle("Number of Entries");
    
    fHistEtMissPhi_after_hadr = new TH1F("h_etmiss_phi_after_hadr",
				  "#varphi of missing E_{T} after_hadr fit",
				  50,-3.2,3.2);
    fHistEtMissPhi_after_hadr->SetXTitle("#varphi");
    fHistEtMissPhi_after_hadr->SetYTitle("Number of Entries");    
    gDirectory->cd("..");
    gDirectory->cd("../..");
}

//____________________________________________________________________
void AtlSgTop_WtChannelFinder::ReconstructW() {
    //
    // reconstruction of W transverse 4-momentum and mass
    //

    if ( fRecoTriangularW != 0 ) {
	delete fRecoTriangularW;
	fRecoTriangularW = 0;
    }
    
    // Require at least a leading lepton
    HepParticle *Lepton = fLepton;//(HepParticle*)fLeptons->At(0);
    if ( Lepton != 0 ) {
	// Get neutrino 4-vector by means of Et_miss	
	TLorentzVector p_nu(fMET.Px(),
			    fMET.Py(),
			    0.,fMET.Mod());
	
	// Get (transverse) leading lepton 4-momentum
	TLorentzVector p_lep = Lepton->P();
	p_lep.SetE(p_lep.Et());
	p_lep.SetPz(0.);
	
	// Reconstruct (transverse) W 4-momentum
	TLorentzVector p_W = p_nu + p_lep;

	// Which W?
	Int_t Wpdg = -24;
	if ( Lepton->IsPositive() ) Wpdg = 24;
	fRecoTriangularW = new HepParticle(1,p_W.Px(),p_W.Py(),p_W.Pz(),p_W.E(), Wpdg);

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

void AtlSgTop_WtChannelFinder::BookLeptonHistograms_BTag() {
    //
   // Book histograms for the leptons used in the reconstruction
    //
    gDirectory->mkdir("CommonEventSelection_BTag", "Objects used for common event selection");
    gDirectory->cd("CommonEventSelection_BTag");
    
    gDirectory->mkdir("SignalLepton","Signal lepton");
    gDirectory->cd("SignalLepton");

    // ===========
    // All leptons
    // ===========
    gDirectory->mkdir("AllSignalLeptons","Inclusive signal leptons");
    gDirectory->cd("AllSignalLeptons");

    
    fHistSignalLepton_pt_btag = new TH1F("h_sig_lepton_pt_btag", "Signal Lepton Transverse Momentum",
				    40, 0, 200);
    fHistSignalLepton_pt_btag->SetXTitle("P_{t} (GeV)");
    fHistSignalLepton_pt_btag->SetYTitle("Number of Entries");
    fHistSignalLepton_eta_btag = new TH1F("h_sig_lepton_eta_btag", "Signal Lepton Pseudo-Rapidity",
				     40, -2.5, 2.5);
    fHistSignalLepton_eta_btag->SetXTitle("#eta");
    fHistSignalLepton_eta_btag->SetYTitle("Number of Entries");
    fHistSignalLepton_eta_btag_cells = new TH1F("h_sig_lepton_eta_btag_cells", "Signal Lepton Pseudo-Rapidity (cell granularity) after b-tag",
						100, -2.5, 2.5);
    fHistSignalLepton_eta_btag_cells->SetXTitle("#eta");
  	     fHistSignalLepton_eta_btag_cells->SetYTitle("Number of Entries");
    fHistSignalLepton_phi_btag = new TH1F("h_sig_lepton_phi_btag", "Signal Lepton Azimuth",
				     40, -3.2, 3.2);
    fHistSignalLepton_phi_btag->SetXTitle("#phi (rad)");
    fHistSignalLepton_phi_btag->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // =========
    // Electrons
    // =========
    gDirectory->mkdir("SignalElectron","Signal electron");
    gDirectory->cd("SignalElectron");


    
    //****************************************
    // Number of signal-electrons by author
    //****************************************
    fHistSignalElectron_author_btag = new TH1F("h_signal_electron_author_btag",
		  "Signal Electron Author",
		  (AtlElectron::fgNAuthors + 1), 0, (AtlElectron::fgNAuthors + 1 ));
    fHistSignalElectron_author_btag->SetYTitle("Number of Entries");
    TAxis *axis = fHistSignalElectron_author_btag->GetXaxis();
    for ( Int_t i = 0; i < AtlElectron::fgNAuthors; i++ )
	axis->SetBinLabel(i+1, AtlElectron::fgAuthorNames[i]);
    axis->SetBinLabel(6,"HighPt and Soft");

    
    // Both charges
    gDirectory->mkdir("both");
    gDirectory->cd("both");

    fHistSignalElectron_pt_btag = new TH1F("h_sig_electron_pt_btag", "Signal e Transverse Momentum",
				      200, 0, 200);
    fHistSignalElectron_pt_btag->SetXTitle("P_{t} (GeV)");
    fHistSignalElectron_pt_btag->SetYTitle("Number of Entries");
    fHistSignalElectron_eta_btag = new TH1F("h_sig_electron_eta_btag_btag", "Signal e Pseudo-Rapidity",
				       50, -2.5, 2.5);
    fHistSignalElectron_eta_btag->SetXTitle("#eta");
    fHistSignalElectron_eta_btag->SetYTitle("Number of Entries");
    fHistSignalElectron_phi_btag = new TH1F("h_sig_electron_phi_btag", "Signal e Azimuth",
				       128, -3.2, 3.2);
    fHistSignalElectron_phi_btag->SetXTitle("#phi (rad)");
    fHistSignalElectron_phi_btag->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // e+
    gDirectory->mkdir("e+");
    gDirectory->cd("e+");

    fHistSignalEplus_pt_btag = new TH1F("h_sig_eplus_pt_btag", "Signal e^{+} Transverse Momentum",
				   200, 0, 200);
    fHistSignalEplus_pt_btag->SetXTitle("P_{t} (GeV)");
    fHistSignalEplus_pt_btag->SetYTitle("Number of Entries");
    fHistSignalEplus_eta_btag = new TH1F("h_sig_eplus_eta_btag", "Signal e^{+} Pseudo-Rapidity",
				    50, -2.5, 2.5);
    fHistSignalEplus_eta_btag->SetXTitle("#eta");
    fHistSignalEplus_eta_btag->SetYTitle("Number of Entries");
    fHistSignalEplus_phi_btag = new TH1F("h_sig_eplus_phi_btag", "Signal e^{+} Azimuth",
				    128, -3.2, 3.2);
    fHistSignalEplus_phi_btag->SetXTitle("#phi (rad)");
    fHistSignalEplus_phi_btag->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // e-
    gDirectory->mkdir("e-");
    gDirectory->cd("e-");

    fHistSignalEminus_pt_btag = new TH1F("h_sig_eminus_pt_btag", "Signal e^{-} Transverse Momentum",
				   200, 0, 200);
    fHistSignalEminus_pt_btag->SetXTitle("P_{t} (GeV)");
    fHistSignalEminus_pt_btag->SetYTitle("Number of Entries");
    fHistSignalEminus_eta_btag = new TH1F("h_sig_eminus_eta_btag", "Signal e^{-} Pseudo-Rapidity",
				     50, -2.5, 2.5);
    fHistSignalEminus_eta_btag->SetXTitle("#eta");
    fHistSignalEminus_eta_btag->SetYTitle("Number of Entries");
    fHistSignalEminus_phi_btag = new TH1F("h_sig_eminus_phi_btag", "Signal e^{-} Azimuth",
				    128, -3.2, 3.2);
    fHistSignalEminus_phi_btag->SetXTitle("#phi (rad)");
    fHistSignalEminus_phi_btag->SetYTitle("Number of Entries");

    gDirectory->cd("../..");

    // =====
    // Muons
    // =====
    gDirectory->mkdir("SignalMuon","Signal muon");
    gDirectory->cd("SignalMuon");
    
    // Both charges
    gDirectory->mkdir("both");
    gDirectory->cd("both");

    fHistSignalMuon_pt_btag = new TH1F("h_sig_muon_pt_btag", "Signal #mu Transverse Momentum",
				  200, 0, 200);
    fHistSignalMuon_pt_btag->SetXTitle("P_{t} (GeV)");
    fHistSignalMuon_pt_btag->SetYTitle("Number of Entries");
    fHistSignalMuon_eta_btag = new TH1F("h_sig_muon_eta_btag", "Signal #mu Pseudo-Rapidity",
				   50, -2.5, 2.5);
    fHistSignalMuon_eta_btag->SetXTitle("#eta");
    fHistSignalMuon_eta_btag->SetYTitle("Number of Entries");
    fHistSignalMuon_phi_btag = new TH1F("h_sig_muon_phi_btag", "Signal #mu Azimuth",
				   128, -3.2, 3.2);
    fHistSignalMuon_phi_btag->SetXTitle("#phi (rad)");
    fHistSignalMuon_phi_btag->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // mu+
    gDirectory->mkdir("mu+");
    gDirectory->cd("mu+");

    fHistSignalMuplus_pt_btag = new TH1F("h_sig_muplus_pt_btag", "Signal #mu^{+} Transverse Momentum",
				    200, 0, 200);
    fHistSignalMuplus_pt_btag->SetXTitle("P_{t} (GeV)");
    fHistSignalMuplus_pt_btag->SetYTitle("Number of Entries");
    fHistSignalMuplus_eta_btag = new TH1F("h_sig_muplus_eta_btag", "Signal #mu^{+} Pseudo-Rapidity",
				     50, -2.5, 2.5);
    fHistSignalMuplus_eta_btag->SetXTitle("#eta");
    fHistSignalMuplus_eta_btag->SetYTitle("Number of Entries");
    fHistSignalMuplus_phi_btag = new TH1F("h_sig_muplus_phi_btag", "Signal #mu^{+} Azimuth",
				     128, -3.2, 3.2);
    fHistSignalMuplus_phi_btag->SetXTitle("#phi (rad)");
    fHistSignalMuplus_phi_btag->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    // mu-
    gDirectory->mkdir("mu-");
    gDirectory->cd("mu-");

    fHistSignalMuminus_pt_btag = new TH1F("h_sig_muminus_pt_btag", "Signal #mu^{-} Transverse Momentum",
				   200, 0, 200);
    fHistSignalMuminus_pt_btag->SetXTitle("P_{t} (GeV)");
    fHistSignalMuminus_pt_btag->SetYTitle("Number of Entries");
    fHistSignalMuminus_eta_btag = new TH1F("h_sig_muminus_eta_btag", "Signal #mu^{-} Pseudo-Rapidity",
				     50, -2.5, 2.5);
    fHistSignalMuminus_eta_btag->SetXTitle("#eta");
    fHistSignalMuminus_eta_btag->SetYTitle("Number of Entries");
    fHistSignalMuminus_phi_btag = new TH1F("h_sig_muminus_phi_btag", "Signal #mu^{-} Azimuth",
				    128, -3.2, 3.2);
    fHistSignalMuminus_phi_btag->SetXTitle("#phi (rad)");
    fHistSignalMuminus_phi_btag->SetYTitle("Number of Entries");

    
    gDirectory->cd("../../..");
}

//____________________________________________________________________

void AtlSgTop_WtChannelFinder::BookJetHistograms_BTag() {
    //
    // Book jet histograms
    //
    gDirectory->mkdir("SignalJets","Signal jets");
    gDirectory->cd("SignalJets");

    // ======
    // Jets
    // ======
    gDirectory->mkdir("all_jets","All selected signal jets");
    gDirectory->cd("all_jets");

    // All jets
    fHistJet_n_btag = new TH1F("h_jet_n_btag", "No. of all jets",
			   10, 0, 10);
    fHistJet_n_btag->SetXTitle("N_{jet}");
    fHistJet_n_btag->SetYTitle("Number of Entries");
    fHistJet_Et_btag = new TH1F("h_jet_Et_btag", "Transverse Energy of all jets",
			    40, 0, 200);
    fHistJet_Et_btag->SetXTitle("E_{t} (GeV)");
    fHistJet_Et_btag->SetYTitle("Number of Entries");
    fHistJet_eta_btag = new TH1F("h_jet_eta_btag", "Pseudo-Rapidity of all jets",
			     40, -2.5, 2.5);
    fHistJet_eta_btag->SetXTitle("#eta");
    fHistJet_eta_btag->SetYTitle("Number of Entries");
    fHistJet_phi_btag = new TH1F("h_jet_phi_btag", "Azimuth of all jets",
			     40, -3.2, 3.2);
    fHistJet_phi_btag->SetXTitle("#phi (rad)");
    fHistJet_phi_btag->SetYTitle("Number of Entries");

    // Leading jet
    fHistJet1_Et_btag = new TH1F("h_jet1_Et_btag", "Transverse Energy of leading jet",
			    40, 0, 200);
    fHistJet1_Et_btag->SetXTitle("E_{t} (GeV)");
    fHistJet1_Et_btag->SetYTitle("Number of Entries");
    fHistJet1_eta_btag = new TH1F("h_jet1_eta_btag", "Pseudo-Rapidity of leading jet",
			     40, -2.5, 2.5);
    fHistJet1_eta_btag->SetXTitle("#eta");
    fHistJet1_eta_btag->SetYTitle("Number of Entries");
    fHistJet1_phi_btag = new TH1F("h_jet1_phi_btag", "Azimuth of leading jet",
			     40, -3.2, 3.2);
    fHistJet1_phi_btag->SetXTitle("#phi (rad)");
    fHistJet1_phi_btag->SetYTitle("Number of Entries");

    // 2nd Leading jet
    fHistJet2_Et_btag = new TH1F("h_jet2_Et_btag", "Transverse Energy of 2nd leading jet",
			    40, 0, 200);
    fHistJet2_Et_btag->SetXTitle("E_{t} (GeV)");
    fHistJet2_Et_btag->SetYTitle("Number of Entries");
    fHistJet2_eta_btag = new TH1F("h_jet2_eta_btag", "Pseudo-Rapidity of 2nd leading jet",
			     40, -2.5, 2.5);
    fHistJet2_eta_btag->SetXTitle("#eta");
    fHistJet2_eta_btag->SetYTitle("Number of Entries");
    fHistJet2_phi_btag = new TH1F("h_jet2_phi_btag", "Azimuth of 2nd leading jet",
			     40, -3.2, 3.2);
    fHistJet2_phi_btag->SetXTitle("#phi (rad)");
    fHistJet2_phi_btag->SetYTitle("Number of Entries");

    gDirectory->cd("..");


    // ======
    // B-Jets
    // ======
    gDirectory->mkdir("b-jet","Signal b-jet");
    gDirectory->cd("b-jet");

    // All b-jets
    fHistBJet_n_btag = new TH1F("h_bjet_n_btag", "No. of all b-jets",
			   10, 0, 10);
    fHistBJet_n_btag->SetXTitle("N_{b-jet}");
    fHistBJet_n_btag->SetYTitle("Number of Entries");
    fHistBJet_Et_btag = new TH1F("h_bjet_Et_btag", "Transverse Energy of all b-jets",
			    40, 0, 200);
    fHistBJet_Et_btag->SetXTitle("E_{t} (GeV)");
    fHistBJet_Et_btag->SetYTitle("Number of Entries");
    fHistBJet_eta_btag = new TH1F("h_bjet_eta_btag", "Pseudo-Rapidity of all b-jets",
			     40, -2.5, 2.5);
    fHistBJet_eta_btag->SetXTitle("#eta");
    fHistBJet_eta_btag->SetYTitle("Number of Entries");
    fHistBJet_phi_btag = new TH1F("h_bjet_phi_btag", "Azimuth of all b-jets",
			     40, -3.2, 3.2);
    fHistBJet_phi_btag->SetXTitle("#phi (rad)");
    fHistBJet_phi_btag->SetYTitle("Number of Entries");
    fHistBJet_weight_btag = new TH1F("h_bjet_weight_btag", "B-tag weights of all b-jets",
			     500, 0., 12);
    fHistBJet_weight_btag->SetXTitle("B-Tag Weight");
    fHistBJet_weight_btag->SetYTitle("Number of Entries");

    // Leading b-jet
    fHistBJet1_Et_btag = new TH1F("h_bjet1_Et_btag", "Transverse Energy of leading b-jet",
			    40, 0, 200);
    fHistBJet1_Et_btag->SetXTitle("E_{t} (GeV)");
    fHistBJet1_Et_btag->SetYTitle("Number of Entries");
    fHistBJet1_eta_btag = new TH1F("h_bjet1_eta_btag", "Pseudo-Rapidity of leading b-jet",
			     40, -2.5, 2.5);
    fHistBJet1_eta_btag->SetXTitle("#eta");
    fHistBJet1_eta_btag->SetYTitle("Number of Entries");
    fHistBJet1_phi_btag = new TH1F("h_bjet1_phi_btag", "Azimuth of leading b-jet",
			     40, -3.2, 3.2);
    fHistBJet1_phi_btag->SetXTitle("#phi (rad)");
    fHistBJet1_phi_btag->SetYTitle("Number of Entries");
    fHistBJet1_weight_btag = new TH1F("h_bjet1_weight_btag", "B-tag weight of leading b-jets",
			     50, -10, 12);
    fHistBJet1_weight_btag->SetXTitle("B-Tag Weight");
    fHistBJet1_weight_btag->SetYTitle("Number of Entries");

    // 2nd Leading b-jet
    fHistBJet2_Et_btag = new TH1F("h_bjet2_Et_btag", "Transverse Energy of 2nd leading b-jet",
			    40, 0, 200);
    fHistBJet2_Et_btag->SetXTitle("E_{t} (GeV)");
    fHistBJet2_Et_btag->SetYTitle("Number of Entries");
    fHistBJet2_eta_btag = new TH1F("h_bjet2_eta_btag", "Pseudo-Rapidity of 2nd leading b-jet",
			     40, -2.5, 2.5);
    fHistBJet2_eta_btag->SetXTitle("#eta");
    fHistBJet2_eta_btag->SetYTitle("Number of Entries");
    fHistBJet2_phi_btag = new TH1F("h_bjet2_phi_btag", "Azimuth of 2nd leading b-jet",
			     40, -3.2, 3.2);
    fHistBJet2_phi_btag->SetXTitle("#phi (rad)");
    fHistBJet2_phi_btag->SetYTitle("Number of Entries");
    fHistBJet2_weight_btag = new TH1F("h_bjet2_weight_btag", "B-tag weight of 2nd leading b-jet",
			     50, -10, 12);
    fHistBJet2_weight_btag->SetXTitle("B-Tag Weight");
    fHistBJet2_weight_btag->SetYTitle("Number of Entries");

    gDirectory->cd("..");


    // ======
    // Non-B-Jets
    // ======
    gDirectory->mkdir("non-b-jet","Signal non-b-jet");
    gDirectory->cd("non-b-jet");

    // All non-b-jets
    fHistNonBJet_n_btag = new TH1F("h_nonbjet_n_btag", "No. of all non-b-jets",
			   10, 0, 10);
    fHistNonBJet_n_btag->SetXTitle("N_{non-b-jet}");
    fHistNonBJet_n_btag->SetYTitle("Number of Entries");
    fHistNonBJet_Et_btag = new TH1F("h_nonbjet_Et_btag", "Transverse Energy of all non-b-jets",
			    40, 0, 200);
    fHistNonBJet_Et_btag->SetXTitle("E_{t} (GeV)");
    fHistNonBJet_Et_btag->SetYTitle("Number of Entries");
    fHistNonBJet_eta_btag = new TH1F("h_nonbjet_eta_btag", "Pseudo-Rapidity of all non-b-jets",
			     40, -2.5, 2.5);
    fHistNonBJet_eta_btag->SetXTitle("#eta");
    fHistNonBJet_eta_btag->SetYTitle("Number of Entries");
    fHistNonBJet_phi_btag = new TH1F("h_nonbjet_phi_btag", "Azimuth of all non-b-jets",
			     40, -3.2, 3.2);
    fHistNonBJet_phi_btag->SetXTitle("#phi (rad)");
    fHistNonBJet_phi_btag->SetYTitle("Number of Entries");

    // Leading non-b-jet
    fHistNonBJet1_Et_btag = new TH1F("h_nonbjet1_Et_btag", "Transverse Energy of leading non-b-jet",
			    40, 0, 200);
    fHistNonBJet1_Et_btag->SetXTitle("E_{t} (GeV)");
    fHistNonBJet1_Et_btag->SetYTitle("Number of Entries");
    fHistNonBJet1_eta_btag = new TH1F("h_nonbjet1_eta_btag", "Pseudo-Rapidity of leading non-b-jet",
			     40, -2.5, 2.5);
    fHistNonBJet1_eta_btag->SetXTitle("#eta");
    fHistNonBJet1_eta_btag->SetYTitle("Number of Entries");
    fHistNonBJet1_phi_btag = new TH1F("h_nonbjet1_phi_btag", "Azimuth of leading non-b-jet",
			     40, -3.2, 3.2);
    fHistNonBJet1_phi_btag->SetXTitle("#phi (rad)");
    fHistNonBJet1_phi_btag->SetYTitle("Number of Entries");

    // 2nd Leading non-b-jet
    fHistNonBJet2_Et_btag = new TH1F("h_nonbjet2_Et_btag", "Transverse Energy of 2nd leading non-b-jet",
			    40, 0, 200);
    fHistNonBJet2_Et_btag->SetXTitle("E_{t} (GeV)");
    fHistNonBJet2_Et_btag->SetYTitle("Number of Entries");
    fHistNonBJet2_eta_btag = new TH1F("h_nonbjet2_eta_btag", "Pseudo-Rapidity of 2nd leading non-b-jet",
			     40, -2.5, 2.5);
    fHistNonBJet2_eta_btag->SetXTitle("#eta");
    fHistNonBJet2_eta_btag->SetYTitle("Number of Entries");
    fHistNonBJet2_phi_btag = new TH1F("h_nonbjet2_phi_btag", "Azimuth of 2nd leading non-b-jet",
			     40, -3.2, 3.2);
    fHistNonBJet2_phi_btag->SetXTitle("#phi (rad)");
    fHistNonBJet2_phi_btag->SetYTitle("Number of Entries");

    gDirectory->cd("../../..");
}

//____________________________________________________________________

 

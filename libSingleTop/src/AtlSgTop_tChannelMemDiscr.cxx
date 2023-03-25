//____________________________________________________________________
//
// Single-top t-Channel Mem Discriminant Analysis
// 
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2013 (C) Soeren Stamm
// 
#ifndef SINGLETOP_AtlSgTop_tChannelMemDiscr
#include <AtlSgTop_tChannelMemDiscr.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlSgTop_tChannelMemDiscr);
#endif

const Double_t AtlSgTop_tChannelMemDiscr::fgBinEdges_tChannel4FSRatio[fgNBins_tChannel4FSRatio+1] = {
    0.00000000, 0.05720000, 0.10245000, 0.14880000, 0.19485000,
    0.24065000, 0.28830000, 0.33725000, 0.38955000, 0.44280000,
    0.49965000, 0.55915000, 0.62080000, 0.68120000, 0.74070000,
    0.79490000, 0.84405000, 0.88805000, 0.92735000, 0.96095000,
    1.00000000};

const Double_t AtlSgTop_tChannelMemDiscr::fgBinEdges_tChannel5FSRatio[fgNBins_tChannel5FSRatio+1] = {
    0.00000000, 0.00240000, 0.02945000, 0.08520000, 0.15505000,
    0.23185000, 0.31030000, 0.39220000, 0.47825000, 0.56455000,
    0.65025000, 0.72795000, 0.79470000, 0.84965000, 0.89300000,
    0.92580000, 0.95045000, 0.96965000, 0.98450000, 0.99385000,
    1.00000000};
    
//____________________________________________________________________

AtlSgTop_tChannelMemDiscr::AtlSgTop_tChannelMemDiscr(const char* OutputFile) :
    AtlSelector(OutputFile) {
    //
    // Default constructor
    //
    
    // Init
    fLeptons   = 0;
    fLooseLeptons = 0;
    fElectrons = 0;
    fMuons     = 0;
    fJets      = 0;
    fBJets     = 0;
    fObjTool   = 0;
    fSFTool    = 0;
    fPdfTool   = 0;

    SetCutDefaults();
	
}

//____________________________________________________________________

AtlSgTop_tChannelMemDiscr::~AtlSgTop_tChannelMemDiscr() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlSgTop_tChannelMemDiscr::Clear(Option_t *option) {
    //
    //  Clears the dynamically allocated objects
    //
}

//____________________________________________________________________

void AtlSgTop_tChannelMemDiscr::Begin(TTree *tree) {
    //
    // Begin analysis job
    //

    // Create event writer
    fEvtWriter = 0;

    AtlSelector::Begin(tree);
}

//____________________________________________________________________

void AtlSgTop_tChannelMemDiscr::SetBranchStatus() {
    //
    // Switch on/off branches in order to gain speed
    //

    // Switch off all branches by default
    fTree->SetBranchStatus("*", kFALSE);
}

//____________________________________________________________________

void AtlSgTop_tChannelMemDiscr::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    fPrintEvent = kFALSE;
    // this _is_ used to determine the bookkeeping histogram names
    fInputMode  = kD3PDSgTop_v2;
    // fInputMode  = kD3PDSgTop;

    fSchannelVeto = kTRUE;

    
    fEvtReader  = new AtlEvtReaderMem(this);
    Info("SetCutDefaults", "Using AtlEvtReaderMem");
}

//____________________________________________________________________

void AtlSgTop_tChannelMemDiscr::BookHistograms() {
    //
    // Book histograms
    //    
	
    // Get ObjectSelection Tool + pointers to analysis objects
    fObjTool = 
	(AtlObjectsToolD3PDSgTop*)GetTool("AtlObjectsToolD3PDSgTop"
					  ,"",kTRUE);
    fElectrons = fObjTool->GetElectrons();
    fMuons     = fObjTool->GetMuons();
    fLeptons   = fObjTool->GetLeptons();
    fLooseLeptons = fObjTool->GetLooseLeptons();
    fJets      = fObjTool->GetJets();
    fBJets     = fObjTool->GetBJets();

    // Provide tools with pointers to analysis objects
    fSFTool = 
	(AtlObjRecoScaleFactorTool*)GetTool("AtlObjRecoScaleFactorTool"
					    ,"",kTRUE);
    fSFTool->SetLeptons(fLeptons);
    fSFTool->SetJets(fJets);

    // Provide tools with histogram names
    fPdfTool = 
    	(AtlPdfReweightingTool*)GetTool("AtlPdfReweightingTool"
    					,"",kTRUE);
    
    fPdfTool->AddObservable("MemDiscriminant/tChannelProb4FS");
    fPdfTool->AddObservable("MemDiscriminant/tChannelProb4FS_odd");
    fPdfTool->AddObservable("MemDiscriminant/tChannelProb4FS_even");

    // a-priori probabilities

    // probabilities after di-lepton veto
    fP_sChannel2j_ePlus   = 0.00467074;
    fP_sChannel2j_muPlus  = 0.0062965;
    fP_sChannel2j_eMinus  = 0.00266235;
    fP_sChannel2j_muMinus = 0.00353846;
    
    fP_tChannel4FS_ePlus   = 0.0963275;
    fP_tChannel4FS_muPlus  = 0.123222;	
    fP_tChannel4FS_eMinus  = 0.0531231;
    fP_tChannel4FS_muMinus = 0.0652147;
    
    fP_Wjj_ePlus   = 0.0045943;
    fP_Wjj_muPlus  = 0.0093268;
    fP_Wjj_eMinus  = 0.00327138;
    fP_Wjj_muMinus = 0.00414807;
    
    Double_t wbbVSwcjFrac = 0.5; // guestimated!
    // redo plotting with other plotting scheme to get a more precise number!
    
    Double_t wHF_ePlus   = 0.0753649;
    Double_t wHF_muPlus  = 0.103652;
    Double_t wHF_eMinus  = 0.0606024;
    Double_t wHF_muMinus = 0.0781944;
    
    fP_Wbb_ePlus   = wbbVSwcjFrac * wHF_ePlus;
    fP_Wbb_muPlus  = wbbVSwcjFrac * wHF_muPlus;
    fP_Wbb_eMinus  = wbbVSwcjFrac * wHF_eMinus;
    fP_Wbb_muMinus = wbbVSwcjFrac * wHF_muMinus;
    
    fP_Wcj_ePlus   = (1.0 - wbbVSwcjFrac) * wHF_ePlus;
    fP_Wcj_muPlus  = (1.0 - wbbVSwcjFrac) * wHF_muPlus;
    fP_Wcj_eMinus  = (1.0 - wbbVSwcjFrac) * wHF_eMinus;
    fP_Wcj_muMinus = (1.0 - wbbVSwcjFrac) * wHF_muMinus;
    
    Double_t ttbarSL_vs_ttbarDL_frac_el = 1. - 0.435;
    Double_t ttbarSL_vs_ttbarDL_frac_mu = 1. - 0.435;
    
    Double_t ttbar_ePlus   = 0.0680078;
    Double_t ttbar_muPlus  = 0.0844124;
    Double_t ttbar_eMinus  = 0.0684759;
    Double_t ttbar_muMinus = 0.0848944;
    
    fP_ttbarSL_ePlus   = ttbarSL_vs_ttbarDL_frac_el * ttbar_ePlus;
    fP_ttbarSL_muPlus  = ttbarSL_vs_ttbarDL_frac_mu * ttbar_muPlus;
    fP_ttbarSL_eMinus  = ttbarSL_vs_ttbarDL_frac_el * ttbar_eMinus;
    fP_ttbarSL_muMinus = ttbarSL_vs_ttbarDL_frac_mu * ttbar_muMinus;
    
    fP_ttbarDL_ePlus   = (1.0 - ttbarSL_vs_ttbarDL_frac_el) * ttbar_ePlus;
    fP_ttbarDL_muPlus  = (1.0 - ttbarSL_vs_ttbarDL_frac_mu) * ttbar_muPlus;
    fP_ttbarDL_eMinus  = (1.0 - ttbarSL_vs_ttbarDL_frac_el) * ttbar_eMinus;
    fP_ttbarDL_muMinus = (1.0 - ttbarSL_vs_ttbarDL_frac_mu) * ttbar_muMinus;
    

    gDirectory->mkdir("MemDiscriminant");
    gDirectory->cd("MemDiscriminant");
	
    // Discriminant Plots (four flavour scheme)
    fHist_llh_tChannel_ratio_4FS = new TH1F("tChannelProb4FS", "t-channel likelihood ratio 4FS",
					    fgNBins_tChannel4FSRatio, 0.0, 1.0);
    fHist_llh_tChannel_ratio_4FS->SetXTitle("P(S|X)");
    fHist_llh_tChannel_ratio_4FS->SetYTitle("Number of Entries");
    
    fHist_llh_tChannel_ratio_4FS_even = new TH1F("tChannelProb4FS_even", "t-channel likelihood ratio 4FS, even evt numbers",
						 fgNBins_tChannel4FSRatio, 0.0, 1.0);
    fHist_llh_tChannel_ratio_4FS_even->SetXTitle("P(S|X)");
    fHist_llh_tChannel_ratio_4FS_even->SetYTitle("Number of Entries");
    
    fHist_llh_tChannel_ratio_4FS_odd = new TH1F("tChannelProb4FS_odd", "t-channel likelihood ratio 4FS, odd evt numbers",
						fgNBins_tChannel4FSRatio, 0.0, 1.0);
    fHist_llh_tChannel_ratio_4FS_odd->SetXTitle("P(S|X)");
    fHist_llh_tChannel_ratio_4FS_odd->SetYTitle("Number of Entries");

    fHist_llh_tChannel_ratio2_4FS = new TH1F("tChannelProb4FS_c", "t-channel likelihood ratio 4FS",
					     25, 0.0, 1.0);
    fHist_llh_tChannel_ratio2_4FS->SetXTitle("P(S|X)");
    fHist_llh_tChannel_ratio2_4FS->SetYTitle("Number of Entries");

    fHist_llh_Wjets_ratio_4FS = new TH1F("WjetsProb4FS", "Wjets likelihood ratio 4FS",
					 25, 0.0, 1.0);
    fHist_llh_Wjets_ratio_4FS->SetXTitle("P(S|X)");
    fHist_llh_Wjets_ratio_4FS->SetYTitle("Number of Entries");

    fHist_llh_Wbb_ratio_4FS = new TH1F("WbbProb4FS", "Wbb likelihood ratio 4FS",
					 25, 0.0, 1.0);
    fHist_llh_Wbb_ratio_4FS->SetXTitle("P(S|X)");
    fHist_llh_Wbb_ratio_4FS->SetYTitle("Number of Entries");

    fHist_llh_Wcj_ratio_4FS = new TH1F("WcjProb4FS", "Wcj likelihood ratio 4FS",
					 25, 0.0, 1.0);
    fHist_llh_Wcj_ratio_4FS->SetXTitle("P(S|X)");
    fHist_llh_Wcj_ratio_4FS->SetYTitle("Number of Entries");

    fHist_llh_Wjj_ratio_4FS = new TH1F("WjjProb4FS", "Wjj likelihood ratio 4FS",
					 25, 0.0, 1.0);
    fHist_llh_Wjj_ratio_4FS->SetXTitle("P(S|X)");
    fHist_llh_Wjj_ratio_4FS->SetYTitle("Number of Entries");

    fHist_llh_ttbar_ratio_4FS = new TH1F("ttbarProb4FS", "ttbar likelihood ratio 4FS",
					 25, 0.0, 1.0);
    fHist_llh_ttbar_ratio_4FS->SetXTitle("P(S|X)");
    fHist_llh_ttbar_ratio_4FS->SetYTitle("Number of Entries");

    
    // Discriminant Plots (five flavour scheme)
    fHist_llh_tChannel_ratio_5FS = new TH1F("tChannelProb5FS", "t-channel likelihood ratio 5FS",
					    fgNBins_tChannel5FSRatio, 0.0, 1.0);
    fHist_llh_tChannel_ratio_5FS->SetXTitle("P(S|X)");
    fHist_llh_tChannel_ratio_5FS->SetYTitle("Number of Entries");
    
    fHist_llh_tChannel_ratio_5FS_even = new TH1F("tChannelProb5FS_even", "t-channel likelihood ratio 5FS, even evt numbers",
						 fgNBins_tChannel5FSRatio, 0.0, 1.0);
    fHist_llh_tChannel_ratio_5FS_even->SetXTitle("P(S|X)");
    fHist_llh_tChannel_ratio_5FS_even->SetYTitle("Number of Entries");
    
    fHist_llh_tChannel_ratio_5FS_odd = new TH1F("tChannelProb5FS_odd", "t-channel likelihood ratio 5FS, odd evt numbers",
						fgNBins_tChannel5FSRatio, 0.0, 1.0);
    fHist_llh_tChannel_ratio_5FS_odd->SetXTitle("P(S|X)");
    fHist_llh_tChannel_ratio_5FS_odd->SetYTitle("Number of Entries");

    fHist_llh_tChannel_ratio2_5FS = new TH1F("tChannelProb5FS_c", "t-channel likelihood ratio 5FS",
					     25, 0.0, 1.0);
    fHist_llh_tChannel_ratio2_5FS->SetXTitle("P(S|X)");
    fHist_llh_tChannel_ratio2_5FS->SetYTitle("Number of Entries");

    fHist_llh_Wjets_ratio_5FS = new TH1F("WjetsProb5FS", "Wjets likelihood ratio 5FS",
					 25, 0.0, 1.0);
    fHist_llh_Wjets_ratio_5FS->SetXTitle("P(S|X)");
    fHist_llh_Wjets_ratio_5FS->SetYTitle("Number of Entries");

    fHist_llh_ttbar_ratio_5FS = new TH1F("ttbarProb5FS", "ttbar likelihood ratio 5FS",
					 25, 0.0, 1.0);
    fHist_llh_ttbar_ratio_5FS->SetXTitle("P(S|X)");
    fHist_llh_ttbar_ratio_5FS->SetYTitle("Number of Entries");

    gDirectory->cd("..");

    gDirectory->mkdir("2D");
    gDirectory->cd("2D");

    fHist_2D_tChannel_4FS_vs_5FS = new TH2F("tChannel_4FS_vs_5FS", "tChannel 4FS vs 5FS",
     					    50, 0., 1.,
     					    50, 0., 1.);
    fHist_2D_tChannel_4FS_vs_5FS->SetXTitle("P(S|X) 4FS");
    fHist_2D_tChannel_4FS_vs_5FS->SetYTitle("P(S|X) 5FS");
    fHist_2D_tChannel_4FS_vs_5FS->SetZTitle("Events");

    fHist_2D_tChannel_4FS_vs_Wjets = new TH2F("tChannel_4FS_vs_Wjets", "tChannel 4FS vs Wjets discriminant",
					      25, 0.0, 1.0,
					      25, 0.0, 1.0);
    fHist_2D_tChannel_4FS_vs_Wjets->SetXTitle("P(S|X) 4FS");
    fHist_2D_tChannel_4FS_vs_Wjets->SetYTitle("P(S|X) Wjets");
    fHist_2D_tChannel_4FS_vs_Wjets->SetZTitle("Events");

    fHist_2D_tChannel_4FS_vs_Wbb = new TH2F("tChannel_4FS_vs_Wbb", "tChannel 4FS vs Wbb discriminant",
					      25, 0.0, 1.0,
					      25, 0.0, 1.0);
    fHist_2D_tChannel_4FS_vs_Wbb->SetXTitle("P(S|X) 4FS");
    fHist_2D_tChannel_4FS_vs_Wbb->SetYTitle("P(S|X) Wbb");
    fHist_2D_tChannel_4FS_vs_Wbb->SetZTitle("Events");

    fHist_2D_tChannel_4FS_vs_Wcj = new TH2F("tChannel_4FS_vs_Wcj", "tChannel 4FS vs Wcj discriminant",
					      25, 0.0, 1.0,
					      25, 0.0, 1.0);
    fHist_2D_tChannel_4FS_vs_Wcj->SetXTitle("P(S|X) 4FS");
    fHist_2D_tChannel_4FS_vs_Wcj->SetYTitle("P(S|X) Wcj");
    fHist_2D_tChannel_4FS_vs_Wcj->SetZTitle("Events");

    fHist_2D_tChannel_4FS_vs_ttbar = new TH2F("tChannel_4FS_vs_ttbar", "tChannel 4FS vs ttbar discriminant",
					      25, 0.0, 1.0,
					      25, 0.0, 1.0);
    fHist_2D_tChannel_4FS_vs_ttbar->SetXTitle("P(S|X) 4FS");
    fHist_2D_tChannel_4FS_vs_ttbar->SetYTitle("P(S|X) ttbar");
    fHist_2D_tChannel_4FS_vs_ttbar->SetZTitle("Events");

    gDirectory->cd("..");
    
    gDirectory->mkdir("ManyBins");
    gDirectory->cd("ManyBins");

    // t-channel ratio all leptons, all charge
    fHist_llh_tChannel_ratio_4FS_raw = new TH1F("tChannelProb4FS_raw", "t-channel likelihood ratio 4FS", 20000, 0.0, 1.0);
    fHist_llh_tChannel_ratio_4FS_raw->SetXTitle("P(S|X)");
    fHist_llh_tChannel_ratio_4FS_raw->SetYTitle("Number of Entries");

    fHist_llh_tChannel_ratio_5FS_raw = new TH1F("tChannelProb5FS_raw", "t-channel likelihood ratio 5FS", 20000, 0.0, 1.0);
    fHist_llh_tChannel_ratio_5FS_raw->SetXTitle("P(S|X)");
    fHist_llh_tChannel_ratio_5FS_raw->SetYTitle("Number of Entries");

    gDirectory->cd("..");

    gDirectory->cd("..");
    
    gDirectory->mkdir("MemLikelihoods");
    gDirectory->cd("MemLikelihoods");

    // Log-Likelihood Plots
    fHist_logllh_sChannel2j = new TH1F("logllh_sChannel2j", "s-channel likelihood 2j", 50., -12., 3.);
    fHist_logllh_sChannel2j->SetXTitle("Log(L_{s-channel 2j})");
    fHist_logllh_sChannel2j->SetYTitle("Number of Entries");

    fHist_logllh_tChannel4FS = new TH1F("logllh_tChannel4FS", "t-channel likelihood", 50., -12., 3.);
    fHist_logllh_tChannel4FS->SetXTitle("Log(L_{t-channel})");
    fHist_logllh_tChannel4FS->SetYTitle("Number of Entries");

    fHist_logllh_tChannel5FS = new TH1F("logllh_tChannel5FS", "t-channel likelihood", 50., -12., 3.);
    fHist_logllh_tChannel5FS->SetXTitle("Log(L_{t-channel})");
    fHist_logllh_tChannel5FS->SetYTitle("Number of Entries");

    fHist_logllh_ttbarSL = new TH1F("logllh_ttbarSL", "ttbar SL", 50., -12., 3.);
    fHist_logllh_ttbarSL->SetXTitle("Log(L_{t#bar{t}, SL})");
    fHist_logllh_ttbarSL->SetYTitle("Number of Entries");

    fHist_logllh_ttbarDL = new TH1F("logllh_ttbarDL", "ttbar DL likelihood", 50., -15, 0.);
    fHist_logllh_ttbarDL->SetXTitle("Log(L_{t#bar{t}, DL})");
    fHist_logllh_ttbarDL->SetYTitle("Number of Entries");

    fHist_logllh_Wjj = new TH1F("logllh_Wjj", "Wjj likelihood", 50., -12., 3.);
    fHist_logllh_Wjj->SetXTitle("Log(L_{Wjj})");
    fHist_logllh_Wjj->SetYTitle("Number of Entries");

    fHist_logllh_Wcj = new TH1F("logllh_Wcj", "Wcj likelihood", 50., -12., 3.);
    fHist_logllh_Wcj->SetXTitle("Log(L_{Wcj})");
    fHist_logllh_Wcj->SetYTitle("Number of Entries");

    fHist_logllh_Wbb = new TH1F("logllh_Wbb", "Wbb likelihood", 50., -12., 3.);
    fHist_logllh_Wbb->SetXTitle("Log(L_{Wb#bar{b}})");
    fHist_logllh_Wbb->SetYTitle("Number of Entries");

    gDirectory->cd("..");
}

//____________________________________________________________________

Bool_t AtlSgTop_tChannelMemDiscr::ProcessPreCut() {
    //
    // Event pre-selection
    //

    // Currently, no cuts applied

    // Remove buggy event for PDF reweighting
    if ( fEvent->RunNr() == 195847 && fEvent->EventNr() == 14022767
	 && fEvent->IsMC() ) return kFALSE;
    
    // Accept event
    return kTRUE;
}

//____________________________________________________________________

Bool_t AtlSgTop_tChannelMemDiscr::ProcessCut() {
    //
    // Event selection
    //

    if ( fSchannelVeto ) {
	AtlBTag::ETagger schan_tagger = AtlBTag::kMV1;
	Double_t schan_wp = 0.7892;
	
	AtlJet *jet1 = (AtlJet*) fJets->At(0);
	AtlJet *jet2 = (AtlJet*) fJets->At(1);
	if ( jet1->GetTag(schan_tagger)->GetWeight() > schan_wp &&
	     jet2->GetTag(schan_tagger)->GetWeight() > schan_wp )
	    return kFALSE;
    }

    // No further event selection
    return kTRUE;
}

//____________________________________________________________________

void AtlSgTop_tChannelMemDiscr::FillHistograms() {
    //
    // Fill histograms
    //
    // This method will be called only for events surviving the
    // ProcessCut() routine
    //

    HepParticle *lepton        = (HepParticle*)fLeptons->At(0);
    Float_t      lepCharge     = lepton->Charge();
    Bool_t       lepIsElectron = lepton->IsElectron();
    Bool_t       lepIsMuon     = lepton->IsMuon();
    
    Double_t llh_sChannel2j  = GetEvtReader()->v_llh_sChannel2j;
    Double_t llh_tChannel4FS = GetEvtReader()->v_llh_tChannel4FS;
    Double_t llh_tChannel5FS = GetEvtReader()->v_llh_tChannel5FS;
    Double_t llh_ttbarSL     = GetEvtReader()->v_llh_ttbarSL;
    Double_t llh_ttbarDL     = GetEvtReader()->v_llh_ttbarDL;
    Double_t llh_Wjj         = GetEvtReader()->v_llh_Wjj;
    Double_t llh_Wcj         = GetEvtReader()->v_llh_Wcj;
    Double_t llh_Wbb         = GetEvtReader()->v_llh_Wbb;

    Double_t p_sChannel2j  = 0.0;
    Double_t p_tChannel4FS = 0.0;
    Double_t p_tChannel5FS = 0.0;
    Double_t p_Wjj         = 0.0;
    Double_t p_Wbb         = 0.0;
    Double_t p_Wcj         = 0.0;
    Double_t p_ttbarSL     = 0.0;
    Double_t p_ttbarDL     = 0.0;
    
    if ( lepIsElectron ) {
	if ( lepCharge > 0.0 ) {
	    // e+
	    p_sChannel2j  = fP_sChannel2j_ePlus;
	    p_tChannel4FS = fP_tChannel4FS_ePlus;
	    p_tChannel5FS = fP_tChannel4FS_ePlus; // same as 4FS
	    p_Wjj         = fP_Wjj_ePlus;
	    p_Wbb         = fP_Wbb_ePlus;
	    p_Wcj         = fP_Wcj_ePlus;
	    p_ttbarSL     = fP_ttbarSL_ePlus;
	    p_ttbarDL     = fP_ttbarDL_ePlus;
	} else {
	    // e-
	    p_sChannel2j  = fP_sChannel2j_eMinus;
	    p_tChannel4FS = fP_tChannel4FS_eMinus;
	    p_tChannel5FS = fP_tChannel4FS_eMinus; // same as 4FS
	    p_Wjj         = fP_Wjj_eMinus;
	    p_Wbb         = fP_Wbb_eMinus;
	    p_Wcj         = fP_Wcj_eMinus;
	    p_ttbarSL     = fP_ttbarSL_eMinus;
	    p_ttbarDL     = fP_ttbarDL_eMinus;
	}
    }
    else if ( lepIsMuon ) {
	if ( lepCharge > 0.0 ) {
	    // mu+
	    p_sChannel2j  = fP_sChannel2j_muPlus;
	    p_tChannel4FS = fP_tChannel4FS_muPlus;
	    p_tChannel5FS = fP_tChannel4FS_muPlus; // same as 4FS
	    p_Wjj         = fP_Wjj_muPlus;
	    p_Wbb         = fP_Wbb_muPlus;
	    p_Wcj         = fP_Wcj_muPlus;
	    p_ttbarSL     = fP_ttbarSL_muPlus;
	    p_ttbarDL     = fP_ttbarDL_muPlus;
	} else {
	    // mu-
	    p_sChannel2j  = fP_sChannel2j_muMinus;
	    p_tChannel4FS = fP_tChannel4FS_muMinus;
	    p_tChannel5FS = fP_tChannel4FS_muMinus; // same as 4FS
	    p_Wjj         = fP_Wjj_muMinus;
	    p_Wbb         = fP_Wbb_muMinus;
	    p_Wcj         = fP_Wcj_muMinus;
	    p_ttbarSL     = fP_ttbarSL_muMinus;
	    p_ttbarDL     = fP_ttbarDL_muMinus;
	}
    }
    
    Double_t sig_4FS = p_tChannel4FS * llh_tChannel4FS;
    Double_t sig_5FS = p_tChannel5FS * llh_tChannel5FS;
    Double_t bkg = p_sChannel2j * llh_sChannel2j +
    	p_Wjj * llh_Wjj +
    	p_Wbb * llh_Wbb +
    	p_Wcj * llh_Wcj +
    	p_ttbarSL * llh_ttbarSL +
    	p_ttbarDL * llh_ttbarDL;

    Double_t sig_wjets = p_Wjj * llh_Wjj + p_Wbb * llh_Wbb + p_Wcj * llh_Wcj;
    Double_t sig_ttbar = p_ttbarSL * llh_ttbarSL + p_ttbarDL * llh_ttbarDL;

    
    Double_t tChannelRatio = 0.;
    Double_t WjetsRatio = 0.;
    Double_t ttbarRatio = 0.;
    Int_t bin = 0.;
    Double_t tChannelRatio_reBinned = 0.;

    // 4FS - stuff
    // ===========
    
    // t-Channel Ratio with t-channel 4FS as signal
    // --------------------------------------------
    tChannelRatio = sig_4FS / (sig_4FS + bkg);
    bin = 1 + TMath::BinarySearch(fgNBins_tChannel4FSRatio,
				  fgBinEdges_tChannel4FSRatio,
				  tChannelRatio);
    if ( bin == 0 )
	bin++; // Don't use underflow bin
    tChannelRatio_reBinned = (bin - 0.5)/fgNBins_tChannel4FSRatio;
    
    fHist_llh_tChannel_ratio_4FS->Fill(tChannelRatio_reBinned, GetTagEvtWeight());
    if ( fEvent->EventNr() % 2 == 0 )
	fHist_llh_tChannel_ratio_4FS_even->Fill(tChannelRatio_reBinned, GetTagEvtWeight());
    else
	fHist_llh_tChannel_ratio_4FS_odd->Fill(tChannelRatio_reBinned, GetTagEvtWeight());
    fHist_llh_tChannel_ratio_4FS_raw->Fill(tChannelRatio, GetTagEvtWeight());
    fHist_llh_tChannel_ratio2_4FS->Fill(tChannelRatio, GetTagEvtWeight());

    // Wjets Ratio with t-channel 4FS as signal
    WjetsRatio = sig_wjets / (sig_4FS + bkg);
    fHist_llh_Wjets_ratio_4FS->Fill(WjetsRatio, GetTagEvtWeight());

    fHist_llh_Wbb_ratio_4FS->Fill((p_Wbb*llh_Wbb)/(sig_4FS + bkg), GetTagEvtWeight());
    fHist_llh_Wcj_ratio_4FS->Fill((p_Wcj*llh_Wcj)/(sig_4FS + bkg), GetTagEvtWeight());
    fHist_llh_Wjj_ratio_4FS->Fill((p_Wjj*llh_Wjj)/(sig_4FS + bkg), GetTagEvtWeight());
				  
    
    // ttbar Ratio with t-channel 4FS as signal
    ttbarRatio = sig_ttbar / (sig_4FS + bkg);
    fHist_llh_ttbar_ratio_4FS->Fill(ttbarRatio, GetTagEvtWeight());

    // 2D
    fHist_2D_tChannel_4FS_vs_5FS->Fill(sig_4FS / (sig_4FS + bkg),
				       sig_5FS / (sig_5FS + bkg),
				       GetTagEvtWeight());
    fHist_2D_tChannel_4FS_vs_Wjets->Fill(sig_4FS/(sig_4FS + bkg),
					 WjetsRatio,			 
					 GetTagEvtWeight());
    fHist_2D_tChannel_4FS_vs_Wbb->Fill(sig_4FS/(sig_4FS + bkg),
				       (p_Wbb*llh_Wbb)/(sig_4FS + bkg),
				       GetTagEvtWeight());
    fHist_2D_tChannel_4FS_vs_Wcj->Fill(sig_4FS/(sig_4FS + bkg),
				       (p_Wcj*llh_Wcj)/(sig_4FS + bkg),
				       GetTagEvtWeight());
    fHist_2D_tChannel_4FS_vs_ttbar->Fill(sig_4FS/(sig_4FS + bkg),
					 ttbarRatio,
					 GetTagEvtWeight());
    
    // t-Channel Ratio with t-channel 5FS as signal
    // --------------------------------------------
    tChannelRatio = sig_5FS / (sig_5FS + bkg);
    bin = 1 + TMath::BinarySearch(fgNBins_tChannel5FSRatio,
				  fgBinEdges_tChannel5FSRatio,
				  tChannelRatio);
    if ( bin == 0 )
	bin++; // Don't use underflow bin
    tChannelRatio_reBinned = (bin - 0.5)/fgNBins_tChannel5FSRatio;
    
    fHist_llh_tChannel_ratio_5FS->Fill(tChannelRatio_reBinned, GetTagEvtWeight());
    if ( fEvent->EventNr() % 2 == 0 )
	fHist_llh_tChannel_ratio_5FS_even->Fill(tChannelRatio_reBinned, GetTagEvtWeight());
    else
	fHist_llh_tChannel_ratio_5FS_odd->Fill(tChannelRatio_reBinned, GetTagEvtWeight());	
    fHist_llh_tChannel_ratio_5FS_raw->Fill(tChannelRatio, GetTagEvtWeight());
    fHist_llh_tChannel_ratio2_5FS->Fill(tChannelRatio, GetTagEvtWeight());

    // Wjets Ratio with t-channel 5FS as signal
    WjetsRatio = sig_wjets / (sig_5FS + bkg);
    fHist_llh_Wjets_ratio_5FS->Fill(WjetsRatio, GetTagEvtWeight());

    // ttbar Ratio with t-channel 5FS as signal
    ttbarRatio = sig_ttbar / (sig_5FS + bkg);
    fHist_llh_ttbar_ratio_5FS->Fill(ttbarRatio, GetTagEvtWeight());


    // Likelihood plots
    // ----------------
    fHist_logllh_sChannel2j->Fill(TMath::Log(llh_sChannel2j), GetTagEvtWeight());
    fHist_logllh_tChannel4FS->Fill(TMath::Log(llh_tChannel4FS), GetTagEvtWeight());
    fHist_logllh_tChannel5FS->Fill(TMath::Log(llh_tChannel5FS), GetTagEvtWeight());
    fHist_logllh_ttbarSL->Fill(TMath::Log(llh_ttbarSL), GetTagEvtWeight());
    fHist_logllh_ttbarDL->Fill(TMath::Log(llh_ttbarDL), GetTagEvtWeight());
    fHist_logllh_Wjj->Fill(TMath::Log(llh_Wjj), GetTagEvtWeight());
    fHist_logllh_Wcj->Fill(TMath::Log(llh_Wcj), GetTagEvtWeight());
    fHist_logllh_Wbb->Fill(TMath::Log(llh_Wbb), GetTagEvtWeight());
}

//____________________________________________________________________

void AtlSgTop_tChannelMemDiscr::InitEvent() {
    //
    // Prepare event analysis
    //
}

//____________________________________________________________________

void AtlSgTop_tChannelMemDiscr::Print(Option_t *option) const {
    //
    // Print user analysis configuration
    //
    cout << endl
	 << "================================================================" << endl
	 << "  Atlas Single-Top t-Channel Mem Discriminant Analysis Selector " << endl
	 << "================================================================" << endl
	 << endl;
    AtlSelector::Print();
    cout << endl
	 << "================================================================" << endl
	 << endl;
}

//____________________________________________________________________

void AtlSgTop_tChannelMemDiscr::Terminate() {
    //
    // Terminate routine called at the end of event loop
    //    
	
    AtlSelector::Terminate();
}

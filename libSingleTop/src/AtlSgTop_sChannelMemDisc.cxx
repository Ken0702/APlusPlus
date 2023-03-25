//____________________________________________________________________
//
// Single-top s-Channel Mem Discriminant Analysis
// 
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2013 (C) Soeren Stamm
// 
#ifndef SINGLETOP_AtlSgTop_sChannelMemDisc
#include <AtlSgTop_sChannelMemDisc.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlSgTop_sChannelMemDisc);
#endif

const Double_t AtlSgTop_sChannelMemDisc::fgBinEdges_schannelRatio[fgNBins_sChannelRatio+1] = {
    0.00000, 0.00015, 0.00115, 0.00410,	0.01015,
    0.01820, 0.02780, 0.03725, 0.04715, 0.05785,
    0.06865, 0.07945, 0.09050, 0.10175, 0.11330,
    0.12540, 0.13835, 0.15235, 0.16835, 0.18740,
    0.21180, 0.24710, 0.31535, 1.00000};

// s-channel likelihoods with W+jets phase space generation, rebinned, zs=5, zb=20
// const Double_t AtlSgTop_sChannelMemDisc::fgBinEdges_schannelRatio[fgNBins_sChannelRatio+1] = {
// 	0.00000,
// 	0.0005,
// 	0.0011,
// 	0.00205,
// 	0.0038,
// 	0.00715,
// 	0.0126,
// 	0.0197,
// 	0.02865,
// 	0.03835,
// 	0.0482,
// 	0.05875,
// 	0.0693,
// 	0.08,
// 	0.09155,
// 	0.103,
// 	0.1149,
// 	0.12735,
// 	0.14025,
// 	0.1555,
// 	0.17215,
// 	0.19155,
// 	0.2157,
// 	0.24925,
// 	0.3131,
// 	1.00000};

const Double_t AtlSgTop_sChannelMemDisc::fgBinEdges_raw_eplus[AtlSgTop_sChannelMemDisc::fgNBins_raw_eplus+1]= {
    0.00000000, 0.00200000, 0.01100000, 0.02400000, 0.03750000,
    0.05100000, 0.06350000, 0.07550000, 0.08700000, 0.09800000,
    0.10900000, 0.12050000, 0.13250000, 0.14500000, 0.15900000,
    0.17600000, 0.19800000, 0.23050000, 0.29500000, 1.00000000};
const Double_t AtlSgTop_sChannelMemDisc::fgBinEdges_raw_eminus[AtlSgTop_sChannelMemDisc::fgNBins_raw_eminus+1]= {
    0.00000000, 0.00050000, 0.00550000, 0.01400000, 0.02400000,
    0.03350000, 0.04250000, 0.05150000, 0.06050000, 0.06850000,
    0.07650000, 0.08500000, 0.09400000, 0.10300000, 0.11300000,
    0.12450000, 0.14000000, 0.16200000, 0.20300000, 1.00000000};    
const Double_t AtlSgTop_sChannelMemDisc::fgBinEdges_raw_muplus[AtlSgTop_sChannelMemDisc::fgNBins_raw_muplus+1]= {
    0.00000000, 0.00350000, 0.01500000, 0.03050000, 0.04450000,
    0.06000000, 0.07550000, 0.09100000, 0.10550000, 0.11900000,
    0.13350000, 0.14750000, 0.16300000, 0.18000000, 0.19950000,
    0.22350000, 0.25500000, 0.30150000, 0.37950000, 1.00000000};
const Double_t AtlSgTop_sChannelMemDisc::fgBinEdges_raw_muminus[AtlSgTop_sChannelMemDisc::fgNBins_raw_muminus+1]= {
    0.00000000, 0.00100000, 0.00700000, 0.01650000, 0.02750000,
    0.03850000, 0.04900000, 0.05950000, 0.06950000, 0.07900000,
    0.08900000, 0.09900000, 0.11000000, 0.12200000, 0.13550000,
    0.15150000, 0.17300000, 0.20500000, 0.26500000, 1.00000000};
const Double_t AtlSgTop_sChannelMemDisc::fgBinEdges_raw1[AtlSgTop_sChannelMemDisc::fgNBins_raw1+1]= {
    0.00000000, 0.00050000, 0.00550000, 0.01600000, 0.02950000,
    0.04350000, 0.05750000, 0.07150000, 0.08500000, 0.09800000,
    0.11100000, 0.12450000, 0.13850000, 0.15400000, 0.17100000,
    0.19150000, 0.21750000, 0.25500000, 0.32650000, 1.00000000};
const Double_t AtlSgTop_sChannelMemDisc::fgBinEdges_raw2[AtlSgTop_sChannelMemDisc::fgNBins_raw2+1]= {
    0.00000000, 0.00100000, 0.00750000, 0.02000000, 0.03400000,
    0.04800000, 0.06150000, 0.07450000, 0.08700000, 0.09950000,
    0.11150000, 0.12400000, 0.13700000, 0.15050000, 0.16550000,
    0.18250000, 0.20250000, 0.22900000, 0.26900000, 0.34250000,
    1.00000000};
const Double_t AtlSgTop_sChannelMemDisc::fgBinEdges_raw3[AtlSgTop_sChannelMemDisc::fgNBins_raw3+1]= {
    0.00000000, 0.00050000, 0.00400000, 0.01150000, 0.02200000,
    0.03300000, 0.04450000, 0.05600000, 0.06700000, 0.07800000,
    0.08900000, 0.10000000, 0.11050000, 0.12150000, 0.13300000,
    0.14450000, 0.15750000, 0.17200000, 0.18850000, 0.20850000,
    0.23400000, 0.27400000, 0.34700000, 1.00000000};
const Double_t AtlSgTop_sChannelMemDisc::fgBinEdges_raw4[AtlSgTop_sChannelMemDisc::fgNBins_raw4+1]= {
    0.00000000, 0.00050000, 0.00250000, 0.00750000, 0.01500000,
    0.02450000, 0.03400000, 0.04400000, 0.05400000, 0.06400000,
    0.07400000, 0.08350000, 0.09300000, 0.10250000, 0.11200000,
    0.12200000, 0.13250000, 0.14300000, 0.15450000, 0.16700000,
    0.18100000, 0.19750000, 0.21750000, 0.24350000, 0.28400000,
    0.35800000, 1.00000000};    
const Double_t AtlSgTop_sChannelMemDisc::fgBinEdges_raw5[AtlSgTop_sChannelMemDisc::fgNBins_raw5+1]= {
    0.00000000, 0.00050000, 0.00200000, 0.00600000, 0.01250000,
    0.02100000, 0.03000000, 0.03900000, 0.04800000, 0.05750000,
    0.06700000, 0.07600000, 0.08500000, 0.09400000, 0.10300000,
    0.11200000, 0.12100000, 0.13050000, 0.14000000, 0.15050000,
    0.16150000, 0.17400000, 0.18800000, 0.20400000, 0.22350000,
    0.25050000, 0.29200000, 0.36650000, 1.00000000};

//____________________________________________________________________

AtlSgTop_sChannelMemDisc::AtlSgTop_sChannelMemDisc(const char* OutputFile) :
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

AtlSgTop_sChannelMemDisc::~AtlSgTop_sChannelMemDisc() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlSgTop_sChannelMemDisc::Clear(Option_t *option) {
    //
    //  Clears the dynamically allocated objects
    //
}

//____________________________________________________________________

void AtlSgTop_sChannelMemDisc::Begin(TTree *tree) {
    //
    // Begin analysis job
    //

    // Create event writer
    fEvtWriter = 0;

    AtlSelector::Begin(tree);
}

//____________________________________________________________________

void AtlSgTop_sChannelMemDisc::SetBranchStatus() {
    //
    // Switch on/off branches in order to gain speed
    //

    // Switch off all branches by default
    fTree->SetBranchStatus("*", kFALSE);
}

//____________________________________________________________________

void AtlSgTop_sChannelMemDisc::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    fPrintEvent = kFALSE;
    // this _is_ used to determine the bookkeeping histogram names
    fInputMode  = kD3PDSgTop_v2;
    // fInputMode  = kD3PDSgTop;
	
    fApplyDileptonVeto  = kTRUE;
    fPt_LeadingJet_min  = 0.0;
    fVetoBTagWeight_min = 1.0e10; // high value by default -> veto off
	
    fEvtReader  = new AtlEvtReaderMem(this);
    Info("SetCutDefaults", "Using AtlEvtReaderMem");
}

//____________________________________________________________________

void AtlSgTop_sChannelMemDisc::BookHistograms() {
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
    
    fPdfTool->AddObservable("MemDiscriminant/sChannelProb");
    fPdfTool->AddObservable("MemDiscriminant/sChannelProb_odd");
    fPdfTool->AddObservable("MemDiscriminant/sChannelProb_even");

    fPdfTool->AddObservable("MemDiscriminant/sChannelProb_bins2N");
    fPdfTool->AddObservable("MemDiscriminant/sChannelProb_bins2N_odd");
    fPdfTool->AddObservable("MemDiscriminant/sChannelProb_bins2N_even");

    fPdfTool->AddObservable("MemDiscriminant/sChannelProb_bins3N");
    fPdfTool->AddObservable("MemDiscriminant/sChannelProb_bins3N_odd");
    fPdfTool->AddObservable("MemDiscriminant/sChannelProb_bins3N_even");

    fPdfTool->AddObservable("MemDiscriminant/sChannelProb_bins4N");
    fPdfTool->AddObservable("MemDiscriminant/sChannelProb_bins4N_odd");
    fPdfTool->AddObservable("MemDiscriminant/sChannelProb_bins4N_even");

    
    // a-priori probabilities

    if ( fApplyDileptonVeto ) {

	// probabilities after di-lepton veto
	Double_t sChannel2jVS3jFrac = 0.65;
	
	Double_t sChannel_eMinus  = 0.007231;
	Double_t sChannel_ePlus   = 0.01279;
	Double_t sChannel_muMinus = 0.009033;
	Double_t sChannel_muPlus  = 0.01634;
	
	fP_sChannel2j_ePlus   = sChannel2jVS3jFrac * sChannel_ePlus;
	fP_sChannel2j_muPlus  = sChannel2jVS3jFrac * sChannel_muPlus;
	fP_sChannel2j_eMinus  = sChannel2jVS3jFrac * sChannel_eMinus;
	fP_sChannel2j_muMinus = sChannel2jVS3jFrac * sChannel_muMinus;
	
	fP_sChannel3j_ePlus   = (1.0 - sChannel2jVS3jFrac) * sChannel_ePlus;
	fP_sChannel3j_muPlus  = (1.0 - sChannel2jVS3jFrac) * sChannel_muPlus;
	fP_sChannel3j_eMinus  = (1.0 - sChannel2jVS3jFrac) * sChannel_eMinus;
	fP_sChannel3j_muMinus = (1.0 - sChannel2jVS3jFrac) * sChannel_muMinus;
	
	fP_tChannel4FS_ePlus   = 0.02537;
	fP_tChannel4FS_muPlus  = 0.0317;
	fP_tChannel4FS_eMinus  = 0.01628;
	fP_tChannel4FS_muMinus = 0.0192;
	
	fP_Wjj_ePlus   = 0.002393;
	fP_Wjj_muPlus  = 0.01169;
	fP_Wjj_eMinus  = 0.0005399;
	fP_Wjj_muMinus = 0.006152;
	
	Double_t wbbVSwcjFrac = 0.8;
	
	Double_t wHF_eMinus  = 0.03918;
	Double_t wHF_ePlus   = 0.05155;
	Double_t wHF_muMinus = 0.04962;
	Double_t wHF_muPlus  = 0.06589;
	
	fP_Wbb_ePlus   = wbbVSwcjFrac * wHF_ePlus;
	fP_Wbb_muPlus  = wbbVSwcjFrac * wHF_muPlus;
	fP_Wbb_eMinus  = wbbVSwcjFrac * wHF_eMinus;
	fP_Wbb_muMinus = wbbVSwcjFrac * wHF_muMinus;
	
	fP_Wcj_ePlus   = (1.0 - wbbVSwcjFrac) * wHF_ePlus;
	fP_Wcj_muPlus  = (1.0 - wbbVSwcjFrac) * wHF_muPlus;
	fP_Wcj_eMinus  = (1.0 - wbbVSwcjFrac) * wHF_eMinus;
	fP_Wcj_muMinus = (1.0 - wbbVSwcjFrac) * wHF_muMinus;

	// v15 fraction after veto, but without lead. jet pt > 40 GeV cut
	Double_t ttbarSL_vs_ttbarDL_frac_el = 0.3217;
	Double_t ttbarSL_vs_ttbarDL_frac_mu = 0.3254;
	
	Double_t ttbar_eMinus  = 0.1444;
	Double_t ttbar_ePlus   = ttbar_eMinus;
	Double_t ttbar_muMinus = 0.1731;
	Double_t ttbar_muPlus  = ttbar_muMinus;
	
	fP_ttbarSL_ePlus   = ttbarSL_vs_ttbarDL_frac_el * ttbar_ePlus;
	fP_ttbarSL_muPlus  = ttbarSL_vs_ttbarDL_frac_mu * ttbar_muPlus;
	fP_ttbarSL_eMinus  = ttbarSL_vs_ttbarDL_frac_el * ttbar_eMinus;
	fP_ttbarSL_muMinus = ttbarSL_vs_ttbarDL_frac_mu * ttbar_muMinus;
	
	fP_ttbarDL_ePlus   = (1.0 - ttbarSL_vs_ttbarDL_frac_el) * ttbar_ePlus;
	fP_ttbarDL_muPlus  = (1.0 - ttbarSL_vs_ttbarDL_frac_mu) * ttbar_muPlus;
	fP_ttbarDL_eMinus  = (1.0 - ttbarSL_vs_ttbarDL_frac_el) * ttbar_eMinus;
	fP_ttbarDL_muMinus = (1.0 - ttbarSL_vs_ttbarDL_frac_mu) * ttbar_muMinus;
	
    } else {
	// probabilities without di-lepton veto
	Double_t sChannel2jVS3jFrac = 0.65;
	
	fP_sChannel2j_ePlus   = sChannel2jVS3jFrac * 0.00987;
	fP_sChannel2j_muPlus  = sChannel2jVS3jFrac * 0.01259;
	fP_sChannel2j_eMinus  = sChannel2jVS3jFrac * 0.005601;
	fP_sChannel2j_muMinus = sChannel2jVS3jFrac * 0.006961;
	
	fP_sChannel3j_ePlus   = (1.0 - sChannel2jVS3jFrac) * 0.00987;
	fP_sChannel3j_muPlus  = (1.0 - sChannel2jVS3jFrac) * 0.01259;
	fP_sChannel3j_eMinus  = (1.0 - sChannel2jVS3jFrac) * 0.005601;
	fP_sChannel3j_muMinus = (1.0 - sChannel2jVS3jFrac) * 0.006961;
	
	fP_tChannel4FS_ePlus   = 0.0199503;
	fP_tChannel4FS_muPlus  = 0.0250097;
	fP_tChannel4FS_eMinus  = 0.0128461;
	fP_tChannel4FS_muMinus = 0.0151461;
	
	fP_Wjj_ePlus   = 0.00233655;
	fP_Wjj_muPlus  = 0.0129941;
	fP_Wjj_eMinus  = 0.000406719;
	fP_Wjj_muMinus = 0.00516182;
	
	Double_t wbbVSwcjFrac = 0.8;
	
	fP_Wbb_ePlus   = wbbVSwcjFrac * 0.0431757;
	fP_Wbb_muPlus  = wbbVSwcjFrac * 0.0558853;
	fP_Wbb_eMinus  = wbbVSwcjFrac * 0.0335755;
	fP_Wbb_muMinus = wbbVSwcjFrac * 0.0409918;
	
	fP_Wcj_ePlus   = (1.0 - wbbVSwcjFrac) * 0.0431757;
	fP_Wcj_muPlus  = (1.0 - wbbVSwcjFrac) * 0.0558853;
	fP_Wcj_eMinus  = (1.0 - wbbVSwcjFrac) * 0.0335755;
	fP_Wcj_muMinus = (1.0 - wbbVSwcjFrac) * 0.0409918;

	// v15 fraction before veto
	Double_t ttbarSL_vs_ttbarDL_frac_el = 0.2361;
	Double_t ttbarSL_vs_ttbarDL_frac_mu = 0.2398;
	
	fP_ttbarSL_ePlus   = ttbarSL_vs_ttbarDL_frac_el * 0.158366;
	fP_ttbarSL_muPlus  = ttbarSL_vs_ttbarDL_frac_mu * 0.190364;
	fP_ttbarSL_eMinus  = ttbarSL_vs_ttbarDL_frac_el * 0.159029;
	fP_ttbarSL_muMinus = ttbarSL_vs_ttbarDL_frac_mu * 0.189734;
	
	fP_ttbarDL_ePlus   = (1.0 - ttbarSL_vs_ttbarDL_frac_el) * 0.15837;
	fP_ttbarDL_muPlus  = (1.0 - ttbarSL_vs_ttbarDL_frac_mu) * 0.19036;
	fP_ttbarDL_eMinus  = (1.0 - ttbarSL_vs_ttbarDL_frac_el) * 0.15903;
	fP_ttbarDL_muMinus = (1.0 - ttbarSL_vs_ttbarDL_frac_mu) * 0.18973;
    }
    
    gDirectory->mkdir("MemDiscriminant");
    gDirectory->cd("MemDiscriminant");
	
    // Discriminant Plots
    fHist_llh_sChannel_ratio = new TH1F("sChannelProb", "s-channel likelihood ratio", fgNBins_sChannelRatio, 0.0, 1.0);
    fHist_llh_sChannel_ratio->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio->SetYTitle("Number of Entries");

    fHist_llh_sChannel_ratio_lplus = new TH1F("sChannelProb_lplus", "s-channel likelihood ratio, l+", fgNBins_sChannelRatio, 0.0, 1.0);
    fHist_llh_sChannel_ratio_lplus->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_lplus->SetYTitle("Number of Entries");

    fHist_llh_sChannel_ratio_lminus = new TH1F("sChannelProb_lminus", "s-channel likelihood ratio, l-", fgNBins_sChannelRatio, 0.0, 1.0);
    fHist_llh_sChannel_ratio_lminus->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_lminus->SetYTitle("Number of Entries");

    Int_t upperBin_bkg= (Int_t)((Double_t)fgNBins_sChannelRatio * 0.5) + 2;
    Double_t bkgUpperEdge = fHist_llh_sChannel_ratio->GetBinLowEdge(upperBin_bkg);
    Int_t    nBinsBkg     = ((Double_t)fgNBins_sChannelRatio)*bkgUpperEdge;
    fHist_llh_sChannel_ratio_bkg = new TH1F("sChannelProb_bkg", "s-channel likelihood ratio", nBinsBkg, 0.0, bkgUpperEdge);
    fHist_llh_sChannel_ratio_bkg->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_bkg->SetYTitle("Number of Entries");
	
    fHist_llh_sChannel_ratio_bin1Spread = new TH1F("sChannelProb_bin1Spread", "s-channel likelihood ratio", fgNBins_sChannelRatio, 0.0, 1.0);
    fHist_llh_sChannel_ratio_bin1Spread->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_bin1Spread->SetYTitle("Number of Entries");
	
    fHist_llh_sChannel_ratio_bins2N = new TH1F("sChannelProb_bins2N", "s-channel likelihood ratio", fgNBins_sChannelRatio - 1, 0.0, 1.0);
    fHist_llh_sChannel_ratio_bins2N->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_bins2N->SetYTitle("Number of Entries");
    
    fHist_llh_sChannel_ratio_bins2N_even = new TH1F("sChannelProb_bins2N_even", "s-channel likelihood ratio", fgNBins_sChannelRatio - 1, 0.0, 1.0);
    fHist_llh_sChannel_ratio_bins2N_even->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_bins2N_even->SetYTitle("Number of Entries");
    
    fHist_llh_sChannel_ratio_bins2N_odd = new TH1F("sChannelProb_bins2N_odd", "s-channel likelihood ratio", fgNBins_sChannelRatio - 1, 0.0, 1.0);
    fHist_llh_sChannel_ratio_bins2N_odd->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_bins2N_odd->SetYTitle("Number of Entries");
	
    fHist_llh_sChannel_ratio_bkg_bins2N = new TH1F("sChannelProb_bkg_bins2N", "s-channel likelihood ratio", nBinsBkg - 1, 0.0, bkgUpperEdge);
    fHist_llh_sChannel_ratio_bkg_bins2N->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_bkg_bins2N->SetYTitle("Number of Entries");
	
    fHist_llh_sChannel_ratio_bkg_bins2N_even = new TH1F("sChannelProb_bkg_bins2N_even", "s-channel likelihood ratio", nBinsBkg - 1, 0.0, bkgUpperEdge);
    fHist_llh_sChannel_ratio_bkg_bins2N_even->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_bkg_bins2N_even->SetYTitle("Number of Entries");
	
    fHist_llh_sChannel_ratio_bkg_bins2N_odd = new TH1F("sChannelProb_bkg_bins2N_odd", "s-channel likelihood ratio", nBinsBkg - 1, 0.0, bkgUpperEdge);
    fHist_llh_sChannel_ratio_bkg_bins2N_odd->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_bkg_bins2N_odd->SetYTitle("Number of Entries");
    
	
    fHist_llh_sChannel_ratio_bins3N = new TH1F("sChannelProb_bins3N", "s-channel likelihood ratio", fgNBins_sChannelRatio - 2, 0.0, 1.0);
    fHist_llh_sChannel_ratio_bins3N->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_bins3N->SetYTitle("Number of Entries");
    
    fHist_llh_sChannel_ratio_bins3N_even = new TH1F("sChannelProb_bins3N_even", "s-channel likelihood ratio", fgNBins_sChannelRatio - 2, 0.0, 1.0);
    fHist_llh_sChannel_ratio_bins3N_even->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_bins3N_even->SetYTitle("Number of Entries");
    
    fHist_llh_sChannel_ratio_bins3N_odd = new TH1F("sChannelProb_bins3N_odd", "s-channel likelihood ratio", fgNBins_sChannelRatio - 2, 0.0, 1.0);
    fHist_llh_sChannel_ratio_bins3N_odd->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_bins3N_odd->SetYTitle("Number of Entries");
	
    fHist_llh_sChannel_ratio_bkg_bins3N = new TH1F("sChannelProb_bkg_bins3N", "s-channel likelihood ratio", nBinsBkg - 2, 0.0, bkgUpperEdge);
    fHist_llh_sChannel_ratio_bkg_bins3N->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_bkg_bins3N->SetYTitle("Number of Entries");
	
    fHist_llh_sChannel_ratio_bkg_bins3N_even = new TH1F("sChannelProb_bkg_bins3N_even", "s-channel likelihood ratio", nBinsBkg - 2, 0.0, bkgUpperEdge);
    fHist_llh_sChannel_ratio_bkg_bins3N_even->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_bkg_bins3N_even->SetYTitle("Number of Entries");
	
    fHist_llh_sChannel_ratio_bkg_bins3N_odd = new TH1F("sChannelProb_bkg_bins3N_odd", "s-channel likelihood ratio", nBinsBkg - 2, 0.0, bkgUpperEdge);
    fHist_llh_sChannel_ratio_bkg_bins3N_odd->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_bkg_bins3N_odd->SetYTitle("Number of Entries");
	
    
	
    fHist_llh_sChannel_ratio_bins4N = new TH1F("sChannelProb_bins4N", "s-channel likelihood ratio", fgNBins_sChannelRatio - 3, 0.0, 1.0);
    fHist_llh_sChannel_ratio_bins4N->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_bins4N->SetYTitle("Number of Entries");
    
    fHist_llh_sChannel_ratio_bins4N_even = new TH1F("sChannelProb_bins4N_even", "s-channel likelihood ratio", fgNBins_sChannelRatio - 3, 0.0, 1.0);
    fHist_llh_sChannel_ratio_bins4N_even->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_bins4N_even->SetYTitle("Number of Entries");
    
    fHist_llh_sChannel_ratio_bins4N_odd = new TH1F("sChannelProb_bins4N_odd", "s-channel likelihood ratio", fgNBins_sChannelRatio - 3, 0.0, 1.0);
    fHist_llh_sChannel_ratio_bins4N_odd->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_bins4N_odd->SetYTitle("Number of Entries");
	
    fHist_llh_sChannel_ratio_bkg_bins4N = new TH1F("sChannelProb_bkg_bins4N", "s-channel likelihood ratio", nBinsBkg - 3, 0.0, bkgUpperEdge);
    fHist_llh_sChannel_ratio_bkg_bins4N->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_bkg_bins4N->SetYTitle("Number of Entries");
	
    fHist_llh_sChannel_ratio_bkg_bins4N_even = new TH1F("sChannelProb_bkg_bins4N_even", "s-channel likelihood ratio", nBinsBkg - 3, 0.0, bkgUpperEdge);
    fHist_llh_sChannel_ratio_bkg_bins4N_even->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_bkg_bins4N_even->SetYTitle("Number of Entries");
	
    fHist_llh_sChannel_ratio_bkg_bins4N_odd = new TH1F("sChannelProb_bkg_bins4N_odd", "s-channel likelihood ratio", nBinsBkg - 3, 0.0, bkgUpperEdge);
    fHist_llh_sChannel_ratio_bkg_bins4N_odd->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_bkg_bins4N_odd->SetYTitle("Number of Entries");
	
    fHist_llh_sChannel_ratio_even = new TH1F("sChannelProb_even", "s-channel likelihood ratio, even evt numbers", fgNBins_sChannelRatio, 0.0, 1.0);
    fHist_llh_sChannel_ratio_even->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_even->SetYTitle("Number of Entries");
    
    fHist_llh_sChannel_ratio_odd = new TH1F("sChannelProb_odd", "s-channel likelihood ratio, odd evt numbers", fgNBins_sChannelRatio, 0.0, 1.0);
    fHist_llh_sChannel_ratio_odd->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_odd->SetYTitle("Number of Entries");
    
    // s-channel ratio per lepton flavour and per lepton charge
    // -- rebinned --

    // Binning for raw (29+1)
    fHist_llh_sChannel_ratio_raw_eplus_rebin = new TH1F("sChannelProb_raw_eplus_rebin", "s-channel likelihood ratio",
							fgNBins_raw_eplus, 0., 1.);
    fHist_llh_sChannel_ratio_raw_eplus_rebin->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_raw_eplus_rebin->SetYTitle("Number of Entries");
    
    // Binning for raw (29+1)
    fHist_llh_sChannel_ratio_raw_eminus_rebin = new TH1F("sChannelProb_raw_eminus_rebin", "s-channel likelihood ratio",
							 fgNBins_raw_eminus, 0., 1.);
    fHist_llh_sChannel_ratio_raw_eminus_rebin->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_raw_eminus_rebin->SetYTitle("Number of Entries");
    
    // Binning for raw (29+1)
    fHist_llh_sChannel_ratio_raw_muplus_rebin = new TH1F("sChannelProb_raw_muplus_rebin", "s-channel likelihood ratio",
							 fgNBins_raw_muplus, 0., 1.);
    fHist_llh_sChannel_ratio_raw_muplus_rebin->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_raw_muplus_rebin->SetYTitle("Number of Entries");
    
    // Binning for raw (29+1)
    fHist_llh_sChannel_ratio_raw_muminus_rebin = new TH1F("sChannelProb_raw_muminus_rebin", "s-channel likelihood ratio",
							  fgNBins_raw_muminus, 0., 1.);
    fHist_llh_sChannel_ratio_raw_muminus_rebin->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_raw_muminus_rebin->SetYTitle("Number of Entries");
	
    fHist_llh_wjets_ratio = new TH1F("wjetsProb", "W+jets likelihood ratio", fgNBins_sChannelRatio, 0.0, 1.0);
    fHist_llh_wjets_ratio->SetXTitle("P(W+jets|X)");
    fHist_llh_wjets_ratio->SetYTitle("Number of Entries");
	
    fHist_llh_wjets_ratio_binNSpread = new TH1F("wjetsProb_binNSpread", "W+jets likelihood ratio", fgNBins_sChannelRatio, 0.0, 1.0);
    fHist_llh_wjets_ratio_binNSpread->SetXTitle("P(W+jets|X)");
    fHist_llh_wjets_ratio_binNSpread->SetYTitle("Number of Entries");
    
    fHist_llh_wjets_ratio_even = new TH1F("wjetsProb_even", "W+jets likelihood ratio, even evt numbers", fgNBins_sChannelRatio, 0.0, 1.0);
    fHist_llh_wjets_ratio_even->SetXTitle("P(W+jets|X)");
    fHist_llh_wjets_ratio_even->SetYTitle("Number of Entries");
    
    fHist_llh_wjets_ratio_odd = new TH1F("wjetsProb_odd", "W+jets likelihood ratio, odd evt numbers", fgNBins_sChannelRatio, 0.0, 1.0);
    fHist_llh_wjets_ratio_odd->SetXTitle("P(W+jets|X)");
    fHist_llh_wjets_ratio_odd->SetYTitle("Number of Entries");
	
    fHist_llh_ttbar_ratio = new TH1F("ttbarProb", "ttbar likelihood ratio", fgNBins_sChannelRatio, 0.0, 1.0);
    fHist_llh_ttbar_ratio->SetXTitle("P(ttbar|X)");
    fHist_llh_ttbar_ratio->SetYTitle("Number of Entries");
    
    fHist_llh_ttbar_ratio_binNSpread = new TH1F("ttbarProb_binNSpread", "ttbar likelihood ratio", fgNBins_sChannelRatio, 0.0, 1.0);
    fHist_llh_ttbar_ratio_binNSpread->SetXTitle("P(ttbar|X)");
    fHist_llh_ttbar_ratio_binNSpread->SetYTitle("Number of Entries");

    gDirectory->mkdir("shapes");
    gDirectory->cd("shapes");
    
    // Binning for raw (29+1)
    fHist_llh_sChannel_ratio_raw_eplus_rebin_odd = new TH1F("sChannelProb_raw_eplus_rebin_odd", "s-channel likelihood ratio",
							    fgNBins_raw_eplus, 0., 1.);
    fHist_llh_sChannel_ratio_raw_eplus_rebin_odd->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_raw_eplus_rebin_odd->SetYTitle("Number of Entries");
    
    // Binning for raw (29+1)
    fHist_llh_sChannel_ratio_raw_eminus_rebin_odd = new TH1F("sChannelProb_raw_eminus_rebin_odd", "s-channel likelihood ratio",
							     fgNBins_raw_eminus, 0., 1.);
    fHist_llh_sChannel_ratio_raw_eminus_rebin_odd->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_raw_eminus_rebin_odd->SetYTitle("Number of Entries");
    
    // Binning for raw (29+1)
    fHist_llh_sChannel_ratio_raw_muplus_rebin_odd = new TH1F("sChannelProb_raw_muplus_rebin_odd", "s-channel likelihood ratio",
							     fgNBins_raw_muplus, 0., 1.);
    fHist_llh_sChannel_ratio_raw_muplus_rebin_odd->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_raw_muplus_rebin_odd->SetYTitle("Number of Entries");
    
    // Binning for raw (29+1)
    fHist_llh_sChannel_ratio_raw_muminus_rebin_odd = new TH1F("sChannelProb_raw_muminus_rebin_odd", "s-channel likelihood ratio",
							      fgNBins_raw_muminus, 0., 1.);
    fHist_llh_sChannel_ratio_raw_muminus_rebin_odd->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_raw_muminus_rebin_odd->SetYTitle("Number of Entries");

    // Binning for raw (29+1)
    fHist_llh_sChannel_ratio_raw_eplus_rebin_even = new TH1F("sChannelProb_raw_eplus_rebin_even", "s-channel likelihood ratio",
							     fgNBins_raw_eplus, 0., 1.);
    fHist_llh_sChannel_ratio_raw_eplus_rebin_even->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_raw_eplus_rebin_even->SetYTitle("Number of Entries");
    
    // Binning for raw (29+1)
    fHist_llh_sChannel_ratio_raw_eminus_rebin_even = new TH1F("sChannelProb_raw_eminus_rebin_even", "s-channel likelihood ratio",
							      fgNBins_raw_eminus, 0., 1.);
    fHist_llh_sChannel_ratio_raw_eminus_rebin_even->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_raw_eminus_rebin_even->SetYTitle("Number of Entries");
    
    // Binning for raw (29+1)
    fHist_llh_sChannel_ratio_raw_muplus_rebin_even = new TH1F("sChannelProb_raw_muplus_rebin_even", "s-channel likelihood ratio",
							      fgNBins_raw_muplus, 0., 1.);
    fHist_llh_sChannel_ratio_raw_muplus_rebin_even->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_raw_muplus_rebin_even->SetYTitle("Number of Entries");
    
    // Binning for raw (29+1)
    fHist_llh_sChannel_ratio_raw_muminus_rebin_even = new TH1F("sChannelProb_raw_muminus_rebin_even", "s-channel likelihood ratio",
							       fgNBins_raw_muminus, 0., 1.);
    fHist_llh_sChannel_ratio_raw_muminus_rebin_even->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_raw_muminus_rebin_even->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    gDirectory->mkdir("ManyBins");
    gDirectory->cd("ManyBins");

    // s-channel ratio all leptons, all charge
    fHist_llh_sChannel_ratio_raw = new TH1F("sChannelProb_raw", "s-channel likelihood ratio", 20000, 0.0, 1.0);
    fHist_llh_sChannel_ratio_raw->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_raw->SetYTitle("Number of Entries");
	
    // s-channel ratio per lepton flavour and per lepton charge
    fHist_llh_sChannel_ratio_raw_eplus = new TH1F("sChannelProb_raw_eplus", "s-channel likelihood ratio",
						  2000, 0., 1.);
    fHist_llh_sChannel_ratio_raw_eplus->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_raw_eplus->SetYTitle("Number of Entries");
    
    fHist_llh_sChannel_ratio_raw_eminus = new TH1F("sChannelProb_raw_eminus", "s-channel likelihood ratio",
						   2000, 0.0, 1.0);
    fHist_llh_sChannel_ratio_raw_eminus->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_raw_eminus->SetYTitle("Number of Entries");

    fHist_llh_sChannel_ratio_raw_muplus = new TH1F("sChannelProb_raw_muplus", "s-channel likelihood ratio",
						   2000, 0.0, 1.0);
    fHist_llh_sChannel_ratio_raw_muplus->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_raw_muplus->SetYTitle("Number of Entries");
    
    fHist_llh_sChannel_ratio_raw_muminus = new TH1F("sChannelProb_raw_muminus", "s-channel likelihood ratio",
						    2000, 0.0, 1.0);
    fHist_llh_sChannel_ratio_raw_muminus->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio_raw_muminus->SetYTitle("Number of Entries");

    // s-channel ratio all leptons, all charge
    fHist_llh_wjets_ratio_raw = new TH1F("wjetsProb_raw", "W+jets likelihood ratio", 20000, 0.0, 1.0);
    fHist_llh_wjets_ratio_raw->SetXTitle("P(W+jets|X)");
    fHist_llh_wjets_ratio_raw->SetYTitle("Number of Entries");

    gDirectory->cd("..");

    gDirectory->mkdir("BinningStudy");
    gDirectory->cd("BinningStudy");
    
    // Binning 1
    fHist_llh_sChannel_ratio1 = new TH1F("sChannelProb1", "s-channel likelihood ratio", fgNBins_raw1, 0.0, 1.0);
    fHist_llh_sChannel_ratio1->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio1->SetYTitle("Number of Entries");
    
    fHist_llh_sChannel_ratio1_even = new TH1F("sChannelProb1_even", "s-channel likelihood ratio, even evt numbers", fgNBins_raw1, 0.0, 1.0);
    fHist_llh_sChannel_ratio1_even->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio1_even->SetYTitle("Number of Entries");
    
    fHist_llh_sChannel_ratio1_odd = new TH1F("sChannelProb1_odd", "s-channel likelihood ratio, odd evt numbers", fgNBins_raw1, 0.0, 1.0);
    fHist_llh_sChannel_ratio1_odd->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio1_odd->SetYTitle("Number of Entries");

    // Binning 2
    fHist_llh_sChannel_ratio2 = new TH1F("sChannelProb2", "s-channel likelihood ratio", fgNBins_raw2, 0.0, 1.0);
    fHist_llh_sChannel_ratio2->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio2->SetYTitle("Number of Entries");
    
    fHist_llh_sChannel_ratio2_even = new TH1F("sChannelProb2_even", "s-channel likelihood ratio, even evt numbers", fgNBins_raw2, 0.0, 1.0);
    fHist_llh_sChannel_ratio2_even->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio2_even->SetYTitle("Number of Entries");
    
    fHist_llh_sChannel_ratio2_odd = new TH1F("sChannelProb2_odd", "s-channel likelihood ratio, odd evt numbers", fgNBins_raw2, 0.0, 1.0);
    fHist_llh_sChannel_ratio2_odd->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio2_odd->SetYTitle("Number of Entries");

    // Binning 3
    fHist_llh_sChannel_ratio3 = new TH1F("sChannelProb3", "s-channel likelihood ratio", fgNBins_raw3, 0.0, 1.0);
    fHist_llh_sChannel_ratio3->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio3->SetYTitle("Number of Entries");
    
    fHist_llh_sChannel_ratio3_even = new TH1F("sChannelProb3_even", "s-channel likelihood ratio, even evt numbers", fgNBins_raw3, 0.0, 1.0);
    fHist_llh_sChannel_ratio3_even->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio3_even->SetYTitle("Number of Entries");
    
    fHist_llh_sChannel_ratio3_odd = new TH1F("sChannelProb3_odd", "s-channel likelihood ratio, odd evt numbers", fgNBins_raw3, 0.0, 1.0);
    fHist_llh_sChannel_ratio3_odd->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio3_odd->SetYTitle("Number of Entries");

    // Binning 4
    fHist_llh_sChannel_ratio4 = new TH1F("sChannelProb4", "s-channel likelihood ratio", fgNBins_raw4, 0.0, 1.0);
    fHist_llh_sChannel_ratio4->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio4->SetYTitle("Number of Entries");
    
    fHist_llh_sChannel_ratio4_even = new TH1F("sChannelProb4_even", "s-channel likelihood ratio, even evt numbers", fgNBins_raw4, 0.0, 1.0);
    fHist_llh_sChannel_ratio4_even->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio4_even->SetYTitle("Number of Entries");
    
    fHist_llh_sChannel_ratio4_odd = new TH1F("sChannelProb4_odd", "s-channel likelihood ratio, odd evt numbers", fgNBins_raw4, 0.0, 1.0);
    fHist_llh_sChannel_ratio4_odd->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio4_odd->SetYTitle("Number of Entries");

    // Binning 5
    fHist_llh_sChannel_ratio5 = new TH1F("sChannelProb5", "s-channel likelihood ratio", fgNBins_raw5, 0.0, 1.0);
    fHist_llh_sChannel_ratio5->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio5->SetYTitle("Number of Entries");
    
    fHist_llh_sChannel_ratio5_even = new TH1F("sChannelProb5_even", "s-channel likelihood ratio, even evt numbers", fgNBins_raw5, 0.0, 1.0);
    fHist_llh_sChannel_ratio5_even->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio5_even->SetYTitle("Number of Entries");
    
    fHist_llh_sChannel_ratio5_odd = new TH1F("sChannelProb5_odd", "s-channel likelihood ratio, odd evt numbers", fgNBins_raw5, 0.0, 1.0);
    fHist_llh_sChannel_ratio5_odd->SetXTitle("P(S|X)");
    fHist_llh_sChannel_ratio5_odd->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    gDirectory->cd("..");

    gDirectory->mkdir("2D");
    gDirectory->cd("2D");

    fHist_2D_sChannel_vs_Wjets = new TH2F("sChannel_vs_Wjets", "sChannel vs Wjets discriminant",
					  100, 0.0, 1.0,
					  100, 0.0, 1.0);
    fHist_2D_sChannel_vs_Wjets->SetXTitle("P(S|X) s-channel");
    fHist_2D_sChannel_vs_Wjets->SetYTitle("P(S|X) Wjets");
    fHist_2D_sChannel_vs_Wjets->SetZTitle("Events");

    fHist_2D_sChannel_vs_ttbar = new TH2F("sChannel_vs_ttbar", "sChannel vs ttbar discriminant",
					  100, 0.0, 1.0,
					  100, 0.0, 1.0);
    fHist_2D_sChannel_vs_ttbar->SetXTitle("P(S|X) s-channel");
    fHist_2D_sChannel_vs_ttbar->SetYTitle("P(S|X) ttbar");
    fHist_2D_sChannel_vs_ttbar->SetZTitle("Events");

    // fHist_2D_sChannel_vs_SumLLH = new TH2F("sChannel_vs_sumllh", "sChannel discriminant vs sumllh",
    // 					   100, 0.0, 1.0,
    // 					   50, -19., 1.);
    // fHist_2D_sChannel_vs_SumLLH->SetXTitle("P(S|X) s-channel");
    // fHist_2D_sChannel_vs_SumLLH->SetYTitle("Log_{10}(#Sigma #alpha(H)P(X|H))");
    // fHist_2D_sChannel_vs_SumLLH->SetZTitle("Events");

    gDirectory->cd("..");

    gDirectory->mkdir("MemLikelihoods");
    gDirectory->cd("MemLikelihoods");

    // Log-Likelihood Plots
    fHist_logllh_sChannel2j = new TH1F("logllh_sChannel2j", "s-channel likelihood 2j", 50, -19., 1.);
    fHist_logllh_sChannel2j->SetXTitle("Log_{10}(L_{s-channel 2j})");
    fHist_logllh_sChannel2j->SetYTitle("Number of Entries");

    fHist_logllh_sChannel3j = new TH1F("logllh_sChannel3j", "s-channel likelihood 3j", 50, -19., 1.);
    fHist_logllh_sChannel3j->SetXTitle("Log_{10}(L_{s-channel 3j})");
    fHist_logllh_sChannel3j->SetYTitle("Number of Entries");
	
    fHist_logllh_tChannel4FS = new TH1F("logllh_tChannel4FS", "t-channel likelihood", 50, -19., 1.);
    fHist_logllh_tChannel4FS->SetXTitle("Log_{10}(L_{t-channel})");
    fHist_logllh_tChannel4FS->SetYTitle("Number of Entries");

    fHist_logllh_ttbarSL = new TH1F("logllh_ttbarSL", "ttbar SL", 50, -19., 1.);
    fHist_logllh_ttbarSL->SetXTitle("Log_{10}(L_{t#bar{t}, SL})");
    fHist_logllh_ttbarSL->SetYTitle("Number of Entries");

    fHist_logllh_ttbarSL_ttbarCR = new TH1F("logllh_ttbarSL_ttbarCR", "ttbar SL", 50., -70., -45.);
    fHist_logllh_ttbarSL_ttbarCR->SetXTitle("Log_{10}(L_{t#bar{t}, SL})");
    fHist_logllh_ttbarSL_ttbarCR->SetYTitle("Number of Entries");

    fHist_logllh_ttbarDL = new TH1F("logllh_ttbarDL", "ttbar DL likelihood", 50., -15, 0.);
    fHist_logllh_ttbarDL->SetXTitle("Log_{10}(L_{t#bar{t}, DL})");
    fHist_logllh_ttbarDL->SetYTitle("Number of Entries");

    fHist_logllh_ttbarDL_wjetsCR = new TH1F("logllh_ttbarDL_wjetsCR", "ttbar DL likelihood", 50., -35., -20.);
    fHist_logllh_ttbarDL_wjetsCR->SetXTitle("Log_{10}(L_{t#bar{t}, DL})");
    fHist_logllh_ttbarDL_wjetsCR->SetYTitle("Number of Entries");

    fHist_logllh_ttbarDL_ttbarCR = new TH1F("logllh_ttbarDL_ttbarCR", "ttbar DL likelihood", 50., -70., -45.);
    fHist_logllh_ttbarDL_ttbarCR->SetXTitle("Log_{10}(L_{t#bar{t}, DL})");
    fHist_logllh_ttbarDL_ttbarCR->SetYTitle("Number of Entries");

    fHist_logllh_Wjj = new TH1F("logllh_Wjj", "Wjj likelihood", 50, -19., 1.);
    fHist_logllh_Wjj->SetXTitle("Log_{10}(L_{Wjj})");
    fHist_logllh_Wjj->SetYTitle("Number of Entries");

    fHist_logllh_Wcj = new TH1F("logllh_Wcj", "Wcj likelihood", 50, -19., 1.);
    fHist_logllh_Wcj->SetXTitle("Log_{10}(L_{Wcj})");
    fHist_logllh_Wcj->SetYTitle("Number of Entries");

    fHist_logllh_Wcj_wjetsCR = new TH1F("logllh_Wcj_wjetsCR", "Wcj likelihood", 50.,
					-35., -10.);
    fHist_logllh_Wcj_wjetsCR->SetXTitle("Log_{10}(L_{Wcj})");
    fHist_logllh_Wcj_wjetsCR->SetYTitle("Number of Entries");

    fHist_logllh_Wbb = new TH1F("logllh_Wbb", "Wbb likelihood", 50, -19., 1.);
    fHist_logllh_Wbb->SetXTitle("Log_{10}(L_{Wb#bar{b}})");
    fHist_logllh_Wbb->SetYTitle("Number of Entries");

    gDirectory->mkdir("PositiveCharge");
    gDirectory->cd("PositiveCharge");
    
    // Log-Likelihood Plots
    fHist_logllh_sChannel2j_plus = new TH1F("logllh_sChannel2j_plus", "s-channel likelihood 2j", 50, -19., 1.);
    fHist_logllh_sChannel2j_plus->SetXTitle("Log_{10}(L_{s-channel 2j})");
    fHist_logllh_sChannel2j_plus->SetYTitle("Number of Entries");

    fHist_logllh_sChannel3j_plus = new TH1F("logllh_sChannel3j_plus", "s-channel likelihood 3j", 50, -19., 1.);
    fHist_logllh_sChannel3j_plus->SetXTitle("Log_{10}(L_{s-channel 3j})");
    fHist_logllh_sChannel3j_plus->SetYTitle("Number of Entries");

    fHist_logllh_tChannel4FS_plus = new TH1F("logllh_tChannel4FS", "t-channel likelihood", 50, -19., 1.);
    fHist_logllh_tChannel4FS_plus->SetXTitle("Log_{10}(L_{t-channel})");
    fHist_logllh_tChannel4FS_plus->SetYTitle("Number of Entries");

    fHist_logllh_ttbarSL_plus = new TH1F("logllh_ttbarSL_plus", "ttbar SL likelihood", 50, -19., 1.);
    fHist_logllh_ttbarSL_plus->SetXTitle("Log_{10}(L_{t#bar{t}, SL})");
    fHist_logllh_ttbarSL_plus->SetYTitle("Number of Entries");

    fHist_logllh_ttbarDL_plus = new TH1F("logllh_ttbarDL_plus", "ttbar DL likelihood", 50., -15., 0.);
    fHist_logllh_ttbarDL_plus->SetXTitle("Log_{10}(L_{t#bar{t}, DL})");
    fHist_logllh_ttbarDL_plus->SetYTitle("Number of Entries");

    fHist_logllh_Wjj_plus = new TH1F("logllh_Wjj_plus", "Wjj likelihood", 50, -19., 1.);
    fHist_logllh_Wjj_plus->SetXTitle("Log_{10}(L_{Wjj})");
    fHist_logllh_Wjj_plus->SetYTitle("Number of Entries");

    fHist_logllh_Wcj_plus = new TH1F("logllh_Wcj_plus", "Wcj likelihood", 50, -19., 1.);
    fHist_logllh_Wcj_plus->SetXTitle("Log_{10}(L_{Wcj})");
    fHist_logllh_Wcj_plus->SetYTitle("Number of Entries");

    fHist_logllh_Wbb_plus = new TH1F("logllh_Wbb_plus", "Wbb likelihood", 50, -19., 1.);
    fHist_logllh_Wbb_plus->SetXTitle("Log_{10}(L_{Wbb})");
    fHist_logllh_Wbb_plus->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");

    gDirectory->mkdir("NegativeCharge");
    gDirectory->cd("NegativeCharge");
    
    // Log-Likelihood Plots
    fHist_logllh_sChannel2j_minus = new TH1F("logllh_sChannel2j_minus", "s-channel likelihood 2j", 50, -19., 1.);
    fHist_logllh_sChannel2j_minus->SetXTitle("Log_{10}(L_{s-channel 2j})");
    fHist_logllh_sChannel2j_minus->SetYTitle("Number of Entries");

    fHist_logllh_sChannel3j_minus = new TH1F("logllh_sChannel3j_minus", "s-channel likelihood 3j", 50, -19., 1.);
    fHist_logllh_sChannel3j_minus->SetXTitle("Log_{10}(L_{s-channel 3j})");
    fHist_logllh_sChannel3j_minus->SetYTitle("Number of Entries");

    fHist_logllh_tChannel4FS_minus = new TH1F("logllh_tChannel4FS", "t-channel likelihood", 50, -19., 1.);
    fHist_logllh_tChannel4FS_minus->SetXTitle("Log_{10}(L_{t-channel})");
    fHist_logllh_tChannel4FS_minus->SetYTitle("Number of Entries");

    fHist_logllh_ttbarSL_minus = new TH1F("logllh_ttbarSL_minus", "ttbar SL likelihood", 50, -19., 1.);
    fHist_logllh_ttbarSL_minus->SetXTitle("Log_{10}(L_{t#bar{t}, SL})");
    fHist_logllh_ttbarSL_minus->SetYTitle("Number of Entries");

    fHist_logllh_ttbarDL_minus = new TH1F("logllh_ttbarDL_minus", "ttbar DL likelihood", 50., -15., 0.);
    fHist_logllh_ttbarDL_minus->SetXTitle("Log_{10}(L_{t#bar{t}, DL})");
    fHist_logllh_ttbarDL_minus->SetYTitle("Number of Entries");

    fHist_logllh_Wjj_minus = new TH1F("logllh_Wjj_minus", "Wjj likelihood", 50, -19., 1.);
    fHist_logllh_Wjj_minus->SetXTitle("Log_{10}(L_{Wjj})");
    fHist_logllh_Wjj_minus->SetYTitle("Number of Entries");

    fHist_logllh_Wcj_minus = new TH1F("logllh_Wcj_minus", "Wcj likelihood", 50, -19., 1.);
    fHist_logllh_Wcj_minus->SetXTitle("Log_{10}(L_{Wcj})");
    fHist_logllh_Wcj_minus->SetYTitle("Number of Entries");

    fHist_logllh_Wbb_minus = new TH1F("logllh_Wbb_minus", "Wbb likelihood", 50, -19., 1.);
    fHist_logllh_Wbb_minus->SetXTitle("Log_{10}(L_{Wbb})");
    fHist_logllh_Wbb_minus->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    gDirectory->cd("..");
}

//____________________________________________________________________

Bool_t AtlSgTop_sChannelMemDisc::ProcessPreCut() {
    //
    // Event pre-selection
    //

    // Currently, no cuts applied
    
    // Accept event
    return kTRUE;
}

//____________________________________________________________________

Bool_t AtlSgTop_sChannelMemDisc::ProcessCut() {
    //
    // Event selection
    //
	
    // Cut on leading jet pt
    AtlJet *Jetlead = (AtlJet*) fJets->At(0);
    if ( Jetlead->Pt() < fPt_LeadingJet_min ) return kFALSE;

    // Ttbar di-lepton veto:
    if ( fApplyDileptonVeto ) {
	Int_t Ntight = fLeptons->GetEntries();
	Int_t Nloose = fLooseLeptons->GetEntries();
	if ( Ntight + Nloose > 1 )
	    return kFALSE;
    }
	
    // B-tag veto (for ctrl region)
    TIter next_bjet(fBJets);
    AtlJet *bjet = 0;
    Bool_t IsVeto = kTRUE;
    while ( (bjet = (AtlJet*)next_bjet()) ) {
    	IsVeto &= ( bjet->GetTag(AtlBTag::kMV1)->GetWeight()
    		    > fVetoBTagWeight_min );
    }
    if ( IsVeto )  return kFALSE;

	
    return kTRUE;
}

//____________________________________________________________________

void AtlSgTop_sChannelMemDisc::FillHistograms() {
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
    Double_t llh_sChannel3j  = GetEvtReader()->v_llh_sChannel3j;
    Double_t llh_tChannel4FS = GetEvtReader()->v_llh_tChannel4FS;
    Double_t llh_ttbarSL     = GetEvtReader()->v_llh_ttbarSL;
    Double_t llh_ttbarDL     = GetEvtReader()->v_llh_ttbarDL;
    Double_t llh_Wjj         = GetEvtReader()->v_llh_Wjj;
    Double_t llh_Wcj         = GetEvtReader()->v_llh_Wcj;
    Double_t llh_Wbb         = GetEvtReader()->v_llh_Wbb;

    Double_t p_sChannel2j  = 0.0;
    Double_t p_sChannel3j  = 0.0;
    Double_t p_tChannel4FS = 0.0;
    Double_t p_Wjj         = 0.0;
    Double_t p_Wbb         = 0.0;
    Double_t p_Wcj         = 0.0;
    Double_t p_ttbarSL     = 0.0;
    Double_t p_ttbarDL     = 0.0;
    
    if ( lepIsElectron ) {
	if ( lepCharge > 0.0 ) {
	    // e+
	    p_sChannel2j  = fP_sChannel2j_ePlus;
	    p_sChannel3j  = fP_sChannel3j_ePlus;
	    p_tChannel4FS = fP_tChannel4FS_ePlus;
	    p_Wjj         = fP_Wjj_ePlus;
	    p_Wbb         = fP_Wbb_ePlus;
	    p_Wcj         = fP_Wcj_ePlus;
	    p_ttbarSL     = fP_ttbarSL_ePlus;
	    p_ttbarDL     = fP_ttbarDL_ePlus;
	} else {
	    // e-
	    p_sChannel2j  = fP_sChannel2j_eMinus;
	    p_sChannel3j  = fP_sChannel3j_eMinus;
	    p_tChannel4FS = fP_tChannel4FS_eMinus;
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
	    p_sChannel3j  = fP_sChannel3j_muPlus;
	    p_tChannel4FS = fP_tChannel4FS_muPlus;
	    p_Wjj         = fP_Wjj_muPlus;
	    p_Wbb         = fP_Wbb_muPlus;
	    p_Wcj         = fP_Wcj_muPlus;
	    p_ttbarSL     = fP_ttbarSL_muPlus;
	    p_ttbarDL     = fP_ttbarDL_muPlus;
	} else {
	    // mu-
	    p_sChannel2j  = fP_sChannel2j_muMinus;
	    p_sChannel3j  = fP_sChannel3j_muMinus;
	    p_tChannel4FS = fP_tChannel4FS_muMinus;
	    p_Wjj         = fP_Wjj_muMinus;
	    p_Wbb         = fP_Wbb_muMinus;
	    p_Wcj         = fP_Wcj_muMinus;
	    p_ttbarSL     = fP_ttbarSL_muMinus;
	    p_ttbarDL     = fP_ttbarDL_muMinus;
	}
    }
    
    Double_t sig = p_sChannel2j * llh_sChannel2j + p_sChannel3j * llh_sChannel3j;
    Double_t bkg = p_tChannel4FS * llh_tChannel4FS +
    	p_Wjj * llh_Wjj +
    	p_Wbb * llh_Wbb +
    	p_Wcj * llh_Wcj +
    	p_ttbarSL * llh_ttbarSL +
    	p_ttbarDL * llh_ttbarDL;
    Double_t wjetsSum = p_Wjj * llh_Wjj + p_Wbb * llh_Wbb + p_Wcj * llh_Wcj;
    Double_t ttbarSum = p_ttbarSL * llh_ttbarSL + p_ttbarDL * llh_ttbarDL;
	
    Double_t sChannelRatio = sig / (sig + bkg);
	
    Double_t wjetsRatio            = wjetsSum / (sig + bkg);
    Double_t wjetsRatio_reBinned   = TMath::Power(wjetsRatio, 2.0/3.0); // Rebinning by simple mapping
    Double_t wjetsRatio_binNSpread = 1.0 - TMath::Power(1.0 - wjetsRatio, 0.5);
    // Double_t wjetsMaxCut         = 0.8; // Can use this value to reduce the impact of W+jets events - refers to the rebinned ratio
    
    Double_t ttbarRatio          = ttbarSum / (sig + bkg);
    Double_t ttbarRatio_reBinned = TMath::Power(ttbarRatio, 2.0/3.0); // Rebinning by simple mapping
    Double_t ttbarRatio_binNSpread = 1.0 - TMath::Power(1.0 - ttbarRatio, 0.5);
    
    Int_t bin_sChannelRatio = 1 + TMath::BinarySearch(fgNBins_sChannelRatio,
						      fgBinEdges_schannelRatio,
						      sChannelRatio);
    if ( bin_sChannelRatio == 0 )
	++bin_sChannelRatio; // Don't use underflow bin
    Double_t binWidth_sChannelRatio   = 1.0 / (Double_t)fgNBins_sChannelRatio;
    Double_t sChannelRatio_reBinned   = binWidth_sChannelRatio * ((Double_t)bin_sChannelRatio - 0.5);
    Double_t sChannelRatio_bin1Spread = TMath::Power(sChannelRatio, 0.25);
    
    fHist_llh_sChannel_ratio_raw->Fill(sChannelRatio, GetTagEvtWeight());
    fHist_llh_sChannel_ratio->Fill(sChannelRatio_reBinned, GetTagEvtWeight());
    if ( lepton->Charge() > 0 )
	fHist_llh_sChannel_ratio_lplus->Fill(sChannelRatio_reBinned, GetTagEvtWeight());
    else
	fHist_llh_sChannel_ratio_lminus->Fill(sChannelRatio_reBinned, GetTagEvtWeight());

    fHist_llh_sChannel_ratio_bin1Spread->Fill(sChannelRatio_bin1Spread, GetTagEvtWeight());
    
    Double_t sChannelRatio_bkg_Max = fHist_llh_sChannel_ratio_bkg->GetBinLowEdge(fHist_llh_sChannel_ratio_bkg->GetNbinsX());
    sChannelRatio_bkg_Max += fHist_llh_sChannel_ratio_bkg->GetBinWidth(1);
    if ( sChannelRatio_reBinned < sChannelRatio_bkg_Max ) {
	fHist_llh_sChannel_ratio_bkg->Fill(sChannelRatio_reBinned, GetTagEvtWeight());
    }
	
    if ( bin_sChannelRatio > 1 ) {
	Double_t sChannelRatio_reBinned_bins2N = fHist_llh_sChannel_ratio_bins2N->GetBinCenter(bin_sChannelRatio - 1);
	fHist_llh_sChannel_ratio_bins2N->Fill(sChannelRatio_reBinned_bins2N,
					      GetTagEvtWeight());
	Double_t sChannelRatio_bkg_bins2N_Max = fHist_llh_sChannel_ratio_bkg_bins2N->GetBinLowEdge(fHist_llh_sChannel_ratio_bkg_bins2N->GetNbinsX());
	sChannelRatio_bkg_bins2N_Max += fHist_llh_sChannel_ratio_bkg_bins2N->GetBinWidth(1);
	Double_t sChannelRatio_reBinned_bkg_bins2N = fHist_llh_sChannel_ratio_bkg_bins2N->GetBinCenter(bin_sChannelRatio - 1);
	if ( sChannelRatio_reBinned_bins2N < sChannelRatio_bkg_bins2N_Max )
	    fHist_llh_sChannel_ratio_bkg_bins2N->Fill(sChannelRatio_reBinned_bkg_bins2N,
						      GetTagEvtWeight());
	if ( fEvent->EventNr() % 2 == 0 ) {
	    fHist_llh_sChannel_ratio_bins2N_even->Fill(sChannelRatio_reBinned_bins2N,
						       GetTagEvtWeight());
	    if ( sChannelRatio_reBinned_bins2N < sChannelRatio_bkg_bins2N_Max )
		fHist_llh_sChannel_ratio_bkg_bins2N_even->Fill(sChannelRatio_reBinned_bkg_bins2N,
							       GetTagEvtWeight());
	}
	else {
	    fHist_llh_sChannel_ratio_bins2N_odd->Fill(sChannelRatio_reBinned_bins2N,
						      GetTagEvtWeight());
	    if ( sChannelRatio_reBinned_bins2N < sChannelRatio_bkg_bins2N_Max )
		fHist_llh_sChannel_ratio_bkg_bins2N_odd->Fill(sChannelRatio_reBinned_bkg_bins2N,
							      GetTagEvtWeight());
	}
    }
	
    if ( bin_sChannelRatio > 2 ) {
	Double_t sChannelRatio_reBinned_bins3N = fHist_llh_sChannel_ratio_bins3N->GetBinCenter(bin_sChannelRatio - 2);
	fHist_llh_sChannel_ratio_bins3N->Fill(sChannelRatio_reBinned_bins3N,
					      GetTagEvtWeight());
	Double_t sChannelRatio_bkg_bins3N_Max = fHist_llh_sChannel_ratio_bkg_bins3N->GetBinLowEdge(fHist_llh_sChannel_ratio_bkg_bins3N->GetNbinsX());
	sChannelRatio_bkg_bins3N_Max += fHist_llh_sChannel_ratio_bkg_bins3N->GetBinWidth(1);
	Double_t sChannelRatio_reBinned_bkg_bins3N = fHist_llh_sChannel_ratio_bkg_bins3N->GetBinCenter(bin_sChannelRatio - 2);
	if ( sChannelRatio_reBinned_bins3N < sChannelRatio_bkg_bins3N_Max )
	    fHist_llh_sChannel_ratio_bkg_bins3N->Fill(sChannelRatio_reBinned_bkg_bins3N,
						      GetTagEvtWeight());
	if ( fEvent->EventNr() % 2 == 0 ) {
	    fHist_llh_sChannel_ratio_bins3N_even->Fill(sChannelRatio_reBinned_bins3N,
						       GetTagEvtWeight());
	    if ( sChannelRatio_reBinned_bins3N < sChannelRatio_bkg_bins3N_Max )
		fHist_llh_sChannel_ratio_bkg_bins3N_even->Fill(sChannelRatio_reBinned_bkg_bins3N,
							       GetTagEvtWeight());
	}
	else {
	    fHist_llh_sChannel_ratio_bins3N_odd->Fill(sChannelRatio_reBinned_bins3N,
						      GetTagEvtWeight());
	    if ( sChannelRatio_reBinned_bins3N < sChannelRatio_bkg_bins3N_Max )
		fHist_llh_sChannel_ratio_bkg_bins3N_odd->Fill(sChannelRatio_reBinned_bkg_bins3N,
							      GetTagEvtWeight());
	}
    }	
	
    if ( bin_sChannelRatio > 3 ) {
	Double_t sChannelRatio_reBinned_bins4N = fHist_llh_sChannel_ratio_bins4N->GetBinCenter(bin_sChannelRatio - 3);
	fHist_llh_sChannel_ratio_bins4N->Fill(sChannelRatio_reBinned_bins4N,
					      GetTagEvtWeight());
	Double_t sChannelRatio_bkg_bins4N_Max = fHist_llh_sChannel_ratio_bkg_bins4N->GetBinLowEdge(fHist_llh_sChannel_ratio_bkg_bins4N->GetNbinsX());
	sChannelRatio_bkg_bins4N_Max += fHist_llh_sChannel_ratio_bkg_bins4N->GetBinWidth(1);
	Double_t sChannelRatio_reBinned_bkg_bins4N = fHist_llh_sChannel_ratio_bkg_bins4N->GetBinCenter(bin_sChannelRatio - 3);
	if ( sChannelRatio_reBinned_bins4N < sChannelRatio_bkg_bins4N_Max )
	    fHist_llh_sChannel_ratio_bkg_bins4N->Fill(sChannelRatio_reBinned_bkg_bins4N,
						      GetTagEvtWeight());
	if ( fEvent->EventNr() % 2 == 0 ) {
	    fHist_llh_sChannel_ratio_bins4N_even->Fill(sChannelRatio_reBinned_bins4N,
						       GetTagEvtWeight());
	    if ( sChannelRatio_reBinned_bins4N < sChannelRatio_bkg_bins4N_Max )
		fHist_llh_sChannel_ratio_bkg_bins4N_even->Fill(sChannelRatio_reBinned_bkg_bins4N,
							       GetTagEvtWeight());
	}
	else {
	    fHist_llh_sChannel_ratio_bins4N_odd->Fill(sChannelRatio_reBinned_bins4N,
						      GetTagEvtWeight());
	    if ( sChannelRatio_reBinned_bins4N < sChannelRatio_bkg_bins4N_Max )
		fHist_llh_sChannel_ratio_bkg_bins4N_odd->Fill(sChannelRatio_reBinned_bkg_bins4N,
							      GetTagEvtWeight());
	}
    }
	
	
    if ( fEvent->EventNr() % 2 == 0 )
	fHist_llh_sChannel_ratio_even->Fill(sChannelRatio_reBinned, GetTagEvtWeight());
    else
	fHist_llh_sChannel_ratio_odd->Fill(sChannelRatio_reBinned, GetTagEvtWeight());
	
    fHist_llh_wjets_ratio->Fill(wjetsRatio_reBinned, GetTagEvtWeight());
    fHist_llh_wjets_ratio_binNSpread->Fill(wjetsRatio_binNSpread, GetTagEvtWeight());
    if ( fEvent->EventNr() % 2 == 0 )
	fHist_llh_wjets_ratio_even->Fill(wjetsRatio_reBinned, GetTagEvtWeight());
    else
	fHist_llh_wjets_ratio_odd->Fill(wjetsRatio_reBinned, GetTagEvtWeight());
    fHist_llh_wjets_ratio_raw->Fill(wjetsRatio, GetTagEvtWeight());
	
    fHist_llh_ttbar_ratio->Fill(ttbarRatio_reBinned, GetTagEvtWeight());
    fHist_llh_ttbar_ratio_binNSpread->Fill(ttbarRatio_binNSpread, GetTagEvtWeight());


    // 2D Histograms:
    fHist_2D_sChannel_vs_Wjets->Fill(sChannelRatio, wjetsRatio, GetTagEvtWeight());
    fHist_2D_sChannel_vs_ttbar->Fill(sChannelRatio, ttbarRatio, GetTagEvtWeight());
    // Double_t SumLLH = sig+bkg;
    // fHist_2D_sChannel_vs_SumLLH->Fill(sChannelRatio, TMath::Log10(SumLLH), GetTagEvtWeight());
    
    if (lepCharge > 0.) {
	fHist_logllh_sChannel2j_plus->Fill(TMath::Log10(llh_sChannel2j), GetTagEvtWeight());
	fHist_logllh_sChannel3j_plus->Fill(TMath::Log10(llh_sChannel3j), GetTagEvtWeight());
	fHist_logllh_tChannel4FS_plus->Fill(TMath::Log10(llh_tChannel4FS), GetTagEvtWeight());
	fHist_logllh_ttbarSL_plus->Fill(TMath::Log10(llh_ttbarSL), GetTagEvtWeight());
	fHist_logllh_ttbarDL_plus->Fill(TMath::Log10(llh_ttbarDL), GetTagEvtWeight());
	fHist_logllh_Wjj_plus->Fill(TMath::Log10(llh_Wjj), GetTagEvtWeight());
	fHist_logllh_Wcj_plus->Fill(TMath::Log10(llh_Wcj), GetTagEvtWeight());
	fHist_logllh_Wbb_plus->Fill(TMath::Log10(llh_Wbb), GetTagEvtWeight());
    } else {
    	fHist_logllh_sChannel2j_minus->Fill(TMath::Log10(llh_sChannel2j), GetTagEvtWeight());
	fHist_logllh_sChannel3j_minus->Fill(TMath::Log10(llh_sChannel3j), GetTagEvtWeight());
	fHist_logllh_tChannel4FS_minus->Fill(TMath::Log10(llh_tChannel4FS), GetTagEvtWeight());
	fHist_logllh_ttbarSL_minus->Fill(TMath::Log10(llh_ttbarSL), GetTagEvtWeight());
	fHist_logllh_ttbarDL_minus->Fill(TMath::Log10(llh_ttbarDL), GetTagEvtWeight());
	fHist_logllh_Wjj_minus->Fill(TMath::Log10(llh_Wjj), GetTagEvtWeight());
	fHist_logllh_Wcj_minus->Fill(TMath::Log10(llh_Wcj), GetTagEvtWeight());
	fHist_logllh_Wbb_minus->Fill(TMath::Log10(llh_Wbb), GetTagEvtWeight());
    }
	
    fHist_logllh_sChannel2j->Fill(TMath::Log10(llh_sChannel2j), GetTagEvtWeight());
    fHist_logllh_sChannel3j->Fill(TMath::Log10(llh_sChannel3j), GetTagEvtWeight());
    fHist_logllh_tChannel4FS->Fill(TMath::Log10(llh_tChannel4FS), GetTagEvtWeight());
    fHist_logllh_ttbarSL->Fill(TMath::Log10(llh_ttbarSL), GetTagEvtWeight());
    fHist_logllh_ttbarSL_ttbarCR->Fill(TMath::Log10(llh_ttbarSL), GetTagEvtWeight());
    fHist_logllh_ttbarDL->Fill(TMath::Log10(llh_ttbarDL), GetTagEvtWeight());
    fHist_logllh_ttbarDL_wjetsCR->Fill(TMath::Log10(llh_ttbarDL), GetTagEvtWeight());
    fHist_logllh_ttbarDL_ttbarCR->Fill(TMath::Log10(llh_ttbarDL), GetTagEvtWeight());
    fHist_logllh_Wjj->Fill(TMath::Log10(llh_Wjj), GetTagEvtWeight());
    fHist_logllh_Wcj->Fill(TMath::Log10(llh_Wcj), GetTagEvtWeight());
    fHist_logllh_Wcj_wjetsCR->Fill(TMath::Log10(llh_Wcj), GetTagEvtWeight());
    fHist_logllh_Wbb->Fill(TMath::Log10(llh_Wbb), GetTagEvtWeight());

    // Fill Histograms depending on choosen binning
    // strategy taken from TH1::FindBin();
    // do not use AddBinContent here, this will cause problems during merging (hadd)

    // Fill schannel prob. for each lepton / charge combination
    Int_t bin = 0;
	
    // Binning Study

    // Binning 1
    bin = 1 + TMath::BinarySearch(fgNBins_raw1, fgBinEdges_raw1, sChannelRatio);
    if ( bin == 0 ) bin++; // prevent underflow bin to be filled
    fHist_llh_sChannel_ratio1->Fill((bin-0.5)/fgNBins_raw1, GetTagEvtWeight());
    if ( fEvent->EventNr() % 2 == 0 )
	fHist_llh_sChannel_ratio1_even->Fill((bin-0.5)/fgNBins_raw1, GetTagEvtWeight());
    else
	fHist_llh_sChannel_ratio1_odd->Fill((bin-0.5)/fgNBins_raw1, GetTagEvtWeight());
    
    // Binning 2
    bin = 1 + TMath::BinarySearch(fgNBins_raw2, fgBinEdges_raw2, sChannelRatio);
    if ( bin == 0 ) bin++; // prevent underflow bin to be filled
    fHist_llh_sChannel_ratio2->Fill((bin-0.5)/fgNBins_raw2, GetTagEvtWeight());
    if ( fEvent->EventNr() % 2 == 0 )
	fHist_llh_sChannel_ratio2_even->Fill((bin-0.5)/fgNBins_raw2, GetTagEvtWeight());
    else
	fHist_llh_sChannel_ratio2_odd->Fill((bin-0.5)/fgNBins_raw2, GetTagEvtWeight());

    // Binning 3
    bin = 1 + TMath::BinarySearch(fgNBins_raw3, fgBinEdges_raw3, sChannelRatio);
    if ( bin == 0 ) bin++; // prevent underflow bin to be filled
    fHist_llh_sChannel_ratio3->Fill((bin-0.5)/fgNBins_raw3, GetTagEvtWeight());
    if ( fEvent->EventNr() % 2 == 0 )
	fHist_llh_sChannel_ratio3_even->Fill((bin-0.5)/fgNBins_raw3, GetTagEvtWeight());
    else
	fHist_llh_sChannel_ratio3_odd->Fill((bin-0.5)/fgNBins_raw3, GetTagEvtWeight());

    // Binning 4
    bin = 1 + TMath::BinarySearch(fgNBins_raw4, fgBinEdges_raw4, sChannelRatio);
    if ( bin == 0 ) bin++; // prevent underflow bin to be filled
    fHist_llh_sChannel_ratio4->Fill((bin-0.5)/fgNBins_raw4, GetTagEvtWeight());
    if ( fEvent->EventNr() % 2 == 0 )
	fHist_llh_sChannel_ratio4_even->Fill((bin-0.5)/fgNBins_raw4, GetTagEvtWeight());
    else
	fHist_llh_sChannel_ratio4_odd->Fill((bin-0.5)/fgNBins_raw4, GetTagEvtWeight());

    // Binning 5
    bin = 1 + TMath::BinarySearch(fgNBins_raw5, fgBinEdges_raw5, sChannelRatio);
    if ( bin == 0 ) bin++; // prevent underflow bin to be filled
    fHist_llh_sChannel_ratio5->Fill((bin-0.5)/fgNBins_raw5, GetTagEvtWeight());
    if ( fEvent->EventNr() % 2 == 0 )
	fHist_llh_sChannel_ratio5_even->Fill((bin-0.5)/fgNBins_raw5, GetTagEvtWeight());
    else
	fHist_llh_sChannel_ratio5_odd->Fill((bin-0.5)/fgNBins_raw5, GetTagEvtWeight());
    
    // Binning optimized with setting zs = 12, zb = 8
    if ( lepton->IsElectron() ) {
	if ( lepCharge > 0. ) {
	    // e+
	    bin = 1 + TMath::BinarySearch(fgNBins_raw_eplus,fgBinEdges_raw_eplus, sChannelRatio);
	    fHist_llh_sChannel_ratio_raw_eplus_rebin->Fill((bin-0.5)/fgNBins_raw_eplus, GetTagEvtWeight());
	    fHist_llh_sChannel_ratio_raw_eplus->Fill(sChannelRatio, GetTagEvtWeight());
	    if ( fEvent->EventNr() % 2 == 0 )
		fHist_llh_sChannel_ratio_raw_eplus_rebin_even->Fill((bin-0.5)/fgNBins_raw_eplus, GetTagEvtWeight());
	    else
		fHist_llh_sChannel_ratio_raw_eplus_rebin_odd->Fill((bin-0.5)/fgNBins_raw_eplus, GetTagEvtWeight());
	} else {
	    // e-
	    bin = 1 + TMath::BinarySearch(fgNBins_raw_eminus,fgBinEdges_raw_eminus, sChannelRatio);
	    fHist_llh_sChannel_ratio_raw_eminus_rebin->Fill((bin-0.5)/fgNBins_raw_eminus, GetTagEvtWeight());
	    fHist_llh_sChannel_ratio_raw_eminus->Fill(sChannelRatio, GetTagEvtWeight());
	    if ( fEvent->EventNr() % 2 == 0 )
		fHist_llh_sChannel_ratio_raw_eminus_rebin_even->Fill((bin-0.5)/fgNBins_raw_eminus, GetTagEvtWeight());
	    else
		fHist_llh_sChannel_ratio_raw_eminus_rebin_odd->Fill((bin-0.5)/fgNBins_raw_eminus, GetTagEvtWeight());
	}
    } else {
	if ( lepCharge > 0. ) {
	    // mu+
	    bin = 1 + TMath::BinarySearch(fgNBins_raw_muplus,fgBinEdges_raw_muplus, sChannelRatio);
	    fHist_llh_sChannel_ratio_raw_muplus_rebin->Fill((bin-0.5)/fgNBins_raw_muplus, GetTagEvtWeight());
	    fHist_llh_sChannel_ratio_raw_muplus->Fill(sChannelRatio, GetTagEvtWeight());
	    if ( fEvent->EventNr() % 2 == 0 )
		fHist_llh_sChannel_ratio_raw_muplus_rebin_even->Fill((bin-0.5)/fgNBins_raw_muplus, GetTagEvtWeight());
	    else
		fHist_llh_sChannel_ratio_raw_muplus_rebin_odd->Fill((bin-0.5)/fgNBins_raw_muplus, GetTagEvtWeight());		
	} else {
	    // mu-
	    bin = 1 + TMath::BinarySearch(fgNBins_raw_muminus,fgBinEdges_raw_muminus, sChannelRatio);
	    fHist_llh_sChannel_ratio_raw_muminus_rebin->Fill((bin-0.5)/fgNBins_raw_muminus, GetTagEvtWeight());
	    fHist_llh_sChannel_ratio_raw_muminus->Fill(sChannelRatio, GetTagEvtWeight());
	    if ( fEvent->EventNr() % 2 == 0 )
		fHist_llh_sChannel_ratio_raw_muminus_rebin_even->Fill((bin-0.5)/fgNBins_raw_muplus, GetTagEvtWeight());
	    else
		fHist_llh_sChannel_ratio_raw_muminus_rebin_odd->Fill((bin-0.5)/fgNBins_raw_muplus, GetTagEvtWeight());		
	}
    }
}

//____________________________________________________________________

void AtlSgTop_sChannelMemDisc::InitEvent() {
    //
    // Prepare event analysis
    //
}

//____________________________________________________________________

void AtlSgTop_sChannelMemDisc::Print(Option_t *option) const {
    //
    // Print user analysis configuration
    //
    cout << endl
	 << "================================================================" << endl
	 << "  Atlas Single-Top s-Channel Mem Discriminant Analysis Selector " << endl
	 << "================================================================" << endl
	 << endl;
    AtlSelector::Print();
    cout << endl
	 << "================================================================" << endl
	 << endl;
}

//____________________________________________________________________

void AtlSgTop_sChannelMemDisc::Terminate() {
    //
    // Terminate routine called at the end of event loop
    //    
	
    AtlSelector::Terminate();
}

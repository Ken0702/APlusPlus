//____________________________________________________________________
//
// Single-top s-Channel Mem Discriminant Analysis selector
// 
//  
// Author: Stephan Kaphle <mailto: kaphle@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2013 (C) Soeren Stamm
// 
#include <AtlSgTop_combinationMemDiscR2.h>
#include <AtlEvtWriterMemR2.h>          // for AtlEvtWriterMemR2
#include <AtlQCDMMTool.h>               // for AtlQCDMMTool
#include <TCollection.h>                // for TIter
#include <TDirectory.h>                 // for TDirectory, gDirectory
#include <TH1.h>                        // for TH1F
#include <TH2.h>                        // for TH2F
#include <TList.h>                      // for TList
#include <TMath.h>                      // for Log10, Power, BinarySearch
#include <TObject.h>                    // for TObject
#include <TString.h>                    // for Form, TString
#include <TTree.h>                      // for TTree
#include <iostream>                     // for operator<<, endl, basic_ostre...
#include "AtlAnalysisTool.h"            // for AtlAnalysisTool, AtlAnalysisT...
#include "AtlCutFlowTool.h"             // for AtlCutFlowTool
#include "AtlEnergySum.h"               // for AtlEnergySum
#include "AtlEvent.h"                   // for AtlEvent
#include "AtlEvtWriterBase.h"           // for AtlEvtWriterBase
#include "AtlHistogramTool.h"           // for AtlHistogramTool
#include "AtlJet.h"                     // for AtlJet
#include "AtlObjRecoScaleFactorTool.h"  // for AtlObjRecoScaleFactorTool
#include "AtlObjectsToolD3PDSgTop.h"    // for AtlObjectsToolD3PDSgTop
#include "AtlPdfReweightingTool.h"      // for AtlPdfReweightingTool
#include "AtlSelector.h"                // for AtlSelector
#include "HepParticle.h"                // for HepParticle

using namespace std;

// rebinned bin edges:
// from 8 TeV analysis
// const Double_t AtlSgTop_sChannelMemDiscR2::fgBinEdges_schannelRatio[fgNBins_sChannelRatio+1] = {
//     0.00000, 0.00015, 0.00115, 0.00410,	0.01015,
//     0.01820, 0.02780, 0.03725, 0.04715, 0.05785,
//     0.06865, 0.07945, 0.09050, 0.10175, 0.11330,
//     0.12540, 0.13835, 0.15235, 0.16835, 0.18740,
//     0.21180, 0.24710, 0.31535, 1.00000};

// from 13 TeV analysis with TRexFitter
const Double_t AtlSgTop_combinationMemDiscR2::fgBinEdges_schannelRatio[fgNBins_sChannelRatio+1] = {
   -0.0217391, 0.000300, 0.002000, 0.005150, 0.008700,
    0.0125500, 0.016575, 0.020575, 0.024600, 0.028500,
    0.0322750, 0.036150, 0.040200, 0.044400, 0.048925,
    0.0538000, 0.059325, 0.065800, 0.073775, 0.083825,
   0.0971500, 0.118175, 0.157150, //0.591450,
   1.021740};

const Double_t AtlSgTop_combinationMemDiscR2::fgBinEdges_raw_eplus[AtlSgTop_combinationMemDiscR2::fgNBins_raw_eplus+1]= {
    0.00000000, 0.00200000, 0.01100000, 0.02400000, 0.03750000,
    0.05100000, 0.06350000, 0.07550000, 0.08700000, 0.09800000,
    0.10900000, 0.12050000, 0.13250000, 0.14500000, 0.15900000,
    0.17600000, 0.19800000, 0.23050000, 0.29500000, 1.00000000};
const Double_t AtlSgTop_combinationMemDiscR2::fgBinEdges_raw_eminus[AtlSgTop_combinationMemDiscR2::fgNBins_raw_eminus+1]= {
    0.00000000, 0.00050000, 0.00550000, 0.01400000, 0.02400000,
    0.03350000, 0.04250000, 0.05150000, 0.06050000, 0.06850000,
    0.07650000, 0.08500000, 0.09400000, 0.10300000, 0.11300000,
    0.12450000, 0.14000000, 0.16200000, 0.20300000, 1.00000000};    
const Double_t AtlSgTop_combinationMemDiscR2::fgBinEdges_raw_muplus[AtlSgTop_combinationMemDiscR2::fgNBins_raw_muplus+1]= {
    0.00000000, 0.00350000, 0.01500000, 0.03050000, 0.04450000,
    0.06000000, 0.07550000, 0.09100000, 0.10550000, 0.11900000,
    0.13350000, 0.14750000, 0.16300000, 0.18000000, 0.19950000,
    0.22350000, 0.25500000, 0.30150000, 0.37950000, 1.00000000};
const Double_t AtlSgTop_combinationMemDiscR2::fgBinEdges_raw_muminus[AtlSgTop_combinationMemDiscR2::fgNBins_raw_muminus+1]= {
    0.00000000, 0.00100000, 0.00700000, 0.01650000, 0.02750000,
    0.03850000, 0.04900000, 0.05950000, 0.06950000, 0.07900000,
    0.08900000, 0.09900000, 0.11000000, 0.12200000, 0.13550000,
    0.15150000, 0.17300000, 0.20500000, 0.26500000, 1.00000000};
const Double_t AtlSgTop_combinationMemDiscR2::fgBinEdges_raw1[AtlSgTop_combinationMemDiscR2::fgNBins_raw1+1]= {
    0.00000000, 0.00050000, 0.00550000, 0.01600000, 0.02950000,
    0.04350000, 0.05750000, 0.07150000, 0.08500000, 0.09800000,
    0.11100000, 0.12450000, 0.13850000, 0.15400000, 0.17100000,
    0.19150000, 0.21750000, 0.25500000, 0.32650000, 1.00000000};
const Double_t AtlSgTop_combinationMemDiscR2::fgBinEdges_raw2[AtlSgTop_combinationMemDiscR2::fgNBins_raw2+1]= {
    0.00000000, 0.00100000, 0.00750000, 0.02000000, 0.03400000,
    0.04800000, 0.06150000, 0.07450000, 0.08700000, 0.09950000,
    0.11150000, 0.12400000, 0.13700000, 0.15050000, 0.16550000,
    0.18250000, 0.20250000, 0.22900000, 0.26900000, 0.34250000,
    1.00000000};
const Double_t AtlSgTop_combinationMemDiscR2::fgBinEdges_raw3[AtlSgTop_combinationMemDiscR2::fgNBins_raw3+1]= {
    0.00000000, 0.00050000, 0.00400000, 0.01150000, 0.02200000,
    0.03300000, 0.04450000, 0.05600000, 0.06700000, 0.07800000,
    0.08900000, 0.10000000, 0.11050000, 0.12150000, 0.13300000,
    0.14450000, 0.15750000, 0.17200000, 0.18850000, 0.20850000,
    0.23400000, 0.27400000, 0.34700000, 1.00000000};
const Double_t AtlSgTop_combinationMemDiscR2::fgBinEdges_raw4[AtlSgTop_combinationMemDiscR2::fgNBins_raw4+1]= {
    0.00000000, 0.00050000, 0.00250000, 0.00750000, 0.01500000,
    0.02450000, 0.03400000, 0.04400000, 0.05400000, 0.06400000,
    0.07400000, 0.08350000, 0.09300000, 0.10250000, 0.11200000,
    0.12200000, 0.13250000, 0.14300000, 0.15450000, 0.16700000,
    0.18100000, 0.19750000, 0.21750000, 0.24350000, 0.28400000,
    0.35800000, 1.00000000};    
const Double_t AtlSgTop_combinationMemDiscR2::fgBinEdges_raw5[AtlSgTop_combinationMemDiscR2::fgNBins_raw5+1]= {
    0.00000000, 0.00050000, 0.00200000, 0.00600000, 0.01250000,
    0.02100000, 0.03000000, 0.03900000, 0.04800000, 0.05750000,
    0.06700000, 0.07600000, 0.08500000, 0.09400000, 0.10300000,
    0.11200000, 0.12100000, 0.13050000, 0.14000000, 0.15050000,
    0.16150000, 0.17400000, 0.18800000, 0.20400000, 0.22350000,
    0.25050000, 0.29200000, 0.36650000, 1.00000000};

//____________________________________________________________________

AtlSgTop_combinationMemDiscR2::AtlSgTop_combinationMemDiscR2(const char* OutputFile) :
    AtlSelector(OutputFile) {
    //
    // Default constructor
    //
    
    // Init
    fLepton    = nullptr;
    fLeptons   = nullptr;
    fLooseLeptons = nullptr;
    fElectrons = nullptr;
    fMuons     = nullptr;
    fJets      = nullptr;
    fBJets     = nullptr;

    fQCDMMToolIsOn = kFALSE;
    fNNegativeLlh = 0;

    fObjTool     = nullptr;
    fSFTool      = nullptr;
    fCutFlowTool = nullptr;
    fPdfTool     = nullptr;
    fHistsLlh    = nullptr;
    fHistsLlhMu  = nullptr;
    fHistsLlhE   = nullptr;

    SetCutDefaults();
	
}

//____________________________________________________________________

AtlSgTop_combinationMemDiscR2::~AtlSgTop_combinationMemDiscR2() {
    //
    // Default destructor
    //
    if ( nullptr != fEvtWriter )
        delete fEvtWriter;
}

//____________________________________________________________________

void AtlSgTop_combinationMemDiscR2::Clear(Option_t */*option*/) {
    //
    //  Clears the dynamically allocated objects
    //
}

//____________________________________________________________________

void AtlSgTop_combinationMemDiscR2::Begin(TTree *tree) {
    //
    // Begin analysis job
    //

    // Create event writer
    // the AtlEvtWriterMemR2 only writes out MEM-related branches
    fEvtWriter = new AtlEvtWriterMemR2;
    dynamic_cast<AtlEvtWriterMemR2*>(fEvtWriter)->SetCloneOnly(fEvtWriterCloneOnly);
    dynamic_cast<AtlEvtWriterMemR2*>(fEvtWriter)->SetReduceOutput(fEvtWriterReduceOutput);

    AtlSelector::Begin(tree);
}

//____________________________________________________________________

void AtlSgTop_combinationMemDiscR2::SetBranchStatus() {
    //
    // Switch on/off branches in order to gain speed
    //

    // Switch off all branches by default
    fTree->SetBranchStatus("*", kFALSE);
}

//____________________________________________________________________

void AtlSgTop_combinationMemDiscR2::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    fPrintEvent = kFALSE;

    // fInputMode: not needed in Run-2, defined in Selector
    // fOutputMode: Depr.
    // fEvtReader: not needed in Run-2, defined in Selector
    // fEvtWriter: set fEvtWriter in ::Begin()

    // Cut defaults
    fMET_min    = 0.;
    fMET_max    = 1.e10;
    fMtW_minPre = 0.;
    fMtW_maxPre = 1.e10;
    fMtW_min    = 0.;
    fMtW_max    = 1.e10;
    fPt_Jet_min = 0.;
    fPt_Jet_max = 1.e10;
    fPtJetlead_min = 0.;
    fPtJetlead_max = 1.e10;
    fNBTags_min = 0;
    fNBTags_max = 999;
    fAdditionalJetVeto = kFALSE;
    fApplyDileptonVeto = kTRUE;  // Run-2 default
    fLeadingJetIsBTagged = kFALSE;
    fVetoBTagger = AtlBTag::kInvalidTagger;  // use invalid to turn off
    fVetoBTagWeight_min = 0.5;   // Run-2 weights are binary 0. or 1.
    fEvtWriterCloneOnly = kFALSE;
    fEvtWriterReduceOutput = kFALSE;
    fProcessNegativeLlh = kFALSE;
    fOnlyOdd = kFALSE;
}

//____________________________________________________________________

void AtlSgTop_combinationMemDiscR2::BookHistograms() {
    //
    // Book histograms
    //    
	
    // Get ObjectSelection Tool + pointers to analysis objects
    fObjTool = dynamic_cast<AtlObjectsToolD3PDSgTop*>(
        GetTool("AtlObjectsToolD3PDSgTop", "", kTRUE));
    fElectrons = fObjTool->GetElectrons();
    fMuons     = fObjTool->GetMuons();
    fLeptons   = fObjTool->GetLeptons();
    fLooseLeptons = fObjTool->GetLooseLeptons();
    fJets      = fObjTool->GetJets();
    fBJets     = fObjTool->GetBJets();

    // Provide tools with pointers to analysis objects
    fSFTool = dynamic_cast<AtlObjRecoScaleFactorTool*>(
        GetTool("AtlObjRecoScaleFactorTool", "", kTRUE));
    fSFTool->SetLeptons(fLeptons);
    fSFTool->SetJets(fJets);

    // Get pointer to cut-flow tool
    fCutFlowTool = dynamic_cast<AtlCutFlowTool*>(
        GetTool("AtlCutFlowTool", "", kTRUE));

    // Provide tools with histogram names
    fPdfTool = dynamic_cast<AtlPdfReweightingTool*>(
        GetTool("AtlPdfReweightingTool", "", kFALSE));

    if ( fPdfTool != 0 ) {
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
    }
    
    // Look if QCD MM Tool is running
    AtlAnalysisTool *tool = dynamic_cast<AtlQCDMMTool*>(
        GetTool("AtlQCDMMTool", "", kFALSE));
    if ( ( tool != 0 ) && ( tool->fProcessMode != AtlAnalysisTool::kOff ) ) {
	fQCDMMToolIsOn = kTRUE;
	Info(__FUNCTION__, "QCD Tool found");
    } else {
	Info(__FUNCTION__, "QCD Tool not found");
    }

    // a-priori probabilities

    if ( fApplyDileptonVeto ) {
        // with AtlasUtils/calculate_alphas.py; input v07_MEM
        // Double_t ttbar_muPlus = 0.17989;
        // Double_t ttbar_eMinus = 0.19144;
        // Double_t ttbar_ePlus = 0.19365;
        // Double_t ttbar_muMinus = 0.17954;
        // Double_t Wjj_muPlus = 0.00067;
        // Double_t Wjj_eMinus = 0.00122;
        // Double_t Wjj_ePlus = -0.00011;
        // Double_t Wjj_muMinus = 0.00142;
        // Double_t sChannel_muPlus = 0.00760;
        // Double_t sChannel_eMinus = 0.00482;
        // Double_t sChannel_ePlus = 0.00734;
        // Double_t sChannel_muMinus = 0.00504;
        // Double_t tChannel4FS_muPlus = 0.02596;
        // Double_t tChannel4FS_eMinus = 0.01687;
        // Double_t tChannel4FS_ePlus = 0.02549;
        // Double_t tChannel4FS_muMinus = 0.01622;
        // Double_t wHF_muPlus = 0.03875;
        // Double_t wHF_eMinus = 0.03460;
        // Double_t wHF_ePlus = 0.03980;
        // Double_t wHF_muMinus = 0.02977;

        // with AtlasUtils/calculate_alphas.py; input v07_MEM/.../Maxcut
        Double_t ttbar_muPlus = 0.16874;
        Double_t ttbar_eMinus = 0.17406;
        Double_t ttbar_ePlus = 0.17752;
        Double_t ttbar_muMinus = 0.16659;
        Double_t Wjj_muPlus = 0.00361;
        Double_t Wjj_eMinus = 0.00331;
        Double_t Wjj_ePlus = 0.00051;
        Double_t Wjj_muMinus = 0.00157;
        Double_t sChannel_muPlus = 0.01198;
        Double_t sChannel_eMinus = 0.00747;
        Double_t sChannel_ePlus = 0.01151;
        Double_t sChannel_muMinus = 0.00827;
        Double_t tChannel4FS_muPlus = 0.03649;
        Double_t tChannel4FS_eMinus = 0.02488;
        Double_t tChannel4FS_ePlus = 0.03529;
        Double_t tChannel4FS_muMinus = 0.02156;
        Double_t wHF_muPlus = 0.03879;
        Double_t wHF_eMinus = 0.03416;
        Double_t wHF_ePlus = 0.04196;
        Double_t wHF_muMinus = 0.03172;

	// probabilities after di-lepton veto
	Double_t sChannel2jVS3jFrac = 0.65; //nominal
     // Double_t sChannel2jVS3jFrac = 0.8;
std::cout<<sChannel2jVS3jFrac<<std::endl;

	// Double_t sChannel_eMinus  = 0.007231;
	// Double_t sChannel_ePlus   = 0.01279;
	// Double_t sChannel_muMinus = 0.009033;
	// Double_t sChannel_muPlus  = 0.01634;
	
	fP_sChannel2j_ePlus   = sChannel2jVS3jFrac * sChannel_ePlus;
	fP_sChannel2j_muPlus  = sChannel2jVS3jFrac * sChannel_muPlus;
	fP_sChannel2j_eMinus  = sChannel2jVS3jFrac * sChannel_eMinus;
	fP_sChannel2j_muMinus = sChannel2jVS3jFrac * sChannel_muMinus;
	
	fP_sChannel3j_ePlus   = (1.0 - sChannel2jVS3jFrac) * sChannel_ePlus;
	fP_sChannel3j_muPlus  = (1.0 - sChannel2jVS3jFrac) * sChannel_muPlus;
	fP_sChannel3j_eMinus  = (1.0 - sChannel2jVS3jFrac) * sChannel_eMinus;
	fP_sChannel3j_muMinus = (1.0 - sChannel2jVS3jFrac) * sChannel_muMinus;
	
	fP_tChannel4FS_ePlus   = tChannel4FS_ePlus;
	fP_tChannel4FS_muPlus  = tChannel4FS_muPlus;
	fP_tChannel4FS_eMinus  = tChannel4FS_eMinus;
	fP_tChannel4FS_muMinus = tChannel4FS_muMinus;
	
	// fP_tChannel4FS_ePlus   = 0.02537;
	// fP_tChannel4FS_muPlus  = 0.0317;
	// fP_tChannel4FS_eMinus  = 0.01628;
	// fP_tChannel4FS_muMinus = 0.0192;

	fP_Wjj_ePlus   = Wjj_ePlus;
	fP_Wjj_muPlus  = Wjj_muPlus;
	fP_Wjj_eMinus  = Wjj_eMinus;
	fP_Wjj_muMinus = Wjj_muMinus;
	// fP_Wjj_ePlus   = 0.002393;
	// fP_Wjj_muPlus  = 0.01169;
	// fP_Wjj_eMinus  = 0.0005399;
	// fP_Wjj_muMinus = 0.006152;
	
	Double_t wbbVSwcjFrac = 0.8;
	
	// Double_t wHF_eMinus  = 0.03918;
	// Double_t wHF_ePlus   = 0.05155;
	// Double_t wHF_muMinus = 0.04962;
	// Double_t wHF_muPlus  = 0.06589;
	
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
	
	// Double_t ttbar_eMinus  = 0.1444;
	// Double_t ttbar_ePlus   = ttbar_eMinus;
	// Double_t ttbar_muMinus = 0.1731;
	// Double_t ttbar_muPlus  = ttbar_muMinus;
	
	fP_ttbarSL_ePlus   = ttbarSL_vs_ttbarDL_frac_el * ttbar_ePlus;
	fP_ttbarSL_muPlus  = ttbarSL_vs_ttbarDL_frac_mu * ttbar_muPlus;
	fP_ttbarSL_eMinus  = ttbarSL_vs_ttbarDL_frac_el * ttbar_eMinus;
	fP_ttbarSL_muMinus = ttbarSL_vs_ttbarDL_frac_mu * ttbar_muMinus;
	
	fP_ttbarDL_ePlus   = (1.0 - ttbarSL_vs_ttbarDL_frac_el) * ttbar_ePlus;
	fP_ttbarDL_muPlus  = (1.0 - ttbarSL_vs_ttbarDL_frac_mu) * ttbar_muPlus;
	fP_ttbarDL_eMinus  = (1.0 - ttbarSL_vs_ttbarDL_frac_el) * ttbar_eMinus;
	fP_ttbarDL_muMinus = (1.0 - ttbarSL_vs_ttbarDL_frac_mu) * ttbar_muMinus;

        // // s-channel: 2j and 3j
        // Double_t sChannel2jVS3jFrac = 0.65;   // theoretical value?
	// Double_t sChannel_eMinus  = 0.016904194578069506;
	// Double_t sChannel_ePlus   = 0.023594462265743712;
	// Double_t sChannel_muMinus = 0.020465810371475778;
	// Double_t sChannel_muPlus  = 0.028281150783959361;

	// fP_sChannel2j_ePlus   = sChannel2jVS3jFrac * sChannel_ePlus;
	// fP_sChannel2j_muPlus  = sChannel2jVS3jFrac * sChannel_muPlus;
	// fP_sChannel2j_eMinus  = sChannel2jVS3jFrac * sChannel_eMinus;
	// fP_sChannel2j_muMinus = sChannel2jVS3jFrac * sChannel_muMinus;
        // fP_sChannel3j_ePlus   = (1.0 - sChannel2jVS3jFrac) * sChannel_ePlus;
	// fP_sChannel3j_muPlus  = (1.0 - sChannel2jVS3jFrac) * sChannel_muPlus;
	// fP_sChannel3j_eMinus  = (1.0 - sChannel2jVS3jFrac) * sChannel_eMinus;
	// fP_sChannel3j_muMinus = (1.0 - sChannel2jVS3jFrac) * sChannel_muMinus;

        // // t-channel
	// fP_tChannel4FS_ePlus   = 0.081926388654398108;
	// fP_tChannel4FS_muPlus  = 0.096566209153519267;
	// fP_tChannel4FS_eMinus  = 0.057770984129838603;
	// fP_tChannel4FS_muMinus = 0.065821030752011941;

        // // W+light jets
	// fP_Wjj_ePlus   = -0.00033766672294445384;
	// fP_Wjj_muPlus  = 0.0024910858203487521;
	// fP_Wjj_eMinus  = 0.0041810906802139999;
	// fP_Wjj_muMinus = 0.0057560091669775623;

        // // W+jets HF: bb and cj
	// Double_t wbbVSwcjFrac = 0.8; // theoretical value?
	// Double_t wHF_eMinus  = 0.11846423593939666;
	// Double_t wHF_ePlus   = 0.12793347965557994;
	// Double_t wHF_muMinus = 0.12082289612535309;
        // Double_t wHF_muPlus  = 0.14414106384017975;
	
	// fP_Wbb_ePlus   = wbbVSwcjFrac * wHF_ePlus;
	// fP_Wbb_muPlus  = wbbVSwcjFrac * wHF_muPlus;
	// fP_Wbb_eMinus  = wbbVSwcjFrac * wHF_eMinus;
	// fP_Wbb_muMinus = wbbVSwcjFrac * wHF_muMinus;	
	// fP_Wcj_ePlus   = (1.0 - wbbVSwcjFrac) * wHF_ePlus;
	// fP_Wcj_muPlus  = (1.0 - wbbVSwcjFrac) * wHF_muPlus;
	// fP_Wcj_eMinus  = (1.0 - wbbVSwcjFrac) * wHF_eMinus;
	// fP_Wcj_muMinus = (1.0 - wbbVSwcjFrac) * wHF_muMinus;

        // // ttbar: SL and DL (lnu jj, lnu lnu)
	// // v15 fraction after veto, but without lead. jet pt > 40 GeV cut
	// Double_t ttbarSL_vs_ttbarDL_frac_el = 0.3217;
	// Double_t ttbarSL_vs_ttbarDL_frac_mu = 0.3254;	
	// Double_t ttbar_eMinus  = 0.65539720361462028;
	// Double_t ttbar_ePlus   = 0.62244639540773261;
	// Double_t ttbar_muMinus = 0.72866812343441878;
	// Double_t ttbar_muPlus  = 0.66912518927367748;
	
	// fP_ttbarSL_ePlus   = ttbarSL_vs_ttbarDL_frac_el * ttbar_ePlus;
	// fP_ttbarSL_muPlus  = ttbarSL_vs_ttbarDL_frac_mu * ttbar_muPlus;
	// fP_ttbarSL_eMinus  = ttbarSL_vs_ttbarDL_frac_el * ttbar_eMinus;
	// fP_ttbarSL_muMinus = ttbarSL_vs_ttbarDL_frac_mu * ttbar_muMinus;
	
	// fP_ttbarDL_ePlus   = (1.0 - ttbarSL_vs_ttbarDL_frac_el) * ttbar_ePlus;
	// fP_ttbarDL_muPlus  = (1.0 - ttbarSL_vs_ttbarDL_frac_mu) * ttbar_muPlus;
	// fP_ttbarDL_eMinus  = (1.0 - ttbarSL_vs_ttbarDL_frac_el) * ttbar_eMinus;
	// fP_ttbarDL_muMinus = (1.0 - ttbarSL_vs_ttbarDL_frac_mu) * ttbar_muMinus;
	
    } else {
        Warning(__FUNCTION__, "fApplyDiLeptonVeto was false, but a priori "
                "probabilities for run 2 are not available in this setting. "
                "Using old parameters from 8 TeV.");
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

    // Discriminant Plots
    // fHistsDisc = new AtlHistogramTool("MemDisc", "MEM discriminant histograms");
    // AddTool(fHistsDisc);
    // BookHistogramsMemDisc(fHistsDisc);
    
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

    fHist_llh_wjets_ratio = new TH1F("wjetsProb", "W+jets likelihood ratio", fgNBins_sChannelRatio, 0.0, 1.0);
    fHist_llh_wjets_ratio->SetXTitle("P(W+jets|X)");
    fHist_llh_wjets_ratio->SetYTitle("Number of Entries");

    fHist_llh_ttbar_ratio = new TH1F("ttbarProb", "ttbar likelihood ratio", fgNBins_sChannelRatio, 0.0, 1.0);
    fHist_llh_ttbar_ratio->SetXTitle("P(ttbar|X)");
    fHist_llh_ttbar_ratio->SetYTitle("Number of Entries");

    // there were a lot more plots that are now probably unnecessary
    // see AtlSgTop_sChannelMemDisc.cxx

    gDirectory->mkdir("shapes");
    gDirectory->cd("shapes");

    gDirectory->cd("..");

    gDirectory->mkdir("ManyBins");
    gDirectory->cd("ManyBins");

    BuildCtrlPlots();
    // s-channel ratio all leptons, all charge
    fHist_llh_sChannel_ratio_raw = new TH1F("sChannelProb_raw", "s-channel likelihood ratio", 1, 0.0, 1.0); //20000
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

    fHist_llh_ttbar_ratio_raw = new TH1F("ttbarProb_raw", "ttbar likelihood ratio", 20000, 0.0, 1.0);
    fHist_llh_ttbar_ratio_raw->SetXTitle("P(W+jets|X)");
    fHist_llh_ttbar_ratio_raw->SetYTitle("Number of Entries");

    gDirectory->cd("..");

    gDirectory->mkdir("BinningStudy");
    gDirectory->cd("BinningStudy");

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

    // MEM Likelihoods
    fHistsLlh         = new AtlHistogramTool("MemLlh", "MEM likelihood histograms");
    fHistsLlhMu       = new AtlHistogramTool("MemLlhMu", "MEM likelihood histograms mu");
    fHistsLlhE        = new AtlHistogramTool("MemLlhE", "MEM likelihood histograms e");
    AddTool(fHistsLlh);
    AddTool(fHistsLlhMu);
    AddTool(fHistsLlhE);
    BookHistogramsMemLogLikelihood(fHistsLlh);
    //BookHistogramsMemLogLikelihood(fHistsLlh, "PositiveCharge");
    //BookHistogramsMemLogLikelihood(fHistsLlh, "NegativeCharge");
    BookHistogramsMemLogLikelihood(fHistsLlhMu);
    //BookHistogramsMemLogLikelihood(fHistsLlhMu, "PositiveCharge");
    //BookHistogramsMemLogLikelihood(fHistsLlhMu, "NegativeCharge");
    BookHistogramsMemLogLikelihood(fHistsLlhE);
    //BookHistogramsMemLogLikelihood(fHistsLlhE, "PositiveCharge");
    //BookHistogramsMemLogLikelihood(fHistsLlhE, "NegativeCharge");

}

void AtlSgTop_combinationMemDiscR2::BuildCtrlPlots(){

	fHist_jet_pt_raw = new TH1F("Leading jet_pt_raw", "Leading jet p_{T}", 20000, 0.0, 500);
	fHist_jet_pt_raw->SetXTitle("Leading jet p_{T}");
	fHist_jet_pt_raw->SetYTitle("Number of Entries");

	fHist_jet_eta_raw = new TH1F("Leading jet_eta_raw", "Leading jet #eta", 20000, -5, 5);
	fHist_jet_eta_raw->SetXTitle("Leading jet #eta");
	fHist_jet_eta_raw->SetYTitle("Number of Entries");

	fHist_subjet_eta_raw = new TH1F("Sub-Leading jet_eta_raw", "Sub-Leading jet #eta", 20000, -5, 5);
	fHist_subjet_eta_raw->SetXTitle("Sub-Leading jet #eta");
	fHist_subjet_eta_raw->SetYTitle("Number of Entries");

	fHist_subjet_pt_raw = new TH1F("Sub-Leading jet_pt_raw", "Sub-Leading jet p_{T}", 20000, 0.0, 500);
	fHist_subjet_pt_raw->SetXTitle("Sub-Leading jet p_{T}");
	fHist_subjet_pt_raw->SetYTitle("Number of Entries");



	fHist_met_raw = new TH1F("MET", "MET", 20000, 0.0, 500);
	fHist_met_raw->SetXTitle("E_{T}^{Miss}");
	fHist_met_raw->SetYTitle("Number of Entries");


	fHist_mtw_raw = new TH1F("MTW", "MTW", 20000, 0.0, 500);
	fHist_mtw_raw->SetXTitle("M_{T}^{W}");
	fHist_mtw_raw->SetYTitle("Number of Entries");

	fHist_lepton_pt_raw = new TH1F("Lepton_pt", "Lepton p_{T}", 20000, 0.0, 500);
	fHist_lepton_pt_raw->SetXTitle("Lepton p_{T}");
	fHist_lepton_pt_raw->SetYTitle("Number of Entries");

	fHist_lepton_eta_raw = new TH1F("Lepton_eta", "Lepton #eta", 20000, -5, 5); //was 0,500
	fHist_lepton_eta_raw->SetXTitle("Lepton #eta");
	fHist_lepton_eta_raw->SetYTitle("Number of Entries");

	fHist_dR_subjLep_raw = new TH1F("dR subjLep", "DR Sub-Leading Jet Lepton", 20000, 0.0, 4);
	fHist_dR_subjLep_raw->SetXTitle("#Delta R(Sub-Leading Jet,Lepton)");
	fHist_dR_subjLep_raw->SetYTitle("Number of Entries");

	fHist_dR_ljLep_raw = new TH1F("dR ljLep", "DR Leading Jet Lepton", 20000, 0.0, 4);
	fHist_dR_ljLep_raw->SetXTitle("#Delta R(Leading Jet,Lepton)");
	fHist_dR_ljLep_raw->SetYTitle("Number of Entries");

	fHist_dR_ljsub_raw = new TH1F("dR ljsub", "DR Leading Jet subLeading", 20000, 0.0, 4);
	fHist_dR_ljsub_raw->SetXTitle("#Delta R(Leading Jet,Sub-Leading Jet)");
	fHist_dR_ljsub_raw->SetYTitle("Number of Entries");




	return;
}

void AtlSgTop_combinationMemDiscR2::FillCtrlPlots(){
	AtlJet::EType type = AtlJet::EType::kDL1r;
	TClonesArray* Jets = fEvent->GetJets(type);
	AtlJet *Jetlead     = (AtlJet*) Jets->At(0);
	AtlJet *Jet2ndlead  = (AtlJet*) Jets->At(1);
	fHist_jet_pt_raw->Fill(Jetlead->Pt(),GetTagEvtWeight());
	fHist_jet_eta_raw->Fill(Jetlead->Eta(),GetTagEvtWeight());
	fHist_subjet_eta_raw->Fill(Jet2ndlead->Eta(),GetTagEvtWeight()); //was Pt()
	fHist_subjet_pt_raw->Fill(Jet2ndlead->Pt(),GetTagEvtWeight());

	Float_t MET = fEvent->GetEnergySum()->MissingEt_Mag();
	fHist_met_raw->Fill(MET,GetTagEvtWeight());

	Float_t mtw = fEvent->W_Mperp(fLepton);
	fHist_mtw_raw->Fill(mtw,GetTagEvtWeight());

	fHist_lepton_pt_raw->Fill(fLepton->Pt(),GetTagEvtWeight());
	fHist_lepton_eta_raw->Fill(fLepton->Eta(),GetTagEvtWeight());


	fHist_dR_subjLep_raw->Fill(fLepton->DeltaR(Jet2ndlead),GetTagEvtWeight());
	fHist_dR_ljLep_raw->Fill(fLepton->DeltaR(Jetlead),GetTagEvtWeight());
	fHist_dR_ljsub_raw->Fill(Jetlead->DeltaR(Jet2ndlead),GetTagEvtWeight());;
	return;
}
//____________________________________________________________________

void AtlSgTop_combinationMemDiscR2::BookHistogramsMemLogLikelihood(AtlHistogramTool *htool,
                                                                const char *subdir) {
    //
    // Book log likelihood histograms
    //
    TString subdirectory(subdir);

    // make sure the subdirectory ends with a slash
    if ( !subdirectory.IsNull() && !subdirectory.EndsWith("/") ) {
        subdirectory.Append("/") ;
    }

    // s-channel
    htool->Add(Form("%slogllh_sChannel2j", subdirectory.Data()),
               "s-channel likelihood 2j", 50, -19., 1.,
               "Log_{10}(L_{s-channel 2j})", "Number of Entries");
    htool->Add(Form("%slogllh_sChannel3j", subdirectory.Data()),
               "s-channel likelihood 3j", 50, -19., 1.,
               "Log_{10}(L_{s-channel 3j})", "Number of Entries");

    // t-channel
    htool->Add(Form("%slogllh_tChannel4FS", subdirectory.Data()),
               "t-channel likelihood", 50, -19., 1.,
               "Log_{10}(L_{t-channel})", "Number of Entries");

    // ttbar
    htool->Add(Form("%slogllh_ttbarSL", subdirectory.Data()),
               "ttbar SL likelihood", 50, -19., 1.,
               "Log_{10}(L_{t#bar{t}, SL})", "Number of Entries");
    htool->Add(Form("%slogllh_ttbarSL_ttbarVR", subdirectory.Data()),
               "ttbar SL likelihood in ttbar VR", 50, -70., -45.,
               "Log_{10}(L_{t#bar{t}, SL})", "Number of Entries");
    htool->Add(Form("%slogllh_ttbarDL", subdirectory.Data()),
               "ttbar DL likelihood", 50., -19, 1.,
               "Log_{10}(L_{t#bar{t}, DL})", "Number of Entries");
    htool->Add(Form("%slogllh_ttbarDL_wjetsVR", subdirectory.Data()),
               "ttbar SL likelihood in wjets VR", 50, -35., -20.,
               "Log_{10}(L_{t#bar{t}, DL})", "Number of Entries");
    htool->Add(Form("%slogllh_ttbarDL_ttbarVR", subdirectory.Data()),
               "ttbar SL likelihood in ttbar VR", 50, -70., -45.,
               "Log_{10}(L_{t#bar{t}, DL})", "Number of Entries");

    // W+jets
    htool->Add(Form("%slogllh_Wjj", subdirectory.Data()),
               "Wjj likelihood", 50, -19., 1.,
               "Log_{10}(L_{Wjj})", "Number of Entries");
    htool->Add(Form("%slogllh_Wcj", subdirectory.Data()),
               "Wcj likelihood", 50, -19., 1.,
               "Log_{10}(L_{Wcj})", "Number of Entries");
    htool->Add(Form("%slogllh_Wbb", subdirectory.Data()),
               "Wbb likelihood", 50, -19., 1.,
               "Log_{10}(L_{Wb#bar{b}})", "Number of Entries");
    
}

//____________________________________________________________________

void AtlSgTop_combinationMemDiscR2::FillHistogramsMemLogLikelihood(AtlHistogramTool *htool,
                                                                Double_t W,
                                                                const char *subdir) {
    //
    // Fill log likelihood histograms
    //
    TString subdirectory(subdir);

    // make sure the subdirectory ends with a slash
    if ( !subdirectory.IsNull() && !subdirectory.EndsWith("/") ) {
        subdirectory.Append("/") ;
    }

    // get likelihoods from reader
    Double_t llh_sChannel2j  = GetEvtReader()->GetLLh_sChannel2j();
    Double_t llh_sChannel3j  = GetEvtReader()->GetLLh_sChannel3j();
    Double_t llh_tChannel4FS = GetEvtReader()->GetLLh_tChannel4FS();
    Double_t llh_ttbarSL     = GetEvtReader()->GetLLh_ttbarSL();
    Double_t llh_ttbarDL     = GetEvtReader()->GetLLh_ttbarDL();
    Double_t llh_Wjj         = GetEvtReader()->GetLLh_Wjj();
    Double_t llh_Wcj         = GetEvtReader()->GetLLh_Wcj();
    Double_t llh_Wbb         = GetEvtReader()->GetLLh_Wbb();

    // fill histograms
    htool->Fill(Form("%slogllh_sChannel2j", subdirectory.Data()),
                TMath::Log10(llh_sChannel2j), W);
    htool->Fill(Form("%slogllh_sChannel3j", subdirectory.Data()),
                TMath::Log10(llh_sChannel3j), W);
    htool->Fill(Form("%slogllh_tChannel4FS", subdirectory.Data()),
                TMath::Log10(llh_tChannel4FS), W);
    htool->Fill(Form("%slogllh_ttbarSL", subdirectory.Data()),
                TMath::Log10(llh_ttbarSL), W);
    htool->Fill(Form("%slogllh_ttbarSL_ttbarVR", subdirectory.Data()),
                TMath::Log10(llh_ttbarSL), W);
    htool->Fill(Form("%slogllh_ttbarDL", subdirectory.Data()),
                TMath::Log10(llh_ttbarDL), W);
    htool->Fill(Form("%slogllh_ttbarDL_wjetsVR", subdirectory.Data()),
                TMath::Log10(llh_ttbarDL), W);
    htool->Fill(Form("%slogllh_ttbarDL_ttbarVR", subdirectory.Data()),
                TMath::Log10(llh_ttbarDL), W);
    htool->Fill(Form("%slogllh_Wjj", subdirectory.Data()),
                TMath::Log10(llh_Wjj), W);
    htool->Fill(Form("%slogllh_Wcj", subdirectory.Data()),
                TMath::Log10(llh_Wcj), W);
    htool->Fill(Form("%slogllh_Wbb", subdirectory.Data()),
                TMath::Log10(llh_Wbb), W);
}
    
//____________________________________________________________________

Bool_t AtlSgTop_combinationMemDiscR2::ProcessPreCut() {
    //
    // Event pre-selection - same selection as in AtlSgTop_sChannelAnalysis
    //
    fCutFlowTool->Fill("PreCut", GetPreTagEvtWeight());

    // Require exactly one lepton and save it in fLepton
    if ( !fQCDMMToolIsOn ) {
	// exactly one tight lepton without QCD
	if ( fLeptons->GetEntries() != 1 ) {
	    return kFALSE;
	} else {
            fLepton = dynamic_cast<HepParticle*>(fLeptons->At(0));
	}
    } else {
	// exactly one tight or loose lepton in case of QCD
	if ( fLeptons->GetEntries() + fLooseLeptons->GetEntries() != 1) {
	    return kFALSE;
	} else if ( fLeptons->GetEntries() == 1 ) {
            fLepton = dynamic_cast<HepParticle*>(fLeptons->At(0));
	} else {
            fLepton = dynamic_cast<HepParticle*>(fLooseLeptons->At(0));
	}
    } // end if not QCDMM
    fCutFlowTool->Fill("1Lepton", GetPreTagEvtWeight());

    // Missing Et
    Float_t met = fEvent->GetEnergySum()->MissingEt_Mag();
    if ( (met < fMET_min) || (met > fMET_max) ) return kFALSE;
    fCutFlowTool->Fill("METpre", GetPreTagEvtWeight());
    
    // Transverse W mass
    Float_t mtw = fEvent->W_Mperp(fLepton);
    if ( (mtw < fMtW_minPre) || (mtw > fMtW_maxPre) ) return kFALSE;
    fCutFlowTool->Fill("MtWpre", GetPreTagEvtWeight());
    
    // Accept event
    return kTRUE;
}

//____________________________________________________________________

Bool_t AtlSgTop_combinationMemDiscR2::ProcessCut() {
    //
    // Event selection - same as sChannelAnalysis without histogram filling
    //

    // ===================
    // Check llh variables
    // ===================
    if(fOnlyOdd){
    	if (fEvent->EventNr() % 2 == 0){
    		return kFALSE;
    	}
    }
    fCutFlowTool->Fill("EventNumberCut", GetPreTagEvtWeight());
    
    fNNegativeLlh = 0;
    if ( GetEvtReader()->GetLLh_sChannel2j() < 0 ) fNNegativeLlh++;
    if ( GetEvtReader()->GetLLh_sChannel3j() < 0 ) fNNegativeLlh++;
    if ( GetEvtReader()->GetLLh_tChannel4FS() < 0 ) fNNegativeLlh++;
    if ( GetEvtReader()->GetLLh_ttbarSL() < 0 ) fNNegativeLlh++;
    if ( GetEvtReader()->GetLLh_ttbarDL() < 0 ) fNNegativeLlh++;
    if ( GetEvtReader()->GetLLh_Wjj() < 0 ) fNNegativeLlh++;
    if ( GetEvtReader()->GetLLh_Wcj() < 0 ) fNNegativeLlh++;
    if ( GetEvtReader()->GetLLh_Wbb() < 0 ) fNNegativeLlh++;
    if ( !fProcessNegativeLlh ) {
        if ( fNNegativeLlh > 0 ) {
            Warning(__FUNCTION__, "There were %d negative likelihood(s) in the event. Skip event!", fNNegativeLlh);
            return kFALSE;
        }
        fCutFlowTool->Fill("NegLlh", GetPreTagEvtWeight());
    }

    // ===========
    // No. of jets
    // ===========

    // Don't allow additional jets outside the given pt-eta range
    if ( fAdditionalJetVeto ) {
        if ( fEvent->GetN_Jets(fObjTool->fJetsAuthor) > fJets->GetEntries() )
            return kFALSE;
    }
    fCutFlowTool->Fill("AddJetVeto", GetTagEvtWeight());

    // Apply analysis Pt cut
    TIter next_jet(fJets);
    AtlJet *jet = 0;
    while ( (jet = dynamic_cast<AtlJet*>(next_jet())) ) {
	if ( (jet->Pt() < fPt_Jet_min) || (jet->Pt() > fPt_Jet_max) )
	    return kFALSE;
    }
    fCutFlowTool->Fill("JetPt", GetTagEvtWeight());

    // =========
    // B-tagging
    // =========

    // No. of b-tagged jets
    if ( fBJets->GetEntries() < fNBTags_min
	 || fBJets->GetEntries() > fNBTags_max) return kFALSE;
    fCutFlowTool->Fill("NBTag", GetTagEvtWeight());

    // Require the leading jet to be b-tagged ?
    if ( fLeadingJetIsBTagged ) {
        if ( fBJets->At(0) != fJets->At(0) ) return kFALSE;
        fCutFlowTool->Fill("LeadingJIsBTagged", GetTagEvtWeight());
    }

    // B-tag veto (for ctrl region)
    // only veto if the tagger is set
    if ( fVetoBTagger != AtlBTag::kInvalidTagger ) {
        TIter next_bjet(fBJets);
        AtlJet *bjet = 0;
        Bool_t IsVeto = kTRUE;
        while ( (bjet = dynamic_cast<AtlJet*>(next_bjet())) ) {
            IsVeto &= ( bjet->GetTag(fVetoBTagger)->GetWeight()
                        > fVetoBTagWeight_min );
        }
        if ( IsVeto ) return kFALSE;
        fCutFlowTool->Fill("BTagVeto", GetTagEvtWeight());
    }

    // ===============
    // Final selection
    // ===============

    // Final transverse W mass cut
    Float_t mtw = fEvent->W_Mperp(fLepton);
    if ( mtw < fMtW_min ) return kFALSE;
    fCutFlowTool->Fill("MtWMin", GetTagEvtWeight());
    if ( mtw > fMtW_max ) return kFALSE;
    fCutFlowTool->Fill("MtWMax", GetTagEvtWeight());

    // Cut on leading jet pt
    AtlJet *Jetlead = dynamic_cast<AtlJet*>(fJets->At(0));
    if ( (Jetlead->Pt() < fPtJetlead_min) || (Jetlead->Pt() > fPtJetlead_max) )
	return kFALSE;
    fCutFlowTool->Fill("LeadingJPt", GetTagEvtWeight());
    
    // Apply ttbar di-lepton veto (run 2 only)
    if ( fApplyDileptonVeto ) {
	Int_t N_Electron = fEvent->GetN_Electrons();
	Int_t N_Muon = fEvent->GetN_Muons();
	if ( N_Electron + N_Muon > 1 ) return kFALSE;
        fCutFlowTool->Fill("DiLepVeto", GetTagEvtWeight());
    }
    std::cout<<"WEIGHT:"<<GetTagEvtWeight()<<std::endl;
    return kTRUE;
}

//____________________________________________________________________

void AtlSgTop_combinationMemDiscR2::FillHistograms() {
    //
    // Fill histograms
    //
    // This method will be called only for events surviving the
    // ProcessCut() routine
    //

    // S.K.: change this to use histogram tools

    HepParticle *lepton        = fLepton;

    Float_t      lepCharge     = lepton->Charge();
    Bool_t       lepIsElectron = lepton->IsElectron();
    Bool_t       lepIsMuon     = lepton->IsMuon();

    Double_t llh_sChannel2j  = GetEvtReader()->GetLLh_sChannel2j();
    Double_t llh_sChannel3j  = GetEvtReader()->GetLLh_sChannel3j();
    Double_t llh_tChannel4FS = GetEvtReader()->GetLLh_tChannel4FS();
    Double_t llh_ttbarSL     = GetEvtReader()->GetLLh_ttbarSL();
    Double_t llh_ttbarDL     = GetEvtReader()->GetLLh_ttbarDL();
    Double_t llh_Wjj         = GetEvtReader()->GetLLh_Wjj();
    Double_t llh_Wcj         = GetEvtReader()->GetLLh_Wcj();
    Double_t llh_Wbb         = GetEvtReader()->GetLLh_Wbb();

    Double_t p_sChannel2j  = 0.0;
    Double_t p_sChannel3j  = 0.0;
    Double_t p_tChannel4FS = 0.0;
    Double_t p_Wjj         = 0.0;
    Double_t p_Wbb         = 0.0;
    Double_t p_Wcj         = 0.0;
    Double_t p_ttbarSL     = 0.0;
    Double_t p_ttbarDL     = 0.0;

    // temporary solution without splitting mu/el/+/-: use only e+
    p_sChannel2j  = fP_sChannel2j_ePlus;
    p_sChannel3j  = fP_sChannel3j_ePlus;
    p_tChannel4FS = fP_tChannel4FS_ePlus;
    p_Wjj         = fP_Wjj_ePlus;
    p_Wbb         = fP_Wbb_ePlus;
    p_Wcj         = fP_Wcj_ePlus;
    p_ttbarSL     = fP_ttbarSL_ePlus;
    p_ttbarDL     = fP_ttbarDL_ePlus;

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

    // S.K.: check if any llh fails, (llh < 0)
    //   set prob. to 0 (to compute some discr. in ttbar regions), give warning
    // the check is performed in ProcessCut()
    if ( fNNegativeLlh > 0 ) {
        if ( llh_sChannel2j < 0 ) { p_sChannel2j = 0; }
        if ( llh_sChannel3j < 0 ) { p_sChannel3j = 0; }
        if ( llh_tChannel4FS < 0 ) { p_tChannel4FS = 0; }
        if ( llh_Wjj < 0 ) { p_Wjj = 0; }
        if ( llh_Wbb < 0 ) { p_Wbb = 0; }
        if ( llh_Wcj < 0 ) { p_Wcj = 0; }
        if ( llh_ttbarSL < 0 ) { p_ttbarSL = 0; }
        if ( llh_ttbarDL < 0 ) { p_ttbarDL = 0; }
        Warning(__FUNCTION__, "There were %d negative likelihood(s) in the event, still using event!", fNNegativeLlh);
    }

    // sum over a priori prob times likelihood
    Double_t sig = p_sChannel2j * llh_sChannel2j + p_sChannel3j * llh_sChannel3j;
    Double_t bkg = p_tChannel4FS * llh_tChannel4FS +
    	p_Wjj * llh_Wjj +
    	p_Wbb * llh_Wbb +
    	p_Wcj * llh_Wcj +
    	p_ttbarSL * llh_ttbarSL +
    	p_ttbarDL * llh_ttbarDL;
    Double_t wjetsSum = p_Wjj * llh_Wjj + p_Wbb * llh_Wbb + p_Wcj * llh_Wcj;
    Double_t ttbarSum = p_ttbarSL * llh_ttbarSL + p_ttbarDL * llh_ttbarDL;

    if ( sig + bkg == 0. ) {
        Warning(__FUNCTION__, "sig+bkg=%g, skip filling event in histograms",
                sig+bkg);
        return;
    }
    fCutFlowTool->Fill("sig+bkg", GetTagEvtWeight());

    // s-channel ratio = prob = MEM discriminant
    Double_t sChannelRatio = sig / (sig + bkg);
    if ( !  (sChannelRatio >= 0. && sChannelRatio <= 1.) ) {
        Error(__FUNCTION__, "sChannelRatio invalid: sig=%g, bkg=%g, sChannelRatio=%g",
              sig, bkg, sChannelRatio);
        return;
    }
    fCutFlowTool->Fill("sch_ratio", GetTagEvtWeight());

    Double_t wjetsRatio            = wjetsSum / (sig + bkg);
    Double_t wjetsRatio_reBinned   = TMath::Power(wjetsRatio, 2.0/3.0); // Rebinning by simple mapping
    // Double_t wjetsRatio_binNSpread = 1.0 - TMath::Power(1.0 - wjetsRatio, 0.5);
    // Double_t wjetsMaxCut         = 0.8; // Can use this value to reduce the impact of W+jets events - refers to the rebinned ratio
    if ( ! (wjetsRatio >= 0. && wjetsRatio <= 1.) ) {
        Error(__FUNCTION__, "wjetsRatio invalid: wjetsSum=%g, sig+bkg=%g, wjetsRatio=%g",
              wjetsSum, sig+bkg, wjetsRatio);
        return;
    }
    fCutFlowTool->Fill("wjets_ratio", GetTagEvtWeight());

    Double_t ttbarRatio          = ttbarSum / (sig + bkg);
    Double_t ttbarRatio_reBinned = TMath::Power(ttbarRatio, 2.0/3.0); // Rebinning by simple mapping
    // Double_t ttbarRatio_binNSpread = 1.0 - TMath::Power(1.0 - ttbarRatio, 0.5);
    if ( ! (ttbarRatio >= 0. && ttbarRatio <= 1.) ) {
        Error(__FUNCTION__, "ttbarRatio invalid: ttbarSum=%g, sig+bkg=%g, ttbarRatio=%g",
              ttbarSum, sig+bkg, ttbarRatio);
        return;
    }
    fCutFlowTool->Fill("ttbar_ratio", GetTagEvtWeight());

    // find which bin the s-channel prob. fits into (number)
    Int_t bin_sChannelRatio = 1 + TMath::BinarySearch(fgNBins_sChannelRatio,
						      fgBinEdges_schannelRatio,
						      sChannelRatio);
    if ( bin_sChannelRatio == 0 )
    {
        // S.K. debug; this should not happen, right?
        if ( gDebug ) Warning(__FUNCTION__, "bin=0; inc by 1");
	++bin_sChannelRatio; // Don't use underflow bin
    }

    // use regular spaced bins
    Double_t binWidth_sChannelRatio   = 1.0 / static_cast<Double_t>(fgNBins_sChannelRatio);
    Double_t sChannelRatio_reBinned   = binWidth_sChannelRatio * (static_cast<Double_t>(bin_sChannelRatio) - 0.5);
    // Double_t sChannelRatio_bin1Spread = TMath::Power(sChannelRatio, 0.25); // S.K.??

    // fill histograms
    if(GetTagEvtWeight() < 0 ) std::cout<<"HALLO "<<sChannelRatio<<"  "<<GetTagEvtWeight()<<std::endl;
    fHist_llh_sChannel_ratio_raw->Fill(sChannelRatio, GetTagEvtWeight());
    fHist_llh_sChannel_ratio->Fill(sChannelRatio_reBinned, GetTagEvtWeight());
    if ( lepton->Charge() > 0 )
	fHist_llh_sChannel_ratio_lplus->Fill(sChannelRatio_reBinned, GetTagEvtWeight());
    else
	fHist_llh_sChannel_ratio_lminus->Fill(sChannelRatio_reBinned, GetTagEvtWeight());
    fHist_llh_ttbar_ratio_raw->Fill(ttbarRatio, GetTagEvtWeight());
    fHist_llh_ttbar_ratio->Fill(ttbarRatio_reBinned, GetTagEvtWeight());
    fHist_llh_wjets_ratio_raw->Fill(wjetsRatio, GetTagEvtWeight());
    fHist_llh_wjets_ratio->Fill(wjetsRatio_reBinned, GetTagEvtWeight());

    // 2D Histograms:
    fHist_2D_sChannel_vs_Wjets->Fill(sChannelRatio, wjetsRatio, GetTagEvtWeight());
    fHist_2D_sChannel_vs_ttbar->Fill(sChannelRatio, ttbarRatio, GetTagEvtWeight());
    // Double_t SumLLH = sig+bkg;
    // fHist_2D_sChannel_vs_SumLLH->Fill(sChannelRatio, TMath::Log10(SumLLH), GetTagEvtWeight());

    // Add discriminant(s) to output tree
    AtlEvtWriterMemR2 *evt_writer = dynamic_cast<AtlEvtWriterMemR2*>(fEvtWriter);
    evt_writer->SetMemDisc_sChannelRatio(sChannelRatio);
    evt_writer->SetMemDisc_wjetsRatio(wjetsRatio);
    evt_writer->SetMemDisc_ttbarRatio(ttbarRatio);

    // fill histogram tool histograms
    if ( lepIsElectron ) {
        FillHistogramsMemLogLikelihood(fHistsLlhE, GetTagEvtWeight());
        /*if ( lepCharge > 0. ) {
            FillHistogramsMemLogLikelihood(fHistsLlhE, GetTagEvtWeight(), "PositiveCharge");
        } else {
            FillHistogramsMemLogLikelihood(fHistsLlhE, GetTagEvtWeight(), "NegativeCharge");
        }*/
    } else {
        FillHistogramsMemLogLikelihood(fHistsLlhMu, GetTagEvtWeight());
        /*if ( lepCharge > 0. ) {
            FillHistogramsMemLogLikelihood(fHistsLlhMu, GetTagEvtWeight(), "PositiveCharge");
        } else {
            FillHistogramsMemLogLikelihood(fHistsLlhMu, GetTagEvtWeight(), "NegativeCharge");
        }*/
    }        
    FillHistogramsMemLogLikelihood(fHistsLlh, GetTagEvtWeight());
    /*if ( lepCharge > 0. ) {
        FillHistogramsMemLogLikelihood(fHistsLlh, GetTagEvtWeight(), "PositiveCharge");
    } else {
        FillHistogramsMemLogLikelihood(fHistsLlh, GetTagEvtWeight(), "NegativeCharge");
    }*/
    FillCtrlPlots();
}

//____________________________________________________________________

void AtlSgTop_combinationMemDiscR2::InitEvent() {
    //
    // Prepare event analysis
    //
}

//____________________________________________________________________

void AtlSgTop_combinationMemDiscR2::Print(Option_t */*option*/) const {
    //
    // Print user analysis configuration
    //
    cout << endl
	 << "================================================================" << endl
	 << "  Atlas Single-Top s-Channel Mem Discriminant Analysis Selector " << endl
	 << "================================================================" << endl
	 << endl;
    AtlSelector::Print();
    cout << "Event pre-selection:" << endl
	 << "  fMET_min             = " << fMET_min << " GeV" << endl
	 << "  fMET_max             = " << fMET_max << " GeV" << endl
	 << "  fMtW_minPre          = " << fMtW_minPre << " GeV" << endl
	 << "  fMtW_maxPre          = " << fMtW_maxPre << " GeV" << endl;
    cout << "Final event selection:" << endl
	 << "  fMtW_min             = " << fMtW_min << " GeV" << endl
	 << "  fMtW_max             = " << fMtW_max << " GeV" << endl
	 << "  fPt_Jet_min          = " << fPt_Jet_min << " GeV" << endl
	 << "  fPt_Jet_max          = " << fPt_Jet_max << " GeV" << endl
	 << "  fPtJetlead_min   = " << fPtJetlead_min << " GeV" << endl
	 << "  fPtJetlead_max   = " << fPtJetlead_max << " GeV" << endl
	 << "  fNBTags_min          = " << fNBTags_min << endl
	 << "  fNBTags_max          = " << fNBTags_max << endl
         << "  fAdditionalJetVeto   = " << (fAdditionalJetVeto ? "true" : "false") << endl
         << "  fApplyDileptonVeto   = " << (fApplyDileptonVeto ? "true" : "false") << endl
         << "  fLeadingJetIsBTagged = " << (fLeadingJetIsBTagged ? "true" : "false") << endl;
    if ( fVetoBTagger != AtlBTag::kInvalidTagger ) {
        cout << "  fVetoBTagger         = " << AtlBTag::GetTaggerName(fVetoBTagger) << endl
             << "  fVetoBTagWeight_min  = " << fVetoBTagWeight_min << endl;
    }
    cout << "  fProcessNegativeLlh  = " << (fProcessNegativeLlh ? "true" : "false") << endl
         << endl
	 << "================================================================" << endl
	 << endl;
}

//____________________________________________________________________

void AtlSgTop_combinationMemDiscR2::Terminate() {
    //
    // Terminate routine called at the end of event loop
    //    
	
    AtlSelector::Terminate();
}

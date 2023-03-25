//____________________________________________________________________
//
// Single-top s-Channel BDT Analysis
// 
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2013 (C) Soeren Stamm
//
#ifndef SINGLETOP_AtlSgTop_sChannelBDT
#include <AtlSgTop_sChannelBDT.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlSgTop_sChannelBDT);
#endif

//____________________________________________________________________

AtlSgTop_sChannelBDT::AtlSgTop_sChannelBDT(const char* OutputFile) :
    AtlSelector(OutputFile) {
    //
    // Default constructor
    //
    
    // Init
    fLeptons   = 0;
    fElectrons = 0;
    fMuons     = 0;
    fJets      = 0;
    fBJets     = 0;
    fObjTool   = 0;
    fSFTool    = 0;
    SetCutDefaults();
}

//____________________________________________________________________

AtlSgTop_sChannelBDT::~AtlSgTop_sChannelBDT() {
    //
    // Default destructor
    //
	delete fBinEdges_BDT;
}

//____________________________________________________________________

void AtlSgTop_sChannelBDT::Clear(Option_t *option) {
    //
    //  Clears the dynamically allocated objects
    //
}

//____________________________________________________________________

void AtlSgTop_sChannelBDT::Begin(TTree *tree) {
    //
    // Begin analysis job
    //

    // Create event writer
    fEvtWriter = 0;

    AtlSelector::Begin(tree);
}

//____________________________________________________________________

void AtlSgTop_sChannelBDT::SetBranchStatus() {
    //
    // Switch on/off branches in order to gain speed
    //

    // Switch off all branches by default
    fTree->SetBranchStatus("*", kFALSE);
}

//____________________________________________________________________

void AtlSgTop_sChannelBDT::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    fPrintEvent = kFALSE;
    fInputMode  = kD3PDSgTop_v2;
	
    fEvtReader  = new AtlEvtReaderMemBDT(this);
    Info("SetCutDefaults", "Input file type: MemBDT - Reader");
}

//____________________________________________________________________

void AtlSgTop_sChannelBDT::BookHistograms() {
    //
    // Book histograms
    //    

    BookHistogramsBDT();
    // BookHistogramsLikelihoodRatios();
    
    // Get ObjectSelection Tool + pointers to analysis objects
    fObjTool = 
	(AtlObjectsToolD3PDSgTop*)GetTool("AtlObjectsToolD3PDSgTop"
					  ,"",kTRUE);
    fElectrons = fObjTool->GetElectrons();
    fMuons     = fObjTool->GetMuons();
    fLeptons   = fObjTool->GetLeptons();
    fJets      = fObjTool->GetJets();
    fBJets     = fObjTool->GetBJets();

    // Provide tools with pointers to analysis objects
    fSFTool = 
	(AtlObjRecoScaleFactorTool*)GetTool("AtlObjRecoScaleFactorTool"
					    ,"",kTRUE);
    fSFTool->SetLeptons(fLeptons);
    fSFTool->SetJets(fJets);

    //
    // Instructions from $TMVAHOME/test/TMVAClassificationApplication.C
    //
    // --- Create the Reader object
    // fReader = new TMVA::Reader( "!Color:!Silent" );
    fReaderEl = new TMVA::Reader( "!Color:!Silent" );
    fReaderMu = new TMVA::Reader( "!Color:!Silent" );

    // Create a set of variables and declare them to the reader
    // - the variable names must corresponds in name and type to
    // those given in the weight file(s) that you use
    // TString llh_sum = "(llh_sChannel2j+llh_sChannel3j+llh_tChannel4FS+llh_ttbarSL+llh_Wjj+llh_Wbb)";   
    // fReader->AddVariable(Form("(llh_sChannel2j+llh_sChannel3j)/%s", llh_sum.Data()),
	// 		 &fBDTsChannelRatio);
    // fReader->AddVariable(Form("llh_tChannel4FS/%s", llh_sum.Data()),
	// 		 &fBDTtChannelRatio);
    // fReader->AddVariable(Form("llh_ttbarSL/%s", llh_sum.Data()),
	// 		 &fBDTttbarSLRatio);
    // fReader->AddVariable(Form("llh_Wjj/%s", llh_sum.Data()),
	// 		 &fBDTWjjRatio);
    // fReader->AddVariable(Form("llh_Wbb/%s", llh_sum.Data()),
	// 		 &fBDTWbbRatio);
	
	// for Grenoble BDT setup
	// ------------------------
	// fReader->AddVariable("bdt_DeltaPhiTopBJet2_BJet1", &fBDT_DeltaPhiTopBJet2_BJet1);
	// fReader->AddVariable("bdt_DeltaPhiTopBJet1_BJet2", &fBDT_DeltaPhiTopBJet1_BJet2);
	// fReader->AddVariable("bdt_SumEtLep_MET", &fBDT_SumEtLep_MET);
	// fReader->AddVariable("bdt_MET", &fBDT_MET);
	// fReader->AddVariable("bdt_MtW", &fBDT_MtW);
	// fReader->AddVariable("bdt_Pt_Lep", &fBDT_Pt_Lep);
	// fReader->AddVariable("bdt_DeltaEtaLep_BJet1", &fBDT_DeltaEtaLep_BJet1);
	// fReader->AddVariable("bdt_SumEtLep_MET_BJets", &fBDT_SumEtLep_MET_BJets);
	// fReader->AddVariable("bdt_DeltaEtaTopBJet2_BJet1", &fBDT_DeltaEtaTopBJet2_BJet1);
	// fReader->AddVariable("bdt_DeltaPhiBJet1_BJet2", &fBDT_DeltaPhiBJet1_BJet2);
	// fReader->AddVariable("bdt_DeltaPhiLep_MET", &fBDT_DeltaPhiLep_MET);
	// fReader->AddVariable("bdt_DeltaEtaLep_BJet2", &fBDT_DeltaEtaLep_BJet2);
	// fReader->AddVariable("bdt_DeltaEtaNu_BJetNonTop", &fBDT_DeltaEtaNu_BJetNonTop);
	// fReader->AddVariable("bdt_SumPtBJet1_BJet2", &fBDT_SumPtBJet1_BJet2);
	// fReader->AddVariable("bdt_CosLepW_WTopBJet1", &fBDT_CosLepW_WTopBJet1);
	// fReader->AddVariable("bdt_CosMET_BJet2", &fBDT_CosMET_BJet2);
	// fReader->AddVariable("bdt_MLep_BJet2", &fBDT_MLep_BJet2);
	// fReader->AddVariable("bdt_CosLepTopBJet2_TopBJet2CMS", &fBDT_CosLepTopBJet2_TopBJet2CMS);
	// fReader->AddVariable("bdt_CosLepTopBJet1_TopBJet1CMS", &fBDT_CosLepTopBJet1_TopBJet1CMS);
	
	// for HU Berlin BDT setup,
	// means BDT 16 Kin Var setup
	// --------------------------
	// fReader->AddVariable("bdt_MET", &fBDT_MET);
	// fReader->AddVariable("bdt_MtW", &fBDT_MtW);
	// fReader->AddVariable("bdt_Pt_Lep", &fBDT_Pt_Lep);
	// fReader->AddVariable("bdt_DeltaPhiTopBJet2_BJet1", &fBDT_DeltaPhiTopBJet2_BJet1);
	// fReader->AddVariable("bdt_DeltaPhiTopBJet1_BJet2", &fBDT_DeltaPhiTopBJet1_BJet2);
	// fReader->AddVariable("bdt_SumEtLep_MET", &fBDT_SumEtLep_MET);
	// fReader->AddVariable("bdt_SumEtLep_MET_BJets", &fBDT_SumEtLep_MET_BJets);
	// fReader->AddVariable("bdt_SumPt", &fBDT_SumPt);
	// fReader->AddVariable("bdt_DeltaEtaLep_BJet1", &fBDT_DeltaEtaLep_BJet1);
	// fReader->AddVariable("bdt_DeltaEtaLep_BJet2", &fBDT_DeltaEtaLep_BJet2);
	// fReader->AddVariable("bdt_DeltaEtaTopBJet2_BJet1", &fBDT_DeltaEtaTopBJet2_BJet1);
	// fReader->AddVariable("bdt_DeltaPhiBJet1_BJet2", &fBDT_DeltaPhiBJet1_BJet2);
	// fReader->AddVariable("bdt_DeltaPhiLep_MET", &fBDT_DeltaPhiLep_MET);
	// fReader->AddVariable("bdt_CosLepW_WTopBJet1", &fBDT_CosLepW_WTopBJet1);
	// fReader->AddVariable("bdt_CosLepTopBJet1_TopBJet1CMS", &fBDT_CosLepTopBJet1_TopBJet1CMS);
	// fReader->AddVariable("bdt_CosLepTopBJet2_TopBJet2CMS", &fBDT_CosLepTopBJet2_TopBJet2CMS);
	
	TMVA::Reader* readerThis = 0;
	for (Int_t iReader = 0; iReader <= 1; ++iReader) {
		if ( iReader == 0 )
			readerThis = fReaderEl;
		else
			readerThis = fReaderMu;
		readerThis->AddVariable("bdt_MtW", &fBDT_MtW);
		readerThis->AddVariable("bdt_DeltaPhiTopBJet2_BJet1", &fBDT_DeltaPhiTopBJet2_BJet1);
		readerThis->AddVariable("bdt_DeltaPhiTopBJet1_BJet2", &fBDT_DeltaPhiTopBJet1_BJet2);
		readerThis->AddVariable("bdt_SumEtLep_MET", &fBDT_SumEtLep_MET);
		readerThis->AddVariable("bdt_MET", &fBDT_MET);
		readerThis->AddVariable("bdt_SumEtLep_MET_BJets", &fBDT_SumEtLep_MET_BJets);
		readerThis->AddVariable("bdt_Pt_Lep", &fBDT_Pt_Lep);
		readerThis->AddVariable("bdt_SumPt", &fBDT_SumPt);
		readerThis->AddVariable("bdt_DeltaEtaLep_BJet1", &fBDT_DeltaEtaLep_BJet1);
		readerThis->AddVariable("bdt_DeltaPhiBJet1_BJet2", &fBDT_DeltaPhiBJet1_BJet2);
		readerThis->AddVariable("bdt_DeltaEtaLep_BJet2", &fBDT_DeltaEtaLep_BJet2);
		readerThis->AddVariable("bdt_CosLepW_WTopBJet1", &fBDT_CosLepW_WTopBJet1);
		readerThis->AddVariable("bdt_DeltaPhiLep_MET", &fBDT_DeltaPhiLep_MET);
		readerThis->AddVariable("bdt_DeltaEtaTopBJet2_BJet1", &fBDT_DeltaEtaTopBJet2_BJet1);
		readerThis->AddVariable("bdt_CosLepTopBJet2_TopBJet2CMS", &fBDT_CosLepTopBJet2_TopBJet2CMS);
		readerThis->AddVariable("bdt_CosLepTopBJet1_TopBJet1CMS", &fBDT_CosLepTopBJet1_TopBJet1CMS);
	}
	
	
    // --- Book the MVA methods
	// for Grenoble BDT setup
    // TString dir    = "/users/eeh/rieck/atlas/APlusPlus070115/APlusPlus/libSingleTop/tasks/";
    // TString prefix = "Weight_schan_2tag_veto_2jets_Leptons_BDTG";
	// for HU Berlin BDT setup
    // TString dir    = "/users/eeh/rieck/atlas/APlusPlus110714/libSingleTop/bdt/bdt_final_MEM/weights/";
    // TString prefix = "TMVAClassification";
	TString dir    = "/users/eeh/rieck/atlas/APlusPlus110714/libSingleTop/bdt/bdt_v14/weights/";
	TString prefix = "TMVAClassification";
	
    // Book method(s)
	// for Grenoble BDT setup
    // TString methodName = "BDT_Grenoble";
    // TString weightfile = dir + prefix + TString(".grenoble.ntupleHU.final.weights.xml");
	// for HU Berlin BDT setup
    // TString methodName = "BDT 16 Kinematic Variables";
    // TString weightfile = dir + prefix + "_" + "BDT" + TString("_16_KinVar.weights.xml");
    // fReader->BookMVA( methodName, weightfile );
	TString methodName   = "BDT 16 Kinematic Variables";
	// nominal BDT setup	
	TString weightFileEl = dir + prefix + TString("_BDT_el_v14_300__0_60__3__25_00__0_80__0_80.weights.xml");
	TString weightFileMu = dir + prefix + TString("_BDT_mu_v14_300__0_80__2__25_00__0_50__0_80.weights.xml");
	
    fReaderEl->BookMVA( methodName, weightFileEl );
    fReaderMu->BookMVA( methodName, weightFileMu );
}

//____________________________________________________________________

void AtlSgTop_sChannelBDT::BookHistogramsLikelihoodRatios() {
    //
    // Book likelihood histograms
    //    

    // Likelihood Ratios
    gDirectory->mkdir("LikelihoodRatios");
    gDirectory->cd("LikelihoodRatios");

    fHist_llh_ttbarSL_ratio = new TH1F("h_llh_ttbarSL_ratio", "Likelihood ratio L_{t#bar{t}}",
				       35., 0., 1.);
    fHist_llh_ttbarSL_ratio->SetXTitle("L_{t#bar{t}}/(L_{S}+L_{B})");
    fHist_llh_ttbarSL_ratio->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
}

//____________________________________________________________________

void AtlSgTop_sChannelBDT::BookHistogramsBDT() {
    //
    // Book BDT histogram
    //    
    gDirectory->mkdir("BDTresponse");
    gDirectory->cd("BDTresponse");
    
	// optimized binning of BDT response
	fNBins_BDT = 30;
	fBinEdges_BDT = new Double_t[fNBins_BDT + 1];
	fBinEdges_BDT[0]  = -1.0;
	fBinEdges_BDT[1]  = -0.88;
	fBinEdges_BDT[2]  = -0.79;
	fBinEdges_BDT[3] = -0.706;
	fBinEdges_BDT[4] = -0.626;
	fBinEdges_BDT[5] = -0.546;
	fBinEdges_BDT[6] = -0.474;
	fBinEdges_BDT[7] = -0.402;
	fBinEdges_BDT[8] = -0.336;
	fBinEdges_BDT[9] = -0.27;
	fBinEdges_BDT[10] = -0.208;
	fBinEdges_BDT[11] = -0.15;
	fBinEdges_BDT[12] = -0.094;
	fBinEdges_BDT[13] = -0.04;
	fBinEdges_BDT[14] = 0.012;
	fBinEdges_BDT[15] = 0.062;
	fBinEdges_BDT[16] = 0.112;
	fBinEdges_BDT[17] = 0.158;
	fBinEdges_BDT[18] = 0.204;
	fBinEdges_BDT[19] = 0.246;
	fBinEdges_BDT[20] = 0.288;
	fBinEdges_BDT[21] = 0.33;
	fBinEdges_BDT[22] = 0.37;
	fBinEdges_BDT[23] = 0.41;
	fBinEdges_BDT[24] = 0.448;
	fBinEdges_BDT[25] = 0.488;
	fBinEdges_BDT[26] = 0.528;
	fBinEdges_BDT[27] = 0.57;
	fBinEdges_BDT[28] = 0.614;
	fBinEdges_BDT[29] = 0.666;	
	fBinEdges_BDT[30] = 1.0;
	
	Int_t nBinsBDT_raw = 30;
	
    fHist_BDT = new TH1F("h_BDT", "BDT response",
						 nBinsBDT_raw, -1., 1.);
    fHist_BDT->SetXTitle("BDT response");
    fHist_BDT->SetYTitle("Number of Entries");
	
    fHist_BDT_even = new TH1F("h_BDT_even", "BDT response, even events",
							  nBinsBDT_raw, -1., 1.);
    fHist_BDT_even->SetXTitle("BDT response");
    fHist_BDT_even->SetYTitle("Number of Entries");
	
    fHist_BDT_odd = new TH1F("h_BDT_odd", "BDT response, odd events",
							  nBinsBDT_raw, -1., 1.);
    fHist_BDT_odd->SetXTitle("BDT response");
    fHist_BDT_odd->SetYTitle("Number of Entries");
    
    fHist_BDT_rebinned = new TH1F("h_BDT_rebinned", "BDT response",
						 fNBins_BDT, 0.0, 1.0);
    fHist_BDT_rebinned->SetXTitle("BDT response");
    fHist_BDT_rebinned->SetYTitle("Number of Entries");
	
    fHist_BDT_rebinned_even = new TH1F("h_BDT_rebinned_even", "BDT response, even events",
									   fNBins_BDT, 0.0, 1.0);
    fHist_BDT_rebinned_even->SetXTitle("BDT response");
    fHist_BDT_rebinned_even->SetYTitle("Number of Entries");
	
    fHist_BDT_rebinned_odd = new TH1F("h_BDT_rebinned_odd", "BDT response, odd events",
							 fNBins_BDT, 0.0, 1.0);
    fHist_BDT_rebinned_odd->SetXTitle("BDT response");
    fHist_BDT_rebinned_odd->SetYTitle("Number of Entries");
	
    fHist_BDT_manyBins = new TH1F("h_BDT_manyBins", "BDT response",
								  1000, -1.0, 1.0);
    fHist_BDT_manyBins->SetXTitle("BDT response");
    fHist_BDT_manyBins->SetYTitle("Number of Entries");
	
	// TH1::SetDefaultSumw2 used?
	fHist_BDT->Sumw2();
	fHist_BDT_even->Sumw2();
	fHist_BDT_odd->Sumw2();
	fHist_BDT_rebinned->Sumw2();
	fHist_BDT_rebinned_even->Sumw2();
	fHist_BDT_rebinned_odd->Sumw2();
	fHist_BDT_manyBins->Sumw2();
    
    gDirectory->cd("..");
}

//____________________________________________________________________

Bool_t AtlSgTop_sChannelBDT::ProcessPreCut() {
    //
    // Event pre-selection
    //

    // Currently, no cuts applied
    
    // Accept event
    return kTRUE;
}

//____________________________________________________________________

Bool_t AtlSgTop_sChannelBDT::ProcessCut() {
    //
    // Event selection
    //

    // Currently, no cuts applied

    return kTRUE;
}

//____________________________________________________________________

void AtlSgTop_sChannelBDT::FillHistograms() {
    //
    // Fill histograms
    //
    // This method will be called only for events surviving the
    // ProcessCut() routine
    //

    // Compute likelihood ratios for TMVA
    // ComputeLikelihoodRatios();
	
    // Get BDT variables
	fBDT_MtW                        = GetEvtReader()->v_bdt_MtW;
	fBDT_DeltaPhiTopBJet2_BJet1     = GetEvtReader()->v_bdt_DeltaPhiTopBJet2_BJet1;
	fBDT_DeltaPhiTopBJet1_BJet2     = GetEvtReader()->v_bdt_DeltaPhiTopBJet1_BJet2;
	fBDT_SumEtLep_MET               = GetEvtReader()->v_bdt_SumEtLep_MET;
	fBDT_MET                        = GetEvtReader()->v_bdt_MET;
	fBDT_SumEtLep_MET_BJets         = GetEvtReader()->v_bdt_SumEtLep_MET_BJets;
	fBDT_Pt_Lep                     = GetEvtReader()->v_bdt_Pt_Lep;
	fBDT_SumPt                      = GetEvtReader()->v_bdt_SumPt;
	fBDT_DeltaEtaNu_BJetNonTop      = GetEvtReader()->v_bdt_DeltaEtaNu_BJetNonTop;
    fBDT_DeltaEtaLep_BJet1          = GetEvtReader()->v_bdt_DeltaEtaLep_BJet1;
    fBDT_DeltaPhiBJet1_BJet2        = GetEvtReader()->v_bdt_DeltaPhiBJet1_BJet2;
    fBDT_DeltaEtaLep_BJet2          = GetEvtReader()->v_bdt_DeltaEtaLep_BJet2;
    fBDT_CosLepW_WTopBJet1          = GetEvtReader()->v_bdt_CosLepW_WTopBJet1;
    fBDT_DeltaPhiLep_MET            = GetEvtReader()->v_bdt_DeltaPhiLep_MET;
    fBDT_DeltaEtaTopBJet2_BJet1     = GetEvtReader()->v_bdt_DeltaEtaTopBJet2_BJet1;
    fBDT_CosLepTopBJet2_TopBJet2CMS = GetEvtReader()->v_bdt_CosLepTopBJet2_TopBJet2CMS;
    fBDT_CosLepTopBJet1_TopBJet1CMS = GetEvtReader()->v_bdt_CosLepTopBJet1_TopBJet1CMS;
	fBDT_MLep_BJet2                 = GetEvtReader()->v_bdt_MLep_BJet2;
	fBDT_CosMET_BJet2               = GetEvtReader()->v_bdt_CosMET_BJet2;
	fBDT_SumPtBJet1_BJet2           = GetEvtReader()->v_bdt_SumPtBJet1_BJet2;
	
    // Evaluate TMVA
	// for Grenoble BDT setup
	// Float_t mvaResponse = fReader->EvaluateMVA("BDT_Grenoble");
	// for HU Berlin BDT setup
	// Float_t mvaResponse = fReader->EvaluateMVA("BDT 16 Kinematic Variables");
    // fHist_BDT->Fill(mvaResponse, GetTagEvtWeight());
    // if ( fEvent->EventNr() % 2 == 0 )
	// 	fHist_BDT_even->Fill(mvaResponse, GetTagEvtWeight());
	// else
	// 	fHist_BDT_odd->Fill(mvaResponse, GetTagEvtWeight());

	Double_t mvaResponse(0.0), mvaResponseRebinned(0.0);
	if ( fElectrons->GetEntries() )
		mvaResponse = fReaderEl->EvaluateMVA("BDT 16 Kinematic Variables");
	else
		mvaResponse = fReaderMu->EvaluateMVA("BDT 16 Kinematic Variables");
	
	// rebinning of mva response
    for (Int_t iBin = 0; iBin < fNBins_BDT; ++iBin) {
		if ( mvaResponse >= fBinEdges_BDT[iBin] &&
			 mvaResponse < fBinEdges_BDT[iBin+1] ) {
			mvaResponseRebinned = (Double_t)iBin / (Double_t)fNBins_BDT;
			break;
		}
	}
    if ( mvaResponse >= 1.0 )
		mvaResponseRebinned = (Double_t)(fNBins_BDT - 0.5) / (Double_t)fNBins_BDT;
    
    fHist_BDT->Fill(mvaResponse, GetTagEvtWeight());
    fHist_BDT_manyBins->Fill(mvaResponse, GetTagEvtWeight());
    fHist_BDT_rebinned->Fill(mvaResponseRebinned, GetTagEvtWeight());
    if ( fEvent->EventNr() % 2 == 0 ) {
		fHist_BDT_even->Fill(mvaResponse, GetTagEvtWeight());
		fHist_BDT_rebinned_even->Fill(mvaResponseRebinned, GetTagEvtWeight());
	}
	else {
		fHist_BDT_odd->Fill(mvaResponse, GetTagEvtWeight());
		fHist_BDT_rebinned_odd->Fill(mvaResponseRebinned, GetTagEvtWeight());
	}
	
    // Fill Likelihood Ratios
    // FillHistogramsLikelihoodRatios();
}

//____________________________________________________________________

void AtlSgTop_sChannelBDT::ComputeLikelihoodRatios() {

    // Compute BDT variables and fill histogram
    Double_t sChannel2j  = GetEvtReader()->v_llh_sChannel2j;
    Double_t sChannel3j  = GetEvtReader()->v_llh_sChannel3j;
    Double_t tChannel4FS = GetEvtReader()->v_llh_tChannel4FS;
    Double_t ttbarSL     = GetEvtReader()->v_llh_ttbarSL;
    Double_t Wjj         = GetEvtReader()->v_llh_Wjj;
    Double_t Wbb         = GetEvtReader()->v_llh_Wbb;
    Double_t LikelihoodSum = sChannel2j + sChannel3j + tChannel4FS + ttbarSL + Wjj + Wbb;
    
    fBDTsChannelRatio = (sChannel2j+sChannel3j)/LikelihoodSum;
    fBDTtChannelRatio = tChannel4FS/LikelihoodSum;
    fBDTttbarSLRatio  = ttbarSL/LikelihoodSum;
    fBDTWjjRatio      = Wjj/LikelihoodSum;
    fBDTWbbRatio      = Wbb/LikelihoodSum;
}

//____________________________________________________________________

void AtlSgTop_sChannelBDT::FillHistogramsLikelihoodRatios() {
    //
    // Fill likelihood ratio histograms
    //
    
    fHist_llh_ttbarSL_ratio->Fill( fBDTttbarSLRatio, GetTagEvtWeight());
}

//____________________________________________________________________

void AtlSgTop_sChannelBDT::InitEvent() {
    //
    // Prepare event analysis
    //
}

//____________________________________________________________________

void AtlSgTop_sChannelBDT::Print(Option_t *option) const {
    //
    // Print user analysis configuration
    //
    cout << endl
	 << "========================================================" << endl
	 << "  Atlas Single-Top s-Channel BDT Analysis Selector          " << endl
	 << "========================================================" << endl
	 << endl;
    AtlSelector::Print();
    cout << endl
	 << "========================================================" << endl
	 << endl;
}

//____________________________________________________________________

void AtlSgTop_sChannelBDT::Terminate() {
    //
    // Terminate routine called at the end of event loop
    //    
    AtlSelector::Terminate();
}

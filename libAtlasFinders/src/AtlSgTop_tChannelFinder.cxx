//____________________________________________________________________
//
// Single top t-channel event finder
//
// Helper class for conducting a kinematic fit and/or cut-based analysis
// of the t-channel of single-top.
//
// Used by AtlSelector.
//
//
// The Finder is looking for events with at least two jets and a W (in channels
// W->e nu, W->mu nu). One of the jet (the b-jet) is combined with the W to
// reconstruct a top quark. The finder selects the combination with the smallest
// chi^2 of the KinFitter output to be the reconstructed event.
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
//  AtlSgTop_tChannelFinder *kinfit =
//  new AtlSgTop_tChannelFinder("sgtop_finder_kinfitter",
// 	    "Single-Top t-Channel Reconstruction by KinFitter");
//  SetToolCut("sgtop_finder_kinfitter", "fMode", "AtlKinFitterTool::kKinFit");
//  SetToolCut("sgtop_finder_kinfitter", "fModeMass", "AtlSgTop_tChannelFinder::kGauss");
//  SetToolCut("sgtop_finder_kinfitter", "fBJetTagger",    "AtlBTag::kBaseline");
//  SetToolCut("sgtop_finder_kinfitter", "fBJetWeightMin", "4.0");
//  .
//  .
//  .
//  fSelector->AddTool(kinfit);
//
//  //************************************************************
//
// When included in the analysis task this class appends an additional
// directory structure to the output file in which the output histograms are
// stored.
//
// If the finder should be run in KinFitter mode, set fMode = kKinFit,
// if the finder should be run in cut-based mode, set fMode = kCutBased,
// if the finder should be run in both modes, set fMode = kAll.
//
// fChi2ProbMin provides the P(chi2) cut on the main fit.
// fVetoWhadProbMin gives the P(chi2) cut on the veto fit of an additional
// hadronically decaying W.
//
// If the usage of MissingEtRefFinal is desired, set fUseMETRefFinal = kTRUE via
// SetToolCut().
//
// If truth-matching is desired, set fTruthMatch = kTRUE via SetToolCut().
//
// If informational and/or debug output is desired, set the verbosity level
// via tool->SetVerbosityLevel(Int_t VerbosityLevel) in your task file:
//
// -> If only informational output on the reconstructed top-quark and hadronic
//    W boson is desired, set VerbosityLevel = 1.
// -> If debug output is desired, set VerbosityLevel = 2. This will print the
//    fit objects and their respective covariance matrices, preceeding and
//    succeeding the fit (main fit and Whad veto fit).
//
// If even more detailed debug output is needed, set the verbosity level of the
// KinFitter object by hand in this file!
//
// Author: Patrick Rieck <mailto:rieck@physik.hu-berlin.de>
// Copyright: 2009 (C) Patrick Rieck
//
#ifndef ATLAS_AtlSgTop_tChannelFinder
#include <AtlSgTop_tChannelFinder.h>
#endif
#include <TH1F.h>
#include <AtlEvent.h>
#include <TTree.h>
#include <TString.h>
#include <TSystem.h>
#include <iostream>
#include <TROOT.h>
#include <TFile.h>
#include <TMath.h>
#include <vector>

using namespace std;

#ifndef __CINT__
ClassImp(AtlSgTop_tChannelFinder);
#endif

const Int_t AtlSgTop_tChannelFinder::fgNSystNames = 13;
const char* AtlSgTop_tChannelFinder::fgSystNames[] = {
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

AtlSgTop_tChannelFinder::AtlSgTop_tChannelFinder(const char* name, const char* title) :
    AtlKinFitterTool(name, title) {
    //
    // Default constructor
    //
    fKinFitter_Whad = new TKinFitter;

    fLeptons     = 0;
    fJets        = 0;
    fBJets       = new TList;
    fBJets_refQCD= new TList;
    fNonBJets    = new TList;
    fWhadJets    = new TList;
    fFwdJets     = new TList;
    fLepton      = 0;
    fNeutrino    = 0;
    fBJet        = 0;
    fLeadFwdJet  = 0;
    fWJet1       = 0;
    fWJet2       = 0;
    fWBoson      = 0;
    fTop         = 0;
    fRecoW       = 0;
    fWmassT      = 0;
    fRecoTop     = 0;
    
    // Initialize covariance matrices
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

    // Initialize 4-vectors needed for the fitting loop
    fLeptonP_refit_cur.SetPxPyPzE(0., 0., 0., 0.);
    fNeutrinoP_refit_cur.SetPxPyPzE(0., 0., 0., 0.);
    fBJetP_refit_cur.SetPxPyPzE(0., 0., 0., 0.);
    fJet1P_refit_cur.SetPxPyPzE(0., 0., 0., 0.);
    fJet2P_refit_cur.SetPxPyPzE(0., 0., 0., 0.);

    fCutflow_tool    = 0;
    fCutflow_tool_2  = 0;

    SetCutDefaults();
	
}

//____________________________________________________________________

AtlSgTop_tChannelFinder::~AtlSgTop_tChannelFinder() {
    //
    // Default destructor
    //
    delete fBJets;
    delete fBJets_refQCD;
    delete fNonBJets;
    delete fWhadJets;
    delete fFwdJets;
    if ( fNeutrino   != 0 ) delete fNeutrino;
    fLepton       = 0;
    fNeutrino     = 0;
    fBJet         = 0;
    fLeadFwdJet   = 0;
    fWJet1        = 0;
    fWJet2        = 0;
    fWBoson      = 0;
    fTop         = 0;

    delete fKinFitter_Whad;
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::Clear() {
    //
    // Clean-up old lists from the previous event
    //
    fBJets->Clear();
    fBJets_refQCD->Clear();
    fNonBJets->Clear();
    fWhadJets->Clear();
    fFwdJets->Clear();
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::SetBranchStatus() {
    //
    // Switch on needed branches
    //
    fTree->SetBranchStatus("*", kFALSE);
    
    fTree->SetBranchStatus("fEventHeader*", kTRUE);
    fTree->SetBranchStatus("fWDecaysLNu*",            kTRUE);
    fTree->SetBranchStatus("fN_WDecaysLNu*",          kTRUE);
    fTree->SetBranchStatus("fWDecaysJJ*",             kTRUE);
    fTree->SetBranchStatus("fN_WDecaysJJ*",           kTRUE);
    fTree->SetBranchStatus("fTopDecays*",             kTRUE);
    fTree->SetBranchStatus("fN_TopDecays*",           kTRUE);

    // For truth matching studies only
//     fTree->SetBranchStatus("fN_MCAntiKt4HadronJets*", kTRUE);
//     fTree->SetBranchStatus("fMCAntiKt4HadronJets*",   kTRUE);
//     fTree->SetBranchStatus("fN_AtlFastJets*", kTRUE);
//     fTree->SetBranchStatus("fAtlFastJets*",   kTRUE);
//     fTree->SetBranchStatus("fN_MCParticles*", kTRUE);
//     fTree->SetBranchStatus("fMCParticles*",   kTRUE);
//     fTree->SetBranchStatus("fN_AntiKt4TopoEMJets*",  kTRUE);
//     fTree->SetBranchStatus("fAntiKt4TopoEMJets*",  kTRUE);
//     fTree->SetBranchStatus("fN_IDTracks*",          kTRUE);  
//     fTree->SetBranchStatus("fIDTracks*",            kTRUE);
//     fTree->SetBranchStatus("fN_Vertices*",         kTRUE);
//     fTree->SetBranchStatus("fVertices*",           kTRUE);
//     fTree->SetBranchStatus("fMCVertices*",         kTRUE);
//     fTree->SetBranchStatus("fN_MCVertices*",       kTRUE);
}

//____________________________________________________________________

Bool_t AtlSgTop_tChannelFinder::AnalyzeEvent() {
    //
    // Event analysis
    //

    // ================
    // Event level cuts
    // ================
    Float_t fLeadNonBJetEta = 0;

    // Initialize the current event
    InitEvent();

    // Require one lepton, missing Et, at least 2 jets
    // and one b-jet candidate 
    if ( fLepton == 0 ) return kFALSE;
    fCutflow_tool->Fill("Signal Lepton", GetPreTagEvtWeight());
    fCutflow_tool_2->Fill("Signal Lepton", GetPreTagEvtWeight());

    Bool_t IsFwdElectron = kTRUE;
    if ( ( fLepton->IsElectron() )
	 && ( TMath::Abs(fLepton->Eta()) < 1.5 ) ) IsFwdElectron = kFALSE;

    fNJets = fJets->GetEntries();
    if ( fNJets < fNJetsMin ) return kFALSE;
    fCutflow_tool->Fill(Form("No. of Jets #geq %d", fNJetsMin), GetPreTagEvtWeight());
    fCutflow_tool_2->Fill(Form("No. of Jets #geq %d", fNJetsMin), GetPreTagEvtWeight());

    if ( fNJets > fNJetsMax ) return kFALSE;
    fCutflow_tool->Fill(Form("No. of Jets #leq %d", fNJetsMax), GetPreTagEvtWeight());
    fCutflow_tool_2->Fill(Form("No. of Jets #leq %d", fNJetsMax), GetPreTagEvtWeight());


    
    //
    // QCD specialities (rejection cuts, ctrl plots)
    //
    ReconstructW();

    
    // Fill transverse W mass histograms for validation of QCD estimate (pretag)
    fHistW_Mt_QCDValidation_pretag->Fill(fWmassT, GetPreTagEvtWeight());
    fHistW_Mt_EtmissMag_QCDValidation_pretag->Fill(fWmassT+fMET_Mag, GetPreTagEvtWeight());

//     if ( fLepton->IsElectron() ) {
	// W transverse mass cut to reduce QCD:
	// Mt(W) > 30 GeV?    
    // if ( fWmassT <= fWmassT_min  ) return kFALSE;
    fCutflow_tool->Fill(Form("Mt(W) > %d GeV",(Int_t)fWmassT_min),GetPreTagEvtWeight());
    fCutflow_tool_2->Fill(Form("Mt(W) > %d GeV",(Int_t)fWmassT_min),GetPreTagEvtWeight());
//     } else if ( fLepton->IsMuon() ) {
// 	// Triangular cut to reduce QCD:
// 	// Mt(W) + Etmiss > 60 GeV?    
// 	if ( (fWmassT + fMET_Mag) <= 60  ) return kFALSE;
// 	fCutflow_tool->Fill("Mt(W) + Etmiss > 60 GeV",GetPreTagEvtWeight());
// 	fCutflow_tool_2->Fill("Mt(W) + Etmiss > 60 GeV",GetPreTagEvtWeight());
//     }

    
    // Leptons & Jets pre-tag
    FillLeptonHistograms();
    FillJetHistograms();
    FillWhadJetHistograms();
    FillBJetHistograms();
//    FillFwdJetHistograms();
    FillNonBJetHistograms();

    

    //
    // Jet bin counts for pretag sample
    //
    if ( fNJets == 2 )
	fHistJetBins_pretag->Fill("2",GetPreTagEvtWeight());
    if ( fNJets == 3 )
	fHistJetBins_pretag->Fill("3",GetPreTagEvtWeight());
    if ( fNJets == 4 )
	fHistJetBins_pretag->Fill("4",GetPreTagEvtWeight());
    if ( fNJets >= 4 )
	fHistJetBins_pretag->Fill("#geq 4",GetPreTagEvtWeight());
    if ( ( fNJets >= 2 ) && ( fNJets <= 4 ) )
	fHistJetBins_pretag->Fill("2 #leq Njets #leq 4",GetPreTagEvtWeight());
    
    fHistEtMissMag_pretag->Fill(fMET_Mag,GetPreTagEvtWeight());
    fHistEtMissPhi_pretag->Fill(fMET_Phi,GetPreTagEvtWeight());
    fHistSumEt_pretag->Fill(fEvent->GetEnergySum()->GetRefFinalEM_etightpp_SumEt(),
			    GetPreTagEvtWeight());

    // HFOR flag pretag (for W jets HF reweighting)
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

    
    // Fill histograms with pileup information
    fHist_AverageIntPerXing_pretag->Fill(fEvent->GetEventHeader()->GetAverageIntPerXing(),fEvent->GetPreTagEvtWeight());
    
    // Number of primary vertices
    fHist_NPrimaryVertices_pretag->Fill(fEvent->GetN_PrimaryVertices(),fEvent->GetPreTagEvtWeight());


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
	    << " " << ((AtlJet*)fJets->At(1))->Eta();
	    /*if (fLepton->IsElectron()){
		fOut << " " << ((AtlElectron*)fLepton)->GetElectronScaleFactor();
	    }
	    else if (fLepton->IsMuon()){
		fOut << " " << ((AtlMuon*)fLepton)->GetMuonScaleFactor();
	    }
	    TIter next_bjet2(fBJets);
	    AtlJet *bjet2 = 0;
	    while ( (bjet2 = (AtlJet*)next_bjet2()) ) {
		fOut << " " << bjet2->GetJetScaleFactor_MV1_60();
	    }*/
	fOut
	    << " " << fMET_Mag
	    << endl;
    }
    
    // Tag counts for positive and negative leptons and the total
    // (reference with b-tag cut used on all other samples except
    // on QCD from jet-electrons)
    if ( fUseLeptonCovRand ) {// QCD from jet-electrons?
	if ( fBJets_refQCD->GetEntries() == 1 ) {
	    if ( fLepton->IsPositive() ) {
		fHistCountLepPlus_tag_refQCD->AddBinContent(1,fEvent->GetPreTagEvtWeight());
		fHistCountLepAll_tag_refQCD->AddBinContent(1,fEvent->GetPreTagEvtWeight());
	    } else if ( fLepton->IsNegative() ) {
		fHistCountLepMinus_tag_refQCD->AddBinContent(1,fEvent->GetPreTagEvtWeight());
		fHistCountLepAll_tag_refQCD->AddBinContent(1,fEvent->GetPreTagEvtWeight());
	    }
	}
    }

    // ==================================
    // Chad's anti-t-channel cuts, pretag
    // (needed for W tool)
    // ==================================
    Bool_t IsW = kFALSE;
    if ( fNJets > 1 ) {
	// Just take pretag count (already W+jets dominated, hardly any t-channel)
	IsW = kTRUE;
	
	// Tag counts for positive and negative leptons in W control region
	// (using Chad's anti-t-channel cuts)
	if ( IsW ) {
	    if ( fLepton->IsPositive() ) {
		fHistCountLepPlus_WCtrl_pretag->AddBinContent(1,fEvent->GetPreTagEvtWeight());
		fHistCountLepAll_WCtrl_pretag->AddBinContent(1,fEvent->GetPreTagEvtWeight());
	    } else if ( fLepton->IsNegative() ) {
		fHistCountLepMinus_WCtrl_pretag->AddBinContent(1,fEvent->GetPreTagEvtWeight());
		fHistCountLepAll_WCtrl_pretag->AddBinContent(1,fEvent->GetPreTagEvtWeight());
	    }
	}
	
    }

    
    // -----------------
    // B-tag requirement
    // -----------------
    if ( fBJets->GetEntries() == 0 ) return kFALSE;
    
    // Fill cutflow
    fCutflow_tool->Fill("b-Jet", GetPreTagEvtWeight());
    fCutflow_tool_2->Fill("b-Jet", GetPreTagEvtWeight());

    //
    // Define b-tag sample as containing exactly 1 b-tag
    // (consistent QCD normalization etc.)
    //

        
    // --------------------------
    // Cuts of cut-based analysis
    // --------------------------

    // Only one b-tag
    if ( fBJets->GetEntries() > 1 ) return kFALSE;
    fCutflow_tool->Fill("1 b-Tag", GetPreTagEvtWeight());
    fCutflow_tool_2->Fill("1 b-Tag", GetPreTagEvtWeight());


    
    FillLeptonHistograms_BTag();
    FillJetHistograms_BTag();
    FillWhadJetHistograms_BTag();
    FillBJetHistograms_BTag();
    FillNonBJetHistograms_BTag();
    
    FillCutBasedCutsHistograms();

    
    fHistEtMissMag_tag->Fill(fMET_Mag,GetPreTagEvtWeight());
    fHistEtMissPhi_tag->Fill(fMET_Phi,GetPreTagEvtWeight());
    fHistSumEt_tag->Fill(fEvent->GetEnergySum()->GetRefFinalEM_etightpp_SumEt(),
			    GetPreTagEvtWeight());


    // Fill transverse W mass histograms for validation of QCD estimate (tag)
    fHistW_Mt_QCDValidation_tag->Fill(fWmassT, GetPreTagEvtWeight());
    fHistW_Mt_EtmissMag_QCDValidation_tag->Fill(fWmassT+fMET_Mag, GetPreTagEvtWeight());

    // Fill histograms with pile-up information
    fHist_AverageIntPerXing_tag->Fill(fEvent->GetEventHeader()->GetAverageIntPerXing(),fEvent->GetPreTagEvtWeight());
    
    // Number of primary vertices
    fHist_NPrimaryVertices_tag->Fill(fEvent->GetN_PrimaryVertices(),fEvent->GetPreTagEvtWeight());

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
    if ( fNJets == 2 )
	fHistJetBins_tag->Fill("2",GetPreTagEvtWeight());
    if ( fNJets == 3 )
	fHistJetBins_tag->Fill("3",GetPreTagEvtWeight());
    if ( fNJets == 4 )
	fHistJetBins_tag->Fill("4",GetPreTagEvtWeight());
    if ( fNJets >= 4 )
	fHistJetBins_tag->Fill("#geq 4",GetPreTagEvtWeight());
    if ( ( fNJets >= 2 ) && ( fNJets <= 4 ) )
	fHistJetBins_tag->Fill("2 #leq Njets #leq 4",GetPreTagEvtWeight());

    
    // Only for 2 and 3 jet bin!
//     if ( (fNJets == 2) || (fNJets == 3) ) {


    // ===============================
    // Chad's anti-t-channel cuts, tag
    // (needed for W tool)
    // ===============================
    IsW = kFALSE;
    Bool_t FailsEtaCut = kFALSE;
    Bool_t FailsTopMassCut = kFALSE;
    Bool_t FailsHtCut = kFALSE;
    Bool_t FailsDeltaEtaCut = kFALSE;
    Bool_t FailsAllJetsMassCut = kFALSE;
    if ( fNJets > 1 ) {
	// 1 : light_jet_eta : | hardest untagged jet eta | > 2.0
	AtlJet* LeadNonBJet = (AtlJet*)fNonBJets->At(0);
	fLeadNonBJetEta = LeadNonBJet->EMScaleEta() + LeadNonBJet->EMJES_EtaCorr();
	if ( ( TMath::Abs(fLeadNonBJetEta) ) <= 2. ) FailsEtaCut = kTRUE;

	// 2: Top mass range
	AtlJet* LeadBJet = (AtlJet*)fBJets->At(0);
	Double_t n_px = fMET_Mag*cos(fMET_Phi);
	Double_t n_py = fMET_Mag*sin(fMET_Phi);
	Double_t scp = -999999.0;
	Double_t truth_pz = 0.0;
	Double_t n_pz = Neutrino_Pz(n_px, n_py, fLepton->P().Px(), fLepton->P().Py(), fLepton->P().Pz(), fLepton->P().E(), scp, truth_pz);
	Double_t n_E = sqrt(n_px*n_px + n_py*n_py + n_pz*n_pz);
	TLorentzVector neutrino;
	neutrino.SetPxPyPzE(n_px, n_py, n_pz, n_E);
	TLorentzVector top = LeadBJet->P() + fLepton->P() + neutrino;
	Double_t top_mass = top.M();
	if ( (top_mass >= 190.) || (top_mass <= 150.)  ) FailsTopMassCut = kTRUE;

	// 3: H_T
	if ( fH_T <= 210. ) FailsHtCut = kTRUE;

	// 4 : | delta eta(b-jet, hardest untagged jet) |
	// use hardest b-jet for now!
	Float_t lead_bjet_eta = LeadBJet->EMScaleEta() + LeadBJet->EMJES_EtaCorr();
	Float_t delta_eta_bjet_nonbjet = TMath::Abs(lead_bjet_eta - fLeadNonBJetEta);
	if ( delta_eta_bjet_nonbjet <= 1. ) FailsDeltaEtaCut = kTRUE;	

	// 5 : invariant mass of all jets in event
	if ( fMassAllJets <= 450. ) FailsAllJetsMassCut = kTRUE;

	
	if ( fNJets == 2 ) {
 	    if ( FailsEtaCut && FailsTopMassCut && FailsHtCut && FailsDeltaEtaCut )
		// Study to loosen cuts:
// 	    if ( FailsTopMassCut && FailsHtCut && FailsDeltaEtaCut )
//	    if ( FailsTopMassCut && FailsHtCut )
		IsW = kTRUE;
	} else if ( fNJets == 3 ) {
	    if ( FailsEtaCut && FailsTopMassCut && FailsHtCut && FailsAllJetsMassCut )
		IsW = kTRUE;
	} else if ( fNJets == 4 ) {
	    if ( FailsEtaCut && FailsTopMassCut && FailsHtCut )
		IsW = kTRUE;
	}
// 	    if ( (fNJets == 2) && (fNonBJets->GetEntries() != 0) ) {
// 	    if ( delta_eta_bjet_nonbjet <= fDeltaEtaBJetNonBJetMin ) return kFALSE; 
// 	    fCutflow_tool->Fill("#Delta#eta(BJet-LightJet) > 0.8", GetPreTagEvtWeight());
// 	    fCutflow_tool_2->Fill("#Delta#eta(BJet-LightJet) > 0.8", GetPreTagEvtWeight());
// //     fCutflow_tool->Fill(Form("#Delta#eta(BJet-LightJet) > %f",fDeltaEtaBJetNonBJetMin), GetPreTagEvtWeight());
// //     fCutflow_tool_2->Fill(Form("#Delta#eta(BJet-LightJet) > %f",fDeltaEtaBJetNonBJetMin), GetPreTagEvtWeight());
	//}
	
	
// 	if ( fNJets == 3 ) {
// 	    // 3 : invariant mass of all jets in event
// 	    if ( fMassAllJets <= fMassAllJetsMin ) return kFALSE;
// 	    fCutflow_tool->Fill("Mass(AllJets) > 450 GeV", GetPreTagEvtWeight());
// 	    fCutflow_tool_2->Fill("Mass(AllJets) > 450 GeV", GetPreTagEvtWeight());
// //     fCutflow_tool->Fill(Form("Mass(AllJets) > %f",fMassAllJetsMin), GetPreTagEvtWeight());
// //     fCutflow_tool_2->Fill(Form("Mass(AllJets) > %f",fMassAllJetsMin), GetPreTagEvtWeight());
// 	}
    
    // Tag counts for positive and negative leptons in W control region
    // (using Chad's anti-t-channel cuts)
	if ( IsW ) {
	    if ( fLepton->IsPositive() ) {
		fHistCountLepPlus_WCtrl_tag->AddBinContent(1,fEvent->GetPreTagEvtWeight());
		fHistCountLepAll_WCtrl_tag->AddBinContent(1,fEvent->GetPreTagEvtWeight());
	    } else if ( fLepton->IsNegative() ) {
		fHistCountLepMinus_WCtrl_tag->AddBinContent(1,fEvent->GetPreTagEvtWeight());
		fHistCountLepAll_WCtrl_tag->AddBinContent(1,fEvent->GetPreTagEvtWeight());
	    }
	    
	    // Keep track of counts of central and forward electrons in data
	    if ( fLepton->IsElectron() ) {
		if ( IsFwdElectron )
		    fHistCountFwdElectrons_WCtrl->AddBinContent(1,fEvent->GetPreTagEvtWeight());
		else
		    fHistCountCentralElectrons_WCtrl->AddBinContent(1,fEvent->GetPreTagEvtWeight());		
	    }
	}
	
    }
// 	fCutflow_tool->Fill("#eta(LeadNonBJet) > 2", GetPreTagEvtWeight());
// 	fCutflow_tool_2->Fill("#eta(LeadNonBJet) > 2", GetPreTagEvtWeight());
// //     fCutflow_tool->Fill(Form("#eta(LeadNonBJet) > %f",fLeadNonBJetEtaMin), GetPreTagEvtWeight());
// //     fCutflow_tool_2->Fill(Form("#eta(LeadNonBJet) > %f",fLeadNonBJetEtaMin), GetPreTagEvtWeight());

		
	// 2 : H_T - scalar sum of pt of all objects (lepton, missEt, jets)
	if ( fH_T <= fH_TMin ) return kFALSE;
	fCutflow_tool->Fill("H_{T} > 160 GeV", GetPreTagEvtWeight());
	fCutflow_tool_2->Fill("H_{T} > 160 GeV", GetPreTagEvtWeight());
//     fCutflow_tool->Fill(Form("H_{T} > %f",fH_TMin), GetPreTagEvtWeight());
//     fCutflow_tool_2->Fill(Form("H_{T} > %f",fH_TMin), GetPreTagEvtWeight());

	


//    }

    
    // ======================================
    // Execute requested reconstruction modes
    // ======================================

    // Guard for 1 jet bin:
    if ( fNJets == 1 ) return kFALSE;

    if ( fMode & kKinFit ) {
	ReconstructionKinFit();
    }
    if ( fMode & kCutBased ) {
	ReconstructionCutBased();
    }
    
    return kTRUE;
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::InitEvent() {
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
	if ( ( lep->IsElectron() )
	     // below implies for SgTop D3PDs: cluster energy over CosH(IDTrack eta) (see TopSlim reader!)
	     && (((AtlElectron*)lep)->Pt() > fElPt_Min) ) {
	    fLepton = lep;
	}
	
	// Muon
	if ( ( lep->IsMuon() )
	     && (( lep->Pt() > fMuPt_Min )) ) {
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
    if ( fUseLeptonCovRand )
	// Get its covariance matrix
	AtlKinFitterTool::GetLeptonCovMatRand(fLepton,fCovLepton);
    else
	// Get its covariance matrix
	fLepton->GetCovMatrixPtEtaPhi(fCovLepton);

    AtlKinFitterTool::FillLeptonCovMatrixHistograms(fLepton,fCovLepton); //control histograms of covariance matrix


    // ======
    // Etmiss
    // ======
    fMET_Mag   = fMET.Mod();
    fMET_Phi   = TVector2::Phi_mpi_pi((Double_t)fMET.Phi());

    
    // ========
    // Neutrino 
    // ========

    // Set neutrino from missing Et. Since its z component cannot be
    // measured eta=0 and E=Et will be set here. Later in
    // ReconstructionKinFit() better starting values will be guessed
    // with the help of the corresponding b-jet and lepton. The
    // neutrino's covariance matrix will be set at that point also,
    // because its elements are eta-dependent and are extracted with
    // the help of the eta starting value
    //
    // The correct PDG code defining if it is a neutrino or an
    // antineutrino will be set inside the loop over all leptons.
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
	if ( (wjet->Pt() <= fWhadJets_Et_min )
	     || ( TMath::Abs(wjet->Eta()) < fWhadJets_Eta_min )
	     || ( TMath::Abs(wjet->Eta()) > fWhadJets_Eta_max )
	    )
	    fWhadJets->Remove(wjet);
    }

    // Sort list
    fWhadJets->Sort(kSortDescending);



    //
    // Additional topological Cuts on Jets
    //

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
	    || ( TMath::Abs(bjet->Eta()) < fBJets_Eta_min )
	    || ( TMath::Abs(bjet->Eta()) > fBJets_Eta_max )
	    )
	    fBJets->Remove(bjet);
    }

    // Sort list
    fBJets->Sort(kSortDescending);


    // ====================================
    // Get refined list of reference b-jets 
    // ====================================
    //(with b-tag cut used on all other samples; only filled on jet-electrons)
    fBJets_refQCD->AddAll(fJets);
    
    TIter next_bjet_ref(fBJets_refQCD);
    AtlJet *bjet_ref = 0;

    // Remove low energetic jets and non-central jets from the list
    while ( (bjet_ref = (AtlJet*)next_bjet_ref()) ) {
	if (
	    ( bjet_ref->GetTag(fBJetRefTagger)->GetWeight() <= fBJetRefWeightMin )
	    || ( bjet_ref->Pt() <= fBJets_Et_min )
	    || ( TMath::Abs(bjet_ref->Eta()) < fBJets_Eta_min )
	    || ( TMath::Abs(bjet_ref->Eta()) > fBJets_Eta_max )
	    )
	    fBJets_refQCD->Remove(bjet_ref);
    }

    // Sort list
    fBJets_refQCD->Sort(kSortDescending);


    
    // ==================================
    // Clean forward jet list from b-jets
    // ==================================
//     fFwdJets->RemoveAll(fBJets);

    // ==================================
    // Clean Whad jet list from b-jets
    // ==================================
    fWhadJets->RemoveAll(fBJets);
    
    // ======================================
    // Get list of constrained non-b-jets now
    // ======================================
    fNonBJets->AddAll(fJets);
    fNonBJets->RemoveAll(fBJets);
    
    // Sort list
    fNonBJets->Sort(kSortDescending);

    fH_T         = 0.;
    fMassAllJets = 0.;
    
//     if ( fNJets < 4 ) {
	// Compute H_T (scalar sum of pt of lepton, missEt and jets) for cut-based analysis cuts
	fH_T = fLepton->Pt()+fMET_Mag;
	
	TLorentzVector p_jets;
	
	TIter next_jet3(fJets);
	AtlJet *jet3 = 0;
	while ( (jet3 = (AtlJet*)next_jet3()) ) {
	    fH_T += jet3->Pt();
	    p_jets += jet3->P(); 
	}
	
// 	if ( fNJets == 3 ) {
	// Compute invariant mass of all jets for cut-based analysis cuts
	fMassAllJets = p_jets.M();
// 	}
	
//     }
    
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::SetNeutrinoStartingValues() {
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

void AtlSgTop_tChannelFinder::FillLeptonHistograms() {
    //
    // Fill lepton histograms
    //

    // =============
    // Signal lepton
    // =============
    if ( fLepton != 0 ) {
	Float_t Pt_sig       = fLepton->Pt(); 
	Float_t Eta_sig      = fLepton->Eta();
	Float_t Phi_sig      = fLepton->Phi();
	Float_t EtCone20_sig = -10e10;
	Float_t PtCone20_sig = -10e10;
	Float_t EtCone30_sig = -10e10;
	Float_t PtCone30_sig = -10e10;
	if ( fLepton->IsElectron() ){
	
	    EtCone20_sig = ((AtlElectron*)fLepton)->GetEtCone20();
	    PtCone20_sig = ((AtlElectron*)fLepton)->GetPtCone20();
	    EtCone30_sig = ((AtlElectron*)fLepton)->GetEtCone30();
	    PtCone30_sig = ((AtlElectron*)fLepton)->GetPtCone30();

	}
	if ( fLepton->IsMuon() ){
	
	    EtCone20_sig = ((AtlMuon*)fLepton)->GetEtCone20();
	    PtCone20_sig = ((AtlMuon*)fLepton)->GetPtCone20();
	    EtCone30_sig = ((AtlMuon*)fLepton)->GetEtCone30();
	    PtCone30_sig = ((AtlMuon*)fLepton)->GetPtCone30();

	}	
	Float_t EtRatio_sig  = EtCone30_sig/Pt_sig;
	Float_t PtRatio_sig  = PtCone30_sig/Pt_sig;
	
	
	fHistSignalLepton_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
	fHistSignalLepton_eta->Fill(Eta_sig, GetPreTagEvtWeight());
	fHistSignalLepton_eta_cells->Fill(Eta_sig, GetPreTagEvtWeight());
	fHistSignalLepton_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	fHistSignalLepton_EtCone20->Fill(EtCone20_sig, GetPreTagEvtWeight());
	fHistSignalLepton_PtCone20->Fill(PtCone20_sig, GetPreTagEvtWeight());
	fHistSignalLepton_EtCone30->Fill(EtCone30_sig, GetPreTagEvtWeight());
	fHistSignalLepton_PtCone30->Fill(PtCone30_sig, GetPreTagEvtWeight());
	
	fHistSignalLepton_EtRatio->Fill(EtRatio_sig, GetPreTagEvtWeight());
	fHistSignalLepton_PtRatio->Fill(PtRatio_sig, GetPreTagEvtWeight());
	    
	
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
	    fHistSignalElectron_EtCone20->Fill(EtCone20_sig, GetPreTagEvtWeight());
	    fHistSignalElectron_PtCone20->Fill(PtCone20_sig, GetPreTagEvtWeight());
	    fHistSignalElectron_EtCone30->Fill(EtCone30_sig, GetPreTagEvtWeight());
	    fHistSignalElectron_PtCone30->Fill(PtCone30_sig, GetPreTagEvtWeight());
	    fHistSignalElectron_EtRatio->Fill(EtRatio_sig, GetPreTagEvtWeight());
	    fHistSignalElectron_PtRatio->Fill(PtRatio_sig, GetPreTagEvtWeight());
	    
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
	    fHistSignalMuon_EtCone20->Fill(EtCone20_sig, GetPreTagEvtWeight());
	    fHistSignalMuon_PtCone20->Fill(PtCone20_sig, GetPreTagEvtWeight());
	    fHistSignalMuon_EtCone30->Fill(EtCone30_sig, GetPreTagEvtWeight());
	    fHistSignalMuon_PtCone30->Fill(PtCone30_sig, GetPreTagEvtWeight());
     	    fHistSignalMuon_EtRatio->Fill(EtRatio_sig, GetPreTagEvtWeight());
	    fHistSignalMuon_PtRatio->Fill(PtRatio_sig, GetPreTagEvtWeight());
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
	
	// tau
	if ( fLepton->IsTau() ) {
	    fHistSignalTau_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
	    fHistSignalTau_eta->Fill(Eta_sig, GetPreTagEvtWeight());
	    fHistSignalTau_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    if ( fLepton->IsPositive() ) {
		fHistSignalTauplus_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistSignalTauplus_eta->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistSignalTauplus_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    } else {
		fHistSignalTauminus_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistSignalTauminus_eta->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistSignalTauminus_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    }
	}
    }
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::FillLeptonHistograms_BTag() {
    //
    // Fill lepton histograms
    //

    // =============
    // Signal lepton
    // =============
    if ( fLepton != 0 ) {
	Float_t Pt_sig  = fLepton->Pt(); 
	Float_t Eta_sig = fLepton->Eta();
	Float_t Phi_sig = fLepton->Phi();
	Float_t EtCone20_sig = -10e10;
	Float_t PtCone20_sig = -10e10;
	Float_t EtCone30_sig = -10e10;
	Float_t PtCone30_sig = -10e10;
	
	if ( fLepton->IsElectron() ){
	
	    EtCone20_sig = ((AtlElectron*)fLepton)->GetEtCone20();
	    PtCone20_sig = ((AtlElectron*)fLepton)->GetPtCone20();
	    EtCone30_sig = ((AtlElectron*)fLepton)->GetEtCone30();
	    PtCone30_sig = ((AtlElectron*)fLepton)->GetPtCone30();
	    

	}
	if ( fLepton->IsMuon() ){
	
	    EtCone20_sig = ((AtlMuon*)fLepton)->GetEtCone20();
	    PtCone20_sig = ((AtlMuon*)fLepton)->GetPtCone20();
	    EtCone30_sig = ((AtlMuon*)fLepton)->GetEtCone30();
	    PtCone30_sig = ((AtlMuon*)fLepton)->GetPtCone30();

	}	
	Float_t EtRatio_sig  = EtCone30_sig/Pt_sig;
	Float_t PtRatio_sig  = PtCone30_sig/Pt_sig;

	
	fHistSignalLepton_pt_btag ->Fill(Pt_sig,  GetPreTagEvtWeight());
	fHistSignalLepton_eta_btag->Fill(Eta_sig, GetPreTagEvtWeight());
	fHistSignalLepton_eta_btag_cells->Fill(Eta_sig, GetPreTagEvtWeight());
	fHistSignalLepton_phi_btag->Fill(Phi_sig, GetPreTagEvtWeight());
	fHistSignalLepton_EtCone20_btag->Fill(EtCone20_sig, GetPreTagEvtWeight());
	fHistSignalLepton_PtCone20_btag->Fill(PtCone20_sig, GetPreTagEvtWeight());
	fHistSignalLepton_EtCone30_btag->Fill(EtCone30_sig, GetPreTagEvtWeight());
	fHistSignalLepton_PtCone30_btag->Fill(PtCone30_sig, GetPreTagEvtWeight());
	fHistSignalLepton_EtRatio_btag->Fill(EtRatio_sig, GetPreTagEvtWeight());
	fHistSignalLepton_PtRatio_btag->Fill(PtRatio_sig, GetPreTagEvtWeight());
	
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
	    fHistSignalElectron_EtCone20_btag->Fill(EtCone20_sig, GetPreTagEvtWeight());
	    fHistSignalElectron_PtCone20_btag->Fill(PtCone20_sig, GetPreTagEvtWeight());
	    fHistSignalElectron_EtCone30_btag->Fill(EtCone30_sig, GetPreTagEvtWeight());
	    fHistSignalElectron_PtCone30_btag->Fill(PtCone30_sig, GetPreTagEvtWeight());
	    fHistSignalElectron_EtRatio_btag->Fill(EtRatio_sig, GetPreTagEvtWeight());
	    fHistSignalElectron_PtRatio_btag->Fill(PtRatio_sig, GetPreTagEvtWeight());
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
	    fHistSignalMuon_EtCone20_btag->Fill(EtCone20_sig, GetPreTagEvtWeight());
	    fHistSignalMuon_PtCone20_btag->Fill(PtCone20_sig, GetPreTagEvtWeight());
	    fHistSignalMuon_EtCone30_btag->Fill(EtCone30_sig, GetPreTagEvtWeight());
	    fHistSignalMuon_PtCone30_btag->Fill(PtCone30_sig, GetPreTagEvtWeight());
	    fHistSignalMuon_EtRatio_btag->Fill(EtRatio_sig, GetPreTagEvtWeight());
	    fHistSignalMuon_PtRatio_btag->Fill(PtRatio_sig, GetPreTagEvtWeight());
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

void AtlSgTop_tChannelFinder::FillJetHistograms() {
    //
    // Fill jet histograms
    //
    Float_t Et  = 0.;
    Float_t eta = 0.;
    Float_t phi = 0.;
    Float_t pt = 0.;
    Float_t weight = 0;
    AtlJet *jet = 0;
    
    TIter next_jet(fJets);
    Int_t njet = 0;
    while ( (jet = (AtlJet*)next_jet()) ) {
	Et  = jet->Et();
	eta = jet->Eta();
	phi = jet->Phi();
	pt  = jet->Pt();  
	weight = jet->GetTag(fBJetTagger)->GetWeight();
	fHistJet_Et ->Fill(Et, GetPreTagEvtWeight());
	fHistJet_eta->Fill(eta, GetPreTagEvtWeight());
	fHistJet_phi->Fill(phi, GetPreTagEvtWeight());
	fHistJet_pt->Fill(pt, GetPreTagEvtWeight());
	fHistJet_weight->Fill(weight, GetPreTagEvtWeight());
	if ( njet == 0 ) {
	    fHistJet1_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistJet1_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistJet1_phi->Fill(phi, GetPreTagEvtWeight());
	    fHistDeltaJet1PhiEtMissPhi->Fill(phi - fMET_Phi, GetPreTagEvtWeight());
	    fHistJet1_pt->Fill(pt, GetPreTagEvtWeight());
	}
	if ( njet == 1 ) {
	    fHistJet2_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistJet2_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistJet2_phi->Fill(phi, GetPreTagEvtWeight());
	    fHistJet2_pt->Fill(pt, GetPreTagEvtWeight());
	}
	njet++;
    }
    fHistJet_n->Fill(fJets->GetEntries(), GetPreTagEvtWeight());
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::FillWhadJetHistograms() {
    //
    // Fill jet histograms
    //
    Float_t Et  = 0.;
    Float_t eta = 0.;
    Float_t phi = 0.;
    AtlJet *jet = 0;
    
    TIter next_jet(fWhadJets);
    Int_t njet = 0;
    while ( (jet = (AtlJet*)next_jet()) ) {
	Et  = jet->Et();
	eta = jet->Eta();
	phi = jet->Phi();
	fHistWhadJet_Et ->Fill(Et, GetPreTagEvtWeight());
	fHistWhadJet_eta->Fill(eta, GetPreTagEvtWeight());
	fHistWhadJet_phi->Fill(phi, GetPreTagEvtWeight());
	if ( njet == 0 ) {
	    fHistWhadJet1_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistWhadJet1_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistWhadJet1_phi->Fill(phi, GetPreTagEvtWeight());
	}
	if ( njet == 1 ) {
	    fHistWhadJet2_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistWhadJet2_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistWhadJet2_phi->Fill(phi, GetPreTagEvtWeight());
	}
	njet++;
    }
    fHistWhadJet_n->Fill(njet, GetPreTagEvtWeight());
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::FillCutBasedCutsHistograms() {
    //
    // Fill histograms of variables for cut-based analysis combined
    // with KinFittter analysis (preceeding it)
    //
    fHistJet_H_T->Fill(fH_T, GetPreTagEvtWeight());
    fHistJet_MassAllJets->Fill(fMassAllJets, GetPreTagEvtWeight());    
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::FillBJetHistograms() {
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

void AtlSgTop_tChannelFinder::FillNonBJetHistograms() {
    //
    // Fill non-b-jet histograms
    //
    Float_t Et  = 0.;
    Float_t eta = 0.;
    Float_t phi = 0.;
    AtlJet *jet = 0;
    
    TIter next_nonbjet(fNonBJets);
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

void AtlSgTop_tChannelFinder::FillJetHistograms_BTag() {
    //
    // Fill jet histograms
    //
    Float_t Et  = 0.;
    Float_t eta = 0.;
    Float_t phi = 0.;
    Float_t pt = 0.;
    Float_t weight = 0;
    AtlJet *jet = 0;
    
    TIter next_jet(fJets);
    Int_t njet = 0;
    while ( (jet = (AtlJet*)next_jet()) ) {
	Et  = jet->Et();
	eta = jet->Eta();
	phi = jet->Phi();
	pt  = jet->Pt();  
	weight = jet->GetTag(fBJetTagger)->GetWeight();
	fHistJet_Et_btag ->Fill(Et, GetPreTagEvtWeight());
	fHistJet_eta_btag->Fill(eta, GetPreTagEvtWeight());
	fHistJet_phi_btag->Fill(phi, GetPreTagEvtWeight());
	fHistJet_pt_btag->Fill(pt, GetPreTagEvtWeight());
	fHistJet_weight_btag->Fill(weight, GetPreTagEvtWeight());
	if ( njet == 0 ) {
	    fHistJet1_Et_btag ->Fill(Et, GetPreTagEvtWeight());
	    fHistJet1_eta_btag->Fill(eta, GetPreTagEvtWeight());
	    fHistJet1_phi_btag->Fill(phi, GetPreTagEvtWeight());
	    fHistDeltaJet1PhiEtMissPhi_btag->Fill(phi - fMET_Phi, GetPreTagEvtWeight());
	    fHistJet1_pt_btag->Fill(pt, GetPreTagEvtWeight());
	}
	if ( njet == 1 ) {
	    fHistJet2_Et_btag ->Fill(Et, GetPreTagEvtWeight());
	    fHistJet2_eta_btag->Fill(eta, GetPreTagEvtWeight());
	    fHistJet2_phi_btag->Fill(phi, GetPreTagEvtWeight());
	    fHistJet2_pt_btag->Fill(pt, GetPreTagEvtWeight());
	}
	njet++;
    }
    fHistJet_n_btag->Fill(njet, GetPreTagEvtWeight());
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::FillWhadJetHistograms_BTag() {
    //
    // Fill jet histograms
    //
    Float_t Et  = 0.;
    Float_t eta = 0.;
    Float_t phi = 0.;
    AtlJet *jet = 0;
    
    TIter next_jet(fWhadJets);
    Int_t njet = 0;
    while ( (jet = (AtlJet*)next_jet()) ) {
	Et  = jet->Et();
	eta = jet->Eta();
	phi = jet->Phi();
	fHistWhadJet_Et_btag ->Fill(Et, GetPreTagEvtWeight());
	fHistWhadJet_eta_btag->Fill(eta, GetPreTagEvtWeight());
	fHistWhadJet_phi_btag->Fill(phi, GetPreTagEvtWeight());
	if ( njet == 0 ) {
	    fHistWhadJet1_Et_btag ->Fill(Et, GetPreTagEvtWeight());
	    fHistWhadJet1_eta_btag->Fill(eta, GetPreTagEvtWeight());
	    fHistWhadJet1_phi_btag->Fill(phi, GetPreTagEvtWeight());
	}
	if ( njet == 1 ) {
	    fHistWhadJet2_Et_btag ->Fill(Et, GetPreTagEvtWeight());
	    fHistWhadJet2_eta_btag->Fill(eta, GetPreTagEvtWeight());
	    fHistWhadJet2_phi_btag->Fill(phi, GetPreTagEvtWeight());
	}
	njet++;
    }
    fHistWhadJet_n_btag->Fill(njet, GetPreTagEvtWeight());
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::FillBJetHistograms_BTag() {
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

void AtlSgTop_tChannelFinder::FillNonBJetHistograms_BTag() {
    //
    // Fill non-b-jet histograms
    //
    Float_t Et  = 0.;
    Float_t eta = 0.;
    Float_t phi = 0.;
    AtlJet *jet = 0;
    
    TIter next_nonbjet(fNonBJets);
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

void AtlSgTop_tChannelFinder::FillFwdJetHistograms() {
    //
    // Fill fwd-jet histograms
    //
    
//     //
//     // Forward jet eta
//     //
//     Float_t Et  = fLeadFwdJet->Et();
//     Float_t eta = fLeadFwdJet->Eta();
//     Float_t phi = fLeadFwdJet->Phi();
    
//     fHistFwdJet_Et ->Fill(Et, GetPreTagEvtWeight());
//     fHistFwdJet_eta->Fill(eta, GetPreTagEvtWeight());
//     fHistFwdJet_phi->Fill(phi, GetPreTagEvtWeight());


//     Float_t Et  = 0.;
//     Float_t eta = 0.;
//     Float_t phi = 0.;
//     AtlJet *jet = 0;
    
//     TIter next_fwdjet(fFwdJets);
//     Int_t njet = 0;
//     while ( (jet = (AtlJet*)next_fwdjet()) ) {
// 	Et  = jet->Et();
// 	eta = jet->Eta();
// 	phi = jet->Phi();
// 	fHistFwdJet_Et ->Fill(Et, GetPreTagEvtWeight());
// 	fHistFwdJet_eta->Fill(eta, GetPreTagEvtWeight());
// 	fHistFwdJet_phi->Fill(phi, GetPreTagEvtWeight());
// 	if ( njet == 0 ) {
// 	    fHistFwdJet1_Et ->Fill(Et, GetPreTagEvtWeight());
// 	    fHistFwdJet1_eta->Fill(eta, GetPreTagEvtWeight());
// 	    fHistFwdJet1_phi->Fill(phi, GetPreTagEvtWeight());
// 	}
// 	if ( njet == 1 ) {
// 	    fHistFwdJet2_Et ->Fill(Et, GetPreTagEvtWeight());
// 	    fHistFwdJet2_eta->Fill(eta, GetPreTagEvtWeight());
// 	    fHistFwdJet2_phi->Fill(phi, GetPreTagEvtWeight());
// 	}
// 	njet++;
//     }
//     fHistFwdJet_n->Fill(njet, GetPreTagEvtWeight());
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::ReconstructionKinFit() {
    //
    // Single-top t-channel event reconstruction (semi-leptonic)
    // 
    // --> loop over all lepton and jet combinations
    // --> perform the fit
    // --> select the combination with the smallest chi^2 as candidate
    //     for a signal event
    //

    // --------------------
    // TopPairFinder result 
    // --------------------
    Double_t chi2PROBttbarBkg = -1.;
    
    if( fDoTtbarVeto && fEvent->GetN_TopDecays() > 1 ) {
	//
	// Find top pair with best (highest) P(Chi2)
	//
	TIter next_top(fEvent->GetTopDecays());
	HepTopDecay* top = 0;
	Double_t  p_chi2 = 0;
	
	// Loop over all top-quarks
	while ( ( top = (HepTopDecay*)next_top() ) ) {
	    p_chi2 = top->GetProb();
	    if ( p_chi2 > chi2PROBttbarBkg ) chi2PROBttbarBkg = p_chi2;
	}

	cout << endl;
	Info("ReconstructionKinFit","Found ttbar candidate with P(Chi2) = %f. In run %d evt %llu lb %d",
	     chi2PROBttbarBkg,fEvent->RunNr(),fEvent->EventNr(),fEvent->LumiBlock());

    }
    
    // --------------------

    
    
    // Number of combinations that led to convergence
    fN_Converged = 0;
  
    // Particles of best fit result
    AtlJet      *BestBJet     = 0;

    // No. of fits that did not converge in this event
    fN_NotConverged = 0;

    // No. of fits that failed numerically in this event
    // (cause: matrix inversions failed)
    fN_FailNumeric = 0;

    // No. of top-quark candidates with higher Chi2 than chosen one
    fN_WorseTopCandidates = 0;
    Int_t max_size = fBJets->GetEntries();
    Float_t chi2[max_size];
    for ( Int_t i = 0; i < max_size; i++ ) {
	chi2[i] = 0;
    }
    Int_t counter = 0;
    
    // Use 1.e10 as starting value for chi^2,
    // reconstruction successful if chi^2 gets smaller
    fChi2 = 1.e10;

    // ==============================================
    // Loop over all combinations of leptons and jets
    // ==============================================
    TIter next_bjet(fBJets);
    
    // Loop over all b-jets
    while( (fBJet = (AtlJet*)next_bjet()) ) {

	// Set b-jet covariance matrix
	SetCovMatrix(fCovBJet, fBJet);

	// Improve starting values for eta and E of the neutrino and set
	// its covariance matrix
	SetNeutrinoStartingValues();
	SetCovMatrix(fCovNeutrino, fNeutrino);

	// Debug output of reco objects and covariance matrices before fit
	if ( fVerbosityLevel > 1 ) {
	    // Lepton
	    cout << endl << "Lepton before fit:" << endl;
	    fLepton->Print();
	    cout << endl << "Lepton covariances:" << endl
		 << " 0 = Pt, 1 = Eta, 2 = Phi " << endl;
	    fCovLepton.Print();
	    // Neutrino
	    cout << endl << "Neutrino before fit:" << endl;
	    fNeutrino->Print();
	    cout << endl << "Neutrino covariances:" << endl
		 << " 0 = Pt, 1 = Eta, 2 = Phi " << endl;
	    fCovNeutrino.Print();
	    // B-Jet
	    cout << endl <<"B-Jet before fit:" << endl;
	    fBJet->Print();
	    cout << endl << "B-jet covariances:" << endl
		 << " 0 = Pt, 1 = Eta, 2 = Phi " << endl;
	    fCovBJet.Print();
	}
	
	// ===============
	// Perform the fit
	// ===============
	DoFit();
	
	// Fill histogram with number of iterations, no weighting!
	fHist_KinFit_NbIterAll->Fill(fKinFitter->getNbIter());

	// Did KinFitter converge ? ("0" means "yes")
	if ( fKinFitter->getStatus() != 0 ) {
	    fN_NotConverged++;
	    continue;
	}
	fN_Converged++;

	// Keep chi-squares of converging fitting procedures to
	// get number of worse top candidates
	chi2[counter++] = fKinFitter->getS();

	// Chi2 smaller than that of the previous fit ?
	if ( fKinFitter->getS() > fChi2 ) {
	    continue;
	}
	
	fChi2   = fKinFitter->getS();
	fNDoF   = fKinFitter->getNDF();
	fNbIter = fKinFitter->getNbIter();

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

	// Store the current BJet and neutrino combination
	// for possible later use
	BestBJet     = fBJet;

	// Get the improved 4-momenta of the outgoing particles
 	// and store them temporarily
	fLeptonP_refit_cur.SetPxPyPzE(fKinFitter->getParticle(0)->getCurr4Vec()->Px(),
				      fKinFitter->getParticle(0)->getCurr4Vec()->Py(),
				      fKinFitter->getParticle(0)->getCurr4Vec()->Pz(),
				      fKinFitter->getParticle(0)->getCurr4Vec()->E());
	fNeutrinoP_refit_cur.SetPxPyPzE(fKinFitter->getParticle(1)->getCurr4Vec()->Px(),
					fKinFitter->getParticle(1)->getCurr4Vec()->Py(),
					fKinFitter->getParticle(1)->getCurr4Vec()->Pz(),
					fKinFitter->getParticle(1)->getCurr4Vec()->E());
	fBJetP_refit_cur.SetPxPyPzE(fKinFitter->getParticle(2)->getCurr4Vec()->Px(),
				    fKinFitter->getParticle(2)->getCurr4Vec()->Py(),
				    fKinFitter->getParticle(2)->getCurr4Vec()->Pz(),
				    fKinFitter->getParticle(2)->getCurr4Vec()->E());

	
	// Debug output of reco objects and covariance matrices after fit
	if ( fVerbosityLevel > 1 ) {
	    // Lepton
	    cout << endl << "Lepton 4-momentum after fit:" << endl;
	    HepParticle::PrintLorentzVector(fLeptonP_refit_cur);
	    // Neutrino
	    cout << endl << "Neutrino 4-momentum after fit:" << endl;
	    HepParticle::PrintLorentzVector(fNeutrinoP_refit_cur);
	    // B-Jet
	    cout << endl <<"B-Jet 4-momentum after fit:" << endl;
	    HepParticle::PrintLorentzVector(fBJetP_refit_cur);
	}
    }


    
    // Multiple top-quark candidate combinations?
    for ( Int_t i = 0; i < counter; i++ ) {
	// exploit that fChi2 is still minimum value achieved in above trials
	if ( chi2[i] > fChi2 ) {
	    fN_WorseTopCandidates++;
	    fHistChi2WorseTopCandidates->Fill(chi2[i]);
	    fHistChi2ProbWorseTopCandidates->Fill(TMath::Prob(chi2[i],fNDoF));
	}
    }
    

    
    // =======================================
    // Main leptonic top-quark fit successful?
    // =======================================
    Bool_t IsWhad = kFALSE; 

    if ( fChi2 < 1.e10 ) {
	fCutflow_tool->Fill("Convergence", GetPreTagEvtWeight());

	// No. of main fits that DID converge in this event
	fHist_KinFit_NbFitsConverged->Fill(fN_Converged, GetPreTagEvtWeight());

	Double_t prob_topfit = TMath::Prob(fChi2,fNDoF);

	// Set leptonic W four-vector
	TLorentzVector P_W = fLeptonP_refit_cur + fNeutrinoP_refit_cur;

	
	// ================
	// Apply ttbar veto
	// ================
	
	// Append reconstructed top-quark to event only if ttbar-finder didn't succeed 
	// and P(chi2) of additional hadronic W reco small enough
	Bool_t IsTtbar = kFALSE;
	
	if ( fDoTtbarVeto ) {
	    // If ttbar finder successful, draw P(chi2)_sgtop vs P(chi2)_ttbar
	    if ( chi2PROBttbarBkg != -1. ) {
		IsTtbar = kTRUE;
		fHist_Chi2ProbSgTop_vs_Chi2ProbTTbar
		    ->Fill(prob_topfit,chi2PROBttbarBkg,GetPreTagEvtWeight());
	    } else {
		fCutflow_tool->Fill("t#bar{t} Veto", GetPreTagEvtWeight());
	    }
	}



	// =====================
	// Apply hadronic W veto
	// =====================
	
	// Try to reconstruct a hadronic W decay from the jets
	// except the b-jet BestBJet used in the best top decay reconstruction.
	// This is supposed to suppress background from semileptonic
	// ttbar and Wt-channel events.
	// 
	// The whole veto idea is yet to be investigated.
	// So far only the reconstruction of additional
	// hadronic W decays is done and histograms
	// are filled.
	// 
	// ApplyWhadVeto(Chi2ProbMin, ...) returns an integer
	// after comparing the chi2-prob. of reconstruction
	// with Chi2ProbMin
	// 1 : Chi2ProbReco >= Chi2ProbMin
	// 0 : Chi2ProbReco <  Chi2ProbMin or not even
	//     convergence in hadronic W reconstruction
	// 
	// in case of convergence the hadronic W
	// is always added to fEvent as AtlWDecayJJ
	// 
	
	if ( fDoWhadVeto ) {
	    IsWhad = ApplyWhadVeto(fVetoWhadProbMin, BestBJet);
	    
	    // If ttbar finder AND Whad-fit successful
	    // (regardless of size of P(Chi2)_Whad), draw
	    // P(chi2)_sgtop / ( P(chi2)_ttbar * P(chi2)_Whad )
	    if ( IsTtbar && fChi2ProbWhad > 0 )
		fHist_Chi2ProbSgTop_over_Chi2ProbTTbarWhad
		    ->Fill(prob_topfit/(chi2PROBttbarBkg*fChi2ProbWhad),GetPreTagEvtWeight()); 
	    	    
	    if ( IsWhad ) {
		// -----------------------------------------------------------
		// Histogram: Leptonic W invar. mass vs hadronic W invar. mass
		// -----------------------------------------------------------
		
		// M_invar of leptonic W
		Float_t m_wlep = P_W.M();
		
		// Get hadronic W
		AtlWDecayJJ *w_test = 0;
		AtlWDecayJJ *Whad   = 0;
		
		for ( Int_t i = 0;  i < fEvent->GetN_WDecaysJJ(); i++ ){
		    w_test = (AtlWDecayJJ*)fEvent->GetWDecaysJJ()->At(i);
		    if ( w_test->IsSgTop_tChannel() ) {
			Whad = w_test;
		    }
		}

		
		// If leptonic AND hadronic W reco successful (PChi2Whad_min exceeded),
		// draw scatter plot of their masses
		if ( IsWhad && (m_wlep != 0) ) {	
		    fHist_M_invar_W_lep_vs_M_invar_W_had
			->Fill(m_wlep, TMath::Abs(Whad->M("REC")), GetPreTagEvtWeight());}
		
		
		// =====================================================
		// P(Chi2) of top-quark fit vs P(Chi2) of hadronic W fit
		// =====================================================	
		if ( fChi2Whad < 1.e10 ) {
		    fHist_Chi2Prob_vs_Chi2ProbWhad->Fill(prob_topfit,fChi2ProbWhad,GetPreTagEvtWeight());
		    fHist_Chi2_vs_Chi2Whad->Fill(fChi2,fChi2Whad,GetPreTagEvtWeight());
		}
	    } else {
		if ( fDoTtbarVeto && !IsTtbar ) fCutflow_tool->Fill("W_{had} Veto", GetPreTagEvtWeight());
	    }
	}
	
	
	
	
	// =============================
	// Does event survive veto fits?
	// =============================
	if ( !IsWhad && !IsTtbar ) {
	    
	    // ================================================================
	    // Add best reconstructed particles/decays to the current event
	    // in case of convergence and unsuccessful ttbar veto and Whad veto
	    // ================================================================
	    
	    
	    // Add neutrino
	    HepParticle *nu_refit = fEvent->AddNeutrino(fNeutrinoP_refit_cur.Px(),
							fNeutrinoP_refit_cur.Py(),
							fNeutrinoP_refit_cur.Pz(),
							fNeutrinoP_refit_cur.E(),
							fNeutrino->GetPdgCode());
		    
	    // Add leptonic W decay
	    AtlWDecayLNu *Wdecay = fEvent->AddWDecayLNu(P_W.Px(),
							P_W.Py(),
							P_W.Pz(),
							P_W.E(),
							fLepton,
							fLeptonP_refit_cur.Px(),
							fLeptonP_refit_cur.Py(),
							fLeptonP_refit_cur.Pz(),
							fLeptonP_refit_cur.E(),
							nu_refit,
							HepWDecay::kSgTop_tChannel);
	    
	    Wdecay->SetChi2NDoF(fChi2, fNDoF);
	    
	    // Add top decay
	    TLorentzVector P_top = P_W + fBJetP_refit_cur;
	    HepTopDecay *TopDecay = fEvent->AddTopDecay(P_top.Px(),
							P_top.Py(),
							P_top.Pz(),
							P_top.E(),
							Wdecay,
							BestBJet,
							fBJetP_refit_cur.Px(),
							fBJetP_refit_cur.Py(),
							fBJetP_refit_cur.Pz(),
							fBJetP_refit_cur.E(),
							HepTopDecay::kSgTop_tChannel);
	    
	    TopDecay->SetChi2NDoF(fChi2, fNDoF);
	    
	    // Information on reconstructed top-quark
	    if ( fVerbosityLevel > 0 ) {
		cout << endl;
		Info("ReconstructionKinFit",
		     "Possible Single-top t-channel event found:");
		TopDecay->Print();
	    }
	    
	    // Fill histograms for reco objects in KinFitter mode
	    FillKinFitLeptonHistograms();
	    FillKinFitJetHistograms();
	}

	
	// =======================
	// Fill pull distributions (already if main fit converged)
	// =======================
	// Be aware of the fact that the assignment pt, eta, phi
	// must be changed here when another particle parameterization
	// is used
	if ( fLepton->IsElectron() ){
	    fHistPullElecPt ->Fill(fPullLeptonPt,  GetPreTagEvtWeight());
	    fHistPullElecEta->Fill(fPullLeptonEta, GetPreTagEvtWeight());
	    fHistPullElecPhi->Fill(fPullLeptonPhi, GetPreTagEvtWeight());
	}  
	if ( fLepton->IsMuon() ){
	    fHistPullMuonPt ->Fill(fPullLeptonPt,  GetPreTagEvtWeight());
	    fHistPullMuonEta->Fill(fPullLeptonEta, GetPreTagEvtWeight());
	    fHistPullMuonPhi->Fill(fPullLeptonPhi, GetPreTagEvtWeight());
	}
	fHistPullEtMissMag->Fill(fPullEtMissMag, GetPreTagEvtWeight());
	fHistPullEtMissPhi->Fill(fPullEtMissPhi, GetPreTagEvtWeight());
	fHistPullBJetPt ->Fill(fPullBJetPt,  GetPreTagEvtWeight());
	fHistPullBJetEta->Fill(fPullBJetEta, GetPreTagEvtWeight());
	fHistPullBJetPhi->Fill(fPullBJetPhi, GetPreTagEvtWeight());
		
    }
    


    if ( IsWhad ) { // fill only if Whad veto was applied...
//    if ( fChi2Whad < 1.e10 ) {// fill already if Whad fit converged... ***NO GOOD IDEA!***
	// ==================================
	// Fill pull histograms of hadronic W
	// ==================================
	fHistPullJet1Pt ->Fill(fPullJet1Pt,  GetPreTagEvtWeight());
	fHistPullJet1Eta->Fill(fPullJet1Eta, GetPreTagEvtWeight());
	fHistPullJet1Phi->Fill(fPullJet1Phi, GetPreTagEvtWeight());
	fHistPullJet2Pt ->Fill(fPullJet2Pt,  GetPreTagEvtWeight());
	fHistPullJet2Eta->Fill(fPullJet2Eta, GetPreTagEvtWeight());
	fHistPullJet2Phi->Fill(fPullJet2Phi, GetPreTagEvtWeight());
    }
    

    
    // ================================
    // KinFitter performance histograms
    // ================================
    
    // No. of main fits that DID NOT converge in this event
    fHist_KinFit_NbFitsNotConverged->Fill(fN_NotConverged);

    // No. of main fits that failed numerically in this event
    fHist_KinFit_NbFitsFailedNumerically->Fill(fN_FailNumeric);

    // No. of worse top-quark candidates w.r.t. Chi2
    // in this event
    fHistNbWorseTopCandidates->Fill(fN_WorseTopCandidates);

    
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::DoFit() {
    //
    // Perform the kinematic fit for the current charged lepton,
    // neutrino and b-jet combination
    //

    // Re-set and init fitter
    fKinFitter->reset();
    fKinFitter->setMaxNbIter(fMaxNbIter);  // maximum number of iterations
    fKinFitter->setMaxDeltaS(5.e-5); // maximum deviation of the minimum function within two iterations
    fKinFitter->setMaxF(1.e-4);      // maximum value of constraints
    fKinFitter->setVerbosity(0);  // verbosity level (3 for DEBUGGING)

    // =============
    // Get 3-momenta
    // =============
    TVector3 p3_lepton   = TVector3(fLepton->P3());
    TVector3 p3_neutrino = TVector3(fNeutrino->P3());
    TVector3 p3_bjet     = TVector3(fBJet->P3());

    
    // ================
    // Define particles
    // ================
    // Necessary for execution of the fit
    TFitParticlePtEtaPhi KinFit_lepton("KinFit_lepton", "Charged lepton",
				       &p3_lepton, fLepton->Mass("PDG"), &fCovLepton);
    TFitParticlePtThetaPhi KinFit_nu("KinFit_nu", "Neutrino",
				     &p3_neutrino, 0., &fCovNeutrino);
    TFitParticlePtEtaPhi KinFit_bjet("KinFit_bjet", "BJet",
				     &p3_bjet, fB_Mass, &fCovBJet); // use b-quark mass
    fKinFitter->addMeasParticle(&KinFit_lepton);
    fKinFitter->addMeasParticle(&KinFit_nu);
    fKinFitter->setParamUnmeas(&KinFit_nu, 1); // Theta (component 1) of neutrino unmeasured
    fKinFitter->addMeasParticle(&KinFit_bjet);


    // ==================
    // Define Constraints
    // ==================

    // Definition of top-quark and W-boson mass constraints
    TFitConstraintM *MassConstraint_W = 0;
    TFitConstraintM *MassConstraint_t = 0;
    if ( fModeMass == kGauss ) {
	MassConstraint_W = new TFitConstraintMGaus("WMassConstraint",
						   "WMassConstraintGauss",
						   0, 0, fW_Mass, fW_Width);
	MassConstraint_t = new TFitConstraintMGaus("TopMassConstraint",
						   "TopMassConstraintGauss",
						   0, 0, fTop_Mass, fTop_Width);
    } else if ( fModeMass == kBW ) {
	MassConstraint_W = new TFitConstraintMBW2("WMassConstraint",
						 "WMassConstraintBW",
						 0, fW_Mass, fW_Width);
	MassConstraint_t = new TFitConstraintMBW2("TopMassConstraint",
						 "TopMassConstraintBW",
						 0, fTop_Mass, fTop_Width);
    }

        
    MassConstraint_W->addParticles1(&KinFit_lepton, &KinFit_nu);
    MassConstraint_t->addParticles1(&KinFit_lepton, &KinFit_nu, &KinFit_bjet);
    fKinFitter->addConstraint(MassConstraint_W);
    fKinFitter->addConstraint(MassConstraint_t);

    // definition of transverse momentum-conservation



//     vector<TAbsFitParticle*> vec_constraint;
//     vec_constraint.push_back(&FitExec_l1);
//     vec_constraint.push_back(&FitExec_l2);
    
//     TFitConstraintEpGaus PxCons(&vec_constraint, TFitConstraintEp::pX,0.0001,8.1);
//     TFitConstraintEpGaus PyCons(&vec_constraint, TFitConstraintEp::pY,0.0001,8.1);
		
    
    // =========================
    // Perform Kinematic Fitting
    // =========================
    fKinFitter->fit();

    // testing a fix for large chi square values:
    // get mass terms in chi square function and pull values
    if ( fKinFitter->getS() < fChi2 ){
	//
	// Get mass terms
	//
	const TMatrixD *MassParameterW   = MassConstraint_W->getParCurr();
	const TMatrixD *MassParameterTop = MassConstraint_t->getParCurr();

	Double_t mu_W   = (*MassParameterW)(0, 0); // *** hier geht das???
	Double_t mu_Top = (*MassParameterTop)(0, 0);

	// fMassConstraintParameters = mu_W*mu_W+mu_Top*mu_Top;
	
	if ( fModeMass == kBW ){
	    fMassConstraintParameterTop = mu_Top;
	    fMassConstraintParameterW   = mu_W;
	    fMassConstraintParameterAll =
		fMassConstraintParameterTop*fMassConstraintParameterTop +
		fMassConstraintParameterW*fMassConstraintParameterW;
	}
	else {
	    fMassConstraintParameterTop = (mu_Top-1)/(fTop_Width)*fTop_Mass;// *** wie wird das verrechnet? Korrekt?
	    fMassConstraintParameterW   = (mu_W-1)/(fW_Width)*fW_Mass;
	    fMassConstraintParameterAll =
		fMassConstraintParameterTop*fMassConstraintParameterTop +
		fMassConstraintParameterW*fMassConstraintParameterW;
	}

	//
	// Get pull values ( pull := [value(fit) - value(initial)] / sqrt[variance of corrections] )
	//
	const TMatrixD ChargedLeptonPull = *(((TAbsFitParticle*)fKinFitter->getParticle(0))->getPull());
	const TMatrixD NeutrinoPull = *(((TAbsFitParticle*)fKinFitter->getParticle(1))->getPull());
	const TMatrixD BJetPull = *(((TAbsFitParticle*)fKinFitter->getParticle(2))->getPull());
	
	fPullLeptonPt  = ChargedLeptonPull(0,0);
	fPullLeptonEta = ChargedLeptonPull(1,0);
	fPullLeptonPhi = ChargedLeptonPull(2,0);
	fPullEtMissMag = NeutrinoPull(0,0);
	fPullEtMissPhi = NeutrinoPull(2,0); 
	fPullBJetPt  = BJetPull(0,0);
	fPullBJetEta = BJetPull(1,0);
	fPullBJetPhi = BJetPull(2,0);
    }
    
    
    if ( fKinFitter->matrixInvFailed() ) fN_FailNumeric++;

    switch ( fKinFitter->getStatus() ) {
	case 0: {
	    fHist_KinFit_KinFitterStatus->AddBinContent(1);
	    break;
	}
	case 1: {
	    fHist_KinFit_KinFitterStatus->AddBinContent(2);
	    break;
	}
	default:{
	    Error("DoFit","Current KinFitter status not defined for this histogram.");
	}
    }

        
    // Fitting okay ?
    if ( fKinFitter->getS() < 0. ) {
	Error("DoFit",
	      "Chi2 negative!!! Possibly bad input of covariance matrices. Abort!");
//	gSystem->Abort(0);
    }

    // Clean-up
    delete MassConstraint_W;
    delete MassConstraint_t;
}

//____________________________________________________________________

Bool_t AtlSgTop_tChannelFinder::ApplyWhadVeto(Double_t Chi2ProbMin,
					      AtlJet *BestBJet) {
    //
    // Loop over the jet list except the b jet candidate from top decay
    //
    AtlJet *BestJet1 = 0;           // jet 1 of best hadronic W reconstruction
    AtlJet *BestJet2 = 0;           // jet 2 of best hadronic W reconstruction
    fChi2Whad = 1.e10;
    fChi2ProbWhad = 0;

    // Nested loop over all jets.
    // Set the covariance matrices already here
    for ( Int_t i = 0; i < fWhadJets->GetEntries() - 1; i++ ){
	fWJet1 = (AtlJet*)fWhadJets->At(i);
	if ( fWJet1 == BestBJet) continue;
	
	// Set cov. matrix of W-jet 1
	SetCovMatrix(fCovWJet1, fWJet1);
	
	for ( Int_t j = i+1; j < fWhadJets->GetEntries(); j++ ){
	    fWJet2 = (AtlJet*)fWhadJets->At(j);
	    if ( fWJet2 == BestBJet) continue;

	    // Set cov. matrix of W-jet 2
	    SetCovMatrix(fCovWJet2, fWJet2);

	    // Debug output of reco objects and covariance matrices 
	    if ( fVerbosityLevel > 1 ) {
		// W-Jet 1
		cout << endl <<"W-jet 1 before fit:" << endl;
		fWJet1->Print();
		cout << endl << "W-jet 1 covariances:" << endl
		     << " 0 = Pt, 1 = Eta, 2 = Phi " << endl;
		fCovWJet1.Print();
		// W-Jet 2
		cout << endl <<"W-jet 2 before fit:" << endl;
		fWJet2->Print();
		cout << endl <<  "W-jet 2 covariances:" << endl
		     << " 0 = Pt, 1 = Eta, 2 = Phi " << endl;
		fCovWJet2.Print();
	    } 
	    
	    // Perform fit
	    DoHadronicWFit();

	    
	    // Debug output of complete covariance matrix
	    if ( fVerbosityLevel > 1 ) {
		// Before fit
		cout << endl << "Complete covariance matrix before fit:" << endl;
		cout<<" 0 - 2 = Pt, Eta, Phi (W-jet 1)"<<endl;
		cout<<" 3 - 5 = Pt, Eta, Phi (W-jet 2)"<<endl;
		cout<<" 6     = ? "<<endl;
		fKinFitter_Whad->getCovMatrix()->Print();
		// After fit
		cout << endl << "Complete covariance matrix after fit:" << endl;
		fKinFitter_Whad->getCovMatrixFit()->Print();
	    }


	    // Did KinFitter converge ? ("0" means "yes")
	    if ( fKinFitter_Whad->getStatus() != 0 ) continue;

	    // Was Chi2 improved by current fit?
	    if ( fKinFitter_Whad->getS() < fChi2Whad ){
		
		fChi2Whad = fKinFitter_Whad->getS();
		fNDoFWhad = fKinFitter_Whad->getNDF();
		BestJet1 = fWJet1;		
		BestJet2 = fWJet2;
		
	    }
	}
    }

    //
    // Add hadronic W decay to the event in case of reconstruction
    //
    if ( fChi2Whad >= 1.e10 ) return kFALSE;

    // Debug output of reco objects and covariance matrices after fit
    if ( fVerbosityLevel > 1 ) {
	// Jet 1
	cout << endl <<"Jet 1 4-momentum after Whad veto fit:" << endl;
	HepParticle::PrintLorentzVector(fJet1P_refit_cur);
	// Jet 2
	cout << endl <<"Jet 2 4-momentum after Whad veto fit:" << endl;
	HepParticle::PrintLorentzVector(fJet2P_refit_cur);
    }
		
    TLorentzVector P_Whad = fJet1P_refit_cur + fJet2P_refit_cur;//*Jet1KinFit + *Jet2KinFit;
    AtlWDecayJJ *Whad = fEvent->AddWDecayJJ(
	P_Whad.Px(),
	P_Whad.Py(),
	P_Whad.Pz(),
	P_Whad.E(),
	BestJet1, BestJet2,
	fJet1P_refit_cur.Px(),
	fJet1P_refit_cur.Py(),
	fJet1P_refit_cur.Pz(),
	fJet1P_refit_cur.E(),
	fJet2P_refit_cur.Px(),
	fJet2P_refit_cur.Py(),
	fJet2P_refit_cur.Pz(),
	fJet2P_refit_cur.E(),
	HepWDecay::kSgTop_tChannel);


    // Information on reconstructed hadronic W
    if ( fVerbosityLevel > 0 ) {
	cout << endl;
	Info("ApplyWhadVeto","Possible hadronic W found:");
	Whad->Print();
	cout << endl << "Refitted jet 1 of hadronic W:" << endl;
	Whad->GetJet1()->Print();
	cout << endl << "Refitted jet 2 of hadronic W:" << endl;
	Whad->GetJet2()->Print();
    }

    Whad->SetChi2NDoF(fChi2Whad, fNDoFWhad);

    // Fill hadronic W histograms
    fChi2ProbWhad = TMath::Prob(fChi2Whad, fNDoFWhad);
    Float_t mt_whad       = Whad->Mperp();
    Float_t RapWhad       = (Whad->P()).Rapidity();
    
    fHistWHadReco_Mt ->Fill(TMath::Abs(mt_whad), GetPreTagEvtWeight());
    fHistWHadReco_M ->Fill(TMath::Abs(Whad->M("REC")), GetPreTagEvtWeight());
    fHistWHadReco_pt ->Fill(Whad->Pt(), GetPreTagEvtWeight());
    fHistWHadReco_rap ->Fill(RapWhad, GetPreTagEvtWeight());
    fHistWHadReco_phi->Fill(Whad->Phi(), GetPreTagEvtWeight());
    fHistWHadReco_chi2->Fill(fChi2Whad, GetPreTagEvtWeight());
    fHistWHadReco_chi2prob->Fill(fChi2ProbWhad, GetPreTagEvtWeight());
    

    if ( fChi2ProbWhad < Chi2ProbMin ) return kFALSE;
    return kTRUE;
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::DoHadronicWFit() {
    //
    // Perform the kinematic fit for the current
    // jet1 and jet2 combination
    //
    // The hypothesis is Whad -> jet1, jet2
    //
    
    // Re-set and init fitter
    fKinFitter_Whad->reset();
    fKinFitter_Whad->setMaxNbIter(fMaxNbIter);   // maximum number of iterations
    fKinFitter_Whad->setMaxDeltaS(5.e-5); // maximum deviation of the minimum function within two iterations
    fKinFitter_Whad->setMaxF(1.e-4);      // maximum value of constraints
    fKinFitter_Whad->setVerbosity(0); // verbosity level (3 for DEBUGGING)

    // =============
    // Get 3-momenta
    // =============
    TVector3 p3_jet1 = TVector3(fWJet1->P3());
    TVector3 p3_jet2 = TVector3(fWJet2->P3());

    // ================
    // Define particles
    // ================
    // Necessary for execution of the fit
    TFitParticlePtEtaPhi KinFit_jet1("KinFit_jet1", "Jet1",
				     &p3_jet1, 0., &fCovWJet1);
    TFitParticlePtEtaPhi KinFit_jet2("KinFit_jet2", "Jet2",
				     &p3_jet2, 0., &fCovWJet2);
    fKinFitter_Whad->addMeasParticle(&KinFit_jet1);
    fKinFitter_Whad->addMeasParticle(&KinFit_jet2);

    // ==================
    // Define Constraints
    // ==================
//    if ( fModeMass == kGauss ) {
    TFitConstraintMGaus *ConstraintM_Whad = new TFitConstraintMGaus("WHadMassConstraint",
								    "WMassConstraint for hadronic decay",
								    0, 0, fW_Mass, fW_Width);
    ConstraintM_Whad->addParticles1(&KinFit_jet1, &KinFit_jet2);
    fKinFitter_Whad->addConstraint(ConstraintM_Whad);
    
    // =========================
    // Perform Kinematic Fitting
    // =========================
    fKinFitter_Whad->fit();

    // Get the pull values    
    if ( fKinFitter_Whad->getS() < fChi2Whad ){
	//
	// pull := [value(fit) - value(initial)] / sqrt[variance of corrections] )
	//
	const TMatrixD Jet1Pull = *(((TAbsFitParticle*)fKinFitter_Whad->getParticle(0))->getPull());
	const TMatrixD Jet2Pull = *(((TAbsFitParticle*)fKinFitter_Whad->getParticle(1))->getPull());

	fPullJet1Pt  = Jet1Pull(0,0);
	fPullJet1Eta = Jet1Pull(1,0);
	fPullJet1Phi = Jet1Pull(2,0);
	fPullJet2Pt  = Jet2Pull(0,0);
	fPullJet2Eta = Jet2Pull(1,0);
	fPullJet2Phi = Jet2Pull(2,0);

    }
    
    fJet1P_refit_cur.SetPxPyPzE(
	fKinFitter_Whad->getParticle(0)->getCurr4Vec()->Px(),
	fKinFitter_Whad->getParticle(0)->getCurr4Vec()->Py(),
	fKinFitter_Whad->getParticle(0)->getCurr4Vec()->Pz(),
	fKinFitter_Whad->getParticle(0)->getCurr4Vec()->E());
    fJet2P_refit_cur.SetPxPyPzE(
	fKinFitter_Whad->getParticle(1)->getCurr4Vec()->Px(),
	fKinFitter_Whad->getParticle(1)->getCurr4Vec()->Py(),
	fKinFitter_Whad->getParticle(1)->getCurr4Vec()->Pz(),
	fKinFitter_Whad->getParticle(1)->getCurr4Vec()->E());

    
    // Fitting okay ?
    if ( fKinFitter_Whad->getS() < 0. ) {
	Error("DoHadronicWFit",
	      "Chi2 negative!!! Possibly bad input of covariance matrices. Abort!");
//	gSystem->Abort(0);
    }
    
    // Clean-up
    delete ConstraintM_Whad;
    

	
//     } else if ( fModeMass == kBW ) {
//         TFitConstraintMBW2 *ConstraintM_Whad = new TFitConstraintMBW2("WHadMassConstraint",
// 								     "WMassConstraint for hadronic decay",
// 								     0, fW_Mass, fW_Width);
// 	ConstraintM_Whad->addParticles1(&KinFit_jet1, &KinFit_jet2);
// 	fKinFitter_Whad->addConstraint(ConstraintM_Whad);

// 	// =========================
// 	// Perform Kinematic Fitting
// 	// =========================
// 	fKinFitter_Whad->fit();
        
// 	// Fitting okay ?
// 	if ( fKinFitter_Whad->getS() < 0. ) {
// 	    Error("DoHadronicWFit",
// 		  "Chi2 negative!!! Possibly bad input of covariance matrices. Abort!");
// 	    gSystem->Abort(0);
// 	}
	
// 	// Clean-up
// 	delete ConstraintM_Whad;
//     }



    

	//
	// earlier approach:
	//
	
//     // ==================
//     // Define Constraints
//     // ==================
//     TFitConstraintM *ConstraintM_Whad = 0;

//     if ( fModeMass == kGauss ) {
// 	ConstraintM_Whad = new TFitConstraintMGaus("WHadMassConstraint",
// 						   "WMassConstraint for hadronic decay",
// 						   0, 0, fW_Mass, fW_Width);
//     } else if ( fModeMass == kBW ) {
// 	ConstraintM_Whad = new TFitConstraintMBW2("WHadMassConstraint",
// 						  "WMassConstraint for hadronic decay",
// 						  0, fW_Mass, fW_Width);
//     }
    
//     ConstraintM_Whad->addParticles1(&KinFit_jet1, &KinFit_jet2);
//     fKinFitter_Whad->addConstraint(ConstraintM_Whad);

    // =========================
    // Perform Kinematic Fitting
    // =========================
//     fKinFitter_Whad->fit();
        
//     // Fitting okay ?
//     if ( fKinFitter_Whad->getS() < 0. ) {
// 	Error("DoHadronicWFit",
// 	      "Chi2 negative!!! Possibly bad input of covariance matrices. Abort!");
// 	gSystem->Abort(0);
//     }

}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::FillHistograms() {
    //
    // Fill histograms
    //
    // This method will be called only for events surviving the
    // ProcessCut() routine of main selector analysis.
    // There, a top-quark reconstructed by the KinFitter is required.
    //
    
    // Fill KinFitter histograms
    FillKinFitHistograms();


    
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::FillKinFitHistograms() {
    //
    // Fill histograms for KinFitter mode
    // ( after Whad-veto but before final P(Chi2)-cut ! )
    //

    // Fill No. of iterations histogram
    fHist_KinFit_NbIterCand->Fill(fNbIter); // no weighting, this is a performance study

    // Fill histogram with final constraint value of chosen candidate fitting procedure
    fHist_KinFit_ConsValue->Fill(fConsValue);


    // Get Top Decays
    HepTopDecay* t_test = 0;
//    Double_t chi2probTTBar = -1;
    Int_t n_tops = fEvent->GetN_TopDecays();
    if ( n_tops == 0 ) {
	Error("FillKinFitHistograms","No top quark reconstructed. Abort!");
	gSystem->Abort();
    }
	    
    for ( Int_t i = 0;  i < n_tops; i++ ){
// 	// Classify top decays present in event
 	t_test = (HepTopDecay*)fEvent->GetTopDecays()->At(i);
// 	if ( t_test->IsTTBar() && (chi2probTTBar == -1)  ){
// 	    chi2probTTBar = t_test->GetProb();
// 	}
	if ( t_test->IsSgTop_tChannel() ){
	    fTop = t_test;
	}
    }


    fWBoson           = (AtlWDecayLNu*)fTop->GetWDecay();
    HepParticle  *lep = fWBoson->GetChargedLepton();
    HepParticle  *nu  = fWBoson->GetNeutrino();
    Double_t chi2     = fWBoson->GetChi2();
    Double_t chi2prob = fWBoson->GetProb();
    
//     if ( chi2probTTBar != -1 ) {
// 	fHist_Chi2ProbSgTop_vs_Chi2ProbTTbar->Fill(chi2prob,chi2probTTBar,GetPreTagEvtWeight());
// 	if ( fChi2ProbWhad != 0 ) 
//     }
    
    // More likely to be single-top event than semi-leptonic ttbar?
//     if ( chi2probTTBar < chi2prob ) {
// 	SetCutFlow("t#bar{t} veto");
	
//     // testing a fix for large chi square values
//     if ( fMassConstraintParameters > 3. ) {
// 	chi2 -= fMassConstraintParameters;
// 	chi2prob = TMath::Prob( chi2, 1 );
//     }
    
	// W-boson
	Float_t pt_w  = fWBoson->Pt();
	Float_t mt_w  = fWBoson->Mperp();
	Float_t m_w   = fWBoson->M("REC");
	Float_t eta_w = fWBoson->Eta();
	Float_t phi_w = fWBoson->Phi();
	Float_t RapW  = (fWBoson->P()).Rapidity();
	TLorentzVector P_w = fWBoson->P();
	

	// Top-Quark
	Float_t pt_top  = fTop->Pt();
	Float_t mt_top  = fTop->Mperp();
	Float_t m_top   = fTop->M("REC");
	Float_t eta_top = fTop->Eta();
	Float_t phi_top = fTop->Phi();
	Float_t Rapt    = (fTop->P()).Rapidity();
	TVector3 P3_top = fTop->P3();
	TLorentzVector P_top = fTop->P();
	
	// Lepton
	TLorentzVector P_lep = fWBoson->GetChargedLepton()->P();
	
	// Neutrino
	Float_t e_nu   = nu->E();
	Float_t pt_nu  = nu->Pt();
	Float_t eta_nu = nu->Eta();
	Float_t phi_nu = nu->Phi();
	
	// b-Jet
	TLorentzVector P_bjet_fit   = fTop->GetP_BJet();


	//
	// Chi2 and Chi2Prob histograms of main top-quark fit BEFORE P(Chi2)-cut on main fit
	//
	if ( fWBoson->GetChargedLepton()->IsElectron() ) {
	    // Fill chi-square and chi-square prob. histograms
	    fHistChi2_e->Fill(chi2, GetPreTagEvtWeight());
	    fHistChi2Prob_e->Fill(chi2prob, GetPreTagEvtWeight());
	    
	    fHistChi2_both->Fill(chi2, GetPreTagEvtWeight());
	    fHistChi2_both_Log->Fill(chi2, GetPreTagEvtWeight());
	    fHistChi2Prob_both->Fill(chi2prob, GetPreTagEvtWeight());
	    fHistChi2Prob_both_Log->Fill(chi2prob, GetPreTagEvtWeight());
	    
	    fHist_Chi2Prob_vs_M_invar_W->Fill(chi2prob, m_w, GetPreTagEvtWeight());
	    fHist_Chi2Prob_vs_M_invar_top->Fill(chi2prob,m_top, GetPreTagEvtWeight());
	    
	}
	
	if ( fWBoson->GetChargedLepton()->IsMuon() ) {
	    // Fill chi-square and chi-square prob. histograms
	    fHistChi2_mu->Fill(chi2, GetPreTagEvtWeight());
	    fHistChi2Prob_mu->Fill(chi2prob, GetPreTagEvtWeight());
	    
	    fHistChi2_both->Fill(chi2, GetPreTagEvtWeight());
	    fHistChi2_both_Log->Fill(chi2, GetPreTagEvtWeight());
	    fHistChi2Prob_both->Fill(chi2prob, GetPreTagEvtWeight());
	    fHistChi2Prob_both_Log->Fill(chi2prob, GetPreTagEvtWeight());

	    
	    fHist_Chi2Prob_vs_M_invar_W->Fill(chi2prob, m_w, GetPreTagEvtWeight());
	    fHist_Chi2Prob_vs_M_invar_top->Fill(chi2prob, m_top, GetPreTagEvtWeight());
	}
	
	
	
	// -------------------------------
	// P(Chi2) above minimum required?
	// -------------------------------    
	if ( chi2prob > fChi2ProbMin ) {
	    Int_t pchi2_min = (Int_t)(fChi2ProbMin*100);
	    TString pchi2_min_str = Form("P(#chi^{2}) > %d",pchi2_min);
	    pchi2_min_str.Append("%");
	    fCutflow_tool->Fill(pchi2_min_str, GetPreTagEvtWeight());
    
	    if ( fVerbosityLevel > 0 ) {
		cout << endl;
		Info("FillKinFitHistograms","Found tchannel candidate with P(Chi2) = %f. In run %d evt %llu lb %d",chi2prob,fEvent->RunNr(),fEvent->EventNr(),fEvent->LumiBlock());
	    }

	    // Total yield histogram
	    fHistTotalYield_allCuts->AddBinContent(1,GetPreTagEvtWeight());
	    

	    // ===========
	    // Forward jet
	    // ===========

	    // Get leading jet of generic jet list
	    fLeadFwdJet = (AtlJet*)fJets->At(0);
	    // Avoid catching the b-jet from top-reco again
	    if ( fLeadFwdJet == (AtlJet*)fTop->GetBJetOrig() ) {
		fLeadFwdJet = (AtlJet*)fJets->At(1);
	    }
		
		
	    Float_t fwdjet_eta = 0;
	    TVector3 fwdjet_P3;
	    if ( fLeadFwdJet != 0 ) { // should always be the case!
		fwdjet_eta = fLeadFwdJet->Eta();
		fwdjet_P3 = fLeadFwdJet->P3();
	    }

	
	    //
	    // Cut on vectorial sum of pt(top) and pt(2ndLeadJet) in combination with eta(2ndLeadJet)
	    //
	    TVector2 top_p2 = TVector2(fTop->Px(),fTop->Py());
	    Double_t delta_p2_mod = 0;
	    TVector2 jet2ndlead_p2 = TVector2(fLeadFwdJet->P3().Px(),fLeadFwdJet->P3().Py());
	    delta_p2_mod = (top_p2 + jet2ndlead_p2).Mod();
	    

	    
// 	    //
// 	    // Some additional cuts (just try):
// 	    // 
// 	    if ( TMath::Abs(eta_top) > 3.2 ) return;
// 	    fCutflow_tool->Fill("#eta_{Top} < 3.2", GetPreTagEvtWeight());

// 	    if ( fNJets < 4 ) {
// 		if ( TMath::Abs(eta_w) > 3. ) return;
// 		fCutflow_tool->Fill("#eta_{W} < 3", GetPreTagEvtWeight());
// 	    } else {
// 		if ( TMath::Abs(eta_w) > 2. ) return;
// 		fCutflow_tool->Fill("#eta_{W} < 2", GetPreTagEvtWeight());
// 	    }
	    
// 	    if ( pt_nu < 26. ) return;
// 	    fCutflow_tool->Fill("p_{T}^{#nu} > 26 GeV", GetPreTagEvtWeight());
	    
//  // 	    if ( delta_p2_mod > 45. ) return;
// // 	    fCutflow_tool->Fill("|p_{T}^{top}+p_{T}^{2ndLeadJet}| < 45 GeV", GetPreTagEvtWeight());

// 	    if ( fNJets < 4 ) {
// 		if ( GetSumPt(fTop->Px(),fTop->Py(),fLeadFwdJet->Px(),fLeadFwdJet->Py()) > 50. )
// 		    return;
// 		fCutflow_tool->Fill("|p_{T}^{top}+p_{T}^{LeadFwdJet}| < 50 GeV", GetPreTagEvtWeight());
// 	    } else {
// 		if ( GetSumPt(fTop->Px(),fTop->Py(),fLeadFwdJet->Px(),fLeadFwdJet->Py()) > 110. )
// 		    return;
// 		fCutflow_tool->Fill("|p_{T}^{top}+p_{T}^{LeadFwdJet}| < 110 GeV", GetPreTagEvtWeight());
// 	    }

	    HepParticle *prt = new HepParticle(0, fTop->GetP_BJet().X(), fTop->GetP_BJet().Y(),
					       fTop->GetP_BJet().Z(), fTop->GetP_BJet().E(), 0); 
// 	    if ( lep->DeltaR(prt) > 3.3 ) return;
// 	    fCutflow_tool->Fill("#Delta R(lepton-bjet) < 3.3", GetPreTagEvtWeight());

	    //
	    // Mass effective
	    //
	    Double_t M_eff = fTop->Pt();      
	    for ( Int_t i=0; i<fJets->GetEntries(); i++ ){
		if ( fJets->At(i) != fTop->GetBJetOrig() )
		    M_eff += ((HepJet*)fJets->At(i))->Pt();
	    }

// 	    if ( fNJets == 4 ) {
// 		if ( M_eff > 500 ) return;
// 		fCutflow_tool->Fill("M_{eff} < 500", GetPreTagEvtWeight());
// 	    }


	    //
	    // Chi2 and Chi2Prob histograms of main top-quark fit AFTER P(Chi2)-cut on main fit
	    //
	    if ( fWBoson->GetChargedLepton()->IsElectron() ) {
	    // Fill chi-square and chi-square prob. histograms
	    fHistChi2_e_cut->Fill(chi2, GetPreTagEvtWeight());
	    fHistChi2Prob_e_cut->Fill(chi2prob, GetPreTagEvtWeight());
	    
	    fHistChi2_both_cut->Fill(chi2, GetPreTagEvtWeight());
	    fHistChi2_both_cut_Log->Fill(chi2, GetPreTagEvtWeight());
	    fHistChi2Prob_both_cut->Fill(chi2prob, GetPreTagEvtWeight());
	    fHistChi2Prob_both_cut_Log->Fill(chi2prob, GetPreTagEvtWeight());
	    }

	    if ( fWBoson->GetChargedLepton()->IsMuon() ) {
	    // Fill chi-square and chi-square prob. histograms
	    fHistChi2_mu_cut->Fill(chi2, GetPreTagEvtWeight());
	    fHistChi2Prob_mu_cut->Fill(chi2prob, GetPreTagEvtWeight());
	    
	    fHistChi2_both_cut->Fill(chi2, GetPreTagEvtWeight());
	    fHistChi2_both_cut_Log->Fill(chi2, GetPreTagEvtWeight());
	    fHistChi2Prob_both_cut->Fill(chi2prob, GetPreTagEvtWeight());
	    fHistChi2Prob_both_cut_Log->Fill(chi2prob, GetPreTagEvtWeight());
	    }

	    

	    //
	    // Histogram of modulus of
	    // ( Pt-vector of top-quark ) +
	    // ( Pt-vector of 2nd lead. jet not chosen as b-jet for top-reco )
	    fHistDeltaP2Mod_Top_2ndLeadJet->Fill(delta_p2_mod, GetPreTagEvtWeight());
	    // 2D correlation plot of above modulus vs eta of 2ndleadJet
	    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_fine->Fill(delta_p2_mod, fwdjet_eta, GetPreTagEvtWeight());
	    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet->Fill(delta_p2_mod, fwdjet_eta, GetPreTagEvtWeight());
	    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins->Fill(delta_p2_mod, fwdjet_eta, GetPreTagEvtWeight());
	    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins2->Fill(delta_p2_mod, fwdjet_eta, GetPreTagEvtWeight());
	    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins3->Fill(delta_p2_mod, fwdjet_eta, GetPreTagEvtWeight());
	    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_coarse->Fill(delta_p2_mod, fwdjet_eta, GetPreTagEvtWeight());
	    
	    //
	    // Eta of 2ndLeadJet (Fwd jet, not b-jet from fit)
	    //
	    fHistEta2ndLeadJet->Fill(fwdjet_eta, GetPreTagEvtWeight());

	    //
	    // Angle between forward jet and top (no boosts)
	    //
	    Double_t angle_fwdjet_top = (fLeadFwdJet->P()).Angle(P_top.Vect());
	    fHistAngleFwdJetTopQuark->Fill(angle_fwdjet_top, GetPreTagEvtWeight());
	    fHistCosAngleFwdJetTopQuark->Fill(TMath::Cos(angle_fwdjet_top), GetPreTagEvtWeight());




	    //
	    // Jet bin counts for successful reco
	    //
	    if ( fNJets == 2 )
		fHistJetBins_fit->Fill("2",GetPreTagEvtWeight());
	    if ( fNJets == 3 )
		fHistJetBins_fit->Fill("3",GetPreTagEvtWeight());
	    if ( fNJets == 4 )
		fHistJetBins_fit->Fill("4",GetPreTagEvtWeight());
	    if ( fNJets >= 4 )
		fHistJetBins_fit->Fill("#geq 4",GetPreTagEvtWeight());
	    if ( ( fNJets >= 2 ) && ( fNJets <= 4 ) )
		fHistJetBins_fit->Fill("2 #leq Njets #leq 4",GetPreTagEvtWeight());

	    
	    // ----------------------------------
	    // Cut on modulus & eta of 2ndLeadJet -- **too restrictive!**
	    // ----------------------------------
	//     if ( ( delta_p2_mod < 50. ) && ( TMath::Abs(fLeadFwdJet->Eta()) > 2.5 ) ) {
// 		SetCutFlow("|p_{t}^{top}+p_{t}^{2ndLeadJet}| < 50 GeV & |#eta_{2ndLeadJet}| > 2.5");}

	    
	    // ------------------------
	    // Cut on eta of 2ndLeadJet
	    // ------------------------
// 	    if ( TMath::Abs(fLeadFwdJet->Eta()) > 2.5 ) {
// 	        SetCutFlow("|#eta_{2ndLeadJet}| > 2.5");    	    
	    
	    
		// =====================================================================
		// Additional Histograms after P(Chi2)-cut and Pt-vector sum and eta cut
		// =====================================================================
		fHist_M_invar_W_vs_M_invar_top->Fill(m_w, m_top, GetPreTagEvtWeight());
		fHist_M_invar_W_over_M_invar_top->Fill((m_w/m_top), GetPreTagEvtWeight());
		
		// W -> e nu
		if ( fWBoson->GetChargedLepton()->IsElectron() ) {
		    
		    // Fill electron histograms of reconstructed W
		    fHistWreco_Mt_e ->Fill(TMath::Abs(mt_w), GetPreTagEvtWeight());
		    fHistWreco_M_e ->Fill(m_w, GetPreTagEvtWeight());
		    fHistWreco_phi_e->Fill(phi_w, GetPreTagEvtWeight());
		    fHistWreco_pt_e ->Fill(pt_w, GetPreTagEvtWeight());
		    fHistWreco_eta_e ->Fill(eta_w, GetPreTagEvtWeight());
		    fHistWreco_rap_e ->Fill(RapW, GetPreTagEvtWeight());
		    
		    fHistWreco_Mt   ->Fill(TMath::Abs(mt_w), GetPreTagEvtWeight());
		    fHistWreco_M   ->Fill(m_w, GetPreTagEvtWeight());
		    fHistWreco_phi  ->Fill(phi_w, GetPreTagEvtWeight());
		    fHistWreco_pt   ->Fill(pt_w, GetPreTagEvtWeight());
		    fHistWreco_eta   ->Fill(eta_w, GetPreTagEvtWeight());
		    fHistWreco_rap ->Fill(RapW, GetPreTagEvtWeight());
		    
		    //  Fill electron histograms of reconstructed top-quark
		    fHistTopReco_Mt_e ->Fill(TMath::Abs(mt_top), GetPreTagEvtWeight());
		    fHistTopReco_M_e ->Fill(m_top, GetPreTagEvtWeight());
		    fHistTopReco_phi_e->Fill(phi_top, GetPreTagEvtWeight());
		    fHistTopReco_pt_e ->Fill(pt_top, GetPreTagEvtWeight());
		    fHistTopReco_eta_e ->Fill(eta_top, GetPreTagEvtWeight());
		    fHistTopReco_rap_e ->Fill(Rapt, GetPreTagEvtWeight());
			
		    fHistTopReco_Mt   ->Fill(TMath::Abs(mt_top), GetPreTagEvtWeight());
		    fHistTopReco_M   ->Fill(m_top, GetPreTagEvtWeight());
		    fHistTopReco_phi  ->Fill(phi_top, GetPreTagEvtWeight());
		    fHistTopReco_pt   ->Fill(pt_top, GetPreTagEvtWeight());
		    fHistTopReco_eta  ->Fill(eta_top, GetPreTagEvtWeight());
				    
		    fHistTopReco_Amount_P3->Fill(P3_top.Mag(), GetPreTagEvtWeight());
		    if ( fLeadFwdJet != 0 ){
			fHistTopReco_DeltaEtaFwdJet->Fill((eta_top - fwdjet_eta), GetPreTagEvtWeight());
		        fHistTopReco_DeltaPhiFwdJet->Fill(TMath::Abs(fLeadFwdJet->DeltaPhi(fTop)),GetPreTagEvtWeight());
			fHistTopReco_SumPxPyFwdJet->Fill(GetSumPt(fTop->Px(),fTop->Py(),fLeadFwdJet->Px(),fLeadFwdJet->Py()),GetPreTagEvtWeight());
			fHistTopReco_DeltaP3FwdJet->Fill(P3_top.Mag()-fwdjet_P3.Mag(),GetPreTagEvtWeight());
			fHistTopReco_DeltaPxFwdJet->Fill(fTop->Px()+fLeadFwdJet->Px(),GetPreTagEvtWeight());
			fHistTopReco_DeltaPyFwdJet->Fill(fTop->Py()+fLeadFwdJet->Py(),GetPreTagEvtWeight());
		    }
		    fHistTopReco_rap  ->Fill(Rapt, GetPreTagEvtWeight());
		    
		    //  Fill electron histograms of reconstructed neutrino
		    fHistNuReco_phi_e->Fill(phi_nu, GetPreTagEvtWeight());
		    fHistNuReco_pt_e ->Fill(pt_nu, GetPreTagEvtWeight());
		    fHistNuReco_eta_e ->Fill(eta_nu, GetPreTagEvtWeight());
		    fHistNuReco_E_e ->Fill(e_nu, GetPreTagEvtWeight());
		    
		    fHistNuReco_phi  ->Fill(phi_nu, GetPreTagEvtWeight());
		    fHistNuReco_pt   ->Fill(pt_nu, GetPreTagEvtWeight());
		    fHistNuReco_eta   ->Fill(eta_nu, GetPreTagEvtWeight());
		    fHistNuReco_E   ->Fill(e_nu, GetPreTagEvtWeight());
			
// 			// Fill chi-square and chi-square prob. histograms
// 			// fHistChi2_e->Fill(chi2, GetPreTagEvtWeight());
// 	fHistChi2Prob_e->Fill(chi2prob, GetPreTagEvtWeight());
			
// 	fHistChi2_both->Fill(chi2, GetPreTagEvtWeight());
// 	fHistChi2Prob_both->Fill(chi2prob, GetPreTagEvtWeight());
			
// 	fHist_Chi2Prob_vs_M_invar_W->Fill(chi2prob, m_w, GetPreTagEvtWeight());
// 	fHist_Chi2Prob_vs_M_invar_top->Fill(chi2prob, m_top, GetPreTagEvtWeight());
			
			
		    if ( fWBoson->GetChargedLepton()->IsNegative() ) {
			fHistWreco_Mt_e_minus ->Fill(TMath::Abs(mt_w), GetPreTagEvtWeight());
			fHistWreco_M_e_minus ->Fill(m_w, GetPreTagEvtWeight());
			fHistWreco_phi_e_minus->Fill(phi_w, GetPreTagEvtWeight());
			fHistWreco_pt_e_minus ->Fill(pt_w, GetPreTagEvtWeight());
			fHistWreco_eta_e_minus ->Fill(eta_w, GetPreTagEvtWeight());
			fHistWreco_rap_e_minus ->Fill(RapW, GetPreTagEvtWeight());
			
			fHistWreco_Mt_minus ->Fill(TMath::Abs(mt_w), GetPreTagEvtWeight());
			fHistWreco_M_minus ->Fill(m_w, GetPreTagEvtWeight());
			fHistWreco_phi_minus->Fill(phi_w, GetPreTagEvtWeight());
			fHistWreco_pt_minus ->Fill(pt_w, GetPreTagEvtWeight());
			fHistWreco_eta_minus ->Fill(eta_w, GetPreTagEvtWeight());
			fHistWreco_rap_minus ->Fill(RapW, GetPreTagEvtWeight());
			    
			fHistTopReco_Mt_e_minus ->Fill(TMath::Abs(mt_top), GetPreTagEvtWeight());
			fHistTopReco_M_e_minus ->Fill(m_top, GetPreTagEvtWeight());
			fHistTopReco_phi_e_minus->Fill(phi_top, GetPreTagEvtWeight());
			fHistTopReco_pt_e_minus ->Fill(pt_top, GetPreTagEvtWeight());
			fHistTopReco_eta_e_minus ->Fill(eta_top, GetPreTagEvtWeight());
			fHistTopReco_rap_e_minus ->Fill(Rapt, GetPreTagEvtWeight());
			    
			fHistTopReco_Mt_minus ->Fill(TMath::Abs(mt_top), GetPreTagEvtWeight());
			fHistTopReco_M_minus ->Fill(m_top, GetPreTagEvtWeight());
			fHistTopReco_phi_minus->Fill(phi_top, GetPreTagEvtWeight());
			fHistTopReco_pt_minus ->Fill(pt_top, GetPreTagEvtWeight());
			fHistTopReco_eta_minus ->Fill(eta_top, GetPreTagEvtWeight());
			fHistTopReco_rap_minus ->Fill(Rapt, GetPreTagEvtWeight());
			
			fHistNuReco_phi_e_minus->Fill(phi_nu, GetPreTagEvtWeight());
			fHistNuReco_pt_e_minus ->Fill(pt_nu, GetPreTagEvtWeight());
			fHistNuReco_eta_e_minus ->Fill(eta_nu, GetPreTagEvtWeight());
			fHistNuReco_E_e_minus ->Fill(e_nu, GetPreTagEvtWeight());
			
			fHistNuReco_phi_minus->Fill(phi_nu, GetPreTagEvtWeight());
			fHistNuReco_pt_minus ->Fill(pt_nu, GetPreTagEvtWeight());
			fHistNuReco_eta_minus ->Fill(eta_nu, GetPreTagEvtWeight());
			fHistNuReco_E_minus ->Fill(e_nu, GetPreTagEvtWeight());
			    
		    } else {
			fHistWreco_Mt_e_plus ->Fill(TMath::Abs(mt_w), GetPreTagEvtWeight());
			fHistWreco_M_e_plus ->Fill(m_w, GetPreTagEvtWeight());
			fHistWreco_phi_e_plus->Fill(phi_w, GetPreTagEvtWeight());
			fHistWreco_pt_e_plus ->Fill(pt_w, GetPreTagEvtWeight());
			fHistWreco_eta_e_plus ->Fill(eta_w, GetPreTagEvtWeight());
			fHistWreco_rap_e_plus ->Fill(RapW, GetPreTagEvtWeight());
			    
			fHistWreco_Mt_plus ->Fill(TMath::Abs(mt_w), GetPreTagEvtWeight());
			fHistWreco_M_plus ->Fill(m_w, GetPreTagEvtWeight());
			fHistWreco_phi_plus->Fill(phi_w, GetPreTagEvtWeight());
			fHistWreco_pt_plus ->Fill(pt_w, GetPreTagEvtWeight());
			fHistWreco_eta_plus ->Fill(eta_w, GetPreTagEvtWeight());
			fHistWreco_rap_plus ->Fill(RapW, GetPreTagEvtWeight());
			
			fHistTopReco_Mt_e_plus ->Fill(TMath::Abs(mt_top), GetPreTagEvtWeight());
			fHistTopReco_M_e_plus ->Fill(m_top, GetPreTagEvtWeight());
			fHistTopReco_phi_e_plus->Fill(phi_top, GetPreTagEvtWeight());
			fHistTopReco_pt_e_plus ->Fill(pt_top, GetPreTagEvtWeight());
			fHistTopReco_eta_e_plus ->Fill(eta_top, GetPreTagEvtWeight());
			fHistTopReco_rap_e_plus ->Fill(Rapt, GetPreTagEvtWeight());
			
			fHistTopReco_Mt_plus ->Fill(TMath::Abs(mt_top), GetPreTagEvtWeight());
			fHistTopReco_M_plus ->Fill(m_top, GetPreTagEvtWeight());
			fHistTopReco_phi_plus->Fill(phi_top, GetPreTagEvtWeight());
			fHistTopReco_pt_plus ->Fill(pt_top, GetPreTagEvtWeight());
			fHistTopReco_eta_plus ->Fill(eta_top, GetPreTagEvtWeight());
			fHistTopReco_rap_plus ->Fill(Rapt, GetPreTagEvtWeight());
			
			fHistNuReco_phi_e_plus->Fill(phi_nu, GetPreTagEvtWeight());
			fHistNuReco_pt_e_plus ->Fill(pt_nu, GetPreTagEvtWeight());
			fHistNuReco_eta_e_plus ->Fill(eta_nu, GetPreTagEvtWeight());
			fHistNuReco_E_e_plus ->Fill(e_nu, GetPreTagEvtWeight());
			
			fHistNuReco_phi_plus->Fill(phi_nu, GetPreTagEvtWeight());
			fHistNuReco_pt_plus ->Fill(pt_nu, GetPreTagEvtWeight());
			fHistNuReco_eta_plus ->Fill(eta_nu, GetPreTagEvtWeight());
			fHistNuReco_E_plus ->Fill(e_nu, GetPreTagEvtWeight());
		    }
		}
		
		
		// W -> mu nu
		if ( fWBoson->GetChargedLepton()->IsMuon() ) {
		    
		    // Fill muon histograms of reconstructed W
		    fHistWreco_Mt_mu ->Fill(TMath::Abs(mt_w), GetPreTagEvtWeight());
		    fHistWreco_M_mu ->Fill(m_w, GetPreTagEvtWeight());
		    fHistWreco_phi_mu->Fill(phi_w, GetPreTagEvtWeight());
		    fHistWreco_pt_mu ->Fill(pt_w, GetPreTagEvtWeight());
		    fHistWreco_eta_mu ->Fill(eta_w, GetPreTagEvtWeight());
		    fHistWreco_rap_mu ->Fill(RapW, GetPreTagEvtWeight());
		    
		    fHistWreco_Mt   ->Fill(TMath::Abs(mt_w), GetPreTagEvtWeight());
		    fHistWreco_M   ->Fill(m_w, GetPreTagEvtWeight());
		    fHistWreco_phi  ->Fill(phi_w, GetPreTagEvtWeight());
		    fHistWreco_pt   ->Fill(pt_w, GetPreTagEvtWeight());
		    fHistWreco_eta   ->Fill(eta_w, GetPreTagEvtWeight());
		    fHistWreco_rap ->Fill(RapW, GetPreTagEvtWeight());
		    
		    //  Fill muon histograms of reconstructed top-quark 
		    fHistTopReco_Mt_mu ->Fill(TMath::Abs(mt_top), GetPreTagEvtWeight());
		    fHistTopReco_M_mu ->Fill(m_top, GetPreTagEvtWeight());
		    fHistTopReco_phi_mu->Fill(phi_top, GetPreTagEvtWeight());
		    fHistTopReco_pt_mu ->Fill(pt_top, GetPreTagEvtWeight());
		    fHistTopReco_eta_mu ->Fill(eta_top, GetPreTagEvtWeight());
		    fHistTopReco_rap_mu ->Fill(Rapt, GetPreTagEvtWeight());
		    
		    fHistTopReco_Mt   ->Fill(TMath::Abs(mt_top), GetPreTagEvtWeight());
		    fHistTopReco_M   ->Fill(m_top, GetPreTagEvtWeight());
		    fHistTopReco_phi  ->Fill(phi_top, GetPreTagEvtWeight());
		    fHistTopReco_pt   ->Fill(pt_top, GetPreTagEvtWeight());
		    fHistTopReco_eta  ->Fill(eta_top, GetPreTagEvtWeight());
		    fHistTopReco_Amount_P3->Fill(P3_top.Mag(), GetPreTagEvtWeight());
		    if ( fLeadFwdJet != 0 ){
			fHistTopReco_DeltaEtaFwdJet->Fill((eta_top - fwdjet_eta), GetPreTagEvtWeight());
			fHistTopReco_DeltaPhiFwdJet->Fill(TMath::Abs(fLeadFwdJet->DeltaPhi(fTop)),GetPreTagEvtWeight());
			fHistTopReco_SumPxPyFwdJet->Fill(GetSumPt(fTop->Px(),fTop->Py(),fLeadFwdJet->Px(),fLeadFwdJet->Py()),GetPreTagEvtWeight());
			fHistTopReco_DeltaP3FwdJet->Fill(P3_top.Mag()-fwdjet_P3.Mag(),GetPreTagEvtWeight());
			fHistTopReco_DeltaPxFwdJet->Fill(fTop->Px()+fLeadFwdJet->Px(),GetPreTagEvtWeight());
			fHistTopReco_DeltaPyFwdJet->Fill(fTop->Py()+fLeadFwdJet->Py(),GetPreTagEvtWeight());
		    }
		    fHistTopReco_rap  ->Fill(Rapt, GetPreTagEvtWeight());
		    
		    //  Fill muon histograms of reconstructed neutrino
		    fHistNuReco_phi_mu->Fill(phi_nu, GetPreTagEvtWeight());
		    fHistNuReco_pt_mu ->Fill(pt_nu, GetPreTagEvtWeight());
		    fHistNuReco_eta_mu ->Fill(eta_nu, GetPreTagEvtWeight());
		    fHistNuReco_E_mu ->Fill(e_nu, GetPreTagEvtWeight());
		    
		    fHistNuReco_phi  ->Fill(phi_nu, GetPreTagEvtWeight());
		    fHistNuReco_pt   ->Fill(pt_nu, GetPreTagEvtWeight());
		    fHistNuReco_eta   ->Fill(eta_nu, GetPreTagEvtWeight());
		    fHistNuReco_E   ->Fill(e_nu, GetPreTagEvtWeight());
		    
// 	// Fill chi-square and chi-square prob. histograms
// 	fHistChi2_mu->Fill(chi2, GetPreTagEvtWeight());
// 	fHistChi2Prob_mu->Fill(chi2prob, GetPreTagEvtWeight());
	    
// 	fHistChi2_both->Fill(chi2, GetPreTagEvtWeight());
// 	fHistChi2Prob_both->Fill(chi2prob, GetPreTagEvtWeight());
	
// 	fHist_Chi2Prob_vs_M_invar_W->Fill(chi2prob, m_w, GetPreTagEvtWeight());
// 	fHist_Chi2Prob_vs_M_invar_top->Fill(chi2prob, m_top, GetPreTagEvtWeight());

			
		    if ( fWBoson->GetChargedLepton()->IsNegative() ) {
			fHistWreco_Mt_mu_minus ->Fill(TMath::Abs(mt_w), GetPreTagEvtWeight());
			fHistWreco_M_mu_minus ->Fill(m_w, GetPreTagEvtWeight());
			fHistWreco_phi_mu_minus->Fill(phi_w, GetPreTagEvtWeight());
			fHistWreco_pt_mu_minus ->Fill(pt_w, GetPreTagEvtWeight());
			fHistWreco_eta_mu_minus ->Fill(eta_w, GetPreTagEvtWeight());
			fHistWreco_rap_mu_minus ->Fill(RapW, GetPreTagEvtWeight());
			
			fHistWreco_Mt_minus ->Fill(TMath::Abs(mt_w), GetPreTagEvtWeight());
			fHistWreco_M_minus ->Fill(m_w, GetPreTagEvtWeight());
			fHistWreco_phi_minus->Fill(phi_w, GetPreTagEvtWeight());
			fHistWreco_pt_minus ->Fill(pt_w, GetPreTagEvtWeight());
			fHistWreco_eta_minus ->Fill(eta_w, GetPreTagEvtWeight());
			fHistWreco_rap_minus ->Fill(RapW, GetPreTagEvtWeight());
			
			fHistTopReco_Mt_mu_minus ->Fill(TMath::Abs(mt_top), GetPreTagEvtWeight());
			fHistTopReco_M_mu_minus ->Fill(m_top, GetPreTagEvtWeight());
			fHistTopReco_phi_mu_minus->Fill(phi_top, GetPreTagEvtWeight());
			fHistTopReco_pt_mu_minus ->Fill(pt_top, GetPreTagEvtWeight());
			fHistTopReco_eta_mu_minus ->Fill(eta_top, GetPreTagEvtWeight());
			fHistTopReco_rap_mu_minus ->Fill(Rapt, GetPreTagEvtWeight());
			
			fHistTopReco_Mt_minus ->Fill(TMath::Abs(mt_top), GetPreTagEvtWeight());
			fHistTopReco_M_minus ->Fill(m_top, GetPreTagEvtWeight());
			fHistTopReco_phi_minus->Fill(phi_top, GetPreTagEvtWeight());
			fHistTopReco_pt_minus ->Fill(pt_top, GetPreTagEvtWeight());
			fHistTopReco_eta_minus ->Fill(eta_top, GetPreTagEvtWeight());
			fHistTopReco_rap_minus ->Fill(Rapt, GetPreTagEvtWeight());
			
			fHistNuReco_phi_mu_minus->Fill(phi_nu, GetPreTagEvtWeight());
			fHistNuReco_pt_mu_minus ->Fill(pt_nu, GetPreTagEvtWeight());
			fHistNuReco_eta_mu_minus ->Fill(eta_nu, GetPreTagEvtWeight());
			fHistNuReco_E_mu_minus ->Fill(e_nu, GetPreTagEvtWeight());
			
			fHistNuReco_phi_minus->Fill(phi_nu, GetPreTagEvtWeight());
			fHistNuReco_pt_minus ->Fill(pt_nu, GetPreTagEvtWeight());
			fHistNuReco_eta_minus ->Fill(eta_nu, GetPreTagEvtWeight());
			fHistNuReco_E_minus ->Fill(e_nu, GetPreTagEvtWeight());
			
		    } else {
			fHistWreco_Mt_mu_plus ->Fill(TMath::Abs(mt_w), GetPreTagEvtWeight());
			fHistWreco_M_mu_plus ->Fill(m_w, GetPreTagEvtWeight());
			fHistWreco_phi_mu_plus->Fill(phi_w, GetPreTagEvtWeight());
			fHistWreco_pt_mu_plus ->Fill(pt_w, GetPreTagEvtWeight());
			fHistWreco_eta_mu_plus ->Fill(eta_w, GetPreTagEvtWeight());
			fHistWreco_rap_mu_plus ->Fill(RapW, GetPreTagEvtWeight());
			
			fHistWreco_Mt_plus ->Fill(TMath::Abs(mt_w), GetPreTagEvtWeight());
			fHistWreco_M_plus ->Fill(m_w, GetPreTagEvtWeight());
			fHistWreco_phi_plus->Fill(phi_w, GetPreTagEvtWeight());
			fHistWreco_pt_plus ->Fill(pt_w, GetPreTagEvtWeight());
			fHistWreco_eta_plus ->Fill(eta_w, GetPreTagEvtWeight());
			fHistWreco_rap_plus ->Fill(RapW, GetPreTagEvtWeight());
			
			
			fHistTopReco_Mt_mu_plus ->Fill(TMath::Abs(mt_top), GetPreTagEvtWeight());
			fHistTopReco_M_mu_plus ->Fill(m_top, GetPreTagEvtWeight());
			fHistTopReco_phi_mu_plus->Fill(phi_top, GetPreTagEvtWeight());
			fHistTopReco_pt_mu_plus ->Fill(pt_top, GetPreTagEvtWeight());
			fHistTopReco_eta_mu_plus ->Fill(eta_top, GetPreTagEvtWeight());
			fHistTopReco_rap_mu_plus ->Fill(Rapt, GetPreTagEvtWeight());
			
			fHistTopReco_Mt_plus ->Fill(TMath::Abs(mt_top), GetPreTagEvtWeight());
			fHistTopReco_M_plus ->Fill(m_top, GetPreTagEvtWeight());
			fHistTopReco_phi_plus->Fill(phi_top, GetPreTagEvtWeight());
			fHistTopReco_pt_plus ->Fill(pt_top, GetPreTagEvtWeight());
			fHistTopReco_eta_plus ->Fill(eta_top, GetPreTagEvtWeight());
			fHistTopReco_rap_plus ->Fill(Rapt, GetPreTagEvtWeight());
			
			fHistNuReco_phi_mu_plus->Fill(phi_nu, GetPreTagEvtWeight());
			fHistNuReco_pt_mu_plus ->Fill(pt_nu, GetPreTagEvtWeight());
			fHistNuReco_eta_mu_plus ->Fill(eta_nu, GetPreTagEvtWeight());
			fHistNuReco_E_mu_plus ->Fill(e_nu, GetPreTagEvtWeight());
			
			fHistNuReco_phi_plus->Fill(phi_nu, GetPreTagEvtWeight());
			fHistNuReco_pt_plus ->Fill(pt_nu, GetPreTagEvtWeight());
			fHistNuReco_eta_plus ->Fill(eta_nu, GetPreTagEvtWeight());
			fHistNuReco_E_plus ->Fill(e_nu, GetPreTagEvtWeight());
		    }
		}
		



		// ==============
		// charged lepton
		// ==============
		fHistChargedLeptonReco_pt->Fill(lep->Pt(), GetPreTagEvtWeight());
		fHistChargedLeptonReco_eta->Fill(lep->Eta(), GetPreTagEvtWeight());
		
		// ====
		// jets
		// ====
		fHistJetMultiplicityReco->Fill(fJets->GetEntries(), GetPreTagEvtWeight());
		fHistBJetMultiplicityReco->Fill(fBJets->GetEntries(), GetPreTagEvtWeight());
		
		// loop over list of untagged jets
		// also get leading untagged jet
		HepJet *FirstNonBJet = 0;
		if ( fNonBJets->GetEntries() ){
		    Int_t NEta2 = 0;
		    Int_t NEta3 = 0;
		    FirstNonBJet = (HepJet*)fNonBJets->At(0);
		    for ( Int_t i=0; i<fNonBJets->GetEntries(); i++){
			if ( i ){
			    if (((HepJet*)fNonBJets->At(i))->Pt() > FirstNonBJet->Pt() ){//*** already sorted list! korrigieren
				FirstNonBJet = (HepJet*)fNonBJets->At(i); 
			    }
			}
			Double_t eta = TMath::Abs(((HepJet*)fNonBJets->At(i))->Eta());
			if ( eta > 3 ) {
			    NEta3++;
			    NEta2++;
			    continue;
			}
			if ( eta > 2 ) {
			    NEta2++;
			}
		    }
		    fHistNonBJetMultiplicityEta2Reco->Fill(NEta2, GetPreTagEvtWeight());
		    fHistNonBJetMultiplicityEta3Reco->Fill(NEta3, GetPreTagEvtWeight());
		    fHistLeadingNonBJetReco_pt->Fill(FirstNonBJet->Pt(), GetPreTagEvtWeight());
		    fHistLeadingNonBJetReco_eta->Fill(FirstNonBJet->Eta(), GetPreTagEvtWeight());
		}
				
		// =============
		// Rest Frame Plots
		// =============
		
		//compute boost vectors
		TVector3 boost_w 	= -P_w.BoostVector();
		TVector3 boost_top 	= -P_top.BoostVector();
		
		//define boosted momenta
		TLorentzVector P_lep_RF_W = P_lep;
		TLorentzVector P_W_RF_Top = P_w;
		TLorentzVector P_BJet_RF_W = P_bjet_fit;
		
		//boost
		P_lep_RF_W.Boost(boost_w);
		P_W_RF_Top.Boost(boost_top);
		P_BJet_RF_W.Boost(boost_w);
		
		//Decay angle Theta1* and Theta2*
		Double_t ThetaStar1 = P_lep_RF_W.Angle(P_W_RF_Top.Vect());
		Double_t ThetaStar2 = P_lep_RF_W.Angle(P_BJet_RF_W.Vect());

		fHist_ThetaStar1_LepW 		 -> Fill(ThetaStar1,GetPreTagEvtWeight());
		fHist_Cos_ThetaStar1_LepW	 -> Fill(TMath::Cos(ThetaStar1),GetPreTagEvtWeight());
		fHist_ThetaStar2_LepBQuark 	 -> Fill(ThetaStar2,GetPreTagEvtWeight());
		fHist_Cos_ThetaStar2_LepBQuark	 -> Fill(TMath::Cos(ThetaStar2),GetPreTagEvtWeight());
		fHist_SumThetaStar1ThetaStar2	 -> Fill(ThetaStar1+ThetaStar2,GetPreTagEvtWeight());
		fHist_Cos_SumThetaStar1ThetaStar2-> Fill(TMath::Cos(ThetaStar1+ThetaStar2),GetPreTagEvtWeight());
		
		//plots in Top RestFrame
		
		//Angle between Top in Lab System and FwdJet in Top rest Frame
		
		TLorentzVector P_fwdjet_TopRF 	= fLeadFwdJet->P();
		
		TVector3 boost_TopRF = -P_top.BoostVector();

		P_fwdjet_TopRF.Boost(boost_TopRF);
		TVector3 SumP_TopRF = P_top.Vect()+P_fwdjet_TopRF.Vect();  //Sum of Top P in LabSystem and FwdJet in TopSystem
 
		fHist_AngleTopFwdJet_TopRF -> Fill(P_top.Angle(P_fwdjet_TopRF.Vect()),GetPreTagEvtWeight());
		
		fHist_AmountSumPTopFwdJet_TopRF ->Fill(SumP_TopRF.Mag(),GetPreTagEvtWeight());
		
		
		
		// =============
		// Miscellaneous
		// =============
		// M_effective
		// fJets is used, this could possibly cause
		// confusion if you use different cuts
		// for b jets and general jets because then
		// fBjets might not be a subsample of fJets
		// (see SetCutDefaults()).
		// In particular M_effective is rather useless
		// if you use different jet algorithms
		// for b jets and general jets.
				
	
// 		//
// 		// Mass effective
// 		//
// 		Double_t M_eff = fTop->Pt();      
// 		for ( Int_t i=0; i<fJets->GetEntries(); i++ ){
// 		    if ( fJets->At(i) != fTop->GetBJetOrig() )
// 			M_eff += ((HepJet*)fJets->At(i))->Pt();
// 		}
		fHistMEffective->Fill(M_eff, GetPreTagEvtWeight());      
		
		// Sphericity
		// usage of b Jet, lepton and neutrino from Top Reconstruction,
		// additionally using jets from fJets
		TList *Vectors  = new TList();
		TVector3 lep_P3 = lep->P3();
		TVector3 nu_P3  = nu->P3();
		TVector3 b_P3   = fTop->GetP_BJet().Vect();
		TVector3 jet_P3;
		Vectors->Add(&lep_P3); 
		Vectors->Add(&nu_P3); 
		Vectors->Add(&b_P3);
		for ( Int_t i=0; i<fJets->GetEntries(); i++ ){
		    if ( fJets->At(i) != fTop->GetBJetOrig() ){
			jet_P3 = ((HepJet*)fJets->At(i))->P3();
			Vectors->Add(&jet_P3);
		    }
		}   
		Float_t Sphericity = fEvent->Sphericity(Vectors);
		fHistSphericity->Fill(Sphericity, GetPreTagEvtWeight());
		delete Vectors;
		
		// DeltaR between Charged Lepton and b jet from top decay
// 		HepParticle *prt = new HepParticle(0, fTop->GetP_BJet().X(), fTop->GetP_BJet().Y(),
// 						   fTop->GetP_BJet().Z(), fTop->GetP_BJet().E(), 0); 
		fHistDeltaRChLepBJet->Fill(lep->DeltaR(prt), GetPreTagEvtWeight());
		delete prt;
		
		// x_Bjorken of light quark und bottom quark as given by the simplest
		// model you can imagine
		Double_t x1 = 0;
		Double_t x2 = 0;
		if ( fNonBJets->GetEntries() ){
		    x1 = (fTop->E()+FirstNonBJet->P().E()+fTop->Pz()+FirstNonBJet->Pz())/14000;
		    x2 = (fTop->E()+FirstNonBJet->P().E()-fTop->Pz()-FirstNonBJet->Pz())/14000;
		    fHistXlightRel->Fill(TMath::Max(x1, x2), GetPreTagEvtWeight());
		    fHistXbottomRel->Fill(TMath::Min(x1, x2), GetPreTagEvtWeight());	  
		    if ( FirstNonBJet->Pz() > 0 ) {
			fHistXlightForwardJet->Fill(x1, GetPreTagEvtWeight());
			fHistXbottomForwardJet->Fill(x2, GetPreTagEvtWeight());
		    }
		    else {
			fHistXlightForwardJet->Fill(x2, GetPreTagEvtWeight());
			fHistXbottomForwardJet->Fill(x1, GetPreTagEvtWeight());
		    }
		}
//	    }

		// ===========================================
		// Fill before/after fit comparison histograms
		// ===========================================
		FillComparisonHistograms(); // pulls of main fit are filled already in ReconstructionKinFit() !

	}
//    }
  
	// *** Decided to fill all these plots only after the P(Chi2)-cut ***
//     // ===========================================
//     // Fill before/after fit comparison histograms
//     // ===========================================
//     FillComparisonHistograms();
  
    // ==============================
    // Fill Truth Matching Histograms
    // ==============================
    // Switch off to prevent crashes in background samples!
    // (reasons for crashes unknown)
    // Need to take a closer look at the truth matching...
    
    if ( fEvent->IsMC() && fTruthMatch ){
	switch (DoTruthMatching()){
	    //1 = TruthMatching is correct
	    //-1=TruthMatching is wrong
	    //0 =TruthMatching isn't possible (for example missing MC article to compare)
	    case  1: FillTruthMatchingHistogramsSignal();
		     break;
	    case -1: FillTruthMatchingHistogramsBkg();
		     break;
	    case  0: break;
	}
	//FillTruthMatchingHistograms();
    }
    
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::FillTruthMatchingHistogramsSignal(){
    //Fill Histogramms if Truth Matching was correct
    
    
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::FillTruthMatchingHistogramsBkg(){
    
    
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::FillKinFitLeptonHistograms() {
    //
    // Fill lepton histograms for top candidates in KinFitter mode
    //

    // =============
    // Signal lepton
    // =============
    if ( fLepton != 0 ) {
	Float_t Pt_sig  = fLepton->Pt();
	Float_t Eta_sig = fLepton->Eta();
	Float_t Phi_sig = fLepton->Phi();
	
	fHistKinFitCandidateSignalLepton_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
	fHistKinFitCandidateSignalLepton_eta->Fill(Eta_sig, GetPreTagEvtWeight());
	fHistKinFitCandidateSignalLepton_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	
	// e
	if ( fLepton->IsElectron() ) {
	    //*************************************
	    // Number of signal electrons by author
	    //*************************************
	    // Mind event weights at low statistics!!!
	    AtlElectron::EAuthor author = ((AtlElectron*)fLepton)->GetAuthor();

	    for ( Int_t j = 0; j < AtlElectron::fgNAuthors; j++ ) {
		if ( author & (1<<j) ) fHistKinFitCandidateSignalElectron_author->AddBinContent(j+1, GetPreTagEvtWeight());
	    }
	    // set of electrons that are highPt AND soft
	    if ( (author & 2) && (author & 4) )
		fHistKinFitCandidateSignalElectron_author->AddBinContent(6, GetPreTagEvtWeight());
//	    fHistKinFitCandidateSignalElectron_author->Sumw2();
	   
	    fHistKinFitCandidateSignalElectron_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
	    fHistKinFitCandidateSignalElectron_eta->Fill(Eta_sig, GetPreTagEvtWeight());
	    fHistKinFitCandidateSignalElectron_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    if ( fLepton->IsPositive() ) {
		fHistKinFitCandidateSignalEplus_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistKinFitCandidateSignalEplus_eta->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistKinFitCandidateSignalEplus_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    } else {
		fHistKinFitCandidateSignalEminus_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistKinFitCandidateSignalEminus_eta->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistKinFitCandidateSignalEminus_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    }
	}
	
	// mu
	if ( fLepton->IsMuon() ) {
	    fHistKinFitCandidateSignalMuon_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
	    fHistKinFitCandidateSignalMuon_eta->Fill(Eta_sig, GetPreTagEvtWeight());
	    fHistKinFitCandidateSignalMuon_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    if ( fLepton->IsPositive() ) {
		fHistKinFitCandidateSignalMuplus_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistKinFitCandidateSignalMuplus_eta->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistKinFitCandidateSignalMuplus_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    } else {
		fHistKinFitCandidateSignalMuminus_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistKinFitCandidateSignalMuminus_eta->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistKinFitCandidateSignalMuminus_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    }
	}
	
	// tau
	if ( fLepton->IsTau() ) {
	    fHistKinFitCandidateSignalTau_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
	    fHistKinFitCandidateSignalTau_eta->Fill(Eta_sig, GetPreTagEvtWeight());
	    fHistKinFitCandidateSignalTau_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    if ( fLepton->IsPositive() ) {
		fHistKinFitCandidateSignalTauplus_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistKinFitCandidateSignalTauplus_eta->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistKinFitCandidateSignalTauplus_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    } else {
		fHistKinFitCandidateSignalTauminus_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistKinFitCandidateSignalTauminus_eta->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistKinFitCandidateSignalTauminus_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    }
	}
    }
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::FillKinFitJetHistograms() {
    //
    // Fill jet histograms for top candidates in KinFitter mode
    //
    Float_t Et  = 0.;
    Float_t eta = 0.;
    Float_t phi = 0.;
    AtlJet *jet = 0;

    // Jets
    TIter next_jet(fJets);
    Int_t njet = 0;
    while ( (jet = (AtlJet*)next_jet()) ) {
	Et  = jet->Et();
	eta = jet->Eta();
	phi = jet->Phi();
	fHistKinFitCandidateJet_Et ->Fill(Et, GetPreTagEvtWeight());
	fHistKinFitCandidateJet_eta->Fill(eta, GetPreTagEvtWeight());
	fHistKinFitCandidateJet_phi->Fill(phi, GetPreTagEvtWeight());
	if ( njet == 0 ) {
	    fHistKinFitCandidateJet1_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistKinFitCandidateJet1_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistKinFitCandidateJet1_phi->Fill(phi, GetPreTagEvtWeight());
	}
	if ( njet == 1 ) {
	    fHistKinFitCandidateJet2_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistKinFitCandidateJet2_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistKinFitCandidateJet2_phi->Fill(phi, GetPreTagEvtWeight());
	}
	njet++;
    }
    fHistKinFitCandidateJet_n->Fill(njet, GetPreTagEvtWeight());

    // B-Jets
    TIter next_bjet(fBJets);
    njet = 0;
    while ( (jet = (AtlJet*)next_bjet()) ) {
	Et  = jet->Et();
	eta = jet->Eta();
	phi = jet->Phi();
	fHistKinFitCandidateBJet_Et ->Fill(Et, GetPreTagEvtWeight());
	fHistKinFitCandidateBJet_eta->Fill(eta, GetPreTagEvtWeight());
	fHistKinFitCandidateBJet_phi->Fill(phi, GetPreTagEvtWeight());
	if ( njet == 0 ) {
	    fHistKinFitCandidateBJet1_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistKinFitCandidateBJet1_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistKinFitCandidateBJet1_phi->Fill(phi, GetPreTagEvtWeight());
	}
	if ( njet == 1 ) {
	    fHistKinFitCandidateBJet2_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistKinFitCandidateBJet2_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistKinFitCandidateBJet2_phi->Fill(phi, GetPreTagEvtWeight());
	}
	njet++;
    }
    fHistKinFitCandidateBJet_n->Fill(njet, GetPreTagEvtWeight());

    // Non-B-Jets
    TIter next_nonbjet(fNonBJets);
    njet = 0;
    while ( (jet = (AtlJet*)next_nonbjet()) ) {
	Et  = jet->Et();
	eta = jet->Eta();
	phi = jet->Phi();
	fHistKinFitCandidateNonBJet_Et ->Fill(Et, GetPreTagEvtWeight());
	fHistKinFitCandidateNonBJet_eta->Fill(eta, GetPreTagEvtWeight());
	fHistKinFitCandidateNonBJet_phi->Fill(phi, GetPreTagEvtWeight());
	if ( njet == 0 ) {
	    fHistKinFitCandidateNonBJet1_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistKinFitCandidateNonBJet1_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistKinFitCandidateNonBJet1_phi->Fill(phi, GetPreTagEvtWeight());
	}
	if ( njet == 1 ) {
	    fHistKinFitCandidateNonBJet2_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistKinFitCandidateNonBJet2_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistKinFitCandidateNonBJet2_phi->Fill(phi, GetPreTagEvtWeight());
	}
	njet++;
    }
    fHistKinFitCandidateNonBJet_n->Fill(njet, GetPreTagEvtWeight());

    // Fwd-Jets
    TIter next_fwdjet(fFwdJets);
    njet = 0;
    while ( (jet = (AtlJet*)next_fwdjet()) ) {
	Et  = jet->Et();
	eta = jet->Eta();
	phi = jet->Phi();
	fHistKinFitCandidateFwdJet_Et ->Fill(Et, GetPreTagEvtWeight());
	fHistKinFitCandidateFwdJet_eta->Fill(eta, GetPreTagEvtWeight());
	fHistKinFitCandidateFwdJet_phi->Fill(phi, GetPreTagEvtWeight());
	if ( njet == 0 ) {
	    fHistKinFitCandidateFwdJet1_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistKinFitCandidateFwdJet1_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistKinFitCandidateFwdJet1_phi->Fill(phi, GetPreTagEvtWeight());
	}
	if ( njet == 1 ) {
	    fHistKinFitCandidateFwdJet2_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistKinFitCandidateFwdJet2_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistKinFitCandidateFwdJet2_phi->Fill(phi, GetPreTagEvtWeight());
	}
	njet++;
    }
    fHistKinFitCandidateFwdJet_n->Fill(njet, GetPreTagEvtWeight());
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::FillComparisonHistograms() {
    //
    // Fill histograms for comparison of distributions before and after
    // KinFitter selection
    //

    
    // Before fit
    AtlJet *bjet_orig = (AtlJet*)fTop->GetBJetOrig();
    
    Float_t lep_pt  = fLepton->Pt();
    Float_t lep_eta = fLepton->Eta();
    Float_t lep_phi = fLepton->Phi();
    Float_t bjet_orig_et  = bjet_orig->Et();
    Float_t bjet_orig_pt  = bjet_orig->Pt();
    Float_t bjet_orig_eta = bjet_orig->Eta();
    Float_t bjet_orig_phi = bjet_orig->Phi();
    Float_t bjet_orig_weight = bjet_orig->GetTag(fBJetTagger)->GetWeight();
    
    fHistSignalLeptonPt_before->Fill(lep_pt, GetPreTagEvtWeight());
    fHistSignalLeptonEta_before->Fill(lep_eta, GetPreTagEvtWeight());
    fHistSignalLeptonEta_before_cells->Fill(lep_eta, GetPreTagEvtWeight());
    fHistSignalLeptonPhi_before->Fill(lep_phi, GetPreTagEvtWeight());
    fHistBJetEt_before->Fill(bjet_orig_et, GetPreTagEvtWeight());
    fHistBJetEta_before->Fill(bjet_orig_eta, GetPreTagEvtWeight());
    fHistBJetPhi_before->Fill(bjet_orig_phi, GetPreTagEvtWeight());
    fHistBJetWeight_before->Fill(bjet_orig_weight, GetPreTagEvtWeight());
    fHistEtMissMag_before->Fill(fMET_Mag, GetPreTagEvtWeight());
    fHistEtMissPhi_before->Fill(fMET_Phi, GetPreTagEvtWeight());
    fHistSumEt_before->Fill(fEvent->GetEnergySum()->GetRefFinalEM_etightpp_SumEt(),
			    GetPreTagEvtWeight());
	
    
    // After fit
    HepParticle   *lepton_fit   = fWBoson->GetChargedLepton();
    HepParticle   *neutrino_fit = fWBoson->GetNeutrino();
    TLorentzVector p_bjet_fit   = fTop->GetP_BJet();
    Float_t lep_fit_pt  = lepton_fit->Pt();
    Float_t lep_fit_eta = lepton_fit->Eta();
    Float_t lep_fit_phi = lepton_fit->Phi();
    Float_t p_bjet_fit_et  = p_bjet_fit.Et();
    Float_t p_bjet_fit_pt  = p_bjet_fit.Pt();
    Float_t p_bjet_fit_eta = p_bjet_fit.Eta();
    Float_t p_bjet_fit_phi = p_bjet_fit.Phi();
    Float_t neutrino_fit_et  = neutrino_fit->Et();
    Float_t neutrino_fit_phi = neutrino_fit->Phi();
    
    fHistSignalLeptonPt_after->Fill(lep_fit_pt, GetPreTagEvtWeight());
    fHistSignalLeptonEta_after->Fill(lep_fit_eta, GetPreTagEvtWeight());
    fHistSignalLeptonEta_after_cells->Fill(lep_fit_eta, GetPreTagEvtWeight());
    fHistSignalLeptonPhi_after->Fill(lep_fit_phi, GetPreTagEvtWeight());
    fHistBJetEt_after->Fill(p_bjet_fit_et, GetPreTagEvtWeight());
    fHistBJetEta_after->Fill(p_bjet_fit_eta, GetPreTagEvtWeight());
    fHistBJetPhi_after->Fill(p_bjet_fit_phi, GetPreTagEvtWeight());
    fHistEtMissMag_after->Fill(neutrino_fit_et, GetPreTagEvtWeight());
    fHistEtMissPhi_after->Fill(neutrino_fit_phi, GetPreTagEvtWeight());
    
    // Normalized residuals distributions
    fHistNormResidLepton_pt->Fill(((lep_fit_pt - lep_pt)/lep_pt));
    fHistNormResidLepton_eta->Fill(((lep_fit_eta - lep_eta)/lep_eta));
    fHistNormResidLepton_phi->Fill(((lep_fit_phi - lep_phi)/lep_phi));
    fHistNormResidBJet_Et->Fill(((p_bjet_fit_et - bjet_orig_et)/bjet_orig_et));
    fHistNormResidBJet_pt->Fill(((p_bjet_fit_pt - bjet_orig_pt)/bjet_orig_pt));
    fHistNormResidBJet_eta->Fill(((p_bjet_fit_eta - bjet_orig_eta)/bjet_orig_eta));
    fHistNormResidBJet_phi->Fill(((p_bjet_fit_phi - bjet_orig_phi)/bjet_orig_phi));
    fHistNormResidEtmiss_Mag->Fill(((neutrino_fit_et - fMET_Mag)/fMET_Mag));
    fHistNormResidEtmiss_phi->Fill(((neutrino_fit_phi - fMET_Phi)/fMET_Phi));


}

//____________________________________________________________________

Int_t AtlSgTop_tChannelFinder::DoTruthMatching(){
    //
    //do truth matching of reconstructed top with mc top 
    //return 1,if truthmatching was succesful and correct
    //return -1, if truthmatching wasn't succesful
    //return 0, if truth matching doesn't work for example,if there aren't MC particles for comparison
    //Get MC and reconstructet tops
    TList        *Tops_MC = fEvent->GetMCTops();
    TClonesArray *Tops_Rec= fEvent->GetTopDecays();
    /*HepTopDecay *Top_Rec = 0;
    for (Int_t i=0;i<Tops_Rec->GetEntries();i++){
	if (((HepTopDecay*)Tops_Rec->At(i))->IsSgTop_tChannel()){
	    Top_Rec=(HepTopDecay*)Tops_Rec->At(i);
	}
    }*/
    //----------------------------
    //check if Event is a SingeTop
    //----------------------------
    if (Tops_Rec->GetEntries()==0) {cout<<endl;Info("DoTruthMatching","Event has no TopDecay");cout<<endl;delete Tops_MC; return 0;}
    if (Tops_Rec->GetEntries()>1) {cout<<endl;Info("DoTruthMatching","Event has more than one TopDecay");cout<<endl;delete Tops_MC; return -1;}
    if (fTop==NULL){cout<<endl;Info("DoTruthMatching","No reconstructed SingleTop  in this Event");cout<<endl;delete Tops_MC; return 0;}
    if (Tops_MC->GetEntries()==0) {cout<<endl;Info("DoTruthMatching","There are no MC Tops in event -> wrong SingleTopreconstruction");cout<<endl;delete Tops_MC; return -1;} 
    //------------------------------------------------------------
    //get reconstructed particles to compare with the MC particles
    //------------------------------------------------------------
    
    HepJet       *BJet_Rec = fTop->GetBJetOrig();
    AtlWDecayLNu *W_Rec = 0;
    HepParticle  *Lept_Rec = 0;
    //get the leptonic W
    if (fTop->GetWDecay()->IsLeptonicDecay()){
	W_Rec=(AtlWDecayLNu*)fTop->GetWDecay();
	Lept_Rec=W_Rec->GetChargedLeptonOrig();
    }else{cout<<endl;Info("DoTruthMatching","Event has no leptonic W decay");cout<<endl;delete Tops_MC; return 0;}
    
    //-----------------------------------------------------------
    //get the MC particles and compare with the reconstructed top
    //-----------------------------------------------------------
    TIter nextMCTop(Tops_MC);
    HepMCParticle *Top_MC = 0;
    //loop over all MC_Tops in the list
    while ( (Top_MC = (HepMCParticle*)nextMCTop()) ) {
	//----------------------------------------
	//get W-Boson and Quark of the MCTop-Decay
	//----------------------------------------
	HepMCParticle *W_MC = 0;
	HepMCParticle *Quark_MC = 0;
	HepMCParticle *TopDaughter = 0;
	for (Int_t i=0;i<Top_MC->GetN_Daughters();i++){  
	    TopDaughter=(HepMCParticle*)Top_MC->GetDaughters()->At(i);
	    if (TopDaughter->IsWBoson()) W_MC=TopDaughter;
	    if (TopDaughter->IsQuark())  Quark_MC=TopDaughter;		    
	}//end for
	//test if one daughter is 0
	if ((W_MC==NULL) || (Quark_MC==NULL)){cout<<endl;Info("DoTruthMatching","At least one daughter of MCTop is 0 pointer");cout<<endl;delete Tops_MC; return 0;}
		
	//--------------------------------
	//get the MC lepton of the W-Boson
	//--------------------------------
	//get the W Boson, which decays not in a W Boson
	Bool_t DaughterIsW=kTRUE;
	while (DaughterIsW){
	    DaughterIsW=kFALSE;
	    for (Int_t i=0; i<W_MC->GetN_Daughters();i++){
		if (((HepMCParticle*)W_MC->GetDaughters()->At(i))->IsWBoson()){
		    DaughterIsW=kTRUE;
		    W_MC=(HepMCParticle*)W_MC->GetDaughters()->At(i);
		    cout<<endl;Info("DoTruthMatching","First W was not a Good Mother");cout<<endl;
		}//end if
	    }//end for
	}//end while
	//test if W Boson has mor than one daughter
	if (W_MC->GetN_Daughters()<2){cout<<endl;Info("DoTruthMatching","W has less than 2 daughters");cout<<endl;delete Tops_MC; return 0;}
	
	//get the lepton from MC W Boson
	HepMCParticle *ChargeLepton_MC = 0;
	//HepMCParticle *Neutrino_MC = 0;
	for (Int_t i=0; i<W_MC->GetN_Daughters();i++){
	    HepMCParticle *MCDaughter_W = (HepMCParticle*)W_MC->GetDaughters()->At(i);
	    if (MCDaughter_W->IsChargedLepton()){ ChargeLepton_MC = MCDaughter_W;}
	    //if (MCDaughter_W->IsNeutrino()){ Neutrino_MC = MCDaughter_W;}
	}
	//test if one daughter is 0
	if (ChargeLepton_MC==NULL){cout<<endl;Info("DoTruthMatching","W has no daughter");cout<<endl;delete Tops_MC; return 0;}
	
	//------------------------------------------------------------------------------------------
	//get from reconstructed lepton the MC lepton and compare it with the MC lepton from MC list
	//------------------------------------------------------------------------------------------
	
	//1. possibility: search lepton in a cone
	//search MC lepton in a cone of he recnstructed lepton and compare them
	TList *ListPossibleMCLeptons = fEvent->FindMatchedParticles((HepParticle*)Lept_Rec,(TCollection*)fEvent->GetMCParticles(),0.1,0.1);
	if (ListPossibleMCLeptons->GetEntries()==0){cout<<endl;Info("DoTruthMatching","No existing MC leptons in MC tree");cout<<endl;delete Tops_MC; return 0;}
	Bool_t fTruthMatchLepton = kFALSE;
	for (Int_t i=0;i<ListPossibleMCLeptons->GetEntries();i++){
	    HepMCParticle *PossibleMCLept = (HepMCParticle*)ListPossibleMCLeptons->At(i);
	    if (PossibleMCLept == ChargeLepton_MC){
		fTruthMatchLepton=kTRUE;
	    }		
	} 
	delete ListPossibleMCLeptons;
	
	//2. possibility:hitbased truth matching of lepton
	/*//get track of lepton (hitbased truth matching) doesn't work for AcerMC Samples
	AtlIDTrack *trk_lep = 0;
	if (Lept_Rec->IsElectron()){ trk_lep = ((AtlElectron*)Lept_Rec)->GetIDTrack();}
	if (Lept_Rec->IsMuon()){ trk_lep = ((AtlMuon*)Lept_Rec)->GetIDTrack();}
	if (trk_lep==NULL){cout<<endl;Info("DoTrthMatching","No track ist given for reconstructed lepton");cout<<endl;}
	Bool_t fTruthMatchLepton = kFALSE;
	//check if MC particle exists for the track and get MC particle if exist
	HepMCParticle *ChargeLepton_match=0;
	if (trk_lep->HasValidTruthMatch(fTruthMatchProb_min)){
	    ChargeLepton_match = trk_lep->GetMCParticle();
	}else{ cout<<endl;Info("DoTruthMatching","No matching MC particle for reconstructed lepton");cout<<endl;return 0;}
	//The eventtree has often decays like mu->mu->mu. ChargeLepton_match is the last lepton, which is detected. ChargeLepton_MC is the first. We have to go up (or down) to compare the leptons
	while (ChargeLepton_match->GetMother()->GetPdgCode() == ChargeLepton_match->GetPdgCode()){
	    ChargeLepton_match = ChargeLepton_match->GetMother();
	}
		
	cout<<endl<<"Lept_match: "<<ChargeLepton_match->GetPdgName()<<"\t"<<ChargeLepton_match->GetId()<<"\tLept_MC: "<<ChargeLepton_MC->GetPdgName()<<"\t"<<ChargeLepton_MC->GetId()<<endl;
	//compare matched lepton and MC lepton
	if (ChargeLepton_match == ChargeLepton_MC){
	    fTruthMatchLepton=kTRUE;
	    cout<<endl<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
	    cout<<endl;Info("DoTruthMatching","TruthMatching of leptons was succesfull");cout<<endl;
	    cout<<endl<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;  
	}*/
	
	
	//----------------------------------------------------------------------------------------------
	//get possible truth particles of reconstructed jet in a cone and compare each with the MC Quark
	//----------------------------------------------------------------------------------------------
	Bool_t fTruthMatchJet = kFALSE;
	const TRefArray *JetTruthList = ((AtlJet*)BJet_Rec)->GetTruthParticles();
	for (Int_t i=0; i<JetTruthList->GetEntries();i++){
	    HepMCParticle *PossibleMCQuark = (HepMCParticle*)JetTruthList->At(i);
	    if (PossibleMCQuark == Quark_MC){
		fTruthMatchJet=kTRUE;
	    }//end if
	}//end for
	if (fTruthMatchJet && fTruthMatchLepton){
	    //cout<<endl;Info("DoTruthMatching","TruthMatching was succesfull");cout<<endl;
	    fTop->SetTruthTop(Top_MC);
	    fWBoson->SetTruthW(W_MC);
	    
	    delete Tops_MC; 
	    return 1;
	}
    }//end while
      
    //deleting Lists
    delete Tops_MC; 
    cout<<endl;Info("DoTruthMatching","TruthMatching was !!! NOT !!! succesfull");cout<<endl;
    //test return
    return -1;
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::FillTruthMatchingHistograms() {
    //
    // Fill truth-matching histograms
    //
    
    // get Top Quark, check whether event is Single Top
    TList *TopQuarks     = fEvent->GetMCParticles("t");
    TList *AntiTopQuarks = fEvent->GetMCParticles("t_bar");    
    // Truth Matching only for Single Top Events
    if ( (TopQuarks->GetEntries() == 0 && AntiTopQuarks->GetEntries() == 0) ||
	 (TopQuarks->GetEntries() > 0 && AntiTopQuarks->GetEntries() > 0) ) return;
    TopQuarks->AddAll(AntiTopQuarks);
    HepMCParticle *TopTrue = 0;
    HepMCParticle *Daughter = 0;
    HepMCParticle *WBoson = 0;
    HepMCParticle *BottomQuark = 0;
    // get top quark that is actually a mother in the MC tree
    for (Int_t i = 0; i < TopQuarks->GetEntries(); i++){
	if ( ((HepMCParticle*)TopQuarks->At(i))->GetN_Daughters() > 0 ) {
	    TopTrue = fEvent->GetGoodMother((HepMCParticle*)TopQuarks->At(i));
	}
    }
    
    TopTrue = fEvent->GetGoodMother((HepMCParticle*)TopQuarks->At(0));

    if ( TopTrue == 0 ) {
	Error("FillTruthMatchingHistograms",
	      "No Top daughters at all. Possibly bad MC sample!!!!!!!!!!!!!!"); //Abort!"); // gSystem->Abort(0);
    }
    
    for ( Int_t i = 0; i < TopTrue->GetN_Daughters(); i++ ){
	Daughter = (HepMCParticle*)TopTrue->GetDaughters()->At(i);
	if( Daughter->IsWBoson() ){
	    WBoson = Daughter;
 	    continue;
	}
	if (  Daughter->IsBeautyQuark() ) { // so far only looking into t->Wb
 	    BottomQuark = Daughter;
	}
    }
    
    // get particles that are actually mothers in the MC tree
    if ( WBoson ) {
	WBoson = fEvent->GetGoodMother((HepMCParticle*)WBoson);
    } else {
	if ( fVerbosityLevel > 0 ) Info("FillTruthMatchingHistograms","No W boson from top at all!");
    }
    
    if ( BottomQuark ) {
	BottomQuark = fEvent->GetGoodMother((HepMCParticle*)BottomQuark);
    } else {
	if ( fVerbosityLevel > 0 ) Info("FillTruthMatchingHistograms","No bottom quark from top at all!");
    }
        
    if ( WBoson == 0 || BottomQuark == 0 ) {
	if ( !WBoson && fVerbosityLevel > 0 )
	    Info("FillTruthMatchingHistograms","No good W boson available!");
	if ( !BottomQuark && fVerbosityLevel > 0 )
	    Info("FillTruthMatchingHistograms","No good bottom quark available!");
      Error("FillTruthMatchingHistograms",
	    "No proper W/b daughters from Top. Possibly bad MC sample. Abort!");
      gSystem->Abort(0);
    }
        
    delete TopQuarks;
    delete AntiTopQuarks;
    
    // get daughters of W boson
    HepMCParticle *ChargedLepton = 0;
    HepMCParticle *Neutrino = 0;
    for ( Int_t i = 0; i < WBoson->GetN_Daughters(); i++ ){
      Daughter = (HepMCParticle*)WBoson->GetDaughters()->At(i);
      if ( Daughter->IsChargedLepton() ){
	ChargedLepton = Daughter;
	continue;
      }
      if ( Daughter->IsNeutrino() ){
	Neutrino = Daughter;
      }
    }
    // get reconstructed charged lepton and b-jet
    // also get original b-jet which was input for the fitter
    AtlWDecayLNu* W  = (AtlWDecayLNu*)fEvent->GetWDecaysLNu()->At(0);
    HepTopDecay* t   = (HepTopDecay*)fEvent->GetTopDecays()->At(0);
    HepParticle* nu  = W->GetNeutrino();
    HepParticle* lep = W->GetChargedLepton();
    TLorentzVector p_BJet = t->GetP_BJet();
    HepJet* BJetOrig = t->GetBJetOrig();
    // Truth Matching for b-jet on Hadron level
    // 
    // find MC Hadron level jet matching true b-quark best
    // then compare the b-jet of reconstruction
    // with this MC Hadron level jet
    HepJet *TrueJet = 0;

    TrueJet = fEvent->FindMatchedMCJet((HepParticle*)BottomQuark, 1., 1.);
    // **** hard-coded numbers for DeltaR/DeltaPtFrac... ****
    
    // get momentum components of true b-quark,
    // Hadron Level MC Jet, charged lepton and neutrino
    Double_t Pt_b_quark     = BottomQuark->Pt();
    Double_t Eta_b_quark    = BottomQuark->Eta();
    Double_t Phi_b_quark    = BottomQuark->Phi();
    Double_t Pt_b_HLJet  = 0.;
    Double_t Eta_b_HLJet = 0.;
    Double_t Phi_b_HLJet = 0.;
    if ( TrueJet ){
	Pt_b_HLJet  = TrueJet->Pt();
	Eta_b_HLJet = TrueJet->Eta();
	Phi_b_HLJet = TrueJet->Phi();
    }
    Double_t Pt_Lep   = ChargedLepton->Pt();
    Double_t Eta_Lep  = ChargedLepton->Eta();
    Double_t Phi_Lep  = ChargedLepton->Phi();
    Double_t Pt_Nu    = Neutrino->Pt();
    Double_t Theta_Nu = Neutrino->Theta();
    Double_t Eta_Nu   = Neutrino->Eta();
    Double_t Phi_Nu   = Neutrino->Phi();
    // get reconstructed momentum components
    Double_t Pt_b_rec      = p_BJet.Pt();
    Double_t Eta_b_rec     = p_BJet.Eta();
    Double_t Phi_b_rec     = p_BJet.Phi();
    Double_t Pt_bOrig_rec  = BJetOrig->Pt();
    Double_t Eta_bOrig_rec = BJetOrig->Eta();
    Double_t Phi_bOrig_rec = BJetOrig->Phi();
    Double_t Pt_Lep_rec    = lep->Pt();
    Double_t Eta_Lep_rec   = lep->Eta();
    Double_t Phi_Lep_rec   = lep->Phi();
    Double_t Pt_Nu_rec     = nu->Pt();
    Double_t Theta_Nu_rec  = nu->Theta();
    Double_t Eta_Nu_rec    = nu->Eta();
    Double_t Phi_Nu_rec    = nu->Phi();    
    // fill Truth Matching histograms
    // electron and muon channel
    fHistNuTruthMatch_pt->Fill( (Pt_Nu_rec-Pt_Nu)/Pt_Nu, GetPreTagEvtWeight());
    fHistNuTruthMatch_theta->Fill( (Theta_Nu_rec-Theta_Nu)/Theta_Nu, GetPreTagEvtWeight());
    fHistNuTruthMatch_eta->Fill( (Eta_Nu_rec-Eta_Nu)/Eta_Nu, GetPreTagEvtWeight());
    fHistNuTruthMatch_phi->Fill( (Phi_Nu_rec-Phi_Nu)/Phi_Nu, GetPreTagEvtWeight());

    fHistBottomTruthMatch_pt->Fill( (Pt_b_rec-Pt_b_quark)/Pt_b_quark, GetPreTagEvtWeight());
    fHistBottomTruthMatch_eta->Fill( (Eta_b_rec-Eta_b_quark)/Eta_b_quark, GetPreTagEvtWeight());    
    fHistBottomTruthMatch_phi->Fill( (Phi_b_rec-Phi_b_quark)/Phi_b_quark, GetPreTagEvtWeight());
    fHistBottomTruthMatchOrig_pt->Fill( (Pt_bOrig_rec-Pt_b_quark)/Pt_b_quark, GetPreTagEvtWeight());
    fHistBottomTruthMatchOrig_eta->Fill( (Eta_bOrig_rec-Eta_b_quark)/Eta_b_quark, GetPreTagEvtWeight());    
    fHistBottomTruthMatchOrig_phi->Fill( (Phi_bOrig_rec-Phi_b_quark)/Phi_b_quark, GetPreTagEvtWeight());
    if ( TrueJet ){	
	fHistHJetTruthMatch_pt->Fill( (Pt_b_rec-Pt_b_HLJet)/Pt_b_HLJet, GetPreTagEvtWeight());
	fHistHJetTruthMatch_eta->Fill( (Eta_b_rec-Eta_b_HLJet)/Eta_b_HLJet, GetPreTagEvtWeight());    
	fHistHJetTruthMatch_phi->Fill( (Phi_b_rec-Phi_b_HLJet)/Phi_b_HLJet, GetPreTagEvtWeight());
	fHistHJetTruthMatchOrig_pt->Fill( (Pt_bOrig_rec-Pt_b_HLJet)/Pt_b_HLJet, GetPreTagEvtWeight());
	fHistHJetTruthMatchOrig_eta->Fill( (Eta_bOrig_rec-Eta_b_HLJet)/Eta_b_HLJet, GetPreTagEvtWeight());    
	fHistHJetTruthMatchOrig_phi->Fill( (Phi_bOrig_rec-Phi_b_HLJet)/Phi_b_HLJet, GetPreTagEvtWeight());
    }    
    // electron channel
    if ( lep->IsElectron() ){
      fHistElectronTruthMatch_pt->Fill( (Pt_Lep_rec-Pt_Lep)/Pt_Lep, GetPreTagEvtWeight());
      fHistElectronTruthMatch_eta->Fill( (Eta_Lep_rec-Eta_Lep)/Eta_Lep, GetPreTagEvtWeight());
      fHistElectronTruthMatch_phi->Fill( (Phi_Lep_rec-Phi_Lep)/Phi_Lep, GetPreTagEvtWeight());
      
      fHistNuTruthMatch_pt_e->Fill( (Pt_Nu_rec-Pt_Nu)/Pt_Nu, GetPreTagEvtWeight());
      fHistNuTruthMatch_theta_e->Fill( (Theta_Nu_rec-Theta_Nu)/Theta_Nu, GetPreTagEvtWeight());
      fHistNuTruthMatch_phi_e->Fill( (Phi_Nu_rec-Phi_Nu)/Phi_Nu, GetPreTagEvtWeight());
      
      fHistBottomTruthMatch_pt_e->Fill( (Pt_b_rec-Pt_b_quark)/Pt_b_quark, GetPreTagEvtWeight());
      fHistBottomTruthMatch_eta_e->Fill( (Eta_b_rec-Eta_b_quark)/Eta_b_quark, GetPreTagEvtWeight());
      fHistBottomTruthMatch_phi_e->Fill( (Phi_b_rec-Phi_b_quark)/Phi_b_quark, GetPreTagEvtWeight());
      
      if ( TrueJet ){
	  fHistHJetTruthMatch_pt_e->Fill( (Pt_b_rec-Pt_b_HLJet)/Pt_b_HLJet, GetPreTagEvtWeight());
	  fHistHJetTruthMatch_eta_e->Fill( (Eta_b_rec-Eta_b_HLJet)/Eta_b_HLJet, GetPreTagEvtWeight());    
	  fHistHJetTruthMatch_phi_e->Fill( (Phi_b_rec-Phi_b_HLJet)/Phi_b_HLJet, GetPreTagEvtWeight());
      }
    }
    // muon channel
    if ( lep->IsMuon() ){
      fHistMuonTruthMatch_pt->Fill( (Pt_Lep_rec-Pt_Lep)/Pt_Lep, GetPreTagEvtWeight());
      fHistMuonTruthMatch_eta->Fill( (Eta_Lep_rec-Eta_Lep)/Eta_Lep, GetPreTagEvtWeight());
      fHistMuonTruthMatch_phi->Fill( (Phi_Lep_rec-Phi_Lep)/Phi_Lep, GetPreTagEvtWeight());
      
      fHistNuTruthMatch_pt_mu->Fill( (Pt_Nu_rec-Pt_Nu)/Pt_Nu, GetPreTagEvtWeight());
      fHistNuTruthMatch_theta_mu->Fill( (Theta_Nu_rec-Theta_Nu)/Theta_Nu, GetPreTagEvtWeight());
      fHistNuTruthMatch_phi_mu->Fill( (Phi_Nu_rec-Phi_Nu)/Phi_Nu, GetPreTagEvtWeight());
      
      fHistBottomTruthMatch_pt_mu->Fill( (Pt_b_rec-Pt_b_quark)/Pt_b_quark, GetPreTagEvtWeight());
      fHistBottomTruthMatch_eta_mu->Fill( (Eta_b_rec-Eta_b_quark)/Eta_b_quark, GetPreTagEvtWeight());
      fHistBottomTruthMatch_phi_mu->Fill( (Phi_b_rec-Phi_b_quark)/Phi_b_quark, GetPreTagEvtWeight());
      
      if ( TrueJet ){
	  fHistHJetTruthMatch_pt_mu->Fill( (Pt_b_rec-Pt_b_HLJet)/Pt_b_HLJet, GetPreTagEvtWeight());
	  fHistHJetTruthMatch_eta_mu->Fill( (Eta_b_rec-Eta_b_HLJet)/Eta_b_HLJet, GetPreTagEvtWeight());    
	  fHistHJetTruthMatch_phi_mu->Fill( (Phi_b_rec-Phi_b_HLJet)/Phi_b_HLJet, GetPreTagEvtWeight());
      }
    }
    // fill histograms for overall MC sum of Px and Py
    // take into account all hadron level jets, neutrino
    // and charged lepton
    Double_t Px = 0.;
    Double_t Py = 0.;
    HepJet *jet;
    for ( Int_t i = 0; i < fEvent->GetN_MCAntiKt4HadronJets(); i++){// *** hard-coded JetType ***
	// try only Et > 10GeV jets
	jet=(HepJet*)fEvent->GetMCAntiKt4HadronJets()->At(i);
	if ( jet->Et() > 10. ){ // **** hard-coded number... ****
	    Px += jet->P3().X();
	    Py += jet->P3().Y();
	}
    }
    Px += ChargedLepton->Px();
    Py += ChargedLepton->Py();
    Px += Neutrino->Px();
    Py += Neutrino->Py();
    fHistTrueSumPx->Fill(Px, GetPreTagEvtWeight());
    fHistTrueSumPy->Fill(Py, GetPreTagEvtWeight());
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::ReconstructionCutBased() {
    //
    // SgTop_tChannel event reconstruction (cut-based)
    //

    // =========================
    // Refining event level cuts
    // =========================

    // Exactly 2 jets ?
    if ( fNJets != 2 ) return;
    fCutflow_tool_2->Fill("N_Jets = 2",GetPreTagEvtWeight());
    
    // Exactly 1 b-jet ?
    if ( fBJets->GetEntries() != 1 ) return;
    fCutflow_tool_2->Fill("N_BJets = 1",GetPreTagEvtWeight());

    
    // ===============================
    // Reconstruct top-quark now
    // ===============================
    ReconstructTop();
    
    // Fill CutBased candidate histograms
    if ( fRecoW != 0 && fRecoTop != 0 ) {
	
	// =====================
	// Further refining cuts
	// =====================
	FillCutBasedHistograms();
	FillCutBasedLeptonHistograms();
	FillCutBasedJetHistograms();
    }

}


//____________________________________________________________________


void AtlSgTop_tChannelFinder::ReconstructW() {
    //
    // reconstruction of W transverse 4-momentum and mass
    //

    if ( fRecoW != 0 ) {
	delete fRecoW;
	fRecoW = 0;
    }
    
    // Require at least a leading lepton
    if ( fLepton != 0 ) {
	// Get neutrino 4-vector by means of Et_miss	
	TLorentzVector p_nu(fMET.Px(),
			    fMET.Py(),
			    0.,fMET.Mod());
	
	// Get (transverse) leading lepton 4-momentum
	TLorentzVector p_lep = fLepton->P();
// 	if ( fLepton->IsElectron() ) {
// 	    // customized electron 4-momentum
// 	    Float_t cluster_e = ((AtlElectron*)fLepton)->ClusterE();
// 	    Float_t track_eta = ((AtlElectron*)fLepton)->GetIDTrack()->Eta();
// 	    Float_t track_phi = ((AtlElectron*)fLepton)->GetIDTrack()->Phi();
// 	    Float_t cosh_track_eta = TMath::CosH((Double_t)track_eta);
// 	    p_lep.SetPtEtaPhiE(
// 		cluster_e/cosh_track_eta, // pt
// 		track_eta,                // eta
// 		track_phi,                // phi
// 		cluster_e);               // e
// 	}
	p_lep.SetE(p_lep.Et());
	p_lep.SetPz(0.);
	
	// Reconstruct (transverse) W 4-momentum
	TLorentzVector p_W = p_nu + p_lep;

	// Which W?
	Int_t Wpdg = -24;
	if ( fLepton->IsPositive() ) Wpdg = 24;
	fRecoW = new HepParticle(1,p_W.Px(),p_W.Py(),p_W.Pz(),p_W.E(), Wpdg);

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

void AtlSgTop_tChannelFinder::ReconstructTop() {
    //
    // reconstruction of top-quark transverse 4-momentum and mass
    //
	
    if ( fRecoTop != 0 ) {
	delete fRecoTop;
	fRecoTop = 0;
    }

    //TLorentzVector p_top;
    
    // Require the W reconstruction to have been successful
    // and require at least one b-tagged jet
    if ( fRecoW != 0 && fBJets->GetEntries() > 0 ) {
    
	// Get jet transverse 4-momentum
	AtlJet *bjet = (AtlJet*)fBJets->At(0);
	TLorentzVector p_j = bjet->P();
	p_j.SetE(p_j.Et());
	p_j.SetPz(0.);
	
	// Reconstruct (transverse) top-quark 4-momentum
	TLorentzVector p_top = fRecoW->P() + p_j;
	    
	// Which top?
	if ( fLepton->IsPositive() ) {
	    
	    // t : PDG Code 6
	    fRecoTop = new HepParticle(1,p_top.Px(),p_top.Py(),p_top.Pz(),p_top.E(),6);
	}
	else {
	    // tbar : PDG Code -6
	    fRecoTop = new HepParticle(1,p_top.Px(),p_top.Py(),p_top.Pz(),p_top.E(),-6);
	}
    }
}

//____________________________________________________________________


void AtlSgTop_tChannelFinder::FillCutBasedHistograms() {
    //
    // Fill histograms for reconstructed W and top-quark in cut-based mode
    //
    
    if ( fRecoW != 0 ) {  // ***actually, a W and a top are already required in ReconstructionCutBased()...
	
	// W -> e nu
	if ( fLepton->IsElectron() ) {
	    
	    // Fill electron histograms of reconstructed W
	    fHistWreco_cutbased_Mt_e ->Fill(fWmassT, GetPreTagEvtWeight());
	    fHistWreco_cutbased_phi_e->Fill(fRecoW->Phi(), GetPreTagEvtWeight());
	    fHistWreco_cutbased_pt_e ->Fill(fRecoW->Pt(), GetPreTagEvtWeight());
	    fHistWreco_cutbased_eta_e ->Fill(fRecoW->Eta(), GetPreTagEvtWeight());
	    	    
	    fHistWreco_cutbased_Mt   ->Fill(fWmassT, GetPreTagEvtWeight());
	    fHistWreco_cutbased_phi  ->Fill(fRecoW->Phi(), GetPreTagEvtWeight());
	    fHistWreco_cutbased_pt   ->Fill(fRecoW->Pt(), GetPreTagEvtWeight());
	    fHistWreco_cutbased_eta ->Fill(fRecoW->Eta(), GetPreTagEvtWeight());
	    

	    if ( fLepton->IsNegative() ) {
		fHistWreco_cutbased_Mt_e_minus ->Fill(fWmassT, GetPreTagEvtWeight());
		fHistWreco_cutbased_phi_e_minus->Fill(fRecoW->Phi(), GetPreTagEvtWeight());
		fHistWreco_cutbased_pt_e_minus ->Fill(fRecoW->Pt(), GetPreTagEvtWeight());
		fHistWreco_cutbased_eta_e_minus ->Fill(fRecoW->Eta(), GetPreTagEvtWeight());

		fHistWreco_cutbased_Mt_minus ->Fill(fWmassT, GetPreTagEvtWeight());
		fHistWreco_cutbased_phi_minus->Fill(fRecoW->Phi(), GetPreTagEvtWeight());
		fHistWreco_cutbased_pt_minus ->Fill(fRecoW->Pt(), GetPreTagEvtWeight());
		fHistWreco_cutbased_eta_minus ->Fill(fRecoW->Eta(), GetPreTagEvtWeight());
	    }
	    else {
		fHistWreco_cutbased_Mt_e_plus ->Fill(fWmassT, GetPreTagEvtWeight());
		fHistWreco_cutbased_phi_e_plus->Fill(fRecoW->Phi(), GetPreTagEvtWeight());
		fHistWreco_cutbased_pt_e_plus ->Fill(fRecoW->Pt(), GetPreTagEvtWeight());
		fHistWreco_cutbased_eta_e_plus ->Fill(fRecoW->Eta(), GetPreTagEvtWeight());

		fHistWreco_cutbased_Mt_plus ->Fill(fWmassT, GetPreTagEvtWeight());
		fHistWreco_cutbased_phi_plus->Fill(fRecoW->Phi(), GetPreTagEvtWeight());
		fHistWreco_cutbased_pt_plus ->Fill(fRecoW->Pt(), GetPreTagEvtWeight());
		fHistWreco_cutbased_eta_plus ->Fill(fRecoW->Eta(), GetPreTagEvtWeight());
	    }



	    if ( fRecoTop != 0 ) {
	    
		//  Fill electron histograms of reconstructed top-quark 
		fHistTopReco_cutbased_Mt_e ->Fill(fRecoTop->M("REC"), GetPreTagEvtWeight());
		fHistTopReco_cutbased_phi_e->Fill(fRecoTop->Phi(), GetPreTagEvtWeight());
		fHistTopReco_cutbased_pt_e ->Fill(fRecoTop->Pt(), GetPreTagEvtWeight());
		fHistTopReco_cutbased_eta_e ->Fill(fRecoTop->Eta(), GetPreTagEvtWeight());
	
		fHistTopReco_cutbased_Mt   ->Fill(fRecoTop->M("REC"), GetPreTagEvtWeight());
		fHistTopReco_cutbased_phi  ->Fill(fRecoTop->Phi(), GetPreTagEvtWeight());
		fHistTopReco_cutbased_pt   ->Fill(fRecoTop->Pt(), GetPreTagEvtWeight());
		fHistTopReco_cutbased_eta ->Fill(fRecoTop->Eta(), GetPreTagEvtWeight());
		

		if ( fLepton->IsPositive() ) {
		    fHistTopReco_cutbased_Mt_e_plus ->Fill(fRecoTop->M("REC"), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_phi_e_plus->Fill(fRecoTop->Phi(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_pt_e_plus ->Fill(fRecoTop->Pt(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_eta_e_plus ->Fill(fRecoTop->Eta(), GetPreTagEvtWeight());

		    fHistTopReco_cutbased_Mt_plus ->Fill(fRecoTop->M("REC"), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_phi_plus->Fill(fRecoTop->Phi(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_pt_plus ->Fill(fRecoTop->Pt(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_eta_plus ->Fill(fRecoTop->Eta(), GetPreTagEvtWeight());
		}
		else {
		    fHistTopReco_cutbased_Mt_e_minus ->Fill(fRecoTop->M("REC"), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_phi_e_minus->Fill(fRecoTop->Phi(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_pt_e_minus ->Fill(fRecoTop->Pt(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_eta_e_minus ->Fill(fRecoTop->Eta(), GetPreTagEvtWeight());

		    fHistTopReco_cutbased_Mt_minus ->Fill(fRecoTop->M("REC"), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_phi_minus->Fill(fRecoTop->Phi(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_pt_minus ->Fill(fRecoTop->Pt(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_eta_minus ->Fill(fRecoTop->Eta(), GetPreTagEvtWeight());
		}


	    }
	}
	
	// W -> mu nu
	if ( fLepton->IsMuon() ) {

	    // Fill muon histograms of reconstructed W
	    fHistWreco_cutbased_Mt_mu ->Fill(fWmassT, GetPreTagEvtWeight());
	    fHistWreco_cutbased_phi_mu->Fill(fRecoW->Phi(), GetPreTagEvtWeight());
	    fHistWreco_cutbased_pt_mu ->Fill(fRecoW->Pt(), GetPreTagEvtWeight());
	    fHistWreco_cutbased_eta_mu ->Fill(fRecoW->Eta(), GetPreTagEvtWeight());
	    
	    fHistWreco_cutbased_Mt    ->Fill(fWmassT, GetPreTagEvtWeight());
	    fHistWreco_cutbased_phi   ->Fill(fRecoW->Phi(), GetPreTagEvtWeight());
	    fHistWreco_cutbased_pt    ->Fill(fRecoW->Pt(), GetPreTagEvtWeight());
	    fHistWreco_cutbased_eta ->Fill(fRecoW->Eta(), GetPreTagEvtWeight());
	    

	    if ( fLepton->IsNegative() ) {
		fHistWreco_cutbased_Mt_mu_minus ->Fill(fWmassT, GetPreTagEvtWeight());
		fHistWreco_cutbased_phi_mu_minus->Fill(fRecoW->Phi(), GetPreTagEvtWeight());
		fHistWreco_cutbased_pt_mu_minus ->Fill(fRecoW->Pt(), GetPreTagEvtWeight());
		fHistWreco_cutbased_eta_mu_minus ->Fill(fRecoW->Eta(), GetPreTagEvtWeight());
		
		fHistWreco_cutbased_Mt_minus ->Fill(fWmassT, GetPreTagEvtWeight());
		fHistWreco_cutbased_phi_minus->Fill(fRecoW->Phi(), GetPreTagEvtWeight());
		fHistWreco_cutbased_pt_minus ->Fill(fRecoW->Pt(), GetPreTagEvtWeight());
		fHistWreco_cutbased_eta_minus ->Fill(fRecoW->Eta(), GetPreTagEvtWeight());
	    }
	    else {
		fHistWreco_cutbased_Mt_mu_plus ->Fill(fWmassT, GetPreTagEvtWeight());
		fHistWreco_cutbased_phi_mu_plus->Fill(fRecoW->Phi(), GetPreTagEvtWeight());
		fHistWreco_cutbased_pt_mu_plus ->Fill(fRecoW->Pt(), GetPreTagEvtWeight());
		fHistWreco_cutbased_eta_mu_plus ->Fill(fRecoW->Eta(), GetPreTagEvtWeight());

		fHistWreco_cutbased_Mt_plus ->Fill(fWmassT, GetPreTagEvtWeight());
		fHistWreco_cutbased_phi_plus->Fill(fRecoW->Phi(), GetPreTagEvtWeight());
		fHistWreco_cutbased_pt_plus ->Fill(fRecoW->Pt(), GetPreTagEvtWeight());
		fHistWreco_cutbased_eta_plus ->Fill(fRecoW->Eta(), GetPreTagEvtWeight());
	    }



	    if ( fRecoTop != 0 ) {

		//  Fill muon histograms of reconstructed top-quark
		fHistTopReco_cutbased_Mt_mu ->Fill(fRecoTop->M("REC"), GetPreTagEvtWeight());
		fHistTopReco_cutbased_phi_mu->Fill(fRecoTop->Phi(), GetPreTagEvtWeight());
		fHistTopReco_cutbased_pt_mu ->Fill(fRecoTop->Pt(), GetPreTagEvtWeight());
		fHistTopReco_cutbased_eta_mu ->Fill(fRecoTop->Eta(), GetPreTagEvtWeight());

		fHistTopReco_cutbased_Mt    ->Fill(fRecoTop->M("REC"), GetPreTagEvtWeight());
		fHistTopReco_cutbased_phi   ->Fill(fRecoTop->Phi(), GetPreTagEvtWeight());
		fHistTopReco_cutbased_pt    ->Fill(fRecoTop->Pt(), GetPreTagEvtWeight());
		fHistTopReco_cutbased_eta ->Fill(fRecoTop->Eta(), GetPreTagEvtWeight());

		
		if ( fLepton->IsPositive() ) {
		    fHistTopReco_cutbased_Mt_mu_plus ->Fill(fRecoTop->M("REC"), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_phi_mu_plus->Fill(fRecoTop->Phi(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_pt_mu_plus ->Fill(fRecoTop->Pt(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_eta_mu_plus ->Fill(fRecoTop->Eta(), GetPreTagEvtWeight());

		    fHistTopReco_cutbased_Mt_plus ->Fill(fRecoTop->M("REC"), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_phi_plus->Fill(fRecoTop->Phi(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_pt_plus ->Fill(fRecoTop->Pt(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_eta_plus ->Fill(fRecoTop->Eta(), GetPreTagEvtWeight());
		}
		else {
		    fHistTopReco_cutbased_Mt_mu_minus ->Fill(fRecoTop->M("REC"), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_phi_mu_minus->Fill(fRecoTop->Phi(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_pt_mu_minus ->Fill(fRecoTop->Pt(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_eta_mu_minus ->Fill(fRecoTop->Eta(), GetPreTagEvtWeight());
		    
		    fHistTopReco_cutbased_Mt_minus ->Fill(fRecoTop->M("REC"), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_phi_minus->Fill(fRecoTop->Phi(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_pt_minus ->Fill(fRecoTop->Pt(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_eta_minus ->Fill(fRecoTop->Eta(), GetPreTagEvtWeight());
		}
		
	    }
	}

	// W -> tau nu
	if ( fLepton->IsTau() ) {

	    // Fill tauon histograms of reconstructed W
	    fHistWreco_cutbased_Mt_tau ->Fill(fWmassT, GetPreTagEvtWeight());
	    fHistWreco_cutbased_phi_tau->Fill(fRecoW->Phi(), GetPreTagEvtWeight());
	    fHistWreco_cutbased_pt_tau ->Fill(fRecoW->Pt(), GetPreTagEvtWeight());
	    fHistWreco_cutbased_eta_tau ->Fill(fRecoW->Eta(), GetPreTagEvtWeight());
	    
	    fHistWreco_cutbased_Mt    ->Fill(fWmassT, GetPreTagEvtWeight());
	    fHistWreco_cutbased_phi   ->Fill(fRecoW->Phi(), GetPreTagEvtWeight());
	    fHistWreco_cutbased_pt    ->Fill(fRecoW->Pt(), GetPreTagEvtWeight());
	    fHistWreco_cutbased_eta ->Fill(fRecoW->Eta(), GetPreTagEvtWeight());
	    

	    if ( fLepton->IsNegative() ) {
		fHistWreco_cutbased_Mt_tau_minus ->Fill(fWmassT, GetPreTagEvtWeight());
		fHistWreco_cutbased_phi_tau_minus->Fill(fRecoW->Phi(), GetPreTagEvtWeight());
		fHistWreco_cutbased_pt_tau_minus ->Fill(fRecoW->Pt(), GetPreTagEvtWeight());
		fHistWreco_cutbased_eta_tau_minus ->Fill(fRecoW->Eta(), GetPreTagEvtWeight());

		fHistWreco_cutbased_Mt_minus ->Fill(fWmassT, GetPreTagEvtWeight());
		fHistWreco_cutbased_phi_minus->Fill(fRecoW->Phi(), GetPreTagEvtWeight());
		fHistWreco_cutbased_pt_minus ->Fill(fRecoW->Pt(), GetPreTagEvtWeight());
		fHistWreco_cutbased_eta_minus ->Fill(fRecoW->Eta(), GetPreTagEvtWeight());
	    }
	    else {
		fHistWreco_cutbased_Mt_tau_plus ->Fill(fWmassT, GetPreTagEvtWeight());
		fHistWreco_cutbased_phi_tau_plus->Fill(fRecoW->Phi(), GetPreTagEvtWeight());
		fHistWreco_cutbased_pt_tau_plus ->Fill(fRecoW->Pt(), GetPreTagEvtWeight());
		fHistWreco_cutbased_eta_tau_plus ->Fill(fRecoW->Eta(), GetPreTagEvtWeight());

		fHistWreco_cutbased_Mt_plus ->Fill(fWmassT, GetPreTagEvtWeight());
		fHistWreco_cutbased_phi_plus->Fill(fRecoW->Phi(), GetPreTagEvtWeight());
		fHistWreco_cutbased_pt_plus ->Fill(fRecoW->Pt(), GetPreTagEvtWeight());
		fHistWreco_cutbased_eta_plus ->Fill(fRecoW->Eta(), GetPreTagEvtWeight());
	    }



	    if ( fRecoTop != 0 ) {
	    
		//  Fill tauon histograms of reconstructed top-quark
		fHistTopReco_cutbased_Mt_tau ->Fill(fRecoTop->M("REC"), GetPreTagEvtWeight());
		fHistTopReco_cutbased_phi_tau->Fill(fRecoTop->Phi(), GetPreTagEvtWeight());
		fHistTopReco_cutbased_pt_tau ->Fill(fRecoTop->Pt(), GetPreTagEvtWeight());
		fHistTopReco_cutbased_eta_tau ->Fill(fRecoTop->Eta(), GetPreTagEvtWeight());
		
		fHistTopReco_cutbased_Mt    ->Fill(fRecoTop->M("REC"), GetPreTagEvtWeight());
		fHistTopReco_cutbased_phi   ->Fill(fRecoTop->Phi(), GetPreTagEvtWeight());
		fHistTopReco_cutbased_pt    ->Fill(fRecoTop->Pt(), GetPreTagEvtWeight());
		fHistTopReco_cutbased_eta ->Fill(fRecoTop->Eta(), GetPreTagEvtWeight());
		

		if ( fLepton->IsPositive() ) {
		    fHistTopReco_cutbased_Mt_tau_plus ->Fill(fRecoTop->M("REC"), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_phi_tau_plus->Fill(fRecoTop->Phi(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_pt_tau_plus ->Fill(fRecoTop->Pt(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_eta_tau_plus ->Fill(fRecoTop->Eta(), GetPreTagEvtWeight());

		    fHistTopReco_cutbased_Mt_plus ->Fill(fRecoTop->M("REC"), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_phi_plus->Fill(fRecoTop->Phi(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_pt_plus ->Fill(fRecoTop->Pt(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_eta_plus ->Fill(fRecoTop->Eta(), GetPreTagEvtWeight());
		}
		else {
		    fHistTopReco_cutbased_Mt_tau_minus ->Fill(fRecoTop->M("REC"), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_phi_tau_minus->Fill(fRecoTop->Phi(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_pt_tau_minus ->Fill(fRecoTop->Pt(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_eta_tau_minus ->Fill(fRecoTop->Eta(), GetPreTagEvtWeight());

		    fHistTopReco_cutbased_Mt_minus ->Fill(fRecoTop->M("REC"), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_phi_minus->Fill(fRecoTop->Phi(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_pt_minus ->Fill(fRecoTop->Pt(), GetPreTagEvtWeight());
		    fHistTopReco_cutbased_eta_minus ->Fill(fRecoTop->Eta(), GetPreTagEvtWeight());
		}
	    }
	}
    }
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::FillCutBasedLeptonHistograms() {
    //
    // Fill lepton histograms for top candidates in cut-based mode
    //

    // =============
    // Signal lepton
    // =============
    if ( fLepton != 0 ) {
	Float_t Pt_sig  = fLepton->Pt();
	Float_t Eta_sig = fLepton->Eta();
	Float_t Phi_sig = fLepton->Phi();
	
	fHistCutBasedCandidateSignalLepton_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
	fHistCutBasedCandidateSignalLepton_eta->Fill(Eta_sig, GetPreTagEvtWeight());
	fHistCutBasedCandidateSignalLepton_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	
	// e
	if ( fLepton->IsElectron() ) {
	    //*************************************
	    // Number of signal electrons by author
	    //*************************************
	    // Mind event weights at low statistics!!!
	    AtlEMShower::EAuthor author = ((AtlElectron*)fLepton)->GetAuthor();

	    for ( Int_t j = 0; j < AtlEMShower::fgNAuthors; j++ ) {
		if ( author & (1<<j) ) fHistCutBasedCandidateSignalElectron_author->AddBinContent(j+1, GetPreTagEvtWeight());
	    }
	    // set of electrons that are highPt AND soft
	    if ( (author & 2) && (author & 4) )
		fHistCutBasedCandidateSignalElectron_author->AddBinContent(6, GetPreTagEvtWeight());
//	    fHistCutBasedCandidateSignalElectron_author->Sumw2();
	   
	    fHistCutBasedCandidateSignalElectron_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
	    fHistCutBasedCandidateSignalElectron_eta->Fill(Eta_sig, GetPreTagEvtWeight());
	    fHistCutBasedCandidateSignalElectron_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    if ( fLepton->IsPositive() ) {
		fHistCutBasedCandidateSignalEplus_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistCutBasedCandidateSignalEplus_eta->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistCutBasedCandidateSignalEplus_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    } else {
		fHistCutBasedCandidateSignalEminus_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistCutBasedCandidateSignalEminus_eta->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistCutBasedCandidateSignalEminus_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    }
	}
	
	// mu
	if ( fLepton->IsMuon() ) {
	    fHistCutBasedCandidateSignalMuon_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
	    fHistCutBasedCandidateSignalMuon_eta->Fill(Eta_sig, GetPreTagEvtWeight());
	    fHistCutBasedCandidateSignalMuon_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    if ( fLepton->IsPositive() ) {
		fHistCutBasedCandidateSignalMuplus_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistCutBasedCandidateSignalMuplus_eta->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistCutBasedCandidateSignalMuplus_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    } else {
		fHistCutBasedCandidateSignalMuminus_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistCutBasedCandidateSignalMuminus_eta->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistCutBasedCandidateSignalMuminus_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    }
	}
	
	// tau
	if ( fLepton->IsTau() ) {
	    fHistCutBasedCandidateSignalTau_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
	    fHistCutBasedCandidateSignalTau_eta->Fill(Eta_sig, GetPreTagEvtWeight());
	    fHistCutBasedCandidateSignalTau_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    if ( fLepton->IsPositive() ) {
		fHistCutBasedCandidateSignalTauplus_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistCutBasedCandidateSignalTauplus_eta->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistCutBasedCandidateSignalTauplus_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    } else {
		fHistCutBasedCandidateSignalTauminus_pt ->Fill(Pt_sig,  GetPreTagEvtWeight());
		fHistCutBasedCandidateSignalTauminus_eta->Fill(Eta_sig, GetPreTagEvtWeight());
		fHistCutBasedCandidateSignalTauminus_phi->Fill(Phi_sig, GetPreTagEvtWeight());
	    }
	}
    }
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::FillCutBasedJetHistograms() {
    //
    // Fill jet histograms for top candidates in cut-based mode
    //
    Float_t Et  = 0.;
    Float_t eta = 0.;
    Float_t phi = 0.;
    AtlJet *jet = 0;

    // Jets
    TIter next_jet(fJets);
    Int_t njet = 0;
    while ( (jet = (AtlJet*)next_jet()) ) {
	Et  = jet->Et();
	eta = jet->Eta();
	phi = jet->Phi();
	fHistCutBasedCandidateJet_Et ->Fill(Et, GetPreTagEvtWeight());
	fHistCutBasedCandidateJet_eta->Fill(eta, GetPreTagEvtWeight());
	fHistCutBasedCandidateJet_phi->Fill(phi, GetPreTagEvtWeight());
	if ( njet == 0 ) {
	    fHistCutBasedCandidateJet1_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistCutBasedCandidateJet1_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistCutBasedCandidateJet1_phi->Fill(phi, GetPreTagEvtWeight());
	}
	if ( njet == 1 ) {
	    fHistCutBasedCandidateJet2_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistCutBasedCandidateJet2_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistCutBasedCandidateJet2_phi->Fill(phi, GetPreTagEvtWeight());
	}
	njet++;
    }
    fHistCutBasedCandidateJet_n->Fill(njet, GetPreTagEvtWeight());

    // B-Jets
    TIter next_bjet(fBJets);
    njet = 0;
    while ( (jet = (AtlJet*)next_bjet()) ) {
	Et  = jet->Et();
	eta = jet->Eta();
	phi = jet->Phi();
	fHistCutBasedCandidateBJet_Et ->Fill(Et, GetPreTagEvtWeight());
	fHistCutBasedCandidateBJet_eta->Fill(eta, GetPreTagEvtWeight());
	fHistCutBasedCandidateBJet_phi->Fill(phi, GetPreTagEvtWeight());
	if ( njet == 0 ) {
	    fHistCutBasedCandidateBJet1_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistCutBasedCandidateBJet1_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistCutBasedCandidateBJet1_phi->Fill(phi, GetPreTagEvtWeight());
	}
	if ( njet == 1 ) {
	    fHistCutBasedCandidateBJet2_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistCutBasedCandidateBJet2_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistCutBasedCandidateBJet2_phi->Fill(phi, GetPreTagEvtWeight());
	}
	njet++;
    }
    fHistCutBasedCandidateBJet_n->Fill(njet, GetPreTagEvtWeight());

    // Non-B-Jets
    TIter next_nonbjet(fNonBJets);
    njet = 0;
    while ( (jet = (AtlJet*)next_nonbjet()) ) {
	Et  = jet->Et();
	eta = jet->Eta();
	phi = jet->Phi();
	fHistCutBasedCandidateNonBJet_Et ->Fill(Et, GetPreTagEvtWeight());
	fHistCutBasedCandidateNonBJet_eta->Fill(eta, GetPreTagEvtWeight());
	fHistCutBasedCandidateNonBJet_phi->Fill(phi, GetPreTagEvtWeight());
	if ( njet == 0 ) {
	    fHistCutBasedCandidateNonBJet1_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistCutBasedCandidateNonBJet1_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistCutBasedCandidateNonBJet1_phi->Fill(phi, GetPreTagEvtWeight());
	}
	if ( njet == 1 ) {
	    fHistCutBasedCandidateNonBJet2_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistCutBasedCandidateNonBJet2_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistCutBasedCandidateNonBJet2_phi->Fill(phi, GetPreTagEvtWeight());
	}
	njet++;
    }
    fHistCutBasedCandidateNonBJet_n->Fill(njet, GetPreTagEvtWeight());

    // Fwd-Jets
    TIter next_fwdjet(fFwdJets);
    njet = 0;
    while ( (jet = (AtlJet*)next_fwdjet()) ) {
	Et  = jet->Et();
	eta = jet->Eta();
	phi = jet->Phi();
	fHistCutBasedCandidateFwdJet_Et ->Fill(Et, GetPreTagEvtWeight());
	fHistCutBasedCandidateFwdJet_eta->Fill(eta, GetPreTagEvtWeight());
	fHistCutBasedCandidateFwdJet_phi->Fill(phi, GetPreTagEvtWeight());
	if ( njet == 0 ) {
	    fHistCutBasedCandidateFwdJet1_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistCutBasedCandidateFwdJet1_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistCutBasedCandidateFwdJet1_phi->Fill(phi, GetPreTagEvtWeight());
	}
	if ( njet == 1 ) {
	    fHistCutBasedCandidateFwdJet2_Et ->Fill(Et, GetPreTagEvtWeight());
	    fHistCutBasedCandidateFwdJet2_eta->Fill(eta, GetPreTagEvtWeight());
	    fHistCutBasedCandidateFwdJet2_phi->Fill(phi, GetPreTagEvtWeight());
	}
	njet++;
    }
    fHistCutBasedCandidateFwdJet_n->Fill(njet, GetPreTagEvtWeight());
}


//____________________________________________________________________

void AtlSgTop_tChannelFinder::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    
    // Mode of finder
    fProcessMode       = kMainAnalysis;
    fMode              = kAll;
    fModeMass          = kGauss;
    fIsFwdElectronMode = kFALSE;
    fSignalSampleMode  = kFALSE;
    fMaxNbIter         = 100;
    fDoTtbarVeto       = kFALSE;
    fDoWhadVeto        = kFALSE;
    fSFSystMode        = kNone;
    fIsAcceptanceChallenge = kFALSE;
    fVerbosityLevel    = 0;

    // cut-based analysis cuts
//    fLeadNonBJetEtaMin      = 2.;
    fH_TMin                 = 160.;
    fDeltaEtaBJetNonBJetMin = 0.8;
    fMassAllJetsMin         = 450.;
    

    fGuessNeutrinoEtaE = kFALSE;
    fTruthMatch        = kFALSE;

    // P(chi2) cut
    fChi2ProbMin = 0.05;
    
    // caution! HepDatabasePDG is NOT up to date    
    fW_Mass   = 80.399;// <-- current online PDG values (was 80.4) 
    fW_Width  = 2.085;// <-- current online PDG values (was 2.108)
    fTop_Mass = 172.9;// <-- current online PDG values (was 172.5)
    fTop_Width = 1.99; // <-- current online PDG values (was 2.); quite uncertain, not measured, depends on mass

    // Electrons
    fElPt_Min = 30.; // was 25.

    // Muons
    fMuPt_Min = 30.; // was 25.

    // Minimum W transverse mass
    fWmassT_min = 30.;
    
    // ====
    // Jets
    // ====
    
    // Jet multiplicity
    fNJetsMin = 1;  
    fNJetsMax = 5;

        
    // Forward jets
    fFwdJets_Et_min   = 30.0;
    fFwdJets_Eta_min  = 2.5;
    fFwdJets_Eta_max  = 5.;
    fFwdJetTagger     = AtlBTag::kMV1;
    fFwdBJetWeightMax = 5.85;

    // Generic jets
    fJets_Et_min  = 30.0;// was 25.;     
    fJets_Eta_min = -4.5; 
    fJets_Eta_max = 4.5; 

    // B-jets
    fBJets_Et_min  = 30.; // was 25.     
    fBJets_Eta_min = -2.5; 
    fBJets_Eta_max = 2.5;
    fBJetTagger    = AtlBTag::kMV1;
    fBJetWeightMin = 0.905363; 
    fBJetRefTagger    = AtlBTag::kMV1;
    fBJetRefWeightMin = 0.905363; 


    // Jets from hadronic W
    fWhadJets_Et_min  = 30.; // was: 10 GeV, but jets available in common D3PDs are all above 25 GeV...
    fWhadJets_Eta_min = -2.0; 
    fWhadJets_Eta_max = 2.0;

    
    // ==========
    // Whad veto
    // ==========
    fVetoWhadProbMin = 0.05;

}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::Terminate() {
    //
    // Terminate Single-top t-channel finder
    //

    // Close output text file for acceptance challenges
    fOut.close(); 
    
    //
    // Efficiency of kinematic fit dependent on value of Chi2Prob cut w.r.t.
    // event yield after Whad veto
    //
    if ( fSignalSampleMode ) {
	Int_t n_bins = fHistChi2Prob_both->GetNbinsX();
	for( Int_t i = 0; i < n_bins; i++) {
	    fHistEfficiencyVsChi2ProbCut->
		AddBinContent(i+1,
			      fHistChi2Prob_both->Integral(i+1,n_bins)/
			      fHistChi2Prob_both->Integral(1,n_bins));
	}
    }
    

    //
    // Fill histogram with slices of fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet in eta direction
    // for better signal discrimination:
    //
    fOutputFile->cd();
    gDirectory->cd("AtlSgTop_tChannelFinder/KinFitterMode/RecoObjects_PChi2_and_otherCuts/Miscellaneous");

    Int_t n_eta_slices = 15;

    // Fine binning
    Int_t nbins_mod = 25;
    Int_t nbins = nbins_mod*n_eta_slices; // 375

    fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_fine = 
      new TH1F("h_deltap2mod_top_2ndleadjet_sliced_in_Eta2ndLeadJet_fine",
	       "Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of 2nd lead. jet (Range 0. to 100.), in 15 slices of #eta of 2nd lead. jet",
	       nbins, 0., 1500.);
    fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_fine->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{2nd lead jet}|");
    fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_fine->SetYTitle("Number of Entries");

    Int_t offset = 0;
    Int_t curr_bin = 0;
    Float_t curr_value = 0;
    Float_t curr_err = 0;

    for ( Int_t i = 1; i <= n_eta_slices; i++ ) {
      for ( Int_t j = 1; j <= nbins_mod; j++ ) {
	curr_bin = offset+j;
	curr_value = fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_fine->GetBinContent(j,i);
	curr_err = fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_fine->GetBinError(j,i);
	fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_fine->SetBinContent(curr_bin, curr_value);
	fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_fine->SetBinError(curr_bin, curr_err);
      }
      offset += nbins_mod;
    }


    // Normal binning
    nbins_mod = 10;
    nbins = nbins_mod*n_eta_slices; // 150

    fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet = 
      new TH1F("h_deltap2mod_top_2ndleadjet_sliced_in_Eta2ndLeadJet",
	       "Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of 2nd lead. jet (Range 0. to 100.), in 15 slices of #eta of 2nd lead. jet",
	       nbins, 0., 1500.);
    fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{2nd lead jet}|");
    fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet->SetYTitle("Number of Entries");

    offset = 0;
    curr_bin = 0;
    curr_value = 0;
    curr_err = 0;

    for ( Int_t i = 1; i <= n_eta_slices; i++ ) {
      for ( Int_t j = 1; j <= nbins_mod; j++ ) {
	curr_bin = offset+j;
	curr_value = fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet->GetBinContent(j,i);
	curr_err = fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet->GetBinError(j,i);
	fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet->SetBinContent(curr_bin, curr_value);
	fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet->SetBinError(curr_bin, curr_err);
      }
      offset += nbins_mod;
    }


    // Coarse binning
    n_eta_slices = 5;
    nbins = nbins_mod*n_eta_slices; // 50
    
    fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_coarse = 
	new TH1F("h_deltap2mod_top_2ndleadjet_sliced_in_Eta2ndLeadJet_coarse",
		 "Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of 2nd lead. jet (Range 0. to 100.), in 5 slices of #eta of 2nd lead. jet",
		 nbins, 0., 500.);
    fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_coarse->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{2nd lead jet}|");
    fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_coarse->SetYTitle("Number of Entries");
    
    offset = 0;
    curr_bin = 0;
    curr_value = 0;
    curr_err = 0;

    for ( Int_t i = 1; i <= n_eta_slices; i++ ) {
      for ( Int_t j = 1; j <= nbins_mod; j++ ) {
	curr_bin = offset+j;
	curr_value = fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_coarse->GetBinContent(j,i);
	curr_err = fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_coarse->GetBinError(j,i);
	fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_coarse->SetBinContent(curr_bin, curr_value);
	fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_coarse->SetBinError(curr_bin, curr_err);
      }
      offset += nbins_mod;
    }

    // Optimized binning 
    nbins_mod = 10;
    n_eta_slices = 11;
    nbins = nbins_mod*n_eta_slices; // 110
    
    fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_Less_Bins =
	new TH1F("h_deltap2mod_top_2ndleadjet_sliced_in_Eta2ndLeadJet_Less_Bins",
		 "Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of 2nd lead. jet (Range 0. to 100.), in 11 slices of #eta of 2nd lead. jet",
		 nbins, 0., 1100.);
    fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_Less_Bins->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{2nd lead jet}|");
    fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_Less_Bins->SetYTitle("Number of Entries");
    
        
    offset = 0;
    curr_bin = 0;
    curr_value = 0;
    curr_err = 0;

    for ( Int_t i = 1; i <= n_eta_slices; i++ ) {
      for ( Int_t j = 1; j <= nbins_mod; j++ ) {
	   curr_bin = offset+j; 
	   curr_value = fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins->GetBinContent(j,i);
	   curr_err = fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins->GetBinError(j,i);
	   fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_Less_Bins->SetBinContent(curr_bin, curr_value);
	   fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_Less_Bins->SetBinError(curr_bin, curr_err);	   
      }
      offset += nbins_mod;
    }
    
    // Optimized binning2 
    nbins_mod = 10;
    n_eta_slices = 9;
    nbins = nbins_mod*n_eta_slices; // 90
    
    fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_Less_Bins2 =
	new TH1F("h_deltap2mod_top_2ndleadjet_sliced_in_Eta2ndLeadJet_Less_Bins2",
		 "Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of 2nd lead. jet (Range 0. to 100.), in 11 slices of #eta of 2nd lead. jet",
		 nbins, 0., 900.);
    fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_Less_Bins2->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{2nd lead jet}|");
    fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_Less_Bins2->SetYTitle("Number of Entries");
    
        
    offset = 0;
    curr_bin = 0;
    curr_value = 0;
    curr_err = 0;

    for ( Int_t i = 1; i <= n_eta_slices; i++ ) {
      for ( Int_t j = 1; j <= nbins_mod; j++ ) {
	   curr_bin = offset+j; 
	   curr_value = fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins2->GetBinContent(j,i);
	   curr_err = fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins2->GetBinError(j,i);
	   fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_Less_Bins2->SetBinContent(curr_bin, curr_value);
	   fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_Less_Bins2->SetBinError(curr_bin, curr_err);	   
      }
      offset += nbins_mod;
    }
    
    // Optimized binning3 
    nbins_mod = 10;
    n_eta_slices = 7;
    nbins = nbins_mod*n_eta_slices; // 70
    
    fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_Less_Bins3 =
	new TH1F("h_deltap2mod_top_2ndleadjet_sliced_in_Eta2ndLeadJet_Less_Bins3",
		 "Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of 2nd lead. jet (Range 0. to 100.), in 11 slices of #eta of 2nd lead. jet",
		 nbins, 0., 700.);
    fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_Less_Bins3->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{2nd lead jet}|");
    fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_Less_Bins3->SetYTitle("Number of Entries");
    
        
    offset = 0;
    curr_bin = 0;
    curr_value = 0;
    curr_err = 0;

    for ( Int_t i = 1; i <= n_eta_slices; i++ ) {
      for ( Int_t j = 1; j <= nbins_mod; j++ ) {
	   curr_bin = offset+j; 
	   curr_value = fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins3->GetBinContent(j,i);
	   curr_err = fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins3->GetBinError(j,i);
	   fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_Less_Bins3->SetBinContent(curr_bin, curr_value);
	   fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_Less_Bins3->SetBinError(curr_bin, curr_err);	   
      }
      offset += nbins_mod;
    }


    
    fOutputFile->cd();

    AtlKinFitterTool::Terminate();

}

//--------------------------------------------------------------------

void AtlSgTop_tChannelFinder::Print() const {
    //
    // Prints chosen parameter settings for the finder tool to
    // standard output
    //

    cout << endl
	 << "========================================================" << endl
	 << "  Single-Top t-Channel Finder Tool                      " << endl
	 << "========================================================" << endl
	 << "  Chosen run mode is                             ";
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
	if ( fUseLeptonCovRand ) {
	    cout
		<< "  fUseLeptonCovRand                              = kTRUE" << endl
		<< "  fIsFwdElectronMode                             = " << ( (fIsFwdElectronMode) ? "kTRUE" : "kFALSE" ) << endl;
	} else cout
	     << "  fUseLeptonCovRand                              = kFALSE"<<endl;
	cout << "  fMaxNbIter                                     = " << fMaxNbIter << endl
	     << "  Chosen veto configuration is: " << endl
	     << "  fDoTtbarVeto                                   = " << ( (fDoTtbarVeto) ? "kTRUE" : "kFALSE" ) << endl
	     << "  fDoWhadVeto                                    = " << ( (fDoWhadVeto)  ? "kTRUE" : "kFALSE" ) << endl;
	cout << "  fSFSystMode                                    = " << fgSystNames[fSFSystMode] << endl;
	cout << "  Chosen mass constraint is    "
	 << ( (fModeMass == kBW) ? "kBW" : "kGauss" ) << endl
	 << "  Is GuessNeutrinoEtaE used?   "
	 << ( (fGuessNeutrinoEtaE) ? "kTRUE" : "kFALSE" ) << endl
	 << "  Is truth-matching applied?   "
	 << ( (fTruthMatch) ? "kTRUE" : "kFALSE" ) << endl
	 << "  Chosen main P(chi2) cut is   fChi2ProbMin      = "
	 << fChi2ProbMin << endl
	 << "  Chosen Whad veto cut is      fVetoWhadProbMin  = "
	 << fVetoWhadProbMin << endl
	 << "  Minimum jet multiplicity is  fNJetsMin         = "
	 << fNJetsMin << endl
	 << "  Maximum jet multiplicity is  fNJetsMax         = "
	 << fNJetsMax << endl
	 << "--------------------------------------------------------" << endl	
	 << "  Cut-based analysis cuts:" << endl
	 << "--------------------------------------------------------" << endl
	 << "  fLeadNonBJetEtaMin         = " << fLeadNonBJetEtaMin << endl
	 << "  fH_TMin                    = " << fH_TMin << endl
	 << "  fDeltaEtaBJetNonBJetMin    = " << fDeltaEtaBJetNonBJetMin << endl
	 << "  fMassAllJetsMin            = " << fMassAllJetsMin << endl
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
       << "  fWmassT_min                = " << fWmassT_min << endl
	 << "--------------------------------------------------------" << endl
	 << "  fJets_Et_min               = " << fJets_Et_min << endl
	 << "  fJets_Eta_min              = " << fJets_Eta_min << endl
	 << "  fJets_Eta_max              = " << fJets_Eta_max << endl   
	 << "  fBJets_Et_min              = " << fBJets_Et_min << endl
	 << "  fBJets_Eta_min             = " << fBJets_Eta_min << endl
	 << "  fBJets_Eta_max             = " << fBJets_Eta_max << endl
	 << "  fBJetTagger                = " << AtlBTag::GetTaggerName(fBJetTagger) << endl
	 << "  fBJetWeightMin             = " << fBJetWeightMin << endl
	 << "  fBJetRefTagger             = " << AtlBTag::GetTaggerName(fBJetRefTagger) << endl
	 << "  fBJetRefWeightMin          = " << fBJetRefWeightMin << endl
         << "  fWhadJets_Et_min           = " << fWhadJets_Et_min << endl
	 << "  fWhadJets_Eta_min          = " << fWhadJets_Eta_min << endl
	 << "  fWhadJets_Eta_max          = " << fWhadJets_Eta_max << endl
	 << "  fFwdJets_Et_min            = " << fFwdJets_Et_min << endl
	 << "  fFwdJets_Eta_min           = " << fFwdJets_Eta_min << endl
	 << "  fFwdJets_Eta_max           = " << fFwdJets_Eta_max << endl
// 	 << "  fFwdJetTagger              = " << AtlBTag::GetTaggerName(fFwdJetTagger) << endl
// 	 << "  fFwdBJetWeightMax          = " << fFwdBJetWeightMax << endl
	 << "========================================================" << endl
	 << endl;
	
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::BookHistograms() {
    //
    // Book histograms for
    // -- event selection objects
    // -- reconstructed objects (KinFitter and/or cut-based mode)
    // -- fit performance (KinFitter mode)
    // -- cut-flow (KinFitter mode)
    // -- truth-matching (optional in KinFitter mode)
    //

    // Objects used for event selection
    BookLeptonHistograms();
    BookJetHistograms();
    BookLeptonHistograms_BTag();
    BookJetHistograms_BTag();
    
    if (fMode & kKinFit) {
	// Objects reconstructed by KinFitter analysis
	BookKinFitHistograms();
	BookHadronicWHistograms();
	// Fit performance
	BookChi2Histograms();
	BookComparisonHistograms();
	// Optional truth-matching
	if ( fTruthMatch == kTRUE ) BookTruthMatchingHistograms();
	// Generic KinFitter performance and cut-flow
	AtlKinFitterTool::BookHistograms();
    }
    if (fMode & kCutBased) {
	// Objects reconstructed by cut-based analysis
	BookCutBasedHistograms();
    }

    // Initialize pointers to tools needed in event loop
    fCutflow_tool   = (AtlCutFlowTool*)GetTool("AtlCutFlowTool","AtlCutFlowTool",kTRUE);
    fCutflow_tool_2 = (AtlCutFlowTool*)GetTool("AtlCutFlowTool","AtlCutFlowTool2",kTRUE);

    // Initialize text file for acceptance challenge output
    // gDirectory->cd("$LIBSINGLETOP/tasks");
//    fOut("logfile_AcceptanceChallenge");
    // fOut.open("logfile_AcceptanceChallenge");
    
}

//____________________________________________________________________
    
void AtlSgTop_tChannelFinder::BookLeptonHistograms() {
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



    gDirectory->cd("..");
    
    gDirectory->mkdir("CommonEventSelection", "Objects used for common event selection");
    gDirectory->cd("CommonEventSelection");

    gDirectory->mkdir("EventVariables","Event variables");
    gDirectory->cd("EventVariables");

    // QCD rejection cut variables (pretag)
    fHistW_Mt_QCDValidation_pretag = new TH1F("h_w_mt_qcd_valid_pretag", "W Transverse Mass after common SgTop selection (pretag)",
				    100, 0, 500);
    fHistW_Mt_QCDValidation_pretag->SetXTitle("m_{t}^{W} (GeV)");
    fHistW_Mt_QCDValidation_pretag->SetYTitle("Number of Entries");

    fHistW_Mt_EtmissMag_QCDValidation_pretag = new TH1F("h_w_mt_etmiss_mag_qcd_valid_pretag", "Triangular cut variable (W Transverse Mass + Etmiss ) after common SgTop selection (pretag)",
				    100, 0, 500);
    fHistW_Mt_EtmissMag_QCDValidation_pretag->SetXTitle("m_{t}^{W}+E_{T}^{Miss} (GeV)");
    fHistW_Mt_EtmissMag_QCDValidation_pretag->SetYTitle("Number of Entries");


    // QCD rejection cut variables (tag)
    fHistW_Mt_QCDValidation_tag = new TH1F("h_w_mt_qcd_valid_tag", "W Transverse Mass after common SgTop selection (tag)",
				    100, 0, 500);
    fHistW_Mt_QCDValidation_tag->SetXTitle("m_{t}^{W} (GeV)");
    fHistW_Mt_QCDValidation_tag->SetYTitle("Number of Entries");

    fHistW_Mt_EtmissMag_QCDValidation_tag = new TH1F("h_w_mt_etmiss_mag_qcd_valid_tag", "Triangular cut variable (W Transverse Mass + Etmiss ) after common SgTop selection (tag)",
				    100, 0, 500);
    fHistW_Mt_EtmissMag_QCDValidation_tag->SetXTitle("m_{t}^{W}+E_{T}^{Miss} (GeV)");
    fHistW_Mt_EtmissMag_QCDValidation_tag->SetYTitle("Number of Entries");

    
    // Etmiss, SumEt (pretag, tag)
    fHistEtMissMag_pretag = new TH1F("h_etmiss_mag_pretag", "EtMiss after common SgTop selection (pretag)",
				    40, 0, 200);
    fHistEtMissMag_pretag->SetXTitle("E_{T}^{Miss} (GeV)");
    fHistEtMissMag_pretag->SetYTitle("Number of Entries");

    fHistEtMissPhi_pretag = new TH1F("h_etmiss_phi_pretag", "#phi(EtMiss) after common SgTop selection (pretag)",
				    40, -3.2, 3.2);
    fHistEtMissPhi_pretag->SetXTitle("#phi(E_{T}^{Miss})");
    fHistEtMissPhi_pretag->SetYTitle("Number of Entries");

    
    fHistEtMissMag_tag = new TH1F("h_etmiss_mag_tag", "EtMiss after B-Tag requirement (tag)",
				    40, 0, 200);
    fHistEtMissMag_tag->SetXTitle("E_{T}^{Miss} (GeV)");
    fHistEtMissMag_tag->SetYTitle("Number of Entries");

    fHistEtMissPhi_tag = new TH1F("h_etmiss_phi_tag", "#phi(EtMiss) after B-Tag requirement (tag)",
				    40, -3.2, 3.2);
    fHistEtMissPhi_tag->SetXTitle("#phi(E_{T}^{Miss})");
    fHistEtMissPhi_tag->SetYTitle("Number of Entries");

    
    
    fHistSumEt_pretag = new TH1F("h_sum_et_pretag", "SumEt after common SgTop selection (pretag)",
				    40, 0, 1000);
    fHistSumEt_pretag->SetXTitle("#sum E_{t} (GeV)");
    fHistSumEt_pretag->SetYTitle("Number of Entries");

    fHistSumEt_tag = new TH1F("h_sum_et_tag", "SumEt after B-Tag requirement (tag)",
				    40, 0, 1000);
    fHistSumEt_tag->SetXTitle("#sum E_{t} (GeV)");
    fHistSumEt_tag->SetYTitle("Number of Entries");


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

    
    // ----------------
    // W control region
    // ----------------
    fHistCountFwdElectrons_WCtrl = new TH1F("h_count_fwd_electrons_wctrl", 
				      "Weighted number of events with forward electrons (wctrl)",
				      1, 0, 1);
    fHistCountFwdElectrons_WCtrl->SetXTitle("passed wctrl");
    fHistCountFwdElectrons_WCtrl->SetYTitle("Number of Entries");
    axis_qcd = fHistCountFwdElectrons_WCtrl->GetXaxis();
    axis_qcd->SetBinLabel(1,"True");

    fHistCountCentralElectrons_WCtrl = new TH1F("h_count_central_electrons_wctrl", 
				      "Weighted number of events with central electrons (wctrl)",
				      1, 0, 1);
    fHistCountCentralElectrons_WCtrl->SetXTitle("passed wctrl");
    fHistCountCentralElectrons_WCtrl->SetYTitle("Number of Entries");
    axis_qcd = fHistCountCentralElectrons_WCtrl->GetXaxis();
    axis_qcd->SetBinLabel(1,"True");
    

    

    //
    // Histograms of pretag and tag counts by lepton flavour for standalone tool 
    // determining  W reweighting factors
    //

    // ------
    // Pretag
    // ------

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


    // pretag, lepton all, W ctrl region
    fHistCountLepAll_WCtrl_pretag = new TH1F("h_count_lep_all_wctrl_pretag", 
				     "Total number of events (weighted, pretag)",
				     1, 0, 1);
    fHistCountLepAll_WCtrl_pretag->SetXTitle("passed pretag");
    fHistCountLepAll_WCtrl_pretag->SetYTitle("Number of Entries");
    axis1 = fHistCountLepAll_WCtrl_pretag->GetXaxis();
    axis1->SetBinLabel(1,"True");

    
    // pretag, lepton +
    fHistCountLepPlus_WCtrl_pretag = new TH1F("h_count_lep_plus_wctrl_pretag", 
				     "Number of events (weighted) with positive lepton (pretag)",
				     1, 0, 1);
    fHistCountLepPlus_WCtrl_pretag->SetXTitle("passed pretag");
    fHistCountLepPlus_WCtrl_pretag->SetYTitle("Number of Entries");
    axis1 = fHistCountLepPlus_WCtrl_pretag->GetXaxis();
    axis1->SetBinLabel(1,"True");


    // pretag, lepton -
    fHistCountLepMinus_WCtrl_pretag = new TH1F("h_count_lep_minus_wctrl_pretag", 
				      "Number of events (weighted) with negative lepton (pretag)",
				      1, 0, 1);
    fHistCountLepMinus_WCtrl_pretag->SetXTitle("passed pretag");
    fHistCountLepMinus_WCtrl_pretag->SetYTitle("Number of Entries");
    axis1 = fHistCountLepMinus_WCtrl_pretag->GetXaxis();
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


    
    // tag, lepton all, W ctrl region
    fHistCountLepAll_WCtrl_tag = new TH1F("h_count_lep_all_wctrl_tag", 
				     "Total number of events (weighted, tag)",
				     1, 0, 1);
    fHistCountLepAll_WCtrl_tag->SetXTitle("passed tag");
    fHistCountLepAll_WCtrl_tag->SetYTitle("Number of Entries");
    axis1 = fHistCountLepAll_WCtrl_tag->GetXaxis();
    axis1->SetBinLabel(1,"True");

    
    // tag, lepton +
    fHistCountLepPlus_WCtrl_tag = new TH1F("h_count_lep_plus_wctrl_tag", 
				     "Number of events (weighted) with positive lepton (tag)",
				     1, 0, 1);
    fHistCountLepPlus_WCtrl_tag->SetXTitle("passed tag");
    fHistCountLepPlus_WCtrl_tag->SetYTitle("Number of Entries");
    axis1 = fHistCountLepPlus_WCtrl_tag->GetXaxis();
    axis1->SetBinLabel(1,"True");


    // tag, lepton -
    fHistCountLepMinus_WCtrl_tag = new TH1F("h_count_lep_minus_wctrl_tag", 
				      "Number of events (weighted) with negative lepton (tag)",
				      1, 0, 1);
    fHistCountLepMinus_WCtrl_tag->SetXTitle("passed tag");
    fHistCountLepMinus_WCtrl_tag->SetYTitle("Number of Entries");
    axis1 = fHistCountLepMinus_WCtrl_tag->GetXaxis();
    axis1->SetBinLabel(1,"True");

    
    
    // tag, lepton all, QCD reference
    fHistCountLepAll_tag_refQCD = new TH1F("h_count_lep_all_tag_refQCD", 
				     "Total number of events (weighted, tag)",
				     1, 0, 1);
    fHistCountLepAll_tag_refQCD->SetXTitle("passed tag");
    fHistCountLepAll_tag_refQCD->SetYTitle("Number of Entries");
    axis1 = fHistCountLepAll_tag_refQCD->GetXaxis();
    axis1->SetBinLabel(1,"True");

    
    // tag, lepton +, QCD reference
    fHistCountLepPlus_tag_refQCD = new TH1F("h_count_lep_plus_tag_refQCD", 
				     "Number of events (weighted) with positive lepton (tag)",
				     1, 0, 1);
    fHistCountLepPlus_tag_refQCD->SetXTitle("passed tag");
    fHistCountLepPlus_tag_refQCD->SetYTitle("Number of Entries");
    axis1 = fHistCountLepPlus_tag_refQCD->GetXaxis();
    axis1->SetBinLabel(1,"True");


    // tag, lepton -, QCD reference
    fHistCountLepMinus_tag_refQCD = new TH1F("h_count_lep_minus_tag_refQCD", 
				      "Number of events (weighted) with negative lepton (tag)",
				      1, 0, 1);
    fHistCountLepMinus_tag_refQCD->SetXTitle("passed tag");
    fHistCountLepMinus_tag_refQCD->SetYTitle("Number of Entries");
    axis1 = fHistCountLepMinus_tag_refQCD->GetXaxis();
    axis1->SetBinLabel(1,"True");



    // top_hfor_flag for use by wjets flavour reweighting tool
    fHist_HFOR_flag_pretag = new TH1F("h_hfor_flag_pretag", "HFOR flag after common SgTop selection (pretag)",
				    4, 0, 4);
    fHist_HFOR_flag_pretag->SetXTitle("HFOR flag (pretag)");
    fHist_HFOR_flag_pretag->SetYTitle("Number of Entries");

    
    // pileup control plots (pretag)
    fHist_AverageIntPerXing_pretag = new TH1F("h_average_int_per_xing_pretag", "Average Interactions per Bunch-Crossing (pretag)",
			  40, 0., 40.);
    fHist_AverageIntPerXing_pretag->SetXTitle("#bar{N}_{ints per xing}");
    fHist_AverageIntPerXing_pretag->SetYTitle("Number of Entries");

    fHist_NPrimaryVertices_pretag = new TH1F("h_n_vxp_pretag", "Number of Primary Vertices in Event (pretag)",
			  40, 0., 40.);
    fHist_NPrimaryVertices_pretag->SetXTitle("N_{prim. vertices}");
    fHist_NPrimaryVertices_pretag->SetYTitle("Number of Entries");

    // pileup control plots (tag)
    fHist_AverageIntPerXing_tag = new TH1F("h_average_int_per_xing_tag", "Average Interactions per Bunch-Crossing (tag)",
			  40, 0., 40.);
    fHist_AverageIntPerXing_tag->SetXTitle("#bar{N}_{ints per xing}");
    fHist_AverageIntPerXing_tag->SetYTitle("Number of Entries");

    fHist_NPrimaryVertices_tag = new TH1F("h_n_vxp_tag", "Number of Primary Vertices in Event (tag)",
			  40, 0., 40.);
    fHist_NPrimaryVertices_tag->SetXTitle("N_{prim. vertices}");
    fHist_NPrimaryVertices_tag->SetYTitle("Number of Entries");

    
    gDirectory->cd("..");
    
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
				     50, -2.5, 2.5);
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

    fHistSignalLepton_EtCone20 = new TH1F("h_sig_lepton_EtCone20", "EtCone20",
					  15, 0, 30);
    fHistSignalLepton_EtCone20->SetXTitle("EtCone20 [GeV]");
    fHistSignalLepton_EtCone20->SetYTitle("Number of Entries");

    fHistSignalLepton_PtCone20 = new TH1F("h_sig_lepton_PtCone20", "PtCone20",
					  30, 0, 300);
    fHistSignalLepton_PtCone20->SetXTitle("PtCone20 [GeV]");
    fHistSignalLepton_PtCone20->SetYTitle("Number of Entries");

    fHistSignalLepton_EtCone30 = new TH1F("h_sig_lepton_EtCone30", "EtCone30",
					  15, 0, 30);
    fHistSignalLepton_EtCone30->SetXTitle("EtCone30 [GeV]");
    fHistSignalLepton_EtCone30->SetYTitle("Number of Entries");

    fHistSignalLepton_PtCone30 = new TH1F("h_sig_lepton_PtCone30", "PtCone30",
					  30, 0, 300);
    fHistSignalLepton_PtCone30->SetXTitle("PtCone30 [GeV]");
    fHistSignalLepton_PtCone30->SetYTitle("Number of Entries");

    

    fHistSignalLepton_EtRatio = new TH1F("h_sig_lepton_EtRatio", "EtCone30/LeptonPt",
				     40, 0, 1);
    fHistSignalLepton_EtRatio->SetXTitle("EtRatio [GeV]");
    fHistSignalLepton_EtRatio->SetYTitle("Number of Entries");

    fHistSignalLepton_PtRatio = new TH1F("h_sig_lepton_PtRatio", "PtCone30/LeptonPt",
					 40, 0, 1);
    fHistSignalLepton_PtRatio->SetXTitle("PtRatio [GeV]");
    fHistSignalLepton_PtRatio->SetYTitle("Number of Entries");

    
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
				      40, 0, 200);
    fHistSignalElectron_pt->SetXTitle("P_{t} (GeV)");
    fHistSignalElectron_pt->SetYTitle("Number of Entries");
    fHistSignalElectron_eta = new TH1F("h_sig_electron_eta", "Signal e Pseudo-Rapidity",
				       40, -2.5, 2.5);
    fHistSignalElectron_eta->SetXTitle("#eta");
    fHistSignalElectron_eta->SetYTitle("Number of Entries");
    fHistSignalElectron_phi = new TH1F("h_sig_electron_phi", "Signal e Azimuth",
				       40, -3.2, 3.2);
    fHistSignalElectron_phi->SetXTitle("#phi (rad)");
    fHistSignalElectron_phi->SetYTitle("Number of Entries");

    fHistSignalElectron_EtCone20 = new TH1F("h_sig_electron_EtCone20", "EtCone20",
					    15, 0, 30);
    fHistSignalElectron_EtCone20->SetXTitle("EtCone20 [GeV]");
    fHistSignalElectron_EtCone20->SetYTitle("Number of Entries");

    fHistSignalElectron_PtCone20 = new TH1F("h_sig_electron_PtCone20", "PtCone20",
					    30, 0, 300);
    fHistSignalElectron_PtCone20->SetXTitle("PtCone20 [GeV]");
    fHistSignalElectron_PtCone20->SetYTitle("Number of Entries");

    fHistSignalElectron_EtCone30 = new TH1F("h_sig_electron_EtCone30", "EtCone30",
					    15, 0, 30);
    fHistSignalElectron_EtCone30->SetXTitle("EtCone30 [GeV]");
    fHistSignalElectron_EtCone30->SetYTitle("Number of Entries");

    fHistSignalElectron_PtCone30 = new TH1F("h_sig_electron_PtCone30", "PtCone30",
					    30, 0, 300);
    fHistSignalElectron_PtCone30->SetXTitle("PtCone30 [GeV]");
    fHistSignalElectron_PtCone30->SetYTitle("Number of Entries");
    

    fHistSignalElectron_EtRatio = new TH1F("h_sig_electron_EtRatio", "EtCone30/ElectronPt",
				     40, 0, 1);
    fHistSignalElectron_EtRatio->SetXTitle("EtRatio [GeV]");
    fHistSignalElectron_EtRatio->SetYTitle("Number of Entries");

    fHistSignalElectron_PtRatio = new TH1F("h_sig_electron_PtRatio", "PtCone30/ElectronPt",
				     40, 0, 1);
    fHistSignalElectron_PtRatio->SetXTitle("PtRatio [GeV]");
    fHistSignalElectron_PtRatio->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    // e+
    gDirectory->mkdir("e+");
    gDirectory->cd("e+");

    fHistSignalEplus_pt = new TH1F("h_sig_eplus_pt", "Signal e^{+} Transverse Momentum",
				   40, 0, 200);
    fHistSignalEplus_pt->SetXTitle("P_{t} (GeV)");
    fHistSignalEplus_pt->SetYTitle("Number of Entries");
    fHistSignalEplus_eta = new TH1F("h_sig_eplus_eta", "Signal e^{+} Pseudo-Rapidity",
				    40, -2.5, 2.5);
    fHistSignalEplus_eta->SetXTitle("#eta");
    fHistSignalEplus_eta->SetYTitle("Number of Entries");
    fHistSignalEplus_phi = new TH1F("h_sig_eplus_phi", "Signal e^{+} Azimuth",
				    40, -3.2, 3.2);
    fHistSignalEplus_phi->SetXTitle("#phi (rad)");
    fHistSignalEplus_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // e-
    gDirectory->mkdir("e-");
    gDirectory->cd("e-");

    fHistSignalEminus_pt = new TH1F("h_sig_eminus_pt", "Signal e^{-} Transverse Momentum",
				   40, 0, 200);
    fHistSignalEminus_pt->SetXTitle("P_{t} (GeV)");
    fHistSignalEminus_pt->SetYTitle("Number of Entries");
    fHistSignalEminus_eta = new TH1F("h_sig_eminus_eta", "Signal e^{-} Pseudo-Rapidity",
				     40, -2.5, 2.5);
    fHistSignalEminus_eta->SetXTitle("#eta");
    fHistSignalEminus_eta->SetYTitle("Number of Entries");
    fHistSignalEminus_phi = new TH1F("h_sig_eminus_phi", "Signal e^{-} Azimuth",
				    40, -3.2, 3.2);
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
				  40, 0, 200);
    fHistSignalMuon_pt->SetXTitle("P_{t} (GeV)");
    fHistSignalMuon_pt->SetYTitle("Number of Entries");
    fHistSignalMuon_eta = new TH1F("h_sig_muon_eta", "Signal #mu Pseudo-Rapidity",
				   40, -2.5, 2.5);
    fHistSignalMuon_eta->SetXTitle("#eta");
    fHistSignalMuon_eta->SetYTitle("Number of Entries");
    fHistSignalMuon_phi = new TH1F("h_sig_muon_phi", "Signal #mu Azimuth",
				   40, -3.2, 3.2);
    fHistSignalMuon_phi->SetXTitle("#phi (rad)");
    fHistSignalMuon_phi->SetYTitle("Number of Entries");

    fHistSignalMuon_EtCone20 = new TH1F("h_sig_muon_EtCone20", "EtCone20",
					15, 0, 30);
    fHistSignalMuon_EtCone20->SetXTitle("EtCone20 [GeV]");
    fHistSignalMuon_EtCone20->SetYTitle("Number of Entries");

    fHistSignalMuon_PtCone20 = new TH1F("h_sig_muon_PtCone20", "PtCone20",
					30, 0, 300);
    fHistSignalMuon_PtCone20->SetXTitle("PtCone20 [GeV]");
    fHistSignalMuon_PtCone20->SetYTitle("Number of Entries");

    fHistSignalMuon_EtCone30 = new TH1F("h_sig_muon_EtCone30", "EtCone30",
					15, 0, 30);
    fHistSignalMuon_EtCone30->SetXTitle("EtCone30 [GeV]");
    fHistSignalMuon_EtCone30->SetYTitle("Number of Entries");

    fHistSignalMuon_PtCone30 = new TH1F("h_sig_muon_PtCone30", "PtCone30",
					30, 0, 300);
    fHistSignalMuon_PtCone30->SetXTitle("PtCone30 [GeV]");
    fHistSignalMuon_PtCone30->SetYTitle("Number of Entries");

    fHistSignalMuon_EtRatio = new TH1F("h_sig_muon_EtRatio", "EtCone30/MuonPt",
				       40, 0, 1);
    fHistSignalMuon_EtRatio->SetXTitle("EtRatio [GeV]");
    fHistSignalMuon_EtRatio->SetYTitle("Number of Entries");

    fHistSignalMuon_PtRatio = new TH1F("h_sig_muon_PtRatio", "PtCone30/MuonPt",
				       40, 0, 1);
    fHistSignalMuon_PtRatio->SetXTitle("PtRatio [GeV]");
    fHistSignalMuon_PtRatio->SetYTitle("Number of Entries");

    
    gDirectory->cd("..");
    
    // mu+
    gDirectory->mkdir("mu+");
    gDirectory->cd("mu+");

    fHistSignalMuplus_pt = new TH1F("h_sig_muplus_pt", "Signal #mu^{+} Transverse Momentum",
				    40, 0, 200);
    fHistSignalMuplus_pt->SetXTitle("P_{t} (GeV)");
    fHistSignalMuplus_pt->SetYTitle("Number of Entries");
    fHistSignalMuplus_eta = new TH1F("h_sig_muplus_eta", "Signal #mu^{+} Pseudo-Rapidity",
				     40, -2.5, 2.5);
    fHistSignalMuplus_eta->SetXTitle("#eta");
    fHistSignalMuplus_eta->SetYTitle("Number of Entries");
    fHistSignalMuplus_phi = new TH1F("h_sig_muplus_phi", "Signal #mu^{+} Azimuth",
				     40, -3.2, 3.2);
    fHistSignalMuplus_phi->SetXTitle("#phi (rad)");
    fHistSignalMuplus_phi->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    // mu-
    gDirectory->mkdir("mu-");
    gDirectory->cd("mu-");

    fHistSignalMuminus_pt = new TH1F("h_sig_muminus_pt", "Signal #mu^{-} Transverse Momentum",
				   40, 0, 200);
    fHistSignalMuminus_pt->SetXTitle("P_{t} (GeV)");
    fHistSignalMuminus_pt->SetYTitle("Number of Entries");
    fHistSignalMuminus_eta = new TH1F("h_sig_muminus_eta", "Signal #mu^{-} Pseudo-Rapidity",
				     40, -2.5, 2.5);
    fHistSignalMuminus_eta->SetXTitle("#eta");
    fHistSignalMuminus_eta->SetYTitle("Number of Entries");
    fHistSignalMuminus_phi = new TH1F("h_sig_muminus_phi", "Signal #mu^{-} Azimuth",
				    40, -3.2, 3.2);
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
				  40, 0, 200);
    fHistSignalTau_pt->SetXTitle("P_{t} (GeV)");
    fHistSignalTau_pt->SetYTitle("Number of Entries");
    fHistSignalTau_eta = new TH1F("h_sig_tau_eta", "Signal #tau Pseudo-Rapidity",
				   40, -2.5, 2.5);
    fHistSignalTau_eta->SetXTitle("#eta");
    fHistSignalTau_eta->SetYTitle("Number of Entries");
    fHistSignalTau_phi = new TH1F("h_sig_tau_phi", "Signal #tau Azimuth",
				   40, -3.2, 3.2);
    fHistSignalTau_phi->SetXTitle("#phi (rad)");
    fHistSignalTau_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // tau+
    gDirectory->mkdir("tau+");
    gDirectory->cd("tau+");

    fHistSignalTauplus_pt = new TH1F("h_sig_tauplus_pt", "Signal #tau^{+} Transverse Momentum",
				    40, 0, 200);
    fHistSignalTauplus_pt->SetXTitle("P_{t} (GeV)");
    fHistSignalTauplus_pt->SetYTitle("Number of Entries");
    fHistSignalTauplus_eta = new TH1F("h_sig_tauplus_eta", "Signal #tau^{+} Pseudo-Rapidity",
				     40, -2.5, 2.5);
    fHistSignalTauplus_eta->SetXTitle("#eta");
    fHistSignalTauplus_eta->SetYTitle("Number of Entries");
    fHistSignalTauplus_phi = new TH1F("h_sig_tauplus_phi", "Signal #tau^{+} Azimuth",
				     40, -3.2, 3.2);
    fHistSignalTauplus_phi->SetXTitle("#phi (rad)");
    fHistSignalTauplus_phi->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    // tau-
    gDirectory->mkdir("tau-");
    gDirectory->cd("tau-");

    fHistSignalTauminus_pt = new TH1F("h_sig_tauminus_pt", "Signal #tau^{-} Transverse Momentum",
				   40, 0, 200);
    fHistSignalTauminus_pt->SetXTitle("P_{t} (GeV)");
    fHistSignalTauminus_pt->SetYTitle("Number of Entries");
    fHistSignalTauminus_eta = new TH1F("h_sig_tauminus_eta", "Signal #tau^{-} Pseudo-Rapidity",
				     40, -2.5, 2.5);
    fHistSignalTauminus_eta->SetXTitle("#eta");
    fHistSignalTauminus_eta->SetYTitle("Number of Entries");
    fHistSignalTauminus_phi = new TH1F("h_sig_tauminus_phi", "Signal #tau^{-} Azimuth",
				    40, -3.2, 3.2);
    fHistSignalTauminus_phi->SetXTitle("#phi (rad)");
    fHistSignalTauminus_phi->SetYTitle("Number of Entries");
    
    gDirectory->cd("../../../..");
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::BookLeptonHistograms_BTag() {
    //
    // Book histograms for the leptons used in the reconstruction
    // (b-tag selection)
    //
    gDirectory->mkdir("CommonEventSelection_BTag", "Objects used for common event selection");
    gDirectory->cd("CommonEventSelection_BTag");

    gDirectory->mkdir("CutBasedCuts","Event variables (cut-based cuts)");
    gDirectory->cd("CutBasedCuts");

    
    // Mass of all jets
    fHistJet_MassAllJets = new TH1D("h_jet_mass_all_jets", "Mass of all jets together",
			       20, 0., 700.);
    fHistJet_MassAllJets->SetXTitle("Mass(all jets)");
    fHistJet_MassAllJets->SetYTitle("Number of Entries");
    
    // H_T
    fHistJet_H_T = new TH1F("h_h_t_all_jet_lepton_etmiss", "H_{T}: lepton pt + pt of all jets + etmiss",
			       20, 0., 800.);
    fHistJet_H_T->SetXTitle("H_{T}");
    fHistJet_H_T->SetYTitle("Number of Entries");

    // del_eta_bjet_ltjet 
    fHistDeltaEtaBJetLightJet = new TH1F("h_del_eta_bjet_ltjet", "#Delta#eta(Hardest BJet,Hardest LightJet)",
			       25, 0., 10.);
    fHistDeltaEtaBJetLightJet->SetXTitle("#Delta#eta");
    fHistDeltaEtaBJetLightJet->SetYTitle("Number of Entries");

    // eta of hardest non-bjet 
    fHistHardestLightJetEta = new TH1F("h_eta_hardest_ltjet", "#eta(Hardest LightJet)",
			       25, -5., 5.);
    fHistHardestLightJetEta->SetXTitle("#eta");
    fHistHardestLightJetEta->SetYTitle("Number of Entries");

    gDirectory->cd("..");



    
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

    fHistSignalLepton_EtCone20_btag = new TH1F("h_sig_lepton_EtCone20_btag", "EtCone20",
				     15, 0, 30);
    fHistSignalLepton_EtCone20_btag->SetXTitle("EtCone20 [GeV]");
    fHistSignalLepton_EtCone20_btag->SetYTitle("Number of Entries");

    fHistSignalLepton_PtCone20_btag = new TH1F("h_sig_lepton_PtCone20_btag", "PtCone20",
				     30, 0, 300);
    fHistSignalLepton_PtCone20_btag->SetXTitle("PtCone20 [GeV]");
    fHistSignalLepton_PtCone20_btag->SetYTitle("Number of Entries");
    
    fHistSignalLepton_EtCone30_btag = new TH1F("h_sig_lepton_EtCone30_btag", "EtCone30",
				     15, 0, 30);
    fHistSignalLepton_EtCone30_btag->SetXTitle("EtCone30 [GeV]");
    fHistSignalLepton_EtCone30_btag->SetYTitle("Number of Entries");

    fHistSignalLepton_PtCone30_btag = new TH1F("h_sig_lepton_PtCone30_btag", "PtCone30",
				     30, 0, 300);
    fHistSignalLepton_PtCone30_btag->SetXTitle("PtCone30 [GeV]");
    fHistSignalLepton_PtCone30_btag->SetYTitle("Number of Entries");
    fHistSignalLepton_EtRatio_btag = new TH1F("h_sig_lepton_EtRatio_btag", "EtCone30/LeptonPt",
					      100, 0, 1);
    fHistSignalLepton_EtRatio_btag->SetXTitle("EtRatio [GeV]");
    fHistSignalLepton_EtRatio_btag->SetYTitle("Number of Entries");

    fHistSignalLepton_PtRatio_btag = new TH1F("h_sig_lepton_PtRatio_btag", "PtCone30/LeptonPt",
					      100, 0, 1);
    fHistSignalLepton_PtRatio_btag->SetXTitle("PtRatio [GeV]");
    fHistSignalLepton_PtRatio_btag->SetYTitle("Number of Entries");


    
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
				      40, 0, 200);
    fHistSignalElectron_pt_btag->SetXTitle("P_{t} (GeV)");
    fHistSignalElectron_pt_btag->SetYTitle("Number of Entries");
    fHistSignalElectron_eta_btag = new TH1F("h_sig_electron_eta_btag_btag", "Signal e Pseudo-Rapidity",
				       40, -2.5, 2.5);
    fHistSignalElectron_eta_btag->SetXTitle("#eta");
    fHistSignalElectron_eta_btag->SetYTitle("Number of Entries");
    fHistSignalElectron_phi_btag = new TH1F("h_sig_electron_phi_btag", "Signal e Azimuth",
				       40, -3.2, 3.2);
    fHistSignalElectron_phi_btag->SetXTitle("#phi (rad)");
    fHistSignalElectron_phi_btag->SetYTitle("Number of Entries");

    fHistSignalElectron_EtCone20_btag = new TH1F("h_sig_electron_EtCone20_btag", "EtCone20",
				     15, 0, 30);
    fHistSignalElectron_EtCone20_btag->SetXTitle("EtCone20 [GeV]");
    fHistSignalElectron_EtCone20_btag->SetYTitle("Number of Entries");

    fHistSignalElectron_PtCone20_btag = new TH1F("h_sig_electron_PtCone20_btag", "PtCone20",
				     30, 0, 300);
    fHistSignalElectron_PtCone20_btag->SetXTitle("PtCone20 [GeV]");
    fHistSignalElectron_PtCone20_btag->SetYTitle("Number of Entries");
    
    fHistSignalElectron_EtCone30_btag = new TH1F("h_sig_electron_EtCone30_btag", "EtCone30",
				     15, 0, 30);
    fHistSignalElectron_EtCone30_btag->SetXTitle("EtCone30 [GeV]");
    fHistSignalElectron_EtCone30_btag->SetYTitle("Number of Entries");

    fHistSignalElectron_PtCone30_btag = new TH1F("h_sig_electron_PtCone30_btag", "PtCone30",
				     30, 0, 300);
    fHistSignalElectron_PtCone30_btag->SetXTitle("PtCone30 [GeV]");
    fHistSignalElectron_PtCone30_btag->SetYTitle("Number of Entries");
    
    fHistSignalElectron_EtRatio_btag = new TH1F("h_sig_electron_EtRatio_btag", "EtCone30/ElectronPt",
				     100, 0, 1);
    fHistSignalElectron_EtRatio_btag->SetXTitle("EtRatio [GeV]");
    fHistSignalElectron_EtRatio_btag->SetYTitle("Number of Entries");

    fHistSignalElectron_PtRatio_btag = new TH1F("h_sig_electron_PtRatio_btag", "PtCone30/ElectronPt",
				      100, 0, 1);
    fHistSignalElectron_PtRatio_btag->SetXTitle("PtRatio [GeV]");
    fHistSignalElectron_PtRatio_btag->SetYTitle("Number of Entries");

    
    gDirectory->cd("..");
    
    // e+
    gDirectory->mkdir("e+");
    gDirectory->cd("e+");

    fHistSignalEplus_pt_btag = new TH1F("h_sig_eplus_pt_btag", "Signal e^{+} Transverse Momentum",
				   40, 0, 200);
    fHistSignalEplus_pt_btag->SetXTitle("P_{t} (GeV)");
    fHistSignalEplus_pt_btag->SetYTitle("Number of Entries");
    fHistSignalEplus_eta_btag = new TH1F("h_sig_eplus_eta_btag", "Signal e^{+} Pseudo-Rapidity",
				    40, -2.5, 2.5);
    fHistSignalEplus_eta_btag->SetXTitle("#eta");
    fHistSignalEplus_eta_btag->SetYTitle("Number of Entries");
    fHistSignalEplus_phi_btag = new TH1F("h_sig_eplus_phi_btag", "Signal e^{+} Azimuth",
				    40, -3.2, 3.2);
    fHistSignalEplus_phi_btag->SetXTitle("#phi (rad)");
    fHistSignalEplus_phi_btag->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // e-
    gDirectory->mkdir("e-");
    gDirectory->cd("e-");

    fHistSignalEminus_pt_btag = new TH1F("h_sig_eminus_pt_btag", "Signal e^{-} Transverse Momentum",
				   40, 0, 200);
    fHistSignalEminus_pt_btag->SetXTitle("P_{t} (GeV)");
    fHistSignalEminus_pt_btag->SetYTitle("Number of Entries");
    fHistSignalEminus_eta_btag = new TH1F("h_sig_eminus_eta_btag", "Signal e^{-} Pseudo-Rapidity",
				     40, -2.5, 2.5);
    fHistSignalEminus_eta_btag->SetXTitle("#eta");
    fHistSignalEminus_eta_btag->SetYTitle("Number of Entries");
    fHistSignalEminus_phi_btag = new TH1F("h_sig_eminus_phi_btag", "Signal e^{-} Azimuth",
				    40, -3.2, 3.2);
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
				  40, 0, 200);
    fHistSignalMuon_pt_btag->SetXTitle("P_{t} (GeV)");
    fHistSignalMuon_pt_btag->SetYTitle("Number of Entries");
    fHistSignalMuon_eta_btag = new TH1F("h_sig_muon_eta_btag", "Signal #mu Pseudo-Rapidity",
				   40, -2.5, 2.5);
    fHistSignalMuon_eta_btag->SetXTitle("#eta");
    fHistSignalMuon_eta_btag->SetYTitle("Number of Entries");
    fHistSignalMuon_phi_btag = new TH1F("h_sig_muon_phi_btag", "Signal #mu Azimuth",
				   40, -3.2, 3.2);
    fHistSignalMuon_phi_btag->SetXTitle("#phi (rad)");
    fHistSignalMuon_phi_btag->SetYTitle("Number of Entries");

    fHistSignalMuon_EtCone20_btag = new TH1F("h_sig_muon_EtCone20_btag", "EtCone20",
					     15, 0, 30);
    fHistSignalMuon_EtCone20_btag->SetXTitle("EtCone20 [GeV]");
    fHistSignalMuon_EtCone20_btag->SetYTitle("Number of Entries");

    fHistSignalMuon_PtCone20_btag = new TH1F("h_sig_muon_PtCone20_btag", "PtCone20",
					     30, 0, 300);
    fHistSignalMuon_PtCone20_btag->SetXTitle("PtCone20 [GeV]");
    fHistSignalMuon_PtCone20_btag->SetYTitle("Number of Entries");

    fHistSignalMuon_EtCone30_btag = new TH1F("h_sig_muon_EtCone30_btag", "EtCone30",
					     15, 0, 30);
    
    fHistSignalMuon_EtCone30_btag->SetXTitle("EtCone30 [GeV]");
    fHistSignalMuon_EtCone30_btag->SetYTitle("Number of Entries");

    fHistSignalMuon_PtCone30_btag = new TH1F("h_sig_muon_PtCone30_btag", "PtCone30",
					     30, 0, 300);
    fHistSignalMuon_PtCone30_btag->SetXTitle("PtCone30 [GeV]");
    fHistSignalMuon_PtCone30_btag->SetYTitle("Number of Entries");

    fHistSignalMuon_EtRatio_btag = new TH1F("h_sig_muon_EtRatio_btag", "EtCone30/MuonPt",
				     100, 0, 1);
    fHistSignalMuon_EtRatio_btag->SetXTitle("EtRatio [GeV]");
    fHistSignalMuon_EtRatio_btag->SetYTitle("Number of Entries");

    fHistSignalMuon_PtRatio_btag = new TH1F("h_sig_muon_PtRatio_btag", "PtCone30/MuonPt",
				     100, 0, 1);
    fHistSignalMuon_PtRatio_btag->SetXTitle("PtRatio [GeV]");
    fHistSignalMuon_PtRatio_btag->SetYTitle("Number of Entries");

    
    gDirectory->cd("..");
    
    // mu+
    gDirectory->mkdir("mu+");
    gDirectory->cd("mu+");

    fHistSignalMuplus_pt_btag = new TH1F("h_sig_muplus_pt_btag", "Signal #mu^{+} Transverse Momentum",
				    40, 0, 200);
    fHistSignalMuplus_pt_btag->SetXTitle("P_{t} (GeV)");
    fHistSignalMuplus_pt_btag->SetYTitle("Number of Entries");
    fHistSignalMuplus_eta_btag = new TH1F("h_sig_muplus_eta_btag", "Signal #mu^{+} Pseudo-Rapidity",
				     40, -2.5, 2.5);
    fHistSignalMuplus_eta_btag->SetXTitle("#eta");
    fHistSignalMuplus_eta_btag->SetYTitle("Number of Entries");
    fHistSignalMuplus_phi_btag = new TH1F("h_sig_muplus_phi_btag", "Signal #mu^{+} Azimuth",
				     40, -3.2, 3.2);
    fHistSignalMuplus_phi_btag->SetXTitle("#phi (rad)");
    fHistSignalMuplus_phi_btag->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    // mu-
    gDirectory->mkdir("mu-");
    gDirectory->cd("mu-");

    fHistSignalMuminus_pt_btag = new TH1F("h_sig_muminus_pt_btag", "Signal #mu^{-} Transverse Momentum",
				   40, 0, 200);
    fHistSignalMuminus_pt_btag->SetXTitle("P_{t} (GeV)");
    fHistSignalMuminus_pt_btag->SetYTitle("Number of Entries");
    fHistSignalMuminus_eta_btag = new TH1F("h_sig_muminus_eta_btag", "Signal #mu^{-} Pseudo-Rapidity",
				     40, -2.5, 2.5);
    fHistSignalMuminus_eta_btag->SetXTitle("#eta");
    fHistSignalMuminus_eta_btag->SetYTitle("Number of Entries");
    fHistSignalMuminus_phi_btag = new TH1F("h_sig_muminus_phi_btag", "Signal #mu^{-} Azimuth",
				    40, -3.2, 3.2);
    fHistSignalMuminus_phi_btag->SetXTitle("#phi (rad)");
    fHistSignalMuminus_phi_btag->SetYTitle("Number of Entries");

    
    gDirectory->cd("../../../..");
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::BookJetHistograms() {
    //
    // Book jet histograms
    //
    gDirectory->cd("CommonEventSelection");
    gDirectory->mkdir("SignalJets","Signal jets");
    gDirectory->cd("SignalJets");

    // ======
    // Jets
    // ======
    gDirectory->mkdir("all_jets","All selected signal jets");
    gDirectory->cd("all_jets");

    // All jets
    fHistJet_n = new TH1F("h_jet_n", "No. of all jets",
			  10, -0.5, 9.5);
    fHistJet_n->SetXTitle("N_{jet}");
    fHistJet_n->SetYTitle("Number of Entries");
    fHistJet_Et = new TH1F("h_jet_Et", "Transverse Energy of all jets",
			    40, 0, 200);
    fHistJet_Et->SetXTitle("E_{t} (GeV)");
    fHistJet_Et->SetYTitle("Number of Entries");
    fHistJet_eta = new TH1F("h_jet_eta", "Pseudo-Rapidity of all jets",
			     40, -5, 5);
    fHistJet_eta->SetXTitle("#eta");
    fHistJet_eta->SetYTitle("Number of Entries");
    fHistJet_phi = new TH1F("h_jet_phi", "Azimuth of all jets",
			     40, -3.2, 3.2);
    fHistJet_phi->SetXTitle("#phi (rad)");   
    fHistJet_phi->SetYTitle("Number of Entries");
    fHistJet_pt = new TH1F("h_jet_pt", "Tranverse momentum of all jets",
			     130, 20, 400);
    fHistJet_pt->SetXTitle("pt (GeV)");   
    fHistJet_pt->SetYTitle("Number of Entries");

    
    fHistJet_weight = new TH1F("h_jet_weight", "B-tag weights of all jets",
			     40, 0., 1.);
    
    fHistJet_weight->SetXTitle("B-Tag Weight");
    fHistJet_weight->SetYTitle("Number of Entries");

    // Leading jet
    fHistJet1_Et = new TH1F("h_jet1_Et", "Transverse Energy of leading jet",
			    40, 0, 200);
    fHistJet1_Et->SetXTitle("E_{t} (GeV)");
    fHistJet1_Et->SetYTitle("Number of Entries");
    fHistJet1_eta = new TH1F("h_jet1_eta", "Pseudo-Rapidity of leading jet",
			     40, -5, 5);
    fHistJet1_eta->SetXTitle("#eta");
    fHistJet1_eta->SetYTitle("Number of Entries");
    fHistJet1_phi = new TH1F("h_jet1_phi", "Azimuth of leading jet",
			     40, -3.2, 3.2);
    fHistJet1_phi->SetXTitle("#phi (rad)");
    fHistJet1_phi->SetYTitle("Number of Entries");
    fHistJet1_pt = new TH1F("h_jet1_pt", "Tranverse momentum of leading jet",
			     40, 0, 400);
    fHistJet1_pt->SetXTitle("pt (GeV)");   
    fHistJet1_pt->SetYTitle("Number of Entries");

    fHistDeltaJet1PhiEtMissPhi = new TH1F("h_delta_jet1_etmiss", "",
			     40, -3.6, 3.6);
    fHistDeltaJet1PhiEtMissPhi->SetXTitle("#Delta#phi(leading jet, MET )");   
    fHistDeltaJet1PhiEtMissPhi->SetYTitle("Number of Entries");
    
    
    // 2nd Leading jet
    fHistJet2_Et = new TH1F("h_jet2_Et", "Transverse Energy of 2nd leading jet",
			    40, 0, 200);
    fHistJet2_Et->SetXTitle("E_{t} (GeV)");
    fHistJet2_Et->SetYTitle("Number of Entries");
    fHistJet2_eta = new TH1F("h_jet2_eta", "Pseudo-Rapidity of 2nd leading jet",
			     40, -5, 5);
    fHistJet2_eta->SetXTitle("#eta");
    fHistJet2_eta->SetYTitle("Number of Entries");
    fHistJet2_phi = new TH1F("h_jet2_phi", "Azimuth of 2nd leading jet",
			     40, -3.2, 3.2);
    fHistJet2_phi->SetXTitle("#phi (rad)");
    fHistJet2_phi->SetYTitle("Number of Entries");
    fHistJet2_pt = new TH1F("h_jet2_pt", "Tranverse momentum of 2nd leading jet",
			     40, 0, 400);
    fHistJet2_pt->SetXTitle("pt (GeV)");   
    fHistJet2_pt->SetYTitle("Number of Entries");
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
			    40, 0, 200);
    fHistBJet_Et->SetXTitle("E_{t} (GeV)");
    fHistBJet_Et->SetYTitle("Number of Entries");
    fHistBJet_eta = new TH1F("h_bjet_eta", "Pseudo-Rapidity of all b-jets",
			     40, -2.5, 2.5);
    fHistBJet_eta->SetXTitle("#eta");
    fHistBJet_eta->SetYTitle("Number of Entries");
    fHistBJet_phi = new TH1F("h_bjet_phi", "Azimuth of all b-jets",
			     40, -3.2, 3.2);
    fHistBJet_phi->SetXTitle("#phi (rad)");
    fHistBJet_phi->SetYTitle("Number of Entries");
    fHistBJet_weight = new TH1F("h_bjet_weight", "B-tag weights of all b-jets",
			     40, 0., 1.);
    fHistBJet_weight->SetXTitle("B-Tag Weight");
    fHistBJet_weight->SetYTitle("Number of Entries");

    
    // Leading b-jet
    fHistBJet1_Et = new TH1F("h_bjet1_Et", "Transverse Energy of leading b-jet",
			    40, 0, 200);
    fHistBJet1_Et->SetXTitle("E_{t} (GeV)");
    fHistBJet1_Et->SetYTitle("Number of Entries");
    fHistBJet1_eta = new TH1F("h_bjet1_eta", "Pseudo-Rapidity of leading b-jet",
			     40, -2.5, 2.5);
    fHistBJet1_eta->SetXTitle("#eta");
    fHistBJet1_eta->SetYTitle("Number of Entries");
    fHistBJet1_phi = new TH1F("h_bjet1_phi", "Azimuth of leading b-jet",
			     40, -3.2, 3.2);
    fHistBJet1_phi->SetXTitle("#phi (rad)");
    fHistBJet1_phi->SetYTitle("Number of Entries");
    fHistBJet1_weight = new TH1F("h_bjet1_weight", "B-tag weight of leading b-jets",
			     40,  0., 1.);
    fHistBJet1_weight->SetXTitle("B-Tag Weight");
    fHistBJet1_weight->SetYTitle("Number of Entries");

    // 2nd Leading b-jet
    fHistBJet2_Et = new TH1F("h_bjet2_Et", "Transverse Energy of 2nd leading b-jet",
			    40, 0, 200);
    fHistBJet2_Et->SetXTitle("E_{t} (GeV)");
    fHistBJet2_Et->SetYTitle("Number of Entries");
    fHistBJet2_eta = new TH1F("h_bjet2_eta", "Pseudo-Rapidity of 2nd leading b-jet",
			     40, -2.5, 2.5);
    fHistBJet2_eta->SetXTitle("#eta");
    fHistBJet2_eta->SetYTitle("Number of Entries");
    fHistBJet2_phi = new TH1F("h_bjet2_phi", "Azimuth of 2nd leading b-jet",
			     40, -3.2, 3.2);
    fHistBJet2_phi->SetXTitle("#phi (rad)");
    fHistBJet2_phi->SetYTitle("Number of Entries");
    fHistBJet2_weight = new TH1F("h_bjet2_weight", "B-tag weight of 2nd leading b-jet",
			     40,  0., 1.);
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
			    40, 0, 200);
    fHistNonBJet_Et->SetXTitle("E_{t} (GeV)");
    fHistNonBJet_Et->SetYTitle("Number of Entries");
    fHistNonBJet_eta = new TH1F("h_nonbjet_eta", "Pseudo-Rapidity of all non-b-jets",
			     40, -5, 5);
    fHistNonBJet_eta->SetXTitle("#eta");
    fHistNonBJet_eta->SetYTitle("Number of Entries");
    fHistNonBJet_phi = new TH1F("h_nonbjet_phi", "Azimuth of all non-b-jets",
			     40, -3.2, 3.2);
    fHistNonBJet_phi->SetXTitle("#phi (rad)");
    fHistNonBJet_phi->SetYTitle("Number of Entries");

    // Leading non-b-jet
    fHistNonBJet1_Et = new TH1F("h_nonbjet1_Et", "Transverse Energy of leading non-b-jet",
			    40, 0, 200);
    fHistNonBJet1_Et->SetXTitle("E_{t} (GeV)");
    fHistNonBJet1_Et->SetYTitle("Number of Entries");
    fHistNonBJet1_eta = new TH1F("h_nonbjet1_eta", "Pseudo-Rapidity of leading non-b-jet",
			     40, -5, 5);
    fHistNonBJet1_eta->SetXTitle("#eta");
    fHistNonBJet1_eta->SetYTitle("Number of Entries");
    fHistNonBJet1_phi = new TH1F("h_nonbjet1_phi", "Azimuth of leading non-b-jet",
			     40, -3.2, 3.2);
    fHistNonBJet1_phi->SetXTitle("#phi (rad)");
    fHistNonBJet1_phi->SetYTitle("Number of Entries");

    // 2nd Leading non-b-jet
    fHistNonBJet2_Et = new TH1F("h_nonbjet2_Et", "Transverse Energy of 2nd leading non-b-jet",
			    40, 0, 200);
    fHistNonBJet2_Et->SetXTitle("E_{t} (GeV)");
    fHistNonBJet2_Et->SetYTitle("Number of Entries");
    fHistNonBJet2_eta = new TH1F("h_nonbjet2_eta", "Pseudo-Rapidity of 2nd leading non-b-jet",
			     40, -5, 5);
    fHistNonBJet2_eta->SetXTitle("#eta");
    fHistNonBJet2_eta->SetYTitle("Number of Entries");
    fHistNonBJet2_phi = new TH1F("h_nonbjet2_phi", "Azimuth of 2nd leading non-b-jet",
			     40, -3.2, 3.2);
    fHistNonBJet2_phi->SetXTitle("#phi (rad)");
    fHistNonBJet2_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // ============
    // Forward jets
    // ============
    gDirectory->mkdir("Whad","Signal whad jet");
    gDirectory->cd("Whad");

    // All whad jets
    fHistWhadJet_n = new TH1F("h_whadjet_n", "No. of all whad jets",
			     10, 0, 10);
    fHistWhadJet_n->SetXTitle("N_{whad-jet}");
    fHistWhadJet_n->SetYTitle("Number of Entries");
    fHistWhadJet_Et = new TH1F("h_whadjet_Et", "Transverse Energy of all whad jets",
			      40, 0, 200);
    fHistWhadJet_Et->SetXTitle("E_{t} (GeV)");
    fHistWhadJet_Et->SetYTitle("Number of Entries");
    fHistWhadJet_eta = new TH1F("h_whadjet_eta", "Pseudo-Rapidity of all whad jets",
			       40, -5, 5);
    fHistWhadJet_eta->SetXTitle("#eta");
    fHistWhadJet_eta->SetYTitle("Number of Entries");
    fHistWhadJet_phi = new TH1F("h_whadjet_phi", "Azimuth of all whad jets",
			       40, -3.2, 3.2);
    fHistWhadJet_phi->SetXTitle("#phi (rad)");
    fHistWhadJet_phi->SetYTitle("Number of Entries");

    // Leading  whad jet
    fHistWhadJet1_Et = new TH1F("h_whadjet1_Et", "Transverse Energy of leading whad jet",
			       40, 0, 200);
    fHistWhadJet1_Et->SetXTitle("E_{t} (GeV)");
    fHistWhadJet1_Et->SetYTitle("Number of Entries");
    fHistWhadJet1_eta = new TH1F("h_whadjet1_eta", "Pseudo-Rapidity of leading whad jet",
				40, -5, 5);
    fHistWhadJet1_eta->SetXTitle("#eta");
    fHistWhadJet1_eta->SetYTitle("Number of Entries");
    fHistWhadJet1_phi = new TH1F("h_whadjet1_phi", "Azimuth of leading whad jet",
				40, -3.2, 3.2);
    fHistWhadJet1_phi->SetXTitle("#phi (rad)");
    fHistWhadJet1_phi->SetYTitle("Number of Entries");

    // 2nd Leading whad jet
    fHistWhadJet2_Et = new TH1F("h_whadjet2_Et", "Transverse Energy of 2nd leading whad jet",
			       40, 0, 200);
    fHistWhadJet2_Et->SetXTitle("E_{t} (GeV)");
    fHistWhadJet2_Et->SetYTitle("Number of Entries");
    fHistWhadJet2_eta = new TH1F("h_whadjet2_eta", "Pseudo-Rapidity of 2nd leading whad jet",
				40, -5, 5);
    fHistWhadJet2_eta->SetXTitle("#eta");
    fHistWhadJet2_eta->SetYTitle("Number of Entries");
    fHistWhadJet2_phi = new TH1F("h_whadjet2_phi", "Azimuth of 2nd leading whad jet",
				40, -3.2, 3.2);
    fHistWhadJet2_phi->SetXTitle("#phi (rad)");
    fHistWhadJet2_phi->SetYTitle("Number of Entries");

    gDirectory->cd("../../..");
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::BookJetHistograms_BTag() {
    
    
    gDirectory->cd("CommonEventSelection_BTag");

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
			     40, -5, 5);
    fHistJet_eta_btag->SetXTitle("#eta");
    fHistJet_eta_btag->SetYTitle("Number of Entries");
    fHistJet_phi_btag = new TH1F("h_jet_phi_btag", "Azimuth of all jets",
			     40, -3.2, 3.2);
    fHistJet_phi_btag->SetXTitle("#phi (rad)");   
    fHistJet_phi_btag->SetYTitle("Number of Entries");
    fHistJet_pt_btag = new TH1F("h_jet_pt_btag", "Tranverse momentum of all jets",
			     40, 0, 200);
    fHistJet_pt_btag->SetXTitle("pt (GeV)");   
    fHistJet_pt_btag->SetYTitle("Number of Entries");
    
    fHistJet_weight_btag = new TH1F("h_jet_weight_btag", "B-tag weights of all jets",
			     40, 0., 1.);
    
    fHistJet_weight_btag->SetXTitle("B-Tag Weight");
    fHistJet_weight_btag->SetYTitle("Number of Entries");

    // Leading jet
    fHistJet1_Et_btag = new TH1F("h_jet1_Et_btag", "Transverse Energy of leading jet",
			    40, 0, 200);
    fHistJet1_Et_btag->SetXTitle("E_{t} (GeV)");
    fHistJet1_Et_btag->SetYTitle("Number of Entries");
    fHistJet1_eta_btag = new TH1F("h_jet1_eta_btag", "Pseudo-Rapidity of leading jet",
			     40, -5, 5);
    fHistJet1_eta_btag->SetXTitle("#eta");
    fHistJet1_eta_btag->SetYTitle("Number of Entries");
    fHistJet1_phi_btag = new TH1F("h_jet1_phi_btag", "Azimuth of leading jet",
			     40, -3.2, 3.2);
    fHistJet1_phi_btag->SetXTitle("#phi (rad)");
    fHistJet1_phi_btag->SetYTitle("Number of Entries");
    fHistJet1_pt_btag = new TH1F("h_jet1_pt_btag", "Tranverse momentum of leading jet",
			     40, 0, 200);
    fHistJet1_pt_btag->SetXTitle("pt (GeV)");   
    fHistJet1_pt_btag->SetYTitle("Number of Entries");

    fHistDeltaJet1PhiEtMissPhi_btag = new TH1F("h_delta_jet1_etmiss_btag", "",
			     40, -3.6, 3.6);
    fHistDeltaJet1PhiEtMissPhi_btag->SetXTitle("#Delta#phi(leading jet, MET )");   
    fHistDeltaJet1PhiEtMissPhi_btag->SetYTitle("Number of Entries");
    
    
    // 2nd Leading jet
    fHistJet2_Et_btag = new TH1F("h_jet2_Et_btag", "Transverse Energy of 2nd leading jet",
			    40, 0, 200);
    fHistJet2_Et_btag->SetXTitle("E_{t} (GeV)");
    fHistJet2_Et_btag->SetYTitle("Number of Entries");
    fHistJet2_eta_btag = new TH1F("h_jet2_eta_btag", "Pseudo-Rapidity of 2nd leading jet",
			     40, -5, 5);
    fHistJet2_eta_btag->SetXTitle("#eta");
    fHistJet2_eta_btag->SetYTitle("Number of Entries");
    fHistJet2_phi_btag = new TH1F("h_jet2_phi_btag", "Azimuth of 2nd leading jet",
			     40, -3.2, 3.2);
    fHistJet2_phi_btag->SetXTitle("#phi (rad)");
    fHistJet2_phi_btag->SetYTitle("Number of Entries");
    fHistJet2_pt_btag = new TH1F("h_jet2_pt_btag", "Tranverse momentum of 2nd leading jet",
			     40, 0, 400);
    fHistJet2_pt_btag->SetXTitle("pt (GeV)");   
    fHistJet2_pt_btag->SetYTitle("Number of Entries");
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
			     40, 0., 1.);
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
			     40,  0., 1.);
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
			     40,  0., 1.);
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
			     40, -5, 5);
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
			     40, -5, 5);
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
			     40, -5, 5);
    fHistNonBJet2_eta_btag->SetXTitle("#eta");
    fHistNonBJet2_eta_btag->SetYTitle("Number of Entries");
    fHistNonBJet2_phi_btag = new TH1F("h_nonbjet2_phi_btag", "Azimuth of 2nd leading non-b-jet",
			     40, -3.2, 3.2);
    fHistNonBJet2_phi_btag->SetXTitle("#phi (rad)");
    fHistNonBJet2_phi_btag->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // ============
    // Forward jets
    // ============
    gDirectory->mkdir("Whad","Signal Whad jet");
    gDirectory->cd("Whad");

    // All Whad jets
    fHistWhadJet_n_btag = new TH1F("h_whadjet_n_btag", "No. of all Whad jets",
			     10, 0, 10);
    fHistWhadJet_n_btag->SetXTitle("N_{whad-jet}");
    fHistWhadJet_n_btag->SetYTitle("Number of Entries");
    fHistWhadJet_Et_btag = new TH1F("h_whadjet_Et_btag", "Transverse Energy of all Whad jets",
			      40, 0, 200);
    fHistWhadJet_Et_btag->SetXTitle("E_{t} (GeV)");
    fHistWhadJet_Et_btag->SetYTitle("Number of Entries");
    fHistWhadJet_eta_btag = new TH1F("h_whadjet_eta_btag", "Pseudo-Rapidity of all Whad jets",
			       40, -5, 5);
    fHistWhadJet_eta_btag->SetXTitle("#eta");
    fHistWhadJet_eta_btag->SetYTitle("Number of Entries");
    fHistWhadJet_phi_btag = new TH1F("h_whadjet_phi_btag", "Azimuth of all Whad jets",
			       40, -3.2, 3.2);
    fHistWhadJet_phi_btag->SetXTitle("#phi (rad)");
    fHistWhadJet_phi_btag->SetYTitle("Number of Entries");

    // Leading  Whad jet
    fHistWhadJet1_Et_btag = new TH1F("h_whadjet1_Et_btag", "Transverse Energy of leading Whad jet",
			       40, 0, 200);
    fHistWhadJet1_Et_btag->SetXTitle("E_{t} (GeV)");
    fHistWhadJet1_Et_btag->SetYTitle("Number of Entries");
    fHistWhadJet1_eta_btag = new TH1F("h_whadjet1_eta_btag", "Pseudo-Rapidity of leading Whad jet",
				40, -5, 5);
    fHistWhadJet1_eta_btag->SetXTitle("#eta");
    fHistWhadJet1_eta_btag->SetYTitle("Number of Entries");
    fHistWhadJet1_phi_btag = new TH1F("h_whadjet1_phi_btag", "Azimuth of leading Whad jet",
				40, -3.2, 3.2);
    fHistWhadJet1_phi_btag->SetXTitle("#phi (rad)");
    fHistWhadJet1_phi_btag->SetYTitle("Number of Entries");

    // 2nd Leading Whad jet
    fHistWhadJet2_Et_btag = new TH1F("h_whadjet2_Et_btag", "Transverse Energy of 2nd leading Whad jet",
			       40, 0, 200);
    fHistWhadJet2_Et_btag->SetXTitle("E_{t} (GeV)");
    fHistWhadJet2_Et_btag->SetYTitle("Number of Entries");
    fHistWhadJet2_eta_btag = new TH1F("h_whadjet2_eta_btag", "Pseudo-Rapidity of 2nd leading Whad jet",
				40, -5, 5);
    fHistWhadJet2_eta_btag->SetXTitle("#eta");
    fHistWhadJet2_eta_btag->SetYTitle("Number of Entries");
    fHistWhadJet2_phi_btag = new TH1F("h_whadjet2_phi_btag", "Azimuth of 2nd leading Whad jet",
				40, -3.2, 3.2);
    fHistWhadJet2_phi_btag->SetXTitle("#phi (rad)");
    fHistWhadJet2_phi_btag->SetYTitle("Number of Entries");

    gDirectory->cd("../../..");
    
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::BookKinFitHistograms() {
    // 
    // Book histograms for reconstructed W-boson, top-quark and neutrino
    // in KinFitter mode and call booking of reco. lepton and jet histograms
    //
    gDirectory->mkdir("KinFitterMode","KinFitter mode of sgtop t-channel finder");
    gDirectory->cd("KinFitterMode");
 
   
    gDirectory->mkdir("RecoObjects_WhadVetoCut", "Reconstructed objects after WhadVeto-cut");
    gDirectory->cd("RecoObjects_WhadVetoCut");    
    
    BookKinFitLeptonHistograms();
    BookKinFitJetHistograms();

    gDirectory->cd("..");    

    // =====================================================
    // Reconstructed objects in events surviving P(Chi2)-cut
    // =====================================================
    gDirectory->mkdir("RecoObjects_PChi2_and_otherCuts", "Reconstructed objects after P(Chi2)-cut and other final cuts");
    gDirectory->cd("RecoObjects_PChi2_and_otherCuts");

    
    // =================
    // W reconstruction
    // =================
    gDirectory->mkdir("W-reco", "W reconstruction");
    gDirectory->cd("W-reco");
    
    gDirectory->mkdir("Both", "W+/W- reconstruction");
    gDirectory->cd("Both");
    
    // All leptons
    fHistWreco_Mt = new TH1F("h_Wreco_Mt", "W Invariant Transverse Mass (all leptons)",
			      200, 0, 130);
    fHistWreco_Mt->SetXTitle("M_{t,W} (GeV)");
    fHistWreco_Mt->SetYTitle("Number of Entries");
    fHistWreco_M = new TH1F("h_Wreco_M", "W Invariant Mass (all leptons)",
			      200, 65, 95);
    fHistWreco_M->SetXTitle("M_{W} (GeV)");
    fHistWreco_M->SetYTitle("Number of Entries");
    fHistWreco_phi = new TH1F("h_Wreco_phi", "W Azimuth Angle (all leptons)",
				128, -3.2, 3.2);
    fHistWreco_phi->SetXTitle("#phi (rad)");
    fHistWreco_phi->SetYTitle("Number of Entries");
    fHistWreco_pt = new TH1F("h_Wreco_pt", "W Transverse Momentum (all leptons)",
			       200, 0, 200);
    fHistWreco_pt->SetXTitle("P_{t} (GeV)");
    fHistWreco_pt->SetYTitle("Number of Entries");
    fHistWreco_eta = new TH1F("h_Wreco_eta", "W Pseudo-Rapidity (all leptons)",
				     40, -5., 5.);
    fHistWreco_eta->SetXTitle("#eta");
    fHistWreco_eta->SetYTitle("Number of Entries");
    fHistWreco_rap = new TH1F("h_Wreco_rap", "W Rapidity (all leptons)",
				     40, -5., 5.);
    fHistWreco_rap->SetXTitle("y (rapidity)");
    fHistWreco_rap->SetYTitle("Number of Entries");

    // Electron channel
    fHistWreco_Mt_e = new TH1F("h_Wreco_Mt_e", "W Invariant Transverse Mass (e channel)",
			      200, 0, 130);
    fHistWreco_Mt_e->SetXTitle("M_{t,W} (GeV)");
    fHistWreco_Mt_e->SetYTitle("Number of Entries");
    fHistWreco_M_e = new TH1F("h_Wreco_M_e", "W Invariant Mass (e channel)",
			      200, 65, 95);
    fHistWreco_M_e->SetXTitle("M_{W} (GeV)");
    fHistWreco_M_e->SetYTitle("Number of Entries");
    fHistWreco_phi_e = new TH1F("h_Wreco_phi_e", "W Azimuth Angle (e channel)",
				128, -3.2, 3.2);
    fHistWreco_phi_e->SetXTitle("#phi (rad)");
    fHistWreco_phi_e->SetYTitle("Number of Entries");
    fHistWreco_pt_e = new TH1F("h_Wreco_pt_e", "W Transverse Momentum (e channel)",
			       200, 0, 200);
    fHistWreco_pt_e->SetXTitle("P_{t} (GeV)");
    fHistWreco_pt_e->SetYTitle("Number of Entries");
    fHistWreco_eta_e = new TH1F("h_Wreco_eta_e", "W Pseudo-Rapidity (e channel)",
				     40, -5., 5.);
    fHistWreco_eta_e->SetXTitle("#eta");
    fHistWreco_eta_e->SetYTitle("Number of Entries");
    fHistWreco_rap_e = new TH1F("h_Wreco_rap_e", "W Rapidity (e channel)",
				     40, -5.0, 5.0);
    fHistWreco_rap_e->SetXTitle("y (rapidity)");
    fHistWreco_rap_e->SetYTitle("Number of Entries");
    
    // Muon channel
    fHistWreco_Mt_mu = new TH1F("h_Wreco_Mt_mu", "W Invariant Transverse Mass (#mu channel)",
			       200, 0, 130);
    fHistWreco_Mt_mu->SetXTitle("M_{t,W} (GeV)");
    fHistWreco_Mt_mu->SetYTitle("Number of Entries");
    fHistWreco_M_mu = new TH1F("h_Wreco_M_mu", "W Invariant Mass (#mu channel)",
			       200, 65, 95);
    fHistWreco_M_mu->SetXTitle("M_{W} (GeV)");
    fHistWreco_M_mu->SetYTitle("Number of Entries");
    fHistWreco_phi_mu = new TH1F("h_Wreco_phi_mu", "W Azimuth Angle (#mu channel)",
				 128, -3.2, 3.2);
    fHistWreco_phi_mu->SetXTitle("#phi (rad)");
    fHistWreco_phi_mu->SetYTitle("Number of Entries");
    fHistWreco_pt_mu = new TH1F("h_Wreco_pt_mu", "W Transverse Momentum (#mu channel)",
				200, 0, 200);
    fHistWreco_pt_mu->SetXTitle("P_{t} (GeV)");
    fHistWreco_pt_mu->SetYTitle("Number of Entries");
    fHistWreco_eta_mu = new TH1F("h_Wreco_eta_mu", "W Pseudo-Rapidity (#mu channel)",
				     40, -5.0, 5.0);
    fHistWreco_eta_mu->SetXTitle("#eta");
    fHistWreco_eta_mu->SetYTitle("Number of Entries");
    fHistWreco_rap_mu = new TH1F("h_Wreco_rap_mu", "W Rapidity (#mu channel)",
				     40, -5.0, 5.0);
    fHistWreco_rap_mu->SetXTitle("y (rapidity)");
    fHistWreco_rap_mu->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    gDirectory->mkdir("W-", "W- reconstruction");
    gDirectory->cd("W-");
    
    // All negative sign leptons
    fHistWreco_Mt_minus = new TH1F("h_Wreco_Mt_minus",
				   "W^{-} Invariant Transverse Mass (all negative sign leptons)",
				   200, 0, 130);
    fHistWreco_Mt_minus->SetXTitle("M_{t,W} (GeV)");
    fHistWreco_Mt_minus->SetYTitle("Number of Entries");
    fHistWreco_M_minus = new TH1F("h_Wreco_M_minus",
				   "W^{-} Invariant Mass (all negative sign leptons)",
				   200, 65, 95);
    fHistWreco_M_minus->SetXTitle("M_{W} (GeV)");
    fHistWreco_M_minus->SetYTitle("Number of Entries");
    fHistWreco_phi_minus = new TH1F("h_Wreco_phi_minus",
				    "W^{-} Azimuth Angle (all negative sign leptons)",
				    128, -3.2, 3.2);
    fHistWreco_phi_minus->SetXTitle("#phi (rad)");
    fHistWreco_phi_minus->SetYTitle("Number of Entries");
    fHistWreco_pt_minus = new TH1F("h_Wreco_pt_minus",
				   "W^{-} Transverse Momentum (all negative sign leptons)",
				   200, 0, 200);
    fHistWreco_pt_minus->SetXTitle("P_{t} (GeV)");
    fHistWreco_pt_minus->SetYTitle("Number of Entries");
    fHistWreco_eta_minus = new TH1F("h_Wreco_eta_minus",
				    "W^{-} Pseudo-Rapidity (all negative sign leptons)",
				    40, -5.0, 5.0);
    fHistWreco_eta_minus->SetXTitle("#eta");
    fHistWreco_eta_minus->SetYTitle("Number of Entries");
    fHistWreco_rap_minus = new TH1F("h_Wreco_rap_minus",
				    "W^{-} Rapidity (all negative sign leptons)",
				    40, -5.0, 5.0);
    fHistWreco_rap_minus->SetXTitle("y (rapidity)");
    fHistWreco_rap_minus->SetYTitle("Number of Entries");
        
    // Electron e- channel
    fHistWreco_Mt_e_minus = new TH1F("h_Wreco_Mt_e_minus",
				     "W^{-} Invariant Transverse Mass (e^{-} channel)",
				     200, 0, 130);
    fHistWreco_Mt_e_minus->SetXTitle("M_{t,W} (GeV)");
    fHistWreco_Mt_e_minus->SetYTitle("Number of Entries");
    fHistWreco_M_e_minus = new TH1F("h_Wreco_M_e_minus",
				     "W^{-} Invariant Mass (e^{-} channel)",
				     200, 65, 95);
    fHistWreco_M_e_minus->SetXTitle("M_{W} (GeV)");
    fHistWreco_M_e_minus->SetYTitle("Number of Entries");
    fHistWreco_phi_e_minus = new TH1F("h_Wreco_phi_e_minus",
				      "W^{-} Azimuth Angle (e^{-} channel)",
				      128, -3.2, 3.2);
    fHistWreco_phi_e_minus->SetXTitle("#phi (rad)");
    fHistWreco_phi_e_minus->SetYTitle("Number of Entries");
    fHistWreco_pt_e_minus = new TH1F("h_Wreco_pt_e_minus",
				     "W^{-} Transverse Momentum (e^{-} channel)",
				     200, 0, 200);
    fHistWreco_pt_e_minus->SetXTitle("P_{t} (GeV)");
    fHistWreco_pt_e_minus->SetYTitle("Number of Entries");
    fHistWreco_eta_e_minus = new TH1F("h_Wreco_eta_e_minus",
				      "W^{-} Pseudo-Rapidity (e^{-} channel)",
				      40, -5.0, 5.0);
    fHistWreco_eta_e_minus->SetXTitle("#eta");
    fHistWreco_eta_e_minus->SetYTitle("Number of Entries");
    fHistWreco_rap_e_minus = new TH1F("h_Wreco_rap_e_minus",
				      "W^{-} Rapidity (e^{-} channel)",
				      40, -5.0, 5.0);
    fHistWreco_rap_e_minus->SetXTitle("y (rapidity)");
    fHistWreco_rap_e_minus->SetYTitle("Number of Entries");

    // Muon mu- channel
    fHistWreco_Mt_mu_minus = new TH1F("h_Wreco_Mt_mu_minus",
				      "W^{-} Invariant Transverse Mass (#mu^{-} channel)",
				      200, 0, 130);
    fHistWreco_Mt_mu_minus->SetXTitle("M_{t,W} (GeV)");
    fHistWreco_Mt_mu_minus->SetYTitle("Number of Entries");
    fHistWreco_M_mu_minus = new TH1F("h_Wreco_M_mu_minus",
				      "W^{-} Invariant Mass (#mu^{-} channel)",
				      200, 65, 95);
    fHistWreco_M_mu_minus->SetXTitle("M_{W} (GeV)");
    fHistWreco_M_mu_minus->SetYTitle("Number of Entries");
    fHistWreco_phi_mu_minus = new TH1F("h_Wreco_phi_mu_minus",
				       "W^{-} Azimuth Angle (#mu^{-} channel)",
				       128, -3.2, 3.2);
    fHistWreco_phi_mu_minus->SetXTitle("#phi (rad)");
    fHistWreco_phi_mu_minus->SetYTitle("Number of Entries");
    fHistWreco_pt_mu_minus = new TH1F("h_Wreco_pt_mu_minus",
				      "W^{-} Transverse Momentum (#mu^{-} channel)",
				      200, 0, 200);
    fHistWreco_pt_mu_minus->SetXTitle("P_{t} (GeV)");
    fHistWreco_pt_mu_minus->SetYTitle("Number of Entries");
    fHistWreco_eta_mu_minus = new TH1F("h_Wreco_eta_mu_minus",
				       "W^{-} Pseudo-Rapidity (#mu^{-} channel)",
				       40, -5.0, 5.0);
    fHistWreco_eta_mu_minus->SetXTitle("#eta");
    fHistWreco_eta_mu_minus->SetYTitle("Number of Entries");
    fHistWreco_rap_mu_minus = new TH1F("h_Wreco_rap_mu_minus",
				       "W^{-} Rapidity (#mu^{-} channel)",
				       40, -5.0, 5.0);
    fHistWreco_rap_mu_minus->SetXTitle("y (rapidity)");
    fHistWreco_rap_mu_minus->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    gDirectory->mkdir("W+", "W+ reconstruction");
    gDirectory->cd("W+");
    
    // All positive sign leptons
    fHistWreco_Mt_plus = new TH1F("h_Wreco_Mt_plus",
				  "W^{+} Invariant Transverse Mass (all positive sign leptons)",
				  200, 0, 130);
    fHistWreco_Mt_plus->SetXTitle("M_{t,W} (GeV)");
    fHistWreco_Mt_plus->SetYTitle("Number of Entries");
    fHistWreco_M_plus = new TH1F("h_Wreco_M_plus",
				  "W^{+} Invariant Mass (all positive sign leptons)",
				  200, 65, 95);
    fHistWreco_M_plus->SetXTitle("M_{W} (GeV)");
    fHistWreco_M_plus->SetYTitle("Number of Entries");
    fHistWreco_phi_plus = new TH1F("h_Wreco_phi_plus",
				   "W^{+} Azimuth Angle (all positive sign leptons)",
				   128, -3.2, 3.2);
    fHistWreco_phi_plus->SetXTitle("#phi (rad)");
    fHistWreco_phi_plus->SetYTitle("Number of Entries");
    fHistWreco_pt_plus = new TH1F("h_Wreco_pt_plus",
				  "W^{+} Transverse Momentum (all positive sign leptons)",
				  200, 0, 200);
    fHistWreco_pt_plus->SetXTitle("P_{t} (GeV)");
    fHistWreco_pt_plus->SetYTitle("Number of Entries");
    fHistWreco_eta_plus = new TH1F("h_Wreco_eta_plus",
				   "W^{+} Pseudo-Rapidity (all positive sign leptons)",
				   40, -5.0, 5.0);
    fHistWreco_eta_plus->SetXTitle("#eta");
    fHistWreco_eta_plus->SetYTitle("Number of Entries");
    fHistWreco_rap_plus = new TH1F("h_Wreco_rap_plus",
				   "W^{+} Rapidity (all positive sign leptons)",
				   40, -5.0, 5.0);
    fHistWreco_rap_plus->SetXTitle("y (rapidity)");
    fHistWreco_rap_plus->SetYTitle("Number of Entries");
    
    // Electron e+ channel
    fHistWreco_Mt_e_plus = new TH1F("h_Wreco_Mt_e_plus",
				    "W^{+} Invariant Transverse Mass (e^{+} channel)",
				    200, 0, 130);
    fHistWreco_Mt_e_plus->SetXTitle("M_{t,W} (GeV)");
    fHistWreco_Mt_e_plus->SetYTitle("Number of Entries");
    fHistWreco_M_e_plus = new TH1F("h_Wreco_M_e_plus",
				    "W^{+} Invariant Mass (e^{+} channel)",
				    200, 65, 95);
    fHistWreco_M_e_plus->SetXTitle("M_{W} (GeV)");
    fHistWreco_M_e_plus->SetYTitle("Number of Entries");
    fHistWreco_phi_e_plus = new TH1F("h_Wreco_phi_e_plus",
				     "W^{+} Azimuth Angle (e^{+} channel)",
				     128, -3.2, 3.2);
    fHistWreco_phi_e_plus->SetXTitle("#phi (rad)");
    fHistWreco_phi_e_plus->SetYTitle("Number of Entries");
    fHistWreco_pt_e_plus = new TH1F("h_Wreco_pt_e_plus",
				    "W^{+} Transverse Momentum (e^{+} channel)",
				    200, 0, 200);
    fHistWreco_pt_e_plus->SetXTitle("P_{t} (GeV)");
    fHistWreco_pt_e_plus->SetYTitle("Number of Entries");
    fHistWreco_eta_e_plus = new TH1F("h_Wreco_eta_e_plus",
				     "W^{+} Pseudo-Rapidity (e^{+} channel)",
				     40, -5.0, 5.0);
    fHistWreco_eta_e_plus->SetXTitle("#eta");
    fHistWreco_eta_e_plus->SetYTitle("Number of Entries");
    fHistWreco_rap_e_plus = new TH1F("h_Wreco_rap_e_plus", "W^{+} Rapidity (e^{+} channel)",
				     40, -5.0, 5.0);
    fHistWreco_rap_e_plus->SetXTitle("y (rapidity)");
    fHistWreco_rap_e_plus->SetYTitle("Number of Entries");


    // Muon mu+ channel
    fHistWreco_Mt_mu_plus = new TH1F("h_Wreco_Mt_mu_plus",
				     "W^{+} Invariant Transverse Mass (#mu^{+} channel)",
				     200, 0, 130);
    fHistWreco_Mt_mu_plus->SetXTitle("M_{t,W} (GeV)");
    fHistWreco_Mt_mu_plus->SetYTitle("Number of Entries");
    fHistWreco_M_mu_plus = new TH1F("h_Wreco_M_mu_plus",
				     "W^{+} Invariant Mass (#mu^{+} channel)",
				     200, 65, 95);
    fHistWreco_M_mu_plus->SetXTitle("M_{W} (GeV)");
    fHistWreco_M_mu_plus->SetYTitle("Number of Entries");
    fHistWreco_phi_mu_plus = new TH1F("h_Wreco_phi_mu_plus",
				      "W^{+} Azimuth Angle (#mu^{+} channel)",
				      128, -3.2, 3.2);
    fHistWreco_phi_mu_plus->SetXTitle("#phi (rad)");
    fHistWreco_phi_mu_plus->SetYTitle("Number of Entries");
    fHistWreco_pt_mu_plus = new TH1F("h_Wreco_pt_mu_plus",
				     "W^{+} Transverse Momentum (#mu^{+} channel)",
				     200, 0, 200);
    fHistWreco_pt_mu_plus->SetXTitle("P_{t} (GeV)");
    fHistWreco_pt_mu_plus->SetYTitle("Number of Entries");
    fHistWreco_eta_mu_plus = new TH1F("h_Wreco_eta_mu_plus",
				      "W^{+} Pseudo-Rapidity (#mu^{+} channel)",
				      40, -5.0, 5.0);
    fHistWreco_eta_mu_plus->SetXTitle("#eta");
    fHistWreco_eta_mu_plus->SetYTitle("Number of Entries");
    fHistWreco_rap_mu_plus = new TH1F("h_Wreco_rap_mu_plus",
				      "W^{+} Rapidity (#mu^{+} channel)",
				      40, -5.0, 5.0);
    fHistWreco_rap_mu_plus->SetXTitle("y (rapidity)");
    fHistWreco_rap_mu_plus->SetYTitle("Number of Entries");

    gDirectory->cd("../..");
    
    // ========================
    // Top-quark reconstruction
    // ========================
    gDirectory->mkdir("Top-reco", "Top reconstruction");
    gDirectory->cd("Top-reco");

    gDirectory->mkdir("Both", "t/#bar{t} reconstruction");
    gDirectory->cd("Both");

    // All leptons
    fHistTopReco_Mt = new TH1F("h_treco_Mt", "Top Transverse Invariant Mass (all leptons)",
			       120, 0, 210);
    fHistTopReco_Mt->SetXTitle("M_{t,top} (GeV)");
    fHistTopReco_Mt->SetYTitle("Number of Entries");
    fHistTopReco_M = new TH1F("h_treco_M", "Top Invariant Mass (all leptons)",
			       120, 165, 185);
    fHistTopReco_M->SetXTitle("M_{top} (GeV)");
    fHistTopReco_M->SetYTitle("Number of Entries");
    fHistTopReco_phi = new TH1F("h_treco_phi", "Top Azimuth Angle (all leptons)",
				128, -3.2, 3.2);
    fHistTopReco_phi->SetXTitle("#phi (rad)");
    fHistTopReco_phi->SetYTitle("Number of Entries");
    fHistTopReco_pt = new TH1F("h_treco_pt", "Top Transverse Momentum (all leptons)",
			       200, 0, 200);
    fHistTopReco_pt->SetXTitle("P_{t} (GeV)");
    fHistTopReco_pt->SetYTitle("Number of Entries");
    fHistTopReco_eta = new TH1F("h_treco_eta", "Top Pseudo-Rapidity (all leptons)",
				25, -5.0, 5.0);
    fHistTopReco_eta->SetXTitle("#eta");
    fHistTopReco_eta->SetYTitle("Number of Entries");
    fHistTopReco_rap = new TH1F("h_treco_rap", "Top Rapidity (all leptons)",
				25, -5.0, 5.0);
    fHistTopReco_rap->SetXTitle("y (rapidity)");
    fHistTopReco_rap->SetYTitle("Number of Entries");
    fHistTopReco_DeltaEtaFwdJet = new TH1F("h_treco_delta_eta_fwd_jet", "Difference of Top Pseudo-Rapidity (all leptons) to Eta of forward Jet",
				25, -5.0, 5.0);
    fHistTopReco_DeltaEtaFwdJet->SetXTitle("#Delta #eta (top,fwd jet)");
    fHistTopReco_DeltaEtaFwdJet->SetYTitle("Number of Entries");
    
    fHistTopReco_DeltaPhiFwdJet = new TH1F("h_TopReco_DeltaPhiFwdJet","Difference of Phi in x-y-plane for Jet and Top",30,0,3.15);
    fHistTopReco_DeltaPhiFwdJet->SetXTitle("|Phi_{Jet}-Phi_{Top}|");
    fHistTopReco_DeltaPhiFwdJet->SetYTitle("Number of entries");
    
    fHistTopReco_SumPxPyFwdJet = new TH1F("h_TopReco_SumPxPyFwdJet","Sum Of Momentum in X-Y-Plane for Top and FwdJet",30,0,60);
    fHistTopReco_SumPxPyFwdJet->SetXTitle("|P_{t}^{JetQuark}+P_{t}^{Top}| (GeV)");
    fHistTopReco_SumPxPyFwdJet->SetYTitle("Number of Entries");
    
    fHistTopReco_DeltaP3FwdJet = new TH1F("h_TopReco_DeltaP3FwdJet","Subtract amount |P3_Top|-|P3_FwdJet|",30,-300,300);
    fHistTopReco_DeltaP3FwdJet->SetXTitle("|P_{3}^{Top}|-|P_{3}^{FwdJet}|");
    fHistTopReco_DeltaP3FwdJet->SetYTitle("Number of Entries");
    
    fHistTopReco_DeltaPxFwdJet = new TH1F("h_TopReco_DeltaPxFwdJet","Sum of Px-components: P_{x}^{Top}+P_{x}^{FwdJet}",30,-30,30);
    fHistTopReco_DeltaPxFwdJet->SetXTitle("P_{x}^{Top}+P_{x}^{FwdJet}");
    fHistTopReco_DeltaPxFwdJet->SetYTitle("Number of Entries");
    
    fHistTopReco_DeltaPyFwdJet = new TH1F("h_TopReco_DeltaPyFwdJet","Sum of Py-components: P_{y}^{Top}+P_{y}^{FwdJet}",30,-30,30);
    fHistTopReco_DeltaPyFwdJet->SetXTitle("P_{y}^{Top}+P_{y}^{FwdJet}");
    fHistTopReco_DeltaPyFwdJet->SetYTitle("Number of Entries");
    
    fHistTopReco_Amount_P3 = new TH1F("h_TopReco_Amount_P3","Amount of P3 of reconstructed Single-Top",30,0,300);
    fHistTopReco_Amount_P3->SetXTitle("|P_{3}^{Top}|");
    fHistTopReco_Amount_P3->SetYTitle("Number of Entries");
    
    // Electron channel
    fHistTopReco_Mt_e = new TH1F("h_treco_Mt_e", "Top Transverse Invariant Mass (e channel)",
				 120, 0, 210);
    fHistTopReco_Mt_e->SetXTitle("M_{t,top} (GeV)");
    fHistTopReco_Mt_e->SetYTitle("Number of Entries");
    fHistTopReco_M_e = new TH1F("h_treco_M_e", "Top Invariant Mass (e channel)",
				 120, 165, 185);
    fHistTopReco_M_e->SetXTitle("M_{top} (GeV)");
    fHistTopReco_M_e->SetYTitle("Number of Entries");
    fHistTopReco_phi_e = new TH1F("h_treco_phi_e", "Top Azimuth Angle (e channel)",
				  128, -3.2, 3.2);
    fHistTopReco_phi_e->SetXTitle("#phi (rad)");
    fHistTopReco_phi_e->SetYTitle("Number of Entries");
    fHistTopReco_pt_e = new TH1F("h_treco_pt_e", "Top Transverse Momentum (e channel)",
				 200, 0, 200);
    fHistTopReco_pt_e->SetXTitle("P_{t} (GeV)");
    fHistTopReco_pt_e->SetYTitle("Number of Entries");
    fHistTopReco_eta_e = new TH1F("h_treco_eta_e", "Top Pseudo-Rapidity (e channel)",
				  50, -5.0, 5.0);
    fHistTopReco_eta_e->SetXTitle("#eta");
    fHistTopReco_eta_e->SetYTitle("Number of Entries");
    fHistTopReco_rap_e = new TH1F("h_treco_rap_e", "Top Rapidity (e channel)",
				  50, -5.0, 5.0);
    fHistTopReco_rap_e->SetXTitle("y (rapidity)");
    fHistTopReco_rap_e->SetYTitle("Number of Entries");
    
    // Muon channel
    fHistTopReco_Mt_mu = new TH1F("h_treco_Mt_mu", "Top Transverse Invariant Mass (#mu channel)",
				  120, 0, 210);
    fHistTopReco_Mt_mu->SetXTitle("M_{t,top} (GeV)");
    fHistTopReco_Mt_mu->SetYTitle("Number of Entries");
    fHistTopReco_M_mu = new TH1F("h_treco_M_mu", "Top Invariant Mass (#mu channel)",
				  120, 165, 185);
    fHistTopReco_M_mu->SetXTitle("M_{top} (GeV)");
    fHistTopReco_M_mu->SetYTitle("Number of Entries");
    fHistTopReco_phi_mu = new TH1F("h_treco_phi_mu", "Top Azimuth Angle (#mu channel)",
				   128, -3.2, 3.2);
    fHistTopReco_phi_mu->SetXTitle("#phi (rad)");
    fHistTopReco_phi_mu->SetYTitle("Number of Entries");
    fHistTopReco_pt_mu = new TH1F("h_treco_pt_mu", "Top Transverse Momentum (#mu channel)",
				  200, 0, 200);
    fHistTopReco_pt_mu->SetXTitle("P_{t} (GeV)");
    fHistTopReco_pt_mu->SetYTitle("Number of Entries");
    fHistTopReco_eta_mu = new TH1F("h_treco_eta_mu", "Top Pseudo-Rapidity (#mu channel)",
				   50, -5.0, 5.0);
    fHistTopReco_eta_mu->SetXTitle("#eta");
    fHistTopReco_eta_mu->SetYTitle("Number of Entries");
    fHistTopReco_rap_mu = new TH1F("h_treco_rap_mu", "Top Rapidity (#mu channel)",
				   50, -5.0, 5.0);
    fHistTopReco_rap_mu->SetXTitle("y (rapidity)");
    fHistTopReco_rap_mu->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    gDirectory->mkdir("t", "t-quark reconstruction");
    gDirectory->cd("t");

    // All positive sign leptons
    fHistTopReco_Mt_plus = new TH1F("h_treco_Mt_plus",
				    "t-Quark Transverse Invariant Mass (all positive sign leptons)",
				    120, 0, 210);
    fHistTopReco_Mt_plus->SetXTitle("M_{t,top} (GeV)");
    fHistTopReco_Mt_plus->SetYTitle("Number of Entries");
    fHistTopReco_M_plus = new TH1F("h_treco_M_plus",
				    "t-Quark Invariant Mass (all positive sign leptons)",
				    120, 165, 185);
    fHistTopReco_M_plus->SetXTitle("M_{top} (GeV)");
    fHistTopReco_M_plus->SetYTitle("Number of Entries");
    fHistTopReco_phi_plus = new TH1F("h_treco_phi_plus",
				     "t-Quark Azimuth Angle (all positive sign leptons)",
				     128, -3.2, 3.2);
    fHistTopReco_phi_plus->SetXTitle("#phi (rad)");
    fHistTopReco_phi_plus->SetYTitle("Number of Entries");
    fHistTopReco_pt_plus = new TH1F("h_treco_pt_plus",
				    "t-Quark Transverse Momentum (all positive sign leptons)",
				    200, 0, 200);
    fHistTopReco_pt_plus->SetXTitle("P_{t} (GeV)");
    fHistTopReco_pt_plus->SetYTitle("Number of Entries");
    fHistTopReco_eta_plus = new TH1F("h_treco_eta_plus",
				     "t-Quark Pseudo-Rapidity (all positive sign leptons)",
				     50, -5.0, 5.0);
    fHistTopReco_eta_plus->SetXTitle("#eta");
    fHistTopReco_eta_plus->SetYTitle("Number of Entries");
    fHistTopReco_rap_plus = new TH1F("h_treco_rap_plus",
				     "t-Quark Rapidity (all positive sign leptons)",
				     50, -5.0, 5.0);
    fHistTopReco_rap_plus->SetXTitle("y (rapidity)");
    fHistTopReco_rap_plus->SetYTitle("Number of Entries");

    // Electron e+ channel
    fHistTopReco_Mt_e_plus = new TH1F("h_treco_Mt_e_plus",
				      "t-Quark Transverse Invariant Mass (e^{+} channel)",
				      120, 0, 210);
    fHistTopReco_Mt_e_plus->SetXTitle("M_{t,top} (GeV)");
    fHistTopReco_Mt_e_plus->SetYTitle("Number of Entries");
    fHistTopReco_M_e_plus = new TH1F("h_treco_M_e_plus",
				      "t-Quark Invariant Mass (e^{+} channel)",
				      120, 165, 185);
    fHistTopReco_M_e_plus->SetXTitle("M_{top} (GeV)");
    fHistTopReco_M_e_plus->SetYTitle("Number of Entries");
    fHistTopReco_phi_e_plus = new TH1F("h_treco_phi_e_plus",
				       "t-Quark Azimuth Angle (e^{+} channel)",
				       128, -3.2, 3.2);
    fHistTopReco_phi_e_plus->SetXTitle("#phi (rad)");
    fHistTopReco_phi_e_plus->SetYTitle("Number of Entries");
    fHistTopReco_pt_e_plus = new TH1F("h_treco_pt_e_plus",
				      "t-Quark Transverse Momentum (e^{+} channel)",
				      200, 0, 200);
    fHistTopReco_pt_e_plus->SetXTitle("P_{t} (GeV)");
    fHistTopReco_pt_e_plus->SetYTitle("Number of Entries");
    fHistTopReco_eta_e_plus = new TH1F("h_treco_eta_e_plus",
				       "t-Quark Pseudo-Rapidity (e^{+} channel)",
				       50, -5.0, 5.0);
    fHistTopReco_eta_e_plus->SetXTitle("#eta");
    fHistTopReco_eta_e_plus->SetYTitle("Number of Entries");
    fHistTopReco_rap_e_plus = new TH1F("h_treco_rap_e_plus",
				       "t-Quark Rapidity (e^{+} channel)",
				       50, -5.0, 5.0);
    fHistTopReco_rap_e_plus->SetXTitle("y (rapidity)");
    fHistTopReco_rap_e_plus->SetYTitle("Number of Entries");
    
    // Muon mu+ channel
    fHistTopReco_Mt_mu_plus = new TH1F("h_treco_Mt_mu_plus",
				       "t-Quark Transverse Invariant Mass (#mu^{+} channel)",
				       120, 0, 210);
    fHistTopReco_Mt_mu_plus->SetXTitle("M_{t,top} (GeV)");
    fHistTopReco_Mt_mu_plus->SetYTitle("Number of Entries");
    fHistTopReco_M_mu_plus = new TH1F("h_treco_M_mu_plus",
				       "t-Quark Invariant Mass (#mu^{+} channel)",
				      120, 165, 185);
    fHistTopReco_M_mu_plus->SetXTitle("M_{top} (GeV)");
    fHistTopReco_M_mu_plus->SetYTitle("Number of Entries");
    fHistTopReco_phi_mu_plus = new TH1F("h_treco_phi_mu_plus",
					"t-Quark Azimuth Angle (#mu^{+} channel)",
					128, -3.2, 3.2);
    fHistTopReco_phi_mu_plus->SetXTitle("#phi (rad)");
    fHistTopReco_phi_mu_plus->SetYTitle("Number of Entries");
    fHistTopReco_pt_mu_plus = new TH1F("h_treco_pt_mu_plus",
				       "t-Quark Transverse Momentum (#mu^{+} channel)",
				       200, 0, 200);
    fHistTopReco_pt_mu_plus->SetXTitle("P_{t} (GeV)");
    fHistTopReco_pt_mu_plus->SetYTitle("Number of Entries");
    fHistTopReco_eta_mu_plus = new TH1F("h_treco_eta_mu_plus",
					"t-Quark Pseudo-Rapidity (#mu^{+} channel)",
					50, -5.0, 5.0);
    fHistTopReco_eta_mu_plus->SetXTitle("#eta");
    fHistTopReco_eta_mu_plus->SetYTitle("Number of Entries");
    fHistTopReco_rap_mu_plus = new TH1F("h_treco_rap_mu_plus",
					"Top-Quark Rapidity (#mu^{+} channel)",
					50, -5.0, 5.0);
    fHistTopReco_rap_mu_plus->SetXTitle("y (rapidity)");
    fHistTopReco_rap_mu_plus->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    gDirectory->mkdir("tbar", "#bar{t}-quark reconstruction");
    gDirectory->cd("tbar");
    
    // All negative sign leptons
    fHistTopReco_Mt_minus = new TH1F("h_treco_Mt_minus",
				     "#bar{t}-Quark Transverse Invariant Mass (all negative sign leptons)",
				     120, 0, 210);
    fHistTopReco_Mt_minus->SetXTitle("M_{t,top} (GeV)");
    fHistTopReco_Mt_minus->SetYTitle("Number of Entries");
    fHistTopReco_M_minus = new TH1F("h_treco_M_minus",
				     "#bar{t}-Quark Invariant Mass (all negative sign leptons)",
				     120, 165, 185);
    fHistTopReco_M_minus->SetXTitle("M_{top} (GeV)");
    fHistTopReco_M_minus->SetYTitle("Number of Entries");
    fHistTopReco_phi_minus = new TH1F("h_treco_phi_minus",
				      "#bar{t}-Quark Azimuth Angle (all negative sign leptons)",
				      128, -3.2, 3.2);
    fHistTopReco_phi_minus->SetXTitle("#phi (rad)");
    fHistTopReco_phi_minus->SetYTitle("Number of Entries");
    fHistTopReco_pt_minus = new TH1F("h_treco_pt_minus",
				     "#bar{t}-Quark Transverse Momentum (all negative sign leptons)",
				     200, 0, 200);
    fHistTopReco_pt_minus->SetXTitle("P_{t} (GeV)");
    fHistTopReco_pt_minus->SetYTitle("Number of Entries");
    fHistTopReco_eta_minus = new TH1F("h_treco_eta_minus",
				      "t-Quark Pseudo-Rapidity (all negative sign leptons)",
				      50, -5.0, 5.0);
    fHistTopReco_eta_minus->SetXTitle("#eta");
    fHistTopReco_eta_minus->SetYTitle("Number of Entries");
    fHistTopReco_rap_minus = new TH1F("h_treco_rap_minus",
				      "t-Quark Rapidity (all negative sign leptons)",
				      50, -5.0, 5.0);
    fHistTopReco_rap_minus->SetXTitle("y (rapidity)");
    fHistTopReco_rap_minus->SetYTitle("Number of Entries");

    // Electron e- channel
    fHistTopReco_Mt_e_minus = new TH1F("h_treco_Mt_e_minus",
				       "#bar{t}-Quark Transverse Invariant Mass (e^{-} channel)",
				       120, 0, 210);
    fHistTopReco_Mt_e_minus->SetXTitle("M_{t,top} (GeV)");
    fHistTopReco_Mt_e_minus->SetYTitle("Number of Entries");
    fHistTopReco_M_e_minus = new TH1F("h_treco_M_e_minus",
				       "#bar{t}-Quark Invariant Mass (e^{-} channel)",
				       120, 165, 185);
    fHistTopReco_M_e_minus->SetXTitle("M_{top} (GeV)");
    fHistTopReco_M_e_minus->SetYTitle("Number of Entries");
    fHistTopReco_phi_e_minus = new TH1F("h_treco_phi_e_minus",
					"#bar{t}-Quark Azimuth Angle (e^{-} channel)",
					128, -3.2, 3.2);
    fHistTopReco_phi_e_minus->SetXTitle("#phi (rad)");
    fHistTopReco_phi_e_minus->SetYTitle("Number of Entries");
    fHistTopReco_pt_e_minus = new TH1F("h_treco_pt_e_minus",
				       "#bar{t}-Quark Transverse Momentum (e^{-} channel)",
				       200, 0, 200);
    fHistTopReco_pt_e_minus->SetXTitle("P_{t} (GeV)");
    fHistTopReco_pt_e_minus->SetYTitle("Number of Entries");
    fHistTopReco_eta_e_minus = new TH1F("h_treco_eta_e_minus",
					"#bar{t}-Quark Pseudo-Rapidity (e^{-} channel)",
					50, -5.0, 5.0);
    fHistTopReco_eta_e_minus->SetXTitle("#eta");
    fHistTopReco_eta_e_minus->SetYTitle("Number of Entries");
    fHistTopReco_rap_e_minus = new TH1F("h_treco_rap_e_minus",
					"#bar{t}-Quark Rapidity (e^{-} channel)",
					50, -5.0, 5.0);
    fHistTopReco_rap_e_minus->SetXTitle("y (rapidity)");
    fHistTopReco_rap_e_minus->SetYTitle("Number of Entries");

    // Muon mu- channel
    fHistTopReco_Mt_mu_minus = new TH1F("h_treco_Mt_mu_minus",
					"#bar{t}-Quark Transverse Invariant Mass (#mu^{-} channel)",
					120, 0, 210);
    fHistTopReco_Mt_mu_minus->SetXTitle("M_{t,top} (GeV)");
    fHistTopReco_Mt_mu_minus->SetYTitle("Number of Entries");
    fHistTopReco_M_mu_minus = new TH1F("h_treco_M_mu_minus",
					"#bar{t}-Quark Invariant Mass (#mu^{-} channel)",
					120, 165, 185);
    fHistTopReco_M_mu_minus->SetXTitle("M_{top} (GeV)");
    fHistTopReco_M_mu_minus->SetYTitle("Number of Entries");
    fHistTopReco_phi_mu_minus = new TH1F("h_treco_phi_mu_minus",
					 "#bar{t}-Quark Azimuth Angle (#mu^{-} channel)",
					 128, -3.2, 3.2);
    fHistTopReco_phi_mu_minus->SetXTitle("#phi (rad)");
    fHistTopReco_phi_mu_minus->SetYTitle("Number of Entries");
    fHistTopReco_pt_mu_minus = new TH1F("h_treco_pt_mu_minus",
					"#bar{t}-Quark Transverse Momentum (#mu^{-} channel)",
					200, 0, 200);
    fHistTopReco_pt_mu_minus->SetXTitle("P_{t} (GeV)");
    fHistTopReco_pt_mu_minus->SetYTitle("Number of Entries");
    fHistTopReco_eta_mu_minus = new TH1F("h_treco_eta_mu_minus",
					 "#bar{t}-Quark Pseudo-Rapidity (#mu^{-} channel)",
					 50, -5.0, 5.0);
    fHistTopReco_eta_mu_minus->SetXTitle("#eta");
    fHistTopReco_eta_mu_minus->SetYTitle("Number of Entries");
    fHistTopReco_rap_mu_minus = new TH1F("h_treco_rap_mu_minus",
					 "#bar{t}-Quark Rapidity (#mu^{-} channel)",
					 50, -5.0, 5.0);
    fHistTopReco_rap_mu_minus->SetXTitle("y (rapidity)");
    fHistTopReco_rap_mu_minus->SetYTitle("Number of Entries");
    
    gDirectory->cd("../..");
    
    // ========================
    // Neutrino reconstruction
    // ========================
    gDirectory->mkdir("Neutrino-reco", "Neutrino reconstruction");
    gDirectory->cd("Neutrino-reco");

    gDirectory->mkdir("Both", "#nu/#bar{#nu} reconstruction");
    gDirectory->cd("Both");

    // All leptons
    fHistNuReco_phi = new TH1F("h_nureco_phi", "#nu Azimuth Angle (all leptons)",
			       128, -3.2, 3.2);
    fHistNuReco_phi->SetXTitle("#phi (rad)");
    fHistNuReco_phi->SetYTitle("Number of Entries");
    fHistNuReco_pt = new TH1F("h_nureco_pt", "#nu Transverse Momentum (all leptons)",
			      100, 0, 200);
    fHistNuReco_pt->SetXTitle("P_{t} (GeV)");
    fHistNuReco_pt->SetYTitle("Number of Entries");
    fHistNuReco_eta = new TH1F("h_nureco_eta", "#nu Pseudo-Rapidity (all leptons)",
			       25, -5.0, 5.0);
    fHistNuReco_eta->SetXTitle("#eta");
    fHistNuReco_eta->SetYTitle("Number of Entries");
    fHistNuReco_E = new TH1F("h_nureco_E", "#nu Energy (all leptons)",
				     200, 0., 200);
    fHistNuReco_E->SetXTitle("E (GeV)");
    fHistNuReco_E->SetYTitle("Number of Entries");
    
    // Electron channel
    fHistNuReco_phi_e = new TH1F("h_nureco_phi_e", "#nu Azimuth Angle (e channel)",
				 128, -3.2, 3.2);
    fHistNuReco_phi_e->SetXTitle("#phi (rad)");
    fHistNuReco_phi_e->SetYTitle("Number of Entries");
    fHistNuReco_pt_e = new TH1F("h_nureco_pt_e", "#nu Transverse Momentum (e channel)",
				100, 0, 200);
    fHistNuReco_pt_e->SetXTitle("P_{t} (GeV)");
    fHistNuReco_pt_e->SetYTitle("Number of Entries");
    fHistNuReco_eta_e = new TH1F("h_nureco_eta_e", "#nu Pseudo-Rapidity (e channel)",
				 50, -5.0, 5.0);
    fHistNuReco_eta_e->SetXTitle("#eta");
    fHistNuReco_eta_e->SetYTitle("Number of Entries");
    fHistNuReco_E_e = new TH1F("h_nureco_E_e", "#nu Energy (e channel)",
				     200, 0., 200);
    fHistNuReco_E_e->SetXTitle("E (GeV)");
    fHistNuReco_E_e->SetYTitle("Number of Entries");
    
    // Muon channel
    fHistNuReco_phi_mu = new TH1F("h_nureco_phi_mu", "#nu Azimuth Angle (#mu channel)",
				  128, -3.2, 3.2);
    fHistNuReco_phi_mu->SetXTitle("#phi (rad)");
    fHistNuReco_phi_mu->SetYTitle("Number of Entries");
    fHistNuReco_pt_mu = new TH1F("h_nureco_pt_mu", "#nu Transverse Momentum (#mu channel)",
				 100, 0, 200);
    fHistNuReco_pt_mu->SetXTitle("P_{t} (GeV)");
    fHistNuReco_pt_mu->SetYTitle("Number of Entries");
    fHistNuReco_eta_mu = new TH1F("h_nureco_eta_mu", "#nu Pseudo-Rapidity (#mu channel)",
				  50, -5.0, 5.0);
    fHistNuReco_eta_mu->SetXTitle("#eta");
    fHistNuReco_eta_mu->SetYTitle("Number of Entries");
    fHistNuReco_E_mu = new TH1F("h_nureco_E_mu", "#nu Energy (#mu channel)",
				     200, 0., 200);
    fHistNuReco_E_mu->SetXTitle("E (GeV)");
    fHistNuReco_E_mu->SetYTitle("Number of Entries");
    
    
    gDirectory->cd("..");
    
    gDirectory->mkdir("neutrino", "neutrino reconstruction");
    gDirectory->cd("neutrino");

    // All positive sign leptons
    fHistNuReco_phi_plus = new TH1F("h_nureco_phi_plus",
				    "#nu Azimuth Angle (all positive sign leptons)",
				    128, -3.2, 3.2);
    fHistNuReco_phi_plus->SetXTitle("#phi (rad)");
    fHistNuReco_phi_plus->SetYTitle("Number of Entries");
    fHistNuReco_pt_plus = new TH1F("h_nureco_pt_plus",
				   "#nu Transverse Momentum (all positive sign leptons)",
				   200, 0, 200);
    fHistNuReco_pt_plus->SetXTitle("P_{t} (GeV)");
    fHistNuReco_pt_plus->SetYTitle("Number of Entries");
    fHistNuReco_eta_plus = new TH1F("h_nureco_eta_plus",
				    "#nu Pseudo-Rapidity (all positive sign leptons)",
				    50, -5.0, 5.0);
    fHistNuReco_eta_plus->SetXTitle("#eta");
    fHistNuReco_eta_plus->SetYTitle("Number of Entries");
    fHistNuReco_E_plus = new TH1F("h_nureco_E_plus","#nu Energy (all positive sign leptons)",
				     200, 0., 200);
    fHistNuReco_E_plus->SetXTitle("E (GeV)");
    fHistNuReco_E_plus->SetYTitle("Number of Entries");
    

    // Electron e+ channel
    fHistNuReco_phi_e_plus = new TH1F("h_nureco_phi_e_plus",
				      "#nu Azimuth Angle (e^{+} channel)",
				      128, -3.2, 3.2);
    fHistNuReco_phi_e_plus->SetXTitle("#phi (rad)");
    fHistNuReco_phi_e_plus->SetYTitle("Number of Entries");
    fHistNuReco_pt_e_plus = new TH1F("h_nureco_pt_e_plus", "#nu Transverse Momentum (e^{+} channel)",
				     200, 0, 200);
    fHistNuReco_pt_e_plus->SetXTitle("P_{t} (GeV)");
    fHistNuReco_pt_e_plus->SetYTitle("Number of Entries");
    fHistNuReco_eta_e_plus = new TH1F("h_nureco_eta_e_plus", "#nu Pseudo-Rapidity (e^{+} channel)",
				      50, -5.0, 5.0);
    fHistNuReco_eta_e_plus->SetXTitle("#eta");
    fHistNuReco_eta_e_plus->SetYTitle("Number of Entries");
    fHistNuReco_E_e_plus = new TH1F("h_nureco_E_e_plus", "#nu Energy (e^{+} channel)",
				     200, 0., 200);
    fHistNuReco_E_e_plus->SetXTitle("E (GeV)");
    fHistNuReco_E_e_plus->SetYTitle("Number of Entries");
    
    // Muon mu+ channel
    fHistNuReco_phi_mu_plus = new TH1F("h_nureco_phi_mu_plus", "#nu Azimuth Angle (#mu^{+} channel)",
				       128, -3.2, 3.2);
    fHistNuReco_phi_mu_plus->SetXTitle("#phi (rad)");
    fHistNuReco_phi_mu_plus->SetYTitle("Number of Entries");
    fHistNuReco_pt_mu_plus = new TH1F("h_nureco_pt_mu_plus", "#nu Transverse Momentum (#mu^{+} channel)",
				      200, 0, 200);
    fHistNuReco_pt_mu_plus->SetXTitle("P_{t} (GeV)");
    fHistNuReco_pt_mu_plus->SetYTitle("Number of Entries");
    fHistNuReco_eta_mu_plus = new TH1F("h_nureco_eta_mu_plus", "#nu Pseudo-Rapidity (#mu^{+} channel)",
				       50, -5.0, 5.0);
    fHistNuReco_eta_mu_plus->SetXTitle("#eta");
    fHistNuReco_eta_mu_plus->SetYTitle("Number of Entries");
    fHistNuReco_E_mu_plus = new TH1F("h_nureco_E_mu_plus", "#nu Energy (#mu^{+} channel)",
				     200, 0, 200);
    fHistNuReco_E_mu_plus->SetXTitle("E (GeV)");
    fHistNuReco_E_mu_plus->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    gDirectory->mkdir("anti-neutrino", "#bar{#nu} reconstruction");
    gDirectory->cd("anti-neutrino");

    // All negative sign leptons
    fHistNuReco_phi_minus = new TH1F("h_nureco_phi_minus",
				     "#bar{#nu} Azimuth Angle (all negative sign leptons)",
				     128, -3.2, 3.2);
    fHistNuReco_phi_minus->SetXTitle("#phi (rad)");
    fHistNuReco_phi_minus->SetYTitle("Number of Entries");
    fHistNuReco_pt_minus = new TH1F("h_nureco_pt_minus",
				    "#bar{#nu} Transverse Momentum (all negative sign leptons)",
				    200, 0, 200);
    fHistNuReco_pt_minus->SetXTitle("P_{t} (GeV)");
    fHistNuReco_pt_minus->SetYTitle("Number of Entries");
    fHistNuReco_eta_minus = new TH1F("h_nureco_eta_minus",
				     "#bar{#nu} Pseudo-Rapidity (all negative sign leptons)",
				     50, -5.0, 5.0);
    fHistNuReco_eta_minus->SetXTitle("#eta");
    fHistNuReco_eta_minus->SetYTitle("Number of Entries");
    fHistNuReco_E_minus = new TH1F("h_nureco_E_minus", "#bar{#nu} Energy (all negative sign leptons)",
				     200, 0, 200);
    fHistNuReco_E_minus->SetXTitle("E (GeV)");
    fHistNuReco_E_minus->SetYTitle("Number of Entries");

    // Electron e- channel
    fHistNuReco_phi_e_minus = new TH1F("h_nureco_phi_e_minus",
				       "#bar{#nu} Azimuth Angle (e^{-} channel)",
				       128, -3.2, 3.2);
    fHistNuReco_phi_e_minus->SetXTitle("#phi (rad)");
    fHistNuReco_phi_e_minus->SetYTitle("Number of Entries");
    fHistNuReco_pt_e_minus = new TH1F("h_nureco_pt_e_minus",
				      "#bar{#nu} Transverse Momentum (e^{-} channel)",
				      200, 0, 200);
    fHistNuReco_pt_e_minus->SetXTitle("P_{t} (GeV)");
    fHistNuReco_pt_e_minus->SetYTitle("Number of Entries");
    fHistNuReco_eta_e_minus = new TH1F("h_nureco_eta_e_minus",
				       "#bar{#nu} Pseudo-Rapidity (e^{-} channel)",
				       50, -5.0, 5.0);
    fHistNuReco_eta_e_minus->SetXTitle("#eta");
    fHistNuReco_eta_e_minus->SetYTitle("Number of Entries");
    fHistNuReco_E_e_minus = new TH1F("h_nureco_E_e_minus", "#bar{#nu} Energy (e^{-} channel)",
				     200, 0, 200);
    fHistNuReco_E_e_minus->SetXTitle("E (GeV)");
    fHistNuReco_E_e_minus->SetYTitle("Number of Entries");
    
    // Muon mu- channel
    fHistNuReco_phi_mu_minus = new TH1F("h_nureco_phi_mu_minus",
					"#bar{#nu} Azimuth Angle (#mu^{-} channel)",
					128, -3.2, 3.2);
    fHistNuReco_phi_mu_minus->SetXTitle("#phi (rad)");
    fHistNuReco_phi_mu_minus->SetYTitle("Number of Entries");
    fHistNuReco_pt_mu_minus = new TH1F("h_nureco_pt_mu_minus",
				       "#bar{#nu} Transverse Momentum (#mu^{-} channel)",
				       200, 0, 200);
    fHistNuReco_pt_mu_minus->SetXTitle("P_{t} (GeV)");
    fHistNuReco_pt_mu_minus->SetYTitle("Number of Entries");
    fHistNuReco_eta_mu_minus = new TH1F("h_nureco_eta_mu_minus",
					"#bar{#nu} Pseudo-Rapidity (#mu^{-} channel)",
					50, -5.0, 5.0);
    fHistNuReco_eta_mu_minus->SetXTitle("#eta");
    fHistNuReco_eta_mu_minus->SetYTitle("Number of Entries");
    fHistNuReco_E_mu_minus = new TH1F("h_nureco_E_mu_minus","#bar{#nu} Energy (#mu^{-} channel)",
				     200, 0, 200);
    fHistNuReco_E_mu_minus->SetXTitle("E (GeV)");
    fHistNuReco_E_mu_minus->SetYTitle("Number of Entries");

    gDirectory->cd("../..");

    
    // ===================================
    // Reconstructed charged signal lepton
    // ===================================
    gDirectory->mkdir("ChargedLeptonReco", "Reconstructed charged lepton after P(Chi2)-cut");
    gDirectory->cd("ChargedLeptonReco");

    // pt
    fHistChargedLeptonReco_pt = new TH1F("h_ChLep_pt",
			      "p_{T} of charged lepton (electron and muon) after P(Chi2)-cut",
			      30, 10., 200.);
    fHistChargedLeptonReco_pt->SetXTitle("p_{T}");
    fHistChargedLeptonReco_pt->SetYTitle("Number of Entries");
    // eta
    fHistChargedLeptonReco_eta = new TH1F("h_ChLep_eta",
			      "p_{T} of charged lepton (electron and muon) after P(Chi2)-cut",
			      30, -5., 5.);
    fHistChargedLeptonReco_eta->SetXTitle("#eta");
    fHistChargedLeptonReco_eta->SetYTitle("Number of Entries");

    gDirectory->cd("..");

    
    // ==================================================
    // Jet distributions for events surviving P(Chi2)-cut
    // ==================================================
    gDirectory->mkdir("JetReco", "Reconstructed jets after P(Chi2)-cut");
    gDirectory->cd("JetReco");

    // Jet Multiplicity
    fHistJetMultiplicityReco = new TH1F("h_NJets",
			      "Jet Multiplicity after P(Chi2)-cut",
			      10, -0.5, 9.5);
    fHistJetMultiplicityReco->SetXTitle("Multiplicity");
    fHistJetMultiplicityReco->SetYTitle("Number of Entries");
    // B Jet Multiplicity
    fHistBJetMultiplicityReco = new TH1F("h_NBJets",
			      "B Jet Multiplicity after P(Chi2)-cut",
			      5, 0.5, 4.5);
    fHistBJetMultiplicityReco->SetXTitle("Multiplicity");
    fHistBJetMultiplicityReco->SetYTitle("Number of Entries");
    // Number of untagged jets with |eta|>2
    fHistNonBJetMultiplicityEta2Reco = new TH1F("h_NNonBJetsEta2",
			      "# untagged jets with |#eta|>2 after P(Chi2)-cut",
			      8, -0.5, 7.5);
    fHistNonBJetMultiplicityEta2Reco->SetXTitle("Multiplicity");
    fHistNonBJetMultiplicityEta2Reco->SetYTitle("Number of Entries");
    // Number of untagged jets with |eta|>3
    fHistNonBJetMultiplicityEta3Reco = new TH1F("h_NNonBJetsEta3",
			      "# untagged jets with |#eta|>3 after P(Chi2)-cut",
			      8, -0.5, 7.5);
    fHistNonBJetMultiplicityEta3Reco->SetXTitle("Multiplicity");
    fHistNonBJetMultiplicityEta3Reco->SetYTitle("Number of Entries");
    // pt of leading non b jet
    fHistLeadingNonBJetReco_pt = new TH1F("h_NonBJet_pt",
			      "p_{T} of leading untagged jet after P(Chi2)-cut",
			      30, 10., 200.);
    fHistLeadingNonBJetReco_pt->SetXTitle("p_{T}");
    fHistLeadingNonBJetReco_pt->SetYTitle("Number of Entries");
    // eta of leading non b jet
    fHistLeadingNonBJetReco_eta = new TH1F("h_NonBJet_eta",
			      "#eta of leading untagged jet after P(Chi2)-cut",
			      30, -5., 5.);
    fHistLeadingNonBJetReco_eta->SetXTitle("#eta");
    fHistLeadingNonBJetReco_eta->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // ==========================================
    // Rest Frame Plots after P(Chi2)-cut
    // ==========================================
    gDirectory->mkdir("RestFrame", "Rest Frame Plots after P(Chi2)-cut");
    gDirectory->cd("RestFrame");
    
    //RestFrame of Top and of W
    gDirectory->mkdir("Top_W");
    gDirectory->cd("Top_W");
    //Theta*, which is the angle between charged lepton momentum  in W rest frame and W-Boson momentum in Top rest frame
    fHist_ThetaStar1_LepW = new TH1F("h_ThetaStar1_LepW",
				     "Angle between P_{Lep}^{W-RF} and P_{W}^{Top-RF} (RF = Rest Frame)",
				     30,0,3.15);
    fHist_ThetaStar1_LepW->SetXTitle("#vartheta_{1}^{*} [rad]");
    fHist_ThetaStar1_LepW->SetYTitle("Number of Entries");
    
    //cos(Theta*). Theta* is the angle between charged lepton momentum  in W rest frame and W-Boson momentum in Top rest frame
    fHist_Cos_ThetaStar1_LepW = new TH1F("h_Cos_ThetaStar1_LepW",
				     "Cos of Angle between P_{Lep}^{W-RF} and P_{W}^{Top-RF} (RF = Rest Frame)",
				     20,-1,1);
    fHist_Cos_ThetaStar1_LepW->SetXTitle("Cos(#vartheta_{1}^{*})");
    fHist_Cos_ThetaStar1_LepW->SetYTitle("Number of Entries");
    
    //Theta*, which is the angle between charged lepton momentum  in W rest frame b-Quark momentum also in W rest frame
    fHist_ThetaStar2_LepBQuark = new TH1F("h_ThetaStar2_LepBQuark",
				     "Angle between P_{Lep}^{W-RF} and P_{b-Quark}^{W-RF} (RF = Rest Frame)",
				     30,0,3.15);
    fHist_ThetaStar2_LepBQuark->SetXTitle("#vartheta_{2}^{*} [rad]");
    fHist_ThetaStar2_LepBQuark->SetYTitle("Number of Entries");
    
    //cos(Theta*). Theta* is the angle between charged lepton momentum  in W rest frame b-Quark momentum also in W rest frame
    fHist_Cos_ThetaStar2_LepBQuark = new TH1F("h_Cos_ThetaStar2_LepBQuark",
				     "Cos of Angle between P_{Lep}^{W-RF} and P_{b-Quark}^{W-RF} (RF = Rest Frame)",
				     20,-1,1);
    fHist_Cos_ThetaStar2_LepBQuark->SetXTitle("Cos(#vartheta_{2}^{*})");
    fHist_Cos_ThetaStar2_LepBQuark->SetYTitle("Number of Entries");
    
    //Sum of Theta1*+Theta2*
    fHist_SumThetaStar1ThetaStar2 = new TH1F("h_SumThetaStar1ThetaStar2",
				     "Sum of #vartheta_{1}^{*} and #vartheta_{2}^{*}",
				     20,2,4);
    fHist_SumThetaStar1ThetaStar2->SetXTitle("#vartheta_{1}^{*}+#vartheta_{2}^{*} [rad]");
    fHist_SumThetaStar1ThetaStar2->SetYTitle("Number of Entries");
   
    //cos(Theta1*+Theta2*)
    fHist_Cos_SumThetaStar1ThetaStar2 = new TH1F("h_Cos_SumThetaStar1ThetaStar2",
				     "Cos of #vartheta_{1}^{*} and #vartheta_{2}^{*}",
				     20,-1,-0.6);
    fHist_Cos_SumThetaStar1ThetaStar2->SetXTitle("Cos(#vartheta_{1}^{*}+#vartheta_{2}^{*}) [rad]");
    fHist_Cos_SumThetaStar1ThetaStar2->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    //RestFrame of Top
    
    //angle between Top and FwdJet in Top Rest Frame (only FwdJet is boosted)
    gDirectory->mkdir("Top");
    gDirectory->cd("Top");
    
    fHist_AngleTopFwdJet_TopRF = new TH1F("h_AngleTopFwdJet_TopRF",
					"Angle between Top and FwdJet in Top RF (only FwdJet is boosted)",
					30,0,3.15);
    fHist_AngleTopFwdJet_TopRF->SetXTitle("#Delta Angle(P_{Lab}^{Top},P_{TopRF}^{FwdJet}) [rad]"); 
    fHist_AngleTopFwdJet_TopRF->SetYTitle("Number of Entries");
    
    fHist_AmountSumPTopFwdJet_TopRF = new TH1F("h_AmountSumPTopFwdJet_TopRF",
					     "|P_{Lab}^{Top}+P_{TopRF}^{FwdJet}| in Top RF (only FwdJet is boosted)",
					     20,0,200);
    fHist_AmountSumPTopFwdJet_TopRF->SetXTitle("|P_{Lab}^{Top}+P_{TopRF}^{FwdJet}|");
    fHist_AmountSumPTopFwdJet_TopRF->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    gDirectory->cd("..");
     
    // ==========================================
    // Miscellaneous kinematics after P(Chi2)-cut
    // ==========================================
    gDirectory->mkdir("Miscellaneous", "Miscellaneous kinematics after P(Chi2)-cut");
    gDirectory->cd("Miscellaneous");

    // Modulus of ( Pt-vector of top-quark ) + ( Pt-vector of 2nd lead. jet
    // not chosen as b-jet for top-reco )
    fHistDeltaP2Mod_Top_2ndLeadJet = new TH1F("h_deltap2mod_top_2ndleadjet",
					      "Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of 2nd lead. jet",50, 0., 100.);
    fHistDeltaP2Mod_Top_2ndLeadJet->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{2nd lead jet}|");
    fHistDeltaP2Mod_Top_2ndLeadJet->SetYTitle("Number of Entries");    

    // The above modulus vs eta of the 2ndLeadJet (fine binning)
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_fine = new TH2F(
	"h_deltap2mod_top_2ndleadjet_vs_Eta2ndLeadJet_fine",
	"Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of 2nd lead. jet vs #eta of 2nd lead. jet",
	25, 0., 100.,
	15, -5., 5.);
    //	100, 0., 100.,
    //	100, -5., 5.);
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_fine->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{2nd lead jet}|");
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_fine->SetYTitle("#eta_{2nd lead jet}");
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_fine->SetZTitle("Number of Entries");


    // The above modulus vs eta of the 2ndLeadJet (normal binning)
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet = new TH2F(
	"h_deltap2mod_top_2ndleadjet_vs_Eta2ndLeadJet",
	"Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of 2nd lead. jet vs #eta of 2nd lead. jet",
	10, 0., 100.,
	15, -5., 5.);
    //	100, 0., 100.,
    //	100, -5., 5.);
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{2nd lead jet}|");
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet->SetYTitle("#eta_{2nd lead jet}");
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet->SetZTitle("Number of Entries");

     // The above modulus vs eta of the 2ndLeadJet (normal binning). The bins for high Eta are merged
    Double_t EtaRange[12]; 
    EtaRange[0]=-5.; 
    EtaRange[1]=-5.+30./15.;
    for (Int_t i=2;i<11;i++){
	EtaRange[i] = EtaRange[i-1]+10./15.;
    }
    EtaRange[11] = 5.;
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins = new TH2F(
	"h_deltap2mod_top_2ndleadjet_vs_Eta2ndLeadJet_Less_Bins",
	"Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of 2nd lead. jet vs #eta of 2nd lead. jet",
	10, 0., 100.,
	11, EtaRange);
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{2nd lead jet}|");
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins->SetYTitle("#eta_{2nd lead jet}");
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins->SetZTitle("Number of Entries");   
    
    Double_t EtaRange2[10]; 
    EtaRange2[0]=-5.; 
    EtaRange2[1]=-5.+40./15.;
    for (Int_t i=2;i<9;i++){
	EtaRange2[i] = EtaRange2[i-1]+10./15.;
    }
    EtaRange2[9] = 5.;
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins2 = new TH2F(
	"h_deltap2mod_top_2ndleadjet_vs_Eta2ndLeadJet_Less_Bins2",
	"Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of 2nd lead. jet vs #eta of 2nd lead. jet",
	10, 0., 100.,
	9, EtaRange2);
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins2->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{2nd lead jet}|");
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins2->SetYTitle("#eta_{2nd lead jet}");
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins2->SetZTitle("Number of Entries");   
    
    Double_t EtaRange3[8]; 
    EtaRange3[0]=-5.; 
    EtaRange3[1]=-5.+50./15.;
    for (Int_t i=2;i<7;i++){
	EtaRange3[i] = EtaRange3[i-1]+10./15.;
    }
    EtaRange3[7] = 5.;
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins3 = new TH2F(
	"h_deltap2mod_top_2ndleadjet_vs_Eta2ndLeadJet_Less_Bins3",
	"Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of 2nd lead. jet vs #eta of 2nd lead. jet",
	10, 0., 100.,
	7, EtaRange3);
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins3->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{2nd lead jet}|");
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins3->SetYTitle("#eta_{2nd lead jet}");
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins3->SetZTitle("Number of Entries"); 
    
    // The above modulus vs eta of the 2ndLeadJet (coarse binning)
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_coarse = new TH2F(
	"h_deltap2mod_top_2ndleadjet_vs_Eta2ndLeadJet_coarse",
	"Modulus of #vec{P}_{t} of reco top-quark + #vec{P}_{t} of 2nd lead. jet vs #eta of 2nd lead. jet",
	10, 0., 100.,
	5, -5., 5.);
    //	100, 0., 100.,
    //	100, -5., 5.);
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_coarse->SetXTitle("|#vec{P}_{t}^{top}+#vec{P}_{t}^{2nd lead jet}|");
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_coarse->SetYTitle("#eta_{2nd lead jet}");
    fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_coarse->SetZTitle("Number of Entries");
    

    // eta of 2ndLeadJet
    fHistEta2ndLeadJet = new TH1F("h_eta_2ndleadjet",
				  "#eta of 2nd lead. jet",
				  15, -5., 5.);
    fHistEta2ndLeadJet->SetXTitle("#eta_{2nd lead jet}");
    fHistEta2ndLeadJet->SetYTitle("Number of Entries");

    // angle between fwd jet and top quark
    fHistAngleFwdJetTopQuark = new TH1F("h_angle_2ndleadjet_top",
					"angle betweeen lead. jet besides b-jet used in reco and top-quark",
				  31, -6.3, 6.3);

    fHistAngleFwdJetTopQuark->SetXTitle("angle(lead jet #neq b-jet fit,top-quark)");

    fHistAngleFwdJetTopQuark->SetYTitle("Number of Entries");

    // cosine of angle between fwd jet and top quark
    fHistCosAngleFwdJetTopQuark = new TH1F("h_cos_angle_2ndleadjet_top",
					"Cosine of angle betweeen lead. jet besides b-jet used in reco and top-quark",
				  20, -1., 1.);

    fHistCosAngleFwdJetTopQuark->SetXTitle("Cos(angle(lead jet #neq b-jet fit,top-quark))");

    fHistCosAngleFwdJetTopQuark->SetYTitle("Number of Entries");

    
    // M_effective
    fHistMEffective = new TH1F("h_Meff",
			      "M_{effective} = #sum_{i}p_{T}^{i}+E_{T}^{Miss}",
			      30, 0., 1000.);
    fHistMEffective->SetXTitle("M_{eff}");
    fHistMEffective->SetYTitle("Number of Entries");
    // Sphericity
    fHistSphericity = new TH1F("h_Sphericity",
			      "Sphericity",
			      30, 0., 1.);
    fHistSphericity->SetXTitle("Sphericity");
    fHistSphericity->SetYTitle("Number of Entries");
    // x_light
    fHistXlightRel = new TH1F("h_x_lightRel",
			      "x_{Bjorken} of light quark, simple model, using max(x1, x2)",
			      30, 0., 0.3);
    fHistXlightRel->SetXTitle("x_{light}^{Rel}");
    fHistXlightRel->SetYTitle("Number of Entries");
    fHistXlightForwardJet = new TH1F("h_x_lightFJet",
			      "x_{Bjorken} of light quark, simple model, using Forward Jet",
			      30, 0., 0.15 );
    fHistXlightForwardJet->SetXTitle("x_{light}^{ForwardJet}");
    fHistXlightForwardJet->SetYTitle("Number of Entries");
    // x_bottom
    fHistXbottomRel = new TH1F("h_x_bottomRel",
			       "x_{Bjorken} of bottom quark, simple model, using min(x1, x2)",
			       30, 0., 0.04);
    fHistXbottomRel->SetXTitle("x_{bottom}^{Rel}");
    fHistXbottomRel->SetYTitle("Number of Entries");
    fHistXbottomForwardJet = new TH1F("h_x_bottomFJet",
				      "x_{Bjorken} of bottom quark, simple model, using Forward Jet",
				      30, 0., 0.04);
    fHistXbottomForwardJet->SetXTitle("x_{bottom}^{ForwardJet}");
    fHistXbottomForwardJet->SetYTitle("Number of Entries");
    // DeltaR between Charged Lepton and b jet from top decay
    fHistDeltaRChLepBJet = new TH1F("h_DeltaRChLepBJet",
			      "DeltaR between Charged Lepton and b jet from top decay",
			      30, 0., 5.);
    fHistDeltaRChLepBJet->SetXTitle("#Delta R(l,b)");
    fHistDeltaRChLepBJet->SetYTitle("Number of Entries");
    
    gDirectory->cd("../..");

}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::BookKinFitLeptonHistograms() {
    //
    // Book lepton histograms for top candidate events in KinFitter mode
    //
    gDirectory->mkdir("Leptons","Leptons used in reconstruction");
    gDirectory->cd("Leptons");

    // ===========
    // All leptons
    // ===========
    gDirectory->mkdir("All_leptons");
    gDirectory->cd("All_leptons");

    
    fHistKinFitCandidateSignalLepton_pt = new TH1F("h_sig_lepton_pt", "Signal Lepton Transverse Momentum",
				    200, 0, 200);
    fHistKinFitCandidateSignalLepton_pt->SetXTitle("P_{t} (GeV)");
    fHistKinFitCandidateSignalLepton_pt->SetYTitle("Number of Entries");
    fHistKinFitCandidateSignalLepton_eta = new TH1F("h_sig_lepton_eta", "Signal Lepton Pseudo-Rapidity",
				     50, -2.5, 2.5);
    fHistKinFitCandidateSignalLepton_eta->SetXTitle("#eta");
    fHistKinFitCandidateSignalLepton_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateSignalLepton_phi = new TH1F("h_sig_lepton_phi", "Signal Lepton Azimuth",
				     128, -3.2, 3.2);
    fHistKinFitCandidateSignalLepton_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateSignalLepton_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // =========
    // Electrons
    // =========
    gDirectory->mkdir("Electron");
    gDirectory->cd("Electron");


    
    //****************************************
    // Number of signal-electrons by author
    //****************************************
    fHistKinFitCandidateSignalElectron_author = new TH1F("h_signal_electron_author",
		  "Signal Electron Author",
		  (AtlElectron::fgNAuthors + 1), 0, (AtlElectron::fgNAuthors + 1 ));
    fHistKinFitCandidateSignalElectron_author->SetYTitle("Number of Entries");
    TAxis *axis = fHistKinFitCandidateSignalElectron_author->GetXaxis();
    for ( Int_t i = 0; i < AtlElectron::fgNAuthors; i++ )
	axis->SetBinLabel(i+1, AtlElectron::fgAuthorNames[i]);
    axis->SetBinLabel(6,"HighPt and Soft");

    // Both charges
    gDirectory->mkdir("both");
    gDirectory->cd("both");

    fHistKinFitCandidateSignalElectron_pt = new TH1F("h_sig_electron_pt", "Signal e Transverse Momentum",
				      200, 0, 200);
    fHistKinFitCandidateSignalElectron_pt->SetXTitle("P_{t} (GeV)");
    fHistKinFitCandidateSignalElectron_pt->SetYTitle("Number of Entries");
    fHistKinFitCandidateSignalElectron_eta = new TH1F("h_sig_electron_eta", "Signal e Pseudo-Rapidity",
				       50, -2.5, 2.5);
    fHistKinFitCandidateSignalElectron_eta->SetXTitle("#eta");
    fHistKinFitCandidateSignalElectron_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateSignalElectron_phi = new TH1F("h_sig_electron_phi", "Signal e Azimuth",
				       128, -3.2, 3.2);
    fHistKinFitCandidateSignalElectron_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateSignalElectron_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // e+
    gDirectory->mkdir("e+");
    gDirectory->cd("e+");

    fHistKinFitCandidateSignalEplus_pt = new TH1F("h_sig_eplus_pt", "Signal e^{+} Transverse Momentum",
				   200, 0, 200);
    fHistKinFitCandidateSignalEplus_pt->SetXTitle("P_{t} (GeV)");
    fHistKinFitCandidateSignalEplus_pt->SetYTitle("Number of Entries");
    fHistKinFitCandidateSignalEplus_eta = new TH1F("h_sig_eplus_eta", "Signal e^{+} Pseudo-Rapidity",
				    50, -2.5, 2.5);
    fHistKinFitCandidateSignalEplus_eta->SetXTitle("#eta");
    fHistKinFitCandidateSignalEplus_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateSignalEplus_phi = new TH1F("h_sig_eplus_phi", "Signal e^{+} Azimuth",
				    128, -3.2, 3.2);
    fHistKinFitCandidateSignalEplus_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateSignalEplus_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // e-
    gDirectory->mkdir("e-");
    gDirectory->cd("e-");

    fHistKinFitCandidateSignalEminus_pt = new TH1F("h_sig_eminus_pt", "Signal e^{-} Transverse Momentum",
				   200, 0, 200);
    fHistKinFitCandidateSignalEminus_pt->SetXTitle("P_{t} (GeV)");
    fHistKinFitCandidateSignalEminus_pt->SetYTitle("Number of Entries");
    fHistKinFitCandidateSignalEminus_eta = new TH1F("h_sig_eminus_eta", "Signal e^{-} Pseudo-Rapidity",
				     50, -2.5, 2.5);
    fHistKinFitCandidateSignalEminus_eta->SetXTitle("#eta");
    fHistKinFitCandidateSignalEminus_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateSignalEminus_phi = new TH1F("h_sig_eminus_phi", "Signal e^{-} Azimuth",
				    128, -3.2, 3.2);
    fHistKinFitCandidateSignalEminus_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateSignalEminus_phi->SetYTitle("Number of Entries");
    
    gDirectory->cd("../..");

    // =====
    // Muons
    // =====
    gDirectory->mkdir("Muon");
    gDirectory->cd("Muon");
    
    // Both charges
    gDirectory->mkdir("both");
    gDirectory->cd("both");

    fHistKinFitCandidateSignalMuon_pt = new TH1F("h_sig_muon_pt", "Signal #mu Transverse Momentum",
				  200, 0, 200);
    fHistKinFitCandidateSignalMuon_pt->SetXTitle("P_{t} (GeV)");
    fHistKinFitCandidateSignalMuon_pt->SetYTitle("Number of Entries");
    fHistKinFitCandidateSignalMuon_eta = new TH1F("h_sig_muon_eta", "Signal #mu Pseudo-Rapidity",
				   50, -2.5, 2.5);
    fHistKinFitCandidateSignalMuon_eta->SetXTitle("#eta");
    fHistKinFitCandidateSignalMuon_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateSignalMuon_phi = new TH1F("h_sig_muon_phi", "Signal #mu Azimuth",
				   128, -3.2, 3.2);
    fHistKinFitCandidateSignalMuon_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateSignalMuon_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // mu+
    gDirectory->mkdir("mu+");
    gDirectory->cd("mu+");

    fHistKinFitCandidateSignalMuplus_pt = new TH1F("h_sig_muplus_pt", "Signal #mu^{+} Transverse Momentum",
				    200, 0, 200);
    fHistKinFitCandidateSignalMuplus_pt->SetXTitle("P_{t} (GeV)");
    fHistKinFitCandidateSignalMuplus_pt->SetYTitle("Number of Entries");
    fHistKinFitCandidateSignalMuplus_eta = new TH1F("h_sig_muplus_eta", "Signal #mu^{+} Pseudo-Rapidity",
				     50, -2.5, 2.5);
    fHistKinFitCandidateSignalMuplus_eta->SetXTitle("#eta");
    fHistKinFitCandidateSignalMuplus_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateSignalMuplus_phi = new TH1F("h_sig_muplus_phi", "Signal #mu^{+} Azimuth",
				     128, -3.2, 3.2);
    fHistKinFitCandidateSignalMuplus_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateSignalMuplus_phi->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    // mu-
    gDirectory->mkdir("mu-");
    gDirectory->cd("mu-");

    fHistKinFitCandidateSignalMuminus_pt = new TH1F("h_sig_muminus_pt", "Signal #mu^{-} Transverse Momentum",
				   200, 0, 200);
    fHistKinFitCandidateSignalMuminus_pt->SetXTitle("P_{t} (GeV)");
    fHistKinFitCandidateSignalMuminus_pt->SetYTitle("Number of Entries");
    fHistKinFitCandidateSignalMuminus_eta = new TH1F("h_sig_muminus_eta", "Signal #mu^{-} Pseudo-Rapidity",
				     50, -2.5, 2.5);
    fHistKinFitCandidateSignalMuminus_eta->SetXTitle("#eta");
    fHistKinFitCandidateSignalMuminus_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateSignalMuminus_phi = new TH1F("h_sig_muminus_phi", "Signal #mu^{-} Azimuth",
				    128, -3.2, 3.2);
    fHistKinFitCandidateSignalMuminus_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateSignalMuminus_phi->SetYTitle("Number of Entries");

    gDirectory->cd("../..");

    // ===
    // Tau
    // ===
    gDirectory->mkdir("Tau");
    gDirectory->cd("Tau");
    
    // Both charges
    gDirectory->mkdir("both");
    gDirectory->cd("both");

    fHistKinFitCandidateSignalTau_pt = new TH1F("h_sig_tau_pt", "Signal #tau Transverse Momentum",
				  200, 0, 200);
    fHistKinFitCandidateSignalTau_pt->SetXTitle("P_{t} (GeV)");
    fHistKinFitCandidateSignalTau_pt->SetYTitle("Number of Entries");
    fHistKinFitCandidateSignalTau_eta = new TH1F("h_sig_tau_eta", "Signal #tau Pseudo-Rapidity",
				   50, -2.5, 2.5);
    fHistKinFitCandidateSignalTau_eta->SetXTitle("#eta");
    fHistKinFitCandidateSignalTau_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateSignalTau_phi = new TH1F("h_sig_tau_phi", "Signal #tau Azimuth",
				   128, -3.2, 3.2);
    fHistKinFitCandidateSignalTau_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateSignalTau_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // tau+
    gDirectory->mkdir("tau+");
    gDirectory->cd("tau+");

    fHistKinFitCandidateSignalTauplus_pt = new TH1F("h_sig_tauplus_pt", "Signal #tau^{+} Transverse Momentum",
				    200, 0, 200);
    fHistKinFitCandidateSignalTauplus_pt->SetXTitle("P_{t} (GeV)");
    fHistKinFitCandidateSignalTauplus_pt->SetYTitle("Number of Entries");
    fHistKinFitCandidateSignalTauplus_eta = new TH1F("h_sig_tauplus_eta", "Signal #tau^{+} Pseudo-Rapidity",
				     50, -2.5, 2.5);
    fHistKinFitCandidateSignalTauplus_eta->SetXTitle("#eta");
    fHistKinFitCandidateSignalTauplus_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateSignalTauplus_phi = new TH1F("h_sig_tauplus_phi", "Signal #tau^{+} Azimuth",
				     128, -3.2, 3.2);
    fHistKinFitCandidateSignalTauplus_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateSignalTauplus_phi->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    // tau-
    gDirectory->mkdir("tau-");
    gDirectory->cd("tau-");

    fHistKinFitCandidateSignalTauminus_pt = new TH1F("h_sig_tauminus_pt", "Signal #tau^{-} Transverse Momentum",
				   200, 0, 200);
    fHistKinFitCandidateSignalTauminus_pt->SetXTitle("P_{t} (GeV)");
    fHistKinFitCandidateSignalTauminus_pt->SetYTitle("Number of Entries");
    fHistKinFitCandidateSignalTauminus_eta = new TH1F("h_sig_tauminus_eta", "Signal #tau^{-} Pseudo-Rapidity",
				     50, -2.5, 2.5);
    fHistKinFitCandidateSignalTauminus_eta->SetXTitle("#eta");
    fHistKinFitCandidateSignalTauminus_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateSignalTauminus_phi = new TH1F("h_sig_tauminus_phi", "Signal #tau^{-} Azimuth",
				    128, -3.2, 3.2);
    fHistKinFitCandidateSignalTauminus_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateSignalTauminus_phi->SetYTitle("Number of Entries");
    
    gDirectory->cd("../../..");
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::BookKinFitJetHistograms() {
    //
    // Book jet histograms for top candidate events in KinFitter mode
    //
    gDirectory->mkdir("Jets","Jets used in reconstruction");
    gDirectory->cd("Jets");

    // ======
    // Jets
    // ======
    gDirectory->mkdir("Jet");
    gDirectory->cd("Jet");

    // All jets
    fHistKinFitCandidateJet_n = new TH1F("h_jet_n", "No. of all jets",
			   10, 0, 10);
    fHistKinFitCandidateJet_n->SetXTitle("N_{jet}");
    fHistKinFitCandidateJet_n->SetYTitle("Number of Entries");
    fHistKinFitCandidateJet_Et = new TH1F("h_jet_Et", "Transverse Energy of all jets",
			    200, 0, 200);
    fHistKinFitCandidateJet_Et->SetXTitle("E_{t} (GeV)");
    fHistKinFitCandidateJet_Et->SetYTitle("Number of Entries");
    fHistKinFitCandidateJet_eta = new TH1F("h_jet_eta", "Pseudo-Rapidity of all jets",
			     50, -5, 5);
    fHistKinFitCandidateJet_eta->SetXTitle("#eta");
    fHistKinFitCandidateJet_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateJet_phi = new TH1F("h_jet_phi", "Azimuth of all jets",
			     128, -3.2, 3.2);
    fHistKinFitCandidateJet_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateJet_phi->SetYTitle("Number of Entries");

    // Leading jet
    fHistKinFitCandidateJet1_Et = new TH1F("h_jet1_Et", "Transverse Energy of leading jet",
			    200, 0, 200);
    fHistKinFitCandidateJet1_Et->SetXTitle("E_{t} (GeV)");
    fHistKinFitCandidateJet1_Et->SetYTitle("Number of Entries");
    fHistKinFitCandidateJet1_eta = new TH1F("h_jet1_eta", "Pseudo-Rapidity of leading jet",
			     50, -5, 5);
    fHistKinFitCandidateJet1_eta->SetXTitle("#eta");
    fHistKinFitCandidateJet1_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateJet1_phi = new TH1F("h_jet1_phi", "Azimuth of leading jet",
			     128, -3.2, 3.2);
    fHistKinFitCandidateJet1_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateJet1_phi->SetYTitle("Number of Entries");

    // 2nd Leading jet
    fHistKinFitCandidateJet2_Et = new TH1F("h_jet2_Et", "Transverse Energy of 2nd leading jet",
			    200, 0, 200);
    fHistKinFitCandidateJet2_Et->SetXTitle("E_{t} (GeV)");
    fHistKinFitCandidateJet2_Et->SetYTitle("Number of Entries");
    fHistKinFitCandidateJet2_eta = new TH1F("h_jet2_eta", "Pseudo-Rapidity of 2nd leading jet",
			     50, -5, 5);
    fHistKinFitCandidateJet2_eta->SetXTitle("#eta");
    fHistKinFitCandidateJet2_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateJet2_phi = new TH1F("h_jet2_phi", "Azimuth of 2nd leading jet",
			     128, -3.2, 3.2);
    fHistKinFitCandidateJet2_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateJet2_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");


    // ======
    // B-Jets
    // ======
    gDirectory->mkdir("B-jet");
    gDirectory->cd("B-jet");

    // All b-jets
    fHistKinFitCandidateBJet_n = new TH1F("h_bjet_n", "No. of all b-jets",
			   10, 0, 10);
    fHistKinFitCandidateBJet_n->SetXTitle("N_{b-jet}");
    fHistKinFitCandidateBJet_n->SetYTitle("Number of Entries");
    fHistKinFitCandidateBJet_Et = new TH1F("h_bjet_Et", "Transverse Energy of all b-jets",
			    200, 0, 200);
    fHistKinFitCandidateBJet_Et->SetXTitle("E_{t} (GeV)");
    fHistKinFitCandidateBJet_Et->SetYTitle("Number of Entries");
    fHistKinFitCandidateBJet_eta = new TH1F("h_bjet_eta", "Pseudo-Rapidity of all b-jets",
			     50, -2.5, 2.5);
    fHistKinFitCandidateBJet_eta->SetXTitle("#eta");
    fHistKinFitCandidateBJet_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateBJet_phi = new TH1F("h_bjet_phi", "Azimuth of all b-jets",
			     128, -3.2, 3.2);
    fHistKinFitCandidateBJet_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateBJet_phi->SetYTitle("Number of Entries");

    // Leading b-jet
    fHistKinFitCandidateBJet1_Et = new TH1F("h_bjet1_Et", "Transverse Energy of leading b-jet",
			    200, 0, 200);
    fHistKinFitCandidateBJet1_Et->SetXTitle("E_{t} (GeV)");
    fHistKinFitCandidateBJet1_Et->SetYTitle("Number of Entries");
    fHistKinFitCandidateBJet1_eta = new TH1F("h_bjet1_eta", "Pseudo-Rapidity of leading b-jet",
			     50, -2.5, 2.5);
    fHistKinFitCandidateBJet1_eta->SetXTitle("#eta");
    fHistKinFitCandidateBJet1_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateBJet1_phi = new TH1F("h_bjet1_phi", "Azimuth of leading b-jet",
			     128, -3.2, 3.2);
    fHistKinFitCandidateBJet1_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateBJet1_phi->SetYTitle("Number of Entries");

    // 2nd Leading b-jet
    fHistKinFitCandidateBJet2_Et = new TH1F("h_bjet2_Et", "Transverse Energy of 2nd leading b-jet",
			    200, 0, 200);
    fHistKinFitCandidateBJet2_Et->SetXTitle("E_{t} (GeV)");
    fHistKinFitCandidateBJet2_Et->SetYTitle("Number of Entries");
    fHistKinFitCandidateBJet2_eta = new TH1F("h_bjet2_eta", "Pseudo-Rapidity of 2nd leading b-jet",
			     50, -2.5, 2.5);
    fHistKinFitCandidateBJet2_eta->SetXTitle("#eta");
    fHistKinFitCandidateBJet2_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateBJet2_phi = new TH1F("h_bjet2_phi", "Azimuth of 2nd leading b-jet",
			     128, -3.2, 3.2);
    fHistKinFitCandidateBJet2_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateBJet2_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");


    // ======
    // Non-B-Jets
    // ======
    gDirectory->mkdir("Non-B-jet");
    gDirectory->cd("Non-B-jet");

    // All non-b-jets
    fHistKinFitCandidateNonBJet_n = new TH1F("h_nonbjet_n", "No. of all non-b-jets",
			   10, 0, 10);
    fHistKinFitCandidateNonBJet_n->SetXTitle("N_{non-b-jet}");
    fHistKinFitCandidateNonBJet_n->SetYTitle("Number of Entries");
    fHistKinFitCandidateNonBJet_Et = new TH1F("h_nonbjet_Et", "Transverse Energy of all non-b-jets",
			    200, 0, 200);
    fHistKinFitCandidateNonBJet_Et->SetXTitle("E_{t} (GeV)");
    fHistKinFitCandidateNonBJet_Et->SetYTitle("Number of Entries");
    fHistKinFitCandidateNonBJet_eta = new TH1F("h_nonbjet_eta", "Pseudo-Rapidity of all non-b-jets",
			     50, -5, 5);
    fHistKinFitCandidateNonBJet_eta->SetXTitle("#eta");
    fHistKinFitCandidateNonBJet_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateNonBJet_phi = new TH1F("h_nonbjet_phi", "Azimuth of all non-b-jets",
			     128, -3.2, 3.2);
    fHistKinFitCandidateNonBJet_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateNonBJet_phi->SetYTitle("Number of Entries");

    // Leading non-b-jet
    fHistKinFitCandidateNonBJet1_Et = new TH1F("h_nonbjet1_Et", "Transverse Energy of leading non-b-jet",
			    200, 0, 200);
    fHistKinFitCandidateNonBJet1_Et->SetXTitle("E_{t} (GeV)");
    fHistKinFitCandidateNonBJet1_Et->SetYTitle("Number of Entries");
    fHistKinFitCandidateNonBJet1_eta = new TH1F("h_nonbjet1_eta", "Pseudo-Rapidity of leading non-b-jet",
			     50, -5, 5);
    fHistKinFitCandidateNonBJet1_eta->SetXTitle("#eta");
    fHistKinFitCandidateNonBJet1_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateNonBJet1_phi = new TH1F("h_nonbjet1_phi", "Azimuth of leading non-b-jet",
			     128, -3.2, 3.2);
    fHistKinFitCandidateNonBJet1_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateNonBJet1_phi->SetYTitle("Number of Entries");

    // 2nd Leading non-b-jet
    fHistKinFitCandidateNonBJet2_Et = new TH1F("h_nonbjet2_Et", "Transverse Energy of 2nd leading non-b-jet",
			    200, 0, 200);
    fHistKinFitCandidateNonBJet2_Et->SetXTitle("E_{t} (GeV)");
    fHistKinFitCandidateNonBJet2_Et->SetYTitle("Number of Entries");
    fHistKinFitCandidateNonBJet2_eta = new TH1F("h_nonbjet2_eta", "Pseudo-Rapidity of 2nd leading non-b-jet",
			     50, -5, 5);
    fHistKinFitCandidateNonBJet2_eta->SetXTitle("#eta");
    fHistKinFitCandidateNonBJet2_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateNonBJet2_phi = new TH1F("h_nonbjet2_phi", "Azimuth of 2nd leading non-b-jet",
			     128, -3.2, 3.2);
    fHistKinFitCandidateNonBJet2_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateNonBJet2_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // ============
    // Forward jets
    // ============
    gDirectory->mkdir("Fwd");
    gDirectory->cd("Fwd");

    // All forward jets
    fHistKinFitCandidateFwdJet_n = new TH1F("h_fwdjet_n", "No. of all forward jets",
			     10, 0, 10);
    fHistKinFitCandidateFwdJet_n->SetXTitle("N_{fwd-jet}");
    fHistKinFitCandidateFwdJet_n->SetYTitle("Number of Entries");
    fHistKinFitCandidateFwdJet_Et = new TH1F("h_fwdjet_Et", "Transverse Energy of all forward jets",
			      200, 0, 200);
    fHistKinFitCandidateFwdJet_Et->SetXTitle("E_{t} (GeV)");
    fHistKinFitCandidateFwdJet_Et->SetYTitle("Number of Entries");
    fHistKinFitCandidateFwdJet_eta = new TH1F("h_fwdjet_eta", "Pseudo-Rapidity of all forward jets",
			       100, -5, 5);
    fHistKinFitCandidateFwdJet_eta->SetXTitle("#eta");
    fHistKinFitCandidateFwdJet_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateFwdJet_phi = new TH1F("h_fwdjet_phi", "Azimuth of all forward jets",
			       128, -3.2, 3.2);
    fHistKinFitCandidateFwdJet_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateFwdJet_phi->SetYTitle("Number of Entries");

    // Leading  forward jet
    fHistKinFitCandidateFwdJet1_Et = new TH1F("h_fwdjet1_Et", "Transverse Energy of leading forward jet",
			       200, 0, 200);
    fHistKinFitCandidateFwdJet1_Et->SetXTitle("E_{t} (GeV)");
    fHistKinFitCandidateFwdJet1_Et->SetYTitle("Number of Entries");
    fHistKinFitCandidateFwdJet1_eta = new TH1F("h_fwdjet1_eta", "Pseudo-Rapidity of leading forward jet",
				100, -5, 5);
    fHistKinFitCandidateFwdJet1_eta->SetXTitle("#eta");
    fHistKinFitCandidateFwdJet1_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateFwdJet1_phi = new TH1F("h_fwdjet1_phi", "Azimuth of leading forward jet",
				128, -3.2, 3.2);
    fHistKinFitCandidateFwdJet1_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateFwdJet1_phi->SetYTitle("Number of Entries");

    // 2nd Leading forward jet
    fHistKinFitCandidateFwdJet2_Et = new TH1F("h_fwdjet2_Et", "Transverse Energy of 2nd leading forward jet",
			       200, 0, 200);
    fHistKinFitCandidateFwdJet2_Et->SetXTitle("E_{t} (GeV)");
    fHistKinFitCandidateFwdJet2_Et->SetYTitle("Number of Entries");
    fHistKinFitCandidateFwdJet2_eta = new TH1F("h_fwdjet2_eta", "Pseudo-Rapidity of 2nd leading forward jet",
				100, -5, 5);
    fHistKinFitCandidateFwdJet2_eta->SetXTitle("#eta");
    fHistKinFitCandidateFwdJet2_eta->SetYTitle("Number of Entries");
    fHistKinFitCandidateFwdJet2_phi = new TH1F("h_fwdjet2_phi", "Azimuth of 2nd leading forward jet",
				128, -3.2, 3.2);
    fHistKinFitCandidateFwdJet2_phi->SetXTitle("#phi (rad)");
    fHistKinFitCandidateFwdJet2_phi->SetYTitle("Number of Entries");

    gDirectory->cd("../..");
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::BookHadronicWHistograms() {
    // 
    // Book histograms of hadronic W reconstruction in Whad veto
    // 
    gDirectory->mkdir("HadronicW","HadronicW histograms based on ApplyWhadVeto");
    gDirectory->cd("HadronicW");
    
    fHistWHadReco_Mt = new TH1F("h_WHadReco_Mt", "hadronic W Invariant Transverse Mass",
			      200, 0, 130);
    fHistWHadReco_Mt->SetXTitle("M_{t,W} (GeV)");
    fHistWHadReco_Mt->SetYTitle("Number of Entries");
    fHistWHadReco_M = new TH1F("h_WHadReco_M", "hadronic W Invariant Mass",
			      200, 0, 95);
    fHistWHadReco_M->SetXTitle("M_{W} (GeV)");
    fHistWHadReco_M->SetYTitle("Number of Entries");
    fHistWHadReco_phi = new TH1F("h_WHadReco_phi", "hadronic W Azimuth Angle",
				128, -3.2, 3.2);
    fHistWHadReco_phi->SetXTitle("#phi (rad)");
    fHistWHadReco_phi->SetYTitle("Number of Entries");
    fHistWHadReco_pt = new TH1F("h_WHadReco_pt", "hadronic W Transverse Momentum",
			       200, 0, 200);
    fHistWHadReco_pt->SetXTitle("P_{t} (GeV)");
    fHistWHadReco_pt->SetYTitle("Number of Entries");
    fHistWHadReco_rap = new TH1F("h_WHadReco_rap", "hadronic W Rapidity",
				     50, -5., 5.);
    fHistWHadReco_rap->SetXTitle("y (rapidity)");
    fHistWHadReco_rap->SetYTitle("Number of Entries");

    fHistWHadReco_chi2 = new TH1F("h_chi2_both",
			      "#chi^{2} distribution of hadronic W reconstruction",
			      50,0,10);
    fHistWHadReco_chi2->SetXTitle("#chi^{2}");
    fHistWHadReco_chi2->SetYTitle("Number of Entries");
    fHistWHadReco_chi2prob = new TH1F("h_chi2prob_both",
				  "#chi^{2} prob. distribution of hadronic W reconstruction",
				  40,0,1);
    fHistWHadReco_chi2prob->SetXTitle("P(#chi^{2})");
    fHistWHadReco_chi2prob->SetYTitle("Number of Entries");

    
    // ==================
    // Pull Distributions
    // ==================    
    fHistPullJet1Pt = new TH1F("h_PullJet1Pt","Pull of Jet 1 Pt",
				  15,-4.,4.);
    fHistPullJet1Pt->SetXTitle("#frac{#Delta p_{T}}{#sigma_{#Delta p_{T}}}");
    fHistPullJet1Pt->SetYTitle("Number of Entries");
    fHistPullJet1Eta = new TH1F("h_PullJet1Eta","Pull of Jet 1 #eta",
				  15,-5.,5.);
    fHistPullJet1Eta->SetXTitle("#frac{#Delta#eta}{#sigma_{#Delta#eta}}");
    fHistPullJet1Eta->SetYTitle("Number of Entries");
    fHistPullJet1Phi = new TH1F("h_PullJet1Phi","Pull of Jet 1 #varphi",
				  15,-4.,4.);
    fHistPullJet1Phi->SetXTitle("#frac{#Delta#varphi}{#sigma_{#Delta#varphi}}");
    fHistPullJet1Phi->SetYTitle("Number of Entries");

    fHistPullJet2Pt = new TH1F("h_PullJet2Pt","Pull of Jet 2 Pt",
				  15,-4.,4.);
    fHistPullJet2Pt->SetXTitle("#frac{#Delta p_{T}}{#sigma_{#Delta p_{T}}}");
    fHistPullJet2Pt->SetYTitle("Number of Entries");
    fHistPullJet2Eta = new TH1F("h_PullJet2Eta","Pull of Jet 2 #eta",
				  15,-5.,5.);
    fHistPullJet2Eta->SetXTitle("#frac{#Delta#eta}{#sigma_{#Delta#eta}}");
    fHistPullJet2Eta->SetYTitle("Number of Entries");
    fHistPullJet2Phi = new TH1F("h_PullJet2Phi","Pull of Jet 2 #varphi",
				  15,-4.,4.);
    fHistPullJet2Phi->SetXTitle("#frac{#Delta#varphi}{#sigma_{#Delta#varphi}}");
    fHistPullJet2Phi->SetYTitle("Number of Entries");

    
    // =====================================================
    // 2D correlation plots: top-quark fit vs hadronic W fit
    // =====================================================
    
    // correlation P(chi2)-top vs P(chi2)-Whad
    fHist_Chi2Prob_vs_Chi2ProbWhad = new TH2F("h_chi2prob_vs_chi2probWhad",
					   "#chi^{2} prob. of top-quark fit vs #chi^{2} prob. of hadronic W fit in sgtop_tchannel reco",
					      50,0.,1.,
					      50,0.,1.);
    fHist_Chi2Prob_vs_Chi2ProbWhad->SetXTitle("P(#chi^{2}) top");
    fHist_Chi2Prob_vs_Chi2ProbWhad->SetYTitle("P(#chi^{2}) W_{had}");
    fHist_Chi2Prob_vs_Chi2ProbWhad->SetZTitle("Number of Entries");

    // correlation chi2-top vs chi2-Whad
    fHist_Chi2_vs_Chi2Whad = new TH2F("h_chi2_vs_chi2Whad",
				      "#chi^{2} of top-quark fit vs #chi^{2} of hadronic W fit in sgtop_tchannel reco",
				      100,0.,10.,
				      100,0.,10.);
    fHist_Chi2_vs_Chi2Whad->SetXTitle("#chi^{2} top");
    fHist_Chi2_vs_Chi2Whad->SetYTitle("#chi^{2} W_{had}");
    fHist_Chi2_vs_Chi2Whad->SetZTitle("Number of Entries");
    
    
    // correlation of M_invar_W_lep (top fit) and M_invar_W_had (Whad veto fit)
    fHist_M_invar_W_lep_vs_M_invar_W_had = new TH2F("h_m_invar_w_lep_vs_m_invar_W_had",
						    "Invariant mass of W in sgtop reco vs invariant mass of hadronic W",
						    100,0.,200.,
						    100,0.,200.);// 80, 50.,130. ***
    fHist_M_invar_W_lep_vs_M_invar_W_had->SetXTitle("M_{W}^{lep} (GeV)");
    fHist_M_invar_W_lep_vs_M_invar_W_had->SetYTitle("M_{W}^{had} (GeV)");
    fHist_M_invar_W_lep_vs_M_invar_W_had->SetZTitle("Number of Entries");

    
    gDirectory->cd("..");

}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::BookChi2Histograms() {
    //
    // Book Chi2 and P(Chi2) histograms of KinFitter
    //
    gDirectory->cd("RecoObjects_WhadVetoCut");
    
    gDirectory->mkdir("Chi2_Chi2prob","Chi-square and chi-square probability distributions");
    gDirectory->cd("Chi2_Chi2prob");

    // ================================
    // Chi2 of KinFitter reconstruction
    // ================================

    const Int_t nbins = 20;
    Double_t xmin = 1e-15;
    Double_t xmin2 =1e-15;
    Double_t xmax = 10000;
    Double_t xmax2 = 1;
    Double_t logxmin = TMath::Log10(xmin);
    Double_t logxmin2 = TMath::Log10(xmin2);
    Double_t logxmax = TMath::Log10(xmax);
    Double_t logxmax2 = TMath::Log10(xmax2);
    Double_t binwidth = (logxmax-logxmin)/nbins;
    Double_t binwidth2 = (logxmax2-logxmin2)/nbins;
    Double_t xbins[nbins+1];
    Double_t xbins2[nbins+1];
    xbins[0] = xmin;
//     xbins[0] = 0.;
//     xbins[1] = xmin;
    xbins2[0] = xmin2;
//     xbins2[0] = 0;
//     xbins2[1] = xmin2;
    for (Int_t i=1;i<=nbins;i++) {
	xbins[i] = xmin + TMath::Power(10,logxmin+i*binwidth);
	xbins2[i] = xmin2 + TMath::Power(10,logxmin2+i*binwidth2);
    }

    
    //
    // Before cut on P(Chi2)
    //
    
    // both W decays
    fHistChi2_both = new TH1F("h_chi2_both",
			      "#chi^{2}/NdoF distribution of sgtop_tchannel reco (both W decays)",
			      50,0,10);
    fHistChi2_both->SetXTitle("#chi^{2}/NdoF");
    fHistChi2_both->SetYTitle("Number of Entries");
    fHistChi2Prob_both = new TH1F("h_chi2prob_both",
				  "#chi^{2} prob. distribution of sgtop_tchannel reco (both W decays)",
				  40,0,1);
    fHistChi2Prob_both->SetXTitle("P(#chi^{2})");
    fHistChi2Prob_both->SetYTitle("Number of Entries");
    // also in logarithmic scale:
    fHistChi2_both_Log = new TH1F("h_chi2_both_log",
			      "#chi^{2}/NdoF distribution of sgtop_tchannel reco (both W decays)",
				  nbins,xbins);
    fHistChi2_both_Log->SetXTitle("#chi^{2}/NdoF");
    fHistChi2_both_Log->SetYTitle("Number of Entries");
    fHistChi2Prob_both_Log = new TH1F("h_chi2prob_both_log",
				  "#chi^{2} prob. distribution of sgtop_tchannel reco (both W decays)",
				  nbins,xbins2);
    fHistChi2Prob_both_Log->SetXTitle("P(#chi^{2})");
    fHistChi2Prob_both_Log->SetYTitle("Number of Entries");

    
    
    // W -> e nu decays
    fHistChi2_e = new TH1F("h_chi2_e",
			      "#chi^{2}/NdoF distribution of sgtop_tchannel reco (e channel)",
			      50,0,10);
    fHistChi2_e->SetXTitle("#chi^{2}/NdoF");
    fHistChi2_e->SetYTitle("Number of Entries");
    
    fHistChi2Prob_e = new TH1F("h_chi2prob_e",
				  "#chi^{2} prob. distribution of sgtop_tchannel reco (e channel)",
				  40,0,1);
    fHistChi2Prob_e->SetXTitle("P(#chi^{2})");
    fHistChi2Prob_e->SetYTitle("Number of Entries");
    
    // W -> mu nu decays
    fHistChi2_mu = new TH1F("h_chi2_mu",
			       "#chi^{2}/NdoF distribution of sgtop_tchannel reco (#mu channel)",
			       50,0,10);
    fHistChi2_mu->SetXTitle("#chi^{2}/NdoF");
    fHistChi2_mu->SetYTitle("Number of Entries");
    
    fHistChi2Prob_mu = new TH1F("h_chi2prob_mu",
				   "#chi^{2} prob. distr. of sgtop_tchannel reco (#mu channel)",
				   40,0,1);
    fHistChi2Prob_mu->SetXTitle("P(#chi^{2})");
    fHistChi2Prob_mu->SetYTitle("Number of Entries");


    //
    // After cut on P(Chi2)
    //
    
    // both W decays
    fHistChi2_both_cut = new TH1F("h_chi2_both_cut",
			      "#chi^{2}/NdoF distribution of sgtop_tchannel reco (both W decays) after P(Chi2) cut",
			      50,0,10);
    fHistChi2_both_cut->SetXTitle("#chi^{2}/NdoF");
    fHistChi2_both_cut->SetYTitle("Number of Entries");
    fHistChi2Prob_both_cut = new TH1F("h_chi2prob_both_cut",
				  "#chi^{2} prob. distribution of sgtop_tchannel reco (both W decays) after P(Chi2) cut",
				  40,0,1);
    fHistChi2Prob_both_cut->SetXTitle("P(#chi^{2})");
    fHistChi2Prob_both_cut->SetYTitle("Number of Entries");
    // also in logarithmic scale:
    fHistChi2_both_cut_Log = new TH1F("h_chi2_both_cut_log",
				      "#chi^{2}/NdoF distribution of sgtop_tchannel reco (both W decays) after P(Chi2) cut",
				      nbins,xbins);
    fHistChi2_both_cut_Log->SetXTitle("#chi^{2}/NdoF");
    fHistChi2_both_cut_Log->SetYTitle("Number of Entries");
    fHistChi2Prob_both_cut_Log = new TH1F("h_chi2prob_both_cut_log",
					  "#chi^{2} prob. distribution of sgtop_tchannel reco (both W decays) after P(Chi2) cut",
					  nbins,xbins2);
    fHistChi2Prob_both_cut_Log->SetXTitle("P(#chi^{2})");
    fHistChi2Prob_both_cut_Log->SetYTitle("Number of Entries");

    
    // W -> e nu decays
    fHistChi2_e_cut = new TH1F("h_chi2_e_cut",
			      "#chi^{2}/NdoF distribution of sgtop_tchannel reco (e channel) after P(Chi2) cut",
			      50,0,10);
    fHistChi2_e_cut->SetXTitle("#chi^{2}/NdoF");
    fHistChi2_e_cut->SetYTitle("Number of Entries");
    
    fHistChi2Prob_e_cut = new TH1F("h_chi2prob_e_cut",
				  "#chi^{2} prob. distribution of sgtop_tchannel reco (e channel)after P(Chi2) cut",
				  40,0,1);
    fHistChi2Prob_e_cut->SetXTitle("P(#chi^{2})");
    fHistChi2Prob_e_cut->SetYTitle("Number of Entries");
    
    // W -> mu nu decays
    fHistChi2_mu_cut = new TH1F("h_chi2_mu_cut",
			       "#chi^{2}/NdoF distribution of sgtop_tchannel reco (#mu channel) after P(Chi2) cut",
			       50,0,10);
    fHistChi2_mu_cut->SetXTitle("#chi^{2}/NdoF");
    fHistChi2_mu_cut->SetYTitle("Number of Entries");
    
    fHistChi2Prob_mu_cut = new TH1F("h_chi2prob_mu_cut",
				   "#chi^{2} prob. distr. of sgtop_tchannel reco (#mu channel) after P(Chi2) cut",
				   40,0,1);
    fHistChi2Prob_mu_cut->SetXTitle("P(#chi^{2})");
    fHistChi2Prob_mu_cut->SetYTitle("Number of Entries");

    

    //    
    // Eff. of kinematic fit depend. on value of Chi2Prob cut w.r.t. evt yield after Whad veto
    //
    fHistEfficiencyVsChi2ProbCut = new TH1F("h_eff_vs_chi2prob",
				  "Efficiency of sgtop_tchannel reco (both W decays) on signal MC vs #chi^{2} prob. cut w.r.t. event yield after Whad veto",
				  40,0,1);
    fHistEfficiencyVsChi2ProbCut->SetXTitle("P(#chi^{2})");
    fHistEfficiencyVsChi2ProbCut->SetYTitle("Efficiency");
    TAxis *axis = fHistEfficiencyVsChi2ProbCut->GetXaxis();
    TString dummy;
    for ( Int_t i = 0; i < 40; i++ ) {
      dummy = Form("%d",i);
      axis->SetBinLabel(i+1, dummy.Data());
    }


    
    //
    // 2D correlation plots
    //
    
    // correlation of P(chi2) and M_invar_W
    fHist_Chi2Prob_vs_M_invar_W = new TH2F("h_chi2prob_vs_m_invar_W",
					   "#chi^{2} prob. vs invariant mass of W in sgtop_tchannel reco",
					   25,0.,1.,
					   80,50.,130.);
    fHist_Chi2Prob_vs_M_invar_W->SetXTitle("P(#chi^{2})");
    fHist_Chi2Prob_vs_M_invar_W->SetYTitle("M_{W} (GeV)");
    fHist_Chi2Prob_vs_M_invar_W->SetZTitle("Number of Entries");
    
    // correlation of P(chi2) and M_invar_top
    fHist_Chi2Prob_vs_M_invar_top = new TH2F("h_chi2prob_vs_m_invar_top",
					   "#chi^{2} prob. vs invariant mass of top in sgtop_tchannel reco",
					     25,0.,1.,
					     60,150.,210.);
    fHist_Chi2Prob_vs_M_invar_top->SetXTitle("P(#chi^{2})");
    fHist_Chi2Prob_vs_M_invar_top->SetYTitle("M_{top} (GeV)");
    fHist_Chi2Prob_vs_M_invar_top->SetZTitle("Number of Entries");

    // correlation of M_invar_W and M_invar_top
    fHist_M_invar_W_vs_M_invar_top = new TH2F("h_m_invar_W_vs_m_invar_top",
					   "Invar. mass of W vs invar. mass of top in sgtop_tchannel reco",
					     100,70.,85.,
					     100,170.,175.);
    fHist_M_invar_W_vs_M_invar_top->SetXTitle("M_{W} (GeV)");
    fHist_M_invar_W_vs_M_invar_top->SetYTitle("M_{top} (GeV)");
    fHist_M_invar_W_vs_M_invar_top->SetZTitle("Number of Entries");

    // 1D histogram of ratio M_W over M_top
    fHist_M_invar_W_over_M_invar_top = new TH1F("h_m_invar_W_over_m_invar_top",
					   "Ratio of invar. mass of W and invar. mass of top in sgtop_tchannel reco",
						100,0.,2.5);
    fHist_M_invar_W_over_M_invar_top->SetXTitle("#frac{M_{W}}{M_{top}}");
    fHist_M_invar_W_over_M_invar_top->SetYTitle("Number of Entries");

    // correlation of P(chi2) of sgtop fit and P(chi2) of ttbar veto fit
    fHist_Chi2ProbSgTop_vs_Chi2ProbTTbar = new TH2F("h_chi2probSgtop_vs_Chi2ProbTTbar",
						    "#chi^{2} prob. of SgTop fit vs #chi^{2} prob. of t#bar{t} fit",
						    100,0.,1.,
						    100,0.,1.);
    fHist_Chi2ProbSgTop_vs_Chi2ProbTTbar->SetXTitle("P_{sgtop}(#chi^{2})");
    fHist_Chi2ProbSgTop_vs_Chi2ProbTTbar->SetYTitle("P_{ttbar}(#chi^{2})");
    fHist_Chi2ProbSgTop_vs_Chi2ProbTTbar->SetZTitle("Number of Entries");


    // P(Chi2)_SgTop / ( P(Chi2)_TTbar * P(Chi2)_Whad )
    fHist_Chi2ProbSgTop_over_Chi2ProbTTbarWhad = new TH1F("h_chi2prob_sgtop_over_ttbarWhad",
					 "P_{sgtop}(#chi^{2}) / ( P_{t#bar{t}}(#chi^{2}) * P_{Whad}(#chi^{2}) )",
					 200,0,20);
    fHist_Chi2ProbSgTop_over_Chi2ProbTTbarWhad->SetXTitle("#frac{P_{sgtop}(#chi^{2})}{P_{t #bar{t}}(#chi^{2}) * P_{Whad}(#chi^{2})}");
    fHist_Chi2ProbSgTop_over_Chi2ProbTTbarWhad->SetYTitle("No. of Events");

    
    //
    // Worse top candidates in event
    //

    // Number of worse top-quark candidates w.r.t. Chi2
    fHistNbWorseTopCandidates = new TH1I("h_NbWorseTopCandidates",
					 "No. of worse top-quark candidates (w.r.t. #chi^{2}) per event",
					 20,0,20);
    fHistNbWorseTopCandidates->SetXTitle("No. of top-quarks with worse #chi^{2} / Evt");
    fHistNbWorseTopCandidates->SetYTitle("No. of Events");

    // Chi2 value of worse top-quark candidates w.r.t. Chi2
    fHistChi2WorseTopCandidates = new TH1F("h_Chi2WorseTopCandidates",
			       "#chi^{2} of worse top-quark candidates (w.r.t. #chi^{2}) per event",
			       200,0,20);
    fHistChi2WorseTopCandidates->SetXTitle("#chi^{2} of top-quarks with worse #chi^{2} / Evt");
    fHistChi2WorseTopCandidates->SetYTitle("No. of Events");
    
    // P(Chi2) value of worse top-quark candidates w.r.t. Chi2
    fHistChi2ProbWorseTopCandidates = new TH1F("h_Chi2ProbWorseTopCandidates",
			       "P(#chi^{2}) of worse top-quark candidates (w.r.t. #chi^{2}) per event",
			       100,0,1);
    fHistChi2ProbWorseTopCandidates->SetXTitle("P(#chi^{2}) of top-quarks with worse #chi^{2} / Evt");
    fHistChi2ProbWorseTopCandidates->SetYTitle("No. of Events");

    
    
    gDirectory->cd("..");

}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::BookComparisonHistograms() {
    //
    // Book histograms for comparison of distributions before and after
    // KinFitter selection
    //
    gDirectory->mkdir("Comparison",
		      "Before/after fit comparison histograms");
    gDirectory->cd("Comparison");

    gDirectory->mkdir("Before",
		      "Histograms before fit ");
    gDirectory->cd("Before");

    // signal lepton
    fHistSignalLeptonPt_before = new TH1F("h_signal_lepton_pt_before",
				  "p_{T} of signal lepton before fit",
				  50,0.,200);
    fHistSignalLeptonPt_before->SetXTitle("p_{T}");
    fHistSignalLeptonPt_before->SetYTitle("Number of Entries");

    fHistSignalLeptonEta_before = new TH1F("h_signal_lepton_eta_before",
				  "#eta of signal lepton before fit",
				  50,-2.5,2.5);
    fHistSignalLeptonEta_before->SetXTitle("#eta");
    fHistSignalLeptonEta_before->SetYTitle("Number of Entries");


    fHistSignalLeptonEta_before_cells = new TH1F("h_signal_lepton_eta_before_cells",
				  "#eta of signal lepton before fit (cell granularity)",
				  100,-2.5,2.5);
    fHistSignalLeptonEta_before_cells->SetXTitle("#eta");
    fHistSignalLeptonEta_before_cells->SetYTitle("Number of Entries");

    
    fHistSignalLeptonPhi_before = new TH1F("h_signal_lepton_phi_before",
				  "#varphi of signal lepton before fit",
				  50,-3.2,3.2);
    fHistSignalLeptonPhi_before->SetXTitle("#varphi");
    fHistSignalLeptonPhi_before->SetYTitle("Number of Entries");

    // b-jet
    fHistBJetEt_before = new TH1F("h_bjet_et_before",
				  "E_{T} of b-jet before fit",
				  50,0.,200);
    fHistBJetEt_before->SetXTitle("E_{T}");
    fHistBJetEt_before->SetYTitle("Number of Entries");

    fHistBJetEta_before = new TH1F("h_bjet_eta_before",
				  "#eta of b-jet before fit",
				  50,-5.,5.0);
    fHistBJetEta_before->SetXTitle("#eta");
    fHistBJetEta_before->SetYTitle("Number of Entries");
    
    fHistBJetPhi_before = new TH1F("h_bjet_phi_before",
				  "#varphi of b-jet before fit",
				  50,-3.2,3.2);
    fHistBJetPhi_before->SetXTitle("#varphi");
    fHistBJetPhi_before->SetYTitle("Number of Entries");

    fHistBJetWeight_before = new TH1F("h_bjet_weight_before",
				  "b-tag weight of b-jet used in fit",
				  50,-10,12);
    fHistBJetWeight_before->SetXTitle("B-Tag Weight");
    fHistBJetWeight_before->SetYTitle("Number of Entries");

    
    // Et_miss
    fHistEtMissMag_before = new TH1F("h_etmiss_mag_before",
				  "Magnitude of missing E_{T} before fit",
				  100,0.,300);
    fHistEtMissMag_before->SetXTitle("E_{T}^{miss}");
    fHistEtMissMag_before->SetYTitle("Number of Entries");
    
    fHistEtMissPhi_before = new TH1F("h_etmiss_phi_before",
				  "#varphi of missing E_{T} before fit",
				  50,-3.2,3.2);
    fHistEtMissPhi_before->SetXTitle("#varphi");
    fHistEtMissPhi_before->SetYTitle("Number of Entries");

    // SumEt
    fHistSumEt_before = new TH1F("h_sumet_before",
				  "SumEt in events used for fit",
				  100,0,1000);
    fHistSumEt_before->SetXTitle("#sum E_{t} (GeV)");
    fHistSumEt_before->SetYTitle("Number of Entries");

    
    gDirectory->cd("..");

    gDirectory->mkdir("After",
		      "Histograms after fit ");
    gDirectory->cd("After");

    // signal lepton
    fHistSignalLeptonPt_after = new TH1F("h_signal_lepton_pt_after",
				  "p_{T} of signal lepton after fit",
				  50,0.,200);
    fHistSignalLeptonPt_after->SetXTitle("p_{T}");
    fHistSignalLeptonPt_after->SetYTitle("Number of Entries");

    fHistSignalLeptonEta_after = new TH1F("h_signal_lepton_eta_after",
				  "#eta of signal lepton after fit",
				  50,-2.5,2.5);
    fHistSignalLeptonEta_after->SetXTitle("#eta");
    fHistSignalLeptonEta_after->SetYTitle("Number of Entries");

    fHistSignalLeptonEta_after_cells = new TH1F("h_signal_lepton_eta_after_cells",
				  "#eta of signal lepton after fit (cell granularity)",
				  100,-2.5,2.5);
    fHistSignalLeptonEta_after_cells->SetXTitle("#eta");
    fHistSignalLeptonEta_after_cells->SetYTitle("Number of Entries");

    
    fHistSignalLeptonPhi_after = new TH1F("h_signal_lepton_phi_after",
				  "#varphi of signal lepton after fit",
				  50,-3.2,3.2);
    fHistSignalLeptonPhi_after->SetXTitle("#varphi");
    fHistSignalLeptonPhi_after->SetYTitle("Number of Entries");

    // b-jet
    fHistBJetEt_after = new TH1F("h_bjet_et_after",
				  "E_{T} of b-jet after fit",
				  50,0.,200);
    fHistBJetEt_after->SetXTitle("E_{T}");
    fHistBJetEt_after->SetYTitle("Number of Entries");

    fHistBJetEta_after = new TH1F("h_bjet_eta_after",
				  "#eta of b-jet after fit",
				  50,-5.,5.0);
    fHistBJetEta_after->SetXTitle("#eta");
    fHistBJetEta_after->SetYTitle("Number of Entries");
    
    fHistBJetPhi_after = new TH1F("h_bjet_phi_after",
				  "#varphi of b-jet after fit",
				  50,-3.2,3.2);
    fHistBJetPhi_after->SetXTitle("#varphi");
    fHistBJetPhi_after->SetYTitle("Number of Entries");
    
    // Et_miss
    fHistEtMissMag_after = new TH1F("h_etmiss_mag_after",
				    "Magnitude of missing E_{T} after fit",
				  50,0.,200);
    fHistEtMissMag_after->SetXTitle("E_{T}^{miss}");
    fHistEtMissMag_after->SetYTitle("Number of Entries");
    
    fHistEtMissPhi_after = new TH1F("h_etmiss_phi_after",
				  "#varphi of missing E_{T} after fit",
				  50,-3.2,3.2);
    fHistEtMissPhi_after->SetXTitle("#varphi");
    fHistEtMissPhi_after->SetYTitle("Number of Entries");    

    gDirectory->cd("..");

    // =================================
    // Normalized residual distributions
    // =================================
    gDirectory->mkdir("Residuals","Normalized residual histograms");
    gDirectory->cd("Residuals");
    
    // Lepton  
    gDirectory->mkdir("lepton_resid","Normalized residual histograms for leptons");
    gDirectory->cd("lepton_resid");
    
    // Lepton pt
    fHistNormResidLepton_pt = new TH1F("h_NormResidLepton_pt",
				  "Normalized Residual of Fit for Lepton p_{T}",
				  200, -1., 1.);
    fHistNormResidLepton_pt->SetXTitle("#frac{p_{T}^{final}-p_{T}^{initial}}{p_{T}^{initial}}");
    fHistNormResidLepton_pt->SetYTitle("Number of Entries");

    // Lepton eta
    fHistNormResidLepton_eta = new TH1F("h_NormResidLepton_eta",
				   "Normalized Residual of Fit for Lepton #eta",
				   200, -1., 1.);
    fHistNormResidLepton_eta->SetXTitle("#frac{#eta^{final}-#eta^{initial}}{#eta^{initial}}");
    fHistNormResidLepton_eta->SetYTitle("Number of Entries");
    
    // Lepton phi
    fHistNormResidLepton_phi = new TH1F("h_NormResidLepton_phi",
				   "Normalized Residual of Fit for Lepton #varphi",
				   200, -1., 1.);
    fHistNormResidLepton_phi->SetXTitle("#frac{#varphi^{final}-#varphi^{initial}}{#varphi^{initial}}");
    fHistNormResidLepton_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");

    // B-Jets  
    gDirectory->mkdir("bjet_resid","Normalized residual histograms for B-Jets");
    gDirectory->cd("bjet_resid");

    // B-Jet Et
    fHistNormResidBJet_Et = new TH1F("h_NormResidBJet_Et",
				"Normalized Residual of Fit for B-Jet E_{T}",
				100, -1., 1.);
    fHistNormResidBJet_Et->SetXTitle("#frac{E_{T}^{final}-E_{T}^{initial}}{E_{T}^{initial}}");
    fHistNormResidBJet_Et->SetYTitle("Number of Entries");
    
    // B-Jet pt
    fHistNormResidBJet_pt = new TH1F("h_NormResidBJet_pt",
				"Normalized Residual of Fit for B-Jet p_{T}",
				100, -1., 1.);
    fHistNormResidBJet_pt->SetXTitle("#frac{p_{T}^{final}-p_{T}^{initial}}{p_{T}^{initial}}");
    fHistNormResidBJet_pt->SetYTitle("Number of Entries");

    // B-Jet eta
    fHistNormResidBJet_eta = new TH1F("h_NormResidBJet_eta",
				 "Normalized Residual of Fit for B-Jet #eta",
				 100, -1., 1.);
    fHistNormResidBJet_eta->SetXTitle("#frac{#eta^{final}-#eta^{initial}}{#eta^{initial}}");
    fHistNormResidBJet_eta->SetYTitle("Number of Entries");
    
    // B-Jet phi
    fHistNormResidBJet_phi = new TH1F("h_NormResidBJet_phi",
				 "Normalized Residual of Fit for B-Jet #varphi",
				 100, -1., 1.);
    fHistNormResidBJet_phi->SetXTitle("#frac{#varphi^{final}-#varphi^{initial}}{#varphi^{initial}}");
    fHistNormResidBJet_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");



    // MET  
    gDirectory->mkdir("etmiss_resid","Normalized residual histograms for MET");
    gDirectory->cd("etmiss_resid");
    
    // MET Magnitude
    fHistNormResidEtmiss_Mag = new TH1F("h_NormResidEtmiss_Mag",
				"Normalized Residual of Fit for Magnitude of E_{T}^{miss}",
				100, -1., 1.);
    fHistNormResidEtmiss_Mag->SetXTitle("#frac{E_{T}^{miss,final}-E_{T}^{miss,initial}}{E_{T}^{miss,initial}}");
    fHistNormResidEtmiss_Mag->SetYTitle("Number of Entries"); 
    
    // MET phi
    fHistNormResidEtmiss_phi = new TH1F("h_NormResidEtmiss_phi",
				 "Normalized Residual of Fit for #varphi of E_{T}^{miss}",
				 200, -5., 5.);
    fHistNormResidEtmiss_phi->SetXTitle("#frac{#varphi^{final}-#varphi^{initial}}{#varphi^{initial}}");
    fHistNormResidEtmiss_phi->SetYTitle("Number of Entries");

    gDirectory->cd("../..");

    // ==================
    // Pull Distributions
    // ==================

    gDirectory->mkdir("Pulls","Pull Distributions");
    gDirectory->cd("Pulls");
    
    gDirectory->mkdir("Charged Lepton","Charged Lepton");
    gDirectory->cd("Charged Lepton");
    
    gDirectory->mkdir("Electron","Electron");
    gDirectory->cd("Electron");

    fHistPullElecPt = new TH1F("h_PullElecPt","Pull of Electron Pt",
				  15,-4.,4.);
    fHistPullElecPt->SetXTitle("#frac{#Delta p_{T}}{#sigma_{#Delta p_{T}}}");
    fHistPullElecPt->SetYTitle("Number of Entries");
    fHistPullElecEta = new TH1F("h_PullElecEta","Pull of Electron #eta",
				  15,-4.,4.);
    fHistPullElecEta->SetXTitle("#frac{#Delta#eta}{#sigma_{#Delta#eta}}");
    fHistPullElecEta->SetYTitle("Number of Entries");
    fHistPullElecPhi = new TH1F("h_PullElecPhi","Pull of Electron #varphi",
				  15,-4.,4.);
    fHistPullElecPhi->SetXTitle("#frac{#Delta#varphi}{#sigma_{#Delta#varphi}}");
    fHistPullElecPhi->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    gDirectory->mkdir("Muon","Muon");
    gDirectory->cd("Muon");

    fHistPullMuonPt = new TH1F("h_PullMuonPt","Pull of Muon Pt",
				  15,-4.,4.);
    fHistPullMuonPt->SetXTitle("#frac{#Delta p_{T}}{#sigma_{#Delta p_{T}}}");
    fHistPullMuonPt->SetYTitle("Number of Entries");
    fHistPullMuonEta = new TH1F("h_PullMuonEta","Pull of Muon #eta",
				  15,-4.,4.);
    fHistPullMuonEta->SetXTitle("#frac{#Delta#eta}{#sigma_{#Delta#eta}}");
    fHistPullMuonEta->SetYTitle("Number of Entries");
    fHistPullMuonPhi = new TH1F("h_PullMuonPhi","Pull of Muon #varphi",
				  15,-4.,4.);
    fHistPullMuonPhi->SetXTitle("#frac{#Delta#varphi}{#sigma_{#Delta#varphi}}");
    fHistPullMuonPhi->SetYTitle("Number of Entries");
    
    gDirectory->cd("../..");
    
    gDirectory->mkdir("EtMiss","EtMiss");
    gDirectory->cd("EtMiss");

    fHistPullEtMissMag = new TH1F("h_PullEtMMag","Pull of E_{T}^{miss}",
				  15,-4.,4.);
    fHistPullEtMissMag->SetXTitle("#frac{#Delta E_{T}^{miss}}{#sigma_{#Delta E_{T}^{miss}}}");
    fHistPullEtMissMag->SetYTitle("Number of Entries");
    fHistPullEtMissPhi = new TH1F("h_PullEtMPhi","Pull #varphi_{E_{T}^{miss}}",
				  15,-4.,4.);
    fHistPullEtMissPhi->SetXTitle("#frac{#Delta#varphi_{E_{T}^{miss}}}{#sigma_{#Delta#varphi_{E_{T}^{miss}}}}");
    fHistPullEtMissPhi->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    gDirectory->mkdir("BJet","BJet");
    gDirectory->cd("BJet");

    fHistPullBJetPt = new TH1F("h_PullBJetPt","Pull of BJet Pt",
				  15,-4.,4.);
    fHistPullBJetPt->SetXTitle("#frac{#Delta p_{T}}{#sigma_{#Delta p_{T}}}");
    fHistPullBJetPt->SetYTitle("Number of Entries");
    fHistPullBJetEta = new TH1F("h_PullBJetEta","Pull of BJet #eta",
				  15,-4.,4.);
    fHistPullBJetEta->SetXTitle("#frac{#Delta#eta}{#sigma_{#Delta#eta}}");
    fHistPullBJetEta->SetYTitle("Number of Entries");
    fHistPullBJetPhi = new TH1F("h_PullBJetPhi","Pull of BJet #varphi",
				  15,-4.,4.);
    fHistPullBJetPhi->SetXTitle("#frac{#Delta#varphi}{#sigma_{#Delta#varphi}}");
    fHistPullBJetPhi->SetYTitle("Number of Entries");

    gDirectory->cd("../..");

    
    //
    // Jet bin counts
    //
    gDirectory->mkdir("JetBins","Counts per jet bin");
    gDirectory->cd("JetBins");

    fHistJetBins_pretag = new TH1F("h_JetBins_pretag","Counts in jet bins (pretag)",
				   5,0.,5.);
    fHistJetBins_pretag->SetXTitle("Jet Bin");
    fHistJetBins_pretag->SetYTitle("Number of Entries");
    TAxis *axis = fHistJetBins_pretag->GetXaxis();
    axis->SetBinLabel(1,"2");
    axis->SetBinLabel(2,"3");
    axis->SetBinLabel(3,"4");
    axis->SetBinLabel(4,"#geq 4");
    axis->SetBinLabel(5,"2 #leq Njets #leq 4");


    fHistJetBins_tag = new TH1F("h_JetBins_tag","Counts in jet bins (tag)",
				   5,0.,5.);
    fHistJetBins_tag->SetXTitle("Jet Bin");
    fHistJetBins_tag->SetYTitle("Number of Entries");
    axis = fHistJetBins_tag->GetXaxis();
    axis->SetBinLabel(1,"2");
    axis->SetBinLabel(2,"3");
    axis->SetBinLabel(3,"4");
    axis->SetBinLabel(4,"#geq 4");
    axis->SetBinLabel(5,"2 #leq Njets #leq 4");


    fHistJetBins_fit = new TH1F("h_JetBins_fit","Counts in jet bins (after full fit selection)",
				   5,0.,5.);
    fHistJetBins_fit->SetXTitle("Jet Bin");
    fHistJetBins_fit->SetYTitle("Number of Entries");
    axis = fHistJetBins_fit->GetXaxis();
    axis->SetBinLabel(1,"2");
    axis->SetBinLabel(2,"3");
    axis->SetBinLabel(3,"4");
    axis->SetBinLabel(4,"#geq 4");
    axis->SetBinLabel(5,"2 #leq Njets #leq 4");
      

        
    if ( fTruthMatch == kTRUE ) gDirectory->cd("../..");
    else gDirectory->cd("../../../..");


}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::BookTruthMatchingHistograms() {
    // 
    // Book truth matching histograms
    //
    gDirectory->mkdir("TruthMatching","TruthMatching histograms");
    gDirectory->cd("TruthMatching");
    
    // charged lepton truth matching
    gDirectory->mkdir("ChargedLepton",
		      "TruthMatching histograms for electrons and muons");
    gDirectory->cd("ChargedLepton");
    
    // electron
    gDirectory->mkdir("electron","TruthMatching histograms for electrons");
    gDirectory->cd("electron");
    
    // Electron pt
    fHistElectronTruthMatch_pt = new TH1F("h_TMElectron_pt",
					  "Truth Matching of Electron p_{T}",
					  30, -0.15, 0.15);
    fHistElectronTruthMatch_pt->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistElectronTruthMatch_pt->SetYTitle("Number of Entries");
    
    // Electron phi
    fHistElectronTruthMatch_phi = new TH1F("h_TMElectron_phi",
					   "Truth Matching of Electron #varphi",
					   30, -0.05, 0.05);
    fHistElectronTruthMatch_phi->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistElectronTruthMatch_phi->SetYTitle("Number of Entries");

    // Electron eta
    fHistElectronTruthMatch_eta = new TH1F("h_TMElectron_eta",
					   "Truth Matching of Electron #eta",
					   30, -0.05, 0.05);
    fHistElectronTruthMatch_eta->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistElectronTruthMatch_eta->SetYTitle("Number of Entries");

//     // Electron pt
//     fHistElectronTruthMatchHits_pt = new TH1F("h_TMElectronHitsHits_pt",
// 					  "Truth Matching of Electron p_{T} based on ID hits",
// 					  30, -1.5, 1.5);
//     fHistElectronTruthMatchHits_pt->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
//     fHistElectronTruthMatchHits_pt->SetYTitle("Number of Entries");
    
//     // Electron phi
//     fHistElectronTruthMatchHits_phi = new TH1F("h_TMElectronHits_phi",
// 					   "Truth Matching of Electron #varphi based on ID hits",
// 					   30, -1.5, 1.5);
//     fHistElectronTruthMatchHits_phi->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
//     fHistElectronTruthMatchHits_phi->SetYTitle("Number of Entries");

//     // Electron eta
//     fHistElectronTruthMatchHits_eta = new TH1F("h_TMElectronHits_eta",
// 					   "Truth Matching of Electron #eta based on ID hits",
// 					   30, -1.5, 1.5);
//     fHistElectronTruthMatchHits_eta->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
//     fHistElectronTruthMatchHits_eta->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    // muon    
    gDirectory->mkdir("muon","TruthMatching histograms for muons");
    gDirectory->cd("muon");
    
    // Muon pt
    fHistMuonTruthMatch_pt = new TH1F("h_TMMuon_pt",
				      "Truth Matching of Muon p_{T}",
				      30, -1.5, 1.5);
    fHistMuonTruthMatch_pt->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistMuonTruthMatch_pt->SetYTitle("Number of Entries");
    
    // Muon phi
    fHistMuonTruthMatch_phi = new TH1F("h_TMMuon_phi",
				       "Truth Matching of Muon #varphi",
				       30, -1.5, 1.5);
    fHistMuonTruthMatch_phi->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistMuonTruthMatch_phi->SetYTitle("Number of Entries");

    // Muon eta
    fHistMuonTruthMatch_eta = new TH1F("h_TMMuon_eta",
				       "Truth Matching of Muon #eta",
				       30, -1.5, 1.5);
    fHistMuonTruthMatch_eta->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistMuonTruthMatch_eta->SetYTitle("Number of Entries");
    
    gDirectory->cd("../..");
    
    // Neutrino Truth Matching
    gDirectory->mkdir("Neutrino","TruthMatching histograms for Neutrino");
    gDirectory->cd("Neutrino");
    
    gDirectory->mkdir("e-channel","TruthMatching histograms in electron events");
    gDirectory->cd("e-channel");

    // Neutrino pt
    fHistNuTruthMatch_pt_e = new TH1F("h_TMNu_pt_e",
				      "Truth Matching of Neutrino p_{T}, e-channel",
				      30, -1.5, 1.5);
    fHistNuTruthMatch_pt_e->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistNuTruthMatch_pt_e->SetYTitle("Number of Entries");
    
    // Neutrino phi
    fHistNuTruthMatch_phi_e = new TH1F("h_TMNu_phi_e",
				       "Truth Matching of Neutrino #varphi, e-channel",
				       30, -1.5, 1.5);
    fHistNuTruthMatch_phi_e->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistNuTruthMatch_phi_e->SetYTitle("Number of Entries");

    // Neutrino theta
    fHistNuTruthMatch_theta_e = new TH1F("h_TMNu_theta_e",
					 "Truth Matching of Neutrino #theta, e-channel",
					 30, -1.5, 1.5);
    fHistNuTruthMatch_theta_e->SetXTitle("#frac{#theta^{rec}-#theta^{MC}}{#theta^{MC}}");
    fHistNuTruthMatch_theta_e->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");

    gDirectory->mkdir("mu-channel","TruthMatching histograms in muon events");
    gDirectory->cd("mu-channel");

    // Neutrino pt
    fHistNuTruthMatch_pt_mu = new TH1F("h_TMNu_pt_mu",
				       "Truth Matching of Neutrino p_{T}, #mu-channel",
				       30, -1.5, 1.5);
    fHistNuTruthMatch_pt_mu->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistNuTruthMatch_pt_mu->SetYTitle("Number of Entries");
    
    // Neutrino phi
    fHistNuTruthMatch_phi_mu = new TH1F("h_TMNu_phi_mu",
					"Truth Matching of Neutrino #varphi, #mu-channel",
					30, -1.5, 1.5);
    fHistNuTruthMatch_phi_mu->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistNuTruthMatch_phi_mu->SetYTitle("Number of Entries");
    
    // Neutrino theta
    fHistNuTruthMatch_theta_mu = new TH1F("h_TMNu_theta_mu",
					  "Truth Matching of Neutrino #theta, #mu-channel",
					  30, -1.5, 1.5);
    fHistNuTruthMatch_theta_mu->SetXTitle("#frac{#theta^{rec}-#theta^{MC}}{#theta^{MC}}");
    fHistNuTruthMatch_theta_mu->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");

    gDirectory->mkdir("both","TruthMatching histograms for electron and muon channel");
    gDirectory->cd("both");
    
    // Neutrino pt
    fHistNuTruthMatch_pt = new TH1F("h_TMNu_pt",
				    "Truth Matching of Neutrino p_{T}",
				    30, -1.5, 1.5);
    fHistNuTruthMatch_pt->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistNuTruthMatch_pt->SetYTitle("Number of Entries");
    
    // Neutrino phi
    fHistNuTruthMatch_phi = new TH1F("h_TMNu_phi",
				     "Truth Matching of Neutrino #varphi",
				     30, -1.5, 1.5);
    fHistNuTruthMatch_phi->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistNuTruthMatch_phi->SetYTitle("Number of Entries");

    // Neutrino theta
    fHistNuTruthMatch_theta = new TH1F("h_TMNu_theta",
				       "Truth Matching of Neutrino #theta",
				       30, -1.5, 1.5);
    fHistNuTruthMatch_theta->SetXTitle("#frac{#theta^{rec}-#theta^{MC}}{#theta^{MC}}");
    fHistNuTruthMatch_theta->SetYTitle("Number of Entries");

    // Neutrino eta
    fHistNuTruthMatch_eta = new TH1F("h_TMNu_eta",
				       "Truth Matching of Neutrino #eta",
				       30, -1.5, 1.5);
    fHistNuTruthMatch_eta->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistNuTruthMatch_eta->SetYTitle("Number of Entries");
    
    gDirectory->cd("../..");
    
    // b-jet truth matching w.r.t. the true b-quark
    gDirectory->mkdir("b-jet","TruthMatching histograms for b-jet");
    gDirectory->cd("b-jet");
    
    gDirectory->mkdir("b-quark","TruthMatching histograms for b-jet w.r.t. b-quark");
    gDirectory->cd("b-quark");
    
    gDirectory->mkdir("e-channel","TruthMatching histograms in electron events");
    gDirectory->cd("e-channel");

    // b-jet pt
    fHistBottomTruthMatch_pt_e = new TH1F("h_TMb_pt_e",
					"Truth Matching of b-jet p_{T} w.r.t. MC b-quark, e-channel",
					30, -1.5, 1.5);
    fHistBottomTruthMatch_pt_e->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistBottomTruthMatch_pt_e->SetYTitle("Number of Entries");
    
    // b-jet phi
    fHistBottomTruthMatch_phi_e = new TH1F("h_TMb_phi_e",
					 "Truth Matching of b-jet #varphi w.r.t. MC b-quark, e-channel",
					 30, -1.5, 1.5);
    fHistBottomTruthMatch_phi_e->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistBottomTruthMatch_phi_e->SetYTitle("Number of Entries");

    // b-jet eta
    fHistBottomTruthMatch_eta_e = new TH1F("h_TMb_eta_e",
					 "Truth Matching of b-jet #eta w.r.t. MC b-quark, e-channel",
					 30, -1.5, 1.5);
    fHistBottomTruthMatch_eta_e->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistBottomTruthMatch_eta_e->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    gDirectory->mkdir("mu-channel","TruthMatching histograms in muon events");
    gDirectory->cd("mu-channel");

    // b-jet pt
    fHistBottomTruthMatch_pt_mu = new TH1F("h_TMb_pt_mu",
					 "Truth Matching of b-jet p_{T} w.r.t. MC b-quark, #mu-channel",
					 30, -1.5, 1.5);
    fHistBottomTruthMatch_pt_mu->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistBottomTruthMatch_pt_mu->SetYTitle("Number of Entries");
    
    // b-jet phi
    fHistBottomTruthMatch_phi_mu = new TH1F("h_TMb_phi_mu",
					  "Truth Matching of b-jet #varphi w.r.t. MC b-quark, #mu-channel",
					  30, -1.5, 1.5);
    fHistBottomTruthMatch_phi_mu->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistBottomTruthMatch_phi_mu->SetYTitle("Number of Entries");
    
    // b-jet eta
    fHistBottomTruthMatch_eta_mu = new TH1F("h_TMb_eta_mu",
					  "Truth Matching of b-jet #eta w.r.t. MC b-quark, #mu-channel",
					  30, -1.5, 1.5);
    fHistBottomTruthMatch_eta_mu->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistBottomTruthMatch_eta_mu->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    gDirectory->mkdir("both","TruthMatching histograms for electron and muon channel");
    gDirectory->cd("both");
    
    // b-jet pt
    fHistBottomTruthMatch_pt = new TH1F("h_TMb_pt",
				      "Truth Matching of b-jet p_{T} w.r.t. MC b-quark",
				      30, -1.5, 1.5);
    fHistBottomTruthMatch_pt->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistBottomTruthMatch_pt->SetYTitle("Number of Entries");
    
    // b-jet phi
    fHistBottomTruthMatch_phi = new TH1F("h_TMb_phi",
				       "Truth Matching of b-jet #varphi w.r.t. MC b-quark",
				       30, -1.5, 1.5);
    fHistBottomTruthMatch_phi->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistBottomTruthMatch_phi->SetYTitle("Number of Entries");

    // b-jet eta
    fHistBottomTruthMatch_eta = new TH1F("h_TMb_eta",
				       "Truth Matching of b-jet #eta w.r.t. MC b-quark",
				       30, -1.5, 1.5);
    fHistBottomTruthMatch_eta->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistBottomTruthMatch_eta->SetYTitle("Number of Entries");

    gDirectory->mkdir("Original Jet","TruthMatching histograms for Original B Jet, e- and mu-channel");
    gDirectory->cd("Original Jet");

    // original b-jet pt
    fHistBottomTruthMatchOrig_pt = new TH1F("h_TMOrigb_pt",
				      "Truth Matching of original b-jet p_{T} w.r.t. MC b-quark",
				      30, -1.5, 1.5);
    fHistBottomTruthMatchOrig_pt->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistBottomTruthMatchOrig_pt->SetYTitle("Number of Entries");
    
    // original b-jet phi
    fHistBottomTruthMatchOrig_phi = new TH1F("h_TMOrigb_phi",
				       "Truth Matching of original b-jet #varphi w.r.t. MC b-quark",
				       30, -1.5, 1.5);
    fHistBottomTruthMatchOrig_phi->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistBottomTruthMatchOrig_phi->SetYTitle("Number of Entries");

    // original b-jet eta
    fHistBottomTruthMatchOrig_eta = new TH1F("h_TMOrigb_eta",
				       "Truth Matching of original b-jet #eta",
				       30, -1.5, 1.5);
    fHistBottomTruthMatchOrig_eta->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistBottomTruthMatchOrig_eta->SetYTitle("Number of Entries");
    
    gDirectory->cd("../../..");
    
    gDirectory->mkdir("Hadron Level Jet","TruthMatching histograms w.r.t. hadron level jet");
    gDirectory->cd("Hadron Level Jet");
    
    gDirectory->mkdir("e-channel","TruthMatching histograms in electron events w.r.t. hadron level jet");
    gDirectory->cd("e-channel");
    
    // b-jet truth matching w.r.t. the best matching Hadron Level jet
    // b-jet pt
    fHistHJetTruthMatch_pt_e = new TH1F("h_TMHJet_pt_e",
					"Truth Matching of b-jet p_{T} w.r.t. Hadron Level jet, e-channel",
					30, -1.5, 1.5);
    fHistHJetTruthMatch_pt_e->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistHJetTruthMatch_pt_e->SetYTitle("Number of Entries");
    
    // b-jet phi
    fHistHJetTruthMatch_phi_e = new TH1F("h_TMHJet_phi_e",
					 "Truth Matching of b-jet #varphi w.r.t. Hadron Level jet, e-channel",
					 30, -1.5, 1.5);
    fHistHJetTruthMatch_phi_e->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistHJetTruthMatch_phi_e->SetYTitle("Number of Entries");

    // b-jet eta
    fHistHJetTruthMatch_eta_e = new TH1F("h_TMHJet_eta_e",
					 "Truth Matching of b-jet #eta w.r.t. Hadron Level jet, e-channel",
					 30, -1.5, 1.5);
    fHistHJetTruthMatch_eta_e->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistHJetTruthMatch_eta_e->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    gDirectory->mkdir("mu-channel","TruthMatching histograms in muon events");
    gDirectory->cd("mu-channel");
    
    // b-jet pt
    fHistHJetTruthMatch_pt_mu = new TH1F("h_TMHJet_pt_mu",
					 "Truth Matching of b-jet p_{T} w.r.t. Hadron Level jet, #mu-channel",
					 30, -1.5, 1.5);
    fHistHJetTruthMatch_pt_mu->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistHJetTruthMatch_pt_mu->SetYTitle("Number of Entries");
    
    // b-jet phi
    fHistHJetTruthMatch_phi_mu = new TH1F("h_TMHJet_phi_mu",
					  "Truth Matching of b-jet #varphi w.r.t. Hadron Level jet, #mu-channel",
					  30, -1.5, 1.5);
    fHistHJetTruthMatch_phi_mu->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistHJetTruthMatch_phi_mu->SetYTitle("Number of Entries");
    
    // b-jet eta
    fHistHJetTruthMatch_eta_mu = new TH1F("h_TMHJet_eta_mu",
					  "Truth Matching of b-jet #eta w.r.t. Hadron Level jet, #mu-channel",
					  30, -1.5, 1.5);
    fHistHJetTruthMatch_eta_mu->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistHJetTruthMatch_eta_mu->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    gDirectory->mkdir("both","TruthMatching histograms for electron and muon channel");
    gDirectory->cd("both");
    
    // b-jet pt
    fHistHJetTruthMatch_pt = new TH1F("h_TMHJet_pt",
				      "Truth Matching of b-jet p_{T} w.r.t. Hadron Level jet",
				      30, -1.5, 1.5);
    fHistHJetTruthMatch_pt->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistHJetTruthMatch_pt->SetYTitle("Number of Entries");
    
    // b-jet phi
    fHistHJetTruthMatch_phi = new TH1F("h_TMHJet_phi",
				       "Truth Matching of b-jet #varphi w.r.t. Hadron Level jet",
				       30, -1.5, 1.5);
    fHistHJetTruthMatch_phi->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistHJetTruthMatch_phi->SetYTitle("Number of Entries");
    
    // b-jet eta
    fHistHJetTruthMatch_eta = new TH1F("h_TMHJet_eta",
				       "Truth Matching of b-jet #eta w.r.t. Hadron Level jet",
				       30, -1.5, 1.5);
    fHistHJetTruthMatch_eta->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistHJetTruthMatch_eta->SetYTitle("Number of Entries");

    gDirectory->mkdir("Original Jet","TruthMatching histograms for Original B Jet, e- and mu-channel");
    gDirectory->cd("Original Jet");

    // original b-jet pt
    fHistHJetTruthMatchOrig_pt = new TH1F("h_TMHJetOrig_pt",
				      "Truth Matching of original b-jet p_{T} w.r.t. Hadron Level jet",
				      30, -1.5, 1.5);
    fHistHJetTruthMatchOrig_pt->SetXTitle("#frac{p_{T}^{rec}-p_{T}^{MC}}{p_{T}^{MC}}");
    fHistHJetTruthMatchOrig_pt->SetYTitle("Number of Entries");
    
    // original b-jet phi
    fHistHJetTruthMatchOrig_phi = new TH1F("h_TMHJetOrig_phi",
				       "Truth Matching of original b-jet #varphi w.r.t. Hadron Level jeti",
				       30, -1.5, 1.5);
    fHistHJetTruthMatchOrig_phi->SetXTitle("#frac{#varphi^{rec}-#varphi^{MC}}{#varphi^{MC}}");
    fHistHJetTruthMatchOrig_phi->SetYTitle("Number of Entries");
    
    // original b-jet eta
    fHistHJetTruthMatchOrig_eta = new TH1F("h_TMHJetOrig_eta",
				       "Truth Matching of original b-jet #eta w.r.t. Hadron Level jet",
				       30, -1.5, 1.5);
    fHistHJetTruthMatchOrig_eta->SetXTitle("#frac{#eta^{rec}-#eta^{MC}}{#eta^{MC}}");
    fHistHJetTruthMatchOrig_eta->SetYTitle("Number of Entries");

    gDirectory->cd("../../../..");
    
    // histograms for overall MC Px and Py sum
    fHistTrueSumPx = new TH1F("h_MCsumPx",
			      "Sum of P_{x} of MC hadron jets, l^{#pm} and #nu",			      30, -100., 100.);
    fHistTrueSumPx->SetXTitle("#sum_{i}P_{x}^{i}");
    fHistTrueSumPx->SetYTitle("Number of Entries");
    fHistTrueSumPy = new TH1F("h_MCsumPy",
			      "Sum of P_{y} of MC hadron jets, l^{#pm} and #nu",
			      30, -100., 100.);
    fHistTrueSumPy->SetXTitle("#sum_{i}P_{y}^{i}");
    fHistTrueSumPy->SetYTitle("Number of Entries");

    gDirectory->cd("../..");

}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::BookCutBasedHistograms() {
    //
    // Book histograms for reconstructed W-boson and top-quark in
    // cut-based mode and call booking of reco. lepton and jet histograms
    //
    gDirectory->mkdir("CutBasedMode","Cut-based mode of sgtop t-channel finder");
    gDirectory->cd("CutBasedMode");
    
    BookCutBasedLeptonHistograms();
    BookCutBasedJetHistograms();

    // ================
    // W reconstruction
    // ================
    gDirectory->mkdir("W-reco", "W reconstruction");
    gDirectory->cd("W-reco");
    
    gDirectory->mkdir("Both", "Incl. W+/W- reconstruction");
    gDirectory->cd("Both");
    
    
    // All leptons
    fHistWreco_cutbased_Mt = new TH1F("h_Wreco_cutbased_Mt", "W Invariant Transverse Mass (all leptons)",
			      200, 30, 130);
    fHistWreco_cutbased_Mt->SetXTitle("M_{t,W} (GeV)");
    fHistWreco_cutbased_Mt->SetYTitle("Number of Entries");
    fHistWreco_cutbased_phi = new TH1F("h_Wreco_cutbased_phi", "W Azimuth Angle (all leptons)",
				128, -3.2, 3.2);
    fHistWreco_cutbased_phi->SetXTitle("#phi (rad)");
    fHistWreco_cutbased_phi->SetYTitle("Number of Entries");
    fHistWreco_cutbased_pt = new TH1F("h_Wreco_cutbased_pt", "W Transverse Momentum (all leptons)",
			       200, 0, 200);
    fHistWreco_cutbased_pt->SetXTitle("P_{t} (GeV)");
    fHistWreco_cutbased_pt->SetYTitle("Number of Entries");
    fHistWreco_cutbased_eta = new TH1F("h_Wreco_cutbased_eta", "W Pseudo-Rapidity (all leptons)",
				     50, -2.5, 2.5);
    fHistWreco_cutbased_eta->SetXTitle("#eta");
    fHistWreco_cutbased_eta->SetYTitle("Number of Entries");
    
    
    // Electron channel
    fHistWreco_cutbased_Mt_e = new TH1F("h_Wreco_cutbased_Mt_e", "W Invariant Transverse Mass (e channel)",
			      200, 30, 130);
    fHistWreco_cutbased_Mt_e->SetXTitle("M_{t,W} (GeV)");
    fHistWreco_cutbased_Mt_e->SetYTitle("Number of Entries");
    fHistWreco_cutbased_phi_e = new TH1F("h_Wreco_cutbased_phi_e", "W Azimuth Angle (e channel)",
				128, -3.2, 3.2);
    fHistWreco_cutbased_phi_e->SetXTitle("#phi (rad)");
    fHistWreco_cutbased_phi_e->SetYTitle("Number of Entries");
    fHistWreco_cutbased_pt_e = new TH1F("h_Wreco_cutbased_pt_e", "W Transverse Momentum (e channel)",
			       200, 0, 200);
    fHistWreco_cutbased_pt_e->SetXTitle("P_{t} (GeV)");
    fHistWreco_cutbased_pt_e->SetYTitle("Number of Entries");
    fHistWreco_cutbased_eta_e = new TH1F("h_Wreco_cutbased_eta_e", "W Pseudo-Rapidity (e channel)",
				     50, -2.5, 2.5);
    fHistWreco_cutbased_eta_e->SetXTitle("#eta");
    fHistWreco_cutbased_eta_e->SetYTitle("Number of Entries");
    


    // Muon channel
    fHistWreco_cutbased_Mt_mu = new TH1F("h_Wreco_cutbased_Mt_mu", "W Invariant Transverse Mass (#mu channel)",
			       200, 30, 130);
    fHistWreco_cutbased_Mt_mu->SetXTitle("M_{t,W} (GeV)");
    fHistWreco_cutbased_Mt_mu->SetYTitle("Number of Entries");
    fHistWreco_cutbased_phi_mu = new TH1F("h_Wreco_cutbased_phi_mu", "W Azimuth Angle (#mu channel)",
				 128, -3.2, 3.2);
    fHistWreco_cutbased_phi_mu->SetXTitle("#phi (rad)");
    fHistWreco_cutbased_phi_mu->SetYTitle("Number of Entries");
    fHistWreco_cutbased_pt_mu = new TH1F("h_Wreco_cutbased_pt_mu", "W Transverse Momentum (#mu channel)",
				200, 0, 200);
    fHistWreco_cutbased_pt_mu->SetXTitle("P_{t} (GeV)");
    fHistWreco_cutbased_pt_mu->SetYTitle("Number of Entries");
    fHistWreco_cutbased_eta_mu = new TH1F("h_Wreco_cutbased_eta_mu", "W Pseudo-Rapidity (#mu channel)",
				     50, -2.5, 2.5);
    fHistWreco_cutbased_eta_mu->SetXTitle("#eta");
    fHistWreco_cutbased_eta_mu->SetYTitle("Number of Entries");

    
    // Tauon channel
    fHistWreco_cutbased_Mt_tau = new TH1F("h_Wreco_cutbased_Mt_tau", "W Invariant Transverse Mass (#tau channel)",
			       200, 30, 130);
    fHistWreco_cutbased_Mt_tau->SetXTitle("M_{t,W} (GeV)");
    fHistWreco_cutbased_Mt_tau->SetYTitle("Number of Entries");
    fHistWreco_cutbased_phi_tau = new TH1F("h_Wreco_cutbased_phi_tau", "W Azimuth Angle (#tau channel)",
				 128, -3.2, 3.2);
    fHistWreco_cutbased_phi_tau->SetXTitle("#phi (rad)");
    fHistWreco_cutbased_phi_tau->SetYTitle("Number of Entries");
    fHistWreco_cutbased_pt_tau = new TH1F("h_Wreco_cutbased_pt_tau", "W Transverse Momentum (#tau channel)",
				200, 0, 200);
    fHistWreco_cutbased_pt_tau->SetXTitle("P_{t} (GeV)");
    fHistWreco_cutbased_pt_tau->SetYTitle("Number of Entries");
    fHistWreco_cutbased_eta_tau = new TH1F("h_Wreco_cutbased_eta_tau", "W Pseudo-Rapidity (#tau channel)",
				     50, -2.5, 2.5);
    fHistWreco_cutbased_eta_tau->SetXTitle("#eta");
    fHistWreco_cutbased_eta_tau->SetYTitle("Number of Entries");

    
    gDirectory->cd("..");

    gDirectory->mkdir("W-", "W- reconstruction");
    gDirectory->cd("W-");
    

    // All negative sign leptons
    fHistWreco_cutbased_Mt_minus = new TH1F("h_Wreco_cutbased_Mt_minus", "W^{-} Invariant Transverse Mass (all negative sign leptons)", 200, 30, 130);
    fHistWreco_cutbased_Mt_minus->SetXTitle("M_{t,W} (GeV)");
    fHistWreco_cutbased_Mt_minus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_phi_minus = new TH1F("h_Wreco_cutbased_phi_minus", "W^{-} Azimuth Angle (all negative sign leptons)",
				128, -3.2, 3.2);
    fHistWreco_cutbased_phi_minus->SetXTitle("#phi (rad)");
    fHistWreco_cutbased_phi_minus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_pt_minus = new TH1F("h_Wreco_cutbased_pt_minus", "W^{-} Transverse Momentum (all negative sign leptons)",
			       200, 0, 200);
    fHistWreco_cutbased_pt_minus->SetXTitle("P_{t} (GeV)");
    fHistWreco_cutbased_pt_minus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_eta_minus = new TH1F("h_Wreco_cutbased_eta_minus", "W^{-} Pseudo-Rapidity (all negative sign leptons)",
				     50, -2.5, 2.5);
    fHistWreco_cutbased_eta_minus->SetXTitle("#eta");
    fHistWreco_cutbased_eta_minus->SetYTitle("Number of Entries");
    
    
    // Electron e- channel
    fHistWreco_cutbased_Mt_e_minus = new TH1F("h_Wreco_cutbased_Mt_e_minus", "W^{-} Invariant Transverse Mass (e^{-} channel)",
			      200, 30, 130);
    fHistWreco_cutbased_Mt_e_minus->SetXTitle("M_{t,W} (GeV)");
    fHistWreco_cutbased_Mt_e_minus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_phi_e_minus = new TH1F("h_Wreco_cutbased_phi_e_minus", "W^{-} Azimuth Angle (e^{-} channel)",
				128, -3.2, 3.2);
    fHistWreco_cutbased_phi_e_minus->SetXTitle("#phi (rad)");
    fHistWreco_cutbased_phi_e_minus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_pt_e_minus = new TH1F("h_Wreco_cutbased_pt_e_minus", "W^{-} Transverse Momentum (e^{-} channel)",
			       200, 0, 200);
    fHistWreco_cutbased_pt_e_minus->SetXTitle("P_{t} (GeV)");
    fHistWreco_cutbased_pt_e_minus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_eta_e_minus = new TH1F("h_Wreco_cutbased_eta_e_minus", "W^{-} Pseudo-Rapidity (e^{-} channel)",
				     50, -2.5, 2.5);
    fHistWreco_cutbased_eta_e_minus->SetXTitle("#eta");
    fHistWreco_cutbased_eta_e_minus->SetYTitle("Number of Entries");
    
    

    // Muon mu- channel
    fHistWreco_cutbased_Mt_mu_minus = new TH1F("h_Wreco_cutbased_Mt_mu_minus", "W^{-} Invariant Transverse Mass (#mu^{-} channel)",
			       200, 30, 130);
    fHistWreco_cutbased_Mt_mu_minus->SetXTitle("M_{t,W} (GeV)");
    fHistWreco_cutbased_Mt_mu_minus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_phi_mu_minus = new TH1F("h_Wreco_cutbased_phi_mu_minus", "W^{-} Azimuth Angle (#mu^{-} channel)",
				 128, -3.2, 3.2);
    fHistWreco_cutbased_phi_mu_minus->SetXTitle("#phi (rad)");
    fHistWreco_cutbased_phi_mu_minus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_pt_mu_minus = new TH1F("h_Wreco_cutbased_pt_mu_minus", "W^{-} Transverse Momentum (#mu^{-} channel)",
				200, 0, 200);
    fHistWreco_cutbased_pt_mu_minus->SetXTitle("P_{t} (GeV)");
    fHistWreco_cutbased_pt_mu_minus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_eta_mu_minus = new TH1F("h_Wreco_cutbased_eta_mu_minus", "W^{-} Pseudo-Rapidity (#mu^{-} channel)",
				     50, -2.5, 2.5);
    fHistWreco_cutbased_eta_mu_minus->SetXTitle("#eta");
    fHistWreco_cutbased_eta_mu_minus->SetYTitle("Number of Entries");

    
    // Tauon tau- channel
    fHistWreco_cutbased_Mt_tau_minus = new TH1F("h_Wreco_cutbased_Mt_tau_minus", "W^{-} Invariant Transverse Mass (#tau^{-} channel)",
			       200, 30, 130);
    fHistWreco_cutbased_Mt_tau_minus->SetXTitle("M_{t,W} (GeV)");
    fHistWreco_cutbased_Mt_tau_minus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_phi_tau_minus = new TH1F("h_Wreco_cutbased_phi_tau_minus", "W^{-} Azimuth Angle (#tau^{-} channel)",
				 128, -3.2, 3.2);
    fHistWreco_cutbased_phi_tau_minus->SetXTitle("#phi (rad)");
    fHistWreco_cutbased_phi_tau_minus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_pt_tau_minus = new TH1F("h_Wreco_cutbased_pt_tau_minus", "W^{-} Transverse Momentum (#tau^{-} channel)",
				200, 0, 200);
    fHistWreco_cutbased_pt_tau_minus->SetXTitle("P_{t} (GeV)");
    fHistWreco_cutbased_pt_tau_minus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_eta_tau_minus = new TH1F("h_Wreco_cutbased_eta_tau_minus", "W^{-} Pseudo-Rapidity (#tau^{-} channel)",
				     50, -2.5, 2.5);
    fHistWreco_cutbased_eta_tau_minus->SetXTitle("#eta");
    fHistWreco_cutbased_eta_tau_minus->SetYTitle("Number of Entries");


    
    gDirectory->cd("..");

    
    gDirectory->mkdir("W+", "W+ reconstruction");
    gDirectory->cd("W+");


    
    // All positive sign leptons
    fHistWreco_cutbased_Mt_plus = new TH1F("h_Wreco_cutbased_Mt_plus", "W^{+} Invariant Transverse Mass (all positive sign leptons)", 200, 30, 130);
    fHistWreco_cutbased_Mt_plus->SetXTitle("M_{t,W} (GeV)");
    fHistWreco_cutbased_Mt_plus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_phi_plus = new TH1F("h_Wreco_cutbased_phi_plus", "W^{+} Azimuth Angle (all positive sign leptons)",
				128, -3.2, 3.2);
    fHistWreco_cutbased_phi_plus->SetXTitle("#phi (rad)");
    fHistWreco_cutbased_phi_plus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_pt_plus = new TH1F("h_Wreco_cutbased_pt_plus", "W^{+} Transverse Momentum (all positive sign leptons)", 200, 0, 200);
    fHistWreco_cutbased_pt_plus->SetXTitle("P_{t} (GeV)");
    fHistWreco_cutbased_pt_plus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_eta_plus = new TH1F("h_Wreco_cutbased_eta_plus", "W^{+} Pseudo-Rapidity (all positive sign leptons)",
				     50, -2.5, 2.5);
    fHistWreco_cutbased_eta_plus->SetXTitle("#eta");
    fHistWreco_cutbased_eta_plus->SetYTitle("Number of Entries");
    
    
    // Electron e+ channel
    fHistWreco_cutbased_Mt_e_plus = new TH1F("h_Wreco_cutbased_Mt_e_plus", "W^{+} Invariant Transverse Mass (e^{+} channel)",
			      200, 30, 130);
    fHistWreco_cutbased_Mt_e_plus->SetXTitle("M_{t,W} (GeV)");
    fHistWreco_cutbased_Mt_e_plus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_phi_e_plus = new TH1F("h_Wreco_cutbased_phi_e_plus", "W^{+} Azimuth Angle (e^{+} channel)",
				128, -3.2, 3.2);
    fHistWreco_cutbased_phi_e_plus->SetXTitle("#phi (rad)");
    fHistWreco_cutbased_phi_e_plus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_pt_e_plus = new TH1F("h_Wreco_cutbased_pt_e_plus", "W^{+} Transverse Momentum (e^{+} channel)",
			       200, 0, 200);
    fHistWreco_cutbased_pt_e_plus->SetXTitle("P_{t} (GeV)");
    fHistWreco_cutbased_pt_e_plus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_eta_e_plus = new TH1F("h_Wreco_cutbased_eta_e_plus", "W^{+} Pseudo-Rapidity (e^{+} channel)",
				     50, -2.5, 2.5);
    fHistWreco_cutbased_eta_e_plus->SetXTitle("#eta");
    fHistWreco_cutbased_eta_e_plus->SetYTitle("Number of Entries");


    // Muon mu+ channel
    fHistWreco_cutbased_Mt_mu_plus = new TH1F("h_Wreco_cutbased_Mt_mu_plus", "W^{+} Invariant Transverse Mass (#mu^{+} channel)", 200, 30, 130);
    fHistWreco_cutbased_Mt_mu_plus->SetXTitle("M_{t,W} (GeV)");
    fHistWreco_cutbased_Mt_mu_plus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_phi_mu_plus = new TH1F("h_Wreco_cutbased_phi_mu_plus", "W^{+} Azimuth Angle (#mu^{+} channel)",
				 128, -3.2, 3.2);
    fHistWreco_cutbased_phi_mu_plus->SetXTitle("#phi (rad)");
    fHistWreco_cutbased_phi_mu_plus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_pt_mu_plus = new TH1F("h_Wreco_cutbased_pt_mu_plus", "W^{+} Transverse Momentum (#mu^{+} channel)",
				200, 0, 200);
    fHistWreco_cutbased_pt_mu_plus->SetXTitle("P_{t} (GeV)");
    fHistWreco_cutbased_pt_mu_plus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_eta_mu_plus = new TH1F("h_Wreco_cutbased_eta_mu_plus", "W^{+} Pseudo-Rapidity (#mu^{+} channel)",
				     50, -2.5, 2.5);
    fHistWreco_cutbased_eta_mu_plus->SetXTitle("#eta");
    fHistWreco_cutbased_eta_mu_plus->SetYTitle("Number of Entries");

    
    // Tauon tau+ channel
    fHistWreco_cutbased_Mt_tau_plus = new TH1F("h_Wreco_cutbased_Mt_tau_plus", "W^{+} Invariant Transverse Mass (#tau^{+} channel)", 200, 30, 130);
    fHistWreco_cutbased_Mt_tau_plus->SetXTitle("M_{t,W} (GeV)");
    fHistWreco_cutbased_Mt_tau_plus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_phi_tau_plus = new TH1F("h_Wreco_cutbased_phi_tau_plus", "W^{+} Azimuth Angle (#tau^{+} channel)",
				 128, -3.2, 3.2);
    fHistWreco_cutbased_phi_tau_plus->SetXTitle("#phi (rad)");
    fHistWreco_cutbased_phi_tau_plus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_pt_tau_plus = new TH1F("h_Wreco_cutbased_pt_tau_plus", "W^{+} Transverse Momentum (#tau^{+} channel)", 200, 0, 200);
    fHistWreco_cutbased_pt_tau_plus->SetXTitle("P_{t} (GeV)");
    fHistWreco_cutbased_pt_tau_plus->SetYTitle("Number of Entries");
    fHistWreco_cutbased_eta_tau_plus = new TH1F("h_Wreco_cutbased_eta_tau_plus", "W^{+} Pseudo-Rapidity (#tau^{+} channel)",
				     50, -2.5, 2.5);
    fHistWreco_cutbased_eta_tau_plus->SetXTitle("#eta");
    fHistWreco_cutbased_eta_tau_plus->SetYTitle("Number of Entries");

    
    // ========================
    // Top-quark reconstruction
    // ========================
    gDirectory->cd("../..");
    gDirectory->mkdir("Top-reco", "Top reconstruction");
    gDirectory->cd("Top-reco");
    gDirectory->mkdir("Both", "Incl. t/#bar{t} reconstruction");
    gDirectory->cd("Both");
    

    // All leptons
    fHistTopReco_cutbased_Mt = new TH1F("h_treco_cutbased_Mt", "Top Transverse Invariant Mass (all leptons)",
			      200, 20, 220);
    fHistTopReco_cutbased_Mt->SetXTitle("M_{t,top} (GeV)");
    fHistTopReco_cutbased_Mt->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_phi = new TH1F("h_treco_cutbased_phi", "Top Azimuth Angle (all leptons)",
				128, -3.2, 3.2);
    fHistTopReco_cutbased_phi->SetXTitle("#phi (rad)");
    fHistTopReco_cutbased_phi->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_pt = new TH1F("h_treco_cutbased_pt", "Top Transverse Momentum (all leptons)",
			       200, 0, 200);
    fHistTopReco_cutbased_pt->SetXTitle("P_{t} (GeV)");
    fHistTopReco_cutbased_pt->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_eta = new TH1F("h_treco_cutbased_eta", "Top Pseudo-Rapidity (all leptons)",
				     50, -2.5, 2.5);
    fHistTopReco_cutbased_eta->SetXTitle("#eta");
    fHistTopReco_cutbased_eta->SetYTitle("Number of Entries");

    // Electron channel
    fHistTopReco_cutbased_Mt_e = new TH1F("h_treco_cutbased_Mt_e", "Top Transverse Invariant Mass (e channel)",
			      200, 20, 220);
    fHistTopReco_cutbased_Mt_e->SetXTitle("M_{t,top} (GeV)");
    fHistTopReco_cutbased_Mt_e->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_phi_e = new TH1F("h_treco_cutbased_phi_e", "Top Azimuth Angle (e channel)",
				128, -3.2, 3.2);
    fHistTopReco_cutbased_phi_e->SetXTitle("#phi (rad)");
    fHistTopReco_cutbased_phi_e->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_pt_e = new TH1F("h_treco_cutbased_pt_e", "Top Transverse Momentum (e channel)",
			       200, 0, 200);
    fHistTopReco_cutbased_pt_e->SetXTitle("P_{t} (GeV)");
    fHistTopReco_cutbased_pt_e->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_eta_e = new TH1F("h_treco_cutbased_eta_e", "Top Pseudo-Rapidity (e channel)",
				     50, -2.5, 2.5);
    fHistTopReco_cutbased_eta_e->SetXTitle("#eta");
    fHistTopReco_cutbased_eta_e->SetYTitle("Number of Entries");

    // Muon channel
    fHistTopReco_cutbased_Mt_mu = new TH1F("h_treco_cutbased_Mt_mu", "Top Transverse Invariant Mass (#mu channel)",
			      200, 20, 220);
    fHistTopReco_cutbased_Mt_mu->SetXTitle("M_{t,top} (GeV)");
    fHistTopReco_cutbased_Mt_mu->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_phi_mu = new TH1F("h_treco_cutbased_phi_mu", "Top Azimuth Angle (#mu channel)",
				128, -3.2, 3.2);
    fHistTopReco_cutbased_phi_mu->SetXTitle("#phi (rad)");
    fHistTopReco_cutbased_phi_mu->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_pt_mu = new TH1F("h_treco_cutbased_pt_mu", "Top Transverse Momentum (#mu channel)",
			       200, 0, 200);
    fHistTopReco_cutbased_pt_mu->SetXTitle("P_{t} (GeV)");
    fHistTopReco_cutbased_pt_mu->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_eta_mu = new TH1F("h_treco_cutbased_eta_mu", "Top Pseudo-Rapidity (#mu channel)",
				     50, -2.5, 2.5);
    fHistTopReco_cutbased_eta_mu->SetXTitle("#eta");
    fHistTopReco_cutbased_eta_mu->SetYTitle("Number of Entries");

    // Tau channel
    fHistTopReco_cutbased_Mt_tau = new TH1F("h_treco_cutbased_Mt_tau", "Top Transverse Invariant Mass (#tau channel)",
			      200, 20, 220);
    fHistTopReco_cutbased_Mt_tau->SetXTitle("M_{t,top} (GeV)");
    fHistTopReco_cutbased_Mt_tau->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_phi_tau = new TH1F("h_treco_cutbased_phi_tau", "Top Azimuth Angle (#tau channel)",
				128, -3.2, 3.2);
    fHistTopReco_cutbased_phi_tau->SetXTitle("#phi (rad)");
    fHistTopReco_cutbased_phi_tau->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_pt_tau = new TH1F("h_treco_cutbased_pt_tau", "Top Transverse Momentum (#tau channel)",
			       200, 0, 200);
    fHistTopReco_cutbased_pt_tau->SetXTitle("P_{t} (GeV)");
    fHistTopReco_cutbased_pt_tau->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_eta_tau = new TH1F("h_treco_cutbased_eta_tau", "Top Pseudo-Rapidity (#tau channel)",
				     50, -2.5, 2.5);
    fHistTopReco_cutbased_eta_tau->SetXTitle("#eta");
    fHistTopReco_cutbased_eta_tau->SetYTitle("Number of Entries");


    gDirectory->cd("..");


    gDirectory->mkdir("t", "t-quark reconstruction");
    gDirectory->cd("t");


    // All positive sign leptons
    fHistTopReco_cutbased_Mt_plus = new TH1F("h_treco_cutbased_Mt_plus", "T-Quark Transverse Invariant Mass (all positive sign leptons)",
			      200, 20, 220);
    fHistTopReco_cutbased_Mt_plus->SetXTitle("M_{t,top} (GeV)");
    fHistTopReco_cutbased_Mt_plus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_phi_plus = new TH1F("h_treco_cutbased_phi_plus", "T-Quark Azimuth Angle (all positive sign leptons)",
				128, -3.2, 3.2);
    fHistTopReco_cutbased_phi_plus->SetXTitle("#phi (rad)");
    fHistTopReco_cutbased_phi_plus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_pt_plus = new TH1F("h_treco_cutbased_pt_plus", "T-Quark Transverse Momentum (all positive sign leptons)",
			       200, 0, 200);
    fHistTopReco_cutbased_pt_plus->SetXTitle("P_{t} (GeV)");
    fHistTopReco_cutbased_pt_plus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_eta_plus = new TH1F("h_treco_cutbased_eta_plus", "T-Quark Pseudo-Rapidity (all positive sign leptons)",
				     50, -2.5, 2.5);
    fHistTopReco_cutbased_eta_plus->SetXTitle("#eta");
    fHistTopReco_cutbased_eta_plus->SetYTitle("Number of Entries");
    

    // Electron e+ channel
    fHistTopReco_cutbased_Mt_e_plus = new TH1F("h_treco_cutbased_Mt_e_plus", "T-Quark Transverse Invariant Mass (e^{+} channel)",
			      200, 20, 220);
    fHistTopReco_cutbased_Mt_e_plus->SetXTitle("M_{t,top} (GeV)");
    fHistTopReco_cutbased_Mt_e_plus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_phi_e_plus = new TH1F("h_treco_cutbased_phi_e_plus", "T-Quark Azimuth Angle (e^{+} channel)",
				128, -3.2, 3.2);
    fHistTopReco_cutbased_phi_e_plus->SetXTitle("#phi (rad)");
    fHistTopReco_cutbased_phi_e_plus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_pt_e_plus = new TH1F("h_treco_cutbased_pt_e_plus", "T-Quark Transverse Momentum (e^{+} channel)",
			       200, 0, 200);
    fHistTopReco_cutbased_pt_e_plus->SetXTitle("P_{t} (GeV)");
    fHistTopReco_cutbased_pt_e_plus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_eta_e_plus = new TH1F("h_treco_cutbased_eta_e_plus", "T-Quark Pseudo-Rapidity (e^{+} channel)",
				     50, -2.5, 2.5);
    fHistTopReco_cutbased_eta_e_plus->SetXTitle("#eta");
    fHistTopReco_cutbased_eta_e_plus->SetYTitle("Number of Entries");

    // Muon mu+ channel
    fHistTopReco_cutbased_Mt_mu_plus = new TH1F("h_treco_cutbased_Mt_mu_plus", "T-Quark Transverse Invariant Mass (#mu^{+} channel)",
			      200, 20, 220);
    fHistTopReco_cutbased_Mt_mu_plus->SetXTitle("M_{t,top} (GeV)");
    fHistTopReco_cutbased_Mt_mu_plus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_phi_mu_plus = new TH1F("h_treco_cutbased_phi_mu_plus", "T-Quark Azimuth Angle (#mu^{+} channel)",
				128, -3.2, 3.2);
    fHistTopReco_cutbased_phi_mu_plus->SetXTitle("#phi (rad)");
    fHistTopReco_cutbased_phi_mu_plus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_pt_mu_plus = new TH1F("h_treco_cutbased_pt_mu_plus", "T-Quark Transverse Momentum (#mu^{+} channel)",
			       200, 0, 200);
    fHistTopReco_cutbased_pt_mu_plus->SetXTitle("P_{t} (GeV)");
    fHistTopReco_cutbased_pt_mu_plus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_eta_mu_plus = new TH1F("h_treco_cutbased_eta_mu_plus", "T-Quark Pseudo-Rapidity (#mu^{+} channel)",
				     50, -2.5, 2.5);
    fHistTopReco_cutbased_eta_mu_plus->SetXTitle("#eta");
    fHistTopReco_cutbased_eta_mu_plus->SetYTitle("Number of Entries");

    // Tauon tau+ channel
    fHistTopReco_cutbased_Mt_tau_plus = new TH1F("h_treco_cutbased_Mt_tau_plus", "T-Quark Transverse Invariant Mass (#tau^{+} channel)",
			      200, 20, 220);
    fHistTopReco_cutbased_Mt_tau_plus->SetXTitle("M_{t,top} (GeV)");
    fHistTopReco_cutbased_Mt_tau_plus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_phi_tau_plus = new TH1F("h_treco_cutbased_phi_tau_plus", "T-Quark Azimuth Angle (#tau^{+} channel)",
				128, -3.2, 3.2);
    fHistTopReco_cutbased_phi_tau_plus->SetXTitle("#phi (rad)");
    fHistTopReco_cutbased_phi_tau_plus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_pt_tau_plus = new TH1F("h_treco_cutbased_pt_tau_plus", "T-Quark Transverse Momentum (#tau^{+} channel)",
			       200, 0, 200);
    fHistTopReco_cutbased_pt_tau_plus->SetXTitle("P_{t} (GeV)");
    fHistTopReco_cutbased_pt_tau_plus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_eta_tau_plus = new TH1F("h_treco_cutbased_eta_tau_plus", "T-Quark Pseudo-Rapidity (#tau^{+} channel)",
				     50, -2.5, 2.5);
    fHistTopReco_cutbased_eta_tau_plus->SetXTitle("#eta");
    fHistTopReco_cutbased_eta_tau_plus->SetYTitle("Number of Entries");


    
    
    gDirectory->cd("..");

    
    gDirectory->mkdir("tbar", "#bar{t}-quark reconstruction");
    gDirectory->cd("tbar");

    
    // All negative sign leptons
    fHistTopReco_cutbased_Mt_minus = new TH1F("h_treco_cutbased_Mt_minus", "#bar{T}-Quark Transverse Invariant Mass (all negative sign leptons)",
			      200, 20, 220);
    fHistTopReco_cutbased_Mt_minus->SetXTitle("M_{t,top} (GeV)");
    fHistTopReco_cutbased_Mt_minus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_phi_minus = new TH1F("h_treco_cutbased_phi_minus", "#bar{T}-Quark Azimuth Angle (all negative sign leptons)",
				128, -3.2, 3.2);
    fHistTopReco_cutbased_phi_minus->SetXTitle("#phi (rad)");
    fHistTopReco_cutbased_phi_minus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_pt_minus = new TH1F("h_treco_cutbased_pt_minus", "#bar{T}-Quark Transverse Momentum (all negative sign leptons)",
			       200, 0, 200);
    fHistTopReco_cutbased_pt_minus->SetXTitle("P_{t} (GeV)");
    fHistTopReco_cutbased_pt_minus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_eta_minus = new TH1F("h_treco_cutbased_eta_minus", "T-Quark Pseudo-Rapidity (all negative sign leptons)",
				     50, -2.5, 2.5);
    fHistTopReco_cutbased_eta_minus->SetXTitle("#eta");
    fHistTopReco_cutbased_eta_minus->SetYTitle("Number of Entries");

    // Electron e- channel
    fHistTopReco_cutbased_Mt_e_minus = new TH1F("h_treco_cutbased_Mt_e_minus", "#bar{T}-Quark Transverse Invariant Mass (e^{-} channel)",
			      200, 20, 220);
    fHistTopReco_cutbased_Mt_e_minus->SetXTitle("M_{t,top} (GeV)");
    fHistTopReco_cutbased_Mt_e_minus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_phi_e_minus = new TH1F("h_treco_cutbased_phi_e_minus", "#bar{T}-Quark Azimuth Angle (e^{-} channel)",
				128, -3.2, 3.2);
    fHistTopReco_cutbased_phi_e_minus->SetXTitle("#phi (rad)");
    fHistTopReco_cutbased_phi_e_minus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_pt_e_minus = new TH1F("h_treco_cutbased_pt_e_minus", "#bar{T}-Quark Transverse Momentum (e^{-} channel)",
			       200, 0, 200);
    fHistTopReco_cutbased_pt_e_minus->SetXTitle("P_{t} (GeV)");
    fHistTopReco_cutbased_pt_e_minus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_eta_e_minus = new TH1F("h_treco_cutbased_eta_e_minus", "#bar{T}-Quark Pseudo-Rapidity (e^{-} channel)",
				     50, -2.5, 2.5);
    fHistTopReco_cutbased_eta_e_minus->SetXTitle("#eta");
    fHistTopReco_cutbased_eta_e_minus->SetYTitle("Number of Entries");

    // Muon mu- channel
    fHistTopReco_cutbased_Mt_mu_minus = new TH1F("h_treco_cutbased_Mt_mu_minus", "#bar{T}-Quark Transverse Invariant Mass (#mu^{-} channel)",
			      200, 20, 220);
    fHistTopReco_cutbased_Mt_mu_minus->SetXTitle("M_{t,top} (GeV)");
    fHistTopReco_cutbased_Mt_mu_minus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_phi_mu_minus = new TH1F("h_treco_cutbased_phi_mu_minus", "#bar{T}-Quark Azimuth Angle (#mu^{-} channel)",
				128, -3.2, 3.2);
    fHistTopReco_cutbased_phi_mu_minus->SetXTitle("#phi (rad)");
    fHistTopReco_cutbased_phi_mu_minus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_pt_mu_minus = new TH1F("h_treco_cutbased_pt_mu_minus", "#bar{T}-Quark Transverse Momentum (#mu^{-} channel)",
			       200, 0, 200);
    fHistTopReco_cutbased_pt_mu_minus->SetXTitle("P_{t} (GeV)");
    fHistTopReco_cutbased_pt_mu_minus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_eta_mu_minus = new TH1F("h_treco_cutbased_eta_mu_minus", "#bar{T}-Quark Pseudo-Rapidity (#mu^{-} channel)",
				     50, -2.5, 2.5);
    fHistTopReco_cutbased_eta_mu_minus->SetXTitle("#eta");
    fHistTopReco_cutbased_eta_mu_minus->SetYTitle("Number of Entries");

    // Tauon tau- channel
    fHistTopReco_cutbased_Mt_tau_minus = new TH1F("h_treco_cutbased_Mt_tau_minus", "#bar{T}-Quark Transverse Invariant Mass (#tau^{-} channel)",
			      200, 20, 220);
    fHistTopReco_cutbased_Mt_tau_minus->SetXTitle("M_{t,top} (GeV)");
    fHistTopReco_cutbased_Mt_tau_minus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_phi_tau_minus = new TH1F("h_treco_cutbased_phi_tau_minus", "#bar{T}-Quark Azimuth Angle (#tau^{-} channel)",
				128, -3.2, 3.2);
    fHistTopReco_cutbased_phi_tau_minus->SetXTitle("#phi (rad)");
    fHistTopReco_cutbased_phi_tau_minus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_pt_tau_minus = new TH1F("h_treco_cutbased_pt_tau_minus", "#bar{T}-Quark Transverse Momentum (#tau^{-} channel)",
			       200, 0, 200);
    fHistTopReco_cutbased_pt_tau_minus->SetXTitle("P_{t} (GeV)");
    fHistTopReco_cutbased_pt_tau_minus->SetYTitle("Number of Entries");
    fHistTopReco_cutbased_eta_tau_minus = new TH1F("h_treco_cutbased_eta_tau_minus", "#bar{T}-Quark Pseudo-Rapidity (#tau^{-} channel)",
				     50, -2.5, 2.5);
    fHistTopReco_cutbased_eta_tau_minus->SetXTitle("#eta");
    fHistTopReco_cutbased_eta_tau_minus->SetYTitle("Number of Entries");

    gDirectory->cd("../../..");
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::BookCutBasedLeptonHistograms() {
    //
    // Book lepton histograms for top candidate events in cut-based mode
    //
    gDirectory->mkdir("Leptons","Leptons used in reconstruction");
    gDirectory->cd("Leptons");

    // ===========
    // All leptons
    // ===========
    gDirectory->mkdir("All_leptons");
    gDirectory->cd("All_leptons");
    
    fHistCutBasedCandidateSignalLepton_pt = new TH1F("h_sig_lepton_pt", "Signal Lepton Transverse Momentum",
				    200, 0, 200);
    fHistCutBasedCandidateSignalLepton_pt->SetXTitle("P_{t} (GeV)");
    fHistCutBasedCandidateSignalLepton_pt->SetYTitle("Number of Entries");
    fHistCutBasedCandidateSignalLepton_eta = new TH1F("h_sig_lepton_eta", "Signal Lepton Pseudo-Rapidity",
				     50, -2.5, 2.5);
    fHistCutBasedCandidateSignalLepton_eta->SetXTitle("#eta");
    fHistCutBasedCandidateSignalLepton_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateSignalLepton_phi = new TH1F("h_sig_lepton_phi", "Signal Lepton Azimuth",
				     128, -3.2, 3.2);
    fHistCutBasedCandidateSignalLepton_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateSignalLepton_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // =========
    // Electrons
    // =========
    gDirectory->mkdir("Electron");
    gDirectory->cd("Electron");


    
    //****************************************
    // Number of signal-electrons by author
    //****************************************
    fHistCutBasedCandidateSignalElectron_author = new TH1F("h_signal_electron_author",
		  "Signal Electron Author",
		  (AtlEMShower::fgNAuthors + 1), 0, (AtlEMShower::fgNAuthors + 1));
    fHistCutBasedCandidateSignalElectron_author->SetYTitle("Number of Entries");
    TAxis *axis = fHistCutBasedCandidateSignalElectron_author->GetXaxis();
    for ( Int_t i = 0; i < AtlEMShower::fgNAuthors; i++ )
	axis->SetBinLabel(i+1, AtlEMShower::fgAuthorNames[i]);
    axis->SetBinLabel(6,"HighPt and Soft");

    // Both charges
    gDirectory->mkdir("both");
    gDirectory->cd("both");

    fHistCutBasedCandidateSignalElectron_pt = new TH1F("h_sig_electron_pt", "Signal e Transverse Momentum",
				      200, 0, 200);
    fHistCutBasedCandidateSignalElectron_pt->SetXTitle("P_{t} (GeV)");
    fHistCutBasedCandidateSignalElectron_pt->SetYTitle("Number of Entries");
    fHistCutBasedCandidateSignalElectron_eta = new TH1F("h_sig_electron_eta", "Signal e Pseudo-Rapidity",
				       50, -2.5, 2.5);
    fHistCutBasedCandidateSignalElectron_eta->SetXTitle("#eta");
    fHistCutBasedCandidateSignalElectron_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateSignalElectron_phi = new TH1F("h_sig_electron_phi", "Signal e Azimuth",
				       128, -3.2, 3.2);
    fHistCutBasedCandidateSignalElectron_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateSignalElectron_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // e+
    gDirectory->mkdir("e+");
    gDirectory->cd("e+");

    fHistCutBasedCandidateSignalEplus_pt = new TH1F("h_sig_eplus_pt", "Signal e^{+} Transverse Momentum",
				   200, 0, 200);
    fHistCutBasedCandidateSignalEplus_pt->SetXTitle("P_{t} (GeV)");
    fHistCutBasedCandidateSignalEplus_pt->SetYTitle("Number of Entries");
    fHistCutBasedCandidateSignalEplus_eta = new TH1F("h_sig_eplus_eta", "Signal e^{+} Pseudo-Rapidity",
				    50, -2.5, 2.5);
    fHistCutBasedCandidateSignalEplus_eta->SetXTitle("#eta");
    fHistCutBasedCandidateSignalEplus_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateSignalEplus_phi = new TH1F("h_sig_eplus_phi", "Signal e^{+} Azimuth",
				    128, -3.2, 3.2);
    fHistCutBasedCandidateSignalEplus_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateSignalEplus_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // e-
    gDirectory->mkdir("e-");
    gDirectory->cd("e-");

    fHistCutBasedCandidateSignalEminus_pt = new TH1F("h_sig_eminus_pt", "Signal e^{-} Transverse Momentum",
				   200, 0, 200);
    fHistCutBasedCandidateSignalEminus_pt->SetXTitle("P_{t} (GeV)");
    fHistCutBasedCandidateSignalEminus_pt->SetYTitle("Number of Entries");
    fHistCutBasedCandidateSignalEminus_eta = new TH1F("h_sig_eminus_eta", "Signal e^{-} Pseudo-Rapidity",
				     50, -2.5, 2.5);
    fHistCutBasedCandidateSignalEminus_eta->SetXTitle("#eta");
    fHistCutBasedCandidateSignalEminus_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateSignalEminus_phi = new TH1F("h_sig_eminus_phi", "Signal e^{-} Azimuth",
				    128, -3.2, 3.2);
    fHistCutBasedCandidateSignalEminus_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateSignalEminus_phi->SetYTitle("Number of Entries");
    
    gDirectory->cd("../..");

    // =====
    // Muons
    // =====
    gDirectory->mkdir("Muon");
    gDirectory->cd("Muon");
    
    // Both charges
    gDirectory->mkdir("both");
    gDirectory->cd("both");

    fHistCutBasedCandidateSignalMuon_pt = new TH1F("h_sig_muon_pt", "Signal #mu Transverse Momentum",
				  200, 0, 200);
    fHistCutBasedCandidateSignalMuon_pt->SetXTitle("P_{t} (GeV)");
    fHistCutBasedCandidateSignalMuon_pt->SetYTitle("Number of Entries");
    fHistCutBasedCandidateSignalMuon_eta = new TH1F("h_sig_muon_eta", "Signal #mu Pseudo-Rapidity",
				   50, -2.5, 2.5);
    fHistCutBasedCandidateSignalMuon_eta->SetXTitle("#eta");
    fHistCutBasedCandidateSignalMuon_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateSignalMuon_phi = new TH1F("h_sig_muon_phi", "Signal #mu Azimuth",
				   128, -3.2, 3.2);
    fHistCutBasedCandidateSignalMuon_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateSignalMuon_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // mu+
    gDirectory->mkdir("mu+");
    gDirectory->cd("mu+");

    fHistCutBasedCandidateSignalMuplus_pt = new TH1F("h_sig_muplus_pt", "Signal #mu^{+} Transverse Momentum",
				    200, 0, 200);
    fHistCutBasedCandidateSignalMuplus_pt->SetXTitle("P_{t} (GeV)");
    fHistCutBasedCandidateSignalMuplus_pt->SetYTitle("Number of Entries");
    fHistCutBasedCandidateSignalMuplus_eta = new TH1F("h_sig_muplus_eta", "Signal #mu^{+} Pseudo-Rapidity",
				     50, -2.5, 2.5);
    fHistCutBasedCandidateSignalMuplus_eta->SetXTitle("#eta");
    fHistCutBasedCandidateSignalMuplus_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateSignalMuplus_phi = new TH1F("h_sig_muplus_phi", "Signal #mu^{+} Azimuth",
				     128, -3.2, 3.2);
    fHistCutBasedCandidateSignalMuplus_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateSignalMuplus_phi->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    // mu-
    gDirectory->mkdir("mu-");
    gDirectory->cd("mu-");

    fHistCutBasedCandidateSignalMuminus_pt = new TH1F("h_sig_muminus_pt", "Signal #mu^{-} Transverse Momentum",
				   200, 0, 200);
    fHistCutBasedCandidateSignalMuminus_pt->SetXTitle("P_{t} (GeV)");
    fHistCutBasedCandidateSignalMuminus_pt->SetYTitle("Number of Entries");
    fHistCutBasedCandidateSignalMuminus_eta = new TH1F("h_sig_muminus_eta", "Signal #mu^{-} Pseudo-Rapidity",
				     50, -2.5, 2.5);
    fHistCutBasedCandidateSignalMuminus_eta->SetXTitle("#eta");
    fHistCutBasedCandidateSignalMuminus_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateSignalMuminus_phi = new TH1F("h_sig_muminus_phi", "Signal #mu^{-} Azimuth",
				    128, -3.2, 3.2);
    fHistCutBasedCandidateSignalMuminus_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateSignalMuminus_phi->SetYTitle("Number of Entries");

    gDirectory->cd("../..");

    // ===
    // Tau
    // ===
    gDirectory->mkdir("Tau");
    gDirectory->cd("Tau");
    
    // Both charges
    gDirectory->mkdir("both");
    gDirectory->cd("both");

    fHistCutBasedCandidateSignalTau_pt = new TH1F("h_sig_tau_pt", "Signal #tau Transverse Momentum",
				  200, 0, 200);
    fHistCutBasedCandidateSignalTau_pt->SetXTitle("P_{t} (GeV)");
    fHistCutBasedCandidateSignalTau_pt->SetYTitle("Number of Entries");
    fHistCutBasedCandidateSignalTau_eta = new TH1F("h_sig_tau_eta", "Signal #tau Pseudo-Rapidity",
				   50, -2.5, 2.5);
    fHistCutBasedCandidateSignalTau_eta->SetXTitle("#eta");
    fHistCutBasedCandidateSignalTau_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateSignalTau_phi = new TH1F("h_sig_tau_phi", "Signal #tau Azimuth",
				   128, -3.2, 3.2);
    fHistCutBasedCandidateSignalTau_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateSignalTau_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // tau+
    gDirectory->mkdir("tau+");
    gDirectory->cd("tau+");

    fHistCutBasedCandidateSignalTauplus_pt = new TH1F("h_sig_tauplus_pt", "Signal #tau^{+} Transverse Momentum",
				    200, 0, 200);
    fHistCutBasedCandidateSignalTauplus_pt->SetXTitle("P_{t} (GeV)");
    fHistCutBasedCandidateSignalTauplus_pt->SetYTitle("Number of Entries");
    fHistCutBasedCandidateSignalTauplus_eta = new TH1F("h_sig_tauplus_eta", "Signal #tau^{+} Pseudo-Rapidity",
				     50, -2.5, 2.5);
    fHistCutBasedCandidateSignalTauplus_eta->SetXTitle("#eta");
    fHistCutBasedCandidateSignalTauplus_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateSignalTauplus_phi = new TH1F("h_sig_tauplus_phi", "Signal #tau^{+} Azimuth",
				     128, -3.2, 3.2);
    fHistCutBasedCandidateSignalTauplus_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateSignalTauplus_phi->SetYTitle("Number of Entries");
    
    gDirectory->cd("..");
    
    // tau-
    gDirectory->mkdir("tau-");
    gDirectory->cd("tau-");

    fHistCutBasedCandidateSignalTauminus_pt = new TH1F("h_sig_tauminus_pt", "Signal #tau^{-} Transverse Momentum",
				   200, 0, 200);
    fHistCutBasedCandidateSignalTauminus_pt->SetXTitle("P_{t} (GeV)");
    fHistCutBasedCandidateSignalTauminus_pt->SetYTitle("Number of Entries");
    fHistCutBasedCandidateSignalTauminus_eta = new TH1F("h_sig_tauminus_eta", "Signal #tau^{-} Pseudo-Rapidity",
				     50, -2.5, 2.5);
    fHistCutBasedCandidateSignalTauminus_eta->SetXTitle("#eta");
    fHistCutBasedCandidateSignalTauminus_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateSignalTauminus_phi = new TH1F("h_sig_tauminus_phi", "Signal #tau^{-} Azimuth",
				    128, -3.2, 3.2);
    fHistCutBasedCandidateSignalTauminus_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateSignalTauminus_phi->SetYTitle("Number of Entries");
    
    gDirectory->cd("../../..");
}

//____________________________________________________________________

void AtlSgTop_tChannelFinder::BookCutBasedJetHistograms() {
    //
    // Book jet histograms for top candidate events in cut-based mode
    //
    gDirectory->mkdir("Jets","Jets used in reconstruction");
    gDirectory->cd("Jets");

    // ======
    // Jets
    // ======
    gDirectory->mkdir("Jet");
    gDirectory->cd("Jet");

    // All jets
    fHistCutBasedCandidateJet_n = new TH1F("h_jet_n", "No. of all jets",
			   10, 0, 10);
    fHistCutBasedCandidateJet_n->SetXTitle("N_{jet}");
    fHistCutBasedCandidateJet_n->SetYTitle("Number of Entries");
    fHistCutBasedCandidateJet_Et = new TH1F("h_jet_Et", "Transverse Energy of all jets",
			    200, 0, 200);
    fHistCutBasedCandidateJet_Et->SetXTitle("E_{t} (GeV)");
    fHistCutBasedCandidateJet_Et->SetYTitle("Number of Entries");
    fHistCutBasedCandidateJet_eta = new TH1F("h_jet_eta", "Pseudo-Rapidity of all jets",
			     50, -5, 5);
    fHistCutBasedCandidateJet_eta->SetXTitle("#eta");
    fHistCutBasedCandidateJet_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateJet_phi = new TH1F("h_jet_phi", "Azimuth of all jets",
			     128, -3.2, 3.2);
    fHistCutBasedCandidateJet_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateJet_phi->SetYTitle("Number of Entries");

    // Leading jet
    fHistCutBasedCandidateJet1_Et = new TH1F("h_jet1_Et", "Transverse Energy of leading jet",
			    200, 0, 200);
    fHistCutBasedCandidateJet1_Et->SetXTitle("E_{t} (GeV)");
    fHistCutBasedCandidateJet1_Et->SetYTitle("Number of Entries");
    fHistCutBasedCandidateJet1_eta = new TH1F("h_jet1_eta", "Pseudo-Rapidity of leading jet",
			     50, -5, 5);
    fHistCutBasedCandidateJet1_eta->SetXTitle("#eta");
    fHistCutBasedCandidateJet1_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateJet1_phi = new TH1F("h_jet1_phi", "Azimuth of leading jet",
			     128, -3.2, 3.2);
    fHistCutBasedCandidateJet1_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateJet1_phi->SetYTitle("Number of Entries");

    // 2nd Leading jet
    fHistCutBasedCandidateJet2_Et = new TH1F("h_jet2_Et", "Transverse Energy of 2nd leading jet",
			    200, 0, 200);
    fHistCutBasedCandidateJet2_Et->SetXTitle("E_{t} (GeV)");
    fHistCutBasedCandidateJet2_Et->SetYTitle("Number of Entries");
    fHistCutBasedCandidateJet2_eta = new TH1F("h_jet2_eta", "Pseudo-Rapidity of 2nd leading jet",
			     50, -5, 5);
    fHistCutBasedCandidateJet2_eta->SetXTitle("#eta");
    fHistCutBasedCandidateJet2_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateJet2_phi = new TH1F("h_jet2_phi", "Azimuth of 2nd leading jet",
			     128, -3.2, 3.2);
    fHistCutBasedCandidateJet2_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateJet2_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");


    // ======
    // B-Jets
    // ======
    gDirectory->mkdir("B-jet");
    gDirectory->cd("B-jet");

    // All b-jets
    fHistCutBasedCandidateBJet_n = new TH1F("h_bjet_n", "No. of all b-jets",
			   10, 0, 10);
    fHistCutBasedCandidateBJet_n->SetXTitle("N_{b-jet}");
    fHistCutBasedCandidateBJet_n->SetYTitle("Number of Entries");
    fHistCutBasedCandidateBJet_Et = new TH1F("h_bjet_Et", "Transverse Energy of all b-jets",
			    200, 0, 200);
    fHistCutBasedCandidateBJet_Et->SetXTitle("E_{t} (GeV)");
    fHistCutBasedCandidateBJet_Et->SetYTitle("Number of Entries");
    fHistCutBasedCandidateBJet_eta = new TH1F("h_bjet_eta", "Pseudo-Rapidity of all b-jets",
			     50, -2.5, 2.5);
    fHistCutBasedCandidateBJet_eta->SetXTitle("#eta");
    fHistCutBasedCandidateBJet_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateBJet_phi = new TH1F("h_bjet_phi", "Azimuth of all b-jets",
			     128, -3.2, 3.2);
    fHistCutBasedCandidateBJet_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateBJet_phi->SetYTitle("Number of Entries");

    // Leading b-jet
    fHistCutBasedCandidateBJet1_Et = new TH1F("h_bjet1_Et", "Transverse Energy of leading b-jet",
			    200, 0, 200);
    fHistCutBasedCandidateBJet1_Et->SetXTitle("E_{t} (GeV)");
    fHistCutBasedCandidateBJet1_Et->SetYTitle("Number of Entries");
    fHistCutBasedCandidateBJet1_eta = new TH1F("h_bjet1_eta", "Pseudo-Rapidity of leading b-jet",
			     50, -2.5, 2.5);
    fHistCutBasedCandidateBJet1_eta->SetXTitle("#eta");
    fHistCutBasedCandidateBJet1_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateBJet1_phi = new TH1F("h_bjet1_phi", "Azimuth of leading b-jet",
			     128, -3.2, 3.2);
    fHistCutBasedCandidateBJet1_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateBJet1_phi->SetYTitle("Number of Entries");

    // 2nd Leading b-jet
    fHistCutBasedCandidateBJet2_Et = new TH1F("h_bjet2_Et", "Transverse Energy of 2nd leading b-jet",
			    200, 0, 200);
    fHistCutBasedCandidateBJet2_Et->SetXTitle("E_{t} (GeV)");
    fHistCutBasedCandidateBJet2_Et->SetYTitle("Number of Entries");
    fHistCutBasedCandidateBJet2_eta = new TH1F("h_bjet2_eta", "Pseudo-Rapidity of 2nd leading b-jet",
			     50, -2.5, 2.5);
    fHistCutBasedCandidateBJet2_eta->SetXTitle("#eta");
    fHistCutBasedCandidateBJet2_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateBJet2_phi = new TH1F("h_bjet2_phi", "Azimuth of 2nd leading b-jet",
			     128, -3.2, 3.2);
    fHistCutBasedCandidateBJet2_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateBJet2_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");


    // ======
    // Non-B-Jets
    // ======
    gDirectory->mkdir("Non-B-jet");
    gDirectory->cd("Non-B-jet");

    // All non-b-jets
    fHistCutBasedCandidateNonBJet_n = new TH1F("h_nonbjet_n", "No. of all non-b-jets",
			   10, 0, 10);
    fHistCutBasedCandidateNonBJet_n->SetXTitle("N_{non-b-jet}");
    fHistCutBasedCandidateNonBJet_n->SetYTitle("Number of Entries");
    fHistCutBasedCandidateNonBJet_Et = new TH1F("h_nonbjet_Et", "Transverse Energy of all non-b-jets",
			    200, 0, 200);
    fHistCutBasedCandidateNonBJet_Et->SetXTitle("E_{t} (GeV)");
    fHistCutBasedCandidateNonBJet_Et->SetYTitle("Number of Entries");
    fHistCutBasedCandidateNonBJet_eta = new TH1F("h_nonbjet_eta", "Pseudo-Rapidity of all non-b-jets",
			     50, -5, 5);
    fHistCutBasedCandidateNonBJet_eta->SetXTitle("#eta");
    fHistCutBasedCandidateNonBJet_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateNonBJet_phi = new TH1F("h_nonbjet_phi", "Azimuth of all non-b-jets",
			     128, -3.2, 3.2);
    fHistCutBasedCandidateNonBJet_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateNonBJet_phi->SetYTitle("Number of Entries");

    // Leading non-b-jet
    fHistCutBasedCandidateNonBJet1_Et = new TH1F("h_nonbjet1_Et", "Transverse Energy of leading non-b-jet",
			    200, 0, 200);
    fHistCutBasedCandidateNonBJet1_Et->SetXTitle("E_{t} (GeV)");
    fHistCutBasedCandidateNonBJet1_Et->SetYTitle("Number of Entries");
    fHistCutBasedCandidateNonBJet1_eta = new TH1F("h_nonbjet1_eta", "Pseudo-Rapidity of leading non-b-jet",
			     50, -5, 5);
    fHistCutBasedCandidateNonBJet1_eta->SetXTitle("#eta");
    fHistCutBasedCandidateNonBJet1_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateNonBJet1_phi = new TH1F("h_nonbjet1_phi", "Azimuth of leading non-b-jet",
			     128, -3.2, 3.2);
    fHistCutBasedCandidateNonBJet1_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateNonBJet1_phi->SetYTitle("Number of Entries");

    // 2nd Leading non-b-jet
    fHistCutBasedCandidateNonBJet2_Et = new TH1F("h_nonbjet2_Et", "Transverse Energy of 2nd leading non-b-jet",
			    200, 0, 200);
    fHistCutBasedCandidateNonBJet2_Et->SetXTitle("E_{t} (GeV)");
    fHistCutBasedCandidateNonBJet2_Et->SetYTitle("Number of Entries");
    fHistCutBasedCandidateNonBJet2_eta = new TH1F("h_nonbjet2_eta", "Pseudo-Rapidity of 2nd leading non-b-jet",
			     50, -5, 5);
    fHistCutBasedCandidateNonBJet2_eta->SetXTitle("#eta");
    fHistCutBasedCandidateNonBJet2_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateNonBJet2_phi = new TH1F("h_nonbjet2_phi", "Azimuth of 2nd leading non-b-jet",
			     128, -3.2, 3.2);
    fHistCutBasedCandidateNonBJet2_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateNonBJet2_phi->SetYTitle("Number of Entries");

    gDirectory->cd("..");
    
    // ============
    // Forward jets
    // ============
    gDirectory->mkdir("Fwd");
    gDirectory->cd("Fwd");

    // All forward jets
    fHistCutBasedCandidateFwdJet_n = new TH1F("h_fwdjet_n", "No. of all forward jets",
			     10, 0, 10);
    fHistCutBasedCandidateFwdJet_n->SetXTitle("N_{fwd-jet}");
    fHistCutBasedCandidateFwdJet_n->SetYTitle("Number of Entries");
    fHistCutBasedCandidateFwdJet_Et = new TH1F("h_fwdjet_Et", "Transverse Energy of all forward jets",
			      200, 0, 200);
    fHistCutBasedCandidateFwdJet_Et->SetXTitle("E_{t} (GeV)");
    fHistCutBasedCandidateFwdJet_Et->SetYTitle("Number of Entries");
    fHistCutBasedCandidateFwdJet_eta = new TH1F("h_fwdjet_eta", "Pseudo-Rapidity of all forward jets",
			       100, -5, 5);
    fHistCutBasedCandidateFwdJet_eta->SetXTitle("#eta");
    fHistCutBasedCandidateFwdJet_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateFwdJet_phi = new TH1F("h_fwdjet_phi", "Azimuth of all forward jets",
			       128, -3.2, 3.2);
    fHistCutBasedCandidateFwdJet_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateFwdJet_phi->SetYTitle("Number of Entries");

    // Leading  forward jet
    fHistCutBasedCandidateFwdJet1_Et = new TH1F("h_fwdjet1_Et", "Transverse Energy of leading forward jet",
			       200, 0, 200);
    fHistCutBasedCandidateFwdJet1_Et->SetXTitle("E_{t} (GeV)");
    fHistCutBasedCandidateFwdJet1_Et->SetYTitle("Number of Entries");
    fHistCutBasedCandidateFwdJet1_eta = new TH1F("h_fwdjet1_eta", "Pseudo-Rapidity of leading forward jet",
				100, -5, 5);
    fHistCutBasedCandidateFwdJet1_eta->SetXTitle("#eta");
    fHistCutBasedCandidateFwdJet1_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateFwdJet1_phi = new TH1F("h_fwdjet1_phi", "Azimuth of leading forward jet",
				128, -3.2, 3.2);
    fHistCutBasedCandidateFwdJet1_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateFwdJet1_phi->SetYTitle("Number of Entries");

    // 2nd Leading forward jet
    fHistCutBasedCandidateFwdJet2_Et = new TH1F("h_fwdjet2_Et", "Transverse Energy of 2nd leading forward jet",
			       200, 0, 200);
    fHistCutBasedCandidateFwdJet2_Et->SetXTitle("E_{t} (GeV)");
    fHistCutBasedCandidateFwdJet2_Et->SetYTitle("Number of Entries");
    fHistCutBasedCandidateFwdJet2_eta = new TH1F("h_fwdjet2_eta", "Pseudo-Rapidity of 2nd leading forward jet",
				100, -5, 5);
    fHistCutBasedCandidateFwdJet2_eta->SetXTitle("#eta");
    fHistCutBasedCandidateFwdJet2_eta->SetYTitle("Number of Entries");
    fHistCutBasedCandidateFwdJet2_phi = new TH1F("h_fwdjet2_phi", "Azimuth of 2nd leading forward jet",
				128, -3.2, 3.2);
    fHistCutBasedCandidateFwdJet2_phi->SetXTitle("#phi (rad)");
    fHistCutBasedCandidateFwdJet2_phi->SetYTitle("Number of Entries");

    gDirectory->cd("../..");
}


// ----------------------------------------------------------------------------

double AtlSgTop_tChannelFinder::Neutrino_Pz( double nu_px, double nu_py,
		    double l_px, double l_py, double l_pz, double l_e,
		    double& scf,double nu_pz_true) {
    //Neutrino_Pz
    double solution1;
    double solution2;
    
    // Find the two possible solutions:
    Neutrino_Pz_Base( nu_px, nu_py, l_px, l_py, l_pz, l_e, // input arguments
		      scf, solution1, solution2);          // return variables
    
    // Now choose the lower one of the two
    //if(nu_pz_true>100)
    //cout<<"possible solutions: "<<solution1<<" and "<<solution2;
    //cout<<" and truth is "<<nu_pz_true<<" and pz_l "<<l_pz<<endl;
    // choose solution with smallest |l_pz| a la Run I
    // alternatives don't work !!
    double nu_pz = fabs (solution1) < fabs (solution2) ? solution1 : solution2 ;
    
    return nu_pz;
    
} 

// ----------------------------------------------------------------------------

void AtlSgTop_tChannelFinder::Neutrino_Pz_Base( double nu_px, double nu_py,
		       double l_px, double l_py, double l_pz, double l_e,
		       double& scf,double &solution1, double &solution2) {
    //Neutrino_Pz_Base
    bool usescf = true;
    if(scf < 0.0) {
	usescf = false;
	//cout<<"not using scale factor!"<<endl;
    } //if
    
    scf = 1.0;
    double nu_pz = 0.;
    double l_pt  = TMath::Sqrt(pow(l_px,2) + pow(l_py,2));
    
    double Mw    = 80.43;
    double Mt;
    double nu_pt = TMath::Sqrt(pow(nu_px,2)+pow(nu_py,2));
    Mt = TMath::Sqrt(pow(l_pt+nu_pt,2) - pow(l_px+nu_px,2) - pow(l_py+nu_py,2));
    
    //  cout<<"   W mass is "<<Mw<<", mt is "<<Mt<<endl;
    double A;
    //rs: only change the scale factor if the user wants to.
    if (Mt < Mw || !usescf) A = pow(Mw,2) / 2.;
    // assume Mt=Mw, rescale MET accordingly
    else       {       
	//cout<<"  scaling met"<<endl;
	A = pow(Mt,2) / 2.;
	double k = nu_pt * l_pt - nu_px * l_px - nu_py * l_py;
	k = (k == 0 ? 0.00001 : k);
	scf = 0.5 * pow(Mw,2) / k;
	nu_px *= scf;
	nu_py *= scf;
	nu_pt = sqrt(pow(nu_px,2) + pow(nu_py,2));
    } //else
    
      // If the electron goes along the beam direction (Px==Py==0),
      // then the algorithm below doesn't work, so do the calculation here.
    double S_denom = pow(l_pz,2) - pow(l_e,2);
    if(S_denom == 0.0) {
	//cout<<"lepton along beam direction, Mt is "<<Mt<<endl;
	//cout<<"lpx "<<l_px<<", l_py "<<l_py<<", l_pz "<<l_pz<<", n_px "<<nu_px<<", n_py "<<nu_py<<endl;
	// in this case there is one exact solution
	double mwplz = pow(Mw,2) / fabs(l_pz);
	nu_pz= (pow(nu_px,2) + pow(nu_py,2) - pow(mwplz/2.,2)) / mwplz;
	solution1 = nu_pz;
	solution2 = nu_pz;
	return;
    } //if
    
    double B = nu_px * l_px + nu_py * l_py;
    double C;
    double C1 = 1.0E0 + pow(nu_pt,2) * (pow(l_pz,2)-pow(l_e,2)) / pow(A+B,2);
    if(C1 <= 0.0) {
	//cout<<"Warning in Neutrino_Pz! negative C1 "<<C1<<", Setting it to 0."<<endl;
	// in this case C will be zero and there is only one solution,
	// so simply return the one possible solution
	solution1 = (-(A+B) * l_pz) / (pow(l_pz,2) - pow(l_e,2));
	solution2 = solution1;
	return;
    } //if
    else {
	C = TMath::Sqrt(C1);
	solution1 = (-(A+B)*l_pz + (A+B)*l_e*C) / S_denom;
	  solution2 = (-(A+B)*l_pz - (A+B)*l_e*C) / S_denom;
	  return;
    } //else
    
}

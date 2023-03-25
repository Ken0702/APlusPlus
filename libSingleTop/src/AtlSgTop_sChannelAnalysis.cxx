//____________________________________________________________________
//
// Single-top s-Channel Analysis
// 
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2013 (C) Soeren Stamm
//
#ifndef SINGLETOP_AtlSgTop_sChannelAnalysis
#include <AtlSgTop_sChannelAnalysis.h>
#endif
#include <AtlWDecayLNu.h>
#include <HepTopDecay.h>
#include <AtlEvtWriterLPSC.h>
#include <iostream>
#include <TH2F.h>
#include <AtlQCDMMTool.h>
#include <TKey.h>
#include <TChainElement.h>

using namespace std;

//____________________________________________________________________

AtlSgTop_sChannelAnalysis::AtlSgTop_sChannelAnalysis(const char* OutputFile) :
    AtlSelector(OutputFile) {
    //
    // Default constructor
    //
    
    // Init
    fLepton    = 0;
    fLeptons   = 0;
    fElectrons = 0;
    fMuons     = 0;
    fLooseLeptons = 0;
    fLooseTaus = 0;
    fJets      = 0;
    fBJets     = 0;
    fObjTool   = 0;
    fSFTool    = 0;
    fPdfTool   = 0;
    fQCDMMToolIsOn = kFALSE;
    fCutFlowTool       = 0;
    SetCutDefaults();
}

//____________________________________________________________________

AtlSgTop_sChannelAnalysis::~AtlSgTop_sChannelAnalysis() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlSgTop_sChannelAnalysis::Clear(Option_t */*option*/) {
    //
    //  Clears the dynamically allocated objects
    //
    fLepton = 0;
}

//____________________________________________________________________

void AtlSgTop_sChannelAnalysis::Begin(TTree *tree) {
    //
    // Begin analysis job
    //

    // Create event writer
    fEvtWriter = new AtlEvtWriterLPSC;
    dynamic_cast<AtlEvtWriterD3PDSgTop*>(fEvtWriter)->SetCloneOnly(fEvtWriterCloneOnly);

    AtlSelector::Begin(tree);
}

//____________________________________________________________________

void AtlSgTop_sChannelAnalysis::SetBranchStatus() {
    //
    // Switch on/off branches in order to gain speed
    //

    // Switch off all branches by default
    fTree->SetBranchStatus("*", kFALSE);
}

//____________________________________________________________________

void AtlSgTop_sChannelAnalysis::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    fPrintEvent = kFALSE;
    fInputMode  = kD3PDSgTop_v2;
    // fInputMode  = kD3PDSgTop;
    
    // Cut defaults
    fMET_min    = 0.;
    fMET_max    = 1.e10;
    fMtW_minPre = 0.;
    fMtW_min    = 0.;
    fMtW_max    = 1.e10;
    fMtW_maxPre = 1.e10;
    fPt_Lepton_min = 0.;
    fEta_Lepton_min = 0.;
    fEta_Lepton_max = 1.e10;
    fPt_Jet_min = 0.;
    fPt_Jet_max = 1.e10;
    fPt_LeadingJet_min = 0.;
    fPt_LeadingJet_max = 1.e10;
    fNBTags_min = 0;
    fNBTags_max = 999;
    fLeadingJetIsBTagged = kFALSE;
    fApplyDileptonVeto = kFALSE;
    fApplyTtbar4JetVeto = kFALSE;
    fLeptonFlavour = kLepton;
    fPt_BJetTop_min = 0.;
    fVetoBTagger = AtlBTag::kMV1;
    fVetoBTagWeight_min = 1.e10; // need to be a high value, see IsVeto
    fAdditionalJetVeto = kFALSE;
    fAdditionalJetVetoPt = kTRUE;
    fAdditionalJetVetoEta = kTRUE;
    fDoReconstructTop = kFALSE;

    fHt_min = 0.;
    fDeltaR_Jet1Jet2_min = 0.;
    fBDTResponse_min = -1.0;

    fQCDVeto = kFALSE;

    fEvtWriterCloneOnly = kFALSE;
    
    fMCTruthStudy = kFALSE;
}


//___________________________________________


void AtlSgTop_sChannelAnalysis::BookHistograms() {
    //
    // Book histograms
    //
    if ( fMCTruthStudy )
	BookHistogramsMCTruthStudy();

    if ( fDoReconstructTop ) {
        fHistsBDT = new AtlHistogramTool(
            "BDT", "BDT histograms in el+mu channel");
        AddTool(fHistsBDT);
        BookHistogramsBDT(fHistsBDT);
        fHistsBDTElectron = new AtlHistogramTool(
            "BDT_el", "BDT histograms in el channel");
        AddTool(fHistsBDTElectron);
        BookHistogramsBDT(fHistsBDTElectron);
        fHistsBDTMuon = new AtlHistogramTool(
            "BDT_mu", "BDT histograms in mu channel");
        AddTool(fHistsBDTMuon);
        BookHistogramsBDT(fHistsBDTMuon);
    }

    // Pretag
    fHistsPretag = new AtlHistogramTool("PreTag", "Pre-tagged histograms");
    AddTool(fHistsPretag);
    BookHistogramsPretag(fHistsPretag);

    // BTag
    fHistsBTag = new AtlHistogramTool("bTagged", "B-tagged histograms");
    AddTool(fHistsBTag);
    BookHistogramsPreSel(fHistsBTag);
    BookHistogramsSel(fHistsBTag);

    // split up in muon and electron
    fHistsBTagElectron = new AtlHistogramTool("bTagged_el",
					      "B-tagged histograms in e channel");
    AddTool(fHistsBTagElectron);
    BookHistogramsPreSel(fHistsBTagElectron);
    BookHistogramsSel(fHistsBTagElectron);
    fHistsBTagMuon = new AtlHistogramTool("bTagged_mu",
					  "B-tagged histograms in mu channel");
    AddTool(fHistsBTagMuon);
    BookHistogramsPreSel(fHistsBTagMuon);
    BookHistogramsSel(fHistsBTagMuon);

    // Get object selection tool and object lists
    fObjTool = (AtlObjectsToolD3PDSgTop*)GetTool("AtlObjectsToolD3PDSgTop"
						 ,"",kTRUE);
    fElectrons    = fObjTool->GetElectrons();
    fMuons        = fObjTool->GetMuons();
    fLeptons      = fObjTool->GetLeptons();
    fLooseLeptons = fObjTool->GetLooseLeptons();
    fLooseTaus    = fObjTool->GetLooseTaus();
    fJets         = fObjTool->GetJets();
    fBJets        = fObjTool->GetBJets();

    // Get scale factor tool
    fSFTool = (AtlObjRecoScaleFactorTool*)GetTool("AtlObjRecoScaleFactorTool"
						  ,"",kTRUE);
    fSFTool->SetLeptons(fLeptons);
    fSFTool->SetJets(fJets);
    
    // Get pointer to cut-flow tool
    fCutFlowTool = (AtlCutFlowTool*)GetTool("AtlCutFlowTool", "", kTRUE);
    
    // Get Pdf reweighting tool
    fPdfTool = (AtlPdfReweightingTool*)GetTool("AtlPdfReweightingTool",
					       "",kFALSE);
    if ( fPdfTool == 0 ) {
	Warning(__FUNCTION__, "PdfReweightingTool not found.");
    } else {
	fPdfTool->AddObservable("bTagged/Sel/h_sel_Lepton_charge");
	fPdfTool->AddObservable("bTagged/Sel/h_sel_Lepton_charge_even");
	fPdfTool->AddObservable("bTagged/Sel/h_sel_Lepton_charge_odd");
	
	fPdfTool->AddObservable("bTagged/Sel/h_sel_LeadingJet_pt");
	fPdfTool->AddObservable("bTagged/Sel/h_sel_LeadingJet_pt_even");
	fPdfTool->AddObservable("bTagged/Sel/h_sel_LeadingJet_pt_odd");
    }

    // Look if QCD MM Tool is running
    AtlAnalysisTool *tool = (AtlQCDMMTool*)GetTool("AtlQCDMMTool", "", kFALSE);
    if ( ( tool != 0 ) && ( tool->fProcessMode != AtlAnalysisTool::kOff ) ) {
	fQCDMMToolIsOn = kTRUE;
	Info(__FUNCTION__, "QCD Tool found");
    } else {
	Info(__FUNCTION__, "QCD Tool not found");
    }
}
		    
//____________________________________________________________________

void AtlSgTop_sChannelAnalysis::BookHistogramsPretag(AtlHistogramTool *htool) {
    //
    // Book pretag histograms
    //    
    htool->Add("h_pretag_Ht", "H_T", 40, 0., 400.,
               "H_{T} [GeV]", "Number of Entries");
    htool->Add("h_pretag_MET", "Missing E_T", 40, 0., 200.,
	       "E_{T, miss} [GeV]", "Number of Entries");
    htool->Add("h_pretag_MtW", "Transverse W-Boson mass", 42, 0., 210.,
	       "M_{T, W} [GeV]", "Number of Entries");
    htool->Add("h_pretag_LeadingJet_pt", "Leading Jet p_{T}", 44, 0., 220.,
	       "p_{T} [GeV]", "Number of Entries");
    htool->Add("h_pretag_LeadingJet_eta", "Leading Jet #eta", 25, -5., 5.,
	       "#eta", "Number of Entries");
    htool->Add("h_pretag_LeadingJet_phi", "Leading Jet #phi", 20, -TMath::Pi(), TMath::Pi(),
	       "#phi", "Number of Entries");
    htool->Add("h_pretag_2ndLeadingJet_pt", "2nd Leading Jet p_{T}", 44, 0., 220.,
	       "p_{T} [GeV]", "Number of Entries");
    htool->Add("h_pretag_2ndLeadingJet_eta", "2nd Leading Jet #eta", 25, -5., 5.,
	       "#eta", "Number of Entries");
    htool->Add("h_pretag_2ndLeadingJet_phi", "2nd Leading Jet #phi", 20, -TMath::Pi(), TMath::Pi(),
	       "#phi", "Number of Entries");
    htool->Add("h_pretag_Lepton_pt", "Lepton p_{T}", 30., 0., 150.,
	       "Lepton p_{T} [GeV]", "Number of Entries");
    htool->Add("h_pretag_Lepton_eta", "Lepton #eta", 20., -4., 4.,
	       "Lepton #eta", "Number of Entries");
    htool->Add("h_pretag_Lepton_phi", "Lepton #phi", 20, -TMath::Pi(), TMath::Pi(),
	       "Lepton #phi", "Number of Entries");
}

//____________________________________________________________________

void AtlSgTop_sChannelAnalysis::BookHistogramsPreSel(AtlHistogramTool *htool) {
    //
    // Book event pre-selection histograms
    //

    // No. of events
    htool->Add("PreSel/h_presel_nevt", "No. of Pre-Selected Events", 1, 0, 1,
	       "Number of Entries", "PreSel");
}    

//____________________________________________________________________

void AtlSgTop_sChannelAnalysis::BookHistogramsSel(AtlHistogramTool *htool){
    //
    // Book final event selection histograms
    //

    // No. of events
    htool->Add("Sel/h_sel_nevt", "No. of Selected Events", 1, 0, 1,
	       "Selection", "Number of Entries");

    htool->Add("Sel/h_sel_Ht", "Ht", 40, 0., 400.,
               "H_{T} [GeV]", "Number of Entries");

    // MET
    htool->Add("Sel/h_sel_met", "Missing E_T", 40, 0., 200.,
	       "E_{T, miss} [GeV]", "Number of Entries");

    // Mt_W
    htool->Add("Sel/h_sel_MtW", "Transverse W-Boson mass", 42, 0., 210.,
	       "M_{T, W} [GeV]", "Number of Entries");

    // 1st jet
    htool->Add("Sel/h_sel_LeadingJet_pt", "Leading Jet p_{T}", 60, 0., 300.,
	       "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingJet_pt_even", "Leading Jet p_{T}, even events",
	       60, 0., 300., "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingJet_pt_odd", "Leading Jet p_{T}, odd events",
	       60, 0., 300., "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingJet_eta", "Leading Jet #eta", 25, -5., 5.,
	       "#eta", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingJet_phi", "Leading Jet #phi", 20, -TMath::Pi(), TMath::Pi(),
	       "#phi", "Number of Entries");

    // 2nd jet
    htool->Add("Sel/h_sel_2ndLeadingJet_pt", "2nd Leading Jet p_{T}", 60, 0., 300.,
	       "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_2ndLeadingJet_eta", "2nd Leading Jet #eta", 25, -5., 5.,
	       "#eta", "Number of Entries");
    htool->Add("Sel/h_sel_2ndLeadingJet_phi", "2nd Leading Jet #phi", 20, -TMath::Pi(), TMath::Pi(),
	       "#phi", "Number of Entries");

    // Special histograms for ttbar ctrl region
    // Bjets
    htool->Add("Sel/h_sel_LeadingBJet_pt", "Leading B-Jet p_{T}", 44, 0., 220.,
	       "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingBJet_eta", "Leading B-Jet #eta", 25, -5., 5.,
	       "#eta", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingBJet_phi", "Leading B-Jet #phi", 20, -TMath::Pi(), TMath::Pi(),
	       "#phi", "Number of Entries");
    htool->Add("Sel/h_sel_2ndLeadingBJet_pt", "2nd Leading B-Jet p_{T}", 44, 0., 220.,
	       "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_2ndLeadingBJet_eta", "2nd Leading B-Jet #eta", 25, -5., 5.,
	       "#eta", "Number of Entries");
    htool->Add("Sel/h_sel_2ndLeadingBJet_phi", "2nd Leading B-Jet #phi", 20, -TMath::Pi(), TMath::Pi(),
	       "#phi", "Number of Entries");    
    // Ljets
    htool->Add("Sel/h_sel_LeadingLJet_pt", "Leading L-Jet p_{T}", 44, 0., 220.,
	       "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingLJet_eta", "Leading L-Jet #eta", 25, -5., 5.,
	       "#eta", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingLJet_phi", "Leading L-Jet #phi", 20, -TMath::Pi(), TMath::Pi(),
	       "#phi", "Number of Entries");
    htool->Add("Sel/h_sel_2ndLeadingLJet_pt", "2nd Leading L-Jet p_{T}", 44, 0., 220.,
	       "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_2ndLeadingLJet_eta", "2nd Leading L-Jet #eta", 25, -5., 5.,
	       "#eta", "Number of Entries");
    htool->Add("Sel/h_sel_2ndLeadingLJet_phi", "2nd Leading L-Jet #phi", 20, -TMath::Pi(), TMath::Pi(),
	       "#phi", "Number of Entries");

    // Lepton
    htool->Add("Sel/h_sel_Lepton_pt", "Lepton p_{T}", 30., 0., 150.,
	       "Lepton p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_Lepton_eta", "Lepton #eta", 20., -4., 4.,
	       "Lepton #eta", "Number of Entries");
    htool->Add("Sel/h_sel_Lepton_phi", "Lepton #phi", 20, -TMath::Pi(), TMath::Pi(),
	       "Lepton #phi", "Number of Entries");
    htool->Add("Sel/h_sel_Lepton_charge", "Lepton Charge", 2, -1.0001, 1.0001,
	       "Lepton Charge", "Number of Entries");
    htool->Add("Sel/h_sel_Lepton_charge_even", "Lepton Charge, even events", 2, -1.0001, 1.0001,
	       "Lepton Charge", "Number of Entries");
    htool->Add("Sel/h_sel_Lepton_charge_odd", "Lepton Charge, odd events", 2, -1.0001, 1.0001,
	       "Lepton Charge", "Number of Entries");
    
    // Leading jet pt for 5 eta bins
    htool->Add("Sel/h_sel_LeadingJet_pt_eta1", "Leading Jet p_{T}", 44, 0., 220.,
	       "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingJet_pt_eta2", "Leading Jet p_{T}", 44, 0., 220.,
	       "p_{T} [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_LeadingJet_pt_eta3", "Leading Jet p_{T}", 44, 0., 220.,
	       "p_{T} [GeV]", "Number of Entries");  
    htool->Add("Sel/h_sel_LeadingJet_pt_eta4", "Leading Jet p_{T}", 44, 0., 220.,
	       "p_{T} [GeV]", "Number of Entries");    
    htool->Add("Sel/h_sel_LeadingJet_pt_eta5", "Leading Jet p_{T}", 44, 0., 220.,
	       "p_{T} [GeV]", "Number of Entries");

    // Delta R
    htool->Add("Sel/h_sel_DeltaR_Lepton_LeadingJet", "#DeltaR(lep, leading jet)", 36., 0.4, 4.,
	       "#DeltaR(lepton, leading jet)", "Number of Entries");
    htool->Add("Sel/h_sel_DeltaR_Lepton_2ndLeadingJet","#DeltaR(lep, 2nd leading jet)",36., 0.4, 4.,
	       "#DeltaR(lepton, 2nd leading jet)", "Number of Entries");
    htool->Add("Sel/h_sel_DeltaR_LeadingJet_2ndLeadingJet", "#DeltaR(leading jet, 2nd leading jet)",
	       36., 0.4, 4., "#DeltaR(leading jet, 2nd leading jet)", "Number of Entries");

    //Inv. mass
    htool->Add("Sel/h_sel_InvM_Lepton_LeadingJet", "Inv. mass (lepton, leading jet)", 50., 0., 400.,
	       "Inv. mass (lepton, j_{1}) [GeV]", "Number of Entries");
    htool->Add("Sel/h_sel_InvM_LeadingJet_2ndLeadingJet", "Inv. mass (leading jet, 2nd leading jet)",
	       50., 0., 400., "Inv. mass (j_{1}, j_{2}) [GeV]", "Number of Enrties");

    // Pt_lepton * Et_miss
    htool->Add("Sel/h_sel_ScalarProd_Lepton_Etmiss","P_{Lepton} * E_{T, miss} [GeV^{2}]", 120., -5000., 10000.,
	      "#vec{p}_{lepton}#vec{E}_{T, miss}", "Number of Entries");

    // QCD Veto
    htool->Add("Sel/h_sel_QCDVeto", "QCD Veto", 20., -TMath::Pi(), TMath::Pi(), 20., 0., 100., "#Delta#phi(j_{1},l)", "Lepton p_{T} (GeV)", "Events");
}

//____________________________________________________________________

void AtlSgTop_sChannelAnalysis::FillHistogramsSel(AtlHistogramTool *htool, Double_t W){
    //
    // Fill histograms for final event selection
    //
    HepParticle *lepton= fLepton;
    
    AtlJet *Jetlead = (AtlJet*)fJets->At(0);
    AtlJet *Jet2ndlead = (AtlJet*)fJets->At(1);

    htool->Fill("Sel/h_sel_nevt", 0.5, W);
    htool->Fill("Sel/h_sel_Lepton_pt",  lepton->Pt(), W);
    htool->Fill("Sel/h_sel_Lepton_eta", lepton->Eta(), W);
    htool->Fill("Sel/h_sel_Lepton_phi", lepton->Phi(), W);
    htool->Fill("Sel/h_sel_Lepton_charge", lepton->Charge(), W);
    htool->Fill("Sel/h_sel_Ht", fHt, W);
    htool->Fill("Sel/h_sel_met", fMET, W);
    htool->Fill("Sel/h_sel_MtW", fMtW, W);
    htool->Fill("Sel/h_sel_LeadingJet_pt",Jetlead->Pt(), W);
    htool->Fill("Sel/h_sel_LeadingJet_eta",Jetlead->Eta(), W);
    htool->Fill("Sel/h_sel_LeadingJet_phi",Jetlead->Phi(), W);
    htool->Fill("Sel/h_sel_2ndLeadingJet_pt",Jet2ndlead->Pt(), W);
    htool->Fill("Sel/h_sel_2ndLeadingJet_eta",Jet2ndlead->Eta(), W);
    htool->Fill("Sel/h_sel_2ndLeadingJet_phi",Jet2ndlead->Phi(), W);

    // even/odd events
    if ( fEvent->EventNr() % 2 == 0 ) {
	htool->Fill("Sel/h_sel_LeadingJet_pt_even", Jetlead->Pt(), W);
	htool->Fill("Sel/h_sel_Lepton_charge_even", lepton->Charge(), W);
    } else {
	htool->Fill("Sel/h_sel_LeadingJet_pt_odd", Jetlead->Pt(), W);
	htool->Fill("Sel/h_sel_Lepton_charge_odd", lepton->Charge(), W);
    }

    // Special histograms for ttbar ctrl region
    // make sure there are exactly 2 btags (counter example QCDJetLepton)
    if ( fJets->GetEntries() == 4 && fNBTags_min == 2 && fNBTags_max == 2) {
	AtlJet *BJetlead = (AtlJet*) fBJets->At(0);
	AtlJet *BJet2ndlead = (AtlJet*) fBJets->At(1);
	TIter next_ljet(fJets);
	AtlJet *jet = 0;
	AtlJet *LJetlead = 0;
	AtlJet *LJet2ndlead = 0;
	while ( (jet = (AtlJet*) next_ljet()) ) {
	    if ( jet != BJetlead && jet != BJet2ndlead ) {
		if ( LJetlead == 0 )
		    LJetlead = jet;
		else
		    LJet2ndlead = jet;
	    }
	}
	htool->Fill("Sel/h_sel_LeadingBJet_pt",BJetlead->Pt(), W);
	htool->Fill("Sel/h_sel_LeadingBJet_eta",BJetlead->Eta(), W);
	htool->Fill("Sel/h_sel_LeadingBJet_phi",BJetlead->Phi(), W);
	htool->Fill("Sel/h_sel_2ndLeadingBJet_pt",BJet2ndlead->Pt(), W);
	htool->Fill("Sel/h_sel_2ndLeadingBJet_eta",BJet2ndlead->Eta(), W);
	htool->Fill("Sel/h_sel_2ndLeadingBJet_phi",BJet2ndlead->Phi(), W);
	htool->Fill("Sel/h_sel_LeadingLJet_pt",LJetlead->Pt(), W);
	htool->Fill("Sel/h_sel_LeadingLJet_eta",LJetlead->Eta(), W);
	htool->Fill("Sel/h_sel_LeadingLJet_phi",LJetlead->Phi(), W);
	htool->Fill("Sel/h_sel_2ndLeadingLJet_pt",LJet2ndlead->Pt(), W);
	htool->Fill("Sel/h_sel_2ndLeadingLJet_eta",LJet2ndlead->Eta(), W);
	htool->Fill("Sel/h_sel_2ndLeadingLJet_phi",LJet2ndlead->Phi(), W);
    }

    // Eta Bins:
    // [-2.5, -2.], ]-2., -1.], ]-1., 1], ]1., 2.], ]2., 2.5]
    if ( -2.5 <= Jetlead->Eta() && Jetlead->Eta() <= -2. )
	htool->Fill("Sel/h_sel_LeadingJet_pt_eta1",Jetlead->Pt(), W);
    else if (  -2. <  Jetlead->Eta() && Jetlead->Eta() <= -1. )
	htool->Fill("Sel/h_sel_LeadingJet_pt_eta2",Jetlead->Pt(), W);
    else if (  -1. <  Jetlead->Eta() && Jetlead->Eta() <= +1. )
	htool->Fill("Sel/h_sel_LeadingJet_pt_eta3",Jetlead->Pt(), W);
    else if (  +1. <  Jetlead->Eta() && Jetlead->Eta() <= +2. )
	htool->Fill("Sel/h_sel_LeadingJet_pt_eta4",Jetlead->Pt(), W);
    else if (  +2. <  Jetlead->Eta() && Jetlead->Eta() <= +2.5 )
	htool->Fill("Sel/h_sel_LeadingJet_pt_eta5",Jetlead->Pt(), W);

    // Delta R
    htool->Fill("Sel/h_sel_DeltaR_Lepton_LeadingJet", lepton->DeltaR(Jetlead), W);
    htool->Fill("Sel/h_sel_DeltaR_Lepton_2ndLeadingJet", lepton->DeltaR(Jet2ndlead), W);
    htool->Fill("Sel/h_sel_DeltaR_LeadingJet_2ndLeadingJet", Jetlead->DeltaR(Jet2ndlead), W);

    // Inv. mass
    TLorentzVector p_lep_j1 = lepton->P() + Jetlead->P();
    htool->Fill("Sel/h_sel_InvM_Lepton_LeadingJet",p_lep_j1.M(), W);

    TLorentzVector p_j1_j2 = Jetlead->P() + Jet2ndlead->P();
    htool->Fill("Sel/h_sel_InvM_LeadingJet_2ndLeadingJet",p_j1_j2.M(), W);

    // Pt_lepton * Et_miss
    TVector2 vec_met = fEvent->GetEnergySum()->GetMissingEt();
    TVector3 vec_lep = lepton->P3();    
    htool->Fill("Sel/h_sel_ScalarProd_Lepton_Etmiss",vec_met*(vec_lep.XYvector()), W );

   //QCD Veto
   htool->Fill("Sel/h_sel_QCDVeto", lepton->DeltaPhi(Jetlead), lepton->Pt(), W);
}

//____________________________________________________________________

Bool_t AtlSgTop_sChannelAnalysis::ProcessPreCut() {
    //
    // Event pre-selection
    //
    fCutFlowTool->Fill("JetBin", GetTagEvtWeight());

    // Require exactly one lepton and save it in fLepton
    if ( !fQCDMMToolIsOn ) {
	// exactly one tight lepton without QCD
	if ( fLeptons->GetEntries() != 1 ) {
	    return kFALSE;
	} else {
	    fLepton = (HepParticle*)fLeptons->At(0);
	}
    } else {
	// exactly one tight or loose lepton in case of QCD
	if ( fLeptons->GetEntries() + fLooseLeptons->GetEntries() != 1) {
	    return kFALSE;
	} else if ( fLeptons->GetEntries() == 1 ) {
	    fLepton = (HepParticle*)fLeptons->At(0);
	} else {
	    fLepton = (HepParticle*)fLooseLeptons->At(0);
	}
    } // end if not QCDMM
    fCutFlowTool->Fill("1Lepton", GetTagEvtWeight());
    
    // Require lepton flavour. Needed for AntiMuon sample which includes electrons.
    if ( fLeptonFlavour != kLepton) {
        if (( fLepton->IsElectron() && fLeptonFlavour != kElectron) ||
            ( fLepton->IsMuon() && fLeptonFlavour != kMuon )) {
            return kFALSE;
        }
    }
    fCutFlowTool->Fill("LepFlav", GetTagEvtWeight());

    // Missing Et
    fMET = fEvent->GetEnergySum()->MissingEt_Mag();
    if ( (fMET < fMET_min) || (fMET > fMET_max) ) return kFALSE;
    fCutFlowTool->Fill("MET", GetTagEvtWeight());
   
    
    // Transverse W mass
    fMtW = fEvent->W_Mperp(fLepton);
    if ( (fMtW < fMtW_minPre) || (fMtW > fMtW_maxPre) ) return kFALSE;
    fCutFlowTool->Fill("MtW", GetTagEvtWeight());
    

    if(fJets->GetEntries() != 2 ) return kFALSE;
    // Accept event
    return kTRUE;
}

//____________________________________________________________________

Bool_t AtlSgTop_sChannelAnalysis::DiLeptonVeto() {
    //
    // Dilepton veto (against ttbar dilepton bkg)
    //
    // A return value of 'true' means the veto is fulfilled, ie. the
    // event ought to be discarded.
    //
    // Note: for run-1 events, the veto uses loose lepton information,
    // which is at present not available for run-2 events. Therefore
    // here the tight leptons are used for the moment.
    //
    if ( fEvent->IsRun1() ) {
        Int_t Ntight = fLeptons->GetEntries();
        Int_t Nloose = fLooseLeptons->GetEntries();
        if ( Ntight + Nloose > 1 ) return kTRUE;
    } else  if ( fEvent->IsRun2() ) {
	Int_t N_Electron = fEvent->GetN_Electrons();
	Int_t N_Muon = fEvent->GetN_Muons();
	if ( N_Electron + N_Muon > 1 ) return kTRUE;
    } else {
	Fatal(__FUNCTION__, "Unknown run period. Abort!");
	gSystem->Abort(1);
    }
    return kFALSE;
}

//____________________________________________________________________

Bool_t AtlSgTop_sChannelAnalysis::ProcessCut() {
    //
    // Event selection
    //

    // Fill Pretag-Histograms
    FillHistogramsPretag(fHistsPretag, GetPreTagEvtWeight());

    // Count no. of pre-tagged events
    fPreTaggedEvents++;
    fPreTaggedEventsW += fEvent->GetPreTagEvtWeight();

    // ===========
    // No. of jets
    // ===========

    // Don't allow additional jets outside the given pt-eta range
    // optional flags: do the veto based on pt or eta only
    if ( fAdditionalJetVeto ) {
        if ( fAdditionalJetVetoPt && fAdditionalJetVetoEta ) {
            // if both Pt and Eta are vetoed, simply take all jets from event
            if ( fEvent->GetN_Jets(fObjTool->fJetsAuthor) > fJets->GetEntries() )
                return kFALSE;
        } else {
            // else only do one of the vetoes and cut the other variable
            TClonesArray *evt_jets = fEvent->GetJets(fObjTool->fJetsAuthor);
            TIter next_jet(evt_jets);
            AtlJet *jet = 0;
            Int_t n_jets_pt = 0;
            Int_t n_jets_eta = 0;
            while ( (jet = (AtlJet*)next_jet()) ) {
                // count jets that pass eta requirement (pt is vetoed)
                if ( TMath::Abs(jet->Eta()) >= fObjTool->fJetEta_min &&
                     TMath::Abs(jet->Eta()) <= fObjTool->fJetEta_max ) {
                    n_jets_pt++;
                }
                // count jets that pass pt requirement (eta is vetoed)
                if ( jet->Pt() >= fObjTool->fJetPt_min ) {
                    n_jets_eta++;
                }

            }
            if ( (fAdditionalJetVetoPt && n_jets_pt > fJets->GetEntries()) ||
                 (fAdditionalJetVetoEta && n_jets_eta > fJets->GetEntries()) )
                return kFALSE;
        }
    }
    fCutFlowTool->Fill("AddJetVeto", GetTagEvtWeight());

    // Apply analysis Pt cut
    TIter next_jet(fJets);
    AtlJet *jet = 0;
    while ( (jet = (AtlJet*)next_jet()) ) {
	if ( (jet->Pt() < fPt_Jet_min) || (jet->Pt() > fPt_Jet_max) )
	    return kFALSE;
    }
    fCutFlowTool->Fill("JetPt", GetTagEvtWeight());

    // Cumpute Ht
    HepParticle *lepton = (HepParticle*) fLeptons->At(0);
    fHt = fMET + lepton->Pt();
    next_jet.Reset();
    while ( (jet = (AtlJet*)next_jet()) ) {
        fHt += jet->Pt();
    }

    // =========
    // B-tagging
    // =========

    // No. of b-tagged jets
    if ( fBJets->GetEntries() < fNBTags_min
         || fBJets->GetEntries() > fNBTags_max ) return kFALSE;
    fCutFlowTool->Fill("NBtag", GetTagEvtWeight());

    // Require the leading jet to be b-tagged ?
    if ( fLeadingJetIsBTagged ) {
        if ( fBJets->At(0) != fJets->At(0) ) return kFALSE;
        fCutFlowTool->Fill("LeadingJIsBTagged", GetTagEvtWeight());
    }

    // B-tag veto (for ctrl region)
    // only apply veto if all Jets are b-Jets
    TIter next_bjet(fBJets);
    AtlJet *bjet = 0;
    Bool_t IsVeto = ( fBJets->GetEntries() == fJets->GetEntries() );
    while ( (bjet = (AtlJet*)next_bjet()) ) {
	IsVeto &= ( bjet->GetTag(fVetoBTagger)->GetWeight()
		    > fVetoBTagWeight_min );
    }
    if ( IsVeto ) return kFALSE;
    fCutFlowTool->Fill("BTagVeto", GetTagEvtWeight());
    
    // Fill pre-selected histograms
    FillHistogramsPreSel(fHistsBTag, GetTagEvtWeight());
    if ( fLepton->IsElectron() ) {
	FillHistogramsPreSel(fHistsBTagElectron, GetTagEvtWeight());
    } else {
	FillHistogramsPreSel(fHistsBTagMuon, GetTagEvtWeight());
    }

    // ===============
    // Final selection
    // ===============

    // Final transverse W mass cut
    if ( fMtW < fMtW_min ) return kFALSE;
    fCutFlowTool->Fill("MtWMin", GetTagEvtWeight());
    if ( fMtW > fMtW_max ) return kFALSE;
    fCutFlowTool->Fill("MtWMax", GetTagEvtWeight());

    // Cut on lepton pt / eta
    if ( fLepton->Pt() < fPt_Lepton_min )
        return kFALSE;
    fCutFlowTool->Fill("LeptonPt", GetTagEvtWeight());
    if ( (TMath::Abs(fLepton->Eta()) < fEta_Lepton_min) ||
         (TMath::Abs(fLepton->Eta()) > fEta_Lepton_max) )
        return kFALSE;
    fCutFlowTool->Fill("LeptonEta", GetTagEvtWeight());

    // Cut on leading jet pt
    AtlJet *Jetlead = (AtlJet*) fJets->At(0);
    if ( (Jetlead->Pt() < fPt_LeadingJet_min) || (Jetlead->Pt() > fPt_LeadingJet_max) )
	return kFALSE;
    fCutFlowTool->Fill("LeadingJPt", GetTagEvtWeight());
    
    // Apply ttbar di-lepton veto
    if ( fApplyDileptonVeto ) {
	if ( DiLeptonVeto() ) return kFALSE;
        fCutFlowTool->Fill("DiLepVeto", GetTagEvtWeight());
    }

    // QCD Veto
    if ( fQCDVeto ) {
       HepParticle *lepton= fLepton;
        Double_t DeltaPhi = TMath::Abs(lepton->DeltaPhi(Jetlead));
        //Double_t qcdveto  = 40.*(1. - (TMath::Pi() - DeltaPhi)/(TMath::Pi() - 0.5));
        //Double_t qcdveto  = 40.*(1. - (TMath::Pi() - DeltaPhi)/(TMath::Pi() - 1));
        Double_t qcdveto  = 40. * DeltaPhi/TMath::Pi() ;
        if ( lepton->Pt() < qcdveto ) return kFALSE;
        fCutFlowTool->Fill("QCDVeto", GetTagEvtWeight());
    }

    // Ht cut
    if ( fHt < fHt_min ) return kFALSE;

    // DeltaR_Jet1Jet2  cut
    AtlJet *Jet2ndlead  = (AtlJet*) fJets->At(1);
    if ( Jetlead->DeltaR(Jet2ndlead) < fDeltaR_Jet1Jet2_min ) return kFALSE;

    // BDT response cut
    if (fBDTResponse_min > fEvent->GetBDTResponse() ) return kFALSE;
    
    // Apply ttbar 4-jet veto
    if ( fApplyTtbar4JetVeto ) {
	AtlJet *BJetlead    = (AtlJet*) fBJets->At(0);
	AtlJet *BJet2ndlead = (AtlJet*) fBJets->At(1);
	if ( Jetlead != BJetlead || Jet2ndlead != BJet2ndlead )
	    return kFALSE;
	
    	// Light jets
	TIter next_ljet(fJets);
	AtlJet *jet = 0;
	while ( (jet = (AtlJet*) next_ljet()) ) {
	    if ( jet != BJetlead && jet != BJet2ndlead ) {
		if ( jet->Pt() > 60. )  return kFALSE;
	    }
	}
        fCutFlowTool->Fill("Ttbar4JetVeto", fEvent->GetTagEvtWeight());
    }
    
    // Reconstruct top-quark and compute BDT variables
    // (if enabled)
    if ( fDoReconstructTop ) {
    
	// Reconstruct neutrino
	if ( ReconstructNeutrino() == kFALSE ) return kFALSE;
        fCutFlowTool->Fill("BDT_RecoNeutrino", GetTagEvtWeight());
	
	// Reconstruc W and top quark
	ReconstructWDecay();
	ReconstructTopDecays();
	
	// Cut on b-jet from top decay
	fPt_BJetTop = GetTopDecayBest()->GetBJetOrig()->Pt();
	if ( fPt_BJetTop < fPt_BJetTop_min )
	    return kFALSE;
        fCutFlowTool->Fill("BDT_BJetTopPt", GetTagEvtWeight());
	
	// Compute kinematic BDT input variables
	ComputeBDTVariables();
    }


    // Fill selected histograms
    FillHistogramsSel(fHistsBTag, GetTagEvtWeight());
    if ( fLepton->IsElectron() ) {
	FillHistogramsSel(fHistsBTagElectron, GetTagEvtWeight());
    } else {
	FillHistogramsSel(fHistsBTagMuon, GetTagEvtWeight());
    }

    return kTRUE;
}

//____________________________________________________________________


void AtlSgTop_sChannelAnalysis::FillHistograms() {
    //
    // Fill histograms
    //
    // This method will be called only for events surviving the
    // ProcessCut() routine
    //
    if ( fDoReconstructTop ) {
        FillHistogramsBDT(fHistsBDT);
        if ( fLepton->IsElectron() ) {
            FillHistogramsBDT(fHistsBDTElectron);
        } else {
            FillHistogramsBDT(fHistsBDTMuon);
        }
    }

    if ( fMCTruthStudy ) {
	FillHistogramsMCTruthStudy();
    }
}

//____________________________________________________________________

void AtlSgTop_sChannelAnalysis::InitEvent() {
    //
    // Prepare event analysis
    //
}

//____________________________________________________________________

void AtlSgTop_sChannelAnalysis::Print(Option_t */*option*/) const {
    //
    // Print user analysis configuration
    //
    cout << endl
	 << "========================================================" << endl
	 << "  Atlas Single-Top s-Channel Analysis Selector          " << endl
	 << "========================================================" << endl
	 << endl;
    AtlSelector::Print();
    cout << "Event pre-selection:" << endl
         << "  number of leptons    = 1" << endl
         << "  fLeptonFlavour       = "
         << (( fLeptonFlavour == kLepton ) ? "Lepton" : ( fLeptonFlavour == kElectron ) ? "Electron" : "Muon") << endl
	 << "  fMET_min             = " << fMET_min << " GeV" << endl
	 << "  fMET_max             = " << fMET_max << " GeV" << endl
	 << "  fMtW_minPre          = " << fMtW_minPre << " GeV" << endl
	 << "  fMtW_maxPre          = " << fMtW_maxPre << " GeV" << endl;
    cout << "Final event selection:" << endl
	 << "  fMtW_min             = " << fMtW_min << " GeV" << endl
	 << "  fMtW_max             = " << fMtW_max << " GeV" << endl
         << "  fAdditionalJetVeto   = " << (fAdditionalJetVeto ? "true" : "false") << endl
	 << "  fPt_Jet_min          = " << fPt_Jet_min << " GeV" << endl
	 << "  fPt_Jet_max          = " << fPt_Jet_max << " GeV" << endl
	 << "  fNBTags_min          = " << fNBTags_min << endl
	 << "  fNBTags_max          = " << fNBTags_max << endl
	 << "  fPt_Lepton_min       = " << fPt_Lepton_min << " GeV" << endl
	 << "  fEta_Lepton_min      = " << fEta_Lepton_min << endl
	 << "  fEta_Lepton_max      = " << fEta_Lepton_max << endl
         << "  fLeadingJetIsBTagged = " << (fLeadingJetIsBTagged ? "true" : "false") << endl
	 << "  fVetoBTagger         = " << AtlBTag::GetTaggerName(fVetoBTagger) << endl
	 << "  fVetoBTagWeight_min  = " << fVetoBTagWeight_min << endl
         << "  fPt_LeadingJet_min   = " << fPt_LeadingJet_min << " GeV" << endl
         << "  fPt_LeadingJet_max   = " << fPt_LeadingJet_max << " GeV" << endl
	 << "  fApplyDileptonVeto   = " << (fApplyDileptonVeto ? "true" : "false") << endl
	 << "  fApplyTtbar4JetVeto  = " << (fApplyTtbar4JetVeto ? "true" : "false") << endl
	 << "  fDoReconstructTop    = " << (fDoReconstructTop ? "true" : "false") << endl;
    if ( fDoReconstructTop )
        cout << "  fPt_BJetTop_min      = " << fPt_BJetTop_min << endl;
    cout << endl
	 << "========================================================" << endl
	 << endl;
}

//____________________________________________________________________

void AtlSgTop_sChannelAnalysis::Terminate() {
    //
    // Terminate routine called at the end of event loop
    //
    dynamic_cast<AtlEvtWriterLPSC*>(fEvtWriter)->SetXsection(fXsection);
    dynamic_cast<AtlEvtWriterLPSC*>(fEvtWriter)->SetNEvents(GetInitialNumberOfEvents());
    AtlSelector::Terminate();
}

//____________________________________________________________________

void AtlSgTop_sChannelAnalysis::FillHistogramsPretag(AtlHistogramTool *htool,
						     Double_t W) {
    //
    // Fill pretag Histograms
    //
    HepParticle *lepton = fLepton;
    AtlJet *Jetlead     = (AtlJet*) fJets->At(0);
    AtlJet *Jet2ndlead  = (AtlJet*) fJets->At(1);


    htool->Fill("h_pretag_Ht", fHt, W);
    htool->Fill("h_pretag_MET", fMET, W);
    htool->Fill("h_pretag_MtW", fMtW, W);
    htool->Fill("h_pretag_LeadingJet_pt", Jetlead->Pt(), W);
    htool->Fill("h_pretag_LeadingJet_eta", Jetlead->Eta(), W);
    htool->Fill("h_pretag_LeadingJet_phi", Jetlead->Phi(), W);
    htool->Fill("h_pretag_2ndLeadingJet_pt", Jet2ndlead->Pt(), W);
    htool->Fill("h_pretag_2ndLeadingJet_eta", Jet2ndlead->Eta(), W);
    htool->Fill("h_pretag_2ndLeadingJet_phi", Jet2ndlead->Phi(), W);
    htool->Fill("h_pretag_Lepton_pt", lepton->Pt(), W);
    htool->Fill("h_pretag_Lepton_eta", lepton->Eta(), W);
    htool->Fill("h_pretag_Lepton_phi", lepton->Phi(), W);
}

//____________________________________________________________________

void AtlSgTop_sChannelAnalysis::FillHistogramsPreSel(AtlHistogramTool *htool,
						     Double_t W) {
    //
    // Fill event pre-selection histograms
    //
    htool->Fill("PreSel/h_presel_nevt", 0.5, W);
}

//____________________________________________________________________

Bool_t AtlSgTop_sChannelAnalysis::ReconstructNeutrino() {
    //
    // Reconstruct neutrino 4-momentum using the W mass constraint
    //
    // Returns kTRUE if a neutrino could be successfully reconstructed
    // - otherwise kFALSE is returned.
    //
    static const Double_t m2_W = 80.403*80.403;
    
    HepParticle *lep   = fLepton;
    Double_t E2_lep    = TMath::Power(lep->E(), 2.);
    Double_t Px_lep    = lep->Px();
    Double_t Py_lep    = lep->Py();
    Double_t Pz_lep    = lep->Pz();
    Double_t Pz2_lep   = Pz_lep*Pz_lep;
    Double_t m2_lep    = TMath::Power(lep->M(), 2.);
    Double_t Px_nu     = fEvent->GetEnergySum()->GetMissingEt().Px();
    Double_t Py_nu     = fEvent->GetEnergySum()->GetMissingEt().Py();
    Double_t MET       = fEvent->GetEnergySum()->MissingEt_Mag();
    Double_t MET2      = MET*MET;
    Double_t CosPhiMET = TMath::Cos(fEvent->GetEnergySum()->MissingEt_Phi());
    Double_t SinPhiMET = TMath::Sin(fEvent->GetEnergySum()->MissingEt_Phi());
    
    Double_t alpha = E2_lep - Pz2_lep;
    Double_t beta  = Pz_lep * ( m2_lep - m2_W
				- 2.*(Px_lep*Px_nu + Py_lep*Py_nu) );
    Double_t root = E2_lep * ( TMath::Power((-m2_lep + m2_W
					     + 2.*(Px_lep*Px_nu
						   + Py_lep*Py_nu)), 2.)
			       + 4. * MET2 * (-E2_lep + Pz2_lep) );

    if ( root < 0. ) {
	Double_t MET_alpha = E2_lep - Pz2_lep
	    - TMath::Power((Px_lep*CosPhiMET + Py_lep*SinPhiMET), 2.);
	Double_t MET_beta  = (m2_lep - m2_W) * (Px_lep*CosPhiMET
						+ Py_lep*SinPhiMET);
	Double_t MET_root  = TMath::Power((-m2_W+m2_lep), 2.)
	    * (E2_lep-Pz2_lep);

	Double_t METnew_sol[] = { 0., 0. };
	METnew_sol[0] = (-MET_beta + TMath::Sqrt(MET_root))/(2.*MET_alpha);
	METnew_sol[1] = (-MET_beta - TMath::Sqrt(MET_root))/(2.*MET_alpha);

//?	MET = 0.;
	if ( METnew_sol[0] >= 0. && METnew_sol[1] <  0. ) MET = METnew_sol[0];
	if ( METnew_sol[0] <  0. && METnew_sol[1] >= 0. ) MET = METnew_sol[1];
	if ( METnew_sol[0] <  0. && METnew_sol[1] <  0. ) return kFALSE;
	if ( METnew_sol[0] >= 0. && METnew_sol[1] >= 0. ) {
	    if ( TMath::Abs(MET-METnew_sol[0])
		 < TMath::Abs(MET-METnew_sol[1]) ) {
		MET = METnew_sol[0];
	    } else {
		MET = METnew_sol[1];
	    }
	}

	while ( root < 0. ) {
//	    MET -= 0.000001;
	    MET -= 0.0001;
	    Px_nu = MET*CosPhiMET;
	    Py_nu = MET*SinPhiMET;
	    root = E2_lep * (TMath::Power((-m2_lep + m2_W
					   + 2.*Px_lep*Px_nu
					   + 2.*Py_lep*Py_nu), 2.)
			     + 4.*MET2*(-E2_lep + Pz2_lep));
	}
	beta = (m2_lep - m2_W - 2.*Px_lep*Px_nu - 2.*Py_lep*Py_nu) * Pz_lep;
    }

    Double_t Pz_nu_sol[] = { 0., 0. };
    Pz_nu_sol[0] = (-beta + TMath::Sqrt(root)) / (2.*alpha);
    Pz_nu_sol[1] = (-beta - TMath::Sqrt(root)) / (2.*alpha);

    // Take solution with smallest neutrino |Pz| value
    Double_t Pz_nu = Pz_nu_sol[0];
    if ( TMath::Abs(Pz_nu_sol[0]) > TMath::Abs(Pz_nu_sol[1]) ) {
	Pz_nu = Pz_nu_sol[1];
    }
    Double_t E_nu = TMath::Sqrt(TMath::Power(MET*CosPhiMET, 2.)
				+ TMath::Power(MET*SinPhiMET, 2.)
				+ TMath::Power(Pz_nu, 2));

    Int_t pdg = 0;
    if ( lep->IsEPlus() ) {
	pdg =  12;
    } else if ( lep->IsEMinus() ) {
	pdg = -12;
    } else if ( lep->IsMuPlus() ) {
	pdg =  14;
    } else if ( lep->IsMuMinus() ) {
	pdg = -14;
    } else {
	Error("ReconstructNeutrino", "Wrong lepton type. Abort!");
	gSystem->Abort(1);
    }
    fEvent->AddNeutrino(MET*CosPhiMET, MET*SinPhiMET, Pz_nu, E_nu, pdg);
    return kTRUE;
}

//____________________________________________________________________

void AtlSgTop_sChannelAnalysis::ReconstructWDecay() {
    //
    // Rconstruct W->lnu decay. ReconstructNeutrino() needs to be
    // called before.
    //
    HepParticle *lep = fLepton;    
    HepParticle *nu = (HepParticle*)fEvent->GetNeutrinos()->At(0);    
    Double_t Px_W = lep->Px() + nu->Px();
    Double_t Py_W = lep->Py() + nu->Py();
    Double_t Pz_W = lep->Pz() + nu->Pz();
    Double_t E_W  = lep->E()  + nu->E();
    fEvent->AddWDecayLNu(Px_W, Py_W, Pz_W, E_W,
			 lep, lep->Px(), lep->Py(), lep->Pz(), lep->E(),
			 nu, HepWDecay::kSgTop_sChannel);
}

//____________________________________________________________________

void AtlSgTop_sChannelAnalysis::ReconstructTopDecays() {
    //
    // Rconstruct t->Wb decay candidates. ReconstructWDecay() and
    // ReconstructNeutrino() need to be called before.
    //
    // The candidates for all b-jets are added to the list of top
    // decays of the event. The list is sorted in descendent order (!)
    // w.r.t. the proximity of the top pole-mass of 172.5 GeV.
    //
    AtlWDecayLNu *W = (AtlWDecayLNu*)fEvent->GetWDecaysLNu()->At(0);
    TIter next(fBJets);
    AtlJet *bjet = 0;
    TLorentzVector p_top;
    while ( (bjet = (AtlJet*)next()) ) {
	p_top.SetPxPyPzE(W->Px() + bjet->Px(),
			 W->Py() + bjet->Py(),
			 W->Pz() + bjet->Pz(),
			 W->E()  + bjet->E());
	fEvent->AddTopDecay(W->Px() + bjet->Px(),
			    W->Py() + bjet->Py(),
			    W->Pz() + bjet->Pz(),
			    W->E()  + bjet->E(),
			    W, bjet, bjet->Px(),
			    bjet->Py(), bjet->Pz(), bjet->E(),
			    HepTopDecay::kSgTop_sChannel);
    }
    fEvent->GetTopDecays()->Sort();
}
	
//____________________________________________________________________

void AtlSgTop_sChannelAnalysis::ComputeBDTVariables() {
    //
    // Compute BDT input kinematic variables
    //
    AtlJet *bjet1 = GetBJet1st();
    Double_t bjet1_eta = bjet1->Eta();
    AtlJet *bjet2 = GetBJet2nd();
    Double_t bjet2_eta = bjet2->Eta();
    HepParticle *lep   = fLepton;
    Double_t lep_eta = lep->Eta();

    // loose leptons
    TIter next_looselepton(fObjTool->GetLooseLeptons());
    HepParticle *looselep   = 0;
    Int_t n_looseleps = 0;
    while ( (looselep = (HepParticle*)next_looselepton()) ) {
        if ( looselep->Pt() < 30.0 || TMath::Abs(looselep->Eta()) > 2.5 )
        n_looseleps++;
    } 
    fN_looseleptons = n_looseleps;

    // loose jets
    TIter next_jet(fEvent->GetJets(fObjTool->fJetsAuthor));
    //TIter next_jet(fJets);
    AtlJet *jet = 0;
    Int_t n_loosejets = 0;
    while ( (jet = (AtlJet*)next_jet()) ) {
        if ( jet->Pt() < 30.0 || TMath::Abs(jet->Eta()) > 2.5 )
        n_loosejets++;    
    }    
    fN_loosejets = n_loosejets;

    // Lepton Pt
    fPtLep = lep->Pt();

    // Lepton charge
    fQLep = lep->Charge();
    
    // Delta phi between top-quark candidate and b-jet
    HepTopDecay *top_bjet1 = GetTopDecayWith1stBJet();
    HepTopDecay *top_bjet2 = GetTopDecayWith2ndBJet();
    fDeltaPhiTopBJet2_BJet1 = TMath::Abs(top_bjet2->DeltaPhi(bjet1));
    fDeltaPhiTopBJet1_BJet2 = TMath::Abs(top_bjet1->DeltaPhi(bjet2));

    // Sum Et and Sum Pt
    fSumEtLep_MET = lep->Et() + fMET;

    AtlJet *bjet = 0;
    TIter next_bjet(fBJets);
    TVector2 vecPt(0., 0.);
    vecPt += lep->P().Vect().XYvector();
    vecPt += fEvent->GetEnergySum()->GetMissingEt();
    fSumEtLep_MET_BJets = fSumEtLep_MET;
    while ( (bjet = (AtlJet*)next_bjet()) ) {
	fSumEtLep_MET_BJets += bjet->Et();
	vecPt += bjet->P().Vect().XYvector();
    }
    fSumPt = vecPt.Mod();
    
    // Delta eta between lepton and leading b-jet
    fDeltaEtaLep_BJet1 = TMath::Abs(lep_eta - bjet1_eta);

    // Delta eta between lepton and sub-leading jet
    fDeltaEtaLep_BJet2 = TMath::Abs(lep_eta - bjet2_eta);

    // Delta eta between top decay with sub-leading b-jet and leading
    // b-jet
    fDeltaEtaTopBJet2_BJet1 = TMath::Abs(top_bjet2->Eta() - bjet1_eta);

    // Delta phi between leading and sub-leading b-jet
    fDeltaPhiBJet1_BJet2 = TMath::Abs(bjet1->DeltaPhi(bjet2));

    // Delta phi between lepton and MET
    fDeltaPhiLep_MET
	= TMath::Abs(TVector2::Phi_mpi_pi(lep->Phi() - fEvent->GetEnergySum()
					  ->MissingEt_Phi()));

    // Cos of angle between lepton momentum in W rest-frame and the W
    // momentum in the rest-frame of the top-quark candidate with the
    // leading b-jet
    AtlWDecayLNu *W = (AtlWDecayLNu*)fEvent->GetWDecaysLNu()->First();
    TVector3 boost_W = -W->P().BoostVector();
    TLorentzVector p_lep_Wframe(lep->P());
    p_lep_Wframe.Boost(boost_W);
    TVector3 boost_top1 = -top_bjet1->P().BoostVector();
    TLorentzVector p_W_topframe1(W->P());
    p_W_topframe1.Boost(boost_top1);
    fCosLepW_WTopBJet1
	= TMath::Cos(p_lep_Wframe.Angle(p_W_topframe1.Vect()));

    // Cos of angle between the lepton momentum in the top-quark
    // rest-frame and the top-quark momentum in the c.m.s. The used
    // top-quark is the candidate reconstructed using the leading
    // b-jet. The c.m.s vector is the sum of the top momentum and the
    // sub-leading b-jet momentum
    TLorentzVector p_lep_topframe1(lep->P());
    p_lep_topframe1.Boost(boost_top1);
    TLorentzVector p_cms_top1(top_bjet1->P());
    p_cms_top1 += bjet2->P();
    TVector3 boost_cms_top1 = -p_cms_top1.BoostVector();
    TLorentzVector p_top1_cms(top_bjet1->P());
    p_top1_cms.Boost(boost_cms_top1);
    fCosLepTopBJet1_TopBJet1CMS = TMath::Cos(p_lep_topframe1
						      .Angle(p_top1_cms.Vect()));

    // Cos of angle between the lepton momentum in the top-quark
    // rest-frame and the top-quark momentum in the c.m.s. The used
    // top-quark is the candidate reconstructed using the sub-leading
    // b-jet. The c.m.s vector is the sum of the top momentum and the
    // leading b-jet momentum
    TLorentzVector p_lep_topframe2(lep->P());
    TVector3 boost_top2 = -top_bjet2->P().BoostVector();
    p_lep_topframe2.Boost(boost_top2);
    TLorentzVector p_cms_top2(top_bjet2->P());
    p_cms_top2 += bjet1->P();
    TVector3 boost_cms_top2 = -p_cms_top2.BoostVector();
    TLorentzVector p_top2_cms(top_bjet2->P());
    p_top2_cms.Boost(boost_cms_top2);
    fCosLepTopBJet2_TopBJet2CMS = TMath::Cos(p_lep_topframe2
						      .Angle(p_top2_cms.Vect()));

    // Pt of b-jet from best recosntructed top decay
    HepTopDecay *top_best = GetTopDecayBest();
    fPt_BJetTop = top_best->GetBJetOrig()->Pt();
    
    // Delta eta between the rec. neutrino and the b-jet from the
    // non-top candidate
    HepTopDecay *top_2ndbest = GetTopDecay2ndBest();
    fDeltaEtaNu_BJetNonTop = TMath::Abs(W->GetNeutrino()->Eta()
					- top_2ndbest->GetBJetOrig()->Eta());

    // Vectoriell Pt sum of the leading and the sub-leading b-jet
    TVector2 vecPt_bjet12(0., 0.);
    vecPt_bjet12 += bjet1->P().Vect().XYvector();
    vecPt_bjet12 += bjet2->P().Vect().XYvector();
    fSumPtBJet1_BJet2 = vecPt_bjet12.Mod();

    // Cos of angle between MET and sub-leading b-jet
    TVector2 MET2d = fEvent->GetEnergySum()->GetMissingEt();
    TVector3 MET3d(MET2d.X(), MET2d.Y(), 0.);
    fCosMET_BJet2 = TMath::Cos(MET3d.Angle(bjet2->P().Vect()));

    // Invariant mass of lepton and sub-leading b-jet
    TLorentzVector p_lep_bjet2(lep->P());
    p_lep_bjet2 += bjet2->P();
    fMLep_BJet2 = p_lep_bjet2.M();

    // Additional variables, S.K.
    // for now not added to written tree (see SetBDTVariables())
    // Top Mass
    TLorentzVector p_top(GetTopDecayBest()->P());
    fMTop = p_top.M();

    // invariant mass of lepton and b-jet from top
    TLorentzVector p_lep_bjettop(lep->P());
    p_lep_bjettop += GetTopDecayBest()->GetBJetOrig()->P();
    fMLep_BJetTop = p_lep_bjettop.M();

    // invariant mass of top and other b-jet
    TLorentzVector p_top_bjetnontop(p_top);
    p_top_bjetnontop += GetTopDecay2ndBest()->GetBJetOrig()->P();
    fMTop_NonTopBJet = p_top_bjetnontop.M();
    
    // Transfer BDT variables to the event writer
    SetBDTVariables();
}

//____________________________________________________________________

void AtlSgTop_sChannelAnalysis::BookHistogramsBDT(AtlHistogramTool *htool) {
    //
    // Book histograms of BDT input variables using the histogram tool
    //

    // Number of loose leptons
    htool->Add("h_bdt_number_looseleptons", "number of loose leptons", 5, -0.5001, 4.5001,
               "n(loose leptons)", "Number of Entries");

    // Number of loose jets
    htool->Add("h_bdt_number_loosejets", "number of loose jets", 5, -0.5001, 4.5001,
               "n(loose jets)", "Number of Entries");

    // MET
    htool->Add("h_bdt_MET", "MET (BDT)", 40, 0., 200.,
               "MET [GeV]", "Number of Entries");
    // MtW
    htool->Add("h_bdt_MtW", "MtW (BDT)", 60, 0., 300.,
               "M_{t}(W) [GeV]", "Number of Entries");
    // Lepton Pt
    htool->Add("h_bdt_lep_pt", "Lepton Pt", 40, 0., 200.,
               "P_{t}^{lep} [GeV]", "Number of Entries");
    // Lepton Charge
    htool->Add("h_bdt_lep_charge", "Lepton charge", 2, -1.0001, 1.0001,
               "q{lep}", "Number of Entries");    
    // Sum Et = lepton Pt + MET
    htool->Add("h_bdt_sumEt_lep_met", "Sum Et, lepton Pt + MET", 60, 0., 300.,
               "#sum E_{t}(lepton, MET) [GeV]", "Number of Entries");
    // Sum Et = lepton Pt + MET + b-jets
    htool->Add("h_bdt_sumEt_lep_met_bjet", "Sum Et, lepton Pt + MET + BJets", 140, 100., 700.,
               "#sum E_{t}(lepton, MET, b-jets) [GeV]", "Number of Entries");
    // Sum Pt
    htool->Add("h_bdt_sumPt", "Sum Pt, lepton + MET + b-jets", 30, 0., 150.,
               "#sum P_{t}(lepton, MET, b-jets) [GeV]", "Number of Entries");
    // Delta Phi, top2 - leading b-jet
    htool->Add("h_bdt_DeltaPhi_TopBJet2_BJet1", "DeltaPhi", 25., 0., 4.,
               "#Delta#phi(top, leading b-jet) (rad)", "Number of Entries");
    // Delta Phi, top1 - sub-leading b-jet
    htool->Add("h_bdt_DeltaPhi_TopBJet1_BJet2", "DeltaPhi", 25, 0., 4.,
               "#Delta#phi(top, sub-leading b-jet) (rad)", "Number of Entries");
    // Delta eta, lepton - leading b-jet
    htool->Add("h_bdt_DeltaEta_Lep_BJet1", "Delta Eta", 32, 0., 4.,
               "#Delta#eta(lepton, leading b-jet", "Number of Entries");
    // Delta eta, lepton - sub-leading b-jet
    htool->Add("h_bdt_DeltaEta_Lep_BJet2", "Delta Eta", 32, 0., 4.,
               "#Delta#eta(lepton, sub-leading b-jet", "Number of Entries");
    // Delta eta, top2 - leading b-jet
    htool->Add("h_bdt_DeltaEta_TopBJet2_BJet1", "Delta Eta", 40, 0., 5.,
               "#Delta#eta(top, leading b-jet", "Number of Entries");
    // Delta phi, leading b-jet, sub-leading b-jet
    htool->Add("h_bdt_DeltaPhi_BJet1_BJet2", "Delta Phi", 20, 0., 4.,
               "#Delta#phi(leading b-jet, sub-leading b-jet) (rad)", "Number of Entries");
    // Delta phi, lepton - MET
    htool->Add("h_bdt_DeltaPhi_Lep_MET", "Delta Phi", 20, 0., 4.,
               "#Delta#phi(lepton, MET) (rad)", "Number of Entries");
    // Cos, LepW - Wtop
    htool->Add("h_bdt_Cos_LepW_WTopBJet1", "Cos", 20, -1., 1.,
               "cos#theta(lepton_{W-frame}, W_{top-frame})", "Number of Entries");
    // Cos, LepTop1 - Top1CMS
    htool->Add("h_bdt_Cos_LepTopBJet1_TopBJet1CMS", "Cos", 20, -1., 1.,
               "cos#theta(lepton_{top1-frame}, top1_{cms})", "Number of Entries");
    // Cos, LepTop2 - Top2CMS
    htool->Add("h_bdt_Cos_LepTopBJet2_TopBJet2CMS", "Cos", 20, -1., 1.,
               "cos#theta(lepton_{top2-frame}, top2_{cms})", "Number of Entries");
    // Pt, b-jet from top candidate
    htool->Add("h_bdt_PtBJet_Top", "PtBJet_Top", 50, 0., 250.,
               "P_{t}(b-jet from best top) [GeV]", "Number of Entries");
    // Delta eta, neutrino - b-jet from non-to candidate
    htool->Add("h_bdt_DeltaEtaNu_BJetNonTop", "DeltaEtaNu_BJetNonTop", 32, 0., 4.,
               "#Delta#eta(#nu, b-jet_{non-top})", "Number of Entries");
    // Sum Pt, b-jet1 + b-jet2
    htool->Add("h_bdt_SumPtBJet1_BJet2", "SumPtBJet1_BJet2", 25, 0., 250.,
               "#sum P_{t} (leading b-jet, sub-leading b-jet) [GeV]", "Number of Entries");
    // Cos, MET - sub-leading b-jet
    htool->Add("h_bdt_CosMET_BJet2", "CosMET_BJet2", 20, -1., 1.,
               "cos#theta(MT, sub-leading b-jet)", "Number of Entries");
    // Inv. mass, lepton + sub-leading b-jet
    htool->Add("h_bdt_MLep_BJet2", "MLep_BJet2", 60, 0., 300.,
               "M(lepton, sub-leading b-jet) [GeV]", "Number of Entries");
    // Top Mass
    htool->Add("h_bdt_MTop", "M_top", 100, 0., 500.,
               "M(top) [GeV]", "Number of Entries");
    // invariant mass of lepton and b-jet from top
    htool->Add("h_bdt_MLep_BJetTop", "MLep_BJetTop", 60, 0., 300.,
               "M(lepton, b-jet from top) [GeV]", "Number of Entries");
    // invariant mass of top and other b-jet
    htool->Add("h_bdt_MTop_NonTopBJet", "MTop_NonTopBJet", 200, 0., 1000.,
               "M(top, b-jet not from top) [GeV]", "Number of Entries");

}

//____________________________________________________________________

void AtlSgTop_sChannelAnalysis::FillHistogramsBDT(AtlHistogramTool *htool) {
    //
    // Fill histograms of BDT input variables using the histogram tool
    //
    Double_t W = GetTagEvtWeight();
    htool->Fill("h_bdt_number_looseleptons", fN_looseleptons, W);
    htool->Fill("h_bdt_number_loosejets", fN_loosejets, W);
    htool->Fill("h_bdt_MET", fMET, W);
    htool->Fill("h_bdt_MtW", fMtW, W);
    htool->Fill("h_bdt_lep_pt", fPtLep, W);
    htool->Fill("h_bdt_lep_charge", fQLep, W);
    htool->Fill("h_bdt_sumEt_lep_met", fSumEtLep_MET, W);
    htool->Fill("h_bdt_sumEt_lep_met_bjet", fSumEtLep_MET_BJets, W);
    htool->Fill("h_bdt_sumPt", fSumPt, W);
    htool->Fill("h_bdt_DeltaPhi_TopBJet2_BJet1", fDeltaPhiTopBJet2_BJet1, W);
    htool->Fill("h_bdt_DeltaPhi_TopBJet1_BJet2", fDeltaPhiTopBJet1_BJet2, W);
    htool->Fill("h_bdt_DeltaEta_Lep_BJet1", fDeltaEtaLep_BJet1, W);
    htool->Fill("h_bdt_DeltaEta_Lep_BJet2", fDeltaEtaLep_BJet2, W);
    htool->Fill("h_bdt_DeltaEta_TopBJet2_BJet1", fDeltaEtaTopBJet2_BJet1, W);
    htool->Fill("h_bdt_DeltaPhi_BJet1_BJet2", fDeltaPhiBJet1_BJet2, W);
    htool->Fill("h_bdt_DeltaPhi_Lep_MET", fDeltaPhiLep_MET, W);
    htool->Fill("h_bdt_Cos_LepW_WTopBJet1", fCosLepW_WTopBJet1, W);
    htool->Fill("h_bdt_Cos_LepTopBJet1_TopBJet1CMS", fCosLepTopBJet1_TopBJet1CMS, W);
    htool->Fill("h_bdt_Cos_LepTopBJet2_TopBJet2CMS", fCosLepTopBJet2_TopBJet2CMS, W);
    htool->Fill("h_bdt_PtBJet_Top", fPt_BJetTop, W);
    htool->Fill("h_bdt_DeltaEtaNu_BJetNonTop", fDeltaEtaNu_BJetNonTop, W);
    htool->Fill("h_bdt_SumPtBJet1_BJet2", fSumPtBJet1_BJet2, W);
    htool->Fill("h_bdt_CosMET_BJet2", fCosMET_BJet2, W);
    htool->Fill("h_bdt_MLep_BJet2", fMLep_BJet2, W);
    htool->Fill("h_bdt_MTop", fMTop, W);
    htool->Fill("h_bdt_MLep_BJetTop", fMLep_BJetTop, W);
    htool->Fill("h_bdt_MTop_NonTopBJet", fMTop_NonTopBJet, W);
}

// ____________________________________________________________________

void AtlSgTop_sChannelAnalysis::SetBDTVariables() {
    //
    // Provide BDT variables to the LPSC event writer (if any)
    //
    if ( fEvtWriter == 0 ) return;

    AtlEvtWriterLPSC* EvtWriter = dynamic_cast<AtlEvtWriterLPSC*>(fEvtWriter);
    EvtWriter->fN_looseleptons = fN_looseleptons;
    EvtWriter->fN_loosejets = fN_loosejets;
    EvtWriter->fMET = fMET;
    EvtWriter->fMtW = fMtW;
    EvtWriter->fPtLep = fPtLep; 
    EvtWriter->fQLep = fQLep; 
    EvtWriter->fDeltaPhiTopBJet2_BJet1 = fDeltaPhiTopBJet2_BJet1;
    EvtWriter->fDeltaPhiTopBJet1_BJet2 = fDeltaPhiTopBJet1_BJet2;
    EvtWriter->fSumEtLep_MET = fSumEtLep_MET;
    EvtWriter->fSumEtLep_MET_BJets = fSumEtLep_MET_BJets;
    EvtWriter->fSumPt = fSumPt;
    EvtWriter->fDeltaEtaLep_BJet1 = fDeltaEtaLep_BJet1;
    EvtWriter->fDeltaEtaLep_BJet2 = fDeltaEtaLep_BJet2;
    EvtWriter->fDeltaEtaTopBJet2_BJet1 = fDeltaEtaTopBJet2_BJet1;
    EvtWriter->fDeltaPhiBJet1_BJet2 = fDeltaPhiBJet1_BJet2;
    EvtWriter->fDeltaPhiLep_MET = fDeltaPhiLep_MET;
    EvtWriter->fCosLepW_WTopBJet1 = fCosLepW_WTopBJet1;
    EvtWriter->fCosLepTopBJet1_TopBJet1CMS = fCosLepTopBJet1_TopBJet1CMS;
    EvtWriter->fCosLepTopBJet2_TopBJet2CMS = fCosLepTopBJet2_TopBJet2CMS;
    EvtWriter->fPt_BJetTop = fPt_BJetTop;
    EvtWriter->fDeltaEtaNu_BJetNonTop = fDeltaEtaNu_BJetNonTop;
    EvtWriter->fSumPtBJet1_BJet2 = fSumPtBJet1_BJet2;
    EvtWriter->fCosMET_BJet2 = fCosMET_BJet2;
    EvtWriter->fMLep_BJet2 = fMLep_BJet2;
    EvtWriter->fMTop = fMTop;
    EvtWriter->fMLep_BJetTop = fMLep_BJetTop;
    EvtWriter->fMTop_NonTopBJet = fMTop_NonTopBJet;
}

// ____________________________________________________________________

void AtlSgTop_sChannelAnalysis::FillHistogramsMCTruthStudy() {
    //
    // Fill histograms for mc truth study
    //

    // Perform truth matching
    DoTruthMatching();
    
    AtlJet *Jetlead     = (AtlJet*) fJets->At(0);
    AtlJet *Jet2ndlead  = (AtlJet*) fJets->At(1);
    
    // Highest pt leptons
    HepParticle *lepton = (HepParticle*) fLeptons->At(0);
    HepParticle *loose_lepton = (HepParticle*)fLooseLeptons->At(0);

    // MC truth level jets
    TClonesArray *mcjets = fEvent->GetJets(AtlJet::kMCAntiKt4Hadron);

    // Fill Jet truth info:
    fHist_mc_jet_truth_label->Fill(Jetlead->GetTruthFlavour(), GetTagEvtWeight());
    fHist_mc_jet_truth_label->Fill(Jet2ndlead->GetTruthFlavour(), GetTagEvtWeight());
    
    // Fill Decay Channel
    fHist_mc_DecayChannels->AddBinContent(fTtbarDecayMode, GetTagEvtWeight());
    fHist_mc_MET->Fill(fTtbarDecayMode, fMET, GetTagEvtWeight());

    // Get Number of tight and loose leptons
    Int_t Ntight = fLeptons->GetEntries();
    Int_t Nloose = fLooseLeptons->GetEntries();
    if ( Ntight + Nloose > 1 ) {
	fHist_mc_DecayChannels_AllVeto->AddBinContent(fTtbarDecayMode, GetTagEvtWeight());
    } else {
	fHist_mc_MET_nonVeto->Fill(fTtbarDecayMode, fMET, GetTagEvtWeight());
    }
    
    if ( Ntight + fLooseTaus->GetEntries() > 1 ) {
	fHist_mc_DecayChannels_TauVeto->AddBinContent(fTtbarDecayMode, GetTagEvtWeight());
    }

    // Only correct if taus are considered
    if ( Ntight + Nloose - fLooseTaus->GetEntries() > 1 ) {
	fHist_mc_DecayChannels_Veto->AddBinContent(fTtbarDecayMode, GetTagEvtWeight());
    }
    
    // Does a true lepton matches the trigger lepton?
    // (within DeltaR < 0.4)
    // (truth matching was already performed, check if a true particle was found)
    if ( fMCTightLepton != 0 ) {
	fHist_mc_TriggerMatch->Fill(fTtbarDecayMode, 1., GetTagEvtWeight());
    } else {
	fHist_mc_TriggerMatch->Fill(fTtbarDecayMode, 2., GetTagEvtWeight());
    }
    
    // Check reconstruction efficiencies
    // --> Is the ttbar event fully reconstructed using loose leptons?
    // 1: both leptons are reconstructed
    // 2: only tight lepton reconstructed
    // 3: only loose lepton reconstructed
    // 4: no true lepton reconstructed
    if ( fMCTightLepton != 0 && fMCLooseLepton != 0 ) {
	fHist_mc_RecoEff->Fill(fTtbarDecayMode, 1., GetTagEvtWeight());
    } else if ( fMCTightLepton != 0 ) {
	fHist_mc_RecoEff->Fill(fTtbarDecayMode, 2., GetTagEvtWeight());
    } else if ( fMCLooseLepton != 0 ) {
	fHist_mc_RecoEff->Fill(fTtbarDecayMode, 3., GetTagEvtWeight());
    } else {
    	fHist_mc_RecoEff->Fill(fTtbarDecayMode, 4., GetTagEvtWeight());
    }

    // Fill DeltaR between reco lepton (for loose and tight)
    // and _closest_ (!) mc truth lepton
    // take closest, since loose/tight might not be matched!
    Double_t DeltaR_min_lepton = 99.;
    Double_t DeltaR_min_looselepton = 99.;

    if ( fMCLeptonPlus != 0 )
	DeltaR_min_lepton = fMCLeptonPlus->DeltaR(lepton);
    if ( fMCLeptonMinus != 0 )
	if ( fMCLeptonMinus->DeltaR(lepton) < DeltaR_min_lepton )
	    DeltaR_min_lepton = fMCLeptonMinus->DeltaR(lepton);

    if ( loose_lepton != 0 ) {
	if ( fMCLeptonPlus != 0 )
	    DeltaR_min_looselepton = fMCLeptonPlus->DeltaR(loose_lepton);
	if ( fMCLeptonMinus != 0 )
	    if ( fMCLeptonMinus->DeltaR(loose_lepton) < DeltaR_min_looselepton )
		DeltaR_min_looselepton = fMCLeptonMinus->DeltaR(loose_lepton);
    }
    
    if ( DeltaR_min_lepton != 99. )
	fHist_mc_DeltaR_tight_lepton->Fill(fTtbarDecayMode, DeltaR_min_lepton, GetTagEvtWeight());
    if ( DeltaR_min_looselepton != 99. )
	fHist_mc_DeltaR_loose_lepton->Fill(fTtbarDecayMode, DeltaR_min_looselepton, GetTagEvtWeight());


    // Di-lepton kinematics:
    // =====================
    // High / Low pt lepton
    // DeltaR between truth particles (di-lepton events only)
    // ------------------------------------------------------
    HepMCParticle *MCLeadLepton = 0;
    HepMCParticle *MC2ndLeadLepton = 0;
    HepMCParticle *MCLeadLep_bQuark = 0;
    HepMCParticle *MC2ndLeadLep_bQuark = 0;
    TList *mc_leptons = new TList;
    TList *match = 0;
    switch (fTtbarDecayMode) {
	// Di-Lepton events
	// -----------------
	case kElEl:
	case kMuMu:
	case kElMu:
	case kElTau:
	case kMuTau:
	case kTauTau:
	    
	    mc_leptons->Add(fMCLeptonPlus);
	    mc_leptons->Add(fMCLeptonMinus);
	    
	    match = fEvent->FindMatchedParticles(lepton,
						 mc_leptons,
						 0.2,  // delta R < 0.2
						 0.1); // pt fraction, 10%
	    if ( fMCLeptonPlus == match->At(0) ) {
		fHist_mc_truth_2ndLeadLepton_Pt->Fill(fTtbarDecayMode, fMCLeptonMinus->Pt(), GetTagEvtWeight());
	    } else if ( fMCLeptonMinus == match->At(0) ) {
		fHist_mc_truth_2ndLeadLepton_Pt->Fill(fTtbarDecayMode, fMCLeptonPlus->Pt(), GetTagEvtWeight());
	    }
	    
	    // Find high pt lepton
	    if ( fMCLeptonPlus->Pt() > fMCLeptonMinus->Pt() ) {
		MCLeadLepton = fMCLeptonPlus;
		MC2ndLeadLepton = fMCLeptonMinus;
	    } else {
		MCLeadLepton = fMCLeptonMinus;
		MC2ndLeadLepton = fMCLeptonPlus;
	    }
	    
	    if ( MCLeadLepton->Charge() > 0 ) {
		// Q+ lepton --> b-quark
		MCLeadLep_bQuark = fMCbQuark;
		MC2ndLeadLep_bQuark = fMCAntibQuark;
	    } else {
		// Q+ lepton --> b-quark
		MCLeadLep_bQuark = fMCAntibQuark;
		MC2ndLeadLep_bQuark = fMCbQuark;
	    }
	    
	    // Fill pt histograms (truth level)
	    fHist_mc_truth_LeadLepton_Pt->Fill(fTtbarDecayMode, MCLeadLepton->Pt(), GetTagEvtWeight());
	    if ( fMCLooseLepton != 0 )
		fHist_mc_truth_2ndLeadLepton_Pt_matched->Fill(fTtbarDecayMode, MC2ndLeadLepton->Pt(), GetTagEvtWeight());
	    else
		fHist_mc_truth_2ndLeadLepton_Pt_unmatched->Fill(fTtbarDecayMode, MC2ndLeadLepton->Pt(), GetTagEvtWeight());

	    // Fill pt histograms (reco level)
	    fHist_mc_reco_LeadLepton_Pt->Fill(fTtbarDecayMode, lepton->Pt(), GetTagEvtWeight());
	    if ( loose_lepton != 0 ) {
		fHist_mc_reco_2ndLeadLepton_Pt->Fill(fTtbarDecayMode, loose_lepton->Pt(), GetTagEvtWeight());
	    }

	    // Fill pt resolution histogram
	    fHist_mc_reco_LeadLepton_Pt_res->Fill(fTtbarDecayMode,
						  (MCLeadLepton->Pt() - lepton->Pt())/MCLeadLepton->Pt(),
						  GetTagEvtWeight());
	    if ( loose_lepton != 0 ) {
		fHist_mc_reco_2ndLeadLepton_Pt_res->Fill(fTtbarDecayMode,
							 (MC2ndLeadLepton->Pt() - loose_lepton->Pt())/MC2ndLeadLepton->Pt(),
							 GetTagEvtWeight());
	    }
	    
	    // Fill delta R histograms
	    // -----------------------
	    // Find opposite sign (OS) b-Quark and same sign b-Quark (SS)
	    // OS b-Quark should be closer (?) to lepton since it's from the same
	    // top quark decay...
	    
	    // Leading lepton and OS b-quark
	    fHist_mc_DeltaR_LeadLepton_bQuarkOS->Fill(fTtbarDecayMode,
						      MCLeadLepton->DeltaR(MCLeadLep_bQuark),
						      GetTagEvtWeight());
	    
	    // Leading lepton and SS b-quark
	    fHist_mc_DeltaR_LeadLepton_bQuarkSS->Fill(fTtbarDecayMode,
						      MCLeadLepton->DeltaR(MC2ndLeadLep_bQuark),
						      GetTagEvtWeight());
	    
	    // Leading lepton and OS b-quark
	    fHist_mc_DeltaR_2ndLeadLepton_bQuarkOS->Fill(fTtbarDecayMode,
							 MC2ndLeadLepton->DeltaR(MC2ndLeadLep_bQuark),
							 GetTagEvtWeight());
	    
	    // Leading lepton and SS b-quark
	    fHist_mc_DeltaR_2ndLeadLepton_bQuarkSS->Fill(fTtbarDecayMode,
							 MC2ndLeadLepton->DeltaR(MCLeadLep_bQuark),
							 GetTagEvtWeight());

	    // Leading lepton and 2nd leading lepton
	    fHist_mc_DeltaR_LeadLepton_2ndLeadLepton->Fill(fTtbarDecayMode,
							   MCLeadLepton->DeltaR(MC2ndLeadLepton),
							   GetTagEvtWeight());

	    // Compare unmatched taus to mc truth jets
	    if ( fMCLooseLepton != 0 ) {
		AtlJet *mcjet = 0;
		AtlJet *closest_mcjet = 0;
		Double_t deltaR = 99.;
		Double_t deltaR_min = 99.;
		TIter next_jet(mcjets);
		while( (mcjet = (AtlJet*)next_jet()) ) {
		    deltaR = fMCLooseLepton->DeltaR(mcjet);
		    if ( deltaR < deltaR_min ) {
			deltaR_min = deltaR;
			closest_mcjet = mcjet;
		    }
		}
		fHist_mc_DeltaR_unmatchedMCLepton_truthjet->Fill(fTtbarDecayMode,
								 deltaR_min,
								 GetTagEvtWeight());
		fHist_mc_unmatchedMCLepton_truthjet_pt->Fill(fTtbarDecayMode,
							     closest_mcjet->Pt(),
							     GetTagEvtWeight());
	    }
	    
	default:
	    ;// Do nothing
    }
    delete mc_leptons;
    if ( match != 0 ) { delete match; }
    
}

// ____________________________________________________________________

void AtlSgTop_sChannelAnalysis::BookHistogramsMCTruthStudy() {
    //
    // Book histograms for mc truth study
    //

    gDirectory->mkdir("MCTruthInfo");
    gDirectory->cd("MCTruthInfo");

    fHist_mc_DecayChannels = new TH1F("h_mc_DecayChannels", "Decay channels of ttbar events",
				      11, 0.5, 11.5);
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(7,  "e^{#pm}, q#bar{q}");
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(8,  "#mu^{#pm}, q#bar{q}");
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(9,  "#tau^{#pm}, q#bar{q}");
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(10, "q#bar{q}, q#bar{q}");
    fHist_mc_DecayChannels->GetXaxis()->SetBinLabel(11, "Unknown");    
    fHist_mc_DecayChannels->SetYTitle("Number of Entries");

    fHist_mc_DecayChannels_Veto = new TH1F("h_mc_DecayChannels_Veto", "Decay channels of ttbar events",
				      11, 0.5, 11.5);
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(7,  "e^{#pm}, q#bar{q}");
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(8,  "#mu^{#pm}, q#bar{q}");
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(9,  "#tau^{#pm}, q#bar{q}");
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(10, "q#bar{q}, q#bar{q}");
    fHist_mc_DecayChannels_Veto->GetXaxis()->SetBinLabel(11, "Unknown");    
    fHist_mc_DecayChannels_Veto->SetYTitle("Number of Entries");

    fHist_mc_DecayChannels_TauVeto = new TH1F("h_mc_DecayChannels_TauVeto", "Decay channels of ttbar events",
					   11, 0.5, 11.5);
    fHist_mc_DecayChannels_TauVeto->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DecayChannels_TauVeto->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DecayChannels_TauVeto->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DecayChannels_TauVeto->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DecayChannels_TauVeto->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DecayChannels_TauVeto->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DecayChannels_TauVeto->GetXaxis()->SetBinLabel(7,  "e^{#pm}, q#bar{q}");
    fHist_mc_DecayChannels_TauVeto->GetXaxis()->SetBinLabel(8,  "#mu^{#pm}, q#bar{q}");
    fHist_mc_DecayChannels_TauVeto->GetXaxis()->SetBinLabel(9,  "#tau^{#pm}, q#bar{q}");
    fHist_mc_DecayChannels_TauVeto->GetXaxis()->SetBinLabel(10, "q#bar{q}, q#bar{q}");
    fHist_mc_DecayChannels_TauVeto->GetXaxis()->SetBinLabel(11, "Unknown");    
    fHist_mc_DecayChannels_TauVeto->SetYTitle("Number of Entries");

    fHist_mc_DecayChannels_AllVeto = new TH1F("h_mc_DecayChannels_AllVeto", "Decay channels of ttbar events",
					   11, 0.5, 11.5);
    fHist_mc_DecayChannels_AllVeto->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DecayChannels_AllVeto->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DecayChannels_AllVeto->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DecayChannels_AllVeto->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DecayChannels_AllVeto->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DecayChannels_AllVeto->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DecayChannels_AllVeto->GetXaxis()->SetBinLabel(7,  "e^{#pm}, q#bar{q}");
    fHist_mc_DecayChannels_AllVeto->GetXaxis()->SetBinLabel(8,  "#mu^{#pm}, q#bar{q}");
    fHist_mc_DecayChannels_AllVeto->GetXaxis()->SetBinLabel(9,  "#tau^{#pm}, q#bar{q}");
    fHist_mc_DecayChannels_AllVeto->GetXaxis()->SetBinLabel(10, "q#bar{q}, q#bar{q}");
    fHist_mc_DecayChannels_AllVeto->GetXaxis()->SetBinLabel(11, "Unknown");    
    fHist_mc_DecayChannels_AllVeto->SetYTitle("Number of Entries");

    fHist_mc_jet_truth_label = new TH1F("h_mc_jet_truth_label", "Truth label of jets",
					5, -0.5, 4.5);
    fHist_mc_jet_truth_label->GetXaxis()->SetBinLabel(1,  "Unknown");
    fHist_mc_jet_truth_label->GetXaxis()->SetBinLabel(2,  "Light");
    fHist_mc_jet_truth_label->GetXaxis()->SetBinLabel(3,  "C");    
    fHist_mc_jet_truth_label->GetXaxis()->SetBinLabel(4,  "B");
    fHist_mc_jet_truth_label->GetXaxis()->SetBinLabel(5,  "#tau");
    fHist_mc_jet_truth_label->SetYTitle("Number of Entries");

    fHist_mc_TriggerMatch = new TH2F("h_mc_TriggerMatch", "Reconstructed Lepton matches Trigger",
				     11, 0.5, 11.5, 2, 0.5, 2.5);
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(7,  "e^{#pm}, q#bar{q}");
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(8,  "#mu^{#pm}, q#bar{q}");
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(9,  "#tau^{#pm}, q#bar{q}");
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(10, "q#bar{q}, q#bar{q}");
    fHist_mc_TriggerMatch->GetXaxis()->SetBinLabel(11, "Unknown");    
    fHist_mc_TriggerMatch->GetYaxis()->SetBinLabel(1, "Matched");
    fHist_mc_TriggerMatch->GetYaxis()->SetBinLabel(2, "Unmatched");
    fHist_mc_TriggerMatch->SetZTitle("Number of Entries");
    
    fHist_mc_MET = new TH2F("h_mc_MET", "MET distribution per decay channel",
			    11, 0.5, 11.5, 50, 0., 200.);
    fHist_mc_MET->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_MET->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_MET->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_MET->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_MET->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_MET->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_MET->GetXaxis()->SetBinLabel(7,  "e^{#pm}, q#bar{q}");
    fHist_mc_MET->GetXaxis()->SetBinLabel(8,  "#mu^{#pm}, q#bar{q}");
    fHist_mc_MET->GetXaxis()->SetBinLabel(9,  "#tau^{#pm}, q#bar{q}");
    fHist_mc_MET->GetXaxis()->SetBinLabel(10, "q#bar{q}, q#bar{q}");
    fHist_mc_MET->GetXaxis()->SetBinLabel(11, "Unknown");    
    fHist_mc_MET->GetYaxis()->SetTitle("MET");
    fHist_mc_MET->SetZTitle("Number of Entries");

    fHist_mc_MET_nonVeto = new TH2F("h_mc_MET_nonVeto", "MET distribution per decay channel after veto",
			    11, 0.5, 11.5, 50, 0., 200.);
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(7,  "e^{#pm}, q#bar{q}");
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(8,  "#mu^{#pm}, q#bar{q}");
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(9,  "#tau^{#pm}, q#bar{q}");
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(10, "q#bar{q}, q#bar{q}");
    fHist_mc_MET_nonVeto->GetXaxis()->SetBinLabel(11, "Unknown");    
    fHist_mc_MET_nonVeto->GetYaxis()->SetTitle("MET");
    fHist_mc_MET_nonVeto->SetZTitle("Number of Entries");

    // Reconstructed Efficiencies
    fHist_mc_RecoEff = new TH2F("h_mc_RecoEff", "Reconstructed Efficiency",
				11, 0.5, 11.5, 4, 0.5, 4.5);
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(7,  "e^{#pm}, q#bar{q}");
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(8,  "#mu^{#pm}, q#bar{q}");
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(9,  "#tau^{#pm}, q#bar{q}");
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(10, "q#bar{q}, q#bar{q}");
    fHist_mc_RecoEff->GetXaxis()->SetBinLabel(11, "Unknown");    
    fHist_mc_RecoEff->GetYaxis()->SetBinLabel(1, "2 lep");
    fHist_mc_RecoEff->GetYaxis()->SetBinLabel(2, "only tight");
    fHist_mc_RecoEff->GetYaxis()->SetBinLabel(3, "only loose");
    fHist_mc_RecoEff->GetYaxis()->SetBinLabel(4, "no lep");

    
    // DeltaR per channel (reco, truth)
    fHist_mc_DeltaR_tight_lepton = new TH2F("h_mc_DeltaR_tight_lepton", "DeltaR(reco, truth)",
					    11, 0.5, 11.5, 50, 0., 3.);
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(7,  "e^{#pm}, q#bar{q}");
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(8,  "#mu^{#pm}, q#bar{q}");
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(9,  "#tau^{#pm}, q#bar{q}");
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(10, "q#bar{q}, q#bar{q}");
    fHist_mc_DeltaR_tight_lepton->GetXaxis()->SetBinLabel(11, "Unknown");    
    fHist_mc_DeltaR_tight_lepton->GetYaxis()->SetTitle("#Delta R");

    // DeltaR per channel (reco, truth) 
    fHist_mc_DeltaR_loose_lepton = new TH2F("h_mc_DeltaR_loose_lepton", "DeltaR(reco, truth)",
					    11, 0.5, 11.5, 50, 0., 3.);
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(7,  "e^{#pm}, q#bar{q}");
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(8,  "#mu^{#pm}, q#bar{q}");
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(9,  "#tau^{#pm}, q#bar{q}");
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(10, "q#bar{q}, q#bar{q}");
    fHist_mc_DeltaR_loose_lepton->GetXaxis()->SetBinLabel(11, "Unknown");    
    fHist_mc_DeltaR_loose_lepton->GetYaxis()->SetTitle("#Delta R");

    gDirectory->mkdir("Dilepton_kinematics");
    gDirectory->cd("Dilepton_kinematics");

    gDirectory->mkdir("truth_level");
    gDirectory->cd("truth_level");
    
    // DeltaR between truth particles
    fHist_mc_DeltaR_LeadLepton_bQuarkOS = new TH2F("h_mc_DeltaR_LeadLepton_bQuarkOS", "DeltaR(lep, b-quark)",
						   6, 0.5, 6.5, 50, 0., 3.);
    fHist_mc_DeltaR_LeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DeltaR_LeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DeltaR_LeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DeltaR_LeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DeltaR_LeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_LeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_LeadLepton_bQuarkOS->GetYaxis()->SetTitle("#Delta R");
    
    fHist_mc_DeltaR_LeadLepton_bQuarkSS = new TH2F("h_mc_DeltaR_LeadLepton_bQuarkSS", "DeltaR(lep, b-quark)",
						   6, 0.5, 6.5, 50, 0., 3.);
    fHist_mc_DeltaR_LeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DeltaR_LeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DeltaR_LeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DeltaR_LeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DeltaR_LeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_LeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_LeadLepton_bQuarkSS->GetYaxis()->SetTitle("#Delta R");

    fHist_mc_DeltaR_2ndLeadLepton_bQuarkOS = new TH2F("h_mc_DeltaR_2ndLeadLepton_bQuarkOS", "DeltaR(lep, b-quark)",
						      6, 0.5, 6.5, 50, 0., 3.);
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkOS->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkOS->GetYaxis()->SetTitle("#Delta R");

    fHist_mc_DeltaR_2ndLeadLepton_bQuarkSS = new TH2F("h_mc_DeltaR_2ndLeadLepton_bQuarkSS", "DeltaR(lep, b-quark)",
						      6, 0.5, 6.5, 50, 0., 3.);
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkSS->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_2ndLeadLepton_bQuarkSS->GetYaxis()->SetTitle("#Delta R");

    fHist_mc_DeltaR_LeadLepton_2ndLeadLepton = new TH2F("h_mc_DeltaR_LeadLepton_2ndLeadLepton", "DeltaR(lep1, lep2)",
							6, 0.5, 6.5, 50, 0., 3.);
    fHist_mc_DeltaR_LeadLepton_2ndLeadLepton->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DeltaR_LeadLepton_2ndLeadLepton->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DeltaR_LeadLepton_2ndLeadLepton->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DeltaR_LeadLepton_2ndLeadLepton->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DeltaR_LeadLepton_2ndLeadLepton->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_LeadLepton_2ndLeadLepton->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_LeadLepton_2ndLeadLepton->GetYaxis()->SetTitle("#Delta R");

    fHist_mc_truth_LeadLepton_Pt = new TH2F("h_mc_truth_LeadLepton_Pt", "Leading Lepton Pt (MC truth info)",
					    6, 0.5, 6.5, 50, 0., 100.);
    fHist_mc_truth_LeadLepton_Pt->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_truth_LeadLepton_Pt->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_truth_LeadLepton_Pt->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_truth_LeadLepton_Pt->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_truth_LeadLepton_Pt->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_truth_LeadLepton_Pt->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_truth_LeadLepton_Pt->GetYaxis()->SetTitle("Leading lepton p_{T} (MC truth info)");

    fHist_mc_truth_2ndLeadLepton_Pt_matched = new TH2F("h_mc_truth_2ndLeadLepton_Pt_matched", "Leading Lepton Pt (MC truth info), reco matched",
						  6, 0.5, 6.5, 50, 0., 100.);
    fHist_mc_truth_2ndLeadLepton_Pt_matched->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_truth_2ndLeadLepton_Pt_matched->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_truth_2ndLeadLepton_Pt_matched->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_truth_2ndLeadLepton_Pt_matched->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_truth_2ndLeadLepton_Pt_matched->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_truth_2ndLeadLepton_Pt_matched->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_truth_2ndLeadLepton_Pt_matched->GetYaxis()->SetTitle("2ndLeading lepton p_{T} (MC truth info)");

    fHist_mc_truth_2ndLeadLepton_Pt_unmatched = new TH2F("h_mc_truth_2ndLeadLepton_Pt_unmatched", "Leading Lepton Pt (MC truth info), unmatched",
							 6, 0.5, 6.5, 50, 0., 100.);
    fHist_mc_truth_2ndLeadLepton_Pt_unmatched->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_truth_2ndLeadLepton_Pt_unmatched->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_truth_2ndLeadLepton_Pt_unmatched->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_truth_2ndLeadLepton_Pt_unmatched->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_truth_2ndLeadLepton_Pt_unmatched->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_truth_2ndLeadLepton_Pt_unmatched->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_truth_2ndLeadLepton_Pt_unmatched->GetYaxis()->SetTitle("2ndLeading lepton p_{T} (MC truth info)");

    fHist_mc_truth_2ndLeadLepton_Pt = new TH2F("h_mc_truth_2ndLeadLepton_Pt", "Leading Lepton Pt (MC truth info), reco matched",
						  6, 0.5, 6.5, 50, 0., 100.);
    fHist_mc_truth_2ndLeadLepton_Pt->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_truth_2ndLeadLepton_Pt->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_truth_2ndLeadLepton_Pt->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_truth_2ndLeadLepton_Pt->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_truth_2ndLeadLepton_Pt->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_truth_2ndLeadLepton_Pt->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_truth_2ndLeadLepton_Pt->GetYaxis()->SetTitle("2ndLeading lepton p_{T} (MC truth info)");

    
    // Truth level jets
    fHist_mc_DeltaR_unmatchedMCLepton_truthjet = new TH2F("h_mc_DeltaR_unmatchedMCLepton_truthjet", "DeltaR(unmatched MC lepton, truth jet)",
							  6, 0.5, 6.5, 50, 0., 3.);
    fHist_mc_DeltaR_unmatchedMCLepton_truthjet->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_DeltaR_unmatchedMCLepton_truthjet->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_DeltaR_unmatchedMCLepton_truthjet->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_DeltaR_unmatchedMCLepton_truthjet->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_DeltaR_unmatchedMCLepton_truthjet->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_unmatchedMCLepton_truthjet->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_DeltaR_unmatchedMCLepton_truthjet->GetYaxis()->SetTitle("#DeltaR");

    fHist_mc_unmatchedMCLepton_truthjet_pt = new TH2F("h_mc_unmatchedMCLepton_truthjet_pt", "Pt of closest mcjet to unmatched MC lepton",
						      6, 0.5, 6.5, 50., 0., 100.);
    fHist_mc_unmatchedMCLepton_truthjet_pt->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_unmatchedMCLepton_truthjet_pt->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_unmatchedMCLepton_truthjet_pt->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_unmatchedMCLepton_truthjet_pt->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_unmatchedMCLepton_truthjet_pt->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_unmatchedMCLepton_truthjet_pt->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_unmatchedMCLepton_truthjet_pt->GetYaxis()->SetTitle("jet p_{T}");
    
    gDirectory->cd("..");
    
    gDirectory->mkdir("reco_level");
    gDirectory->cd("reco_level");

    fHist_mc_reco_LeadLepton_Pt = new TH2F("h_mc_reco_LeadLepton_Pt", "Leading Lepton Pt (reco)",
					    6, 0.5, 6.5, 50, 0., 100.);
    fHist_mc_reco_LeadLepton_Pt->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_reco_LeadLepton_Pt->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_reco_LeadLepton_Pt->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_reco_LeadLepton_Pt->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_reco_LeadLepton_Pt->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_reco_LeadLepton_Pt->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_reco_LeadLepton_Pt->GetYaxis()->SetTitle("Leading lepton p_{T} (reco)");
    
    fHist_mc_reco_2ndLeadLepton_Pt = new TH2F("h_mc_reco_2ndLeadLepton_Pt", "Leading Lepton Pt (reco)",
					      6, 0.5, 6.5, 50, 0., 100.);
    fHist_mc_reco_2ndLeadLepton_Pt->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_reco_2ndLeadLepton_Pt->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_reco_2ndLeadLepton_Pt->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_reco_2ndLeadLepton_Pt->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_reco_2ndLeadLepton_Pt->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_reco_2ndLeadLepton_Pt->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_reco_2ndLeadLepton_Pt->GetYaxis()->SetTitle("2ndLeading lepton p_{T} (reco)");

    gDirectory->cd("..");

    fHist_mc_reco_LeadLepton_Pt_res = new TH2F("h_mc_reco_LeadLepton_Pt_res", "Leading Lepton Pt_res (reco)",
					       6, 0.5, 6.5, 50, -1., 1.);
    fHist_mc_reco_LeadLepton_Pt_res->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_reco_LeadLepton_Pt_res->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_reco_LeadLepton_Pt_res->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_reco_LeadLepton_Pt_res->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_reco_LeadLepton_Pt_res->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_reco_LeadLepton_Pt_res->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_reco_LeadLepton_Pt_res->GetYaxis()->SetTitle("Resolution (true - reco)/true p_{T}");
    
    fHist_mc_reco_2ndLeadLepton_Pt_res = new TH2F("h_mc_reco_2ndLeadLepton_Pt_res", "Leading Lepton Pt (reco)",
						  6, 0.5, 6.5, 50, -1., 1.);
    fHist_mc_reco_2ndLeadLepton_Pt_res->GetXaxis()->SetBinLabel(1,  "e^{+}, e^{-}");
    fHist_mc_reco_2ndLeadLepton_Pt_res->GetXaxis()->SetBinLabel(2,  "#mu^{+}, #mu^{-}");
    fHist_mc_reco_2ndLeadLepton_Pt_res->GetXaxis()->SetBinLabel(3,  "#tau^{+}, #tau^{-}");    
    fHist_mc_reco_2ndLeadLepton_Pt_res->GetXaxis()->SetBinLabel(4,  "e^{#pm}, #mu^{#mp}");
    fHist_mc_reco_2ndLeadLepton_Pt_res->GetXaxis()->SetBinLabel(5,  "e^{#pm}, #tau^{#mp}");
    fHist_mc_reco_2ndLeadLepton_Pt_res->GetXaxis()->SetBinLabel(6,  "#mu^{#pm}, #tau^{#mp}");
    fHist_mc_reco_2ndLeadLepton_Pt_res->GetYaxis()->SetTitle("Resolution (true - reco)/true p_{T}");
    
    gDirectory->cd("..");
}

// ____________________________________________________________________

void AtlSgTop_sChannelAnalysis::DoTruthMatching() {
    //
    // - Assign true particles to data members
    // - Determine decay channel of top pair based on MC truth info
    //
    
    fMCbQuark = 0;
    fMCAntibQuark = 0;
    
    fMCLeptonPlus  = 0;
    fMCLeptonMinus = 0;

    fMCTightLepton = 0;
    fMCLooseLepton = 0;

    TIter next_MCprt(fEvent->GetMCParticles());
    HepMCParticle *mcprt = 0;

    // Count number of leptons and quarks
    Int_t N_El  = 0;
    Int_t N_Mu  = 0;
    Int_t N_Tau = 0;
    Int_t N_Quarks = 0;

    while ( (mcprt = (HepMCParticle*) next_MCprt()) ) {
	// skip b-quarks
	if ( mcprt->IsBeauty() ) {
	    fMCbQuark = mcprt;
	    continue;
	}
	if ( mcprt->IsBeautyBar() ) {
	    fMCAntibQuark = mcprt;
	    continue;
	}
	
	// Count leptons
	if ( mcprt->IsElectron() ) N_El++;
	if ( mcprt->IsMuon() ) N_Mu++;
	if ( mcprt->IsTau() ) N_Tau++;

	// Assign Leptons
	if ( mcprt->IsChargedLepton() ) {
	    if ( mcprt->Charge() > 0 )
		fMCLeptonPlus = mcprt;
	    else 
		fMCLeptonMinus = mcprt;
	}
	
	// Count Quarks
	if ( mcprt->IsQuark() ) { N_Quarks++; }
    }

    // Find the MC truth lepton matching the tight reco lepton
    HepParticle *lepton = (HepParticle*) fLeptons->At(0);
    // Find the MC truth lepton matching the loose reco lepton
    HepParticle *loose_lepton = (HepParticle*)fLooseLeptons->At(0);

    // Assign truth lepton matched by deltaR
    // -------------------------------------
    Double_t DeltaR_tight_plus = 99.;
    Double_t DeltaR_loose_plus = 99.;
    if ( fMCLeptonPlus != 0 ) {
	DeltaR_tight_plus = fMCLeptonPlus->DeltaR(lepton);
    	if ( DeltaR_tight_plus < 0.4 )
    	    fMCTightLepton = fMCLeptonPlus;
	
	if ( loose_lepton != 0 ) {
	    DeltaR_loose_plus = fMCLeptonPlus->DeltaR(loose_lepton);
	    if ( DeltaR_loose_plus < 0.4 )
		fMCLooseLepton = fMCLeptonPlus;
	}
    }
    
    Double_t DeltaR_tight_minus = 99.;
    Double_t DeltaR_loose_minus = 99.;
    if ( fMCLeptonMinus != 0 ) {
	DeltaR_tight_minus = fMCLeptonMinus->DeltaR(lepton);
	if ( DeltaR_tight_minus < 0.4
	     && DeltaR_tight_minus < DeltaR_tight_plus )
    	    fMCTightLepton = fMCLeptonMinus;
    
    	if ( loose_lepton != 0 ) {
	    DeltaR_loose_minus = fMCLeptonMinus->DeltaR(loose_lepton);
	    if ( DeltaR_loose_minus < 0.4 &&
		 DeltaR_loose_minus < DeltaR_loose_plus )
		fMCLooseLepton = fMCLeptonMinus;
	}
    }

    if ( fMCTightLepton != 0 &&
	 fMCLooseLepton != 0 &&
	 fMCLooseLepton == fMCTightLepton ) {
	
	Error("DoTruthMatching",
	      "MC tight lepton and MC loose lepton is identical. Check OverlapRemoval or Truthmatching! Abort!");

	// Some debug info
    	// cout << "DeltaR(plus, lepton) = " << fMCLeptonPlus->DeltaR(lepton) << endl
    	//      << "DeltaR(plus, loose lepton) = " << fMCLeptonPlus->DeltaR(loose_lepton) << endl
	//      << "DeltaR(minus, lepton) = " << fMCLeptonMinus->DeltaR(lepton) << endl
    	//      << "DeltaR(minus, loose lepton) = " << fMCLeptonMinus->DeltaR(loose_lepton) << endl
	//      << "DeltaR(lepton, loose lepton) = " << lepton->DeltaR(loose_lepton) << endl;
	    
	// cout << "fMCLeptonPlus: " << endl;
    	// fMCLeptonPlus->P().Print();
    	// cout << "fMCLeptonMinus: " << endl;
    	// fMCLeptonMinus->P().Print();
    	// cout << "tight lepton: " << endl;
    	// lepton->P().Print();
    	// cout << "loose lepton: " << endl;
    	// loose_lepton->P().Print();
    
    	// gSystem->Abort();
    }
    
    // Determine the decay mode of the event
    if ( N_El == 2 ) fTtbarDecayMode = kElEl;
    else if ( N_Mu == 2 ) fTtbarDecayMode = kMuMu;
    else if ( N_Tau == 2 ) fTtbarDecayMode = kTauTau;
    else if ( N_El == 1 && N_Mu == 1) fTtbarDecayMode = kElMu;
    else if ( N_El == 1 && N_Tau == 1) fTtbarDecayMode = kElTau;
    else if ( N_Mu == 1 && N_Tau == 1) fTtbarDecayMode = kMuTau;
    else if ( N_El == 1 && N_Quarks == 2) fTtbarDecayMode = kElQQ;
    else if ( N_Mu == 1 && N_Quarks == 2) fTtbarDecayMode = kMuQQ;
    else if ( N_Tau == 1 && N_Quarks == 2 ) fTtbarDecayMode = kTauQQ;
    else if ( N_Quarks == 4 ) fTtbarDecayMode = kHadronic;
    else {
	Warning("DoTruthMatching",
		"Mode unkown!");
	cout << "N_El     = " << N_El << endl
	     << "N_Mu     = " << N_Mu << endl
	     << "N_Tau    = " << N_Tau << endl
	     << "N_Quarks = " << N_Quarks << endl
	     << endl;
	fTtbarDecayMode = kUnknown;
    }
    
}

//____________________________________________________________________
//
// Single-top t-Channel Analysis
// 
//  
// Author: Ruth Herrberg <mailto: ruthh@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef SINGLETOP_Atl_tChannelAnalysis
#include <Atl_tChannelAnalysis.h>
#endif
#include <iostream>
#include <AtlPdfReweightingTool.h>

using namespace std;

#ifndef __CINT__
ClassImp(Atl_tChannelAnalysis);
#endif



//____________________________________________________________________

Atl_tChannelAnalysis::Atl_tChannelAnalysis(const char* OutputFile) :
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
    fTopPair_tool    = 0;
    ftChannel_tool   = 0;
    fCutflow_tool    = 0;
    fCutflow_tool_2  = 0;
    fQCD_tool        = 0;
	
    SetCutDefaults();
}

//____________________________________________________________________

Atl_tChannelAnalysis::~Atl_tChannelAnalysis() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void Atl_tChannelAnalysis::Clear( Option_t *option ) {
    //
    //  Clears the dynamically allocated objects
    //
}


//____________________________________________________________________

void Atl_tChannelAnalysis::SetBranchStatus() {
    //
    // Switch on/off branches in order to gain speed
    //

    // Switch off all branches by default
    fTree->SetBranchStatus("*", kFALSE);
    
    fTree->SetBranchStatus("fEventHeader*", kTRUE);
    fTree->SetBranchStatus("fTopDecays*",   kTRUE);
    fTree->SetBranchStatus("fN_TopDecays*", kTRUE);
    fTree->SetBranchStatus("fN_Vertices*",  kTRUE);
    fTree->SetBranchStatus("fVertices*",    kTRUE); 
    fTree->SetBranchStatus("fN_Electrons*",           kTRUE);
    fTree->SetBranchStatus("fElectrons*",             kTRUE);
    fTree->SetBranchStatus("fN_Muons*",               kTRUE);
    fTree->SetBranchStatus("fMuons*",                 kTRUE);
    fTree->SetBranchStatus("fEnergySum*",             kTRUE);
    fTree->SetBranchStatus("fN_AntiKt4LCTopoJets*",  kTRUE);
    fTree->SetBranchStatus("fAntiKt4LCTopoJets*",  kTRUE);
}

//____________________________________________________________________

void Atl_tChannelAnalysis::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    fPrintEvent = kFALSE;// kTRUE
    fInputMode  = kD3PDSgTop;

    // Etmiss
    fMET_Mag_min = 25;

    // Pileup-weight cut
    fPileupWeightMax = 100.;

  //   // Do QCD estimation with MM?
//     fDoMatrixMethod = kTRUE;

//     fSignalSampleMode = kFALSE;
}

//____________________________________________________________________

void Atl_tChannelAnalysis::BookHistograms() {
    //
    // Book histograms
    //    
    BookVertexHistograms();

    // Initialize pointers to tools needed in event loop
    fTopPair_tool  =
	(AtlTopPairFinder*)GetTool("AtlTopPairFinder","",kTRUE); 
    ftChannel_tool =
	(AtlSgTop_tChannelFinder*)GetTool("AtlSgTop_tChannelFinder","",kTRUE);
    fCutflow_tool =
	(AtlCutFlowTool*)GetTool("AtlCutFlowTool","AtlCutFlowTool",kTRUE);
    fCutflow_tool_2 =
	(AtlCutFlowTool*)GetTool("AtlCutFlowTool","AtlCutFlowTool2",kTRUE);

    // QCD tool
    fQCD_tool =
	(AtlQCDMMTool*)GetTool("AtlQCDMMTool","",kFALSE);

    //
    // Setup PDF reweighting tool
    //
    AtlPdfReweightingTool *pdf_tool =
	(AtlPdfReweightingTool*)GetTool("AtlPdfReweightingTool","",kTRUE);

    // Yield histograms for W+jets reweighting:
    // Not using W control region
    pdf_tool->AddObservable("/AtlSgTop_tChannelFinder/CommonEventSelection/EventVariables/h_count_lep_all_tag");
    pdf_tool->AddObservable("/AtlSgTop_tChannelFinder/CommonEventSelection/EventVariables/h_count_lep_plus_pretag");
    pdf_tool->AddObservable("/AtlSgTop_tChannelFinder/CommonEventSelection/EventVariables/h_count_lep_plus_tag");
    pdf_tool->AddObservable("/AtlSgTop_tChannelFinder/CommonEventSelection/EventVariables/h_count_lep_minus_pretag");
    pdf_tool->AddObservable("/AtlSgTop_tChannelFinder/CommonEventSelection/EventVariables/h_count_lep_minus_tag");
    // Using W control region
    pdf_tool->AddObservable("/AtlSgTop_tChannelFinder/CommonEventSelection/EventVariables/h_count_lep_all_wctrl_pretag");
    pdf_tool->AddObservable("/AtlSgTop_tChannelFinder/CommonEventSelection/EventVariables/h_count_lep_all_wctrl_tag");
    pdf_tool->AddObservable("/AtlSgTop_tChannelFinder/CommonEventSelection/EventVariables/h_count_lep_plus_wctrl_pretag");
    pdf_tool->AddObservable("/AtlSgTop_tChannelFinder/CommonEventSelection/EventVariables/h_count_lep_plus_wctrl_tag");
    pdf_tool->AddObservable("/AtlSgTop_tChannelFinder/CommonEventSelection/EventVariables/h_count_lep_minus_wctrl_pretag");
    pdf_tool->AddObservable("/AtlSgTop_tChannelFinder/CommonEventSelection/EventVariables/h_count_lep_minus_wctrl_tag");
    
    // BILL signal extraction histograms
    pdf_tool->AddObservable("/AtlSgTop_tChannelFinder/TotalYield/h_total_yield_all_cuts");
    pdf_tool->AddObservable("/AtlSgTop_tChannelFinder/KinFitterMode/RecoObjects_PChi2_and_otherCuts/Miscellaneous/h_deltap2mod_top_2ndleadjet_vs_Eta2ndLeadJet");
    pdf_tool->AddObservable("/AtlSgTop_tChannelFinder/KinFitterMode/RecoObjects_PChi2_and_otherCuts/Miscellaneous/h_deltap2mod_top_2ndleadjet_vs_Eta2ndLeadJet_Less_Bins");
    pdf_tool->AddObservable("/AtlSgTop_tChannelFinder/KinFitterMode/RecoObjects_PChi2_and_otherCuts/Miscellaneous/h_deltap2mod_top_2ndleadjet_vs_Eta2ndLeadJet_Less_Bins2");
    pdf_tool->AddObservable("/AtlSgTop_tChannelFinder/KinFitterMode/RecoObjects_PChi2_and_otherCuts/Miscellaneous/h_deltap2mod_top_2ndleadjet_vs_Eta2ndLeadJet_Less_Bins3");
    pdf_tool->AddObservable("/AtlSgTop_tChannelFinder/KinFitterMode/RecoObjects_PChi2_and_otherCuts/Miscellaneous/h_deltap2mod_top_2ndleadjet_vs_Eta2ndLeadJet_coarse");
    pdf_tool->AddObservable("/AtlSgTop_tChannelFinder/KinFitterMode/RecoObjects_PChi2_and_otherCuts/Miscellaneous/h_deltap2mod_top_2ndleadjet_vs_Eta2ndLeadJet_fine");

	
}

//____________________________________________________________________

Bool_t Atl_tChannelAnalysis::ProcessPreCut() {
    //
    // Event pre-selection
    //

    // Get Analysis Objects from AtlObjectsToolD3PDSgTop
    fElectrons = fObjTool->GetElectrons();
    fMuons     = fObjTool->GetMuons();
    fLeptons   = fObjTool->GetLeptons();
    fJets      = fObjTool->GetJets();
    fBJets     = fObjTool->GetBJets();
    
    //if ( fEvent->EventNr() == 121512 ) return kFALSE; // Evt in tchannel_enu with negative Chi^2

    FillVertexHistograms();
//     if ( fInputMode == kApp ) FillVertexHistograms();
//     // Pileup-weight cut *** (REMOVE for mc11b!!!) ***
//     if ( fEvent->GetEventHeader()->GetPileupWeight() >= fPileupWeightMax )
// 	return kFALSE;

    fCutflow_tool->Fill("No Cuts (D3PD)", GetPreTagEvtWeight());
    fCutflow_tool_2->Fill("No Cuts (D3PD)", GetPreTagEvtWeight());
    

    // Additional cuts for QCD case:
    
    // // =============================================
    // // 1: Passed respective lepton pretag selection?
    // // =============================================
    // Bool_t pretag_sel = kFALSE;
    // if ( fLeptonMode & kElectronMode )
    // 	pretag_sel = fEvent->GetEventHeader()->HasPassedElectronPreTag();
    // else if ( fLeptonMode & kMuonMode )
    // 	pretag_sel = fEvent->GetEventHeader()->HasPassedMuonPreTag();

    // if ( !pretag_sel ) return kFALSE;

    // if ( fLeptonMode & kElectronMode ) {
    // 	fCutflow_tool->Fill("ElectronPreTag", GetPreTagEvtWeight());
    // 	fCutflow_tool_2->Fill("ElectronPreTag", GetPreTagEvtWeight());
    // } else if ( fLeptonMode & kMuonMode ) {
    // 	fCutflow_tool->Fill("MuonPreTag", GetPreTagEvtWeight());
    // 	fCutflow_tool_2->Fill("MuonPreTag", GetPreTagEvtWeight());
    // }

    
    // =========
    // 2: Etmiss
    // =========
    if ( fEvent->GetEnergySum()->MissingEt_Mag() <= fMET_Mag_min )
	return kFALSE;

    fCutflow_tool->Fill(Form("Etmiss > %d GeV",(Int_t)fMET_Mag_min), GetPreTagEvtWeight());
    fCutflow_tool_2->Fill(Form("Etmiss > %d GeV",(Int_t)fMET_Mag_min), GetPreTagEvtWeight());

    return kTRUE;
}

//____________________________________________________________________

Bool_t Atl_tChannelAnalysis::ProcessCut() {
    //
    // Event selection
    //

    //
    // Make sure top pair finder histograms are filled
    //
    // (not ensured since all events with success of top pair finder
    // will per construction of ttbar veto return kFALSE for this
    // Process()-method such that top pair finder's FillHistograms
    // will NEVER be called by the selector)
    fTopPair_tool->FillHistograms();
    
    // Reject events where top-reconstruction by t-channel finder failed
    HepTopDecay* t_test = 0;
    Bool_t success = kFALSE;
    
    for ( Int_t i = 0;  i < fEvent->GetN_TopDecays(); i++ ){
	t_test = (HepTopDecay*)fEvent->GetTopDecays()->At(i);
	if ( t_test->IsSgTop_tChannel() ) success = kTRUE;
    }

    return success;
}

//____________________________________________________________________


void Atl_tChannelAnalysis::FillHistograms() {
    //
    // Fill histograms
    //
    // This method will be called only for events surviving the
    // ProcessCut() routine
    //

}

//____________________________________________________________________

void Atl_tChannelAnalysis::InitEvent() {
    //
    // Prepare event analysis
    //

    // ============
    //  Missing Et
    // ============
    fMET = fEvent->GetEnergySum()->GetMissingEt();

    // ============================
    // Send Objects to finder tools
    // ============================

    // TopPairFinder
    fTopPair_tool->SetEtmiss(fMET);
    fTopPair_tool->SetLeptons(fLeptons);
    fTopPair_tool->SetJets(fJets);

    // t-channel Finder
    ftChannel_tool->SetEtmiss(fMET);
    ftChannel_tool->SetLeptons(fLeptons);
    ftChannel_tool->SetJets(fJets);

}

//____________________________________________________________________

void Atl_tChannelAnalysis::BookVertexHistograms() {
    //
    // Book histograms of primary vertex x,y,z-components
    //
    gDirectory->mkdir("vertices");
    gDirectory->cd("vertices");

    fHist_PV_X = new TH1F("h_PV_x", "X - Component of Primary Vertex",
			  200, -0.2, 0.2);
    fHist_PV_X->SetXTitle("X_{PV}");
    fHist_PV_X->SetYTitle("Number of Entries");
    
    fHist_PV_Y = new TH1F("h_PV_y", "Y - Component of Primary Vertex",
			  200, -0.01, 0.19);
    fHist_PV_Y->SetXTitle("Y_{PV}");
    fHist_PV_Y->SetYTitle("Number of Entries");

    fHist_PV_Z = new TH1F("h_PV_z", "Z - Component of Primary Vertex",
			  200, -40., 40.);
    fHist_PV_Z->SetXTitle("Z_{PV}");
    fHist_PV_Z->SetYTitle("Number of Entries");
    
         
    
    fHist_PV_XY = new TH2F("h_PV_xy", "(X,Y) - Components of Primary Vertex",
			   200, -0.02, 0.02, 200, 0.04, 0.15);

    
    gDirectory->cd("..");

    //
    // Book pileup histograms
    //
    gDirectory->mkdir("pileup");
    gDirectory->cd("pileup");

    fHist_AverageIntPerXing = new TH1F("h_average_int_per_xing", "Average Interactions per Bunch-Crossing",
			  40, 0., 40.);
    fHist_AverageIntPerXing->SetXTitle("#bar{N}_{ints per xing}");
    fHist_AverageIntPerXing->SetYTitle("Number of Entries");

    fHist_NPrimaryVertices = new TH1F("h_n_vxp", "Number of Primary Vertices in Event",
			  40, 0., 40.);
    fHist_NPrimaryVertices->SetXTitle("N_{prim. vertices}");
    fHist_NPrimaryVertices->SetYTitle("Number of Entries");


    gDirectory->cd("..");
}

//____________________________________________________________________

void Atl_tChannelAnalysis::FillVertexHistograms() {
    //
    // Fill histograms with pile-up information
    //
    fHist_AverageIntPerXing->Fill(fEvent->GetEventHeader()->GetAverageIntPerXing(),fEvent->GetPreTagEvtWeight());
    
    //
    // Fill histograms for vertex information
    //

    // Number of primary vertices
    fHist_NPrimaryVertices->Fill(fEvent->GetN_PrimaryVertices(),fEvent->GetPreTagEvtWeight());

    // Coordinates of prim. vertex
    HepVertex *pv = fEvent->GetPrimaryVtx();
    
    if ( pv != 0 ) {
      fHist_PV_X->Fill(pv->X(), GetPreTagEvtWeight());
      fHist_PV_Y->Fill(pv->Y(), GetPreTagEvtWeight());
      fHist_PV_Z->Fill(pv->Z(), GetPreTagEvtWeight());
      fHist_PV_XY->Fill(pv->X(),pv->Y(), GetPreTagEvtWeight());
    }
}

//____________________________________________________________________

void Atl_tChannelAnalysis::Print() const {
    //
    // Print user analysis configuration
    //
    cout
	<< endl
	<< "========================================================" << endl
	<< "  Atlas Single-Top t-Channel Analysis Selector          " << endl
	<< "========================================================" << endl
	<< "  Chosen sample type is fInputMode        = kD3PDSgTop  " << endl;
}

//____________________________________________________________________

void Atl_tChannelAnalysis::Terminate() {
    //
    // Terminate routine called at the end of event loop
    //    
    AtlSelector::Terminate();
}

//____________________________________________________________________

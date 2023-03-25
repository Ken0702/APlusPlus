//____________________________________________________________________
//
// Single-top wt-Channel Analysis
// 
//  
// Author: Michelangelo Giorgi <mailto: strider@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef SINGLETOP_Atl_WtChannelAnalysis
#include <Atl_WtChannelAnalysis.h>
#endif
#include <iostream>
#include <AtlPdfReweightingTool.h>

using namespace std;

#ifndef __CINT__
ClassImp(Atl_WtChannelAnalysis);
#endif


//____________________________________________________________________

Atl_WtChannelAnalysis::Atl_WtChannelAnalysis(const char* OutputFile) :
    AtlSelector(OutputFile) {
    //
    // Default constructor
    //
    
    // Init
    fJets        = new TList;
    fLeptons     = new TList;
    fElectrons   = new TList;
    fMuons       = new TList;

    fTopPair_tool  = 0;
    fWtChannel_tool = 0;
    fCutflow_tool  = 0;
    fCutflow_tool_AC  = 0;
    
    SetCutDefaults();
}

//____________________________________________________________________

Atl_WtChannelAnalysis::~Atl_WtChannelAnalysis() {
    //
    // Default destructor
    //
    if ( fLeptons      != 0 ) delete fLeptons;
    if ( fElectrons    != 0 ) delete fElectrons;
    if ( fMuons        != 0 ) delete fMuons;
    if ( fJets         != 0 ) delete fJets;    
}

//____________________________________________________________________

void Atl_WtChannelAnalysis::SetBranchStatus() {
    //
    // Switch on/off branches in order to gain speed
    //

    // Switch on all branches by default
    fTree->SetBranchStatus("*", kFALSE);
    
}

//____________________________________________________________________

void Atl_WtChannelAnalysis::Clear(Option_t *option){
    //
    //  Clears the dynamically allocated objects
    //
    //Info("Atl_WtChannelAnalysis","Calling Clear() !");
    fLeptons->Clear();
    fElectrons->Clear();
    fMuons->Clear();
    fJets->Clear();    
}

//____________________________________________________________________

void Atl_WtChannelAnalysis::SetCutDefaults() {
    //
    // Selection cut defaults
    //
    fPrintEvent = kFALSE;// kTRUE
    fInputMode  = kD3PDSgTop;
    fLeptonMode = kMuonMode;

    // Jets suitable for reconstruction
    fJetsAuthor = AtlJet::kAntiKt4TopoEM;

    // Etmiss
    fMET_Mag_min = 25;

    // Pileup-weight cut
    fPileupWeightMax = 100.;

}

//____________________________________________________________________

void Atl_WtChannelAnalysis::BookHistograms() {
    //
    // Book histograms
    //    
    BookVertexHistograms();

    // Initialize pointers to tools needed in event loop
    fTopPair_tool  =
	(AtlTopPairFinder*)GetTool("AtlTopPairFinder","",kTRUE);
    fWtChannel_tool =
     	(AtlSgTop_WtChannelFinder*)GetTool("AtlSgTop_WtChannelFinder","",kTRUE);
    fCutflow_tool =
	(AtlCutFlowTool*)GetTool("AtlCutFlowTool","AtlCutFlowTool",kTRUE);
    fCutflow_tool_AC =
	(AtlCutFlowTool*)GetTool("AtlCutFlowTool","AtlCutFlowTool_AC",kTRUE);

    //
    // Setup PDF reweighting tool
    //
         AtlPdfReweightingTool *pdf_tool =
	     (AtlPdfReweightingTool*)GetTool("AtlPdfReweightingTool","",kTRUE);

	 // Yield histograms for W+jets reweighting:
	 // Not using W control region
	 pdf_tool->AddObservable("AtlSgTop_WtChannelFinder/EventVariables//AtlSgTop_tChh_count_lep_all_tag");
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
	 pdf_tool->AddObservable("/AtlSgTop_WtChannelFinder/TotalYield/h_total_yield_p10WtPt");
}

//____________________________________________________________________

Bool_t Atl_WtChannelAnalysis::ProcessPreCut() {
    //
    // Event pre-selection
    //
    FillVertexHistograms();

//     // Pileup-weight cut *** (REMOVE for mc11b!!!) ***
//     if ( fEvent->GetEventHeader()->GetPileupWeight() >= fPileupWeightMax )
// 	return kFALSE;

    fCutflow_tool->Fill("No Cuts (D3PD)", GetPreTagEvtWeight());
    fCutflow_tool_AC->Fill("No Cuts (D3PD)", GetPreTagEvtWeight());
    
    // Additional cuts for QCD case:
    
    // =========
    // 1: Etmiss
    // =========
    fMET = fEvent->GetEnergySum()->GetRefFinalEM_etightpp_MissingEt();
    Float_t MET_Mag = fMET.Mod();
    
    if ( MET_Mag <= fMET_Mag_min ) return kFALSE;
    
    return kTRUE;
}

//____________________________________________________________________

Bool_t Atl_WtChannelAnalysis::ProcessCut() {
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
    
//     cout << " fPassedSelection : "
// 	 << ((fPassedSelection)  ? "kTRUE" : "kFALSE" ) << endl;
    return fPassedSelection;
}

//____________________________________________________________________


void Atl_WtChannelAnalysis::FillHistograms() {
    //
    // Fill histograms
    //
    // This method will be called only for events surviving the
    // ProcessCut() routine
    //

}

//____________________________________________________________________

void Atl_WtChannelAnalysis::InitEvent() {
    //
    // Prepare event analysis
    //

    //---------
    // Leptons
    //---------

    // 
    // Find Signal Electrons
    //
    AtlElectron* el = 0;
    TClonesArray* electrons = fEvent->GetElectrons();
    
    for ( Int_t i = 0; i < fEvent->GetN_Electrons(); ++i ){
	el = (AtlElectron*)electrons->At(i);
	fElectrons->Add(el);        
    }

    fElectrons->Sort(kSortDescending);


    // 
    // Find Signal Muons
    //
    AtlMuon *muon = 0;
    TClonesArray* muons = fEvent->GetMuons();

    for ( Int_t i = 0; i < fEvent->GetN_Muons(); i++ ) {
	muon = (AtlMuon*)muons->At(i);
	fMuons->Add(muon);
    } 

    fMuons->Sort(kSortDescending);

    
    //
    // Build inclusive lepton list
    //
    if ( fLeptonMode & kElectronMode ) { 
	fLeptons->AddAll(fElectrons);
    }
    if ( fLeptonMode & kMuonMode ) { 
	fLeptons->AddAll(fMuons);
    }

    fLeptons->Sort(kSortDescending);


    // ======
    // Etmiss
    // ======
    fMET = fEvent->GetEnergySum()->GetRefFinalEM_etightpp_MissingEt();
    //    fMET = fEvent->GetEnergySum()->GetRefFinalEM_etight_MissingEt();


    //------
    // Jets
    //------
    

    //
    // Get all jets:
    //
    AtlJet *jet = 0;
    TClonesArray *jets = fEvent->GetJets(fJetsAuthor);

    for ( Int_t i = 0; i < fEvent->GetN_Jets(fJetsAuthor); ++i ) {
  	jet = (AtlJet*)jets->At(i);
	fJets->Add(jet);
    }
    
    fJets->Sort(kSortDescending);    


    
    // ============================
    // Send Objects to finder tools
    // ============================
    
//     // TopPairFinder
     fTopPair_tool->SetEtmiss(fMET);
     fTopPair_tool->SetLeptons(fLeptons);
     fTopPair_tool->SetJets(fJets);

    // Wt-channel Finder
    fWtChannel_tool->SetEtmiss(fMET);
    fWtChannel_tool->SetLeptons(fLeptons);
    fWtChannel_tool->SetJets(fJets);
    //fWtChannel_tool->SetHasLArError(fHasLArError);
    
    
}

//____________________________________________________________________

void Atl_WtChannelAnalysis::BookVertexHistograms() {
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
    fHist_PV_XY->SetXTitle("X_{PV}");
    fHist_PV_XY->SetYTitle("Y_{PV}");
    fHist_PV_XY->SetZTitle("Number of Entries");
    
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

void Atl_WtChannelAnalysis::FillVertexHistograms() {
    //
    // Fill histograms for vertex informations
    //
    fHist_AverageIntPerXing->Fill(fEvent->GetEventHeader()->GetAverageIntPerXing(),fEvent->GetPreTagEvtWeight());
    fHist_NPrimaryVertices->Fill(fEvent->GetN_PrimaryVertices(),fEvent->GetPreTagEvtWeight());

//    HepVertex *pv = fEvent->GetPrimaryVtx();
//     if ( pv != 0 ) {
// 	fHist_PV_X->Fill(pv->X(), GetPreTagEvtWeight());
// 	fHist_PV_Y->Fill(pv->Y(), GetPreTagEvtWeight());
// 	fHist_PV_Z->Fill(pv->Z(), GetPreTagEvtWeight());
// 	fHist_PV_XY->Fill(pv->X(),pv->Y(), GetPreTagEvtWeight());
//     }
}

//____________________________________________________________________

void Atl_WtChannelAnalysis::Print() const {
    //
    // Print user analysis configuration
    //
    cout
	<< endl
	<< "========================================================"  << endl
	<< "  Atlas Single-Top Wt-Channel Analysis Selector          " << endl
	<< "========================================================"  << endl
	<< "  Chosen sample type is fInputMode =  "<< fInputMode       << endl  
	<< "--------------------------------------------------------"  << endl
	<< "  Chosen lepton mode is fLeptonMode = ";
    switch ( fLeptonMode ) {
	case kElectronMode:
	    cout << "kElectronMode" << endl;
	    break;
	case kMuonMode:
	    cout << "kMuonMode" << endl;
	    break;
	case kLeptonAll:
	    cout << "kLeptonAll" << endl;
	    break;
	default:
	    Error("Print", "Which Lepton Mode?");
    }
    cout
	<< "  Maximum pile-up weight fPileupWeightMax = " << fPileupWeightMax << endl
	<< "--------------------------------------------------------" << endl
	<< "  Specifications of physics objects:                    " << endl
	<< "--------------------------------------------------------" << endl
	<< "  fJetsAuthor                             = " << AtlJet::NameOfType(fJetsAuthor) << endl
	<< "  fMET_Mag_min                            = " << fMET_Mag_min << endl
	<< "========================================================" << endl
	<< endl;

}

//____________________________________________________________________

void Atl_WtChannelAnalysis::Terminate() {
    //
    // Calls Terminate() of Selector
    //    
    AtlSelector::Terminate();
}

//____________________________________________________________________

//____________________________________________________________________
//
// Tool for computing trigger efficienies in MC events
//
// The tool counts events fulfilling certain trigger requirements
// given by the user and creates efficiency graphs as a function of
// Pt, Eta and Phi of the trigger object in question (jet, electron,
// muon).
//
// The tool is not affected by any other event selection which might
// be exisiting in the parent analysis.
//
// Example in MyTask.C :
//
//    // Add trigger tool
//    AtlTriggerTool *trigger = new AtlTriggerTool("trigger_tool",
//                                                 "Trigger Efficiencies");
//    SetToolCut("trigger_tool", "fTriggerSelection", "L2_e10_medium");
//    fSelector->AddTool(trigger);
//
// (See class AtlAnalysisTool for a general description and usage of
// A++ analysis tools.)
// 
// Note that the tools allows also for combined items (logical OR only), eg
// "item1 || item2 || item3"
// In this case the tool creates a sub-folder for the efficencies of
// each item and another folder forr the combined case.
//
//
// Author: Charlotte Santer <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2010 (C) Charlotte Santer
//
#ifndef ATLAS_AtlTriggerTool
#include <AtlTriggerTool.h>
#endif
#include <TPRegexp.h>
#include <TDirectory.h>
#include <TGraphAsymmErrors.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlTriggerTool);
#endif

//____________________________________________________________________

AtlTriggerTool::AtlTriggerTool(const char* name, const char* title) :
    AtlAnalysisTool(name, title) {
    //
    // Default constructor
    //
    fListOfTriggerItems = 0;
    fTriggerSelection = new TString;
    fProcessMode = kPreAnalysis;
}

//____________________________________________________________________

AtlTriggerTool::~AtlTriggerTool() {
    //
    // Default destructor
    //
    delete fTriggerSelection;
}

//____________________________________________________________________

void AtlTriggerTool::CompileSelectionString() {
    //
    // Splits the selection string given by the user into different
    // items and determines their corresponding trigger types
    //
    if ( fListOfTriggerItems != 0 ) {
	fListOfTriggerItems->Delete();
	delete fListOfTriggerItems;
    }
    fListOfTriggerItems = new TList;

    TObjArray *items_arr = fTriggerSelection->Tokenize("||");
    TIter next_item(items_arr);
    TObjString *item_objstr = 0;
    while ( (item_objstr = (TObjString*)next_item()) ) {
	TString item_str = item_objstr->GetString();
	AtlTriggerToolItem::ETriggerType  type  = GetTriggerType(item_str);
	AtlTriggerToolItem::ETriggerLevel level = GetTriggerLevel(item_str);
	fListOfTriggerItems->Add(new AtlTriggerToolItem(item_str.Data(), "",
							level, type));
    }
}

//____________________________________________________________________

AtlTriggerToolItem::ETriggerType AtlTriggerTool::GetTriggerType(TString item) {
    //
    // Determines and returns the trigger type of the given trigger item
    //
    item.ToLower();
    
    // Define regular expressions
    TPRegexp reg_em("_[1-9]?em");
    TPRegexp reg_e("_[1-9]?e");
    TPRegexp reg_mu("_[1-9]?mu");
    TPRegexp reg_j("_[1-9]?j");

    // Test item string
    if ( item.Contains(reg_em) ) return AtlTriggerToolItem::kEM;
    if ( item.Contains(reg_e)  ) return AtlTriggerToolItem::kElectron;
    if ( item.Contains(reg_mu) ) return AtlTriggerToolItem::kMuon;
    if ( item.Contains(reg_j)  ) return AtlTriggerToolItem::kJet;
    return AtlTriggerToolItem::kUnknownType;
}

//____________________________________________________________________

AtlTriggerToolItem::ETriggerLevel AtlTriggerTool::GetTriggerLevel(TString item) {
    //
    // Determines and returns the trigger level of the given trigger item
    //
    item.ToLower();

    // Define regular expressions
    TPRegexp reg_lv1("^l1_");
    TPRegexp reg_lv2("^l2_");
    TPRegexp reg_ef("^ef_");

    // Test item string
    if ( item.Contains(reg_lv1) ) return AtlTriggerToolItem::kLV1;
    if ( item.Contains(reg_lv2) ) return AtlTriggerToolItem::kLV2;
    if ( item.Contains(reg_ef)  ) return AtlTriggerToolItem::kEF;
    return AtlTriggerToolItem::kUnknownLevel;
}
    
//____________________________________________________________________

void AtlTriggerTool::BookHistograms() {
    //
    // Book histograms for the number of passed and all events for every
    // trigger item as a function of Pt, phi and eta of the
    // corresponding MC object
    //

    // Decode selection string
    CompileSelectionString();

    // ================
    // Individual items
    // ================
    
    // Loop over all given items and book the corresponding histograms
    TIter next_item(fListOfTriggerItems);
    AtlTriggerToolItem *item = 0;
    while ( (item = (AtlTriggerToolItem*)next_item()) ) {
	if ( item->fTriggerType == AtlTriggerToolItem::kUnknownType ) {
	    Error("BookHistogram",
		  "Unknown type of trigger item \"%s\". Please check. No histogram booked.",
		  item->GetName());
	    continue;
	}

	// Define log scale for Pt graph
	Double_t binlimit[20];
	float a = TMath::Power (1000, 0.05);
	for (int i = 0; i <= 19; i++) {
	     binlimit[i] = TMath::Power (a, i);
	}

	// Create subdirectory for each item
	TDirectory *savdir = gDirectory;
	gDirectory->mkdir(item->GetName(),
			  item->GetName());
	gDirectory->cd(item->GetName());
	
	// Transverse momentum
	TH1F *h_npass_pt = new TH1F(Form("h_%s_npass_pt", item->GetName()),
				    item->GetName(),
				    19, binlimit);
	TH1F *h_ntot_pt = new TH1F(Form("h_%s_ntot_pt", item->GetName()),
				   item->GetName(),
				   19, binlimit);
	h_npass_pt->SetYTitle("No. of Passed Events");
	h_ntot_pt->SetYTitle("Total No. of Events");
	switch ( item->fTriggerType ) {
	    case AtlTriggerToolItem::kElectron:
		h_npass_pt->SetXTitle("p_{t, e}^{truth} (GeV)");
		h_ntot_pt->SetXTitle("p_{t, e}^{truth} (GeV)");
		break;
	    case AtlTriggerToolItem::kMuon:
		h_npass_pt->SetXTitle("p_{t, #mu}^{truth} (GeV)");
		h_ntot_pt->SetXTitle("p_{t, #mu}^{truth} (GeV)");
		break;
	    case AtlTriggerToolItem::kJet:
		h_npass_pt->SetXTitle("p_{t, jet}^{truth} (GeV)");
		h_ntot_pt->SetXTitle("p_{t, jet}^{truth} (GeV)");
		break;
	    case AtlTriggerToolItem::kEM:
		h_npass_pt->SetXTitle("p_{t, e|#gamma}^{truth} (GeV)");
		h_ntot_pt->SetXTitle("p_{t, e|#gamma}^{truth} (GeV)");
		break;
            default:
		;
	}
	
	// Azimuth
	TH1F *h_npass_phi = new TH1F(Form("h_%s_npass_phi", item->GetName()),
			       item->GetName(),
			       64, -3.2, 3.2);
	TH1F *h_ntot_phi = new TH1F(Form("h_%s_ntot_phi", item->GetName()),
				    item->GetName(),
				    64, -3.2, 3.2);
	h_npass_phi->SetYTitle("No. of Passed Events");
	h_ntot_phi->SetYTitle("Total No. of Events");
	switch ( item->fTriggerType ) {
	    case AtlTriggerToolItem::kElectron:
		h_npass_phi->SetXTitle("#phi_{t, e}^{truth} (rad)");
		h_ntot_phi->SetXTitle("#phi_{t, e}^{truth} (rad)");
		break;
	    case AtlTriggerToolItem::kMuon:
		h_npass_phi->SetXTitle("#phi_{t, #mu}^{truth} (rad)");
		h_ntot_phi->SetXTitle("#phi_{t, #mu}^{truth} (rad)");
		break;
	    case AtlTriggerToolItem::kJet:
		h_npass_phi->SetXTitle("#phi_{t, jet}^{truth} (rad)");
		h_ntot_phi->SetXTitle("#phi_{t, jet}^{truth} (rad)");
		break;
	    case AtlTriggerToolItem::kEM:
		h_npass_phi->SetXTitle("#phi_{t, e|#gamma}^{truth} (rad)");
		h_ntot_phi->SetXTitle("#phi_{t, e|#gamma}^{truth} (rad)");
		break;
	    default:
		;
	}

	// Pseudo-rapidity
	TH1F *h_npass_eta = new TH1F(Form("h_%s_npass_eta", item->GetName()),
			      item->GetName(),
			      100, -5, 5);
	TH1F *h_ntot_eta = new TH1F(Form("h_%s_ntot_eta", item->GetName()),
				    item->GetName(),
				    100, -5, 5);
	h_npass_eta->SetYTitle("No. of Passed Events");
	h_ntot_eta->SetYTitle("Total No. of Events");
	switch ( item->fTriggerType ) {
	    case AtlTriggerToolItem::kElectron:
		h_npass_eta->SetXTitle("#eta_{t, e}^{truth}");
		h_ntot_eta->SetXTitle("#eta_{t, e}^{truth}");
		break;
	    case AtlTriggerToolItem::kMuon:
		h_npass_eta->SetXTitle("#eta_{t, #mu}^{truth}");
		h_ntot_eta->SetXTitle("#eta_{t, #mu}^{truth}");
		break;
	    case AtlTriggerToolItem::kJet:
		h_npass_eta->SetXTitle("#eta_{t, jet}^{truth}");
		h_ntot_eta->SetXTitle("#eta_{t, jet}^{truth}");
		break;
	    case AtlTriggerToolItem::kEM:
		h_npass_eta->SetXTitle("#eta_{t, e|#gamma}^{truth}");
		h_ntot_eta->SetXTitle("#eta_{t, e|#gamma}^{truth}");
		break;	   
	    default:
		;
	}

	item->fHistNPassPt  = h_npass_pt;
	item->fHistNPassPhi = h_npass_phi;
	item->fHistNPassEta = h_npass_eta;
	item->fHistNTotPt  = h_ntot_pt;
	item->fHistNTotPhi = h_ntot_phi;
	item->fHistNTotEta = h_ntot_eta;
	savdir->cd();
    }

    // =============
    // Combined Case
    // =============

    //Create sub-directory for combined case
    TDirectory *savdir = gDirectory;
    gDirectory->mkdir("Combination",
		      "Combination");
    gDirectory->cd("Combination");
    
    // Log scale for Pt graph
    Double_t binlimit[20];
    float a = TMath::Power (1000, 0.05);
    for (int i = 0; i <= 19; i++) {
	binlimit[i] = TMath::Power (a, i);
    }
    
    // Transverse momentum for combination
    fHistCombPt = new TH1F("h_comb_npass_pt", "Combination",
			   19, binlimit);
    fHistCombTotPt = new TH1F("h_comb_ntot_pt", "Combination",
			   19, binlimit);

    fHistCombPt->SetYTitle("No. of Passed Events");
    fHistCombTotPt->SetYTitle("Total No. of Events");

    fHistCombPt->SetXTitle("p_{t}^{truth} (GeV)");
    fHistCombTotPt->SetXTitle("p_{t}^{truth} (GeV)");
    
    // Azimuth for combination
    fHistCombPhi = new TH1F("h_comb_npass_phi", "Combintion",
				64, -3.2, 3.2);
    fHistCombTotPhi = new TH1F("h_comb_ntot_phi", "Combintion",
				64, -3.2, 3.2);
    
    fHistCombPhi->SetYTitle("No. of Passed Events");
    fHistCombTotPhi->SetYTitle("Total No. of Events");
    
    fHistCombPhi->SetXTitle("#phi_{t}^{truth} (rad)");
    fHistCombTotPhi->SetXTitle("#phi_{t}^{truth} (rad)");

	
    // Pseudo-rapidity for combination
    fHistCombEta = new TH1F("h_comb_npass_eta", "Combination",
				100, -5, 5);
    fHistCombTotEta = new TH1F("h_comb_ntot_eta", "Combination",
				100, -5, 5);
    
    fHistCombEta->SetYTitle("No. of Passed Events");
    fHistCombTotEta->SetYTitle("Total No. of Events");
    
    fHistCombEta->SetXTitle("#eta_{t}^{truth}");
    fHistCombTotEta->SetXTitle("#eta_{t}^{truth}");
    savdir->cd();
}

//____________________________________________________________________

void AtlTriggerTool::FillHistogramsNoEvtSel() {
    //
    // Fill histograms for the number of passed and all events for every
    // trigger item as a function of Pt, phi and eta of the
    // corresponding MC object
    //
    Int_t passed_comb = 0;
    Float_t pt_max  = 0.;
    Float_t phi_max = 0.;
    Float_t eta_max = 0.;

    // Check if only MC events are being processed - abort otherwise
    if ( fEvent->IsDATA() ) {
	Error("FillHistogramsNoEvtSel", "Processing real DATA events is not possible with the AtlTriggerTool. Check your configuration!");
	gSystem->Abort(0);
    }
    
    // ================
    // Individiul items
    // ================
    
    // Loop over all given trigger items
    TIter next_item(fListOfTriggerItems);
    AtlTriggerToolItem *item = 0;
    while ( (item = (AtlTriggerToolItem*)next_item()) ) {
	if ( item->fTriggerType == AtlTriggerToolItem::kUnknownType ) {
	    Error("FillHistogram",
		  "Unknown type of trigger item \"%s\". Please check. No histogram booked.",
		  item->GetName());
	    continue;
	}
	if ( item->fTriggerLevel == AtlTriggerToolItem::kUnknownLevel ) {
	    Error("FillHistogram",
		  "Unknown level of trigger item \"%s\". Please check. Histogram will not be filled.",
		  item->GetName());
	    continue;
	}

	Float_t pt  = 0.;
	Float_t phi = 0.;
	Float_t eta = 0.;

	// Obtain Pt, phi and eta for the different trigger types
	switch ( item->fTriggerType ) {
	    case AtlTriggerToolItem::kElectron: {
		// Get truth e+ or e- with highest Pt
 		TList *mc_eplus = fEvent->GetMCParticles("e+");
		TList *mc_eminus = fEvent->GetMCParticles("e-");
		TList *mc_el = new TList;
		mc_el->AddAll(mc_eplus);
		mc_el->AddAll(mc_eminus);
		mc_el->Sort(kSortDescending);
		pt  = ((HepMCParticle*)mc_el->First())->Pt();
		phi = ((HepMCParticle*)mc_el->First())->Phi();
		eta = ((HepMCParticle*)mc_el->First())->Eta();
		if ( pt > pt_max ) {
		    pt_max  = pt;
		    phi_max = phi;
		    eta_max = eta;
		}
		delete mc_eplus;
		delete mc_eminus;
		delete mc_el;
		break;
	    }
	    case AtlTriggerToolItem::kMuon: {
		// Get truth mu+ or mu- with highest Pt
		TList *mc_muplus = fEvent->GetMCParticles("mu+");
		TList *mc_muminus = fEvent->GetMCParticles("mu-");
		TList *mc_mu = new TList;
		mc_mu->AddAll(mc_muplus);
		mc_mu->AddAll(mc_muminus);
		mc_mu->Sort(kSortDescending);
		pt  = ((HepMCParticle*)mc_mu->First())->Pt();
		phi = ((HepMCParticle*)mc_mu->First())->Phi();
		eta = ((HepMCParticle*)mc_mu->First())->Eta();
		if ( pt > pt_max ) {
		    pt_max  = pt;
		    phi_max = phi;
		    eta_max = eta;
		}
		delete mc_muplus;
		delete mc_muminus;
		delete mc_mu;
		break;
	    }
	    case AtlTriggerToolItem::kJet: {
		// Get Pt, phi and eta of the cone4 hadron level jet
		// with the largest Et
		AtlJet *jet = fEvent->GetLeadingMCCone4HadronJet();
		pt  = jet->Pt();
		phi = jet->Phi();
		eta = jet->Eta();
		if ( pt > pt_max ) {
		    pt_max  = pt;
		    phi_max = phi;
		    eta_max = eta;
		}
		break;
	    }
	    case AtlTriggerToolItem::kEM: {
		// Get truth e+, e- or gamma with highest Pt
		TList *mc_eplus = fEvent->GetMCParticles("e+");
		TList *mc_eminus = fEvent->GetMCParticles("e-");
		TList *mc_gamma = fEvent->GetMCParticles("gamma");
		TList *mc_em = new TList;
		mc_em->AddAll(mc_eplus);
		mc_em->AddAll(mc_eminus);
		mc_em->AddAll(mc_gamma);
		mc_em->Sort(kSortDescending);
		pt  = ((HepMCParticle*)mc_em->First())->Pt();
		phi = ((HepMCParticle*)mc_em->First())->Phi();
		eta = ((HepMCParticle*)mc_em->First())->Eta();
		if ( pt > pt_max ) {
		    pt_max  = pt;
		    phi_max = phi;
		    eta_max = eta;
		}
		delete mc_eplus;
		delete mc_eminus;
		delete mc_gamma;
		delete mc_em;
		break;
	    }
	    default:
		;
	}

	// Event has passed the trigger item in question ?
	Bool_t passed = kFALSE;
	if ( item->fTriggerLevel == AtlTriggerToolItem::kLV1 ) {
	    if ( fEvent->HasPassedL1(item->GetName()) ) {
		passed = kTRUE;
		passed_comb++;
	    }
	    
	} else if ( item->fTriggerLevel == AtlTriggerToolItem::kLV2 ||
		    item->fTriggerLevel == AtlTriggerToolItem::kEF ) {
	    if ( fEvent->HasPassedHLT(item->GetName()) ) {
		passed = kTRUE;
		passed_comb++;
	    }
	}
	
	// Fill the histograms for the no. of passed events
	if ( passed == kTRUE ) {
	    item->fHistNPassPt ->Fill(pt,  GetPreTagEvtWeight());
	    item->fHistNPassPhi->Fill(phi, GetPreTagEvtWeight());
	    item->fHistNPassEta->Fill(eta, GetPreTagEvtWeight());
	}

	// Fill the histograms for the no. of all events
	item->fHistNTotPt ->Fill(pt,  GetPreTagEvtWeight());
	item->fHistNTotPhi->Fill(phi, GetPreTagEvtWeight());
	item->fHistNTotEta->Fill(eta, GetPreTagEvtWeight());
    }

    // =============
    // Combined case
    // =============
    
    // Fill the histograms for the no. of passed events
    if ( passed_comb >= 0 ) {
	fHistCombPt ->Fill(pt_max,  GetPreTagEvtWeight());
	fHistCombPhi->Fill(phi_max, GetPreTagEvtWeight());
	fHistCombEta->Fill(eta_max, GetPreTagEvtWeight());
    }

    // Fill the histograms for the no. of all events to be used in
    // combined case
    fHistCombTotPt ->Fill(pt_max, GetPreTagEvtWeight());
    fHistCombTotPhi->Fill(pt_max, GetPreTagEvtWeight());
    fHistCombTotEta->Fill(pt_max, GetPreTagEvtWeight());
}

//____________________________________________________________________

void AtlTriggerTool::SetBranchStatus() {
    //
    // Set branch status
    //
    fTree->SetBranchStatus("fTrigger*", kTRUE);
    fTree->SetBranchStatus("fEventHeader.fIsMC*", kTRUE);
}

//____________________________________________________________________

void AtlTriggerTool::Terminate() {
   //
   // Termination routine for the trigger tool.
   // Used for the computation of the efficiency graphs
   //
   TDirectory *savdir = gDirectory;

   // ================
   // Individual items
   // ================
   
   // Loop over all given items and create the efficiency graphs
   TIter next_item(fListOfTriggerItems);
   AtlTriggerToolItem *item = 0;
   while ( (item = (AtlTriggerToolItem*)next_item()) ) {

	// Create graphs
	fOutputFile->cd();
	gDirectory->cd(Form("%s/%s/", GetName(), item->GetName()));
	TGraphAsymmErrors *g_pt  = new TGraphAsymmErrors(item->fHistNPassPt->GetNbinsX());
	g_pt->SetName(Form("g_%s_pt", item->GetName()));
	g_pt->SetTitle(Form("Efficiency vs. Transverse Momentum : %s", item->GetName()));

	TGraphAsymmErrors *g_phi = new TGraphAsymmErrors(item->fHistNPassPhi->GetNbinsX());	
	g_phi->SetName(Form("g_%s_phi", item->GetName()));
	g_phi->SetTitle(Form("Efficiency vs. Azimuth : %s", item->GetName()));

	TGraphAsymmErrors *g_eta = new TGraphAsymmErrors(item->fHistNPassEta->GetNbinsX());
	g_eta->SetName(Form("g_%s_eta", item->GetName()));
	g_eta->SetTitle(Form("Efficiency vs. Pseudo-Rapidity : %s", item->GetName()));

	// Re-compute sum of weights
	item->fHistNPassPt->Sumw2();
	item->fHistNPassPhi->Sumw2();
	item->fHistNPassEta->Sumw2();
	item->fHistNTotPt->Sumw2();
	item->fHistNTotPhi->Sumw2();
	item->fHistNTotEta->Sumw2();

	// Compute efficiencies
	g_pt ->BayesDivide(item->fHistNPassPt,      item->fHistNTotPt);
	g_phi->BayesDivide(item->fHistNPassPhi,     item->fHistNTotPhi);
	g_eta->BayesDivide(item->fHistNPassEta,     item->fHistNTotEta);

	// Set axis titles
	g_pt->GetHistogram()->GetXaxis()->SetTitle("P_{t} (GeV)");
	g_pt->GetHistogram()->GetYaxis()->SetTitle("#epsilon");

	g_phi->GetHistogram()->GetXaxis()->SetTitle("phi (rad)");
	g_phi->GetHistogram()->GetYaxis()->SetTitle("#epsilon");
	
	g_eta->GetHistogram()->GetXaxis()->SetTitle("#eta");
	g_eta->GetHistogram()->GetYaxis()->SetTitle("#epsilon");

	
	// Write graphs
	g_pt ->Write();
	g_phi->Write();
	g_eta->Write();
   }

   // =============
   // Combined case
   // =============
   
   // Combination of trigger items
   TGraphAsymmErrors *g_combpt = new TGraphAsymmErrors(fHistCombPt->GetNbinsX());
   g_combpt->SetName("g_comb_pt");
   g_combpt->SetTitle("Efficiency vs. Transverse Momentum : Combination");
   
   TGraphAsymmErrors *g_combphi = new TGraphAsymmErrors(fHistCombPhi->GetNbinsX());	
   g_combphi->SetName("g_comb_phi");
   g_combphi->SetTitle("Efficiency vs. Azimuth : Combination");
   
   TGraphAsymmErrors *g_combeta = new TGraphAsymmErrors(fHistCombEta->GetNbinsX());
   g_combeta->SetName("g_comb_eta");
   g_combeta->SetTitle("Efficiency vs. Pseudo-Rapidity : Combination");

   fHistCombPt->Sumw2();
   fHistCombPhi->Sumw2();
   fHistCombEta->Sumw2();

   g_combpt ->BayesDivide(fHistCombPt,   fHistCombTotPt);
   g_combphi->BayesDivide(fHistCombPhi,  fHistCombTotPhi);
   g_combeta->BayesDivide(fHistCombEta,  fHistCombTotEta);
   
   
   g_combpt->GetHistogram()->GetXaxis()->SetTitle("P_{t} (GeV)");
   g_combpt->GetHistogram()->GetYaxis()->SetTitle("#epsilon");
   
   g_combphi->GetHistogram()->GetXaxis()->SetTitle("phi (rad)");
   g_combphi->GetHistogram()->GetYaxis()->SetTitle("#epsilon");
   
   g_combeta->GetHistogram()->GetXaxis()->SetTitle("#eta");
   g_combeta->GetHistogram()->GetYaxis()->SetTitle("#epsilon");

   g_combpt ->Write();
   g_combphi->Write();
   g_combeta->Write();

   savdir->cd();
}

//____________________________________________________________________

void AtlTriggerTool::Print(Option_t *option) const {
    //
    // Print this object
    //
    cout << endl
	 << "==============================================" << endl
	 << "Trigger tool " << GetName() << endl
	 << "----------------------------------------------" << endl
	 << "Selection string = " << fTriggerSelection->Data() << endl
	 << "==============================================" << endl << endl;
}

    
    

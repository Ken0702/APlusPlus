//____________________________________________________________________
//
// Wrapper for Soeren's Pdf reweighting class
// needed for the evaluation of Pdf systematics
// 
// Simply create the tool in your task (see AtlAppAnalysisTask) or
// your analysis selector (AtlSelector). Choose the systematics mode
// (up/down/nominal) by setting the public data member fSystMode (eg
// by using AtlAppAnalysisTask::SetToolVariable()).
//
// Next add the histograms of the observable you want to reweight by
// using AddObservable(). Here you have to give the full path name to
// of each histogram inside the output file. Note that currently
// 1-dim. and 2-dim. histograms are supported. Hint: in case of event
// counting use a single-bin histogram. AddObservable() can be called
// anytime before the event loop, for instance in BookHistograms().
//

//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2012 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlPdfReweightingTool
#include <AtlPdfReweightingTool.h>
#endif
#include <AtlSelector.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TObjString.h>
#include <TFile.h>
#include <TSystem.h>
#include <TROOT.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlPdfReweightingTool);
#endif

//____________________________________________________________________

AtlPdfReweightingTool::AtlPdfReweightingTool(const char* name,
					     const char* title) :
    AtlAnalysisTool(name, title) {
    //
    // Default constructor
    //
    fPdfReweightingTool = 0;
    fProcessMode = kPostAnalysis;
    fSystMode = kNominal;
    fFirstEvent = kTRUE;
    fPdfReweightingTool = 0;
    fListObsNames    = new TList;
    fListObsHistsOrig = new TList;
    fListObsHistsTmp = new TList;
    fDSID = 0;
}

//____________________________________________________________________

AtlPdfReweightingTool::~AtlPdfReweightingTool() {
    //
    // Default destrcuctor
    //
    if ( fPdfReweightingTool != 0 )  delete fPdfReweightingTool;
    fListObsNames->Delete(); delete fListObsNames;
    delete fListObsHistsOrig;
    fListObsHistsTmp->Delete(); delete fListObsHistsTmp;
}

//____________________________________________________________________

void AtlPdfReweightingTool::BookHistograms() {
    //
    // Book observables
    //
}

//____________________________________________________________________

Bool_t AtlPdfReweightingTool::AnalyzeEvent() {
    //
    // Compute Pdf weights
    //

    // In the nominal case do nothing
    if ( fSystMode == kNominal ) return kTRUE;

    // First event ?
    // Add observables to Pdf tool
    if ( fFirstEvent ) {
	fFirstEvent = kFALSE;
	FirstEvent();
    }
    
    // Compute Pdf weights
    fPdfReweightingTool->ComputePdfWeights(fEvent->GetEventHeader()->Get_mcevt_pdf_x1(),
					   fEvent->GetEventHeader()->Get_mcevt_pdf_x2(),
					   fEvent->GetEventHeader()->Get_mcevt_pdf_scale(),
					   fEvent->GetEventHeader()->Get_mcevt_pdf_id1(),
					   fEvent->GetEventHeader()->Get_mcevt_pdf_id2(),
					   fEvent->GetEventHeader()->Get_mcevt_pdf1(),
					   fEvent->GetEventHeader()->Get_mcevt_pdf2());

    // Fill observables
    FillObservables();
    return kTRUE;
}

//____________________________________________________________________

void AtlPdfReweightingTool::Print() const {
    //
    // Prints chosen parameter settings for the tool to
    // standard output
    //
    cout << endl
         << "========================================================" << endl
         << "  Pdf Reweighting Tool                      " << endl
         << "========================================================" << endl
         << "  Systematics mode :  ";
        switch ( fSystMode ) {
	    case kUp:
		cout << "Up" << endl;
		break;
	    case kDown:
		cout << "Down" << endl;
		break;
	    case kNominal:
		cout << "Nominal (=do nothing)" << endl;
	}
	cout << "--------------------------------------------------------" << endl;
	cout << "  List of observables:" << endl;
	TIter next_obs(fListObsNames);
	TObjString *objstr = 0;
	while ( (objstr = (TObjString*)next_obs()) )
	    cout << "     " << objstr->GetString().Data() << endl;
	cout << "--------------------------------------------------------" << endl;
	fPdfReweightingTool->Print("PDFinfo");
	cout << "========================================================" << endl
	     << endl;
}

//____________________________________________________________________

void AtlPdfReweightingTool::SetBranchStatus() {
    //
    // Enable on needed branches
    //
    fTree->SetBranchStatus("fEventHeader*", kTRUE);
}

//____________________________________________________________________

void AtlPdfReweightingTool::Init() {
    //
    // Init Pdf tool
    //
    Info("Init", "Start LHAPDF interface ...");
    fPdfReweightingTool = PdfReweightingTool::Instance();
}

//____________________________________________________________________

void AtlPdfReweightingTool::AddObservable(const char* HistName) {
    //
    // Add observable to reweight.
    //
    // The given histogram is the histogram inside the user analysis
    // used to histogram the observable in question. The full path
    // name of the histogram inside the output file is needed here.
    //
    // In case of event counting use a single-bin histogram.
    //
    fListObsNames->Add(new TObjString(HistName));
}

//____________________________________________________________________

void AtlPdfReweightingTool::FillObservables() {
    //
    // Fill observables
    //
    TDirectory *savdir = gDirectory;
    gROOT->cd();
    
    // Loop over all observables
    TH1F *h_tmp = 0;
    TH1F *h_orig = 0;
    TIter next_orig(fListObsHistsOrig);
    while ( (h_orig = (TH1F*)next_orig()) ) {
	TString hname(h_orig->GetName());
	hname.Append("_apptmp");
	    
	// Check each bin for differences against reference (original histogram)
	// Differences mean that a bin in the original histogram was filled
	switch ( h_orig->GetDimension() ) {
	    case 1:
		// 1-dim.  case
		h_tmp = (TH1F*)fListObsHistsTmp->FindObject(hname.Data());
		for ( Int_t i = 0; i <= h_orig->GetNbinsX()+1; i++ ) {
		    if ( h_tmp->GetBinContent(i) != h_orig->GetBinContent(i) ) {
			h_tmp->SetBinContent(i, h_orig->GetBinContent(i));
			h_tmp->SetBinError(i, h_orig->GetBinError(i));
			fPdfReweightingTool->FillObservable(h_tmp , h_tmp->GetBinCenter(i),
							    fEvent->GetTagEvtWeight());
			break;
		    }
		}
		break;
	    case 2:
		// 2-dim. case
		for ( Int_t j = 0; j <= ((TH2F*)h_orig)->GetNbinsY()+1; j++ ) {
		    h_tmp = (TH1F*)fListObsHistsTmp
			   ->FindObject(Form("%s_ybin%-d", hname.Data(), j));
		    for ( Int_t i = 0; i <= h_orig->GetNbinsX()+1; i++ ) {
			if ( h_tmp->GetBinContent(i) != h_orig->GetBinContent(i, j) ) {
			    h_tmp->SetBinContent(i, h_orig->GetBinContent(i, j));
			    h_tmp->SetBinError(i, h_orig->GetBinError(i, j));
			    fPdfReweightingTool->FillObservable(h_tmp , h_tmp->GetBinCenter(i),
								fEvent->GetTagEvtWeight());
			break;
			}
		    }
		}
		break;
	}
    }
    savdir->cd();
}

//____________________________________________________________________

void AtlPdfReweightingTool::Terminate() {
    //
    // Terminate routine called by AtlSelector::SlaveTerminate().
    //
    // Here, the final Pdf uncertainty is determined. The original
    // histograms for each given observable are replaced with the
    // respected up/down variations.
    //
    Info("Terminate", "Terminate Pdf reweighting ...");
    if ( fSystMode == kNominal ) return;
    TDirectory *savdir = gDirectory;
    gROOT->cd();
    
    // Get varied histograms and replace the original ones
    TH1F *h_orig = 0;
    TH1F *h_tmp = 0;
    TH1F *h_syst = 0;
    TIter next_orig(fListObsHistsOrig);
    while ( (h_orig = (TH1F*)next_orig()) ) {
	if ( fSystMode == kUp ) {
	    Info("Terminate", "Replace histogram \"%s\" by Pdf systematic upward variation.",
		 h_orig->GetName());
	} else if ( fSystMode == kDown ) {
	    Info("Terminate", "Replace histogram \"%s\" by Pdf systematic downward variation.",
		 h_orig->GetName());
	}

	Int_t ny = ( h_orig->GetDimension() == 1 ) ? 0 : h_orig->GetNbinsY()+1;
	for ( Int_t j = 0; j <= ny; j++ ) {
	    TString hname(h_orig->GetName());
	    hname.Append("_apptmp");

	    if ( h_orig->GetDimension() == 2 ) hname.Append(Form("_ybin%-d", j));
	    h_tmp = (TH1F*)fListObsHistsTmp->FindObject(hname.Data());
	    
	    if ( fSystMode == kUp ) {
		h_syst = fPdfReweightingTool->PdfUncertainty(h_tmp, PdfSet::kUp);
	    } else if ( fSystMode == kDown ) {
		h_syst = fPdfReweightingTool->PdfUncertainty(h_tmp, PdfSet::kDown);
	    }
	    for ( Int_t i = 0; i <= h_orig->GetNbinsX()+1; i++ ) {
		if ( h_orig->GetDimension() == 1 ) {
		    h_orig->SetBinContent(i, h_syst->GetBinContent(i));
		    h_orig->SetBinError(i, h_syst->GetBinError(i));
		} else {
		    ((TH2F*)h_orig)->SetBinContent(i, j, h_syst->GetBinContent(i));
		    ((TH2F*)h_orig)->SetBinError(i, j, h_syst->GetBinError(i));
		}
	    }
	}
    }

    // Clean-up
    delete fPdfReweightingTool;
    fPdfReweightingTool = 0;
    savdir->cd();
}

//____________________________________________________________________

void AtlPdfReweightingTool::FirstEvent() {
    //
    // Initialize normalization scale factors and
    // add observables to the Pdf tool
    //

    if ( fDSID != 0 ) {
	fPdfReweightingTool->SetApplyNormfactor(kTRUE);
	fPdfReweightingTool->GetNormfactor(fDSID);
    }

    
    // Loop over all given observable names
    // and add histograms to temporary list
    cout << endl;
    Info("FirstEvent", "Adding observables to Pdf tool ...");
    TFile *f_out = fParent->GetOutputFile();
    TH1F *h_orig = 0;
    TObjString *objstr = 0;
    TIter next_obs(fListObsNames);
    while ( (objstr = (TObjString*)next_obs()) ) {
	h_orig = (TH1F*)f_out->Get(objstr->GetString().Data());
	if ( h_orig == 0 ) {
	    Error("FirstEvent", "Cannot find histogram \"%s\". Please check given histogram name. Abort !",
		  objstr->GetString().Data());
	}
	
	// Check dimension
	if ( h_orig->GetDimension() > 2 ) {
	    Error("FirstEvent", "Histogram \"%s\" has more than 2 dimensions. Currently only 1-dim. and 2-dim. histograms are supported. Abort !", h_orig->GetName());
	    gSystem->Abort(1);
	}

	// Handle 1d/2d case
	TDirectory *savdir = gDirectory;
	gROOT->cd();
	switch ( h_orig->GetDimension() ) {
	    case 1:
		AddObs1D(h_orig); break;
	    case 2:
		AddObs2D((TH2F*)h_orig); break;
	}
	savdir->cd();
	Info("FirstEvent", "Added observable \"%s\" for Pdf reweighting.",
	     h_orig->GetName());
    }
}

//____________________________________________________________________

void AtlPdfReweightingTool::AddObs1D(TH1F *h_orig) {
    //
    // Add 1-dim. histogram to the list of observables of the Pdf tool
    //
    fListObsHistsOrig->Add(h_orig);
    TH1F *h_tmp = (TH1F*)h_orig->Clone(Form("%s_apptmp", h_orig->GetName()));
    h_tmp->Reset();
    fListObsHistsTmp->Add(h_tmp);
    fPdfReweightingTool->AddObservable(h_tmp);
} 

//____________________________________________________________________

void AtlPdfReweightingTool::AddObs2D(TH2F *h_orig) {
    //
    // Add 2-dim. histogram to the list of observables of the Pdf tool
    //
    // Internally the 2-dim. histogram is splitted into 1-dim.
    // histograms for each bin along the y-axis.
    //
    fListObsHistsOrig->Add(h_orig);
    TH1F *h_proj = 0;
    for ( Int_t i = 0; i <= h_orig->GetNbinsY()+1; i++ ) {
	h_proj = (TH1F*)h_orig->ProjectionX(Form("%s_apptmp_ybin%-d",
						 h_orig->GetName(), i), i, i);
	h_proj->Reset();
	fListObsHistsTmp->Add(h_proj);
	fPdfReweightingTool->AddObservable(h_proj);
    }
}

//____________________________________________________________________

TList* AtlPdfReweightingTool::FIndObjects(TList *list, const TRegexp& pat) {
    //
    // Find all objects inside the given list whose names fulfill the
    // regular expression.
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! The user is responsible for deleting the returned list !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    TList *sublist = new TList;
    TIter next(list);
    TObject *obj;
    while ( (obj = next()) ) {
	TString name(obj->GetName());
	if ( name.Contains(pat) ) sublist->Add(obj);
    }
    return sublist;
}

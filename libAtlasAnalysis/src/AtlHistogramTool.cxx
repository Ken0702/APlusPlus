//____________________________________________________________________
//
// BEGIN_HTML
// <h2>Histogram tool</h2>
// <p>
// The tool allows to book and fill many histograms inside an analysis
// in a transparent and efficient way.
// </p>
// <p>
// For each tool a folder with the name of the tool is created inside
// the output file of the analysis job. The histograms can also be
// managed in subfolders simply by prepending the path in the
// hisotgram name (see the example below). The subfolders are created
// automatically if they do not exist.
// </p>
// <p>
// <h3>Example:</h3>
// MyAnalysis.h:
// <pre>
// private:
//   AtlHistogramTool *fHistograms1;   // List 1
//   AtlHistogramTool *fHistograms2;   // List 2
//   ...
// </pre>
// MyAnalysis::BookHistograms():
// <pre>
//
// // Pre-tagged histograms
// fHistograms1 = new AtlHistogramTool("PreTag",
//                                     "Pre-tagged histograms");
// AddTool(fHistograms1)
//
// // Leading lepton Pt
// fHistograms1->Add("leptons/h_lep1_Pt", "Leading Lepton Pt", 40, 0., 200.,
//                   "Leading lepton p_{T} [GeV]", "Events");
//
// // Leading lepton eta
// fHistograms1->Add("leptons/h_lep1_Eta", "Leading Lepton Eta",
//                   50, -2.5, 2.5, "Leading lepton #eta", "Events");
//
// // Leading electron Pt
// fHistograms1->Add("leptons/el/h_el1_Pt", "Leading Electron Pt", 40, 0., 200.,
//                   "Leading electron p_{T} [GeV]", "Events");
// ...
//
// // b-tagged histograms
// fHistograms2 = new AtlHistogramTool("bTag",
//                                     "b-tagged histograms");
// AddTool(fHistograms2)
//
// // Leading jet Pt
// fHistograms2->Add("jets/h_jet1_Pt", "Leading Jet Pt",
//                   40, 0., 200., "Leading jet p_{T} [GeV]", "Events");
//    
// // Leading jet eta
// fHistograms2->Add("jets/h_jet1_Eta", "Leading Jet Eta",
//                   100, -5., 5., "Leading jet #eta", "Events");
// ...
// </pre>
// 
// MyAnalysis::ProcessCut():
// <pre>
// fHistograms1->Fill("leptons/h_lep1_Pt",  lep1->Pt(), w1);
// fHistograms1->Fill("leptons/h_lep1_Eta", lep1->Eta(), w1);
// fHistograms1->Fill("leptons/el/h_el1_Pt",  el1->Pt(), w1);
// ...
// // Perform b-tagging
// ...
// fHistograms1->Fill("jets/h_jet1_Pt",  jet1->Pt(), w2);
// fHistograms1->Fill("jets/h_jet1_Eta", jet1->Eta(), w2);
// ...
// </pre>
//
// END_HTML
//  
//  
// Author: Oliver Maria Kind <mailto: kind@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlHistogramTool
#include <AtlHistogramTool.h>
#endif
#include <AtlSelector.h>
#include <TFile.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlHistogramTool);
#endif

//____________________________________________________________________

AtlHistogramTool::AtlHistogramTool(const char* name, const char* title) :
    AtlAnalysisTool(name, title) {
    //
    // Default constructor
    //
    fProcessMode = kIndividual;
    fParentDir = 0;
    fHistograms = new THashList();
}

//____________________________________________________________________

AtlHistogramTool::~AtlHistogramTool() {
    //
    // Default destructor
    //
    fHistograms->Delete();
    delete fHistograms;
}

//____________________________________________________________________

TH1D* AtlHistogramTool::Add(TH1D* hist) {
    //
    // Add TH1D histogram from a reference histogram
    //
    TH1D *h = Add(hist->GetName(), hist->GetTitle(), hist->GetNbinsX(),
                  hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax(),
                  hist->GetXaxis()->GetTitle(), hist->GetYaxis()->GetTitle());
    return h;
}

//____________________________________________________________________

TH1D* AtlHistogramTool::Add(const char* hname, const char* title,
			    Int_t nbinsx, Double_t xlow, Double_t xup,
			    const char* xtitle, const char* ytitle) {
    //
    // Add TH1D histogram
    //
    // The histogram name can contain a path of subdirectories. These
    // directories are created automatically if they do not exist. All
    // path names are relative to the parent folder of the tool.
    //
    
    // Store current dirctory
    TDirectory *savdir = gDirectory;

    // Top-level folder exists ?
    if ( fParentDir == 0 ) {
	// Go to output file folder and create parent folder
	fParent->GetOutputFile()->cd();
	fParentDir = gDirectory->mkdir(GetName(), GetTitle());
    }
    fParentDir->cd();

    // Check if the histogram belongs to a subfolder
    // and create folder if necessary 
    TString dirname = gSystem->DirName(hname);
    if ( dirname != "." ) {
	MkDirWithParents(dirname.Data());
    }

    // Check if a histogram with the same name already exists in the
    // current folder
    const char* bname = gSystem->BaseName(hname); 
    if ( gDirectory->FindObject(bname) != 0 ) {
	Error("Add", "Histogram with given name \"%s\" exists already in the current folder"
	      "\"%s\". Please use a different name. Abort!", bname, gDirectory->GetPath());
	gSystem->Abort(1);
    }
    
    // Create and add histogram
    TH1D *h = new TH1D(bname, title, nbinsx, xlow, xup);
    h->SetXTitle(xtitle);
    h->SetYTitle(ytitle);
    fHistograms->Add(new AtlHistObject(hname, title, h));

    // Restore pwd
    savdir->cd();

    return h;
}

//____________________________________________________________________

TH2D* AtlHistogramTool::Add(const char* hname, const char* title,
			    Int_t nbinsx, Double_t xlow, Double_t xup,
			    Int_t nbinsy, Double_t ylow, Double_t yup,
			    const char* xtitle, const char* ytitle,
			    const char* ztitle) {
    //
    // Add TH2D histogram
    //
    // The histogram name can contain a path of subdirectories. These
    // directories are created automatically if they do not exist. All
    // path names are relative to the parent folder of the tool.
    //
    
    // Store current dirctory
    TDirectory *savdir = gDirectory;

    // Top-level folder exists ?
    if ( fParentDir == 0 ) {
	// Go to output file folder and create parent folder
	fParent->GetOutputFile()->cd();
	fParentDir = gDirectory->mkdir(GetName(), GetTitle());
    }
    fParentDir->cd();

    // Check if the histogram belongs to a subfolder
    // and create folder if necessary 
    TString dirname = gSystem->DirName(hname);
    if ( dirname != "." ) {
	MkDirWithParents(dirname.Data());
    }

    // Check if a histogram with the same name already exists in the
    // current folder
    const char* bname = gSystem->BaseName(hname); 
    if ( gDirectory->FindObject(bname) != 0 ) {
	Error("Add", "Histogram with given name \"%s\" exists already in the current folder. Please use a different name. Abort!", bname);
	gSystem->Abort(1);
    }
    
    // Create and add histogram
    TH2D *h = new TH2D(bname, title, nbinsx, xlow, xup, nbinsy, ylow, yup);
    h->SetXTitle(xtitle);
    h->SetYTitle(ytitle);
    h->SetZTitle(ztitle);
    fHistograms->Add(new AtlHistObject(hname, title, h));

    // Restore pwd
    savdir->cd();

    return h;
}

//____________________________________________________________________

TH2D* AtlHistogramTool::Add(const char* hname, const char* title,
			    Int_t nbinsx, Double_t xlow, Double_t xup,
			    Int_t nbinsy, const Double_t *ybins,
			    const char* xtitle, const char* ytitle,
			    const char* ztitle) {
    //
    // Add TH2D histogram
    //
    // The histogram name can contain a path of subdirectories. These
    // directories are created automatically if they do not exist. All
    // path names are relative to the parent folder of the tool.
    //
    
    // Store current dirctory
    TDirectory *savdir = gDirectory;

    // Top-level folder exists ?
    if ( fParentDir == 0 ) {
	// Go to output file folder and create parent folder
	fParent->GetOutputFile()->cd();
	fParentDir = gDirectory->mkdir(GetName(), GetTitle());
    }
    fParentDir->cd();

    // Check if the histogram belongs to a subfolder
    // and create folder if necessary 
    TString dirname = gSystem->DirName(hname);
    if ( dirname != "." ) {
	MkDirWithParents(dirname.Data());
    }

    // Check if a histogram with the same name already exists in the
    // current folder
    const char* bname = gSystem->BaseName(hname); 
    if ( gDirectory->FindObject(bname) != 0 ) {
	Error("Add", "Histogram with given name \"%s\" exists already in the current folder. Please use a different name. Abort!", bname);
	gSystem->Abort(1);
    }
    
    // Create and add histogram
    TH2D *h = new TH2D(bname, title, nbinsx, xlow, xup, nbinsy, ybins);
    h->SetXTitle(xtitle);
    h->SetYTitle(ytitle);
    h->SetZTitle(ztitle);
    fHistograms->Add(new AtlHistObject(hname, title, h));

    // Restore pwd
    savdir->cd();

    return h;
}

//____________________________________________________________________

void AtlHistogramTool::Fill(const char* name, Double_t x, Double_t w) {
    //
    // Fill given histogram with value x and weight w
    //
    // The histogram name needs to contain the full path of the
    // histogram as given when booking the histogram using the Add()
    // member function.
    //
    AtlHistObject *h_obj = (AtlHistObject*)fHistograms->FindObject(name);
    if ( h_obj == 0 ) {
	Error("Fill",
	      "Histogram \"%s\" not found. Check histogram name! Abort.",
	      name);
	gSystem->Abort(1);
    }
    TH1D *h = (TH1D*)h_obj->GetHistogram();
    h->Fill(x, w);
}

//____________________________________________________________________

void AtlHistogramTool::Fill(const char* name, Double_t x, Double_t y,
			    Double_t w) {
    //
    // Fill given histogram with values x,y and weight w
    //
    // The histogram name needs to contain the full path of the
    // histogram as given when booking the histogram using the Add()
    // member function.
    //
    AtlHistObject *h_obj = (AtlHistObject*)fHistograms->FindObject(name);
    if ( h_obj == 0 ) {
	Error("Fill",
	      "Histogram \"%s\" not found. Check histogram name! Abort.",
	      name);
	gSystem->Abort(1);
    }
    TH2D *h = (TH2D*)h_obj->GetHistogram();
    h->Fill(x, y, w);
}

//____________________________________________________________________

TDirectory* AtlHistogramTool::MkDirWithParents(const char* dir) {
    //
    // Create the given directoy and all of its parents if necessary
    // in the top-level directory
    //
    TString fulldir(dir);
    TObjArray *subdirs = fulldir.Tokenize("/");
    fParentDir->cd();
    TIter next_dir(subdirs);
    TObjString *subdir = 0;
    while ( (subdir = (TObjString*)next_dir()) ) {
        if ( gDirectory->FindObject(subdir->GetString().Data()) == 0 ) {
            gDirectory->mkdir(subdir->GetString().Data());
	    if ( gDebug > 0 ) {
		Info("MkDirWithParents", "Create folder %s/",
		     subdir->GetString().Data());
	    }
        }
        gDirectory->cd(subdir->GetString().Data());
    }

    //Clear objects
    delete subdirs;
    delete subdir;

    return gDirectory;
}

//____________________________________________________________________

void AtlHistogramTool::Print() const {
    //
    // Print tool configuration
    //
    Info("Print", "Registered analysis tool \"%s\" of type AtlHistogramTool with the following configuration:",
	 GetName());
    cout << endl;
    fParentDir->ls();
    cout << endl;
}

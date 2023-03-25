//____________________________________________________________________
//
// Run/Event Selection Tool
// 
// With the help of this tool an analysis can be run over a
// pre-selected list of events either given by a text file or by a
// flat Root tree. The tool is designed fot high speed and good
// performance, also for large lists.
//
// Usage:
// ======
// Note that when using tasks the content of the tool's data members
// can be set by using AtlAppAnalysiTask::AtlSetToolVariable() in the
// usual way.
//
// (1) Create tool with text file:
// AtlRunEvtSelectionTool *tool = new AtlRunEvtSelectionTool("runevttool", "Run/Evt Selection Tool");
// tool->fInputName("runevt.dat");
//
// The file runevt.dat looks like (events can be disordered):
// 1243393 1020
// 1243393 998
// 1243393 2346
// 1243393 10044
// 1243355 25
//
// In case the file has a different format, the format string for
// reading the file can be stored by the data member fInputFormat.
//
// (2) Create tool with ntuple:
// AtlRunEvtSelectionTool *tool = new AtlRunEvtSelectionTool("runevt_tool", "RunEvt Tool")
// tool->fInputFilename = "AntiElectronQCDModel.root"
// tool->fInputTreename = "AntiElectrons"
// tool->fBranchnameRun = "runnumber"
// tool->fBranchnameEvent = "eventnumber"
//
// (3) Inside the analysis simply ask by using
// Bool_t IsValid = tool->IsValidEvent();
// if the current event is contained in the list. This information can
// be used, for instance, in ProcessCuut to select only the given
// events. In a similar way an inverted selection would be possible as
// well.  The tool makes it easy to convert a given list (text fiel or
// ntuple) into an EntryList which can be used in a subsequent
// analysis.
//
//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2011 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlRunEvtSelectionTool
#include <AtlRunEvtSelectionTool.h>
#endif
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TList.h>
#include <TSystem.h>
#include <TROOT.h>
#include <iostream>
#include <fstream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlRunEvtSelectionTool);
#endif

//____________________________________________________________________

AtlRunEvtSelectionTool::AtlRunEvtSelectionTool(const char* name,
					       const char* title) :
    AtlAnalysisTool(name, title) {
    //
    // Default constructor
    //
    fProcessMode = kPreAnalysis;
    fListOfRuns = new TList;
    fInputFilename = "";
    fInputFormat = "%d%d";
    fLastRun = 0;
    fNRuns = 0;
    fNEvents = 0;
}

//____________________________________________________________________

AtlRunEvtSelectionTool::~AtlRunEvtSelectionTool() {
    //
    // Default destructor
    //
    fListOfRuns->Delete();
    delete fListOfRuns;
}

//____________________________________________________________________

void AtlRunEvtSelectionTool::BookHistograms() {
    //
    // Initialze the tool
    //
    ParseInputFile();
}

//____________________________________________________________________

void AtlRunEvtSelectionTool::ParseInputFile() {
    //
    // Read from input file (either text or Root file and setup
    // run-event structure for fast comparison
    //

    // Re-set run-evt structure
    Reset();
    
    // Type of file ?
    TString line;
    FILE *ftype = gSystem->OpenPipe(Form("file %s", fInputFilename.Data()), "r");
    line.Gets(ftype);
    gSystem->ClosePipe(ftype);
    if ( line.Contains("ROOT file") ) {
	ParseRootFile();
    } else if ( line.Contains("ASCII text") ) {
	ParseTextFile();
    } else {
	Error("ParseInputFile",
	      "Unrecognized file format \"%s\". Abort!",
	      line.Data());
	gSystem->Abort(0);
    }
}

//____________________________________________________________________

void AtlRunEvtSelectionTool::ParseRootFile() {
    //
    // Parse Root input file
    //

    // Open input file and fetch the tree
    TFile *f_in = new TFile(fInputFilename.Data(), "read");
    TTree *t_in = (TTree*)f_in->Get(fInputTreename.Data());
    Int_t run = 0; Int_t evt = 0;
    t_in->SetBranchAddress(fBranchnameRun.Data(),   &run);
    t_in->SetBranchAddress(fBranchnameEvent.Data(), &evt);

    // Read tree
    for ( Int_t i = 0; i < t_in->GetEntries(); i++ ) {
	t_in->GetEntry(i);
	cout << "Run " << run << "   Evt " << evt << endl;
	Add(run, evt);
    }	
}

//____________________________________________________________________

void AtlRunEvtSelectionTool::ParseTextFile() {
    //
    // Parse text (ASCII) input file
    //

    // Open input file
    ifstream infile(fInputFilename.Data());
    if ( !infile.good() ) {
	Error("ParseTextFile", "Cannot open input file. Abort!");
    }

    // Read input file
    std::string line;
    Int_t run = 0; Int_t evt = 0;
    while ( std::getline(infile, line, '\n') ) {
	if ( 2 != sscanf(line.c_str(), fInputFormat, &run, &evt) ) {
	    continue; // skip empty and ill-formed lines
	}
	cout << "Run " << run << "   Evt " << evt << endl;
	Add(run, evt);
    }
}

//____________________________________________________________________

void AtlRunEvtSelectionTool::Add(Int_t run, Int_t evt) {
    //
    //  Add run/evt to the internal structure
    //  Prevent from double filling
    //
    AtlRunItem *item = 0;
    
    // First run or same run as last time ?
    if ( fLastRun == 0 ) {
	item = AddRun(run);
	fLastRun = item;
    } else {
	if ( run != fLastRun->fRunNr ) {
	    
	    // Run already added ?
	    item = FindRunItem(run);
	    if ( item == 0 ) item = AddRun(run);
	    
	    // Sort list of runs
	    fListOfRuns->Sort();
	    fLastRun = item;
	} else {
	    item = fLastRun;
	}
    }

    // Add event and sort event list (only if not already added before)
    if ( !std::binary_search(item->fEvents->begin(), item->fEvents->end(), evt) ) {
	item->fEvents->push_back(evt);
	std::sort(item->fEvents->begin(), item->fEvents->end());
	fNEvents++;
    }
}

//____________________________________________________________________

void AtlRunEvtSelectionTool::ExportAsText() {
    //
    // Export run-evt structure as text
    //
    TIter next_run(fListOfRuns);
    AtlRunItem *item = 0;
    while ( (item = (AtlRunItem*)next_run()) ) {
	for ( vector<int>::const_iterator it = item->fEvents->begin();
	      it != item->fEvents->end(); ++it ) {
	    cout << "Run " << item->fRunNr << "   Evt " << *it << endl;
	}
    }
}

//____________________________________________________________________

AtlRunItem* AtlRunEvtSelectionTool::FindRunItem(Int_t run) {
    //
    // Returns pointer to item with given run number - otherwise 0
    //
    TIter next_run(fListOfRuns);
    AtlRunItem *item = 0;
    while ( (item = (AtlRunItem*)next_run()) ) {
	if ( item->fRunNr == run ) return item;
    }
    return 0;
}

//____________________________________________________________________

AtlRunItem* AtlRunEvtSelectionTool::AddRun(Int_t run) {
    //
    // Add run item to the list of runs
    //
    AtlRunItem *item = new AtlRunItem(run);
    fListOfRuns->Add(item);
    fNRuns++;
    return item;
}

//____________________________________________________________________

Bool_t AtlRunEvtSelectionTool::Contains(Int_t run, Int_t evt,
					Bool_t remove) {
    //
    // Checks if run/evt is contained in existing run-evt structure
    //
    AtlRunItem *item = FindRunItem(run);
    if ( item == 0 ) return kFALSE;
    Bool_t IsContained = std::binary_search(item->fEvents->begin(),
					    item->fEvents->end(), evt);
    if ( remove == kFALSE || IsContained == kFALSE ) return IsContained;
    cout << "Size " << item->fEvents->size();
    item->fEvents->erase(std::remove(item->fEvents->begin(), item->fEvents->end(), evt),
			 item->fEvents->end());
    cout << "   "  << item->fEvents->size() << endl;
    if ( item->fEvents->size() == 0 ) fListOfRuns->Remove(item);
    return IsContained;
}

//____________________________________________________________________

void AtlRunEvtSelectionTool::SetBranchStatus() {
    //
    // Switch on needed branches
    //
    fTree->SetBranchStatus("fEventHeader.fRunNr*",   kTRUE);
    fTree->SetBranchStatus("fEventHeader.fEventNr*", kTRUE);
}

//____________________________________________________________________

void AtlRunEvtSelectionTool::Reset() {
    //
    // Reset run-evt structure
    //
    fListOfRuns->Delete();
    fNRuns = 0;
    fNEvents = 0;
}

//____________________________________________________________________

void AtlRunEvtSelectionTool::Print() const {
    //
    // Print toof information
    //
    cout << endl
	 << "========================================================" << endl
	 << "  Run/Event Selection Tool \"" << GetName() << "\"" << endl
	 << "========================================================" << endl
	 << "Input file: " << fInputFilename.Data() << endl
	 << "========================================================" << endl
	 << endl;
}

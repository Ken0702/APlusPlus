//____________________________________________________________________
//
// Analysis Cut-Flow Tool
//
// This tools provides an weighted and an un-weighted cut-flow
// histogram. Both histograms are stored inside a folder at the
// top-most level of the analysis output file. The name of the tool is
// used for naming the folder as well as the histograms.
//
// Once created the tool can be used from inside the analysis simply
// by calling the Fill() routine from everywhere inside your analysis
// with the appropriate label, eg.
//
//   cutflow_tool->Fill("Cut 1");
//   ...
//   cutflow_tool->Fill("Cut 2");
//
// The labels are used for labelling the histogram bins of the
// corresponding cuts. The binning of the histograms is extended
// automatically everytime a not yet cut label is filled.
//
// This is sometimes not the desired behaviour, for instance when
// using the cut-flow histograms in combination with the
// HepDataMCPlotter. Here, a fixed number of bins is needed. This can
// be achieved by setting the number of bins with the public data
// member fNBins to a large enough value and disable the lable-deflate
// mechanism with the switch fLabelsDeflate.
//
// Also, the tool allows to create text files containing run and event
// numbers of those events passing a certain cut. Enabling a set of
// streams is done by providing a comma-separated list containing the
// corresponding cut labels used later when filling the
// histograms. This is achieved by using
// AtlAppAnalysisTask::SetToolVariable(), for instance:
//
//   mytask->SetToolVariable("MyCutFlowTool", "fRunEvtOutputList", "Cut 4,Cut 7,Cut 8,Cut 12,...");
//
// Here, for each of the given cuts a text file will be produced
// containing the run/event numbers of all events passing that
// particular cut. The default format is of the form "%d\t%d", where
// the first number is the run and the second the event number, eg.
//
//   120001   234
//   120001   1203
//   120003   576
//   ...
//
// Note that the format of the output streams can be changed easily eg. by using
//
//   mytaks->SetToolVariable("MyCutFlowTool", "fRunEvtOutputFormat", "Run %d   Evt %d");
//
// This example would result in output files like
//
//   Run 12001   Evt 234
//   Run 12001   Evt 1203
//   Run 12003   Evt 576
//   ...
//
//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2011 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlCutFlowTool
#include <AtlCutFlowTool.h>
#endif
//#include <AtlSelector.h>
#include <TDirectory.h>
#include <TObjArray.h>
#include <TString.h>

using namespace std;

//____________________________________________________________________

AtlCutFlowTool::AtlCutFlowTool(const char* name, const char* title) :
    AtlAnalysisTool(name, title) {
    //
    // Default constructor
    //
    fHistCutFlowWeighted   = 0;
    fHistCutFlowUnweighted = 0;
    fOutputStreamers = new TObjArray;
    fRunEvtOutputList = "";
    fRunEvtOutputFormat = "%d\t%d";
    fNBins = 1;
    fLabelsDeflate = kTRUE;
    fPassedSelection = kFALSE;
}

//____________________________________________________________________

AtlCutFlowTool::~AtlCutFlowTool() {
    //
    // Default detructor
    //
    if ( fHistCutFlowWeighted != 0 ) {
	delete fHistCutFlowWeighted;
	fHistCutFlowWeighted = 0;
    }
    if ( fHistCutFlowUnweighted != 0 ) {
	delete fHistCutFlowUnweighted;
	fHistCutFlowUnweighted = 0;
    }
    delete fOutputStreamers;
}

//____________________________________________________________________

void AtlCutFlowTool::Clear() {
    //
    // Routine setting variables to default values for each event
    //
  fPassedSelection = kFALSE;

}


//____________________________________________________________________

void AtlCutFlowTool::BookHistograms() {
    //
    // Book cut-flow histograms inside its own folder.
    //
    // The folder is located in the first level of the outputfile of
    // the parent analysis selector. The name of the folder is the
    // name of this tool.
    //
    // Also, the run/evt output streams (if any) are opened here.
    //

    // Save current directory
    TDirectory *savdir = gDirectory;

    // Create cut-flow folder
    fParent->GetOutputFile()->cd();
    if(strcmp(fOutputDir.Data(),"/") != 0) {
	gDirectory->cd(fOutputDir.Data());
    }
    gDirectory->cd(GetName());

    // Get number of bins (if bin labels are given)
    TObjArray *labels = 0;
    if ( fBinLabels.Length() > 0 ) {
	labels = fBinLabels.Tokenize(",");
	fNBins = labels->GetEntries();
    }

    // Book weighted histogram
    fHistCutFlowWeighted = new TH1F(Form("h_%s_weighted", GetName()),
				    Form("%s (weighted)", GetTitle()),
				    fNBins, 0., (Float_t)fNBins);
    fHistCutFlowWeighted->SetYTitle("Number of Entries");

    // Book un-weighted histogram
    fHistCutFlowUnweighted = new TH1F(Form("h_%s_unweighted", GetName()),
				      Form("%s (unweighted)", GetTitle()),
				      fNBins, 0., (Float_t)fNBins);
    fHistCutFlowUnweighted->SetYTitle("Number of Entries");

    // Set bin labels (if given) for both histograms
    if ( fBinLabels.Length() > 0 ) {
	for ( Int_t i = 0; i < fNBins; i++ ) {
	    TString label = ((TObjString*)labels->At(i))->GetString();
	    TAxis *axis = fHistCutFlowWeighted->GetXaxis();
	    axis->SetBinLabel(i+1, label.Data());
	    axis = fHistCutFlowUnweighted->GetXaxis();
	    axis->SetBinLabel(i+1, label.Data());
	}
    }

    // Return to previous folder
    savdir->cd();

    // Open run/evt streamers
    OpenStreamers();
}

//____________________________________________________________________

void AtlCutFlowTool::Fill(const char* label, Float_t weight) {
    //
    // Fill weight into bin labelled with given label of the weighted
    // histogram. The un-weighted histogram will be filled with 1.0.
    //
    // In case a bin with this label does not exist, a new bin will be
    // added. Note that Root doubles the number of bins in this case.
    // To prevent empty bins the cut-flow histograms will be deflated
    // in the Terminate() routine.
    //
    fHistCutFlowWeighted->Fill(label, weight);
    fHistCutFlowUnweighted->Fill(label, 1.);

    // Write streamer (if defined)
    AtlCutFlowStreamer *out = (AtlCutFlowStreamer*)fOutputStreamers
	->FindObject(label);
    if ( out != 0 ) {
	out->fRunEvtStreamer << Form(fRunEvtOutputFormat.Data(),
				     fParent->GetEvent()->RunNr(),
				     fParent->GetEvent()->EventNr()) << endl;
    }
}

//____________________________________________________________________

void AtlCutFlowTool::Terminate() {
    //
    // Terminate this tool
    //
    // The cut-flow histograms are deflated and any open
    // output streamer is closed
    //
    if ( fLabelsDeflate == kTRUE ) {
	fHistCutFlowWeighted->LabelsDeflate("X");
	fHistCutFlowUnweighted->LabelsDeflate("X");
    }
    CloseStreamers();
}

//____________________________________________________________________

void AtlCutFlowTool::Print() const {
    //
    // Print tool information
    //
    cout << endl
	 << "========================================================" << endl
	 << "  CutFlow Tool \"" << GetName() << "\"" << endl
	 << "========================================================" << endl
	 << "  fNBins         = " << fNBins << endl
	 << "  fLabelsDeflate = ";
    if ( fLabelsDeflate ) {
	cout << "true" << endl;
    } else {
	cout << "false" << endl;
    }
    if ( fOutputStreamers->GetEntries() > 0 ) {
	for ( Int_t i = 0; i < fOutputStreamers->GetEntries(); i++ ) {
	    cout << "  Run/evt output list enabled for cut \""
		 << ((AtlCutFlowStreamer*)fOutputStreamers->At(i))->GetName()
		 << "\"" << endl;
	}
 	cout << "  Run/evt format = \"" << fRunEvtOutputFormat.Data()
 	     << "\"" << endl;
    } else {
	cout << "  Run/evt output lists disabled." << endl;
    }

    cout << "========================================================"
	 << endl << endl;
}

//____________________________________________________________________

void AtlCutFlowTool::OpenStreamers() {
    //
    // Set output tokens from given string and open output streamers
    //

    // Reset list of streamers
    if ( fOutputStreamers->GetEntries() > 0 ) {
	CloseStreamers();
	fOutputStreamers->Delete();
    }

    // Tokenize string and open streamer
    TObjArray *OutputTokens = fRunEvtOutputList.Tokenize(",");
    for ( Int_t i = 0; i < OutputTokens->GetEntries(); i++ ) {
	TString cut = ((TObjString*)OutputTokens->At(i))->GetString();
	AtlCutFlowStreamer *out = new AtlCutFlowStreamer(cut.Data());
	fOutputStreamers->AddAt(out, i);
	out->fRunEvtStreamer.open(Form("CutFlow_%s_Cut_%s_RunEvt.dat",
				       GetName(), cut.Data()));
    }
}

//____________________________________________________________________

void AtlCutFlowTool::CloseStreamers() {
    //
    // Close all output streams and clean streamer list
    //
    for ( Int_t i = 0; i < fOutputStreamers->GetEntries(); i++ ) {
	AtlCutFlowStreamer *out = (AtlCutFlowStreamer*)fOutputStreamers->At(i);
	out->fRunEvtStreamer.close();
    }
    fOutputStreamers->Delete();
}

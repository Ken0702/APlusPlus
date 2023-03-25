
//
// MC sample inside HepDataMCPlotter
// Helper class for creating the histogram stacks
//
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef HEP_HepDataMCSample
#include <HepDataMCSample.h>
#endif
#include <iostream>
#include <TSystem.h>

using namespace std;

#ifndef __CINT__
ClassImp(HepDataMCSample);
#endif

//____________________________________________________________________

HepDataMCSample::HepDataMCSample() {
    //
    // Default constructor
    //
    fFile = 0;
    fHistogram = 0;
}

//____________________________________________________________________

HepDataMCSample::HepDataMCSample(const char* name, const char* title,
				 Color_t color, Style_t line_style,
				 Width_t line_width, Style_t fill_style) :
    TNamed(name, title), fAttFill(color, fill_style),
    fAttLine(color, line_style, line_width) {
    //
    // Normal constructor
    //
    fNEvents = 0.;
    fXsection = 0.;
    fLumi = 0.;
    fFile = 0;
    fHistogram = 0;
}

//____________________________________________________________________

HepDataMCSample::~HepDataMCSample() {
    //
    // Default destructor
    //
    Info("~HepDataMCSample", "Sample = %s", GetName());
    if ( fFile != 0 ) {
 	Info("~HepDataMCSample", "File = %s", fFile->GetName());
 	cout.flush();
	delete fFile; fFile = 0;
    }
    fHistogram = 0;
}

//____________________________________________________________________

TFile* HepDataMCSample::Open() {
    //
    // Open histogram file for reading.
    //
    // Read also the weighted number of events contained in this
    // sample. In a first attempt the JobInfo histogram is tried. If
    // this cannot be found the original CutFlow histogram from the
    // Top-D3PDs is searched for.
    //
    if ( fFile == 0 ) {
	fFile = new TFile(GetName(), "read");

	// Look for JobInfo histogram. If this doesn't exist try
	// cut-flow
	Float_t  nevt = 0.;
	TH1F *h = (TH1F*)fFile->Get("job_info/h_nevts");
	if ( h != 0 ) {
	    nevt = h->GetBinContent(3);
	} else {
	    h = (TH1F*)fFile->Get("fullCutFlow_genXpileupXzvertexWeights");
	    if ( h != 0 ) nevt = h->GetBinContent(1);
	}

	// Set lumi
	if ( fLumi <= 0. ) {
	    if ( h == 0 ) {
		Fatal("Open",
		      "Cannot find histogram for number of events in file %s needed to compute the luminosity. Abort!",
		      fFile->GetName());
		gSystem->Abort(0);
	    }
	    fNEvents = nevt;
	}
    }
    return fFile;
}

//____________________________________________________________________

void HepDataMCSample::Close() {
    //
    // Close histogram file
    //
    if ( fFile != 0 ) delete fFile;
    fFile = 0;
}

//____________________________________________________________________

Float_t HepDataMCSample::GetLumi() const {
    //
    // Get luminosity (pb^-1)
    //
    // In case the luminosity had been set via SetLumi() beforehand
    // this number will be returned. Otherwiese the luminosity
    // calculated from the given x-section and the number of processed
    // events is returned (this is the standard case).
    //
	cout<<"LUMI "<<fLumi<<" "<<fNEvents<<" "<<fXsection<<endl;
    return ( fLumi > 0. ) ? fLumi : fNEvents/fXsection;
}


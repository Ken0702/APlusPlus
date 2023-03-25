//____________________________________________________________________
//
// Class for storing information on HistFactory Measuremant parameters
//
// Stores various parameters for a HistFactory measurement:
//  - a list of channels
//  - lumi, lumi rel. error
//    (see HistFactory documentation for more details)
//  - parameter of interest (poi)
//    (currently only one poi is possible)
//
// All output is stored in <outdir>.
//
// - see AtlHistFactoryTask for more details
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactoryMeasurement
#include <AtlHistFactoryMeasurement.h>
#endif
#include <AtlHistFactoryChannel.h>
#include <AtlHistFactorySample.h>
#include <AtlHistFactorySystematic.h>
#include <TList.h>
#include <TObjString.h>
#include <TString.h>

#ifndef __CINT__
ClassImp(AtlHistFactoryMeasurement);
#endif

//____________________________________________________________________

AtlHistFactoryMeasurement::AtlHistFactoryMeasurement(const char* name,
						     const char* title,
						     const char* out_dir,
						     const char* scheme,
						     const char* out_prefix) :
    TNamed(name, title) {
    //
    // Default constructor
    //
    fOutputDir = new TString(out_dir);
    fOutPrefix = new TString(out_prefix);
    fScheme    = new TString(scheme);
    
    fListOfChannels = new TList;

    fPOIs = new TList;
    fLumi = 0.;
    fLumiRelError = 0.;

    fBinLow  = -1; // full range of discriminant is considered
    fBinHigh = -1; // by default

    fUseAsimovData = kTRUE;
    fUsePseudoExp  = kFALSE;
    fExportShapePlots = kFALSE;
    fExportCorrMatrix = kFALSE;
    fRunPulls = kFALSE;
    fNoSystematics = kFALSE;
}

//____________________________________________________________________

AtlHistFactoryMeasurement::~AtlHistFactoryMeasurement() {
    //
    // Default destructor
    //
    delete fOutputDir;
    delete fListOfChannels;
}

//____________________________________________________________________

void AtlHistFactoryMeasurement::AddChannel(AtlHistFactoryChannel *ch) {
    //
    // Add channel to list of channels
    //
    
    fListOfChannels->Add(ch);
}

//____________________________________________________________________

void AtlHistFactoryMeasurement::AddPOI(const char* poi) {
    //
    // Add parameter of interest (POI)
    //
    if ( fPOIs->FindObject(poi) == 0 )
	fPOIs->Add(new TObjString(poi));
    else
	Warning("AddPOI", "Parameter of interest already added. Do not add it again.");
}

//____________________________________________________________________

void AtlHistFactoryMeasurement::SetDrawOptions(const char* sample_name,
					       Color_t fill_color,
					       Style_t fill_style,
					       Color_t line_color,
					       Style_t line_style) {
    //
    // Set draw options for given sample in all channels
    //
    
    TIter next_channel(fListOfChannels);
    AtlHistFactoryChannel *ch = 0;
    while ( (ch = (AtlHistFactoryChannel*) next_channel()) ) {
	TList *samples = ch->GetListOfSamples();
	
	AtlHistFactorySample *sample = (AtlHistFactorySample*) samples->FindObject(sample_name);
	if ( sample != 0 ) {
	    // channel not necessarily contains this sample
	    sample->SetDrawOptions(fill_color, fill_style,
				   line_color, line_style);
	}
    }

}

//____________________________________________________________________

TString* AtlHistFactoryMeasurement::GetModelFileName(const char* channel) {
    //
    // Return the file name to the model root file.
    //
    // The user is responsible for deleting the returned pointer.
    //
    
    TString *filename = new TString(Form("%s/%s_%s_%s_model.root",
					 fOutputDir->Data(),
					 fOutPrefix->Data(),
					 channel, GetName()));
    return filename;
}

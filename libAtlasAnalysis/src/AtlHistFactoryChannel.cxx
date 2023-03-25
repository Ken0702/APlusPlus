//____________________________________________________________________
//
// Class for storing HistFactory Channel Information
//
// The class holds a list of samples (AtlHistFactorySample) and a list
// of systematics (AtlHistFactorySystematic) that should be used for
// each sample.
//
// The histogram of the discriminant is expected in the directory
// <basedir>/<systematic>/plots_datamc/MCPlotter.root
//
// - see AtlHistFactorySystematik::Initialize for more details.
//
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactoryChannel
#include <AtlHistFactoryChannel.h>
#endif
#include <TList.h>
#include <AtlHistFactorySystematic.h>
#include <AtlHistFactorySample.h>

#ifndef __CINT__
ClassImp(AtlHistFactoryChannel);
#endif

//____________________________________________________________________

AtlHistFactoryChannel::AtlHistFactoryChannel(const char* name,
					     const char* discriminant,
					     const char* basedir) :
    TNamed(name, name), fDiscriminant(discriminant),
    fMCPlotterBaseDir(basedir) {
    //
    // Default constructor
    //

    fListOfSamples = new TList;
    fListOfSystematics = new TList;

    fShapeDiscriminantRef  = 0;
    fShapeDiscriminantSyst = 0;
}

//____________________________________________________________________

AtlHistFactoryChannel::~AtlHistFactoryChannel() {
    //
    // Default destructor
    //

    delete fListOfSystematics;
    if ( fShapeDiscriminantRef != 0 ) delete fShapeDiscriminantRef;
    if ( fShapeDiscriminantSyst != 0 ) delete fShapeDiscriminantSyst;	
}

//____________________________________________________________________

void AtlHistFactoryChannel::AddSystematic(AtlHistFactorySystematic *syst) {
    //
    // Add Systematic to channel
    //
    
    fListOfSystematics->Add(syst);
}

//____________________________________________________________________

void AtlHistFactoryChannel::AddSystematics(TList *list) {
    //
    // Add Systematic to channel
    //
    
    fListOfSystematics->AddAll(list);
}

//____________________________________________________________________

void AtlHistFactoryChannel::AddSample(AtlHistFactorySample *sample) {
    //
    // Add Systematic to channel
    //
    
    fListOfSamples->Add(sample);
}

//____________________________________________________________________

void AtlHistFactoryChannel::SetShapeDiscriminants(const char* ShapeRef,
						  const char* ShapeSyst) {
    //
    // Set shape discriminants
    // The histogram 'ShapeRef' will be used as Reference histogram
    // ans tested against the histogram 'ShapeSyst'.
    //
    if ( fShapeDiscriminantRef != 0 ) {
	delete fShapeDiscriminantRef;
    }
    fShapeDiscriminantRef  = new TString(ShapeRef);
    
    if ( fShapeDiscriminantSyst != 0 ) {
	delete fShapeDiscriminantSyst;
    }
    fShapeDiscriminantSyst  = new TString(ShapeSyst);
}

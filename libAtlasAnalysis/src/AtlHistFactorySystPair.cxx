//____________________________________________________________________
//
// Class for a pair of systematics
//
// The 'standard' case for most of the systematics. There exist two
// variations, up and down which will be used as variation templates.
// No computation needed.
// 
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactorySystPair
#include <AtlHistFactorySystPair.h>
#endif
#include <TFile.h>
#include <HepDataMCPlot.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlHistFactorySystPair);
#endif

//____________________________________________________________________

AtlHistFactorySystPair::AtlHistFactorySystPair() {
    //
    // Default constructor for TObject Clone
    //
}

//____________________________________________________________________

AtlHistFactorySystPair::AtlHistFactorySystPair(const char* systname,
					       const char *systtitle,
					       AtlTopLevelAnalysis::ESystematic SystUp,
					       AtlTopLevelAnalysis::ESystematic SystDown,
					       Bool_t useShape) :
    AtlHistFactorySystematic(systname, systtitle, useShape) {
    //
    // Default constructor
    //

    fSystUp   = SystUp;
    fSystDown = SystDown;

    fPlotterFileUp   = 0;
    fPlotterFileDown = 0;

    fHistsUp = 0;
    fHistsDown = 0;
}

//____________________________________________________________________

AtlHistFactorySystPair::~AtlHistFactorySystPair() {
    //
    // Default destructor
    //
    
    if ( fPlotterFileUp   != 0 ) delete fPlotterFileUp;
    if ( fPlotterFileDown != 0 ) delete fPlotterFileDown;

    if ( fHistsUp != 0 ) {
	fHistsUp->Delete();
	delete fHistsUp;
    }
    if ( fHistsDown != 0 ) {
	fHistsDown->Delete();
	delete fHistsDown;
    }    
}

//____________________________________________________________________

void AtlHistFactorySystPair::Initialize(const char* BaseDir,
					const char* scheme) {
    //
    // Initialize for MCPlotter files
    //
    // Set the MCPlotter files for the up-var. and down variation.
    //
    // The file directory is
    // 'BaseDir' + 'systname' + "%s/MCPlotter.root"
    //
    // Get the list of templates from the HepDataMCPlots
    //

    if ( fPlotterFileUp != 0 ) delete fPlotterFileUp;
    fPlotterFileUp = TFile::Open(Form("%s/%s/%s/MCPlotter.root",
				      BaseDir,
				      AtlTopLevelAnalysis::fgSystematicNames[fSystUp],
				      scheme));
    
    if ( fPlotterFileDown != 0 ) delete fPlotterFileDown;
    fPlotterFileDown = TFile::Open(Form("%s/%s/%s/MCPlotter.root",
					BaseDir,
					AtlTopLevelAnalysis::fgSystematicNames[fSystDown],
					scheme));

    if ( fPlotterFileUp == 0 || fPlotterFileDown == 0 ) {
	Error("Initialize",
	      "Could not find MCPlotter files. Abort!");
	gSystem->Abort();
    }
    
    if ( fDiscriminant == 0 ) {
	Error("Initialize",
	      "Discriminant not set. Please use SetDiscriminant(..). Abort!");
	gSystem->Abort();
    }	
    
    HepDataMCPlot *HepUp = (HepDataMCPlot*) fPlotterFileUp->Get(fDiscriminant->Data());
    HepDataMCPlot *HepDown = (HepDataMCPlot*) fPlotterFileDown->Get(fDiscriminant->Data());

    if ( HepUp == 0 || HepDown == 0 ) {
	Error("Initialize",
	      "Could not find discriminant '%s' in file.",
	      fDiscriminant->Data());
	Error("Initialize", "Files are:\n%s\n%s",
	      fPlotterFileUp->GetName(),
	      fPlotterFileDown->GetName());
	gSystem->Abort();
    }

    fHistsUp = HepUp->GetListOfMCTemplates(AtlTopLevelAnalysis::fgSystematicNames[fSystUp]);
    fHistsDown = HepDown->GetListOfMCTemplates(AtlTopLevelAnalysis::fgSystematicNames[fSystDown]);
}

//____________________________________________________________________

void AtlHistFactorySystPair::GetHistsFromFile(const char* process) {
    //
    // Change the process and load the corresponding informations
    // into the class data members.
    //
    
    // we get the histos directly from the template file
    fHistUp = (TH1*) fTemplateFile->Get(Form("%s_%s",
					     process,
					     AtlTopLevelAnalysis::fgSystematicNames[fSystUp]));
    fHistDown = (TH1*) fTemplateFile->Get(Form("%s_%s",
					       process,
					       AtlTopLevelAnalysis::fgSystematicNames[fSystDown]));
}

//____________________________________________________________________

void AtlHistFactorySystPair::ComputeUpDownVariation(const char* process) {
    //
    // Compute the up var. and down variation for this systematic pair
    //

    // No need to compute anything, just take the histograms
    // from the plotter files
    fHistUp = (TH1*) fHistsUp->FindObject(Form("%s_%s",
					       process,
					       AtlTopLevelAnalysis::fgSystematicNames[fSystUp]));
    fHistDown = (TH1*) fHistsDown->FindObject(Form("%s_%s",
						   process,
						   AtlTopLevelAnalysis::fgSystematicNames[fSystDown]));
}

//____________________________________________________________________

void AtlHistFactorySystPair::Clear(Option_t *option) {
    //
    // Close MCPlottr files
    // - Do not close the template file. The class does not own this
    //   file. The owner of this file is responsible of closing it.
    //

    if ( fPlotterFileUp != 0 ) {
	fPlotterFileUp->Close();
	delete fPlotterFileUp;
	fPlotterFileUp = 0;
    }

    if ( fPlotterFileDown != 0 ) {
	fPlotterFileDown->Close();
	delete fPlotterFileDown;
	fPlotterFileDown = 0;
    }

    if ( fHistsUp != 0 ) {
	// fHistsUp->Delete();
	delete fHistsUp;
	fHistsUp = 0;
    }

    if ( fHistsDown != 0 ) {
	// fHistsDown->Delete();
	delete fHistsDown;
	fHistsDown = 0;
    }

    // Question: Why can we not delete fHistsDown? Are the pointers in the list gone
    //           when we close the MCPlotter file...?
    
    // Delete the pointer?
    // - Problem with SaveTemplate()?
    fHistUp = 0;
    fHistDown = 0;

}

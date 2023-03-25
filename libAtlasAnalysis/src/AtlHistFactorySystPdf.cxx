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
#ifndef ATLAS_AtlHistFactorySystPdf
#include <AtlHistFactorySystPdf.h>
#endif
#include <TFile.h>
#include <HepDataMCPlot.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlHistFactorySystPdf);
#endif

//____________________________________________________________________

AtlHistFactorySystPdf::AtlHistFactorySystPdf() {
    //
    // Default constructor
    //
}

//____________________________________________________________________

AtlHistFactorySystPdf::AtlHistFactorySystPdf(const char* systname,
					     const char *systtitle,
					     AtlTopLevelAnalysis::ESystematic SystUp,
					     AtlTopLevelAnalysis::ESystematic SystDown,
					     Bool_t useShape) :
    AtlHistFactorySystematic(systname, systtitle, useShape) {
    //
    // Default constructor
    //

    fSystUp      = SystUp;
    fSystDown    = SystDown;
    fNominal     = AtlTopLevelAnalysis::kNOMINAL;

    fPlotterFileUp   = 0;
    fPlotterFileDown = 0;
    fPlotterFileNom  = 0;

    fHistsUp   = 0;
    fHistsDown = 0;
    fHistsNom  = 0;
}

//____________________________________________________________________

AtlHistFactorySystPdf::~AtlHistFactorySystPdf() {
    //
    // Default destructor
    //
    
    if ( fPlotterFileUp   != 0 ) delete fPlotterFileUp;
    if ( fPlotterFileDown != 0 ) delete fPlotterFileDown;
    if ( fPlotterFileNom  != 0 ) delete fPlotterFileNom;

    if ( fHistsUp != 0 ) {
	fHistsUp->Delete();
	delete fHistsUp;
    }
    if ( fHistsDown != 0 ) {
	fHistsDown->Delete();
	delete fHistsDown;
    }    
    if ( fHistsNom != 0 ) {
	fHistsNom->Delete();
	delete fHistsNom;
    }    
}

//____________________________________________________________________

void AtlHistFactorySystPdf::Initialize(const char* BaseDir,
				       const char* scheme) {
    //
    // Initialize for MCPlotter files
    //
    // Set the MCPlotter files for the up-var. and down variation.
    //
    // The file directory is
    // 'BaseDir' + 'systname' + 'scheme' + "MCPlotter.root"
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

        if ( fPlotterFileNom != 0 ) delete fPlotterFileNom;
    fPlotterFileNom = TFile::Open(Form("%s/%s/%s/MCPlotter.root",
				       BaseDir,
				       AtlTopLevelAnalysis::fgSystematicNames[fNominal],
				       scheme));
    
    if ( fPlotterFileUp == 0 || fPlotterFileDown == 0 || fPlotterFileNom == 0 ) {
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
    HepDataMCPlot *HepNom = (HepDataMCPlot*) fPlotterFileNom->Get(fDiscriminant->Data());

    if ( HepUp == 0 || HepDown == 0 || HepNom == 0 ) {
	Error("Initialize",
	      "Could not find discriminant '%s' in file.",
	      fDiscriminant->Data());
	Error("Initialize", "Files are:\n%s\n%s",
	      fPlotterFileUp->GetName(),
	      fPlotterFileDown->GetName());
	gSystem->Abort();
    }

    fHistsUp   = HepUp->GetListOfMCTemplates(AtlTopLevelAnalysis::fgSystematicNames[fSystUp]);
    fHistsDown = HepDown->GetListOfMCTemplates(AtlTopLevelAnalysis::fgSystematicNames[fSystDown]);
    fHistsNom  = HepNom->GetListOfMCTemplates(AtlTopLevelAnalysis::fgSystematicNames[fNominal]);
}

//____________________________________________________________________

void AtlHistFactorySystPdf::GetHistsFromFile(const char* process) {
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

void AtlHistFactorySystPdf::ComputeUpDownVariation(const char* process) {
    //
    // Compute the up var. and down variation for this systematic pair
    //

    // The Pdf uncertainty does not necessarily use the nominal samples
    // Therefore, compute the relative uncertainty per bin and then
    // use the nominal template to get the correct shape

    TH1 *h_pdf_up = (TH1*) fHistsUp->FindObject(Form("%s_%s",
						     process,
						     AtlTopLevelAnalysis::fgSystematicNames[fSystUp]));
    TH1 *h_pdf_down = (TH1*) fHistsDown->FindObject(Form("%s_%s",
							 process,
							 AtlTopLevelAnalysis::fgSystematicNames[fSystDown]));
    TH1 *h_nom = (TH1*) fHistsNom->FindObject(Form("%s_%s",
						   process,
						   AtlTopLevelAnalysis::fgSystematicNames[fNominal]));
    
    // Compute the center of the envelope
    // (up + down)/2.
    TH1 *h_pdf_center = (TH1*) h_pdf_up->Clone("pdf_center");
    h_pdf_center->Add(h_pdf_down);
    h_pdf_center->Scale(0.5);
    
    // Compute the relative uncertainty per bin
    TH1 *h_pdf_rel = (TH1*) h_pdf_up->Clone("pdf_rel");
    h_pdf_rel->Add(h_pdf_center, -1.);
    h_pdf_rel->Divide(h_pdf_center);

    // Compute the pdf templates
    fHistUp   = (TH1*) h_nom->Clone(Form("%s_%s",
    					 process,
    					 AtlTopLevelAnalysis::fgSystematicNames[fSystUp]));
    fHistDown = (TH1*) h_nom->Clone(Form("%s_%s",
    					 process,
    					 AtlTopLevelAnalysis::fgSystematicNames[fSystDown]));

    // pdf_up/down = (1 +- pdf_rel)*nominal
    fHistUp->Multiply(h_pdf_rel);
    fHistUp->Add(h_nom);
    
    fHistDown->Multiply(h_pdf_rel);
    fHistDown->Scale(-1.);
    fHistDown->Add(h_nom);
}

//____________________________________________________________________

void AtlHistFactorySystPdf::Clear(Option_t *option) {
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

    if ( fPlotterFileNom != 0 ) {
	fPlotterFileNom->Close();
	delete fPlotterFileNom;
	fPlotterFileNom = 0;
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
    
    if ( fHistsNom != 0 ) {
	// fHistsNom->Delete();
	delete fHistsNom;
	fHistsNom = 0;
    }

    // Delete the pointer?
    // - Problem with SaveTemplate()?
    fHistUp = 0;
    fHistDown = 0;
    fHistNom = 0;
}

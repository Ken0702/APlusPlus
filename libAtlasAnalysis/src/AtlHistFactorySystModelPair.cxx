//____________________________________________________________________
//
// Class for modelling systematics
//
// The variation of this systematics is computed as
//
//    Up/Down = Nominal +/- (up - down)/2
//
// where up and down are two histograms corresponding to a different
// model. The systematic is symmetrized around the nominal histogram.
//
// Atlernatively, a different nominal sample can be used then the
// systematic is computed as
//
//    Up/Down = Nominal + (up/down - syst_nom)
//
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactorySystModelPair
#include <AtlHistFactorySystModelPair.h>
#endif
#include <TFile.h>
#include <HepDataMCPlot.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlHistFactorySystModelPair);
#endif

//____________________________________________________________________

AtlHistFactorySystModelPair::AtlHistFactorySystModelPair() {
    //
    // Default constructor
    //
}

//____________________________________________________________________

AtlHistFactorySystModelPair::AtlHistFactorySystModelPair(
    const char* systname,
    const char *systtitle,
    AtlTopLevelAnalysis::ESystematic SystUp,
    AtlTopLevelAnalysis::ESystematic SystDown,
    Bool_t useShape) :
    AtlHistFactorySystPair(systname, systtitle,
			   SystUp, SystDown, useShape) {
    //
    // Default constructor
    //

    fPlotterFileNom = 0;
    fPlotterFileSystNom = 0;
    fHistsNom = 0;
    fHistsSystNom = 0;

    fNominal  = AtlTopLevelAnalysis::kNOMINAL;
    fSystNom  = AtlTopLevelAnalysis::kNOMINAL;
}

//____________________________________________________________________

AtlHistFactorySystModelPair::~AtlHistFactorySystModelPair() {
    //
    // Default destructor
    //
    if ( fPlotterFileNom  != 0 ) delete fPlotterFileNom;
    if ( fPlotterFileSystNom != 0 ) delete fPlotterFileSystNom;
    if ( fHistsNom != 0 ) {
	// fHistsNom->Delete();
	delete fHistsNom;
    }    
    if ( fHistsSystNom != 0 ) {
	// fHistsSystNom->Delete();
	delete fHistsSystNom;
    }
}

//____________________________________________________________________

void AtlHistFactorySystModelPair::Initialize(const char* BaseDir,
					     const char* scheme) {
    //
    // Initialize for MCPlotter files
    //
    // Set the MCPlotter files for the systematic and nominal.
    //
    // The file directory is
    // 'BaseDir' + 'systname' + "plots_datamc/MCPlotter.root"
    //
    // Get the list of templates from the HepDataMCPlots
    //
    
    if ( fPlotterFileNom != 0 ) delete fPlotterFileNom;
    fPlotterFileNom = TFile::Open(Form("%s/%s/%s/MCPlotter.root",
				       BaseDir,
				       AtlTopLevelAnalysis::fgSystematicNames[fNominal],
				       scheme));

    if ( fPlotterFileNom == 0 ) {
	Error("Initialize",
	      "Could not find nominal MCPlotter file. Abort!");
	gSystem->Abort();
    }

    if ( fNominal != fSystNom ) {
	if ( fPlotterFileSystNom != 0 ) delete fPlotterFileSystNom;
	fPlotterFileSystNom = TFile::Open(Form("%s/%s/%s/MCPlotter.root",
					       BaseDir,
					       AtlTopLevelAnalysis::fgSystematicNames[fSystNom],
					       scheme));
	if ( fPlotterFileSystNom == 0 ) {
	    Error("Initialize",
		  "Could not find MCPlotter files. Abort!");
	    gSystem->Abort();
	}
    }

    if ( fDiscriminant == 0 ) {
	Error("Initialize",
	      "Discriminant not set. Please use SetDiscriminant(..). Abort!");
	gSystem->Abort();
    }
    
    HepDataMCPlot *HepNom = (HepDataMCPlot*) fPlotterFileNom->Get(fDiscriminant->Data());
    
    if ( HepNom == 0 ) {
	Error("Initialize",
	      "Could not find discriminant '%s' in file.",
	      fDiscriminant->Data());
	Error("Initialize", "File is:\n%s",
	      fPlotterFileNom->GetName());
	gSystem->Abort();
    }

    HepDataMCPlot *HepSystNom = 0;
    if ( fSystNom != fNominal ) {
	HepSystNom = (HepDataMCPlot*) fPlotterFileSystNom->Get(fDiscriminant->Data());
	if ( HepSystNom == 0 ) {
	    Error("Initialize",
		  "Could not find discriminant '%s' in file.",
		  fDiscriminant->Data());
	    Error("Initialize", "File is:\n%s",
		  fPlotterFileSystNom->GetName());
	    gSystem->Abort();
	}
	fHistsSystNom = HepSystNom->GetListOfMCTemplates(AtlTopLevelAnalysis::fgSystematicNames[fSystNom]);
    }

    fHistsNom  = HepNom->GetListOfMCTemplates(AtlTopLevelAnalysis::fgSystematicNames[fNominal]);

    // Get Systematic Files using SystPair functionality
    AtlHistFactorySystPair::Initialize(BaseDir, scheme);
}

//____________________________________________________________________

void AtlHistFactorySystModelPair::ComputeUpDownVariation(const char* process) {
    //
    // Compute the up var. and down variation for this modelling
    // systematic pair
    //

    // Modelling uncertainty is
    //
    // Up/Down = Nominal +/- (up - down)/2
    //
    // --> Up   = Nominal + up/2 - down/2
    // --> Down = Nominal - up/2 + down/2
    //

    // Get up and down histograms
    AtlHistFactorySystPair::ComputeUpDownVariation(process);

    TH1 *h_nom  = (TH1*) fHistsNom->FindObject(Form("%s_%s",
						    process,
						    AtlTopLevelAnalysis::fgSystematicNames[fNominal]));

    TH1 *h_systnom = 0;
    if ( fSystNom != fNominal ) {
	h_systnom = (TH1*) fHistsSystNom->FindObject(Form("%s_%s",
							  process,
							  AtlTopLevelAnalysis::fgSystematicNames[fSystNom]));
    }
    
    // reassign pointers from up/down histograms
    TH1 *h_up   = fHistUp;
    TH1 *h_down = fHistDown;


    if ( fSystNom != fNominal ) {
	//    Up/Down = Nominal + (up/down - syst_nom)
	fHistUp   = (TH1*) h_nom->Clone(Form("%s_%s_up",
					     process,
					     GetName() ));
	fHistUp->Add(h_up, +1.);
	fHistUp->Add(h_systnom, -1.);
	
	fHistDown = (TH1*) h_nom->Clone(Form("%s_%s_down",
					     process,
					     GetName() ));
	fHistDown->Add(h_down, +1.);
	fHistDown->Add(h_systnom, -1.);
	
    } else {
	//    Up/Down = Nominal +/- (up - down)/2
	fHistUp   = (TH1*) h_nom->Clone(Form("%s_%s_up",
					     process,
					     GetName() ));
	fHistUp->Add(h_up,   +0.5);
	fHistUp->Add(h_down, -0.5);
	
	fHistDown = (TH1*) h_nom->Clone(Form("%s_%s_down",
					     process,
					     GetName() ));
	fHistDown->Add(h_up,   -0.5);
	fHistDown->Add(h_down, +0.5);
    }
}

//____________________________________________________________________

void AtlHistFactorySystModelPair::Clear(Option_t *option) {
    //
    // Close MCPlotter files
    // - Do not close the template file. The class does not own this
    //   file. The owner of this file is responsible of closing it.
    //

    if ( fPlotterFileNom != 0 ) {
	fPlotterFileNom->Close();
	delete fPlotterFileNom;
	fPlotterFileNom = 0;
    }

    if ( fHistsNom != 0 ) {
	// fHistsNom->Delete();
	delete fHistsNom;
	fHistsNom = 0;
    }

    if ( fSystNom != fNominal ) {
        if ( fPlotterFileSystNom != 0 ) {
	    fPlotterFileSystNom->Close();
	    delete fPlotterFileSystNom;
	    fPlotterFileSystNom = 0;
	}
	if ( fHistsSystNom != 0 ) {
	    // fHistsSystNom->Delete();
	    delete fHistsSystNom;
	    fHistsSystNom = 0;
	}
    }

    AtlHistFactorySystPair::Clear();
}

//____________________________________________________________________

void AtlHistFactorySystModelPair::GetHistsFromFile(const char* process) {
    //
    // Change the process and load the corresponding informations
    // into the class data members.
    //
    
    // we get the histos directly from the template file
    fHistUp = (TH1*) fTemplateFile->Get(Form("%s_%s_up",
					     process,
					     GetName() ));
    fHistDown = (TH1*) fTemplateFile->Get(Form("%s_%s_down",
					       process,
					       GetName() ));
    
    if ( fHistUp == 0 ) {
	Error("GetHistsFromFile",
	      "Could not find up histogram for systematic '%s'",
	      GetName());
    }
    if ( fHistDown == 0 ) {
	Error("GetHistsFromFile",
	      "Could not find down histogram for systematic '%s'",
	      GetName());
    }
}

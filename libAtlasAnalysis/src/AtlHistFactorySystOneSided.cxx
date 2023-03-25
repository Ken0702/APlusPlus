//____________________________________________________________________
//
// Class for one sided systematic
//
// One sided systematics are symmetrized around the nominal case. Two
// modes are available:
//
// - FullDiff: 	up/down = nom +- (syst-nom)
// - HalfDiff: 	up/down = nom +- (syst-nom)/2.
//
// 'syst' corresponds to the systematic histogram.
//
// Alternative method:
// - compare with a different nominal histogram
// 
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactorySystOneSided
#include <AtlHistFactorySystOneSided.h>
#endif
#include <TFile.h>
#include <HepDataMCPlot.h>

#ifndef __CINT__
ClassImp(AtlHistFactorySystOneSided);
#endif

//____________________________________________________________________

AtlHistFactorySystOneSided::AtlHistFactorySystOneSided() {
    //
    // Default constructor
    //
}

//____________________________________________________________________

AtlHistFactorySystOneSided::AtlHistFactorySystOneSided(const char* systname,
						       const char *systtitle,
						       AtlTopLevelAnalysis::ESystematic Syst,
						       ESymmetryMode Mode,
						       Bool_t useShape) :
    AtlHistFactorySystematic(systname, systtitle, useShape) {
    //
    // Default constructor
    //
    fSyst    = Syst;
    fNominal = AtlTopLevelAnalysis::kNOMINAL;
    fSystNom = AtlTopLevelAnalysis::kNOMINAL;
    fSymMode = Mode;

    fPlotterFileSyst = 0;
    fPlotterFileSystNom = 0;
    fPlotterFileNom  = 0;
    
    fHistsSyst = 0;
    fHistsNom  = 0;
    fHistsSystNom = 0;
}

//____________________________________________________________________

AtlHistFactorySystOneSided::~AtlHistFactorySystOneSided() {
    //
    // Default destructor
    //
    if ( fPlotterFileSyst != 0 ) delete fPlotterFileSyst;
    if ( fPlotterFileSystNom != 0 ) delete fPlotterFileSystNom;
    if ( fPlotterFileNom  != 0 ) delete fPlotterFileNom;
    
    if ( fHistsSyst != 0 ) {
	// fHistsSyst->Delete();
	delete fHistsSyst;
    }
    if ( fHistsSystNom != 0 ) {
	// fHistsSystNom->Delete();
	delete fHistsSystNom;
    }
    if ( fHistsNom != 0 ) {
	// fHistsNom->Delete();
	delete fHistsNom;
    }    
}

//____________________________________________________________________

void AtlHistFactorySystOneSided::Initialize(const char* BaseDir,
					    const char* scheme) {
    //
    // Initialize for MCPlotter files
    //
    // Set the MCPlotter files for the systematic and nominal.
    //
    // The file directory is
    // 'BaseDir' + 'systname' + "%s/MCPlotter.root"
    //
    // Get the list of templates from the HepDataMCPlots
    //
    
    if ( fPlotterFileSyst != 0 ) delete fPlotterFileSyst;
    fPlotterFileSyst = TFile::Open(Form("%s/%s/%s/MCPlotter.root",
					BaseDir,
					AtlTopLevelAnalysis::fgSystematicNames[fSyst],
					scheme));
    
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
    
    if ( fPlotterFileNom != 0 ) delete fPlotterFileNom;
    fPlotterFileNom = TFile::Open(Form("%s/%s/%s/MCPlotter.root",
				       BaseDir,
				       AtlTopLevelAnalysis::fgSystematicNames[fNominal],
				       scheme));

    if ( fPlotterFileSyst == 0 || fPlotterFileNom == 0 ) {
	Error("Initialize",
	      "Could not find MCPlotter files. Abort!");
	gSystem->Abort();
    }

    if ( fDiscriminant == 0 ) {
	Error("Initialize",
	      "Discriminant not set. Please use SetDiscriminant(..). Abort!");
	gSystem->Abort();
    }	
    
    HepDataMCPlot *HepSyst = (HepDataMCPlot*) fPlotterFileSyst->Get(fDiscriminant->Data());
    HepDataMCPlot *HepNom = (HepDataMCPlot*) fPlotterFileNom->Get(fDiscriminant->Data());

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
    
    if ( HepSyst == 0 || HepNom == 0 ) {
	Error("Initialize",
	      "Could not find discriminant '%s' in file.",
	      fDiscriminant->Data());
	Error("Initialize", "Files are:\n%s\n%s",
	      fPlotterFileSyst->GetName(),
	      fPlotterFileNom->GetName());
	gSystem->Abort();
    }

    fHistsSyst = HepSyst->GetListOfMCTemplates(AtlTopLevelAnalysis::fgSystematicNames[fSyst]);
    fHistsNom  = HepNom->GetListOfMCTemplates(AtlTopLevelAnalysis::fgSystematicNames[fNominal]);
}

//____________________________________________________________________

void AtlHistFactorySystOneSided::GetHistsFromFile(const char* process) {
    //
    // Change the process and load the corresponding informations
    // into the class data members.
    //
    
    // we get the histos directly from the template file
    fHistUp = (TH1*) fTemplateFile->Get(Form("%s_%s_up",
					     process,
					     AtlTopLevelAnalysis::fgSystematicNames[fSyst]));
    fHistDown = (TH1*) fTemplateFile->Get(Form("%s_%s_down",
					       process,
					       AtlTopLevelAnalysis::fgSystematicNames[fSyst]));

    if ( fHistUp == 0 ) {
	Error("GetHistsFromFile",
	      "Could not find up histogram for systematic '%s'",
	      AtlTopLevelAnalysis::fgSystematicNames[fSyst]);
    }
    if ( fHistDown == 0 ) {
	Error("GetHistsFromFile",
	      "Could not find down histogram for systematic '%s'",
	      AtlTopLevelAnalysis::fgSystematicNames[fSyst]);
    }
}

//____________________________________________________________________

void AtlHistFactorySystOneSided::ComputeUpDownVariation(const char* process) {
    //
    // Compute the up var. and down variation for this systematic pair
    //

    TH1 *h_syst = (TH1*) fHistsSyst->FindObject(Form("%s_%s",
						     process,
						     AtlTopLevelAnalysis::fgSystematicNames[fSyst]));
    TH1 *h_nom  = (TH1*) fHistsNom->FindObject(Form("%s_%s",
						    process,
						    AtlTopLevelAnalysis::fgSystematicNames[fNominal]));
    // Symmetry mode: full difference
    if ( fSymMode == kFullDiff ) {
	if ( fSystNom != fNominal ) {
	    TH1 *h_systnom = (TH1*) fHistsSystNom->FindObject(Form("%s_%s",
								   process,
								   AtlTopLevelAnalysis::fgSystematicNames[fSystNom]));
	    // Use different nominal sample

	    // Syst variation is:
	    // up/down = nom +- (syst-nom_syst)
	    // --> up   = nom + syst - nom_syst
	    // --> down = nom + nom_syst - syst

	    fHistUp   = (TH1*) h_nom->Clone(Form("%s_%s_up",
						 process,
						 AtlTopLevelAnalysis::fgSystematicNames[fSyst]));
	    fHistDown = (TH1*) h_nom->Clone(Form("%s_%s_down",
						 process,
						 AtlTopLevelAnalysis::fgSystematicNames[fSyst]));

	    fHistUp->Add(h_syst, +1.);
	    fHistUp->Add(h_systnom, -1.);

	    fHistDown->Add(h_syst, -1.);
	    fHistDown->Add(h_systnom, +1.);

	} else {
	    // up/down = nom +- (syst-nom)
	    // --> up   = syst
	    // --> down = 2.*nom - syst
	    
	    // only down needs to be computed
	    fHistUp   = (TH1*) h_syst->Clone(Form("%s_%s_up",
						  process,
						  AtlTopLevelAnalysis::fgSystematicNames[fSyst]));
	    fHistDown = (TH1*) h_nom->Clone(Form("%s_%s_down",
						 process,
						 AtlTopLevelAnalysis::fgSystematicNames[fSyst]));
	    fHistDown->Scale(2.);
	    fHistDown->Add(h_syst, -1.);
	}
    } else if ( fSymMode == kHalfDiff ) {
	if ( fSystNom != fNominal ) {
	    // Use different nominal sample

	    // Syst variation is:
	    // up/down = nom +- (syst-nom_syst)
	    // --> up   = nom + syst - nom_syst
	    // --> down = nom + nom_syst - syst
	    Error("ComputeUpDownVariation", "Not supported. Abort!");
	    gSystem->Abort();
	} else {
	    // up/down = nom +- (syst-nom)/2.
	    // --> up   = 0.5*(nom + syst)
	    // --> down = 1.5*nom - 0.5*syst
	    
	    fHistUp   = (TH1*) h_syst->Clone(Form("%s_%s_up",
						  process,
						  AtlTopLevelAnalysis::fgSystematicNames[fSyst]));
	    fHistUp->Add(h_nom);
	    fHistUp->Scale(0.5);
	    
	    fHistDown = (TH1*) h_nom->Clone(Form("%s_%s_down",
						 process,
						 AtlTopLevelAnalysis::fgSystematicNames[fSyst]));
	    fHistDown->Scale(1.5);
	    fHistDown->Add(h_syst, -0.5);
	}
    }
}

//____________________________________________________________________

void AtlHistFactorySystOneSided::Clear(Option_t *option) {
    //
    // Close MCPlotter files
    // - Do not close the template file. The class does not own this
    //   file. The owner of this file is responsible of closing it.
    //

    if ( fPlotterFileSyst != 0 ) {
	fPlotterFileSyst->Close();
	delete fPlotterFileSyst;
	fPlotterFileSyst = 0;
    }

    if ( fPlotterFileNom != 0 ) {
	fPlotterFileNom->Close();
	delete fPlotterFileNom;
	fPlotterFileNom = 0;
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
    
    if ( fHistsSyst != 0 ) {
	// fHistsSyst->Delete();
	delete fHistsSyst;
	fHistsSyst = 0;
    }

    if ( fHistsNom != 0 ) {
	// fHistsNom->Delete();
	delete fHistsNom;
	fHistsNom = 0;
    }

    fHistUp = 0;
    delete fHistUp;
    fHistDown = 0;
    delete fHistDown;
}

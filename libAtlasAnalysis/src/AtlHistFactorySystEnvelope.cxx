//____________________________________________________________________
//
// Class for special systematics using an envelope computation
//
// The up and down variation of this systematic is found by 
// the maximum and minimum value in each bin using all 6 variations.
//
//
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactorySystEnvelope
#include <AtlHistFactorySystEnvelope.h>
#endif
#include <TFile.h>
#include <HepDataMCPlot.h>
#include <iostream>

#ifndef __CINT__
ClassImp(AtlHistFactorySystEnvelope);
#endif

//____________________________________________________________________

AtlHistFactorySystEnvelope::AtlHistFactorySystEnvelope() {
    //
    // Default constructor
    //
}

//____________________________________________________________________

AtlHistFactorySystEnvelope::AtlHistFactorySystEnvelope(const char* systname,
						       const char *systtitle,
						       AtlTopLevelAnalysis::ESystematic Syst1,
						       AtlTopLevelAnalysis::ESystematic Syst2,
						       AtlTopLevelAnalysis::ESystematic Syst3,
						       AtlTopLevelAnalysis::ESystematic Syst4,
						       AtlTopLevelAnalysis::ESystematic Syst5,
						       AtlTopLevelAnalysis::ESystematic Syst6,
						       Bool_t useShape) :
    AtlHistFactorySystematic(systname, systtitle, useShape) {
    //
    // Default constructor
    //
    fSyst1   = Syst1;
    fSyst2   = Syst2;
    fSyst3   = Syst3;
    fSyst4   = Syst4;
    fSyst5   = Syst5;
    fSyst6   = Syst6;
    // fNominal = AtlTopLevelAnalysis::kNOMINAL;

    fPlotterFileSyst1 = 0;
    fPlotterFileSyst2 = 0;
    fPlotterFileSyst3 = 0;
    fPlotterFileSyst4 = 0;
    fPlotterFileSyst5 = 0;
    fPlotterFileSyst6 = 0;
    // fPlotterFileNom   = 0;
    
    fHistsSyst1 = 0;
    fHistsSyst2 = 0;
    fHistsSyst3 = 0;
    fHistsSyst4 = 0;
    fHistsSyst5 = 0;
    fHistsSyst6 = 0;
    // fHistsNom  = 0;
}


//____________________________________________________________________

AtlHistFactorySystEnvelope::~AtlHistFactorySystEnvelope() {
    //
    // Default destructor
    //
    if ( fPlotterFileSyst1 != 0 ) delete fPlotterFileSyst1;
    if ( fPlotterFileSyst2 != 0 ) delete fPlotterFileSyst2;
    if ( fPlotterFileSyst3 != 0 ) delete fPlotterFileSyst3;
    if ( fPlotterFileSyst4 != 0 ) delete fPlotterFileSyst4;
    if ( fPlotterFileSyst5 != 0 ) delete fPlotterFileSyst5;
    if ( fPlotterFileSyst6 != 0 ) delete fPlotterFileSyst6;
    // if ( fPlotterFileNom   != 0 ) delete fPlotterFileNom;
    
    if ( fHistsSyst1 != 0 ) delete fHistsSyst1;
    if ( fHistsSyst2 != 0 ) delete fHistsSyst2;
    if ( fHistsSyst3 != 0 ) delete fHistsSyst3;
    if ( fHistsSyst4 != 0 ) delete fHistsSyst4;
    if ( fHistsSyst5 != 0 ) delete fHistsSyst5;
    if ( fHistsSyst6 != 0 ) delete fHistsSyst6;
    // if ( fHistsNom != 0 ) delete fHistsNom;    
}

//____________________________________________________________________

void AtlHistFactorySystEnvelope::Initialize(const char* BaseDir,
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
    
    if ( fPlotterFileSyst1 != 0 ) delete fPlotterFileSyst1;
    fPlotterFileSyst1 = TFile::Open(Form("%s/%s/%s/MCPlotter.root",
					 BaseDir,
					 AtlTopLevelAnalysis::fgSystematicNames[fSyst1],
					 scheme));
    if ( fPlotterFileSyst2 != 0 ) delete fPlotterFileSyst2;
    fPlotterFileSyst2 = TFile::Open(Form("%s/%s/%s/MCPlotter.root",
					 BaseDir,
					 AtlTopLevelAnalysis::fgSystematicNames[fSyst2],
					 scheme));

    // These are optional systematic templates

    // Optional
    if ( fSyst3 != AtlTopLevelAnalysis::kUndefined ) {
	if ( fPlotterFileSyst3 != 0 ) delete fPlotterFileSyst3;
	fPlotterFileSyst3 = TFile::Open(Form("%s/%s/%s/MCPlotter.root",
					     BaseDir,
					     AtlTopLevelAnalysis::fgSystematicNames[fSyst3],
					     scheme));
    }
    
    // Optional
    if ( fSyst4 != AtlTopLevelAnalysis::kUndefined ) {
	if ( fPlotterFileSyst4 != 0 ) delete fPlotterFileSyst4;
	fPlotterFileSyst4 = TFile::Open(Form("%s/%s/%s/MCPlotter.root",
					     BaseDir,
					     AtlTopLevelAnalysis::fgSystematicNames[fSyst4],
					     scheme));
    }

    // Optional:
    if ( fSyst5 != AtlTopLevelAnalysis::kUndefined ) {
	if ( fPlotterFileSyst5 != 0 ) delete fPlotterFileSyst5;
	fPlotterFileSyst5 = TFile::Open(Form("%s/%s/%s/MCPlotter.root",
					     BaseDir,
					     AtlTopLevelAnalysis::fgSystematicNames[fSyst5],
					    scheme));
    }

    // Optional:
    if ( fSyst6 != AtlTopLevelAnalysis::kUndefined ) {
	if ( fPlotterFileSyst6 != 0 ) delete fPlotterFileSyst6;
	fPlotterFileSyst6 = TFile::Open(Form("%s/%s/%s/MCPlotter.root",
					     BaseDir,
					     AtlTopLevelAnalysis::fgSystematicNames[fSyst6],
					     scheme));
    }
    
    if ( fPlotterFileSyst1 == 0 ||
	 fPlotterFileSyst2 == 0 ||
	 // optional:
	 (fPlotterFileSyst3 == 0 && fSyst3 != AtlTopLevelAnalysis::kUndefined) ||
	 (fPlotterFileSyst4 == 0 && fSyst4 != AtlTopLevelAnalysis::kUndefined) ||
	 (fPlotterFileSyst5 == 0 && fSyst5 != AtlTopLevelAnalysis::kUndefined) ||
	 (fPlotterFileSyst6 == 0 && fSyst6 != AtlTopLevelAnalysis::kUndefined) ) {
	 // fPlotterFileNom == 0 ) {
	Error("Initialize",
	      "Could not find MCPlotter files. Abort!");
	gSystem->Abort();
    }
    
    if ( fDiscriminant == 0 ) {
	Error("Initialize",
	      "Discriminant not set. Please use SetDiscriminant(..). Abort!");
	gSystem->Abort();
    }	
    
    HepDataMCPlot *HepSyst1 = (HepDataMCPlot*) fPlotterFileSyst1->Get(fDiscriminant->Data());
    HepDataMCPlot *HepSyst2 = (HepDataMCPlot*) fPlotterFileSyst2->Get(fDiscriminant->Data());
    // Optional:
    HepDataMCPlot *HepSyst3 = 0;
    HepDataMCPlot *HepSyst4 = 0;
    HepDataMCPlot *HepSyst5 = 0;
    HepDataMCPlot *HepSyst6 = 0;
    
    if ( fPlotterFileSyst3 != 0 )
	HepSyst3 = (HepDataMCPlot*) fPlotterFileSyst3->Get(fDiscriminant->Data());
    if ( fPlotterFileSyst4 != 0 )
	HepSyst4 = (HepDataMCPlot*) fPlotterFileSyst4->Get(fDiscriminant->Data());
    if ( fPlotterFileSyst5 != 0 )
	HepSyst5 = (HepDataMCPlot*) fPlotterFileSyst5->Get(fDiscriminant->Data());
    if ( fPlotterFileSyst6 != 0 )
	HepSyst6 = (HepDataMCPlot*) fPlotterFileSyst6->Get(fDiscriminant->Data());
    
    // HepDataMCPlot *HepNom = (HepDataMCPlot*) fPlotterFileNom->Get(fDiscriminant->Data());

    if ( HepSyst1 == 0 ||
	 HepSyst2 == 0 ||
	 (HepSyst3 == 0 && fPlotterFileSyst3 != 0) ||
	 (HepSyst4 == 0 && fPlotterFileSyst4 != 0) ||
	 (HepSyst5 == 0 && fPlotterFileSyst5 != 0) ||
	 (HepSyst6 == 0 && fPlotterFileSyst6 != 0) ) {
	// HepNom == 0 ) {
	Error("Initialize",
	      "Could not find discriminant '%s' in file.",
	      fDiscriminant->Data());
	Error("Initialize", "Files are:\n%s",
	      fPlotterFileSyst1->GetName());
	gSystem->Abort();
    }

    fHistsSyst1 = HepSyst1->GetListOfMCTemplates(AtlTopLevelAnalysis::fgSystematicNames[fSyst1]);
    fHistsSyst2 = HepSyst2->GetListOfMCTemplates(AtlTopLevelAnalysis::fgSystematicNames[fSyst2]);
    if ( HepSyst3 != 0 )
	fHistsSyst3 = HepSyst3->GetListOfMCTemplates(AtlTopLevelAnalysis::fgSystematicNames[fSyst3]);
    if ( HepSyst4 != 0 )
        fHistsSyst4 = HepSyst4->GetListOfMCTemplates(AtlTopLevelAnalysis::fgSystematicNames[fSyst4]);
    if ( HepSyst5 != 0 )
	fHistsSyst5 = HepSyst5->GetListOfMCTemplates(AtlTopLevelAnalysis::fgSystematicNames[fSyst5]);
    if ( HepSyst6 != 0 )
	fHistsSyst6 = HepSyst6->GetListOfMCTemplates(AtlTopLevelAnalysis::fgSystematicNames[fSyst6]);
    // fHistsNom  = HepNom->GetListOfMCTemplates(AtlTopLevelAnalysis::fgSystematicNames[fNominal]);
}

//____________________________________________________________________

void AtlHistFactorySystEnvelope::GetHistsFromFile(const char* process) {
    //
    // Change the process and load the corresponding informations
    // into the class data members.
    //
    
    // we get the histos directly from the template file
    fHistUp = (TH1*) fTemplateFile->Get(Form("%s_%s_up",
					     process,
					     AtlTopLevelAnalysis::fgSystematicNames[fSyst1]));
    fHistDown = (TH1*) fTemplateFile->Get(Form("%s_%s_down",
					       process,
					       AtlTopLevelAnalysis::fgSystematicNames[fSyst1]));

    if ( fHistUp == 0 ) {
	Error("GetHistsFromFile",
	      "Could not find up histogram for systematic '%s'",
	      AtlTopLevelAnalysis::fgSystematicNames[fSyst1]);
    }
    if ( fHistDown == 0 ) {
	Error("GetHistsFromFile",
	      "Could not find down histogram for systematic '%s'",
	      AtlTopLevelAnalysis::fgSystematicNames[fSyst1]);
    }
}

//____________________________________________________________________

void AtlHistFactorySystEnvelope::ComputeUpDownVariation(const char* process) {
    //
    // Compute the up var. and down variation for this systematic
    //

    // Collect all histograms for all 6 variations
    TList *hists = new TList;

    // Skip the first histogram here, it is used below as starting point for
    // the min/max search!
    //
    hists->Add(fHistsSyst2->FindObject(Form("%s_%s",
					    process,
					    AtlTopLevelAnalysis::fgSystematicNames[fSyst2])));

    if ( fHistsSyst3 != 0 )
	hists->Add(fHistsSyst3->FindObject(Form("%s_%s",
						process,
						AtlTopLevelAnalysis::fgSystematicNames[fSyst3])));
    if ( fHistsSyst4 != 0 )
	hists->Add(fHistsSyst4->FindObject(Form("%s_%s",
						process,
						AtlTopLevelAnalysis::fgSystematicNames[fSyst4])));
    if ( fHistsSyst5 != 0 )
	hists->Add(fHistsSyst5->FindObject(Form("%s_%s",
						process,
						AtlTopLevelAnalysis::fgSystematicNames[fSyst5])));
    if ( fHistsSyst6 != 0 )
	hists->Add(fHistsSyst6->FindObject(Form("%s_%s",
						process,
						AtlTopLevelAnalysis::fgSystematicNames[fSyst6])));
    
    // TH1 *h_nom  = (TH1*) fHistsNom->FindObject(Form("%s_%s",
    // 						    process,
    // 						    AtlTopLevelAnalysis::fgSystematicNames[fNominal]));

    TH1 *h_syst = (TH1*) fHistsSyst1->FindObject(Form("%s_%s",
						      process,
						      AtlTopLevelAnalysis::fgSystematicNames[fSyst1]));
    
    // Clone the first systematic histogram for up and down envelope
    // using the name of the first systematic for envelope
    fHistUp   = (TH1*) h_syst->Clone(Form("%s_%s_up",
					  process,
					  AtlTopLevelAnalysis::fgSystematicNames[fSyst1]));
    fHistDown = (TH1*) h_syst->Clone(Form("%s_%s_down",
					  process,
					  AtlTopLevelAnalysis::fgSystematicNames[fSyst1]));
    
    // Search the maximum and minimum in each bin
    TIter next_hist(hists);
    TH1 *hist = 0;
    while ( (hist = (TH1*)next_hist()) ) {
	for ( Int_t bin = 0; bin <= fHistUp->GetNbinsX(); bin++ ) {
	    Double_t BinMax      = fHistUp->GetBinContent(bin);
	    Double_t BinMin      = fHistDown->GetBinContent(bin);

	    Double_t BinContent = hist->GetBinContent(bin);
	    Double_t BinError   = hist->GetBinError(bin);

	    if ( BinContent > BinMax ) {
		fHistUp->SetBinContent(bin, BinContent);
		fHistUp->SetBinError(bin, BinError);
	    }
	    
	    if ( BinContent < BinMin ) {
		fHistDown->SetBinContent(bin, BinContent);
		fHistDown->SetBinError(bin, BinError);
	    }
	}
    }
}

//____________________________________________________________________

void AtlHistFactorySystEnvelope::Clear(Option_t *option) {
    //
    // Close MCPlotter files
    // - Do not close the template file. The class does not own this
    //   file. The owner of this file is responsible of closing it.
    //

    if ( fPlotterFileSyst1 != 0 ) {
	fPlotterFileSyst1->Close();
	delete fPlotterFileSyst1;
	fPlotterFileSyst1 = 0;
    }
    if ( fPlotterFileSyst2 != 0 ) {
	fPlotterFileSyst2->Close();
	delete fPlotterFileSyst2;
	fPlotterFileSyst2 = 0;
    }
    if ( fPlotterFileSyst3 != 0 ) {
	fPlotterFileSyst3->Close();
	delete fPlotterFileSyst3;
	fPlotterFileSyst3 = 0;
    }
    if ( fPlotterFileSyst4 != 0 ) {
	fPlotterFileSyst4->Close();
	delete fPlotterFileSyst4;
	fPlotterFileSyst4 = 0;
    }
    if ( fPlotterFileSyst5 != 0 ) {
	fPlotterFileSyst5->Close();
	delete fPlotterFileSyst5;
	fPlotterFileSyst5 = 0;
    }
    if ( fPlotterFileSyst6 != 0 ) {
	fPlotterFileSyst6->Close();
	delete fPlotterFileSyst6;
	fPlotterFileSyst6 = 0;
    }

    // if ( fPlotterFileNom != 0 ) {
    // 	fPlotterFileNom->Close();
    // 	delete fPlotterFileNom;
    // 	fPlotterFileNom = 0;
    // }

    if ( fHistsSyst1 != 0 ) {
	delete fHistsSyst1;
	fHistsSyst1 = 0;
    }
    if ( fHistsSyst2 != 0 ) {
	delete fHistsSyst2;
	fHistsSyst2 = 0;
    }
    if ( fHistsSyst3 != 0 ) {
	delete fHistsSyst3;
	fHistsSyst3 = 0;
    }
    if ( fHistsSyst4 != 0 ) {
	delete fHistsSyst4;
	fHistsSyst4 = 0;
    }
    if ( fHistsSyst5 != 0 ) {
	delete fHistsSyst5;
	fHistsSyst5 = 0;
    }
    if ( fHistsSyst6 != 0 ) {
	delete fHistsSyst6;
	fHistsSyst6 = 0;
    }

    // if ( fHistsNom != 0 ) {
    // 	delete fHistsNom;
    // 	fHistsNom = 0;
    // }

    fHistUp = 0;
    delete fHistUp;
    fHistDown = 0;
    delete fHistDown;
}

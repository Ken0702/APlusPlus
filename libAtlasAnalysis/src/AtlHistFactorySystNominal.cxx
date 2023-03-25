//____________________________________________________________________
//
// Class for information corresponding to the nominal case
//
// This class holds the information for the nominal case. It does not
// support any other systematic specific feature, but is needed to
// store and read the nominal histograms.
// 
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactorySystNominal
#include <AtlHistFactorySystNominal.h>
#endif
#include <TFile.h>
#include <HepDataMCPlot.h>

#ifndef __CINT__
ClassImp(AtlHistFactorySystNominal);
#endif

//____________________________________________________________________

AtlHistFactorySystNominal::AtlHistFactorySystNominal(const char* systname,
						     const char *systtitle) :
    AtlHistFactorySystematic(systname, systtitle, kFALSE) {
    //
    // Default constructor
    //

    fPlotterFileNom = 0;
    fNominal  = AtlTopLevelAnalysis::kNOMINAL; 
    fHistsNom = 0;

    fIsNominal = kTRUE;
}

//____________________________________________________________________

AtlHistFactorySystNominal::~AtlHistFactorySystNominal() {
    //
    // Default constructor
    //
    
    if ( fPlotterFileNom  != 0 ) delete fPlotterFileNom;
    
    if ( fHistsNom != 0 ) {
	fHistsNom->Delete();
	delete fHistsNom;
    }    
}

//____________________________________________________________________

void AtlHistFactorySystNominal::Initialize(const char* BaseDir,
					   const char* scheme) {
    //
    // Initialize for MCPlotter files
    //
    // Set the MCPlotter files for the nominal case.
    //
    // The file directory is
    // 'BaseDir' + 'systname' + "plots_datamc/MCPlotter.root"
    //
    // Get the list of templates from the HepDataMCPlots
    //
    
    fPlotterFileNom = TFile::Open(Form("%s/%s/%s/MCPlotter.root",
				       BaseDir,
				       AtlTopLevelAnalysis::fgSystematicNames[fNominal],
				       scheme));

    if ( fPlotterFileNom == 0 ) {
	Error("Initialize",
	      "Could not find MCPlotter file. Abort!");
	gSystem->Abort();
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
	Error("Initialize", "Files is %s.",
	      fPlotterFileNom->GetName());
	gSystem->Abort();
    }

    fHistsNom  = HepNom->GetListOfMCTemplates(AtlTopLevelAnalysis::fgSystematicNames[fNominal]);
}

//____________________________________________________________________

void AtlHistFactorySystNominal::GetHistsFromFile(const char* process) {
    //
    // Change the process and load the corresponding informations
    // into the class data members.
    //

    // Nominal is already loaded by default. Nothing to be done here.
}

//____________________________________________________________________

void AtlHistFactorySystNominal::ComputeUpDownVariation(const char* process) {
    //
    // Get the nominal histogram for a given process
    //

    fHistNom = (TH1*) fHistsNom->FindObject(Form("%s_%s",
						 process,
						 AtlTopLevelAnalysis::fgSystematicNames[fNominal]));
}

//____________________________________________________________________

void AtlHistFactorySystNominal::Clear(Option_t *option) {
    //
    // Close MCPlottr files
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
}

//____________________________________________________________________

void AtlHistFactorySystNominal::SaveTemplates(TFile *fout) {
    //
    // Save the templates of the nominal case in the output file.
    //
    
    // Write() will save it to the current directory
    // therefore change the current directory
    fout->cd();
    
    fHistNom->SetDirectory(fout);
    fHistNom->Write();
}

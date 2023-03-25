//____________________________________________________________________
//
// Data/MC Plotter Task for A++ analyses
// 
//
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2013 (C) Soeren Stamm
//
#ifndef ATLAS_AtlDataMCPlotterTask
#include <AtlDataMCPlotterTask.h>
#endif
#include <TROOT.h>
#include <TBrowser.h>
#include <TMath.h>
#include <TString.h>
#include <TObjString.h>
#include <TObjArray.h>
#include <TSystem.h>
#include <fstream>
#include <iostream>
#include <TClass.h>
#include <TTree.h>
#include <TFile.h>
#include <TChain.h>
#include <HepNtuplePlotCmd.h>

using namespace std;

#ifndef __CINT__
ClassImp(AtlDataMCPlotterTask);
#endif

//____________________________________________________________________

AtlDataMCPlotterTask::AtlDataMCPlotterTask(const char* name,
					   const char* title) :
    AtlTask(name, title) {
    //
    // Default constructor
    //
    fRootScript = 0;
    fListOfUserEnvs = new TList;
    fListOfFolders  = new TList;
    fNtuplePlotCmds = new TList;
    fDataLuminosity = 0.;
    fLuminosityLabel         = 0;
    fCenterOfMassEnergyLabel = 0;
    fAtlasLabelStatus        = 0;
    fDrawDataZeroEntryErrors = kFALSE;
    fDrawMCError             = kTRUE;
    fExportPDF               = kFALSE;
    fUseOverflowUnderflow    = kFALSE;
    fDrawSignalOverlay       = kFALSE;
    fScaleOverlay            = 1.;
    
    SetBatchNodeAll(kTRUE);
    
    AddUserEnv("LIBHEPUTILS");
    AddUserEnv("LIBATLASANALYSIS");
}

//____________________________________________________________________

AtlDataMCPlotterTask::~AtlDataMCPlotterTask() {
    //
    // Default destructor
    //
    fListOfUserEnvs->Delete(); delete fListOfUserEnvs;
    fListOfFolders->Delete();  delete fListOfFolders;
    delete fNtuplePlotCmds;
    if ( fLuminosityLabel != 0 )         delete fLuminosityLabel;
    if ( fCenterOfMassEnergyLabel != 0)  delete fCenterOfMassEnergyLabel;
    if ( fAtlasLabelStatus != 0 )        delete fAtlasLabelStatus;
}

//____________________________________________________________________

void AtlDataMCPlotterTask::ExecInteractiveJob(Option_t *option) {
    //
    // Interactive execution
    //
    TString opt = option;
    
    // Start Plotting
    CreateRootScript(opt.Data());

    gROOT->Macro(Form("%s/plottertask.C", fJobHome->Data()));
}

//____________________________________________________________________

void AtlDataMCPlotterTask::CreateRunScript(Option_t*) {
    //
    // Create Run Script
    //
    
    ofstream out;
    out.open(fRunScript->Data());
    out << "#!/bin/sh" << endl
	<< "# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
	<< "# !!! This is an automatically generated file !!!" << endl
	<< "# !!! D O   N O T   E D I T                   !!!" << endl
	<< "# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
	<< "#" << endl
	<< "# Run script for A++ Data/MC plotter job submission" << endl
	<< "#" << endl << endl
	<< "# Set environment" << endl;
    // Platform
    if ( gSystem->Getenv("ARCH_TYPE") != 0 ) {
	out << "export ARCH_TYPE=" << gSystem->Getenv("ARCH_TYPE") << endl;
    } else {
	Error("CreateRunScript",
	      "Environment variable ARCH_TYPE not set. Abort!");
	gSystem->Abort(0);
    }
    
    if (!WriteEnvSetup(out)) {

    // ROOT
    out << "export ROOTSYS=" << gSystem->Getenv("ROOTSYS") << endl
	<< "export PATH=${ROOTSYS}/bin:${PATH}" << endl
	<< "export LD_LIBRARY_PATH=${ROOTSYS}/lib:${LD_LIBRARY_PATH}" << endl;
    
    // A++ and user-defined libraries
    TIter next_lib(fListOfUserEnvs);
    TNamed *env = 0;
    while ( (env = (TNamed*)next_lib()) ) {
	if ( gSystem->Getenv(env->GetName()) != 0 ) {
	    out << "export " << env->GetName() << "="
		<< gSystem->Getenv(env->GetName()) << endl
		<< "export LD_LIBRARY_PATH=${"
		<< env->GetName() << "}/lib/${ARCH_TYPE}:${LD_LIBRARY_PATH}"
		<< endl;
	} else {
	    Error("CreateRunScript", "Environment variable %s not set. Abort!",
		  env->GetName());
	    gSystem->Abort(0);
	}
    }

    }
    out	<< endl << endl
	<< "# Job execution" << endl;

    out << "JOBHOME=" << fJobHome->Data() << endl
	<< "cd $JOBHOME" << endl	
	<< "root -q -l -b plottertask.C > " << fLogFilePath->Data()
	<< " 2>&1" << endl;
    out.close();    
}

//____________________________________________________________________

Bool_t AtlDataMCPlotterTask::ExecBatchJob(Option_t *option) {
    //
    // Exec Batch Job
    //

    TString opt = option;
    
    // Create submit scripts
    CreateRootScript(opt.Data());

    // Submit batch job
    return SubmitBatchJob();
}

//____________________________________________________________________

void AtlDataMCPlotterTask::AddUserEnv(const char* env) {
    //
    // Add user-defined environment variable for shared library search
    // paths
    //
    fListOfUserEnvs->Add(new TNamed(env, ""));
}


//____________________________________________________________________

void AtlDataMCPlotterTask::CreateRootScript(Option_t *option) {
    //
    // Create Root script for A++ analysis job execution
    //
    TString opt = option;
    
    // =============
    // Create script
    // =============
    if ( fRootScript != 0 ) delete fRootScript;
    fRootScript = new TString(fJobHome->Data());
    fRootScript->Append("/plottertask.C");
    
    ofstream out;
    out.open(fRootScript->Data());
    
    // Header
    out << "{" << endl
	<< "// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
	<< "// !!! This is an automatically generated file !!!" << endl
	<< "// !!! D O   N O T   E D I T                   !!!" << endl
	<< "// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
	<< "//" << endl;
    if ( fGridJob ) {
	out << "// Root script for A++ Data/MC Plotter grid job execution"
	    << endl;
    } else {
	out << "// Root script for A++ Data/MC Plotter batch job execution"
	    << endl;
    }
    out << "//" << endl;
    
    out << "HepDataMCPlotter *pl = new HepDataMCPlotter(\"MCPlotter\", \"Create all Data-MC plots\");" << endl
	<< "pl->SetWorkingDir(\"" << gSystem->DirName(fOutputFileName->Data())
	<< "\");"  << endl;
    if ( fDrawDataZeroEntryErrors ) {
	out << "pl->SetDrawDataZeroEntryErrors(kTRUE);" << endl;
    } else {
	out << "pl->SetDrawDataZeroEntryErrors(kFALSE);" << endl;
    }
    if ( fDrawMCError ) {
	out << "pl->SetDrawMCError(kTRUE);" << endl;
    } else {
	out << "pl->SetDrawMCError(kFALSE);" << endl;
    }
    out << "pl->SetAtlasLabelStatus(\"" << fAtlasLabelStatus->Data()
	<< "\");" << endl
	<< "pl->SetCenterOfMassEnergyLabel(\""
	<< fCenterOfMassEnergyLabel->Data() << "\");" << endl
	<< "pl->SetLuminosityLabel(\"" << fLuminosityLabel->Data()
	<< "\");" << endl;
    if ( fExportPDF ) {
	out << "pl->SetExportPDF(kTRUE);" << endl;
    } else {
	out << "pl->SetExportPDF(kFALSE);" << endl;
    }
    if ( fUseOverflowUnderflow ) {
	out << "pl->SetUseOverflowUnderflow(kTRUE);" << endl;
    } else {
	out << "pl->SetUseOverflowUnderflow(kFALSE);" << endl;
    }
    if ( fDrawSignalOverlay ) {
	out << "pl->SetDrawSignalOverlay(kTRUE);" << endl;
    } else {
	out << "pl->SetDrawSignalOverlay(kFALSE);" << endl;
    }
    out << "pl->SetScaleOverlay(" << fScaleOverlay << ");" << endl;
    if ( fDrawSignificance ) {
	out << "pl->SetDrawSignificance(kTRUE);" << endl;
    } else {
	out << "pl->SetDrawSignificance(kFALSE);" << endl;
    }
    
    // Add ntuple plot commands (if any)
    if ( fNtuplePlotCmds->GetEntries() > 0 ) {
        out << "// Add ntuple plot commands" << endl;
        TIter next_cmd(fNtuplePlotCmds);
        HepNtuplePlotCmd *cmd = 0;
        while ( (cmd = (HepNtuplePlotCmd*)next_cmd()) ) {
	    out << "pl->AddNtuplePlotCmd(\""
	        << cmd->GetTreeName() << "\", \""
		<< cmd->GetVarExp() << "\", \""
		<< cmd->GetSelection() << "\", \""
		<< cmd->GetPlotName() << "\", \""
		<< cmd->GetPlotTitle() << "\", "
		<< cmd->GetNbins() << ", "
		<< cmd->GetXlow() << ", "
		<< cmd->GetXup() << ", \""
		<< cmd->GetXTitle() << "\", \""
		<< cmd->GetYTitle() << "\");"
		<< endl;
	}
	out << endl;
    }

    // scale with data lumi independent of whether there are data samples
    out << "pl->SetLumiDATA(" << fDataLuminosity  << ");" << endl;

    // Add samples
    AtlPlotterFolder *folder = 0;
    AtlSample *sample = 0;
    TIter next_folder(fListOfFolders);
    while ( (folder = (AtlPlotterFolder*)next_folder()) ) {
        if ( folder->GetListOfSamples()->IsEmpty() )
            continue;

	// Data
	if ( ((AtlSample*)folder->GetListOfSamples()->At(0))->IsDATA() ) {
	    TIter next_sample( folder->GetListOfSamples() );
	    
	    out << "// Add " << folder->GetName() << " sample" << endl;
	    while ( (sample = (AtlSample*)next_sample()) ) {
		out << "pl->AddDataFile(\"" << sample->GetPathName() << "\", "
		    << "\"" << folder->GetName() << "\");" << endl;
	    }
	} else if ( ((AtlSample*)folder->GetListOfSamples()->At(0))->IsQCD() ) {
	    // QCD
	    out << "// Add group " << folder->GetName() << endl
		<< "pl->AddMCFolder(\"" << folder->GetName() << "\", "
		<< "\"" << folder->GetTitle() << "\", "
		<< folder->GetColor() << ");" << endl;
	    
	    TIter next_sample( folder->GetListOfSamples() );
	    while ( (sample = (AtlSample*)next_sample()) ) {
		out << "(pl->AddMCSample(\"" << folder->GetName() << "\", "
		    << "\"" << sample->GetPathName() << "\", "
		    << "\"" << sample->GetName() << "\", "
		    << 1. << ", " // QCD has xsection = 1
		    << sample->GetColor() << "))"
		    << "->SetLumi(" << fDataLuminosity << ");" << endl;
	    }
	} else {
	    // MC
	    out << "// Add group " << folder->GetName() << endl
			<< "pl->AddMCFolder(\"" << folder->GetName() << "\", "
			<< "\""  << folder->GetTitle() << "\", "
			<< folder->GetColor() << ");" << endl;
	    
	    TIter next_sample( folder->GetListOfSamples() );
	    while ( (sample = (AtlSample*)next_sample()) ) {
		out << "pl->AddMCSample(\"" << folder->GetName() << "\", "
		    << "\"" << sample->GetPathName() << "\", "
		    << "\"" << sample->GetName() << "\", "
		    << sample->GetXsection() << ", "
		    << sample->GetColor() << ");" << endl;
	    }
	}	
	out << endl << endl;
    }

    // Turn on debugging
    if ( fDebug > 0 ) {
	out << "" << endl;
	out << "gDebug = " << fDebug << ";" << endl;
    }

    out << "" << endl;
    out << "pl->Exec();" << endl;
    out << "}" << endl;

    out.close();
}

//____________________________________________________________________

void AtlDataMCPlotterTask::ExecGridJob(const Option_t*) {
    //
    // Exec grid job
    //
    Error("ExecGridJob", "Not supported! Abort!");
    gSystem->Abort(0);
}

//____________________________________________________________________

void AtlDataMCPlotterTask::ExecNAFBatchJob(const Option_t*) {
    //
    // Exec NAF batch job
    //
    Error("ExecNAFBatchJob", "Not supported! Abort!");
    gSystem->Abort(0);
}

//____________________________________________________________________

void AtlDataMCPlotterTask::CreateNAFBatchRunScript() {
    //
    // Create NAF Batch Run Script
    //
    Error("ExecNAFBatchRunScript", "Not supported! Abort!");
    gSystem->Abort(0);
}

//____________________________________________________________________

void AtlDataMCPlotterTask::CreateGridRunScript() {
    //
    // Create Grid Run Script
    //
    Error("ExecGridRunScript", "Not supported! Abort!");
    gSystem->Abort(0);
}

//____________________________________________________________________

void AtlDataMCPlotterTask::SetAtlasLabelStatus(const char* label) {
    //
    // Sets the Atlas label status of the plotter
    //
    if ( fAtlasLabelStatus != 0 ) delete fAtlasLabelStatus;
    fAtlasLabelStatus = new TString(label);
}

//____________________________________________________________________

void AtlDataMCPlotterTask::SetCenterOfMassEnergyLabel(const char* label) {
    //
    // Sets the  of the plotter
    //
    if ( fCenterOfMassEnergyLabel != 0 ) delete fCenterOfMassEnergyLabel;
    fCenterOfMassEnergyLabel = new TString(label);
}

//____________________________________________________________________

void AtlDataMCPlotterTask::SetLuminosityLabel(const char* label) {
    //
    // Sets the luminosity label of the plotter
    //
    if ( fLuminosityLabel != 0 ) delete fLuminosityLabel;
    fLuminosityLabel = new TString(label);
}

//____________________________________________________________________


void AtlDataMCPlotterTask::SetDrawDataZeroEntryErrors(Bool_t status) {
    //
    // Sets whether the plotter shall draw zero entry errors for data
    //
    fDrawDataZeroEntryErrors = status;
}

//____________________________________________________________________

void AtlDataMCPlotterTask::SetDrawMCError(Bool_t status) {
    //
    // Sets whether the plotter shall draw the MC error
    //
    fDrawMCError = status;
}

//____________________________________________________________________

void AtlDataMCPlotterTask::SetExportPDF(Bool_t status) {
    //
    // Sets whether the plotter automatically exports a Pdf file
    //
    fExportPDF = status;
}

//____________________________________________________________________

void AtlDataMCPlotterTask::SetUseOverflowUnderflow(Bool_t status) {
    //
    // Set flag for drawing the under- and overflow bins
    //
    fUseOverflowUnderflow = status;
}

//____________________________________________________________________

void AtlDataMCPlotterTask::SetDrawSignalOverlay(Bool_t status) {
    //
    // Set flag for drawing the signal MC (assumed to be the least
    // entry in the MC histogram stack) as overlay instead as part of
    // the stack
    //
    fDrawSignalOverlay = status;
}

//____________________________________________________________________

void AtlDataMCPlotterTask::SetScaleOverlay(Float_t scale) {
    //
    // Set scale factor of the overlay histogram (see
    // SetDrawSignalOverlay())
    //
    fScaleOverlay = scale;
}


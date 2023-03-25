//____________________________________________________________________
//
// Hfor Splitting Task for A++ analyses
// 
//
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2013 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHforSplittingTask
#include <AtlHforSplittingTask.h>
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
#include <TH1D.h>

using namespace std;

#ifndef __CINT__
ClassImp(AtlHforSplittingTask);
#endif

//____________________________________________________________________

AtlHforSplittingTask::AtlHforSplittingTask(const char* name,
					   const char* title) :
    AtlTask(name, title) {
    //
    // Default constructor
    //
    fInputTreeName  = new TString("physics");
    fListOfUserEnvs = new TList;

    // AddUserEnv("LIBHEPEVENT");
    // AddUserEnv("LIBATLASRUN");
    // AddUserEnv("LIBATLASEVENT");
    AddUserEnv("LIBATLASANALYSIS");
}

//____________________________________________________________________

AtlHforSplittingTask::~AtlHforSplittingTask() {
    //
    // Default destructor
    //
    delete fInputTreeName;
    fListOfUserEnvs->Delete(); delete fListOfUserEnvs;
}

//____________________________________________________________________

void AtlHforSplittingTask::SetInputTreeName(const char* TreeName) {
    //
    // Set input tree name
    //
    delete fInputTreeName;
    fInputTreeName = new TString(TreeName);  
}

//____________________________________________________________________

void AtlHforSplittingTask::ExecInteractiveJob(Option_t *option) {
    //
    // Interactive execution
    //

    TString opt = option;
    
    // Start HFOR splitting
    CreateRootScript(opt.Data());
//    gROOT->Macro(Form("%s/hfor_run.C", fJobHome->Data()));
}

//____________________________________________________________________

void AtlHforSplittingTask::CreateRunScript(Option_t*) {
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
	<< "# Run script for A++ HFOR job submission" << endl
	<< "#" << endl << endl
	<< "# Set environment" << endl;
    // Platform
    if ( gSystem->Getenv("ARCH_TYPE") != 0 ) {
	out << "export ARCH_TYPE=" << gSystem->Getenv("ARCH_TYPE") << endl;
    } else {
	Error("CreateRunScript", "Environment variable ARCH_TYPE not set. Abort!");
	gSystem->Abort(0);
    }
    
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
    out	<< endl << endl
	<< "# Job execution" << endl;

    if (  fTempOutputFileName != 0 ) {
	out << "if [ ! -d " << gSystem->DirName( fTempOutputFileName->Data() ) << " ]; then " << endl
	    << "   mkdir -p " << gSystem->DirName( fTempOutputFileName->Data() ) << endl
	    << "fi" << endl;
    }
    
    if ( fTempOutputPath != 0 && fTempLogFilePath != 0 ) {
	out << "mkdir -p " << fTempOutputPath->Data() << endl
	    << "cp " << fJobHome->Data() << "/hfor_run.C " << fTempOutputPath->Data() << endl
	    << "cd " << fTempOutputPath->Data() << endl
	    << "root -q -l -b hfor_run.C > " << fTempLogFilePath->Data()
	    << " 2>&1" << endl
	    << "mv " << fTempLogFilePath->Data() << " " << fLogFilePath->Data() << endl;
    } else {
	out << "JOBHOME=" << fJobHome->Data() << endl
	    << "cd $JOBHOME" << endl	
	    << "root -q -l -b hfor_run.C > " << fLogFilePath->Data()
	    << " 2>&1" << endl;
    }
    
    if (  fTempOutputFileName != 0 )
	out << "mv " << fTempOutputFileName->Data() << " " << fOutputFileName->Data() << endl;
    
    out.close();    
}

//____________________________________________________________________

Bool_t AtlHforSplittingTask::ExecBatchJob(Option_t *option) {
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

void AtlHforSplittingTask::AddUserEnv(const char* env) {
    //
    // Add user-defined environment variable for shared library search
    // paths
    //
    fListOfUserEnvs->Add(new TNamed(env, ""));
}

//____________________________________________________________________

void AtlHforSplittingTask::HforSplit(const char* InputFile,
				     const char* TreeName ) {
    //
    // HFOR splitting for given input file.
    //
    // Splitted files will be written to same directory as
    // the input file.
    //
    // The scale factor for bookkeeping is set to one, since the
    // x-section for each individuell hfor-type is not known

    cout << Form( "HforSplit: Processing file %s ...", InputFile) << endl;
   
    // Open input file for reading
    TFile *f_in = new TFile(InputFile, "read");
    TTree *t_in = (TTree*)f_in->Get( TreeName );

    // Loop over all valid HFOR types (0-3)
    for ( Int_t hfor = 0; hfor <= 3; hfor++ ) {
	
	// Open output file
	TString *fname_out = new TString( InputFile );
	fname_out->ReplaceAll(".root", Form("_hfor%d.root", hfor));

	cout << Form("HforSplit: Creating hfor file: %s ...", fname_out->Data() ) << endl;
	
	TFile *f_out = new TFile(fname_out->Data(), "recreate");
	
	// Copy tree
	TTree *t_out = t_in->CopyTree(Form("top_hfor_type==%d", hfor));
	t_out->SetDirectory(f_out);

	Double_t sf = 1.; // see above
	
	// Copy histograms
	CopyFolder(f_in, f_out, sf); 
	
	f_out->Write();
	delete f_out;
	delete fname_out;
    }
}

//____________________________________________________________________

void AtlHforSplittingTask::CreateRootScript(Option_t *option) {
    //
    // Create Root script for A++ analysis job execution
    //
    TString opt = option;

    // =============
    // Create script
    // =============
    if ( fRootScript != 0 ) delete fRootScript;
    fRootScript = new TString(fJobHome->Data());
    fRootScript->Append("/hfor_run.C");
    
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
	out << "// Root script for A++ HFOR grid job execution" << endl;
    } else {
	out << "// Root script for A++ HFOR batch job execution" << endl;
    }
    out << "//" << endl;
    
    // Add input files to script
    // (This is a hack in order to use wildcards...)
    TChain *ch = new TChain( fInputTreeName->Data() );
    
    TIter next(fInputFiles);
    TObjString *item = 0;
    while ( (item = (TObjString*)next()) )
	ch->Add( item->GetString().Data() );
    
    TIter next_file( ch->GetListOfFiles() );
    TObject *obj = 0;
    while ( ( obj = next_file() ) ) {
	out << "AtlHforSplittingTask::HforSplit(\"" 
	    << obj->GetTitle() 
	    << "\", \""
	    << fInputTreeName->Data()
	    << "\");" << endl;
    }
    out << "}" << endl;

    out.close();
}

//____________________________________________________________________

void AtlHforSplittingTask::ExecGridJob(const Option_t*) {
    //
    // Exec grid job
    //
    Error("ExecGridJob", "Not supported! Abort!");
    gSystem->Abort(0);
}

//____________________________________________________________________

void AtlHforSplittingTask::ExecNAFBatchJob(const Option_t*) {
    //
    // Exec NAF batch job
    //
    Error("ExecNAFBatchJob", "Not supported! Abort!");
    gSystem->Abort(0);
}

//____________________________________________________________________

void AtlHforSplittingTask::CreateNAFBatchRunScript() {
    //
    // Create NAF Batch Run Script
    //
    Error("ExecNAFBatchRunScript", "Not supported! Abort!");
    gSystem->Abort(0);
}

//____________________________________________________________________

void AtlHforSplittingTask::CreateGridRunScript() {
    //
    // Create Grid Run Script
    //
    Error("ExecGridRunScript", "Not supported! Abort!");
    gSystem->Abort(0);
}

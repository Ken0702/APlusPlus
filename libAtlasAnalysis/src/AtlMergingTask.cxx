//____________________________________________________________________
//
// A++ Merging Task for merging analysis output files
//
// Supported: hadd all given input files (e.g. used for ttbar)
// 
//
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2013 (C) Soeren Stamm
//
#ifndef ATLAS_AtlMergingTask
#include <AtlMergingTask.h>
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

using namespace std;

#ifndef __CINT__
ClassImp(AtlMergingTask);
#endif

//____________________________________________________________________

AtlMergingTask::AtlMergingTask(const char* name,
					   const char* title) :
    AtlTask(name, title) {
    //
    // Default constructor
    //
    SetBatchNodeAll(kTRUE);
}

//____________________________________________________________________

AtlMergingTask::~AtlMergingTask() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlMergingTask::ExecInteractiveJob(Option_t *option) {
    //
    // Interactive execution
    //

    TString opt = option;

    Error("ExecInteractiveJob", "Not supported! Use batch instead!\nAbort!");
    gSystem->Abort(0);

    // CreateRunScriptPath();
    // CreateRunScript();

    // // Start Merging
    // gSystem->Exec(Form("chmod u+x %s", fRunScript->Data()));
    // gSystem->Exec(Form("%s", fRunScript->Data()));
}

//____________________________________________________________________

void AtlMergingTask::CreateRunScript(Option_t*) {
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
	<< "# Run script for A++ merging job submission" << endl
	<< "#" << endl << endl
	<< "# Set environment" << endl;
    // Platform
    if ( gSystem->Getenv("ARCH_TYPE") != 0 ) {
	out << "export ARCH_TYPE=" << gSystem->Getenv("ARCH_TYPE") << endl;
    } else {
	Error("CreateRunScript", "Environment variable ARCH_TYPE not set. Abort!");
	gSystem->Abort(0);
    }
    
    if ( !WriteEnvSetup(out) ) {

    // ROOT
    out << "export ROOTSYS=" << gSystem->Getenv("ROOTSYS") << endl
	<< "export PATH=${ROOTSYS}/bin:${PATH}" << endl
	<< "export LD_LIBRARY_PATH=${ROOTSYS}/lib:${LD_LIBRARY_PATH}" << endl;

    }
    out	<< endl << endl
	<< "# Job execution" << endl;

    // cd to base dir of outputfile
    TString outdir(gSystem->DirName(fOutputFileName->Data()));
    out << "cd " << outdir.Data() << endl
	<< "hadd -f9 " << fOutputFileName->Data() << " ";

    TIter next_file(fInputFiles); 
    TString expd_infile;
    TObjString* objs = 0;
    while( (objs = (TObjString*)next_file()) ) {
	expd_infile = TString(gSystem->ExpandPathName(objs->GetString()));
	out << expd_infile.Data() << " ";
    }

    out << "> " << fLogFilePath->Data()
	<< " 2>&1" << endl;
    
    out.close();    
}

//____________________________________________________________________

Bool_t AtlMergingTask::ExecBatchJob(Option_t *option) {
    //
    // Exec Batch Job
    //

    TString opt = option;
    
    // Submit batch job
    return SubmitBatchJob();
}

//____________________________________________________________________

void AtlMergingTask::ExecGridJob(const Option_t*) {
    //
    // Exec grid job
    //
    Error("ExecGridJob", "Not supported! Abort!");
    gSystem->Abort(0);
}

//____________________________________________________________________

void AtlMergingTask::ExecNAFBatchJob(const Option_t*) {
    //
    // Exec NAF batch job
    //
    Error("ExecNAFBatchJob", "Not supported! Abort!");
    gSystem->Abort(0);
}

//____________________________________________________________________

void AtlMergingTask::CreateNAFBatchRunScript() {
    //
    // Create NAF Batch Run Script
    //
    Error("ExecNAFBatchRunScript", "Not supported! Abort!");
    gSystem->Abort(0);
}

//____________________________________________________________________

void AtlMergingTask::CreateGridRunScript() {
    //
    // Create Grid Run Script
    //
    Error("ExecGridRunScript", "Not supported! Abort!");
    gSystem->Abort(0);
}

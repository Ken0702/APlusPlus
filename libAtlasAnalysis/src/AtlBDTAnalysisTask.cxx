//____________________________________________________________________
//
// Task for running analyses using a BDT from TMVA
//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2014 (C) Soeren Stamm
//
#ifndef ATLAS_AtlBDTAnalysisTask
#include <AtlBDTAnalysisTask.h>
#endif
#include <TSystem.h>
#include <iostream>
#include <fstream>
#include <TClass.h>
#include <TDataMember.h>
#include <TROOT.h>

using namespace std;

#ifndef __CINT__
ClassImp(AtlBDTAnalysisTask);
#endif

//____________________________________________________________________

AtlBDTAnalysisTask::AtlBDTAnalysisTask(const char* name, const char* title) :
    AtlAppAnalysisTask(name, title) {
    //
    // Default constructor
    //

    SetBatchJob(kTRUE);
    SetBatchNodeAutomatic(kFALSE);

    AddUserEnv("TMVAHOME");
}

//____________________________________________________________________

AtlBDTAnalysisTask::~AtlBDTAnalysisTask() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlBDTAnalysisTask::CreateRunScript(Option_t *option) {
    //
    // Create run script for job submission
    //
    ofstream out;
    out.open(fRunScript->Data());
    out << "#!/bin/sh" << endl
	<< "# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
	<< "# !!! This is an automatically generated file !!!" << endl
	<< "# !!! D O   N O T   E D I T                   !!!" << endl
	<< "# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
	<< "#" << endl
	<< "# Run script for A++ analysis job submission" << endl
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
	    TString host2(gSystem->HostName());
	    if ( host2.Contains("lx") ) {	    
		out << "source /home/grid/lcg/sw/root_setup_sles11.sh-v5.34" << endl;
	    }
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

    // Load TMVA
    if ( gSystem->Getenv("TMVAHOME") != 0 ) {
	out << endl
	    << "# Setup TVMA version" << endl
	    << "source " << gSystem->Getenv("TMVAHOME") << "/test/setup.sh " << gSystem->Getenv("TMVAHOME")
	    << endl << endl;
    } else {
	Error("CreateRunScript", "Environment variable TMVAHOME not set. Abort!");
	gSystem->Abort(0);
    }
    
    out	<< endl << endl
	<< "# Job execution" << endl;
//	<< "export PBS_O_HOME=" << fTempOutputPath->Data() << endl
//	<< "export PBS_O_WORKDIR=" << fTempOutputPath->Data() << endl;
//	<< "cd $PBS_O_WORKDIR" << endl;

    if (  fTempOutputFileName != 0 ) {
	out << "if [ ! -d " << gSystem->DirName( fTempOutputFileName->Data() ) << " ]; then " << endl
	    << "   mkdir -p " << gSystem->DirName( fTempOutputFileName->Data() ) << endl
	    << "fi" << endl;
    }
    
    if ( fTempOutputPath != 0 && fTempLogFilePath != 0 ) {
	out << "mkdir -p " << fTempOutputPath->Data() << endl
	    << "cp " << fJobHome->Data() << "/analysis_run.C " << fTempOutputPath->Data() << endl
	    << "cd " << fTempOutputPath->Data() << endl
	    << endl
	    << "Remove old Logfiles " << endl
	    << "rm " << fTempLogFilePath->Data() << endl
	    << "rm " << fLogFilePath->Data() << endl
	    << "root -q -l -b analysis_run.C > " << fTempLogFilePath->Data()
	    << " 2>&1" << endl
	    << "mv " << fTempLogFilePath->Data() << " " << fLogFilePath->Data() << endl;
    } else {
	out << "JOBHOME=" << fJobHome->Data() << endl
	    << "cd $JOBHOME" << endl	
	    << "root -q -l -b analysis_run.C > " << fLogFilePath->Data()
	    << " 2>&1" << endl;
    }
    
    if (  fTempOutputFileName != 0 )
	out << "mv " << fTempOutputFileName->Data() << " " << fOutputFileName->Data() << endl;
    
    out.close();
}


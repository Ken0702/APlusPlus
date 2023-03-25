//____________________________________________________________________
//
// Atl Task for MemTk Analysis
// 
//  
// Author: stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2013 (C) stamm
//
#ifndef ATL_AtlMemTkAnalysisTask
#include <AtlMemTkAnalysisTask.h>
#endif
#include <TChain.h>
#include <TString.h>
#include <TObjString.h>
#include <TNamed.h>
#include <TSystem.h>
#include <iostream>
#include <fstream>
#include <TDatime.h>

using namespace std;

#ifndef __CINT__
ClassImp(AtlMemTkAnalysisTask);
#endif

//____________________________________________________________________

AtlMemTkAnalysisTask::AtlMemTkAnalysisTask(const char* name, const char* title) :
    AtlTask(name, title) {
    //
    // Default constructor
    //
    
    SetBatchJob(kTRUE);
    SetBatchNodeAutomatic(kFALSE);
    fRootScript = 0;
    fListOfUserEnvs = new TList;

    fNSubjobs = 1;
    fJobid = 1;

    fMemTkRunScript = 0;
    fMemTkTreatAllJetsAsBJets = kFALSE;
//    fGridHomeSetupScript = "";
    fD3PDVersion = TString("");
    fTarBallOnly = kFALSE;
    fGridAnalysisOnGrid = kFALSE;
    fGridUsePrecompiledMem = kTRUE;

    // Set environment variables for common libraries
    AddUserEnv("LIBATLASANALYSIS");
    AddUserEnv("LIBMEM");
}

//____________________________________________________________________

AtlMemTkAnalysisTask::~AtlMemTkAnalysisTask() {
    //
    // Default destructor
    //
    if ( fRootScript != 0 ) delete fRootScript;
    if ( fMemTkRunScript != 0 ) delete fMemTkRunScript;
    fListOfUserEnvs->Delete(); delete fListOfUserEnvs;
}

//____________________________________________________________________

void AtlMemTkAnalysisTask::ExecInteractiveJob(Option_t *option) {
    //
    // Interactive execution
    //
    // The interactive execution is done via an intermediate step of
    // an analysis run .C script in the same manner as for batch
    // jobs. Thus only a single way of job execution has to be
    // defined. In particular all selection cuts are set in the same
    // way.
    //
    TString opt = option;
    
    // Start analysis process

    // Check input chain
    // ToDo: CheckInputChain();

    Error("ExecInteractiveJob", "Currently not supported! Abort!\n");
    gSystem->Abort(0);
}    

//____________________________________________________________________

Bool_t AtlMemTkAnalysisTask::ExecBatchJob(Option_t *option) {
    //
    // Batch job submission
    //
    // Returns kFALSE in case of error.
    //
    TString opt = option;
    
    // Create submit scripts
    CreateRunScript();

    // Submit batch job
    return SubmitBatchJob();
}

//____________________________________________________________________

void AtlMemTkAnalysisTask::ExecGridJob(Option_t *option) {
    //
    // Grid job submission
    //
    TString opt = option;

    // setup script to create GRID_HOME and copy relevant files
    // -> for now do this externally; not for every submitted job!
    // if ( fGridHomeSetupScript == "" ) {
    //     Warning(__FUNCTION__, "No GridHomeSetupScript defined; make sure that "
    //             "all necessary files are present in $GRID_HOME!");
    // } else {
    //     TString gridhomesetup(Form("source %s", fGridHomeSetupScript.Data()));
    //     Info(__FUNCTION__, "Execute %s", gridhomesetup.Data());
    //     if ( gSystem->Exec(gridhomesetup.Data()) != 0 ) {
    //         Error(__FUNCTION__, "Failed to setup $GRID_HOME. Abort!");
    //         gSystem->Abort(0);
    //     }
    // }

    // abort if GRID_HOME not defined
    if ( gSystem->Getenv("GRID_HOME") == 0 ) {    
	Error("ExecGridJob", "Env. variable GRID_HOME is not defined, Abort!");
	gSystem->Abort(0);
    }

    // Create submit scripts
    CreateGridRunScript();

    // Submit grid job
    SubmitGridJob();
}

//____________________________________________________________________

void AtlMemTkAnalysisTask::CreateRunScript(Option_t *option) {
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

    // add A++ libraries (manually if WriteEnvSetup doesn't work)
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

    // add MemTk setup script
    char const * mem_setup_script = gSystem->Getenv("MEMTK_SETUP");
    if ( !mem_setup_script ) {
        Error(__FUNCTION__, "Need setup script for MemTk in MEMTK_SETUP");
        gSystem->Abort(0);
    }
    TString mem_setup_escaped(mem_setup_script);
    mem_setup_escaped.ReplaceAll("'", "\'");
    mem_setup_escaped.Insert(0, "'");
    mem_setup_escaped.Append("'");
    out << "source " << mem_setup_escaped << "\n";

    out << endl << endl
        << "# Job execution" << endl;

    out << "JOBHOME=" << fJobHome->Data() << endl
        << "cd $JOBHOME;" << endl
        << "# Remove old Logfiles " << endl
        << "rm " << fLogFilePath->Data() << endl;

    // Create Root-Script from master file
    if ( !(fInputFiles->GetEntries() == 1) ) {
        Error("CreateRunScript",
              "Number of inputfiles needs to be exactly one! This is not the case (n=%d)! Abort!",
              fInputFiles->GetEntries());
        gSystem->Abort(0);
    }

    // use "|" for sed since path name contains "/"
    if ( fMemTkRunScript == 0 ) {
        Error("CreateRunScript", "No MemTk run script defined. Abort!");
        gSystem->Abort();
    }
//    out << "cp " << fMemTkRunScript->Data() << " analysis_run.C"
    out << "cp $LIBMEM/test/" << fMemTkRunScript->Data() << " analysis_run.C"
        << endl
        << "sed -i 's|NSUBJOBS|"
        << fNSubjobs
        << "|g' analysis_run.C"
        << endl
        << "sed -i 's|INPUTFILE|"
        << ((TObjString*) fInputFiles->At(0))->GetString().Data();
    if ( fIsTest ) {
        // use this if analysis was also run with "test"
        out << "_test";
    }
    out << "|g' analysis_run.C"
        << endl
        << "sed -i 's|OUTPUTFILE|"
        << fOutputFileName->Data()
        << "|g' analysis_run.C"
        << endl
        << "sed -i 's|SUBJOBID|"
        << fJobid
        << "|g' analysis_run.C"
        << endl;
    if ( !fD3PDVersion.EqualTo("") ) {
        out << "sed -i 's|v20lj|"
            << fD3PDVersion.Data()
            << "|g' analysis_run.C"
            << endl;
    }

    // other MemTk options that should be set within A++
    if ( fMemTkTreatAllJetsAsBJets ) {
        out << "sed -i '/^.*evt_reader = new MemEvtReaderD3PDSgTopRun2.*/a\\"
            << endl
            << "    evt_reader->SetTreatAllJetsAsBJets(kTRUE);' analysis_run.C"
            << endl;
    }

    out << "root -q -l -b analysis_run.C > " << fLogFilePath->Data()
        << " 2>&1" << endl;
    out.close();
}

//____________________________________________________________________

void AtlMemTkAnalysisTask::CreateGridRunScript() {
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

    // Platform: same as on local machine
    if ( gSystem->Getenv("ARCH_TYPE") != 0 ) {
        out << "export ARCH_TYPE=" << gSystem->Getenv("ARCH_TYPE") << endl;
    } else {
        Fatal(__FUNCTION__, "Environment variable ARCH_TYPE not set.");
    }
    // out << "export ARCH_TYPE=`uname -i`-`uname -s`-gcc`gcc -dumpversion`-root`root-config --version | sed 's|/|.|'`" << endl
    //     << endl;

    // Cuba
    out << "# Cuba config" << endl
	<< "export CUBACORES=0" << endl
	<< "export CUBALIB=$PWD/Cuba-3.0" << endl
	<< "export LD_LIBRARY_PATH=${CUBALIB}:${LD_LIBRARY_PATH}" << endl
	<< endl;

    // LHAPDF
    out << "# LHAPDF config" << endl
	// the line below has to match with the gcc config...
        << "export LHAPDF6=/cvmfs/sft.cern.ch/lcg/releases/LCG_87/MCGenerators/lhapdf/6.2.0/x86_64-slc6-gcc49-opt" << endl
        << "export LHAPDF_DATA_PATH=/cvmfs/sft.cern.ch/lcg/external/lhapdfsets/current/:${LHAPDF6}/share/LHAPDF/" << endl
	<< "export LD_LIBRARY_PATH=${LHAPDF6}/lib/:${LD_LIBRARY_PATH}" << endl
	<< endl;

    // MemTk
    out << "# MemTk" << endl
	<< "export LIBMEM=$PWD/libMEM" << endl
	<< "export LD_LIBRARY_PATH=${LIBMEM}/lib/${ARCH_TYPE}:${LD_LIBRARY_PATH}" << endl
	<< endl;

    // ROOT_INCLUDE_PATH
    out << "# root include path" << endl
        << "export ROOT_INCLUDE_PATH=$LIBMEM/inc:$CUBALIB/inc:$LHAPDF6/include:$ROOT_INCLUDE_PATH" << endl
        << endl;

    out	<< endl << endl
	<< "# Job execution" << endl;

    // Create Root-Script from master file
    if ( !(fInputFiles->GetEntries() == 1) ) {
	Error(__FUNCTION__,
	      "Number of inputfiles needs to be exactly one! This is not the case (n=%d)! Abort!",
	      fInputFiles->GetEntries());
	gSystem->Abort(0);
    }

    // Copy config run_master from MemTk tarball (default file) to current directory
    // Modify it as it is done in CreateRunScript()
    if ( fMemTkRunScript == 0 ) {
	Error(__FUNCTION__, "No MemTk run script defined. Abort!");
	gSystem->Abort();
    }
    out << "cp $LIBMEM/test/" << fMemTkRunScript->Data() << " mem_analysis_run.C"
	<< endl
	<< "sed -i 's|NSUBJOBS|"
	<< fNSubjobs
        << "|g' mem_analysis_run.C"
        << endl;
    if ( fGridAnalysisOnGrid ) {
        out << "sed -i 's|INPUTFILE|"
            << "app_output.root"
            // << gSystem->BaseName(((TObjString*) fInputFiles->At(0))->GetString().Data())
            << "|g' mem_analysis_run.C"
            << endl;
    } else {
        out << "# Move input file"
            << endl
            << "mv " << gSystem->BaseName(((TObjString*) fInputFiles->At(0))->GetString().Data()) << " inputfile.root"
            << endl
            << "sed -i 's|INPUTFILE|"
            << "inputfile.root"
            << "|g' mem_analysis_run.C"
            << endl;
    }
    out << "sed -i 's|OUTPUTFILE|"
        << "mem_output.root"
        // << gSystem->BaseName(fOutputFileName->Data())
        << "|g' mem_analysis_run.C"
        << endl
	<< "sed -i 's|SUBJOBID|"
	<< fJobid
        << "|g' mem_analysis_run.C"
        << endl;
    if ( !fD3PDVersion.EqualTo("") ) {
        out << "sed -i 's|v20lj|"
            << fD3PDVersion.Data()
            << "|g' mem_analysis_run.C"
            << endl;
    }
    
    out << "root -q -l -b -n $LIBMEM/test/rootlogon.C mem_analysis_run.C" << endl;
    // Logfile is always available on grid machines!
    out.close();

    // Copy run script to GRID_HOME (replacing existing one)
    // Keep it for now (might be useful to check quickly offline
    // what has been produced on the grid)
    gSystem->Exec(Form("cp %s %s",
		       fRunScript->Data(),
		       gSystem->Getenv("GRID_HOME")));
}

//____________________________________________________________________

void AtlMemTkAnalysisTask::AddUserEnv(const char* env) {
    //
    // Add user-defined environment variable for shared library search
    // paths
    //
    fListOfUserEnvs->Add(new TNamed(env, ""));
}

//____________________________________________________________________

void AtlMemTkAnalysisTask::Print(Option_t *option) const {
    //
    // Print config
    //
    cout << "=============================" << endl;
    cout << " Task: " << fName << endl;
    cout << " ----------------------------" << endl;
    cout << " JobHome : " << fJobHome->Data() << endl;
    cout << " OutputFileName: " << fOutputFileName->Data() << endl;
    cout << " " << endl;
    cout << " " << endl;
    cout << " " << endl;
    cout << "=============================" << endl;
}

//____________________________________________________________________

void AtlMemTkAnalysisTask::SubmitGridJob() {
    //
    // Submit MemTk grid job
    //

    // strategy for submission of Analysis+MemTk on grid:
    // - check that requirements are there (grid_home or whereever)
    // - go to run directory and copy requirements there
    // - create prun submission command, write to command.txt
    // - use alternative run script that goes over files in input.txt
    // - outside the scope of A++:
    //   - copy code to collect_sub directory
    //   - edit prun command to combine commands from A++ and MemTk

    Bool_t app_is_on_grid = fGridAnalysisOnGrid;
    TString cmd = "";

    // go to GRID_HOME directory
    TString gridhome = gSystem->ExpandPathName("$GRID_HOME");
    gSystem->ChangeDirectory(gridhome.Data());

    TString pwd = gSystem->Getenv("PWD");
    Info(__FUNCTION__, "Current directory: %s", pwd.Data());

    // Check if all necessary files are present
    // 1. cuba library
    TString cuba30 = "Cuba-3.0.tar.gz";
    TString cuba30_tmp = cuba30;
    if ( gSystem->FindFile(gridhome.Data(), cuba30_tmp) == 0) {
	Error("SubmitGridJob", "File not found! %s/%s",
	      gridhome.Data(),
	      cuba30.Data());
	gSystem->Abort(0);
    }

    // 2. MemTk library
    TString libMEM = "libMEM.tgz";
    TString libMEM_tmp = libMEM;
    if ( gSystem->FindFile(gridhome.Data(), libMEM_tmp) == 0) {
	Error("SubmitGridJob", "File not found! %s/%s",
	      gridhome.Data(),
	      libMEM.Data());
	gSystem->Abort(0);
    }
    
    // 3. LHAPDF: no archive, just env variable pointing to cvmfs

    // 4. bexec.sh script to unpack files on grid
    TString bexec = "bexec.sh";
    TString bexec_tmp = bexec;
    if ( gSystem->FindFile(gridhome.Data(), bexec_tmp) == 0) {
	Error("SubmitGridJob", "File not found! %s/%s",
	      gridhome.Data(),
	      bexec.Data());
	gSystem->Abort(0);
    }

    // 5. rootlogon.C script to load libraries before analysis_run.C
    // since this comes from $LIBMEM/test/rootlogon.C, don't require it here

    // prepare extFile option
    // without app on grid, this will include the input file
    // extFile will be added to prun except for app-on-grid tarballs
    TString extFile = Form("--extFile=%s,%s",
                           cuba30.Data(),
                           libMEM.Data());

    // file to save run commands in
    // we work in GRID_HOME but move files to run_dir later
    TString run_dir = gSystem->ConcatFileName(gSystem->DirName(fRunScript->Data()), "run");
    gSystem->mkdir(run_dir.Data(), kTRUE);
    TString command_file = gSystem->ConcatFileName(run_dir,"mem_command.txt");

    // create gridid for inDS/outDS (don't use time for uniqueness any more)
    TString gridid = Form("%s_%s", GetName(), GetTitle());
    gridid.Append(fGridIdSuffix.Data());
    TString input_file_path = ((TObjString*) fInputFiles->At(0))->GetString();

    // Rename run script to avoid possible conflict with AppAnalysis scripts
    TString new_runscript_name = "mem_";
    new_runscript_name.Append(gSystem->BaseName(fRunScript->Data()));
    cmd = Form("mv %s %s", gSystem->BaseName(fRunScript->Data()), new_runscript_name.Data());
    if ( gDebug ) Info(__FUNCTION__, "Rename run script:\n\"%s\"", cmd.Data());
    gSystem->Exec(cmd.Data());
    cmd = Form("chmod +x %s", new_runscript_name.Data());
    if ( gDebug ) Info(__FUNCTION__, "Make executable:\n\"%s\"", cmd.Data());
    gSystem->Exec(cmd.Data());

    // create prun submission command
    TString jobsub_cmd("prun ");
    // environment options
    jobsub_cmd.Append(Form("--rootVer=%s ", fGridRootVersion->Data()));
    jobsub_cmd.Append(Form("--cmtConfig=%s ", fGridCmtVersion->Data()));
    // run options
    jobsub_cmd.Append("--bexec='./bexec.sh' ");
    jobsub_cmd.Append(Form("--exec='./%s' ", new_runscript_name.Data()));
    // gSystem->BaseName(fRunScript->Data())));
    jobsub_cmd.Append("--long ");
    jobsub_cmd.Append("--maxCpuCount=172800 ");
    // I/O options
    // inDS
    if ( app_is_on_grid ) {
        // take inDS with same name as outDS of AtlAppAnalysisTask
        jobsub_cmd.Append(Form("--inDS='user.%s.schannelAPP.%s.%s' ",
                               fGridUserName->Data(),
                               gridid.Data(),
                               fGridSuffix->Data()));
        jobsub_cmd.Append("--writeInputToTxt=IN:input.txt ");
        jobsub_cmd.Append("--nFilesPerJob=1 ");
    } else {
        // Add input file to grid home and to extFile, no inDS
        TString addfile = Form("cp %s %s",
                               input_file_path.Data(),
                               gSystem->Getenv("GRID_HOME"));
        Info("SubmitGridJob", "Add file to grid_home:\n%s", addfile.Data());
        gSystem->Exec(addfile.Data());
        extFile.Append(",");
        extFile.Append(gSystem->BaseName(input_file_path.Data()));
    }
    extFile.Append(" ");

    // outDS
    jobsub_cmd.Append(Form("--outDS='user.%s.schannelMemTk.%s.%s' ",
                           fGridUserName->Data(),
                           gridid.Data(),
                           fGridSuffix->Data()));
    jobsub_cmd.Append("--outputs=mem_output.root ");
    jobsub_cmd.Append("--destSE=DESY-ZN_LOCALGROUPDISK ");

    // submission options
    if ( fDebugBuild ) {
        jobsub_cmd.Append("-v ");
    }

    // Produce only tarball, without submission?
    // The result is copied to run_dir (for now original is kept)
    if ( fTarBallOnly ) {
        TString tarball_name = TString(gSystem->BaseName(fOutputFileName->Data()));
        tarball_name.ReplaceAll(".root", ".tgz");
        Bool_t extra_dry = kTRUE;
        if ( extra_dry && app_is_on_grid ) {
            // extra dry: don't execute prun because it takes a while;
            //   simply build the archive with tar, also
            //   don't add libMEM -> use external scripts to add on demand
            // TODO/WARNING: this is hardcoded for now for App+MemTk runs,
            //   separate runs for MEM on grid-produced analysis output
            //   won't work with extra_dry
            TString extra_dry_cmd(
                Form("tar -czvf %s %s %s %s ",
                     tarball_name.Data(),
                     "bexec.sh",
                     cuba30.Data(),
                     // libMEM.Data(),
                     new_runscript_name.Data()));
            Info(__FUNCTION__, "Execute %s", extra_dry_cmd.Data());
            Int_t err_code = gSystem->Exec(extra_dry_cmd.Data());
            if ( err_code != 0 ) {
                Fatal(__FUNCTION__, "Submission command was not successful. Abort!");
            }
        } else {
            // use prun --noSubmit to create archive
            TString dry_cmd = jobsub_cmd;
            dry_cmd.Append(Form("--outTarBall=%s ", tarball_name.Data()));
            dry_cmd.Append(Form("--noSubmit "));
            dry_cmd.Append(extFile.Data());
            Info("SubmitGridJob", "Execute %s", dry_cmd.Data());
            Int_t err_code = gSystem->Exec(dry_cmd.Data());
            if ( err_code != 0 ) {
                Fatal(__FUNCTION__, "Submission command was not successful. Abort!");
            }
        }
        // write execution command to file
        jobsub_cmd.Append(Form("--inTarBall=%s ", tarball_name.Data()));
        ofstream out;
        out.open(command_file);
        out << jobsub_cmd.Data() << endl;
        out.close();
        Info(__FUNCTION__, "Job not submitted, submission command is in %s.",
             command_file.Data());
        // copy tarball to run directory
        cmd = Form("cp -t %s %s", run_dir.Data(), tarball_name.Data());
        if ( fDebugBuild )
            Info(__FUNCTION__, "Copy tarball to run directory:\n\"%s\"", cmd.Data());
        gSystem->Exec(cmd.Data());
    } else {
        // Submit prun cmd directly and check success
        jobsub_cmd.Append(extFile.Data());
        Info("SubmitGridJob", "Execute %s", jobsub_cmd.Data());
        Int_t err_code = gSystem->Exec(jobsub_cmd.Data());
        if ( err_code != 0 ) {
            Error(__FUNCTION__, "Submission command was not successful. Abort!");
            gSystem->Abort();
        }
    }

    if ( !app_is_on_grid ) {
        // remove large input file from grid home directory
        TString removefile = Form("rm %s/%s",
                                  gSystem->Getenv("GRID_HOME"),
                                  gSystem->BaseName(input_file_path.Data()));
        Info("SubmitGridJob", "Remove file from grid_home:\n%s", removefile.Data());
        gSystem->Exec(removefile.Data());
    }
    // Remove runscript from GRID_HOME
    TString removeRunscript = Form("rm %s/%s",
                                   gSystem->Getenv("GRID_HOME"),
                                   new_runscript_name.Data());
//                                   gSystem->BaseName(fRunScript->Data()));
    Info("SubmitGridJob","%s", removeRunscript.Data());
    gSystem->Exec(removeRunscript.Data());

    // Create Logfile
    CreateLogFilePath();
    // Remove old logfile if it exist
    gSystem->Exec(Form("rm -f %s", fLogFilePath->Data()));

    // Add the string that is used in AtlTopLevelAnalysis::GetTaskStatus
    // to check if the job was successful

    // Get some information on the day, month, year.
    TDatime time;
    TString weekday = "";
    switch ( time.GetDayOfWeek() ) {
	case 1: weekday.Append("Mon."); break;
	case 2: weekday.Append("Tue."); break;
	case 3: weekday.Append("Wed."); break;
	case 4: weekday.Append("Thu."); break;
	case 5: weekday.Append("Fri."); break;
	case 6: weekday.Append("Sat."); break;
	case 7: weekday.Append("Sun."); break;
	default:
	    Error("SubmitGridJob", "Unkown day of week. Abort!");
	    gSystem->Abort();
    }	    
    ofstream out;
    out.open(fLogFilePath->Data());
    out << "Job Summary" << endl
	<< "Grid job was submitted on "
	<< weekday.Data() << ", "
	<< time.GetDay() << "."
	<< time.GetMonth() << "."
	<< time.GetYear() << " at "
	<< time.GetHour() << ":"
	<< time.GetMinute() << "."
	<< time.GetSecond() << endl;
    out.close();
}

//____________________________________________________________________

void AtlMemTkAnalysisTask::SetMemTkRunScript(const char* script) {
    //
    //
    //
    if ( fMemTkRunScript != 0 ) delete fMemTkRunScript;
    fMemTkRunScript = new TString(script);
}

//____________________________________________________________________
//
// Abstract analysis task
// 
//
// Batch job submission (at HU cluster):
// =====================================
//
// There are three types of batch job submission:
//
// 1. Run on all nodes: SetBatchNodeAll(kTRUE) The jobs are submitted
// to all free nodes.
//
// 2. Run on specific node: SetBatchNode(node) You can specify the
// node the job is submitted to (eg "ms01b"). Opens a window in
// TBrowser where you can enter the node name.
//
// 3. Automatic job submission: SetBatchNodeAutomatic(kTRUE) Sets node
// automatically, regarding the machine the sample files are saved on.
//
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!                                                                     !!!!
// !!!! Important note                                                      !!!!
// !!!!                                                                     !!!!
// !!!! The last case is the default and should never be changed. Otherwise !!!!
// !!!! the data will be transferred via the network file system (NFS) from !!!!
// !!!! the computer containing the data disk to the batch node in question !!!!
// !!!! This will put a heavy load both on the network and the NFS server   !!!!
// !!!! machine which causes heavy performance losses of the whole system   !!!!
// !!!! affecting all users.                                                !!!!
// !!!!                                                                     !!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlTask
#include <AtlTask.h>
#endif
#include <TObjString.h>
#include <TString.h>
#include <TList.h>
#include <TSystem.h>
#include <iostream>
#include <TDirectory.h>
#include <TDirectoryFile.h>
#include <TKey.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TH2D.h>

using namespace std;

#ifndef __CINT__
ClassImp(AtlTask);
#endif

//____________________________________________________________________

AtlTask::AtlTask(const char* name, const char* title) :
    TTask(name, title) {
    //
    // Default constructor
    //
    fInputFiles = new TList;
    fInputEntryLists = new TList;
    SetBatchJob(kTRUE);
    fOutputFileName = 0;
    fOutputTreeName = 0;
    fNEvents = 0;
    fFirstEntry = 0;
    fIsTest = kFALSE;
    fJobHome     = 0;
    fLogFilePath = 0;
    fRunScript   = 0;
    fTempLogFilePath = 0;
    fTempOutputPath  = 0;
    fTempOutputFileName = 0;
    fBatchNodeAll = kFALSE;
    fBatchNodeAutomatic = kTRUE;
    fBatchNode = "ms01b";
    fGridRootVersion = 0;
    fGridCmtVersion  = 0;
    fGridUserName    = 0;
    fGridSuffix      = 0;
    fGridIdSuffix    = "";
    fDebug           = 0;
    fDebugBuild      = 0;
}

//____________________________________________________________________

AtlTask::~AtlTask() {
    //
    // Default destructor
    //
    fInputFiles->Delete(); delete fInputFiles;
    if ( fOutputFileName  != 0 ) delete fOutputFileName;
    if ( fOutputTreeName  != 0 ) delete fOutputTreeName;
    if ( fJobHome         != 0 ) delete fJobHome;
    if ( fLogFilePath     != 0 ) delete fLogFilePath;
    if ( fRunScript       != 0 ) delete fRunScript;
    if ( fTempLogFilePath != 0 ) delete fTempLogFilePath;
    if ( fTempOutputPath  != 0 ) delete fTempOutputPath;
    if ( fTempOutputFileName != 0 ) delete fTempOutputFileName;
    if ( fGridRootVersion != 0 ) delete fGridRootVersion;
    if ( fGridCmtVersion  != 0 ) delete fGridCmtVersion; 
    if ( fGridUserName    != 0 ) delete fGridUserName;
    if ( fGridSuffix      != 0 ) delete fGridSuffix;
}

//____________________________________________________________________

void AtlTask::AddInputFiles(const char* InputFiles) {
    //
    // Add files to input chain (wildcards allowed)
    //
    TObjString *item = new TObjString(InputFiles);
    fInputFiles->Add(item);
}

//____________________________________________________________________

void AtlTask::AddInputEntryLists(const char* InputFile) {
    //
    // Add files to input entrylists
    //
    
    TObjString *item = new TObjString(InputFile);
    fInputEntryLists->Add(item);
}

//____________________________________________________________________

void AtlTask::SetOutputFile(const char* OutputFile) {
    //
    // Set output file name
    //
    fOutputFileName = new TString(OutputFile);
}

//____________________________________________________________________

void AtlTask::SetTempOutputFile(const char* OutputFile) {
    //
    // Set temporarily output file name during batch job
    //
    fTempOutputFileName = new TString(OutputFile);
}

//____________________________________________________________________

void AtlTask::SetInteractiveJob(Bool_t InteractiveJob) {
    //
    // Set execution mode to interactive
    // Switch off writing to logfile
    //
    fInteractiveJob =  InteractiveJob;
    fBatchJob       = !InteractiveJob;
    fNAFBatchJob    = !InteractiveJob;
    fGridJob        = !InteractiveJob;
    SetLogFile(kFALSE);
}

//____________________________________________________________________

void AtlTask::SetBatchJob(Bool_t BatchJob) {
    //
    // Set execution mode to batch job
    // Switch on writing to logfile
    //
    fBatchJob       =  BatchJob;
    fNAFBatchJob    = !BatchJob;
    fInteractiveJob = !BatchJob;
    fGridJob        = !BatchJob;
    SetLogFile(kTRUE);
}

//____________________________________________________________________

void AtlTask::SetNAFBatchJob(Bool_t NAFBatchJob) {
    //
    // Set execution mode to batch job
    // Switch on writing to logfile
    //
    fNAFBatchJob    =  NAFBatchJob;
    fBatchJob       = !NAFBatchJob;
    fInteractiveJob = !NAFBatchJob;
    fGridJob        = !NAFBatchJob;
    SetLogFile(kTRUE);
}

//____________________________________________________________________

void AtlTask::SetGridJob(Bool_t GridJob) {
    //
    // Set execution mode to Grid job
    // Switch on writing to logfile
    //

    fGridJob        =  GridJob;
    fInteractiveJob = !GridJob;
    fBatchJob       = !GridJob;
    fNAFBatchJob    = !GridJob;
    SetLogFile(kTRUE);
}

//____________________________________________________________________

void AtlTask::SetOutputTree(const char* name, const char* title) {
    //
    // Set name and title of the output tree
    //
    if ( fOutputTreeName ==  0 ) {
	fOutputTreeName = new TNamed(name, title);
    } else{
	fOutputTreeName->SetNameTitle(name, title);
    }
}

//____________________________________________________________________

void AtlTask::SetBatchNodeAutomatic(Bool_t BatchNodeAutomatic) {
    //
    // Submit job to automatically set node
    //
    fBatchNodeAutomatic   = BatchNodeAutomatic;
    fBatchNodeAll = !BatchNodeAutomatic;
}

//____________________________________________________________________

void AtlTask::SetBatchNodeAll(Bool_t BatchNodeAll) {
    //
    // Submit job to all nodes
    //
    fBatchNodeAll         = BatchNodeAll;
    fBatchNodeAutomatic   = !BatchNodeAll;
}

//____________________________________________________________________

void AtlTask::SetBatchNode(const char* BatchNode)  {
  //
  // Set batch node (eg. "ms01b")
  //
  if( strcmp(BatchNode,"ms01b") != 0 && 
      strcmp(BatchNode,"ms02b") != 0 && 
      strcmp(BatchNode,"ms03b") != 0 &&
      strcmp(BatchNode,"ms04b") != 0 && 
      strcmp(BatchNode,"ms05b") != 0 &&
      strcmp(BatchNode,"ms06b") != 0 &&
      strcmp(BatchNode,"ms07b") != 0 &&
      strcmp(BatchNode,"ms08b") != 0 &&
      strcmp(BatchNode,"as01b") != 0 &&
      strcmp(BatchNode,"as02b") != 0 &&
      strcmp(BatchNode,"as03b") != 0 &&
      strcmp(BatchNode,"as04b") != 0 &&
      strcmp(BatchNode,"as05b") != 0 &&
      strcmp(BatchNode,"as06b") != 0 &&
      strcmp(BatchNode,"as07b") != 0 &&
      strcmp(BatchNode,"as08b") != 0 &&
      strcmp(BatchNode,"as21b") != 0 &&
      strcmp(BatchNode,"as22b") != 0 &&
      strcmp(BatchNode,"as23b") != 0 &&
      strcmp(BatchNode,"as24b") != 0 &&
      strcmp(BatchNode,"as25b") != 0 &&
      strcmp(BatchNode,"as26b") != 0 &&
      strcmp(BatchNode,"as27b") != 0 &&
      strcmp(BatchNode,"as28b") != 0 )  {
    Error("SetBatchNode", "Invalid node given. Node not set.");
  } 
  fBatchNodeAll = kFALSE;
  fBatchNodeAutomatic = kFALSE;
  fBatchNode = BatchNode;
}

//____________________________________________________________________

Char_t AtlTask::GetBatchNodeFromPath(const char* path) {
    //
    // Extract batch node from path name (usually of input filenames)
    // Jobs should run on the node where the input data resides to
    // minimise network traffic. The data disks at the HU cluster are
    // labelled /datm1, /datm2, /datm3, /dats1 etc, w.r.t. the machine name.
    // If the node name cannot be found '1' will be returned
    //
    TString path_str(path);
    if ( (path_str.Index("/datm") == 0) ||
	 (path_str.Index("/dats") == 0) ) return path_str(5);
    return '1';
}

//____________________________________________________________________

void AtlTask::Exec(Option_t *option) {
    //
    // Task execution
    //
    // Options available:
    //  "test" - run in test modus: process 1000 events only
    //           and append _test to the output filename
    //
    cout << endl;
    Info("Exec", "Task \"%s\" started", GetName());

    Bool_t success = kTRUE;

    TString opt = option;
    opt.ToLower();
    if ( opt.Contains("test") ) {
	fIsTest = kTRUE;
	fNEvents = 1000;
    }

    // Create paths
    CreateJobHome();
    if ( !fGridJob ) CreateLogFilePath();
    if ( !fGridJob ) CreateOutFilePath();
    
    // Start job
    if ( fInteractiveJob ) {
	// ================
	// Interactive mode
	// ================
	ExecInteractiveJob(option);
    } else if ( fBatchJob ) {
	// ===============
	// Batch execution
	// ===============
	CreateRunScriptPath();
	CreateRunScript();
	success = ExecBatchJob(option);
    } else if ( fGridJob ) {
	// ==================
	// Grid job execution
	// ==================
	CreateRunScriptPath();
	ExecGridJob(option);
    } else if ( fNAFBatchJob ) {
	// ===================
	// NAF Batch execution
	// ===================
	CreateRunScriptPath();
	CreateNAFBatchRunScript();
	ExecNAFBatchJob(option);
    } 

    if (success) Info("Exec", "Task \"%s\" finished", GetName());
    else Error("Exec", "Task \"%s\" execution failed.", GetName());
    cout << endl;
}

//____________________________________________________________________

void AtlTask::SetJobHome(const char* JobHome) {
    //
    // Set working directory for the job.
    //
    if ( fJobHome != 0 ) delete fJobHome;
    fJobHome = new TString(JobHome);
}

//____________________________________________________________________

void AtlTask::CreateJobHome() {
    //
    // Create working directory for this job (for log files etc) In
    // case no directory was given so far by SetJobHome() create a
    // directory with the job's name in the current working directory
    //
    if ( fJobHome == 0 ) {
	if ( fGridJob ) {
	    fJobHome = new TString(gSystem->ExpandPathName("$GRID_HOME/submitscripts"));
	} else {
	    fJobHome = new TString(gSystem->pwd());
	}
	fJobHome->Append("/");
	fJobHome->Append(GetName());
    }
    
    Info("CreateJobHome", "Create job working directory\n%s",
	 fJobHome->Data());
    gSystem->Exec(Form("rm -fr %s", fJobHome->Data()));
    gSystem->Exec(Form("mkdir -p %s", fJobHome->Data()));
}

//____________________________________________________________________

void AtlTask::CreateRunScriptPath() {
    //
    // Create full path to the runscript used for batch job submission
    //
    if ( fRunScript != 0 ) {
	delete fRunScript; fRunScript = 0;
    }
    fRunScript = new TString(fJobHome->Data());
    fRunScript->Append("/");
    fRunScript->Append(GetName());
    fRunScript->Append(".run");
    fRunScript->ReplaceAll("//", 2, "/", 1);
}

//____________________________________________________________________

void AtlTask::CreateLogFilePath() {
    //
    // Create full path to the logfile
    //
    if ( fOutputFileName == 0 ) {
      Error("CreateLogFilePath", "No output filename given for this task \"%s\". Abort!",
	    GetName());
       gSystem->Abort(0);
    }

    if ( fLogFilePath != 0 ) { delete fLogFilePath; fLogFilePath = 0; }
    fLogFilePath = new TString(fJobHome->Data());
    fLogFilePath->Append("/");
    fLogFilePath->Append(gSystem->BaseName(fOutputFileName->Data()));
    fLogFilePath->ReplaceAll(".root", 5, ".log", 4);
    fLogFilePath->ReplaceAll("//", 2, "/", 1);

    if ( fTempOutputPath != 0 && fTempOutputPath->BeginsWith("/") ) {
	fTempLogFilePath = new TString(fTempOutputPath->Data());
	fTempLogFilePath->Append("/");
	fTempLogFilePath->Append(gSystem->BaseName(fOutputFileName->Data()));
	fTempLogFilePath->ReplaceAll(".root", 5, ".log", 4);
	fTempLogFilePath->ReplaceAll("//", 2, "/", 1);
    }
}

//____________________________________________________________________

void AtlTask::CreateOutFilePath() {
    //
    // Create path to the output file(s).  In case the already given
    // outfile name is absolute (ie starts with'/') do nothing.
    // Otherwise prepend the JobHome path
    //
    TString expd_name(gSystem->ExpandPathName(fOutputFileName->Data()));
    if ( !expd_name.BeginsWith("/") ) {
	fOutputFileName->Prepend("/");
	fOutputFileName->Prepend(fJobHome->Data());
    }
    if ( fIsTest ) {
	fOutputFileName->Append("_test");
    }
    Info("CreateOutFilePath", "Create outfile path:\n%s",
	 gSystem->DirName(fOutputFileName->Data()));
    gSystem->Exec(Form("mkdir -p %s",  gSystem->DirName(fOutputFileName->Data())));
}

//____________________________________________________________________

Bool_t AtlTask::SubmitBatchJob() {
    //
    // Batch job submission If no node had been specified it is tried
    // to guess the node name from the input file list
    //
    // Returns kFALSE in case of error in batch job submission
    //

    TString jobsub_cmd("condor_qsub ");
    TString expd_outfile(gSystem->ExpandPathName(fOutputFileName->Data()));
    TString expd_infile;
    if ( fInputFiles->GetEntries() > 0 )
	expd_infile = TString(gSystem->ExpandPathName(((TObjString*)fInputFiles
						       ->At(0))->GetString()));

    TString host2(gSystem->HostName());
    if ( host2.Contains("lx") ) {
      jobsub_cmd.Append(" -o /dev/null -j y "); // redirect std output and std error to black hole
      jobsub_cmd.Append(fRunScript->Data());
    } else { 
	if ( fBatchNodeAll ) {
	  jobsub_cmd.Append(" ");
	} else if ( fBatchNodeAutomatic  ) {
	  if ( expd_infile.Index("/rdsk/datm") == 0 ) {
	    jobsub_cmd.Append("-l nodes=ms0");
	    jobsub_cmd.Append(expd_infile(10));
	    jobsub_cmd.Append("b ");
	  }
	  if ( expd_infile.Index("/rdsk/dats") == 0 ) {
	    jobsub_cmd.Append("-l nodes=as0");
	    jobsub_cmd.Append(expd_infile(10));
	    jobsub_cmd.Append("b ");
	  }
	  if ( expd_infile.Index("/rdsk/datv") == 0 ) {
	    jobsub_cmd.Append("-l nodes=as1");
	    jobsub_cmd.Append(expd_infile(10));
	    jobsub_cmd.Append("b ");
	  }
	  if ( expd_infile.Index("/rdsk/dat2") == 0 ) {
		  fBatchNodeAll=kTRUE; //v34: kkreul: wasnt able to say condor which node he should use, Right now condor decides
		  	  	  	  	  	   //Olf said its ok to use all node (preferred order: as2, as1, as0, ms0),
		  	  	  	  	  	  // Condor uses as08b then ms0xb -.-
	  	  }

	} else {
	  jobsub_cmd.Append("-l nodes=");
	  jobsub_cmd.Append(fBatchNode);
	  jobsub_cmd.Append(" ");
	}
	//if(fPriority != 0 ) jobsub_cmd.Append(Form(" -p %d ", fPriority)); //condor Priority

	jobsub_cmd.Append(" -V -o /dev/null -e /dev/null "); // redirect std output and std error to black hole, condor
			// "-j oe "); // redirect std output and std error to black hole
	jobsub_cmd.Append(fRunScript->Data());
	//jobsub_cmd.Append("/users/eeh/kkreul/APlusPlus/libSingleTop/tasks/13TeV/combination/hostname.run");

	/*// Check for correct submission machine
	TString host(gSystem->HostName());
	host.Remove(3);
	host.Prepend("nodes=");

	if ( !jobsub_cmd.Contains(host) && !fBatchNodeAll) {
            Error("SubmitBatchJob", "Submission from this machine to desired queue impossible.\nNote that the batch systems on the ms0Xb and as0Xb machines are separated and you cannot submit jobs from one system to the other or vice versa. Either switch to the other system for submission (make sure everything is compiled and linked on that platform), or set a node of the system you are working on explicitly.\nInput: %s\nHost: %s", expd_infile.Data(), host.Data());
	  return kFALSE;
	}*/
    }
    Info("SubmitBatchJob", "Execute %s", jobsub_cmd.Data());
    if (fPriority < 10) gSystem->Exec(jobsub_cmd.Data()); //kkreul: This is a hack
    else{                        // too lazy to include a new variable
     TString jobsub2 = "condor_submit " + *fJobHome + "submit";
     
     gSystem->Exec("chmod 777 " +*fRunScript);
     std::cout<<jobsub2.Data()<<std::endl;
     gSystem->Exec(jobsub2.Data());
    // Sleep for 2 sec to avoid overloading the batch system when
    // submitting huge numbers of jobs
    }
    if ( ! host2.Contains("lx") ) {    
      gSystem->Sleep(200);
      
    }
    
    return kTRUE;
}

//____________________________________________________________________

void AtlTask::SubmitNAFBatchJob() {
    //
    // Batch job submission If no node had been specified it is tried
    // to guess the node name from the input file list
    //
    TString jobsub_cmd("qsub ");
    TString expd_outfile(gSystem->ExpandPathName(fOutputFileName->Data()));
    TString expd_infile(gSystem->ExpandPathName(((TObjString*)fInputFiles
						 ->At(0))->GetString()));
    
    jobsub_cmd.Append(fRunScript->Data());
    Info("SubmitBatchJob", "Execute %s", jobsub_cmd.Data());
    gSystem->Exec(jobsub_cmd.Data());
}

//____________________________________________________________________

void AtlTask::SubmitGridJob() {
    //
    // Grid job submission 
    //
    TString jobsub_cmd("source "); 
    jobsub_cmd.Append(GetName());
    jobsub_cmd.Append(".run");
    Info("SubmitGridJob", "Execute %s", jobsub_cmd.Data());
    gSystem->ChangeDirectory(gSystem->ExpandPathName("$GRID_HOME"));
    gSystem->Exec(jobsub_cmd.Data());
}

//____________________________________________________________________

void AtlTask::SetTempOutputPath(const char* OutputPath) {
    //
    // Set temporarily output path for local HU batch system
    //
    
    // If you use this, make sure your outfile name is a relative one
    fTempOutputPath = new TString(OutputPath);
}

//____________________________________________________________________

void AtlTask::CopyFolder(TDirectory *SrcDir, TDirectory *DestDir,
			 Float_t scale) {
    //
    // Recursively copying of the content (histograms, subfolders) of the given source
    // directory to the destination folder.
    // All histograms will be scaled by the given factor.
    //
    
    // Search source content
    SrcDir->cd();
    
    TIter next_key(SrcDir->GetListOfKeys());
    TKey *key = 0;
    while ( (key = (TKey*)next_key()) ) {
	if ( strcmp(key->GetClassName(), "TDirectoryFile") == 0 ) {
	    TDirectory *NewSrcDir = (TDirectoryFile*)key->ReadObj();
 	    DestDir->cd();
 	    TDirectory *NewDestDir = DestDir->mkdir(key->GetName());
	    CopyFolder(NewSrcDir, NewDestDir, scale);
	} else if ( (strcmp(key->GetClassName(), "TH1F") == 0) ) {
	    DestDir->cd();
	    TH1F *h1f = (TH1F*)key->ReadObj()->Clone();
	    h1f->SetDirectory(DestDir);
	    h1f->Scale(scale);
	} else if ( (strcmp(key->GetClassName(), "TH1D") == 0) ) {
	    DestDir->cd();
	    TH1D *h1d = (TH1D*)key->ReadObj()->Clone();
	    h1d->SetDirectory(DestDir);
	    h1d->Scale(scale);
	} else if ( (strcmp(key->GetClassName(), "TH2F") == 0) ) {
	    DestDir->cd();
	    TH2F *h2f = (TH2F*)key->ReadObj()->Clone();
	    h2f->SetDirectory(DestDir);
	    h2f->Scale(scale);
	} else if ( (strcmp(key->GetClassName(), "TH2D") == 0) ) {
	    DestDir->cd();
	    TH2D *h2d = (TH2D*)key->ReadObj()->Clone();
	    h2d->SetDirectory(DestDir);
	    h2d->Scale(scale);
	}
    }
}

//____________________________________________________________________

void AtlTask::SetGridRootVersion(const char* RootVersion) {
    //
    // Set grid root version
    //
    if ( fGridRootVersion != 0 ) delete fGridRootVersion;
    fGridRootVersion = new TString(RootVersion);
}

//____________________________________________________________________

void AtlTask::SetGridCmtVersion(const char* CmtVersion) {
    //
    // Set grid cmt version
    //
    if ( fGridCmtVersion != 0 ) delete fGridCmtVersion;
    fGridCmtVersion = new TString(CmtVersion);
}

//____________________________________________________________________

void AtlTask::SetGridUserName(const char* UserName) {
    //
    // Set grid root version
    //
    if ( fGridUserName != 0 ) delete fGridUserName;
    fGridUserName = new TString(UserName);
}

//____________________________________________________________________

void AtlTask::SetGridSuffix(const char* Suffix) {
    //
    // Set grid root version
    //
    if ( fGridSuffix != 0 ) delete fGridSuffix;
    fGridSuffix = new TString(Suffix);
}

//____________________________________________________________________

Bool_t AtlTask::WriteEnvSetup(std::ofstream & out) const {
    //
    // Generates the shell command(s) that set up the current A++/ROOT/... environment.
    // Returns kFALSE if it's not known how to set up the environment.
    //
    char const * appSetupScriptPath = gSystem->Getenv("APLUSPLUS_SETUP");
    if ( !appSetupScriptPath )
        return kFALSE;

    TString appSetupScriptPathEscaped(appSetupScriptPath);
    appSetupScriptPathEscaped.ReplaceAll("'", "\'");
    appSetupScriptPathEscaped.Insert(0, "'");
    appSetupScriptPathEscaped.Append("'");
    out << "unset ROOTSYS" << "\n";
    out << "source " << appSetupScriptPathEscaped << "\n";
    return kTRUE;
}

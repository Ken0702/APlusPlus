//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlTask
#define ATLAS_AtlTask
#include <fstream>
#ifndef ROOT_TTask
#include <TTask.h>
#endif

class TObjString;
class TString;
class TList;
class TSystem;

class AtlTask : public TTask {
    
  protected:
    Bool_t       fInteractiveJob;   // Interactive job execution
    Bool_t       fBatchJob;         // Batch job execution
    Bool_t       fNAFBatchJob;      // NAF Batch job execution
    Bool_t       fGridJob;          // Grid job execution
    Bool_t       fLogFile;          // Write output to logfile
    TList       *fInputFiles;       // Input file names
    TList       *fInputEntryLists;  // Input entry lists file names
    TString     *fOutputFileName;   // Name of the output file
    TNamed      *fOutputTreeName;   // Name and title of the output tree (if any)
    Long64_t     fNEvents;          // No. of events to process
    Long64_t     fFirstEntry;       // Skip FirstEntry-1 events
    Bool_t       fIsTest;           // Is test job ?
    Int_t        fPriority;			// Priority of Job submission in condor
    TString     *fJobHome;          // Job working directory
    TString     *fLogFilePath;      // Logfile path
    TString     *fRunScript;        // Runscript path
    Char_t       fNode;             // Node on which to run the jpb
    TString     *fTempOutputPath;   // Temporarily output path (must start with "/")
    TString     *fTempLogFilePath;  // Temporarily logfile path
    TString     *fTempOutputFileName; // Temporarily output file name
    
    Bool_t   fBatchNodeAll;         // Submit batch job to all nodes
    Bool_t   fBatchNodeAutomatic;   // Submit batch job to node automatically set
    TString  fBatchNode;            // Node on which to run the job
    
    TString     *fGridRootVersion;  // Root version for Grid submission
    TString     *fGridCmtVersion;   // Cmt version for Grid submission
    TString     *fGridUserName;     // Grid user name
    TString     *fGridSuffix;       // Suffix for grid dataset names
    TString      fGridIdSuffix;     // Suffix for middle part of grid dataset names

    Int_t    fDebug;                // Debug flag for derived tasks
    Int_t    fDebugBuild;           // Debug flag for derived tasks (during build)

  public:
    AtlTask(const char*name, const char*title);
    virtual ~AtlTask();
    void AddInputFiles(const char* InputFiles);
    void AddInputEntryLists(const char* InputFile);
    void SetOutputFile(const char* OutputFile);
    void SetTempOutputFile(const char* OutputFile);
    void SetTempOutputPath(const char* OutputPath);
    void SetOutputTree(const char* name, const char* title); // *MENU*    
    virtual void CreateJobHome();
    Char_t GetBatchNodeFromPath(const char* path);
    virtual void Exec(Option_t *option = "");
    virtual void ExecInteractiveJob(Option_t *option) = 0;
    virtual void ExecGridJob(Option_t *option) = 0;
    virtual Bool_t ExecBatchJob(Option_t *option) = 0;
    virtual void ExecNAFBatchJob(Option_t *option) = 0;
    virtual void CreateRunScript(Option_t *option = "") = 0;
    virtual void CreateNAFBatchRunScript() = 0;
    virtual void CreateGridRunScript() = 0;
    void CreateRunScriptPath();
    void CreateLogFilePath();
    void CreateOutFilePath();
    void SetJobHome(const char* JobHome); // *MENU*
    virtual Bool_t WriteEnvSetup(std::ofstream & out) const;
    virtual Bool_t SubmitBatchJob();
    virtual void SubmitGridJob();
    virtual void SubmitNAFBatchJob();
    
    void SetBatchNodeAll(Bool_t BatchNodeAll);             // *TOGGLE*
    void SetBatchNodeAutomatic(Bool_t BatchNodeAutomatic); // *TOGGLE*
    void SetBatchNode(const char* BatchNode);              // *MENU*
    virtual void SetInteractiveJob(Bool_t InteractiveJob); // *TOGGLE*
    virtual void SetBatchJob(Bool_t BatchJob);             // *TOGGLE*
    virtual void SetNAFBatchJob(Bool_t NAFBatchJob);       // *TOGGLE*
    virtual void SetGridJob(Bool_t GridJob);               // *TOGGLE*
    void SetGridRootVersion(const char* RootVersion); // *MENU*
    void SetGridCmtVersion(const char* CmtVersion); // *MENU*
    void SetGridUserName(const char* UserName); // *MENU*
    void SetGridSuffix(const char* suffix); // *MENU*
    inline void SetGridIdSuffix(const char* suffix) { fGridIdSuffix = suffix; }
    
    static void CopyFolder(TDirectory *SrcDir, TDirectory *DestDir, Float_t scale);

    inline Bool_t GetBatchNodeAll()  { return fBatchNodeAll; }
    inline Bool_t GetBatchNodeAutomatic()   { return fBatchNodeAutomatic; }
    inline Bool_t GetInteractiveJob() { return fInteractiveJob; }
    inline Bool_t GetBatchJob()       { return fBatchJob; }
    inline Bool_t GetNAFBatchJob()    { return fNAFBatchJob; }
    inline Bool_t GetGridJob()        { return fGridJob; }
    inline Bool_t GetLogFile() { return fLogFile; }
    inline virtual void ExecuteTask(Option_t *option = "")
	{ TTask::ExecuteTask(option); } // *MENU*
    inline void SetNEvents(Int_t NEvents) { fNEvents = NEvents; } // *MENU*
    inline void SetFirstEntry(Int_t FirstEntry) { fFirstEntry = FirstEntry; } // *MENU*
    inline void SetLogFile(Bool_t LogFile) { fLogFile = LogFile; } // *TOGGLE*

    inline void SetDebug(Int_t level) { fDebug = level; } // *MENU*
    inline void SetDebugBuild(Int_t level) { fDebugBuild = level; }

    inline void SetPriority(Int_t Prio){
        	fPriority = Prio;
        }

    ClassDef(AtlTask,0) // Customised analysis task class
};
#endif


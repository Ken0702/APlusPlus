//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlAppAnalysisTask
#define ATLAS_AtlAppAnalysisTask
#ifndef ATLAS_AtlTask
#include <AtlTask.h>
#endif
#ifndef ATLAS_AtlToolCut
#include <AtlToolCut.h>
#endif
#ifndef ATLAS_AtlCut
#include <AtlCut.h>
#endif
#ifndef ROOT_TDataMember
#include <TDataMember.h>
#endif

class TChain;
class TSystem;
class AtlSelector;
class TClass;
class TDataMember;
class TROOT;
class AtlAnalysisTool;

class AtlAppAnalysisTask : public AtlTask {

protected:
    TString     *fInputTreeName;           // Name of the input tree
    TString     *fSelector;                // Name of Analysis selector
    Bool_t       fCtrlPlots;               // Switch on/off filling of ctrl plots
    TString     *fRootScript;              // Path to Root script used for job submission
    TList       *fListOfCuts;              // List of cut objects
    TList       *fListOfSubselectionCuts;  // List of subselection objects
    TList       *fListOfToolCuts;          // List of cuts for analysis tools
    TList       *fListOfUserEnvs;          // List of user environment variables
    TList       *fListOfTools;             // List of tools used in the analysis
    Bool_t       fWriteEntryList;          // Switch for writing an entry list
    Bool_t       fLoadEntryList;           // Switch for reading from entry list
    Bool_t       fUseAntiList;             // Switch for excluding events stored in an entry list
    Int_t       fNProcessNthEventsOnly;    // process only every Nth event (default=1 every event)
    Bool_t       fPrintEvent;              // Switch for printing every event
    Bool_t       fPrintObjectTable;        // Switch for printing Root's object table
    Bool_t       fUseAppInput;             // Switch to show App output is used as input
    
    TString     *fGridUser;                // Grid user name
    TString      fGridAnalysisName;        // Name of analysis run on the grid
    Bool_t       fGridSubmit;              // Do submission (or just produce scripts)
    Bool_t       fGridUsePrecompiledApp;   // Use a precompiled A++ version in APlusPlus.tgz
    Bool_t       fGridTemplateOnly;        // Only create template, not whole tree
    TString     *fAthenaTag;               // Athena version used for grid analysis
    TString     *fExcludedSites;           // Excluded grid sites (separated by comma)
    TString     *fSingleSite;              // Single site for job
    
    TString     *fReaderClass;             // user-defined reader class
    TString     *fReaderArgs;              // additional arguments to be passed to the reader's constructor
    Float_t      fXsection;                // sample cross section for BDT evt weight norm.
    Int_t        fEvtReaderMCWeightPosition; // mc weight position for evt reader
    Double_t     fEvtReaderMCWeightTotalEvents; // totalEventsWeighted for evt reader
    TString      fEvtReaderMCWeightPositionString;    // positions for multiple weights
    TString      fEvtReaderMCWeightTotalEventsString; // total events for multiple weights



public:
    AtlAppAnalysisTask(const char* name, const char* title);
    virtual ~AtlAppAnalysisTask();
    void CheckInputChain();
    virtual void InitSelector() {;}
    void SetInputTreeName(const char* TreeName);
    void SetSelector(const char* Selector);
    virtual void ExecInteractiveJob(Option_t *option);
    virtual Bool_t ExecBatchJob(Option_t *option);
    virtual void ExecNAFBatchJob(Option_t *option);
    virtual void ExecGridJob(Option_t *option);
    virtual void CreateRootScript(Option_t *option);
    virtual void CreateNAFBatchRootScript(Option_t *option);
    virtual void CreateRunScript(Option_t *option = "");
    virtual void CreateNAFBatchRunScript();
    virtual void CreateGridRunScript();
    virtual void CreateGridRunScript_old();
    virtual void SubmitGridJob();
    virtual void Print(Option_t *option = "") const; // *MENU*
    void SetCut(const char* var, const char* val);
    void SetToolCut(const char* tool, const char* var, const char* val);
    AtlToolCut* FindToolCut(const char* tool, const char* var);
    AtlCut*     FindCut(const char* var);
    void AddUserEnv(const char* env); // *MENU*

    void SetEvtReader(char const * readerClass, char const * readerArgs = 0);
    inline void AddCuts( TList* cuts ) { fListOfCuts->AddAll(cuts); }
    inline void AddSubselectionCuts( TList* subselectionCuts ) { fListOfSubselectionCuts->AddAll(subselectionCuts); }
    inline void AddTools( TList* tools ) { fListOfTools->AddAll(tools); }
    inline void AddToolCuts( TList* toolcuts ) { fListOfToolCuts->AddAll(toolcuts); }
    inline void AddUserEnvs( TList* envs ) { fListOfUserEnvs->AddAll(envs); }
    
    inline void AddTool(AtlAnalysisTool *tool) {
	// Add analysis tool
	fListOfTools->Add((TObject*)tool);
    }
    inline Bool_t GetCtrlPlots() { return fCtrlPlots; }
    inline virtual void SetCtrlPlots(Bool_t CtrlPlots)
	{ fCtrlPlots = CtrlPlots; } // *TOGGLE*
    inline TString* GetSelector() { return fSelector; }
    inline TList* GetListOfCuts() { return fListOfCuts; }
    inline TList* GetListOfSubselectionCuts() { return fListOfSubselectionCuts; }
    inline TList* GetListOfToolCuts() { return fListOfToolCuts; }
    inline Bool_t GetWriteEntryList() const { return fWriteEntryList; }
    inline Bool_t GetLoadEntryList() const { return fLoadEntryList; }
    inline Bool_t GetUseAntiList() const { return fUseAntiList; }
    inline Bool_t GetPrintEvent() const { return fPrintEvent; }
    inline Bool_t GetPrintObjectTable() const { return fPrintObjectTable; }

    inline void SetToolVariable(const char* tool, const char* var, const char* val) {
	//
	// Set public data member of a tool (same as SetToolCut())
	//
	//
	// CAUTION!
	//
	// First argument "tool" must be the class name of the tool!
	//
	SetToolCut(tool, var, val);
    }

    inline void SetWriteEntryList(Bool_t WriteEntryList)
    { fWriteEntryList = WriteEntryList; } // *TOGGLE*
    inline void SetLoadEntryList(Bool_t LoadEntryList)
    { fLoadEntryList = LoadEntryList; } // *TOGGLE*
    inline void SetUseAntiList(Bool_t UseAntiList)
    { fUseAntiList = UseAntiList; } // *TOGGLE*
    inline void SetPrintEvent(Bool_t PrintEvent)
    { fPrintEvent = PrintEvent; } // *TOGGLE*
    inline void SetPrintObjectTable(Bool_t PrintObjectTable)
    { fPrintObjectTable = PrintObjectTable; } // *TOGGLE*
    inline void SetNProcessNthEventsOnly(Int_t N) { fNProcessNthEventsOnly = N; }
    inline void SetXsection(Float_t xsec) { fXsection = xsec; }

    inline void SetGridUser(TString *GridUser)
    { fGridUser = GridUser; }
    inline void SetGridAnalysisName(TString GridAnalysisName)
        { fGridAnalysisName = GridAnalysisName; }
    inline void SetAthenaTag(TString *AthenaTag)
    { fAthenaTag = AthenaTag; }
    inline void SetExcludedSites(TString *ExcludedSites)
    { fExcludedSites = ExcludedSites; }
    inline void SetSingleSite(TString *SingleSite)
    { fSingleSite = SingleSite; }
    inline void SetGridSubmit(Bool_t sub = kTRUE) { fGridSubmit = sub; }
    inline void SetGridTemplateOnly(Bool_t g = kTRUE) { fGridTemplateOnly = g; }
    inline void SetUseAppInput(Bool_t use = kTRUE) { fUseAppInput = use; }
    inline void SetEvtReaderMCWeightPosition(Int_t pos) {
        fEvtReaderMCWeightPosition = pos;
    }
    inline void SetEvtReaderMCWeightTotalEvents(Double_t tot) {
        fEvtReaderMCWeightTotalEvents = tot;
    }
    inline void SetEvtReaderMCWeightPositionString(TString pos) {
        fEvtReaderMCWeightPositionString = pos;
    }
    inline void SetEvtReaderMCWeightTotalEventsString(TString tot) {
        fEvtReaderMCWeightTotalEventsString = tot;
    }


  private:
    TDataMember* FindDataMember(TClass *cl, const char* DMName) const;
    
    ClassDef(AtlAppAnalysisTask,0) // A++ analysis task
};
#endif


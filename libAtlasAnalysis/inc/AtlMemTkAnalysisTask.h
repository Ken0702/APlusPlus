//  
// Author: stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2013 (C) stamm
//
#ifndef ATLAS_AtlMemTkAnalysisTask
#define ATLAS_AtlMemTkAnalysisTask
#ifndef ATLAS_AtlTask
#include <AtlTask.h>
#endif

class AtlMemTkAnalysisTask : public AtlTask {

private:
    TString *fRootScript;
    TString *fMemTkRunScript;
//    TString fGridHomeSetupScript;
    TList   *fListOfUserEnvs;
    Int_t    fNSubjobs;
    Int_t    fJobid;
    TString  fD3PDVersion;
    Bool_t   fTarBallOnly;
    Bool_t   fMemTkTreatAllJetsAsBJets;
    Bool_t   fGridAnalysisOnGrid;
    Bool_t   fGridUsePrecompiledMem;

public:
    AtlMemTkAnalysisTask(const char* name  = "MemTkAnalysisTask",
			 const char* title = "MemTkAnalysisTask");
    virtual ~AtlMemTkAnalysisTask();
    virtual void ExecInteractiveJob(Option_t *option);
    virtual Bool_t ExecBatchJob(Option_t *option);
    virtual void ExecGridJob(Option_t *option);
    virtual void CreateRunScript(Option_t *option = "");
    virtual void ExecNAFBatchJob(const Option_t*) {;} // not supported
    virtual void CreateNAFBatchRunScript() {;} // not supported
    virtual void CreateGridRunScript();
    virtual void SubmitGridJob();
    void AddUserEnv(const char* env);
    virtual void Print(Option_t *option = "") const;
    virtual void SetMemTkRunScript(const char* script);
//    inline void SetGridHomeSetupScript(const char* script){ fGridHomeSetupScript = script; }

    inline void AddUserEnvs( TList* envs ) { fListOfUserEnvs->AddAll(envs); }
    inline void SetNSubjobs(Int_t njobs) { fNSubjobs = njobs; }
    inline void SetJobId(Int_t jobid) { fJobid = jobid; }
    inline void SetD3PDVersion(const char* D3PDVersion) { fD3PDVersion = TString(D3PDVersion); }
    inline void SetTarBallOnly(Bool_t tarballonly = kTRUE) { fTarBallOnly = tarballonly; }
    inline void SetMemTkTreatAllJetsAsBJets(Bool_t b = kTRUE) { fMemTkTreatAllJetsAsBJets = b; }
    inline void SetGridAnalysisOnGrid(Bool_t ongrid = kTRUE) {
      fGridAnalysisOnGrid = ongrid;
    }
    
    ClassDef(AtlMemTkAnalysisTask,0) // Atl MemTk Analysis Task
};
#endif


//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2013 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHforSplittingTask
#define ATLAS_AtlHforSplittingTask
#ifndef ROOT_TTask
#include <TTask.h>
#endif
#ifndef ROOT_TList
#include <TList.h>
#endif
#ifndef ATLAS_AtlSample
#include <AtlSample.h>
#endif
#ifndef ATLAS_AtlTask
#include <AtlTask.h>
#endif

class AtlHforSplittingTask : public AtlTask {
    
  protected:
    TString     *fInputTreeName;    // Name of the input tree
    TString     *fRootScript;       // Path to Root script used f
    TList       *fListOfUserEnvs;   // List of user environment variables
    
    
  public:
    AtlHforSplittingTask(const char* name, const char* title);
    virtual ~AtlHforSplittingTask();
    
    void SetInputTreeName(const char* TreeName);
    void CreateRootScript(Option_t *option);
    virtual void CreateRunScript(Option_t *option = "");
    virtual void ExecInteractiveJob(Option_t *option);
    virtual Bool_t ExecBatchJob(Option_t *option);
    void AddUserEnv(const char* env);

    virtual void ExecGridJob(const Option_t*);
    virtual void ExecNAFBatchJob(const Option_t*);
    virtual void CreateNAFBatchRunScript();
    virtual void CreateGridRunScript();
    static void HforSplit(const char* InputFile, const char* TreeName );
    
    ClassDef(AtlHforSplittingTask,0) // Hfor Splitting task
};
#endif

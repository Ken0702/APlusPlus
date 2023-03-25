//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2013 (C) Soeren Stamm
//
#ifndef ATLAS_AtlMergingTask
#define ATLAS_AtlMergingTask
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

class AtlMergingTask : public AtlTask {
    
  public:
    AtlMergingTask(const char* name, const char* title);
    virtual ~AtlMergingTask();
    
    virtual void CreateRunScript(Option_t *option = "");
    virtual void ExecInteractiveJob(Option_t *option);
    virtual Bool_t ExecBatchJob(Option_t *option);
    virtual void ExecGridJob(const Option_t*);
    virtual void ExecNAFBatchJob(const Option_t*);
    virtual void CreateNAFBatchRunScript();
    virtual void CreateGridRunScript();
    
    ClassDef(AtlMergingTask,0) // A++ Merging task
};
#endif

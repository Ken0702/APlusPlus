//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactoryYieldTableTask
#define ATLAS_AtlHistFactoryYieldTableTask
#ifndef ROOT_AtlHistFactoryTask
#include <AtlHistFactoryTask.h>
#endif

class TFile;

class AtlHistFactoryYieldTableTask : public AtlHistFactoryTask {

protected:
    TFile *fModelFile;  //! Pointer to Model File
    
public:
    AtlHistFactoryYieldTableTask(const char* name, const char* title);
    virtual ~AtlHistFactoryYieldTableTask();
    virtual void ExecInteractiveJob(Option_t *option);
    virtual void Initialize();
    
protected:
    void PrintYieldTable();

    ClassDef(AtlHistFactoryYieldTableTask,0) // DOCUMENT ME
};
#endif


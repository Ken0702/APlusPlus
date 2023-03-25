//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2014 (C) Soeren Stamm
//
#ifndef ATLAS_AtlBDTAnalysisTask
#define ATLAS_AtlBDTAnalysisTask
#ifndef ATLAS_AtlAppAnalysisTask
#include <AtlAppAnalysisTask.h>
#endif

class AtlBDTAnalysisTask : public AtlAppAnalysisTask {

public:
    AtlBDTAnalysisTask(const char* name, const char* title);
    virtual ~AtlBDTAnalysisTask();
    virtual void CreateRunScript(Option_t *option = "");

    ClassDef(AtlBDTAnalysisTask,0) // A++ BDT analysis task
};
#endif


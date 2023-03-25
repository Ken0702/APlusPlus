//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactoryBreakdownTask
#define ATLAS_AtlHistFactoryBreakdownTask
#ifndef ROOT_TTask
#include <TTask.h>
#endif

class AtlHistFactoryMeasurement;

class AtlHistFactoryBreakdownTask : public TTask {

private:
    AtlHistFactoryMeasurement *fMeasurement;

    
public:
    AtlHistFactoryBreakdownTask(const char* name, const char* title,
				AtlHistFactoryMeasurement *meas);
    virtual ~AtlHistFactoryBreakdownTask();

private:
    void Initialize();
    
    ClassDef(AtlHistFactoryBreakdownTask,0) // DOCUMENT ME
};
#endif


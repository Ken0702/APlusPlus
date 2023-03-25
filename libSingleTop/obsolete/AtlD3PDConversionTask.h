//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef SINGLETOP_AtlD3PDConversionTask
#define SINGLETOP_AtlD3PDConversionTask
#ifndef ATLAS_AtlTask
#include <AtlTask.h>
#endif

class AtlSingleTopD3PDConverter;

class AtlD3PDConversionTask : public AtlTask {

private:
    
public:
    AtlD3PDConversionTask(const char*name, const char*title);
    virtual ~AtlD3PDConversionTask();
    virtual void InitSelector();

    inline virtual void SetCtrlPlots(Bool_t CtrlPlots) {
	// Switch off ctrl plots in any case for the converter
	fCtrlPlots = kFALSE;
    }
    
    ClassDef(AtlD3PDConversionTask,0) // Single-top D3PD conversion task
};
#endif


//____________________________________________________________________
//
// Single-Top Entry List Task for Electron Channel
// 
//  
// Author: Ruth Herrberg <mailto: ruthh@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef SINGLETOP_AtlSgTopEntryListTask_e
#define SINGLETOP_AtlSgTopEntryListTask_e
#ifndef ATLAS_AtlAppAnalysisTask
#include <AtlAppAnalysisTask.h>
#endif
#include <AtlSgTopEntryListWriter.h>

class AtlSgTopEntryListTask_e : public AtlAppAnalysisTask {
    
public:
    AtlSgTopEntryListTask_e(const char* name, const char* title);
    virtual ~AtlSgTopEntryListTask_e();
    virtual void InitSelector();
    
    ClassDef(AtlSgTopEntryListTask_e,0) // Single-top event list task
};
#endif

#ifndef __CINT__
ClassImp(AtlSgTopEntryListTask_e);
#endif

//____________________________________________________________________

AtlSgTopEntryListTask_e::AtlSgTopEntryListTask_e(const char* name,
				   const char* title) :
    AtlAppAnalysisTask(name, title) {
    //
    // Normal constructor
    //
    SetInputTreeName("t_app");
    AddUserEnv("LIBSINGLETOP");
}

//____________________________________________________________________

AtlSgTopEntryListTask_e::~AtlSgTopEntryListTask_e() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlSgTopEntryListTask_e::InitSelector() {
    //
    // Initialise the t-channel analysis selector
    //
    if ( fGridJob ) {
	fSelector = new AtlSgTopEntryListWriter(fGridAnalysisName->Data());
    } else {
	fSelector = new AtlSgTopEntryListWriter(fOutputFileName->Data());
    }

    // =================
    // Trigger selection  
    // =================
    SetWriteEntryList(kTRUE); // *** CAUTION! The AtlAppAnalysisTask configures the selector and writes the analysis root script ***
//    fSelector->SetWriteEntryList(kTRUE);  // *** it's no use configuring the selector here.... ***
    SetCut("fTriggerChoice","\"EF_e20_medium\""); 
    
}

//____________________________________________________________________

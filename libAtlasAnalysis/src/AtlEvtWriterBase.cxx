//____________________________________________________________________
//
// Abstract base class for writing events with the AtlSelector
// 

//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2013 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtWriterBase
#include <AtlEvtWriterBase.h>
#endif

#ifndef __CINT__
ClassImp(AtlEvtWriterBase);
#endif

//____________________________________________________________________

AtlEvtWriterBase::AtlEvtWriterBase() {
    //
    // Default constructor
    //
    fTree = 0;
    fEvent = 0;
}

//____________________________________________________________________

AtlEvtWriterBase::~AtlEvtWriterBase() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

TTree* AtlEvtWriterBase::CreateTree(const char* name, const char* title,
				    TTree *InputTree) {
    //
    // Create output tree
    //
    fTree = new TTree(name, title);
    fTree->SetAutoSave(2000000000); // autosave each 2GB
    fTree->SetBranchStyle(1); // use new branch style (bronch)
    return fTree;
}



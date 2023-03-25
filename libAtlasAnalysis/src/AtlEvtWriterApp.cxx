//____________________________________________________________________
//
// Event writer for A++ events used by AtlSelector
// 
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2013 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtWriterApp
#include <AtlEvtWriterApp.h>
#endif

#ifndef __CINT__
ClassImp(AtlEvtWriterApp);
#endif

//____________________________________________________________________

AtlEvtWriterApp::AtlEvtWriterApp() {
    //
    // Default constructor
    //
}

//____________________________________________________________________

AtlEvtWriterApp::~AtlEvtWriterApp() {
    //
    // Default denstructor
    //
}

//____________________________________________________________________

void AtlEvtWriterApp::BookTree(TTree *tree, AtlEvent *evt) {
    //
    // Book A++ output tree
    //
    fTree  = tree;
    fEvent = evt;
    
    // Create superbranch
    TBranch *b_evt = fTree->Branch("Event", "AtlEvent",
				   &fEvent);
    // Don't re-create event in event loop
    b_evt->SetAutoDelete(kFALSE);
    Info("BookTree",
	 "Created output A++ tree, name=\"%s\" title=\"%s\"",
	 fTree->GetName(), fTree->GetTitle());
}
    
//____________________________________________________________________

void AtlEvtWriterApp::WriteEvent() {
    //
    // Write event
    //
    fTree->Fill();
}



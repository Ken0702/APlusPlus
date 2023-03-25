//____________________________________________________________________
//
// Event writer for SgTop-D3PDs used by AtlSelector
// 
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2013 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtWriterD3PDSgTop
#include <AtlEvtWriterD3PDSgTop.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlEvtWriterD3PDSgTop);
#endif

//____________________________________________________________________

AtlEvtWriterD3PDSgTop::AtlEvtWriterD3PDSgTop() {
    //
    // Default constructor
    //
    fCloneOnly = kFALSE;
}

//____________________________________________________________________

AtlEvtWriterD3PDSgTop::~AtlEvtWriterD3PDSgTop() {
    //
    // Default constructor
    //
}

//____________________________________________________________________

void AtlEvtWriterD3PDSgTop::BookTree(TTree *tree, AtlEvent *evt) {
    //
    // Book output SgTopD3PD
    //
    fTree  = tree;
    fEvent = evt;

    // ===============
    // Create branches
    // ===============

    // Evt header
    if ( fCloneOnly ) {
        Info(__FUNCTION__, "Not adding branch \"evt_weight\" (clone only)");
    } else {
        fTree->Branch("evt_weight", &fEventWeight, "evt_weight/F");
    }
}

//____________________________________________________________________

void AtlEvtWriterD3PDSgTop::WriteEvent() {
    //
    // Write event
    //
    fEventWeight = fEvent->GetTagEvtWeight();
    fTree->Fill();
}

//____________________________________________________________________

TTree* AtlEvtWriterD3PDSgTop::CreateTree(const char* name,
                                         const char* title,
					 TTree *InputTree) {
    //
    // Create output tree
    //
    InputTree->SetBranchStatus("*", kTRUE);
    fTree = InputTree->CloneTree(0);
    if ( fTree == 0 ) {
	// TTree::CloneTree() cannot cope with empty trees. Bypass by
	// using TTree::CopyTree()
    	fTree = InputTree->CopyTree("","");
    }
    fTree->SetNameTitle(name, title);
    return fTree;
}

//____________________________________________________________________
//
// Event writer for SgTop-D3PDs used by AtlSelector, changed to
// only write out MEM-produced branches, based on AtlEvtWriterD3PDSgTop
// 
//  
// Author: Stephan Kaphle <mailto: kaphle@physik.hu-berlin.de>
// Update: $ $
// Copyright: 2013 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtWriterMemR2
#include <AtlEvtWriterMemR2.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlEvtWriterMemR2);
#endif

//____________________________________________________________________

AtlEvtWriterMemR2::AtlEvtWriterMemR2() {
    //
    // Default constructor
    //
    fCloneOnly = kFALSE;
    fReduceOutput = kFALSE;
}

//____________________________________________________________________

AtlEvtWriterMemR2::~AtlEvtWriterMemR2() {
    //
    // Default constructor
    //
}

//____________________________________________________________________

void AtlEvtWriterMemR2::BookTree(TTree *tree, AtlEvent *evt) {
    //
    // Book output SgTopD3PD
    //
    fTree  = tree;
    fEvent = evt;

    if ( fCloneOnly ) {
        Info(__FUNCTION__, "Not booking any branches (clone only)");
        return;
    }

    // Book evt_weight, recreate branch after removing in CreateTree()
    fTree->Branch("evt_weight", &fEventWeight, "evt_weight/F");

    // Add MemDisc branches
    fTree->Branch("MemDisc_sChannelRatio", &fMemDisc_sChannelRatio,
                  "MemDisc_sChannelRatio/D");
    fTree->Branch("MemDisc_wjetsRatio", &fMemDisc_wjetsRatio,
                  "MemDisc_wjetsRatio/D");
    fTree->Branch("MemDisc_ttbarRatio", &fMemDisc_ttbarRatio,
                  "MemDisc_ttbarRatio/D");
}

//____________________________________________________________________

void AtlEvtWriterMemR2::WriteEvent() {
    //
    // Write event
    //
    fEventWeight = fEvent->GetTagEvtWeight();
    fTree->Fill();
}

//____________________________________________________________________

TTree* AtlEvtWriterMemR2::CreateTree(const char* name,
                                     const char* title,
                                     TTree *InputTree) {
    //
    // Create output tree
    //
    InputTree->SetBranchStatus("*", kTRUE);
    if ( !fCloneOnly ) {
        // turn off copying to recreate the branch
        Info(__FUNCTION__, "Branch evt_weight will be recreated");
        InputTree->SetBranchStatus("evt_weight", kFALSE);
    }
    if ( fReduceOutput ) {
        InputTree->Show(); // S.K. okay I have no idea why, but if I don't insert this line, the deactivation and activation of branches does not work in sequence but gets mangled and I fail an assertion
        InputTree->SetBranchStatus("*", kFALSE);
        InputTree->SetBranchStatus("llh*", kTRUE);
    }
    fTree = InputTree->CloneTree(0);
    if ( fTree == 0 ) {
	// TTree::CloneTree() cannot cope with empty trees. Bypass by
	// using TTree::CopyTree()
        Info(__FUNCTION__, "InputTree is empty; copy instead of cloning");
    	fTree = InputTree->CopyTree("","");
    }
    if ( fReduceOutput ) {
        InputTree->SetBranchStatus("*", kTRUE); // turn active after cloning again
    }
    fTree->SetNameTitle(name, title);
    return fTree;
}

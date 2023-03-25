//____________________________________________________________________
//
// Event reader for slimmed Top D3PDs used by AtlSelector
// 
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2011 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtReaderMem
#include <AtlEvtReaderMem.h>
#endif
#include <AtlEventHeader.h>
#include <HepVertex.h>
#include <iostream>
#include <vector>
#include <TTree.h>

using namespace std;

#ifndef __CINT__
ClassImp(AtlEvtReaderMem);
#endif

//____________________________________________________________________

AtlEvtReaderMem::AtlEvtReaderMem(AtlSelector *parent) :
    AtlEvtReaderD3PDSgTop(parent) {
    //
    // Default constructor
    //
}

//____________________________________________________________________

AtlEvtReaderMem::~AtlEvtReaderMem() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlEvtReaderMem::SetBranches(TTree *t) {
    //
    // Set branch assignments of the input D3PD tree.
    //
    // Note that at this stage methods like TTree::FindBranch()
    // etc. do not work because the tree is not yet loaded from the
    // input file(s) and thus its structure not really known. For this
    // the Notify() function can be used.
    //
    AtlEvtReaderD3PDSgTop::SetBranches(t);
    
    // ====================
    // Set branch addresses
    // ====================
	
    // Event header
    SetupBranch(t, "llh_sChannel2j",  &v_llh_sChannel2j,  &b_llh_sChannel2j);
    SetupBranch(t, "llh_sChannel3j",  &v_llh_sChannel3j,  &b_llh_sChannel3j);
    SetupBranch(t, "llh_tChannel4FS", &v_llh_tChannel4FS, &b_llh_tChannel4FS);
    SetupBranch(t, "llh_tChannel5FS", &v_llh_tChannel5FS, &b_llh_tChannel5FS);
    SetupBranch(t, "llh_ttbarSL",     &v_llh_ttbarSL,     &b_llh_ttbarSL);
    SetupBranch(t, "llh_ttbarDL",     &v_llh_ttbarDL,     &b_llh_ttbarDL);
    SetupBranch(t, "llh_Wjj",         &v_llh_Wjj,         &b_llh_Wjj);
    SetupBranch(t, "llh_Wcj",         &v_llh_Wcj,         &b_llh_Wcj);
    SetupBranch(t, "llh_Wbb",         &v_llh_Wbb,         &b_llh_Wbb);
}

//____________________________________________________________________

Bool_t AtlEvtReaderMem::Notify() {
    //
    // This method is called whenever a new input file is opened
    //

    Bool_t notifySgTop = AtlEvtReaderD3PDSgTop::Notify();
    return (kTRUE & notifySgTop);
}

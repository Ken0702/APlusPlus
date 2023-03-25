//____________________________________________________________________
//
// AtlEvtReaderD3PD base class
//
// Abstract base class for the D3PD event readers
// (TopPair, TopSlim, etc.)
//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2012 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtReaderD3PDBase
#include <AtlEvtReaderD3PDBase.h>
#endif

#ifndef __CINT__
ClassImp(AtlEvtReaderD3PDBase);
#endif

//____________________________________________________________________

AtlEvtReaderD3PDBase::AtlEvtReaderD3PDBase(AtlSelector *parent) :
    AtlEvtReaderBase(parent) {
    //
    // Default constructor
    //
    fRunLast = -999;
    fIsMC = kFALSE;
}

//____________________________________________________________________

AtlEvtReaderD3PDBase::~AtlEvtReaderD3PDBase() {
    //
    // Default denstructor
    //
}

//____________________________________________________________________

void AtlEvtReaderD3PDBase::InitBranches(TTree *t) {
    //
    // Helper function for SetBranchStatus()
    //
    if ( gDebug > 0 )
        Info(__FUNCTION__, "SetMakeClass and SetBranchStatus off");
    t->SetMakeClass(1);
    t->SetBranchStatus("*", kFALSE);
    InitObjPointers();
}

//____________________________________________________________________

void AtlEvtReaderD3PDBase::SetupBranch(TTree *t, const char* branchname,
				       void* value, TBranch** branch) {
    //
    // Set branch assignment and set branch status to true
    //
    t->SetBranchStatus(branchname, kTRUE);
    t->SetBranchAddress(branchname, value, branch);
}

//____________________________________________________________________

Int_t AtlEvtReaderD3PDBase::GetEntry(TTree *t, Long64_t entry) {
    //
    // Get entry from current input tree. This (virtual) method can be
    // overloaded if needed (different type of input tree)
    //
    fEvent->Clear();

    Int_t evtsize = t->GetEntry(entry);

    // For empty trees, TSelector/TTreePlayer still calls GetEntry().
    // In this case, we should not build the event.
    if ( IsFirstEvent() && (evtsize == 0) ) {
        Warning(__FUNCTION__, "0 bytes read for entry %lld, skip building event.",
                entry);
        return 0;
    }

    if ( gDebug > 1 ) {
        Info(__FUNCTION__, "%d bytes read for entry %lld", evtsize, entry);
        t->Show(entry);
        Info(__FUNCTION__, "Running BuildEvent()");
    }

    BuildEvent();

    fRunLast = fEvent->RunNr();

    return evtsize;
}

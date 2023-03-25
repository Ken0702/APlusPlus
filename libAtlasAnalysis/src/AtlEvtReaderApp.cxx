//____________________________________________________________________
//
// Event reader for A++ events used by AtlSelector
// 
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2011 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtReaderApp
#include <AtlEvtReaderApp.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlEvtReaderApp);
#endif

//____________________________________________________________________

AtlEvtReaderApp::AtlEvtReaderApp(AtlSelector *parent) :
    AtlEvtReaderBase(parent) {
    //
    // Default constructor
    //
}

//____________________________________________________________________

AtlEvtReaderApp::~AtlEvtReaderApp() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlEvtReaderApp::SetBranches(TTree *t) {
    //
    // Set branch assignments of the input tree. Here an A++ input
    // tree is assumed. For other trees this (virtual) method has to
    // be overloaded. See also the GetEntry() method
    //
    t->SetBranchAddress("Event", &fEvent);
}

//____________________________________________________________________

Int_t AtlEvtReaderApp::GetEntry(TTree *t, Long64_t entry) {
    //
    // Get entry from current input tree. This (virtual) method can be
    // overloaded if needed (different type of input tree)
    //
    return t->GetEntry(entry);
}



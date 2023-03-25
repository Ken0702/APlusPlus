//____________________________________________________________________
//
// Event reader for slimmed Top D3PDs used by AtlSelector (run 2)
// 
// Author: Stephan Kaphle <mailto: kaphle@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2011 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtReaderMemR2JetLepton
#include <AtlEvtReaderMemR2JetLepton.h>
#endif
#include <AtlEventHeader.h>
#include <HepVertex.h>
#include <iostream>
#include <vector>
#include <TTree.h>

using namespace std;

#ifndef __CINT__
ClassImp(AtlEvtReaderMemR2JetLepton);
#endif

//____________________________________________________________________

AtlEvtReaderMemR2JetLepton::AtlEvtReaderMemR2JetLepton(AtlSelector *parent,
                                     EDataType dataType,
                                     const char * D3PDversion,
                                     char const * systematicName,
                                     char const * treeName,
									 Int_t mc_weight_number ) : //kkreul: It crashes without this.
    AtlEvtReaderD3PDJetLepton(parent, dataType, D3PDversion, systematicName,
                            "") {
    //
    // Default constructor
    //
    fTreeName = treeName;
}

//____________________________________________________________________

AtlEvtReaderMemR2JetLepton::~AtlEvtReaderMemR2JetLepton() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlEvtReaderMemR2JetLepton::ClearBranches() {
    //
    // Clear all tree branches
    //
    AtlEvtReaderD3PDJetLepton::ClearBranches();
    v_llh_sChannel2j = -2.;
    v_llh_sChannel3j = -2.;
    v_llh_tChannel4FS = -2.;
    v_llh_tChannel5FS = -2.;
    v_llh_ttbarSL = -2.;
    v_llh_ttbarDL = -2.;
    v_llh_Wjj = -2.;
    v_llh_Wcj = -2.;
    v_llh_Wbb = -2.;
}

//____________________________________________________________________

void AtlEvtReaderMemR2JetLepton::SetBranches(TTree *t) {
    //
    // Set branch assignments of the input D3PD tree.
    //
    // Note that at this stage methods like TTree::FindBranch()
    // etc. do not work because the tree is not yet loaded from the
    // input file(s) and thus its structure not really known. For this
    // the Notify() function can be used.
    //
    Info(__FUNCTION__, "Setting up branches for AtlEvtReaderD3PDJetLepton");
    AtlEvtReaderD3PDJetLepton::SetBranches(t);

    // ====================
    // Set branch addresses
    // ====================
    Info(__FUNCTION__, "Setting up llh branches");
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

Bool_t AtlEvtReaderMemR2JetLepton::Notify() {
    //
    // This method is called whenever a new input file is opened
    //

    Bool_t notifySgTop = AtlEvtReaderD3PDJetLepton::Notify();
    return (kTRUE & notifySgTop);
}

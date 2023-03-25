//____________________________________________________________________
//
// Event reader for slimmed Top D3PDs used by AtlSelector
// 
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2011 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtReaderMemBDT
#include <AtlEvtReaderMemBDT.h>
#endif
#include <AtlEventHeader.h>
#include <HepVertex.h>
#include <iostream>
#include <vector>
#include <TTree.h>


using namespace std;

#ifndef __CINT__
ClassImp(AtlEvtReaderMemBDT);
#endif

//____________________________________________________________________

AtlEvtReaderMemBDT::AtlEvtReaderMemBDT(AtlSelector *parent) :
    AtlEvtReaderD3PDSgTop(parent) {
    //
    // Default constructor
    //
}

//____________________________________________________________________

AtlEvtReaderMemBDT::~AtlEvtReaderMemBDT() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlEvtReaderMemBDT::SetBranches(TTree *t) {
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
    // bdt variables
    SetupBranch(t, "bdt_MtW",  &v_bdt_MtW,  &b_bdt_MtW);
    SetupBranch(t, "bdt_DeltaPhiTopBJet2_BJet1",  &v_bdt_DeltaPhiTopBJet2_BJet1,  &b_bdt_DeltaPhiTopBJet2_BJet1);
    SetupBranch(t, "bdt_DeltaPhiTopBJet1_BJet2",  &v_bdt_DeltaPhiTopBJet1_BJet2,  &b_bdt_DeltaPhiTopBJet1_BJet2);
    SetupBranch(t, "bdt_SumEtLep_MET",  &v_bdt_SumEtLep_MET,  &b_bdt_SumEtLep_MET);
    SetupBranch(t, "bdt_MET",  &v_bdt_MET,  &b_bdt_MET);
    SetupBranch(t, "bdt_SumEtLep_MET_BJets",  &v_bdt_SumEtLep_MET_BJets,  &b_bdt_SumEtLep_MET_BJets);
    SetupBranch(t, "bdt_Pt_Lep",  &v_bdt_Pt_Lep,  &b_bdt_Pt_Lep);
	SetupBranch(t, "bdt_SumPt",  &v_bdt_SumPt,  &b_bdt_SumPt);
    SetupBranch(t, "bdt_DeltaEtaLep_BJet1",  &v_bdt_DeltaEtaLep_BJet1,  &b_bdt_DeltaEtaLep_BJet1);
    SetupBranch(t, "bdt_DeltaPhiBJet1_BJet2",  &v_bdt_DeltaPhiBJet1_BJet2,  &b_bdt_DeltaPhiBJet1_BJet2);
    SetupBranch(t, "bdt_DeltaEtaLep_BJet2",  &v_bdt_DeltaEtaLep_BJet2,  &b_bdt_DeltaEtaLep_BJet2);
    SetupBranch(t, "bdt_CosLepW_WTopBJet1",  &v_bdt_CosLepW_WTopBJet1,  &b_bdt_CosLepW_WTopBJet1);
    SetupBranch(t, "bdt_DeltaPhiLep_MET",  &v_bdt_DeltaPhiLep_MET,  &b_bdt_DeltaPhiLep_MET);
    SetupBranch(t, "bdt_DeltaEtaTopBJet2_BJet1",  &v_bdt_DeltaEtaTopBJet2_BJet1,  &b_bdt_DeltaEtaTopBJet2_BJet1);
    SetupBranch(t, "bdt_CosLepTopBJet2_TopBJet2CMS",  &v_bdt_CosLepTopBJet2_TopBJet2CMS,  &b_bdt_CosLepTopBJet2_TopBJet2CMS);
    SetupBranch(t, "bdt_CosLepTopBJet1_TopBJet1CMS",  &v_bdt_CosLepTopBJet1_TopBJet1CMS,  &b_bdt_CosLepTopBJet1_TopBJet1CMS);
    SetupBranch(t, "bdt_MLep_BJet2",  &v_bdt_MLep_BJet2,  &b_bdt_MLep_BJet2);
    SetupBranch(t, "bdt_CosMET_BJet2",  &v_bdt_CosMET_BJet2,  &b_bdt_CosMET_BJet2);
    SetupBranch(t, "bdt_SumPtBJet1_BJet2",  &v_bdt_SumPtBJet1_BJet2,  &b_bdt_SumPtBJet1_BJet2);
    SetupBranch(t, "bdt_DeltaEtaNu_BJetNonTop",  &v_bdt_DeltaEtaNu_BJetNonTop,  &b_bdt_DeltaEtaNu_BJetNonTop);
    // mem likelihoods
    // SetupBranch(t, "llh_sChannel2j",  &v_llh_sChannel2j,  &b_llh_sChannel2j);
    // SetupBranch(t, "llh_sChannel3j",  &v_llh_sChannel3j,  &b_llh_sChannel3j);
    // SetupBranch(t, "llh_tChannel4FS", &v_llh_tChannel4FS, &b_llh_tChannel4FS);
    // SetupBranch(t, "llh_ttbarSL",     &v_llh_ttbarSL,     &b_llh_ttbarSL);
    // SetupBranch(t, "llh_Wjj",         &v_llh_Wjj,         &b_llh_Wjj);
    // SetupBranch(t, "llh_Wbb",         &v_llh_Wbb,         &b_llh_Wbb);
}

//____________________________________________________________________

Bool_t AtlEvtReaderMemBDT::Notify() {
    //
    // This method is called whenever a new input file is opened
    //

    Bool_t notifySgTop = AtlEvtReaderD3PDSgTop::Notify();
    return (kTRUE & notifySgTop);
}

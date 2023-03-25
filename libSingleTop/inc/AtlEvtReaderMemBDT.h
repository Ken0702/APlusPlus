//
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2014 (C) Soeren Stamm
//
#ifndef ATLAS_AtlEvtReaderMemBDT
#define ATLAS_AtlEvtReaderMemBDT
#ifndef ATLAS_AtlEvtReaderD3PDSgTop
#include <AtlEvtReaderD3PDSgTop.h>
#endif

class AtlEvtReaderMemBDT : public AtlEvtReaderD3PDSgTop {

  public:

    // =========================
    // Declaration of leaf types
    // =========================

    // Kinematic Variables
    Float_t        v_bdt_MtW;
    Float_t        v_bdt_DeltaPhiTopBJet2_BJet1;
    Float_t        v_bdt_DeltaPhiTopBJet1_BJet2;
    Float_t        v_bdt_SumEtLep_MET;
    Float_t        v_bdt_MET;
    Float_t        v_bdt_SumEtLep_MET_BJets;
    Float_t        v_bdt_Pt_Lep;
    Float_t        v_bdt_SumPt;
    Float_t        v_bdt_DeltaEtaLep_BJet1;
    Float_t        v_bdt_DeltaPhiBJet1_BJet2;
    Float_t        v_bdt_DeltaEtaLep_BJet2;
    Float_t        v_bdt_CosLepW_WTopBJet1;
    Float_t        v_bdt_DeltaPhiLep_MET;
    Float_t        v_bdt_DeltaEtaTopBJet2_BJet1;
    Float_t        v_bdt_CosLepTopBJet2_TopBJet2CMS;
    Float_t        v_bdt_CosLepTopBJet1_TopBJet1CMS;
    Float_t        v_bdt_MLep_BJet2;
    Float_t        v_bdt_CosMET_BJet2;
    Float_t        v_bdt_SumPtBJet1_BJet2;
    Float_t        v_bdt_DeltaEtaNu_BJetNonTop;

    // Kinematic branches
    TBranch        *b_bdt_MtW;   //!
    TBranch        *b_bdt_DeltaPhiTopBJet2_BJet1;   //!
    TBranch        *b_bdt_DeltaPhiTopBJet1_BJet2;   //!
    TBranch        *b_bdt_SumEtLep_MET;   //!
    TBranch        *b_bdt_MET;   //!
    TBranch        *b_bdt_SumEtLep_MET_BJets;   //!
    TBranch        *b_bdt_Pt_Lep;   //!
    TBranch        *b_bdt_SumPt;   //!
    TBranch        *b_bdt_DeltaEtaLep_BJet1;   //!
    TBranch        *b_bdt_DeltaPhiBJet1_BJet2;   //!
    TBranch        *b_bdt_DeltaEtaLep_BJet2;   //!
    TBranch        *b_bdt_CosLepW_WTopBJet1;   //!
    TBranch        *b_bdt_DeltaPhiLep_MET;   //!
    TBranch        *b_bdt_DeltaEtaTopBJet2_BJet1;   //!
    TBranch        *b_bdt_CosLepTopBJet2_TopBJet2CMS;   //!
    TBranch        *b_bdt_CosLepTopBJet1_TopBJet1CMS;   //!
    TBranch        *b_bdt_MLep_BJet2;   //!
    TBranch        *b_bdt_CosMET_BJet2;   //!
    TBranch        *b_bdt_SumPtBJet1_BJet2;   //!
    TBranch        *b_bdt_DeltaEtaNu_BJetNonTop; //!

    // Likelihood variables
    Double_t        v_llh_sChannel2j;
    Double_t        v_llh_sChannel3j;
    Double_t        v_llh_tChannel4FS;
    Double_t        v_llh_ttbarSL;
    Double_t        v_llh_Wjj;
    Double_t        v_llh_Wbb;

    // Likelihood branches
    TBranch        *b_llh_sChannel2j;  //!
    TBranch        *b_llh_sChannel3j;  //!
    TBranch        *b_llh_tChannel4FS; //!
    TBranch        *b_llh_ttbarSL;     //!
    TBranch        *b_llh_Wjj;         //!
    TBranch        *b_llh_Wbb;         //!

  public:
    AtlEvtReaderMemBDT(AtlSelector *parent);
    virtual ~AtlEvtReaderMemBDT();
    virtual void SetBranches(TTree *t);
    virtual Bool_t Notify();

    ClassDef(AtlEvtReaderMemBDT,0) // Event reader for MemTk ntuples
};
#endif

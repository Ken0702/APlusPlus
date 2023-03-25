//
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2014 (C) Soeren Stamm
//
#ifndef ATLAS_AtlEvtReaderMem
#define ATLAS_AtlEvtReaderMem
#ifndef ATLAS_AtlEvtReaderD3PDSgTop
#include <AtlEvtReaderD3PDSgTop.h>
#endif

class AtlEvtReaderMem : public AtlEvtReaderD3PDSgTop {

  public:

    // =========================
    // Declaration of leaf types
    // =========================

    // Likelihood variables
    Double_t        v_llh_sChannel2j;
    Double_t        v_llh_sChannel3j;
    Double_t        v_llh_tChannel4FS;
    Double_t        v_llh_tChannel5FS;
    Double_t        v_llh_ttbarSL;
    Double_t        v_llh_ttbarDL;
    Double_t        v_llh_Wjj;
    Double_t        v_llh_Wcj;
    Double_t        v_llh_Wbb;

    // Likelihood branches
    TBranch        *b_llh_sChannel2j;  //!
    TBranch        *b_llh_sChannel3j;  //!
    TBranch        *b_llh_tChannel4FS; //!
    TBranch        *b_llh_tChannel5FS; //!
    TBranch        *b_llh_ttbarSL;     //!
    TBranch        *b_llh_ttbarDL;     //!
    TBranch        *b_llh_Wjj;         //!
    TBranch        *b_llh_Wcj;         //!
    TBranch        *b_llh_Wbb;         //!

  public:
    AtlEvtReaderMem(AtlSelector *parent);
    virtual ~AtlEvtReaderMem();
    virtual void SetBranches(TTree *t);
    virtual Bool_t Notify();

    ClassDef(AtlEvtReaderMem,0) // Event reader for MemTk ntuples
};
#endif

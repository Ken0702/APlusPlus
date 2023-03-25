//
// Author: Stephan Kaphle <mailto: kaphle@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2014 (C) Soeren Stamm
//
#ifndef ATLAS_AtlEvtReaderMemR2JetLepton
#define ATLAS_AtlEvtReaderMemR2JetLepton
#ifndef ATLAS_AtlEvtReaderD3PDJetLepton
#include <AtlEvtReaderD3PDJetLepton.h>
#endif

class AtlEvtReaderMemR2JetLepton : public AtlEvtReaderD3PDJetLepton {

  private:

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
    AtlEvtReaderMemR2JetLepton(AtlSelector *parent,
                      EDataType dataType,
                      const char * D3PDversion,
                      char const * systematicName,
                      char const * treeName,
					  Int_t mc_weight_number);
    virtual ~AtlEvtReaderMemR2JetLepton();
    virtual void ClearBranches() override;
    virtual void SetBranches(TTree *t) override;
    virtual Bool_t Notify() override;
    virtual Double_t GetLLh_sChannel2j() override { return v_llh_sChannel2j; }
    virtual Double_t GetLLh_sChannel3j() override { return v_llh_sChannel3j; }
    virtual Double_t GetLLh_tChannel4FS() override { return v_llh_tChannel4FS; }
    virtual Double_t GetLLh_tChannel5FS() override { return v_llh_tChannel5FS; }
    virtual Double_t GetLLh_ttbarSL() override { return v_llh_ttbarSL; }
    virtual Double_t GetLLh_ttbarDL() override { return v_llh_ttbarDL; }
    virtual Double_t GetLLh_Wjj() override { return v_llh_Wjj; }
    virtual Double_t GetLLh_Wcj() override { return v_llh_Wcj; }
    virtual Double_t GetLLh_Wbb() override { return v_llh_Wbb; }

    ClassDefOverride(AtlEvtReaderMemR2JetLepton,0) // Event reader for MemTk ntuples in run 2
};
#endif

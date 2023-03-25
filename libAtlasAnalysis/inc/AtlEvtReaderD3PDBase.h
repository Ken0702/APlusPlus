//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2012 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtReaderD3PDBase
#define ATLAS_AtlEvtReaderD3PDBase
#include <AtlEvtReaderBase.h>

class AtlEvtReaderD3PDBase : public AtlEvtReaderBase {

protected:
    Int_t   fRunLast;     // Run number of the previous run (needed for writing the trigger config dbase in case of D3PD input)
    Bool_t  fIsMC;        // Looks whether input file is MC or not

public:
    explicit AtlEvtReaderD3PDBase(AtlSelector *parent);
    virtual ~AtlEvtReaderD3PDBase();
    Int_t GetEntry(TTree *t, Long64_t entry) override;
    inline Int_t GetRunLast() { return fRunLast; }

    // S.K.:
    // - to give MemDiscR2 access to llhs for SgTopR2 and JetLepton input
    // - should probably be solved via the AtlEvent, but this works
    virtual Double_t GetLLh_sChannel2j() {return -999.; }
    virtual Double_t GetLLh_sChannel3j() {return -999.; }
    virtual Double_t GetLLh_tChannel4FS() {return -999.; }
    virtual Double_t GetLLh_tChannel5FS() {return -999.; }
    virtual Double_t GetLLh_ttbarSL() {return -999.; }
    virtual Double_t GetLLh_ttbarDL() {return -999.; }
    virtual Double_t GetLLh_Wjj() {return -999.; }
    virtual Double_t GetLLh_Wcj() {return -999.; }
    virtual Double_t GetLLh_Wbb() {return -999.; }

protected:
    void InitBranches(TTree *t);
    void SetupBranch(TTree *t, const char* branchname,
		     void* value, TBranch** branch);
    virtual void InitObjPointers() {;}

    ClassDefOverride(AtlEvtReaderD3PDBase,0) // D3PD event reader base class
};
#endif

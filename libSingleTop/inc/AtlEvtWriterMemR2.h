//  
// Author: Stephan Kaphle <mailto: kaphle@physik.hu-berlin.de>
// Update: $Id: AtlEvtWriterMemR2.h,v 1.4 2018/02/23 17:14:53 kaphle Exp $
// Copyright: 2013 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtWriterMemR2
#define ATLAS_AtlEvtWriterMemR2
#ifndef ATLAS_AtlEvtWriterBase
#include <AtlEvtWriterBase.h>
#endif

class AtlEvtWriterMemR2 : public AtlEvtWriterBase {

private:
    Float_t fEventWeight; // Total event weight
    Bool_t  fCloneOnly;   // Don't add branches to output
    Bool_t  fReduceOutput;// option to output only "llh*" branches

    // MemDisc results
    Double_t fMemDisc_sChannelRatio;
    Double_t fMemDisc_wjetsRatio;
    Double_t fMemDisc_ttbarRatio;

public:
    AtlEvtWriterMemR2();
    virtual ~AtlEvtWriterMemR2();
    virtual void BookTree(TTree *tree, AtlEvent *evt);
    virtual void WriteEvent();
    virtual TTree* CreateTree(const char* name, const char* title,
			      TTree *InputTree);
    virtual const char* GetNameOfType() /*override*/ { return "kMemR2"; }
    inline void SetCloneOnly(Bool_t co) { fCloneOnly = co; }
    inline void SetReduceOutput(Bool_t ro) { fReduceOutput = ro; }
    inline void SetMemDisc_sChannelRatio(Double_t r) { fMemDisc_sChannelRatio = r; }
    inline void SetMemDisc_wjetsRatio(Double_t r) { fMemDisc_wjetsRatio = r; }
    inline void SetMemDisc_ttbarRatio(Double_t r) { fMemDisc_ttbarRatio = r; }

    ClassDef(AtlEvtWriterMemR2,0) // Event writer for SgTop-D3PD format with only MEM branches
};
#endif


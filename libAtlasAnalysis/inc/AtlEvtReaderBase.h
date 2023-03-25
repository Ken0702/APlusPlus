//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2011 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtReaderBase
#define ATLAS_AtlEvtReaderBase
#include <cmath>
#ifndef ROOT_TObject
#include <TObject.h>
#endif
#ifndef ATLAS_AtlEvent
#include <AtlEvent.h>
#endif
#ifndef ATLAS_AtlSelector
#include <AtlSelector.h>
#endif
#ifndef ROOT_TTree
#include <TTree.h>
#endif

class AtlEvtReaderBase : public TObject {

  protected:
    AtlEvent    *fEvent;  // Event object
    AtlSelector *fParent; // Parent selector

  public:

    struct InitialSumOfWeights_t {
        InitialSumOfWeights_t()
            : one(NAN)
            , mcevt(NAN)
            , mcevt_pileup_zvtx(NAN)
        { }

        /* choose a weight -> */
        double one;
        double mcevt;
        double mcevt_pileup_zvtx;
    };

    AtlEvtReaderBase(AtlSelector *parent);
    virtual ~AtlEvtReaderBase();
    virtual void SetBranches(TTree * /*t*/) = 0;
    virtual Int_t GetEntry(TTree * /*t*/, Long64_t /*entry = 0*/) = 0;
    virtual Bool_t Notify() { return kTRUE; }
    inline void SetEvent(AtlEvent *evt) { fEvent = evt; }
    virtual InitialSumOfWeights_t GetInitialSumOfWeights(TFile * inputFile) const;

    static Bool_t SumOverTree(TTree * tree, char const * expression, Double_t & result);
    inline Bool_t IsFirstEvent() const { return fParent->IsFirstEvent(); }

  protected:
    virtual void BuildEvent() = 0;
    virtual void BuildEventHeader() = 0;

    ClassDef(AtlEvtReaderBase,0) // Abstract event reader class
};
#endif

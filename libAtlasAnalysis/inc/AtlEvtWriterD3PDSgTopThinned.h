#ifndef LIBATLASANALYSIS_INC_ATLEVTWRITERD3PDSGTOPTHINNED_H_
#define LIBATLASANALYSIS_INC_ATLEVTWRITERD3PDSGTOPTHINNED_H_

#include <Rtypes.h>

#include "AtlEvtWriterBase.h"

class AtlEvent;
class TList;
class TMap;
class TTree;


class AtlEvtWriterD3PDSgTopThinned : public AtlEvtWriterBase {

public:

    enum { kMaxJets = 128 };
    enum { kMaxLeptons = 16 };

    AtlEvtWriterD3PDSgTopThinned();
    ~AtlEvtWriterD3PDSgTopThinned() /*override*/;

    void BookTree(TTree * tree, AtlEvent * evt) /*override*/;
    void WriteEvent() /*override*/;
    TTree * CreateTree(char const * name, char const * title, TTree * inputTree) /*override*/;

    virtual void SetJets(TList * jets, TList * bjets = 0);
    virtual void SetLeptons(TList * leptons);

    virtual void BookSubselectionFlag(char const * name);
    virtual void SetSubselectionFlag(char const * name, Bool_t value);

protected:

    TList * fJets;
    TList * fBJets;
    TList * fLeptons;

    TMap * fSubselectionFlags;

    Float_t v_evt_weight_mc;
    Float_t v_evt_weight_pretag;
    Float_t v_evt_weight_tag;

    Long_t v_eventNumber;
    Int_t v_channelNumber;

    Int_t v_jet_n;
    Float_t * v_jet_pt;
    Float_t * v_jet_eta;
    Float_t * v_jet_phi;
    Int_t * v_jet_btag;

    Int_t v_lep_n;
    Float_t * v_lep_pt;
    Float_t * v_lep_eta;
    Float_t * v_lep_phi;
    Int_t * v_lep_pdgId;

    Float_t v_met_pt;
    Float_t v_met_phi;

    ClassDef(AtlEvtWriterD3PDSgTopThinned, 0)
};


#endif /* LIBATLASANALYSIS_INC_ATLEVTWRITERD3PDSGTOPTHINNED_H_ */

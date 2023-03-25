//  
// Author: Felix Thomas <mailto: Felix.Thomas@mailbox.tu-dresden.de>
// Update: $Id$
// Copyright: 2011 (C) Felix Thomas
//
#ifndef ATLAS_AtlMETWeights
#define ATLAS_AtlMETWeights
#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif


class AtlMETWeights {

private:

    Float_t        fMETw_tight_Px;         // Missing Et tight weight px
    Float_t        fMETw_tight_Py;         // Missing Et tight weight py
    Float_t        fMETw_tight_Et;         // Missing Et tight weight et
    UInt_t         fMET_tight_statusWord;  // Missing Et tight status word
    Float_t        fMETw_medium_Px;        // Missing Et medium weight px
    Float_t        fMETw_medium_Py;        // Missing Et medium weight py
    Float_t        fMETw_medium_Et;        // Missing Et medium weight et
    UInt_t         fMET_medium_statusWord; // Missing Et medium status word

public:
    AtlMETWeights();
    virtual ~AtlMETWeights();
    virtual void Clear(Option_t *option = "");

    // Weights are computed with tight electron definition
    inline Float_t GetMETw_tight_Px() {  return fMETw_tight_Px; }
    inline Float_t GetMETw_tight_Py() {  return fMETw_tight_Py; }
    inline Float_t GetMETw_tight_Et() {  return fMETw_tight_Et; }
    inline UInt_t GetMET_tight_statusWord() { return fMET_tight_statusWord; }
    inline void SetMET_tight_Weights(Float_t w_Px, Float_t w_Py, Float_t w_Et) {
        fMETw_tight_Px = w_Px;
        fMETw_tight_Py = w_Py;
        fMETw_tight_Et = w_Et;
    }
    // This method is necessary for backwards compatibility
    inline void SetMETWeights(Float_t w_Px, Float_t w_Py, Float_t w_Et) {
        SetMET_tight_Weights(w_Px, w_Py, w_Et);
    }
    inline void SetMET_tight_statusWord(UInt_t statusWord) { fMET_tight_statusWord = statusWord; }

    // Weights are computed with medium electron definition
    inline Float_t GetMETw_medium_Px() { return fMETw_medium_Px; }
    inline Float_t GetMETw_medium_Py() { return fMETw_medium_Py; }
    inline Float_t GetMETw_medium_Et() { return fMETw_medium_Et; }
    inline UInt_t GetMET_medium_statusWord() { return fMET_medium_statusWord; }
    
    inline void SetMET_medium_Weights(Float_t w_Px, Float_t w_Py, Float_t w_Et) {
        fMETw_medium_Px = w_Px;
        fMETw_medium_Py = w_Py;
        fMETw_medium_Et = w_Et;
    }
    inline void SetMET_medium_statusWord(UInt_t statusWord) { fMET_medium_statusWord = statusWord; }

    ClassDef(AtlMETWeights,1) // Interface for setting MET weights
};
#endif

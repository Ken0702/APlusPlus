//  
// Author: Daniel Richter <mailto: danri@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2008 (C) Daniel Richter
//
#ifndef ATLAS_AtlSCT3DHit
#define ATLAS_AtlSCT3DHit
#ifndef ATLAS_AtlSilicon3DHit
#include "AtlSilicon3DHit.h"
#endif

class AtlSCT3DHit : public AtlSilicon3DHit {
private:
    Int_t fSide;           // Inner outer side of Si crystal
    Int_t fStrip;          // Strip number of the cluster
public:
    AtlSCT3DHit();
    AtlSCT3DHit(Int_t Id, Float_t X, Float_t Y, Float_t Z);
    virtual ~AtlSCT3DHit();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "") const;
    static void PrintHeader();
    static void PrintFooter();

    inline Int_t Side() const { return fSide; }
    inline Int_t Strip() const { return fStrip; }
    inline void  SetSide(Int_t Side) { fSide = Side; }
    inline void  SetStrip(Int_t Strip) { fStrip = Strip; }

    ClassDef(AtlSCT3DHit,1) // ATLAS SCT digitisation
};

#endif

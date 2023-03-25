//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlFastMuon
#define ATLAS_AtlFastMuon
#ifndef HEP_HepMuon
#include <HepMuon.h>
#endif

class AtlFastMuon : public HepMuon {

private:

public:
    AtlFastMuon();
    AtlFastMuon(Int_t Id, Float_t Px, Float_t Py, Float_t Pz, Float_t E,
	    Bool_t IsMuPlus);
    virtual ~AtlFastMuon();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "");
    static void PrintHeader();
    static void PrintFooter();

    ClassDef(AtlFastMuon,1) // Atlas muon class
};
#endif

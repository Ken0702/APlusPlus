//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlFastPhoton
#define ATLAS_AtlFastPhoton
#ifndef HEP_HepPhoton
#include <HepPhoton.h>
#endif

class AtlFastPhoton : public HepPhoton {

private:

public:
    AtlFastPhoton();
    AtlFastPhoton(Int_t Id, Float_t Px, Float_t Py, Float_t Pz, Float_t E);
    virtual ~AtlFastPhoton();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "");
    static void PrintHeader();
    static void PrintFooter();

    ClassDef(AtlFastPhoton,1) // Atlas photon class
};
#endif

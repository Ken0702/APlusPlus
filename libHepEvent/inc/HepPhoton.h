//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepPhoton
#define HEP_HepPhoton
#ifndef HEP_HepParticle
#include <HepParticle.h>
#endif

class HepPhoton : public HepParticle {

protected:

public:
    HepPhoton();
    HepPhoton(Int_t Id, Float_t Px, Float_t Py, Float_t Pz, Float_t E);
    virtual ~HepPhoton();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "");
    static void PrintHeader();
    static void PrintFooter();

    ClassDef(HepPhoton,1) // Basic photon class
};
#endif


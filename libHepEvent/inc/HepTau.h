//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepTau
#define HEP_HepTau
#ifndef HEP_HepParticle
#include <HepParticle.h>
#endif

class HepTau : public HepParticle {

protected:

public:
    HepTau();
    HepTau(Int_t Id, Float_t Px, Float_t Py, Float_t Pz, Float_t E,
	Bool_t IsTauPlus);
    virtual ~HepTau();
    virtual void Clear(Option_t *option = "");

    ClassDef(HepTau,1) // Basic tau class
};
#endif


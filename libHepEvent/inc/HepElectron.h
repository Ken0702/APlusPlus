//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepElectron
#define HEP_HepElectron
#ifndef HEP_HepParticle
#include <HepParticle.h>
#endif

class HepElectron : public HepParticle {

protected:

public:
    HepElectron();
    HepElectron(Int_t Id, Float_t Px, Float_t Py, Float_t Pz, Float_t E,
       Bool_t IsPositron);
    virtual ~HepElectron();
    virtual void Clear(Option_t *option = "");

    ClassDef(HepElectron,2) // Basic electron class
};
#endif


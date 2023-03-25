//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepMuon
#define HEP_HepMuon
#ifndef HEP_HepParticle
#include <HepParticle.h>
#endif

class HepMuon : public HepParticle {

protected:

public:
    HepMuon();
    HepMuon(Int_t Id, Float_t Px, Float_t Py, Float_t Pz, Float_t E,
	Bool_t IsMuPlus);
    virtual ~HepMuon();
    virtual void Clear(Option_t *option = "");

    ClassDef(HepMuon,1) // Basic muon class
};
#endif


//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlZ0DecayJJ
#define ATLAS_AtlZ0DecayJJ
#ifndef HEP_HepZ0Decay
#include <HepZ0Decay.h>
#endif
#ifndef ATLAS_AtlJet
#include <AtlJet.h>
#endif

class AtlZ0DecayJJ : public HepZ0Decay {

private:

public:
    AtlZ0DecayJJ();
    AtlZ0DecayJJ(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		 Float_t E, AtlJet *Jet1, AtlJet *Jet2,
	         HepParticle ReFitJet1, HepParticle ReFitJet2);
    virtual ~AtlZ0DecayJJ();
    virtual void Clear(Option_t *option = "");
    
    inline AtlJet* GetJet1()  { return (AtlJet*)GetDaughter1(); }
    inline AtlJet* GetJet2() { return (AtlJet*)GetDaughter2(); }
    inline HepParticle GetReFitJet1(){return GetReFit_Daughter1(); }
    inline HepParticle GetReFitJet2(){return GetReFit_Daughter2(); }

    ClassDef(AtlZ0DecayJJ,1) // Z0 -> q bar{q} decay
};
#endif


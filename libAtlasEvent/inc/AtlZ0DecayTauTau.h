//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
// Copyright: 2010 (C) Steven Bilski
//
#ifndef ATLAS_AtlZ0DecayTauTau
#define ATLAS_AtlZ0DecayTauTau
#ifndef HEP_HepZ0Decay
#include <HepZ0Decay.h>
#endif
#ifndef ATLAS_AtlTau
#include <AtlTau.h>
#endif

class AtlZ0DecayTauTau : public HepZ0Decay {

private:

public:
    AtlZ0DecayTauTau();
    AtlZ0DecayTauTau(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		     Float_t E, AtlTau *TauPlus, AtlTau *TauMinus,
	             HepParticle ReFitTauPlus, HepParticle ReFitTauMinus);
    virtual ~AtlZ0DecayTauTau();
    virtual void Clear(Option_t *option = "");
    
    inline AtlTau* GetTauPlus()  { return (AtlTau*)GetDaughter1(); }
    inline AtlTau* GetTauMinus() { return (AtlTau*)GetDaughter2(); }
    inline HepParticle GetReFitTauPlus(){return GetReFit_Daughter1(); }
    inline HepParticle GetReFitTauMinus(){return GetReFit_Daughter2(); }

    ClassDef(AtlZ0DecayTauTau,1) // Z0 -> tau bar{tau} decay
};
#endif


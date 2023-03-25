//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
// Copyright: 2010 (C) Steven Bilski
//
#ifndef ATLAS_AtlZ0DecayEE
#define ATLAS_AtlZ0DecayEE
#ifndef HEP_HepZ0Decay
#include <HepZ0Decay.h>
#endif
#ifndef ATLAS_AtlElectron
#include <AtlElectron.h>
#endif

class AtlZ0DecayEE : public HepZ0Decay {

private:

public:
    AtlZ0DecayEE();
    AtlZ0DecayEE(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		 Float_t E, AtlElectron *EPlus, AtlElectron *EMinus,
	         HepParticle ReFitEPlus, HepParticle ReFitEMinus);
    virtual ~AtlZ0DecayEE();
    virtual void Clear(Option_t *option = "");
    
    inline AtlElectron* GetEPlus()  { return (AtlElectron*)GetDaughter1(); }
    inline AtlElectron* GetEMinus() { return (AtlElectron*)GetDaughter2(); }
    inline HepParticle GetReFitEPlus(){return GetReFit_Daughter1(); }
    inline HepParticle GetReFitEMinus(){return GetReFit_Daughter2(); }

    ClassDef(AtlZ0DecayEE,1) // Z0 -> e+e- decay
};
#endif


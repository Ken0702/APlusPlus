//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
// Copyright: 2010 (C) Steven Bilski
//
#ifndef ATLAS_AtlZ0DecayMuMu
#define ATLAS_AtlZ0DecayMuMu
#ifndef HEP_HepZ0Decay
#include <HepZ0Decay.h>
#endif
#ifndef ATLAS_AtlMuon
#include <AtlMuon.h>
#endif

class AtlZ0DecayMuMu : public HepZ0Decay {

private:

public:
    AtlZ0DecayMuMu();
    AtlZ0DecayMuMu(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		   Float_t E, AtlMuon *MuPlus, AtlMuon *MuMinus,
                   HepParticle ReFitMuPlus, HepParticle ReFitMuMinus);
    virtual ~AtlZ0DecayMuMu();
    virtual void Clear(Option_t *option = "");
    
    inline AtlMuon* GetMuPlus()  { return (AtlMuon*)GetDaughter1(); }
    inline AtlMuon* GetMuMinus() { return (AtlMuon*)GetDaughter2(); }
    inline HepParticle GetReFitMuPlus(){return GetReFit_Daughter1(); }
    inline HepParticle GetReFitMuMinus(){return GetReFit_Daughter2(); }
    
    ClassDef(AtlZ0DecayMuMu,1) // Z0 -> mu+mu- decay
};
#endif


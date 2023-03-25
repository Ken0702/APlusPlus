


//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef HEP_HepZ0Decay
#define HEP_HepZ0Decay
#ifndef HEP_HepDecayParticle
#include <HepDecayParticle.h>
#endif

class HepZ0Decay : public HepDecayParticle {

private:
    TRef  *fDaughter1;  // Daughter particle 1
    TRef  *fDaughter2;  // Daughter particle 2
    HepParticle fReFitDaughter1; // in case of using the kinematic fitter,
    HepParticle fReFitDaughter2; // storig new momenta and PDGCode
    
    
public:
    HepZ0Decay();
    HepZ0Decay(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
	       Float_t E, TObject *Daughter1, TObject *Daughter2);
    HepZ0Decay(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
	       Float_t E, TObject *Daughter1, TObject *Daughter2,
	       HepParticle ReFitDaughter1, HepParticle ReFitDaughter2);
    virtual ~HepZ0Decay();
    virtual void Clear(Option_t *option);
    virtual Int_t Compare(const TObject *obj) const;
 
    inline virtual Bool_t IsSortable() const { return kTRUE; }
    inline TObject* GetDaughter1() { return fDaughter1->GetObject(); }
    inline TObject* GetDaughter2() { return fDaughter2->GetObject(); }
    inline HepParticle GetReFit_Daughter1(){ return fReFitDaughter1;   }
    inline HepParticle GetReFit_Daughter2(){ return fReFitDaughter2;   }

    inline Bool_t IsEE()     { return ((HepParticle*)GetDaughter1())->IsElectron(); }
    inline Bool_t IsMuMu()   { return ((HepParticle*)GetDaughter1())->IsMuon(); }
    inline Bool_t IsTauTau() { return ((HepParticle*)GetDaughter1())->IsTau(); }
    
    ClassDef(HepZ0Decay,2) // Base Z0 boson decay class
};
#endif


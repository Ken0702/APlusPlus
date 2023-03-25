//  
// Author: Sebastian Beumler <mailto: beumler@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2010 (C) Sebastian Beumler
//
#ifndef HEP_HepK0sDecay
#define HEP_HepK0sDecay
#ifndef HEP_HepDecayParticle
#include <HepDecayParticle.h>
#endif

class HepVertex;

class HepK0sDecay : public HepDecayParticle {

private:
    TRef    *fDaughter1;   // Daughter particle 1
    TRef    *fDaughter2;   // Daughter particle 2
    TRef    *fVtx;         // Associated Vertex
    Float_t  fDecayLength; // Distance from primary vtx (cm)
    HepParticle fFit_Daughter1; // in case of using the kinematic fitter,
    HepParticle fFit_Daughter2; // storig new momenta and PDGCode
    
public:
    HepK0sDecay();
    HepK0sDecay(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		Float_t E, TObject *Daughter1, TObject *Daughter2,
		HepVertex *DecayVtx, HepVertex* PrimaryVtx,
		HepParticle Fit_Daughter1, HepParticle Fit_Daughter2);
    virtual ~HepK0sDecay();
    virtual void Clear(Option_t *option);
    Float_t GetLifeTime();
    
    inline Float_t GetDecayLength() const { return fDecayLength; }
    inline TObject* GetDaughter1()   { return fDaughter1->GetObject(); }
    inline TObject* GetDaughter2()   { return fDaughter2->GetObject(); }
    virtual inline TObject* GetVtx() { return fVtx->GetObject();       }
    inline HepParticle GetFit_Daughter1() { return fFit_Daughter1;     }
    inline HepParticle GetFit_Daughter2() { return fFit_Daughter2;     }
     
    ClassDef(HepK0sDecay,3) // Base K0s decay class
};
#endif


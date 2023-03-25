//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef HEP_HepDecayParticle
#define HEP_HepDecayParticle
#ifndef HEP_HepParticle
#include <HepParticle.h>
#endif
#ifndef ROOT_TRef
#include <TRef.h>
#endif

class HepDecayParticle : public HepParticle {

protected:
    TRef   *fMCDecay; // Matched MC truth decay (if any)
    Float_t fChi2;    // Chi2 of the kinematic fit
    Int_t   fNDoF;    // No. of degrees per freedom of the kinematic fit
    Float_t fProb;    // Chi2 probability of the kinematic fit
    
public:
    HepDecayParticle();
    HepDecayParticle(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		     Float_t E, Int_t PdgCode);
    virtual ~HepDecayParticle();
    virtual void Clear(Option_t *option = "");
    inline void SetChi2NDoF(Float_t chi2, Int_t ndof) {
	fChi2 = chi2;
	fNDoF = ndof;
	fProb = TMath::Prob(chi2, ndof);
    }
    inline Float_t GetChi2() const { return fChi2; }
    inline Float_t GetProb() const { return fProb; }
    inline Int_t   GetNDoF() const { return fNDoF; }
    inline Float_t GetChi2overNDoF() const {
	return ( fNDoF == 0 ) ? 0. : fChi2/fNDoF;
    }
    
    ClassDef(HepDecayParticle,3) // Base class for particle decays
};
#endif


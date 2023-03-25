//  
// Author: Sebastian Beumler <mailto: beumler@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2010 (C) Sebastian Beumler
//

#ifndef HEP_HepK0sDecay
#include <HepK0sDecay.h>
#endif
#include <HepVertex.h>

#ifndef __CINT__
ClassImp(HepK0sDecay);
#endif

//____________________________________________________________________

HepK0sDecay::HepK0sDecay() {
    //
    // Default constructor
    //
    fDaughter1 = new TRef;
    fDaughter2 = new TRef;
    fVtx       = new TRef;

    fFit_Daughter1.Clear();
    fFit_Daughter2.Clear();
    
}

//____________________________________________________________________

HepK0sDecay::HepK0sDecay(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
			 Float_t E, TObject *Daughter1, TObject *Daughter2,
			 HepVertex *DecayVtx, HepVertex *PrimaryVtx,
			 HepParticle Fit_Daughter1, HepParticle Fit_Daughter2) :
    HepDecayParticle(Id, Px, Py, Pz, E, 310) {
    //
    // Normal constructor
    //
    fDaughter1 = new TRef; *fDaughter1 = Daughter1;
    fDaughter2 = new TRef; *fDaughter2 = Daughter2;
    fVtx       = new TRef; *fVtx       = (TObject*)DecayVtx;
    fFit_Daughter1 = Fit_Daughter1;
    fFit_Daughter2 = Fit_Daughter2;
    TVector3 d = PrimaryVtx->GetPos() - DecayVtx->GetPos();
    fDecayLength = d.Mag();
}

//____________________________________________________________________

HepK0sDecay::~HepK0sDecay() {
    //
    // Default destructor
    //
    delete fDaughter1;
    delete fDaughter2;
    delete fVtx;
}

//____________________________________________________________________

void HepK0sDecay::Clear(Option_t *option) {
    //
    // Clear this object
    //
    HepDecayParticle::Clear(option);
    delete fDaughter1; fDaughter1 = 0;
    delete fDaughter2; fDaughter2 = 0;
    delete fVtx;       fVtx       = 0;
    fDecayLength = 0.;
}

//____________________________________________________________________

Float_t HepK0sDecay::GetLifeTime() {
    //
    // Compute life-time (in ps)
    //
    Double_t BetaGamma = Pmag()/M("pdg");
    return GetDecayLength()/(BetaGamma*TMath::Ccgs())*1.e12;
}

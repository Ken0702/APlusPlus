//____________________________________________________________________
//
// Abstract Z0 boson decay class
// 
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef HEP_HepZ0Decay
#include <HepZ0Decay.h>
#endif
#ifndef __CINT__
ClassImp(HepZ0Decay);
#endif

//____________________________________________________________________

HepZ0Decay::HepZ0Decay() {
    //
    // Default constructor
    //
    fDaughter1 = new TRef;
    fDaughter2 = new TRef;
}

//____________________________________________________________________

HepZ0Decay::HepZ0Decay(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		       Float_t E, TObject *Daughter1, TObject *Daughter2) :
    HepDecayParticle(Id, Px, Py, Pz, E, 23) {
    //
    // Normal constructor
    //
    fDaughter1 = new TRef; *fDaughter1 = Daughter1;
    fDaughter2 = new TRef; *fDaughter2 = Daughter2;
}

//____________________________________________________________________

HepZ0Decay::HepZ0Decay(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		       Float_t E, TObject *Daughter1, TObject *Daughter2,
                       HepParticle ReFitDaughter1, HepParticle ReFitDaughter2) :
    HepDecayParticle(Id, Px, Py, Pz, E, 23) {
    //
    // Normal constructor for kinematic fitter
    //
    fDaughter1 = new TRef; *fDaughter1 = Daughter1;
    fDaughter2 = new TRef; *fDaughter2 = Daughter2;
    fReFitDaughter1 = ReFitDaughter1;
    fReFitDaughter2 = ReFitDaughter2;
}

//____________________________________________________________________

HepZ0Decay::~HepZ0Decay() {
    //
    // Default destructor
    //
    delete fDaughter1;
    delete fDaughter2;
}

//____________________________________________________________________

void HepZ0Decay::Clear(Option_t *option) {
    //
    // Clear this object
    //
    HepDecayParticle::Clear(option);
    delete fDaughter1; fDaughter1 = 0;
    delete fDaughter2; fDaughter2 = 0;
    fReFitDaughter1.Clear();
    fReFitDaughter2.Clear();
}

//____________________________________________________________________

Int_t HepZ0Decay::Compare(const TObject *obj) const {
    //
    // Compare invariant mass of Z decay candidates
    //
    // Return values:
    //    +1 = invariant mass of this decay is closer to the pole-mass of
    //         91.187GeV than the given decay
    //     0 = both invariant masses are equal
    //    -1 = invariant mass of the compared decay is closer to the
    //         pole-mass than this decay
    //
    static const Double_t m_Z = 91.187;
    if ( TMath::Abs(M("rec") - m_Z)
	 < TMath::Abs(((HepZ0Decay*)obj)->M("rec") - m_Z) ) return  1;
    if ( TMath::Abs(M("rec") - m_Z)
	 > TMath::Abs(((HepZ0Decay*)obj)->M("rec") - m_Z) ) return -1;
    return 0;
}

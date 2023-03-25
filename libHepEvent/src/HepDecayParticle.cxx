//____________________________________________________________________
//
// Base class for reconstructed particle decays (Z->ee, Z->mumu etc.)
// 
//
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef HEP_HepDecayParticle
#include <HepDecayParticle.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(HepDecayParticle);
#endif

//____________________________________________________________________

HepDecayParticle::HepDecayParticle() {
    //
    // Default constructor
    //
    fMCDecay = new TRef;
}

//____________________________________________________________________

HepDecayParticle::HepDecayParticle(Int_t Id, Float_t Px, Float_t Py,
				   Float_t Pz, Float_t E, Int_t PdgCode) :
    HepParticle(Id, Px, Py, Pz, E, PdgCode) {
    //
    // Normal constructor
    //
    fMCDecay = new TRef;
}

//____________________________________________________________________

HepDecayParticle::~HepDecayParticle() {
    //
    // Default destructor
    //
    delete fMCDecay;
}

//____________________________________________________________________

void HepDecayParticle::Clear(Option_t *option) {
    //
    // Clear this object
    //	
    HepParticle::Clear(option);
    delete fMCDecay; fMCDecay = 0;
}

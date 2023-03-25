//____________________________________________________________________
//
// Basic tau class
// 

//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepTau
#include <HepTau.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(HepTau);
#endif

//____________________________________________________________________

HepTau::HepTau() {
  //
  // Default constructor
  //
}

//____________________________________________________________________

HepTau::HepTau(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		 Float_t E, Bool_t IsTauPlus) :
    HepParticle(Id, Px, Py, Pz, E, ( IsTauPlus ) ? -15 : 15) {
    //
    // Normal constructor
    //
}

//____________________________________________________________________

HepTau::~HepTau() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void HepTau::Clear(Option_t *option) {
    //
    // Clear object
    //
    HepParticle::Clear(option);
}

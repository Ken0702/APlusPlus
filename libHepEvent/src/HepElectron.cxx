//____________________________________________________________________
//
// Basic electron class
// 

//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepElectron
#include <HepElectron.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(HepElectron);
#endif

//____________________________________________________________________

HepElectron::HepElectron() {
  //
  // Default constructor
  //
}

//____________________________________________________________________

HepElectron::HepElectron(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
			 Float_t E, Bool_t IsPositron) :
    HepParticle(Id, Px, Py, Pz, E, ( IsPositron ) ? -11 : 11) {
    //
    // Normal constructor
    //
}

//____________________________________________________________________

HepElectron::~HepElectron() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void HepElectron::Clear(Option_t *option) {
    //
    // Clear object
    //
    HepParticle::Clear(option);
}

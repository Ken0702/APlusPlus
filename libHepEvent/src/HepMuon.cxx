//____________________________________________________________________
//
// Basic muon class
// 

//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepMuon
#include <HepMuon.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(HepMuon);
#endif

//____________________________________________________________________

HepMuon::HepMuon() {
  //
  // Default constructor
  //
}

//____________________________________________________________________

HepMuon::HepMuon(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
		 Float_t E, Bool_t IsMuPlus) :
    HepParticle(Id, Px, Py, Pz, E, ( IsMuPlus ) ? -13 : 13) {
    //
    // Normal constructor
    //
}

//____________________________________________________________________

HepMuon::~HepMuon() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void HepMuon::Clear(Option_t *option) {
    //
    // Clear object
    //
    HepParticle::Clear(option);
}

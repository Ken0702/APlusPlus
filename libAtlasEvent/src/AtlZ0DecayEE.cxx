//____________________________________________________________________
//
// Z0 -> e+e- decay
// 
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
// Copyright: 2010 (C) Steven Bilski
//
#ifndef ATLAS_AtlZ0DecayEE
#include <AtlZ0DecayEE.h>
#endif

#ifndef __CINT__
ClassImp(AtlZ0DecayEE);
#endif

//____________________________________________________________________

AtlZ0DecayEE::AtlZ0DecayEE() {
    //
    // Default constructor
    //
}

//____________________________________________________________________

AtlZ0DecayEE::AtlZ0DecayEE(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
			   Float_t E, AtlElectron *EPlus, AtlElectron *EMinus,
                           HepParticle ReFitEPlus, HepParticle ReFitEMinus) :
    HepZ0Decay(Id, Px, Py, Pz, E, (TObject*)EPlus, (TObject*)EMinus,
	       ReFitEPlus, ReFitEMinus) {
    //
    // Normal constructor
    //
}

//____________________________________________________________________

AtlZ0DecayEE::~AtlZ0DecayEE() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlZ0DecayEE::Clear(Option_t *option) {
    //
    // Clear this object
    //
    HepZ0Decay::Clear(option);
}

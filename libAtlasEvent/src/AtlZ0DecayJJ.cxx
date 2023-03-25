//____________________________________________________________________
//
// Z0 -> q bar{q} decay
// 
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlZ0DecayJJ
#include <AtlZ0DecayJJ.h>
#endif

#ifndef __CINT__
ClassImp(AtlZ0DecayJJ);
#endif

//____________________________________________________________________

AtlZ0DecayJJ::AtlZ0DecayJJ() {
    //
    // Default constructor
    //
}

//____________________________________________________________________

AtlZ0DecayJJ::AtlZ0DecayJJ(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
			   Float_t E, AtlJet *Jet1, AtlJet *Jet2,
                           HepParticle ReFitJet1, HepParticle ReFitJet2) :
    HepZ0Decay(Id, Px, Py, Pz, E, (TObject*)Jet1, (TObject*)Jet2,
	       ReFitJet1,ReFitJet2) {
    //
    // Normal constructor
    //
}

//____________________________________________________________________

AtlZ0DecayJJ::~AtlZ0DecayJJ() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlZ0DecayJJ::Clear(Option_t *option) {
    //
    // Clear this object
    //
    HepZ0Decay::Clear(option);
}

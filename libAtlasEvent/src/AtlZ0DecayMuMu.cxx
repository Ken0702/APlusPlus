//____________________________________________________________________
//
// Z0 -> mu+mu- decay
// 
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
// Copyright: 2010 (C) Steven Bilski
//
#ifndef ATLAS_AtlZ0DecayMuMu
#include <AtlZ0DecayMuMu.h>
#endif

#ifndef __CINT__
ClassImp(AtlZ0DecayMuMu);
#endif

//____________________________________________________________________

AtlZ0DecayMuMu::AtlZ0DecayMuMu() {
    //
    // Default constructor
    //
}

//____________________________________________________________________

AtlZ0DecayMuMu::AtlZ0DecayMuMu(Int_t Id, Float_t Px, Float_t Py, Float_t Pz,
			       Float_t E, AtlMuon *MuPlus, AtlMuon *MuMinus,
                               HepParticle ReFitMuPlus, HepParticle ReFitMuMinus) :
    HepZ0Decay(Id, Px, Py, Pz, E, (TObject*)MuPlus, (TObject*)MuMinus,
	       ReFitMuPlus,ReFitMuMinus) {
    //
    // Normal constructor
    //
}

//____________________________________________________________________

AtlZ0DecayMuMu::~AtlZ0DecayMuMu() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlZ0DecayMuMu::Clear(Option_t *option) {
    //
    // Clear this object
    //
    HepZ0Decay::Clear(option);
}

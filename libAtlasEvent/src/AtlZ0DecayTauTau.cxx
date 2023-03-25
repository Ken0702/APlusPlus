//____________________________________________________________________
//
// Z0 -> tau bar{tau} decay
// 
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
// Copyright: 2010 (C) Steven Bilski
//
#ifndef ATLAS_AtlZ0DecayTauTau
#include <AtlZ0DecayTauTau.h>
#endif

#ifndef __CINT__
ClassImp(AtlZ0DecayTauTau);
#endif

//____________________________________________________________________

AtlZ0DecayTauTau::AtlZ0DecayTauTau() {
    //
    // Default constructor
    //
}

//____________________________________________________________________

AtlZ0DecayTauTau::AtlZ0DecayTauTau(Int_t Id, Float_t Px, Float_t Py,
				   Float_t Pz, Float_t E, AtlTau *TauPlus,
				   AtlTau *TauMinus,HepParticle ReFitTauPlus,
				   HepParticle ReFitTauMinus) :
    HepZ0Decay(Id, Px, Py, Pz, E, (TObject*)TauPlus, (TObject*)TauMinus,
	       ReFitTauPlus,ReFitTauMinus) {
    //
    // Normal constructor
    //
}

//____________________________________________________________________

AtlZ0DecayTauTau::~AtlZ0DecayTauTau() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlZ0DecayTauTau::Clear(Option_t *option) {
    //
    // Clear this object
    //
    HepZ0Decay::Clear(option);
}

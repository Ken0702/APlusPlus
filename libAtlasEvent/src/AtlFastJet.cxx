//____________________________________________________________________
//
// Atlas MC truth jet
//
// The jet is formed by taking all truth particles - with the
// exception of neutrinos, muons and non interacting particles -
// within a cone around the reconstructed jet.
// BEGIN_HTML
// For details see the <a href="https://twiki.cern.ch/twiki/bin/view/Atlas/JetS#Truth_Jets">Atlas Wiki</a>.
// END_HTML
// 
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlFastJet
#include <AtlFastJet.h>
#endif
#ifndef HEP_Jet
#include <HepJet.h>
#endif

#ifndef __CINT__
ClassImp(AtlFastJet);
#endif

//____________________________________________________________________

AtlFastJet::AtlFastJet() {
  //
  // Default constructor
  //
}

//____________________________________________________________________

AtlFastJet::AtlFastJet(Int_t Id, Float_t E, Float_t Px, Float_t Py, Float_t Pz) :
    HepJet(Id, E, Px, Py, Pz) {
    //
    // Default constructor
    //
}

//____________________________________________________________________

AtlFastJet::~AtlFastJet() {
    //
    // Default destructor
    //
}



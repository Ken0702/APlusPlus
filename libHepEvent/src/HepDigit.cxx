//____________________________________________________________________
//
// Abstract base class for digitisations (hits) in tracking chambers
//
// Might be hits in a drift chamber/tubes (see class
// HepDriftChamberDigit) or a TPC etc.
// 

//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef HEP_HepDigit
#include <HepDigit.h>
#endif

#ifndef __CINT__
ClassImp(HepDigit);
#endif

//____________________________________________________________________

HepDigit::HepDigit() {
  //
  // Default constructor
  //
}

//____________________________________________________________________

HepDigit::HepDigit(Int_t Id) : fId(Id) {
    //
    // Normal constructor
    //
}

//____________________________________________________________________

HepDigit::~HepDigit() {
    //
    // Default destructor
    //
}

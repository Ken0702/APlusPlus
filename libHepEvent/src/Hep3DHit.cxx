//____________________________________________________________________
//
// Abstract 3D hit
// 

//  
// Author: Daniel Richter <mailto: danri@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2008 (C) Daniel Richter
//
#ifndef HEP_Hep3DHit
#include <Hep3DHit.h>
#endif
#include <iostream>
#include <TString.h>

using namespace std;

#ifndef __CINT__
ClassImp(Hep3DHit);
#endif

//____________________________________________________________________

Hep3DHit::Hep3DHit() {
    //
    // Default constructor
    //
}

//____________________________________________________________________

Hep3DHit::Hep3DHit(Int_t Id, Float_t X, Float_t Y, 
		   Float_t Z) : fId(Id), fPos(X, Y, Z) {
    //
    // Normal constructor
    //
}

//____________________________________________________________________

Hep3DHit::~Hep3DHit() {
    //
    // Default destructor
    //    
}

//____________________________________________________________________

void Hep3DHit::Clear(Option_t *option) {
    //
    // Clear this object
    //
}

//____________________________________________________________________

void Hep3DHit::Print(Option_t *option) const {
    //
    // Print digitisation information
    //
    TString opt = option;
    opt.ToLower();
    
    // Print header
    if ( !opt.Contains("nohead") ) PrintHeader();
        
    // Print footer
    if ( !opt.Contains("nohead") ) PrintFooter();
}

//____________________________________________________________________

void Hep3DHit::PrintHeader() {
    //
    // Print information header
    //
}

//____________________________________________________________________

void Hep3DHit::PrintFooter() {
    //
    // Print footer
    //
}


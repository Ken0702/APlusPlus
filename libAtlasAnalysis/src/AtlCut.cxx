//____________________________________________________________________
//
// Small helper class describing cut used by A++ selectors
//
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlCut
#include <AtlCut.h>
#endif

#ifndef __CINT__
ClassImp(AtlCut);
#endif

//____________________________________________________________________

AtlCut::AtlCut(const char* var, const char* val) :
    TNamed(var, val) {
    //
    // Default constructor
    //
}

//____________________________________________________________________

AtlCut::~AtlCut() {
    //
    // Default destructor
    //
}

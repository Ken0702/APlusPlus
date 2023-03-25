//____________________________________________________________________
//
// ATLAS trigger item
// 

//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlTriggerItem
#include <AtlTriggerItem.h>
#endif

#ifndef __CINT__
ClassImp(AtlTriggerItem);
#endif

//____________________________________________________________________

AtlTriggerItem::AtlTriggerItem() {
    //
    // Default constructor
    //
}

//____________________________________________________________________

AtlTriggerItem::AtlTriggerItem(Bool_t Passed, Float_t Prescale,
			       Float_t PassThrough) :
    fPassed(Passed), fPrescale(Prescale), fPassThrough(PassThrough) {
    //
    // Normal constructor
    //
}

//____________________________________________________________________

AtlTriggerItem::~AtlTriggerItem() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlTriggerItem::Clear(Option_t *option) {
    //
    // Clear this object
    //
}

  


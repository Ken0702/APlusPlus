//____________________________________________________________________
//
// Matching of trigger objects to offline reconstructed objects
// 

//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2010 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlTriggerMatch
#include <AtlTriggerMatch.h>
#endif
#include <AtlTrigger.h>

#ifndef __CINT__
ClassImp(AtlTriggerMatch);
#endif

//____________________________________________________________________

AtlTriggerMatch::AtlTriggerMatch() {
    //
    // Default constructor
    //
    fL1Items = new TRefArray;
    fHLTItems = new TRefArray;
}

//____________________________________________________________________

AtlTriggerMatch::~AtlTriggerMatch() {
    //
    // Default destructor
    //
    delete fL1Items;  fL1Items  = 0;
    delete fHLTItems; fHLTItems = 0;
}

//____________________________________________________________________


void AtlTriggerMatch::Clear(Option_t *option) {
    //
    // Clear this object
    //
    delete fL1Items;  fL1Items  = 0;
    delete fHLTItems; fHLTItems = 0;

    
}

//____________________________________________________________________

Bool_t AtlTriggerMatch::HasMatchedL1(const char* L1ItemName, Int_t RunNr,
				     AtlTrigger *trigger) const {
    //
    // Test if this object has matched the given L1 trigger item
    //
    AtlTriggerItem *probe = trigger->GetL1Item(L1ItemName, RunNr, kFALSE);
    
    TIter next(fL1Items);
    AtlTriggerItem *item = 0;
    while ( (item = (AtlTriggerItem*)next()) ) {
	if ( item == probe ) return kTRUE;
    }
    return kFALSE;
}

//____________________________________________________________________

Bool_t AtlTriggerMatch::HasMatchedHLT(const char* HLTItemName, Int_t RunNr,
				      AtlTrigger *trigger) const {
    //
    // Test if this object has matched the given HLT trigger item
    //
    AtlTriggerItem *probe = trigger->GetHLTItem(HLTItemName, RunNr, kFALSE);
    
    TIter next(fHLTItems);
    AtlTriggerItem *item = 0;
    while ( (item = (AtlTriggerItem*)next()) ) {
	if ( item == probe ) return kTRUE;
    }
    return kFALSE;
}

//____________________________________________________________________

void AtlTriggerMatch::AddL1Match(AtlTriggerItem *item) {
    //
    // Add matched L1 trigger item.
    // The method ensures that a given item is stored only once.
    //
    if ( fL1Items->FindObject(item) == 0 ) fL1Items->Add(item);
}

//____________________________________________________________________

void AtlTriggerMatch::AddHLTMatch(AtlTriggerItem *item) {
    //
    // Add matched HLT trigger item.
    // The method ensures that a given item is stored only once.
    //
    if ( fHLTItems->FindObject(item) == 0 ) fHLTItems->Add(item);
}

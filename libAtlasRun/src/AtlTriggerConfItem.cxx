//____________________________________________________________________
//
// ATLAS Trigger database entry
// 

//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlTriggerConfItem
#include <AtlTriggerConfItem.h>
#endif
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlTriggerConfItem);
#endif

//____________________________________________________________________

AtlTriggerConfItem::AtlTriggerConfItem() {
  //
  // Default constructor
  //
}

//____________________________________________________________________

AtlTriggerConfItem::AtlTriggerConfItem(const char* name,
				       const char* title, Int_t bit) :
    TNamed(name, title), fBit(bit) {
    //
    // Normal constructor
    //
}

//____________________________________________________________________

AtlTriggerConfItem::~AtlTriggerConfItem() {
    //
    // Default destructor
    //
}

//____________________________________________________________________

void AtlTriggerConfItem::Print(Option_t *option) const {
    //
    // Print item configuration
    //    ItemName   BitNumber   ItemTitle
    //
    cout.width(30); cout << GetName();
    cout << " :   Bit";
    cout.width(5);  cout << fBit;
    cout << "   " << GetTitle() << endl;
}


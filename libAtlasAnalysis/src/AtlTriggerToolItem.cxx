//____________________________________________________________________
//
// Helper class for the trigger tool
// 

//  
// Author: Charlotte Santer <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2010 (C) Charlotte Santer
//
#ifndef ATLAS_AtlTriggerToolItem
#include <AtlTriggerToolItem.h>
#endif

#ifndef __CINT__
ClassImp(AtlTriggerToolItem);
#endif

//____________________________________________________________________

AtlTriggerToolItem::AtlTriggerToolItem(const char* name,
				       const char* title,
				       ETriggerLevel level,
				       ETriggerType type) :
    TNamed(name, title) {
    //
    // Default constructor
    //
    fTriggerLevel = level;
    fTriggerType  = type;
    fHistNPassPt  = 0;
    fHistNPassPhi = 0;
    fHistNPassEta = 0;
    fHistNTotPt  = 0;
    fHistNTotPhi = 0;
    fHistNTotEta = 0;
}

//____________________________________________________________________

AtlTriggerToolItem::~AtlTriggerToolItem() {
    //
    // Default denstructor
    //
}



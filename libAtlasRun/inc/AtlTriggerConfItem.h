//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlTriggerConfItem
#define ATLAS_AtlTriggerConfItem
#ifndef ROOT_TNamed
#include <TNamed.h>
#endif

class AtlTriggerConfItem : public TNamed {

private:

public:
    Int_t  fBit;  // Bit number of this trigger within the trigger bits array
    
    AtlTriggerConfItem();
    AtlTriggerConfItem(const char* name, const char* title, Int_t bit);
    virtual ~AtlTriggerConfItem();
    virtual void Print(Option_t *option = "") const;
    
    ClassDef(AtlTriggerConfItem,1) // ATLAS trigger dbase item
};
#endif

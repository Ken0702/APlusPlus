//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlTriggerItem
#define ATLAS_AtlTriggerItem
#ifndef ROOT_TObject
#include <TObject.h>
#endif

class AtlTriggerItem : public TObject {

private:
    Bool_t  fPassed;      // Trigger decision
    Float_t fPrescale;    // Pre-scale factor for this trigger
    Float_t fPassThrough; // For non-zero values of this variable
			  // events are accepted even if the trigger
			  // decision was negative. The number gives
			  // the pre-scale factor of the
			  // force-accepted events
    
public:
    AtlTriggerItem();
    AtlTriggerItem(Bool_t Passed, Float_t Prescale, Float_t PassThrough);
    virtual ~AtlTriggerItem();
    virtual void Clear(Option_t *option = "");
    
    inline Bool_t HasPassed() const { return fPassed; }
    inline Float_t GetPrescale() const { return fPrescale; }
    inline Float_t GetPassThrough() const { return fPassThrough; }
    
    ClassDef(AtlTriggerItem,1) // ATLAS trigger item
};
#endif


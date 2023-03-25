//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2010 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlTriggerMatch
#define ATLAS_AtlTriggerMatch
#ifndef ROOT_TRefArray
#include <TRefArray.h>
#endif
#ifndef ATLAS_AtlTriggerItem
#include <AtlTriggerItem.h>
#endif

class AtlTrigger;

class AtlTriggerMatch {

private:
    TRefArray  *fL1Items;  // Matched L1 trigger items
    TRefArray  *fHLTItems; // Matched HLT trigger items
    
public:
    AtlTriggerMatch();
    virtual ~AtlTriggerMatch();
    virtual void Clear(Option_t *option = "");
    Bool_t HasMatchedL1(const char* L1ItemName, Int_t RunNr,
			AtlTrigger *trigger) const;
    Bool_t HasMatchedHLT(const char* HLTItemName, Int_t RunNr,
			 AtlTrigger *trigger) const;
    void AddL1Match(AtlTriggerItem *item);
    void AddHLTMatch(AtlTriggerItem *item);

    inline const TRefArray* GetMatchedL1Items()  const { return fL1Items;  }
    inline const TRefArray* GetMatchedHLTItems() const { return fHLTItems; }
    inline Int_t GetN_L1Matches()  const { return fL1Items->GetEntries();  }
    inline Int_t GetN_HLTMatches() const { return fHLTItems->GetEntries(); }
    inline Int_t GetN_TriggerMatches() const { return GetN_L1Matches() + GetN_HLTMatches(); }
    
    ClassDef(AtlTriggerMatch,2) // Match of trigger objects to offline reconstructed objects
};
#endif


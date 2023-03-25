//  
// Author: Charlotte Santer <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2010 (C) Charlotte Santer
//
#ifndef ATLAS_AtlTriggerToolItem
#define ATLAS_AtlTriggerToolItem
#ifndef ROOT_TNamed
#include <TNamed.h>
#endif
#ifndef ROOT_TH1F
#include <TH1F.h>
#endif

class AtlTriggerToolItem : public TNamed {

  private:
    
  public:
    enum ETriggerLevel {
	kLV1,
	kLV2,
	kEF,
	kUnknownLevel
    };
    enum ETriggerType {
	kElectron,
	kMuon,
	kJet,
	kMissingEt,
	kTau,
	kPhoton,
	kEM,
	kUnknownType
    };
    
    ETriggerLevel fTriggerLevel; // Level of tigger item
    ETriggerType  fTriggerType;  // Type of trigger item
    TH1F         *fHistNPassPt;  // Pointer to Pt histogram for passed events
    TH1F         *fHistNPassPhi; // Pointer to Phi histogram for passed events
    TH1F         *fHistNPassEta; // Pointer to Eta histogram for passed events
    TH1F         *fHistNTotPt;   // Pointer to Pt histogram for all events
    TH1F         *fHistNTotPhi;  // Pointer to Phi histogram for all events
    TH1F         *fHistNTotEta;  // Pointer to Eta histogram for all events
    
    AtlTriggerToolItem(const char* name, const char* title,
		       ETriggerLevel level, ETriggerType type);
    virtual ~AtlTriggerToolItem();
    
    ClassDef(AtlTriggerToolItem,0) // Helper class for the trigger tool
};
#endif


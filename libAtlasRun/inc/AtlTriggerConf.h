//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlTriggerConf
#define ATLAS_AtlTriggerConf
#ifndef ROOT_TObject
#include <TObject.h>
#endif
#ifndef ROOT_TTree
#include <TTree.h>
#endif
#ifndef ROOT_TSystem
#include <TSystem.h>
#endif
#ifndef ATLAS_AtlTriggerConfItem
#include <AtlTriggerConfItem.h>
#endif

class THashTable;
class TClonesArray;
class TDirectory;

class AtlTriggerConf : public TObject {

  public:
    static const Int_t fgL1MaxBits;
    static const Int_t fgHLTMaxBits;
    static const char* fgTreeName;
    static const char* fgTreeTitle;
    enum ETrigger { kL1, kHLT };
    
  private:
    static AtlTriggerConf *fgInstance; // Trigger configuration singleton
    TTree         *fConfigTree;        // Tree containing the configuration
    Int_t          fN_L1TriggerItems;  // No. of items filled in L1 clones array
                                       // (internal use only)
    Int_t          fN_HLTTriggerItems; // No. of items filled in HLT clones array
                                       // (internal use only)
    TClonesArray  *fL1TriggerItemsCA;  // Array of L1 trigger items in config tree
    TClonesArray  *fHLTTriggerItemsCA; // Array of HLT trigger items in config tree
    THashTable    *fL1TriggerItemsHT;  // Hash table of L1 trigger items for fast access
    THashTable    *fHLTTriggerItemsHT; // Hash table of HLT trigger items for fast access
    Int_t          fRunStart;          // Begin of run range
                                       // valid for the current configuration
    Int_t          fRunEnd;            // End of run range
                                       // valid for the current configuration

  public:
    AtlTriggerConf();
    virtual ~AtlTriggerConf();
    static AtlTriggerConf* Instance();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Option_t *option = "ALL");
    AtlTriggerConf* LoadTree(TDirectory *dir);
    TTree* CreateTree();
    AtlTriggerConfItem* GetTriggerItem(ETrigger trigger,
				       const char* name,
				       Int_t RunNr);
    AtlTriggerConfItem* GetTriggerItem(ETrigger trigger,
				       const char* name,
				       Bool_t CheckValidity = kFALSE);
    void ReadConfiguration(Int_t RunNr);
    AtlTriggerConfItem* AddTriggerItem(ETrigger trigger,
				       const char* name,
				       const char* title,
				       Int_t bit);
    void WriteConfiguration();
    void Rehash();
    TClonesArray* GetTriggerItemsCA(ETrigger trigger) const;
    Int_t GetN_TriggerItems(ETrigger trigger, Int_t RunNr);
    Int_t GetN_TriggerItems(ETrigger trigger) const;
    const char* GetL1ItemName(Int_t Bit, Int_t RunNr);
    const char* GetHLTItemName(Int_t Bit, Int_t RunNr);
    const char* GetL1ItemName(Int_t Bit);
    const char* GetHLTItemName(Int_t Bit);

    inline TTree* GetConfigTree() const { return fConfigTree; }
    inline Int_t GetL1TriggerBit(const char* name, Int_t RunNr) {
	//
	// Return bit number of L1 trigger item <name>.
	// Check if the present configuration is still valid for the
	// given run - load appropiate config otherwise.
	//
	// Return -1 in case the given item could not be found/
	//
	AtlTriggerConfItem *item = GetTriggerItem(kL1, name, RunNr);
	return ( item == 0 ) ? -1 : item->fBit;
    }
    inline Int_t GetHLTTriggerBit(const char* name, Int_t RunNr) {
	//
	// Return bit number of HLT trigger item <name>
	// Check if the present configuration is still valid for the
	// given run - load appropiate config otherwise.
	//
	// Return -1 in case the given item could not be found/
	//
	AtlTriggerConfItem *item  = GetTriggerItem(kHLT, name, RunNr);
	return ( item == 0 ) ? -1 : item->fBit;
    }
    inline Int_t GetL1TriggerBit(const char* name) {
	//
	// Return bit number of L1 trigger item <name>
	// from the current configuration.
	//
	return GetTriggerItem(kL1, name)->fBit;
    }
    inline Int_t GetHLTTriggerBit(const char* name) {
	//
	// Return bit number of HLT trigger item <name>
	// from the current configuration.
	//
	return GetTriggerItem(kHLT, name)->fBit;
    }
    inline Int_t GetRunStart() const { return fRunStart; }
    inline Int_t GetRunEnd() const { return fRunEnd; }
    inline void SetRunStart(Int_t RunStart) {
	// Set begin of valid run range for the current configuration
	fRunStart = RunStart;
    }
    inline void SetRunEnd(Int_t RunEnd) {
	// Set end of valid run range for the current configuration
	// Check that the given end run number is larger than the already
	// given start run number of the run range
	if ( RunEnd < fRunStart ) {
	    Error("WriteConfig", "Given end run number (%d) is smaller than the start run number of the configuration run range (%d). Abort!", RunEnd, fRunStart);
	    gSystem->Abort();
	}
	fRunEnd = RunEnd;
    }
    
  private:
    THashTable* GetTriggerItemsHT(ETrigger trigger) const;
    void SetN_TriggerItems(ETrigger trigger, Int_t NItems);
    
    ClassDef(AtlTriggerConf,0) // ATLAS trigger configuration dbase
};
#endif

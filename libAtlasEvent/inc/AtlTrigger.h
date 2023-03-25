//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlTrigger
#define ATLAS_AtlTrigger
#ifndef ROOT_TObject
#include <TObject.h>
#endif
#ifndef ROOT_TClonesArray
#include <TClonesArray.h>
#endif
#ifndef ATLAS_AtlTriggerConf
#include <AtlTriggerConf.h>
#endif
#ifndef ATLAS_AtlTriggerItem
#include <AtlTriggerItem.h>
#endif

class AtlTrigger;

class AtlTrigger : public TObject {

  public:
    enum ETriggerStream { kPhysics      = BIT(0), // required for Run-2
			  kMuons        = BIT(1),
		          kEgamma       = BIT(2),
		          kJetTauEtmiss = BIT(3),
		          kExpress      = BIT(4),
			  kMinBias      = BIT(5),
			  kRNDM         = BIT(6),
			  kL1Calo       = BIT(7),
			  kL1CaloEM     = BIT(8),
			  kMuonswBeam   = BIT(9),
			  kBPTX         = BIT(10)
    };
    
  private:
    ETriggerStream  fStream;   // Trigger stream of the current event
    TClonesArray   *fL1Items;  //-> Array of all level 1 trigger items
    TClonesArray   *fHLTItems; //-> Array of all higher level trigger items

    const char *fL1NameCache;  //! Last inquired L1 item name for caching reasons
    Int_t       fL1BitCache;   //! Cache for last inquired L1 item bit number
    const char *fHLTNameCache; //! Last inquired HLT item name for caching reasons
    Int_t       fHLTBitCache;  //! Cache for last inquired HLT item bit number
    static AtlTriggerConf *fgTriggerConf;  //! Trigger configuration dbase
    
public:
    AtlTrigger();
    virtual ~AtlTrigger();
    virtual void Clear(Option_t *option = "");
    virtual void Print(Int_t RunNr,
		       Option_t *option = "") const;
    
    inline ETriggerStream GetStream() const {
	// Returns trigger stream of the current event
	return fStream;
    }
    inline AtlTriggerItem* GetL1Item(const char* name, Int_t RunNr,
				     Bool_t UseCache = kFALSE) {
	//
	// Returns pointer to requested level 1 trigger item <name>.
	// Check if the current configuration is still valid for the
	// given run - load appropriate configuration otherwise.
	//
	Int_t bit = GetL1TriggerBit(name, RunNr, UseCache);
	return ( bit < 0 ) ? 0 : (AtlTriggerItem*)fL1Items->At(bit);
    }
    inline AtlTriggerItem* GetHLTItem(const char* name, Int_t RunNr,
				      Bool_t UseCache = kFALSE) {
	//
	// Returns pointer to requested HLT trigger item <name>.
	// Check if the current configuration is still valid for the
	// given run - load appropriate configuration otherwise.
	//
	Int_t bit = GetHLTTriggerBit(name, RunNr, UseCache);
	return ( bit < 0 ) ? 0 : (AtlTriggerItem*)fHLTItems->At(bit);
    }
    inline AtlTriggerItem* GetL1Item(const char* name) {
	//
	// Returns pointer to requested level 1 trigger item <name>
	// from the current configuration w/o using the cache.
	//
	return (AtlTriggerItem*)fL1Items->At(GetL1TriggerBit(name));
    }
    inline AtlTriggerItem* GetHLTItem(const char* name) {
	//
	// Returns pointer to requested HLT trigger item <name>.
	// from the current configuration w/o using the cache.
	//
	return (AtlTriggerItem*)fHLTItems->At(GetHLTTriggerBit(name));
    }
    inline Bool_t HasPassedL1(const char* name, Int_t RunNr, Bool_t UseCache = kTRUE) {
	// Returns decision for level 1 trigger item <name>
	// and given run number
	return GetL1Item(name, RunNr, UseCache)->HasPassed();
    }
    inline Bool_t HasPassedHLT(const char* name, Int_t RunNr, Bool_t UseCache = kTRUE) {
	// Returns decision for higher level trigger item <name>
	// and given run number
	return GetHLTItem(name, RunNr, UseCache)->HasPassed();
    }
    inline Bool_t IsPhysicsStream()      const { return fStream & kPhysics; }
    inline Bool_t IsMuonsStream()        const { return fStream & kMuons; }
    inline Bool_t IsEgammaStream()       const { return fStream & kEgamma; }
    inline Bool_t IsJetTauEtmissStream() const { return fStream & kJetTauEtmiss; }
    inline Bool_t IsExpressStream()      const { return fStream & kExpress; }
    inline Bool_t IsMinBiasStream()      const { return fStream & kMinBias; }
    inline Bool_t IsRNDMStream()         const { return fStream & kRNDM; }
    inline Bool_t IsL1CaloStream()       const { return fStream & kL1Calo; }
    inline Bool_t IsL1CaloEMStream()     const { return fStream & kL1CaloEM; }
    inline Bool_t IsMuonswBeamStream()   const { return fStream & kMuonswBeam; }
    inline Bool_t IsBPTXStream()         const { return fStream & kBPTX; }

    inline void SetStream(ETriggerStream stream) {
	fStream = stream;
    }
    inline AtlTriggerItem* SetL1Item(Int_t Bit, Bool_t Passed,
				     Float_t Prescale, Float_t PassThrough) {
      // Set level 1 trigger item for given bit number
      TClonesArray &items = *fL1Items;
      AtlTriggerItem *item = new(items[Bit])
	AtlTriggerItem(Passed, Prescale, PassThrough);
      return item;
    }
    inline AtlTriggerItem* SetHLTItem(Int_t Bit, Bool_t Passed,
				      Float_t Prescale, Float_t PassThrough) {
      // Set higher level trigger item for given bit number
      TClonesArray &items = *fHLTItems;
      AtlTriggerItem *item = new(items[Bit])
	AtlTriggerItem(Passed, Prescale, PassThrough);
      return item;
    }
    inline static AtlTriggerConf* GetTriggerConf() { return fgTriggerConf; }
    inline static void SetConfig(AtlTriggerConf *conf)
    { fgTriggerConf = conf; }
    inline TClonesArray* GetL1Items()  const { return fL1Items; }
    inline TClonesArray* GetHLTItems() const { return fHLTItems; }

 private:
    Int_t GetL1TriggerBit(const char *name, Int_t RunNr, Bool_t UseCache = kTRUE);
    Int_t GetHLTTriggerBit(const char *name, Int_t RunNr, Bool_t UseCache = kTRUE);
    inline Int_t GetL1TriggerBit(const char *name) {
	//
	// Get trigger bit for the given item by name from the
	// currently loaded configuration.
	//
	return fgTriggerConf->GetL1TriggerBit(name);
    }
    inline Int_t GetHLTTriggerBit(const char *name) {
	//
	// Get trigger bit for the given item by name from the
	// currently loaded configuration.
	//
	return fgTriggerConf->GetHLTTriggerBit(name);
    }
    void LoadConfig();
    
    ClassDef(AtlTrigger,3) // Atlas trigger
};
#endif


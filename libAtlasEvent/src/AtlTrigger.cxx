//____________________________________________________________________
//
// Atlas trigger class
// 
// Make sure, that you activate the Trigger and EventHeader branches,
// in order to use trigger items:
//
// fTree->SetBranchStatus("fTrigger*",     kTRUE);
// fTree->SetBranchStatus("fEventHeader*", kTRUE);
//
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlTrigger
#include <AtlTrigger.h>
#endif
#include <TDirectory.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlTrigger);
#endif

AtlTriggerConf *AtlTrigger::fgTriggerConf = 0;

//____________________________________________________________________

AtlTrigger::AtlTrigger() {
    //
    // Default constructor
    //
    fL1NameCache  = "";
    fHLTNameCache = "";
    fL1BitCache  = 0;
    fHLTBitCache = 0;
    fL1Items  = new TClonesArray("AtlTriggerItem", AtlTriggerConf::fgL1MaxBits);
    fHLTItems = new TClonesArray("AtlTriggerItem", AtlTriggerConf::fgHLTMaxBits);
}

//____________________________________________________________________

AtlTrigger::~AtlTrigger() {
    //
    // Default destructor
    //
    fL1Items->Clear("C");  delete fL1Items;
    fHLTItems->Clear("C"); delete fHLTItems;
}

//____________________________________________________________________

void AtlTrigger::Clear(Option_t *option) {
    //
    // Clear this object: re-set all bits
    //
    fL1Items->Clear(option);
    fHLTItems->Clear(option);
}

//____________________________________________________________________

void AtlTrigger::Print(Int_t RunNr, Option_t *option) const {
    //
    // Print trigger decisions for configuration given by run number
    //
    AtlTriggerConfItem *item_conf = 0;
    AtlTriggerItem *item = 0;
    
    // Level 1
    cout << endl
	 << "Level 1 Trigger" << endl
	 << "---------------------------------------------------------------" << endl
	 << "L1 Item                             Passed Prescale PassThrough" << endl
	 << "---------------------------------------------------------------" << endl;
    for ( Int_t i = 0; i < fgTriggerConf->GetN_TriggerItems(AtlTriggerConf::kL1, RunNr);
	  i++ ) {
	item_conf = (AtlTriggerConfItem*)
	    fgTriggerConf->GetTriggerItemsCA(AtlTriggerConf::kL1)->At(i);
	item = (AtlTriggerItem*)fL1Items->At(item_conf->fBit);
	cout.width(35); cout << item_conf->GetName();
	cout.width(6);  cout << item->HasPassed();
	cout << " "; cout.width(8);  cout << item->GetPrescale();
	cout << " "; cout.width(11); cout << item->GetPassThrough() << endl;
    }
    cout << "---------------------------------------------------------------" << endl << endl;
    
    // Higher Level
    cout << endl
	 << "Higher Level Trigger" << endl
	 << "---------------------------------------------------------------" << endl
	 << "HLT Item                            Passed Prescale PassThrough" << endl
	 << "---------------------------------------------------------------" << endl;
    for ( Int_t i = 0; i < fgTriggerConf->GetN_TriggerItems(AtlTriggerConf::kHLT, RunNr);
	  i++ ) {
	item_conf = (AtlTriggerConfItem*)
	    fgTriggerConf->GetTriggerItemsCA(AtlTriggerConf::kHLT)->At(i);
	item = (AtlTriggerItem*)fHLTItems->At(item_conf->fBit);
	cout.width(35); cout << item_conf->GetName();
	cout.width(6);  cout << item->HasPassed();
	cout << " "; cout.width(8);  cout << item->GetPrescale();
	cout << " "; cout.width(11); cout << item->GetPassThrough() << endl;
    }
    cout << "---------------------------------------------------------------" << endl << endl;

    // Streams
    cout << endl
         << "Trigger Streams" << endl
         << "----------------------------------------------------" << endl
	 << "Stream                                      IsPassed" << endl
	 << "physiscs                                      " << IsPhysicsStream() << endl
         << "muons                                         " << IsMuonsStream() << endl
         << "egamma                                        " << IsEgammaStream() << endl
         << "jetTauEtmiss                                  " << IsJetTauEtmissStream() << endl
         << "express                                       " << IsExpressStream() << endl
         << "minbias                                       " << IsMinBiasStream() << endl
         << "rndm                                          " << IsRNDMStream() << endl
         << "l1calo                                        " << IsL1CaloStream() << endl
         << "l1caloem                                      " << IsL1CaloEMStream() << endl
         << "muonswbeam                                    " << IsMuonswBeamStream() << endl
         << "bptx                                          " << IsBPTXStream() << endl;
    cout << "----------------------------------------------------" << endl << endl;

}

//____________________________________________________________________

Int_t AtlTrigger::GetL1TriggerBit(const char *name, Int_t RunNr,
				  Bool_t UseCache) {
    //
    // Return bit number for given L1 item and run number.
    //
    // If enabled, the cache will be checked first and updated if
    // neccessary. The caching is intended for AtlEvent::HasPassedL1()
    // to have a very fast event selection based on the given trigger
    // decision.
    //
    // In case no trigger configuration had been loaded yet, try to
    // get it from the current directory
    //

    // Check cache
    if ( UseCache == kTRUE && strcmp(name, fL1NameCache) == 0 )
	return fL1BitCache;

    // Config already loaded ?
    if ( fgTriggerConf == 0 ) LoadConfig();

    // Get bit
    Int_t L1Bit = fgTriggerConf->GetL1TriggerBit(name, RunNr);
    if ( UseCache == kTRUE ) {
	fL1NameCache = name;
	fL1BitCache = L1Bit;
    }
    return L1Bit;
}

//____________________________________________________________________

Int_t AtlTrigger::GetHLTTriggerBit(const char *name, Int_t RunNr,
				   Bool_t UseCache) {
    //
    // Return bit number for given HLT item and run number.
    //
    // If enabled, the cache will be checked first and updated if
    // neccessary. The caching is intended for
    // AtlEvent::HasPassedHLT() to have a very fast event selection
    // based on the given trigger decision.
    //
    // In case no trigger configuration had been loaded yet, try to
    // get it from the current directory
    //
    // In case the given item could not be found, -1 will be returned.
    //
    
    // Check cache
    if ( UseCache == kTRUE && strcmp(name, fHLTNameCache) == 0 )
	return fHLTBitCache;

    // Config already loaded ?
    if ( fgTriggerConf == 0 ) LoadConfig();

    // Get bit
    Int_t HLTBit = fgTriggerConf->GetHLTTriggerBit(name, RunNr);
    if ( UseCache == kTRUE ) {
	fHLTNameCache = name;
	fHLTBitCache = HLTBit;
    }
    return HLTBit;
}

//____________________________________________________________________

void AtlTrigger::LoadConfig() {
    //
    // Load trigger configuration from current directory.
    // Abort in case no configuration is found
    //
    AtlTriggerConf *trig_conf = AtlTriggerConf::Instance()
	->LoadTree(gDirectory);
    if ( trig_conf == 0 ) {
	Error("LoadConfig",
	      "Cannot find trigger configuration in current directory. Abort!");
	gSystem->Abort(0);
    }
    SetConfig(trig_conf);
}

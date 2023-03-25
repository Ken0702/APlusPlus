//____________________________________________________________________
//
// ATLAS trigger configuration database + interface singleton
//
// This class provides an interface to the trigger configuration
// database which is stored inside a Root tree of its own. This tree
// is stored together with its corresponding A++ event tree in one
// .root file.
//
// The trigger configuration tree contains multiple configurations
// valid for certain run ranges. Each configuration consists of two
// run numbers indicating the valid run range of the configuration,
// and three maps (level 1, level 2 and event filter). These maps are
// lists of pair-wise information, the association between a trigger
// item name (note that in this context 'item' means 'chain' as well)
// and its corresponding trigger bit number. The bit numbers are
// needed for accessing the actual trigger information in an A++
// event.
//
//
//    A++ .root file
//   +--------------------------+
//   | +------------------+     |           +-----------------------------+
//   | | TriggerConf Tree |---------------->| Trigger Configuration (RAM) |
//   | +------------------+     |           +-----------------------------+
//   |                          |                          |<------------------ Run Number
//   | +------------------+     |               +----------V----------+
//   | | A++ Event Tree   |-------------------->| Bit No. / Item Name |<------- Trigger Item
//   | +------------------+     |               +--------x------------+
//   +-------------|------------+                     |
//                 |                                  |
//                 |                                  V
//                 |                             +-----------+
//                 +---------------------------->| A++ Event |----------------> Trigger Decision
//                                               +-----------+
//
//
// For fast access, copies of the maps (which are of type TClonesArray
// inside the tree) are being kept in form of hash tables (THashTable)
// in RAM.
//
//
// Usage examples:
// ===============
//
// (1) Create a new configuration dbase:
//
//    AtlTriggerConf *conf = new AtlTriggerConf;
//    TTree *t_conf = conf->CreateTree();
//
// (2) Build a configuration (in memory)
//
//    conf->Clear();
//    conf->AddTriggerItem(); // Item 1
//    conf->AddTriggerItem(); // Item 2
//    ...
//    conf->Rehash();
//
// (3) Write the current configuration (RAM) to the tree
//     for the run range RunStart -> RunEnd
//
//    conf->WriteConfiguration(RunStart, RunEnd);
//
//
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlTriggerConf
#include <AtlTriggerConf.h>
#endif
#include <THashTable.h>
#include <TDirectory.h>
#include <TClonesArray.h>
#include <iostream>

using namespace std;

#ifndef __CINT__
ClassImp(AtlTriggerConf);
#endif

AtlTriggerConf *AtlTriggerConf::fgInstance = 0;
const Int_t AtlTriggerConf::fgL1MaxBits  = 256;
const Int_t AtlTriggerConf::fgHLTMaxBits = 512;
const char* AtlTriggerConf::fgTreeName  = "t_trig_config";
const char* AtlTriggerConf::fgTreeTitle = "Trigger Configuration Dbase";

//____________________________________________________________________

AtlTriggerConf::AtlTriggerConf() {
    //
    // Default constructor
    //

    // Protect against multiple instances
    if ( fgInstance != 0 ) {
	Error("AtlTriggerConf", "Multiple instances of this object not allowed. Use AtlTriggerConf::Instance() instead!");
	gSystem->Abort(0);
    }
    fgInstance = this;
    
    // Defaults
    fL1TriggerItemsCA  = new TClonesArray("AtlTriggerConfItem", fgL1MaxBits);
    fHLTTriggerItemsCA = new TClonesArray("AtlTriggerConfItem", fgHLTMaxBits);
    fL1TriggerItemsHT  = new THashTable(fgL1MaxBits);
    fHLTTriggerItemsHT = new THashTable(fgHLTMaxBits);
    fRunStart = 0;
    fRunEnd   = 0;
    fConfigTree = 0;
    fN_L1TriggerItems  = 0;
    fN_HLTTriggerItems = 0;
}

//____________________________________________________________________

AtlTriggerConf::~AtlTriggerConf() {
    //
    // Default destructor
    //
    if ( fL1TriggerItemsCA != 0 ) {
	fL1TriggerItemsCA->Delete();
	delete fL1TriggerItemsCA;
    }
    if ( fHLTTriggerItemsCA != 0 ) {
	fHLTTriggerItemsCA->Delete();
	delete fHLTTriggerItemsCA;
    }
    if ( fL1TriggerItemsHT != 0 ) {
	fL1TriggerItemsHT->Delete();
	delete fL1TriggerItemsHT;
    }
    if ( fHLTTriggerItemsHT != 0 ) {
	fHLTTriggerItemsHT->Delete();
	delete fHLTTriggerItemsHT;
    }
    if ( fConfigTree != 0 ) delete fConfigTree;
    
    fgInstance = 0;
}

//____________________________________________________________________

AtlTriggerConf* AtlTriggerConf::Instance() {
    //
    // Create trigger db singleton
    //
    return ( fgInstance != 0 ) ? fgInstance : new AtlTriggerConf();
}

//____________________________________________________________________

void AtlTriggerConf::Clear(Option_t *option) {
    //
    // Clear the current configuration
    //
    // Clear the clone arrays and the hash tables and remove their
    // contents.
    //
    fL1TriggerItemsHT->Clear();
    fHLTTriggerItemsHT->Clear();
    fL1TriggerItemsCA->Clear("C");
    fHLTTriggerItemsCA->Clear("C");
    fN_L1TriggerItems = 0;
    fN_HLTTriggerItems = 0;
}
    
//____________________________________________________________________

AtlTriggerConf* AtlTriggerConf::LoadTree(TDirectory *dir) {
    //
    // Load configuration tree from given directory
    //
    fConfigTree = (TTree*)dir->Get(fgTreeName);
    if ( fConfigTree == 0 ) {
	Error("LoadTree",
	      "Trigger configuration tree \"%s\" not found in directory \"%s\".",
	      fgTreeName, dir->GetName());
	return 0;
    }
    
    // Set branches
    fConfigTree->SetBranchAddress("L1TriggerItems",    &fL1TriggerItemsCA);
    fConfigTree->SetBranchAddress("HLTTriggerItems",   &fHLTTriggerItemsCA);
    fConfigTree->SetBranchAddress("N_L1TriggerItems",  &fN_L1TriggerItems);
    fConfigTree->SetBranchAddress("N_HLTTriggerItems", &fN_HLTTriggerItems);
    fConfigTree->SetBranchAddress("RunRangeStart",     &fRunStart);
    fConfigTree->SetBranchAddress("RunRangeEnd",       &fRunEnd);

    // Initialize
    fRunStart = 0; fRunEnd = 0;
    
    // Print info
    Info("LoadTree",
	 "Trigger configuration tree \"%s\" successfully loaded from directory \"%s\".",
	 fgTreeName, dir->GetName());
    fgInstance->Print("LIST");

    // Load first entry
    fConfigTree->GetEntry(0);
    ReadConfiguration(fRunStart);
    
    return fgInstance;
}

//____________________________________________________________________

TTree* AtlTriggerConf::CreateTree() {
    //
    // Create the configuration tree and its branches
    // 
    fConfigTree = new TTree(fgTreeName, fgTreeTitle);

    // Create branches
    fConfigTree->Branch("L1TriggerItems",  "TClonesArray", &fL1TriggerItemsCA);
    fConfigTree->Branch("HLTTriggerItems", "TClonesArray", &fHLTTriggerItemsCA);
    fConfigTree->Branch("N_L1TriggerItems",  &fN_L1TriggerItems, "N_L1TriggerItems/I");
    fConfigTree->Branch("N_HLTTriggerItems", &fN_HLTTriggerItems, "N_HLTTriggerItems/I");
    fConfigTree->Branch("RunRangeStart", &fRunStart, "RunRangeStart/I");
    fConfigTree->Branch("RunRangeEnd",   &fRunEnd,  "RunRangeEnd/I");
    
    return fConfigTree;
}

//____________________________________________________________________

AtlTriggerConfItem* AtlTriggerConf::GetTriggerItem(ETrigger trigger,
						   const char* name,
						   Int_t RunNr) {
    //
    // Find trigger item by name for given run nmber
    //
    // Check if current configuration is still valid
    // Load new configuration otherwise.
    //
    if ( RunNr < fRunStart || RunNr > fRunEnd )
	ReadConfiguration(RunNr);
    return GetTriggerItem(trigger, name);
}
    
//____________________________________________________________________
    
AtlTriggerConfItem* AtlTriggerConf::GetTriggerItem(ETrigger trigger,
						   const char* name,
						   Bool_t CheckValidity) {
    //
    // Find trigger item by name using the currently loaded
    // configuration.
    //
    // If CheckValidity == true the program is aborted in case the
    // item is not found (default is false).
    //
    AtlTriggerConfItem *item = (AtlTriggerConfItem*)GetTriggerItemsHT(trigger)
	->FindObject(name);
    if ( CheckValidity && item == 0 ) {
	if ( trigger == kL1 ) {
	    Error("GetTriggerItem", "Trigger item %s not found in L1 list", name);
	} else if ( trigger == kHLT ) {
	    Error("GetTriggerItem", "Trigger item %s not found in HLT list", name);
	}
	gSystem->Abort(0);
    }
    return item;
}

//____________________________________________________________________

AtlTriggerConfItem* AtlTriggerConf::AddTriggerItem(ETrigger trigger,
						   const char* name,
						   const char* title,
						   Int_t bit) {
    //
    // Add item to the clone arrays of the current configuration for
    // the given trigger.
    //
    // Before adding the first item, the Clear() function has to be
    // called in order to clean the arrays and tablles from the
    // previous configuration.
    //
    // Note that the hash tables are NOT updated. This is done with
    // the help Rehash() member function, which needs to be called
    // when all trigger items are added
    //
    TClonesArray &items = *GetTriggerItemsCA(trigger);
    Int_t NItems = GetN_TriggerItems(trigger);
    AtlTriggerConfItem *item = new(items[NItems])
	AtlTriggerConfItem(name, title, bit);
    SetN_TriggerItems(trigger, NItems+1);
    return item;
}

//____________________________________________________________________

void AtlTriggerConf::ReadConfiguration(Int_t RunNr) {
    //
    // Read configuration for the given run number from the tree and
    // update the hash tables. Any previous configuration is removed
    // from memory
    //

    // Clear current configuration
    Clear();
    
    // Config tree already loaded ?
    if ( fConfigTree == 0 ) {
	Error("ReadConfiguration", "No configuration tree loaded. Abort!");
	gSystem->Abort(0);
    }

    // Search the tree for the configuration needed
    for ( Int_t i = 0; i < fConfigTree->GetEntries(); i++ ) {
	fConfigTree->GetEntry(i);
	if ( (RunNr >= fRunStart) && (RunNr <= fRunEnd) ) {
	    
	    // Fill hash tables with the information from the clone arrays.
	    // Re-hashing is done by THashTable::AddAll()
	    fL1TriggerItemsHT->AddAll(fL1TriggerItemsCA);
	    fHLTTriggerItemsHT->AddAll(fHLTTriggerItemsCA);
	    cout << "<AtlTriggerConf::ReadConfiguration> Read trigger config for run range "
		 << fRunStart << " -> " << fRunEnd << " from dbase." << endl;
	    
	    return;
	}
    }

    // No valid configuration found ?
    Error("ReadConfiguration",
	  "Could not find a valid configuration for run %d. Abort!",
	  RunNr);
    gSystem->Abort(0);
}

//____________________________________________________________________

void AtlTriggerConf::WriteConfiguration() {
    //
    // Fill the current configuration of trigger items stored in the
    // clone arrays into the configuration tree for the given run
    // period.
    //
    Info("WriteConfiguration", "Write trigger config to dbase for run range %d -> %d .",
	 fRunStart, fRunEnd);
    fConfigTree->Fill();
}

//____________________________________________________________________

void AtlTriggerConf::Rehash() {
    //
    // Clear hash tables (without deleting its contents) and fill them
    // with the information from the clone arrays.
    //
    fL1TriggerItemsHT->Clear();
    fHLTTriggerItemsHT->Clear();

    // Re-hashing is done by THashTable::AddAll()
    fL1TriggerItemsHT->AddAll(fL1TriggerItemsCA);
    fHLTTriggerItemsHT->AddAll(fHLTTriggerItemsCA);
}

//____________________________________________________________________

void AtlTriggerConf::Print(Option_t *option) {
    //
    // Print current configuration
    //
    // Options available:
    //    "CUR"  - Print current configuration
    //    "LIST" - Print list of all configurations
    //    "ALL"  - Print all (default)
    //
    TString opt = option;
    opt.ToUpper();

    // Print current config
    if ( opt.Contains("ALL") || opt.Contains("CUR") ) {
	cout << endl
	     << "====================================================" << endl
	     << "          Trigger Configuration (current)" << endl
	     << "====================================================" << endl
	     << "Valid run range:   Run " << fRunStart << " -> Run " << fRunEnd
	     << endl
	     << endl
	     << "Level 1 Trigger   (" << fN_L1TriggerItems << " items)" << endl
	     << "-----------------------------------------------------" << endl;
	for ( Int_t i = 0; i < fN_L1TriggerItems; i++ ) 
	    ((AtlTriggerConfItem*)fL1TriggerItemsCA->At(i))->Print();
	cout << "-----------------------------------------------------" << endl
	     << endl
	     << "Higher Level Trigger   (" << fN_HLTTriggerItems << " items)" << endl
	     << "-----------------------------------------------------" << endl;
	for ( Int_t i = 0; i < fN_HLTTriggerItems; i++ ) 
	    ((AtlTriggerConfItem*)fHLTTriggerItemsCA->At(i))->Print();
	cout << "-----------------------------------------------------" << endl
	     << "End of trigger configuration for run range "
	     << fRunStart << " -> " << fRunEnd << endl
	     << endl;
    }

    // Print list
    if ( opt.Contains("ALL") || opt.Contains("LIST") ) {

	// Config tree already loaded ?
	if ( fConfigTree == 0 ) {
	    Error("Print", "No configuration tree loaded. Abort!");
	    gSystem->Abort(0);
	}

	// Loop over all tree entries and write out run range and no. of items
	Int_t RunStart = fRunStart;
	cout << endl
	     << "==============================================" << endl
	     << "        List of Trigger Configurations" << endl
	     << "==============================================" << endl
	     << endl
	     << "   # Run Range            #L1_Items #HLT_Items" << endl
	     << "----------------------------------------------" << endl;
	for ( Int_t i = 0; i < fConfigTree->GetEntries(); i++ ) {
	    fConfigTree->GetEntry(i);
	    cout.width(4); cout << i+1;
	    cout << " ";
	    cout.width(6); cout << fRunStart;
	    cout << " -> ";
	    cout.width(6); cout << fRunEnd;
	    cout << "     ";
	    cout.width(9); cout << fN_L1TriggerItems; cout << " ";
	    cout.width(9); cout << fN_HLTTriggerItems; cout << endl;
	}
	cout << "----------------------------------------------"
	     << endl << endl;

	// Re-load current configuration
	if ( RunStart != 0 ) ReadConfiguration(RunStart);
    }
}

//____________________________________________________________________

TClonesArray* AtlTriggerConf::GetTriggerItemsCA(ETrigger trigger) const {
    //
    // Get trigger items clone array
    //
    switch ( trigger ) {
	case kL1:
	    return fL1TriggerItemsCA;
	case kHLT:
	    return fHLTTriggerItemsCA;
    }
    return 0;
}

//____________________________________________________________________

THashTable* AtlTriggerConf::GetTriggerItemsHT(ETrigger trigger) const {
    //
    // Get trigger items hash table
    //
    switch ( trigger ) {
	case kL1:
	    return fL1TriggerItemsHT;
	case kHLT:
	    return fHLTTriggerItemsHT;
    }
    return 0;
}

//____________________________________________________________________

Int_t AtlTriggerConf::GetN_TriggerItems(ETrigger trigger, Int_t RunNr) {
    //
    // Get no. of trigger items in clones array for given run number
    //

    // Check if current configuration is still valid
    // Load new configuration otherwise
    if ( RunNr < fRunStart || RunNr > fRunEnd )
	ReadConfiguration(RunNr);

    // Return no. of items
    return GetN_TriggerItems(trigger);
}

//____________________________________________________________________

Int_t AtlTriggerConf::GetN_TriggerItems(ETrigger trigger) const {
    //
    // Get no. of trigger items in clones array for given run number
    // for current configuration
    //
    switch ( trigger ) {
	case kL1:
	    return fN_L1TriggerItems;
	case kHLT:
	    return fN_HLTTriggerItems;
    }
    return 0;
}

//____________________________________________________________________

void AtlTriggerConf::SetN_TriggerItems(ETrigger trigger,
				       Int_t NItems) {
    //
    // Set no. of trigger items in clones array
    //
    switch ( trigger ) {
	case kL1:
	    fN_L1TriggerItems = NItems; break;
	case kHLT:
	    fN_HLTTriggerItems = NItems; break;
    }
}

//____________________________________________________________________

const char* AtlTriggerConf::GetL1ItemName(Int_t Bit, Int_t RunNr) {
    //
    // Get name of the L1 item associated to the given bit.
    //
    // Check if the current configuration is still valid for the given
    // run. Load new configuration otherwise
    //
    if ( RunNr < fRunStart || RunNr > fRunEnd )
	ReadConfiguration(RunNr);
    return GetL1ItemName(Bit);
}

//____________________________________________________________________

const char* AtlTriggerConf::GetHLTItemName(Int_t Bit, Int_t RunNr) {
    //
    // Get name of the HLT item associated to the given bit.
    //
    // Check if the current configuration is still valid for the given
    // run. Load new configuration otherwise.
    //
    if ( RunNr < fRunStart || RunNr > fRunEnd )
	ReadConfiguration(RunNr);
    return GetHLTItemName(Bit);
}

//____________________________________________________________________

const char* AtlTriggerConf::GetL1ItemName(Int_t Bit) {
    //
    // Get name of the L1 item associated to the given bit
    // from the currently loaded configuration
    //
    return ((AtlTriggerConfItem*)fL1TriggerItemsCA->At(Bit))->GetName();
}

//____________________________________________________________________

const char* AtlTriggerConf::GetHLTItemName(Int_t Bit) {
    //
    // Get name of the HLT item associated to the given bit
    // from the currently loaded configuration
    //
    return ((AtlTriggerConfItem*)fHLTTriggerItemsCA->At(Bit))->GetName();
}

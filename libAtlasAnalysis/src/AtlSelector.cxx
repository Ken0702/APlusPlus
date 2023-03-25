//____________________________________________________________________
//
// Basic selector for analysis of ATLAS data
//
// Inherit from this class in order to write your own specific
// analysis selector. See the description of the AtlExampleAnalysis
// class for an example
//
// Main schema:
// ============
// The idea is to have different user analyses by means of inheritance:
//
//
//                                  AtlSelector
//                                       |
//                                       |
//          +-------------------+--------------------+--------------+
//          |                   |                    |              |
//          V                   V                    V              V
//    UserAnalaysis_1      UserAnalysis_2      UserAnalysis_3      ...
//
//
// The selector is mainly used for filling histograms during event
// processing. In addition, it is possible to create and fill an
// output A++ tree (see SetOutputTree()). This option is being used
// for the conversion of D3PDs into A++ files, for instance. The
// output tree as well as the histograms are filled only in case the
// ProcessCut() member function returns kTRUE for the event in
// question. This function can be optionally given by the user (see
// the example above how to do this). The default is to accept every
// event.
//
// The general picture of the data-flow looks like the following:
//
//  +-------------+
//  |             |                                    +-----------------+
//  | A++         +----+                         +---->| User Histograms |
//  | .root files |    |                         |     +-----------------+
//  +-------------+    |      +-------------+    |
//                     +----->|             |    |     +-----------------+
//        OR                  | AtlSelector +----+---->| Ctrl Histograms |
//                     +----->|             |    |     +-----------------+
//  +-------------+    |      +-------------+    |
//  |             |    |                         |     +-----------------+
//  | DPD Ntuples +----+                         +---->|    A++ Tree     |
//  |             |                                    +-----------------+
//  +-------------+
//
//
// Information about the processing status (no. of precessed events
// etc) is continuously printed to std output for non-batch jobs
// testet by gROOT->IsBatch()
//
//
// Processing and event selection:
// ===============================
// Flow-chart of user-defined functions
//
//     +-------------------+
//     | SetBranchStatus() |
//     +--------+----------+
//              |
//              V
//     +-------------------+
//     | BookHistograms()  |
//     +--------+----------+
//              |               +------------------------------+
//              V               |                              |
//     +-------------------+    V   +------------------+ false |
//     |    Event Loop     +--------+ ObjSelectionTool +------>|
//     +--------+----------+        +---+--------------+       |
//              |                       V                      |
//              |                   +------------------+ false |
//              |                   | ProcessPreCut()  +------>|
//              |                   +---+--------------+       |
//              |                       |                      |
//              |                   +-----------------+  false |
//              |                   |   ProcessCut()  +------->|
//              |                   +---+-------------+        |
//              |                       |                      |
//              |                 true? |    +-----------------++
//              |                       +----+ FillHistograms() |
//              |                            +------------------+
//              V
//     +-------------------+
//     |    Terminate()    |
//     +-------------------+
//
//
// Analysis tools:
// ===============
// There exist a couple of tools (decay finders, ctrl plots,
// b-tagging, trigger etc.) which can be plugged into the analysis at
// different positions. See the class AtlAnalysisTool for details.
//
// ObjSelection Tool:
// ==================
// The AtlObjSelectionTool selects the analysis objects (leptons, jets, b-jets)
// and performs cuts. The cuts have to be set with the use of public datamembers.
//
// Lepton cuts available:
// Pt-cut: fElectron_Pt_min, fMuon_Pt_min
// 
// Jet-Cut:
// Pt-cut:    fJetPt_min
// Eta-Range: fJetEta_min < |eta_jet| < fJetEta_max
//
// The number of jets in this list defines the Jet Multiplicity
// and the selector will remove any event that does not match the jet multiplicity
// provided via fJetMults.
//
// Selection cuts:
// ===============
// Selection cuts are supposed to be stored in public data members of
// the derived classes (eg see Atl_tChannelAnalysis). The default
// values of these cuts are to be given in the virtual member function
// SetCutDefaults(). It is common to enclose an analysis selector
// inside an analysis task. The base class for this is the class
// AtlAppAnalysisTask which in turn is inherited to the various
// analyses. The actual cut values for an analysis should be given
// there via AtlAppAnalysis::SetCut().
//
// Entry lists:
// ============
// Events that passed the ProcessCut() can be stored in a TEntryList. Later on,
// this list can be used to select only these events in a tree or chain.
// The chain has to be the same as when creating the list.
// To enable writing the entry list, use sel->SetWriteEntryList(kTRUE) or 
// set it in the task class.
// This list will be created in the output root file with the tree name
// "app_entrylist".
// To apply the list on a chain, you first have to load the list from
// the root file.
// TFile *f = new TFile("outputfile.root","read");
// TEntryList *elist = new TEntryList;
// elist = (TEntryList*)f->Get("app_entrylist");
// Then you can apply it on your chain:
// ch->SetEntryList(elist);
// Running your analysis now will only include the events stored in 
// the list.
//
//    Author: Oliver Maria Kind <mailto:kind@mail.desy.de>
//    Update: $Id$
//    Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlSelector
#include <AtlSelector.h>
#endif
#include <cassert>
#include <TString.h>
#include <TObjectTable.h>
#include <iostream>
#include <TROOT.h>
#include <TSystem.h>
#include <TEntryList.h>
#include <TKey.h>
#include <AtlEvtReaderApp.h>
#include "AtlEvtReaderBase.h"
#include <AtlEvtReaderD3PDSgTop.h>
#include <AtlEvtWriterApp.h>
#include <AtlEvtWriterMem.h>
#include <AtlEvtWriterD3PDSgTop.h>
#include <AtlObjectsToolD3PDSgTop.h>
#include <AtlEvtReaderD3PDCKM.h>
#include <TChainElement.h>

using namespace std;

static const char* fgLepChannelNames[AtlSelector::fgNumLepChannels+1] = {
    "enu", "munu", "lnu"
};

static const char* fgJetMultNames[AtlSelector::fgNumJetMults] = {
    "1", "2", "3", "4", "5", "6", "4to6", "1+", "2+", "3+", "all"
};

#ifndef __CINT__
ClassImp(AtlSelector);
#endif

//____________________________________________________________________

AtlSelector::AtlSelector(const char* OutputFilename) :
    TSelector(),
    fEvtReaderUser(0) {
    //
    // Default constructor
    //
    fEvent = 0;
    fTree = 0;
    fCurrentTree = 0;
    fBookkeepingList = new TObjArray;
    fNBookkeeping = 0;
    fNProcessedFiles = -1;
    fOutputFile = 0;
    fOutputFilename  = new TString(OutputFilename);
    fOutputTreeName  = new TString("");
    fOutputTreeTitle = new TString("");
    fOutputTree = 0;
    fOutputTriggerConfTree = 0;
    fNProcessNthEventsOnly = 1;
    fProcessedEvents    = 0;
    fProcessedEventsW   = 0.;
    fAcceptedEvents     = 0;
    fAcceptedEventsW    = 0.;
    fAcceptedEventsB    = 0.;
    fPreAcceptedEvents  = 0;
    fPreAcceptedEventsW = 0.;
    fJetBinEvents       = 0;
    fJetBinEventsW      = 0.;
    fPreTaggedEvents    = 0;
    fPreTaggedEventsW   = 0.;
    fEntry = 0;
    fListOfTools = new TList;
    fWriteEntryList = kFALSE;
    fEntryList = 0;
    fPrintEvent = kFALSE;
    fPrintObjectTable = kFALSE;
    fPassedSelection = kFALSE;
    fCountUnfilteredEvents = kTRUE;
    fInputMode = kD3PDSgTop_v2;
    fOutputMode = kApp;
    fEvtReader = 0;
    fEvtWriter = 0;
    fDoTruthTree = kFALSE;
    
    fHistCutflow_NoWeights                 = 0;
    fHistCutflow_genWeights                = 0;
    fHistCutflow_genXpileupWeights         = 0;
    fHistCutflow_genXpileupXzvertexWeights = 0;
    fHistCutflow_allEventsHFOR_NoWeights   = 0;
    fHistCutflow_allEventsHFOR_genWeights  = 0;

    fHistsArrayCutflow = new TObjArray;
    fCopyCutflowHistograms = kFALSE;
}

//____________________________________________________________________

AtlSelector::~AtlSelector() {
    //
    // Default destructor
    //
    delete fOutputFile;
    delete fOutputFilename;
    delete fOutputTreeName;
    delete fOutputTreeTitle;
    if ( fOutputTree != 0 ) delete fOutputTree;
    if ( fOutputTriggerConfTree != 0 ) delete fOutputTriggerConfTree;
    //if ( fCtrlPlots != 0 ) delete fCtrlPlots;
    //if ( fZ0Finder  != 0 ) delete fZ0Finder;
    fListOfTools->Delete(); delete fListOfTools;
    if ( fEvtReaderUser != 0 && fEvtReader != fEvtReaderUser ) delete fEvtReaderUser;
    if ( fEvtReader != 0 ) delete fEvtReader;
    if ( fEvtWriter != 0 ) delete fEvtWriter;
    delete fBookkeepingList;
    fHistsArrayCutflow->Delete();
    delete fHistsArrayCutflow;
}

//____________________________________________________________________

void AtlSelector::SetOutputTree(const char* name, const char* title) {
    //
    // Set output tree/ntuple
    // 
    // If the name of the output tree is set it is automatically being
    // filled with all events passing the ProcessCut() method.
    //
    // The trigger configuration dbase of the used runs is also copied
    // from the input files.
    // 
    // If the size of the output tree exceeds the maximum tree size
    // (see TTree::SetMaxTreeSize()) of (currently) 2Gb, Root splits
    // the output file automatically. Iin contrast to Root's internal
    // mechanism, which writes all histograms to the last of the
    // splitted files only, the AtlSelector class splits also the
    // histograms and writes them to their corresponding output files
    // (see ChangeOutputFile()). The same holds for the trigger
    // configuration dbase.
    //
    fOutputTreeName->Remove(0, fOutputTreeName->Length());
    fOutputTreeName->Append(name);
    fOutputTreeTitle->Remove(0, fOutputTreeTitle->Length());
    fOutputTreeTitle->Append(title);
}

//____________________________________________________________________

void AtlSelector::SetBranches() {
    //
    // Set branch assignments of the input tree. Here an A++ input
    // tree is assumed. For other trees this (virtual) method has to
    // be overloaded. See also the GetEntry() method
    //
    fEvtReader->SetEvent(fEvent);
    fEvtReader->SetBranches(fTree);
}

//____________________________________________________________________

Int_t AtlSelector::GetEntry(Long64_t entry) {
    //
    // Get entry from current input tree. This (virtual) method can be
    // overloaded if needed (different type of input tree)
    //
    fEntry = entry;
    fEvtReader->GetEntry(fCurrentTree, entry);
    return 0;
}

//____________________________________________________________________

void AtlSelector::Init(TTree *tree) {
    //
    // The Init() function is called when the selector needs to initialize
    // a new tree or chain. Typically here the branch addresses and branch
    // pointers of the tree will be set.
    // It is normally not necessary to make changes to the generated
    // code, but the routine can be extended by the user if needed.
    // Init() will be called many times when running on PROOF
    // (once per file to be processed).
    //
    fTree = tree;
    
    // Allow for event-wise deletion of ref objects in case of input chain
    if ( fIsChain ) ((TChain*)fTree)->CanDeleteRefs(kTRUE);
}

//____________________________________________________________________

Bool_t AtlSelector::Notify() {
    //
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when a new TTree
    // is started when using PROOF.
    //
    // Load the configurations (trigger etc.) from the
    // newly opened file. In case of writing an event list make sure a
    // new one is appended for the current tree.
    //
    // At the end of this function, the Notify() functions of the
    // current event reader and all active tools are called.
    //
    fCurrentTree = ( fIsChain ) ? fTree->GetTree() : fTree;
	
    // Note that Notify() usually is run once before the event loop by
    // TTreePlayer::Process(). In this case we do not need the
    // following code to be executed.
    if ( fCurrentTree == 0 ) {
	Info("Notify", "No tree currently loaded. Return.");
	return kTRUE;
    }
    Info("Notify", "Opened new input file %s", 
	 fCurrentTree->GetCurrentFile()->GetName());

    // Any events inside the tree ?
    if ( fCurrentTree->GetEntries() == 0 ) {
	Warning("Notify",
		"The event tree in the current input file contains 0 events.");
    } else {
	// ==============
	// Trigger config
	// ==============
	if ( fInputMode == kApp ) {
	    // Load trigger configuration (A++ input only)
	    Info("Notify", "Load trigger configuration dbase from current input file ...");
	    AtlTriggerConf *trig_conf = LoadTriggerConfig();
	    if ( HasOutputTree() && fOutputMode == kApp ) {
		// In case of writing an A++ output tree merge the new trigger config
		// with the dbases from the input files before.
		TTree *trig_tree = trig_conf->GetConfigTree();
		if ( fOutputTriggerConfTree == 0 ) {
		    // For the first input file simply copy the trigger conf tree
		    fOutputFile->cd();
		    fOutputTriggerConfTree = trig_tree->CloneTree(-1, "fast"); // raw byte copy
		} else {
		    TList *treelist = new TList;
		    treelist->Add(trig_tree);
		    fOutputTriggerConfTree->Merge(treelist);
		    delete treelist;
		}
	    }
	}
    }

    // Do bookkeeping of cut-flow and job info histograms
    DoBookkeeping(fCurrentTree->GetCurrentFile());

    // Call Notify() of the current event reader
    fEvtReader->Notify();

    // Call Notify() of all enabled tools
    AtlAnalysisTool *tool = 0;
    TIter next_tool(fListOfTools);    
    while ( (tool = (AtlAnalysisTool*)next_tool()) ) {
        if ( gDebug > 0 )
            Info(__FUNCTION__, "Calling notify on tool %s", tool->GetName());
	if ( !tool->IsOff() ) tool->Notify();
    }
    Info(__FUNCTION__, "End Notify()");
    return kTRUE;
}

//____________________________________________________________________

void AtlSelector::DoBookkeeping(TFile *InputFile) {
    //
    // Perform bookkeeping of the cut-flow and job info histograms.
    // This is done for every input file.
    //

    //
    // Check if the bookkeeping has already been done for this input
    // file
    //
    
    // Find corresponding chain element for given file
    TObjArray* files = ((TChain*)fTree)->GetListOfFiles();
    TIter next_el(files);
    TChainElement *el = 0;
    while ( (el = (TChainElement*)next_el()) ) {
	if ( strcmp(el->GetTitle(), InputFile->GetName()) == 0 )
	    break;
    }
    
    // No chain element found ?
    if ( el == 0 ) {
	Error("DoBookkeeping",
	      "Could not find chain element for current input file %s.",
	      InputFile->GetName());
	Error("DoBookkeeping", "Abort !");
	gSystem->Abort(1);
    }
    
    // Bookkeeping already done for the given file ?
    if ( fBookkeepingList->FindObject(el) ) {
	Info("DoBookkeeping", "Perform bookkeeping for input file %s",
	     el->GetTitle());
	fBookkeepingList->Remove(el);
	fNBookkeeping--;
    } else {
	Info("DoBookkeeping",
	     "Bookkeeping for input file %s already done",
	     el->GetTitle());
	Info("DoBookkeeping", "Skip.");
	return;
    }

    // For A++-Ntuples:
    // Check if the new input file contains a JobInfo histogram. If
    // so, take the no. of all unfiltered events from there and
    // add it to the JobInfo histogram of the output file
    // For Mem Discriminant computation in run 2:
    // Also take job_info histograms, because cutflows are not available.
    // Actually that is wrong, they are just named differently and can
    // be copied by setting fCopyCutflowHistograms.
    TH1F *h = 0;
    TH1D *h_cf = 0;
    TDirectoryFile * dir = 0;

    // For SgTopD3PDs:
    // Copy the TRC cutflow histograms. Abort if no histogram is found.
    // For this input mode, the A++-job-info histograms are ignored, even if
    // existing.
    
    // Cutflow histograms from SgTopD3PD
    TH1D *h_cf1 = 0;
    TH1D *h_cf2 = 0;
    TH1D *h_cf3 = 0;
    TH1D *h_cf4 = 0;
    TH1D *h_cf5 = 0;
    TH1D *h_cf6 = 0;

    if ( fInputMode == kApp || fInputMode == kCustomMem ) {
        h = (TH1F*)InputFile->Get("/job_info/h_nevts");
	if ( h != 0 ) {
	    Info("DoBookkeeping", "JobInfo histogram in input file found. Disable counting of unfiltered events");
	    fCountUnfilteredEvents = kFALSE;
	    fHistEvents->AddBinContent(1, h->GetBinContent(1));
	    fHistEvents->AddBinContent(2, h->GetBinContent(2));
	    fHistEvents->AddBinContent(3, h->GetBinContent(3));
	} else {
	    Info("DoBookkeeping", "No JobInfo histogram in input file found. Enable counting of unfiltered events.");
	}
    } else if ( fInputMode == kD3PDSgTop ||
		fInputMode == kD3PDCKM ||
		fInputMode == kD3PDSgTop_v2 ) {
	
	// Copy all SgTopD3PD histograms
	if ( fInputMode == kD3PDSgTop_v2 ) {
	    // Cutflow histograms from SgTopD3PD
	    
	    h_cf1 = (TH1D*) InputFile->Get(Form("%s_fullCutFlow_noWeights", fTree->GetName()));
	    h_cf2 = (TH1D*) InputFile->Get(Form("%s_fullCutFlow_genWeights", fTree->GetName()));
	    h_cf3 = (TH1D*) InputFile->Get(Form("%s_fullCutFlow_genXpileupWeights", fTree->GetName()));
	    h_cf4 = (TH1D*) InputFile->Get(Form("%s_fullCutFlow_genXpileupXzvertexWeights", fTree->GetName()));
	    h_cf5 = (TH1D*) InputFile->Get(Form("%s_allEventsHFOR_noWeights", fTree->GetName()));
	    h_cf6 = (TH1D*) InputFile->Get(Form("%s_allEventsHFOR_genWeights", fTree->GetName()));

	    if ( h_cf1 == 0 ) {
		// For v14, cutflows are named:
		// 'physics_systematic_fullCutFlow_something'
		// --> the systematic is unknown when using MemTk ntuples
		//     do assignment to h_cf1, etc. with list of keys
		//
		// Do this only if the histogram can not be found with the above method.
		TList *keys = InputFile->GetListOfKeys();
		for ( Int_t i = 0; i < keys->GetEntries(); i++ ) {
		    TString keyname = ((TKey*) keys->At(i))->GetName();
		    if ( keyname.Contains("fullCutFlow_noWeights") ) {
			h_cf1 = (TH1D*)InputFile->Get(keyname.Data());
		    } else if ( keyname.Contains("fullCutFlow_genWeights") ) {
			h_cf2 = (TH1D*)InputFile->Get(keyname.Data());
		    } else if ( keyname.Contains("fullCutFlow_genXpileupWeights") ) {
			h_cf3 = (TH1D*)InputFile->Get(keyname.Data());
		    } else if ( keyname.Contains("fullCutFlow_genXpileupXzvertexWeights") ) {
			h_cf4 = (TH1D*)InputFile->Get(keyname.Data());
		    } else if ( keyname.Contains("allEventsHFOR_noWeights") ) {
			h_cf5 = (TH1D*)InputFile->Get(keyname.Data());
		    } else if ( keyname.Contains("allEventsHFOR_genWeights") ) {
			h_cf6 = (TH1D*)InputFile->Get(keyname.Data());
		    }
		}
	    }
	} else {
	    h_cf1 = (TH1D*) InputFile->Get("fullCutFlow_noWeights");
	    h_cf2 = (TH1D*) InputFile->Get("fullCutFlow_genWeights");
	    h_cf3 = (TH1D*) InputFile->Get("fullCutFlow_genXpileupWeights");
	    h_cf4 = (TH1D*) InputFile->Get("fullCutFlow_genXpileupXzvertexWeights");
	    h_cf5 = (TH1D*) InputFile->Get("allEventsHFOR_noWeights");
	    h_cf6 = (TH1D*) InputFile->Get("allEventsHFOR_genWeights");
	}

	// Abort here if no bookkeeping histogram is found.
	// The histograms are needed for e.g. plotting otherwise
	// the original number of events is unkown.
	if ( h_cf1 == 0 ) {
	    Error("DoBookkeeping", "No Bookkeeping histogram found! Abort!");
	    gSystem->Abort();
	}
	
	if ( fHistCutflow_NoWeights == 0 ) {
	    
	    fHistCutflow_NoWeights = (TH1D*) h_cf1->Clone();
	    fHistCutflow_NoWeights->SetDirectory(fOutputFile);

	    if ( h_cf2 != 0 ) { // MC
		fHistCutflow_genWeights                = (TH1D*) h_cf2->Clone();
		fHistCutflow_genXpileupWeights         = (TH1D*) h_cf3->Clone();
		fHistCutflow_genXpileupXzvertexWeights = (TH1D*) h_cf4->Clone();
		fHistCutflow_allEventsHFOR_NoWeights   = (TH1D*) h_cf5->Clone();
		fHistCutflow_allEventsHFOR_genWeights  = (TH1D*) h_cf6->Clone();

		fHistCutflow_genWeights->SetDirectory(fOutputFile);
		fHistCutflow_genXpileupWeights->SetDirectory(fOutputFile);
		fHistCutflow_genXpileupXzvertexWeights
		    ->SetDirectory(fOutputFile);
		fHistCutflow_allEventsHFOR_NoWeights->SetDirectory(fOutputFile);
		fHistCutflow_allEventsHFOR_genWeights
		    ->SetDirectory(fOutputFile);
	    }
	} else {
	    fHistCutflow_NoWeights->Add(h_cf1);
	    if ( h_cf2 != 0 ) { // MC
		fHistCutflow_genWeights->Add(h_cf2);
		fHistCutflow_genXpileupWeights->Add(h_cf3);
		fHistCutflow_genXpileupXzvertexWeights->Add(h_cf4);
		fHistCutflow_allEventsHFOR_NoWeights->Add(h_cf5);
		fHistCutflow_allEventsHFOR_genWeights->Add(h_cf6);
	    }
	}

	Info("DoBookkeeping",
	     "TRC D3PD bookkeeping histogram in input file found. Disable counting of unfiltered events.");
	fCountUnfilteredEvents = kFALSE;
	fHistEvents->AddBinContent(1, h_cf1->GetBinContent(1));
	if ( ( h_cf2 != 0 ) && ( h_cf4 != 0 ) ) { // MC
	    fHistEvents->AddBinContent(2, h_cf2->GetBinContent(1)); 
	    fHistEvents->AddBinContent(3, h_cf4->GetBinContent(1));
	} else { // DATA
	    fHistEvents->AddBinContent(2, h_cf1->GetBinContent(1)); 
	    fHistEvents->AddBinContent(3, h_cf1->GetBinContent(1));
	}
    }
    else if ( fInputMode == kCustom ) {
        AtlEvtReaderBase::InitialSumOfWeights_t const & sumOfWeights = fEvtReader->GetInitialSumOfWeights(InputFile);
        fCountUnfilteredEvents = kFALSE;
        fHistEvents->AddBinContent(1, sumOfWeights.one);
        fHistEvents->AddBinContent(2, sumOfWeights.mcevt);
        fHistEvents->AddBinContent(3, sumOfWeights.mcevt_pileup_zvtx);
        if ( fCopyCutflowHistograms ) {
            // To copy the cutflow histograms:
            // - loop over all selection directories in the input file,
            // - take all cutflow histograms and copy them to the output file
            // - keep track of all histograms in a list.
            // The directory structure should be preserved.

            Info(__FUNCTION__, "Copy selection cutflow histograms");
            TDirectory *savdir = gDirectory;

            // go through all selection directories in the input file
            TIter next_dir(InputFile->GetListOfKeys());
            TKey *dkey = 0;
            while ( (dkey = (TKey*)next_dir()) ) {
                if ( strcmp(dkey->GetClassName(), "TDirectoryFile") != 0 )
                    continue;
                dir = (TDirectoryFile*)InputFile->Get(dkey->GetName());
                TIter next_hist(dir->GetListOfKeys());
                TKey * hkey = 0;
                if ( gDebug>0 )
                    Info(__FUNCTION__, "Look for histograms in %s", dir->GetName());

                // get cutflow histograms from the directory
                while ( (hkey = (TKey*)next_hist()) ) {
                    if ( strcmp(hkey->GetClassName(), "TH1F") != 0 and
                         strcmp(hkey->GetClassName(), "TH1D") != 0 ) {
                        continue;
                    }
                    TString hname = hkey->GetName();
                    if ( !hname.BeginsWith("cutflow_") ) {
                        continue;
                    }
                    h_cf = (TH1D*)InputFile->Get(Form("/%s/%s", dkey->GetName(), hkey->GetName()));
                    if ( gDebug>0 )
                        Info(__FUNCTION__,
                             "Found histogram with Name \"%s\" and Title \"%s\""
                             " in InputFile", h_cf->GetName(), h_cf->GetTitle());

                    // Check if histogram is already in list.
                    // The hist title is unique because it includes selection
                    // and histogram name, separated by underscore.
                    Bool_t IsInList = kFALSE;
                    TIter next_h(fHistsArrayCutflow);
                    TH1D * hlist = 0;
                    while ( (hlist = (TH1D*)next_h()) ) {
                        if ( strcmp(hlist->GetTitle(), h_cf->GetTitle()) == 0 ) {
                            IsInList = kTRUE;
                            hlist->Add(h_cf); // add to listed hist
                            if ( gDebug>0 )
                                Info(__FUNCTION__, "Found histogram titled \"%s\" in list",
                                     h_cf->GetTitle());
                            break;
                        }
                    }

                    // New histograms should be cloned and added to the list,
                    // otherwise they were already added to the appropriate
                    // histogram above.
                    if ( !IsInList ) {
                        TString subdir(dir->GetName());
                        if ( gDirectory->FindObject(subdir.Data()) == 0 ) {
                            gDirectory->mkdir(subdir.Data());
                        }
                        gDirectory->cd(subdir.Data());
                        hlist = (TH1D*)h_cf->Clone();
                        fHistsArrayCutflow->Add(hlist);
                        if ( gDebug>0 )
                            Info(__FUNCTION__, "Histogram titled %s added to list",
                                 hlist->GetTitle());
                        gDirectory->cd("..");
                    }
                }
            }
            savdir->cd();
        }
    } else {
	Error("Notify", "Unknown input mode given. Abort!");
	gSystem->Abort(1);
    }
    fNProcessedFiles += 1;
}

//____________________________________________________________________

void AtlSelector::Begin(TTree *tree) {
    //
    // The Begin() function is called at the start of the query.
    // When running with PROOF Begin() is only called on the client.
    // The tree argument is deprecated (on PROOF 0 is passed).
    //
    TString option = GetOption(); 

    // Start stop watch
    fStopwatch.Start();
    
    // Start here
    Info("Begin",
	 "==================== Start analysis job here ====================");

    // Write out Panda information in case of Panda jobs
    if ( gSystem->Getenv("PandaID") ) {
	Info("Begin", "Panda job Id: %s", gSystem->Getenv("PandaID"));
    }

    // Input is chain or tree ?
    // In case of a chain, check if there are any input files
    fTree = tree;
    if( fTree->InheritsFrom("TChain") ) {
	fIsChain = kTRUE;
	Int_t nfiles = ((TChain*)fTree)->GetListOfFiles()->GetEntries();
	if (  nfiles < 1 ) {
	    Error("Begin", "No input files given. Abort!");
	    gSystem->Abort(1);
	} else if ( nfiles == 1 ) {
	    Info("Begin", "This job contains %d input file.", nfiles);
	} else {
	    Info("Begin", "This job contains %d input files.", nfiles);
	}
    } else {
	fIsChain = kFALSE;
    }
    
    // Name of input chain
    Info("Begin", "Input chain name = \"%s\"", fTree->GetName());
    
    // Create event reader (if needed)
    if ( fEvtReader == 0 ) {
        if ( (fEvtReaderUser == 0) == (fInputMode==kCustom || fInputMode==kCustomMem) ) {
            Fatal(__FUNCTION__, "fEvtReaderUser is (not) undefined while "
                  "fInputMode is (not) set to kCustom or kCustomMem");
        }
        switch ( fInputMode ) {
        case kApp:
            fEvtReader = new AtlEvtReaderApp(this);
            Info("Begin", "Input file type = kApp");
            break;
        case kD3PDSgTop:
        case kD3PDSgTop_v2:
            fEvtReader = new AtlEvtReaderD3PDSgTop(this);
            Info("Begin", "Input file type = kD3PDSgTop");
            break;
        case kD3PDCKM:
            fEvtReader = new AtlEvtReaderD3PDCKM(this);
            Info("Begin", "Input file type = kD3PDCKM");
            break;
        case kCustom:
        case kCustomMem:
            fEvtReader = fEvtReaderUser;
            Info("Begin", "Input file type = %s", fEvtReader->Class()->GetName());
            break;
        default:
            Error("Begin", "Bad input file type - abort!");
            gSystem->Abort(1);
        }
    }

    // Create event writer (if any)
    if ( HasOutputTree() ) {
	Info("Begin", "Write output tree \"%s\"",
	     fOutputTreeName->Data());

	// Check if the event writer object was already given. If not
	// create one. The latter is still included for
	// backward-compability reasons
	if ( fEvtWriter == 0 ) {
	    switch ( fOutputMode ) {
		case kApp:
		    fEvtWriter = new AtlEvtWriterApp();
		    Info("Begin", "Output tree type: kAPP");
		    break;
		case kMemNtuple:
		    fEvtWriter = new AtlEvtWriterMem();
		    Info("Begin", "Output tree type: kMemNtuple");
		    break;
		case kD3PDSgTop:
		case kD3PDSgTop_v2:
		    fEvtWriter = new AtlEvtWriterD3PDSgTop();
		    Info("Begin", "Output tree type: kD3PDSgTop");
		    break;
		case kD3PDCKM:
		    fEvtWriter = new AtlEvtWriterD3PDSgTop();
		    Info("Begin", "Output tree type: kD3PDSgTop");
		    break;			    
		default:
		    Error("Begin", "Bad output tree type - abort!");
		    gSystem->Abort(1);
	    }
	} else {
	    Info("Begin", "Output tree type: %s",
		 fEvtWriter->GetNameOfType());
	}
    } else {
	Info("Begin", "Output tree: none");
    }
    
    // Open output file with maximum compression level
    Info("Begin", "Open output file \"%s\"", fOutputFilename->Data());
    fOutputFile = new TFile(fOutputFilename->Data(), "recreate");
    if ( fOutputFile->IsZombie() )
        Fatal(__FUNCTION__, "Could not open output file %s",
              fOutputFilename->Data());
    fOutputFile->SetCompressionLevel(9);
    
    // Create entry list (if wished)
    if ( fWriteEntryList ) {
	fEntryList = new TEntryList("app_entrylist", "A++ event list");
	fEntryList->SetDirectory(fOutputFile);
    }
}

//____________________________________________________________________

void AtlSelector::SlaveBegin(TTree */*tree*/) {
    //
    // The SlaveBegin() function is called after the Begin() function.
    // When running with PROOF SlaveBegin() is called on each slave server.
    // The tree argument is deprecated (on PROOF 0 is passed).
    //
    // Function called before starting the event loop
    //  -it performs some cleanup
    //  -it creates histograms
    //  -it sets some initialisation for the event list
    //
    fProcInfo = new ProcInfo_t;
    fMemInfo  = new MemInfo_t;
    
    // ============
    // Create event
    // ============
    fEvent = new AtlEvent;

    // =======================================
    // Map branch addresses to branch pointers
    // =======================================
    Info("SlaveBegin", "Set branch addresses.");
    SetBranches();
    
    // ============================
    // Print analysis configuration
    // ============================
    
    // Print user analysis configuration
    Info("SlaveBegin", "User analysis configuration:\n");
    Print();

    // Init tools and print configuration of all analysis tools
    TIter next_tool(fListOfTools);
    AtlAnalysisTool *tool = 0;
    while ( (tool = (AtlAnalysisTool*)next_tool()) ) {
	tool->Init();
	cout << endl;
	Info("SlaveBegin",
	     "Registered analysis tool \"%s\" of type %s with the following configuration:\n",
	     tool->GetName(), tool->ClassName());
	tool->Print();
	// Remove switched off tools from list
	if ( tool->fProcessMode == AtlAnalysisTool::kOff ) {
	    fListOfTools->Remove(tool);
	    delete tool;
	}
    }
    
    // ======================
    // Switch off/on branches
    // ======================

    // Call user-defined function first
    if ( fInputMode == kApp ) SetBranchStatus();

    // Set tree and event pointers.
    // Set branch status for all analysis tools
    next_tool.Reset();
    while ( (tool=(AtlAnalysisTool*)next_tool()) ) {
	tool->SetTree(fTree);
	tool->SetOutputFile(fOutputFile);
	tool->SetEvent(fEvent);
	if ( fInputMode == kApp ) tool->SetBranchStatus();
    }
	
    // Make sure the event weight branch is loaded any time
    if ( fInputMode == kApp )
	fTree->SetBranchStatus("fEventHeader.fEventWeight", kTRUE);
    
    // ===============
    // Book histograms
    // ===============
    Info("SlaveBegin", "Book histograms ...");

    // Set Sumw2 for all histograms
    TH1::SetDefaultSumw2(kTRUE);
	
    // Book job info histograms
    BookJobInfoHistograms();
    Info("SlaveBegin", "Booked JobInfo histograms.");
    
    // Book user-defined histograms
    fOutputFile->cd();
    BookHistograms();
    Info("SlaveBegin", "Booked user-defined histograms.");

    // Book histograms of all analysis tools
    next_tool.Reset();
    while ( (tool = (AtlAnalysisTool*)next_tool()) ) { 
	fOutputFile->cd();
	if(strcmp(tool->GetOutputDir(),"/") != 0) {
	  if( !fOutputFile->cd( tool->GetOutputDir() ) ) {
		fOutputFile->mkdir(tool->GetOutputDir(), "Tool directory");
		gDirectory->cd( tool->GetOutputDir() );
	  }
	}
	// Check if tool folder already exists
	if ( gDirectory->FindObject(tool->GetName()) != 0 ) {
	    Info("SlaveBegin", "Folder for tool \"%s\" already exists. Will re-use folder.",
		 tool->GetName());
	} else {
	    gDirectory->mkdir(tool->GetName(), tool->GetTitle());
	}
	gDirectory->cd(tool->GetName());
	tool->BookHistograms();
    }
    fOutputFile->cd();
    Info("SlaveBegin", "Booked histograms of all analysis tools.");

    // Print configuration of all booked histogram tools
    next_tool.Reset();
    while ( (tool = (AtlAnalysisTool*)next_tool()) ) {
	if ( tool->InheritsFrom("AtlHistogramTool") ) {
	    tool->Print();
	}
    }

    // =======================================
    // Create input file list for bookkeeping
    // =======================================
    TChainElement *el = 0;
    TIter next_el(((TChain*)fTree)->GetListOfFiles());
    while ( (el = (TChainElement*)next_el()) ) {
	fBookkeepingList->Add(el);
	fNBookkeeping++;
    }
    Info("SlaveBegin", "Registered %d input files for bookkeeping.",
	 fNBookkeeping);

    assert(fNProcessedFiles < 0);
    fNProcessedFiles = 0;

    // ==========================================
    // Create A++ output tree (if name was given)
    // ==========================================
    if ( HasOutputTree() ) {
	fOutputFile->cd();
	fOutputTree = fEvtWriter->CreateTree(fOutputTreeName->Data(),
					     fOutputTreeTitle->Data(),
					     fTree);
	fEvtWriter->BookTree(fOutputTree, fEvent);
        Info(__FUNCTION__, "Created output tree %s.", fOutputTreeName->Data());
    }
}

//____________________________________________________________________

Bool_t AtlSelector::Process(Long64_t entry) {
    //
    // The Process() function is called for each entry in the tree (or possibly
    // keyed object in the case of PROOF) to be processed. The entry argument
    // specifies which entry in the currently loaded tree is to be processed.
    // It can be passed to either AtlSelector::GetEntry() or
    // TBranch::GetEntry() to read either all or the required parts of
    // the data. When processing keyed objects with PROOF, the object
    // is already loaded and is available via the fObject pointer.
    //
    // This function should contain the "body" of the analysis. It can contain
    // simple or elaborate selection criteria, run algorithms on the data
    // of the event and typically fill histograms.
    //
    // The processing can be stopped by calling Abort().
    //
    // Use fStatus to set the return value of TTree::Process().
    //
    // The return value is currently not used.
    //
    if ( gDebug > 1 )
        Info(__FUNCTION__, "Start entry %lld", entry);
    fPassedSelection = kFALSE;
    // Save current Object count
    Int_t ObjectNumber = TProcessID::GetObjectCount();

    // skip event if using reduced samples
    if ( entry % fNProcessNthEventsOnly != 0 )
        return kFALSE;

    // ===================
    // Step 1: Clear event
    // ===================
    fEvent->Clear();

    // Clear tools
    AtlAnalysisTool *tool = 0;
    TIter next_tool(fListOfTools);    
    next_tool.Reset();
    while ( (tool = (AtlAnalysisTool*)next_tool()) ) {
	tool->Clear();
    }

    // Clear analysis selector
    AtlSelector::Clear(); // Clear objects defined here
    Clear(); // Clear user-defined objects
    
    // ========================
    // Step 2: Fetch next event
    // ========================
    GetEntry(entry);
    
    // Set tree for output entry list
    if ( fWriteEntryList ) fEntryList->SetTree(fTree);
    

    /*cout<<"EvtNumber="<<fEvent->GetEventHeader()->EventNr();
        cout<<"_RunNumber="<<fEvent->GetEventHeader()->RunNr()<<endl;
        return kTRUE;*/
    // Process info
    //ProcessInfo();

    // =========================
    // Step 3: Apply systematics
    // =========================

    // Perform systematics tools
    next_tool.Reset();
    while ( (tool = (AtlAnalysisTool*)next_tool()) ) {
	if ( tool->IsSystematics() )
	    tool->Process();
    }

    // =================================
    // Step 4: Print event & Bookkeeping
    // =================================
    
    // This is for tracking memory leaks
    // Make sure your .rootrc file is properly set
    if ( fPrintObjectTable == kTRUE ) {
	gObjectTable->Print();
    }
    
    // Print the full content of every event w/o trigger. Note that
    // when you try to print the trigger content with switched-off
    // trigger branches, the job crashes. The reason is still unknown.
    if ( fPrintEvent == kTRUE ) {
	fEvent->Print("all-notrig");
    }



    // Count no. of processed events
    fHistEvents->Fill(3.5, 1.);
    fHistEvents->Fill(4.5, fEvent->GetMCWeight());
    fHistEvents->Fill(5.5, fEvent->GetPreTagEvtWeight());
    fHistEvtWeights->Fill(fEvent->GetPreTagEvtWeight());
    fHistEvtWeightsExtended->Fill(fEvent->GetPreTagEvtWeight());
    fProcessedEvents++;
    fProcessedEventsW += fEvent->GetPreTagEvtWeight();  

    // In case the input A++ file does not contain a JobInfo Histogram
    // count the no. of all unfiltered events as well
    if ( fCountUnfilteredEvents == kTRUE ) {
	fHistEvents->Fill(0.5, 1.);
	fHistEvents->Fill(1.5, fEvent->GetMCWeight());
	fHistEvents->Fill(2.5, fEvent->GetPreTagEvtWeight());
    }
    

    // =============================================
    // Step 5: Define Objects for Analysis and Tools
    // =============================================
    
    // Perform object selection tool
    //  - if any of the object selection fails (e.g. jet-bin cut)
    //    the event is removed (Step 6)
    Bool_t PassedObjSelection = kTRUE;
    next_tool.Reset();
    while ( (tool = (AtlAnalysisTool*)next_tool()) ) {
	if ( tool->IsObjectsDefinition() )
	    PassedObjSelection = PassedObjSelection & tool->Process();
    }
    
    // =========================
    // Step 6: Obj Selection cut
    // =========================
    if ( PassedObjSelection ) {
	// Process only events contained in jet bin
	fJetBinEvents++;
	fJetBinEventsW += fEvent->GetPreTagEvtWeight();
    
	// =====================
	// Step 7: Scale Factors
	// =====================

	// Calculate pretag and tag event weights
	next_tool.Reset();
	while ( (tool = (AtlAnalysisTool*)next_tool()) ) {
	    if ( tool->IsScaleFactor() )
		tool->Process();
	}

	// ====================
	// Step 8: Pre-Analysis
	// ====================
	
	// Perform pre-analysis tools
	next_tool.Reset();
	while ( (tool = (AtlAnalysisTool*)next_tool()) ) {
	    if ( tool->IsPreAnalysis() )
		tool->Process();
	}
	
	// Fill tool histograms without event selection (optional)
	next_tool.Reset();
	while ( (tool = (AtlAnalysisTool*)next_tool()) ) {
	    tool->FillHistogramsNoEvtSel();
	}
	
	// Perform user-defined pre-analysis. Only events passing this
	// selection will be completely analyzed
	if ( ProcessPreCut() == kTRUE ) {
	    // Count no. of pre-accepted events
	    fHistEvents->Fill(6.5, 1.);
	    fHistEvents->Fill(7.5, fEvent->GetMCWeight());
	    fHistEvents->Fill(8.5, fEvent->GetPreTagEvtWeight());
	    fPreAcceptedEvents++;
	    fPreAcceptedEventsW += fEvent->GetPreTagEvtWeight();
	    	    
	    // =====================
	    // Step 9: Main Analysis
	    // =====================
	    InitEvent();
	    
	    // Perform main analysis for each analysis tool
	    next_tool.Reset();
	    while ( (tool = (AtlAnalysisTool*)next_tool()) ) {
		if ( tool->IsMainAnalysis() )
		    tool->Process();
	    }
	    
	    // ======================
	    // Step 10: Post-analysis
	    // ======================
	    
	    // Fill histograms of selector and tools
	    if ( ProcessCut() == kTRUE ) {
		// Fill user histograms and output A++ tree. Note that all
		// histograms must be filled before, because otherwise
		// they won't be stored properly in case of changing
		// output files, see ChangeOutputFile().
		ProcessFill();
		// Count no. of accepted events
		fHistEvents->Fill(9.5, 1.);
		fHistEvents->Fill(10.5, fEvent->GetMCWeight());
		fHistEvents->Fill(11.5, fEvent->GetPreTagEvtWeight());
		fAcceptedEvents++;
		fAcceptedEventsW += fEvent->GetPreTagEvtWeight();
		fAcceptedEventsB += fEvent->GetTagEvtWeight();		
	    }
	    
	    // Perform post-analysis for each analysis tool
	    next_tool.Reset();
	    while ( (tool = (AtlAnalysisTool*)next_tool()) ) {
		if ( tool->IsPostAnalysis() )
		    tool->Process();
	    }
	}
    }

    // Restore Object count
    // To save space in the table keeping track of all referenced objects
    // we assume that our events do not address each other. We reset the
    // object count to what it was at the beginning of the event.
    TProcessID::SetObjectCount(ObjectNumber);
    
    if ( gDebug > 1 )
        Info(__FUNCTION__, "End entry  %lld", entry);
    return kTRUE;
}

//____________________________________________________________________

void AtlSelector::SlaveTerminate() {
    //
    // The SlaveTerminate() function is called after all entries or objects
    // have been processed. When running with PROOF SlaveTerminate() is called
    // on each slave server.
    //
    Info("SlaveTerminate", "Terminating slave process");
    delete fEvent;

    // Close last input file
    if( fCurrentTree != 0 ) {
        TFile *file = fCurrentTree->GetCurrentFile();
        if ( file != 0 )
            file->Close();
        else
            Warning(__FUNCTION__, "No current file to close; was there an error in the input?");
    }

    // Call terminate functions for all tools
    AtlAnalysisTool *tool = 0;
    TIter next_tool(fListOfTools);
    while ( (tool = (AtlAnalysisTool*)next_tool()) ) {
	tool->Terminate();
    }

    // Perform bookkeeping on the remaining input files (if any)
    Info("SlaveTerminate", "Finish bookkeeping ...   %d input files left",
	fNBookkeeping);
    TIter next_el(fBookkeepingList);
    TChainElement *el = 0;
    while ( (el = (TChainElement*)next_el()) ) {
	TFile *infile = TFile::Open(el->GetTitle(),"read");
	DoBookkeeping(infile);
	delete infile;
    }
    if ( fNBookkeeping > 0 ) {
	Warning("SlaveTerminate", "Bookkeeping left for %d input files.",
		fNBookkeeping);
    } else {
	Info("SlaveTerminate", "Bookkeeping done for all input files.");
    }
	
    // Clean-up
    delete fProcInfo;
    delete fMemInfo;
}

//____________________________________________________________________

void AtlSelector::Terminate() {
    //
    // The Terminate() function is the last function to be called during
    // a query. It always runs on the client, it can be used to present
    // the results graphically or save the results to file.
    Info("Terminate", "Terminating analysis job ...");

    // Terminate event writer
    if ( fEvtWriter != 0 ) fEvtWriter->Terminate();
    
    // Write output file
    cout << endl;
    Info("Terminate", "Write output file \"%s\".",
	 fOutputFilename->Data());
    fOutputFile->cd();
    fOutputFile->Write();

    // Stop timer
    fStopwatch.Stop();
    
    // Print summary
    PrintSummary();
}

//____________________________________________________________________

void AtlSelector::ProcessFill() {
    //
    // Event fill routine (histograms etc)
    //

    // Fill user-defined histograms
    FillHistograms();

    // Fill tool histograms
    AtlAnalysisTool *tool = 0;
    TIter next_tool(fListOfTools);
    while ( (tool = (AtlAnalysisTool*)next_tool()) ) {
	tool->FillHistograms();
    }

    // Maximum output filesize reached ?
    if ( fOutputFile != 0 &&
	 fOutputFile->GetEND() > fOutputTree->GetMaxTreeSize() )
	ChangeOutputFile();
    
    // Fill output tree/ntuple (optional)
    if ( fOutputTree != 0 ) fEvtWriter->WriteEvent();
    
    // Fill entry list (optional)
    if ( fWriteEntryList ) {
      fEntryList->Enter(fEntry);
    }

}

//____________________________________________________________________

void AtlSelector::SetCutDefaults() {
    //
    // Set Default Values for public member variables.
    // When implementing this virtual method in a derived
    // Selector class, make sure to call this base method 
    // as well.
    // SetCutDefaults should be called in the ctor of the
    // derived class.
    // 
    // void DerivedSelector::SetCutDefaults(){
    //   AtlSelector::SetCutDefaults(); // calls base portion
    //   fFoo = bar; // set default value of fFoo to bar
    //   ...
    // }
}


//____________________________________________________________________

void AtlSelector::ProcessInfo() {
    //
    // Print information about the current process.  For interactive
    // (ie non-batch) jobs continuous output is given while for batch
    // jobs only a few out is given to avoid too much junk in the log
    // file
    //
    if ( gROOT->IsBatch() ) {
	Int_t mod = 0;
	if ( fProcessedEvents < 10 ) {
	    mod = 1;
	} else if ( fProcessedEvents < 1000 ) {
	    mod = 100;
	} else if ( fProcessedEvents < 10000 ) {
	    mod = 1000;
	} else {
	    mod = 10000;
	}
	if ( (fProcessedEvents+1) % mod == 0 )  {
	    printf("<AtlSelector::Process>: Processing chain \"%s\"  Run %-d  Evt %-llu",
		   fTree->GetName(), fEvent->RunNr(), fEvent->EventNr());
	    printf("    #Chain entry %-d/%-d\n",
		   (Int_t)fTree->GetReadEntry(),
		   (Int_t)fTree->GetEntriesFast());
	    gSystem->GetProcInfo(fProcInfo);
	    gSystem->GetMemInfo(fMemInfo);
	    printf("<AtlSelector::Process>: Memory usage proc_res=%d proc_virt=%d total=%d Mb\n",
		   Int_t(fProcInfo->fMemResident/1024),
		   Int_t(fProcInfo->fMemVirtual/1024),
		   fMemInfo->fMemTotal);
	    cout.flush();
	}
    } else {
	printf("\r<AtlSelector::Process>: Processing chain \"%s\"  Run %-d  Evt %-llu",
	       fTree->GetName(), fEvent->RunNr(), fEvent->EventNr());
	printf("    #Chain entry %-d/%-d",
	       (Int_t)fTree->GetReadEntry(),
	       (Int_t)fTree->GetEntriesFast());
	cout.flush();
    }
}

//____________________________________________________________________

void AtlSelector::PrintSummary() {
    //
    // Print summary at end of job
    //
    cout << endl << endl;
    cout << "================================================================"
	 << endl
	 << "                        Job Summary"  << endl
	 << "================================================================"
	 << endl
	 << "No. of processed events,       unweighted : "
	 << fProcessedEvents << endl
	 << "No. of processed events,         weighted : "
	 << fProcessedEventsW << endl
	 << "No. of events in jet bin,      unweighted : "
	 << fJetBinEvents << endl
	 << "No. of events in jet bin,        weighted : "
	 << fJetBinEventsW << endl
	 << "No. of pre-accepted events,    unweighted : "
	 << fPreAcceptedEvents << endl
	 << "No. of pre-accepted events,      weighted : "
	 << fPreAcceptedEventsW << endl
	 << "No. of pre-tagged events,      unweighted : "
	 << fPreTaggedEvents << endl
	 << "No. of pre-tagged events,        weighted : "
	 << fPreTaggedEventsW << endl
	 << "No. of accepted events,        unweighted : "
	 << fAcceptedEvents << endl
	 << "No. of accepted events (pretag), weighted : "
	 << fAcceptedEventsW << endl
	 << "No. of accepted events (tag),    weighted : "
	 << fAcceptedEventsB << endl
	 << "Time consumption                          : ";
    fStopwatch.Print();
    cout << "Job status                                : successful" << endl
	 << endl
	 << "For more information have a look at the histograms inside the"
	 << endl
	 << "JobInfo folder of the output .root file" << endl
	 << "================================================================"
	 << endl << endl;
}

//____________________________________________________________________

void AtlSelector::AddTool(AtlAnalysisTool *tool) {
    //
    // Add analysis tool to list of tools
    //
    fListOfTools->Add(tool);
    tool->SetParent(this);
    tool->SetEvent(fEvent);
    tool->SetTree(fTree);
}

//____________________________________________________________________

AtlAnalysisTool* AtlSelector::GetTool(const char* ClassName,
				      const char* ToolName, Bool_t force) { 
    //
    // Retrieves Tool from fListOfTools
    // If parameter force is true, the method will abort if no tool with
    // the description is found
    // otherwise it won't bother and simply return 0
    //
    AtlAnalysisTool *itertool = 0;
    AtlAnalysisTool *tool = 0;
    Int_t toolcount = 0;

    TIter next_tool(fListOfTools);
    next_tool.Reset();

    // searching tool
    while ( (itertool = (AtlAnalysisTool*)next_tool()) ) {
	if( ((TString)itertool->ClassName()).Contains(ClassName) && 
	    ( strcmp(ToolName, "") ==  0
	      || strcmp(itertool->GetName(), ToolName) == 0) ) {
	    tool = itertool;
	    toolcount++;
	}
    }

    // Checking if everything is alright
    if ( toolcount == 1 ){
	return tool;
    } else if ( toolcount == 0 ) {
	if ( force == kTRUE ) {
	    Error("GetTool",
		  "Requested tool %s of class %s not found!",
		  ToolName, ClassName);
	    gSystem->Abort(0);
	    return 0;
	} else {
	    return 0;
	}
    } else {
	Error("GetTool",
	      "Request with toolname %s of class %s returned %d instances! Please be more specific.",
	      ToolName, ClassName, toolcount);
	gSystem->Abort(0);
	return 0;
    }
}

//____________________________________________________________________

AtlTriggerConf* AtlSelector::LoadTriggerConfig() {
    //
    // Load trigger configuration
    //
    fCurrentTree->GetCurrentFile()->cd();
    
    // Trigger config
    AtlTriggerConf *trig_conf = AtlTriggerConf::Instance()
	->LoadTree(gDirectory);
    if ( trig_conf == 0 ) {
	Error("LoadTriggerConfig", "No trigger configuration dbase found! Do not try to access any trigger information.");
    }
    AtlTrigger::SetConfig(trig_conf);
    return trig_conf;
}

//____________________________________________________________________

void AtlSelector::BookJobInfoHistograms() {
    //
    // Book histograms containing information about the analysis job
    // like the no. of processed events, the event weights etc.
    //
    // The binning for the nevt histogram is as follows:
    // Bin  1 : All unfiltered events (unweighted)
    // Bin  2 : All unfiltered events (MC weight only)
    // Bin  3 : All unfiltered events (all weights)
    // Bin  4 : Processed events (unweighted)
    // Bin  5 : Processed events (MC weight only)
    // Bin  6 : Processed events (all weights)
    // Bin  7 : Pre-accepted events by ProcessPreCut() (unweighted)
    // Bin  8 : Pre-accepted events by ProcessPreCut() (MC weights only)
    // Bin  9 : Pre-accepted events by ProcessPreCut() (all weights)
    // Bin 10 : Accepted events by ProcessCut() (unweighted)
    // Bin 11 : Accepted events by ProcessCut() (MC weight only)
    // Bin 12 : Accepted events by ProcessCut() (all weights)
    //
    TDirectory *savdir = gDirectory;
    fOutputFile->cd();
    gDirectory->mkdir("job_info", "Analysis Job Information");
    gDirectory->cd("job_info");

    // No. of events
    fHistEvents = new TH1F("h_nevts", "No. of Events", 12, 0, 12);
    fHistEvents->GetXaxis()->SetBinLabel(1,  "All unfiltered Evts (unweighted)");
    fHistEvents->GetXaxis()->SetBinLabel(2,  "All unfiltered Evts (MC weights)");
    fHistEvents->GetXaxis()->SetBinLabel(3,  "All unfiltered Evts (all weights)");
    fHistEvents->GetXaxis()->SetBinLabel(4,  "Processed Evts (unweighted)");
    fHistEvents->GetXaxis()->SetBinLabel(5,  "Processed Evts (MC weights)");
    fHistEvents->GetXaxis()->SetBinLabel(6,  "Processed Evts (all weights)");
    fHistEvents->GetXaxis()->SetBinLabel(7,  "Pre-accepted Evts (unweighted)");
    fHistEvents->GetXaxis()->SetBinLabel(8,  "Pre-accepted Evts (MC weights)");
    fHistEvents->GetXaxis()->SetBinLabel(9,  "Pre-accepted Evts (all weights)");
    fHistEvents->GetXaxis()->SetBinLabel(10, "Accepted Evts (unweighted)");
    fHistEvents->GetXaxis()->SetBinLabel(11, "Accepted Evts (MC weights)");
    fHistEvents->GetXaxis()->SetBinLabel(12, "Accepted Evts (all weights)");
    fHistEvents->SetYTitle("Number of Events");
    fHistEvents->SetStats(kFALSE);
    
    // Event weights
    fHistEvtWeights = new TH1F("h_evtweight", "Event Weights (all processed events)",
                               52, -1.6, 3.6);
    fHistEvtWeights->SetXTitle("Weight");
    fHistEvtWeights->SetYTitle("Number of Entries");
    fHistEvtWeights->SetStats(kFALSE);

    // event weights with extended x axis
    fHistEvtWeightsExtended = std::make_unique<TH1D>(
        "h_evtweight_extended", "Event Weights (all processed events)",
        1100, -10., 100.);
    fHistEvtWeightsExtended->SetXTitle("Weight");
    fHistEvtWeightsExtended->SetYTitle("Number of Entries");
    fHistEvtWeightsExtended->SetStats(kFALSE);

    savdir->cd();
}

//___________________________________________________________________

void AtlSelector::ChangeOutputFile() {
    //
    // This method is used to prevent Root from wrinting all
    // histograms only to the last file in case of splited output
    // files (see TTree::ChangeFile()). Instead the histograms are
    // splitted as well and written to their corresponding output
    // file.
    //

    // Write all histograms to the old file and reset them
    TObject *h = 0;
    while ( (h = fOutputFile->GetList()->First()) ) {
	if ( h->InheritsFrom("TH1") ) {
	    h->Write();
	    ((TH1*)h)->Reset();
	}
    }
}

//___________________________________________________________________

const char* AtlSelector::GetJetMultLabel(EJetMult jetmult) {
    //
    // Get Jet Multiplicity label
    //
    for ( Int_t i = 0; i < AtlSelector::fgNumJetMults; i++ ) {
	if ( jetmult & 0x1 << i ) return AtlSelector::GetJetMultLabel(i);
    }
    return "";
}

//___________________________________________________________________

const char* AtlSelector::GetJetMultLabel(Int_t i) {
    //
    // Returns Jet Multiplicity label ('1', '2', ...)
    //
    
    if ( i >= fgNumJetMults ) {
	cout << "Jet Multiplicity unknown! Abort!" << endl;
	gSystem->Abort(0);
    }
    return fgJetMultNames[i];
}

//___________________________________________________________________

const char* AtlSelector::GetLeptonLabel(ELepChannel lep) {
    //
    // Get Lepton Label
    // 
    // Works only for single-leptons
    //
    if ( lep & kElectron ) { return fgLepChannelNames[0]; };
    if ( lep & kMuon ) { return fgLepChannelNames[1]; };
	if ( lep & kLepton ) { return fgLepChannelNames[2]; };
    
    return "";
}

//___________________________________________________________________

const char* AtlSelector::GetLeptonLabel(Int_t i) {
    //
    // Returns Lepton Channel Label
    //
    if ( i >= fgNumLepChannels + 1 ) {
		cout << "Lepton Channel unknown! Abort!" << endl;
		gSystem->Abort(0);
    }
    return fgLepChannelNames[i];
}

//___________________________________________________________________

void AtlSelector::Clear(Option_t */*option*/) {
    //
    // Clear AtlSelector
    //
    // User has to call this function in derived selector
    //
}

//___________________________________________________________________

const char* AtlSelector::GetJetMultEnum(UInt_t jetmult) {
    //
    // Get Jet Multiplicity Enum ('kOneJet', 'kTwoJet', ...)
    //
    switch ( jetmult ) {
	case AtlSelector::kOneJet:
	    return "AtlSelector::kOneJet";
	case AtlSelector::kTwoJet:
	    return "AtlSelector::kTwoJet";
	case AtlSelector::kThreeJet:
	    return "AtlSelector::kThreeJet";
	case AtlSelector::kFourJet:
	    return "AtlSelector::kFourJet";
	case AtlSelector::kFiveJet:
	    return "AtlSelector::kFiveJet";
	case AtlSelector::kSixJet:
	    return "AtlSelector::kSixJet";
	case AtlSelector::k4To6Jets:
	    return "AtlSelector::k4To6Jets";
	case AtlSelector::kOneOrMoreJets:
		return "AtlSelector::kOneOrMoreJets";
	case AtlSelector::kTwoOrMoreJets:
		return "AtlSelector::kTwoOrMoreJets";
	case AtlSelector::kThreeOrMoreJets:
	    return "AtlSelector::kThreeOrMoreJets";
	case AtlSelector::kAllJets:
		return "AtlSelector::kAllJets";
	default:
		cout << "<AtlSelector::GetJetMultEnum>: Jet Multiplicity not known! Abort!" << endl;
		gSystem->Abort(0);
	}
	return "";
}

//___________________________________________________________________

const char* AtlSelector::GetLeptonEnum(UInt_t lep) {
	//
	// Get Lepton Channel Enum ('kElectron', 'kMuon', ...)
	//
	switch ( lep ) {
		case AtlSelector::kElectron:
			return "AtlSelector::kElectron";
		case AtlSelector::kMuon:
			return "AtlSelector::kMuon";
		case AtlSelector::kLepton:
			return "AtlSelector::kLepton";
		default:
			cout << "<AtlSelector::GetLepChannelEnum>: Lepton not known! Abort!" << endl;
			gSystem->Abort(0);
	}
	return "";
}

//____________________________________________________________________

void AtlSelector::Print(Option_t */*option*/) const {
    //
    // Print configuration of base selector.
    // (this function is supposed to be used from inside the user
    // analysis Print()
    //
    fEvtReader->Print();
    fEvtWriter->Print();
    cout << endl << "  fDoTruthTree   = ";
    if ( fDoTruthTree ) {
	cout << "true";
    } else {
	cout << "false";
    }
    cout << endl;
    cout << "  fInputMode     = " << fInputMode << endl;
    if ( fNProcessNthEventsOnly > 1 ) 
        cout << "  fNProcessNthEventsOnly = " << fNProcessNthEventsOnly << endl;
    cout << endl;
}

//____________________________________________________________________

void AtlSelector::SetEvtReader(AtlEvtReaderBase * reader, Bool_t use_job_info) {
    //
    // Explcitly sets the event reader.
    // Ownership of the reader is transferred to AtlSelector!
    //
    // If use_job_info is set, the event weights are taken from existing job_info
    // histograms instead of cutflow histograms (for already processed events).
    //
    if ( fEvtReader ) {
        Fatal(__FUNCTION__, "... called too late!");
    }
    if (use_job_info) {
        fInputMode = kCustomMem;
    } else {
        fInputMode = kCustom;
    }
    fEvtReaderUser = reader;
}

//____________________________________________________________________

void AtlSelector::SetInputMode(EIOMode inputMode) {
	//
	// Sets the input mode.
	// Deprecated, please use SetEvtReader() if possible.
	//
	if ( fEvtReader ) {
		Fatal(__FUNCTION__, "... called too late!");
	}
	if ( fEvtReaderUser ) {
		delete fEvtReaderUser;
		fEvtReaderUser = 0;
	}
	fInputMode = inputMode;
}

//____________________________________________________________________
//
// Selector for analysing TopD3PDs/CommonD3PDs with
// the PdfReweightingTool
//
// The selector is only needed for an event loop over all events in
// order to extract the global correction factor for a specific sample.
// If one wants to use the PdfReweightingTool in one's analysis, this
// selector is NOT needed! 
//
// Inherit from this class in order to write your own specific
// analysis selector for a different D3PD format (only needed if branch
// names for truth info is different).
//
//
// Author: Soeren Stamm <mailto:stamm@physik.hu-berlin.de>
// Copyright: 2013 (C) Soeren Stamm
//
#ifndef PdfSelector_h
#include "PdfReweightingTool/PdfSelector.h"
#endif
#include "PdfReweightingTool/PdfReweightingTool.h"
#include <TString.h>
#include <TObjectTable.h>
#include <iostream>
#include <TROOT.h>
#include <TKey.h>
#include <TProcessID.h>

using namespace std;

#ifndef __CINT__
ClassImp(PdfSelector);
#endif

//____________________________________________________________________

PdfSelector::PdfSelector(Int_t DSID) : TSelector() {
    //
    // Default constructor
    //
    fTree = 0;
    fCurrentTree = 0;
    fProcessedEvents    = 0;
    fEntry = 0;
    fPrintObjectTable = kFALSE;
    fOutputFile = 0;

    fOutputFilename = new TString(Form("%d.root", DSID));
    fDSID    = DSID;

    fPdfTool = PdfReweightingTool::Instance();
    
    // ntuple contents
    v_RunNr = 0;
    v_EvtNr = 0;
    v_mcevt_pdf1 = 0;
    v_mcevt_pdf2 = 0;
    v_mcevt_pdf_id1 = 0;
    v_mcevt_pdf_id2 = 0;
    v_mcevt_pdf_scale = 0;
    v_mcevt_pdf_x1 = 0;
    v_mcevt_pdf_x2 = 0;
    
    b_RunNumber   = 0;
    b_EventNumber = 0;
    b_mcevt_pdf1  = 0;
    b_mcevt_pdf2  = 0;
    b_mcevt_pdf_id1 = 0;
    b_mcevt_pdf_id2 = 0;
    b_mcevt_pdf_scale = 0;
    b_mcevt_pdf_x1 = 0;
    b_mcevt_pdf_x2 = 0;
}

//____________________________________________________________________

PdfSelector::~PdfSelector() {
    //
    // Default destructor
    //
    if ( fOutputFile != 0 ) delete fOutputFile;
    delete fOutputFilename;
    delete fPdfTool;
}

//____________________________________________________________________

void PdfSelector::SetBranches(TTree *tree) {
  //
  // Set branch assignments of the input tree.
  //
  // If your branch names are different from the ones given here,
  // overwrite this function and replace the names but leave the
  // data members unchanged.
  //

  // Turn of all branches by default
  tree->SetMakeClass(1);
  tree->SetBranchStatus("*", kFALSE);

  SetupBranch(tree, "RunNumber",   &v_RunNr, &b_RunNumber);
  SetupBranch(tree, "EventNumber", &v_EvtNr, &b_EventNumber);
  SetupBranch(tree, "mc_event_weight", &v_GenWeight, &b_GeneratorWeight);
  SetupBranch(tree, "mcevt_pdf1",    &v_mcevt_pdf1,    &b_mcevt_pdf1);
  SetupBranch(tree, "mcevt_pdf2",    &v_mcevt_pdf2,    &b_mcevt_pdf2);
  SetupBranch(tree, "mcevt_pdf_id1", &v_mcevt_pdf_id1, &b_mcevt_pdf_id1);
  SetupBranch(tree, "mcevt_pdf_id2", &v_mcevt_pdf_id2, &b_mcevt_pdf_id2);
  SetupBranch(tree, "mcevt_pdf_scale", &v_mcevt_pdf_scale, &b_mcevt_pdf_scale);
  SetupBranch(tree, "mcevt_pdf_x1",  &v_mcevt_pdf_x1,  &b_mcevt_pdf_x1);
  SetupBranch(tree, "mcevt_pdf_x2",  &v_mcevt_pdf_x2,  &b_mcevt_pdf_x2);
}

//____________________________________________________________________

Int_t PdfSelector::GetEntry(Long64_t entry) {
    //
    // Get entry from current input tree. This (virtual) method can be
    // overloaded if needed (different type of input tree)
    //
    fEntry = entry;
    return fCurrentTree->GetEntry(entry);
}

//____________________________________________________________________

void PdfSelector::Init(TTree *tree) {
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

void PdfSelector::SetupBranch(TTree *tree,
			      const char* branchname,
			      void* value, TBranch** branch) {
  //
  // Set branch assignment and set branch to true
  //
  tree->SetBranchStatus(branchname, kTRUE);
  tree->SetBranchAddress(branchname, value, branch);
}

//____________________________________________________________________

Bool_t PdfSelector::Notify() {
    //
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when a new TTree
    // is started when using PROOF.
    //
    
    // Fetch current tree
    fCurrentTree = ( fIsChain ) ? fTree->GetTree() : fTree;

    // Note that Notify() usually is run once before the event loop by
    // TTreePlayer::Process(). In this case we do not need the
    // following code to be executed.
    if ( fCurrentTree == 0 ) return kTRUE;
    Info("Notify", "\nNew input file %s opened.", 
	 fCurrentTree->GetCurrentFile()->GetName());

    // Any events inside the tree ?
    if ( fCurrentTree->GetEntries() == 0 ) {
	Warning("Notify",
		"The event tree in the current input file contains 0 events.");
    }

    return kTRUE;
}

//____________________________________________________________________

void PdfSelector::Begin(TTree *tree) {
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
	 "\n====================== Start analysis job here ======================\n");
    
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
	} else {
	    Info("Begin", "This job contains %d input files.", nfiles);
	}
    } else {
	fIsChain = kFALSE;
    }
    
    // Open output file with maximum compression level
    if ( fOutputFilename != 0 ) {
      Info("Begin", "Open output file \"%s\"", fOutputFilename->Data());
      fOutputFile = new TFile(fOutputFilename->Data(), "recreate");
      fOutputFile->SetCompressionLevel(9);
    }
}

//____________________________________________________________________

void PdfSelector::SlaveBegin(TTree *tree) {
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
    
    //
    // Map branch addresses to branch pointers
    // =======================================
    Info("SlaveBegin", "Set branch addresses.");
    SetBranches(tree);
    
    //
    // Init PdfTool
    // ============
    Info("SlaveBegin", "InitPdfTool ...");
    InitPdfTool();

    //
    // Print analysis configuration
    // ============================
    
    // Print user analysis configuration
    Info("SlaveBegin", "User analysis configuration:");
    Print();
}

//____________________________________________________________________

Bool_t PdfSelector::Process(Long64_t entry) {
    //
    // The Process() function is called for each entry in the tree (or possibly
    // keyed object in the case of PROOF) to be processed. The entry argument
    // specifies which entry in the currently loaded tree is to be processed.
    // It can be passed to either PdfSelector::GetEntry() or
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
  
    // Save current Object count
    Int_t ObjectNumber = TProcessID::GetObjectCount();

    // =============
    // Step 1: Clear
    // =============
    
    // Clear analysis selector
    PdfSelector::Clear(); // Clear objects defined here
    Clear(); // Clear user-defined objects
    
    // ========================
    // Step 2: Fetch next event
    // =======================
    GetEntry(entry);
    
    // Process info
    ProcessInfo();

    // ===================
    // Step 3: Bookkeeping
    // ===================
    
    // This is for tracking memory leaks
    // Make sure your .rootrc file is properly set
    if ( fPrintObjectTable == kTRUE ) {
	gObjectTable->Print();
    }
    fProcessedEvents++;

    // =====================
    // Step 4: Main Analysis
    // =====================
    ProcessEvent();
    
    // Restore Object count
    // To save space in the table keeping track of all referenced objects
    // we assume that our events do not address each other. We reset the
    // object count to what it was at the beginning of the event.
    TProcessID::SetObjectCount(ObjectNumber);
    
    return kTRUE;
}

//____________________________________________________________________

void PdfSelector::SlaveTerminate() {
    //
    // The SlaveTerminate() function is called after all entries or objects
    // have been processed. When running with PROOF SlaveTerminate() is called
    // on each slave server.
    //
    Info("SlaveTerminate", "\nTerminating slave process");
    
    // Close last input file
    if( fCurrentTree != 0 ) fCurrentTree->GetCurrentFile()->Close();

    delete fProcInfo;
    delete fMemInfo;
}

//____________________________________________________________________

void PdfSelector::Terminate() {
    //
    // The Terminate() function is the last function to be called during
    // a query. It always runs on the client, it can be used to present
    // the results graphically or save the results to file.
    Info("Terminate", "Terminating analysis job ...");
    fOutputFile->cd();

    // Warning:
    // --------
    // Do NOT compute the scale factor here. If you have multiple
    // output files (e.g. when running on the grid), you need to add
    // all weights first (i.e. enumerator and denominator separatly)
    // and then compute the scale factor.
    //
    // --> This is done in a separate step!
    //
    // Get the indivdual reweighted histograms
    for ( Int_t i = 0; i < PdfSet::fgNumPdfSets; i++ ) {
      TList* list = fPdfTool->GetReweightedObservable(fHistPdf,
						      PdfSet::fgPdfNames[i]);
      TIter next_hist(list);
      TH1D* hist = 0;
      while ( (hist = (TH1D*) next_hist()) ) {
	hist->SetDirectory(fOutputFile);
      }
    }
    
    // Write output file
    cout << endl;
    Info("Terminate", "Write output file \"%s\".",
     	 fOutputFilename->Data());
    fOutputFile->Write();

    // Stop timer
    fStopwatch.Stop();
    
    // Print summary
    PrintSummary();
}

//____________________________________________________________________

void PdfSelector::ProcessInfo() {
  //
  // Print information about the current process.  For interactive
  // (ie non-batch) jobs continuous output is given while for batch
  // jobs only a few out is given to avoid too much junk in the log
  // file
  //
  
  Int_t mod = 0;
  if ( fProcessedEvents < 1000 ) {
    mod = 100;
  } else {
    mod = 100;
  }
  if ( (fProcessedEvents) % mod == 0 )  {
    Info("ProcessInfo", "Processing chain \"%s\"  Run %-d  Evt %-d",
	   fTree->GetName(), v_RunNr, v_EvtNr);
    Info("ProcessInfo", "    #Chain entry %-d/%-d\n",
	 (Int_t)fTree->GetReadEntry(),
	 (Int_t)fTree->GetEntriesFast());
  } if ( (fProcessedEvents) % 10000 == 0 ) {
    gSystem->GetProcInfo(fProcInfo);
    gSystem->GetMemInfo(fMemInfo);
    Info("ProcessInfo", "Memory usage proc_res=%d proc_virt=%d total=%d Mb\n",
	 Int_t(fProcInfo->fMemResident/1024),
	 Int_t(fProcInfo->fMemVirtual/1024),
	 fMemInfo->fMemTotal);
  }
}

//____________________________________________________________________

void PdfSelector::PrintSummary() {
    //
    // Print summary at end of job
    //
    cout << endl << endl;
    cout << "================================================================"
	 << endl
	 << "                        Job Summary"  << endl
	 << "================================================================"
	 << endl
	 << "No. of processed events : " << fProcessedEvents << endl
	 << "Time consumption        : ";
    fStopwatch.Print();
    cout << endl
	 << "================================================================"
	 << endl << endl;
}

//___________________________________________________________________

void PdfSelector::ChangeOutputFile() {
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

//____________________________________________________________________

void PdfSelector::Print(Option_t *option) const {
    //
    // Print configuration of base selector.
    //
}

//____________________________________________________________________

void PdfSelector::ProcessEvent() {
  //
  // Process current event
  //
  
  // Protection against broken truth record
  if(v_mcevt_pdf1      && v_mcevt_pdf2 &&
     v_mcevt_pdf_id1   && v_mcevt_pdf_id2 &&
     v_mcevt_pdf_scale &&
     v_mcevt_pdf_x1    &&  v_mcevt_pdf_x2) {
    if(v_mcevt_pdf1->size() > 0      && v_mcevt_pdf2->size() > 0 &&
       v_mcevt_pdf_id1->size() > 0   &&  v_mcevt_pdf_id2->size() > 0 &&
       v_mcevt_pdf_scale->size() > 0 &&
       v_mcevt_pdf_x1->size() > 0    &&  v_mcevt_pdf_x2->size() > 0) {
      
      // compute weights
      fPdfTool->ComputePdfWeights(v_mcevt_pdf_x1->at(0),
				  v_mcevt_pdf_x2->at(0),
				  v_mcevt_pdf_scale->at(0),
				  v_mcevt_pdf_id1->at(0),
				  v_mcevt_pdf_id2->at(0),
				  v_mcevt_pdf1->at(0),
				  v_mcevt_pdf2->at(0));
      
      // Fill histogram and the PdfReweightingTool histogram
      // ---------------------------------------------------
      // observable = 1., weight = generator weight
      fHistPdf->Fill(1., v_GenWeight);
      
      // The tool will reweight this with:
      //
      // new_pdf1 * new_pdf2 / ( old_pdf1 * old_pdf2 )
      //
      // One has to keep the generator weight in order to be able
      // to normalize correctly on the MC xsection
      //
      // observable = 1., weight = generator weight,
      fPdfTool->FillObservable(fHistPdf, 1., v_GenWeight);
    }
  }
}

//____________________________________________________________________

void PdfSelector::InitPdfTool() {
  //
  // Init PdfTool
  //
    
  // Set Sumw2 for all histograms
  TH1::SetDefaultSumw2(kTRUE);
  
  // Book histograms (only one)
  fOutputFile->cd();
  fHistPdf = new TH1F("HistPdf", "HistPdf", 1, 0.5, 1.5);
  
  // reweight a simple event count histogram
  fPdfTool->AddObservable(fHistPdf);

  Info("InitPdfTool", "Initializing PdfTool done ...");
}

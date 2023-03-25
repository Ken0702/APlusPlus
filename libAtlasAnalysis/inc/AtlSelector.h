//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlSelector
#define ATLAS_AtlSelector
#ifndef ROOT_TSelector
#include <TSelector.h>
#endif
#ifndef ROOT_TList
#include <TList.h>
#endif
#ifndef ROOT_TChain
#include <TChain.h>
#endif
#ifndef ROOT_TFile
#include <TFile.h>
#endif
#ifndef ROOT_TStopwatch
#include <TStopwatch.h>
#endif
#ifndef ATLAS_AtlEvent
#include <AtlEvent.h>
#endif
#ifndef ATLAS_AtlAnalysisTool
#include <AtlAnalysisTool.h>
#endif
#ifndef ATLAS_AtlTriggerConf
#include <AtlTriggerConf.h>
#endif
#ifndef ROOT_TEntryList
#include <TEntryList.h>
#endif
#ifndef ROOT_TH1F
#include <TH1F.h>
#endif
#ifndef ROOT_TH1D
#include <TH1D.h>
#endif

class TString;
class TROOT;
class TSystem;
class AtlEvtReaderBase;
class AtlEvtWriterBase;
class TChainElement;

class AtlSelector : public TSelector {
    
  public:
    enum EIOMode { kApp,
		   kD3PDSgTop,
		   kD3PDSgTop_v2,
		   kD3PDCKM,
		   kMemNtuple,
		   kCustom,    // when setting the event reader in AtlTopLevelAnalysis (Run 2)
                   kCustomMem, // like kCustom, but for processed events with single tree
    };

    static const Int_t fgNumLepChannels = 2;
    static const Int_t fgNumJetMults = 11;
    
    enum ELepChannel {
	kElectron = BIT(0), // Electron channel
	kMuon     = BIT(1), // Muon channel
	kLepton   = BIT(2)  // Electron + Muon merged channel
    };
    
    enum EJetMult {
	kOneJet        = BIT(0),
	kTwoJet        = BIT(1),
	kThreeJet      = BIT(2),
	kFourJet       = BIT(3),
	kFiveJet       = BIT(4),
	kSixJet        = BIT(5),
	k4To6Jets      = BIT(6),
	kOneOrMoreJets = BIT(7),
	kTwoOrMoreJets = BIT(8),
	kThreeOrMoreJets = BIT(9),
	kAllJets       = BIT(10)
    };

    Bool_t fDoTruthTree; // Read and process TruthTree information. Caution: this is a very time-consuming operation (Run2 SgTop-D3PDs only; default=false)
    Bool_t fCopyCutflowHistograms; // Copy run-2 cutflow histograms, default=kFALSE
    
  protected:
    EIOMode       fInputMode;   // Defines input file mode
    EIOMode       fOutputMode;  // Defines mode for the output tree (if any)

    TTree      *fTree;                  // Pointer to the currently analysed tree or chain
    TTree      *fCurrentTree;           // Currently analysed tree
    TObjArray  *fBookkeepingList;       // List of chain elements needed for bookkeeping
    Int_t       fNBookkeeping;          // No. of bookkeeping files
    AtlEvtReaderBase *fEvtReader;       // Reader for input events
    AtlEvtReaderBase *fEvtReaderUser;   // User-provided reader for input events (owned!)
    AtlEvtWriterBase *fEvtWriter;       // Writer for output event
    AtlEvent   *fEvent;                 // Point to event object in memory
    TFile      *fOutputFile;            // Output file
    TString    *fOutputFilename;        // Name of the output file
    Bool_t      fIsChain;               // Input is chain or tree ?
    TString    *fOutputTreeName;        // Name of the A++ output tree (if any)
    TString    *fOutputTreeTitle;       // Title of the A++ output tree (if any)
    TTree      *fOutputTree;            // A++ output tree (if any)
    TTree      *fOutputTriggerConfTree; // Needed for copying the trigger config dbases into the output file in case of written out A++ events 
    Int_t       fNProcessNthEventsOnly; // process only every Nth event (default=1 every event)
    Int_t       fProcessedEvents;       // No. of processed events (w/o weights)
    Int_t       fAcceptedEvents;        // No. of accepted events (w/o weights) selected by ProcessCut()
    Int_t       fPreAcceptedEvents;     // No. of pre-accepted events (w/o weights) selected by ProcessPreCut()
    Int_t       fJetBinEvents;          // No. of events in jet bin (w/o weights)
    Float_t     fProcessedEventsW;      // No. of processed events (weighted)
    Float_t     fAcceptedEventsW;       // No. of accepted events (weighted) selected by ProcessCut()
    Float_t     fAcceptedEventsB;       // No. of accepted events (weighted) selected by ProcessCut()
    Float_t     fPreAcceptedEventsW;    // No. of pre-accepted events (weighted) selected by ProcessPreCut()
    Float_t     fJetBinEventsW;         // No. of events in jet bin (weighted)
    Float_t     fPreTaggedEvents;       // No. of events before b-tagging
    Float_t     fPreTaggedEventsW;       // No. of events before b-tagging (weighted)
    TStopwatch  fStopwatch;             // Stopwatch used for process monitoring
    Long64_t    fEntry;                 // Global entry number of the current event in input chain
    TList      *fListOfTools;           // List of analysis tools
    Bool_t      fWriteEntryList;        // Switch for writing an entry list
    TEntryList *fEntryList;             // Global entry list for all files in a chain
    TH1F       *fHistEvents;            // Histogram for no. of processed and accepted events etc.
    TH1F       *fHistEvtWeights;        // Histogram of event weights
    TH1D       *fHistCutflow_NoWeights;                 // SgTopD3PD CutFlow Histogram
    TH1D       *fHistCutflow_genWeights;                // SgTopD3PD CutFlow Histogram
    TH1D       *fHistCutflow_genXpileupWeights;         // SgTopD3PD CutFlow Histogram
    TH1D       *fHistCutflow_genXpileupXzvertexWeights; // SgTopD3PD CutFlow Histogram
    TH1D       *fHistCutflow_allEventsHFOR_NoWeights;   // SgTopD3PD CutFlow Histogram
    TH1D       *fHistCutflow_allEventsHFOR_genWeights;  // SgTopD3PD CutFlow Histogram
    TObjArray  *fHistsArrayCutflow;     // Run-2 cutflow histograms
    ProcInfo_t *fProcInfo;              // Current process information
    MemInfo_t  *fMemInfo;               // Current memory usage information
    Bool_t      fPrintEvent;            // Print the content of every event (useful for testing/debugging)
    Bool_t      fPrintObjectTable;      // Print Root's object table for every event (useful for tracking memory leaks)
    Bool_t      fPassedSelection;       // variable for storing the selection decisions for the event
    Bool_t      fCountUnfilteredEvents; // Count the no. of all unfiltered events (in case the input A++ file does not contain a JobInfo histogram)
    Int_t       fNProcessedFiles;       // No. of fully processed files
    std::unique_ptr<TH1D> fHistEvtWeightsExtended;  // Histogram of event weights, larger x-axis

  public:
    AtlSelector(const char* OutputFilename);
    virtual ~AtlSelector();

    virtual Int_t     Version() const override { return 2; }
    virtual void      Begin(TTree *tree) override;
    virtual void      SlaveBegin(TTree *tree) override;
    virtual void      Init(TTree *tree = 0) override;
    virtual Bool_t    Notify() override;
    virtual Bool_t    Process(Long64_t entry) override;
    virtual void      SlaveTerminate() override;
    virtual void      Terminate() override;
    virtual void      SetBranchStatus() = 0;
    virtual void      InitEvent() {;}
    virtual void      BookHistograms()  = 0;
    virtual void      FillHistograms()  = 0;
    virtual void      ProcessFill();
    virtual void      SetBranches();
    virtual Int_t     GetEntry(Long64_t entry = 0);
    virtual void      SetCutDefaults();
    virtual void      Clear(Option_t *otpion = "") override;
    virtual void      Print(Option_t *option = "") const override;
    void              SetOutputTree(const char* name, const char* title);
    void              ProcessInfo();
    void              PrintSummary();
    void              AddTool(AtlAnalysisTool *tool);
    AtlAnalysisTool*  GetTool(const char* ClassName,
			      const char* ToolName = "", Bool_t force = kFALSE);
    AtlTriggerConf*   LoadTriggerConfig();
    static const char* GetJetMultEnum(UInt_t jetmult);
    static const char* GetJetMultLabel(EJetMult jetmult);
    static const char* GetJetMultLabel(Int_t i);
    static const char* GetLeptonLabel(ELepChannel lep);
    static const char* GetLeptonLabel(Int_t i);
    static const char* GetLeptonEnum(UInt_t lep);
    inline Float_t GetInitialNumberOfEvents() {
        //
        // Get weighted initial number of events from event count
        // histogram. The histogram is created in BookJobInfoHistograms()
        // and the relevant bin is filled in DoBookKeeping().
        //
        return fHistEvents->GetBinContent(3);
    }

    inline TTree* GetTree() { return fTree; }
    inline TTree* GetCurrentTree() const { return fCurrentTree; }
    inline AtlEvent* GetEvent() const { return fEvent; }
    inline Float_t GetPreTagEvtWeight() const { return fEvent->GetPreTagEvtWeight(); }
    inline Float_t GetTagEvtWeight() const { return fEvent->GetTagEvtWeight(); }
    inline TFile* GetOutputFile() { return fOutputFile; }
    inline virtual Bool_t ProcessCut() { return kTRUE; }
    inline virtual Bool_t ProcessPreCut() { return kTRUE; }
    inline Bool_t GetWriteEntryList() const { return fWriteEntryList; }
    inline Bool_t PassedSelection() const { return fPassedSelection; }
    inline void SetWriteEntryList(Bool_t WriteEntryList)
    { fWriteEntryList = WriteEntryList; }
    inline TList* GetListOfTools() const { return fListOfTools; }
    inline Int_t GetProcessedEvents() const { return fProcessedEvents; }
    Int_t GetNProcessedFiles() const { return fNProcessedFiles; }
    inline Bool_t HasOutputTree() const {
	//
	// Is there an A++ tree written to the output .root file ?
	//
	return ( fOutputTreeName->Length() > 0 );
    }
    inline Bool_t IsFirstEvent() const { return fProcessedEvents == 0; }
    inline void SetNProcessNthEventsOnly(Int_t N) { fNProcessNthEventsOnly = N; }
    inline void SetCopyCutflowHistograms(Bool_t doit = kTRUE) { fCopyCutflowHistograms = doit; }
    inline void SetPrintEvent(Bool_t PrintEvent) {
	//
	// Switch for printing every event
	// (useful for testing/debugging)
	//
	fPrintEvent = PrintEvent;
    }
    inline void SetPrintObjectTable(Bool_t PrintObjectTable) {
	// Switch for printing the Root's object table
	// (useful for tracking memory leaks)
	fPrintObjectTable = PrintObjectTable;
    }
    inline void SetPassedSelection(Bool_t result) { fPassedSelection = result; }
   
    void SetInputMode(EIOMode inputMode);
    void SetEvtReader(AtlEvtReaderBase * reader, Bool_t use_job_info=kFALSE);

  protected:
    void BookJobInfoHistograms();
    void SetSumw2(TDirectory *dir);
    void ChangeOutputFile();
    void DoBookkeeping(TFile *InputFile);

    ClassDefOverride(AtlSelector,0) // ATLAS analysis selector
};
#endif


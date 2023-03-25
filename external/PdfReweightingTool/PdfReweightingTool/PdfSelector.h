//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Copyright: 2013 (C) Soeren Stamm
//
#ifndef PdfSelector_h
#define PdfSelector_h
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
#ifndef ROOT_TSystem
#include <TSystem.h>
#endif
#ifndef ROOT_TH1
#include <TH1.h>
#endif

#include <vector>

class TString;
class TROOT;
class PdfReweightingTool;

class PdfSelector : public TSelector {
  
 protected:
  TTree      *fTree;                  // Pointer to the currently analysed tree or chain
  TTree      *fCurrentTree;           // Currently analysed tree
  Bool_t      fIsChain;               // Input is chain or tree ?
  Int_t       fProcessedEvents;       // No. of processed events
  TStopwatch  fStopwatch;             // Stopwatch used for process monitoring
  Long64_t    fEntry;                 // Global entry number of the current event in input chain
  ProcInfo_t *fProcInfo;              // Current process information
  MemInfo_t  *fMemInfo;               // Current memory usage information
  Bool_t      fPrintObjectTable;      // Print Root's object table for every event (useful for tracking memory leaks)
  TFile      *fOutputFile;            // Output file
  TString    *fOutputFilename;        // Name of the output file
  TString    *fPdfName;               // Name of the (single) pdf set
  Int_t       fDSID;                  // DSID of the samplei
  PdfReweightingTool *fPdfTool;

  // Histograms for reweighting
  TH1F *fHistPdf;

  
  // D3PD content
  UInt_t v_RunNr;      // Run number
  UInt_t v_EvtNr;      // Event number
  Float_t v_GenWeight; // MC generator weight
  std::vector<double>  *v_mcevt_pdf1;
  std::vector<double>  *v_mcevt_pdf2;
  std::vector<int>     *v_mcevt_pdf_id1;
  std::vector<int>     *v_mcevt_pdf_id2;
  std::vector<double>  *v_mcevt_pdf_scale;
  std::vector<double>  *v_mcevt_pdf_x1;
  std::vector<double>  *v_mcevt_pdf_x2;
  
  // List of branches
  TBranch        *b_RunNumber;       //!
  TBranch        *b_EventNumber;     //!
  TBranch        *b_GeneratorWeight; //!
  TBranch        *b_mcevt_pdf1;      //!
  TBranch        *b_mcevt_pdf2;      //!
  TBranch        *b_mcevt_pdf_id1;   //!
  TBranch        *b_mcevt_pdf_id2;   //!
  TBranch        *b_mcevt_pdf_scale; //!
  TBranch        *b_mcevt_pdf_x1;    //!
  TBranch        *b_mcevt_pdf_x2;    //!

  public:
  PdfSelector(Int_t DSID);
  virtual ~PdfSelector();
  
  virtual Int_t     Version() const { return 2; }
  virtual void      Begin(TTree *tree);
  virtual void      SlaveBegin(TTree *tree);
  virtual void      Init(TTree *tree = 0);
  virtual Bool_t    Notify();
  virtual Bool_t    Clear() { return kTRUE; }
  virtual Bool_t    Process(Long64_t entry);
  virtual void      SlaveTerminate();
  virtual void      Terminate();
  virtual Int_t     GetEntry(Long64_t entry = 0);
  virtual void      Print(Option_t *option = "") const;

 private:
  virtual void SetBranches(TTree *tree);
  void SetupBranch(TTree *tree,
		   const char* branchname,
		   void* value, TBranch** branch);
  void ProcessInfo();
  void PrintSummary();
  void ProcessEvent();
  void InitPdfTool();
  
 public:
  inline TTree* GetTree() { return fTree; }
  inline TTree* GetCurrentTree() const { return fCurrentTree; }
  inline TFile* GetOutputFile() { return fOutputFile; }
  inline Int_t GetProcessedEvents() const { return fProcessedEvents; }
  inline Bool_t IsFirstEvent() const { return fProcessedEvents == 0; }
  inline void SetPrintObjectTable(Bool_t PrintObjectTable) {
    // Switch for printing the Root's object table
    // (useful for tracking memory leaks)
    fPrintObjectTable = PrintObjectTable;
  }
  
 protected:
  void ChangeOutputFile();

  ClassDef(PdfSelector,0) // PdfReweightingTool analysis selector
};
#endif

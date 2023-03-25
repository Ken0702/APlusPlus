//____________________________________________________________________
//
// Cross-section class
// 
// This class is used to calculate the cross section and its error
//
// Author: Umberto Prosperi Porta <mailto: umberto.prosperi@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2011 (C) Umberto Prosperi Porta
//
#ifndef HEP_HepCrossSectionTask
#define HEP_HepCrossSectionTask
#ifndef ROOT_TTask
#include <TTask.h>
#endif
#include <iostream>
#include <fstream>

class HepDataMCPlot;
class HepDataMCSample;
class TString;
class TFile;
class TList;
class TH1F;

class HepCrossSectionTask : public TTask {

  public:
    
    TString         fInputFilename;  // Name of input file containing plots (incouding path)
    TString         fOutputFilename; // Name of output file containing histograms (incouding path)

    TString         fCutFlowName;    // Name of cut-flow plot (including path)
    TString         fSignalName;     // Name of signal
    
  private:
    
    HepDataMCPlot   *fCutFlowPlot;   // Cut-flow plot used for acceptance determination
    TFile           *f_input;        // Input file;
    TFile           *f_output;       // Output file;
    HepDataMCPlot   *h_cutflow;      // Histogram containing cut-flow plot
    HepDataMCSample *signal_sample;  // Histogram containing only the signal from cut-flow polt
    TList           *signal;         //  

    Int_t          Nbins;               // Number of bins in cut-flow plot
    Float_t        Ndata;               // Number of selected DATA-events
    Float_t        NdataErr;            // Number of selected DATA-events error
    Float_t        Nevents_sig;         // Number of selected  MC-events of signal
    Float_t        Nevents_sigErr;      // Number of selected  MC-events of signal error
    Float_t        Nevents_sig_gen;     // Number of generated MC-events of signal
    Float_t        Nevents;             // Number of selected  MC-DATA-events
    Float_t        NeventsErr;          // Number of selected  MC-DATA-events error
    Float_t        Nevents_bkg;         // Number of selected  MC-events of background
    Float_t        Nevents_sig_data;    // Number of selected events of signal
    Float_t        Nevents_sig_dataErr;           // Number of selected events of signal error
    Float_t        Acceptance;                    // Acceptance
    Float_t        AcceptanceErr;                 // Acceptance error
    Float_t        AcceptanceStatErr;             // Acceptance statistical error
    Float_t        AcceptanceSystErr;             // Acceptance systematic error
    Float_t        Luminosity;                    // Luminosity
    Float_t        LuminosityErr;                 // Luminosity error
    Float_t        Cross_Section;                 // Cross section
    Float_t        Cross_SectionErr;              // Cross section statistical error
    Float_t        Cross_SectionErr_Syst_up;      // Cross section systematic error (up)
    Float_t        Cross_SectionErr_Syst_down;    // Cross section systematic error (down)

    TH1F           *fHistXsecErrors; 
     

	
public:
    HepCrossSectionTask(const char* name, const char* title);
    virtual ~HepCrossSectionTask();

    virtual void Exec(Option_t *option);
    virtual void Print();
    void SetInputFilename(const char* InputFilename);
    void SetOutputFilename(const char* OutputFilename);
    void SetCutFlowName(const char* CutFlowName);
    void SetSignalName(const char* SignalName);
  

    

private:
    void AcceptanceCalc();
    void XsecCalc();



    
    ClassDef(HepCrossSectionTask,0) // Cross-section calculation
};
#endif


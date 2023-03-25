//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2011 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlCutFlowTool
#define ATLAS_AtlCutFlowTool
#ifndef ATLAS_AtlAnalysisTool
#include <AtlAnalysisTool.h>
#endif
#ifndef ATLAS_AtlSelector
#include <AtlSelector.h>
#endif
#ifndef ROOT_TH1F
#include <TH1F.h>
#endif
#ifndef ROOT_TNamed
#include <TNamed.h>
#endif
#include <fstream>

class TDirectory;
class TString;
class TObjArray;

class AtlCutFlowStreamer : public TNamed {
    //
    // Small helper class for output streamers
    //
  public:
    std::ofstream fRunEvtStreamer; // Run/evt output streamer
    explicit AtlCutFlowStreamer(const char* name) : TNamed(name, "") {;}
    virtual ~AtlCutFlowStreamer() {;}
};

class AtlCutFlowTool : public AtlAnalysisTool {

  private:
    TH1F      *fHistCutFlowWeighted;   // Cut-flow histogram filled with weights
    TH1F      *fHistCutFlowUnweighted; // Cut-flow histogram filled w/o weights
    TObjArray *fOutputStreamers;       // Run/evt output streamers


  protected:
    Bool_t     fPassedSelection;       // Bool registrating whether the event passed the selection or not

  public:
    Int_t   fNBins;              // Number of bins used for the booking of the cut-flow histograms (default=1).
    Bool_t  fLabelsDeflate;      // Switch for deflating the number of bins of the cut-flow histograms at the end of the analysis job (default=true)
    TString fRunEvtOutputList;   // Comma-separated list of those cut-variables for which run and event numbers should be written into individual text files
    TString fRunEvtOutputFormat; // Format string of the run/evt output (default="%d\t%d")
    TString fBinLabels;          // Bin labels of cutflow histograms (optional)

  public:
    AtlCutFlowTool(const char* name, const char* title);
    virtual ~AtlCutFlowTool();
    virtual void SetBranchStatus() {;}
    virtual void BookHistograms();
    virtual void FillHistograms() {;}
    virtual void Clear();
    virtual void Terminate();
    virtual void Print() const;
    virtual void Fill(const char* label, Float_t weight);
    inline  void Fill(const char* label) {
	//
	// Fill the event weight of the current event into bin labelled
	// with given label of the weighted histogram. The un-weighted
	// histogram will be filled with 1.0.
	//
	// In case a bin with this label does not exist, a new bin will be
	// added. Note that Root doubles the number of bins in this case.
	// To prevent empty bins the cut-flow histograms will be deflated
	// in the Terminate() routine.
	//
	Fill(label, GetTagEvtWeight());
    }
    TH1F *GetCutFlowWeighted()   { return fHistCutFlowWeighted; }
    TH1F *GetCutFlowUnweighted() { return fHistCutFlowUnweighted; }

    virtual inline Bool_t PassedSelection() { return fPassedSelection; }

  private:
    void OpenStreamers();
    void CloseStreamers();


    ClassDef(AtlCutFlowTool,0) // Analysis cut-flow tool
};
#endif

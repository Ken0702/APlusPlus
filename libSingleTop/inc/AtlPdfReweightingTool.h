//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2012 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlPdfReweightingTool
#define ATLAS_AtlPdfReweightingTool
#ifndef ATLAS_AtlAnalysisTool
#include <AtlAnalysisTool.h>
#endif
#include "PdfReweightingTool/PdfReweightingTool.h"
#ifndef ROOT_TList
#include <TList.h>
#endif

class TH1F;
class TH2F;
class TObjString;
class TFile;
class TSystem;
class TROOT;

class AtlPdfReweightingTool : public AtlAnalysisTool {

  public:
    enum ESystMode { kUp, kDown, kNominal};
    ESystMode  fSystMode;     // Systematics mode (up/down/nominal)
    TList     *fListObsNames; // List of observable histogram names
    Int_t      fDSID;         // DSID of the sample (needed for norm. factors)
    
  private:
    PdfReweightingTool  *fPdfReweightingTool;  // Pdf tool object
    TList  *fListObsHistsOrig; // List of original observable histogram pointers
    TList  *fListObsHistsTmp;  // List of interal observable histogram pointers
    Bool_t  fFirstEvent;       // First event ?
    
  public:
    AtlPdfReweightingTool(const char* name, const char* title);
    virtual ~AtlPdfReweightingTool();
    virtual void   Init();
    virtual void   BookHistograms();
    virtual Bool_t AnalyzeEvent();
    virtual void   Print() const;
    virtual void   SetBranchStatus();
    virtual void   FillHistograms() {;}
    virtual void   Terminate();
    void AddObservable(const char* HistName);

  private:
    void FirstEvent();
    void FillObservables();
    void AddObs1D(TH1F *h_orig);
    void AddObs2D(TH2F *h_orig);
    TList* FIndObjects(TList *list, const TRegexp& pat);
    
    ClassDef(AtlPdfReweightingTool,0) // Pdf reweighting tool
};
#endif


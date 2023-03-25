//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlCtrlPlots
#define ATLAS_AtlCtrlPlots
#ifndef ATLAS_AtlAnalysisTool
#include <AtlAnalysisTool.h>
#endif
#ifndef ROOT_TObject
#include <TObject.h>
#endif
#ifndef ROOT_TList
#include <TList.h>
#endif
#ifndef ROOT_TDirectory
#include <TDirectory.h>
#endif
#ifndef ROOT_TFile
#include <TFile.h>
#endif
#ifndef ROOT_TH1F
#include <TH1F.h>
#endif
#ifndef ROOT_TH2F
#include <TH2F.h>
#endif
#ifndef ROOT_TProfile
#include <TProfile.h>
#endif
#ifndef ROOT_THashTable
#include <THashTable.h>
#endif

class AtlEvent;
class TTree;
class TString;
class TROOT;

class AtlCtrlPlots : public AtlAnalysisTool {

private:
    THashTable *fHistogramsNoEvtSel; // List of all histograms filled 
                                     // before event selection
    THashTable *fHistogramsEvtSel;   // List of all histograms filled
                                     // after event selection
    TDirectory *fDir_noevtsel;       // Directory for histograms
                                     // without selection
    TDirectory *fDir_evtsel;         // Directory for histograms
                                     // with selection
    TDirectory *fDir_MCvsREC;        // Directory with MC-vs-REC quantities
    
public:
    AtlCtrlPlots(const char* name, const char* title);
    virtual ~AtlCtrlPlots();
    virtual void SetBranchStatus();
    virtual void BookHistograms();
    virtual void FillHistogramsNoEvtSel()
    { FillHistograms(kFALSE); }
    virtual void FillHistograms()
    { FillHistograms(kTRUE); }
    void FillHistograms(Bool_t EvtSelection);
    void BookEnergySumsHistograms();
    void BookJetHistograms();
    void BookElectronHistograms();
    void BookMuonHistograms();
    void BookTauHistograms();
    void BookPhotonHistograms();
    void BookAtlFastHistograms();
    void BookTriggerHistograms(Bool_t EvtSelection);
    void FillEnergySumsHistograms(Bool_t EvtSelection = kFALSE);
    void FillJetHistograms(Bool_t EvtSelection = kFALSE);
    void FillElectronHistograms(Bool_t EvtSelection = kFALSE);
    void FillMuonHistograms(Bool_t EvtSelection = kFALSE);
    void FillTauHistograms(Bool_t EvtSelection = kFALSE);
    void FillPhotonHistograms(Bool_t EvtSelection = kFALSE);
    void FillAtlFastHistograms(Bool_t EvtSelection = kFALSE);
    void FillTriggerHistograms(Bool_t EvtSelection = kFALSE);

  private:
    void CopyFolder(TDirectory *dir_src, TDirectory *dir_dest);
    inline TObject* GetHistogram(const char* name, Bool_t evtsel) {
	// Get histogram pointer by histogram name
	return ( evtsel ) ? fHistogramsEvtSel->FindObject(name)
	    : fHistogramsNoEvtSel->FindObject(name);
    }
    
    ClassDef(AtlCtrlPlots,0) // ATLAS control plots
};
#endif


//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2013 (C) Soeren Stamm
//
#ifndef SINGLETOP_AtlSgTop_tChannelMemDiscr
#define SINGLETOP_AtlSgTop_tChannelMemDiscr
#ifndef ROOT_TH2
#include "TH2.h"
#endif
#ifndef ATLAS_AtlSelector
#include <AtlSelector.h>
#endif
#ifndef ATLAS_AtlObjectsToolD3PDSgTop
#include <AtlObjectsToolD3PDSgTop.h>
#endif
#ifndef ATLAS_AtlObjRecoScaleFactorTool
#include <AtlObjRecoScaleFactorTool.h>
#endif
#ifndef ATLAS_AtlEvtReaderMem
#include <AtlEvtReaderMem.h>
#endif
#ifndef ATLAS_AtlPdfReweightingTool
#include <AtlPdfReweightingTool.h>
#endif

class AtlSgTop_tChannelMemDiscr : public AtlSelector {

  public:
    static const Int_t     fgNBins_tChannel4FSRatio = 20;
    static const Double_t  fgBinEdges_tChannel4FSRatio[fgNBins_tChannel4FSRatio+1];   // lower edges used for t-channel probability rebinning

    static const Int_t     fgNBins_tChannel5FSRatio = 20;
    static const Double_t  fgBinEdges_tChannel5FSRatio[fgNBins_tChannel5FSRatio+1];   // lower edges used for t-channel probability rebinning

    Bool_t fSchannelVeto;

    
  private:
    TList   *fLeptons;   // List of selected leptons
    TList   *fLooseLeptons;   // List of selected loose leptons
    TList   *fElectrons; // List of selected electron objects
    TList   *fMuons;     // List of selected muon objects
    TList   *fJets;      // List of selected jets
    TList   *fBJets;     // List of selected b-jets
    
    // Pointer to tools in event loop
    AtlObjectsToolD3PDSgTop   *fObjTool;
    AtlObjRecoScaleFactorTool *fSFTool;
    AtlPdfReweightingTool     *fPdfTool;

    // 
    // MEM likelihood ratio
    // 
    // a-priori probabilities
    Double_t fP_sChannel2j_ePlus;
    Double_t fP_sChannel2j_muPlus;
    Double_t fP_sChannel2j_eMinus;
    Double_t fP_sChannel2j_muMinus;
    
    Double_t fP_sChannel3j_ePlus;
    Double_t fP_sChannel3j_muPlus;
    Double_t fP_sChannel3j_eMinus;
    Double_t fP_sChannel3j_muMinus;
    
    Double_t fP_tChannel4FS_ePlus;
    Double_t fP_tChannel4FS_muPlus;
    Double_t fP_tChannel4FS_eMinus;
    Double_t fP_tChannel4FS_muMinus;
    
    Double_t fP_Wjj_ePlus;
    Double_t fP_Wjj_muPlus;
    Double_t fP_Wjj_eMinus;
    Double_t fP_Wjj_muMinus;
    
    Double_t fP_Wbb_ePlus;
    Double_t fP_Wbb_muPlus;
    Double_t fP_Wbb_eMinus;
    Double_t fP_Wbb_muMinus;
    
    Double_t fP_Wcj_ePlus;
    Double_t fP_Wcj_muPlus;
    Double_t fP_Wcj_eMinus;
    Double_t fP_Wcj_muMinus;
    
    Double_t fP_ttbarSL_ePlus;
    Double_t fP_ttbarSL_muPlus;
    Double_t fP_ttbarSL_eMinus;
    Double_t fP_ttbarSL_muMinus;
    
    Double_t fP_ttbarDL_ePlus;
    Double_t fP_ttbarDL_muPlus;
    Double_t fP_ttbarDL_eMinus;
    Double_t fP_ttbarDL_muMinus;

    // Likelihood ratio Histogram
    TH1F *fHist_llh_tChannel_ratio_4FS;       // t-channel probability, rebinned
    TH1F *fHist_llh_tChannel_ratio_4FS_even;  // t-channel probability, even evt numbers
    TH1F *fHist_llh_tChannel_ratio_4FS_odd;   // t-channel probability, odd evt numbers
    TH1F *fHist_llh_tChannel_ratio_4FS_raw;   // t-channel probability, original binning
    TH1F *fHist_llh_tChannel_ratio2_4FS;      // t-channel probability, const bin width
    TH1F *fHist_llh_Wjets_ratio_4FS;          // Wjets probability
    TH1F *fHist_llh_Wbb_ratio_4FS;            // Wbb probability
    TH1F *fHist_llh_Wcj_ratio_4FS;            // Wcj probability
    TH1F *fHist_llh_Wjj_ratio_4FS;            // Wjj probability
    TH1F *fHist_llh_ttbar_ratio_4FS;          // ttbar probability
    TH2F *fHist_2D_tChannel_4FS_vs_5FS;       // t-channel 4FS vs t-channel 5FS
    TH2F *fHist_2D_tChannel_4FS_vs_Wbb;       // t-channel 4FS vs Wbb
    TH2F *fHist_2D_tChannel_4FS_vs_Wcj;       // t-channel 4FS vs Wcj
    TH2F *fHist_2D_tChannel_4FS_vs_Wjets;     // t-channel 4FS vs wjets
    TH2F *fHist_2D_tChannel_4FS_vs_ttbar;     // t-channel 4FS vs ttbar
    TH1F *fHist_comb_llh_tChannel_ratio_4FS;  // t-channel probability, rebinned, (veto s-channel events)

    TH1F *fHist_llh_tChannel_ratio_5FS;       // t-channel probability, rebinned
    TH1F *fHist_llh_tChannel_ratio_5FS_even;  // t-channel probability, even evt numbers
    TH1F *fHist_llh_tChannel_ratio_5FS_odd;   // t-channel probability, odd evt numbers
    TH1F *fHist_llh_tChannel_ratio_5FS_raw;   // t-channel probability, original binning
    TH1F *fHist_llh_tChannel_ratio2_5FS;      // t-channel probability, const bin width
    TH1F *fHist_llh_Wjets_ratio_5FS;          // Wjets probability
    TH1F *fHist_llh_ttbar_ratio_5FS;          // ttbar probability

    TH1F *fHist_logllh_sChannel2j;
    TH1F *fHist_logllh_sChannel3j;
    TH1F *fHist_logllh_tChannel4FS;
    TH1F *fHist_logllh_tChannel5FS;
    TH1F *fHist_logllh_ttbarSL;
    TH1F *fHist_logllh_ttbarDL;
    TH1F *fHist_logllh_Wjj;
    TH1F *fHist_logllh_Wcj;
    TH1F *fHist_logllh_Wbb;

  public:
    AtlSgTop_tChannelMemDiscr(const char* OutputFilename);
    virtual ~AtlSgTop_tChannelMemDiscr();
    
    virtual void   SetBranchStatus();
    virtual void   Clear( Option_t *option = "");
    virtual void   Begin(TTree *tree);
    virtual void   BookHistograms();
    virtual void   FillHistograms();
    virtual Bool_t ProcessPreCut();
    virtual Bool_t ProcessCut();
    virtual void   SetCutDefaults();
    virtual void   Print(Option_t *option) const;
    virtual void   Terminate();

  protected:
    void InitEvent();
	
    AtlEvtReaderMem* GetEvtReader() { return (AtlEvtReaderMem*) fEvtReader; }
    
    ClassDef(AtlSgTop_tChannelMemDiscr,0) // Single-top 8 TeV t-Channel MemDiscriminant analysis
};
#endif

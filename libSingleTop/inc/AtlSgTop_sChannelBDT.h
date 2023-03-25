//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2013 (C) Soeren Stamm
//
#ifndef SINGLETOP_AtlSgTop_sChannelBDT
#define SINGLETOP_AtlSgTop_sChannelBDT
#ifndef ATLAS_AtlSelector
#include <AtlSelector.h>
#endif
#ifndef ATLAS_AtlObjectsToolD3PDSgTop
#include <AtlObjectsToolD3PDSgTop.h>
#endif
#ifndef ATLAS_AtlObjRecoScaleFactorTool
#include <AtlObjRecoScaleFactorTool.h>
#endif
#ifndef ATLAS_AtlEvtReaderMemBDT
#include <AtlEvtReaderMemBDT.h>
#endif

#include "TMVA/Reader.h"

class AtlSgTop_sChannelBDT : public AtlSelector {
    
  private:
    TList   *fLeptons;   // List of selected leptons
    TList   *fElectrons; // List of selected electron objects
    TList   *fMuons;     // List of selected muon objects
    TList   *fJets;      // List of selected jets
    TList   *fBJets;     // List of selected b-jets

    // Pointer to tools in event loop
    AtlObjectsToolD3PDSgTop   *fObjTool;
    AtlObjRecoScaleFactorTool *fSFTool;

    // TMVA
    /* TMVA::Reader* fReader; */
    TMVA::Reader* fReaderEl; // electron case
    TMVA::Reader* fReaderMu; // muon case

    // TMVA variables
	// Kinematic variables
	Float_t fBDT_MtW;
	Float_t fBDT_DeltaPhiTopBJet2_BJet1;
	Float_t fBDT_DeltaPhiTopBJet1_BJet2;
	Float_t fBDT_SumEtLep_MET;
	Float_t fBDT_MET;
	Float_t fBDT_SumEtLep_MET_BJets;
	Float_t fBDT_Pt_Lep;
    Float_t fBDT_SumPt;
    Float_t fBDT_DeltaEtaLep_BJet1;
    Float_t fBDT_DeltaPhiBJet1_BJet2;
    Float_t fBDT_DeltaEtaLep_BJet2;
    Float_t fBDT_CosLepW_WTopBJet1;
    Float_t fBDT_DeltaPhiLep_MET;
    Float_t fBDT_DeltaEtaTopBJet2_BJet1;
    Float_t fBDT_CosLepTopBJet2_TopBJet2CMS;
    Float_t fBDT_CosLepTopBJet1_TopBJet1CMS;
	Float_t fBDT_MLep_BJet2;
	Float_t fBDT_CosMET_BJet2;
	Float_t fBDT_SumPtBJet1_BJet2;
	Float_t fBDT_DeltaEtaNu_BJetNonTop;
	
	// MEM likelihood ratios
    Float_t fBDTsChannelRatio;
    Float_t fBDTtChannelRatio;
    Float_t fBDTttbarSLRatio;
    Float_t fBDTWjjRatio;
    Float_t fBDTWbbRatio;

    // TMVA Histogram
    TH1F *fHist_BDT;      // BDT output
    TH1F *fHist_BDT_even; // BDT output, even event numbers
    TH1F *fHist_BDT_odd;  // BDT output, odd event numbers
    TH1F *fHist_BDT_rebinned;      // BDT output, rebinned
    TH1F *fHist_BDT_rebinned_even; // BDT output, rebinned, even event numbers
    TH1F *fHist_BDT_rebinned_odd;  // BDT output, rebinned, odd event numbers
    TH1F *fHist_BDT_manyBins; // BDT output, many bins, needed for rebinning
    
    Int_t     fNBins_BDT;     // number of bins for rebinned BDT response
    Double_t *fBinEdges_BDT;  // edges used for BDT response rebinning
	
    // Likelihood ratio Histograms
    TH1F *fHist_llh_ttbarSL_ratio;
    
  public:
    AtlSgTop_sChannelBDT(const char* OutputFilename);
    virtual ~AtlSgTop_sChannelBDT();
    
    virtual void SetBranchStatus();
    virtual void Clear( Option_t *option = "");
    virtual void Begin(TTree *tree);
    virtual void BookHistograms();
    virtual void FillHistograms();
    virtual Bool_t ProcessPreCut();
    virtual Bool_t ProcessCut();
    virtual void SetCutDefaults();
    virtual void Print(Option_t *option) const;
    virtual void Terminate();

  protected:
    void InitEvent();
    void BookHistogramsLikelihoodRatios();
    void BookHistogramsBDT();
    void FillHistogramsLikelihoodRatios();
    void ComputeLikelihoodRatios();

    AtlEvtReaderMemBDT* GetEvtReader() { return (AtlEvtReaderMemBDT*) fEvtReader; }
    
    ClassDef(AtlSgTop_sChannelBDT,0) // Single-top 8 TeV s-Channel BDT analysis
};
#endif

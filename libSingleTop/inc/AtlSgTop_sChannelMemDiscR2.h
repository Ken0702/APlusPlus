// 
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2013 (C) Soeren Stamm
// 
#ifndef SINGLETOP_AtlSgTop_sChannelMemDiscR2
#define SINGLETOP_AtlSgTop_sChannelMemDiscR2

#include <AtlSelector.h>           // for AtlSelector
#include <Rtypes.h>                // for ClassDef
#include <RtypesCore.h>            // for Double_t, Float_t, Int_t, Bool_t
#include "AtlBTag.h"               // for AtlBTag, AtlBTag::ETagger
#include "AtlEvtReaderBase.h"      // for AtlEvtReaderBase
#include "AtlEvtReaderD3PDBase.h"  // for AtlEvtReaderD3PDBase
class AtlCutFlowTool;
class AtlHistogramTool;
class AtlObjRecoScaleFactorTool;
class AtlObjectsToolD3PDSgTop;
class AtlPdfReweightingTool;
class HepParticle;
class TBuffer;
class TClass;
class TH1F;
class TH2F;
class TList;
class TMemberInspector;
class TTree;

class AtlSgTop_sChannelMemDiscR2 : public AtlSelector {

  public:
    // S.K. 13 TeV
    static const Int_t     fgNBins_sChannelRatio = 23; // number of bins for rebinned s-channel probability
    /* static const Int_t     fgNBins_sChannelRatio = 25; // number of bins for rebinned s-channel probability */
    static const Double_t  fgBinEdges_schannelRatio[fgNBins_sChannelRatio+1];   // lower edges used for s-channel probability rebinning
    static const Int_t     fgNBins_raw_eplus   = 19;
    static const Int_t     fgNBins_raw_eminus  = 19;
    static const Int_t     fgNBins_raw_muplus  = 19;
    static const Int_t     fgNBins_raw_muminus = 19;
    static const Double_t  fgBinEdges_raw_eminus[fgNBins_raw_eminus+1];
    static const Double_t  fgBinEdges_raw_eplus[fgNBins_raw_eplus+1];
    static const Double_t  fgBinEdges_raw_muplus[fgNBins_raw_muplus+1];
    static const Double_t  fgBinEdges_raw_muminus[fgNBins_raw_muminus+1];
	
    static const Int_t     fgNBins_raw1 = 19;
    static const Int_t     fgNBins_raw2 = 20;
    static const Int_t     fgNBins_raw3 = 23;
    static const Int_t     fgNBins_raw4 = 26;
    static const Int_t     fgNBins_raw5 = 28;
    static const Double_t  fgBinEdges_raw1[fgNBins_raw1+1];
    static const Double_t  fgBinEdges_raw2[fgNBins_raw2+1];
    static const Double_t  fgBinEdges_raw3[fgNBins_raw3+1];
    static const Double_t  fgBinEdges_raw4[fgNBins_raw4+1];
    static const Double_t  fgBinEdges_raw5[fgNBins_raw5+1];
    
    Float_t fMET_min;              // Minimum missing Et (GeV)
    Float_t fMET_max;              // Maximum missing Et (GeV)
    Float_t fMtW_minPre;           // Transverse W mass minimum (GeV), event pre-selection
    Float_t fMtW_maxPre;           // Transverse W mass maximum (GeV), event pre-selection
    Float_t fMtW_min;              // Transverse W mass minimum (GeV), final event selection
    Float_t fMtW_max;              // Transverse W mass maximum (GeV), final event selection
    Float_t fPt_Jet_min;           // Additional minimum Pt cut for all jets (GeV)
    Float_t fPt_Jet_max;           // Additional maximum Pt cut for all jets (GeV
    Float_t fPt_LeadingJet_min;    // Additional minimum Pt cut for the leading jet (GeV)
    Float_t fPt_LeadingJet_max;    // Additional maximum Pt cut for the leading jet (GeV)
    Int_t   fNBTags_min;           // Minimum no. of b-tagged jets
    Int_t   fNBTags_max;           // Maximum no. of b-tagged jets
    Bool_t  fAdditionalJetVeto;    // Remove events with additional jets outside the given pt-eta range
    Bool_t  fApplyDileptonVeto;    // Apply dilepton veto?
    Bool_t  fLeadingJetIsBTagged;  // Require the leading jet to be b-tagged
    AtlBTag::ETagger fVetoBTagger; // B-tagger used for b-tag veto (ctrl region)
    Float_t fVetoBTagWeight_min;   // Minimum weight for b-tag veto (ctrl region)
    Bool_t  fEvtWriterReduceOutput;// Parameter to give to EvtWriter
    Bool_t  fEvtWriterCloneOnly;   // Parameter to give to EvtWriter
    Bool_t  fProcessNegativeLlh;   // Fill event in histogram even if >=1 llh is negative

  private:

    HepParticle *fLepton;       // The leading lepton
    TList   *fLeptons;   // List of selected leptons
    TList   *fLooseLeptons;   // List of selected loose leptons
    TList   *fElectrons; // List of selected electron objects
    TList   *fMuons;     // List of selected muon objects
    TList   *fJets;      // List of selected jets
    TList   *fBJets;     // List of selected b-jets
    
    // QCD Tool
    Bool_t    fQCDMMToolIsOn;  // Flag for QCD Matrix Method tasks

    Int_t     fNNegativeLlh;   // To count number of negative llh

    // Pointer to tools in event loop
    AtlObjectsToolD3PDSgTop   *fObjTool;
    AtlObjRecoScaleFactorTool *fSFTool;
    AtlCutFlowTool            *fCutFlowTool;
    AtlPdfReweightingTool     *fPdfTool;
    AtlHistogramTool          *fHistsLlh;
    AtlHistogramTool          *fHistsLlhMu;
    AtlHistogramTool          *fHistsLlhE;

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
    TH1F *fHist_llh_sChannel_ratio;                   // s-channel probability
    TH1F *fHist_llh_sChannel_ratio_lplus;             // s-channel probability, l+
    TH1F *fHist_llh_sChannel_ratio_lminus;            // s-channel probability, l-
    TH1F *fHist_llh_sChannel_ratio_bkg;               // s-channel probability, bkg region only (low scores)
    TH1F *fHist_llh_sChannel_ratio_bin1Spread;        // s-channel probability, mapped monotonously onto a function spreading the first bins
	
    TH1F *fHist_llh_sChannel_ratio_bins2N;            // s-channel probability, bins 2 to N_all
    TH1F *fHist_llh_sChannel_ratio_bins2N_even;       // s-channel probability, bins 2 to N_all, even evt numbers
    TH1F *fHist_llh_sChannel_ratio_bins2N_odd;        // s-channel probability, bins 2 to N_all, odd evt numbers
    TH1F *fHist_llh_sChannel_ratio_bkg_bins2N;            // s-channel probability, bins 2 to N_all, bkg region only (low scores)
    TH1F *fHist_llh_sChannel_ratio_bkg_bins2N_even;       // s-channel probability, bins 2 to N_all, even evt numbers, bkg region only (low scores)
    TH1F *fHist_llh_sChannel_ratio_bkg_bins2N_odd;        // s-channel probability, bins 2 to N_all, odd evt numbers, bkg region only (low scores)
	
    TH1F *fHist_llh_sChannel_ratio_bins3N;            // s-channel probability, bins 3 to N_all
    TH1F *fHist_llh_sChannel_ratio_bins3N_even;       // s-channel probability, bins 3 to N_all, even evt numbers
    TH1F *fHist_llh_sChannel_ratio_bins3N_odd;        // s-channel probability, bins 3 to N_all, odd evt numbers
    TH1F *fHist_llh_sChannel_ratio_bkg_bins3N;            // s-channel probability, bins 3 to N_all, bkg region only (low scores)
    TH1F *fHist_llh_sChannel_ratio_bkg_bins3N_even;       // s-channel probability, bins 3 to N_all, even evt numbers, bkg region only (low scores)
    TH1F *fHist_llh_sChannel_ratio_bkg_bins3N_odd;        // s-channel probability, bins 3 to N_all, odd evt numbers, bkg region only (low scores)
	
    TH1F *fHist_llh_sChannel_ratio_bins4N;            // s-channel probability, bins 4 to N_all
    TH1F *fHist_llh_sChannel_ratio_bins4N_even;       // s-channel probability, bins 4 to N_all, even evt numbers
    TH1F *fHist_llh_sChannel_ratio_bins4N_odd;        // s-channel probability, bins 4 to N_all, odd evt numbers
    TH1F *fHist_llh_sChannel_ratio_bkg_bins4N;            // s-channel probability, bins 4 to N_all, bkg region only (low scores)
    TH1F *fHist_llh_sChannel_ratio_bkg_bins4N_even;       // s-channel probability, bins 4 to N_all, even evt numbers, bkg region only (low scores)
    TH1F *fHist_llh_sChannel_ratio_bkg_bins4N_odd;        // s-channel probability, bins 4 to N_all, odd evt numbers, bkg region only (low scores)

    TH2F *fHist_2D_sChannel_vs_Wjets;
    TH2F *fHist_2D_sChannel_vs_ttbar;
    TH2F *fHist_2D_sChannel_vs_SumLLH;
    
    TH1F *fHist_llh_sChannel_ratio_even;          // s-channel probability, even evt numbers
    TH1F *fHist_llh_sChannel_ratio_odd;           // s-channel probability, odd evt numbers
    TH1F *fHist_llh_sChannel_ratio_raw;           // s-channel probability, original binning
    TH1F *fHist_llh_sChannel_ratio_raw_eplus;
    TH1F *fHist_llh_sChannel_ratio_raw_eminus;
    TH1F *fHist_llh_sChannel_ratio_raw_muplus;
    TH1F *fHist_llh_sChannel_ratio_raw_muminus;
    TH1F *fHist_llh_sChannel_ratio_raw_eplus_rebin;
    TH1F *fHist_llh_sChannel_ratio_raw_eminus_rebin;
    TH1F *fHist_llh_sChannel_ratio_raw_muplus_rebin;
    TH1F *fHist_llh_sChannel_ratio_raw_muminus_rebin;

    TH1F *fHist_llh_sChannel_ratio_raw_eplus_rebin_odd;
    TH1F *fHist_llh_sChannel_ratio_raw_eminus_rebin_odd;
    TH1F *fHist_llh_sChannel_ratio_raw_muplus_rebin_odd;
    TH1F *fHist_llh_sChannel_ratio_raw_muminus_rebin_odd;
    TH1F *fHist_llh_sChannel_ratio_raw_eplus_rebin_even;
    TH1F *fHist_llh_sChannel_ratio_raw_eminus_rebin_even;
    TH1F *fHist_llh_sChannel_ratio_raw_muplus_rebin_even;
    TH1F *fHist_llh_sChannel_ratio_raw_muminus_rebin_even;
	
    TH1F *fHist_llh_wjets_ratio;                // W+jets probability
    TH1F *fHist_llh_wjets_ratio_binNSpread;     // W+jets probability, mapped monotonously onto a function spreading the last bins
    TH1F *fHist_llh_wjets_ratio_even;
    TH1F *fHist_llh_wjets_ratio_odd;
    TH1F *fHist_llh_wjets_ratio_raw;
	
    TH1F* fHist_llh_ttbar_ratio;            // ttbar probability
    TH1F* fHist_llh_ttbar_ratio_binNSpread; // ttbar probability, mapped monotonously onto a function spreading the last bins
    TH1F* fHist_llh_ttbar_ratio_raw;            // ttbar probability
	
    // Likelihood ratio Histogram 5 new bin variations
    TH1F *fHist_llh_sChannel_ratio1;
    TH1F *fHist_llh_sChannel_ratio1_even;
    TH1F *fHist_llh_sChannel_ratio1_odd;

    TH1F *fHist_llh_sChannel_ratio2;
    TH1F *fHist_llh_sChannel_ratio2_even;
    TH1F *fHist_llh_sChannel_ratio2_odd;

    TH1F *fHist_llh_sChannel_ratio3;
    TH1F *fHist_llh_sChannel_ratio3_even;
    TH1F *fHist_llh_sChannel_ratio3_odd;

    TH1F *fHist_llh_sChannel_ratio4;
    TH1F *fHist_llh_sChannel_ratio4_even;
    TH1F *fHist_llh_sChannel_ratio4_odd;

    TH1F *fHist_llh_sChannel_ratio5;
    TH1F *fHist_llh_sChannel_ratio5_even;
    TH1F *fHist_llh_sChannel_ratio5_odd;
    
    TH1F *fHist_logllh_sChannel2j;
    TH1F *fHist_logllh_sChannel3j;
    TH1F *fHist_logllh_tChannel4FS;
    TH1F *fHist_logllh_ttbarSL;
    TH1F *fHist_logllh_ttbarSL_ttbarCR;
    TH1F *fHist_logllh_ttbarDL;
    TH1F *fHist_logllh_ttbarDL_wjetsCR;
    TH1F *fHist_logllh_ttbarDL_ttbarCR;
    TH1F *fHist_logllh_Wjj;
    TH1F *fHist_logllh_Wcj;
    TH1F *fHist_logllh_Wcj_wjetsCR;
    TH1F *fHist_logllh_Wbb;

    TH1F *fHist_logllh_sChannel2j_plus;
    TH1F *fHist_logllh_sChannel3j_plus;
    TH1F *fHist_logllh_tChannel4FS_plus;
    TH1F *fHist_logllh_ttbarSL_plus;
    TH1F *fHist_logllh_ttbarDL_plus;
    TH1F *fHist_logllh_Wjj_plus;
    TH1F *fHist_logllh_Wcj_plus;
    TH1F *fHist_logllh_Wbb_plus;

    TH1F *fHist_logllh_sChannel2j_minus;
    TH1F *fHist_logllh_sChannel3j_minus;
    TH1F *fHist_logllh_tChannel4FS_minus;
    TH1F *fHist_logllh_ttbarSL_minus;
    TH1F *fHist_logllh_ttbarDL_minus;
    TH1F *fHist_logllh_Wjj_minus;
    TH1F *fHist_logllh_Wcj_minus;
    TH1F *fHist_logllh_Wbb_minus;

  public:
    explicit AtlSgTop_sChannelMemDiscR2(const char* OutputFilename);
    virtual ~AtlSgTop_sChannelMemDiscR2();
    
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
    void BookHistogramsMemLogLikelihood(AtlHistogramTool *htool, const char *subdir="");
    void FillHistogramsMemLogLikelihood(AtlHistogramTool *htool, Double_t W, const char *subdir="");
    void InitEvent();

    AtlEvtReaderD3PDBase* GetEvtReader() { return dynamic_cast<AtlEvtReaderD3PDBase*>(fEvtReader); }

    ClassDef(AtlSgTop_sChannelMemDiscR2,0) // Single-top 13 TeV s-Channel Mem Selector
};
#endif

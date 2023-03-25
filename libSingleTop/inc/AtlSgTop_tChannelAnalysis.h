//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2013 (C) Soeren Stamm
//
#ifndef SINGLETOP_AtlSgTop_tChannelAnalysis
#define SINGLETOP_AtlSgTop_tChannelAnalysis
#ifndef ATLAS_AtlSelector
#include <AtlSelector.h>
#endif
#ifndef ATLAS_AtlSgTop_tChannelFinder
#include <AtlSgTop_tChannelFinder.h>
#endif
#ifndef ATLAS_AtlObjectsToolD3PDSgTop
#include <AtlObjectsToolD3PDSgTop.h>
#endif
#ifndef ATLAS_AtlObjRecoScaleFactorTool
#include <AtlObjRecoScaleFactorTool.h>
#endif
#ifndef ATLAS_AtlWjetsScaleFactorTool
#include <AtlWjetsScaleFactorTool.h>
#endif
#ifndef ATLAS_AtlCutFlowTool
#include <AtlCutFlowTool.h>
#endif
#ifndef ATLAS_HistogramTool
#include <AtlHistogramTool.h>
#endif

class AtlSgTop_tChannelAnalysis : public AtlSelector {
    
  public:
	float fCounter;
	int Old;
	int New;

    enum ETtbarDecayMode {
	kElEl = 1,
	kMuMu,
	kTauTau,
	kElMu,
	kElTau,
	kMuTau,
	kElQQ,
	kMuQQ,
	kTauQQ,
	kHadronic,
	kUnknown };       
    ETtbarDecayMode fTtbarDecayMode; // Top Quark decay products (MC truth, ttbar only)

    // List of final cuts
    Float_t fMljb_min;         // Minimum invariant mass of the lepton (l) and tagged jet (jb) (GeV), final CUT
    Float_t fMljb_max;         // Minimum invariant mass of the lepton (l) and tagged jet (jb) (GeV), final  CUT
    Float_t fMjlfjb_min;       // Minimum invariant mass of the light quark jet (jlf) and tagged jet (jb) (GeV), final CUT
    Float_t fMtW_max;          // Maximum transverse W mass (GeV), final CUT
    Float_t fHt_min;           // Minimum Ht (GeV), final CUT
    Float_t fPtJetlead_min;    // Minimum transverse momentum Pt of leading jet (GeV), final CUT
    Bool_t fMjlfjbHt_cut;      // 2-dimensional cut on M(jlfjb) vs Ht, final CUT
    
    Float_t fMET;        // Missing Et (GeV)
    Float_t fMET_min;    // Minimum missing Et (GeV)
    Float_t fMtW_minPre; // Transverse W mass minimum (GeV), event pre-selection 
    Float_t fMtW_min;    // Transverse W mass minimum (GeV)
    Float_t fPt_Jet_min;           // Additional minimum Pt cut for all jets (GeV)
    Float_t fPt_Lepton_min;           // Additional minimum Pt cut for all jets (GeV)
    Float_t fPt_ForwardJet_min;    // Additional minimum Pt cut for all jets with  (GeV)
    Float_t fEta_Jet_max;          // Additional maximum eta cut for all jets (GeV)
    Float_t fDeltaEta_Jets_max;    // Maximum eta-difference between leading b-jet and leading light jet    
    Int_t   fNBTags_min;           // Minimum no. of b-tagged jets
    Int_t   fNBTags_max;		   // Maximum no. of b-tagged jets
    Int_t   fNJets; 			   // exactly fNJets number of Jets
    Bool_t  fLeadingJetIsBTagged;  // Require the leading jet to be b-tagged
    Bool_t  fAdditionalJetVeto;    // Remove events with additional jets outside the given pt-eta range
    AtlBTag::ETagger fVetoBTagger; // B-tagger used for b-tag veto
    Float_t fVetoBTagWeight_min;   // Minimum weight for b-tag veto
    ELepChannel fLeptonFlavour;    // Select mu or el channel at event cut level

    Bool_t fMCTruthStudy;          // Flag for filling mc truth info for dilepton study
    Bool_t fApplyDileptonVeto;     // Use dilepton veto? (default = kFALSE)
    Bool_t fApplyBJetVeto;         // Veto BJets? (default = kFALSE)
    Bool_t fApplySchannelVeto;     // Remove s-channel events (2 b-tags)
    Bool_t fQCDVeto;               // Apply QCD veto?

	
  private:
    HepParticle *fLepton;       // The leading lepton

    HepMCParticle *fMCbQuark;
    HepMCParticle *fMCAntibQuark;
    HepMCParticle *fMCLeptonPlus ;
    HepMCParticle *fMCLeptonMinus;
    HepMCParticle *fMCTightLepton;
    HepMCParticle *fMCLooseLepton;

    TList *fLeptons;        // List of selected leptons
    TList *fLooseLeptons;   // List of selected loose leptons
    TList *fElectrons;      // List of selected electron objects
    TList *fMuons;          // List of selected muon objects
    TList *fJets;           // List of selected jets
    TList *fBJets;          // List of selected b-jets
    Float_t  fMtW;          // Transverse W mass (GeV)

    // List of computed variables
    Float_t fHt;               // Scalar sum of the transverse momenta of the jets, the charged leptons and the missing transverse momentum (GeV)
    Float_t fMjlfjb;           // Invariant mass of the tagged (jb) and light quark jet (jlf) (GeV)
    Float_t fMljb;             // Invariant mass of the lepton (l) and tagged jet (jb) (GeV)
    Float_t fDeltaEtaljlf;     // DeltaEta of the charged lepton (l) and the light quark (untagged) jet (jlf)
    Float_t fDeltaPhiljlf;     // DeltaPhi of the charged lepton (l) and the light quark (untagged) jet (jlf)
    Float_t fDeltaRljlf;       // DeltaR of the charged lepton (l) and the light quark (untagged) jet (jlf)
    Float_t fDeltaRljb;        // DeltaR of the charged lepton (l) and the b-tagged jet (jb)
    Float_t fDeltaRjlfjb;      // DeltaR of the light quark (untagged) jet (jlf)  and b-tagged jet (jb)
    Float_t fCosThetaljlf;     // Cosine of  theta between the charged lepton (l) and the light quark (untagged) jet (jlf)
    Float_t fEtajlf;           // Pseudorapidity of the light quark (untagged) jet (jlf)
    Float_t fEtajlfQl;         // Product of the charge of the lepton (l) and the pseudorapidity of the light quark (untagged) jet (lj)
    
    // Pointer to tools in event loop
    AtlObjectsToolD3PDSgTop   *fObjTool;
    AtlObjRecoScaleFactorTool *fSFTool;
    AtlWjetsScaleFactorTool   *fWjetsSFTool;
    AtlCutFlowTool            *fCutFlowTool;
    AtlHistogramTool          *fHistsPretag;
    AtlHistogramTool          *fHistsBTag;
    AtlHistogramTool          *fHistsBTagElectron;
    AtlHistogramTool          *fHistsBTagMuon;
    //AtlHistogramTool          *fHistsBDT;
    //AtlHistogramTool          *fHistsBDTElectron;
    //AtlHistogramTool          *fHistsBDTMuon;
    // AtlSgTop_tChannelFinder *fSgTop_tFinderTool;

    // Histograms (pretag)

    // BEYS 1-D histograms
    TH1F *fHist_Pretag_Ht;
    // BEYE 1-D histograms
    
    TH1F *fHist_Pretag_met;
    TH1F *fHist_Pretag_MtW;
    TH1F *fHist_Pretag_LeadingJet_pt;
    TH1F *fHist_Pretag_LeadingJet_eta;
    TH1F *fHist_Pretag_LeadingJet_phi;
    TH1F *fHist_Pretag_2ndLeadingJet_pt;
    TH1F *fHist_Pretag_2ndLeadingJet_eta;
    TH1F *fHist_Pretag_2ndLeadingJet_phi;
    TH1F *fHist_Pretag_Lepton_pt;
    TH1F *fHist_Pretag_Lepton_eta;
    TH1F *fHist_Pretag_Lepton_phi;

    TH2F *fHist_Pretag_QCDVeto;

    // Histograms (tag)

    // BEYS 1-D histograms
    TH1F *fHist_Tag_UwHt; // Unweighted histrogram in order to count the unweighted number of events
    TH1F *fHist_Tag_Ht;
    TH1F *fHist_Tag_Etajlf;
    TH1F *fHist_Tag_EtajlfQl;
    TH1F *fHist_Tag_Mjlfjb;
    TH1F *fHist_Tag_Mljb;
    TH1F *fHist_Tag_DeltaEtaljlf;
    TH1F *fHist_Tag_DeltaPhiljlf;
    TH1F *fHist_Tag_DeltaRljlf;
    TH1F *fHist_Tag_DeltaRljb;
    TH1F *fHist_Tag_DeltaRjlfjb;
    TH1F *fHist_Tag_CosThetaljlf;
    // BEYE 1-D histograms

    
    TH1F *fHist_Tag_met;
    TH1F *fHist_Tag_MtW;
    TH1F *fHist_Tag_PtW;
    TH1F *fHist_Tag_LeadingJet_pt;
    TH1F *fHist_Tag_LeadingJet_eta;
    TH1F *fHist_Tag_LeadingJet_phi;
    TH1F *fHist_Tag_2ndLeadingJet_pt;
    TH1F *fHist_Tag_2ndLeadingJet_eta;
    TH1F *fHist_Tag_2ndLeadingJet_phi;
    TH1F *fHist_Tag_Lepton_pt;
    TH1F *fHist_Tag_Lepton_eta;
    TH1F *fHist_Tag_Lepton_phi;

    TH1F *fHist_Tag_BJet_pt;
    TH1F *fHist_Tag_BJet_eta;
    TH1F *fHist_Tag_BJet_phi;
    TH1F *fHist_Tag_LJet_pt;
    TH1F *fHist_Tag_LJet_eta;
    TH1F *fHist_Tag_LJet_phi;

    //BEYS 2-D histograms
    TH2F *fHist_Tag_MjlfjbHt;
    TH2F *fHist_Tag_MljbHt;
    TH2F *fHist_Tag_MjlfjbMljb;
    TH2F *fHist_Tag_MjlfjbMtW;
    TH2F *fHist_Tag_MljbMtW;
    TH2F *fHist_Tag_MjlfjbEtajlf;
    TH2F *fHist_Tag_MljbEtajlf;
    //BEYE 2-D histograms
    
    TH2F *fHist_Tag_QCDVeto;

    // Histograms (mc truth study)
    // ============================
    TH1F *fHist_mc_DecayChannels;
    TH1F *fHist_mc_DecayChannels_Veto;
    TH2F *fHist_mc_TriggerMatch;
    TH1F *fHist_mc_jet_truth_label;
    TH2F *fHist_mc_MET;
    TH2F *fHist_mc_MET_nonVeto;
    TH2F *fHist_mc_RecoEff;
    TH2F *fHist_mc_DeltaR_tight_lepton;
    TH2F *fHist_mc_DeltaR_loose_lepton;
    
    // Dilepton kinematics
    TH2F *fHist_mc_DeltaR_LeadLepton_bQuarkOS;
    TH2F *fHist_mc_DeltaR_LeadLepton_bQuarkSS;
    TH2F *fHist_mc_DeltaR_2ndLeadLepton_bQuarkOS;
    TH2F *fHist_mc_DeltaR_2ndLeadLepton_bQuarkSS;
    TH2F *fHist_mc_DeltaR_LeadLepton_2ndLeadLepton;
    TH2F *fHist_mc_truth_LeadLepton_Pt;
    TH2F *fHist_mc_truth_2ndLeadLepton_Pt_matched;
    TH2F *fHist_mc_truth_2ndLeadLepton_Pt_unmatched;
    TH2F *fHist_mc_reco_LeadLepton_Pt;
    TH2F *fHist_mc_reco_2ndLeadLepton_Pt;
    TH2F *fHist_mc_DeltaR_unmatchedMCLepton_truthjet;
    TH2F *fHist_mc_unmatchedMCLepton_truthjet_pt;

public:
    AtlSgTop_tChannelAnalysis(const char* OutputFilename);
    virtual ~AtlSgTop_tChannelAnalysis();
    
    virtual void Begin(TTree *tree);
    virtual void SetBranchStatus();
    virtual void Clear( Option_t *option = "");
    virtual void BookHistograms();
    virtual void FillHistograms();
    virtual Bool_t DiLeptonVeto();
    virtual Bool_t ProcessPreCut();
    virtual Bool_t ProcessCut();
    virtual void SetCutDefaults();
    virtual void Print(Option_t *option) const;
    virtual void Terminate();

    void BookHistogramsPretag();
    void BookHistogramsTag();
    void BookHistogramsMCTruthStudy();
    void BookHistogramsPreSel(AtlHistogramTool *htool);
    void BookHistogramsSel(AtlHistogramTool *htool);


    void FillHistogramsPretag();
    void FillHistogramsTag();
    void FillHistogramsPreSel(AtlHistogramTool *htool, Double_t W);
    void FillHistogramsSel(AtlHistogramTool *htool, Double_t W);
    void FillHistogramsMCTruthStudy();
    
    void InitEvent();
    void DoTruthMatching();

    ClassDef(AtlSgTop_tChannelAnalysis,0) // Single-top 8 TeV t-Channel analysis
};
#endif

//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2013 (C) Soeren Stamm
//
#ifndef SINGLETOP_AtlSgTop_sChannelAnalysis
#define SINGLETOP_AtlSgTop_sChannelAnalysis
#ifndef ATLAS_AtlSelector
#include <AtlSelector.h>
#endif
#ifndef ATLAS_AtlObjectsToolD3PDSgTop
#include <AtlObjectsToolD3PDSgTop.h>
#endif
#ifndef ATLAS_AtlObjRecoScaleFactorTool
#include <AtlObjRecoScaleFactorTool.h>
#endif
#ifndef ATLAS_AtlCutFlowTool
#include <AtlCutFlowTool.h>
#endif
#ifndef ATLAS_AtlPdfReweightingTool
#include <AtlPdfReweightingTool.h>
#endif
#ifndef ATLAS_HistogramTool
#include <AtlHistogramTool.h>
#endif

class HepTopDecay;
class AtlWDecayLNu;
class TH2F;

class AtlSgTop_sChannelAnalysis : public AtlSelector {
    
  public:
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
    
    Float_t fMET_min;              // Minimum missing Et (GeV)
    Float_t fMET_max;              // Maximum missing Et (GeV)
    Float_t fMtW_minPre;           // Transverse W mass minimum (GeV), event pre-selection
    Float_t fMtW_maxPre;           // Transverse W mass maximum (GeV), event pre-selection
    Float_t fMtW_min;              // Transverse W mass minimum (GeV), final event selection
    Float_t fMtW_max;              // Transverse W mass maximum (GeV), final event selection
    Float_t fPt_Lepton_min;        // Additional minmum Pt cut for lepton (GeV)
    Float_t fEta_Lepton_min;       // Additional minimum eta cut for lepton
    Float_t fEta_Lepton_max;       // Additional maximum eta cut for lepton
    Float_t fPt_Jet_min;           // Additional minimum Pt cut for all jets (GeV)
    Float_t fPt_Jet_max;           // Additional maximum Pt cut for all jets (GeV
    Float_t fPt_LeadingJet_min;    // Additional minimum Pt cut for the leading jet (GeV)
    Float_t fPt_LeadingJet_max;    // Additional maximum Pt cut for the leading jet (GeV)
    Int_t   fNBTags_min;           // Minimum no. of b-tagged jets
    Int_t   fNBTags_max;           // Maximum no. of b-tagged jets
    Bool_t  fLeadingJetIsBTagged;  // Require the leading jet to be b-tagged
    Float_t fPt_BJetTop_min;       // Minimium Pt of b-jet from top decay
    AtlBTag::ETagger fVetoBTagger; // B-tagger used for b-tag veto (ctrl region)
    Float_t fVetoBTagWeight_min;   // Minimum weight for b-tag veto (ctrl region); use 0.5 in case of run-2 data if the veto should be used (for run-2 no tag weights are stored - only boolean numbers
    Bool_t  fAdditionalJetVeto;    // Remove events with additional jets outside the given pt-eta range (default false)
    Bool_t  fAdditionalJetVetoPt;  // Remove events with additional jets outside the given pt range if fAdditionalJetVeto is set (default true)
    Bool_t  fAdditionalJetVetoEta; // Remove events with additional jets outside the given eta range if fAdditionalJetVeto is set (default true)
    Bool_t  fDoReconstructTop;     // Reconstruct the neutrino, the W-boson and the top-quark in the old-fashioned way and compute the 16 LPSC BDT variables (default=off). !!! Note that the neutrino reconstruction might fail in a very few cases leading to a slight loss of events !!!
    Bool_t  fApplyDileptonVeto;    // Apply dilepton veto?
    Bool_t  fApplyTtbar4JetVeto;   // Apply ttbar 4 jet veto?
    ELepChannel fLeptonFlavour;    // Select mu or el channel at event cut level
    Bool_t  fEvtWriterCloneOnly;   // Parameter to give to EvtWriter
    Bool_t fQCDVeto;               // Apply QCD veto?
 
    Float_t fHt_min;               // Minimum Ht (GeV)
    Float_t fDeltaR_Jet1Jet2_min;  // Delta R between the 2 jets
    Float_t fBDTResponse_min;      // BDT response 
    
    Bool_t fMCTruthStudy;          // Flag for filling mc truth info for dilepton study
    HepMCParticle *fMCbQuark;
    HepMCParticle *fMCAntibQuark;
    HepMCParticle *fMCLeptonPlus ;
    HepMCParticle *fMCLeptonMinus;
    HepMCParticle *fMCTightLepton;
    HepMCParticle *fMCLooseLepton;
    
  private:
    HepParticle *fLepton;       // The leading lepton
    TList       *fLeptons;      // List of selected leptons
    TList       *fLooseLeptons; // List of selected loose leptons
    TList       *fElectrons;    // List of selected electron objects
    TList       *fMuons;        // List of selected muon objects
    TList       *fLooseTaus;    // List of selected loose taus
    TList       *fJets;         // List of selected jets
    TList       *fBJets;        // List of selected b-jets

    // QCD Tool
    Bool_t    fQCDMMToolIsOn;  // Flag for QCD Matrix Method tasks

    // BDT variables
    Int_t     fN_looseleptons;             // Number of loose leptons 
    Int_t     fN_loosejets;                // Number of loose jets 
    Float_t   fHt;                         // Scalar sum of the transverse momenta of the jets, the charged leptons and the missing transverse momentum (GeV)
    Float_t   fMET;                        // MissingEt of current event
    Float_t   fMtW;                        // Transverse W mass (GeV)
    Float_t   fPtLep;                      // Transverse momentum (GeV) of leading lepton
    Float_t   fQLep;                       // Charge of leading lepton
    Float_t   fDeltaPhiTopBJet2_BJet1;     // Delta phi between top-quark candidate reconstructed using the sub-leading b-jet and the leading b-jet
    Float_t   fDeltaPhiTopBJet1_BJet2;     // Delta phi between top-quark candidate reconstructed using the leading b-jet and the sub-leading b-jet
    Float_t   fSumEtLep_MET;               // Sum Et of lepton and MET
    Float_t   fSumEtLep_MET_BJets;         // Sum Et of lepton, MET and all b-jets
    Float_t   fSumPt;                      // Vectoriell Pt sum of the lepton, MET and all b-jets
    Float_t   fDeltaEtaLep_BJet1;          // Delta eta between the lepton and the leading b-jet
    Float_t   fDeltaEtaLep_BJet2;          // Delta eta between the lepton and the sub-leading b-jet
    Float_t   fDeltaEtaTopBJet2_BJet1;     // Delta eta between top decay reconstructed using the sub-leading b-jet and the leading b-jet
    Float_t   fDeltaPhiBJet1_BJet2;        // Delta phi between leading and sub-leading b-jet
    Float_t   fDeltaPhiLep_MET;            // Delta phi between lepton and MET
    Float_t   fCosLepW_WTopBJet1;          // Cos of angle between lepton momentum in W rest-frame and the W momentum in the rest-frame of the top-quark candidate with the leading b-jet
    Float_t   fCosLepTopBJet1_TopBJet1CMS; // Cos of angle between the lepton momentum in the top-quark rest-frame and the top-quark momentum in the c.m.s. The used top-quark is the candidate reconstructed using the leading b-jet. The c.m.s vector is the sum of the top momentum and the sub-leading b-jet momentum
    Float_t   fCosLepTopBJet2_TopBJet2CMS; // Cos of angle between the lepton momentum in the top-quark rest-frame and the top-quark momentum in the c.m.s. The used top-quark is the candidate reconstructed using the sub-leading b-jet. The c.m.s vector is the sum of the top momentum and the leading b-jet momentum
    Float_t   fPt_BJetTop;                 // Pt of b-jet from best recosntructed top decay (GeV)
    Float_t   fDeltaEtaNu_BJetNonTop;      // Delta eta between the rec. neutrino and the b-jet from the non-top candidate
    Float_t   fSumPtBJet1_BJet2;           // Vectoriell Pt sum of the leading and the sub-leading b-jet (GeV)
    Float_t   fCosMET_BJet2;               // Cos of angle between MET and sub-leading b-jet
    Float_t   fMLep_BJet2;                 // Invariant mass of lepton and sub-leading b-jet (GeV)
    Float_t   fMTop;                       // Inv mass of best reconstructed top
    Float_t   fMLep_BJetTop;               // Inv mass of lepton and b-jet from top
    Float_t   fMTop_NonTopBJet;            // Inv mass of top and b-jet not from top
    Float_t   fXsection;                   // sample xsec used for BDT evt wgt norm.
    
    // Pointer to tools in event loop
    AtlObjectsToolD3PDSgTop   *fObjTool;
    AtlObjRecoScaleFactorTool *fSFTool;
    AtlCutFlowTool            *fCutFlowTool;
    AtlPdfReweightingTool     *fPdfTool;
    AtlHistogramTool          *fHistsPretag;
    AtlHistogramTool          *fHistsBTag;
    AtlHistogramTool          *fHistsBTagElectron;
    AtlHistogramTool          *fHistsBTagMuon;
    AtlHistogramTool          *fHistsBDT;
    AtlHistogramTool          *fHistsBDTElectron;
    AtlHistogramTool          *fHistsBDTMuon;

    // Histograms (mc truth study)
    // ============================
    TH1F *fHist_mc_DecayChannels;
    TH1F *fHist_mc_DecayChannels_Veto;
    TH1F *fHist_mc_DecayChannels_TauVeto;
    TH1F *fHist_mc_DecayChannels_AllVeto;
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
    TH2F *fHist_mc_truth_2ndLeadLepton_Pt;
    TH2F *fHist_mc_truth_2ndLeadLepton_Pt_matched;
    TH2F *fHist_mc_truth_2ndLeadLepton_Pt_unmatched;
    TH2F *fHist_mc_reco_LeadLepton_Pt;
    TH2F *fHist_mc_reco_2ndLeadLepton_Pt;
    TH2F *fHist_mc_reco_LeadLepton_Pt_res;
    TH2F *fHist_mc_reco_2ndLeadLepton_Pt_res;
    TH2F *fHist_mc_DeltaR_unmatchedMCLepton_truthjet;
    TH2F *fHist_mc_unmatchedMCLepton_truthjet_pt;
    
public:
    explicit AtlSgTop_sChannelAnalysis(const char* OutputFilename);
    virtual ~AtlSgTop_sChannelAnalysis();
    
    virtual void SetBranchStatus();
    virtual void Clear( Option_t *option = "");
    virtual void Begin(TTree *tree);
    virtual void BookHistograms();
    virtual void FillHistograms();
    virtual Bool_t ProcessPreCut();
    virtual Bool_t DiLeptonVeto();
    virtual Bool_t ProcessCut();
    virtual void SetCutDefaults();
    virtual void Print(Option_t *option) const;
    virtual void Terminate();
    
    inline void SetXsection(Float_t xsec) { fXsection = xsec; }
    inline AtlJet* GetBJet1st() const {
	//
	// Get leading b-jet.
	//
	return dynamic_cast<AtlJet*>(fBJets->At(0));
    }
    inline AtlJet* GetBJet2nd() const {
	//
	// Get second leading b-jet.
	//
	return dynamic_cast<AtlJet*>(fBJets->At(1));
    }
    inline HepTopDecay* GetTopDecayBest() const {
	//
	// Get best top decay candidate (invariant mass closest to
	// pole-mass.
	//
	return dynamic_cast<HepTopDecay*>(
            fEvent->GetTopDecays()->At(fEvent->GetN_TopDecays()-1));
    }
    inline HepTopDecay* GetTopDecay2ndBest() const {
	//
	// Get 2nd best top decay candidate.
	//
	return dynamic_cast<HepTopDecay*>(
            fEvent->GetTopDecays()->At(fEvent->GetN_TopDecays()-2));
    }
    inline HepTopDecay* GetTopDecayWith1stBJet() const {
	//
	// Get top decay candidate with leading b-jet.
	//
	for ( Int_t i = 0; i < fEvent->GetN_TopDecays(); i++ ) {
	    HepTopDecay* top = dynamic_cast<HepTopDecay*>(
                fEvent->GetTopDecays()->At(i));
	    if ( top->GetBJetOrig() == GetBJet1st() )
		return top;
	}
	return 0;
    }
    inline HepTopDecay* GetTopDecayWith2ndBJet() const {
	//
	// Get top decay candidate with next-leading b-jet.
	//
	for ( Int_t i = 0; i < fEvent->GetN_TopDecays(); i++ ) {
            HepTopDecay* top = dynamic_cast<HepTopDecay*>(
                fEvent->GetTopDecays()->At(i));
	    if ( top->GetBJetOrig() == GetBJet2nd() )
		return top;
	}
	return 0;
    }
    
  protected:
    void BookHistogramsPretag(AtlHistogramTool *htool);
    void BookHistogramsPreSel(AtlHistogramTool *htool);
    void BookHistogramsSel(AtlHistogramTool *htool);
    void BookHistogramsBDT(AtlHistogramTool *htool);
    void BookHistogramsMCTruthStudy();
    void FillHistogramsPretag(AtlHistogramTool *htool, Double_t W);
    void FillHistogramsPreSel(AtlHistogramTool *htool, Double_t W);
    void FillHistogramsSel(AtlHistogramTool *htool, Double_t W);
    void FillHistogramsBDT(AtlHistogramTool *htool);
    void FillHistogramsMCTruthStudy();
    void FillHistogramsLeptons(AtlHistogramTool *htool, Double_t W);
    void FillHistogramsElectrons( AtlHistogramTool *htool, Double_t W);
    void FillHistogramsMuons( AtlHistogramTool *htool, Double_t W);
    void InitEvent();

  private:
    Bool_t ReconstructNeutrino();
    void ReconstructWDecay();
    void ReconstructTopDecays();
    void ComputeBDTVariables();
    void SetBDTVariables();
    void DoTruthMatching();
    
    ClassDef(AtlSgTop_sChannelAnalysis,0) // Single-top 8 TeV s-Channel analysis
};
#endif

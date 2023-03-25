//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2015 (C) Oliver Maria Kind
//
#ifndef SINGLETOP_AtlSgTopFCNC_tZ_Base
#define SINGLETOP_AtlSgTopFCNC_tZ_Base
#ifndef ATLAS_AtlSelector
#include <AtlSelector.h>
#endif
#ifndef ATLAS_AtlObjectsToolD3PDSgTop
#include <AtlObjectsToolD3PDSgTop.h>
#endif
#ifndef ATLAS_AtlObjRecoScaleFactorTool
#include <AtlObjRecoScaleFactorTool.h>
#endif
#ifndef ATLAS_AtlHistogramTool
#include <AtlHistogramTool.h>
#endif
#ifndef ATLAS_AtlMCTruthClassifier
#include <AtlMCTruthClassifier.h>
#endif
#ifndef ROOT_TPie
#include <TPie.h>
#endif

class AtlSgTopFCNC_tZ_Base : public AtlSelector {

  public:
    enum EChgComb { kChgOpposite, kChgSame, kChgAny };
    
  public:
    Int_t    fNLeptons_min;   // Minimum number of leptons per event
    Int_t    fNLeptons_max;   // Maximum number of leptons per event
    Int_t    fNBTags_min;     // Minimum number of b-jets
    Int_t    fNBTags_max;     // Maximum number of b-jets
    Double_t fPt_Lep1_min;    // Minimum Pt of leading lepton
    Double_t fPt_Lep2_min;    // Minimum Pt of sub-leading lepton
    Double_t fPt_Lep3_min;    // Minimum Pt of third lepton
    Double_t fMET_min;        // Minimum missing energy
    Double_t fMET_max;        // Maximum missing energy
    Double_t fMtW_min;        // Minimum transverse W mass
    Double_t fMtW_max;        // Maximum transverse W mass
    Double_t fMassZ_min;      // Minimum reconstructed Z mass
    Double_t fMassZ_max;      // Maximum reconstructed Z mass
    Double_t fMassTop_min;    // Minimum reconstructed top mass
    Double_t fMassTop_max;    // Maximum reconstructed top mass
    Bool_t   fReconstructTop; // whether to attempt reconstruction of a top-quark candidate
    Bool_t   fRequireZ;       // whether to require a reconstructed Z-boson candidate
    Bool_t   fRequireTop;     // whether to require a reconstructed top-quark candidate
    Bool_t   fVetoZboson;     // Veto for reconstructed Z bosons. Accept event only if the reconstructed Z is outside the given mass window
    Bool_t   fVetoTop;        // Veto for reconstructed top quarks. Accept event only if the reconstructed Top is outside the given mass window

  protected:
    TList    *fLeptons;   // List of selected leptons
    TList    *fElectrons; // List of selected electron objects
    TList    *fMuons;     // List of selected muon objects
    TList    *fJets;      // List of selected jets
    TList    *fBJets;     // List of selected b-tagged jets
    Double_t  fMET;       // Missing transverse momentum
    Double_t  fMtW;       // Transverse W boson mass (using the leading lepton !)
    Double_t  fDileptonM; // Invariant mass of the leading and sub-leading lepton

    // Tools
    AtlObjectsToolD3PDSgTop   *fObjTool; // Object definition
    AtlObjRecoScaleFactorTool *fSFTool;  // Scale factors

    // Histograms
    AtlHistogramTool *fHistsNoCuts;  // Histograms w/o any cuts
    AtlHistogramTool *fHistsPreSel;  // Pre-selection histograms
    AtlHistogramTool *fHistsPreTag;  // Pre-tagged histograms
    AtlHistogramTool *fHistsAllCuts; // Histograms with all cuts applied (also b-tagging)

    Bool_t fReconstructedZ; // whether Z reconstruction has run
    
public:
    AtlSgTopFCNC_tZ_Base(const char* OutputFileName);
    virtual ~AtlSgTopFCNC_tZ_Base();

    virtual void Begin(TTree * tree);
    virtual void SlaveBegin(TTree * tree);
    virtual void Terminate();
    virtual void Print(Option_t * option) const;
    virtual void SetCutDefaults();
    virtual void SetBranchStatus();
    virtual void Clear(Option_t * option = "");
    virtual Bool_t ProcessCut() = 0;
    virtual void BookHistograms();
    virtual void FillHistograms() {;}
    virtual void BookHistogramsCommon(AtlHistogramTool *htool);
    virtual void BookHistogramsLeptons(AtlHistogramTool *htool);
    virtual void BookHistogramsJets(AtlHistogramTool *htool);
    virtual void BookHistogramsMET(AtlHistogramTool *htool);
    virtual void FillHistogramsCommon(AtlHistogramTool *htool,
				      Double_t w);
    virtual void FillHistogramsLeptons(AtlHistogramTool *htool,
				       Double_t w);
    virtual void FillHistogramsJets(AtlHistogramTool *htool,
				    Double_t w);
    virtual void FillHistogramsMET(AtlHistogramTool *htool,
				   Double_t w);
    void ComputeDecayAngleE(HepParticle *mother, HepParticle *daughter,
			    Double_t &DecayAngle, Double_t &E);
    
    inline HepZ0Decay* GetZ0DecayBest() const {
	//
	// Get best Z->l+l- decay candidate
	// (invariant mass closest to true Z mass)
	//
	return (HepZ0Decay*)fEvent->GetZ0Decays()
	    ->At(fEvent->GetN_Z0Decays()-1);
    }
    
  protected:
    virtual void InitEvent();
    void ReconstructZ0Decays();
    virtual void BookHistogramsZ0(AtlHistogramTool *htool, const char* path);
    virtual void BookHistogramsZ0Subdir(AtlHistogramTool *htool, const char* path);
    virtual void FillHistogramsZ0(AtlHistogramTool *htool, Double_t w,
				  const char* path, HepZ0Decay *Z0cand);
    virtual void FillHistogramsZ0Subdir(AtlHistogramTool *htool, Double_t w, 
					const char* path, const char* subdir,
					HepZ0Decay *Z0cand);
    virtual void CreatePieCharts();
    TPie* CreateParticleOriginPie(const char *HistName, Int_t MaxSlices = 5);
    void SetParticleOriginHistBinLabels(TH1D *h);
    
    ClassDef(AtlSgTopFCNC_tZ_Base,0) // Analysis base class for the single-top FCNC tZ analysis
};
#endif


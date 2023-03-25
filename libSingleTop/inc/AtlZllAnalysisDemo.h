//  
// Author: Sebastian Mergelmeyer <mailto: mergelm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Sebastian Mergelmeyer
//
#ifndef SINGLETOP_AtlZllAnalysisDemo
#define SINGLETOP_AtlZllAnalysisDemo
#ifndef ATLAS_AtlSelector
#include <AtlSelector.h>
#endif
#ifndef ATLAS_AtlObjectsToolD3PDSgTop
#include <AtlObjectsToolD3PDSgTop.h>
#endif
#ifndef ATLAS_AtlObjRecoScaleFactorTool
#include <AtlObjRecoScaleFactorTool.h>
#endif

class AtlZllAnalysisDemo : public AtlSelector {

  public:
    enum EChgComb { kChgOpposite, kChgSame, kChgAny };
    
  private:
    enum EHistDir { kPreTag, kBTag, kZPeak, kNDirs };
    
  public:
    EChgComb fChgCombination; // Charge combination of thw first two leading leptons
    Int_t    fNLeptons_min;   // Minimum number of leptons per event
    Int_t    fNBTags_min;     // Minimum number of b-jets
    Int_t    fNBTags_max;     // Maximum number of b-jets
    Double_t fPt_Lep1_min;    // Minimum Pt of leading lepton
    Double_t fMZ_min;         // Minimum Z mass
    Double_t fMZ_max;         // Maximum Z mass
    Double_t fMET_min;        // Minimum missing energy
    Double_t fMET_max;        // Maximum missing energy
    Double_t fMtW_min;        // Minimum transverse W mass
    Double_t fMtW_max;        // Maximum transverse W mass
    
  private:
    TList    *fLeptons;   // List of selected leptons
    TList    *fElectrons; // List of selected electron objects
    TList    *fMuons;     // List of selected muon objects
    TList    *fJets;      // List of selected jets
    TList    *fBJets;     // List of selected b-tagged jets
    Double_t  fMET;       // Missing transverse momentum
    Double_t  fMtW;       // Transverse W boson mass
    Double_t  fDileptonM; // Invariant mass of the leading and sub-leading lepton

    // Tools
    AtlObjectsToolD3PDSgTop   *fObjTool;
    AtlObjRecoScaleFactorTool *fSFTool;

    // ==========
    // Histograms
    // ==========

    // Leptons
    TH1D *fHist_Lep1_Pt[AtlZllAnalysisDemo::kNDirs];
    TH1D *fHist_Lep1_Eta[AtlZllAnalysisDemo::kNDirs];
    TH1D *fHist_Lep1_Phi[AtlZllAnalysisDemo::kNDirs];
    TH1D *fHist_Lep2_Pt[AtlZllAnalysisDemo::kNDirs];
    TH1D *fHist_Lep2_Eta[AtlZllAnalysisDemo::kNDirs];
    TH1D *fHist_Lep2_Phi[AtlZllAnalysisDemo::kNDirs];
    TH1D *fHist_Lep_Chg[AtlZllAnalysisDemo::kNDirs];
    TH1D *fHist_Lep_N[AtlZllAnalysisDemo::kNDirs];
    TH1D *fHist_Lep_ID[AtlZllAnalysisDemo::kNDirs];

    // Jets
    TH1D *fHist_Jet1_Pt[AtlZllAnalysisDemo::kNDirs];
    TH1D *fHist_Jet1_Eta[AtlZllAnalysisDemo::kNDirs];
    TH1D *fHist_Jet1_Phi[AtlZllAnalysisDemo::kNDirs];
    TH1D *fHist_Jet_N[AtlZllAnalysisDemo::kNDirs];
    
    // MET
    TH1D *fHist_MET_mag[AtlZllAnalysisDemo::kNDirs];
    TH1D *fHist_MET_phi[AtlZllAnalysisDemo::kNDirs];
    TH1D *fHist_MtW[AtlZllAnalysisDemo::kNDirs];

    // DiLepton
    TH1D *fHist_Dilepton_DeltaPhi[AtlZllAnalysisDemo::kNDirs];
    TH1D *fHist_Dilepton_Mass[AtlZllAnalysisDemo::kNDirs];
    TH1D *fHist_Dilepton_Chg[AtlZllAnalysisDemo::kNDirs];

  public:
    AtlZllAnalysisDemo(char const* OutputFileName);
    virtual ~AtlZllAnalysisDemo();
    
    virtual void Begin(TTree * tree);
    virtual void SlaveBegin(TTree * tree);
    virtual void BookHistograms();
    virtual void SetCutDefaults();
    virtual void SetBranchStatus();
    virtual void Clear(Option_t * option = "");
    virtual Bool_t ProcessPreCut();
    virtual Bool_t ProcessCut();
    virtual void FillHistograms();
    virtual void Terminate();
    virtual void Print(Option_t * option) const;

  protected:
    virtual void InitEvent();
    virtual void BookHistogramsCommon(EHistDir HistDir);
    virtual void FillHistogramsCommon(EHistDir HistDir);
    
    ClassDef(AtlZllAnalysisDemo,0) // Z->ll 13 TeV demo analysis
};
#endif

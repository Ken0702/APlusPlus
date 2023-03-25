//  
// Author: Ruth Herrberg <mailto: ruthh@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef SINGLETOP_Atl_tChannelAnalysis
#define SINGLETOP_Atl_tChannelAnalysis
#ifndef ATLAS_AtlSelector
#include <AtlSelector.h>
#endif
#ifndef ATLAS_AtlTopPairFinder
#include <AtlTopPairFinder.h>
#endif
#ifndef ATLAS_AtlQCDMMTool
#include <AtlQCDMMTool.h>
#endif
#ifndef ATLAS_AtlSgTop_tChannelFinder
#include <AtlSgTop_tChannelFinder.h>
#endif
#ifndef ATLAS_AtlCutFlowTool
#include <AtlCutFlowTool.h>
#endif
#ifndef ATLAS_AtlObjectsToolD3PDSgTop
#include <AtlObjectsToolD3PDSgTop.h>
#endif

class Atl_tChannelAnalysis : public AtlSelector {

private:
    
  // Pile-up control histograms
  TH1F *fHist_AverageIntPerXing; // average interactions per bunch-crossing
  TH1F *fHist_NPrimaryVertices;  // number of primary vertices
    
  // Vertex histograms
  TH1F *fHist_PV_X; // Primary Vertex - X-Component
  TH1F *fHist_PV_Y; // Primary Vertex - Y-Component
  TH1F *fHist_PV_Z; // Primary Vertex - Z-Component
  TH2F *fHist_PV_XY; // Primary Vertex - XYvsZ-Component
  
  
  // Lepton candidates
  TList *fLeptons;   // Merged list of signal leptons (e, mu)
  TList *fElectrons; // Signal electron list 
  TList *fMuons;     // Signal muon list
  TList *fJets;      // list of jets
  TList *fBJets;     // list of b-jets

  // Pointer to tools in event loop
  AtlObjectsToolD3PDSgTop *fObjTool;
  AtlTopPairFinder        *fTopPair_tool;  
  AtlSgTop_tChannelFinder *ftChannel_tool;
  AtlCutFlowTool          *fCutflow_tool;
  AtlCutFlowTool          *fCutflow_tool_2;
  AtlQCDMMTool            *fQCD_tool;  
  
  public:
  
  Bool_t   fDoMatrixMethod; // Do QCD estimation with MM?
  Bool_t fSignalSampleMode; // Analyzing signal MC?
  
  // Etmiss
  TVector2 fMET;         // MissingEt of current event
  Float_t  fMET_Mag_min; // minimum missing Et        

  // Pileup-weight cut
  Float_t fPileupWeightMax; // Maximum value of pile-up weight allowed
  
public:
  Atl_tChannelAnalysis(const char* OutputFilename);
  virtual ~Atl_tChannelAnalysis();
  
  virtual void SetBranchStatus();
  virtual void Clear( Option_t *option = "");
  virtual void BookHistograms();
  virtual void FillHistograms();
  virtual Bool_t ProcessPreCut();
  virtual Bool_t ProcessCut();
  virtual void SetCutDefaults();
  virtual void Print() const;
  virtual void Terminate();
  
  void InitEvent();
  void BookVertexHistograms();
  void FillVertexHistograms();
    
  ClassDef(Atl_tChannelAnalysis,0) // Single-top t-Channel analysis
};
#endif








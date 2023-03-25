//  
// Author: Michelangelo Giorgi <mailto: michelangelo.giorgi@cern.ch>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef SINGLETOP_Atl_WtChannelAnalysis
#define SINGLETOP_Atl_WtChannelAnalysis
#ifndef ATLAS_AtlSelector
#include <AtlSelector.h>
#endif
#ifndef ATLAS_AtlTopPairFinder
#include <AtlTopPairFinder.h>
#endif
#ifndef ATLAS_AtlSgTop_WtChannelFinder
#include <AtlSgTop_WtChannelFinder.h>
#endif
#ifndef ATLAS_AtlCutFlowTool
#include <AtlCutFlowTool.h>
#endif

class Atl_WtChannelAnalysis : public AtlSelector {

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
  HepParticle *fLepton; // Signal lepton for effiency scale factor determination used for evt re-weighting
  TList *fLeptons;      // Merged list of signal leptons (e, mu) from Top Common Objects selection
  TList *fElectrons;    // Signal electron list from  Top Common Objects selection
  TList *fMuons;        // Signal muon list from  Top Common Objects selection

  // Generic jet and b-jet candidates
  TList *fJets;  // List of all jets from Top Common Objects selection

/*   // Veto decisions */
/*   Bool_t fHasLArError;         // event affected by LArError? */

  // Pointer to tools in event loop
  AtlTopPairFinder                *fTopPair_tool;  
  AtlSgTop_WtChannelFinder        *fWtChannel_tool;
  AtlCutFlowTool                  *fCutflow_tool;
  AtlCutFlowTool                  *fCutflow_tool_AC;
  
  
  public:
  
  // Modes of running
  enum ELeptonMode {
      kElectronMode = BIT(0), // electron channel
      kMuonMode     = BIT(1), // muon channel
      kLeptonAll    = ( kElectronMode | kMuonMode ) // e or mu channel
  };

  ELeptonMode fLeptonMode;  // Indicate the lepton channel mode (e/mu)
  
  Bool_t fSignalSampleMode; // Analyzing signal MC?

  // Etmiss
  TVector2 fMET;         // MissingEt of current event
  Float_t  fMET_Mag_min; // minimum missing Et        

  // Jets
  AtlJet::EType        fJetsAuthor;       // Author type of jets

  // Pileup-weight cut
  Float_t fPileupWeightMax; // Maximum value of pile-up weight allowed

  
public:
  Atl_WtChannelAnalysis(const char* OutputFilename);
  virtual ~Atl_WtChannelAnalysis();
  
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
  
  ClassDef(Atl_WtChannelAnalysis,0) // Single-top Wt-Channel analysis
};
#endif








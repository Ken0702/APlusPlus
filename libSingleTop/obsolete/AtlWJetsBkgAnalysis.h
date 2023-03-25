//  
// Author: Steven Bilski <mailto: bilski@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2011 (C) Steven Bilski
//
#ifndef ATLAS_AtlWJetsBkgAnalysis
#define ATLAS_AtlWJetsBkgAnalysis
#ifndef ATLAS_AtlSelector
#include <AtlSelector.h>
#endif


class TString;
class TH1F;
class TH2F;

class AtlWJetsBkgAnalysis : public AtlSelector {

 private:
    /*TH1F *fHistAtlFastNJets; // No. of AtlFast jets
  TH1F *fHistMCPrtPt;      // Transv. momentum of MC particles
  TH2F *fHistMCPrtEtaPhi;  // Eta-Phi plane of MC particles
  TH1F *fHistNMuons;       // No. of muons per event
  TH1F *fHistMuonPt;       // Trans. momentum of muons*/
  TH1I *fHist_NSgTop;     // Number of SingeTops per Event
  TH1F *fHist_NSgTop_bar; // Number of SingeTops_bar per Event
  TH1F *fHist_NSgTopTop_bar; // Number of Single Tops + Tops_bar per Event
  TH1F *fHist_SgTop_Pt;  //Trans. momentum of SgTop
  TH1F *fHist_SgTopBar_Pt; //Trans. momentum of SgTop_bar
  TH1F *fHist_SgTopTopBar_Pt; //Trans. momentum of SgTop+SgTop_bar
  TH1F *fHist_NJets;		//Number of Jets per Event
  TH1F *fHist_FwrdJet_Pt;   //Trans. momentum of SgTop
  TH1F *fHist_SumPxPyOfTopAndJet; //Sum of Momentum in X-Y-Plane of Top and FwrdJet
  TH1F *fHist_SumPxPyOfTopAndJet_EMScale; ////Sum of Momentum EMScale in X-Y-Plane of Top and FwrdJet
  TH1F *fHist_SumPxPyOfTopAndJet_JES;	//Sum of Momentum JES in X-Y-Plane of Top and FwrdJet
  TH1F *fHist_SumPxPyOfTopAndJetQuark;  ////Sum of Momentum JES in X-Y-Plane of Top and JetQuark
  TH1F *fHist_DiffPtOfTopAndJetQuark; //Diff of Pt of Top and JetQuark
  TH1F *fHist_JetTop_DeltaPhi; 	  //Delta Phi of Jet and Top in x-y-plane
  TH1F *fHist_JetQuarkTop_DeltaPhi; //Delta Phi of Quark,which products the jet and Top in x-y-plane
  
 
 
  
 public:
  AtlWJetsBkgAnalysis(const char* OutputFilename);
  virtual ~AtlWJetsBkgAnalysis();
  
  virtual void    SetBranchStatus();
  virtual void    BookHistograms();
  virtual void    FillHistograms();
  virtual void    Terminate();
  virtual Bool_t  ProcessCut();
  virtual AtlJet* GetFwrdJet();
  //virtual Float_t GetPxPyBalance(HepMcParticle *top,AtlJet *fwrdjet);
  inline Float_t GetSumPt(Float_t Ptx1,Float_t Pty1,Float_t Ptx2, Float_t Pty2){
      //returns the vector length of two P2 vectors in x-y-plane
      return TMath::Sqrt((Ptx1+Ptx2)*(Ptx1+Ptx2)+(Pty1+Pty2)*(Pty1+Pty2));
  }
  
  
  ClassDef(AtlWJetsBkgAnalysis,0) // A++ example analysis task
};
#endif


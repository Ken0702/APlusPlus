//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlExampleAnalysis
#define ATLAS_AtlExampleAnalysis
#ifndef ATLAS_AtlSelector
#include <AtlSelector.h>
#endif

class TString;
class TH1F;
class TH2F;

class AtlExampleAnalysis : public AtlSelector {

 private:
  TH1F *fHistAtlFastNJets; // No. of AtlFast jets
  TH1F *fHistMCPrtPt;      // Transv. momentum of MC particles
  TH2F *fHistMCPrtEtaPhi;  // Eta-Phi plane of MC particles
  TH1F *fHistNMuons;       // No. of muons per event
  TH1F *fHistMuonPt;       // Trans. momentum of muons
  
 public:
  AtlExampleAnalysis(const char* OutputFilename);
  virtual ~AtlExampleAnalysis();
  
  virtual void   SetBranchStatus();
  virtual void   BookHistograms();
  virtual void   FillHistograms();
  virtual Bool_t ProcessCut();
  
  ClassDef(AtlExampleAnalysis,0) // A++ example analysis task
};
#endif


//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2013 (C) Oliver Maria Kind
//
#ifndef SINGLETOP_AtlMemNtupleProduction
#define SINGLETOP_AtlMemNtupleProduction
#ifndef ATLAS_AtlSelector
#include <AtlSelector.h>
#endif
#ifndef ATLAS_AtlObjectsToolD3PDSgTop
#include <AtlObjectsToolD3PDSgTop.h>
#endif
#ifndef ATLAS_AtlObjRecoScaleFactorTool
#include <AtlObjRecoScaleFactorTool.h>
#endif
#ifndef ATLAS_AtlBTag
#include <AtlBTag.h>
#endif

class TList;

class AtlMemNtupleProduction : public AtlSelector {

public:
    Float_t fMET_min;             // Minimum missing Et (GeV)
    Float_t fMtW_min;             // Transverse W masss minimum (GeV)
    Int_t   fNBTags_min;          // Minimum no. of b-tagged jets
    Int_t   fNBTags_max;          // Maximum no. of b-tagged jets
    Bool_t  fLeadingJetIsBTagged; // Require the leading jet to be b-tagged
	
private:
    TList *fElectrons; // List of selected electron objects
    TList *fMuons;     // List of selected muon objects
    TList *fLeptons;   // List of selected leptons
    TList *fJets;      // List of selected jets
    TList *fBJets;     // List of selected b-jets
    AtlObjectsToolD3PDSgTop   *fObjTool; // Object selection
    AtlObjRecoScaleFactorTool *fSFTool;  // Scale factor calculation

public:
    AtlMemNtupleProduction(const char* OutputFilename);
    virtual ~AtlMemNtupleProduction();
    virtual void SetBranchStatus() {;}
    virtual void Clear( Option_t *option = "");
    virtual void BookHistograms();
    virtual void FillHistograms();
    virtual Bool_t ProcessCut();
    virtual void Print(Option_t *option = "") const;
    
    ClassDef(AtlMemNtupleProduction,0) // MEM Ntuple production
};
#endif


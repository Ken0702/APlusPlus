//
//    Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
//    Update: $Id$
//    Copyright: 2013 (C) Soeren Stamm
//
#ifndef ATLAS_AtlObjectsToolD3PDSgTop
#define ATLAS_AtlObjectsToolD3PDSgTop
#ifndef ATLAS_AtlObjectsDefinitionTool
#include <AtlObjectsDefinitionTool.h>
#endif
#ifndef ATLAS_AtlSelector
#include <AtlSelector.h>
#endif
#ifndef ATLAS_AtlCutFlowTool
#include <AtlCutFlowTool.h>
#endif
#include <fstream>
#include <iostream>

class AtlEvent;
class TString;
class TROOT;

class AtlObjectsToolD3PDSgTop : public AtlObjectsDefinitionTool {

  public:
    // Mode
	Int_t  fCounter; //Counter for events TEST
    UInt_t fLepChannel;  // Indicate the lepton channel mode (e/mu)
    UInt_t fJetMults;    // Indicate the Jet multiplicity
    Bool_t fAddToCutFlow;    // Flag if cutflow should be filled
    
    Float_t fElectronPt_min; // Min. Pt of signal electrons
    Float_t fMuonPt_min;     // Min. Pt of signal muons
    Int_t   fLeptonN_min;    // Min. number of leptons
    Int_t   fLeptonN_max;    // Max. number of leptons
    Float_t fJetEta_min;     // Min. allowed |Eta| for jets
    Float_t fJetEta_max;     // Max. allowed |Eta| for jets
    Float_t fJetPt_min;      // Min. Pt of jets
    Float_t fFwdJetEta_min;     // Min. allowed |Eta| for fwd jets
    Float_t fFwdJetEta_max;     // Max. allowed |Eta| for fwd jets
    Float_t fFwdJetPt_min;      // Min. Pt of fwd jets
    Float_t fBJetEta_min;    // Min. allowed |Eta| for b-jets
    Float_t fBJetEta_max;    // Max. allowed |Eta| for b-jets
    Int_t   fJetN_min;       // Min. Number of Jets required in Event
    Int_t   fJetN_max;       // Max. Number of Jets allowed in Event

    Bool_t fUseTaus;         // Flag for using loose taus (incl. overlap removal!)

    // Jet selection cuts       
    AtlJet::EType        fJetsAuthor;     // Author type of jets

    // B-jet selection cuts
    AtlBTag::ETagger     fBTagger;        // B-tagger used
    Float_t              fBTagWeight_min; // min B-tag weight cut
    Float_t              fBTagWeight_max; // max B-tag weight cut for ctrl regions

    Int_t                fFilterMEPhoton; // filter events based on presence of ME photon (0 = off; 1 = require; -1 = veto)

  protected:
    TList *fLeptons;        // List of leptons
    TList *fElectrons;      // List of electrons
    TList *fMuons;          // List of muons
    TList *fLooseLeptons;   // List of loose leptons
    TList *fLooseElectrons; // List of loose electrons
    TList *fLooseMuons;     // List of loose muons
    TList *fLooseTaus;      // List of loose taus
    TList *fJets;           // List of jets
    TList *fBJets;          // List of b-jets
    Bool_t fQCDMMToolIsOn;  // Flag for QCD Matrix Method tasks
    Bool_t fIsClean;        // Flag to track event clearing

  public:
    AtlObjectsToolD3PDSgTop(const char* name, const char* title);
    virtual ~AtlObjectsToolD3PDSgTop();
    void ApplyLooseLeptonPtCut();

    //
    // Getters
    //
    inline Int_t GetJetN_min() const { return fJetN_min; }
    inline Int_t GetJetN_max() const { return fJetN_max; }
    inline AtlBTag::ETagger GetBTagger() const { return fBTagger; }
    inline Float_t GetBTagWeightMin() const { return fBTagWeight_min; }
    
    // Objects of signal quality
    virtual TList*  GetLeptons() { return fLeptons; }
    virtual TList*  GetElectrons() { return fElectrons; }
    virtual TList*  GetMuons() { return fMuons; }
    virtual TList*  GetLooseLeptons() { return fLooseLeptons; }
    virtual TList*  GetLooseElectrons() { return fLooseElectrons; }
    virtual TList*  GetLooseMuons() { return fLooseMuons; }
    virtual TList*  GetLooseTaus() { return fLooseTaus; }
    virtual TList*  GetJets() { return fJets; }
    virtual TList*  GetBJets() { return fBJets; }
    virtual UInt_t  GetLepChannel() { return fLepChannel; }

 protected:
    virtual void   BookHistograms();
    virtual void   FillHistograms();
    virtual void   SetBranchStatus();
    virtual void   SetCutDefaults();
    virtual void   Clear();
    virtual void   Init();
    virtual void   Terminate();
    virtual void   Print() const;
    virtual Bool_t AnalyzeEvent();
    virtual void   FindElectrons();
    virtual void   FindMuons();
    virtual void   FindTaus();
    virtual void   FindLeptons();
    virtual void   FindJets();
    virtual void   FindBJets();

  private:
    void SetJetN();
    void LooseLeptonOverlapRemoval();
    AtlCutFlowTool            *fCutFlowTool;
    
    ClassDef(AtlObjectsToolD3PDSgTop, 0)  // Atlas Objects Tool D3PD SgTop
};
#endif


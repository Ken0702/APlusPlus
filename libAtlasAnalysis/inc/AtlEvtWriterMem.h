//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2013 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlEvtWriterMem
#define ATLAS_AtlEvtWriterMem
#ifndef ROOT_AtlEvtWriterBase
#include <AtlEvtWriterBase.h>
#endif

class AtlBTag;

class AtlEvtWriterMem : public AtlEvtWriterBase {

  public:
    static const Int_t fgMaxElectrons = 10;
    static const Int_t fgMaxMuons     = 10;
    static const Int_t fgMaxJets      = 30;
    
  private:
    AtlBTag::ETagger fBTagger;            // Type of b-tagger to be used for writing the b-tag weights for each jet
    TList  *fElectrons;                   // List of selected elctron objects
    TList  *fMuons;                       // List of selected muon objects
    TList  *fJets;                        // List of selected jet objects
    TList  *fBJets;                       // List of selected b-jet objects
    Int_t   fRunNr;                       // Run number
    Int_t   fEventNr;                     // Event number
    Float_t fEventWeight;                 // Total event weight  
    Float_t fMET_Et;                      // MET Et (GeV)
    Float_t fMET_Phi;                     // MET Phi (rad)
    Int_t   fElectronN;                   // No. of electrons
    Float_t fElectronE[fgMaxElectrons];   // Electron E (GeV)
    Float_t fElectronPt[fgMaxElectrons];  // Electron Pt (GeV)
    Float_t fElectronEta[fgMaxElectrons]; // Electron Eta
    Float_t fElectronPhi[fgMaxElectrons]; // Electron Phi (rad)
    Int_t   fElectronChg[fgMaxElectrons]; // Electron charge (+1e/-1e)
    Int_t   fMuonN;                       // No. of muons
    Float_t fMuonE[fgMaxMuons];           // Muon E (GeV)
    Float_t fMuonPt[fgMaxMuons];          // Muon Pt (GeV)
    Float_t fMuonEta[fgMaxMuons];         // Muon Eta
    Float_t fMuonPhi[fgMaxMuons];         // Muon Phi (rad)
    Int_t   fMuonChg[fgMaxMuons];         // Muon charge (+1e/-1e)
    Int_t   fJetN;                        // No. of jets
    Float_t fJetE[fgMaxJets];             // Jet E (GeV)
    Float_t fJetPt[fgMaxJets];            // Jet Pt (GeV)
    Float_t fJetEta[fgMaxJets];           // Jet Eta
    Float_t fJetPhi[fgMaxJets];           // Jet Phi (rad)
    Float_t fJetBTagWeight[fgMaxJets];    // B-tag weight (defined by SetBTagger())
    Bool_t  fJetBTagged[fgMaxJets];       // Jet is b-tagged ?
    Char_t  fJetFlav[fgMaxJets];          // Jet truth flavour: l=light lfavour u,d,s; c=charm; b=beauty; t=tau; u=undefined (for DATA)
    
public:
    AtlEvtWriterMem();
    virtual ~AtlEvtWriterMem();
    virtual void BookTree(TTree *tree, AtlEvent *evt);
    virtual void WriteEvent();
    void SetElectrons(TList *electrons);
    void SetMuons(TList *muons);
    void SetJets(TList *jets, TList *bjets);
    void SetBTagger(AtlBTag::ETagger BTagger);
    
    ClassDef(AtlEvtWriterMem,0) // Event writer for MemNtuples
};
#endif


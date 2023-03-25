//
// Author: Michelangelo Giorgi <mailto: strider@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlWtChallenge
#define ATLAS_AtlWtChallenge
#ifndef ATLAS_AtlSelector
#include <AtlSelector.h>
#endif
#include <fstream>


class TString;
class TH1F;
class TH2F;

class AtlWtChallenge : public AtlSelector {
    
  private:
    std::ofstream fOut;
    
    TH1F *fHistCutFlow_e;  // Cut-flow, electrons
    TH1F *fHistCutFlow_mu; // Cut-flow, muons

    // Lepton candidates
    HepParticle *fLepton; // Signal lepton for effiency scale factor determination used for evt re-weighting
    TList *fLeptons;      // Merged list of signal leptons (e, mu) from Top Common Objects selection
    TList *fElectrons;    // Signal electron list from  Top Common Objects selection
    TList *fMuons;        // Signal muon list from  Top Common Objects selection
    TList *fVetoLeptons;  // Merged list of veto leptons (e, mu) from Top Common Objects selection

    // Generic jet and b-jet candidates
    TList *fJets;  // List of all jets from Top Common Objects selection
    TList *fBJets; // List of B-jets from Top Common Objects selection

    // Transverse W mass
    Double_t fWmassT;
    
    // Veto decisions
    Bool_t fVertexTracksPresent; // Enough vertex tracks present?
    Bool_t fBadJetsPresent;      // Bad jets search in Top Common Objects tool successful?
    Bool_t fElMuTrackMatched;    // Electron-muon track-match in Top Common Objects tool successful?

    
 public:
    enum ELeptonMode {
	kElectronMode = BIT(0), // electron channel
	kMuonMode     = BIT(1), // muon channel
    };
    
    
    // Mode
    ELeptonMode fLeptonMode;  // Indicate the lepton channel mode (e/mu)
    
    TString fEleTriggerChoice;
    TString fMuTriggerChoice;

    // Etmiss
    TVector2 fMET;              // MissingEt of current event
    Float_t  fMET_Mag_min;      // MissingEt cut
    Float_t  fElPt_Min;         // ElectronPt cut
    Float_t  fMuPt_Min;         // ElectronPt cut
    Float_t  fJets_Pt_min;      // JetPt cut
    Float_t  fJets_Eta_min;     // JetEta min cut
    Float_t  fJets_Eta_max;     // JetEta max cut
    Float_t  fBJets_Pt_min;      // JetPt cut
    Float_t  fBJets_Eta_min;    // BJetEta min cut
    Float_t  fBJets_Eta_max;    // BJetEta max cut

    Float_t  fWmassT_min;
    
    HepParticle *fRecoW;        // reconstructed W-boson

    
    AtlWtChallenge(const char* OutputFilename);
    virtual ~AtlWtChallenge();
    
    virtual void SetBranchStatus();
    virtual void SetCutDefaults();
    virtual void BookHistograms();
    virtual void InitEvent();
    virtual void FillHistograms();
    virtual Bool_t ProcessCut();
    virtual void Terminate();
    virtual void Print() const;
    void ReconstructW();
    
    ClassDef(AtlWtChallenge,2) // Single-top analysis validation
};
#endif


//  
// Author:  <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2017 (C) 
//
#ifndef ATLAS_AtlBDecayGenTool
#define ATLAS_AtlBDecayGenTool
#ifndef ATLAS_AtlAnalysisTool
#include <AtlAnalysisTool.h>
#endif
#ifndef ATLAS_AtlSelector
#include <AtlSelector.h>
#endif
#ifndef ATLAS_AtlTopLevelAnalysis
#include <AtlTopLevelAnalysis.h>
#endif
#ifndef HEP_HepParticle
#include <HepParticle.h>
#endif

class TH2D;
class TRandom;
class TList;

class AtlBDecayGenTool : public AtlAnalysisTool {

  public:
    static const Int_t fgNDSIDsMax = 256;
    enum EBJetMode {
	kRandom,     // Pick parent b-jets randomly
	kLowPt,      // Pick b-jet with lowest Pt as parent jet
	kHighPt,     // Pick b-jet with highest Pt as parent jet
	kHalfRandom, // Randomly fluctuate between kLowPt and kRandom
	kNBJetModes
    };
    
  public:
    const char*                      fValidDSIDs;            // Comma separated list of valid DSID numbers for which the tool will be enabled; for all other samples the won't run
    Bool_t                           fDoOverlapRemoval;      // Perform overlap removal of accompanying b-jet (default=true)
    Bool_t                           fRequireOverlapRemoval; // Reject event if overlap removal fails
    Float_t                          fORdeltaR_max;          // Maximum deltaR used for the overlap removal (default=0.4)
    EBJetMode                        fBJetMode;              // Define how to pick the parent b-jet in case of more than one b-jets (default=random)
    AtlTopLevelAnalysis::ESystematic fSystematicMode;        // Systematic mode (default = nominal)
    Float_t                          fSystDecayLength;       // Decay length used for shape variations of the fake lepton pT spectrum (default = 30)
    Bool_t                           fIsZjets;               // Perform changes in the pt-spectrum of the lepton (default=false)
    Bool_t                           fArtificialLeptons;               // Add artificial leptons (default=true)
    
  private:
    Int_t     fNDSIDs;            // Number of given valid DSIDs
    UInt_t    fDSID[fgNDSIDsMax]; // Array of valid DSID numbers
    Bool_t    fFirstEvt;          // First event ?
    TH2D     *fHistGen;           // Histogram for decay angle / energy generation
    TRandom  *fRandom;            // Random generator
    TList    *fJets;              // List of jets
    TList    *fBJets;             // List of b-jets
    TList    *fElectrons;         // List of electrons
    TList    *fMuons;             // List of muons
    TList    *fLeptons;           // List of leptons
    TH1F     *fHistDeltaR;        // Histogram of deltaR between the generated lepton and its parent jet
    TH1F     *fHistAlphaLoc;      // Histogram of alpha, local
    TH1F     *fHistPhiLoc;        // Histogram of phi, local
    TH1F     *fHistPzLoc;         // Histogram of Pz, local
    TH1F     *fHistPtLoc;         // Histogram of Pt, local
    TH1F     *fHistPmagLoc;       // Histogram of P magnitude, local
    TH1F     *fHistAlphaLab;      // Histogram of alpha, lab frame
    TH1F     *fHistEtaLab;        // Histogram of eta, lab frame
    TH1F     *fHistPhiLab;        // Histogram of phi, lab frame
    TH1F     *fHistPzLab;         // Histogram of Pz, lab frame
    TH1F     *fHistPtLab;         // Histogram of Pt, lab frame
    TH1F     *fHistPmagLab;       // Histogram of P magnitude, lab frame

  public:
    AtlBDecayGenTool(const char* name, const char* title);
    virtual ~AtlBDecayGenTool();

    const char* GetBJetModeName(AtlBDecayGenTool::EBJetMode BJetMode) const;
    
protected:
    virtual Bool_t AnalyzeEvent();
    virtual void   Init();
    virtual void   BookHistograms();
    virtual void   FillHistograms() {;}
    virtual void   SetBranchStatus();
    virtual void   SetCutDefaults();
    virtual void   Print() const;
    virtual void   Terminate();    
    
private:
    void SetDSIDArray();
    void CreateHistGen();
    HepParticle* GenerateBDecay(TLorentzVector p_mother, HepParticle *lep);
    Bool_t AddLeptons();
    
    ClassDef(AtlBDecayGenTool,0) // Tool for semileptonic B-meson decay generation
};
#endif


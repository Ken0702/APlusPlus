//
//    Author: Felix Thomas <mailto: Felix.Thomas@mailbox.tu-dresden.de>
//    Update: $Id$
//    Copyright: 2010 (C) Felix Thomas
//

#ifndef ATLAS_AtlTopPairFinder
#define ATLAS_AtlTopPairFinder
#ifndef ATLAS_AtlKinFitterTool
#include <AtlKinFitterTool.h>
#endif
#ifndef HEP_HepDatabasePDG
#include <HepDatabasePDG.h>
#endif
#ifndef ROOT_AtlSelector
#include <AtlSelector.h>
#endif
#ifndef ATLAS_AtlTopPairDocumenter
#include <AtlTopPairDocumenter.h>
#endif
#include <fstream>
#include <iostream>

class TH1F;
class TH2F;
class AtlEvent;
class TTree;
class TString;
class TROOT;
class TFile;

class AtlTopPairFinder : public AtlKinFitterTool {

    AtlTopPair* fBestTopPair;
    
    TList *fLeptons;                            // Merged list of electrons and muons
    TList *fJets;                               // List of light jets
    TList *fKinFitJets;                         // List of jets that is used by the kinematic fitter
    TList *fBJets;                              // List of b-Jets
    
    TVector2 fEtMiss;                                          
    Float_t  fEtMissMag;                         // Missing ET Magnitude
    Float_t  fEtMissPhi;                         // Missing ET Phi
    
    Int_t    fNTopPairCandidates;                // Number of Top Pair Candidates in Event
    Double_t fBestChi2Prob;                              
    // Objects that are used in the fitting procedure
    HepParticle *fLepton;   // Current lepton used as input for the fit
    HepParticle *fNeutrino; // Current neutrino (missing Et) used as input for the fit                                                                         
    AtlJet      *fLepBJet;  // Current b-jet candidate used as input for the fit
    AtlJet      *fHadBJet;  // Currend b-jet candidate used as input for the fit
    AtlJet      *fHadJet1;  // Current light jet candidate used as input for the fit
    AtlJet      *fHadJet2;  // Currend light jet candidate used as input for the fit

    TMatrixD cov_lep;
    TMatrixD cov_nu;
    TMatrixD cov_LepBJet;
    TMatrixD cov_HadJet1;
    TMatrixD cov_HadJet2;
    TMatrixD cov_HadBJet;


    // Pull matrices
    TMatrixD fLeptonpull;
    TMatrixD fNeutrinopull;
    TMatrixD fLepbjetpull; 
    TMatrixD fHadjet1pull; 
    TMatrixD fHadjet2pull; 
    TMatrixD fHadbjetpull;
    
    
  // Histograms

    AtlTopPairDocumenter *fAllTopPairDocumenter;
    AtlTopPairDocumenter *fElectronChannelTopPairDocumenter;
    AtlTopPairDocumenter *fMuonChannelTopPairDocumenter;

  // All decays
    TH1F *fHistNbIterAll;                                // Nr. of Iterations of all fits
    TH1F *fHistNbIterConv;                               // Nr. of Iterations of converging fits
    TH1F *fHistNTopPairs;                                // Nr. of top pairs satisfying the cut
    TH1F *fHistNTopPairCandidates;
    TH2F *fHistTopPairsVsCandidates;
    TH2F *fHistChi2ProbVsCandidates;
    TH2F *fHistChi2VsCandidates;
    TH2F *fHistSimpleChi2VsChi2;

 public:
 
    enum ETmassConstraint {
    kSameTmass,                                             // both tops have to have the same mass
    kFixedTmass                                             // top mass is fixed on value fTop_Mass
    };

    AtlTopPairFinder::ETmassConstraint  fTmassconstraint;   // specifies which constraint is used
    Double_t      fMaxChi2;		                            // Sets Max. Chi2 of fit
    Double_t      fMinChi2Prob;                             // Sets Min. Chi2Prob of fit
    
    // True W and top masses for the fit
    Double_t fW_Mass;     // Pole mass of the W used in  the fit
    Double_t fW_Width;    // Width of the W used in the fit
    Double_t fTop_Mass;   // Pole mass of the t-quark used in the fit
    Double_t fTop_Width;  // Width of the t-quark used in the fit


    // variables regarding the selection cuts    
    Bool_t   fQuietMode;

    // Switch for histogram filling
    Bool_t fAlwaysFillHistos; // if this is kTRUE, it also fills histograms if
                              // fParent->PassedSelection() == kFALSE or fQuietMode == kTRUE
    
    // BJet selection cuts   
    AtlBTag::ETagger fBJetTagger;           // Taggerinformation for b-jets
    Float_t          fBJetWeightMin;             // minimum weight to be a b-jet
    Int_t            fIterMax;
    Int_t	        fBJets_N_Min;                  // Minimum number of required b-jets
    Int_t            fBJets_N_Min_InHadCombo;  // Minimum Number of required b-jets in combo
    Int_t            fJets_N_Max;                    // Take care that the jet list is sorted. The fitter takes only the first fJets_N_Max number of jets from the fJets list.
    Bool_t           fApplyBTagInReco;        // Is b-tag information applied in the reconstruction?
    
    // Additional Cuts for Jets used in Reconstruction
    Bool_t fIsFwdElectronMode; // Use only forward electrons when analyzing jet-electrons? If set to kFALSE, only central electrons will be analyzed.
    
 public:

    AtlTopPairFinder(const char* name, const char* title);
    virtual ~AtlTopPairFinder();
    virtual void SetBranchStatus();
    virtual void BookHistograms();
    virtual void FillHistograms();
    virtual void SetCutDefaults();
    virtual void Clear();
    virtual void Print() const;
    virtual Bool_t AnalyzeEvent();
    virtual void Terminate();
    
    inline AtlTopPair* GetBestTopPair() { return fBestTopPair; }
    
    inline TList* GetKinFitJets() { return fKinFitJets; }
    inline Int_t GetJets_N_Max() { return fJets_N_Max; }
    inline void SetMinChi2Prob(Double_t value) { fMinChi2Prob = value; }
    inline void SetJets(TList* list) { fJets = list; }
    inline void SetKinFitJets(TList* list) { fKinFitJets = list; }
    inline void SetLeptons(TList* list) { fLeptons = list; }
    inline void SetEtmiss(TVector2 etmiss) { fEtMiss = etmiss; }
    inline void SetBJetTagger(AtlBTag::ETagger tagger) { fBJetTagger = tagger; }
    inline void SetBJetWeightMin(Float_t value) { fBJetWeightMin = value; }

 private:
    void    InitEvent();
    void    ReconstructionKinFit(TList* Leptons, TList* LepBJets, TList* HadJets1, TList* HadJets2, TList* HadBJets);
    void    DoFit();
    Double_t GetEtaNuStartingValue(TLorentzVector PBjet, TLorentzVector PLepton, TVector2 ETMiss);
    
    ClassDef(AtlTopPairFinder,0)  // Atlas Top Pair Finder
};
#endif

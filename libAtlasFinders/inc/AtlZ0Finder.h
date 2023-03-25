
//  
// Author: Holger Schulz <mailto:holger.schulz@physik.hu-berlin.de>, Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Holger Schulz, Oliver Maria Kind
//
#ifndef ATLAS_AtlZ0Finder
#define ATLAS_AtlZ0Finder
#ifndef ATLAS_AtlKinFitterTool
#include <AtlKinFitterTool.h>
#endif
#ifndef ROOT_AtlSelector
#include <AtlSelector.h>
#endif
#include <fstream>
#include <iostream>
#include <TH2F.h>

class TH1F;
class AtlEvent;
class TTree;
class TString;
class TROOT;
class TFile;

class AtlZ0Finder : public AtlKinFitterTool {

  public:
    enum EModeLepton {kElectron= BIT(0),
		      kMuon    = BIT(1),
		      kTau     = BIT(2),
		      kAll     = (kElectron | kMuon | kTau)
                     };
    
    
    Bool_t   fchargetest;
    EModeLepton fModeLepton;
  private:
    //Bool_t   fchargetest;
    //EModeLepton fModeLepton;
    // small resolution study
    // this is a preliminary analysis and can be deleted once there is access to covariance matrices
    // other histograms are used to analyze the performance of the fitter and are also not mandatory

    TH1F *fHist_Zee_PtRes;     // Histograms to compare reconstructed and true particles
    TH1F *fHist_Zee_EtaRes;
    TH1F *fHist_Zee_PhiRes;

    TH1F *fHist_Zee_PtRes_KinFit;     // Histograms to compare Kinfitter Output and true particles
    TH1F *fHist_Zee_EtaRes_KinFit;
    TH1F *fHist_Zee_PhiRes_KinFit;

    // histogram to compare chi^2 probability with the Bremsstrahlungsloss of electrons and positrons

    TH2F *fHist_Zee_BremslossProb;

    // histogram to compare the occurence of final state radiation in Z->ee
    // with chi^2 probability of the kinematic fit

    TH1F *fHist_Zee_RadProb;
    
    // histogram to compare  Z boson properties with chi^2 probability of the kinematic fit
    
    TH2F *fHist_Zee_ZPtProb;
    TH2F *fHist_Zee_ZEProb;
    TH2F *fHist_Zee_ZMProb;
    
    // histogram to test TFitConstraintEta
    // comparison of chi^2 probability and Eta of the electron
    
    TH1D *EtaHist;              // input histogram for constraint
    TH1D *fHist_Zee_AbsEtaElectron;       // histogram of |eta| of one Z boson daughter
                                // to be used as input for TFitConstraintEta
    TH2F *fHist_Prob_EtaElec;   // comparison of chi^2 probability and electron eta
    
    // histogram of electron+positron px_true to set pt-conservation constraint
    
    TH1F *fHist_e_px_true;
    
    Int_t TruthMatchCounter;
    
    Double_t c11; // Elements of Covariance Matrix
    Double_t c22;
    Double_t c33;
    Double_t c12;
    Double_t c13;
    Double_t c23;
    
    // Z -> ee (signal)
    TH1F *fHistZreco_m_ee;     // Reconstructed invariant transverse mass of the Z decaying into electrons
    TH1F *fHistZreco_pt_ee;    // Reconstructed azimuth angle of the (real) Z decaying into electrons
    TH1F *fHistZreco_phi_ee;   // Reconstructed Pt of the Z decaying into electrons
    TH1F *fHistZreco_eta_ee;   // Reconstructed invariant transverse mass of the Z decaying into e-
    TH1F *fHistZreco_n_ee;     // Reconstructed invariant transverse mass of the Z decaying into e-
    //with probability cut
    TH1F *fHistZreco_m_ee_with_prob;     // Reconstructed invariant transverse mass of the Z decaying into electrons
    TH1F *fHistZreco_pt_ee_with_prob;    // Reconstructed azimuth angle of the (real) Z decaying into electrons
    TH1F *fHistZreco_phi_ee_with_prob;   // Reconstructed Pt of the Z decaying into electrons
    TH1F *fHistZreco_eta_ee_with_prob;   // Reconstructed invariant transverse mass of the Z decaying into e-
    TH1F *fHistZreco_n_ee_with_prob;     // Reconstructed invariant transverse mass of the Z decaying into e-
    TH1F *fHistZreco_DeltaPhi_ee_with_prob;     // Reconstructed absolute Value of Phi(e+)-Phi(e-)
    
    TH1F *fHistZreco_eplus_pt;   // Reconstructed transverse momentum of the e+ from Z decaying into ee
    TH1F *fHistZreco_eplus_eta;  // Reconstructed pseudo rapidity of e+ from Z decaying into ee
    TH1F *fHistZreco_eplus_phi;  // Reconstructed azimuth angle of e+ from Z decaying into ee
    TH1F *fHistZreco_eminus_pt;  // Reconstructed transverse momentum of the e- from Z decaying into ee
    TH1F *fHistZreco_eminus_eta; // Reconstructed pseudo rapidity of e- from Z decaying into ee
    TH1F *fHistZreco_eminus_phi;  // Reconstructed azimuth angle of e- from Z decaying into ee
    TH1F *fHistZreco_DeltaPhi_ee;     // Reconstructed absolute Value of Phi(e+)-Phi(e-)
    TH1F *fHistZreco_prob_ee;   //Probability of reconstructed Z->ee
    // Z -> mumu (signal)
    TH1F *fHistZreco_m_mumu_with_prob;     // Reconstructed invariant transverse mass of the Z decaying into muons
    TH1F *fHistZreco_pt_mumu_with_prob;    // Reconstructed azimuth angle of the (real) Z decaying into muons
    TH1F *fHistZreco_phi_mumu_with_prob;   // Reconstructed Pt of the Z decaying into muons
    TH1F *fHistZreco_eta_mumu_with_prob;   // Reconstructed invariant transverse mass of the Z decaying into e-
    TH1F *fHistZreco_n_mumu_with_prob;     // Reconstructed invariant transverse mass of the Z decaying into e-
    TH1F *fHistZreco_prob_mumu;   //Probability of reconstructed Z->mumu
    //Histogramm after probability cut
    TH1F *fHistZreco_m_mumu;     // Reconstructed invariant transverse mass of the Z decaying into muons
    TH1F *fHistZreco_pt_mumu;    // Reconstructed azimuth angle of the (real) Z decaying into muons
    TH1F *fHistZreco_phi_mumu;   // Reconstructed Pt of the Z decaying into muons
    TH1F *fHistZreco_eta_mumu;   // Reconstructed invariant transverse mass of the Z decaying into e-
    TH1F *fHistZreco_n_mumu;     // Reconstructed invariant transverse mass of the Z decaying into e-
    TH1F *fHistZreco_DeltaPhi_mumu_with_prob;     // Reconstructed absolute Value of Phi(mu+)-Phi(mu-)
    
    
    TH1F *fHistZreco_muplus_pt;   // Reconstructed transverse momentum of the mu+ from Z decaying into mumu
    TH1F *fHistZreco_muplus_eta;  // Reconstructed pseudo rapidity of mu+ from Z decaying into mumu
    TH1F *fHistZreco_muplus_phi;  // Reconstructed azimuth angle of mu+ from Z decaying into mumu
    TH1F *fHistZreco_muminus_pt;  // Reconstructed transverse momentum of the mu- from Z decaying into mumu
    TH1F *fHistZreco_muminus_eta; // Reconstructed pseudo rapidity of mu- from Z decaying into mumu
    TH1F *fHistZreco_muminus_phi;  // Reconstructed azimuth angle of mu- from Z decaying into mumu
    TH1F *fHistZreco_DeltaPhi_mumu;     // Reconstructed absolute Value of Phi(mu+)-Phi(mu-)

    // Z -> tautau (signal)
    TH1F *fHistZreco_m_tautau;     // Reconstructed invariant transverse mass of the Z decaying into taus
    TH1F *fHistZreco_pt_tautau;    // Reconstructed azimuth angle of the (real) Z decaying into taus
    TH1F *fHistZreco_phi_tautau;   // Reconstructed Pt of the Z decaying into taus
    TH1F *fHistZreco_eta_tautau;   // Reconstructed invariant transverse mass of the Z decaying into e-
    TH1F *fHistZreco_n_tautau;     // Reconstructed invariant transverse mass of the Z decaying into e-
    TH1F *fHistZreco_DeltaPhi_tautau;     // Reconstructed absolute Value of Phi(tau+)-Phi(tau-)
    
    
    // Z -> leptons (signal)
    TH1F *fHistZreco_m_ll;     // Reconstructed invariant transverse mass of the Z decaying into leptons
    TH1F *fHistZreco_pt_ll;    // Reconstructed azimuth angle of the (real) Z decaying into leptons
    TH1F *fHistZreco_phi_ll;   // Reconstructed Pt of the Z decaying into leptons
    TH1F *fHistZreco_eta_ll;   // Reconstructed invariant transverse mass of the Z decaying into e-
    TH1F *fHistZreco_n_ll;     // Reconstructed invariant transverse mass of the Z decaying into e-
    TH1F *fHistZreco_DeltaPhi_ll;     // Reconstructed absolute Value of Phi(l+)-Phi(l-)
    
    // Z -> ee (bkg)
    TH1F *fHistZreco_m_ee_bkg;        // Reconstructed invariant transverse mass of the Z decaying into electrons (bkg)
    TH1F *fHistZreco_pt_ee_bkg;       // Reconstructed azimuth angle of the (real) Z decaying into electrons (bkg)
    TH1F *fHistZreco_phi_ee_bkg;      // Reconstructed Pt of the Z decaying into electrons (bkg)
    TH1F *fHistZreco_eta_ee_bkg;      // Reconstructed invariant transverse mass of the Z decaying into e- (bkg)
    TH1F *fHistZreco_DeltaPhi_ee_bkg; // Reconstructed absolute Value of Phi(e+)-Phi(e-) (bkg)

    // Z -> mumu (bkg)
    TH1F *fHistZreco_m_mumu_bkg;     // Reconstructed invariant transverse mass of the Z decaying into muons (bkg)
    TH1F *fHistZreco_pt_mumu_bkg;    // Reconstructed azimuth angle of the (real) Z decaying into muons (bkg)
    TH1F *fHistZreco_phi_mumu_bkg;   // Reconstructed Pt of the Z decaying into muons (bkg)
    TH1F *fHistZreco_eta_mumu_bkg;   // Reconstructed invariant transverse mass of the Z decaying into e- (bkg)
    TH1F *fHistZreco_DeltaPhi_mumu_bkg;// Reconstructed absolute Value of Phi(mu+)-Phi(mu-) (bkg)
    
    // Z -> tautau (bkg)
    TH1F *fHistZreco_m_tautau_bkg;     // Reconstructed invariant transverse mass of the Z decaying into taus (bkg)
    TH1F *fHistZreco_pt_tautau_bkg;    // Reconstructed azimuth angle of the (real) Z decaying into taus (bkg)
    TH1F *fHistZreco_phi_tautau_bkg;   // Reconstructed Pt of the Z decaying into taus (bkg)
    TH1F *fHistZreco_eta_tautau_bkg;   // Reconstructed invariant transverse mass of the Z decaying into e- (bkg)
    TH1F *fHistZreco_DeltaPhi_tautau_bkg;// Reconstructed absolute Value of Phi(tau+)-Phi(tau-) (bkg)
    
    // Z -> leptons (bkg)
    TH1F *fHistZreco_m_ll_bkg;     // Reconstructed invariant transverse mass of the Z decaying into leptons (bkg)
    TH1F *fHistZreco_pt_ll_bkg;    // Reconstructed azimuth angle of the (real) Z decaying into leptons (bkg)
    TH1F *fHistZreco_phi_ll_bkg;   // Reconstructed Pt of the Z decaying into leptons (bkg)
    TH1F *fHistZreco_eta_ll_bkg;   // Reconstructed invariant transverse mass of the Z decaying into e- (bkg)
    TH1F *fHistZreco_DeltaPhi_ll_bkg;// Reconstructed absolute Value of Phi(l+)-Phi(l-) (bkg)
    
    TList *fSignalElectrons;   // List of tight e from Z decay
    TList *fSignalMuons;       // List of tight mu from Z decay
    TList *fSignalTaus;        // List of tight tau from Z decay
    TList *fBkgZ0Decays;       // List of combinatorial bkg.
    
  public:
    // Lepton selection cuts
    AtlEMShower::EAuthor fSignalElectronAuthor;   // Author type of signal electrons
    AtlEMShower::EIsEM   fSignalElectronIsEM;     // IsEM type of signal electrons
    Float_t              fSignalElectron_Pt_min;  // Min. Pt of signal electrons
    Float_t              fSignalElectron_Pt_max;  // Max. Pt of signal electrons
    Float_t              fSignalElectron_Eta_min; // Min. eta of signal electrons
    Float_t              fSignalElectron_Eta_max; // Max. eta of signal electrons
    Float_t              fSignalElectron_EtCone20_max; // Max. Et within a cone of 0.2
    
    AtlMuon::EAuthor     fSignalMuonAuthor;       // Author type of signal muons
    Float_t              fSignalMuon_Pt_min;      // Min. Pt of signal muons
    Float_t              fSignalMuon_Pt_max;      // Max. Pt of signal muons
    Float_t              fSignalMuon_Eta_min;     // Min. eta of signal muons
    Float_t              fSignalMuon_Eta_max;     // Max. eta of signal muons
    Float_t              fSignalMuon_Chi2_max;    // Max. chi2/ndof of signal muons
    Float_t              fSignalMuon_EtCone20_max; // Max. Et within a cone of 0.2
    
    AtlTau::EAuthor      fSignalTauAuthor;       // Author type of signal taus
    AtlTau::ETauFlag     fSignalTauAuthorFlag;   // AuthorFlag type of signal taus
    Float_t              fSignalTau_Pt_min;      // Min. Pt of signal taus
    Float_t              fSignalTau_Pt_max;      // Max. Pt of signal taus
    Float_t              fSignalTau_Eta_min;     // Min. eta of signal taus
    Float_t              fSignalTau_Eta_max;     // Max. eta of signal taus
    //Float_t              fSignalTau_Chi2_max;    // Max. chi2/ndof of signal taus
    //Float_t              fSignalTau_EtCone20_max; // Max. Et within a cone of 0.2
    
    Float_t              fZBoson_M_min; // Lower boundary for allowed Z mass range
    Float_t              fZBoson_M_max; // Upper boundary for allowed Z mass range
    
  public:
    AtlZ0Finder(const char* name, const char* title, Bool_t chargetest=kTRUE, EModeLepton fModeLepton=kAll);
    virtual ~AtlZ0Finder();
    virtual void SetBranchStatus();
    virtual void BookHistograms();
    virtual void FillHistograms();
    virtual void SetCutDefaults();
    virtual Bool_t AnalyzeEvent();
    virtual void Terminate();
    void InitEvent();
    void ReconstructZ0CutBased(TList *SignalLeptons);
    void ReconstructZ0KinFit(TList *SignalLeptons);
    void FillTruthHistograms();
    inline void SetMode(EMode mode = kKinFit)
    { fMode = mode; }
    ClassDef(AtlZ0Finder,0) // Z0 decay finder class
};
#endif


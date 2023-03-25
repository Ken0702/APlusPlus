// Author: Patrick Rieck <mailto:rieck@physik.hu-berlin.de>
// Copyright: 2009 (C) Patrick Rieck
//
#ifndef ATLAS_AtlSgTop_WtChannelFinder
#define ATLAS_AtlSgTop_WtChannelFinder
#ifndef ATLAS_AtlKinFitterTool
#include <AtlKinFitterTool.h>
#endif
#ifndef HEP_HepDatabasePDG
#include <HepDatabasePDG.h>
#endif
#ifndef ROOT_AtlSelector
#include <AtlSelector.h>
#endif
#ifndef TFITCONSTRAINTETA_H
#include <TFitConstraintEta.h>
#endif
#ifndef ATLAS_AtlCutFlowTool
#include <AtlCutFlowTool.h>
#endif

#include <fstream>
#include <iostream>
#include <TH2F.h>

//____________________________________________________________________

class TH1F;
class AtlEvent;
class TTree;
class TString;
class TROOT;
class TFile;
class TBranch;
//____________________________________________________________________

class AtlSgTop_WtChannelFinder : public AtlKinFitterTool {

  public:

    enum EModeMass { kGauss, kBW };
    enum EMode     { kAll, kKinFit, kCutBased };
    enum ELeptonMode {
	kElectronMode = BIT(0), // Search for W -> e nu_e events
	kMuonMode     = BIT(1), // Search for W -> mu nu_mu events
	kTauMode      = BIT(2), // Search for W -> tau nu_tau events
	kLeptonAll    = ( kElectronMode | kMuonMode ) // e or mu channel only (at present)
    };

  private:

    std::ofstream fOut; // write text file with event info for acceptance challenges
    AtlCutFlowTool         *fCutflow_tool;
    AtlCutFlowTool         *fCutflow_tool_AC;

    TKinFitter *fKinFitter_HadronicTop; // KinFitter object for reco of hadron. top

    TList *fLeptons;        // Merged list of electrons and muons
    TList *fJets;           // List of all jets
    TList *fWhadJets;       // List of jets for hadronic W reconstruction
    TList *fBJets;          // List of b-jets used for reconstruction

    Int_t fNJets;           // number of jets in generic list

    TVector2 fMET;          // MissingEt of current event
    Float_t  fMET_Mag;      // Magnitude of MissingEt of current event
    Float_t  fMET_Phi;      // Phi of MissingEt of current event

    HepParticle *fLepton;           // Current lepton used as input for the fit

    HepParticle *fNeutrino;         // Current neutrino (missing Et) used as input for the fit

    AtlJet      *fBJet;             // Current b-jet candidate used as input for the fit
    AtlJet      *fBJet_had;             // Current b-jet candidate used as input for the fit
    AtlJet      *fBestLeptonicBJet; // b-jet candidate used in best KinFitter reconstruction,
                                    // leptonic top decay
    AtlJet      *fBestHadronicBJet; // b-jet candidate used in best KinFitter reconstruction,
                                    // hadronic top decay
    AtlJet      *fJet1;             // 1st Jet candidate from W boson
    AtlJet      *fJet2;             // 2nd Jet candidate from W boson
    AtlJet      *fJet1_had;             // 1st Jet candidate from W boson
    AtlJet      *fJet2_had;             // 2nd Jet candidate from W boson

    AtlJet      *fBestLeptonicJet1; // 1st jet used in best KinFitter reconstruction,
                                    // leptonic top decay
    AtlJet      *fBestLeptonicJet2; // 2nd jet used in best KinFitter reconstruction,
                                    // leptonic top decay
    AtlJet      *fBestHadronicJet1; // 1st jet used in best KinFitter reconstruction,
                                    // hadronic top decay
    AtlJet      *fBestHadronicJet2; // 2nd jet used in best KinFitter reconstruction,
                                    // hadronic top decay

    TMatrixD fCovLepton;    // Covariance matrix of the charged lepton from the W decay
    TMatrixD fCovNeutrino;  // Covariance matrix of the neutrino from the W decay
    TMatrixD fCovBJet;      // Covariance matrix of the b-jet candidate from the t decay
    TMatrixD fCovWJet1;     // Covariance matrix of the 1st jet candidate from the W hadronic decay
    TMatrixD fCovWJet2;     // Covariance matrix of the 2nd jet candidate from the W hadronic decay

    TMatrixD fCovLepton_had;    // Covariance matrix of the charged lepton from the W decay
    //TMatrixD fCovNeutrino_had;  // Covariance matrix of the neutrino from the W decay
    TMatrixD fCovBJet_had;      // Covariance matrix of the b-jet candidate from the t decay
    TMatrixD fCovWJet1_had;     // Covariance matrix of the 1st jet candidate from the W hadronic decay
    TMatrixD fCovWJet2_had;     // Covariance matrix of the 2nd jet candidate from the W hadronic decay

    // true particles to be compared with reconstruction
    // so far only looking into t->Wb
    HepMCParticle *fTrueChargedLepton;
    HepMCParticle *fTrueNeutrino;
    HepMCParticle *fTrueBQuark;
    HepMCParticle *fTrueWQuark1;
    HepMCParticle *fTrueWQuark2;

    TLorentzVector *fNeutrinoLeptonicKinFit; // momentum 4 vector of reco. KinFitter Neutrino
    TLorentzVector *fLeptonLeptonicKinFit;   // momentum 4 vector of reco. KinFitter Charged lepton
    TLorentzVector *fBJetLeptonicKinFit;     // momentum 4 vector of reco. KinFitter b-jet,
                                             // leptonic top decay
    TLorentzVector *fJet1LeptonicKinFit;     // momentum 4 vector of reco. KinFitter jet1,
                                             // leptonic top decay
    TLorentzVector *fJet2LeptonicKinFit;     // momentum 4 vector of reco. KinFitter jet2,

    TLorentzVector *fNeutrinoHadronicKinFit; // momentum 4 vector of reco. KinFitter Neutrino
    TLorentzVector *fLeptonHadronicKinFit;   // momentum 4 vector of reco. KinFitter Charged lepton
                                              // leptonic top decay
    TLorentzVector *fBJetHadronicKinFit;     // momentum 4 vector of reco. KinFitter b-jet,
                                             // hadronic top decay
    TLorentzVector *fJet1HadronicKinFit;     // momentum 4 vector of reco. KinFitter jet1,
                                             // hadronic top decay
    TLorentzVector *fJet2HadronicKinFit;     // momentum 4 vector of reco. KinFitter jet2,

    Double_t fChi2Whad;          // Chi square of hadronic W reconstruction
                                 // done in ApplyTTbarVeto()
    Int_t    fNDoFWhad;          // No. of D.o.F. of hadronic W kinematic fit

    Int_t fN_Converged_lept;          // No. of fits that DID converge in this event
    Int_t fN_NotConverged_lept;       // No. of fits that did not converge in this event
    Int_t fN_FailNumeric_lept;        // No. of fits that failed numerically in this event
    Int_t fN_Converged_hadr;          // No. of fits that DID converge in this event
    Int_t fN_NotConverged_hadr;       // No. of fits that did not converge in this event
    Int_t fN_FailNumeric_hadr;        // No. of fits that failed numerically in this event

    Int_t fN_WorseTopCandidates; // No. of top-quark candidates with higher
                                 // Chi2 than chosen one in this event
    // mass term in chi square function
    // for Breit Wigner constraints:
    //     mu_top^2+mu_W^2, mu are the transformed masses
    // for Gauss constraints:
    // [(alpha_top-1)/(Gamma_Top/M_Top)]^2+[Top->W],
    // alpha are the additional variables used appearing in the constraint equations
    Float_t fMassConstraintParameterAll;   // mass term
    Float_t fMassConstraintParameterTop;   // mass term of top quark
    Float_t fMassConstraintParameterWtop;     // mass term of W boson
    Float_t fMassConstraintParameterWnoTop;     // mass term of W boson

    Float_t fPullLeptonPt;   // Pull of lepton Pt
    Float_t fPullLeptonEta;  // Pull of lepton eta
    Float_t fPullLeptonPhi;  // Pull of lepton phi
    Float_t fPullEtMissMag;  // Pull of missing Et magnitude
    Float_t fPullEtMissPhi;  // Pull of missing Et phi
    Float_t fPullBJetPt;     // Pull of Bjet Pt
    Float_t fPullBJetEta;    // Pull of Bjet eta
    Float_t fPullBJetPhi;    // Pull of Bjet phi
    Float_t fPullJet1Pt;     // Pull of jet1 Pt
    Float_t fPullJet1Eta;    // Pull of jet1 eta
    Float_t fPullJet1Phi;    // Pull of jet1 phi
    Float_t fPullJet2Pt;     // Pull of jet2 Pt
    Float_t fPullJet2Eta;    // Pull of jet2 eta
    Float_t fPullJet2Phi;    // Pull of jet2 phi
    Float_t fPullLeptonPt_had;   // Pull of lepton Pt
    Float_t fPullLeptonEta_had;  // Pull of lepton eta
    Float_t fPullLeptonPhi_had;  // Pull of lepton phi
    Float_t fPullEtMissMag_had;  // Pull of missing Et magnitude
    Float_t fPullEtMissPhi_had;  // Pull of missing Et phi
    Float_t fPullBJetPt_had;     // Pull of Bjet Pt
    Float_t fPullBJetEta_had;    // Pull of Bjet eta
    Float_t fPullBJetPhi_had;    // Pull of Bjet phi
    Float_t fPullJet1Pt_had;     // Pull of jet1 Pt
    Float_t fPullJet1Eta_had;    // Pull of jet1 eta
    Float_t fPullJet1Phi_had;    // Pull of jet1 phi
    Float_t fPullJet2Pt_had;     // Pull of jet2 Pt
    Float_t fPullJet2Eta_had;    // Pull of jet2 eta
    Float_t fPullJet2Phi_had;    // Pull of jet2 phi

    Float_t fResidualLeptonPt;   // Residual of lepton Pt
    Float_t fResidualLeptonEta;  // Residual of lepton eta
    Float_t fResidualLeptonPhi;  // Residual of lepton phi
    Float_t fResidualEtMissMag;  // Residual of missing Et magnitude
    Float_t fResidualEtMissPhi;  // Residual of missing Et phi
    Float_t fResidualBJetPt;     // Residual of Bjet Pt
    Float_t fResidualBJetEta;    // Residual of Bjet eta
    Float_t fResidualBJetPhi;    // Residual of Bjet phi
    Float_t fResidualJet1Pt;     // Residual of jet1 Pt
    Float_t fResidualJet1Eta;    // Residual of jet1 eta
    Float_t fResidualJet1Phi;    // Residual of jet1 phi
    Float_t fResidualJet2Pt;     // Residual of jet2 Pt
    Float_t fResidualJet2Eta;    // Residual of jet2 eta
    Float_t fResidualJet2Phi;    // Residual of jet2 phi

    Float_t fBeforeLeptonPt;   // Before of lepton Pt
    Float_t fBeforeLeptonEta;  // Before of lepton eta
    Float_t fBeforeLeptonPhi;  // Before of lepton phi
    Float_t fBeforeEtMissMag;  // Before of missing Et magnitude
    Float_t fBeforeEtMissEta;
    Float_t fBeforeEtMissPhi;  // Before of missing Et phi
    Float_t fBeforeBJetPt;     // Before of Bjet Pt
    Float_t fBeforeBJetEta;    // Before of Bjet eta
    Float_t fBeforeBJetPhi;    // Before of Bjet phi
    Float_t fBeforeJet1Pt;     // Before of jet1 Pt
    Float_t fBeforeJet1Eta;    // Before of jet1 eta
    Float_t fBeforeJet1Phi;    // Before of jet1 phi
    Float_t fBeforeJet2Pt;     // Before of jet2 Pt
    Float_t fBeforeJet2Eta;    // Before of jet2 eta
    Float_t fBeforeJet2Phi;    // Before of jet2 phi
    Float_t fAfterLeptonPt;   // After of lepton Pt
    Float_t fAfterLeptonEta;  // After of lepton eta
    Float_t fAfterLeptonPhi;  // After of lepton phi
    Float_t fAfterEtMissMag;  // After of missing Et magnitude
    Float_t fAfterEtMissEta;
    Float_t fAfterEtMissPhi;  // After of missing Et phi
    Float_t fAfterBJetPt;     // After of Bjet Pt
    Float_t fAfterBJetEta;    // After of Bjet eta
    Float_t fAfterBJetPhi;    // After of Bjet phi
    Float_t fAfterJet1Pt;     // After of jet1 Pt
    Float_t fAfterJet1Eta;    // After of jet1 eta
    Float_t fAfterJet1Phi;    // After of jet1 phi
    Float_t fAfterJet2Pt;     // After of jet2 Pt
    Float_t fAfterJet2Eta;    // After of jet2 eta
    Float_t fAfterJet2Phi;    // Before of jet2 phi

    Float_t fResidualLeptonPt_hadr;   // Residual of lepton Pt
    Float_t fResidualLeptonEta_hadr;  // Residual of lepton eta
    Float_t fResidualLeptonPhi_hadr;  // Residual of lepton phi
    Float_t fResidualEtMissMag_hadr;  // Residual of missing Et magnitude
    Float_t fResidualEtMissPhi_hadr;  // Residual of missing Et phi
    Float_t fResidualBJetPt_hadr;     // Residual of Bjet Pt
    Float_t fResidualBJetEta_hadr;    // Residual of Bjet eta
    Float_t fResidualBJetPhi_hadr;    // Residual of Bjet phi
    Float_t fResidualJet1Pt_hadr;     // Residual of jet1 Pt
    Float_t fResidualJet1Eta_hadr;    // Residual of jet1 eta
    Float_t fResidualJet1Phi_hadr;    // Residual of jet1 phi
    Float_t fResidualJet2Pt_hadr;     // Residual of jet2 Pt
    Float_t fResidualJet2Eta_hadr;    // Residual of jet2 eta
    Float_t fResidualJet2Phi_hadr;    // Residual of jet2 phi

    Float_t fBeforeLeptonPt_hadr;   // Before of lepton Pt
    Float_t fBeforeLeptonEta_hadr;  // Before of lepton eta
    Float_t fBeforeLeptonPhi_hadr;  // Before of lepton phi
    Float_t fBeforeEtMissMag_hadr;  // Before of missing Et magnitude
    Float_t fBeforeEtMissPhi_hadr;  // Before of missing Et phi
    Float_t fBeforeBJetPt_hadr;     // Before of Bjet Pt
    Float_t fBeforeBJetEta_hadr;    // Before of Bjet eta
    Float_t fBeforeBJetPhi_hadr;    // Before of Bjet phi
    Float_t fBeforeJet1Pt_hadr;     // Before of jet1 Pt
    Float_t fBeforeJet1Eta_hadr;    // Before of jet1 eta
    Float_t fBeforeJet1Phi_hadr;    // Before of jet1 phi
    Float_t fBeforeJet2Pt_hadr;     // Before of jet2 Pt
    Float_t fBeforeJet2Eta_hadr;    // Before of jet2 eta
    Float_t fBeforeJet2Phi_hadr;    // Before of jet2 phi
    Float_t fAfterLeptonPt_hadr;   // After of lepton Pt
    Float_t fAfterLeptonEta_hadr;  // After of lepton eta
    Float_t fAfterLeptonPhi_hadr;  // After of lepton phi
    Float_t fAfterEtMissMag_hadr;  // After of missing Et magnitude
    Float_t fAfterEtMissPhi_hadr;  // After of missing Et phi
    Float_t fAfterBJetPt_hadr;     // After of Bjet Pt
    Float_t fAfterBJetEta_hadr;    // After of Bjet eta
    Float_t fAfterBJetPhi_hadr;    // After of Bjet phi
    Float_t fAfterJet1Pt_hadr;     // After of jet1 Pt
    Float_t fAfterJet1Eta_hadr;    // After of jet1 eta
    Float_t fAfterJet1Phi_hadr;    // After of jet1 phi
    Float_t fAfterJet2Pt_hadr;     // After of jet2 Pt
    Float_t fAfterJet2Eta_hadr;    // After of jet2 eta
    Float_t fAfterJet2Phi_hadr;    // Before of jet2 phi

    static const char* fgStatusNames[]; // Status strings from KinFitter object

    // kinematic quantities of kinematic fit "ingredients" before and after the fit

    TTree *fCommonTree;

    // WtFinder performance histograms
    TH1F *fHist_KinFit_NbIterLeptonicTop;
    TH1F *fHist_KinFit_KinFitterStatus_LeptonicTop;
    TH1F *fHist_KinFit_NbFitsConverged_LeptonicTop;  // Number of fits converged
    TH1F *fHist_KinFit_NbFitsNotConverged_LeptonicTop;  // Number of fits not converged
    TH1F *fHist_KinFit_NbFitsFailedNumerically_LeptonicTop;// Number of fits failing numerically (1 or more matrix inversions fail)

    TH1F *fHist_KinFit_NbIterHadronicTop;
    TH1F *fHist_KinFit_KinFitterStatus_HadronicTop;
    TH1F *fHist_KinFit_NbFitsConverged_HadronicTop;  // Number of fits converged
    TH1F *fHist_KinFit_NbFitsNotConverged_HadronicTop;  // Number of fits not converged
    TH1F *fHist_KinFit_NbFitsFailedNumerically_HadronicTop;// Number of fits failing numerically (1 or more matrix inversions fail)

    TH2F *fHist_PValue_LeptHadrComp;
    TH2F *fHist_Chi2_LeptHadrComp;
    TH1F *fHistPerformanceComparison_before;
    TH1F *fHistPerformanceComparison_after;
    TH2F *fHist_W_hadr_top_reco_comp_MChi2;
    TH2F *fHist_W_hadr_top_reco_comp_MPChi2;
    TH2F *fHist_t_hadr_top_reco_comp_MChi2;
    TH2F *fHist_t_hadr_top_reco_comp_MPChi2;
    TH2F *fHist_W_hadr_top_reco_comp_MChi2_after;
    TH2F *fHist_W_hadr_top_reco_comp_MPChi2_after;
    TH2F *fHist_t_hadr_top_reco_comp_MChi2_after;
    TH2F *fHist_t_hadr_top_reco_comp_MPChi2_after;

    TH2F *fHist_W_hadr_top_reco_comp_Mptbjet;
    TH2F *fHist_W_hadr_top_reco_comp_Metabjet;
    TH2F *fHist_W_hadr_top_reco_comp_Mptjet1;
    TH2F *fHist_W_hadr_top_reco_comp_Metajet1;
    TH2F *fHist_W_hadr_top_reco_comp_Mptjet2;
    TH2F *fHist_W_hadr_top_reco_comp_Metajet2;

    TH2F *fHist_W_hadr_top_reco_comp_chi2ptbjet;
    TH2F *fHist_W_hadr_top_reco_comp_chi2etabjet;
    TH2F *fHist_W_hadr_top_reco_comp_chi2ptjet1;
    TH2F *fHist_W_hadr_top_reco_comp_chi2etajet1;
    TH2F *fHist_W_hadr_top_reco_comp_chi2ptjet2;
    TH2F *fHist_W_hadr_top_reco_comp_chi2etajet2;

    // histograms showing Wt system pt vs top eta
    TH2F *fHistWtSystem_vs_TopEta_sliced_fine;      // 2D fine binning
    TH2F *fHistWtSystem_vs_TopEta_sliced;           // 2D coarse binning
    TH1F *fHistWtSystem_rolled_TopEta_sliced_fine;  // rolled wrt top eta - fine binning
    TH1F *fHistWtSystem_rolled_TopEta_sliced;       // rolled wrt top eta - coarse binning
    TH2F *fHistWtSystem_vs_TopEta_sliced_fine_more;      // 2D fine binning
    TH2F *fHistWtSystem_vs_TopEta_sliced_more;           // 2D coarse binning
    TH1F *fHistWtSystem_rolled_TopEta_sliced_fine_more;  // rolled wrt top eta - fine binning
    TH1F *fHistWtSystem_rolled_TopEta_sliced_more;       // rolled wrt top eta - coarse binning

    TH2F *fHistWtSystem_vs_TopEta_sliced_less;           // 2D coarse binning
    TH1F *fHistWtSystem_rolled_TopEta_sliced_less;       // rolled wrt top eta - coarse binning

    Int_t nbins_pt_fine;
    Int_t nbins_pt_coarse;
    Int_t n_eta_slices;
    Int_t n_eta_slices_more;

    TH2F *fHist_comp_MM;
    TH2F *fHist_comp_MM_after;
    // Et Miss histograms

    TH1F *fHistEtMissMag;
    TH1F *fHistEtMissPhi;
    TH1F *fHistSumEt;

    TH1F *fHistEtMissMag_pretag;  // EtMiss pretag
    TH1F *fHistEtMissPhi_pretag;  // EtMiss Phi pretag
    TH1F *fHistEtMissMag_tag;     // EtMiss tag
    TH1F *fHistEtMissPhi_tag;     // EtMiss Phi tag
    TH1F *fHistSumEt_pretag;      // SumEt pretag
    TH1F *fHistSumEt_tag;         // SumEt tagH
    TH1F *fHistTotalYield_allCuts; // Total weighted yield after all cuts
    TH1F *fHistTotalYield_p10WtPt;
    TH1F *fHistTotalYield_allCuts_cut20; // Total weighted yield after all cuts
    TH1F *fHistTotalYield_allCuts_cut30; // Total weighted yield after all cuts
    TH1F *fHistTotalYield_allCuts_cut40; // Total weighted yield after all cuts
    TH1F *fHistTotalYield_allCuts_cut50; // Total weighted yield after all cuts
    TH1F *fHistTotalYield_allCuts_cut60; // Total weighted yield after all cuts

    TH1F *fHistCountLepPlus_pretag; // Pretag count positive leptons
    TH1F *fHistCountLepMinus_pretag; // Pretag count negative leptons
    TH1F *fHistCountLepAll_tag; // Tag count (all leptons)
    TH1F *fHistCountLepPlus_tag; // Tag count positive leptons
    TH1F *fHistCountLepMinus_tag; // Tag count negative leptons
    TH1F *fHistCountLepAll_tag_refQCD; // Tag count (all leptons), reference for QCD norm.
    TH1F *fHistCountLepPlus_tag_refQCD; // Tag count (positive leptons), reference for QCD norm.
    TH1F *fHistCountLepMinus_tag_refQCD; // Tag count (negative leptons), reference for QCD norm.
    TH1I *fHist_HFOR_flag_pretag; // top_hfor_flag for use by wjets flavour reweighting tool

    TH1F *fHistCountFwdElectrons_pretag;// no. of events (pretag) for fwd electrons
    TH1F *fHistCountCentralElectrons_pretag;// no. of events (pretag) for central electrons
    TH1F *fHistCountFwdElectrons_tag;// no. of events (tag) for fwd electrons
    TH1F *fHistCountCentralElectrons_tag;// no. of events (tag) for central electrons
    TH1F *fHistCountFwdElectrons_WCtrl;// no. of events (W control region) for fwd electrons
    TH1F *fHistCountCentralElectrons_WCtrl;// no. of events (W control region) for central electrons

    TH1F *fHistW_Mt_QCDValidation_pretag; // transverse mass lept-etmiss for qcd validation plot - pretag
    TH1F *fHistW_Mt_QCDValidation_tag;    // transverse mass lept-etmiss for qcd validation plot - tag
    // Signal lepton histograms

    TH1F *fHistSignalLepton_pt;    // Pt of leading signal lepton
    TH1F *fHistSignalLepton_eta;   // Eta of leading signal lepton
    TH1F *fHistSignalLepton_eta_cells;
    TH1F *fHistSignalLepton_phi;   // Phi of leading signal lepton

    TH1F *fHistSignalElectron_author; // Author of signal electron
    TH1F *fHistSignalElectron_pt;  // Pt of leading signal electron
    TH1F *fHistSignalElectron_eta; // Eta of leading signal electron
    TH1F *fHistSignalElectron_phi; // Phi of leading signal electron
    TH1F *fHistSignalEplus_pt;     // Pt of leading signal e+
    TH1F *fHistSignalEplus_eta;    // Eta of leading signal e+
    TH1F *fHistSignalEplus_phi;    // Phi of leading signal e+
    TH1F *fHistSignalEminus_pt;    // Pt of leading signal e-
    TH1F *fHistSignalEminus_eta;   // Eta of leading signal e-
    TH1F *fHistSignalEminus_phi;   // Phi of leading signal e-

    TH1F *fHistSignalMuon_pt;      // Pt of leading signal muon
    TH1F *fHistSignalMuon_eta;     // Eta of leading signal muon
    TH1F *fHistSignalMuon_phi;     // Phi of leading signal muon
    TH1F *fHistSignalMuplus_pt;    // Pt of leading signal mu+
    TH1F *fHistSignalMuplus_eta;   // Eta of leading signal mu+
    TH1F *fHistSignalMuplus_phi;   // Phi of leading signal mu+
    TH1F *fHistSignalMuminus_pt;   // Pt of leading signal mu-
    TH1F *fHistSignalMuminus_eta;  // Eta of leading signal mu-
    TH1F *fHistSignalMuminus_phi;  // Phi of leading signal mu-

    TH1F *fHistSignalTau_pt;       // Pt of leading signal tau
    TH1F *fHistSignalTau_eta;      // Eta of leading signal tau
    TH1F *fHistSignalTau_phi;      // Phi of leading signal tau
    TH1F *fHistSignalTauplus_pt;   // Pt of leading signal tau+
    TH1F *fHistSignalTauplus_eta;  // Eta of leading signal tau+
    TH1F *fHistSignalTauplus_phi;  // Phi of leading signal tau+
    TH1F *fHistSignalTauminus_pt;  // Pt of leading signal tau-
    TH1F *fHistSignalTauminus_eta; // Eta of leading signal tau-
    TH1F *fHistSignalTauminus_phi; // Phi of leading signal tau-

    // Before/after fit comparison histograms
    TH1F *fHistSignalLeptonPt_before_lept;
    TH1F *fHistSignalLeptonEta_before_lept;
    TH1F *fHistSignalLeptonEta_before_cells_lept;
    TH1F *fHistSignalLeptonPhi_before_lept;
    TH1F *fHistBJetEt_before_lept;
    TH1F *fHistBJetEta_before_lept;
    TH1F *fHistBJetPhi_before_lept;
    TH1F *fHistBJetWeight_before_lept;
    TH1F *fHistJet1Et_before_lept;
    TH1F *fHistJet1Eta_before_lept;
    TH1F *fHistJet1Phi_before_lept;
    TH1F *fHistJet2Et_before_lept;
    TH1F *fHistJet2Eta_before_lept;
    TH1F *fHistJet2Phi_before_lept;
    TH1F *fHistEtMissMag_before_lept;
    TH1F *fHistEtMissPhi_before_lept;
    TH1F *fHistSumEt_before_lept;

    TH1F *fHistSignalLeptonPt_after_lept;
    TH1F *fHistSignalLeptonEta_after_lept;
    TH1F *fHistSignalLeptonEta_after_cells_lept;
    TH1F *fHistSignalLeptonPhi_after_lept;
    TH1F *fHistBJetEt_after_lept;
    TH1F *fHistBJetEta_after_lept;
    TH1F *fHistBJetPhi_after_lept;
    TH1F *fHistBJetWeight_after_lept;
    TH1F *fHistJet1Et_after_lept;
    TH1F *fHistJet1Eta_after_lept;
    TH1F *fHistJet1Phi_after_lept;
    TH1F *fHistJet2Et_after_lept;
    TH1F *fHistJet2Eta_after_lept;
    TH1F *fHistJet2Phi_after_lept;
    TH1F *fHistEtMissMag_after_lept;
    TH1F *fHistEtMissPhi_after_lept;

    TH1F *fHistSignalLeptonPt_before_hadr;
    TH1F *fHistSignalLeptonEta_before_hadr;
    TH1F *fHistSignalLeptonEta_before_cells_hadr;
    TH1F *fHistSignalLeptonPhi_before_hadr;
    TH1F *fHistBJetEt_before_hadr;
    TH1F *fHistBJetEta_before_hadr;
    TH1F *fHistBJetPhi_before_hadr;
    TH1F *fHistBJetWeight_before_hadr;
    TH1F *fHistJet1Et_before_hadr;
    TH1F *fHistJet1Eta_before_hadr;
    TH1F *fHistJet1Phi_before_hadr;
    TH1F *fHistJet2Et_before_hadr;
    TH1F *fHistJet2Eta_before_hadr;
    TH1F *fHistJet2Phi_before_hadr;
    TH1F *fHistEtMissMag_before_hadr;
    TH1F *fHistEtMissPhi_before_hadr;
    TH1F *fHistSumEt_before_hadr;

    TH1F *fHistSignalLeptonPt_after_hadr;
    TH1F *fHistSignalLeptonEta_after_hadr;
    TH1F *fHistSignalLeptonEta_after_hadr_cells;
    TH1F *fHistSignalLeptonPhi_after_hadr;
    TH1F *fHistBJetEt_after_hadr;
    TH1F *fHistBJetEta_after_hadr;
    TH1F *fHistBJetPhi_after_hadr;
    TH1F *fHistBJetWeight_after_hadr;
    TH1F *fHistJet1Et_after_hadr;
    TH1F *fHistJet1Eta_after_hadr;
    TH1F *fHistJet1Phi_after_hadr;
    TH1F *fHistJet2Et_after_hadr;
    TH1F *fHistJet2Eta_after_hadr;
    TH1F *fHistJet2Phi_after_hadr;

    TH1F *fHistEtMissMag_after_hadr;
    TH1F *fHistEtMissPhi_after_hadr;

    // Jets
    TH1F *fHistJet_n;      // No. of jets
    TH1F *fHistJet_Et;     // Et of all jets
    TH1F *fHistJet_eta;    // Eta of all jets
    TH1F *fHistJet_phi;    // Phi of all jets

    TH1F *fHistJet1_Et;    // Pt of leading jet
    TH1F *fHistJet1_eta;   // Eta of leading jet
    TH1F *fHistJet1_phi;   // Phi of leading jet
    TH1F *fHistJet2_Et;    // Pt of 2nd leading jet
    TH1F *fHistJet2_eta;   // Eta of 2nd leading jet
    TH1F *fHistJet2_phi;   // Phi of 2nd leading jet

    TH1F *fHistBJet_n;      // No. of b-jets
    TH1F *fHistBJet_Et;     // Et of all b-jets
    TH1F *fHistBJet_eta;    // Eta of all b-jets
    TH1F *fHistBJet_phi;    // Phi of all b-jets
    TH1F *fHistBJet_weight;// Weight of all b-jets
    TH1F *fHistBJet1_Et;    // Pt of leading b-jet
    TH1F *fHistBJet1_eta;   // Eta of leading b-jet
    TH1F *fHistBJet1_phi;   // Phi of leading b-jet
    TH1F *fHistBJet1_weight;// Weight of leading b-jet
    TH1F *fHistBJet2_Et;    // Pt of 2nd leading b-jet
    TH1F *fHistBJet2_eta;   // Eta of 2nd leading b-jet
    TH1F *fHistBJet2_phi;   // Phi of 2nd leading b-jet
    TH1F *fHistBJet2_weight;// Weight of leading b-jet

    TH1F *fHistNonBJet_n;      // No. of non-b-jets
    TH1F *fHistNonBJet_Et;     // Et of all non-b-jets
    TH1F *fHistNonBJet_eta;    // Eta of all non-b-jets
    TH1F *fHistNonBJet_phi;    // Phi of all non-b-jets
    TH1F *fHistNonBJet1_Et;    // Pt of leading non-b-jet
    TH1F *fHistNonBJet1_eta;   // Eta of leading non-b-jet
    TH1F *fHistNonBJet1_phi;   // Phi of leading non-b-jet
    TH1F *fHistNonBJet2_Et;    // Pt of 2nd leading non-b-jet
    TH1F *fHistNonBJet2_eta;   // Eta of 2nd leading non-b-jet
    TH1F *fHistNonBJet2_phi;   // Phi of 2nd leading non-b-jet

    TH1F *fHistSignalLepton_pt_btag;
    TH1F *fHistSignalLepton_eta_btag;
    TH1F *fHistSignalLepton_eta_btag_cells;
    TH1F *fHistSignalLepton_phi_btag;

    TH1F *fHistSignalElectron_author_btag;
    TH1F *fHistSignalElectron_pt_btag;
    TH1F *fHistSignalElectron_eta_btag;
    TH1F *fHistSignalElectron_phi_btag;
    TH1F *fHistSignalEplus_pt_btag;
    TH1F *fHistSignalEplus_eta_btag;
    TH1F *fHistSignalEplus_phi_btag;
    TH1F *fHistSignalEminus_pt_btag;
    TH1F *fHistSignalEminus_eta_btag;
    TH1F *fHistSignalEminus_phi_btag;
    TH1F *fHistSignalMuon_pt_btag;
    TH1F *fHistSignalMuon_eta_btag;
    TH1F *fHistSignalMuon_phi_btag;
    TH1F *fHistSignalMuplus_pt_btag;
    TH1F *fHistSignalMuplus_eta_btag;
    TH1F *fHistSignalMuplus_phi_btag;
    TH1F *fHistSignalMuminus_pt_btag;
    TH1F *fHistSignalMuminus_eta_btag;
    TH1F *fHistSignalMuminus_phi_btag;
    TH1F *fHistJet_n_btag;
    TH1F *fHistJet_Et_btag;
    TH1F *fHistJet_eta_btag;
    TH1F *fHistJet_phi_btag;
    TH1F *fHistJet1_Et_btag;
    TH1F *fHistJet1_eta_btag;
    TH1F *fHistJet1_phi_btag;
    TH1F *fHistJet2_Et_btag;
    TH1F *fHistJet2_eta_btag;
    TH1F *fHistJet2_phi_btag;
    TH1F *fHistBJet_n_btag;
    TH1F *fHistBJet_Et_btag;
    TH1F *fHistBJet_eta_btag;
    TH1F *fHistBJet_phi_btag;
    TH1F *fHistBJet_weight_btag;
    TH1F *fHistBJet1_Et_btag;
    TH1F *fHistBJet1_eta_btag;
    TH1F *fHistBJet1_phi_btag;
    TH1F *fHistBJet1_weight_btag;
    TH1F *fHistBJet2_Et_btag;
    TH1F *fHistBJet2_eta_btag;
    TH1F *fHistBJet2_phi_btag;
    TH1F *fHistBJet2_weight_btag;
    TH1F *fHistNonBJet_n_btag;
    TH1F *fHistNonBJet_Et_btag;
    TH1F *fHistNonBJet_eta_btag;
    TH1F *fHistNonBJet_phi_btag;
    TH1F *fHistNonBJet1_Et_btag;
    TH1F *fHistNonBJet1_eta_btag;
    TH1F *fHistNonBJet1_phi_btag;
    TH1F *fHistNonBJet2_Et_btag;
    TH1F *fHistNonBJet2_eta_btag;
    TH1F *fHistNonBJet2_phi_btag;

    // ====================================
    // histograms for KinFit top candidates
    // ====================================

    // Signal lepton histogrmas
    TH1F *fHistKinFitCandidateSignalLepton_pt;    // Pt of leading signal lepton
    TH1F *fHistKinFitCandidateSignalLepton_eta;   // Eta of leading signal lepton
    TH1F *fHistKinFitCandidateSignalLepton_phi;   // Phi of leading signal lepton

    TH1F *fHistKinFitCandidateSignalElectron_author; // Author of signal electron
    TH1F *fHistKinFitCandidateSignalElectron_pt;  // Pt of leading signal electron
    TH1F *fHistKinFitCandidateSignalElectron_eta; // Eta of leading signal electron
    TH1F *fHistKinFitCandidateSignalElectron_phi; // Phi of leading signal electron
    TH1F *fHistKinFitCandidateSignalEplus_pt;     // Pt of leading signal e+
    TH1F *fHistKinFitCandidateSignalEplus_eta;    // Eta of leading signal e+
    TH1F *fHistKinFitCandidateSignalEplus_phi;    // Phi of leading signal e+
    TH1F *fHistKinFitCandidateSignalEminus_pt;    // Pt of leading signal e-
    TH1F *fHistKinFitCandidateSignalEminus_eta;   // Eta of leading signal e-
    TH1F *fHistKinFitCandidateSignalEminus_phi;   // Phi of leading signal e-

    TH1F *fHistKinFitCandidateSignalMuon_pt;      // Pt of leading signal muon
    TH1F *fHistKinFitCandidateSignalMuon_eta;     // Eta of leading signal muon
    TH1F *fHistKinFitCandidateSignalMuon_phi;     // Phi of leading signal muon
    TH1F *fHistKinFitCandidateSignalMuplus_pt;    // Pt of leading signal mu+
    TH1F *fHistKinFitCandidateSignalMuplus_eta;   // Eta of leading signal mu+
    TH1F *fHistKinFitCandidateSignalMuplus_phi;   // Phi of leading signal mu+
    TH1F *fHistKinFitCandidateSignalMuminus_pt;   // Pt of leading signal mu-
    TH1F *fHistKinFitCandidateSignalMuminus_eta;  // Eta of leading signal mu-
    TH1F *fHistKinFitCandidateSignalMuminus_phi;  // Phi of leading signal mu-

    TH1F *fHistKinFitCandidateSignalTau_pt;       // Pt of leading signal tau
    TH1F *fHistKinFitCandidateSignalTau_eta;      // Eta of leading signal tau
    TH1F *fHistKinFitCandidateSignalTau_phi;      // Phi of leading signal tau
    TH1F *fHistKinFitCandidateSignalTauplus_pt;   // Pt of leading signal tau+
    TH1F *fHistKinFitCandidateSignalTauplus_eta;  // Eta of leading signal tau+
    TH1F *fHistKinFitCandidateSignalTauplus_phi;  // Phi of leading signal tau+
    TH1F *fHistKinFitCandidateSignalTauminus_pt;  // Pt of leading signal tau-
    TH1F *fHistKinFitCandidateSignalTauminus_eta; // Eta of leading signal tau-
    TH1F *fHistKinFitCandidateSignalTauminus_phi; // Phi of leading signal tau-

    // Jets
    TH1F *fHistKinFitCandidateJet_n;      // No. of jets
    TH1F *fHistKinFitCandidateJet_Et;     // Et of all jets
    TH1F *fHistKinFitCandidateJet_eta;    // Eta of all jets
    TH1F *fHistKinFitCandidateJet_phi;    // Phi of all jets
    TH1F *fHistKinFitCandidateJet1_Et;    // Pt of leading jet
    TH1F *fHistKinFitCandidateJet1_eta;   // Eta of leading jet
    TH1F *fHistKinFitCandidateJet1_phi;   // Phi of leading jet
    TH1F *fHistKinFitCandidateJet2_Et;    // Pt of 2nd leading jet
    TH1F *fHistKinFitCandidateJet2_eta;   // Eta of 2nd leading jet
    TH1F *fHistKinFitCandidateJet2_phi;   // Phi of 2nd leading jet

    TH1F *fHistKinFitCandidateBJet_n;      // No. of b-jets
    TH1F *fHistKinFitCandidateBJet_Et;     // Et of all b-jets
    TH1F *fHistKinFitCandidateBJet_eta;    // Eta of all b-jets
    TH1F *fHistKinFitCandidateBJet_phi;    // Phi of all b-jets
    TH1F *fHistKinFitCandidateBJet1_Et;    // Pt of leading b-jet
    TH1F *fHistKinFitCandidateBJet1_eta;   // Eta of leading b-jet
    TH1F *fHistKinFitCandidateBJet1_phi;   // Phi of leading b-jet
    TH1F *fHistKinFitCandidateBJet2_Et;    // Pt of 2nd leading b-jet
    TH1F *fHistKinFitCandidateBJet2_eta;   // Eta of 2nd leading b-jet
    TH1F *fHistKinFitCandidateBJet2_phi;   // Phi of 2nd leading b-jet

    TH1F *fHistKinFitCandidateNonBJet_n;      // No. of non-b-jets
    TH1F *fHistKinFitCandidateNonBJet_Et;     // Et of all non-b-jets
    TH1F *fHistKinFitCandidateNonBJet_eta;    // Eta of all non-b-jets
    TH1F *fHistKinFitCandidateNonBJet_phi;    // Phi of all non-b-jets
    TH1F *fHistKinFitCandidateNonBJet1_Et;    // Pt of leading non-b-jet
    TH1F *fHistKinFitCandidateNonBJet1_eta;   // Eta of leading non-b-jet
    TH1F *fHistKinFitCandidateNonBJet1_phi;   // Phi of leading non-b-jet
    TH1F *fHistKinFitCandidateNonBJet2_Et;    // Pt of 2nd leading non-b-jet
    TH1F *fHistKinFitCandidateNonBJet2_eta;   // Eta of 2nd leading non-b-jet
    TH1F *fHistKinFitCandidateNonBJet2_phi;   // Phi of 2nd leading non-b-jet

    // W reconstruction histograms
    //
    // from top  ---    leptonic decays
    TH1F *fHist_W_lept_top_reco_Mt;        // Reconstructed invariant transverse mass of the leptonic W
                                           // (all leptons), the W comes from a top decay
    TH1F *fHist_W_lept_top_reco_M;         // Reconstructed invariant mass of that W (all leptons)
    TH1F *fHist_W_lept_top_reco_phi;       // Reconstructed azimuth angle of that W (all leptons)
    TH1F *fHist_W_lept_top_reco_eta;       // Reconstructed Eta of that W (all leptons)
    TH1F *fHist_W_lept_top_reco_rap;       // Reconstructed y of that W (all leptons)
    TH1F *fHist_W_lept_top_reco_pt;        // Reconstructed Pt of that W (all leptons)
    // electron channel
    TH1F *fHist_W_lept_top_reco_Mt_e;      // Reconstructed invariant transverse mass of the leptonic W decaying
                                           // into electrons, the W comes from a top decay
    TH1F *fHist_W_lept_top_reco_M_e;       // Reconstructed invariant mass of taht W
    TH1F *fHist_W_lept_top_reco_phi_e;     // Reconstructed azimuth angle of that W
    TH1F *fHist_W_lept_top_reco_eta_e;     // Reconstructed Eta of that W
    TH1F *fHist_W_lept_top_reco_rap_e;     // Reconstructed y of that W
    TH1F *fHist_W_lept_top_reco_pt_e;      // Reconstructed Pt of that W
    // muon channel
    TH1F *fHist_W_lept_top_reco_Mt_mu;     // Reconstructed invariant transverse mass of the W
                                           // decaying into muons, the W comes from a top decay
    TH1F *fHist_W_lept_top_reco_M_mu;      // Reconstructed invariant mass of that W
    TH1F *fHist_W_lept_top_reco_phi_mu;    // Reconstructed azimuth angle of that W
    TH1F *fHist_W_lept_top_reco_eta_mu;    // Reconstructed Eta of that W
    TH1F *fHist_W_lept_top_reco_rap_mu;    // Reconstructed y of that W
    TH1F *fHist_W_lept_top_reco_pt_mu;     // Reconstructed Pt of that W

    TH1F *fHist_W_lept_top_reco_Mt_minus;  // Reconstructed invariant transverse mass of the leptonic W
    TH1F *fHist_W_lept_top_reco_M_minus;   // Reconstructed invariant mass of the leptonic W
                                           // (all negative sign leptons)
    TH1F *fHist_W_lept_top_reco_phi_minus; // Reconstructed azimuth angle of the leptonic W (all negative sign leptons)
    TH1F *fHist_W_lept_top_reco_eta_minus; // Reconstructed Eta of the leptonic W (all negative sign leptons)
    TH1F *fHist_W_lept_top_reco_rap_minus; // Reconstructed y of the leptonic W (all negative sign leptons)
    TH1F *fHist_W_lept_top_reco_pt_minus;  // Reconstructed Pt of the leptonic W (all negative sign leptons)
    TH1F *fHist_W_lept_top_reco_Mt_plus;   // Reconstructed invariant transverse mass of the leptonic W
                                           // (all positive sign leptons)
    TH1F *fHist_W_lept_top_reco_M_plus;    // Reconstructed invariant mass of the leptonic W
                                           // (all positive sign leptons)
    TH1F *fHist_W_lept_top_reco_phi_plus;  // Reconstructed azimuth angle of the leptonic W (all positive sign leptons)
    TH1F *fHist_W_lept_top_reco_eta_plus;  // Reconstructed Eta of the leptonic W (all positive sign leptons)
    TH1F *fHist_W_lept_top_reco_rap_plus;  // Reconstructed y of the leptonic W (all positive sign leptons)
    TH1F *fHist_W_lept_top_reco_pt_plus;   // Reconstructed Pt of the leptonic W (all positive sign leptons)

    TH1F *fHist_W_lept_top_reco_Mt_e_minus;  // Reconstructed invariant transverse mass of the W decaying into e-
    TH1F *fHist_W_lept_top_reco_M_e_minus;   // Reconstructed invariant mass of the W decaying into e-
    TH1F *fHist_W_lept_top_reco_phi_e_minus; // Reconstructed azimuth angle of the (real) W decaying into e-
    TH1F *fHist_W_lept_top_reco_eta_e_minus; // Reconstructed Eta of the W decaying into e-
    TH1F *fHist_W_lept_top_reco_rap_e_minus; // Reconstructed y of the (real) W decaying into e-
    TH1F *fHist_W_lept_top_reco_pt_e_minus;  // Reconstructed Pt of the W decaying into e-
    TH1F *fHist_W_lept_top_reco_Mt_e_plus;   // Reconstructed invariant transverse mass of the W decaying into e+
    TH1F *fHist_W_lept_top_reco_M_e_plus;    // Reconstructed invariant mass of the W decaying into e+
    TH1F *fHist_W_lept_top_reco_phi_e_plus;  // Reconstructed azimuth angle of the (real) W decaying into e+
    TH1F *fHist_W_lept_top_reco_eta_e_plus;  // Reconstructed Eta of the W decaying into e+
    TH1F *fHist_W_lept_top_reco_rap_e_plus;  // Reconstructed y of the (real) W decaying into e+
    TH1F *fHist_W_lept_top_reco_pt_e_plus;   // Reconstructed Pt of the W decaying into e+

    TH1F *fHist_W_lept_top_reco_Mt_mu_minus;  // Reconstructed invariant transverse mass of the W decaying into mu-
    TH1F *fHist_W_lept_top_reco_M_mu_minus;   // Reconstructed invariant mass of the W decaying into mu-
    TH1F *fHist_W_lept_top_reco_phi_mu_minus; // Reconstructed azimuth angle of the W decaying into mu-
    TH1F *fHist_W_lept_top_reco_eta_mu_minus; // Reconstructed Eta of the W decaying into mu-
    TH1F *fHist_W_lept_top_reco_rap_mu_minus; // Reconstructed y of the W decaying into mu-
    TH1F *fHist_W_lept_top_reco_pt_mu_minus;  // Reconstructed Pt of the W decaying into mu-
    TH1F *fHist_W_lept_top_reco_Mt_mu_plus;   // Reconstructed invariant transverse mass of the W decaying into mu+
    TH1F *fHist_W_lept_top_reco_M_mu_plus;   // Reconstructed invariant mass of the W decaying into mu+
    TH1F *fHist_W_lept_top_reco_phi_mu_plus;  // Reconstructed azimuth angle of the W decaying into mu+
    TH1F *fHist_W_lept_top_reco_eta_mu_plus;  // Reconstructed Eta of the W decaying into mu+
    TH1F *fHist_W_lept_top_reco_rap_mu_plus;  // Reconstructed y of the W decaying into mu+
    TH1F *fHist_W_lept_top_reco_pt_mu_plus;   // Reconstructed Pt of the W decaying into mu+
    //    ---    hadronic decays
    TH1F *fHist_W_hadr_top_reco_Mt;        // Reconstructed invariant transverse mass of the hadronic W
                                           // coming from a top quark
    TH1F *fHist_W_hadr_top_reco_M;         // Reconstructed invariant mass of the W (all leptons)
    TH1F *fHist_W_hadr_top_reco_pt;        // Reconstructed pt of the W (all leptons)
    TH1F *fHist_W_hadr_top_reco_eta;       // Reconstructed pseudorapidity of the W (all leptons)
    TH1F *fHist_W_hadr_top_reco_phi;       // Reconstructed azimuth angle of the W (all leptons)
    TH1F *fHist_W_hadr_top_reco_rap;       // Reconstructed y of the W (all leptons)
                                // electrons
    TH1F *fHist_W_hadr_top_reco_M_e;   // Reconstructed invariant mass of the hadronic W coming from the
                                           // top quark, the associated W decayed into an electron (or positron)
    TH1F *fHist_W_hadr_top_reco_Mt_e;  // Reconstructed invariant transverse mass of the hadronic W
    TH1F *fHist_W_hadr_top_reco_pt_e;  // Reconstructed transverse momentum of the hadronic W
    TH1F *fHist_W_hadr_top_reco_phi_e; // Reconstructed azimuth angle of the hadronic W
    TH1F *fHist_W_hadr_top_reco_eta_e; // Reconstructed eta of the hadronic W
    TH1F *fHist_W_hadr_top_reco_rap_e; // Reconstructed y of the hadronic W
                                // muons
    TH1F *fHist_W_hadr_top_reco_M_mu;    // Reconstructed invariant mass of the Wc oming from the
                                           // top quark, the associated W decayed into  a muons (or Antimuon)
    TH1F *fHist_W_hadr_top_reco_Mt_mu;   // Reconstructed invariant transverse mass of the hadronic W
    TH1F *fHist_W_hadr_top_reco_phi_mu;  // Reconstructed azimuth angle of the hadronic W
    TH1F *fHist_W_hadr_top_reco_eta_mu;  // Reconstructed eta of the hadronic W
    TH1F *fHist_W_hadr_top_reco_rap_mu;  // Reconstructed y of the hadronic W
    TH1F *fHist_W_hadr_top_reco_pt_mu;   // Reconstructed Pt of the hadronic W

    //
    // associate W -  reconstruction histograms
    //
    //    ---    leptonic decays
    TH1F *fHist_W_lept_notop_reco_Mt;        // Reconstructed invariant transverse mass of the W (all leptons)
    TH1F *fHist_W_lept_notop_reco_phi;       // Reconstructed azimuth angle of the W (all leptons)
    TH1F *fHist_W_lept_notop_reco_pt;        // Reconstructed pt of the W (all leptons)
    //TH1F *fHist_W_lept_notop_reco_rap;       // Reconstructed y of the W (all leptons)
    TH1F *fHist_W_lept_notop_reco_Mt_minus;  // Reconstructed invariant transverse mass of the W
                                // (all negative sign leptons)
    TH1F *fHist_W_lept_notop_reco_phi_minus; // Reconstructed azimuth angle of the W (all negative sign leptons)
    TH1F *fHist_W_lept_notop_reco_pt_minus; // Reconstructed y of the W (all negative sign leptons)
    TH1F *fHist_W_lept_notop_reco_Mt_plus;   // Reconstructed invariant transverse mass of the W
                                // (all positive sign leptons)
    TH1F *fHist_W_lept_notop_reco_phi_plus;  // Reconstructed azimuth angle of the W (all positive sign leptons)
    TH1F *fHist_W_lept_notop_reco_pt_plus;   // Reconstructed Pt of the W (all positive sign leptons)

    TH1F *fHist_W_lept_notop_reco_Mt_e;        // Reconstructed invariant transverse mass of the W decaying
                                  // into electrons
    TH1F *fHist_W_lept_notop_reco_phi_e;       // Reconstructed azimuth angle of the (real) W decaying into electrons
    TH1F *fHist_W_lept_notop_reco_pt_e;        // Reconstructed Pt of the W decaying into electrons
    TH1F *fHist_W_lept_notop_reco_Mt_e_minus;  // Reconstructed invariant transverse mass of the W decaying into e-
    TH1F *fHist_W_lept_notop_reco_phi_e_minus; // Reconstructed azimuth angle of the (real) W decaying into e-
    TH1F *fHist_W_lept_notop_reco_pt_e_minus;  // Reconstructed Pt of the W decaying into e-
    TH1F *fHist_W_lept_notop_reco_Mt_e_plus;   // Reconstructed invariant transverse mass of the W decaying into e+
    TH1F *fHist_W_lept_notop_reco_phi_e_plus;  // Reconstructed azimuth angle of the (real) W decaying into e+
    TH1F *fHist_W_lept_notop_reco_pt_e_plus;   // Reconstructed Pt of the W decaying into e+

    TH1F *fHist_W_lept_notop_reco_Mt_mu;        // Reconstructed invariant transverse mass of the W decaying into muons
    TH1F *fHist_W_lept_notop_reco_phi_mu;       // Reconstructed azimuth angle of the W decaying into muons
    TH1F *fHist_W_lept_notop_reco_pt_mu;        // Reconstructed Pt of the W decaying into muons
    TH1F *fHist_W_lept_notop_reco_Mt_mu_minus;  // Reconstructed invariant transverse mass of the W decaying into mu-
    TH1F *fHist_W_lept_notop_reco_phi_mu_minus; // Reconstructed azimuth angle of the W decaying into mu-
    TH1F *fHist_W_lept_notop_reco_pt_mu_minus;  // Reconstructed Pt of the W decaying into mu-
    TH1F *fHist_W_lept_notop_reco_Mt_mu_plus;   // Reconstructed invariant transverse mass of the W decaying into mu+
    TH1F *fHist_W_lept_notop_reco_phi_mu_plus;  // Reconstructed azimuth angle of the W decaying into mu+
    TH1F *fHist_W_lept_notop_reco_pt_mu_plus;   // Reconstructed Pt of the W decaying into mu+

    // histograms for charged lepton in hadronic top decays
    TH1F *fHist_HadronicTop_ChargedLepton_pt;   // pt of Charged Lepton in hadronic top decays
    TH1F *fHist_HadronicTop_ChargedLepton_eta;  // eta of Charged Lepton in hadronic top decays
    TH1F *fHist_HadronicTop_ChargedLepton_phi;  // phi of Charged Lepton in hadronic top decays
    TH1F *fHist_HadronicTop_electron_pt;   // pt of electron in hadronic top decays
    TH1F *fHist_HadronicTop_electron_eta;  // eta of electron in hadronic top decays
    TH1F *fHist_HadronicTop_electron_phi;  // phi of electron in hadronic top decays
    TH1F *fHist_HadronicTop_muon_pt;   // pt of muon in hadronic top decays
    TH1F *fHist_HadronicTop_muon_eta;  // eta of muon in hadronic top decays
    TH1F *fHist_HadronicTop_muon_phi;  // phi of muon in hadronic top decays

    // histograms for EtMiss in hadronic top decays
    TH1F *fHist_HadronicTop_EtMiss_mag;   // EtMiss in hadronic top decays
    TH1F *fHist_HadronicTop_EtMiss_phi;   // phi of EtMiss in hadronic top decays

    //    ---    hadronic decays
    TH1F *fHist_W_hadr_notop_reco_Mt;        // Reconstructed invariant transverse mass of the associated
                                             // hadronic W (all leptons)
    TH1F *fHist_W_hadr_notop_reco_M;         // Reconstructed invariant mass of that W (all leptons)
    TH1F *fHist_W_hadr_notop_reco_pt;        // Reconstructed pt of that W (all leptons)
    TH1F *fHist_W_hadr_notop_reco_eta;       // Reconstructed pseudorapidity of that W (all leptons)
    TH1F *fHist_W_hadr_notop_reco_phi;       // Reconstructed azimuth angle of that W (all leptons)
    TH1F *fHist_W_hadr_notop_reco_rap;       // Reconstructed y of  that W (all leptons)
    // electron channel
    TH1F *fHist_W_hadr_notop_reco_Mt_e;        // Reconstructed invariant transverse mass of the associated
                                               // hadronic W in the electron channel
    TH1F *fHist_W_hadr_notop_reco_M_e;         // Reconstructed invariant mass of that W
    TH1F *fHist_W_hadr_notop_reco_phi_e;       // Reconstructed azimuth angle of that W
    TH1F *fHist_W_hadr_notop_reco_pt_e;        // Reconstructed Pt of that W
    TH1F *fHist_W_hadr_notop_reco_rap_e;       // Reconstructed rapidity of that W
    // muon channel
    TH1F *fHist_W_hadr_notop_reco_Mt_mu;        // Reconstructed invariant transverse mass of the associated
                                                // hadronic W in the muon channel
    TH1F *fHist_W_hadr_notop_reco_M_mu;         // Reconstructed invariant mass of that W
    TH1F *fHist_W_hadr_notop_reco_phi_mu;       // Reconstructed azimuth angle of that W
    TH1F *fHist_W_hadr_notop_reco_pt_mu;        // Reconstructed Pt of that W
    TH1F *fHist_W_hadr_notop_reco_rap_mu;       // Reconstructed rapidity of that W

                                // (all negative sign leptons)
    TH1F *fHist_W_hadr_notop_reco_M_minus;  // Reconstructed invariant mass of the W
    TH1F *fHist_W_hadr_notop_reco_Mt_minus;  // Reconstructed invariant transverse mass of the W
    TH1F *fHist_W_hadr_notop_reco_pt_minus;  // Reconstructed transverse momentum of the W (all negative sign leptons)
    TH1F *fHist_W_hadr_notop_reco_phi_minus; // Reconstructed azimuth angle of the W (all negative sign leptons)
    TH1F *fHist_W_hadr_notop_reco_eta_minus; // Reconstructed eta of the W (all negative sign leptons)
    TH1F *fHist_W_hadr_notop_reco_rap_minus; // Reconstructed y of the W (all negative sign leptons)
                                // (all positive sign leptons)
    TH1F *fHist_W_hadr_notop_reco_M_plus;   // Reconstructed invariant mass of the W
    TH1F *fHist_W_hadr_notop_reco_Mt_plus;   // Reconstructed invariant transverse mass of the W
    TH1F *fHist_W_hadr_notop_reco_phi_plus;  // Reconstructed azimuth angle of the W (all positive sign leptons)
    TH1F *fHist_W_hadr_notop_reco_eta_plus;  // Reconstructed eta of the W (all positive sign leptons)
    TH1F *fHist_W_hadr_notop_reco_rap_plus;  // Reconstructed y of the W (all positive sign leptons)
    TH1F *fHist_W_hadr_notop_reco_pt_plus;   // Reconstructed Pt of the W (all positive sign leptons)

    TH1F *fHist_W_hadr_notop_reco_Mt_e_minus;  // Reconstructed invariant transverse mass of the W decaying into e-
    TH1F *fHist_W_hadr_notop_reco_phi_e_minus; // Reconstructed azimuth angle of the (real) W decaying into e-
    TH1F *fHist_W_hadr_notop_reco_pt_e_minus;  // Reconstructed Pt of the W decaying into e-
    TH1F *fHist_W_hadr_notop_reco_Mt_e_plus;   // Reconstructed invariant transverse mass of the W decaying into e+
    TH1F *fHist_W_hadr_notop_reco_phi_e_plus;  // Reconstructed azimuth angle of the (real) W decaying into e+
    TH1F *fHist_W_hadr_notop_reco_pt_e_plus;   // Reconstructed Pt of the W decaying into e+

    TH1F *fHist_W_hadr_notop_reco_Mt_mu_minus;  // Reconstructed invariant transverse mass of the W decaying into mu-
    TH1F *fHist_W_hadr_notop_reco_phi_mu_minus; // Reconstructed azimuth angle of the W decaying into mu-
    TH1F *fHist_W_hadr_notop_reco_pt_mu_minus;  // Reconstructed Pt of the W decaying into mu-
    TH1F *fHist_W_hadr_notop_reco_Mt_mu_plus;   // Reconstructed invariant transverse mass of the W decaying into mu+
    TH1F *fHist_W_hadr_notop_reco_phi_mu_plus;  // Reconstructed azimuth angle of the W decaying into mu+
    TH1F *fHist_W_hadr_notop_reco_pt_mu_plus;   // Reconstructed Pt of the W decaying into mu+

    // top quark reconstruction histograms
    // leptonic decay of the top
    TH1F *fHistLeptonicTop_Mt;        // Reconstructed invariant transverse mass of the single-top
                                  // (all leptons)
    TH1F *fHistLeptonicTop_M;        // Reconstructed invariant mass of the single-top
	                          // (all leptons)
    TH1F *fHistLeptonicTop_phi;       // Reconstructed azimuth angle of the single-top (all leptons)
    TH1F *fHistLeptonicTop_eta;       // Reconstructed Eta of the single-top (all leptons)
    TH1F *fHistLeptonicTop_rap;       // Reconstructed y of the single-top (all leptons)
    TH1F *fHistLeptonicTop_pt;        // Reconstructed Pt of the single-top (all leptons)
    TH1F *fHistLeptonicTop_Mt_plus;   // Reconstructed invariant transverse mass of the single-top
                                  // (all positive sign leptons)
    TH1F *fHistLeptonicTop_M_plus;   // Reconstructed invariant mass of the single-top
                                  // (all positive sign leptons)
    TH1F *fHistLeptonicTop_phi_plus;  // Reconstructed azimuth angle of the single-top
                                  // (all positive sign leptons)
    TH1F *fHistLeptonicTop_eta_plus;  // Reconstructed Eta of the single-top (all positive sign leptons)
    TH1F *fHistLeptonicTop_rap_plus;  // Reconstructed y of the single-top (all positive sign leptons)
    TH1F *fHistLeptonicTop_pt_plus;   // Reconstructed Pt of the single-top (all positive sign leptons)
    TH1F *fHistLeptonicTop_Mt_minus;  // Reconstructed invariant transverse mass of the single-top
                                  // (all negative sign leptons)
    TH1F *fHistLeptonicTop_M_minus;  // Reconstructed invariant mass of the single-top
	                          // (all negative sign leptons)
    TH1F *fHistLeptonicTop_phi_minus; // Reconstructed azimuth angle of the single-top
                                  // (all negative sign leptons)
    TH1F *fHistLeptonicTop_eta_minus; // Reconstructed Eta of the single-top (all negative sign leptons)
    TH1F *fHistLeptonicTop_rap_minus; // Reconstructed y of the single-top (all negative sign leptons)
    TH1F *fHistLeptonicTop_pt_minus;  // Reconstructed Pt of the single-top (all negative sign leptons)

    TH1F *fHistLeptonicTop_Mt_e;        // Reconstructed invariant transverse mass of the single-top (e channel)
    TH1F *fHistLeptonicTop_M_e;        // Reconstructed invariant mass of the single-top (e channel)
    TH1F *fHistLeptonicTop_phi_e;       // Reconstructed azimuth angle of the single-top (e channel)
    TH1F *fHistLeptonicTop_eta_e;       // Reconstructed Eta of the single-top (e channel)
    TH1F *fHistLeptonicTop_rap_e;       // Reconstructed y of the top (e channel)
    TH1F *fHistLeptonicTop_pt_e;        // Reconstructed Pt of the single-top (e channel)
    TH1F *fHistLeptonicTop_Mt_e_plus;   // Reconstructed invariant transverse mass of the single-top
                                    // (e+ channel)
    TH1F *fHistLeptonicTop_M_e_plus;   // Reconstructed invariant mass of the single-top
                                    // (e+ channel)
    TH1F *fHistLeptonicTop_phi_e_plus;  // Reconstructed azimuth angle of the single-top (e+ channel)
    TH1F *fHistLeptonicTop_eta_e_plus;  // Reconstructed Eta of the single-top (e+ channel)
    TH1F *fHistLeptonicTop_rap_e_plus;  // Reconstructed y of the single-top (e+ channel)
    TH1F *fHistLeptonicTop_pt_e_plus;   // Reconstructed Pt of the single-top (e+ channel)
    TH1F *fHistLeptonicTop_Mt_e_minus;  // Reconstructed invariant transverse mass of the single-top
                                    // (e- channel)
    TH1F *fHistLeptonicTop_M_e_minus;  // Reconstructed invariant mass of the single-top
                                    // (e- channel)
    TH1F *fHistLeptonicTop_phi_e_minus; // Reconstructed azimuth angle of the single-top (e- channel)
    TH1F *fHistLeptonicTop_eta_e_minus; // Reconstructed Eta of the single-top (e- channel)
    TH1F *fHistLeptonicTop_rap_e_minus; // Reconstructed y of the single-top (e- channel)
    TH1F *fHistLeptonicTop_pt_e_minus;  // Reconstructed Pt of the single-top (e- channel)

    TH1F *fHistLeptonicTop_Mt_mu;        // Reconstructed invariant transverse mass of the single-top
                                     // (mu channel)
    TH1F *fHistLeptonicTop_M_mu;        // Reconstructed invariant mass of the single-top
                                     // (mu channel)
    TH1F *fHistLeptonicTop_phi_mu;       // Reconstructed azimuth angle of the single-top (mu channel)
    TH1F *fHistLeptonicTop_eta_mu;       // Reconstructed Eta of the single-top (mu channel)
    TH1F *fHistLeptonicTop_rap_mu;       // Reconstructed y of the single-top (mu channel)
    TH1F *fHistLeptonicTop_pt_mu;        // Reconstructed Pt of the single-top (mu channel)
    TH1F *fHistLeptonicTop_Mt_mu_plus;   // Reconstructed invariant transverse mass of the single-top
                                     // (mu+ channel)
    TH1F *fHistLeptonicTop_M_mu_plus;   // Reconstructed invariant mass of the single-top
	                             // (mu+ channel)
    TH1F *fHistLeptonicTop_phi_mu_plus;  // Reconstructed azimuth angle of the single-top (mu+ channel)
    TH1F *fHistLeptonicTop_eta_mu_plus;  // Reconstructed Eta of the single-top (mu+ channel)
    TH1F *fHistLeptonicTop_rap_mu_plus;  // Reconstructed y of the single-top (mu+ channel)
    TH1F *fHistLeptonicTop_pt_mu_plus;   // Reconstructed Pt of the single-top (mu+ channel)
    TH1F *fHistLeptonicTop_Mt_mu_minus;  // Reconstructed invariant transverse mass of the single-top
                                     // (mu- channel)
    TH1F *fHistLeptonicTop_M_mu_minus;  // Reconstructed invariant mass of the single-top
                                     // (mu- channel)
    TH1F *fHistLeptonicTop_phi_mu_minus; // Reconstructed azimuth angle of the single-top (mu- channel)
    TH1F *fHistLeptonicTop_eta_mu_minus; // Reconstructed Eta of the single-top (mu- channel)
    TH1F *fHistLeptonicTop_rap_mu_minus; // Reconstructed y of the single-top (mu- channel)
    TH1F *fHistLeptonicTop_pt_mu_minus;  // Reconstructed Pt of the single-top (mu- channel)

    // hadronic decay of the top
    // all leptons
    TH1F *fHistHadronicTop_Mt;        // Reconstructed invariant transverse mass of the hadronic top
                                      // (all leptons)
    TH1F *fHistHadronicTop_M;         // Reconstructed invariant mass of the hadronic top
	                              // (all leptons)
    TH1F *fHistHadronicTop_phi;       // Reconstructed azimuth angle of the hadronic top (all leptons)
    TH1F *fHistHadronicTop_eta;       // Reconstructed Eta of the hadronic top (all leptons)
    TH1F *fHistHadronicTop_rap;       // Reconstructed y of the hadronic top (all leptons)
    TH1F *fHistHadronicTop_pt;        // Reconstructed Pt of the hadronic top (all leptons)
    // electrons
    TH1F *fHistHadronicTop_Mt_e;        // Reconstructed invariant transverse mass of the hadronic top,
                                      // electrons
    TH1F *fHistHadronicTop_M_e;         // Reconstructed invariant mass of the hadronic top,
	                              // electrons
    TH1F *fHistHadronicTop_phi_e;       // Reconstructed azimuth angle of the hadronic top, electrons
    TH1F *fHistHadronicTop_eta_e;       // Reconstructed Eta of the hadronic top, electrons
    TH1F *fHistHadronicTop_rap_e;       // Reconstructed y of the hadronic top, electrons
    TH1F *fHistHadronicTop_pt_e;        // Reconstructed Pt of the hadronic top, electrons
    // muons
    TH1F *fHistHadronicTop_Mt_mu;        // Reconstructed invariant transverse mass of the hadronic top,
                                         // muons
    TH1F *fHistHadronicTop_M_mu;         // Reconstructed invariant mass of the hadronic top,
	                                 // muons
    TH1F *fHistHadronicTop_phi_mu;       // Reconstructed azimuth angle of the hadronic top, muons
    TH1F *fHistHadronicTop_eta_mu;       // Reconstructed Eta of the hadronic top, muons
    TH1F *fHistHadronicTop_rap_mu;       // Reconstructed y of the hadronic top, muons
    TH1F *fHistHadronicTop_pt_mu;        // Reconstructed Pt of the hadronic top, muons

    TH1F *fHistHadronicTop_Mt_plus;   // Reconstructed invariant transverse mass of the hadronic top
                                  // (all positive sign leptons)
    TH1F *fHistHadronicTop_M_plus;   // Reconstructed invariant mass of the hadronic top
                                  // (all positive sign leptons)
    TH1F *fHistHadronicTop_phi_plus;  // Reconstructed azimuth angle of the hadronic top
                                  // (all positive sign leptons)
    TH1F *fHistHadronicTop_eta_plus;  // Reconstructed Eta of the hadronic top (all positive sign leptons)
    TH1F *fHistHadronicTop_rap_plus;  // Reconstructed y of the hadronic top (all positive sign leptons)
    TH1F *fHistHadronicTop_pt_plus;   // Reconstructed Pt of the hadronic top (all positive sign leptons)
    TH1F *fHistHadronicTop_Mt_minus;  // Reconstructed invariant transverse mass of the hadronic top
                                  // (all negative sign leptons)
    TH1F *fHistHadronicTop_M_minus;  // Reconstructed invariant mass of the hadronic top
	                          // (all negative sign leptons)
    TH1F *fHistHadronicTop_phi_minus; // Reconstructed azimuth angle of the hadronic top
                                  // (lal negative sign leptons)
    TH1F *fHistHadronicTop_eta_minus; // Reconstructed Eta of the hadronic top (all negative sign leptons)
    TH1F *fHistHadronicTop_rap_minus; // Reconstructed y of the hadronic top (all negative sign leptons)
    TH1F *fHistHadronicTop_pt_minus;  // Reconstructed Pt of the hadronic top (all negative sign leptons)

    // neutrino reconstruction histograms - leptonic top
    // all leptons
    TH1F *fHistLeptonicTopNuReco_phi;       // Reconstructed azimuth angle of the neutrino (all leptons)
    TH1F *fHistLeptonicTopNuReco_eta;       // Reconstructed Eta of the neutrino (all leptons)
    TH1F *fHistLeptonicTopNuReco_E;         // Reconstructed energy of the neutrino (all leptons)
    TH1F *fHistLeptonicTopNuReco_pt;        // Reconstructed Pt of the neutrino (all leptons)
    // electrons
    TH1F *fHistLeptonicTopNuReco_phi_e;       // Reconstructed azimuth angle of the neutrino (el channel)
    TH1F *fHistLeptonicTopNuReco_eta_e;       // Reconstructed Eta of the neutrino (el channel)
    TH1F *fHistLeptonicTopNuReco_E_e; // Reconstructed energy of the neutrino (el channel)
    TH1F *fHistLeptonicTopNuReco_pt_e;        // Reconstructed Pt of the neutrino (el channel)
    // muons
    TH1F *fHistLeptonicTopNuReco_phi_mu;       // Reconstructed azimuth angle of the neutrino (mu channel)
    TH1F *fHistLeptonicTopNuReco_eta_mu;       // Reconstructed Eta of the neutrino (mu channel)
    TH1F *fHistLeptonicTopNuReco_E_mu; // Reconstructed energy of the neutrino (mu channel)
    TH1F *fHistLeptonicTopNuReco_pt_mu;        // Reconstructed Pt of the neutrino (mu channel)

    TH1F *fHistLeptonicTopNuReco_phi_plus;  // Reconstructed azimuth angle of the neutrino (e+ channel)
    TH1F *fHistLeptonicTopNuReco_eta_plus;  // Reconstructed Eta of the neutrino (e+ channel)
    TH1F *fHistLeptonicTopNuReco_E_plus; // Reconstructed energy of the neutrino (e+ channel)
    TH1F *fHistLeptonicTopNuReco_pt_plus;   // Reconstructed Pt of the neutrino (e+ channel)
    TH1F *fHistLeptonicTopNuReco_phi_minus; // Reconstructed azimuth angle of the neutrino (e- channel)
    TH1F *fHistLeptonicTopNuReco_eta_minus; // Reconstructed Eta of the neutrino (e- channel)
    TH1F *fHistLeptonicTopNuReco_E_minus; // Reconstructed energy of the neutrino (e- channel)
    TH1F *fHistLeptonicTopNuReco_pt_minus;  // Reconstructed Pt of the neutrino (e- channel)

    TH1F *fHistLeptonicTopNuReco_phi_e_plus;  // Reconstructed azimuth angle of the neutrino (e+ channel)
    TH1F *fHistLeptonicTopNuReco_eta_e_plus;  // Reconstructed Eta of the neutrino (e+ channel)
    TH1F *fHistLeptonicTopNuReco_E_e_plus; // Reconstructed energy of the neutrino (e+ channel)
    TH1F *fHistLeptonicTopNuReco_pt_e_plus;   // Reconstructed Pt of the neutrino (e+ channel)
    TH1F *fHistLeptonicTopNuReco_phi_e_minus; // Reconstructed azimuth angle of the neutrino (e- channel)
    TH1F *fHistLeptonicTopNuReco_eta_e_minus; // Reconstructed Eta of the neutrino (e- channel)
    TH1F *fHistLeptonicTopNuReco_E_e_minus; // Reconstructed energy of the neutrino (e- channel)
    TH1F *fHistLeptonicTopNuReco_pt_e_minus;  // Reconstructed Pt of the neutrino (e- channel)

    TH1F *fHistLeptonicTopNuReco_phi_mu_plus;  // Reconstructed azimuth angle of the neutrino (mu+ channel)
    TH1F *fHistLeptonicTopNuReco_eta_mu_plus;  // Reconstructed Eta of the neutrino (mu+ channel)
    TH1F *fHistLeptonicTopNuReco_E_mu_plus; // Reconstructed energy of the neutrino (mu+ channel)
    TH1F *fHistLeptonicTopNuReco_pt_mu_plus;   // Reconstructed Pt of the neutrino (mu+ channel)
    TH1F *fHistLeptonicTopNuReco_phi_mu_minus; // Reconstructed azimuth angle of the neutrino (mu- channel)
    TH1F *fHistLeptonicTopNuReco_eta_mu_minus; // Reconstructed Eta of the neutrino (mu- channel)
    TH1F *fHistLeptonicTopNuReco_E_mu_minus; // Reconstructed energy of the neutrino (mu- channel)
    TH1F *fHistLeptonicTopNuReco_pt_mu_minus;  // Reconstructed Pt of the neutrino (mu- channel)

    // chi-square and chi-square probability distributions for e, mu channels and both
    //- from leptonic top decay
    TH1F * fHistChi2NDOF_both;
    TH1F * fHistChi2Prob_both_after;
    TH1F * fHistChi2_both_after;
    TH1F * fHistChi2NDOF_both_after;
    TH1F * fHistLeptonicTopChi2NDOF_both;
    TH1F * fHistLeptonicTopChi2_both_after;
    TH1F * fHistLeptonicTopChi2Prob_both_after;
    TH1F * fHistLeptonicTopChi2NDOF_both_after;
    TH1F * fHistHadronicTopChi2NDOF_both;
    TH1F * fHistHadronicTopChi2_both_after;
    TH1F * fHistHadronicTopChi2Prob_both_after;
    TH1F * fHistHadronicTopChi2NDOF_both_after;

    TH1F *fHistLeptonicTopChi2_both;          // chi-square of both W decays
    TH1F *fHistLeptonicTopChi2Prob_both;      // chi-square probability of both W decays
    TH1F *fHistLeptonicTopChi2ProbTTBarVeto;  // chi-square probability for TTBar Veto and Wt reconstruction
                                              // with a leptonic top decay
    TH2F *fHistLeptonicTopChi2ProbWtAndTTBar; // histogram for chi-square probability of Wt reconstruction
                                              // (with leptonic top decay) and of TTBar veto reconstruction
    TH1F *fHistLeptonicTopChi2_e;             // chi-square of e channel
    TH1F *fHistLeptonicTopChi2Prob_e;         // chi-square probability of e channel
    TH1F *fHistLeptonicTopChi2_mu;            // chi-square of mu channel
    TH1F *fHistLeptonicTopChi2Prob_mu;        // chi-square probability of mu channel
    TH2F *fHist_LeptonicTopChi2Prob_vs_M_invar_W;   // 2D: chi-square probability vs invariant mass of W-boson
    TH2F *fHist_LeptonicTopChi2Prob_vs_M_invar_top; // 2D: chi-square probability vs invariant mass of top-quark
    // chi-square and chi-square probability distributions
    // - from hadronic top decay
    TH1F *fHistHadronicTopChi2_both;          // chi-square of both W decays
    TH1F *fHistHadronicTopChi2Prob_both;      // chi-square probability of both W decays
    TH1F *fHistHadronicTopChi2ProbTTBarVeto;  // chi-square probability for TTBar Veto and Wt reconstruction
                                              // with a hadronic top decay
    TH2F *fHistHadronicTopChi2ProbWtAndTTBar; // histogram for chi-square probability of Wt reconstruction
                                              // (with hadronic top decay) and of TTBar veto reconstruction
    TH1F *fHistHadronicTopChi2_e;             // chi-square of electronic W decays
    TH1F *fHistHadronicTopChi2Prob_e;         // chi-square probability of electronic W decays
    TH1F *fHistHadronicTopChi2_mu;            // chi-square of muonic W decays
    TH1F *fHistHadronicTopChi2Prob_mu;        // chi-square probability of muonic W decays
    TH2F *fHist_HadronicTopChi2Prob_vs_M_invar_W; // 2D: chi-square probability vs invariant mass of W-boson
    TH2F *fHist_HadronicTopChi2Prob_vs_M_invar_top; // 2D: chi-square probability vs invariant mass of top-quark

    TH1F *fHistChi2Prob_both;      // chi-square probability for leptonic and hadronic top
                                   // decays. This is the sum of the corresponding histograms
                                   // for the two semileptonic decay modes.
    TH1F *fHistChi2_both;          // corresponding chi-square histogram
    TH1F *fHistChi2Red_both;
    // After ProbChi2 cut ///////////////////////////////////////////////////////////////////////////////////////

    // chi-square and chi-square probability distributions for e, mu channels and both
    //- from leptonic top decay
    TH1F *fHistLeptonicTopChi2_both_cut;          // chi-square of both W decays
    TH1F *fHistLeptonicTopChi2Prob_both_cut;      // chi-square probability of both W decays
    TH1F *fHistLeptonicTopChi2ProbTTBarVeto_cut;  // chi-square probability for TTBar Veto and Wt reconstruction
                                              // with a leptonic top decay
    TH2F *fHistLeptonicTopChi2ProbWtAndTTBar_cut; // histogram for chi-square probability of Wt reconstruction
                                              // (with leptonic top decay) and of TTBar veto reconstruction
    TH1F *fHistLeptonicTopChi2_e_cut;             // chi-square of e channel
    TH1F *fHistLeptonicTopChi2Prob_e_cut;         // chi-square probability of e channel
    TH1F *fHistLeptonicTopChi2_mu_cut;            // chi-square of mu channel
    TH1F *fHistLeptonicTopChi2Prob_mu_cut;        // chi-square probability of mu channel
    TH2F *fHist_LeptonicTopChi2Prob_vs_M_invar_W_cut;   // 2D: chi-square probability vs invariant mass of W-boson
    TH2F *fHist_LeptonicTopChi2Prob_vs_M_invar_top_cut; // 2D: chi-square probability vs invariant mass of top-quark
    // chi-square and chi-square probability distributions
    // - from hadronic top decay
    TH1F *fHistHadronicTopChi2_both_cut;          // chi-square of both W decays
    TH1F *fHistHadronicTopChi2Prob_both_cut;      // chi-square probability of both W decays
    TH1F *fHistHadronicTopChi2ProbTTBarVeto_cut;  // chi-square probability for TTBar Veto and Wt reconstruction
                                              // with a hadronic top decay
    TH2F *fHistHadronicTopChi2ProbWtAndTTBar_cut; // histogram for chi-square probability of Wt reconstruction
                                              // (with hadronic top decay) and of TTBar veto reconstruction
    TH1F *fHistHadronicTopChi2_e_cut;             // chi-square of electronic W decays
    TH1F *fHistHadronicTopChi2Prob_e_cut;         // chi-square probability of electronic W decays
    TH1F *fHistHadronicTopChi2_mu_cut;            // chi-square of muonic W decays
    TH1F *fHistHadronicTopChi2Prob_mu_cut;        // chi-square probability of muonic W decays
    TH2F *fHist_HadronicTopChi2Prob_vs_M_invar_W_cut; // 2D: chi-square probability vs invariant mass of W-boson
    TH2F *fHist_HadronicTopChi2Prob_vs_M_invar_top_cut; // 2D: chi-square probability vs invariant mass of top-quark

    TH1F *fHistChi2Prob_both_cut;      // chi-square probability for leptonic and hadronic top
                                   // decays. This is the sum of the corresponding histograms
                                   // for the two semileptonic decay modes.
    TH1F *fHistChi2_both_cut;          // corresponding chi-square histogram

    // end ///////////////////////////////////////////////////////////////////////////////////////////////////////

    // Truth Matching histograms
    // all histograms show the reconstructed value (e.g. pt) minus
    // the true value over the true value

    TH1I *fHistLeptonicTopRecoTrueChannel; // histograms to compare reconstructed channel
                                           // with true channel in case of reconstructed
                                           // leptonic top decay
    TH1I *fHistHadronicTopRecoTrueChannel; // histograms to compare reconstructed channel
                                           // with true channel in case of reconstructed
                                           // leptonic top decay

    // charged lepton
    // truth matching with DeltaR and DeltaPtFraction
    TH1F *fHistLeptonicTopElecTruthMatch_pt;     // pt of Electron, leptonic top decay
    TH1F *fHistLeptonicTopElecTruthMatch_eta;    // eta of Electron, leptonic top decay
    TH1F *fHistLeptonicTopElecTruthMatch_phi;    // phi of Electron, leptonic top decay
    TH1F *fHistLeptonicTopMuTruthMatch_pt;       // pt of Muon, leptonic top decay
    TH1F *fHistLeptonicTopMuTruthMatch_eta;      // eta of Muon, leptonic top decay
    TH1F *fHistLeptonicTopMuTruthMatch_phi;      // phi of Muon, leptonic top decay

    // Neutrino
    TH1F *fHistLeptonicTopNuTruthMatch_pt;       // pt of Neutrino, electron- and muon-channel,
                                                 // leptonic top decay
    TH1F *fHistLeptonicTopNuTruthMatch_theta;    // theta of Neutrino, electron- and muon-channel,
                                                 // leptonic top decay
    TH1F *fHistLeptonicTopNuTruthMatch_eta;      // eta of Neutrino, electron- and muon-channel,
                                                 // leptonic top decay
    TH1F *fHistLeptonicTopNuTruthMatch_phi;      // phi of Neutrino, electron- and muon-channel,
                                                 // leptonic top decay

    // b-quark
    // matching of the jet done w.r.t. the MC b-quark
    TH1F *fHistLeptonicTopBottomTruthMatch_pt;   // pt of b-quark, electron- and muon-channel,
                                                 // leptonic top decay
    TH1F *fHistLeptonicTopBottomTruthMatch_eta;  // eta of b-quark, electron- and muon-channel,
                                                 // leptonic top decay
    TH1F *fHistLeptonicTopBottomTruthMatch_phi;  // phi of b-quark, electron- and muon-channel,
                                                 // leptonic top decay
    TH1F *fHistHadronicTopBottomTruthMatch_pt;   // pt of b-quark, electron- and muon-channel,
                                                 // hadronic top decay
    TH1F *fHistHadronicTopBottomTruthMatch_eta;  // eta of b-quark, electron- and muon-channel,
                                                 // hadronic top decay
    TH1F *fHistHadronicTopBottomTruthMatch_phi;  // phi of b-quark, electron- and muon-channel,
                                                 // hadronic top decay

    // matching of the jet w.r.t. the hadron level jet
    // which matches the true b quark best
    TH1F *fHistLeptonicTopBHJetTruthMatch_pt;    // pt of b-jet, electron- and muon-channel,
                                                 // leptonic top decay
    TH1F *fHistLeptonicTopBHJetTruthMatch_eta;   // eta of b-jet, electron- and muon-channel,
                                                 // leptonic top decay
    TH1F *fHistLeptonicTopBHJetTruthMatch_phi;   // phi of b-jet, electron- and muon-channel,
                                                 // leptonic top decay
    TH1F *fHistHadronicTopBHJetTruthMatch_pt;    // pt of b-jet, electron- and muon-channel,
                                                 // hadronic top decay
    TH1F *fHistHadronicTopBHJetTruthMatch_eta;   // eta of b-jet, electron- and muon-channel,
                                                 // hadronic top decay
    TH1F *fHistHadronicTopBHJetTruthMatch_phi;   // phi of b-jet, electron- and muon-channel,
                                                 // hadronic top decay

    // matching of jets from hadronic W decay
    // w.r.t. original quarks from W boson decay
    // and w.r.t. hadron level jets matching those
    // quarks best
    TH1F *fHistLeptonicTopWQuarkTruthMatch_pt;  // pt of jets from W decay, electron- and muon-channel,
                                                // leptonic top decay, comparing with quarks
    TH1F *fHistLeptonicTopWQuarkTruthMatch_eta; // eta of jets from W decay, electron- and muon-channel,
                                                // leptonic top decay, comparing with quarks
    TH1F *fHistLeptonicTopWQuarkTruthMatch_phi; // phi of jets from W decay, electron- and muon-channel,
                                                //  top decay, comparing with quarks
    TH1F *fHistHadronicTopWQuarkTruthMatch_pt;  // pt of jets from W decay, electron- and muon-channel,
                                                // hadronic top decay, comparing with quarks
    TH1F *fHistHadronicTopWQuarkTruthMatch_eta; // eta of jets from W decay, electron- and muon-channel,
                                                // hadronic top decay, comparing with quarks
    TH1F *fHistHadronicTopWQuarkTruthMatch_phi; // phi of jets from W decay, electron- and muon-channel,
                                                // hadronic top decay, comparing with quarks
    TH1F *fHistLeptonicTopWHLJetTruthMatch_pt;  // pt of jets from W decay, electron- and muon-channel,
                                                // leptonic top decay, comparing with hadron level jets
    TH1F *fHistLeptonicTopWHLJetTruthMatch_eta; // eta of jets from W decay, electron- and muon-channel,
                                                // leptonic top decay, comparing with hadron level jets
    TH1F *fHistLeptonicTopWHLJetTruthMatch_phi; // phi of jets from W decay, electron- and muon-channel,
                                                // leptonic top decay, comparing with hadron level jets
    TH1F *fHistHadronicTopWHLJetTruthMatch_pt;  // pt of jets from W decay, electron- and muon-channel,
                                                // hadronic top decay, comparing with hadron level jets
    TH1F *fHistHadronicTopWHLJetTruthMatch_eta; // eta of jets from W decay, electron- and muon-channel,
                                                // hadronic top decay, comparing with hadron level jets
    TH1F *fHistHadronicTopWHLJetTruthMatch_phi; // phi of jets from W decay, electron- and muon-channel,
                                                // hadronic top decay, comparing with hadron level jets
    TH1F *fHistPullElecPt;
    TH1F *fHistPullElecEta;
    TH1F *fHistPullElecPhi;
    TH1F *fHistPullMuonPt;
    TH1F *fHistPullMuonEta;
    TH1F *fHistPullMuonPhi;
    TH1F *fHistPullEtMissMag;
    TH1F *fHistPullEtMissPhi;
    TH1F *fHistPullBJetPt;
    TH1F *fHistPullBJetEta;
    TH1F *fHistPullBJetPhi;
    TH1F *fHistPullJet1Pt;
    TH1F *fHistPullJet1Eta;
    TH1F *fHistPullJet1Phi;
    TH1F *fHistPullJet2Pt;
    TH1F *fHistPullJet2Eta;
    TH1F *fHistPullJet2Phi;

    TH1F *fHistPullElecPt_had;
    TH1F *fHistPullElecEta_had;
    TH1F *fHistPullElecPhi_had;
    TH1F *fHistPullMuonPt_had;
    TH1F *fHistPullMuonEta_had;
    TH1F *fHistPullMuonPhi_had;
    TH1F *fHistPullEtMissMag_had;
    TH1F *fHistPullEtMissPhi_had;
    TH1F *fHistPullBJetPt_had;
    TH1F *fHistPullBJetEta_had;
    TH1F *fHistPullBJetPhi_had;
    TH1F *fHistPullJet1Pt_had;
    TH1F *fHistPullJet1Eta_had;
    TH1F *fHistPullJet1Phi_had;
    TH1F *fHistPullJet2Pt_had;
    TH1F *fHistPullJet2Eta_had;
    TH1F *fHistPullJet2Phi_had;

    TH1F *fHistResidualElecPt;
    TH1F *fHistResidualElecEta;
    TH1F *fHistResidualElecPhi;
    TH1F *fHistResidualMuonPt;
    TH1F *fHistResidualMuonEta;
    TH1F *fHistResidualMuonPhi;
    TH1F *fHistResidualEtMissMag;
    TH1F *fHistResidualEtMissPhi;
    TH1F *fHistResidualBJetPt;
    TH1F *fHistResidualBJetEta;
    TH1F *fHistResidualBJetPhi;
    TH1F *fHistResidualJet1Pt;
    TH1F *fHistResidualJet1Eta;
    TH1F *fHistResidualJet1Phi;
    TH1F *fHistResidualJet2Pt;
    TH1F *fHistResidualJet2Eta;
    TH1F *fHistResidualJet2Phi;

    TH1F *fHistResidualBJetPt_had;
    TH1F *fHistResidualBJetEta_had;
    TH1F *fHistResidualBJetPhi_had;
    TH1F *fHistResidualJet1Pt_had;
    TH1F *fHistResidualJet1Eta_had;
    TH1F *fHistResidualJet1Phi_had;
    TH1F *fHistResidualJet2Pt_had;
    TH1F *fHistResidualJet2Eta_had;
    TH1F *fHistResidualJet2Phi_had;

    // Jet bin counts
    TH1F *fHistJetBins_pretag;
    TH1F *fHistJetBins_tag;
    TH1F *fHistJetBins_fit;

    // theta and phi histograms for ntuple
    TH1F *fhist_fCosPhiAngleWTop;
    TH1F *fhist_fCosPhiAngleAssociateWTop;
    TH1F *fhist_fCosPhiAngleAssociateWW;
    TH1F *fhist_fWFrameCosThetaStar_DeltaWtop_bW;
    TH1F *fhist_fTopFrameCosThetaStar_DeltaWtop_btop;
    TH1F *fhist_fCosThetaStarLeptNeutrino_LeptTop;
    TH1F *fhist_fTopFrameCosThetaStarWb_LeptTop;
    TH1F *fhist_fCosThetaStar1_LeptTop;
    TH1F *fhist_fCosThetaStar2_LeptTop;
    TH1F *fhist_fLogBest;
    TH1F *fhist_fLogBest1;
    TH1F *fhist_fLogBest2;
    TH1F *fhist_fLogBest3;
    TH1F *fhist_fLogBestlast;
    TH1F *fhist_fLogLept;
    TH1F *fhist_fLogHadr;
    TH1F *fhist_fPWtRatio;
    TH1F *fhist_fPWtSum;
    TH1F *fhist_fPWtDiff;
    TH1F *fhist_fPWtAsymmetry;
    TH1F *fhist_fLogPWtRatio;
    TH1F *fhist_fLogPWtSum;
    TH1F *fhist_fLogPWtDiff;
    TH1F *fhist_fLogPWtAsymmetry;
    TH1F *fhist_fLogBest_logscale;
    TH1F *fhist_fLogLept_logscale;
    TH1F *fhist_fLogHadr_logscale;
    TH1F *fhist_fPWtRatio_logscale;
    TH1F *fhist_fPWtSum_logscale;
    TH1F *fhist_fPWtDiff_logscale;
    TH1F *fhist_fPWtAsymmetry_logscale;
    TH1F *fhist_fLogPWtRatio_logscale;
    TH1F *fhist_fLogPWtSum_logscale;
    TH1F *fhist_fLogPWtDiff_logscale;
    TH1F *fhist_fLogPWtAsymmetry_logscale;

    TVector2 p_System;
    Float_t  pT_System;
    TH1F *fHist_WtSystem_Pt_nocut;        // Reconstructed transverse momentum of the W+top system
    TH1F *fHist_WtSystem_Pt;        // Reconstructed transverse momentum of the W+top system
    TH1F *fHist_EtaTop_nocut;
    TH1F *fHist_PtTop_nocut;
    TAxis *axis;

  public:
    Bool_t    fUseLeptonCovRand; // Use randomly generated lepton covariance matrices?
    Bool_t fIsFwdElectronMode; // Use only forward electrons when analyzing jet-electrons? If set to kFALSE, only central electrons will be analyzed.
    Float_t   fBJetWeightMin;     // Minimum b-tag weight for b-jets to be accepted

    // ntuple production
    Bool_t fProduceCommonTree; // Produce common tree for combined NN analysis?
    // variables in ntuple for common analysis
    Double_t fCosPhiAngleWTop;
    Double_t fCosPhiAngleAssociateWTop;
    Double_t fCosPhiAngleAssociateWW;
    Double_t fTopFrame_DThetaStarAssociateWTop;
    Double_t fWFrameCosThetaStar_DeltaWtop_bW;
    Double_t fTopFrameCosThetaStar_DeltaWtop_btop;

    Int_t fRunNumber;
    Int_t fLumiBlock;
    Int_t fEventNumber;
    Float_t fmc_channel_number;
    Float_t ftop_hfor_type;
    Float_t fRapTop;
    Float_t fEtaTop;
    Float_t fEtaWTop;
    Float_t fEtaNeutrino;

    Float_t  fEtaW_LeptTop;
    Double_t fCosThetaStarLeptNeutrino_LeptTop;
    Double_t fTopFrameCosThetaStarWb_LeptTop;
    Double_t fCosThetaStar1_LeptTop;
    Double_t fCosThetaStar2_LeptTop;
    Double_t fLogPWtBest;
    Double_t fLogPWtWorst;
    Double_t fLogPWtHadronic;
    Double_t fLogPWtLeptonic;
    Double_t fPWtRatio;
    Double_t fPWtSum;
    Double_t fPWtDiff;
    Double_t fPWtAsymmetry;
    Double_t fLogPWtRatio;
    Double_t fLogPWtSum;
    Double_t fLogPWtDiff;
    Double_t fLogPWtAsymmetry;

    Bool_t fSignalSampleMode; // Running over signal MC?

    Int_t fMaxNbIter;         // maximum number of iterations

    // Scale Factor systematics mode
    enum EScaleFactorSystematics {
	kNone,
	kElectronUp,
	kElectronDown,
	kMuonUp,
	kMuonDown,
	kJetBTagsUp,
	kJetBTagsDown,
	kJetCTagsUp,
	kJetCTagsDown,
	kJetMisTagsUp,
	kJetMisTagsDown,
	kJVFWeightUp,
	kJVFWeightDown,
	kNumSystModes
    };

    static const Int_t fgNSystNames;
    static const char* fgSystNames[];

    EScaleFactorSystematics fSFSystMode;// Scale Factor systematics mode

    Bool_t fIsAcceptanceChallenge; // Doing acceptance challenge (then write out event list!)?

    Double_t fChi2ProbMin;    // Minimum chi2-probability. Can be used later to apply
                              // the corresponding cut.
                              // So far it is only used in the cut-flow plot
    Bool_t fGuessNeutrinoEtaE;// Use routine to estimate good starting value of neutrino Eta?
    Bool_t fTruthMatch;       // Run truth-matching at all?

    ELeptonMode fLeptonMode;  // Indicate the lepton channel mode (e/mu/tau)

    EModeMass  fModeMass;        // Indicate the mode of mass constraint (Gauss or BW mass constraint)
    EMode      fMode;            // Indicate the mode of run (KinFit or CutBased)

    Int_t fDecayChannel;         // decay channel of Wt event for which the analysis is performed
                                 // 1: leptonic top, hadronic W decay
                                 // 2: hadronic top, leptonic W decay
                                 // 3: always perform analysis
                                 // 0: fully hadronic Wt, dileptonic Wt and Background
                                 // can be set so that the analysis is made only for
                                 // the events in the corresponding channel
    Int_t fTrueDecayChannel;     // corresponding channel of true event in MC
                                 // perform analysis only for equality or if
                                 // fDecayChannel == 3

    // True W and top masses for the fit
    Double_t fW_Mass;     // Pole mass of the W used in  the fit
    Double_t fW_Width;    // Width of the W used in the fit
    Double_t fTop_Mass;   // Pole mass of the t-quark used in the fit
    Double_t fTop_Width;  // Width of the t-quark used in the fit

    // chi^2 values and No. of degrees of freedom for different fits
    Double_t fchi2LeptonicTop;    // chi^2 value for leptonic top decay from KinFitter
    Double_t fchi2HadronicTop;    // chi^2 value for hadronic top decay from KinFitter
    Double_t fchi2WTop;           // chi^2 value for chosen (final) top decay from KinFitter
    Double_t fchi2ttbarBkg;       // chi^2 value for ttbar hypothesis (bkg)
    Int_t fNDoFLeptonicTop;       //  NDoF for leptonic top decay from KinFitter
    Int_t fNDoFHadronicTop;       //  NDoF for hadronic top decay from KinFitter
    Int_t fNDoFWTop;       //  NDoF for chosen top decay from KinFitter
    Int_t fNDoFttbarBkg;          //  NDoF for ttbar hypothesis (bkg)
    Double_t fchi2ProbLeptonicTop;    // chi^2 p-value for leptonic top decay from KinFitter
    Double_t fchi2ProbHadronicTop;    // chi^2 p-value for hadronic top decay from KinFitter
    Double_t fchi2ProbWTop;             // chi^2 p-value for chosen (final) top decay from KinFitter
    Double_t fchi2RedWTop;             // chi^2 p-value for chosen (final) top decay from KinFitter

    Double_t fchi2RedLeptonicTop;
    Double_t fchi2RedHadronicTop;
    Int_t    fNDoFttbarBkg_debug_LNu;
    Double_t fchi2ttbarBkg_debug_LNu;
    Double_t fChi2ProbTTBarBkg;   // chi^2 p-value of the top decays appended by the toppairfinder tool

    // Etmiss cut
    Float_t              fMET_Mag_min; // Minimum magnitude of Etmiss

    // Lepton selection cuts
    Float_t fElPt_Min;        // Minimum signal electron pt
    Float_t fMuPt_Min;        // Minimum signal electron pt

    // Jet selection cuts
    Int_t                fNJetsMin;       // minimal number of jets
    Int_t                fNJetsMax;       // maximum number of jets
    Int_t                fNJetsExcl;       // jets multiplicity bin
    Float_t              fJets_Et_min;      // Min. Et of signal jets
    Float_t              fJets_Eta_min;     // Min. eta of signal jets
    Float_t              fJets_Eta_max;     // Max. eta of signal jets

    // Selection cuts for jets used in hadronic W reconstruction
    Float_t              fWhadJets_Et_min;      // Min. Et of jets from Whad
    Float_t              fWhadJets_Eta_min;     // Min. eta of jets from Whad
    Float_t              fWhadJets_Eta_max;     // Max. eta of jets from Whad
    AtlBTag::ETagger     fWhadJetTagger;        // Tagger used for signal fwd jets
    Float_t              fWhadJetWeightMax;    // Maximum b-tag weight for jets to be accepted as a light fwd jet
    // BJet selection cuts
    AtlBTag::ETagger     fBJetTagger;        // Tagger used for b-jet selection

    Float_t              fBJets_Et_min;      // Min. Pt of signal b-jets
    Float_t              fBJets_Eta_min;     // Min. eta of signal b-jets
    Float_t              fBJets_Eta_max;     // Max. eta of signal b-jets

    // ttbar selection cuts
    Float_t  fVetoTTbarProbMin; // p(chi2) ttbar cut point -------> Not used since ttbar veto is done by p(chi2) comparison

    // Triangular Cut Quantities
    Float_t  fWmassT_min;
    // Transverse W mass
    Double_t fWmassT;
    HepParticle *fRecoTriangularW;        // reconstructed W-boson

  public:
    AtlSgTop_WtChannelFinder(const char* name, const char* title);
    virtual ~AtlSgTop_WtChannelFinder();
    virtual void SetBranchStatus();
    virtual void BookHistograms();
    void CommonHistograms( );
    void LeptonicTopHistograms();
    void HadronicTopHistograms();
    virtual void FillHistograms();
    virtual void SetCutDefaults();
    virtual Bool_t AnalyzeEvent();
    virtual void Terminate();
    virtual void Print() const;
    inline void SetLeptons (TList* list)   { fLeptons = list; }
    inline void SetJets   (TList* list)    { fJets = list; }
    inline void SetEtmiss (TVector2 met)   { fMET = met; }
    void ReconstructW();

  private:

    // Event selection histograms
    void BookLeptonHistograms();
    void BookJetHistograms();

    void FillLeptonHistograms();
    void FillJetHistograms();
    void FillBJetHistograms();
    void FillNonBJetHistograms();

    // Performance study histograms
    void BookComparisonHistogramsLeptTopFit();
    void BookComparisonHistogramsHadrTopFit();
    void FillComparisonHistograms();

    void BookLeptonHistograms_BTag();
    void BookJetHistograms_BTag();
    void FillLeptonHistograms_BTag();
    void FillJetHistograms_BTag();
    void FillBJetHistograms_BTag();
    void FillNonBJetHistograms_BTag();
    //void FillPreAfterBTagHistograms();
    void InitEvent();
    void SetNeutrinoStartingValues();
    void ReconstructionKinFit();
    void ReconstructionCutBased();
    void LeptonicTopKinFitReco();
    void HadronicTopKinFitReco();
    void DoLeptonicTopFit();
    void DoHadronicTopFit();
    Bool_t ApplyTTbarVeto(Double_t chi2probMin, AtlJet* BestBJet);
    Int_t GetDecayChannel();
    Int_t GetTrueDecayChannel();

    ClassDef(AtlSgTop_WtChannelFinder,0)  // ATLAS single top Wt-channel finder
};
#endif

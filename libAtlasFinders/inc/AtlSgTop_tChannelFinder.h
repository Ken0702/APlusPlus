//
// Author: Patrick Rieck <mailto:rieck@physik.hu-berlin.de>
// Copyright: 2009 (C) Patrick Rieck
//
#ifndef ATLAS_AtlSgTop_tChannelFinder
#define ATLAS_AtlSgTop_tChannelFinder
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
class TH2F;
class AtlEvent;
class TTree;
class TString;
class TROOT;
class TFile;

//____________________________________________________________________

class AtlSgTop_tChannelFinder : public AtlKinFitterTool {


  private:
    std::ofstream fOut; // write text file with event info for acceptance challenges
    AtlCutFlowTool         *fCutflow_tool;
    AtlCutFlowTool         *fCutflow_tool_2;

    TKinFitter *fKinFitter_Whad; // KinFitter object for reco of hadron. W (Whad veto)

    TList *fLeptons;           // List of signal leptons
    TList *fJets;           // List of jets
    TList *fWhadJets;       // List of jets for hadronic W reconstruction
    TList *fBJets;          // List of b-jets
    TList *fBJets_refQCD;   // List of b-jets (reference with b-tag cut used on all other samples;
                            // only filled on jet-electrons)
    TList *fNonBJets;       // List of non b jets
    TList *fFwdJets;        // List of fwd jets
    AtlJet *fLeadFwdJet;    // Leading fwd jet

    Int_t fNJets;           // number of jets in generic list

    TVector2 fMET;          // MissingEt of current event
    Float_t fMET_Mag;       // Magnitude of MissingEt of current event
    Float_t fMET_Phi;       // Phi of MissingEt of current event
    Float_t fH_T;           // scalar sum of pt of all objects (lepton, missEt, jets)
    Double_t fMassAllJets;   // invariant mass of all jets in event
    Float_t fLeadNonBJetEta; // eta of hardest un-tagged jet

    HepParticle *fLepton;     // Signal lepton of current event
    HepParticle *fNeutrino;   // Current neutrino (missing Et) used as input for the fit

    // Reconstructed W and top-quark (KinFitter analysis)
    AtlWDecayLNu *fWBoson;        // Top-quark object from KinFitter reco
    HepTopDecay  *fTop;        // Top-quark object from KinFitter reco

    // Reconstructed W and top-quark (cut based analysis)
    HepParticle *fRecoW;      // Pointer to reconstructed W
    Float_t      fWmassT;     // Reconstructed transverse W mass
    HepParticle *fRecoTop;    // Pointer to reconstructed top-quark

    AtlJet      *fBJet;       // Current b-jet candidate used as input for the fit
    AtlJet      *fWJet1;      // jet 1 used in ApplyWhadVeto() for hadronic W reconstruction
    AtlJet      *fWJet2;      // jet 2 used in ApplyWhadVeto() for hadronic W reconstruction

    TMatrixD fCovLepton;    // Covariance matrix of the charged lepton from the W decay
    TMatrixD fCovNeutrino;  // Covariance matrix of the neutrino from the W decay
    TMatrixD fCovBJet;      // Covariance matrix of the b-jet candidate from the t decay
    TMatrixD fCovWJet1;     // Covariance matrix of the 1st jet from W-decay (Whad veto)
    TMatrixD fCovWJet2;     // Covariance matrix of the 2nd jet from W-decay (Whad veto)

    TLorentzVector fLeptonP_refit_cur;    // Refitted lepton 4-mom. of current best combination inside fit
    TLorentzVector fNeutrinoP_refit_cur;  // Refitted neutrino 4-mom. of current best combination inside fit
    TLorentzVector fBJetP_refit_cur;      // Refitted b-jet 4-mom. of current best combination inside fit

    TLorentzVector fJet1P_refit_cur;    // Refitted 4-mom. of current first jet inside Whad veto fit
    TLorentzVector fJet2P_refit_cur;  // Refitted 4-mom. of current second jet inside Whad veto fit

    Double_t fChi2Whad;          // Chi square of hadronic W reconstruction
                                 // done in ApplyWhadVeto()
    Int_t    fNDoFWhad;          // No. of D.o.F. of hadronic W kinematic fit
                                 // done in ApplyWhadVeto()
    Double_t fChi2ProbWhad;      // P(Chi2) of hadronic W Reco
    Int_t fN_Converged;          // No. of fits that DID converge in this event
    Int_t fN_NotConverged;       // No. of fits that DID NOT converge in this event
    Int_t fN_FailNumeric;        // No. of fits that failed numerically in this event
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
    Float_t fMassConstraintParameterW;     // mass term of W boson

    Float_t fPullLeptonPt;   // Pull of lepton Pt
    Float_t fPullLeptonEta;  // Pull of lepton eta
    Float_t fPullLeptonPhi;  // Pull of lepton phi
    Float_t fPullEtMissMag;  // Pull of missing Et magnitude
    Float_t fPullEtMissPhi;  // Pull of missing Et phi
    Float_t fPullBJetPt;     // Pull of b-jet Pt
    Float_t fPullBJetEta;    // Pull of b-jet eta
    Float_t fPullBJetPhi;    // Pull of b-jet phi
    Float_t fPullJet1Pt;     // Pull of jet 1 Pt
    Float_t fPullJet1Eta;    // Pull of jet 1 eta
    Float_t fPullJet1Phi;    // Pull of jet 1 phi
    Float_t fPullJet2Pt;     // Pull of jet 2 Pt
    Float_t fPullJet2Eta;    // Pull of jet 2 eta
    Float_t fPullJet2Phi;    // Pull of jet 2 phi

   // W reconstruction histograms
    TH1F *fHistW_Mt_QCDValidation_pretag;//transverse W mass histograms for validation of QCD estimate (pretag)
    TH1F *fHistW_Mt_EtmissMag_QCDValidation_pretag; //triangular cut on (transverse W mass+Etmiss) histograms for validation of QCD estimate (pretag)
    TH1F *fHistW_Mt_QCDValidation_tag;//transverse W mass histograms for validation of QCD estimate (tag)
    TH1F *fHistW_Mt_EtmissMag_QCDValidation_tag; //triangular cut on (transverse W mass+Etmiss) histograms for validation of QCD estimate (tag)
    // Pile-up control histograms
    TH1F *fHist_AverageIntPerXing_pretag;; // average interactions per bunch-crossing (pretag)
    TH1F *fHist_AverageIntPerXing_tag;; // average interactions per bunch-crossing (tag)
    TH1F *fHist_NPrimaryVertices_pretag;  // number of primary vertices (pretag)
    TH1F *fHist_NPrimaryVertices_tag;  // number of primary vertices (tag)

    TH1F *fHistWreco_Mt;        // Reconstructed invariant transverse mass of the W (all leptons)
    TH1F *fHistWreco_M;        // Reconstructed invariant mass of the W (all leptons)
    TH1F *fHistWreco_phi;       // Reconstructed azimuth angle of the W (all leptons)
    TH1F *fHistWreco_eta;       // Reconstructed Eta of the W (all leptons)
    TH1F *fHistWreco_rap;       // Reconstructed y of the W (all leptons)
    TH1F *fHistWreco_pt;        // Reconstructed Pt of the W (all leptons)
    TH1F *fHistWreco_Mt_minus;  // Reconstructed invariant transverse mass of the W
    TH1F *fHistWreco_M_minus;  // Reconstructed invariant mass of the W
                                // (all negative sign leptons)
    TH1F *fHistWreco_phi_minus; // Reconstructed azimuth angle of the W (all negative sign leptons)
    TH1F *fHistWreco_eta_minus; // Reconstructed Eta of the W (all negative sign leptons)
    TH1F *fHistWreco_rap_minus; // Reconstructed y of the W (all negative sign leptons)
    TH1F *fHistWreco_pt_minus;  // Reconstructed Pt of the W (all negative sign leptons)
    TH1F *fHistWreco_Mt_plus;   // Reconstructed invariant transverse mass of the W
                                // (all positive sign leptons)
    TH1F *fHistWreco_M_plus;   // Reconstructed invariant mass of the W
                                // (all positive sign leptons)
    TH1F *fHistWreco_phi_plus;  // Reconstructed azimuth angle of the W (all positive sign leptons)
    TH1F *fHistWreco_eta_plus;  // Reconstructed Eta of the W (all positive sign leptons)
    TH1F *fHistWreco_rap_plus;  // Reconstructed y of the W (all positive sign leptons)
    TH1F *fHistWreco_pt_plus;   // Reconstructed Pt of the W (all positive sign leptons)

    TH1F *fHistWreco_Mt_e;        // Reconstructed invariant transverse mass of the W decaying
                                  // into electrons
    TH1F *fHistWreco_M_e;        // Reconstructed invariant mass of the W decaying
                                  // into electrons
    TH1F *fHistWreco_phi_e;       // Reconstructed azimuth angle of the (real) W decaying into electrons
    TH1F *fHistWreco_eta_e;       // Reconstructed Eta of the W decaying into electrons
    TH1F *fHistWreco_rap_e;       // Reconstructed y of the (real) W decaying into electrons
    TH1F *fHistWreco_pt_e;        // Reconstructed Pt of the W decaying into electrons
    TH1F *fHistWreco_Mt_e_minus;  // Reconstructed invariant transverse mass of the W decaying into e-
    TH1F *fHistWreco_M_e_minus;  // Reconstructed invariant mass of the W decaying into e-
    TH1F *fHistWreco_phi_e_minus; // Reconstructed azimuth angle of the (real) W decaying into e-
    TH1F *fHistWreco_eta_e_minus; // Reconstructed Eta of the W decaying into e-
    TH1F *fHistWreco_rap_e_minus; // Reconstructed y of the (real) W decaying into e-
    TH1F *fHistWreco_pt_e_minus;  // Reconstructed Pt of the W decaying into e-
    TH1F *fHistWreco_Mt_e_plus;   // Reconstructed invariant transverse mass of the W decaying into e+
    TH1F *fHistWreco_M_e_plus;   // Reconstructed invariant mass of the W decaying into e+
    TH1F *fHistWreco_phi_e_plus;  // Reconstructed azimuth angle of the (real) W decaying into e+
    TH1F *fHistWreco_eta_e_plus;  // Reconstructed Eta of the W decaying into e+
    TH1F *fHistWreco_rap_e_plus;  // Reconstructed y of the (real) W decaying into e+
    TH1F *fHistWreco_pt_e_plus;   // Reconstructed Pt of the W decaying into e+

    TH1F *fHistWreco_Mt_mu;        // Reconstructed invariant transverse mass of the W decaying into muons
    TH1F *fHistWreco_M_mu;         // Reconstructed invariant mass of the W decaying into muons
    TH1F *fHistWreco_phi_mu;       // Reconstructed azimuth angle of the W decaying into muons
    TH1F *fHistWreco_eta_mu;       // Reconstructed Eta of the W decaying into muons
    TH1F *fHistWreco_rap_mu;       // Reconstructed y of the W decaying into muons
    TH1F *fHistWreco_pt_mu;        // Reconstructed Pt of the W decaying into muons
    TH1F *fHistWreco_Mt_mu_minus;  // Reconstructed invariant transverse mass of the W decaying into mu-
    TH1F *fHistWreco_M_mu_minus;   // Reconstructed invariant mass of the W decaying into mu-
    TH1F *fHistWreco_phi_mu_minus; // Reconstructed azimuth angle of the W decaying into mu-
    TH1F *fHistWreco_eta_mu_minus; // Reconstructed Eta of the W decaying into mu-
    TH1F *fHistWreco_rap_mu_minus; // Reconstructed y of the W decaying into mu-
    TH1F *fHistWreco_pt_mu_minus;  // Reconstructed Pt of the W decaying into mu-
    TH1F *fHistWreco_Mt_mu_plus;   // Reconstructed invariant transverse mass of the W decaying into mu+
    TH1F *fHistWreco_M_mu_plus;    // Reconstructed invariant mass of the W decaying into mu+
    TH1F *fHistWreco_phi_mu_plus;  // Reconstructed azimuth angle of the W decaying into mu+
    TH1F *fHistWreco_eta_mu_plus;  // Reconstructed Eta of the W decaying into mu+
    TH1F *fHistWreco_rap_mu_plus;  // Reconstructed y of the W decaying into mu+
    TH1F *fHistWreco_pt_mu_plus;   // Reconstructed Pt of the W decaying into mu+

    // hadronic W reconstruction histograms
    // these are for the output of ApplyWhadVeto()
    TH1F *fHistWHadReco_Mt;        // Reconstructed invariant transverse mass of the hadronic W
    TH1F *fHistWHadReco_M;         // Reconstructed invariant mass of the hadronic W
    TH1F *fHistWHadReco_phi;       // Reconstructed azimuth angle of the hadronic W
    TH1F *fHistWHadReco_rap;       // Reconstructed rapidity of the hadronic W
    TH1F *fHistWHadReco_pt;        // Reconstructed Pt of the hadronic W
    TH1F *fHistWHadReco_chi2;      // chi square of hadronic W reconstruction
    TH1F *fHistWHadReco_chi2prob;  // chi square prob. of hadronic W reconstruction

    // top quark reconstruction histograms
    TH1F *fHistTopReco_Mt;        // Reconstructed invariant transverse mass of the single-top
                                  // (all leptons)
    TH1F *fHistTopReco_M;        // Reconstructed invariant mass of the single-top
	                          // (all leptons)
    TH1F *fHistTopReco_phi;       // Reconstructed azimuth angle of the single-top (all leptons)
    TH1F *fHistTopReco_eta;       // Reconstructed Eta of the single-top (all leptons)
    TH1F *fHistTopReco_DeltaEtaFwdJet; // Eta difference between recon. top and fwd jet (=leading non-b-jet)
    TH1F *fHistTopReco_DeltaPhiFwdJet; // Phi difference between recon. top and fwd jet (=leading non-b-jet)
    TH1F *fHistTopReco_SumPxPyFwdJet; //length of vector-addition of P in x-y-plane
    TH1F *fHistTopReco_DeltaP3FwdJet; //substract |P3_Top|-|P3_FwdJet|
    TH1F *fHistTopReco_DeltaPxFwdJet; //substract Px_Top-Px_FwdJet
    TH1F *fHistTopReco_DeltaPyFwdJet; //substract Py_Top-Py_FwdJet
    TH1F *fHistTopReco_rap;       // Reconstructed y of the single-top (all leptons)
    TH1F *fHistTopReco_pt;        // Reconstructed Pt of the single-top (all leptons)
    TH1F *fHistTopReco_Amount_P3; // Reconstructed Amount of P3 of single-top (all leptons)
    TH1F *fHistTopReco_Mt_plus;   // Reconstructed invariant transverse mass of the single-top
                                  // (all positive sign leptons)
    TH1F *fHistTopReco_M_plus;    // Reconstructed invariant mass of the single-top
                                  // (all positive sign leptons)
    TH1F *fHistTopReco_phi_plus;  // Reconstructed azimuth angle of the single-top
                                  // (all positive sign leptons)
    TH1F *fHistTopReco_eta_plus;  // Reconstructed Eta of the single-top (all positive sign leptons)
    TH1F *fHistTopReco_rap_plus;  // Reconstructed y of the single-top (all positive sign leptons)
    TH1F *fHistTopReco_pt_plus;   // Reconstructed Pt of the single-top (all positive sign leptons)
    TH1F *fHistTopReco_Mt_minus;  // Reconstructed invariant transverse mass of the single-top
                                  // (all negative sign leptons)
    TH1F *fHistTopReco_M_minus;   // Reconstructed invariant mass of the single-top
	                          // (all negative sign leptons)
    TH1F *fHistTopReco_phi_minus; // Reconstructed azimuth angle of the single-top
                                  // (all negative sign leptons)
    TH1F *fHistTopReco_eta_minus; // Reconstructed Eta of the single-top (all negative sign leptons)
    TH1F *fHistTopReco_rap_minus; // Reconstructed y of the single-top (all negative sign leptons)
    TH1F *fHistTopReco_pt_minus;  // Reconstructed Pt of the single-top (all negative sign leptons)

    TH1F *fHistTopReco_Mt_e;        // Reconstructed invariant transverse mass of the single-top (e channel)
    TH1F *fHistTopReco_M_e;         // Reconstructed invariant mass of the single-top (e channel)
    TH1F *fHistTopReco_phi_e;       // Reconstructed azimuth angle of the single-top (e channel)
    TH1F *fHistTopReco_eta_e;       // Reconstructed Eta of the single-top (e channel)
    TH1F *fHistTopReco_rap_e;       // Reconstructed y of the top (e channel)
    TH1F *fHistTopReco_pt_e;        // Reconstructed Pt of the single-top (e channel)
    TH1F *fHistTopReco_Mt_e_plus;   // Reconstructed invariant transverse mass of the single-top
                                    // (e+ channel)
    TH1F *fHistTopReco_M_e_plus;    // Reconstructed invariant mass of the single-top
                                    // (e+ channel)
    TH1F *fHistTopReco_phi_e_plus;  // Reconstructed azimuth angle of the single-top (e+ channel)
    TH1F *fHistTopReco_eta_e_plus;  // Reconstructed Eta of the single-top (e+ channel)
    TH1F *fHistTopReco_rap_e_plus;  // Reconstructed y of the single-top (e+ channel)
    TH1F *fHistTopReco_pt_e_plus;   // Reconstructed Pt of the single-top (e+ channel)
    TH1F *fHistTopReco_Mt_e_minus;  // Reconstructed invariant transverse mass of the single-top
                                    // (e- channel)
    TH1F *fHistTopReco_M_e_minus;   // Reconstructed invariant mass of the single-top
                                    // (e- channel)
    TH1F *fHistTopReco_phi_e_minus; // Reconstructed azimuth angle of the single-top (e- channel)
    TH1F *fHistTopReco_eta_e_minus; // Reconstructed Eta of the single-top (e- channel)
    TH1F *fHistTopReco_rap_e_minus; // Reconstructed y of the single-top (e- channel)
    TH1F *fHistTopReco_pt_e_minus;  // Reconstructed Pt of the single-top (e- channel)

    TH1F *fHistTopReco_Mt_mu;        // Reconstructed invariant transverse mass of the single-top
                                     // (mu channel)
    TH1F *fHistTopReco_M_mu;         // Reconstructed invariant mass of the single-top
                                     // (mu channel)
    TH1F *fHistTopReco_phi_mu;       // Reconstructed azimuth angle of the single-top (mu channel)
    TH1F *fHistTopReco_eta_mu;       // Reconstructed Eta of the single-top (mu channel)
    TH1F *fHistTopReco_rap_mu;       // Reconstructed y of the single-top (mu channel)
    TH1F *fHistTopReco_pt_mu;        // Reconstructed Pt of the single-top (mu channel)
    TH1F *fHistTopReco_Mt_mu_plus;   // Reconstructed invariant transverse mass of the single-top
                                     // (mu+ channel)
    TH1F *fHistTopReco_M_mu_plus;    // Reconstructed invariant mass of the single-top
	                             // (mu+ channel)
    TH1F *fHistTopReco_phi_mu_plus;  // Reconstructed azimuth angle of the single-top (mu+ channel)
    TH1F *fHistTopReco_eta_mu_plus;  // Reconstructed Eta of the single-top (mu+ channel)
    TH1F *fHistTopReco_rap_mu_plus;  // Reconstructed y of the single-top (mu+ channel)
    TH1F *fHistTopReco_pt_mu_plus;   // Reconstructed Pt of the single-top (mu+ channel)
    TH1F *fHistTopReco_Mt_mu_minus;  // Reconstructed invariant transverse mass of the single-top
                                     // (mu- channel)
    TH1F *fHistTopReco_M_mu_minus;   // Reconstructed invariant mass of the single-top
                                     // (mu- channel)
    TH1F *fHistTopReco_phi_mu_minus; // Reconstructed azimuth angle of the single-top (mu- channel)
    TH1F *fHistTopReco_eta_mu_minus; // Reconstructed Eta of the single-top (mu- channel)
    TH1F *fHistTopReco_rap_mu_minus; // Reconstructed y of the single-top (mu- channel)
    TH1F *fHistTopReco_pt_mu_minus;  // Reconstructed Pt of the single-top (mu- channel)

    // neutrino reconstruction histograms
    TH1F *fHistNuReco_phi;       // Reconstructed azimuth angle of the neutrino (el channel)
    TH1F *fHistNuReco_eta;       // Reconstructed Eta of the neutrino (el channel)
    TH1F *fHistNuReco_E;         // Reconstructed energy of the neutrino (el channel)
    TH1F *fHistNuReco_pt;        // Reconstructed Pt of the neutrino (el channel)
    TH1F *fHistNuReco_phi_plus;  // Reconstructed azimuth angle of the neutrino (e+ channel)
    TH1F *fHistNuReco_eta_plus;  // Reconstructed Eta of the neutrino (e+ channel)
    TH1F *fHistNuReco_E_plus;    // Reconstructed energy of the neutrino (e+ channel)
    TH1F *fHistNuReco_pt_plus;   // Reconstructed Pt of the neutrino (e+ channel)
    TH1F *fHistNuReco_phi_minus; // Reconstructed azimuth angle of the neutrino (e- channel)
    TH1F *fHistNuReco_eta_minus; // Reconstructed Eta of the neutrino (e- channel)
    TH1F *fHistNuReco_E_minus;   // Reconstructed energy of the neutrino (e- channel)
    TH1F *fHistNuReco_pt_minus;  // Reconstructed Pt of the neutrino (e- channel)

    TH1F *fHistNuReco_phi_e;       // Reconstructed azimuth angle of the neutrino (el channel)
    TH1F *fHistNuReco_eta_e;       // Reconstructed Eta of the neutrino (el channel)
    TH1F *fHistNuReco_E_e;         // Reconstructed energy of the neutrino (el channel)
    TH1F *fHistNuReco_pt_e;        // Reconstructed Pt of the neutrino (el channel)
    TH1F *fHistNuReco_phi_e_plus;  // Reconstructed azimuth angle of the neutrino (e+ channel)
    TH1F *fHistNuReco_eta_e_plus;  // Reconstructed Eta of the neutrino (e+ channel)
    TH1F *fHistNuReco_E_e_plus;    // Reconstructed energy of the neutrino (e+ channel)
    TH1F *fHistNuReco_pt_e_plus;   // Reconstructed Pt of the neutrino (e+ channel)
    TH1F *fHistNuReco_phi_e_minus; // Reconstructed azimuth angle of the neutrino (e- channel)
    TH1F *fHistNuReco_eta_e_minus; // Reconstructed Eta of the neutrino (e- channel)
    TH1F *fHistNuReco_E_e_minus;   // Reconstructed energy of the neutrino (e- channel)
    TH1F *fHistNuReco_pt_e_minus;  // Reconstructed Pt of the neutrino (e- channel)

    TH1F *fHistNuReco_phi_mu;       // Reconstructed azimuth angle of the neutrino (mu channel)
    TH1F *fHistNuReco_eta_mu;       // Reconstructed Eta of the neutrino (mu channel)
    TH1F *fHistNuReco_E_mu;         // Reconstructed energy of the neutrino (mu channel)
    TH1F *fHistNuReco_pt_mu;        // Reconstructed Pt of the neutrino (mu channel)
    TH1F *fHistNuReco_phi_mu_plus;  // Reconstructed azimuth angle of the neutrino (mu+ channel)
    TH1F *fHistNuReco_eta_mu_plus;  // Reconstructed Eta of the neutrino (mu+ channel)
    TH1F *fHistNuReco_E_mu_plus;    // Reconstructed energy of the neutrino (mu+ channel)
    TH1F *fHistNuReco_pt_mu_plus;   // Reconstructed Pt of the neutrino (mu+ channel)
    TH1F *fHistNuReco_phi_mu_minus; // Reconstructed azimuth angle of the neutrino (mu- channel)
    TH1F *fHistNuReco_eta_mu_minus; // Reconstructed Eta of the neutrino (mu- channel)
    TH1F *fHistNuReco_E_mu_minus;   // Reconstructed energy of the neutrino (mu- channel)
    TH1F *fHistNuReco_pt_mu_minus;  // Reconstructed Pt of the neutrino (mu- channel)

    // charged lepton
    // electrons and muons
    TH1F *fHistChargedLeptonReco_pt;
    TH1F *fHistChargedLeptonReco_eta;

    // jets
    TH1F *fHistJetMultiplicityReco;             // number of jets
    TH1F *fHistBJetMultiplicityReco;            // number of b-tagged jets
    TH1F *fHistNonBJetMultiplicityEta2Reco;     // number of jets without b tag and |eta|>2
    TH1F *fHistNonBJetMultiplicityEta3Reco;     // number of jets without b tag and |eta|>3
    TH1F *fHistLeadingNonBJetReco_pt;           // pt of the leading jet without b tag
    TH1F *fHistLeadingNonBJetReco_eta;          // eta of the leading jet without b tag

    //Rest Frame Plots
    TH1F *fHist_ThetaStar1_LepW;		//Theta*, which is the angle between charged lepton momentum  in W rest frame
						//and W-Boson momentum in Top rest frame
    TH1F *fHist_Cos_ThetaStar1_LepW;		//cos(Theta*). Theta* is the angle between charged lepton momentum  in W rest frame
						//and W-Boson momentum in Top rest frame
    TH1F *fHist_ThetaStar2_LepBQuark;		//Theta*, which is the angle between charged lepton momentum  in W rest frame
						//b-Quark momentum also in W rest frame
    TH1F *fHist_Cos_ThetaStar2_LepBQuark;	//cos(Theta*). Theta* is the angle between charged lepton momentum  in W rest frame
						//b-Quark momentum also in W rest frame
    TH1F *fHist_SumThetaStar1ThetaStar2;	//Sum of Theta1*+Theta2*
    TH1F *fHist_Cos_SumThetaStar1ThetaStar2;	//cos(Theta1*+Theta2*)

    //Plots in Rest Frame of TopQuark
    TH1F *fHist_AngleTopFwdJet_TopRF;		//angle between TopQuark and Fwd jet
    TH1F *fHist_AmountSumPTopFwdJet_TopRF;	//amount of the P_Top+P_fwdjet in RestFrame of the Top (only FwdJet is boosted)

    // miscellaneous
    TH1F *fHistDeltaP2Mod_Top_2ndLeadJet; // Modulus of ( Pt-vector of top-quark ) +
                                         // ( Pt-vector of 2nd lead. jet
	                                 // not chosen as b-jet for top-reco )
    TH1F *fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_fine; // same as above but for different slices in eta(2ndleadjet) (fine binning)
    TH1F *fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet; // same as above but for different slices in eta(2ndleadjet) (normal binning)
    TH1F *fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_Less_Bins; //same as above but for different slices in eta(2ndleadjet) (coarse binning).
										    //The Bins fur high eta values are merged
    TH1F *fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_Less_Bins2; //same as above but for different slices in eta(2ndleadjet) (coarse binning).
										    //The Bins fur high eta values are merged
    TH1F *fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_Less_Bins3; //same as above but for different slices in eta(2ndleadjet) (coarse binning).
										    //The Bins fur high eta values are merged
    TH1F *fHistDeltaP2Mod_Top_2ndLeadJet_sliced_in_eta_2ndleadjet_coarse; // same as above but for different slices in eta(2ndleadjet) (coarse binning)

    TH2F *fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_fine; // Above modulus vs eta of 2ndLeadJet (fine binning)
    TH2F *fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet; // Above modulus vs eta of 2ndLeadJet (normal binning)
    TH2F *fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins; // Above modulus vs eta of 2ndLeadJet (coarse binning) with less Bins for high Eta
    TH2F *fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins2; // Above modulus vs eta of 2ndLeadJet (coarse binning) with less Bins for high Eta
    TH2F *fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_Less_Bins3; // Above modulus vs eta of 2ndLeadJet (coarse binning) with less Bins for high Eta
    TH2F *fHistDeltaP2Mod_Top_2ndLeadJet_vs_Eta2ndLeadJet_coarse; // Above modulus vs eta of 2ndLeadJet (coarse binning)
    TH1F *fHistEta2ndLeadJet; // eta of 2ndLeadJet (not b-jet from fit)

    TH1F *fHistAngleFwdJetTopQuark; // angle between 2ndLeadJet (not b-jet from fit) and top quark

    TH1F *fHistCosAngleFwdJetTopQuark;// Cosine of angle between 2ndLeadJet (not b-jet from fit) and top quark

    TH1F *fHistMEffective;        // M_eff = sum_{i}Pt_i + ETMiss
    TH1F *fHistSphericity;        // Sphericity, not yet given in Athena,
                                  // computed w.r.t. the reconstructed jets and particles
    TH1F *fHistXlightRel;         // x_Bjorken of light quark in initial state
                                  // as given in a simple parton model, taking max(x1, x2)
    TH1F *fHistXbottomRel;        // x_Bjorken of b quark in initial state
                                  // as given in a simple parton model, taking min(x1, x2)
    TH1F *fHistXlightForwardJet;  // x_Bjorken of light quark in initial state
                                  // as given in a simple parton model, computing with the help
                                  // of the forward jet
    TH1F *fHistXbottomForwardJet; // x_Bjorken of b quark in initial state
                                  // as given in a simple parton model, computing with the help
                                  // of the forward jet
    TH1F *fHistDeltaRChLepBJet;   // DeltaR between Charged Lepton and b jet from top decay

    // chi-square and chi-square probability distributions for e, mu channels and both
    // before P(Chi2) cut
    TH1F *fHistChi2_both;      // chi-square of both W decays
    TH1F *fHistChi2Prob_both;  // chi-square probability of both W decays
    TH1F *fHistChi2_both_Log;      // chi-square of both W decays (log scale)
    TH1F *fHistChi2Prob_both_Log;  // chi-square probability of both W decays (log scale)
    TH1F *fHistChi2_e;      // chi-square of e channel
    TH1F *fHistChi2Prob_e;  // chi-square probability of e channel
    TH1F *fHistChi2_mu;     // chi-square of mu channel
    TH1F *fHistChi2Prob_mu; // chi-square probability of mu channel
    // after P(Chi2) cut
    TH1F *fHistChi2_both_cut;      // chi-square of both W decays
    TH1F *fHistChi2Prob_both_cut;  // chi-square probability of both W decays
    TH1F *fHistChi2_both_cut_Log;      // chi-square of both W decays (log scale)
    TH1F *fHistChi2Prob_both_cut_Log;  // chi-square probability of both W decays (log scale)
    TH1F *fHistChi2_e_cut;      // chi-square of e channel
    TH1F *fHistChi2Prob_e_cut;  // chi-square probability of e channel
    TH1F *fHistChi2_mu_cut;     // chi-square of mu channel
    TH1F *fHistChi2Prob_mu_cut; // chi-square probability of mu channel

    TH1F *fHistEfficiencyVsChi2ProbCut; // efficiency of kinematic fit dependent on value of Chi2Prob cut w.r.t. event yield after Whad veto
    TH2F *fHist_Chi2Prob_vs_Chi2ProbWhad; // 2D: chi-square probability of top-quark fit vs chi-square probability of hadron. W-boson fit
    TH2F *fHist_Chi2_vs_Chi2Whad; // 2D: chi-square of top-quark fit vs chi-square of hadron. W-boson fit
    TH2F *fHist_Chi2Prob_vs_M_invar_W; // 2D: chi-square probability vs invariant mass of W-boson
    TH2F *fHist_Chi2Prob_vs_M_invar_top; // 2D: chi-square probability vs invariant mass of top-quark
    TH2F *fHist_M_invar_W_vs_M_invar_top;// 2D: invar. mass of W vs invar. mass of top-quark
    TH1F *fHist_M_invar_W_over_M_invar_top;// 1D: ratio M_W over M_top
    TH1F *fHist_Chi2ProbSgTop_over_Chi2ProbTTbarWhad; // 1D: P(Chi2)_SgTop / ( P(Chi2)_TTbar * P(Chi2)_Whad )
    TH2F *fHist_M_invar_W_lep_vs_M_invar_W_had; // 2D: invariant mass of leptonic W vs invariant mass of hadronic W
    TH2F *fHist_Chi2ProbSgTop_vs_Chi2ProbTTbar; // 2D: P(Chi2) of sgtop fit vs P(Chi2) of ttbar veto fit

    // KinFitter performance histograms
    TH1I *fHistNbWorseTopCandidates; // No. of worse top-quark candidates per evt (w.r.t. Chi2)
    TH1F *fHistChi2WorseTopCandidates;// Chi2 of worse top-quark candidates per evt (w.r.t. Chi2)
    TH1F *fHistChi2ProbWorseTopCandidates;// Chi2Prob of worse top-quark candidates per evt (w.r.t. Chi2)

    // Event variables histograms
    TH1F *fHistEtMissMag_pretag; // EtMiss pretag
    TH1F *fHistEtMissPhi_pretag; // EtMiss Phi pretag
    TH1F *fHistEtMissMag_tag; // EtMiss tag
    TH1F *fHistEtMissPhi_tag; // EtMiss Phi tag
    TH1F *fHistSumEt_pretag; // SumEt pretag
    TH1F *fHistSumEt_tag; // SumEt tag
    TH1F *fHistTotalYield_allCuts; // Total weighted yield after all cuts
    TH1F *fHistCountLepPlus_pretag; // Pretag count positive leptons
    TH1F *fHistCountLepMinus_pretag; // Pretag count negative leptons
    TH1F *fHistCountLepAll_tag; // Tag count (all leptons)
    TH1F *fHistCountLepPlus_tag; // Tag count (positive leptons)
    TH1F *fHistCountLepMinus_tag; // Tag count (negative leptons)
    TH1F *fHistCountLepAll_tag_refQCD; // Tag count (all leptons), reference for QCD norm.
    TH1F *fHistCountLepPlus_tag_refQCD; // Tag count (positive leptons), reference for QCD norm.
    TH1F *fHistCountLepMinus_tag_refQCD; // Tag count (negative leptons), reference for QCD norm.
    TH1F *fHistCountLepAll_WCtrl_pretag; // Pretag count, W ctrl region (all leptons)
    TH1F *fHistCountLepPlus_WCtrl_pretag; // Pretag count, W ctrl region (positive leptons)
    TH1F *fHistCountLepMinus_WCtrl_pretag; // Pretag count, W ctrl region (negative leptons)
    TH1F *fHistCountLepAll_WCtrl_tag; // Tag count, W ctrl region (all leptons)
    TH1F *fHistCountLepPlus_WCtrl_tag; // Tag count, W ctrl region (positive leptons)
    TH1F *fHistCountLepMinus_WCtrl_tag; // Tag count, W ctrl region (negative leptons)

    TH1F *fHist_HFOR_flag_pretag; // top_hfor_flag for use by wjets flavour reweighting tool

    TH1F *fHistCountFwdElectrons_pretag;// no. of events (pretag) for fwd electrons
    TH1F *fHistCountCentralElectrons_pretag;// no. of events (pretag) for central electrons
    TH1F *fHistCountFwdElectrons_tag;// no. of events (tag) for fwd electrons
    TH1F *fHistCountCentralElectrons_tag;// no. of events (tag) for central electrons
    TH1F *fHistCountFwdElectrons_WCtrl;// no. of events (W control region) for fwd electrons
    TH1F *fHistCountCentralElectrons_WCtrl;// no. of events (W control region) for central electrons

    // Signal lepton histograms
    TH1F *fHistSignalLepton_pt;    // Pt of leading signal lepton
    TH1F *fHistSignalLepton_eta;   // Eta of leading signal lepton
    TH1F *fHistSignalLepton_eta_cells;// Eta of leading signal lepton (cell granularity)
    TH1F *fHistSignalLepton_phi;   // Phi of leading signal lepton
    TH1F *fHistSignalLepton_EtCone20; // EtCone20 of leading signal lepton
    TH1F *fHistSignalLepton_PtCone20; // PtCone20 of leading signal lepton
    TH1F *fHistSignalLepton_EtCone30; // EtCone30 of leading signal lepton
    TH1F *fHistSignalLepton_PtCone30; // PtCone30 of leading signal lepton
    TH1F *fHistSignalLepton_EtRatio;  // EtCone30/LeptonPt of leading signal lepton
    TH1F *fHistSignalLepton_PtRatio;  // PtCone30/LeptonPt of leading signal lepton

    TH1F *fHistSignalElectron_author; // Author of signal electron
    TH1F *fHistSignalElectron_pt;  // Pt of leading signal electron
    TH1F *fHistSignalElectron_eta; // Eta of leading signal electron
    TH1F *fHistSignalElectron_phi; // Phi of leading signal electron
    TH1F *fHistSignalElectron_EtCone20; // EtCone20 of leading signal electron
    TH1F *fHistSignalElectron_PtCone20; // PtCone20 of leading signal electron
    TH1F *fHistSignalElectron_EtCone30; // EtCone30 of leading signal electron
    TH1F *fHistSignalElectron_PtCone30; // PtCone30 of leading signal electron
    TH1F *fHistSignalElectron_EtRatio;  // EtCone30/ElectronPt of leading signal electron
    TH1F *fHistSignalElectron_PtRatio;  // PtCone30/ElectronPt of leading signal electron

    TH1F *fHistSignalEplus_pt;     // Pt of leading signal e+
    TH1F *fHistSignalEplus_eta;    // Eta of leading signal e+
    TH1F *fHistSignalEplus_phi;    // Phi of leading signal e+
    TH1F *fHistSignalEminus_pt;    // Pt of leading signal e-
    TH1F *fHistSignalEminus_eta;   // Eta of leading signal e-
    TH1F *fHistSignalEminus_phi;   // Phi of leading signal e-

    TH1F *fHistSignalMuon_pt;      // Pt of leading signal muon
    TH1F *fHistSignalMuon_eta;     // Eta of leading signal muon
    TH1F *fHistSignalMuon_phi;     // Phi of leading signal muon
    TH1F *fHistSignalMuon_EtCone20; // EtCone20 of leading signal muon
    TH1F *fHistSignalMuon_PtCone20; // PtCone20 of leading signal muon
    TH1F *fHistSignalMuon_EtCone30; // EtCone30 of leading signal muon
    TH1F *fHistSignalMuon_PtCone30; // PtCone30 of leading signal muon
    TH1F *fHistSignalMuon_EtRatio;  // EtCone30/MuonPt of leading signal muon
    TH1F *fHistSignalMuon_PtRatio;  // PtCone30/MuonPt of leading signal muon
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

    // Signal lepton histograms after b-tagging
    TH1F *fHistSignalLepton_pt_btag;
    TH1F *fHistSignalLepton_eta_btag;
    TH1F *fHistSignalLepton_eta_btag_cells;// Eta of leading signal lepton (cell granularity)
    TH1F *fHistSignalLepton_phi_btag;
    TH1F *fHistSignalLepton_EtCone20_btag;
    TH1F *fHistSignalLepton_PtCone20_btag;
    TH1F *fHistSignalLepton_EtCone30_btag;
    TH1F *fHistSignalLepton_PtCone30_btag;
    TH1F *fHistSignalLepton_EtRatio_btag;
    TH1F *fHistSignalLepton_PtRatio_btag;
    TH1F *fHistSignalElectron_author_btag;
    TH1F *fHistSignalElectron_pt_btag;
    TH1F *fHistSignalElectron_eta_btag;
    TH1F *fHistSignalElectron_phi_btag;
    TH1F *fHistSignalElectron_EtCone20_btag;
    TH1F *fHistSignalElectron_PtCone20_btag;
    TH1F *fHistSignalElectron_EtCone30_btag;
    TH1F *fHistSignalElectron_PtCone30_btag;
    TH1F *fHistSignalElectron_EtRatio_btag;
    TH1F *fHistSignalElectron_PtRatio_btag;
    TH1F *fHistSignalEplus_pt_btag;
    TH1F *fHistSignalEplus_eta_btag;
    TH1F *fHistSignalEplus_phi_btag;
    TH1F *fHistSignalEminus_pt_btag;
    TH1F *fHistSignalEminus_eta_btag;
    TH1F *fHistSignalEminus_phi_btag;
    TH1F *fHistSignalMuon_pt_btag;
    TH1F *fHistSignalMuon_eta_btag;
    TH1F *fHistSignalMuon_phi_btag;
    TH1F *fHistSignalMuon_EtCone20_btag;
    TH1F *fHistSignalMuon_PtCone20_btag;
    TH1F *fHistSignalMuon_EtCone30_btag;
    TH1F *fHistSignalMuon_PtCone30_btag;
    TH1F *fHistSignalMuon_EtRatio_btag;
    TH1F *fHistSignalMuon_PtRatio_btag;
    TH1F *fHistSignalMuplus_pt_btag;
    TH1F *fHistSignalMuplus_eta_btag;
    TH1F *fHistSignalMuplus_phi_btag;
    TH1F *fHistSignalMuminus_pt_btag;
    TH1F *fHistSignalMuminus_eta_btag;
    TH1F *fHistSignalMuminus_phi_btag;

    // cut-based cuts histograms after b-tagging
    TH1F *fHistJet_H_T;              // H_T: sum of jets pt and lepton pt and etmiss
    TH1D *fHistJet_MassAllJets;      // Mass of all jets
    TH1F *fHistDeltaEtaBJetLightJet; // DeltaEta(Hardest B-Jet, Hardest Non-B-Jet)
    TH1F *fHistHardestLightJetEta;      // Eta(Hardest Non-B-Jet)

    // Jets
    TH1F *fHistJet_n;      // No. of jets
    TH1F *fHistJet_Et;     // Et of all jets
    TH1F *fHistJet_eta;    // Eta of all jets
    TH1F *fHistJet_phi;    // Phi of all jets
    TH1F *fHistJet_pt;    // Pt of all jets
    TH1F *fHistJet_weight; // B-tag weight of all jets
    TH1F *fHistJet1_Et;    // Pt of leading jet
    TH1F *fHistJet1_eta;   // Eta of leading jet
    TH1F *fHistJet1_phi;   // Phi of leading jet
    TH1F *fHistDeltaJet1PhiEtMissPhi;   // DeltaR(leading jet, MET)
    TH1F *fHistJet1_pt;    // Pt of leading jet
    TH1F *fHistJet2_Et;    // Pt of 2nd leading jet
    TH1F *fHistJet2_eta;   // Eta of 2nd leading jet
    TH1F *fHistJet2_phi;   // Phi of 2nd leading jet
    TH1F *fHistJet2_pt;    // Pt of 2nd leading jet

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
    TH1F *fHistBJet2_weight;// Weight of 2nd leading b-jet

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

    TH1F *fHistWhadJet_n;    // No. of whad jets
    TH1F *fHistWhadJet_Et;   // Et of all whad jets
    TH1F *fHistWhadJet_eta;  // Eta of all whad jets
    TH1F *fHistWhadJet_phi;  // Phi of all whad jets
    TH1F *fHistWhadJet1_Et;  // Pt of leading whad jet
    TH1F *fHistWhadJet1_eta; // Eta of leading whad jet
    TH1F *fHistWhadJet1_phi; // Phi of leading whad jet
    TH1F *fHistWhadJet2_Et;  // Pt of 2nd leading whad jet
    TH1F *fHistWhadJet2_eta; // Eta of 2nd leading whad jet
    TH1F *fHistWhadJet2_phi; // Phi of 2nd leading whad jet

    // Jets after BTag
    TH1F *fHistJet_n_btag;      // No. of jets
    TH1F *fHistJet_Et_btag;     // Et of all jets
    TH1F *fHistJet_eta_btag;    // Eta of all jets
    TH1F *fHistJet_phi_btag;    // Phi of all jets
    TH1F *fHistJet_pt_btag;    // Pt of all jets
    TH1F *fHistJet_weight_btag; // B-tag weight of all jets
    TH1F *fHistJet1_Et_btag;    // Pt of leading jet
    TH1F *fHistJet1_eta_btag;   // Eta of leading jet
    TH1F *fHistJet1_phi_btag;   // Phi of leading jet
    TH1F *fHistDeltaJet1PhiEtMissPhi_btag;   // DeltaR(leading jet, MET)
    TH1F *fHistJet1_pt_btag;    // Pt of leading jet
    TH1F *fHistJet2_Et_btag;    // Pt of 2nd leading jet
    TH1F *fHistJet2_eta_btag;   // Eta of 2nd leading jet
    TH1F *fHistJet2_phi_btag;   // Phi of 2nd leading jet
    TH1F *fHistJet2_pt_btag;    // Pt of 2nd leading jet

    TH1F *fHistBJet_n_btag;      // No. of b-jets
    TH1F *fHistBJet_Et_btag;     // Et of all b-jets
    TH1F *fHistBJet_eta_btag;    // Eta of all b-jets
    TH1F *fHistBJet_phi_btag;    // Phi of all b-jets
    TH1F *fHistBJet_weight_btag;// Weight of all b-jets
    TH1F *fHistBJet1_Et_btag;    // Pt of leading b-jet
    TH1F *fHistBJet1_eta_btag;   // Eta of leading b-jet
    TH1F *fHistBJet1_phi_btag;   // Phi of leading b-jet
    TH1F *fHistBJet1_weight_btag;// Weight of leading b-jet
    TH1F *fHistBJet2_Et_btag;    // Pt of 2nd leading b-jet
    TH1F *fHistBJet2_eta_btag;   // Eta of 2nd leading b-jet
    TH1F *fHistBJet2_phi_btag;   // Phi of 2nd leading b-jet
    TH1F *fHistBJet2_weight_btag;// Weight of 2nd leading b-jet

    TH1F *fHistNonBJet_n_btag;      // No. of non-b-jets
    TH1F *fHistNonBJet_Et_btag;     // Et of all non-b-jets
    TH1F *fHistNonBJet_eta_btag;    // Eta of all non-b-jets
    TH1F *fHistNonBJet_phi_btag;    // Phi of all non-b-jets
    TH1F *fHistNonBJet1_Et_btag;    // Pt of leading non-b-jet
    TH1F *fHistNonBJet1_eta_btag;   // Eta of leading non-b-jet
    TH1F *fHistNonBJet1_phi_btag;   // Phi of leading non-b-jet
    TH1F *fHistNonBJet2_Et_btag;    // Pt of 2nd leading non-b-jet
    TH1F *fHistNonBJet2_eta_btag;   // Eta of 2nd leading non-b-jet
    TH1F *fHistNonBJet2_phi_btag;   // Phi of 2nd leading non-b-jet

    TH1F *fHistWhadJet_n_btag;    // No. of whad jets
    TH1F *fHistWhadJet_Et_btag;   // Et of all whad jets
    TH1F *fHistWhadJet_eta_btag;  // Eta of all whad jets
    TH1F *fHistWhadJet_phi_btag;  // Phi of all whad jets
    TH1F *fHistWhadJet1_Et_btag;  // Pt of leading whad jet
    TH1F *fHistWhadJet1_eta_btag; // Eta of leading whad jet
    TH1F *fHistWhadJet1_phi_btag; // Phi of leading whad jet
    TH1F *fHistWhadJet2_Et_btag;  // Pt of 2nd leading whad jet
    TH1F *fHistWhadJet2_eta_btag; // Eta of 2nd leading whad jet
    TH1F *fHistWhadJet2_phi_btag; // Phi of 2nd leading whad jet

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

    TH1F *fHistKinFitCandidateFwdJet_n;    // No. of forward jets
    TH1F *fHistKinFitCandidateFwdJet_Et;   // Et of all forward jets
    TH1F *fHistKinFitCandidateFwdJet_eta;  // Eta of all forward jets
    TH1F *fHistKinFitCandidateFwdJet_phi;  // Phi of all forward jets
    TH1F *fHistKinFitCandidateFwdJet1_Et;  // Pt of leading forward jet
    TH1F *fHistKinFitCandidateFwdJet1_eta; // Eta of leading forward jet
    TH1F *fHistKinFitCandidateFwdJet1_phi; // Phi of leading forward jet
    TH1F *fHistKinFitCandidateFwdJet2_Et;  // Pt of 2nd leading forward jet
    TH1F *fHistKinFitCandidateFwdJet2_eta; // Eta of 2nd leading forward jet
    TH1F *fHistKinFitCandidateFwdJet2_phi; // Phi of 2nd leading forward jet

    // ====================================
    // histograms for CutBased top candidates
    // ====================================

    // Signal lepton histogrmas
    TH1F *fHistCutBasedCandidateSignalLepton_pt;    // Pt of leading signal lepton
    TH1F *fHistCutBasedCandidateSignalLepton_eta;   // Eta of leading signal lepton
    TH1F *fHistCutBasedCandidateSignalLepton_phi;   // Phi of leading signal lepton

    TH1F *fHistCutBasedCandidateSignalElectron_author; // Author of signal electron
    TH1F *fHistCutBasedCandidateSignalElectron_pt;  // Pt of leading signal electron
    TH1F *fHistCutBasedCandidateSignalElectron_eta; // Eta of leading signal electron
    TH1F *fHistCutBasedCandidateSignalElectron_phi; // Phi of leading signal electron
    TH1F *fHistCutBasedCandidateSignalEplus_pt;     // Pt of leading signal e+
    TH1F *fHistCutBasedCandidateSignalEplus_eta;    // Eta of leading signal e+
    TH1F *fHistCutBasedCandidateSignalEplus_phi;    // Phi of leading signal e+
    TH1F *fHistCutBasedCandidateSignalEminus_pt;    // Pt of leading signal e-
    TH1F *fHistCutBasedCandidateSignalEminus_eta;   // Eta of leading signal e-
    TH1F *fHistCutBasedCandidateSignalEminus_phi;   // Phi of leading signal e-

    TH1F *fHistCutBasedCandidateSignalMuon_pt;      // Pt of leading signal muon
    TH1F *fHistCutBasedCandidateSignalMuon_eta;     // Eta of leading signal muon
    TH1F *fHistCutBasedCandidateSignalMuon_phi;     // Phi of leading signal muon
    TH1F *fHistCutBasedCandidateSignalMuplus_pt;    // Pt of leading signal mu+
    TH1F *fHistCutBasedCandidateSignalMuplus_eta;   // Eta of leading signal mu+
    TH1F *fHistCutBasedCandidateSignalMuplus_phi;   // Phi of leading signal mu+
    TH1F *fHistCutBasedCandidateSignalMuminus_pt;   // Pt of leading signal mu-
    TH1F *fHistCutBasedCandidateSignalMuminus_eta;  // Eta of leading signal mu-
    TH1F *fHistCutBasedCandidateSignalMuminus_phi;  // Phi of leading signal mu-

    TH1F *fHistCutBasedCandidateSignalTau_pt;       // Pt of leading signal tau
    TH1F *fHistCutBasedCandidateSignalTau_eta;      // Eta of leading signal tau
    TH1F *fHistCutBasedCandidateSignalTau_phi;      // Phi of leading signal tau
    TH1F *fHistCutBasedCandidateSignalTauplus_pt;   // Pt of leading signal tau+
    TH1F *fHistCutBasedCandidateSignalTauplus_eta;  // Eta of leading signal tau+
    TH1F *fHistCutBasedCandidateSignalTauplus_phi;  // Phi of leading signal tau+
    TH1F *fHistCutBasedCandidateSignalTauminus_pt;  // Pt of leading signal tau-
    TH1F *fHistCutBasedCandidateSignalTauminus_eta; // Eta of leading signal tau-
    TH1F *fHistCutBasedCandidateSignalTauminus_phi; // Phi of leading signal tau-

    // Jets
    TH1F *fHistCutBasedCandidateJet_n;      // No. of jets
    TH1F *fHistCutBasedCandidateJet_Et;     // Et of all jets
    TH1F *fHistCutBasedCandidateJet_eta;    // Eta of all jets
    TH1F *fHistCutBasedCandidateJet_phi;    // Phi of all jets
    TH1F *fHistCutBasedCandidateJet1_Et;    // Pt of leading jet
    TH1F *fHistCutBasedCandidateJet1_eta;   // Eta of leading jet
    TH1F *fHistCutBasedCandidateJet1_phi;   // Phi of leading jet
    TH1F *fHistCutBasedCandidateJet2_Et;    // Pt of 2nd leading jet
    TH1F *fHistCutBasedCandidateJet2_eta;   // Eta of 2nd leading jet
    TH1F *fHistCutBasedCandidateJet2_phi;   // Phi of 2nd leading jet

    TH1F *fHistCutBasedCandidateBJet_n;      // No. of b-jets
    TH1F *fHistCutBasedCandidateBJet_Et;     // Et of all b-jets
    TH1F *fHistCutBasedCandidateBJet_eta;    // Eta of all b-jets
    TH1F *fHistCutBasedCandidateBJet_phi;    // Phi of all b-jets
    TH1F *fHistCutBasedCandidateBJet1_Et;    // Pt of leading b-jet
    TH1F *fHistCutBasedCandidateBJet1_eta;   // Eta of leading b-jet
    TH1F *fHistCutBasedCandidateBJet1_phi;   // Phi of leading b-jet
    TH1F *fHistCutBasedCandidateBJet2_Et;    // Pt of 2nd leading b-jet
    TH1F *fHistCutBasedCandidateBJet2_eta;   // Eta of 2nd leading b-jet
    TH1F *fHistCutBasedCandidateBJet2_phi;   // Phi of 2nd leading b-jet

    TH1F *fHistCutBasedCandidateNonBJet_n;      // No. of non-b-jets
    TH1F *fHistCutBasedCandidateNonBJet_Et;     // Et of all non-b-jets
    TH1F *fHistCutBasedCandidateNonBJet_eta;    // Eta of all non-b-jets
    TH1F *fHistCutBasedCandidateNonBJet_phi;    // Phi of all non-b-jets
    TH1F *fHistCutBasedCandidateNonBJet1_Et;    // Pt of leading non-b-jet
    TH1F *fHistCutBasedCandidateNonBJet1_eta;   // Eta of leading non-b-jet
    TH1F *fHistCutBasedCandidateNonBJet1_phi;   // Phi of leading non-b-jet
    TH1F *fHistCutBasedCandidateNonBJet2_Et;    // Pt of 2nd leading non-b-jet
    TH1F *fHistCutBasedCandidateNonBJet2_eta;   // Eta of 2nd leading non-b-jet
    TH1F *fHistCutBasedCandidateNonBJet2_phi;   // Phi of 2nd leading non-b-jet

    TH1F *fHistCutBasedCandidateFwdJet_n;    // No. of forward jets
    TH1F *fHistCutBasedCandidateFwdJet_Et;   // Et of all forward jets
    TH1F *fHistCutBasedCandidateFwdJet_eta;  // Eta of all forward jets
    TH1F *fHistCutBasedCandidateFwdJet_phi;  // Phi of all forward jets
    TH1F *fHistCutBasedCandidateFwdJet1_Et;  // Pt of leading forward jet
    TH1F *fHistCutBasedCandidateFwdJet1_eta; // Eta of leading forward jet
    TH1F *fHistCutBasedCandidateFwdJet1_phi; // Phi of leading forward jet
    TH1F *fHistCutBasedCandidateFwdJet2_Et;  // Pt of 2nd leading forward jet
    TH1F *fHistCutBasedCandidateFwdJet2_eta; // Eta of 2nd leading forward jet
    TH1F *fHistCutBasedCandidateFwdJet2_phi; // Phi of 2nd leading forward jet

    // Before/after fit comparison histograms
    TH1F *fHistSignalLeptonPt_before;
    TH1F *fHistSignalLeptonEta_before;
    TH1F *fHistSignalLeptonEta_before_cells;
    TH1F *fHistSignalLeptonPhi_before;
    TH1F *fHistBJetEt_before;
    TH1F *fHistBJetEta_before;
    TH1F *fHistBJetPhi_before;
    TH1F *fHistBJetWeight_before;
    TH1F *fHistEtMissMag_before;
    TH1F *fHistEtMissPhi_before;
    TH1F *fHistSumEt_before;

    TH1F *fHistSignalLeptonPt_after;
    TH1F *fHistSignalLeptonEta_after;
    TH1F *fHistSignalLeptonEta_after_cells;
    TH1F *fHistSignalLeptonPhi_after;
    TH1F *fHistBJetEt_after;
    TH1F *fHistBJetEta_after;
    TH1F *fHistBJetPhi_after;
    TH1F *fHistEtMissMag_after;
    TH1F *fHistEtMissPhi_after;

    // Normalized residual distributions
    TH1F *fHistNormResidLepton_pt;
    TH1F *fHistNormResidLepton_eta;
    TH1F *fHistNormResidLepton_phi;
    TH1F *fHistNormResidBJet_Et;
    TH1F *fHistNormResidBJet_pt;
    TH1F *fHistNormResidBJet_eta;
    TH1F *fHistNormResidBJet_phi;
    TH1F *fHistNormResidEtmiss_Mag;
    TH1F *fHistNormResidEtmiss_phi;

    // Pull distributions p=DeltaY/sigma
    // DeltaY: correction to original input
    // sigma: corresponding standard deviation as given by the fit
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

    // Jet bin counts
    TH1F *fHistJetBins_pretag;
    TH1F *fHistJetBins_tag;
    TH1F *fHistJetBins_fit;

    //
    // cut based analysis
    //

    // W -> enu
    TH1F *fHistWreco_cutbased_Mt_e;     // cutbased Reconstructed invariant transverse mass of the W decaying into electrons
    TH1F *fHistWreco_cutbased_phi_e;    // cutbased Reconstructed azimuth angle of the (real) W decaying into electrons
    TH1F *fHistWreco_cutbased_eta_e;    // cutbased Reconstructed Eta of the (real) W decaying into electrons
    TH1F *fHistWreco_cutbased_pt_e;     // cutbased Reconstructed Pt of the W decaying into electrons
    TH1F *fHistWreco_cutbased_Mt_e_minus;     // cutbased Reconstructed invariant transverse mass of the W decaying into e-
    TH1F *fHistWreco_cutbased_phi_e_minus;    // cutbased Reconstructed azimuth angle of the (real) W decaying into e-
    TH1F *fHistWreco_cutbased_eta_e_minus;    // cutbased Reconstructed Eta of the (real) W decaying into e-
    TH1F *fHistWreco_cutbased_pt_e_minus;     // cutbased Reconstructed Pt of the W decaying into e-
    TH1F *fHistWreco_cutbased_Mt_e_plus;     // cutbased Reconstructed invariant transverse mass of the W decaying into e+
    TH1F *fHistWreco_cutbased_phi_e_plus;    // cutbased Reconstructed azimuth angle of the (real) W decaying into e+
    TH1F *fHistWreco_cutbased_eta_e_plus;    // cutbased Reconstructed Eta of the (real) W decaying into e+
    TH1F *fHistWreco_cutbased_pt_e_plus;     // cutbased Reconstructed Pt of the W decaying into e+

    // W -> munu
    TH1F *fHistWreco_cutbased_Mt_mu;    // cutbased Reconstructed invariant transverse mass of the W decaying into muons
    TH1F *fHistWreco_cutbased_phi_mu;   // cutbased Reconstructed azimuth angle of the W decaying into muons
    TH1F *fHistWreco_cutbased_eta_mu;   // cutbased Reconstructed Eta of the W decaying into muons
    TH1F *fHistWreco_cutbased_pt_mu;    // cutbased Reconstructed Pt of the W decaying into muons
    TH1F *fHistWreco_cutbased_Mt_mu_minus;    // cutbased Reconstructed invariant transverse mass of the W decaying into mu-
    TH1F *fHistWreco_cutbased_phi_mu_minus;   // cutbased Reconstructed azimuth angle of the W decaying into mu-
    TH1F *fHistWreco_cutbased_eta_mu_minus;   // cutbased Reconstructed Eta of the W decaying into mu-
    TH1F *fHistWreco_cutbased_pt_mu_minus;    // cutbased Reconstructed Pt of the W decaying into mu-
    TH1F *fHistWreco_cutbased_Mt_mu_plus;    // cutbased Reconstructed invariant transverse mass of the W decaying into mu+
    TH1F *fHistWreco_cutbased_phi_mu_plus;   // cutbased Reconstructed azimuth angle of the W decaying into mu+
    TH1F *fHistWreco_cutbased_eta_mu_plus;   // cutbased Reconstructed Eta of the W decaying into mu+
    TH1F *fHistWreco_cutbased_pt_mu_plus;    // cutbased Reconstructed Pt of the W decaying into mu+

    // W -> taunu
    TH1F *fHistWreco_cutbased_Mt_tau;   // cutbased Reconstructed invariant transverse mass of the W decaying into tauons
    TH1F *fHistWreco_cutbased_phi_tau;  // cutbased Reconstructed azimuth angle of the W decaying into tauons
    TH1F *fHistWreco_cutbased_eta_tau;  // cutbased Reconstructed Eta of the W decaying into tauons
    TH1F *fHistWreco_cutbased_pt_tau;   // cutbased Reconstructed Pt of the W decaying into tauons
    TH1F *fHistWreco_cutbased_Mt_tau_minus;   // cutbased Reconstructed invariant transverse mass of the W decaying into tau-
    TH1F *fHistWreco_cutbased_phi_tau_minus;  // cutbased Reconstructed azimuth angle of the W decaying into tau-
    TH1F *fHistWreco_cutbased_eta_tau_minus;  // cutbased Reconstructed Eta of the W decaying into tau-
    TH1F *fHistWreco_cutbased_pt_tau_minus;   // cutbased Reconstructed Pt of the W decaying into tau-
    TH1F *fHistWreco_cutbased_Mt_tau_plus;   // cutbased Reconstructed invariant transverse mass of the W decaying into tau+
    TH1F *fHistWreco_cutbased_phi_tau_plus;  // cutbased Reconstructed azimuth angle of the W decaying into tau+
    TH1F *fHistWreco_cutbased_eta_tau_plus;  // cutbased Reconstructed Eta of the W decaying into tau+
    TH1F *fHistWreco_cutbased_pt_tau_plus;   // cutbased Reconstructed Pt of the W decaying into tau+

    // W -> lnu
    TH1F *fHistWreco_cutbased_Mt;       // cutbased Reconstructed invariant transverse mass of the W (all leptons)
    TH1F *fHistWreco_cutbased_phi;      // cutbased Reconstructed azimuth angle of the W (all leptons)
    TH1F *fHistWreco_cutbased_eta;      // cutbased Reconstructed Eta of the W (all leptons)
    TH1F *fHistWreco_cutbased_pt;       // cutbased Reconstructed Pt of the W (all leptons)
    TH1F *fHistWreco_cutbased_Mt_minus;       // cutbased Reconstructed invariant transverse mass of the W (all negative sign leptons)
    TH1F *fHistWreco_cutbased_phi_minus;      // cutbased Reconstructed azimuth angle of the W (all negative sign leptons)
    TH1F *fHistWreco_cutbased_eta_minus;      // cutbased Reconstructed Eta of the W (all negative sign leptons)
    TH1F *fHistWreco_cutbased_pt_minus;       // cutbased Reconstructed Pt of the W (all negative sign leptons)
    TH1F *fHistWreco_cutbased_Mt_plus;       // cutbased Reconstructed invariant transverse mass of the W (all positive sign leptons)
    TH1F *fHistWreco_cutbased_phi_plus;      // cutbased Reconstructed azimuth angle of the W (all positive sign leptons)
    TH1F *fHistWreco_cutbased_eta_plus;      // cutbased Reconstructed Eta of the W (all positive sign leptons)
    TH1F *fHistWreco_cutbased_pt_plus;       // cutbased Reconstructed Pt of the W (all positive sign leptons)

    // top -> e
    TH1F *fHistTopReco_cutbased_Mt_e;   // cutbased Reconstructed invariant transverse mass of the single-top (e channel)
    TH1F *fHistTopReco_cutbased_phi_e;  // cutbased Reconstructed azimuth angle of the single-top (e channel)
    TH1F *fHistTopReco_cutbased_eta_e;  // cutbased Reconstructed Eta of the top (e channel)
    TH1F *fHistTopReco_cutbased_pt_e;   // cutbased Reconstructed Pt of the single-top (e channel)
    TH1F *fHistTopReco_cutbased_Mt_e_plus;   // cutbased Reconstructed invariant transverse mass of the single-top (e+ channel)
    TH1F *fHistTopReco_cutbased_phi_e_plus;  // cutbased Reconstructed azimuth angle of the single-top (e+ channel)
    TH1F *fHistTopReco_cutbased_eta_e_plus;  // cutbased Reconstructed Eta of the single-top (e+ channel)
    TH1F *fHistTopReco_cutbased_pt_e_plus;   // cutbased Reconstructed Pt of the single-top (e+ channel)
    TH1F *fHistTopReco_cutbased_Mt_e_minus;   // cutbased Reconstructed invariant transverse mass of the single-top (e- channel)
    TH1F *fHistTopReco_cutbased_phi_e_minus;  // cutbased Reconstructed azimuth angle of the single-top (e- channel)
    TH1F *fHistTopReco_cutbased_eta_e_minus;  // cutbased Reconstructed Eta of the single-top (e- channel)
    TH1F *fHistTopReco_cutbased_pt_e_minus;   // cutbased Reconstructed Pt of the single-top (e- channel)

    // top -> mu
    TH1F *fHistTopReco_cutbased_Mt_mu;  // cutbased Reconstructed invariant transverse mass of the single-top (mu channel)
    TH1F *fHistTopReco_cutbased_phi_mu; // cutbased Reconstructed azimuth angle of the single-top (mu channel)
    TH1F *fHistTopReco_cutbased_eta_mu; // cutbased Reconstructed Eta of the single-top (mu channel)
    TH1F *fHistTopReco_cutbased_pt_mu;  // cutbased Reconstructed Pt of the single-top (mu channel)
    TH1F *fHistTopReco_cutbased_Mt_mu_plus;  // cutbased Reconstructed invariant transverse mass of the single-top (mu+ channel)
    TH1F *fHistTopReco_cutbased_phi_mu_plus; // cutbased Reconstructed azimuth angle of the single-top (mu+ channel)
    TH1F *fHistTopReco_cutbased_eta_mu_plus; // cutbased Reconstructed Eta of the single-top (mu+ channel)
    TH1F *fHistTopReco_cutbased_pt_mu_plus;  // cutbased Reconstructed Pt of the single-top (mu+ channel)
    TH1F *fHistTopReco_cutbased_Mt_mu_minus;  // cutbased Reconstructed invariant transverse mass of the single-top (mu- channel)
    TH1F *fHistTopReco_cutbased_phi_mu_minus; // cutbased Reconstructed azimuth angle of the single-top (mu- channel)
    TH1F *fHistTopReco_cutbased_eta_mu_minus; // cutbased Reconstructed Eta of the single-top (mu- channel)
    TH1F *fHistTopReco_cutbased_pt_mu_minus;  // cutbased Reconstructed Pt of the single-top (mu- channel)

    // top -> tau
    TH1F *fHistTopReco_cutbased_Mt_tau; // cutbased Reconstructed invariant transverse mass of the single-top (tau channel)
    TH1F *fHistTopReco_cutbased_phi_tau;// cutbased Reconstructed azimuth angle of the single-top (tau channel)
    TH1F *fHistTopReco_cutbased_eta_tau;// cutbased Reconstructed Eta of the single-top (tau channel)
    TH1F *fHistTopReco_cutbased_pt_tau; // cutbased Reconstructed Pt of the single-top (tau channel)
    TH1F *fHistTopReco_cutbased_Mt_tau_plus; // cutbased Reconstructed invariant transverse mass of the single-top (tau+ channel)
    TH1F *fHistTopReco_cutbased_phi_tau_plus;// cutbased Reconstructed azimuth angle of the single-top (tau+ channel)
    TH1F *fHistTopReco_cutbased_eta_tau_plus;// cutbased Reconstructed Eta of the single-top (tau+ channel)
    TH1F *fHistTopReco_cutbased_pt_tau_plus; // cutbased Reconstructed Pt of the single-top (tau+ channel)
    TH1F *fHistTopReco_cutbased_Mt_tau_minus; // cutbased Reconstructed invariant transverse mass of the single-top (tau- channel)
    TH1F *fHistTopReco_cutbased_phi_tau_minus;// cutbased Reconstructed azimuth angle of the single-top (tau- channel)
    TH1F *fHistTopReco_cutbased_eta_tau_minus;// cutbased Reconstructed Eta of the single-top (tau- channel)
    TH1F *fHistTopReco_cutbased_pt_tau_minus; // cutbased Reconstructed Pt of the single-top (tau- channel)

    // top -> all leptons
    TH1F *fHistTopReco_cutbased_Mt;     // cutbased Reconstructed invariant transverse mass of the single-top (all leptons)
    TH1F *fHistTopReco_cutbased_phi;    // cutbased Reconstructed azimuth angle of the single-top (all leptons)
    TH1F *fHistTopReco_cutbased_eta;    // cutbased Reconstructed Eta of the single-top (all leptons)
    TH1F *fHistTopReco_cutbased_pt;     // cutbased Reconstructed Pt of the single-top (all leptons)
    TH1F *fHistTopReco_cutbased_Mt_plus;     // cutbased Reconstructed invariant transverse mass of the single-top (all positive sign leptons)
    TH1F *fHistTopReco_cutbased_phi_plus;    // cutbased Reconstructed azimuth angle of the single-top (all positive sign leptons)
    TH1F *fHistTopReco_cutbased_eta_plus;    // cutbased Reconstructed Eta of the single-top (all positive sign leptons)
    TH1F *fHistTopReco_cutbased_pt_plus;     // cutbased Reconstructed Pt of the single-top (all positive sign leptons)
    TH1F *fHistTopReco_cutbased_Mt_minus;     // cutbased Reconstructed invariant transverse mass of the single-top (all negative sign leptons)
    TH1F *fHistTopReco_cutbased_phi_minus;    // cutbased Reconstructed azimuth angle of the single-top (all negative sign leptons)
    TH1F *fHistTopReco_cutbased_eta_minus;    // cutbased Reconstructed Eta of the single-top (all negative sign leptons)
    TH1F *fHistTopReco_cutbased_pt_minus;     // cutbased Reconstructed Pt of the single-top (all negative sign leptons)

    // Truth Matching histograms
    // all histograms show the reconstructed value (e.g. pt) minus
    // the true value over the true value

    // Charged lepton
    TH1F *fHistElectronTruthMatch_pt;     // pt of Electron
    TH1F *fHistElectronTruthMatch_eta;    // eta of Electron
    TH1F *fHistElectronTruthMatch_phi;    // phi of Electron
    TH1F *fHistMuonTruthMatch_pt;         // pt of Muon
    TH1F *fHistMuonTruthMatch_eta;        // eta of Muon
    TH1F *fHistMuonTruthMatch_phi;        // phi of Muon

    // Neutrino
    TH1F *fHistNuTruthMatch_pt;       // pt of Neutrino, electron- and muon-channel
    TH1F *fHistNuTruthMatch_theta;    // theta of Neutrino, electron- and muon-channel
    TH1F *fHistNuTruthMatch_eta;      // eta of Neutrino, electron- and muon-channel
    TH1F *fHistNuTruthMatch_phi;      // phi of Neutrino, electron- and muon-channel
    TH1F *fHistNuTruthMatch_pt_e;     // pt of Neutrino, electron-channel
    TH1F *fHistNuTruthMatch_theta_e;  // theta of Neutrino, electron-channel
    TH1F *fHistNuTruthMatch_phi_e;    // phi of Neutrino, electron-channel
    TH1F *fHistNuTruthMatch_pt_mu;    // pt of Neutrino, muon-channel
    TH1F *fHistNuTruthMatch_theta_mu; // theta of Neutrino, muon-channel
    TH1F *fHistNuTruthMatch_phi_mu;   // phi of Neutrino, muon-channel

    // b-quark
    // matching of the b jet w.r.t. the MC b-quark
    TH1F *fHistBottomTruthMatch_pt;       // pt of b-quark, electron- and muon-channel
    TH1F *fHistBottomTruthMatch_eta;      // eta of b-quark, electron- and muon-channel
    TH1F *fHistBottomTruthMatch_phi;      // phi of b-quark, electron- and muon-channel
    TH1F *fHistBottomTruthMatch_pt_e;     // pt of b-quark, electron-channel
    TH1F *fHistBottomTruthMatch_eta_e;    // eta of b-quark, electron-channel
    TH1F *fHistBottomTruthMatch_phi_e;    // phi of b-quark, electron-channel
    TH1F *fHistBottomTruthMatch_pt_mu;    // pt of b-quark, muon-channel
    TH1F *fHistBottomTruthMatch_eta_mu;   // eta of b-quark, muon-channel
    TH1F *fHistBottomTruthMatch_phi_mu;   // phi of b-quark, muon-channel

    // matching of the jet w.r.t. the  hadron level jet
    // which matches the true b quark best
    TH1F *fHistHJetTruthMatch_pt;       // pt of b-jet, electron- and muon-channel
    TH1F *fHistHJetTruthMatch_eta;      // eta of b-jet, electron- and muon-channel
    TH1F *fHistHJetTruthMatch_phi;      // phi of b-jet, electron- and muon-channel
    TH1F *fHistHJetTruthMatch_pt_e;     // pt of b-jet, electron-channel
    TH1F *fHistHJetTruthMatch_eta_e;    // eta of b-jet, electron-channel
    TH1F *fHistHJetTruthMatch_phi_e;    // phi of b-jet, electron-channel
    TH1F *fHistHJetTruthMatch_pt_mu;    // pt of b-jet, muon-channel
    TH1F *fHistHJetTruthMatch_eta_mu;   // eta of b-jet, muon-channel
    TH1F *fHistHJetTruthMatch_phi_mu;   // phi of b-jet, muon-channel

    // b-quark
    // matching of the original b jet (before fitting)
    // w.r.t. the MC b-quark
    TH1F *fHistBottomTruthMatchOrig_pt;       // pt of b-quark, electron- and muon-channel
    TH1F *fHistBottomTruthMatchOrig_eta;      // eta of b-quark, electron- and muon-channel
    TH1F *fHistBottomTruthMatchOrig_phi;      // phi of b-quark, electron- and muon-channel

    // b-quark
    // matching of the original b jet (before fitting)
    // w.r.t. the  hadron level jet
    // which matches the true b quark best
    TH1F *fHistHJetTruthMatchOrig_pt;       // pt of b-jet, electron- and muon-channel
    TH1F *fHistHJetTruthMatchOrig_eta;      // eta of b-jet, electron- and muon-channel
    TH1F *fHistHJetTruthMatchOrig_phi;      // phi of b-jet, electron- and muon-channel

    // histograms for overall MC Px and Py distribution
    TH1F *fHistTrueSumPx;   // true sum of Px
    TH1F *fHistTrueSumPy;   // true sum of Py

  public:
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

    // Mode of running
    Bool_t fSignalSampleMode; // Running over signal MC?
    Bool_t fIsAcceptanceChallenge; // Doing acceptance challenge (then write out event list!)?
    Int_t fMaxNbIter;         // maximum number of iterations
    Bool_t fDoTtbarVeto;      // Apply ttbar veto?
    Bool_t fDoWhadVeto;       // Apply hadronic W veto?
    EScaleFactorSystematics fSFSystMode;// Scale Factor systematics mode

    // Further configuration
    Float_t fElPt_Min;        // Minimum signal electron pt
    Float_t fMuPt_Min;        // Minimum signal electron pt

    Float_t fWmassT_min;      // Minimum W transverse mass

    Double_t fChi2ProbMin;    // Minimum chi2-probability. Can be used later to apply
                              // the corresponding cut.
                              // So far it is only used in the cut-flow plot

    Bool_t fGuessNeutrinoEtaE;// Use routine to estimate good starting value of neutrino Eta?
    Bool_t fTruthMatch;       // Run truth-matching at all?
    Float_t fTruthMatchProb_min; //Minimum of matching reconstructedparticle with truth particle

    Bool_t fUseLeptonCovRand; // Use randomly generated lepton covariance matrices?
    Bool_t fIsFwdElectronMode; // Use only forward electrons when analyzing jet-electrons? If set to kFALSE, only central electrons will be analyzed.

    // testing a fix for large chi square values
    Double_t fMassConstraintParameters;

    // cut-based analysis cuts
    Float_t fLeadNonBJetEtaMin;
    Float_t fH_TMin;
    Float_t fDeltaEtaBJetNonBJetMin;
    Double_t fMassAllJetsMin;

    // Jet selection cuts
    Int_t                fNJetsMax;         // Maximum Number of Jets
    Int_t                fNJetsMin;         // Minimum Number of Jets
    Float_t              fJets_Et_min;      // Min. Et of generic jets
    Float_t              fJets_Eta_min;     // Min. eta of generic jets
    Float_t              fJets_Eta_max;     // Max. eta of generic jets

    // Selection cuts for b-jets used in top reconstruction
    Float_t              fBJets_Et_min;      // Min. Et of b-jets
    Float_t              fBJets_Eta_min;     // Min. eta of b-jets
    Float_t              fBJets_Eta_max;     // Max. eta of b-jets
    AtlBTag::ETagger     fBJetTagger;        // B-tagger used
    Float_t              fBJetWeightMin;     // B-tag weight cut
    AtlBTag::ETagger     fBJetRefTagger;     // B-tagger used, reference for QCD
    Float_t              fBJetRefWeightMin;  // B-tag weight cut, reference for QCD

    // Selection cuts for jets used in hadronic W reconstruction
    Float_t              fWhadJets_Et_min;      // Min. Et of jets from Whad
    Float_t              fWhadJets_Eta_min;     // Min. eta of jets from Whad
    Float_t              fWhadJets_Eta_max;     // Max. eta of jets from Whad

    // Fwd jet selection cuts
    Float_t              fFwdJets_Et_min;      // Min. Et of signal fwd jets
    Float_t              fFwdJets_Eta_min;     // Min. eta of signal fwd jets
    Float_t              fFwdJets_Eta_max;     // Max. eta of signal fwd jets
    AtlBTag::ETagger     fFwdJetTagger;        // Tagger used for signal fwd jets
    Float_t              fFwdBJetWeightMax;    // Maximum b-tag weight for jets to be accepted as a light fwd jet

    // Whad P(Chi2)-cut
    Float_t              fVetoWhadProbMin; // Min. chi2 probability for Whad veto

  public:
    AtlSgTop_tChannelFinder(const char* name, const char* title);
    virtual ~AtlSgTop_tChannelFinder();
    virtual void SetBranchStatus();
    virtual void BookHistograms();
    virtual void FillHistograms();
    virtual void SetCutDefaults();
    virtual Bool_t AnalyzeEvent();
    virtual void Terminate();
    virtual void Print() const;
    inline TH1F* GetHistChi2ProbBoth() { return fHistChi2Prob_both; }
    inline void SetJets    ( TList *jets )       { fJets = jets; }
    inline void SetLeptons   ( TList *leptons )      { fLeptons = leptons; }
    inline void SetEtmiss ( TVector2 etmiss ) {fMET = etmiss; }

  private:

    void InitEvent();
    void Clear();

    // KinFitter mode
    void SetNeutrinoStartingValues();
    void ReconstructionKinFit();
    void DoFit();
    Bool_t ApplyWhadVeto(Double_t chi2probMin, AtlJet* BestBJet);
    void DoHadronicWFit();

    // Cut-based mode
    void ReconstructionCutBased();
    void ReconstructW();
    void ReconstructTop();
    Int_t DoTruthMatching();

    // Event selection histograms
    void BookLeptonHistograms();
    void BookLeptonHistograms_BTag();
    void BookJetHistograms();
    void BookJetHistograms_BTag();

    void FillLeptonHistograms();
    void FillLeptonHistograms_BTag();
    void FillJetHistograms();
    void FillJetHistograms_BTag();
    void FillWhadJetHistograms();
    void FillWhadJetHistograms_BTag();
    void FillBJetHistograms();
    void FillBJetHistograms_BTag();
    void FillNonBJetHistograms();
    void FillNonBJetHistograms_BTag();
    void FillFwdJetHistograms();

    // Cuts of cut-based analysis
    void FillCutBasedCutsHistograms();

    // ==============
    // KinFitter mode
    // ==============

    // Reconstructed objects histograms
    void BookKinFitHistograms();
    void BookKinFitLeptonHistograms();
    void BookKinFitJetHistograms();
    void BookHadronicWHistograms(); // filled in ApplyWhadVeto()

    void FillKinFitHistograms();
    void FillKinFitLeptonHistograms();
    void FillKinFitJetHistograms();

    // Performance study histograms
    void BookChi2Histograms(); // filled in ReconstructionKinFit() and FillKinFitHistograms()
    void BookComparisonHistograms();
    void BookTruthMatchingHistograms();

    void FillComparisonHistograms();
    void FillTruthMatchingHistograms();
    void FillTruthMatchingHistogramsSignal();
    void FillTruthMatchingHistogramsBkg();

    // ==============
    // Cut-based mode
    // ==============

    // Reconstructed objects histograms
    void BookCutBasedHistograms();   // reconstructed W and top of cut-based mode1
    void BookCutBasedLeptonHistograms();
    void BookCutBasedJetHistograms();

    void FillCutBasedHistograms();
    void FillCutBasedLeptonHistograms();
    void FillCutBasedJetHistograms();

    inline Float_t GetSumPt(Float_t Ptx1,Float_t Pty1,Float_t Ptx2, Float_t Pty2){
      //returns the vector length of two P2 vectors in x-y-plane
      return TMath::Sqrt((Ptx1+Ptx2)*(Ptx1+Ptx2)+(Pty1+Pty2)*(Pty1+Pty2));
    }

    double Neutrino_Pz( double nu_px, double nu_py, double l_px, double l_py, double l_pz, double l_e, double& scf,double nu_pz_true);

    void Neutrino_Pz_Base( double nu_px, double nu_py,double l_px, double l_py, double l_pz, double l_e,double& scf,double &solution1, double &solution2);

    ClassDef(AtlSgTop_tChannelFinder,0)  // ATLAS single top t-channel finder
};
#endif

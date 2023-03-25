//
// Author: Thomas Kintscher <mailto: kintscher@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2010 (C) Thomas Kintscher
//
#ifndef ATLAS_AtlD0Finder
#define ATLAS_AtlD0Finder
#ifndef ATLAS_AtlKinFitterTool
#include <AtlKinFitterTool.h>
#include <TH1F.h>
#include <TH2F.h>
#endif
#include <fstream>

class TH1F;
class TH2F;
class AtlEvent;
class TTree;
class TString;
class TROOT;
class TFile;

class AtlD0Finder : public AtlKinFitterTool {

  public:
    enum EMode { kCutBased, kKinFit };
    EMode fMode;                       // Indicate the analysis mode (cut-based or using the kinematic fitter)
    Bool_t fDebug;                     // Toggle debug output (prints failed truth matching results)

  private:
    std::ofstream fDebugStream;        // Debug output

    // Lambda -> p pi(signal)
    TH1F *fHistD0reco_m_kaon_pi;
    TH1F *fHistLambdareco_m_PPi;       // Reconstructed invariant mass of the Lambda decaying into proton+pion
    TH1F *fHistLambdareco_m_PiPi;      // Reconstructed invariant mass with pion+pion hypothesis
    TH1F *fHistLambdareco_m_ee;        // Reconstructed invariant mass with ee hypothesis
    TH1F *fHistLambdareco_pt_PPi;      // Reconstructed azimuth angle of the Lambda decaying into proton+pion
    TH1F *fHistLambdareco_phi_PPi;     // Reconstructed Pt of the Lambda decaying into proton+pion
    TH1F *fHistLambdareco_eta_PPi;     // Reconstructed Eta of the Lambda decaying into proton+pion
    TH1F *fHistLambdareco_N_PPi;       // Multilplicity of reconstructed Lambda decaying into proton+pion-r
    TH1F *fHistLambdareco_tau_PPi;     // Lifetime of reconstructed lambdas
    TH2F *fHistLambdareco_truth_PPi;   // Pdg codes of truth-matched particles
    TH1F *fHistLambdareco_R_vtx;       // radial distance from vertex to beam pipe
    TH1F *fHistLambdareco_src_div;     // Angular divergence of lambda source and primary vertex
    TH1F *fHistLambdareco_tdcy_len;    // Transverse decay length of lambda
    TH1F *fHistLambdareco_oangle;      // Opening angle between proton/pion
    TH1F *fHistLambdareco_dangle;      // Decay angle between proton/pion (in lambda frame)

    TH1F *fHistLambdareco_Proton_pt;   // reconstructed pt  of proton
    TH1F *fHistLambdareco_Pion_pt;     // reconstructed pt  of pion
    TH1F *fHistLambdareco_Proton_eta;  // reconstructed eta of proton
    TH1F *fHistLambdareco_Pion_eta;    // reconstructed eta of pion
    TH1F *fHistLambdareco_Proton_phi;  // reconstructed phi of proton
    TH1F *fHistLambdareco_Pion_phi;    // reconstructed phi of pion

    TH1F *fHistLambdareco_Piplus_pt;   // reconstructed pt  of pion+
    TH1F *fHistLambdareco_Piplus_eta;  // reconstructed eta of pion+
    TH1F *fHistLambdareco_Piplus_phi;  // reconstructed phi of pion+
    TH1F *fHistLambdareco_Piminus_pt;  // reconstructed pt  of pion-
    TH1F *fHistLambdareco_Piminus_eta; // reconstructed eta of pion-
    TH1F *fHistLambdareco_Piminus_phi; // reconstructed phi of pion-

    TH1F *fHistLambdareco_Prplus_pt;   // reconstructed pt  of proton
    TH1F *fHistLambdareco_Prplus_eta;  // reconstructed eta of proton
    TH1F *fHistLambdareco_Prplus_phi;  // reconstructed phi of proton
    TH1F *fHistLambdareco_Prminus_pt;  // reconstructed pt  of antiproton
    TH1F *fHistLambdareco_Prminus_eta; // reconstructed eta of antiproton
    TH1F *fHistLambdareco_Prminus_phi; // reconstructed phi of antiproton

    // combinatorial background (same charged particles)
    TList *fBkgLambdaDecays;           // List of combinatorial bkg.

    // Lambda -> pipi(Bkg)
    TH1F *fHistLambdareco_m_PPi_bkg;       // Reconstructed invariant mass of the Lambda decaying into proton+pion
    TH1F *fHistLambdareco_pt_PPi_bkg;      // Reconstructed azimuth angle of the Lambda decaying into proton+pion
    TH1F *fHistLambdareco_phi_PPi_bkg;     // Reconstructed Pt of the Lambda decaying into proton+pion
    TH1F *fHistLambdareco_eta_PPi_bkg;     // Reconstructed Eta of the Lambda decaying into proton+pion
    TH1F *fHistLambdareco_N_PPi_bkg;       // Multiplicity of reconstructed Lambda decaying into proton+pion-r
    TH1F *fHistLambdareco_tau_PPi_bkg;     // Lifetime of reconstructed lambda background
    TH1F *fHistLambdareco_truth_PPi_bkg;   // Pdg codes of truth-matched particles
    TH1F *fHistLambdareco_R_vtx_bkg;       // radial distance from vertex to beam pipe (bkg)
    TH1F *fHistLambdareco_src_div_bkg;     // Angular divergence of lambda source and primary vertex
    TH1F *fHistLambdareco_tdcy_len_bkg;    // Transverse decay length of lambda
    TH1F *fHistLambdareco_oangle_bkg;      // Opening angle between proton/pion
    TH1F *fHistLambdareco_dangle_bkg;      // Decay angle between proton/pion (in lambda frame)

    TH1F *fHistLambdareco_Proton_pt_bkg;   // reconstructed pt  of proton
    TH1F *fHistLambdareco_Pion_pt_bkg;     // reconstructed pt  of pion
    TH1F *fHistLambdareco_Proton_eta_bkg;  // reconstructed eta of proton
    TH1F *fHistLambdareco_Pion_eta_bkg;    // reconstructed eta of pion
    TH1F *fHistLambdareco_Proton_phi_bkg;  // reconstructed phi of proton
    TH1F *fHistLambdareco_Pion_phi_bkg;    // reconstructed phi of pion

    TH1F *fHistLambdareco_Piplus_pt_bkg;   // reconstructed pt  of pion+
    TH1F *fHistLambdareco_Piplus_eta_bkg;  // reconstructed eta of pion+
    TH1F *fHistLambdareco_Piplus_phi_bkg;  // reconstructed phi of pion+
    TH1F *fHistLambdareco_Piminus_pt_bkg;  // reconstructed pt  of pion-
    TH1F *fHistLambdareco_Piminus_eta_bkg; // reconstructed eta of pion-
    TH1F *fHistLambdareco_Piminus_phi_bkg; // reconstructed phi of pion-

    TH1F *fHistLambdareco_Prplus_pt_bkg;   // reconstructed pt  of proton
    TH1F *fHistLambdareco_Prplus_eta_bkg;  // reconstructed eta of proton
    TH1F *fHistLambdareco_Prplus_phi_bkg;  // reconstructed phi of proton
    TH1F *fHistLambdareco_Prminus_pt_bkg;  // reconstructed pt  of antiproton
    TH1F *fHistLambdareco_Prminus_eta_bkg; // reconstructed eta of antiproton
    TH1F *fHistLambdareco_Prminus_phi_bkg; // reconstructed phi of antiproton

    // MC signal and background histos
    TH1F *fHistLambdareco_MC_m;            // reconstructed lambda mass
    TH1F *fHistLambdareco_MC_m_bkg;        // reconstructed lambda mass (bkg)
    TH1F *fHistLambdareco_MC_m_pipi;       // reconstructed lambda mass (pi/pi hypothesis)
    TH1F *fHistLambdareco_MC_m_pipi_bkg;   // reconstructed lambda mass (pi/pi hyp.) (bkg)
    TH1F *fHistLambdareco_MC_m_ppi;        // reconstructed lambda mass (p/pi hypothesis)
    TH1F *fHistLambdareco_MC_m_ppi_bkg;    // reconstructed lambda mass (p/pi hyp.) (bkg)
    TH1F *fHistLambdareco_MC_m_ee;         // reconstructed lambda mass (ee hypothesis)
    TH1F *fHistLambdareco_MC_m_ee_bkg;     // reconstructed lambda mass (ee hyp.) (bkg)
    TH1F *fHistLambdareco_MC_m_pipi_o;     // reconstructed lambda mass (pi/pi hypothesis) (orig. momenta)
    TH1F *fHistLambdareco_MC_m_pipi_o_bkg; // reconstructed lambda mass (pi/pi hyp.) (bkg) (o.m.)
    TH1F *fHistLambdareco_MC_m_ppi_o;      // reconstructed lambda mass (p/pi hypothesis) (o.m.)
    TH1F *fHistLambdareco_MC_m_ppi_o_bkg;  // reconstructed lambda mass (p/pi hyp.) (bkg) (o.m.)
    TH1F *fHistLambdareco_MC_m_ee_o;       // reconstructed lambda mass (ee hypothesis) (o.m.)
    TH1F *fHistLambdareco_MC_m_ee_o_bkg;   // reconstructed lambda mass (ee hyp.) (bkg) (o.m.)
    TH1F *fHistLambdareco_MC_pt;           // reconstructed lambda pt
    TH1F *fHistLambdareco_MC_pt_bkg;       // reconstructed lambda pt (bkg)
    TH1F *fHistLambdareco_MC_phi;          // reconstructed lambda phi
    TH1F *fHistLambdareco_MC_phi_bkg;      // reconstructed lambda phi (bkg)
    TH1F *fHistLambdareco_MC_eta;          // reconstructed lambda eta
    TH1F *fHistLambdareco_MC_eta_bkg;      // reconstructed lambda eta (bkg)
    TH1F *fHistLambdareco_MC_R_vtx;        // reconstructed lambda radius
    TH1F *fHistLambdareco_MC_R_vtx_bkg;    // reconstructed lambda radius (bkg);
    TH1F *fHistLambdareco_MC_tdcy_len;     // reconstructed lambda transverse decay length
    TH1F *fHistLambdareco_MC_tdcy_len_bkg; // reconstructed lambda transverse decay length (bkg)
    TH1F *fHistLambdareco_MC_tau;          // reconstructed lambda lifetime
    TH1F *fHistLambdareco_MC_tau_bkg;      // reconstructed lambda lifetime (bkg)
    TH1F *fHistLambdareco_MC_src_div;      // reconstructed lambda angle to primary vtx
    TH1F *fHistLambdareco_MC_src_div_bkg;  // reconstructed lambda angle to primary vtx (bkg)
    TH1F *fHistLambdareco_MC_dangle;       // reconstructed lambda decay angle
    TH1F *fHistLambdareco_MC_dangle_bkg;   // reconstructed lambda decay angle (bkg)
    TH1F *fHistLambdareco_MC_oangle;       // reconstructed lambda opening angle
    TH1F *fHistLambdareco_MC_oangle_bkg;   // reconstructed lambda opening angle (bkg)
    TH1F *fHistLambdareco_MC_coangle;      // reconstructed lambda cos opening angle
    TH1F *fHistLambdareco_MC_coangle_bkg;  // reconstructed lambda cos opening angle (bkg)
    TH1F *fHistLambdareco_MC_d0_K;
    TH1F *fHistLambdareco_MC_d0_K_bkg;
    TH1F *fHistLambdareco_MC_d0_Pi;
    TH1F *fHistLambdareco_MC_d0_Pi_bkg;
    TH1F *fHistLambdareco_MC_d0ovE_K;
    TH1F *fHistLambdareco_MC_d0ovE_K_bkg;
    TH1F *fHistLambdareco_MC_d0ovE_Pi;
    TH1F *fHistLambdareco_MC_d0ovE_Pi_bkg;
    TH1F *fHistLambdareco_MC_z0_Pi;        // reconstructed z0 of pion track
    TH1F *fHistLambdareco_MC_z0_Pi_bkg;    // reconstructed z0 of pion track (bkg)
    TH1F *fHistLambdareco_MC_z0_K;         // reconstructed z0 of kaon track
    TH1F *fHistLambdareco_MC_z0_K_bkg;     // reconstructed z0 of kaon track (bkg)
    TH1F *fHistLambdareco_MC_pi_tchi2;     // reconstructed chi2/ndf of pion track
    TH1F *fHistLambdareco_MC_pi_tchi2_bkg; // reconstructed chi2/ndf of pion track (bkg)
    TH1F *fHistLambdareco_MC_k_tchi2;      // reconstructed chi2/ndf of kaon track
    TH1F *fHistLambdareco_MC_k_tchi2_bkg;  // reconstructed chi2/ndf of kaon track (bkg)
    TH1F *fHistLambdareco_MC_z0_diff;      // difference of z0 of kaon and pion track
    TH1F *fHistLambdareco_MC_z0_diff_bkg;  // difference of z0 of kaon and pion track (bkg)

    TH2F *fHistLambdareco_MC_m_dangle;
    TH2F *fHistLambdareco_MC_m_dangle_bkg;

    // Misc. stuff
    TH1F *fHistLambdareco_chi2_vtx;        // Chi2/NDoF of vertex fits
    TH1F *fHistLambdareco_chi2_trk;        // Chi2/NDoF of track fits
    TH1F *fHistLambdareco_prob_mc;         // Truth matching probabilities
    TH1F *fHistLambdareco_eff;             // Efficiency histogram

    Int_t fN_MCFail;                       // how often did the truth matching fail
    Int_t fN_Fits;                         // count of successful fits
    Int_t fN_MCProbMiss;                   // how often was truth matching prob below threshold
    Int_t fN_SameOriginMiss;               // count of truth matched tracks with different mother

    TH1F *fHist_MC_KPi_vtx;                // associated vertices of kaon/pion tracks

public:
    // selection cuts
    Float_t              fSignal_Pt_min;          // Min. Pt of signal pion/proton
    Float_t              fSignal_Pt_max;          // Max. Pt of signal pion/proton
    Float_t              fSignal_Eta_min;         // Min. eta of signal pion/proton
    Float_t              fSignal_Eta_max;         // Max. eta of signal pion/proton

    Float_t              fD0_M_min;                   // Lower boundary for allowed Lambda mass range
    Float_t              fD0_M_max;                   // Upper boundary for allowed Lambda mass range
    Float_t              fm_e;                        // Electron mass hypothesis
    Float_t              fm_lambda;                   // Lambda mass hypothesis
    Float_t              fm_pi;                       // Pion mass hypothesis
    Float_t              fm_proton;                   // Proton mass hypothesis
    Float_t              fm_kaon;
    Float_t              fm_D0;
    Float_t              fm_Dstar;

    Float_t              fAngleToPrimary_max;         // Maximum angle divergence to primary vertex
    Float_t              fVertexChi2ovNDoF_max;       // Quality of vertex fit
    Float_t              fTrackChi2ovNDoF_max;        // Quality of track fit
    Float_t              fKinFitPvalue_min;           // Quality of kinematic fit
    Float_t              fMatchingProb_min;           // Quality of truth matching
    Float_t              fPhotonMass_max;             // Maximum invariant mass of e+/e- test
    Float_t              fOAngle_max;                 // Maximum of opening Angle between Kaon-Pion
    Float_t              fD0_K_max;                   // Maximum D0 (track parameter) of Kaon
    Float_t              fD0_K_min;                   // Minimum D0 (track parameter) of Kaon
    Float_t              fD0_Pi_max;                  // Maximum D0 (track parameter) of Pion
    Float_t              fD0_Pi_min;                  // Minimum D0 (track parameter) of Pion
    Float_t              fZDiff_max;

    TKinFitter PerformFit(AtlIDTrack *trk1, AtlIDTrack *trk2, TLorentzVector &FitP_trk1, TLorentzVector &FitP_trk2,
              TMatrixD *cov_trk1, TMatrixD *cov_trk2);
    Int_t DoTruthMatch(AtlIDTrack *trk1, AtlIDTrack *trk2, HepVertex *Vtx);
    void FillMCHistograms(AtlD0DecayKPi *decay, Bool_t signal);
    Bool_t IsConversion(AtlIDTrack *trk1, AtlIDTrack *trk2);
    Bool_t IsKaon(AtlIDTrack *trk1, AtlIDTrack *trk2);
    Bool_t IsLambda(AtlIDTrack *trk1, AtlIDTrack *trk2);
    void FindSharedTracks();

public:
    AtlD0Finder(const char* name, const char* title);
    virtual ~AtlD0Finder();
    virtual void SetBranchStatus();
    virtual void BookHistograms();
    virtual void FillHistograms();
    virtual void SetCutDefaults();
    virtual Bool_t AnalyzeEvent();
    void ReconstructLambdaCutBased(void);
    void ReconstructLambdaKinFit(void);
    void PrintCutValues(void);
    void Terminate();

    inline void SetMode(EMode mode = kKinFit) { fMode = mode; }
    void SetDebugOutput(Bool_t mode = kFALSE) { fDebug = mode; }
    inline Bool_t IsDebugRun() { return fDebug; }

    ClassDef(AtlD0Finder,0) // Lambda decay finder class
};
#endif

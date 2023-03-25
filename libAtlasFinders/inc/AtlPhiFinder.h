//
// Author: Sophia Schade <mailto: schade@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2010 (C) Sophia Schade
//
#ifndef ATLAS_AtlPhiFinder
#define ATLAS_AtlPhiFinder
#ifndef ATLAS_AtlKinFitterTool
#include <AtlKinFitterTool.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
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

class AtlPhiFinder : public AtlKinFitterTool {

  public:
    enum EMode { kCutBased, kKinFit };
    EMode fMode;                      // Indicate the mode of running (eg cut-based or kinematic fit)

    Bool_t fDebug;                    // Indicate the mode of running (data, or mc with test-histograms,
                                      // true means mctest)
    Bool_t fUseKaonPurity;            // different purity criteria

  private:
    std::ofstream fDebugOutput;       // Debugging output
    // General tuning histograms
    TH1F *fHistKaonMatchingProb;      // Matching probability of Particles matching the reconstructed Kaons
    TH1F *fHistTrkChi2ovNDoF;         // Chi2/NDoF for tracks surviving the kinematic fitter
    TH1F *fHistVtxChi2ovNDoF;         // Chi2/NDoF for vertices that fulfil Phi hypothesis

    // SIGNAL: Phi -> KK
    // ----------------------
    // standard histograms
    TH1F *fHistPhireco_m_KK;          // Reconstructed invariant mass of the Phi decaying into Kaons
    TH1F *fHistPhireco_pt_KK;         // Reconstructed azimuth angle of the Phi decaying into Kaons
    TH1F *fHistPhireco_phi_KK;        // Reconstructed Pt of the Phi decaying into Kaons
    TH1F *fHistPhireco_eta_KK;        // Reconstructed Eta of the Phi decaying into Kaons
    TH1F *fHistPhireco_N_KK;          // Multilplicity of reconstructed Phi decaying into K+K-r
    TH1F *fHistPhireco_TRK;           // Trks and their particle

    // kaons histograms
    TH1F *fHistPhireco_KK_OpeningAngle; // Angle between K+ K-
    TH1F *fHistPhireco_DecayAngle_KK;   // Decay Angle between Phi and K+ in Phis restframe
    TH1F *fHistPhireco_KPlus_pt;        // recosntructed pt  of K+
    TH1F *fHistPhireco_KMinus_pt;       // recosntructed pt  of K-
    TH1F *fHistPhireco_KPlus_eta;       // recosntructed eta of K+
    TH1F *fHistPhireco_KMinus_eta;      // recosntructed eta of K-
    TH1F *fHistPhireco_KPlus_phi;       // recosntructed phi of K+
    TH1F *fHistPhireco_KMinus_phi;      // recosntructed phi of K-

    // BACKGROUND: Like-sign approach (Phi -> K+K+/ K-K-)
    // ----------------------------------------------------
    TList *fBkgPhiDecays;              // List of combinatorial bkg
    // Phi -> KK (Bkg)
    TH1F *fHistPhireco_m_KK_bkg;       // Reconstructed invariant mass of the Phi decaying into Kaons
    TH1F *fHistPhireco_pt_KK_bkg;      // Reconstructed azimuth angle of the Phi decaying into Kaons
    TH1F *fHistPhireco_phi_KK_bkg;     // Reconstructed Pt of the Phi decaying into Kaons
    TH1F *fHistPhireco_eta_KK_bkg;     // Reconstructed Eta of the Phi decaying into Kaons
    TH1F *fHistPhireco_N_KK_bkg;       // Multilplicity of reconstructed Phi decaying into K+K-

    // kaons histograms
    TH1F *fHistPhireco_KK_OpeningAngle_bkg;  // Angle between K+ K+ or K- K- (Bkg)
    TH1F *fHistPhireco_KK_DecayAngle_bkg; // Angle between Phi and  K+/K- (Bkg) in Phis restframe
    TH1F *fHistPhireco_KPlus_pt_bkg;         // recosntructed pt  of K+
    TH1F *fHistPhireco_KMinus_pt_bkg;        // recosntructed pt  of K-
    TH1F *fHistPhireco_KPlus_eta_bkg;        // recosntructed eta of K+
    TH1F *fHistPhireco_KMinus_eta_bkg;       // recosntructed eta of K-
    TH1F *fHistPhireco_KPlus_phi_bkg;        // recosntructed phi of K+
    TH1F *fHistPhireco_KMinus_phi_bkg;       // recosntructed phi of K-

    // ====================
    //    MC TRUTH STUDY
    // ====================

    TH1F *fHistPhi_MC_KKsame;             // MC Truth Matching, pdg codes: 2 Kaons same Mother
    TH1F *fHistPhi_MC_same;               // MC Truth Matching, pdg codes: same Mother
    TH1F *fHistPhireco_m_KK_diff;         // Phi mass (fg-bg)
    TH1F *fHistPhireco_MCParticle_Prob;   // Probability to get right MC Particle
    TH1F *fHistPhireco_Chi_TrkFit;        // Chi^2 from Track with Vtx Fit
    TH1F *fHistPhireco_Chi_VtxFit;        // Chi^2 from Vertex  Fit

    // SIGNAL: Reconstructed Phi candidates that  not fail the MCTruthMatching
    // ---------------------------------------------------------------------

    // standard histograms
    TH1F *fHistPhireco_m_MCsignal;       // Reconstructed invariant mass of the Kaon decaying into kaons
    TH1F *fHistPhireco_pt_MCsignal;      // Reconstructed azimuth angle of the Kaon decaying into kaons
    TH1F *fHistPhireco_phi_MCsignal;     // Reconstructed Pt of the Kaon decaying into kaons
    TH1F *fHistPhireco_eta_MCsignal;     // Reconstructed Eta of the Kaon decaying into kaons
    TH1F *fHistPhireco_N_MCsignal;       // Multilplicity of reconstructed Phi decaying into K+K-

    TH1F *fHistPhireco_Number_Hits_MCsignal;
    TH1F *fHistPhireco_ChiNDoF_Trks_MCsignal;

    TH1F *fHistPhireco_OpeningAngle_MCsignal;   // Opening Angle of the Kaon tracks in the lab frame
    TH1F *fHistPhireco_DecayAngle_MCsignal;     // Decay angle between Phi and K+ in Phis restframe

    // BACKGROUND: Reconstructed Phi candidates that fail the MCTruthMatching
    // ----------------------------------------------------------------------

    TH1F *fHistPhireco_m_MCbkg;       // Reconstructed invariant mass of Phi-candidate
    TH1F *fHistPhireco_pt_MCbkg;      // Reconstructed azimuth angle of Phi-candidate
    TH1F *fHistPhireco_phi_MCbkg;     // Reconstructed Pt of Phi-candidate
    TH1F *fHistPhireco_eta_MCbkg;     // Reconstructed Eta of Phi-candidate
    TH1F *fHistPhireco_N_MCbkg;       // Multilplicity of Phi-candidate

    TH1F *fHistPhireco_Number_Hits_MCbkg;
    TH1F *fHistPhireco_ChiNDoF_Trks_MCbkg;

    TH1F *fHistPhireco_OpeningAngle_MCbkg;   // Opening Angle of the Kaon tracks in the lab frame
    TH1F *fHistPhireco_DecayAngle_MCbkg;     // Decay angle between Phi and K+ in Phis restframe

    // dE/dx HISTOGRAMS: Needed for Kaon tagging
    // -----------------------------------------

    TH2F *fHistdEdx_pmag;                    // dE/dx as a function of the momentum for all tracks
                                             // leaving the primary vertex
    TH2F *fHistdEdx_pmag_sig;
    TH2F *fHistdEdx_pmag_MCsig;
    TH2F *fHistdEdx_pmag_bkg;
    TH2F *fHistdEdx_pmag_MCbkg;
    TH2F *fHistdEdx_betagamma;

    TH1F *fHistdEdx_diff_sig;
    TH1F *fHistdEdx_diff_MCsig;              // difference between measured and calculated dEdx
    TH1F *fHistdEdx_diff_bkg;                // for MC Signal and Background
    TH1F *fHistdEdx_diff_MCbkg;

    TF1  *fBetheBloch;                       // Fit function

  public:
  // selection cuts
  Float_t              fSignalKaon_Pt_min;          // Min. Pt of signal electrons
  Float_t              fSignalKaon_Pt_max;          // Max. Pt of signal electrons
  Float_t              fSignalKaon_Eta_min;         // Min. eta of signal electrons
  Float_t              fSignalKaon_Eta_max;         // Max. eta of signal electrons
  Float_t              fSignalKaon_EtCone20_max;    // Max. Et within a cone of 0.2

  Float_t              fPhi_M_min;                  // Lower boundary for allowed Phi mass range
  Float_t              fPhi_M_max;                  // Upper boundary for allowed Phi mass range
  Float_t              fm_K;                        // Kaon mass hypothesis
  Float_t              fm_Phi;                      // Phi mass from PDG

  Float_t              fVertexChi2ovNDoF_max;       // Quality of Vertex fit
  Float_t              fTrackChi2ovNDoF_max;        // Quality of track fit
  Float_t              fAngle_max;                  // Angle criterion
  Float_t              fMatchingProb_min;           // Quality of MC Matching
  Float_t              fKinFitPvalue_min;           // min prob(chi2,ndof) from kinfit

  Float_t              fdEdx_Delta_max;
  Float_t              fPmag_min;
  Float_t              fPmag_max;

  Float_t              fPhi_Eta_min;
  Float_t              fPhi_Eta_max;

  Float_t              fdEdx_p0;                    // Values from Bethe-Bloch like Fit
  Float_t              fdEdx_p1;
  Float_t              fdEdx_p2;
  Float_t              fdEdx_p3;
  Float_t              fdEdx_p4;

  public:
  AtlPhiFinder(const char* name, const char* title);
  virtual ~AtlPhiFinder();
  virtual void SetBranchStatus();
  virtual void BookHistograms();
  virtual void FillHistograms();
  virtual void SetCutDefaults();
  virtual Bool_t AnalyzeEvent();
  void ReconstructPhiCutBased(void);
  void ReconstructPhiKinFit(void);
  void PrintCutValues(void);
  void Terminate();
  Int_t DoTruthMatch(AtlIDTrack *trk1, AtlIDTrack *trk2);
  void FillHistograms_MCTruthStudy(AtlPhiDecayKK *PhiCandidate, char type);
  Double_t* GetLogBins(Int_t BinCount, Double_t MinBinExpo, Double_t MaxBinExpo);
  Float_t Delta_dEdx(AtlIDTrack *trk1);
  void FindSharedTracks();

  inline void SetMode(EMode mode = kKinFit)
  { fMode = mode; }
  // Test-mode or Run-mode
  inline void SetDebugOutput(Bool_t mode = kFALSE)
  { fDebug = mode; }
  inline Bool_t IsDebugRun() { return fDebug; }

  ClassDef(AtlPhiFinder,0) // Phi decay finder class
};
#endif

//
// Author: Sebastian Beumler <mailto: beumler@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2010 (C) Sebastian Beumler
//
#ifndef ATLAS_AtlK0sFinder
#define ATLAS_AtlK0sFinder
#ifndef ATLAS_AtlKinFitterTool
#include <AtlKinFitterTool.h>
#include <TH1F.h>
#include <TH2F.h>
#endif
#include <fstream>
#include <iostream>

class TH1F;
class AtlEvent;
class TTree;
class TString;
class TROOT;
class TFile;

class AtlK0sFinder : public AtlKinFitterTool {

  public:
    enum EMode { kCutBased, kKinFit };
    EMode  fMode;              // Indicate the mode of running (eg cut-based or kinematic fit)
    Bool_t fDebugMode;         // Indicate the mode of running (if debug output should be available)

  private:
    std::ofstream fDebugOutput;     // Debugging output
    Int_t    fNK0s;


    // Bkg decay angle analysis
    Int_t NLambdapos, NLambdaBarpos, NPiPluspos, NProtonpos;
    Int_t NLambdaneg, NLambdaBarneg, NPiPlusneg, NProtonneg;


    // General tuning histograms
    TH1F *fHistPionMatchingProb;         // Matching probability of Particles matching the reconstructed Pions
    TH1F *fHistTrkChi2ovNDoF;            // Chi2/NDoF for tracks surviving the kinematic fitter
    TH1F *fHistVtxChi2ovNDoF;            // Chi2/NDoF for vertices that fulfil K0s hypothesis


    // SIGNAL: K0s -> pipi
    // -----------------------

    TH1F *fHistK0sreco_m_PiPi;       // Reconstructed invariant mass of the Kaon decaying into pions
    TH1F *fHistK0sreco_pt_PiPi;      // Reconstructed azimuth angle of the Kaon decaying into pions
    TH1F *fHistK0sreco_phi_PiPi;     // Reconstructed Pt of the Kaon decaying into pions
    TH1F *fHistK0sreco_eta_PiPi;     // Reconstructed Eta of the Kaon decaying into pions
    TH1F *fHistK0sreco_N_PiPi;       // Multilplicity of reconstructed K0s decaying into pi+pi-r
    TH1F *fHistK0sreco_tau_PiPi;      // Lifetime of reconstructed K0s
    TH1F *fHistK0sreco_DecayLength_PiPi;    // DecayLength of reconstructed K0s
    TH1F *fHistK0sreco_OpeningAngle_PiPi;   // Opening Angle of the pion tracks in the lab frame
    TH1F *fHistK0sreco_DecayAngle_PiPi;     //Decay angle between K0s and pi+ in K0s' restframe
    TH1F *fHistK0sreco_PointingAngle_PiPi;  //cos of angle between reconstructed K0s' momentum and vec(PV)-vec(SecVtxa)
    TH1F *fHistK0sreco_R_vtx;               // radial distance from vertex to beam pipe
    TH2F *fHistK0sreco_RvtxMee;             // 2d histogram for r_vtx and M_ee inv. Mass
    // pion histograms
    TH1F *fHistK0sreco_PiPlus_pt;    // recosntructed pt  of pi+
    TH1F *fHistK0sreco_PiMinus_pt;   // recosntructed pt  of pi-
    TH1F *fHistK0sreco_PiPlus_eta;   // recosntructed eta of pi+
    TH1F *fHistK0sreco_PiMinus_eta;  // recosntructed eta of pi-
    TH1F *fHistK0sreco_PiPlus_phi;   // recosntructed phi of pi+
    TH1F *fHistK0sreco_PiMinus_phi;  // recosntructed phi of pi-
    TH2F *fHistK0sreco_Pions_PtEta;  // recosntructed Pt and eta of both Pions



    // BACKGROUND: Like-sign approach (K0s->pi+pi+ \ K0s->pi-pi-)
    // -----------------------------------------------------------
    TList *fBkgK0sDecays;            // List of like sign K0s decays
    // K0s -> pipi(Bkg)
    TH1F *fHistK0sreco_m_LS_bkg;       // invariant mass of the Kaon decaying into equally signed pions
    TH1F *fHistK0sreco_pt_LS_bkg;      // azimuth angle of the Kaon decaying into equally signed pions
    TH1F *fHistK0sreco_phi_LS_bkg;     // Pt of the Kaon decaying into equally signed pions
    TH1F *fHistK0sreco_eta_LS_bkg;     // Eta of the Kaon decaying into equally signed pions
    TH1F *fHistK0sreco_N_LS_bkg;       // Multilplicity of reconstructed K0s decaying into equally signed pions
    TH1F *fHistK0sreco_tau_LS_bkg;     // Lifetime of reconstructed K0s decaying into equally signed pions
    TH1F *fHistK0sreco_DecayLength_LS_bkg;  // DecayLength of reconstructed K0s decaying ito equaly signed pions

    // pion histograms
    TH1F *fHistK0sreco_PiPlus_pt_LS_bkg;    // recosntructed pt  of pi+
    TH1F *fHistK0sreco_PiMinus_pt_LS_bkg;   // recosntructed pt  of pi-
    TH1F *fHistK0sreco_PiPlus_eta_LS_bkg;   // recosntructed eta of pi+
    TH1F *fHistK0sreco_PiMinus_eta_LS_bkg;  // recosntructed eta of pi-
    TH1F *fHistK0sreco_PiPlus_phi_LS_bkg;   // recosntructed phi of pi+
    TH1F *fHistK0sreco_PiMinus_phi_LS_bkg;  // recosntructed phi of pi-



    // ====================
    //    MC TRUTH STUDY
    // ====================

    TH2F *fHistK0sreco_truthtype;   // truth type of K0s candidates
    TH1F *fHistAllMCK0s_DecayAngle;

    // BACKGROUND: Reconstructed K0s candidates that fail the MCTruthMatching
    // -------------------------------------------------------------------

    TH1F *fHistK0sreco_m_MCbkg;       // Reconstructed invariant mass of K0s-candidate
    TH1F *fHistK0sreco_pt_MCbkg;      // Reconstructed azimuth angle of K0s-candidate
    TH1F *fHistK0sreco_phi_MCbkg;     // Reconstructed Pt of K0s-candidate
    TH1F *fHistK0sreco_eta_MCbkg;     // Reconstructed Eta of K0s-candidate
    TH1F *fHistK0sreco_N_MCbkg;       // Multilplicity of K0s-candidate
    TH1F *fHistK0sreco_tau_MCbkg;     // Lifetime of reconstructed K0s-candidate
    TH1F *fHistK0sreco_DecayLength_MCbkg;    // DecayLength of K0s-candidate
    TH1F *fHistK0sreco_OpeningAngle_MCbkg;   // Opening Angle of the pion tracks in the lab frame
    TH1F *fHistK0sreco_DecayAngle_MCbkg;     // Decay angle between K0s and pi+ in K0s' restframe
    TH1F *fHistK0sreco_PointingAngle_MCbkg;  // cos of angle between reconstructed K0s' momentum and vec(PV)-vec(SecVtx)
    TH1F *fHistK0sreco_R_vtx_MCbkg;          // radial distance from vertex to beam pipe
    TH1F *fHistK0sreco_R_vtx_MCbkg_gamma;    // radial distance from photo-conversion-vertex to beam pipe
    TH2F *fHistK0sreco_RvtxMee_MCbkg;        // 2d histogram for r_vtx and M_ee inv. Mass
    TH1F *fHistK0sreco_Chi2_MCbkg;         //
    TH1F *fHistK0sreco_pValue_MCbkg;       //
    TH1F *fHistK0sreco_Chi2Trk_MCbkg;         //
    TH1F *fHistK0sreco_Chi2Vtx_MCbkg;       //



    // Decay Angle Analysis
    TH1F *fHist_MCK0s_DecayAngle;      // Decay Angle for all mc K0s'
    TH1F *fHist_DecayAngle_0to06;      // Decay Angle from  0     to  0.6
    TH1F *fHist_DecayAngle_06to094;    // Decay Angle from  0.6   to  0.94
    TH1F *fHist_DecayAngle_094to1;     // Decay Angle from  0.94  to  1.0
    TH1F *fHist_DecayAngleNeg_0to06;   // Decay Angle from  0     to -0.6
    TH1F *fHist_DecayAngleNeg_06to094; // Decay Angle from  -0.6  to -0.94
    TH1F *fHist_DecayAngleNeg_094to1;  // Decay Angle from  -0.94 to -1.0

    TH1F *fHist_DA_Comb;   // Decay Angle  Bkg (Combinatorial)
    TH1F *fHist_DA_Gamma;  // Decay Angle  Bkg (Photon Conversions)
    TH1F *fHist_DA_Lambda; // Decay Angle  Bkg (Lambda Decays)


    TH1F *fHist_PionsRight_Pt;          // Pt of Pions [DecayAngle from 0.6 to 0.94]
    TH1F *fHist_PionsLeft_Pt;           // Pt of Pions [DecayAngle from -0.6 to -0.94]

    // Analyse
    TH1F *fHist_OwnBoost;
    TH1F *fHist_InvMass;
    TH1F *fHist_InvMass_OwnBoost;
    TH1F *fHist_SubView;


    // SIGNAL: Reconstructed K0s candidates that fail the MCTruthMatching
    // ---------------------------------------------------------------------

    TH1F *fHistK0sreco_m_MCsignal;       // Reconstructed invariant mass of the Kaon decaying into pions
    TH1F *fHistK0sreco_pt_MCsignal;      // Reconstructed azimuth angle of the Kaon decaying into pions
    TH1F *fHistK0sreco_phi_MCsignal;     // Reconstructed Pt of the Kaon decaying into pions
    TH1F *fHistK0sreco_eta_MCsignal;     // Reconstructed Eta of the Kaon decaying into pions
    TH1F *fHistK0sreco_N_MCsignal;       // Multilplicity of reconstructed K0s decaying into pi+pi
    TH1F *fHistK0sreco_tau_MCsignal;      // Lifetime of reconstructed K0s
    TH1F *fHistK0sreco_DecayLength_MCsignal;    // DecayLength of reconstructed K0s
    TH1F *fHistK0sreco_OpeningAngle_MCsignal;   // Opening Angle of the pion tracks in the lab frame
    TH1F *fHistK0sreco_DecayAngle_MCsignal;     //Decay angle between K0s and pi+ in K0s' restframe
    TH1F *fHistK0sreco_PointingAngle_MCsignal;  //cos of angle between reconstructed K0s' momentum and vec(PV)-vec(SecVt
    TH1F *fHistK0sreco_R_vtx_MCsignal;          // radial distance from vertex to beam pipe
    TH2F *fHistK0sreco_RvtxMee_MCsignal;        // 2d histogram for r_vtx and M_ee inv. Mass

    TH1F *fHistK0sreco_Chi2_MCsignal;         //
    TH1F *fHistK0sreco_pValue_MCsignal;       //
    TH1F *fHistK0sreco_Chi2Trk_MCsignal;         //
    TH1F *fHistK0sreco_Chi2Vtx_MCsignal;       //

  public:
  // selection cuts
  Float_t  fSignalPion_Pt_min;          // Min. Pt of signal electrons
  Float_t  fSignalPion_Pt_max;          // Max. Pt of signal electrons
  Float_t  fSignalPion_Eta_min;         // Min. eta of signal electrons
  Float_t  fSignalPion_Eta_max;         // Max. eta of signal electrons
  Float_t  fSignalPion_EtCone20_max;    // Max. Et within a cone of 0.2

  Float_t  fMatchingProb_min;           // Min. of Matching probability
  Float_t  fK0s_M_min;                   // Lower boundary for allowed Z mass range
  Float_t  fK0s_M_max;                  // Upper boundary for allowed Z mass range
  Float_t  fm_pi;                       // Pion mass hypothesis

  Float_t  fVertexChi2ovNDoF_max;       // Quality of Vertex fit
  Float_t  fTrackChi2ovNDoF_max;        // Quality of Track fit
  Float_t  fKinFitPvalue_min;           // Minimal probability of Chi2/NDoF required
                                        // after converging kinematic fit
  Float_t  fTransvDecayLength_min;      // Minimal probability of Chi2/NDoF required
  Float_t  fPointingAngle_min;          // Minimal cos(PointingAngle)w
  Float_t  fDecayAngle_max;             // Maximum of DecayAngle (Angle between flightdirection of K0s
                                        // and momentum of pi+ in K0s' rerstframe)
  Float_t  fOpeningAngle_min;           // Angle between pion trks




  public:
  AtlK0sFinder(const char* name, const char* title);
  virtual ~AtlK0sFinder();
  virtual void SetBranchStatus();
  virtual void BookHistograms();
  virtual void FillHistograms();
  virtual void SetCutDefaults();
  virtual Bool_t AnalyzeEvent();
  void ReconstructK0sCutBased(void);
  void ReconstructK0sKinFit(void);
  void PrintCutValues(void);
  Int_t DoTruthMatch(AtlK0sDecayPiPi *K0sCandidate);
  Bool_t IsLambdaCandidate(HepVertex *vtx);
  void FillHistograms_MCTruthStudy(AtlK0sDecayPiPi *K0sCandidate, char type);
  virtual void Terminate(void);

  inline void SetMode(EMode mode = kKinFit)
  { fMode = mode; }
  inline void SetDebugMode(Bool_t DebugMode = kFALSE)
  { fDebugMode = DebugMode; }
  inline Bool_t IsDebugRun() { return fDebugMode; }


  ClassDef(AtlK0sFinder,0) // K0s decay finder class
};
#endif

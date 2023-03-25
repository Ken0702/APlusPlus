
// Author: Sebastian Beumler <mailto: beumler@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2010 (C) Sebastian Beumler
//
#ifndef ATLAS_AtlConvFinder
#define ATLAS_AtlConvFinder
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

class AtlConvFinder : public AtlKinFitterTool {

  public:
    enum EMode { kCutBased, kKinFit };
    EMode  fMode;              // Indicate the mode of running (eg cut-based or kinematic fit)
    Bool_t fDebugMode;         // Indicate the mode of running (if debug output should be available)


  private:
    std::ofstream fDebugOutput;      // Debugging output
    // Number of (truely))reconstructed Photo-Conv, number of rec Conv where no truthmatch decission was possible.
    Int_t    fN_TrueRecConv,fN_RecConv, fN_NoDecision;

    TH1F *fHisteMatchingProb;   // Matching probability of Particles matching the reconstructed elektrons


    // SIGNAL: gamma -> e+e-
    // -----------------------

    TH1F *fHistConvreco_m_ee;       // Reconstructed invariant mass of the conversion-candidate
    TH1F *fHistConvreco_pt_ee;      // Reconstructed pt
    TH1F *fHistConvreco_phi_ee;     // Reconstructed phi
    TH1F *fHistConvreco_eta_ee;     // Reconstructed Eta
    TH1F *fHistConvreco_N_ee;       // Multilplicity of reconstructed conversion candidates
    TH1F *fHistConvreco_tau_ee;            // Lifetime
    TH1F *fHistConvreco_DecayLength_ee;    // DecayLength
    TH1F *fHistConvreco_OpeningAngle_ee;   // Opening Angle
    TH1F *fHistConvreco_DecayAngle_ee;     // Decay angle between gamma and e+ in K0s' restframe
    TH1F *fHistConvreco_PointingAngle_ee;  // cos of angle between reconstructed gamma' momentum and vec(PV)-vec(SecVtxa)
    TH1F *fHistConvreco_R_vtx;             // radial distance from vertex to beam pipe
    // elektron histograms
    TH1F *fHistConvreco_ePlus_pt;    // recosntructed pt  of e+
    TH1F *fHistConvreco_eMinus_pt;   // recosntructed pt  of e-
    TH1F *fHistConvreco_ePlus_eta;   // recosntructed eta of e+
    TH1F *fHistConvreco_eMinus_eta;  // recosntructed eta of e-
    TH1F *fHistConvreco_ePlus_phi;   // recosntructed phi of e+
    TH1F *fHistConvreco_eMinus_phi;  // recosntructed phi of e-



    // BACKGROUND: Like-sign approach (gamma->e+e+ \ gamma->e-e-)
    // -----------------------------------------------------------
    TList *fBkgPhotonConv;            // List of like sign conversions

    TH1F *fHistConvreco_m_LS_bkg;       // invariant mass of the Kaon decaying into equally signed pions
    TH1F *fHistConvreco_pt_LS_bkg;      // azimuth angle of the Kaon decaying into equally signed pions
    TH1F *fHistConvreco_phi_LS_bkg;     // Pt of the Kaon decaying into equally signed pions
    TH1F *fHistConvreco_eta_LS_bkg;     // Eta of the Kaon decaying into equally signed pions
    TH1F *fHistConvreco_N_LS_bkg;       // Multilplicity of reconstructed Conv decaying into equally signed pions
    TH1F *fHistConvreco_tau_LS_bkg;     // Lifetime of reconstructed Conv decaying into equally signed pions
    TH1F *fHistConvreco_DecayLength_LS_bkg;  // DecayLength of reconstructed Conv decaying ito equaly signed pions

    // electron histograms
    TH1F *fHistConvreco_ePlus_pt_LS_bkg;    // recosntructed pt  of e+
    TH1F *fHistConvreco_eMinus_pt_LS_bkg;   // recosntructed pt  of e-
    TH1F *fHistConvreco_ePlus_eta_LS_bkg;   // recosntructed eta of e+
    TH1F *fHistConvreco_eMinus_eta_LS_bkg;  // recosntructed eta of e-
    TH1F *fHistConvreco_ePlus_phi_LS_bkg;   // recosntructed phi of e+
    TH1F *fHistConvreco_eMinus_phi_LS_bkg;  // recosntructed phi of e-


    // ====================
    //    MC TRUTH STUDY
    // ====================

    TH2F *fHistConvreco_truthtype;   // truth type of Conv candidates
    TH1F *fHistAllMCConv_DecayAngle;

    // BACKGROUND: Reconstructed conv candidates that fail the MCTruthMatching
    // -------------------------------------------------------------------

    TH1F *fHistConvreco_m_MCbkg;       // Reconstructed invariant mass of conv-candidate
    TH1F *fHistConvreco_pt_MCbkg;      // Reconstructed transv. momentum
    TH1F *fHistConvreco_phi_MCbkg;     // Reconstructed phi
    TH1F *fHistConvreco_eta_MCbkg;     // Reconstructed Eta
    TH1F *fHistConvreco_N_MCbkg;       // Multilplicity of conv-candidate
    TH1F *fHistConvreco_tau_MCbkg;     // Lifetime of reconstructed conv-candidate
    TH1F *fHistConvreco_DecayLength_MCbkg;    // DecayLength
    TH1F *fHistConvreco_OpeningAngle_MCbkg;   // Opening Angle of the electron tracks in the lab frame
    TH1F *fHistConvreco_DecayAngle_MCbkg;     // Decay angle between conv and e+ in K0s' restframe
    TH1F *fHistConvreco_PointingAngle_MCbkg;  // cos of angle between reconstructed conv' momentum and vec(PV)-vec(SecVtx)
    TH1F *fHistConvreco_R_vtx_MCbkg;          // radial distance from vertex to beam pipe

    // SIGNAL: Reconstructed Conv candidates that pass the MCTruthMatching
    // ---------------------------------------------------------------------

    TH1F *fHistConvreco_m_MCsignal;       // Reconstructed invariant mass mass of conv-candidate
    TH1F *fHistConvreco_pt_MCsignal;      // Reconstructed transv. momentum
    TH1F *fHistConvreco_phi_MCsignal;     // Reconstructed phi
    TH1F *fHistConvreco_eta_MCsignal;     // Reconstructed Eta
    TH1F *fHistConvreco_N_MCsignal;       // Multilplicity of reconstructed photoconversions
    TH1F *fHistConvreco_tau_MCsignal;      // Lifetime of reconstructed gammas
    TH1F *fHistConvreco_DecayLength_MCsignal;    // DecayLengt
    TH1F *fHistConvreco_OpeningAngle_MCsignal;   // Opening Angle of the electron tracks in the lab frame
    TH1F *fHistConvreco_DecayAngle_MCsignal;     // Decay angle between gamma and e+ in K0s' restframe
    TH1F *fHistConvreco_PointingAngle_MCsignal;  // cos of angle between gamma's momentum and vec(PV)-vec(SecVt
    TH1F *fHistConvreco_R_vtx_MCsignal;          // radial distance from vertex to beam pipe

  public:
  // selection cuts

    // hard coded
    Float_t  fSignale_Pt_min;          // Min. Pt of signal electrons
    Float_t  fSignale_Pt_max;          // Max. Pt of signal electrons
    Float_t  fSignale_Eta_min;         // Min. eta of signal electrons
    Float_t  fSignale_Eta_max;         // Max. eta of signal electrons
    Float_t  fm_e;                        // electron mass hypothesis

    // KinFit
    Float_t  fKinFitPvalue_min;           // Minimal probability of Chi2/NDoF required (KinFit)
    Float_t  fm_PolMass;                  // Pol mass for kinematic fitting (KinFit)
    Float_t  fm_Width;                    // Decay Width for kinematic fitting (KinFit)
    // CutBased
    Float_t  fConv_M_max;                 // Upper boundary for allowed photon mass (Cutbased)

    // both
    Float_t  fMatchingProb_min;           // Min. of Matching probability
    Float_t  fTransvDecayLength_min;      // Minimal DecayLength
    Float_t  fPointingAngle_min;          // Minimal cos(PointingAngle)
    Float_t  fDecayAngle_min;             // Minimal DecayAngle (Angle between flightdirection of gamma
                                          // and momentum of e+ in K0s' restframe (well defined) )
    Float_t  fR_Vtx_min;                  // Minimum of radial distance from rec. conv Vtx to nominal beamline

  public:
  AtlConvFinder(const char* name, const char* title);
  virtual ~AtlConvFinder();
  virtual void SetBranchStatus();
  virtual void BookHistograms();
  virtual void FillHistograms();
  virtual void SetCutDefaults();
  virtual Bool_t AnalyzeEvent();
  void ReconstructConvCutBased(void);
  void ReconstructConvKinFit(void);
  void PrintCutValues(void);
  Int_t DoTruthMatch(AtlPhotonConv *ConvCandidate);
  void FillHistograms_MCTruthStudy(AtlPhotonConv *ConvCandidate, char type);
  virtual void Terminate(void);

  inline void SetMode(EMode mode = kKinFit)
  { fMode = mode; }
  inline void SetDebugMode(Bool_t DebugMode = kFALSE)
  { fDebugMode = DebugMode; }
  inline Bool_t IsDebugRun() { return fDebugMode; }


  ClassDef(AtlConvFinder,0) // decay finder class
};
#endif

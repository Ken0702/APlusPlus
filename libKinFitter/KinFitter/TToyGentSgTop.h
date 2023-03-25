#ifndef TToyGentSgTop_h
#define TToyGentSgTop_h

/**
    @class TToyGentSgTop
    Author: Patrick Rieck <mailto:rieck@physik.hu-berlin.de>
    Copyright: 2010 (C) Patrick Rieck
    class for generating Toy MC Single Top events
    The top quark is assumed to be at rest. In every loop the momenta
    of a massless b-Quark and a W-boson are given randomly from isotropic decay.
    The same is done with the W-boson decaying into massless particles taken as
    an electron and a neutrino.
    The masses of the top quark and the W boson are given by Breit-Wigner distributions.
    The structure of this program is based on TToyGentT from Thomas Goepfert.
*/

#include <vector>

#include "TMatrixD.h"
#include "TNamed.h"
#include "TObjArray.h"
#include "TH1.h"
#include "TH2.h"
#include "TVector3.h"

#include "KinFitter/TFitConstraintM.h"
#include "KinFitter/TFitConstraintMGaus.h"
#include "KinFitter/TFitConstraintMBW.h"
#include "KinFitter/TFitConstraintMBW2.h"
#include "KinFitter/TAbsFitParticle.h"
#include "KinFitter/TFitParticlePtEtaPhi.h"
#include "KinFitter/TFitParticleRelPtEtaPhi.h"
#include "KinFitter/TFitParticlePtThetaPhi.h"
#include "KinFitter/TFitParticlePxPyPz.h"
#include "KinFitter/TFitParticleRelPxPyPz.h"
#include "KinFitter/TKinFitter.h"

class TAbsFitParticle;
class TAbsFitConstraint;

class TToyGentSgTop : public TNamed {

public :

  TToyGentSgTop();
//  TToyGentSgTop(){};
  virtual ~TToyGentSgTop();
  void SetParticles();
  void SetFitter();
  Bool_t DoToyExperiments( Int_t NbExperiments = 1000 );

  Bool_t kBW; // use Breit-Wigner (kTRUE) constraint? Or Gauﬂ (kFALSE) ?

  // true 3 vectors
  TVector3 fPElectronTrue;
  TVector3 fPNeutrinoTrue;
  TVector3 fPBQuarkTrue;

  TH1F* fHistStatus;
  TH1F* fHistNIter;
  TH1F* fHistPChi2;
  TH1F* fHistChi2;

  TH1F* fHistTrueTopMass;
  TH1F* fHistTrueWMass;
  TH1F* fHistRecoTopMass;
  TH1F* fHistRecoWMass;

  TH1F* fHistMassParameterW;           // W boson mass constraint parameter
  TH1F* fHistMassParameterTop;         // top quark mass constraint parameter
  TH1F* fHistMassParameterBoth;        // both mass constraint parameters
  TH2F* fHistChi2VsMassParameters;     // Chi2 of Fit vs. constraint mass parameters
  TH2F* fHistChi2ProbVsMassParameters; // Chi2 probability of Fit vs. constraint mass parameters

  TH1F *fHistTruthMatchElectronPt;
  TH1F *fHistTruthMatchElectronEta;
  TH1F *fHistTruthMatchElectronPhi;
  TH1F *fHistTruthMatchNeutrinoPt;
  TH1F *fHistTruthMatchNeutrinoEta;
  TH1F *fHistTruthMatchNeutrinoPz;
  TH1F *fHistTruthMatchNeutrinoPhi;
  TH1F *fHistTruthMatchBQuarkPt;
  TH1F *fHistTruthMatchBQuarkEta;
  TH1F *fHistTruthMatchBQuarkPhi;

  TH1F *fHistPullElectronPt;
  TH1F *fHistPullElectronEta;
  TH1F *fHistPullElectronPhi;
  TH1F *fHistPullNeutrinoPt;
  TH1F *fHistPullNeutrinoPhi;
  TH1F *fHistPullBQuarkPt;
  TH1F *fHistPullBQuarkEta;
  TH1F *fHistPullBQuarkPhi;

  TH1F* fHistMWTrue;
  TH1F* fHistMWSmear;
  TH1F* fHistMWFit;
  TH1F* fHistMTopTrue;
  TH1F* fHistMTopSmear;
  TH1F* fHistMTopFit;

  TObjArray fHistsParTrue;
  TObjArray fHistsParSmear;
  TObjArray fHistsParFit;

  TObjArray fHistsPull1;
  TObjArray fHistsError1;
  TObjArray fHistsDiff1;
  TObjArray fHistsPull2;
  TObjArray fHistsError2;
  TObjArray fHistsDiff2;

  TObjArray fHistsPull3;
  TObjArray fHistsError3;
  TObjArray fHistsDiff3;

  int _nPar;

  void SetprintPartIni(Bool_t value) { fPrintPartIni = value; }
  void SetprintConsIni(Bool_t value) { fPrintConsIni = value; }
  void SetprintSmearedPartBefore(Bool_t value) { fPrintSmearedPartBefore = value; }
  void SetprintPartAfter(Bool_t value) { fPrintPartAfter = value; }
  void SetprintConsBefore(Bool_t value) { fPrintConsBefore = value; }
  void SetprintConsAfter(Bool_t value) { fPrintConsAfter = value; }
  void SetCheckConstraintsTruth(Bool_t value) { fDoCheckConstraintsTruth = value; }
  void SetVerbosity(int verbosity){fVerbosity=verbosity;};

protected:

  void SmearParticles();
  void BookHistograms();
  void FillHistograms();
  void FillConstraints();
  void GuessTopDecayNeutrinoEtaE(Double_t& EtaStart, Double_t& EStart );
  TKinFitter fFitter;

  TAbsFitParticle* fElectronStarter;
  TAbsFitParticle* fNeutrinoStarter;
  TAbsFitParticle* fBQuarkStarter;

  TAbsFitParticle* fElectron;
  TAbsFitParticle* fNeutrino;
  TAbsFitParticle* fBQuark;

  TFitConstraintM *fMConsW;
  TFitConstraintM *fMConsTop;

  private :

  Double_t fTrueTopMass;
  Double_t fTrueWMass;
  Double_t fTopMass;
  Double_t fTopWidth;
  Double_t fWMass;
  Double_t fWWidth;

  Bool_t fPrintPartIni;
  Bool_t fPrintConsIni;
  Bool_t fPrintSmearedPartBefore ;
  Bool_t fPrintConsBefore;
  Bool_t fPrintConsAfter;
  Bool_t fPrintPartAfter;
  Bool_t fDoCheckConstraintsTruth;

  int fVerbosity;

  ClassDef(TToyGentSgTop, 0)
};

#endif

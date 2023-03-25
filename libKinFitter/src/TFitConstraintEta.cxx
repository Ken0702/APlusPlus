// //Classname: TFitConstraintEta
// Author: Patrick Rieck <mailto:rieck@physik.hu-berlin.de>
// Authors of original KinFitter: Jan E. Sundermann, Verena Klose, 
// Thomas Goepfert (TU Dresden)      


//________________________________________________________________
// 
// TFitConstraintEta::
// --------------------
//
// Fit constraint: Eta distribution constraint
// enables to use MC information of Eta of a certain particle
//
// distribution is provided via histogram EtaHist that shows
// the distribution of the absolute value of Eta
// 
// EtaHist is supposed to have exactly one maximum
//
// constraint: f=F(|Eta|)-G(y+Delta)=0
//
// F  = primitive of |Eta| pdf f_Eta
// F  = int_{-infinity}^{|Eta|} f_Eta (x) dx
//
// G  = primitive of standardized Gauss-pdf g,
// G  = int_{-infinity}^{y+Delta} g(x) dx
// y  = Transformation of |Eta|, appears in Minimum Function S=...+y^2
// Delta: used to realize equivalence between y=0 and |Eta|=|Eta|_max
// f_Eta (|Eta|_max) is Maximum Likelihood
//
// This is a first version of a 'soft' constraint for eta of a certain particle.
// 
// As an input the user provides the histogram EtaHist for the eta distrubution
// which is supposed to have one maximum.
// eta is then transformed to a new variable 'y' which is of standardized Gauﬂ
// distribution while the maximum value of eta corresponds to y=0 (mean value of y).
// y^2 is added to the minimum function (chi2).
// That means the mathematics are similar to TFitConstraintMBW, but
// the eta distribution may also be rather broad ('soft' constraint).
// It MAY be used in an analysis for signal extraction but it is
// only a first version. It was tested and works as expected but
// it is not yet shown that it can improve an analysis.
// 
// Also don't get confused by possibly strange chi2-probability distributions.
// chi2-probability is computed with chi2 probability densities which hold for
// linear constraints. TFitConstraintEta is definitely non linear so better
// look for chi2 only and not for chi2 probability.
// 



using namespace std;

#include <iostream>

#include "KinFitter/TFitConstraintEta.h"

#include "TLorentzVector.h"
#include "TClass.h"

ClassImp(TFitConstraintEta);

//----------------
// Constructor --
//----------------
TFitConstraintEta::TFitConstraintEta()
  : TAbsFitConstraint() 
{
  _particle=0;
  _EtaHist=0;
}

//________________________________________________________________

TFitConstraintEta::TFitConstraintEta(TAbsFitParticle *particle, TH1D *EtaHist)
  : TAbsFitConstraint()
{
  _particle=0;
  _EtaHist=0;
  addParticle(particle);
  setHistogram(EtaHist);
  init();

}

//________________________________________________________________

TFitConstraintEta::TFitConstraintEta(const TString &name, const TString &title,
				     TAbsFitParticle *particle, TH1D *EtaHist)
  : TAbsFitConstraint( name, title)
{
  _particle=0;
  _EtaHist=0;
  addParticle(particle);  
  setHistogram( EtaHist );
  init();

}

//________________________________________________________________

void TFitConstraintEta::addParticle( TAbsFitParticle *particle ) {

  _particle=particle;
  if ( _EtaHist!=0 ) init();

}

//________________________________________________________________

void TFitConstraintEta::setHistogram( TH1D *EtaHist ) {

  _EtaHist=EtaHist;
  if ( _particle!=0  ) init();

}

//________________________________________________________________

void TFitConstraintEta::init() {

  _nPar = 1;
  _iniparameters.ResizeTo(1,1);
  _iniparameters(0,0) = 0.; // S=...+y^2, push y to 0 <=> push
                            // |Eta| to |Eta|_max
  _parameters.ResizeTo(1,1);
  _parameters = _iniparameters;

  // preparation of histogram

  _EtaHist->Scale(1./_EtaHist->Integral());

  // compute Delta

  _Delta  = (Double_t)_EtaHist->GetMaximumBin();
  _Delta  = 2*_EtaHist->Integral(1,(Int_t)_Delta)-1;
  _Delta  = TMath::ErfInverse(_Delta);
  _Delta *= 1.4142135623731; // 1.41421356237309 == sqrt(2)

  setCovMatrix( 0 );
  _covMatrix(0,0) = 1.; // S=...+y^2, variable y - the transformation
                        // of Eta - is of standardized Gauss distribution

}



//--------------
// Destructor --
//--------------
TFitConstraintEta::~TFitConstraintEta() {

}

//--------------
// Operations --
//--------------

Double_t TFitConstraintEta::getEtaPrimitive( Double_t Eta ) {

  Double_t Primitive=_EtaHist->Integral(1,_EtaHist->FindBin(TMath::Abs(Eta)));
  return Primitive;

}

//________________________________________________________________

Double_t TFitConstraintEta::getGaussPrimitive( Double_t arg ) {

  Double_t Primitive=1-0.5*TMath::Erfc(7.07106781e-01*(arg));
  // 7.07106781e-01 = 1/sqrt(2)
  return Primitive;

}

//________________________________________________________________

Double_t TFitConstraintEta::getInitValue() {
  // Get initial value of constraint (before the fit)

  Double_t InitValue = _particle->getIni4Vec()->Eta();
  InitValue = getEtaPrimitive(InitValue)-
              getGaussPrimitive(_iniparameters(0,0)+_Delta);
  return InitValue;

}

//________________________________________________________________

Double_t TFitConstraintEta::getCurrentValue() {
  // Get value of constraint after the fit

  Double_t CurrentValue = _particle->getCurr4Vec()->Eta();
  CurrentValue = getEtaPrimitive(CurrentValue)-
                 getGaussPrimitive(_parameters(0,0)+_Delta);
  return CurrentValue;

}

//________________________________________________________________

TMatrixD* TFitConstraintEta::getDerivative( TAbsFitParticle *particle ) { 
  // returns derivative df/dP with P=(p,E) and f the constraint (f=0).
  // The matrix contains one row (df/dp, df/dE).

  TMatrixD* DerivativeMatrix = new TMatrixD(1,4);
  (*DerivativeMatrix) *= 0.;
  
  if ( particle!=_particle ) return DerivativeMatrix;
  
  Double_t Eta = _particle->getCurr4Vec()->Eta();
  Double_t Pt  = _particle->getCurr4Vec()->Pt();
  Double_t E   = _particle->getCurr4Vec()->E();
  Double_t Pz  = _particle->getCurr4Vec()->Pz();
  
  Int_t bin  = _EtaHist->FindBin(TMath::Abs(Eta));
  Double_t p = _EtaHist->GetBinContent(bin);
  Double_t w = _EtaHist->GetBinWidth(bin);

  Double_t dev3 = p/(w*Pt*TMath::CosH(Eta));
  if ( Eta<0 ) dev3*=-1.;
  
  (*DerivativeMatrix)(0,2)=dev3;
  (*DerivativeMatrix)(0,3)=dev3*E/Pz;
    
  return DerivativeMatrix;

}

//________________________________________________________________

TMatrixD* TFitConstraintEta::getDerivativeAlpha() {
  // compute df/dy = -1 * g(y+_Delta)

  TMatrixD* DerivativeMatrix = new TMatrixD(1,1);
  DerivativeMatrix->Zero();

  (*DerivativeMatrix)(0,0) = -1.*TMath::Gaus(_parameters(0,0)+_Delta, 0., 1., true);

  return DerivativeMatrix;

}

//________________________________________________________________

void TFitConstraintEta::print() {

  cout << "__________________________" << endl << endl;
  cout <<"OBJ: " << IsA()->GetName() << "\t" << GetName() << "\t" << GetTitle() << endl;

  cout << "initial value: " << getInitValue() << endl;
  cout << "current value: " << getCurrentValue() << endl;
  cout << "Delta: " << _Delta << endl;
  cout << "initial parameter: " << _iniparameters(0,0) << endl;
  cout << "current parameter: " << _parameters(0,0) << endl;

}

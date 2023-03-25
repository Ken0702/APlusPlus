// //Classname: TFitConstraintMBW2
// Author: Patrick Rieck <mailto:rieck@physik.hu-berlin.de>
// Authors of original KinFitter: Jan E. Sundermann, Verena Klose, 
// Thomas Goepfert (TU Dresden)      


//________________________________________________________________
// 
// TFitConstraintMBW2::
// ---------------------
//
// Fit constraint: Breit-Wigner mass distribution
// This is a mathematical modification of TFitConstraintMBW
//
// constraint: f=M-F^{-1}[G(mu)]=0
//
// F      = primitive of Breit-Wigner pdf f_BW
// F      = int_{-infinity}^{M} f_BW (x) dx
// M      = invariant mass of particles p_i
// F^{-1} = inversion of F
// 
// G      = primitive of standardized Gauss-pdf g,
// G      = int_{-infinity}^{mu} g(x) dx
// mu     = Transformation of M, appears in Minimum Function S=...+mu^2
// 
// That means TFitConstraintMBW2 is the same as TFitConstraintMBW
// from the mathematics point of view.
//
// This implies that there is no extra routine getDerivative() here.
// It is inherited from TFitConstraintM.  
// 

using namespace std;

#include <iostream>

#include "KinFitter/TFitConstraintMBW2.h"

#include "TLorentzVector.h"
#include "TClass.h"

ClassImp(TFitConstraintMBW2);

//----------------
// Constructor --
//----------------
TFitConstraintMBW2::TFitConstraintMBW2()
  : TFitConstraintM() 
{

  init();

}

//________________________________________________________________

TFitConstraintMBW2::TFitConstraintMBW2(vector<TAbsFitParticle*>* ParList, 
					 Double_t Mass,
					 Double_t Width)
  : TFitConstraintM(ParList, 0, Mass ) 
{

  init();
  setMassConstraint( Mass, Width );

}

//________________________________________________________________

TFitConstraintMBW2::TFitConstraintMBW2(const TString &name, const TString &title,
					 vector<TAbsFitParticle*>* ParList, 
					 Double_t Mass,
					 Double_t Width)
  : TFitConstraintM( name, title, ParList, 0, Mass )
{

  init();
  setMassConstraint( Mass, Width );

}

//________________________________________________________________

void TFitConstraintMBW2::init() {

  _nPar = 1;
  _iniparameters.ResizeTo(1,1);
  _iniparameters(0,0) = 0.; // S=...+mu^2, push mu to 0 <=> push
                            // invariant mass of particles 
                            // to pole mass Mass
  _parameters.ResizeTo(1,1);
  _parameters = _iniparameters;

}

//--------------
// Destructor --
//--------------
TFitConstraintMBW2::~TFitConstraintMBW2() {

}

//--------------
// Operations --
//--------------

void TFitConstraintMBW2::setMassConstraint(Double_t Mass, Double_t Width) { 
  
  _TheMassConstraint = Mass;
  _width = Width;
  setCovMatrix( 0 );
  _covMatrix(0,0) = 1.; // S=...+mu^2, transformed mass mu is of
                        // standardized Gauss distribution

}

//________________________________________________________________

Double_t TFitConstraintMBW2::getMuDependence( Bool_t initial ) {
  
  // compute the term of the constraint that involves mu
  
  Double_t mu;
  if ( initial ) { mu = _iniparameters(0,0); }
  else { mu = _parameters(0,0); }


  Double_t Value = 1.57079633*TMath::Erf(7.07106781e-01*mu);
  Value  = TMath::Tan(Value);
  Value *= 0.5*_width;
  Value += _TheMassConstraint;
  // 1.57079633=pi/2
  // 7.07106781e-01 = 1/sqrt(2)
  return Value;

}

//________________________________________________________________

Double_t TFitConstraintMBW2::getInitValue() {
  // Get initial value of constraint (before the fit)

  Double_t InitValue =  
    CalcMass( &_ParList1, true ) -
    getMuDependence( true );
  return InitValue;

}

//________________________________________________________________

Double_t TFitConstraintMBW2::getCurrentValue() {
  // Get value of constraint after the fit

  Double_t CurrentValue =
    CalcMass( &_ParList1, false ) -
    getMuDependence( false );
  return CurrentValue;

}

//________________________________________________________________

TMatrixD* TFitConstraintMBW2::getDerivativeAlpha() { 
  // Calculate df/dmu
  
  TMatrixD* DerivativeMatrix = new TMatrixD(1,1);
  DerivativeMatrix->Zero();
  
  (*DerivativeMatrix)(0,0) = -1.*6.26657069e-01*_width*
    TMath::Exp(-0.5*_parameters(0,0)*_parameters(0,0));
  Double_t c = TMath::Cos(1.57079633*TMath::Erf(7.07106781e-01*_parameters(0,0)));
  c *= c;
  (*DerivativeMatrix)(0,0) /= c;
  
  // 6.26657069e-01=sqrt(pi/2)/2
  // 1.57079633=pi/2
  // 7.07106781e-01 = 1/sqrt(2)
  
  return DerivativeMatrix;
  
}

//________________________________________________________________

void TFitConstraintMBW2::print() {

  cout << "__________________________" << endl << endl;
  cout <<"OBJ: " << IsA()->GetName() << "\t" << GetName() << "\t" << GetTitle() << endl;

  cout << "initial value: " << getInitValue() << endl;
  cout << "current value: " << getCurrentValue() << endl;
  cout << "mean mass: " << _TheMassConstraint << endl;
  cout << "width: " << _width << endl;
  cout << "initial transformed mass: " << _iniparameters(0,0) << endl;
  cout << "current transformed mass: " << _parameters(0,0) << endl;

}

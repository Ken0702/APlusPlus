// //Classname: TFitConstraintEta
// Author: Patrick Rieck <mailto:rieck@physik.hu-berlin.de>
// Authors of original KinFitter: Jan E. Sundermann, Verena Klose, 
// Thomas Goepfert (TU Dresden)      


//________________________________________________________________
// 
// TFitConstraintMBW::
// --------------------
//
// Fit constraint: Breit-Wigner mass distribution
//
// constraint: f=F(M)-G(mu)=0
//
// F  = primitive of Breit-Wigner pdf f_BW
// F  = int_{-infinity}^{M} f_BW (x) dx
// M  = invariant mass of particles p_i
// 
// G  = primitive of standardized Gauss-pdf g,
// G  = int_{-infinity}^{mu} g(x) dx
// mu = Transformation of M, appears in Minimum Function S=...+mu^2

using namespace std;

#include <iostream>

#include "KinFitter/TFitConstraintMBW.h"
#include "KinFitter/TFitConstraintM.h"

#include "TLorentzVector.h"
#include "TClass.h"

ClassImp(TFitConstraintMBW);

//----------------
// Constructor --
//----------------
TFitConstraintMBW::TFitConstraintMBW()
  : TFitConstraintM() 
{

  init();

}

//________________________________________________________________

TFitConstraintMBW::TFitConstraintMBW(vector<TAbsFitParticle*>* ParList, 
					 Double_t Mass,
					 Double_t Width)
  : TFitConstraintM(ParList, 0, Mass ) 
{

  init();
  setMassConstraint( Mass, Width );

}

//________________________________________________________________

TFitConstraintMBW::TFitConstraintMBW(const TString &name, const TString &title,
					 vector<TAbsFitParticle*>* ParList, 
					 Double_t Mass,
					 Double_t Width)
  : TFitConstraintM( name, title, ParList, 0, Mass )
{

  init();
  setMassConstraint( Mass, Width );

}

//________________________________________________________________

void TFitConstraintMBW::init() {

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
TFitConstraintMBW::~TFitConstraintMBW() {

}

//--------------
// Operations --
//--------------

void TFitConstraintMBW::setMassConstraint(Double_t Mass, Double_t Width) { 
  
  _TheMassConstraint = Mass;
  _width = Width;
  setCovMatrix( 0 );
  _covMatrix(0,0) = 1.; // S=...+mu^2, transformed mass mu is of
                        // standardized Gauss distribution

}

//________________________________________________________________

Double_t TFitConstraintMBW::getBWPrimitive( Bool_t initial ) {

  Double_t Primitive=CalcMass( &_ParList1, initial )-_TheMassConstraint;
  Primitive *= 2/_width;
  Primitive  = TMath::ATan(Primitive)/(TMath::Pi())+0.5;
  return Primitive;

}

//________________________________________________________________

Double_t TFitConstraintMBW::getGaussPrimitive( Bool_t initial ) {
  
  Double_t mu;
  if ( initial ) { mu = _iniparameters(0,0); }
  else { mu = _parameters(0,0); }


  Double_t Primitive=1-0.5*TMath::Erfc(7.07106781e-01*mu);
  // 7.07106781e-01 = 1/sqrt(2)
  return Primitive;

}

//________________________________________________________________

Double_t TFitConstraintMBW::getInitValue() {
  // Get initial value of constraint (before the fit)

  Double_t InitValue =  
    getBWPrimitive( true ) -
    getGaussPrimitive( true );
  return InitValue;

}

//________________________________________________________________

Double_t TFitConstraintMBW::getCurrentValue() {
  // Get value of constraint after the fit
    Double_t CurrentValue =
	getBWPrimitive( false ) -
	getGaussPrimitive( false );
    return CurrentValue;
    
}

//________________________________________________________________

TMatrixD* TFitConstraintMBW::getDerivative( TAbsFitParticle *particle ) { 
  // returns derivative df/dP with P=(p,E) and f the constraint (f=0).
  // The matrix contains one row (df/dp, df/dE).

  TMatrixD* DerivativeMatrix = new TMatrixD(1,4);
  DerivativeMatrix->Zero();

  DerivativeMatrix     = TFitConstraintM::getDerivative(particle);
  (*DerivativeMatrix) *= TMath::BreitWigner(CalcMass(&_ParList1,false),
						 _TheMassConstraint,_width);
  return DerivativeMatrix;

}

//________________________________________________________________

TMatrixD* TFitConstraintMBW::getDerivativeAlpha() { 
  // Calculate df/dmu = -1 * g(mu)

  TMatrixD* DerivativeMatrix = new TMatrixD(1,1);
  DerivativeMatrix->Zero();

  (*DerivativeMatrix)(0,0) = -1.*TMath::Gaus(_parameters(0,0), 0., 1., true);

  return DerivativeMatrix;

}

//________________________________________________________________

void TFitConstraintMBW::print() {

  cout << "__________________________" << endl << endl;
  cout <<"OBJ: " << IsA()->GetName() << "\t" << GetName() << "\t" << GetTitle() << endl;

  cout << "initial value: " << getInitValue() << endl;
  cout << "current value: " << getCurrentValue() << endl;
  cout << "mean mass: " << _TheMassConstraint << endl;
  cout << "width: " << _width << endl;
  cout << "initial transformed mass: " << _iniparameters(0,0) << endl;
  cout << "current transformed mass: " << _parameters(0,0) << endl;

}

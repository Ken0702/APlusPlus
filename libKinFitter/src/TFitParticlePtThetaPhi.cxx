// //Classname: TFitParticlePtThetaPhi
// Author: Patrick Rieck, <mailto: rieck@physik.hu-berlin.de>
// Authors of original KinFitter package: Thomas Goepfert, Jan E. Sundermann, 
// Verena Klose (TU Dresden)      

//________________________________________________________________
// 
// TFitParticlePtThetaPhi::
// --------------------
//
// Particle parameterized by pT, Theta, Phi.
// fixed mass (3 free parameters). That means:
//
// p = (pT, Theta, Phi)
// E(fit) =  Sqrt( |p|^2 + m^2 )
//

using namespace std;

#include <iostream>

#include "KinFitter/TFitParticlePtThetaPhi.h"

#include "TLorentzVector.h"
#include "TMath.h"

ClassImp(TFitParticlePtThetaPhi)

//----------------
// Constructor --
//----------------
TFitParticlePtThetaPhi::TFitParticlePtThetaPhi()
  :TAbsFitParticle()
{ 
  init( 0, 0., 0);
}

TFitParticlePtThetaPhi::TFitParticlePtThetaPhi( const TFitParticlePtThetaPhi& fitParticle )
  :TAbsFitParticle( fitParticle.GetName(), fitParticle.GetTitle() )
{

  _nPar = fitParticle._nPar;
  _u1 = fitParticle._u1;
  _u2 = fitParticle._u2;
  _u3 = fitParticle._u3;
  _covMatrix.ResizeTo(  fitParticle._covMatrix );
  _covMatrix = fitParticle._covMatrix;
  _iniparameters.ResizeTo( fitParticle._iniparameters );
  _iniparameters = fitParticle._iniparameters;
  _parameters.ResizeTo( fitParticle._parameters );
  _parameters = fitParticle._parameters;
  _pini = fitParticle._pini;
  _pcurr = fitParticle._pcurr;

}

TFitParticlePtThetaPhi::TFitParticlePtThetaPhi(TVector3* p, Double_t M, const TMatrixD* theCovMatrix)
  :TAbsFitParticle()
{ 
  init(p, M, theCovMatrix);
}

TFitParticlePtThetaPhi::TFitParticlePtThetaPhi(const TString &name, const TString &title,
			                     TVector3* p, Double_t M, const TMatrixD* theCovMatrix)
  :TAbsFitParticle(name, title)
{ 
  init(p, M, theCovMatrix);
}

TAbsFitParticle* TFitParticlePtThetaPhi::clone( TString newname ) const {
  // Returns a copy of itself
  
  TAbsFitParticle* myclone = new TFitParticlePtThetaPhi( *this );
  if ( newname.Length() > 0 ) myclone->SetName(newname);
  return myclone;

}

//--------------
// Destructor --
//--------------
TFitParticlePtThetaPhi::~TFitParticlePtThetaPhi() {

}


//--------------
// Operations --
//--------------
void TFitParticlePtThetaPhi::init(TVector3* p, Double_t M, const TMatrixD* theCovMatrix) {

  _nPar = 3;
  setIni4Vec(p, M);
  setCovMatrix(theCovMatrix);
  
}
 
TLorentzVector* TFitParticlePtThetaPhi::calc4Vec( const TMatrixD* params ) {
  // Calculates a 4vector corresponding to the given
  // parameter values

  if (params == 0) {
    return 0;
  }

  if ( params->GetNcols() != 1 || params->GetNrows() !=_nPar ) {
    cout << "Parameter matrix has wrong size." << endl;
    return 0;
  }
  
  Double_t pT = (*params)(0,0);
  Double_t theta = (*params)(1,0);
  Double_t phi = (*params)(2,0);

  Double_t X = pT*TMath::Cos(phi);
  Double_t Y = pT*TMath::Sin(phi);
  Double_t Z = pT/TMath::Tan(theta);
  Double_t E = TMath::Sqrt( pT*pT/(TMath::Sin(theta)*TMath::Sin(theta)) + _pini.M2() );

  TLorentzVector* vec = new TLorentzVector( X, Y, Z, E );
  return vec;

}

void TFitParticlePtThetaPhi::setIni4Vec(const TLorentzVector* pini) {
  // Set the initial 4vector. Will also set the 
  // inital parameter values 

  TVector3 vec( pini->Vect() );
  setIni4Vec( &vec, pini->M() );

}

void TFitParticlePtThetaPhi::setIni4Vec(const TVector3* p, Double_t M) {
  // Set the initial 4vector. Will also set the 
  // inital parameter values 

  if ( p == 0 ) {

    _u1.SetXYZ(0., 0., 0.);
    _u3.SetXYZ(0., 0., 0.);
    _u2.SetXYZ(0., 0., 0.);
    _pini.SetXYZM(0., 0., 0., M);
    _pcurr = _pini;

    _iniparameters.ResizeTo(_nPar,1);
    _iniparameters(0,0) = 0.;
    _parameters.ResizeTo(_nPar,1);
    _parameters = _iniparameters;

  } else {

    _pini.SetXYZM( p->x(), p->y(), p->z(), M);
    _pcurr = _pini;

    Double_t pT = _pini.Pt();
    Double_t theta = _pini.Theta();
    Double_t phi = _pini.Phi();

    _iniparameters.ResizeTo(_nPar,1);
    _iniparameters(0,0) = pT;
    _iniparameters(1,0) = theta;
    _iniparameters(2,0) = phi;
    _parameters.ResizeTo(_nPar,1);
    _parameters = _iniparameters;

    _u1.SetXYZ( TMath::Cos(phi)*TMath::Sin(theta), TMath::Sin(phi)*TMath::Sin(theta), TMath::Cos(theta) );
    _u2.SetXYZ( TMath::Cos(phi)*TMath::Cos(theta), TMath::Sin(phi)*TMath::Cos(theta), -1.*TMath::Sin(theta) );
    _u3.SetXYZ( -1.*TMath::Sin(phi), TMath::Cos(phi), 0. );
    
    // using the same set of unit vectors as in TFitParticlePThetaPhi
    
  }

}

TMatrixD* TFitParticlePtThetaPhi::getDerivative() {
  // returns derivative dP/dy with P=(p,E) and y=pT, Theta, Phi) 
  // which are the free parameters of the fit. The columns of the matrix contain 
  // (dP/dpT, dP/dTheta, dP/dPhi).

  TMatrixD* DerivativeMatrix = new TMatrixD(4,3);
  (*DerivativeMatrix) *= 0.;

  Double_t pT = _parameters(0,0);
  Double_t theta = _parameters(1,0);
  Double_t phi = _parameters(2,0);
  
  Double_t CosPhi            = TMath::Cos(phi);
  Double_t SinPhi            = TMath::Sin(phi);
  Double_t CosTheta          = TMath::Cos(theta);
  Double_t SinTheta          = TMath::Sin(theta);
  Double_t Sin2Theta         = SinTheta*SinTheta;
  Double_t CoTanTheta        = CosTheta/SinTheta;
  Double_t OneOverESin2Theta = 1./(_pcurr.E()*Sin2Theta);

  //1st column: dP/dpT
  (*DerivativeMatrix)(0,0) = CosPhi;
  (*DerivativeMatrix)(1,0) = SinPhi;
  (*DerivativeMatrix)(2,0) = CoTanTheta;
  (*DerivativeMatrix)(3,0) = pT*OneOverESin2Theta;

  //2nd column: dP/dtheta
  (*DerivativeMatrix)(0,1) = 0.;
  (*DerivativeMatrix)(1,1) = 0.;
  (*DerivativeMatrix)(2,1) = -1.*pT/Sin2Theta;
  (*DerivativeMatrix)(3,1) = -1.*pT*pT*CoTanTheta*OneOverESin2Theta;

   //3rd column: dP/dphi
  (*DerivativeMatrix)(0,2) = -pT*SinPhi;
  (*DerivativeMatrix)(1,2) = pT*CosPhi;
  (*DerivativeMatrix)(2,2) = 0.;
  (*DerivativeMatrix)(3,2) = 0.;

  return DerivativeMatrix;  

}

TMatrixD* TFitParticlePtThetaPhi::transform(const TLorentzVector& vec) {
  // Returns the parameters corresponding to the given 
  // 4vector

  // retrieve parameters
  TMatrixD* tparams = new TMatrixD( _nPar, 1 );
  (*tparams)(0,0) = vec.Pt();
  (*tparams)(1,0) = vec.Theta();
  (*tparams)(2,0) = vec.Phi();

  return tparams;

}

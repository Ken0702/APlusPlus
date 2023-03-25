#ifndef TFITPARTICLEPTTHETAPHI_H
#define TFITPARTICLEPTTHETAPHI_H

#include "TMatrixD.h"
#include "TAbsFitParticle.h"
#include "TLorentzVector.h"
#include "TVector3.h"

class TFitParticlePtThetaPhi: public TAbsFitParticle {

public :

  TFitParticlePtThetaPhi();
  TFitParticlePtThetaPhi( const TFitParticlePtThetaPhi& fitParticle );
  TFitParticlePtThetaPhi(TVector3* p, Double_t M, const TMatrixD* theCovMatrix);
  TFitParticlePtThetaPhi(const TString &name, const TString &title, 
			TVector3* p, Double_t M, const TMatrixD* theCovMatrix);
  virtual ~TFitParticlePtThetaPhi();
  virtual TAbsFitParticle* clone( TString newname = "" ) const;

  // returns derivative dP/dy with P=(p,E) and y=(pT, theta, phi) 
  // which are the free parameters of the fit. The columns of the matrix are
  // (dP/dpT, dP/dtheta, dP/dPhi).
  virtual TMatrixD* getDerivative();
  virtual TMatrixD* transform(const TLorentzVector& vec);
  virtual void setIni4Vec(const TLorentzVector* pini);
  void setIni4Vec(const TVector3* p, Double_t M);
  virtual TLorentzVector* calc4Vec( const TMatrixD* params );

protected :

  void init(TVector3* p, Double_t M, const TMatrixD* theCovMatrix);

  ClassDef(TFitParticlePtThetaPhi, 1)    // Particle parameterized by pT, Theta, Phi. mass fixed.
  
private:

};

#endif

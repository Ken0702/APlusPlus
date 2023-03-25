#ifndef TFITCONSTRAINTETA_H
#define TFITCONSTRAINTETA_H

#include "TAbsFitConstraint.h"

#include <vector>
#include "TH1D.h"

#if ROOT_VERSION_CODE >= 329728
#include "TMatrixDfwd.h"
#else
#include "TMatrixD.h"
#endif

class TAbsFitParticle;

class TFitConstraintEta: public TAbsFitConstraint {

public :

  TFitConstraintEta();
  TFitConstraintEta(TAbsFitParticle* particle, TH1D* EtaHist);
  TFitConstraintEta(const TString &name, const TString &title,
		    TAbsFitParticle* particle, TH1D* EtaHist);

  virtual ~TFitConstraintEta();

  void addParticle( TAbsFitParticle* particle );
  Double_t getEtaPrimitive( Double_t Eta );
  Double_t getGaussPrimitive( Double_t arg );
  virtual TMatrixD* getDerivative( TAbsFitParticle *particle );
  virtual Double_t getInitValue();
  virtual Double_t getCurrentValue();
  virtual TMatrixD* getDerivativeAlpha();

  void setHistogram( TH1D* EtaHist );

  virtual void print();

protected :

  TH1D *_EtaHist;
  TAbsFitParticle *_particle;    // constrained particle
  Double_t _Delta;
  void init();

  ClassDef(TFitConstraintEta, 1)   // Class for Eta distribution constraint

};

#endif

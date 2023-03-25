#ifndef TFITCONSTRAINTMBW_H
#define TFITCONSTRAINTMBW_H

#include "TFitConstraintM.h"

#include <vector>

#if ROOT_VERSION_CODE >= 329728
#include "TMatrixDfwd.h"
#else
#include "TMatrixD.h"
#endif

class TAbsFitParticle;

class TFitConstraintMBW: public TFitConstraintM {

public :

  TFitConstraintMBW();
  TFitConstraintMBW(std::vector<TAbsFitParticle*>* ParList,
		      Double_t Mass = 0, Double_t Width = 0);
  TFitConstraintMBW(const TString &name, const TString &title,
		      std::vector<TAbsFitParticle*>* ParList,
		      Double_t Mass = 0, Double_t Width = 0);

  virtual ~TFitConstraintMBW();

  Double_t getBWPrimitive( Bool_t initial);
  Double_t getGaussPrimitive( Bool_t initial);
  virtual TMatrixD* getDerivative( TAbsFitParticle *particle );
  virtual Double_t getInitValue();
  virtual Double_t getCurrentValue();
  virtual TMatrixD* getDerivativeAlpha();

  void setMassConstraint(Double_t Mass, Double_t Width);

  virtual void print();

protected :

  Double_t _width;

  void init();

  ClassDef(TFitConstraintMBW, 1)   // Class for mass constraint

};

#endif

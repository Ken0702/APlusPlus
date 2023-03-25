#ifndef TFITCONSTRAINTMBW2_H
#define TFITCONSTRAINTMBW2_H

#include "TFitConstraintM.h"

#include <vector>

#if ROOT_VERSION_CODE >= 329728
#include "TMatrixDfwd.h"
#else
#include "TMatrixD.h"
#endif

class TAbsFitParticle;

class TFitConstraintMBW2: public TFitConstraintM {

public :

  TFitConstraintMBW2();
  TFitConstraintMBW2(std::vector<TAbsFitParticle*>* ParList,
		      Double_t Mass = 0, Double_t Width = 0);
  TFitConstraintMBW2(const TString &name, const TString &title,
		      std::vector<TAbsFitParticle*>* ParList,
		      Double_t Mass = 0, Double_t Width = 0);

  virtual ~TFitConstraintMBW2();

  Double_t getMuDependence( Bool_t initial);
  virtual Double_t getInitValue();
  virtual Double_t getCurrentValue();
  virtual TMatrixD* getDerivativeAlpha();

  void setMassConstraint(Double_t Mass, Double_t Width);

  virtual void print();

protected :

  Double_t _width;

  void init();

  ClassDef(TFitConstraintMBW2, 1)   // Class for mass constraint

};

#endif

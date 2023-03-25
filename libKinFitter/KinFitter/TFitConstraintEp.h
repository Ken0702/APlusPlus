#ifndef TFITCONSTRAINTEP_H
#define TFITCONSTRAINTEP_H

#include "TAbsFitConstraint.h"
#include "TAbsFitParticle.h"
#include "TMatrixD.h"
#include <vector>

class TFitConstraintEp: public TAbsFitConstraint {

public :

  enum component {
    pX,
    pY,
    pZ,
    E
  };

  TFitConstraintEp( );

  TFitConstraintEp(  std::vector<TAbsFitParticle*>* particles1,
		     std::vector<TAbsFitParticle*>* particles2,
		     TFitConstraintEp::component thecomponent,
		     Double_t constraint = 0.);
  TFitConstraintEp(  std::vector<TAbsFitParticle*>* particles,
		     TFitConstraintEp::component thecomponent,
		     Double_t constraint = 0.);

  TFitConstraintEp(  const TString &name, const TString &title,
		     std::vector<TAbsFitParticle*>* particles1,
		     std::vector<TAbsFitParticle*>* particles2,
		     TFitConstraintEp::component thecomponent,
		     Double_t constraint = 0.);
  TFitConstraintEp(  const TString &name, const TString &title,
		     std::vector<TAbsFitParticle*>* particles,
		     TFitConstraintEp::component thecomponent,
		     Double_t constraint = 0.);
  virtual ~TFitConstraintEp();

  void addParticle( TAbsFitParticle* particle );
  void addParticle1( TAbsFitParticle* particle );
  void addParticle2( TAbsFitParticle* particle );
  void addParticles( TAbsFitParticle* p1, TAbsFitParticle* p2 = 0, TAbsFitParticle* p3 = 0, TAbsFitParticle* p4 = 0,
		     TAbsFitParticle* p5 = 0, TAbsFitParticle* p6 = 0, TAbsFitParticle* p7 = 0, TAbsFitParticle* p8 = 0,
		     TAbsFitParticle* p9 = 0, TAbsFitParticle* p10 = 0);
  void addParticles1( TAbsFitParticle* p1, TAbsFitParticle* p2 = 0, TAbsFitParticle* p3 = 0, TAbsFitParticle* p4 = 0,
		     TAbsFitParticle* p5 = 0, TAbsFitParticle* p6 = 0, TAbsFitParticle* p7 = 0, TAbsFitParticle* p8 = 0,
		     TAbsFitParticle* p9 = 0, TAbsFitParticle* p10 = 0);
  void addParticles2( TAbsFitParticle* p1, TAbsFitParticle* p2 = 0, TAbsFitParticle* p3 = 0, TAbsFitParticle* p4 = 0,
		     TAbsFitParticle* p5 = 0, TAbsFitParticle* p6 = 0, TAbsFitParticle* p7 = 0, TAbsFitParticle* p8 = 0,
		     TAbsFitParticle* p9 = 0, TAbsFitParticle* p10 = 0);

  // returns derivative df/dP with P=(p,E) and f the constraint f=0.
  // The matrix contains one row (df/dp, df/dE).
  virtual TMatrixD* getDerivative( TAbsFitParticle* particle );
  virtual Double_t getInitValue();
  virtual Double_t getCurrentValue();
  void setConstraint( Double_t constraint ) { _constraint = constraint; }

  Bool_t OnList(std::vector<TAbsFitParticle*>* List, TAbsFitParticle* particle);

  virtual void print();

protected :

  std::vector<TAbsFitParticle*> _particles1;    // Vector containing constrained particles
  std::vector<TAbsFitParticle*> _particles2;    // Vector containing constrained particles
  Double_t _constraint;                   // Value of constraint
  TFitConstraintEp::component _component; // 4vector component to be constrained

private:

  ClassDef(TFitConstraintEp, 1)   // Fit constraint: energy and momentum conservation

};

#endif

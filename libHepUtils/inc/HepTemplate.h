//  
// Author: Felix Thomas <mailto: Felix.Thomas@mailbox.tu-dresden.de>
// Update: $$
// Copyright: 2010 (C) Felix Thomas
//
#ifndef HEP_HepTemplate
#define HEP_HepTemplate
#ifndef ROOT_TNamed
#include <TNamed.h>
#endif
#ifndef ROOT_TAttFill
#include <TAttFill.h>
#endif
#ifndef ROOT_TAttLine
#include <TAttLine.h>
#endif
#ifndef ROOT_TFile
#include <TFile.h>
#endif
#ifndef ROOT_TH1F
#include <TH1F.h>
#endif

class HepTemplate : public TNamed {

 private:

  TAttFill fAttFill;            // Fill attributes (used for stacked drawing)
  TAttLine fAttLine;            // Line attributes (used for overlay drawing)
  
  TH1F*     fInputHistogram;    // Corresponding histogram (used in HepDataMCPlot only)
  TH1F*     fFittedHistogram;   // Histogram that is fitted to reference
  Double_t  fFitFraction;       // Fitted fraction
  Double_t  fFitFractionError;  // Error on the fitted fraction
  Double_t  fLowerBound;        // lower bound of fit parameter
  Double_t  fUpperBound;        // upper bound of fit parameter
  Bool_t    fBoundsSet;         // TRUE if bounds are set, otherwise false
  Double_t  fStartValue;        // Starting value of fit parameter
  Double_t  fStartValueError;   // Starting value of error of fit parameter
  Bool_t    fStartValueSet;     // TRUE if start value is set, otherwise false
  Bool_t    fFixFraction;       // TRUE if fraction should be fixed, otherwise false
  Float_t   fCutEfficiency;     // Cut efficiency assigned to this template
      
    
 public:
  HepTemplate();
  virtual ~HepTemplate();
 
  inline Color_t   GetColor() { return fAttFill.GetFillColor(); }
  inline Style_t   GetLineStyle() { return fAttLine.GetLineStyle(); }
  inline Width_t   GetLineWidth() { return fAttLine.GetLineWidth(); }
  inline Style_t   GetFillStyle() { return fAttFill.GetFillStyle(); }
  inline TAttFill GetAttFill() const { return fAttFill; }
  inline TAttLine GetAttLine() const { return fAttLine; }

  inline TH1F*     GetInputHistogram() const { return fInputHistogram; }
  inline TH1F*     GetFittedHistogram() const { return fFittedHistogram; }
  inline Double_t  GetFitFraction() const { return fFitFraction; }
  inline Double_t  GetFitFractionError() const { return fFitFractionError; }

  inline void SetColor(Color_t color) { fAttFill.SetFillColor(color); }
  inline void SetLineStyle(Style_t lstyle) { fAttLine.SetLineStyle(lstyle); }
  inline void SetLineWidth(Width_t lwidth) { fAttLine.SetLineWidth(lwidth); }
  inline void SetFillStyle(Style_t fstyle) { fAttFill.SetFillStyle(fstyle); }

  
  inline void SetInputHistogram(TH1F *h) { fInputHistogram = h; }
  inline void SetFittedHistogram(TH1F *h) { fFittedHistogram = h; }
  inline void SetFitFraction(Double_t fraction) { fFitFraction = fraction; }
  inline void SetFitFractionError(Double_t error) { fFitFractionError = error; }

  inline void SetBounds(Double_t Lower, Double_t Upper) { 
                  fLowerBound = Lower; 
                  fUpperBound = Upper; 
                  fBoundsSet = kTRUE; 
              }
  inline void SetStartValue(Double_t frac, Double_t frac_err) {
      fStartValue = frac;
      fStartValueError = frac_err;
      fStartValueSet = kTRUE;
  }

  inline void FixFraction(Bool_t fix) { fFixFraction = fix; }
  inline void SetCutEfficiency(Float_t eff) { fCutEfficiency = eff; }
  
  inline void     UnsetBounds() { fBoundsSet = kFALSE; }
  inline Bool_t   IsBound() { return fBoundsSet; }
  inline Double_t GetUpperBound() { return fUpperBound; }
  inline Double_t GetLowerBound() { return fLowerBound; }
  inline Bool_t   HasStartValue() { return fStartValueSet; }
  inline Double_t GetStartValue() { return fStartValue; }
  inline Double_t GetStartValueError() { return fStartValueError; }
  inline Bool_t   HasFixFraction() { return fFixFraction; }
  inline Float_t  GetCutEfficiency() { return fCutEfficiency; }
  
  ClassDef(HepTemplate,2) // Template inside HepTemplateFitter
    };
#endif

//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactorySample
#define ATLAS_AtlHistFactorySample
#ifndef ROOT_TNamed
#include <TNamed.h>
#endif
#ifndef ROOT_TStyle
#include <TStyle.h>
#endif

class AtlHistFactorySystematic;
class TList;
class TH1F;
class TFile;

class AtlHistFactorySample : public TNamed {

protected:
    TString  fNormFactor;      // Name of the norm factor parameter

    Double_t fNormUncertainty; // Relative normalization uncertainty
    Double_t fSignalLow;       // Lower bound of signal parameter
    Double_t fSignalHigh;      // Upper bound of signal parameter

    Bool_t fIsSignal;          // Is signal sample?
    Bool_t fIsData;            // Is this a data sample? (e.g. data or QCD)
    Bool_t fUseLogNormal;      // Use log normal constraint for normalization parameter
    Bool_t fUseStatError;      // Use statistical error (default: true)

    TList *fListOfSystematics; // List of systematics for this sample

    TH1F *fHist_ShapeTest_pValue;   // Histogram for Shape test results (Chi2)
    TH1F *fHist_ShapeTest_KStest;   // Histogram for Shape test results (KS)

    Color_t fLineColor;
    Style_t fLineStyle;
    Color_t fFillColor;
    Style_t fFillStyle;
    
public:
    AtlHistFactorySample(const char* name, const char* title,
			 const char* normfactor,
			 Double_t norm_unc, Bool_t isSignal = kFALSE);
    virtual ~AtlHistFactorySample();

    void AddSystematic(AtlHistFactorySystematic *syst);
    void AddShapeSystematic(const char *name, TList *systs);
    void AddSystematics(TList *list);
    void BookHistograms(const char* channel);
    void FillHistograms(const char* syst,
			Double_t pval, Double_t kstest);
    void WriteHistograms(TFile *fout);
    void SetSignalRange(Double_t low, Double_t high);

    inline TList* GetListOfSystematics() const { return fListOfSystematics; }
    inline const char* GetNormFactorName() const { return fNormFactor.Data(); }
    inline Double_t GetNormUncertainty() const { return fNormUncertainty; }
    inline Double_t GetSignalHigh() const { return fSignalHigh; }
    inline Double_t GetSignalLow() const { return fSignalLow; }

    inline Color_t GetLineColor() const { return fLineColor; }
    inline Style_t GetLineStyle() const { return fLineStyle; }
    inline Color_t GetFillColor() const { return fFillColor; }
    inline Style_t GetFillStyle() const { return fFillStyle; }
    
    inline Bool_t IsSignal() const { return fIsSignal; }
    inline Bool_t IsData() const { return fIsData; }
    inline Bool_t UseLogNormal() const { return fUseLogNormal; }
    inline Bool_t UseStatError() const { return fUseStatError; }

    inline void SetIsData(Bool_t flag = kTRUE) { fIsData = flag; }
    inline void SetUseLogNormal(Bool_t flag = kTRUE) { fUseLogNormal = flag; }
    inline void SetUseStatError(Bool_t flag = kTRUE) { fUseStatError = flag; }
    inline void SetDrawOptions(Color_t fill_color, Style_t fill_style,
			       Color_t line_color, Style_t line_style) {
	fFillColor = fill_color; fFillStyle = fill_style;
	fLineColor = line_color; fLineStyle = line_style;
    }

    ClassDef(AtlHistFactorySample,0) // DOCUMENT ME
};
#endif


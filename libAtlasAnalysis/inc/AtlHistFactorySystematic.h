//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactorySystematic
#define ATLAS_AtlHistFactorySystematic
#ifndef ROOT_TNamed
#include <TNamed.h>
#endif
#ifndef ROOT_TH1
#include <TH1.h>
#endif
#ifndef ROOT_TH1F
#include <TH1F.h>
#endif
#ifndef ROOT_TGraph
#include <TGraph.h>
#endif
#ifndef ROOT_TString
#include <TString.h>
#endif
#ifndef ROOT_TFile
#include <TFile.h>
#endif

class AtlHistFactorySystematic : public TNamed {
    
protected:
    TH1 *fHistUp;   // Histogram for   up-variation (1-sigma)
    TH1 *fHistDown; // Histogram for down-variation (1-sigma)
    TH1 *fHistNom;  // Nominal histogram
    TH1 *fHistShapeResidual;    // Chi2 normalized residuals
    TGraph *fGraphShapeQQPlot;  // Q-Q plots of normalized residuals

    TFile *fTemplateFile; // File holding all templates
    
    TString *fDiscriminant;  // Full path to the discriminant histogram

    Bool_t fIsNominal;     // Is this nominal?
    Bool_t fUseShape;      // Use shape information?
    Bool_t fUseFullStats;  // Use full staistics for shape comparison?

    Int_t fNPseudoExp;    // Number of pseudo exp. for shape tests
    TH1F *fHistChi2;      // Chi2 Distribution for shape tests
    Bool_t fUsePseudoExp; // Use pseudo exp. for shape tests?

    Double_t fPValue_up;   // p-value from Chi2 test (up-syst.)
    Double_t fPValue_down; // p-value from Chi2 test (down-syst.)
    
public:
    AtlHistFactorySystematic();
    AtlHistFactorySystematic(const char* systname,
			     const char* systtitle,
			     Bool_t useShape);
    virtual ~AtlHistFactorySystematic();
    
    virtual void Initialize(const char* BaseDir,
			    const char* scheme) = 0;
    virtual void Clear(Option_t *option = "") = 0;
    virtual void SaveTemplates(TFile *fout);
    virtual void SaveShapeCtrlPlots(TFile *fout,
				    const char* process);
    virtual void ExportShapePlots(const char* process,
				  const char* dir);
    
    void ChangeProcess(const char* process);
    void PerformShapeTest(Double_t& pval_up,
			  Double_t& kstest_min);
    
    void GetRateUnc(Double_t& rate_up, Double_t& rate_down);
    TH1F* GetChi2Distribution();
    
    void SetDiscriminant(const char* discriminant);

    inline TH1* GetHistUp() const { return fHistUp; }
    inline TH1* GetHistDown() const { return fHistDown; }
    inline TH1* GetHistNominal() const { return fHistNom; }

    inline Bool_t GetUseShape() const { return fUseShape; }
    inline Bool_t GetUseFullStats() const { return fUseFullStats; }
    
    inline Bool_t IsNominal() const { return fIsNominal; }

    inline void SetChi2Distribution(TH1F *h_chi2) { fHistChi2 = h_chi2; }
    inline void SetHistNominal(TH1 *h) { fHistNom = h; }
    inline void SetHistUp(TH1 *h) { fHistUp = h; }
    inline void SetHistDown(TH1 *h) { fHistDown = h; }

    inline void SetTemplateFile(TFile *f) { fTemplateFile = f; }

    inline void SetUseFullStats(Bool_t flag = kTRUE) { fUseFullStats = flag; }
    inline void SetUsePseudoExp(Bool_t flag = kTRUE) { fUsePseudoExp = flag; }
    inline void SetUseShape(Bool_t useShape = kTRUE) { fUseShape = useShape; }

protected:
    virtual void GetHistsFromFile(const char* process) = 0;
    virtual void ComputeUpDownVariation(const char* process) = 0;
    virtual void ComputeChi2Distribution();
    
    ClassDef(AtlHistFactorySystematic, 1) // Histfactory Systematic
};
#endif


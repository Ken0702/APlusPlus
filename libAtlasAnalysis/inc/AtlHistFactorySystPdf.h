//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactorySystPdf
#define ATLAS_AtlHistFactorySystPdf
#ifndef ATLAS_AtlHistFactorySystematic
#include <AtlHistFactorySystematic.h>
#endif
#ifndef ATLAS_AtlTopLevelAnalysis
#include <AtlTopLevelAnalysis.h>
#endif

class AtlHistFactorySystPdf : public AtlHistFactorySystematic {

protected:
    AtlTopLevelAnalysis::ESystematic fSystUp;
    AtlTopLevelAnalysis::ESystematic fSystDown;
    AtlTopLevelAnalysis::ESystematic fNominal; // Nominal (needed for symmetrization)

    TFile *fPlotterFileUp;   // MCPlotter file
    TFile *fPlotterFileDown; // MCPlotter file
    TFile *fPlotterFileNom;  // MCPlotter file for nominal

    TList *fHistsUp;   // List of MCPlotter templates (up var)
    TList *fHistsDown; // List of MCPlotter templates (down var)
    TList *fHistsNom;  // List of MCPlotter templates for nominal
    
public:
    AtlHistFactorySystPdf();
    AtlHistFactorySystPdf(const char* systname,
			  const char* systtitle,
			  AtlTopLevelAnalysis::ESystematic SystUp,
			  AtlTopLevelAnalysis::ESystematic SystDown,
			  Bool_t useShape = kFALSE);
    virtual ~AtlHistFactorySystPdf();
    virtual void Initialize(const char* BaseDir,
			    const char* scheme = "plots_datamc");
    virtual void Clear(Option_t *option = "");
    
protected:
    virtual void GetHistsFromFile(const char* process);
    virtual void ComputeUpDownVariation(const char* process);
    
    ClassDef(AtlHistFactorySystPdf, 1) // Pdf Systematic
};
#endif


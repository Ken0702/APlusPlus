//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactorySystPair
#define ATLAS_AtlHistFactorySystPair
#ifndef ATLAS_AtlHistFactorySystematic
#include <AtlHistFactorySystematic.h>
#endif
#ifndef ATLAS_AtlTopLevelAnalysis
#include <AtlTopLevelAnalysis.h>
#endif

class AtlHistFactorySystPair : public AtlHistFactorySystematic {

protected:
    AtlTopLevelAnalysis::ESystematic fSystUp;
    AtlTopLevelAnalysis::ESystematic fSystDown;

    TFile *fPlotterFileUp;   // MCPlotter file
    TFile *fPlotterFileDown; // MCPlotter file

    TList *fHistsUp;   // List of MCPlotter templates (up var)
    TList *fHistsDown; // List of MCPlotter templates (down var)
    
public:
    AtlHistFactorySystPair();
    AtlHistFactorySystPair(const char* systname,
			   const char* systtitle,
			   AtlTopLevelAnalysis::ESystematic SystUp,
    			   AtlTopLevelAnalysis::ESystematic SystDown,
			   Bool_t useShape = kFALSE);
    virtual ~AtlHistFactorySystPair();
    virtual void Initialize(const char* BaseDir,
			    const char* scheme = "plots_datamc");
    virtual void Clear(Option_t *option = "");

protected:
    virtual void GetHistsFromFile(const char* process);
    virtual void ComputeUpDownVariation(const char* process);
    
    ClassDef(AtlHistFactorySystPair, 1) // A pair of systematic variations (up & down)
};
#endif


//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactorySystEnvelope
#define ATLAS_AtlHistFactorySystEnvelope
#ifndef ATLAS_AtlHistFactorySystematic
#include <AtlHistFactorySystematic.h>
#endif
#ifndef ATLAS_AtlTopLevelAnalysis
#include <AtlTopLevelAnalysis.h>
#endif

class AtlHistFactorySystEnvelope : public AtlHistFactorySystematic {

protected:
    TFile *fPlotterFileSyst1; // MCPlotter file for systematic
    TFile *fPlotterFileSyst2; // MCPlotter file for systematic
    TFile *fPlotterFileSyst3; // MCPlotter file for systematic
    TFile *fPlotterFileSyst4; // MCPlotter file for systematic
    TFile *fPlotterFileSyst5; // MCPlotter file for systematic
    TFile *fPlotterFileSyst6; // MCPlotter file for systematic
    // TFile *fPlotterFileNom;   // MCPlotter file for nominal
    AtlTopLevelAnalysis::ESystematic fSyst1; // Systematic
    AtlTopLevelAnalysis::ESystematic fSyst2; // Systematic
    AtlTopLevelAnalysis::ESystematic fSyst3; // Systematic
    AtlTopLevelAnalysis::ESystematic fSyst4; // Systematic
    AtlTopLevelAnalysis::ESystematic fSyst5; // Systematic
    AtlTopLevelAnalysis::ESystematic fSyst6; // Systematic
    // AtlTopLevelAnalysis::ESystematic fNominal; // Nominal (needed for envelope compuation)
    TList *fHistsSyst1; // List of MCPlotter templates for systematic
    TList *fHistsSyst2; // List of MCPlotter templates for systematic
    TList *fHistsSyst3; // List of MCPlotter templates for systematic
    TList *fHistsSyst4; // List of MCPlotter templates for systematic
    TList *fHistsSyst5; // List of MCPlotter templates for systematic
    TList *fHistsSyst6; // List of MCPlotter templates for systematic
    // TList *fHistsNom;   // List of MCPlotter templates for nominal
    
public:
    AtlHistFactorySystEnvelope();
    AtlHistFactorySystEnvelope(const char* systname,
			       const char* systtitle,
			       AtlTopLevelAnalysis::ESystematic Syst1,
			       AtlTopLevelAnalysis::ESystematic Syst2,
			       AtlTopLevelAnalysis::ESystematic Syst3 = AtlTopLevelAnalysis::kUndefined,
			       AtlTopLevelAnalysis::ESystematic Syst4 = AtlTopLevelAnalysis::kUndefined,
			       AtlTopLevelAnalysis::ESystematic Syst5 = AtlTopLevelAnalysis::kUndefined,
			       AtlTopLevelAnalysis::ESystematic Syst6 = AtlTopLevelAnalysis::kUndefined,
			       Bool_t useShape = kFALSE);
    virtual ~AtlHistFactorySystEnvelope();

    virtual void Initialize(const char* BaseDir,
			    const char* scheme = "plots_datamc");
    virtual void Clear(Option_t *option = "");

protected:
    virtual void GetHistsFromFile(const char* process);
    virtual void ComputeUpDownVariation(const char* process);
    
    ClassDef(AtlHistFactorySystEnvelope, 1) // Systematic using max. & min. variations as systematic (envelope)
};
#endif


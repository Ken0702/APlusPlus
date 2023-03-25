//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactorySystOneSided
#define ATLAS_AtlHistFactorySystOneSided
#ifndef ATLAS_AtlHistFactorySystematic
#include <AtlHistFactorySystematic.h>
#endif
#ifndef ATLAS_AtlHistFactorySystematic
#include <AtlHistFactorySystematic.h>
#endif
#ifndef ATLAS_AtlTopLevelAnalysis
#include <AtlTopLevelAnalysis.h>
#endif

class AtlHistFactorySystOneSided : public AtlHistFactorySystematic {

public:
    enum ESymmetryMode {
	kFullDiff, // up/down = nom +- (syst-nom)
	kHalfDiff  // up/down = nom +- (syst-nom)/2.
    };
    
private:
    ESymmetryMode fSymMode;  // Mode for symmetrization procedure
    TFile *fPlotterFileSyst; // MCPlotter file for systematic
    TFile *fPlotterFileSystNom; // MCPlotter file for systematic (nominal)
    TFile *fPlotterFileNom;  // MCPlotter file for nominal
    AtlTopLevelAnalysis::ESystematic fSyst; // Systematic
    AtlTopLevelAnalysis::ESystematic fNominal; // Nominal (needed for symmetrization)
    AtlTopLevelAnalysis::ESystematic fSystNom; // Use different nominal vor symmetrization
    TList *fHistsSyst; // List of MCPlotter templates for systematic
    TList *fHistsSystNom; // List of MCPlotter templates for systematic (nominal)
    TList *fHistsNom;  // List of MCPlotter templates for nominal
    
public:
    AtlHistFactorySystOneSided();
    AtlHistFactorySystOneSided(const char* systname,
			       const char* systtitle,
			       AtlTopLevelAnalysis::ESystematic Syst,
			       ESymmetryMode Mode,
			       Bool_t useShape = kFALSE);
    virtual ~AtlHistFactorySystOneSided();

    virtual void Initialize(const char* BaseDir,
			    const char* scheme = "plots_datamc");
    virtual void Clear(Option_t *option = "");
    inline void SetSystNominal(AtlTopLevelAnalysis::ESystematic systnom) {
	fSystNom = systnom;
    }
	
protected:
    virtual void GetHistsFromFile(const char* process);
    virtual void ComputeUpDownVariation(const char* process);
    
    ClassDef(AtlHistFactorySystOneSided, 1) // DOCUMENT ME
};
#endif


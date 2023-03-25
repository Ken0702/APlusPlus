//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactorySystModelPair
#define ATLAS_AtlHistFactorySystModelPair
#ifndef ATLAS_AtlHistFactorySystPair
#include <AtlHistFactorySystPair.h>
#endif
#ifndef ATLAS_AtlTopLevelAnalysis
#include <AtlTopLevelAnalysis.h>
#endif

class AtlHistFactorySystModelPair : public AtlHistFactorySystPair {

protected:
    TFile *fPlotterFileNom;  // MCPlotter file for nominal
    TFile *fPlotterFileSystNom; // MCPlotter file for systematic (nominal)
    AtlTopLevelAnalysis::ESystematic fNominal; // Nominal (needed for symmetrization)
    AtlTopLevelAnalysis::ESystematic fSystNom; // Nominal for difference to models
    TList *fHistsNom;  // List of MCPlotter templates for nominal
    TList *fHistsSystNom; // List of MCPlotter templates for systematic (nominal)

public:
    AtlHistFactorySystModelPair();
    AtlHistFactorySystModelPair(const char* systname,
				const char* systtitle,
				AtlTopLevelAnalysis::ESystematic SystUp,
				AtlTopLevelAnalysis::ESystematic SystDown,
				Bool_t useShape = kFALSE);
    virtual ~AtlHistFactorySystModelPair();

    virtual void Initialize(const char* BaseDir,
			    const char* scheme = "plots_datamc");
    virtual void Clear(Option_t *option = "");
    inline void SetSystNominal(AtlTopLevelAnalysis::ESystematic systnom) {
	fSystNom = systnom;
    }

protected:
    virtual void GetHistsFromFile(const char* process);
    virtual void ComputeUpDownVariation(const char* process);

    ClassDef(AtlHistFactorySystModelPair, 1) // Systematic for model comparisons
};
#endif


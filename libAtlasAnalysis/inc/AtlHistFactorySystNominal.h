//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactorySystNominal
#define ATLAS_AtlHistFactorySystNominal
#ifndef ROOT_AtlHistFactorySystematic
#include <AtlHistFactorySystematic.h>
#endif
#ifndef ATLAS_AtlTopLevelAnalysis
#include <AtlTopLevelAnalysis.h>
#endif

class AtlHistFactorySystNominal : public AtlHistFactorySystematic {

private:
    TFile *fPlotterFileNom;  // MCPlotter file for nominal
    AtlTopLevelAnalysis::ESystematic fNominal; // Nominal
    TList *fHistsNom;  // List of MCPlotter templates for nominal

public:
    AtlHistFactorySystNominal(const char* systname = "Nominal",
			      const char* systtitle = "Nominal");
    virtual ~AtlHistFactorySystNominal();

    virtual void Initialize(const char* BaseDir,
			    const char* scheme = "plots_datamc");
    virtual void Clear(Option_t *option = "");
    virtual void SaveTemplates(TFile *fout);

protected:
    virtual void GetHistsFromFile(const char* process);
    virtual void ComputeUpDownVariation(const char* process);
    
    ClassDef(AtlHistFactorySystNominal, 1) // DOCUMENT ME
};
#endif


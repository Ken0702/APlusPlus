//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlWjetsScaleFactorTool
#define ATLAS_AtlWjetsScaleFactorTool
#ifndef ATLAS_AtlAnalysisTool
#include <AtlAnalysisTool.h>
#endif

class TH1F;
class TDirectory;
class AtlSelector;

class AtlWjetsScaleFactorTool : public AtlAnalysisTool {

  private:
    static const Double_t fgWjetsSF_el[41];
    static const Double_t fgWjetsSF_mu[41];

    TList *fLeptons;
    
  public:
    AtlWjetsScaleFactorTool(const char* name, const char* title);
    virtual ~AtlWjetsScaleFactorTool();
    virtual void Clear(Option_t *option = "");
    virtual void SetBranchStatus();
    virtual void BookHistograms();
    virtual void FillHistograms();
    virtual void Print() const;
    virtual Bool_t AnalyzeEvent();
    virtual void Terminate();

    inline void SetLeptons(TList *leptons) {
	//
	// Set signal leptons for which the scale factor is obtained
	// (must be one lepton, either electron or muon)
	//
	fLeptons = leptons;
    }

    ClassDef(AtlWjetsScaleFactorTool,0) // W+jets Scale Factor Tool
};
#endif


//
//    Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
//    Update: $Id$
//    Copyright: 2013 (C) Soeren Stamm
//

#ifndef ATLAS_AtlQCDMMTool
#define ATLAS_AtlQCDMMTool

#include <AtlAnalysisTool.h>  // for AtlAnalysisTool
#include <Rtypes.h>           // for ClassDefOverride
#include <RtypesCore.h>       // for Bool_t, Float_t
#include <TString.h>          // for TString
class AtlObjectsToolD3PDSgTop;
class FakesWeights;
class TBuffer;
class TClass;
class TH1F;
class TMemberInspector;

class AtlQCDMMTool : public AtlAnalysisTool {

  private:
    Float_t fMMWeight;                // Matrix-method weight
    TH1F*   fHistMMWeight_nominal;    // Histogram of matrix-method weights
    TH1F*   fHistMMWeight_e_nominal;  // same, only for electrons
    TH1F*   fHistMMWeight_mu_nominal; // same, only for muons
    TH1F*   fHistMMEff_fake;          // fake efficiencies from TopFakes
    TH1F*   fHistMMEff_e_fake;        // fake efficiencies from TopFakes
    TH1F*   fHistMMEff_mu_fake;       // fake efficiencies from TopFakes
    TH1F*   fHistMMEff_real;          // real efficiencies from TopFakes
    TH1F*   fHistMMEff_e_real;        // real efficiencies from TopFakes
    TH1F*   fHistMMEff_mu_real;       // real efficiencies from TopFakes
    AtlObjectsToolD3PDSgTop *fObjTool;// Pointer to objects definition
    FakesWeights *fFakeWeightEJets;   // TopFakes object to calculate weights (e+jets)
    FakesWeights *fFakeWeightMuJets;  // TopFakes object to calculate weights (mu+jets)
    TString fYear;                    // Year used in eff. path and param. strings
    
  public:
    AtlQCDMMTool(const char* name, const char* title);
    virtual ~AtlQCDMMTool();

    TString fEfficiencyPath2015;      // Path to the Fake efficiency files (2015)
    TString fEfficiencyPath2016;      // Path to the Fake efficiency files (2016)
    TString fEfficiencyPath2017;      // Path to the Fake efficiency files (2017)
    TString fParamString_fake2015;    // Parametrization for fake eff (2015)
    TString fParamString_fake2016;    // Parametrization for fake eff (2016)
    TString fParamString_fake2017;    // Parametrization for fake eff (2017)
    TString fParamString_real2015;    // Parametrization for real eff (2015)
    TString fParamString_real2016;    // Parametrization for real eff (2016)
    TString fParamString_real2017;    // Parametrization for real eff (2017)

    // other interface for years
    TString fEfficiencyPath;
    TString fParamString_real;
    TString fParamString_fake;

    virtual Bool_t Notify() override;
    
  protected:
    virtual Bool_t AnalyzeEvent() override;
    virtual void   BookHistograms() override;
    virtual void   FillHistograms() override;
    virtual void   FillHistogramsNoEvtSel() override;
    virtual void   SetBranchStatus() override;
    virtual void   SetCutDefaults() override;
    virtual void   Print() const override;
    virtual void   Terminate() override;

  private:
    void InitEvent();
   
    ClassDefOverride(AtlQCDMMTool,0)  // Atlas QCD Estimation Tool
};
#endif

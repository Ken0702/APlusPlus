//
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2011 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlObjRecoScaleFactorTool
#define ATLAS_AtlObjRecoScaleFactorTool
#ifndef ATLAS_AtlAnalysisTool
#include <AtlAnalysisTool.h>
#endif
#ifndef ATLAS_AtlObjectsToolD3PDSgTop
#include <AtlObjectsToolD3PDSgTop.h>
#endif
#ifndef ATLAS_AtlTopLevelAnalysis
#include <AtlTopLevelAnalysis.h>
#endif
#ifndef ANALYSISCALIBRATIONDATAINTERFACEROOT_H
#include "CalibrationDataInterface/CalibrationDataInterfaceROOT.h"
#endif
#include <string>

class TH1F;
class TDirectory;
class AtlSelector;

class AtlObjRecoScaleFactorTool : public AtlAnalysisTool {

  public:
    enum EOperation { kNoSF = 0,
		      kLeptonSF  = BIT(0),
		      kJetBtagSF = BIT(1),
		      kJetVertexSF = BIT(2),
		      kAllSF = ( kLeptonSF | kJetBtagSF | kJetVertexSF )
    };

    enum EBTagWP { kUndefined = 0,
		   kMV1_60,
		   kMV1_70,
		   kMV1_80,
		   kMV1_85,
		   kMV1c_50,
		   kMV1c_80,
		   kMV2c20_77,
		   kMV2c20_85,
		   kMV2c10_60,
		   kMV2c10_70,
		   kMV2c10_77,
		   kMV2c10_85,
		   kDL1r_85,
		   kDL1r_77,
		   kDL1r_70,
		   kDL1r_60,
    };

  public:
    UInt_t                           fOperationMode;  // Operation mode (default = all SFs)
    AtlTopLevelAnalysis::ESystematic fSystematicMode; // Systematic mode (default = nominal)
    Bool_t      fUseCDI;        // Switch for using CDI
    EBTagWP     fBTagWP;        // BTagging WP
    const char* fXsecFile;      // Xsection file: contains shower info for MC/MC SF
    int         fNjetSystMode;                   // whether Njet systematic is applied (1) or not (-1)
    long        fNjetSystNumber;                 // Njet systematic: number of jets
    bool        fNjetSystRestTerm;               // Njet systematic: use as last term
    double      fNjetSystDelta;                  // Njet systematic: variation per jet bin

  private:
    TList                   *fLeptons;           // Signal leptons
    TList                   *fJets;              // List of jets (after acceptance cuts)
    AtlBTag::ETagger         fBTagger;           // B-tagger used
    Float_t                  fBTagWeight_min;    // min B-tag weight cut
    std::string              workingPoint;       // Working point of b-tagger
    AtlObjectsToolD3PDSgTop *fObjDefinitionTool; // Pointer to objects definition (b-tag info)
    unsigned int             CDIindex_SF[4];     // Array for scale factors (b-tagging, CDI)
    unsigned int             CDIindex_Eff[4];    // Array for eff scale factors (b-tagging, CDI)
    int                      tauExtrapFromCharm; // CDI parameter
    Analysis::CalibrationDataInterfaceROOT *CDITool; // Interface for B-tagging SF (CDI)
    Analysis::CalibrationDataVariables CDIVars;      // Container for B-tagging variables (CDI)
    Analysis::Uncertainty CDIUncertainty;            // Container for B-Tagging uncertainty result (CDI)
    unsigned int mapIndex;           // Map for MC/MC eff. scale factors
    Bool_t fInitCDI; // flag for initializing CDI

  public:
    AtlObjRecoScaleFactorTool(const char* name, const char* title);
    virtual ~AtlObjRecoScaleFactorTool();
    virtual void Clear(Option_t *option = "");
    virtual void SetBranchStatus();
    virtual void BookHistograms();
    virtual void FillHistograms();
    virtual void Print() const;
    virtual Bool_t AnalyzeEvent();
    virtual void Terminate();
    TString GetOperationName() const;
    static const char* GetBTagWPName(EBTagWP wp);
    inline void SetLeptons(TList *leptons) {
	//
	// Set signal leptons for which the scale factor is obtained
	// (must be one lepton, either electron or muon)
	//
	fLeptons = leptons;
    }
    inline void SetJets(TList *jets) {
	//
	// Set list of all jets (after all acceptance cuts) for which
        // scale factors are obtained.
	//
	fJets = jets;
    }
    inline void SetObjDefinitionTool(AtlObjectsToolD3PDSgTop *tool) {
	//
	// Set object defintion tool for inter-tool communication
	//
	fObjDefinitionTool = tool;
    }

    Double_t GetLeptonSF() const;
    Double_t GetElectronSF() const;
    Double_t GetMuonSF() const;
    Double_t GetBtagSF();
    Double_t GetJetVertexSF() const;
    Double_t GetForwardJVT() const;
    Double_t GetLeptonTriggerSF() const;

  private:
    Double_t GetBtagSF_DL1r_85(AtlJet* jet) const;
    Double_t GetBtagSF_MV1_60(AtlJet* jet) const;
    Double_t GetBtagSF_MV1_70(AtlJet* jet) const;
    Double_t GetBtagSF_MV1_80(AtlJet* jet) const;
    Double_t GetBtagSF_MV1c_50(AtlJet* jet) const;
    Double_t ComputeBtagSF(AtlJet* jet);

    ClassDef(AtlObjRecoScaleFactorTool,0) // Object reconstruction scale factor tool
};
#endif

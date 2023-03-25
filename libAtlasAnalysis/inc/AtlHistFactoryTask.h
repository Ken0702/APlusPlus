//  
// Author: Soeren Stamm <mailto: stamm@physik.hu-berlin.de>
// Update: $Id$
// Copyright: 2015 (C) Soeren Stamm
//
#ifndef ATLAS_AtlHistFactoryTask
#define ATLAS_AtlHistFactoryTask
#ifndef ATLAS_AtlTask
#include <AtlTask.h>
#endif
#include "RooStats/HistFactory/Measurement.h"
#include "RooStats/HistFactory/MakeModelAndMeasurementsFast.h"
#include <RooStats/ModelConfig.h>
#include <RooAbsData.h>

class AtlHistFactoryMeasurement;
class TList;
class TGraph;

class AtlHistFactoryTask : public AtlTask {
    
public:
    enum ERunMode {
	kUnknown,
	kCreateFitTemplates,
 	kCreateShapeTemplates,
	kPerformShapeTests,
 	kCreateWorkspace,
	kFitModel,
	kRateTable,	
	kHypoTestLimit };
    
protected:
    ERunMode fRunMode;
    AtlHistFactoryMeasurement *fMeasurement;
    TString *fHistFactoryDir;
    TString *fTemplateDir;
    Int_t fNPullsPerPad;       // Max. number of pulls per pad
    Double_t fPullScaleFactor; // Scale factor between pull x-axis and impact on poi x-axis
    TString *fScheme; // Name of plotting scheme
    
public:
    AtlHistFactoryTask(const char* name, const char* title);
    virtual ~AtlHistFactoryTask();

    virtual void Initialize();
    void SetRunMode(ERunMode mode); // *TOOGLE*

    virtual void ExecInteractiveJob(Option_t *option);
    virtual void ExecGridJob(Option_t *option);
    virtual Bool_t ExecBatchJob(Option_t *option);
    virtual void ExecNAFBatchJob(Option_t *option);
    virtual void CreateRunScript(Option_t *option = "");
    virtual void CreateNAFBatchRunScript();
    virtual void CreateGridRunScript();
    inline void SetMeasurement(AtlHistFactoryMeasurement *meas)
	{ fMeasurement = meas; }
    
protected:
    void CreateTemplates();
    void CreateWorkspace();
    void ExportRateUncertainties();
    void ExportPullPlots(const char* outfile,
			 TH1* h_pulls,
			 TH1* h_pulls_err_up,
			 TH1* h_pulls_err_down,
			 TH1* h_prefit_impact_up,
			 TH1* h_prefit_impact_down,
			 TH1* h_postfit_impact_up,
			 TH1* h_postfit_impact_down);
    void FitModel();
    void RunLimit();

    void GetTemplatesFileName(const char* channel,
			      TString &filename);
    void GetShapeTemplatesFileName(const char* channel,
				   TString &filename);
    void GetImpactOnPOI(RooWorkspace *wSpace, const char* data_name,
			const char* snapshot, RooRealVar *par,
			const char* poi_name, Double_t variation,
			TH1F *hresult, const char* logdir);
    TList* MergeListOfSystematics(TList *ch_systs,
				  TList *sample_systs);
    void PerformShapeTests();
    void RunFit(const char* ws_filename,
		const char* ws_name,
		const char* data_name);
    void CreatePullPlots(const char* ws_filename,
			 const char* ws_name,
			 const char* data_name);
    Bool_t IsNormParameter(const char* name);

    Double_t GetNormUncertainty(const char* name);
    Double_t GetGammaUncertainty(const char* name,
				 RooWorkspace *wSpace);
    TString* GetSystematicName(const char* par_name);
    TString* GetDirectoryName(const char* name);
    
    ClassDef(AtlHistFactoryTask, 0) // DOCUMENT ME
};
#endif


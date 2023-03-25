//____________________________________________________________________
//
// Top-level task folder for A++ analyses
// 
//
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2013 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlTopLevelAnalysis
#include <AtlTopLevelAnalysis.h>
#endif
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <TBits.h>
#include <TBrowser.h>
#include <TClass.h>
#include <TMath.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TROOT.h>
#include <TRegexp.h>
#include <TString.h>
#include <TSystem.h>
#include <AtlDataMCPlotterTask.h>
#include <AtlHistFactoryBreakdownTask.h>
#include <AtlHistFactoryPlotterTask.h>
#include <AtlHistFactoryTask.h>
#include <AtlHistFactoryYieldTableTask.h>
#include <AtlMemTkAnalysisTask.h>
#include <HepNtuplePlotCmd.h>

using namespace std;

const char* AtlTopLevelAnalysis::fgSystematicNames[AtlTopLevelAnalysis::fgNumSystematics] = {
    //  nominal - total syst = 1
    "nominal",
    // jet related - total syst = 7
    "jeff", "jer", "jes_down", "jes_up", "jvf_down", "jvf_up",
    // el related - total syst = 11
    "eer_down", "eer_up", "ees_down", "ees_up",
    // mu related - total syst = 15
    "muid_res", "mums_res", "musc_down", "musc_up",
    // met related - total syst = 19
    "res_soft_down", "res_soft_up", "sc_soft_down", "sc_soft_up",
    // JES components - total syst = 71
    "BJesUnc_down", "BJesUnc_up",
    "EtaIntercalibrationModel_down", "EtaIntercalibrationModel_up",
    "EtaIntercalibrationTotalStat_down", "EtaIntercalibrationTotalStat_up",
    "JesEffectiveDet1_down", "JesEffectiveDet1_up",
    "JesEffectiveDet2_down", "JesEffectiveDet2_up",
    "JesEffectiveDet3_down", "JesEffectiveDet3_up",
    "JesEffectiveMix1_down", "JesEffectiveMix1_up",
    "JesEffectiveMix2_down", "JesEffectiveMix2_up",
    "JesEffectiveMix3_down", "JesEffectiveMix3_up",
    "JesEffectiveMix4_down", "JesEffectiveMix4_up",
    "JesEffectiveModel1_down", "JesEffectiveModel1_up",
    "JesEffectiveModel2_down", "JesEffectiveModel2_up",
    "JesEffectiveModel3_down", "JesEffectiveModel3_up",
    "JesEffectiveModel4_down", "JesEffectiveModel4_up",
    "JesEffectiveStat1_down", "JesEffectiveStat1_up",
    "JesEffectiveStat2_down", "JesEffectiveStat2_up",
    "JesEffectiveStat3_down", "JesEffectiveStat3_up",
    "JesEffectiveStat4_down", "JesEffectiveStat4_up",
    "Pileup_OffsetMu_down", "Pileup_OffsetMu_up",
    "Pileup_OffsetNPV_down", "Pileup_OffsetNPV_up",
    "Pileup_Pt_down", "Pileup_Pt_up",
    "Pileup_Rho_down", "Pileup_Rho_up",
    "SinglePart_down", "SinglePart_up",
    "flavor_comp_down", "flavor_comp_up",
    "flavor_response_down", "flavor_response_up",
    "PunchThrough_down", "PunchThrough_up",
    // scale factor systematics
    // lepton sf - total syst = 77
    "lepRecoSF_down", "lepRecoSF_up",
    "lepIdSF_down", "lepIdSF_up",
    "lepTrigSF_down", "lepTrigSF_up",
    // btag sf - total syst = 
    "btagSF_up",
    "btagSF_break0_up",
    "btagSF_break1_up",
    "btagSF_break2_up",
    "btagSF_break3_up",
    "btagSF_break4_up",
    "btagSF_break5_up",
    "btagSF_break6_up",
    "btagSF_break7_up",
    "btagSF_break8_up", // 87
    "btagSF_down",
    "btagSF_break0_down",
    "btagSF_break1_down",
    "btagSF_break2_down",
    "btagSF_break3_down",
    "btagSF_break4_down",
    "btagSF_break5_down",
    "btagSF_break6_down",
    "btagSF_break7_down",
    "btagSF_break8_down", // 97
    "ctagSF_up",
    "ctagSF_break0_up",
    "ctagSF_break1_up",
    "ctagSF_break2_up",
    "ctagSF_break3_up",
    "ctagSF_break4_up",
    "ctagSF_break5_up",
    "ctagSF_break6_up", // 105
    "ctagSF_down",
    "ctagSF_break0_down",
    "ctagSF_break1_down",
    "ctagSF_break2_down",
    "ctagSF_break3_down",
    "ctagSF_break4_down",
    "ctagSF_break5_down",
    "ctagSF_break6_down", // 113
    "mistagSF_up",
    "mistagSF_break0_up",
    "mistagSF_break1_up",
    "mistagSF_break2_up",
    "mistagSF_break3_up",
    "mistagSF_break4_up",
    "mistagSF_break5_up",
    "mistagSF_break6_up",
    "mistagSF_break7_up",
    "mistagSF_break8_up",
    "mistagSF_break9_up",
    "mistagSF_break10_up",
    "mistagSF_break11_up", // 126
    "mistagSF_down",
    "mistagSF_break0_down",
    "mistagSF_break1_down",
    "mistagSF_break2_down",
    "mistagSF_break3_down",
    "mistagSF_break4_down",
    "mistagSF_break5_down",
    "mistagSF_break6_down",
    "mistagSF_break7_down",
    "mistagSF_break8_down",
    "mistagSF_break9_down",
    "mistagSF_break10_down",
    "mistagSF_break11_down", // 139
    // Systematics which exchange samples (e.g. generator systematics)
    "WtDS",
    "MCatNLO", // WARNING: This is a special systematic, see plotting
    "MCatNLO_SgTop", // WARNING: This is a special systematic, see plotting
    "MCatNLO_tchan",
    "MCatNLO_schan",
    "MCatNLO_wt",
    "MCatNLO_ttbar",
    "aMCatNLO_tchan",
    "aMCatNLO_schan",
    "ISRFSR_ttbar_More",
    "ISRFSR_ttbar_Less", // 150
    "RadHi_ttbar",
    "RadLo_ttbar",
    "ScaleVar1",
    "ScaleVar2",
    "ScaleVar3",
    "ScaleVar4",
    "ScaleVar5",
    "ScaleVar6",
    "ScaleVar_tchan1",
    "ScaleVar_tchan2", // 160
    "ScaleVar_tchan3",
    "ScaleVar_tchan4",
    "WjetsGen",
    "PowhegPythia_t",
    "PowhegPythia2011C_t",
    "PowhegHerwig_t",
    "Pdf_down", 
    "Pdf_up", // 168
    "kLEP_FAKE_BDECAYGEN_SHAPE",
    "undefined"
};

namespace {

//
// Helper class managing the translation between the names and the IDs of
// systematic variations. Also provides access to "handles", TNamed objects
// encapulating names, which can be used to store systematic variations in
// TList & co (because we don't want to use std::set).
//
class TDynamicSystematicTable {

public:

    enum { kOffset = 0x10000 };

    TDynamicSystematicTable()
        : fNames(new TObjArray())
    {
        fNames->SetOwner();
    }

    ~TDynamicSystematicTable() {
        delete fNames;
    }

    Int_t GetIdByName(char const * name) {
        //
        // Retrieves an ID for a name, possibly inserting it into the list of names.
        //
        return GetIdByHandle(GetHandleByName(name));
    }

    Int_t GetIdByHandle(TObject * handle) const {
        Int_t index = fNames->IndexOf(handle);
        assert(index >= 0);
        return index + kOffset;
    }

    TObject * GetHandleById(Int_t id) const {
        id -= kOffset;
        assert(id >= 0 && id < fNames->GetEntries());
        return fNames->At(id);
    }

    char const * GetNameById(Int_t id) const {
        return GetHandleById(id)->GetName();
    }

    TObject * GetHandleByName(char const * name) {
        //
        // Retrieves a handle for a name, possibly inserting it into the list of names.
        //
        TObject * handle = fNames->FindObject(name);
        if ( !handle ) {
            handle = new TNamed(name, name);
            fNames->Add(handle);
        }
        return handle;
    }

private:

    TObjArray * fNames;

};

TDynamicSystematicTable fgDynamicSystematicTable;

}

#ifndef __CINT__
ClassImp(AtlTopLevelAnalysis);
#endif

//____________________________________________________________________

AtlTopLevelAnalysis::AtlTopLevelAnalysis(const char* name,
					 const char* title) :
    TTask(name, title),
    fCampaign(0) {
    //
    // Default constructor
    //
    fListOfSamples  = new TList;
    fListOfCuts     = new TList;
    fListOfSubselectionCuts = new TList;
    fListOfToolCuts = new TList;
    fListOfUserEnvs = new TList;
    fListOfTools    = new TList;
    fNtuplePlotCmds = new TList;
    
    fLepChannels = 0x0;
    fJetMults    = 0x0;
    fSystematics = new TBits(fgNumSystematics);
    fUseLepChannelSplit = kFALSE;

    fHostname     = new TString(gSystem->HostName());

    fSgTopVersion = 0;
    fDSIDList     = 0;
    fGridSampleList = "";
    fMCWeightNamesFile = "";

    fSelector    = 0;
    fInputDir    = 0;
    fPlotDir    = 0;
    fHistDir     = 0;
    fOverrideInputDir = "";
    fOverrideBatchNodeAll = kFALSE;

    fJobHomeDir       = 0;
    fTempDir          = 0;
    fOutputFilePrefix = new TString("hist");
    fTreeName         = new TString("physics");
    fLooseTreeOnly    = kFALSE;
    fOutputTreeName   = 0;
    fPlotterScheme    = 0;
    
    fForceRetry        = kFALSE;
    fBuildHforTree     = kFALSE;
    fBuildAnalysisTree = kTRUE;
    fBuildMergingTree  = kTRUE;
    fBuildPlottingTree = kTRUE;
    fBuildMemTkAnalysisTree = kFALSE;
    fBuildMemDiscAnalysisTree   = kFALSE;
    fBuildHistFactoryTree   = kFALSE;
    fUseHforSamples    = kTRUE;
    fUseSubJobsAsInput = kFALSE;
    fTtbarIsSplitted   = kTRUE;
     
    fBuildMode     = kInvalidMode;
    fInputFormat   = kInvalidFormat;

    fGridJob = kFALSE;
    fGridAnalysisOnGrid = kFALSE;
    fGridAnalysisTemplateOnly = kFALSE;
    fGridRootVersion = 0;
    fGridCmtVersion  = 0;
    fGridUserName    = 0;
    fGridSuffix      = 0;
    fGridIdSuffix    = nullptr;

    // MemTk analysis
    fMemTkRunScript = 0;
    fMEMTarBallOnly = kFALSE;
    fMemTkTreatAllJetsAsBJets = kFALSE;
    
    fPriority = 0;

    fNSubJobsData  = 1;
    fNSubJobsQCD   = 2;
    fNSubJobsTtbar = 3;
    fNSubJobsSgTopSChannel = 1; //was 30 --> For Test set it to 1
    fNSubJobsSgTopTChannel = 5;
    fNSubJobsSgTopWt = 1;
    fNSubJobsWjetsB = 3;
    fNSubJobsWjetsC = 1;
    fNSubJobsWjetsLight = 1;
    fNSubJobsZjetsB = 1;
    fNProcessNthEventsOnly = 1;
    fMaxEventsPerSubjob = 0;
    fSampleSizes = new TObjArray();
    
    fMeasurement = 0;

    fDebug = 0;
    fDebugBuild = 0;
    fBatchNodeAutomatic = kTRUE;

    // defaults for the plotter task
    // fSkipData      = kTRUE;
    fPlotterScheme = new TString("Ttbar");
    fAtlasLabelStatus = new TString("Work in Progress"); 
    fCenterOfMassEnergyLabel = new TString("8");
    fLuminosityLabel = new TString("20 fb^{-1}");
    fUsePlotterSchemeDir = kFALSE;
    fDataLuminosity  = 0; // 20339.800 pb^-1 was the default before v31 NTuples. this caused Data vs MC discrepancy. Set now to 0.
    fDrawDataZeroEntryErrors = kFALSE;
    fDrawMCError             = kTRUE;
    fExportPDF               = kFALSE;
    fUseOverflowUnderflow    = kFALSE;
    fDrawSignalOverlay       = kFALSE;
    fScaleOverlay            = 1.;
    fDrawSignificance        = kFALSE;

    fSelectedVariations = new TList();
}

//____________________________________________________________________

AtlTopLevelAnalysis::~AtlTopLevelAnalysis() {
    //
    // Default destructor
    //
    fListOfSamples ->Delete(); delete fListOfSamples;
    fListOfCuts    ->Delete(); delete fListOfCuts;
    fListOfSubselectionCuts->Delete(); delete fListOfSubselectionCuts;
    fListOfToolCuts->Delete(); delete fListOfToolCuts;
    fListOfUserEnvs->Delete(); delete fListOfUserEnvs;
    fListOfTools   ->Delete(); delete fListOfTools;
    fNtuplePlotCmds->Delete(); delete fNtuplePlotCmds;

    delete fHostname;
    if ( fSgTopVersion   != 0 ) delete fSgTopVersion;
    if ( fDSIDList       != 0 ) delete fDSIDList;
    if ( fSelector       != 0 ) delete fSelector;    
    if ( fInputDir       != 0 ) delete fInputDir;
    if ( fPlotDir        != 0 ) delete fPlotDir;
    if ( fHistDir        != 0 ) delete fHistDir;
    if ( fJobHomeDir     != 0 ) delete fJobHomeDir;
    if ( fTempDir        != 0 ) delete fTempDir;
    if ( fOutputTreeName != 0 ) delete fOutputTreeName;
    if ( fPlotterScheme  != 0 ) delete fPlotterScheme;
    delete fOutputFilePrefix;
    delete fTreeName;
    delete fSampleSizes;

    if ( fGridRootVersion != 0 ) delete fGridRootVersion;
    if ( fGridCmtVersion  != 0 ) delete fGridCmtVersion; 
    if ( fGridUserName    != 0 ) delete fGridUserName;
    if ( fGridSuffix      != 0 ) delete fGridSuffix;

    delete fSelectedVariations;
}

//____________________________________________________________________

AtlSample * AtlTopLevelAnalysis::AddSample(const char* Name, const char* Title,
				    const char* color, AtlSample::EType Type,
				    Float_t xsection, const char* PathName) {
    //
    // Add sample
    //
    if (fListOfSamples->FindObject(Name)) {
        Fatal(__FUNCTION__, "Cannot add sample %s a second time. Maybe you should have used AtlSample::AddPath?", Name);
        gSystem->Abort(0);
    }
    AtlSample* sample = new AtlSample(Name, Title, color, Type, xsection, PathName);
    fListOfSamples->Add(sample);
    return sample;
}

//____________________________________________________________________

void AtlTopLevelAnalysis::Print(Option_t *option) const {
    //
    // Print analysis information
    //

    // General information
    cout << endl
	 << "==================================================" << endl
	 << GetTitle() << endl
	 << "==================================================" << endl
	 << "Lepton channels    :";
    PrintLeptonChannelNames();
    cout << endl
	 << "Jet multiplicities :";
    PrintJetMultiplicityNames();
    cout << endl
	 << "Systematics        :";
    PrintSystematicNames();
    cout << endl;
    
    // DATA samples
    TIter next(fListOfSamples);
    AtlSample *sample = 0;
    while ( (sample = (AtlSample*)next()) ) {
	if ( sample->IsDATA() ) sample->Print();
    }

    // MC samples
    next.Reset();
    while ( (sample = (AtlSample*)next()) ) {
	if ( sample->IsMC() ) sample->Print();
    }

    cout << "==================================================" << endl
	 << endl;
}

//____________________________________________________________________

char const * AtlTopLevelAnalysis::GetSystematicName(Int_t syst) {
    if (syst >= fgDynamicSystematicTable.kOffset) {
        return fgDynamicSystematicTable.GetNameById(syst);
    }
    else {
        assert(syst >= 0);
        assert(syst < fgNumSystematics);
        return fgSystematicNames[syst];
    }
}

//____________________________________________________________________

void AtlTopLevelAnalysis::BuildTree(Bool_t OpenBrowser) {
    //
    // Build task tree
    //

    // Reset any existing structure
    fTasks->Delete();

    // Build Hfor Splitting tasks
    if ( fBuildHforTree ) {
	TTask *task_hfor = new TTask("Hfor Splitting","Hfor Splitting");
	Add(task_hfor);
	fBuildMode = kHforSplitting;
	BuildHforSplittingTree(task_hfor);
    }
    
    // Build Analysis tasks
    if ( fBuildAnalysisTree ) {
      TTask *task_analysis = new TTask("Analysis","Analysis");
      Add(task_analysis);
      fBuildMode = kAnalysis;
      Info(__FUNCTION__, "Building analysis tree for output dir %s",
           fHistDir->Data());
      BuildAnalysisTree(task_analysis);
    }

    // Build Merging tasks
    if ( fBuildMergingTree ) {
	TTask *task_merge = new TTask("Merging","Merging");
	Add(task_merge);
	fBuildMode = kMerging;
	BuildMergingTree(task_merge);
    }

    // Build Plotting Tasks
    if ( fBuildPlottingTree ) {
      TTask *task_plot = new TTask("Plotting","Plotting");
      Add(task_plot);
      fBuildMode = kPlotting;
      Info(__FUNCTION__, "Building plotting tree for output dir %s",
           fPlotDir->Data());
      BuildPlottingTree(task_plot);
    }

    // Build MemTkAnalysis tasks
    if ( fBuildMemTkAnalysisTree ) {
      TTask *task_MemTkAnalysis = new TTask("MemTkAnalysis","MemTkAnalysis");
      Add(task_MemTkAnalysis);
      fBuildMode = kMemTkAnalysis;
      BuildAnalysisTree(task_MemTkAnalysis);
    }

    // Build MemDiscAnalysis tasks
    if ( fBuildMemDiscAnalysisTree ) {
      TTask *task_MemDiscAnalysis = new TTask(gSystem->BaseName(fPlotDir->Data()),
                                              gSystem->BaseName(fPlotDir->Data()));
      Add(task_MemDiscAnalysis);
      fBuildMode = kMemDiscAnalysis;
      BuildAnalysisTree(task_MemDiscAnalysis);
    }

    // Build HistFactory tasks
    if ( fBuildHistFactoryTree ) {
	TTask *task_HistFactory = new TTask("HistFactory",
					    "HistFactory");
	Add(task_HistFactory);
	BuildHistFactoryTree(task_HistFactory);
    }

    // Open task structure in ROOT browser
    gROOT->GetListOfTasks()->Add(this);
    gROOT->GetListOfBrowsables()->Add(this);
    if ( OpenBrowser ) {    
	new TBrowser;
    }
}

//____________________________________________________________________

void AtlTopLevelAnalysis::BuildHforSplittingTree(TTask *ParentTask) {
    //
    // Build Hfor Splitting Tree with systematic subfolders
    //
    BuildLeptonChannelFolders(ParentTask, 0);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::BuildAnalysisTree(TTask *ParentTask) {
    //
    // Build Analysis Tree with systematic subfolders
    //
    BuildJetBinFolders(ParentTask);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::BuildMergingTree(TTask *ParentTask) {
    //
    // Build Merging Tree with systematic subfolders
    //
    BuildJetBinFolders(ParentTask);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::BuildPlottingTree(TTask *ParentTask) {
    //
    // Build Analysis Tree with systematic subfolders
    //
    BuildJetBinFolders(ParentTask);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::BuildMemTkAnalysisTree(TTask *ParentTask) {
    //
    // Build Analysis Tree with systematic subfolders
    //
    BuildJetBinFolders(ParentTask);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::BuildMemDiscAnalysisTree(TTask *ParentTask) {
    //
    // Build Analysis Tree with systematic subfolders
    //
    BuildJetBinFolders(ParentTask);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetBuildHistFactoryTree(Bool_t flag) {
    //
    // Only build histfactory tree.
    // For all other trees, the necessary information is not provided.
    //
    fBuildHforTree     = kFALSE;
    fBuildAnalysisTree = kFALSE;
    fBuildMergingTree  = kFALSE;
    fBuildPlottingTree = kFALSE;
    fBuildMemTkAnalysisTree = kFALSE;
    fBuildMemDiscAnalysisTree   = kFALSE;
    fBuildHistFactoryTree   = kTRUE;
}

//____________________________________________________________________

void AtlTopLevelAnalysis::BuildHistFactoryTree(TTask *ParentTask) {
    //
    // Build Histfactory Tree with all tasks needed for fitting
    // - Create Template file with all systematics requested
    // - Create tasks for checking shape systematics
    // - Create RooFit/Histfactory workspace
    //

    // Add Task for creating templates
    AtlHistFactoryTask *task_templates
	= new AtlHistFactoryTask("HistFactory_FitTemplates",
				 "HistFactory_FitTemplates");
    task_templates->SetJobHome( fJobHomeDir->Data() );
    task_templates->SetRunMode(AtlHistFactoryTask::kCreateFitTemplates);
    task_templates->SetMeasurement(fMeasurement);
    task_templates->Initialize(); // Setup job home, logfile, etc. (requested by AtlTask)
    ParentTask->Add(task_templates);

    // Add Task for creating shape templates
    AtlHistFactoryTask *task_shape_templates
	= new AtlHistFactoryTask("HistFactory_ShapeTemplates",
				 "HistFactory_ShapeTemplates");
    task_shape_templates->SetJobHome( fJobHomeDir->Data() );
    task_shape_templates->SetRunMode(AtlHistFactoryTask::kCreateShapeTemplates);
    task_shape_templates->SetMeasurement(fMeasurement);
    task_shape_templates->Initialize(); // Setup job home, logfile, etc. (requested by AtlTask)
    ParentTask->Add(task_shape_templates);
    
    // Add Task for performing p-value test
    AtlHistFactoryTask *task_pvalue
	= new AtlHistFactoryTask("HistFactory_ShapeTest",
				 "HistFactory_ShapeTest");
    task_pvalue->SetJobHome( fJobHomeDir->Data() );
    task_pvalue->SetRunMode(AtlHistFactoryTask::kPerformShapeTests);
    task_pvalue->SetMeasurement(fMeasurement);
    task_pvalue->Initialize(); // Setup job home, logfile, etc. (requested by AtlTask)
    ParentTask->Add(task_pvalue);

    // Add Task for performing p-value test
    AtlHistFactoryTask *task_workspace
	= new AtlHistFactoryTask("HistFactory_Workspace",
				 "HistFactory_Workspace");
    task_workspace->SetJobHome( fJobHomeDir->Data() );
    task_workspace->SetRunMode(AtlHistFactoryTask::kCreateWorkspace);
    task_workspace->SetMeasurement(fMeasurement);
    task_workspace->Initialize(); // Setup job home, logfile, etc. (requested by AtlTask)
    ParentTask->Add(task_workspace);

    // Add Task to fit the model
    AtlHistFactoryTask *task_fitmodel
	= new AtlHistFactoryTask("HistFactory_FitModel",
				 "HistFactory_FitModel");
    task_fitmodel->SetJobHome( fJobHomeDir->Data() );
    task_fitmodel->SetRunMode(AtlHistFactoryTask::kFitModel);
    task_fitmodel->SetMeasurement(fMeasurement);
    task_fitmodel->Initialize(); // Setup job home, logfile, etc. (requested by AtlTask)
    ParentTask->Add(task_fitmodel);

    // Add Task to get a systematic breakdown table
    AtlHistFactoryBreakdownTask *task_breakdown
	= new AtlHistFactoryBreakdownTask("HistFactory_Breakdown",
					  "HistFactory_Breakdown",
					  fMeasurement);
    ParentTask->Add(task_breakdown);

    // Add Task to fit the model
    AtlHistFactoryTask *task_ratetable
	= new AtlHistFactoryTask("HistFactory_RateTable",
				 "HistFactory_RateTable");
    task_ratetable->SetJobHome( fJobHomeDir->Data() );
    task_ratetable->SetRunMode(AtlHistFactoryTask::kRateTable);
    task_ratetable->SetMeasurement(fMeasurement);
    task_ratetable->Initialize(); // Setup job home, logfile, etc. (requested by AtlTask)
    ParentTask->Add(task_ratetable);

    // Add Task to create yield tables
    AtlHistFactoryYieldTableTask *task_yields
	= new AtlHistFactoryYieldTableTask("HF_YieldsTable",
					   "HF_YieldsTable");
    task_yields->SetJobHome( fJobHomeDir->Data() );
    task_yields->SetMeasurement(fMeasurement);
    task_yields->Initialize(); // Setup job home, logfile, etc. (requested by AtlTask)
    ParentTask->Add(task_yields);


    // Add Task to create prefit and postfit plots
    AtlHistFactoryPlotterTask *task_plot
	= new AtlHistFactoryPlotterTask("HF_Plot",
					"HF_Plot");
    task_plot->SetJobHome( fJobHomeDir->Data() );
    task_plot->SetMeasurement(fMeasurement);
    task_plot->Initialize(); // Setup job home, logfile, etc. (requested by AtlTask)
    ParentTask->Add(task_plot);

}

//____________________________________________________________________

void AtlTopLevelAnalysis::BuildHforSplittingTasks(TTask *ParentTask,
						  Int_t Lepton,
						  Int_t Systematic) {
    //
    // Build Hfor Splitting Tasks including all samples
    //
    
    // no extra files for scale factor systematics (in this case nominal files are used)
    // --> i.e. no hfor splitting required --> skip!
    if ( IsScaleFactorSystematic(Systematic) ||
	 IsBTagEVScaleFactorSystematic(Systematic) ) { return; }

    AtlSample *sample = 0;
    TIter next_sample(fListOfSamples);
    
    // =================
    // Standard MC tasks
    // =================
    TTask *task_mcdir = new TTask("MC", "MC");
    ParentTask->Add(task_mcdir);
    
    AtlHforSplittingTask *task_mc = 0;
    while ( (sample = (AtlSample*)next_sample()) ) {
	if ( sample->IsMC() ) {
	    // only W+jets samples
	    if ( !sample->IsWjetsLight() && !sample->IsWjetsHeavy() ) { continue; }
	    // build category folder
	    	    
	    // Add Hfor Splitting Task
	    task_mc = new AtlHforSplittingTask(sample->GetName(), sample->GetTitle());

	    TString *jobhome = new TString( Form("%s/hfor/%s/%s/%s",
						 fJobHomeDir->Data(),
						 AtlSelector::GetLeptonLabel(Lepton),
						 GetSystematicName(Systematic),
						 sample->GetName()) );
	    jobhome->ReplaceAll("//", 2, "/", 1);

	    TString *tempdir = new TString( Form("%s/hfor/%s/%s/%s",
						 fTempDir->Data(),
						 AtlSelector::GetLeptonLabel(Lepton),
						 GetSystematicName(Systematic),
						 sample->GetName()) );
	    tempdir->ReplaceAll("//", 2, "/", 1);

	    TString *tempfile = new TString( Form("%s%s.log",
						  tempdir->Data(),
						  sample->GetName()) );

	    // dummy file (.root will be replaced by .log)
	    task_mc->SetOutputFile( Form("%s.root", sample->GetName()) );
	    task_mc->SetTempOutputFile( tempfile->Data() );
	    task_mc->SetTempOutputPath( tempdir->Data() );
	    task_mc->SetJobHome( jobhome->Data() );
	    
	    // only works since all files are treated separatly
	    task_mc->AddInputFiles( GetInputFileName(sample, Lepton, Systematic) );
	    delete jobhome;
	    delete tempdir;
	    delete tempfile;

	    // ToDo: Check the tree name here
	    task_mc->SetInputTreeName(fTreeName->Data());
	    
	    // Add config to Analysis Task
	    // ---------------------------
	    task_mcdir->Add(task_mc);
	}
    }
}

//____________________________________________________________________

void AtlTopLevelAnalysis::BuildJetBinFolders(TTask *ParentTask) {
    //
    // Build all JetBin Folders with all sub-folders (lepton + systematics)
    //

    // Create folders for jet bins
    for ( Int_t i = 0; i < AtlSelector::fgNumJetMults; i++ ) {
	UInt_t jetbin = 0x1 << i;
	if ( fJetMults & jetbin ) {
            Info(__FUNCTION__, "Creating tasks for n=%s jets", AtlSelector::GetJetMultLabel(i));
	    TTask *task_jetbindir = new TTask(Form("Jet Bin %s",
						   AtlSelector::GetJetMultLabel(i)),
					      Form("Jet Bin %s",
						   AtlSelector::GetJetMultLabel(i)));
	    ParentTask->Add(task_jetbindir);
	    BuildLeptonChannelFolders(task_jetbindir, i);
	}
    }
}

//____________________________________________________________________

void AtlTopLevelAnalysis::BuildLeptonChannelFolders(TTask *ParentTask,
						    Int_t Jetbin ) {
    //
    // Build lepton folders with all sub-folders (systematics)
    //

    // Create folders for lepton channels
    Int_t iLepChan = 0;
    if ( (GetCampaign() > 13) && !fUseLepChannelSplit ) {
        BuildSystematicsFolders(ParentTask, Jetbin, 2);
    } else {
	for (; iLepChan < AtlSelector::fgNumLepChannels; ++iLepChan) {
	    UInt_t channel = 0x1 << iLepChan;
	    if ( fLepChannels & channel ) {
                Info(__FUNCTION__, "Creating tasks for %s lepton channel", AtlSelector::GetLeptonLabel(iLepChan));
		TTask *task_channeldir = new TTask(Form("%s Channel",
							AtlSelector::GetLeptonLabel(iLepChan)),
						   Form("%s Channel",
							AtlSelector::GetLeptonLabel(iLepChan)));
		ParentTask->Add(task_channeldir);
		BuildSystematicsFolders(task_channeldir, Jetbin, iLepChan);
	    }
	}
    }
    if ( iLepChan == 2 && fBuildMode == kPlotting ) {
	// merged enu/munu plotting task
        Info(__FUNCTION__, "Creating tasks for lnu lepton channel");
	TTask *task_channeldir = new TTask("lnu Channel", "lnu Channel");
	ParentTask->Add(task_channeldir);
	BuildSystematicsFolders(task_channeldir, Jetbin, iLepChan);
    }
}

//____________________________________________________________________

void AtlTopLevelAnalysis::BuildSystematicsFolders(TTask *ParentTask,
						 Int_t Jetbin,
						 Int_t LepChannel) {
    //
    // Build systematic folders and with user tasks
    //

    // Create folders for all standard systematics
    for ( Int_t syst = 0; syst < fgNumSystematics; syst++ ) {
	//UInt_t syst = 0x1 << i;
	if ( fSystematics->TestBitNumber(syst) ) {
	    // Only add lepton Systematics if correct lepton
	    UInt_t lep = 0x1 << LepChannel;
	    if ( (lep == AtlSelector::kElectron) &&
			 ( (syst == kMUID_RES) || 
			   (syst == kMUMS_RES) ||
			   (syst == kMUSC_DOWN) || (syst == kMUSC_UP) ) ) {
			continue;
	    }
	    if ( (lep == AtlSelector::kMuon) &&
			 ( (syst == kEER_DOWN) || (syst == kEER_UP) || 
			   (syst == kEES_DOWN) || (syst == kEES_UP) ) ) {
			continue;
		}
            Info(__FUNCTION__, "Creating tasks for systematic %s", GetSystematicName(syst));
	    TTask *task_systdir = new TTask(Form("%s",
						 GetSystematicName(syst)),
					    Form("%s",
						 GetSystematicName(syst)));
	    ParentTask->Add(task_systdir);
	    BuildUserTasks(task_systdir, Jetbin, LepChannel, syst);
	}
    }

    // Create folders for all selected-by-name systematics
    for ( TIter next(fSelectedVariations); TObject * handle = next(); ) {
        Int_t syst = fgDynamicSystematicTable.GetIdByHandle(handle);
        Info(__FUNCTION__, "Creating tasks for systematic %s", GetSystematicName(syst));
        TTask * task_systdir = new TTask(GetSystematicName(syst), GetSystematicName(syst));
        ParentTask->Add(task_systdir);
        BuildUserTasks(task_systdir, Jetbin, LepChannel, syst);
    }

}

//____________________________________________________________________

Int_t AtlTopLevelAnalysis::GetSystematicIdByName(char const * name) {
    //
    // Translate name of variation into
    //

    for (size_t i = 0; i < fgNumSystematics; ++i) {
        if (strcmp(fgSystematicNames[i], name) == 0)
            return i;
    }
    return fgDynamicSystematicTable.GetIdByName(name);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::AddSystematic2(char const * name) {

    TObject * handle = fgDynamicSystematicTable.GetHandleByName(name);
    if ( !fSelectedVariations->Contains(handle) )
        fSelectedVariations->Add(handle);

}

//____________________________________________________________________

void AtlTopLevelAnalysis::BuildUserTasks(TTask *ParentTask,
					 Int_t Jetbin,
					 Int_t LepChannel,
					 Int_t Systematic) {
    //
    // Build user tasks (fBuildMode is used to decide on tasks to build)
    //

    if ( fBuildMode == kHforSplitting ) {
	BuildHforSplittingTasks(ParentTask, LepChannel, Systematic);
    } else if ( fBuildMode == kAnalysis ) {
	BuildAnalysisTasks(ParentTask, Jetbin, LepChannel, Systematic);
    } else if ( fBuildMode == kMerging ) {
	BuildMergingTasks(ParentTask, Jetbin, LepChannel, Systematic);
    } else if ( fBuildMode == kPlotting ) {
	BuildPlottingTasks(ParentTask, Jetbin, LepChannel, Systematic);
    } else if ( fBuildMode == kMemTkAnalysis ) {
	BuildMemTkAnalysisTasks(ParentTask, Jetbin, LepChannel, Systematic);
    } else if ( fBuildMode == kMemDiscAnalysis ) {
	BuildMemDiscAnalysisTasks(ParentTask, Jetbin, LepChannel, Systematic);
    }
}

//____________________________________________________________________

void AtlTopLevelAnalysis::BuildAnalysisTasks(TTask *ParentTask,
					     Int_t Jetbin,
					     Int_t LepChannel,
					     Int_t Systematic) {
    //
    // Build Analysis Tasks for DATA and MC samples
    //
    AtlSample *sample = 0;
    TIter next_sample(fListOfSamples);
    AtlAppAnalysisTask *task_app = 0;

    // ===================
    // Standard DATA tasks
    // ===================
    TTask *task_datadir = new TTask("DATA", "DATA");
    ParentTask->Add(task_datadir);

    // =================
    // Standard MC tasks
    // =================
    TTask *task_mcdir = new TTask("MC", "MC");
    ParentTask->Add(task_mcdir);

    // ==================
    // Standard QCD tasks
    // ==================
    TTask *task_qcddir = new TTask("QCD", "QCD");
    ParentTask->Add(task_qcddir);
    
    while ( (sample = (AtlSample*)next_sample()) ) {
	
	if ( IsIgnored(sample) )
	    continue;

	// Skip systematics for data samples for faster setup
	if ( sample->IsDATA()
             && Systematic != kNOMINAL
             && !( strcmp("TEMPLATE", GetSystematicName(Systematic)) == 0 ) ) {
            continue;
        }

	// Skip systematics for qcd samples (for now)
	if ( sample->IsQCD() && Systematic != kNOMINAL ) { continue; }
	
	// Skip egamma/muon stream if neccessary
	UInt_t lep = 0x1 << LepChannel;
	if ( sample->IsDATAEGamma() && lep == AtlSelector::kMuon ) { continue; }
	if ( sample->IsDATAMuon()   && lep == AtlSelector::kElectron ) { continue; }
	if ( sample->IsQCDEGamma() && lep == AtlSelector::kMuon ) { continue; }
	if ( sample->IsQCDMuon()   && lep == AtlSelector::kElectron ) { continue; }

	
	// Special treatment for samples only needed for a systematic
	// (e.g. generator systematic)
	// Samples are skipped if the systematic is...
	//  a) a sample systematic
	//     --> Skip all samples, but the one that belongs
	//         to this systematic. As identifier, the systematic
	//         name has to be used.
	//  b) not a sample systematic --> skip all samples that belong to a systematic
	TString samplename = sample->GetName();
	if ( (IsSampleSystematic(Systematic) && // a) skip all nominal files
	      !sample->IsSystematicSample()) || 
	     (IsSampleSystematic(Systematic) && // a) skip all files that do not belong
	      !samplename.Contains(GetSystematicName(Systematic))) || // to this systematic
	     (!IsSampleSystematic(Systematic) && // b) skip all systematic samples
	      sample->IsSystematicSample()) )    //    if using nominal or not a sample systematic
	    continue;
	
	// ---------------------------------------
	//  General Setup for DATA and MC Samples
	// ---------------------------------------
	
	task_app = new AtlAppAnalysisTask(sample->GetName(), GetSystematicName(Systematic)); //sample->GetTitle());
	task_app->SetDebug(fDebug);
	task_app->SetDebugBuild(fDebugBuild);
	if(!fGridJob) task_app->SetPriority(fPriority);

        task_app->SetBatchNodeAll(fOverrideBatchNodeAll);
        task_app->SetXsection(sample->GetXsection());

        // Special treatment for MC weight systematics, starting in v29
        // MC weight systematics are named MCWEIGHT_<weight_name>
        // The task has to be given the weight position in the
        //   mc_generator_weights vector and the totalEventsWeighted
        //   from the sumWeights Tree
        // Special treatment for fixed weight positions: use MCWEIGHT_FIXEDPOS_<weight_number>
        // Special treatment for multiplication of MC weight systematics:
        //   use MCWEIGHTS_N_X_name1Xname2X..XnameN,
        //   where N is amount of systematics and X is a separator
        TString systName = TString(GetSystematicName(Systematic));
        TString weightName = "";
        Int_t EvtReaderMCWeightPosition = -1;
        Double_t EvtReaderMCWeightTotalEvents = -1.;
        if ( systName.BeginsWith("MCWEIGHT_") ) {
            if ( systName.BeginsWith("MCWEIGHT_FIXEDPOS_") ) {
                // position is in systName
                TString systPosString(systName(18, systName.Length() - 18));
                EvtReaderMCWeightPosition = systPosString.Atoi();
            } else {
                // position has to be looked up via weight name
                weightName = systName(9, systName.Length() - 9);
                EvtReaderMCWeightPosition = GetMCWeightPosition(samplename.Data(), weightName.Data());
                if ( EvtReaderMCWeightPosition < 0 ) continue;
            }
            // find totalevents by position
            EvtReaderMCWeightTotalEvents = GetMCWeightTotalEvents(samplename.Data(), EvtReaderMCWeightPosition);
            if ( EvtReaderMCWeightTotalEvents < 0 ) continue;
            task_app->SetEvtReaderMCWeightPosition(
                EvtReaderMCWeightPosition);
            task_app->SetEvtReaderMCWeightTotalEvents(
                EvtReaderMCWeightTotalEvents);
        } else if ( systName.BeginsWith("MCWEIGHTS_") ) {
            // deconstruct MCWEIGHTS_N_X_name1Xname2X...XnameN:
            // - get number of systematics and separator
            std::unique_ptr<TObjArray> tokens(systName.Tokenize("_"));
            TString nsyst_str = static_cast<TObjString*>(tokens->At(1))->GetString();
            TString sep = static_cast<TObjString*>(tokens->At(2))->GetString();
            Int_t nsyst = nsyst_str.Atoi();
            if ( nsyst < 2 or nsyst > 9 ) {
                Error(__FUNCTION__, "Using n=%d weights is not supported. Skip!", nsyst);
                continue;
            }
            // - split weights by separator
            Int_t cut = 12 + nsyst_str.Length() + sep.Length();
            TString syst_vec_str(systName(cut, systName.Length()-cut));
            std::unique_ptr<TObjArray> tokens2(syst_vec_str.Tokenize(sep.Data()));
            if ( tokens2->GetEntries() != nsyst ) {
                Error(__FUNCTION__, "Check systematic name. Could not find %d systematics separated by %s in string %s for systematic %s. Skip!",
                      nsyst, sep.Data(), syst_vec_str.Data(), systName.Data());
                continue;
            }
            // - convert weights to names/positions
            Int_t weight_pos = -1;
            Double_t weight_total = -1.;
            Int_t err_code = 0;
            TString weight_pos_vec_str;
            TString weight_total_vec_str;
            for ( Int_t i = 0; i<tokens->GetEntries(); i++ ) {
                auto obj = tokens2->At(i);
                TString weight_syst_name = static_cast<TObjString*>(obj)->GetString();
                if ( weight_syst_name.BeginsWith("FIXEDPOS_") ) {
                    // support fixed positions here
                    TString systPosString(weight_syst_name(9, weight_syst_name.Length()-9));
                    weight_pos = systPosString.Atoi();
                } else {
                    // if not fixed position, it has to be the weight name
                    weight_pos = GetMCWeightPosition(samplename.Data(), weight_syst_name.Data());
                }
                weight_total = GetMCWeightTotalEvents(samplename.Data(), weight_pos);
                if ( weight_pos < 0 or weight_total < 0 ) {
                    err_code = -1;
                    break;
                }
                // fill into strings to pass to task
                if ( weight_pos_vec_str.IsNull() ) {
                    weight_pos_vec_str = Form("%d", weight_pos);
                    weight_total_vec_str = Form("%g", weight_total);
                } else {
                    weight_pos_vec_str.Append(Form("_%d",weight_pos));
                    weight_total_vec_str.Append(Form("_%g", weight_total));
                }
            }
            if ( err_code != 0 ) {
                Error(__FUNCTION__, "Could not get weights from systematic string %s (error %d). Skip!",
                      systName.Data(), err_code);
                continue;
            }


            // - pass positions and totals to task
            task_app->SetEvtReaderMCWeightPosition(-nsyst);
            Double_t total_events_nominal =  GetMCWeightTotalEvents(samplename.Data(), 0);
            task_app->SetEvtReaderMCWeightTotalEvents(total_events_nominal);
            task_app->SetEvtReaderMCWeightPositionString(weight_pos_vec_str);
            task_app->SetEvtReaderMCWeightTotalEventsString(weight_total_vec_str);
       }

        // Set output paths
	TString *outfile = BuildOutputPath( fHistDir, GetName(),
					    Jetbin, LepChannel,
					    Systematic, "" );
	TString *outfileName = BuildOutputFileName( Jetbin, LepChannel,
						    Systematic, sample->GetName() );
	outfile->Append(outfileName->Data());
	outfile->ReplaceAll("//","/");
	
	TString *tmphist = BuildOutputPath( fTempDir, "hist",
					    Jetbin, LepChannel,
					    Systematic, sample->GetName() );
	tmphist->Append(outfileName->Data());
	tmphist->ReplaceAll("//","/");
	
	TString *tempPath = BuildOutputPath( fTempDir, Form("%s/analysis", GetName()),
					     Jetbin, LepChannel,
					     Systematic, sample->GetName() );
        TString *jobHome = nullptr;
        if ( strcmp("TEMPLATE", GetSystematicName(Systematic)) == 0 ) {
            // template systematic can use simpler job home
            jobHome = new TString(Form("%s/%s/%s/%s/%s/",
                                       fJobHomeDir->Data(), GetName(),
                                       "analysis", "TEMPLATE", sample->GetName()));
        } else {
            jobHome  = BuildOutputPath( fJobHomeDir, Form("%s/analysis", GetName()),
                                                 Jetbin, LepChannel,
                                                 Systematic, sample->GetName() );
        }
	task_app->SetOutputFile( outfile->Data() );
	task_app->SetTempOutputFile( tmphist->Data() );
	task_app->SetTempOutputPath( tempPath->Data() );
	task_app->SetJobHome( jobHome->Data() );
    task_app->SetNProcessNthEventsOnly( fNProcessNthEventsOnly );
    task_app->SetBatchNodeAutomatic(fBatchNodeAutomatic);

        // Set grid options
	if ( fGridJob && fGridAnalysisOnGrid ) {
	    task_app->SetGridJob(kTRUE);
            task_app->SetGridRootVersion(fGridRootVersion->Data());
            task_app->SetGridCmtVersion(fGridCmtVersion->Data());
            task_app->SetGridUserName(fGridUserName->Data());
            task_app->SetGridSuffix(fGridSuffix->Data());
            task_app->SetGridIdSuffix(fGridIdSuffix->Data());
            task_app->SetGridAnalysisName(*outfileName);
            task_app->SetGridSubmit(!fMEMTarBallOnly);
            task_app->SetGridTemplateOnly(fGridAnalysisTemplateOnly);
	}

        // Set input path(s)
        //   The input dir can be overridden to run over the output
        //   from looser cuts; code is similar to MemDisc
        if ( fOverrideInputDir.IsNull() ) {
	if ( fHostname->Contains("lx")) {
	    std::ifstream ifs(sample->GetPathName());
	    std::string line;
	    while (std::getline(ifs, line)) {
		std::string inputStr(GetInputFileNameFromFile( sample, LepChannel, Systematic));
		std::size_t foundInputStr = line.find(inputStr);
		if ( foundInputStr!=std::string::npos ) {
		    const char *inputLine = line.c_str();
		    task_app->AddInputFiles( inputLine );
		}
	    }
	} else {
            for ( int pathIndex = 0, pathCount = sample->GetNPaths(); pathIndex < pathCount; ++pathIndex ) {
	    task_app->AddInputFiles( GetInputFileName( sample, 
						       LepChannel,
						       Systematic,
						       pathIndex) );
            }
	}
        } else {
            // if override
    
            // For analysis output filenames
            TString *ana_outfile;
            TString *ana_outfileName;
            
            // if DATA or QCD take nominal
            if ( sample->IsDATA() )  {
                ana_outfile = BuildOutputPath( &fOverrideInputDir, "",
                                               Jetbin, LepChannel,
                                               kNOMINAL, "" );
                ana_outfileName = BuildOutputFileName( Jetbin, LepChannel,
                                                       kNOMINAL,
                                                       sample->GetName(),
                                                       fUseSubJobsAsInput);
            } else if ( sample->IsQCD() ) {
                // add QCD only once
                ana_outfile = BuildOutputPath( &fOverrideInputDir, "",
                                               Jetbin, LepChannel,
                                               kNOMINAL, "" );
                ana_outfileName = BuildOutputFileName( Jetbin, LepChannel,
                                                       kNOMINAL,
                                                       sample->GetName(),
                                                       fUseSubJobsAsInput);
            } else {
                // In case of MemTk analysis input files
                // we need to take nominal files for scale factors systematics
                // (MemTk not run for 'just' event weights)
                //
                // But only here! Outputfilename and directory must be different of course
                // (see above)
                //
                if ( IsScaleFactorSystematic(Systematic) ||
                     IsBTagEVScaleFactorSystematic(Systematic) ||
                     Systematic == kPdf_up || Systematic == kPdf_down ) {
                    ana_outfile = BuildOutputPath( &fOverrideInputDir, "",
                                                   Jetbin, LepChannel,
                                                   kNOMINAL, "" );
		
                    ana_outfileName = BuildOutputFileName( Jetbin, LepChannel,
                                                           kNOMINAL,
                                                           sample->GetName(),
                                                           fUseSubJobsAsInput);
                } else {
                    ana_outfile = BuildOutputPath( &fOverrideInputDir, "",
                                                   Jetbin, LepChannel,
                                                   Systematic, "" );
		
                    ana_outfileName = BuildOutputFileName( Jetbin, LepChannel,
                                                           Systematic,
                                                           sample->GetName(),
                                                           fUseSubJobsAsInput);
                }
            }
            ana_outfile->Append(ana_outfileName->Data());
            ana_outfile->ReplaceAll("//","/");
            task_app->SetUseAppInput(kTRUE);
            task_app->AddInputFiles(ana_outfile->Data());
        }

	if ( fInputFormat == kTreePerSyst ) {
	    if ( GetCampaign() > 13 ) {
	        // the exact tree name is computed by the reader
                if ( fOverrideInputDir.IsNull() )
                    task_app->SetInputTreeName("");
                else
                    task_app->SetInputTreeName(fTreeName->Data());
                        
	    } else if ( Systematic == kNOMINAL
		 || IsScaleFactorSystematic(Systematic)
		 || IsBTagEVScaleFactorSystematic(Systematic)
		 || IsSampleSystematic(Systematic) 
		 || Systematic == kPdf_up || Systematic == kPdf_down ) {
		    // For the SgTop-D3PDs in Run-1 the nominal tree name
		    // is usually "physics"
		    task_app->SetInputTreeName(fTreeName->Data());
	    } else {
		    // For the SgTop-D3PDs in Run-1 the systematic tree names
		    // are "physics_systXYZ"
		    task_app->SetInputTreeName(Form("%s_%s", fTreeName->Data(), GetSystematicName(Systematic)));
	    }
	} else if ( fInputFormat == kFilePerSyst ) {
	    task_app->SetInputTreeName(fTreeName->Data());
	} else {
	    Error("BuildAnalysisTasks",
		  "InputFormat not known! Abort!");
	    gSystem->Abort();
	}
	    
	task_app->SetSelector(fSelector->Data());
	task_app->SetCtrlPlots(kFALSE);

	if ( fOutputTreeName != 0 ) {
	    task_app->SetOutputTree(fOutputTreeName->GetName(),
				    fOutputTreeName->GetTitle());
	}
	
	TIter next_tool(fListOfTools);
	TObject *tool = 0;

	UInt_t jetmult = 0x1 << Jetbin;
	UInt_t lepton  = 0x1 << LepChannel;

	while ( (tool = next_tool()) ) {
	    if ( ((TString)tool->ClassName()).Contains("AtlObjectsToolD3PDSgTop") ) {
		task_app->SetToolCut( tool->GetName(), "fJetMults", AtlSelector::GetJetMultEnum(jetmult) );
		task_app->SetToolCut( tool->GetName(), "fLepChannel", AtlSelector::GetLeptonEnum(lepton) );
	    }
	    if ( ((TString)tool->ClassName()).Contains("AtlObjRecoScaleFactorTool") ) {
		task_app->SetToolCut( tool->GetName(), "fSystematicMode", GetSystematicEnum(Systematic) );
		task_app->SetToolCut( tool->GetName(),
				      "fXsecFile",
				      Form("\"%s\"", fCrossSectionFilePath.Data()) );
	    }
	    if ( ((TString)tool->ClassName()).Contains("AtlBDecayGenTool") ) {
                task_app->SetToolCut( tool->GetName(), "fSystematicMode", GetSystematicEnum(Systematic) );
                if ( sample->IsZjets() )
                {
                task_app->SetToolCut( tool->GetName(), "fIsZjets", "kTRUE" );
                }
	    }
	}

	// Add config to Analysis Task
        if ( GetCampaign() > 13 ) {
            // use "..._Loose" tree for QCD or via fLooseTreeOnly
            // the JetLepton sample for v23 does NOT have _Loose
        	// the JetLepton sample for v31 does NOT have _Loose
	    if ( (fLooseTreeOnly || sample->IsQCD()) &&
                 !( sample->IsQCDJetLepton() && (GetSgTopVersion()->Contains("v23") || GetSgTopVersion()->Contains("v31") ) ) &&
                 fOverrideInputDir.IsNull() ) {
                if ( !fTreeName->EndsWith("_Loose") ) fTreeName->Append("_Loose");
            } else {
                if ( fTreeName->EndsWith("_Loose") ) fTreeName->ReplaceAll("_Loose", "");
            }
            // set sample type for event reader
            TString reader;
            if ( sample->IsQCDJetLepton() ) {
                reader = "AtlEvtReaderD3PDJetLepton";
                task_app->SetCut("fLeptonFlavour", "AtlSelector::kElectron");
            } else if ( sample->IsQCDAntiMuon() ) {
                reader = "AtlEvtReaderD3PDSgTopR2";
                task_app->SetCut("fLeptonFlavour", "AtlSelector::kMuon");
            } else {
                reader = "AtlEvtReaderD3PDSgTopR2";
            }
            char const * readerArgs = 0;
            if ( sample->IsMC() ) {
                assert(!readerArgs);
                readerArgs = Form("%s::kMC", reader.Data());
            }
            if ( sample->IsDATA() || sample->IsQCD() ) {
                assert(!readerArgs);
                readerArgs = Form("%s::kRealData", reader.Data());
            }
            if ( !readerArgs ) {
                Fatal(__FUNCTION__, "what kind of sample is this: %s ?! Abort!", sample->GetName());
            }
            // Set the event reader, the data type (data/MC) and the SgTop-D3PD version
            if ( fOverrideInputDir.IsNull() ) {
                task_app->SetEvtReader(
                    reader.Data(),
                    Form("%s, \"%s\", \"%s\", \"%s\", %d",
                         readerArgs, GetSgTopVersion()->Data(),
                         GetSystematicName(Systematic), fTreeName->Data(),
                         EvtReaderMCWeightPosition >= 0 ? EvtReaderMCWeightPosition : 0));
            } else {
                // force to use the provided treename
                task_app->SetEvtReader(
                    reader.Data(),
                    Form("%s, \"%s\", \"%s\", %d",
                         readerArgs, GetSgTopVersion()->Data(),
                         fTreeName->Data(),
                         EvtReaderMCWeightPosition >= 0 ? EvtReaderMCWeightPosition : 0));
            }
        }
	task_app->AddCuts(fListOfCuts);
	task_app->AddSubselectionCuts(fListOfSubselectionCuts);
	task_app->AddTools(fListOfTools);
	task_app->AddToolCuts(fListOfToolCuts);
	task_app->AddUserEnvs(fListOfUserEnvs);

        // skip job if already successful
        if ( GetTaskStatus(jobHome, outfileName, outfile, kTRUE) ) {
            delete outfile;
            delete outfileName;
            delete tmphist;
            delete tempPath;
            delete jobHome;
            continue;
        }
	
	// clean up TString
	delete outfile;
	delete outfileName;
	delete tmphist;
	delete tempPath;
	delete jobHome;

	
	// const char* lepname = (lep == AtlSelector::kElectron ? "e" : "mu");
	// cout << "Adding Task " << task_app->GetName() << " " << lepname << " for systematic " << fgSystematicNames[Systematic] << endl;
	
	if ( sample->IsDATA() ) {
	    // ---------------------
	    //  DATA specific stuff
	    // ---------------------
	    
	    // Turn QCD Tool off (if present)
	    TIter next_tool(fListOfTools);
	    TObject *tool = 0;
	    while ( (tool = next_tool()) ) {
		if ( ((TString)tool->ClassName()).Contains("AtlQCDMMTool") )
		    task_app->SetToolCut( tool->GetName(), "fProcessMode", "AtlAnalysisTool::kOff");
		// Do not turn off, but choose Nominal Mode
		if ( ((TString)tool->ClassName()).Contains("AtlPdfReweightingTool") )
		    task_app->SetToolCut( tool->GetName(), "fSystMode", "AtlPdfReweightingTool::kNominal");
		if ( ((TString)tool->ClassName()).Contains("AtlWjetsScaleFactorTool") )
		    task_app->SetToolCut( tool->GetName(), "fProcessMode", "AtlAnalysisTool::kOff");
	    }

	    task_datadir->Add(task_app);
	    
	} else if ( sample->IsMC() ) {
	    // -------------------
	    //  MC specific stuff
	    // -------------------

	    // Turn QCD Tool off (if present)
	    TIter next_tool(fListOfTools);
	    TObject *tool = 0;
	    while ( (tool = next_tool()) ) {
		if ( ((TString)tool->ClassName()).Contains("AtlQCDMMTool") )
		    task_app->SetToolCut( tool->GetName(), "fProcessMode", "AtlAnalysisTool::kOff");
		
		if ( ((TString)tool->ClassName()).Contains("AtlWjetsScaleFactorTool") && !sample->IsWjets() )
		    task_app->SetToolCut( tool->GetName(), "fProcessMode", "AtlAnalysisTool::kOff");

		if ( Systematic == kPdf_up ||
		     Systematic == kPdf_down ) {
		    // Needed for PdfReweightingTool
		    // -----------------------------
		    // ToDo: Update Sample-List with DSIDs and
		    //       add a dm to AtlSample. Needed since
		    //       LMU does not have dataset names
		    //
		    TString *path = new TString(sample->GetPathName());
		    TObjArray *tok = path->Tokenize(".");
		    TString dsid = ((TObjString*) tok->At(3))->GetString();
		    
		    if ( ((TString)tool->ClassName()).Contains("AtlPdfReweightingTool") ) {
			task_app->SetToolCut( tool->GetName(), "fDSID", dsid.Data());
			if ( Systematic == kPdf_up ) 
			    task_app->SetToolCut( tool->GetName(), "fSystMode", "AtlPdfReweightingTool::kUp");
			if ( Systematic == kPdf_down ) 
			    task_app->SetToolCut( tool->GetName(), "fSystMode", "AtlPdfReweightingTool::kDown");
		    }
		    delete path;
		    delete tok;
		}
	    }

            // In case of template systematic, create a special task and break the sample loop
            if ( strcmp("TEMPLATE", GetSystematicName(Systematic)) == 0 ) {
                ParentTask->Add(task_app);
                break;
            }

	    TTask *t = (TTask*) (task_mcdir->GetListOfTasks())->FindObject(sample->GetCategoryName());
	    if ( t == 0 ) {
		// Folder does not exist, create it
		t = new TTask(sample->GetCategoryName(),
			      sample->GetCategoryName());
		task_mcdir->Add(t);
	    }
	    t->Add(task_app);
		
	} else if ( sample->IsQCD() ) {
	    // --------------------
	    //  QCD specific stuff
	    // --------------------

	    // At least one qcd tool must be present
            // If the Anti-Muon or Jet-Lepton method is used, turn off any tool
	    TIter next_tool(fListOfTools);
	    TObject *tool = 0;
	    Bool_t qcdtool = kFALSE;
            if ( sample->IsQCDJetLepton() || sample->IsQCDAntiMuon() ) 
                qcdtool = kTRUE;
            while ( (tool = next_tool()) ) {
		if ( ((TString)tool->ClassName()).Contains("AtlQCDMMTool") ) {
		    qcdtool = kTRUE;
                    if ( sample->IsQCDJetLepton() || sample->IsQCDAntiMuon() )
                        task_app->SetToolCut( tool->GetName(), "fProcessMode", "AtlAnalysisTool::kOff");
                }
		// Do not turn off, but choose Nominal Mode
		if ( ((TString)tool->ClassName()).Contains("AtlPdfReweightingTool") )
		    task_app->SetToolCut( tool->GetName(), "fSystMode", "AtlPdfReweightingTool::kNominal");
		if ( ((TString)tool->ClassName()).Contains("AtlWjetsScaleFactorTool") )
		    task_app->SetToolCut( tool->GetName(), "fProcessMode", "AtlAnalysisTool::kOff");
	    }
	    
	    if ( !qcdtool ) {
		Error("BuildAnalysisTasks","No QCD Tool present. Abort!");
		gSystem->Abort(0);
	    }		
	    
	    task_qcddir->Add(task_app);
	}
    }
}

//____________________________________________________________________

void AtlTopLevelAnalysis::AddLeptonChannel(AtlSelector::ELepChannel Channel) {
    //
    // Add lepton channel (e, mu) to the analysis
    //
    fLepChannels += Channel;
}

//____________________________________________________________________

void AtlTopLevelAnalysis::AddJetMultiplicity(AtlSelector::EJetMult JetMult) {
    //
    // Add jet bin to the analysis
    //
    fJetMults += JetMult;
}

//____________________________________________________________________

void AtlTopLevelAnalysis::AddSystematic(ESystematic Systematic) {
    //
    // Add systematic to the analysis
    //
    fSystematics->SetBitNumber(Systematic);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::AddSystematicGroup(ESystematicGroup SystematicGrp) {
    //
    // Add systematic to the analysis
    //
    if ( SystematicGrp & kSystGrpNominal ) {
	AddSystematic(kNOMINAL);
    }
    if ( SystematicGrp & kSystGrpJES ) {
	AddSystematic(kJES_DOWN);
	AddSystematic(kJES_UP);
    }
    if ( SystematicGrp & kSystGrpDefaultSyst ) {
	AddSystematic(kJEFF);
	AddSystematic(kJER);
	AddSystematic(kJVF_DOWN);
	AddSystematic(kJVF_UP);
	AddSystematic(kEER_DOWN);
	AddSystematic(kEER_UP);
	AddSystematic(kEES_DOWN);
	AddSystematic(kEES_UP);
	AddSystematic(kMUID_RES);
	AddSystematic(kMUMS_RES);
	AddSystematic(kMUSC_DOWN);
	AddSystematic(kMUSC_UP);
	AddSystematic(kRES_SOFT_DOWN);
	AddSystematic(kRES_SOFT_UP);
	AddSystematic(kSC_SOFT_DOWN);
	AddSystematic(kSC_SOFT_UP);
    }
    if ( SystematicGrp & kSystGrpScaleFactors ) {
	AddSystematic(kLEP_RECO_SF_DOWN);
	AddSystematic(kLEP_RECO_SF_UP);
	AddSystematic(kLEP_ID_SF_DOWN);
	AddSystematic(kLEP_ID_SF_UP);
	AddSystematic(kLEP_TRIG_SF_DOWN);
	AddSystematic(kLEP_TRIG_SF_UP);
	AddSystematic(kBTAGSF_DOWN);
	AddSystematic(kBTAGSF_UP);
	AddSystematic(kCTAGSF_DOWN);
	AddSystematic(kCTAGSF_UP);
	AddSystematic(kMISTAGSF_DOWN);
	AddSystematic(kMISTAGSF_UP);
    }
    if ( SystematicGrp & kSystGrpJESCompOnly ) {
	AddSystematic(kBJES_DOWN);
	AddSystematic(kBJES_UP);
	AddSystematic(kETAINTERCALIB_MODEL_DOWN);
	AddSystematic(kETAINTERCALIB_MODEL_UP);
	AddSystematic(kETAINTERCALIB_TOTALSTAT_DOWN);
	AddSystematic(kETAINTERCALIB_TOTALSTAT_UP);
	AddSystematic(kJESEFF_DET1_DOWN);
	AddSystematic(kJESEFF_DET1_UP);
	AddSystematic(kJESEFF_DET2_DOWN);
	AddSystematic(kJESEFF_DET2_UP);
	AddSystematic(kJESEFF_DET3_DOWN);
	AddSystematic(kJESEFF_DET3_UP);
	AddSystematic(kJESEFF_MIX1_DOWN);
	AddSystematic(kJESEFF_MIX1_UP);
	AddSystematic(kJESEFF_MIX2_DOWN);
	AddSystematic(kJESEFF_MIX2_UP);
	AddSystematic(kJESEFF_MIX3_DOWN);
	AddSystematic(kJESEFF_MIX3_UP);
	AddSystematic(kJESEFF_MIX4_DOWN);
	AddSystematic(kJESEFF_MIX4_UP);
	AddSystematic(kJESEFF_MODEL1_DOWN);
	AddSystematic(kJESEFF_MODEL1_UP);
	AddSystematic(kJESEFF_MODEL2_DOWN);
	AddSystematic(kJESEFF_MODEL2_UP);
	AddSystematic(kJESEFF_MODEL3_DOWN);
	AddSystematic(kJESEFF_MODEL3_UP);
	AddSystematic(kJESEFF_MODEL4_DOWN);
	AddSystematic(kJESEFF_MODEL4_UP);
	AddSystematic(kJESEFF_STAT1_DOWN);
	AddSystematic(kJESEFF_STAT1_UP);
	AddSystematic(kJESEFF_STAT2_DOWN);
	AddSystematic(kJESEFF_STAT2_UP);
	AddSystematic(kJESEFF_STAT3_DOWN);
	AddSystematic(kJESEFF_STAT3_UP);
	AddSystematic(kJESEFF_STAT4_DOWN);
	AddSystematic(kJESEFF_STAT4_UP);
	AddSystematic(kJESPILEUP_OFFMU_DOWN);
	AddSystematic(kJESPILEUP_OFFMU_UP);
	AddSystematic(kJESPILEUP_OFFNPV_DOWN);
	AddSystematic(kJESPILEUP_OFFNPV_UP);
	AddSystematic(kJESPILEUP_PT_DOWN);
	AddSystematic(kJESPILEUP_PT_UP);
	AddSystematic(kJESPILEUP_RHO_DOWN);
	AddSystematic(kJESPILEUP_RHO_UP);
	AddSystematic(kSINGLEPART_DOWN);
	AddSystematic(kSINGLEPART_UP);
	AddSystematic(kFLAVOR_COMP_DOWN);
	AddSystematic(kFLAVOR_COMP_UP);
	AddSystematic(kFLAVOR_RESP_DOWN);
	AddSystematic(kFLAVOR_RESP_UP);
	AddSystematic(kPUNCH_THROUGH_DOWN);
	AddSystematic(kPUNCH_THROUGH_UP);
    }
    if ( SystematicGrp & kSystGrpBTagEV ) {
	AddSystematic(kBTAGSF_UP_BREAK0);
	AddSystematic(kBTAGSF_UP_BREAK1);
	AddSystematic(kBTAGSF_UP_BREAK2);
	AddSystematic(kBTAGSF_UP_BREAK3);
	AddSystematic(kBTAGSF_UP_BREAK4);
	AddSystematic(kBTAGSF_UP_BREAK5);
	AddSystematic(kBTAGSF_UP_BREAK6);
	AddSystematic(kBTAGSF_UP_BREAK7);
	AddSystematic(kBTAGSF_UP_BREAK8);
	//
	AddSystematic(kBTAGSF_DOWN_BREAK0);
	AddSystematic(kBTAGSF_DOWN_BREAK1);
	AddSystematic(kBTAGSF_DOWN_BREAK2);
	AddSystematic(kBTAGSF_DOWN_BREAK3);
	AddSystematic(kBTAGSF_DOWN_BREAK4);
	AddSystematic(kBTAGSF_DOWN_BREAK5);
	AddSystematic(kBTAGSF_DOWN_BREAK6);
	AddSystematic(kBTAGSF_DOWN_BREAK7);
	AddSystematic(kBTAGSF_DOWN_BREAK8);
	//
	AddSystematic(kCTAGSF_UP_BREAK0);
	AddSystematic(kCTAGSF_UP_BREAK1);
	AddSystematic(kCTAGSF_UP_BREAK2);
	AddSystematic(kCTAGSF_UP_BREAK3);
	AddSystematic(kCTAGSF_UP_BREAK4);
	AddSystematic(kCTAGSF_UP_BREAK5);
	AddSystematic(kCTAGSF_UP_BREAK6);
	//
	AddSystematic(kCTAGSF_DOWN_BREAK0);
	AddSystematic(kCTAGSF_DOWN_BREAK1);
	AddSystematic(kCTAGSF_DOWN_BREAK2);
	AddSystematic(kCTAGSF_DOWN_BREAK3);
	AddSystematic(kCTAGSF_DOWN_BREAK4);
	AddSystematic(kCTAGSF_DOWN_BREAK5);
	AddSystematic(kCTAGSF_DOWN_BREAK6);
	//
	AddSystematic(kMISTAGSF_UP_BREAK0);
	AddSystematic(kMISTAGSF_UP_BREAK1);
	AddSystematic(kMISTAGSF_UP_BREAK2);
	AddSystematic(kMISTAGSF_UP_BREAK3);
	AddSystematic(kMISTAGSF_UP_BREAK4);
	AddSystematic(kMISTAGSF_UP_BREAK5);
	AddSystematic(kMISTAGSF_UP_BREAK6);
	AddSystematic(kMISTAGSF_UP_BREAK7);
	AddSystematic(kMISTAGSF_UP_BREAK8);
	AddSystematic(kMISTAGSF_UP_BREAK9);
	AddSystematic(kMISTAGSF_UP_BREAK10);
	AddSystematic(kMISTAGSF_UP_BREAK11);
	//
	AddSystematic(kMISTAGSF_DOWN_BREAK0);
	AddSystematic(kMISTAGSF_DOWN_BREAK1);
	AddSystematic(kMISTAGSF_DOWN_BREAK2);
	AddSystematic(kMISTAGSF_DOWN_BREAK3);
	AddSystematic(kMISTAGSF_DOWN_BREAK4);
	AddSystematic(kMISTAGSF_DOWN_BREAK5);
	AddSystematic(kMISTAGSF_DOWN_BREAK6);
	AddSystematic(kMISTAGSF_DOWN_BREAK7);
	AddSystematic(kMISTAGSF_DOWN_BREAK8);
	AddSystematic(kMISTAGSF_DOWN_BREAK9);
	AddSystematic(kMISTAGSF_DOWN_BREAK10);
	AddSystematic(kMISTAGSF_DOWN_BREAK11);
    }
    if ( SystematicGrp & kSystGrpSampleSyst ) {
	AddSystematic(kWtDS);
 	AddSystematic(kMCatNLO_wt);
	AddSystematic(kMCatNLO_ttbar);
	AddSystematic(kaMCatNLO_tchan);
	AddSystematic(kaMCatNLO_schan);
	// AddSystematic(kISRFSR_ttbar_More);
	// AddSystematic(kISRFSR_ttbar_Less);
	AddSystematic(kRadHi_ttbar);
	AddSystematic(kRadLo_ttbar);
	AddSystematic(kScaleVar1);
	AddSystematic(kScaleVar2);
	AddSystematic(kScaleVar3);
	AddSystematic(kScaleVar4);
	AddSystematic(kScaleVar5);
	AddSystematic(kScaleVar6);
	// AddSystematic(kScaleVar_tchan1);
	// AddSystematic(kScaleVar_tchan2);
	// AddSystematic(kScaleVar_tchan3);
	// AddSystematic(kScaleVar_tchan4);
    }
    if ( SystematicGrp & kSystGrpMultiLeptonFakes ) {
	AddSystematic(kLEP_FAKE_BDECAYGEN_SHAPE);
    }
		      
}

//____________________________________________________________________

void AtlTopLevelAnalysis::PrintLeptonChannelNames() const {
    //
    // Print name(s) of set lepton channels
    //
    for ( Int_t i = 0; i < AtlSelector::fgNumLepChannels; i++ ) {
	if ( fLepChannels & 0x1 << i ) cout << " " <<  AtlSelector::GetLeptonLabel(i);
    }
}

//____________________________________________________________________

void AtlTopLevelAnalysis::PrintJetMultiplicityNames() const {
    //
    // Print name(s) of set jet multiplicity bins
    //
    for ( Int_t i = 0; i < AtlSelector::fgNumJetMults; i++ ) {
	if ( fJetMults & 0x1 << i ) cout << " " << AtlSelector::GetJetMultLabel(i);
    }
}

//____________________________________________________________________

void AtlTopLevelAnalysis::PrintSystematicNames() const {
    //
    // Print name(s) of set systematics
    //
    for ( Int_t i = 0; i < fgNumSystematics; i++ ) {
	if ( fSystematics->TestBitNumber(i) ) cout << " " << fgSystematicNames[i];
    }
    for ( TIter next(fSelectedVariations); TObject * handle = next(); ) {
        cout << " " << handle->GetName();
    }
}

//____________________________________________________________________

void AtlTopLevelAnalysis::ReadSampleList(const char* basedir) {
    //
    // Read list of samples from text file.
    // Commented (#) or empty lines are ignored
    // If fGridSampleList is set, create a list for grid submission
    //
    TString line;
    TString *SampleListFile = new TString(basedir);
    if ( !SampleListFile->IsNull() ) {
	// Add a "/" if basedir is not empty
	SampleListFile->Append("/");
	// Now remove any "//"
	SampleListFile->ReplaceAll("//", "/");
    }

    TString campaignPrefix;
    Int_t campaign = GetCampaign();
    campaignPrefix = Form("mc%u_", campaign);

    // Build sample list
    // Name: 'SampleList_as/ms_vX_DSIDList.txt'
    //   new cluster as1Xb gets vs instead of as/ms
    if ( fDSIDList == 0 ) {
	Error("ReadSampleList",
	      "DSIDList is not set. Please SetDSIDList(..)!\nAbort!");
	gSystem->Abort(0);
    }
    Info(__FUNCTION__, "Read sample list %s", fDSIDList->Data());

    if ( fHostname->Contains("lx") ) {
	SampleListFile->Append(Form("SampleList_%s%s_%s",
				    campaignPrefix.Data(),
				    fSgTopVersion->Data(),
				    gSystem->BaseName(fDSIDList->Data())));
    } else {
	if ( !fHostname->Contains("as0") && !fHostname->Contains("ms") &&
             !fHostname->Contains("as1") && !fHostname->Contains("as2") ) {
	    Error("ReadSampleList",
		  "Your are not on as/ms machines and can not access the input files!\nAbort!");
	    gSystem->Abort(0);
	}

	SampleListFile->Append(Form("SampleList_%s_%s%s_%s",
				    ((fHostname->Contains("as0") || fHostname->Contains("as2")) ? "as" :
                                     ((fHostname->Contains("as1")) ? "vs" : "ms")),
				    campaignPrefix.Data(),
				    fSgTopVersion->Data(),
				    gSystem->BaseName(fDSIDList->Data())));
    }

    // make sure that "normal" i.e. local list is not overwritten
    if ( !fGridSampleList.IsNull() ) {
        TString end = (*SampleListFile)(TRegexp("[.][a-zA-Z0-9]+$"));
        if ( !end.IsNull() ) {
            TString newend = TString("_grid").Append(end);
            SampleListFile->ReplaceAll(end.Data(), newend.Data());
        } else {
            SampleListFile->Append("_grid");
        }
    }

    ifstream SampleList(SampleListFile->Data());

    if ( ! fHostname->Contains("lx") ) {
	if ( ! SampleList ) { // if file does not exist, create it
	    // Check Additional Parameters
	    if ( fSgTopVersion == 0 ) {
		Error("ReadSampleList",
		      "SampleList does not yet exist. Please SetSgTopVersion(..)!\nAbort!");
		gSystem->Abort(1);
	    }
            TString libAtlAnalysis(gSystem->Getenv("LIBATLASANALYSIS"));
            if ( libAtlAnalysis.IsNull() ) {
                Error(__FUNCTION__, "Environment variable 'LIBATLASANALYSIS' not set. Abort!");
                gSystem->Abort();
            }
            TString command_subpath;
            if ( fGridSampleList.IsNull() ) {
                command_subpath = TString("/utils/GenerateSampleList.sh");
            } else {
              command_subpath = TString("/utils/GenerateSampleList_grid.py");
            }
            TString command_path = libAtlAnalysis + command_subpath;
            if ( gSystem->Exec(Form("[ -e %s ]", command_path.Data())) ) {
                Error(__FUNCTION__, "Could not find %s. Abort!", command_path.Data());
                gSystem->Abort();
            }

            TString crossSectionFilePath(fCrossSectionFilePath);
            if ( crossSectionFilePath.IsNull() ) {
                cout << campaign << endl;
                cout << crossSectionFilePath.Data() << endl;
                Error(__FUNCTION__, "path to cross-section file not set");
                gSystem->Abort(0);
            }

            TString gen_cmd(Form("%s %s %s %s %s %s mc%u",
                                 command_path.Data(),
                                 SampleListFile->Data(),
                                 fHostname->Data(),
                                 fSgTopVersion->Data(),
                                 fDSIDList->Data(),
                                 crossSectionFilePath.Data(),
                                 campaign));
            if ( !fGridSampleList.IsNull() ) {
                gen_cmd.Append(" ");
                gen_cmd.Append(fGridSampleList);
            }

            Info(__FUNCTION__, "Executing command: %s", gen_cmd.Data());
	    Int_t status = gSystem->Exec(gen_cmd.Data());
	    if ( status == 0 ) {
		ReadSampleList(basedir);
	    } else {
		Error("ReadSampleList", "SampleList generation was not successful. Abort!");
		gSystem->Abort(1);
	    }
        }
    }
    AtlSample * lastSample = 0;
    Info(__FUNCTION__, "Found sample list %s", SampleListFile->Data());
    while ( !SampleList.eof() ) {
	line.ReadLine(SampleList);
	if ( line.BeginsWith("#") || line.Length() == 0 ) continue;
        std::unique_ptr<TObjArray> tokens(line.Tokenize(" "));
	TString name  = ((TObjString*)tokens->At(0))->GetString();
	TString type  = ((TObjString*)tokens->At(1))->GetString();
	Float_t xsec  = ((TObjString*)tokens->At(2))->GetString().Atof();
	TString color = ((TObjString*)tokens->At(3))->GetString();
	TString path  = ((TObjString*)tokens->At(4))->GetString();

        if ( name == "+" ) {
            // ... indicating that a path shall be appended to the sample.
            if ( !lastSample ) {
                Fatal(__FUNCTION__, "Invalid sample continuation line in SampleList.");
            }
            for ( int i = 1, n = tokens->GetEntries() - 1; i < n; ++i ) {
                if ( static_cast<TObjString const *>(tokens->At(i))->GetString() != "+" ) {
                    Fatal(__FUNCTION__, "Invalid sample continuation line in SampleList.");
                    // ... just to avoid confusion. 
                }
            }
            lastSample->AddPath(path);
            continue;
        }
	lastSample = AddSample(name.Data(), "", color.Data(),
		  (AtlSample::EType)AtlSample::ReadType(type.Data()),
		  xsec, path.Data());
    }
    SampleList.close();
}

//____________________________________________________________________

void AtlTopLevelAnalysis::AddUserEnv(const char* env) {
    //
    // Add user-defined environment variable for shared library search
    // paths
    //
    fListOfUserEnvs->Add(new TNamed(env, ""));
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetCut(const char* var, const char* val) {
    //
    // Set cut value for analysis selector.
    //
    // The variable name must be a valid data member of the underlying
    // analysis selector class (see AtlSelector). This function can
    // also be used in order to change the value for an already
    // existing cut
    //

    // Cut already in list ? Otherwise add new object
    AtlCut *item = (AtlCut*)fListOfCuts->FindObject(var);
    if ( item != 0 ) {
	item->SetVal(val);
    } else {
	fListOfCuts->Add(new AtlCut(var, val));
    }
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetToolCut(const char* tool, const char* var,
				     const char* val) {
    //
    // Set cut value for an A++ selector tool.
    //
    // The variable name must be a valid data member of the given A++
    // analysis tool class (see AtlAnalysisTool). This function can
    // also be used in order to change the value for an already
    // existing cut
    //

    // Cut already in list ? Otherwise add new object
    AtlToolCut *item = FindToolCut(tool, var);
    if ( item != 0 ) {
	item->SetVal(val);
    } else {
	fListOfToolCuts->Add(new AtlToolCut(tool, var, val));
    }
}

//____________________________________________________________________

AtlToolCut* AtlTopLevelAnalysis::FindToolCut(const char* tool,
					     const char* var) {
    //
    // Find A++ analysis tool cut
    //
    TString name(tool);
    name.Append(";;");
    name.Append(var);
    return (AtlToolCut*)fListOfToolCuts->FindObject(name.Data());
}

//____________________________________________________________________

AtlCut* AtlTopLevelAnalysis::FindCut(const char* var) {
    //
    // Find A++ analysis selector cut
    //
    return (AtlCut*)fListOfCuts->FindObject(var);
}

//____________________________________________________________________

TString* AtlTopLevelAnalysis::BuildOutputPath(TString* dir,
					      const char* ext,
					      Int_t jetbin,
					      Int_t lepchannel,
					      Int_t systematic,
					      const char* sample ) {
    //
    // Build Output path
    //
    // "dir/(ext)/(jetbin)/(lepchannel)/(systematic)
    //
    // WARNING: User has to delete the TString*
    //
    TString* path = new TString( Form("%s/%s/%sJets/%s/%s/%s/",
				      dir->Data(),
				      ext,
				      AtlSelector::GetJetMultLabel(jetbin),
				      AtlSelector::GetLeptonLabel(lepchannel),
				      GetSystematicName(systematic),
				      sample) );
    path->ReplaceAll("//","/");
    return path;
}

//____________________________________________________________________

TString* AtlTopLevelAnalysis::BuildOutputFileName(Int_t jetbin,
						  Int_t lepchannel,
						  Int_t systematic,
						  const char* sample,
						  Bool_t SubJobsAsInput) {
    //
    // Build Output file name
    //  - "hists_jetbin_lepchannel_syst_sample.root"
    //
    // WARNING: User has to delete the TString*
    //

    TString *file = 0;
    if ( SubJobsAsInput ) {
	// If subjobs have been used, use a asterix * to grep them all.
	// User is responsible to not grep merged files from e.g. a merging step
	// (...in this case the option should be false...)
	file = new TString( Form("*%s_%sj_%s_%s_%s*.root",
				 fOutputFilePrefix->Data(),
				 AtlSelector::GetJetMultLabel(jetbin),
				 AtlSelector::GetLeptonLabel(lepchannel),
				 GetSystematicName(systematic),
				 sample) );
    } else {
	file = new TString( Form("%s_%sj_%s_%s_%s.root",
				 fOutputFilePrefix->Data(),
				 AtlSelector::GetJetMultLabel(jetbin),
				 AtlSelector::GetLeptonLabel(lepchannel),
				 GetSystematicName(systematic),
				 sample) );
    }
    return file;
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetDSIDList(const char* list) {
    //
    // Set File for DSID <-> Filesystem match
    //
    if ( fDSIDList != 0 ) delete fDSIDList;
    fDSIDList = new TString(list);
}   

//____________________________________________________________________

void AtlTopLevelAnalysis::SetHistDir(const char* path) {
    //
    // Set Output directory
    //
    if ( fHistDir != 0 ) delete fHistDir;
    fHistDir = new TString(path);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetInputDir(const char* path) {
    //
    // Set input directory (if input is not provided by a sample list)
    //
    if ( fInputDir != 0 ) delete fInputDir;
    fInputDir = new TString(path);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetPlotDir(const char* path) {
    //
    // Set input directory (if input is not provided by a sample list)
    //
    if ( fPlotDir != 0 ) delete fPlotDir;
    fPlotDir = new TString(path);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetSelector(const char* sel) {
    //
    // Set Analysis Selector Name
    //
    if ( fSelector != 0 ) delete fSelector;
    fSelector = new TString(sel);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetSgTopVersion(const char* ver) {
    //
    // Set SgTop minintuple version
    //
    if ( fSgTopVersion != 0 ) delete fSgTopVersion;
    fSgTopVersion = new TString(ver);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetCampaign(Int_t campaign) {
    //
    // Set the campaign
    //
    fCampaign = campaign;
}

//____________________________________________________________________

int AtlTopLevelAnalysis::GetCampaign() const {
    //
    // Get the campaign, failing if unset
    //
    if ( !fCampaign ) {
        Fatal(__FUNCTION__, "campaign not set");
    }
    return fCampaign;
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetCrossSectionFilePath(const char* crossSectionFilePath) {
    //
    // Set the path to the cross-section file
    //
    fCrossSectionFilePath = crossSectionFilePath;
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetJobHomeDir(const char* path) {
    //
    // Set Task directory
    //
    if ( fJobHomeDir != 0 ) delete fJobHomeDir;
    fJobHomeDir = new TString(path);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetTempDir(const char* path) {
    //
    // Set Temp directory directory
    //
    if ( fTempDir != 0 ) delete fTempDir;
    fTempDir = new TString(path);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetTreeName(const char* name) {
    //
    // Set TTree name
    //
    if ( fTreeName != 0 ) delete fTreeName;
    fTreeName = new TString(name);
}

//____________________________________________________________________

const char* AtlTopLevelAnalysis::GetInputFileName(AtlSample *sample, Int_t lep, Int_t syst) {
    return GetInputFileName(sample, lep, syst, -1);
}

//____________________________________________________________________

const char* AtlTopLevelAnalysis::GetInputFileName(AtlSample *sample, Int_t lep, Int_t syst, Int_t pathIndex) {
    //
    // Get suffix for choosing input files
    //
    TString *file = new TString( (pathIndex >= 0 ? sample->GetPath(pathIndex) : sample->GetPathName()) );
    // for grid: remove base path
    if ( fGridJob ) {
        TObjArray* tok = file->Tokenize("/");
        delete file;
        file = new TString(((TObjString*)tok->Last())->GetString());
    }
    // For jes components:
    // Assume that the systematic varied files are in the folder
    // with a "j" appended to the original path.
    if ( IsJESComponentSystematic(syst) ) {
	if ( sample->IsTtbar() ) {
	    file->Insert(file->Sizeof()-5, "j");
	} else {
	    file->Append("j");
	}
	file->Append("/*");
    } else {
	file->Append("/*");
    }
    
    UInt_t lepton = 0x1 << lep;
    if ( GetCampaign() > 13 ) {
        if ( lepton != AtlSelector::kLepton ) {
            Fatal(__FUNCTION__, "Only AtlSelector::kLepton is supported for the mc%u campaign.", GetCampaign());
        }
    } else if ( lepton == AtlSelector::kElectron ) {
	file->Append("el");
    } else if ( lepton == AtlSelector::kMuon ) {
	file->Append("mu");
    } else {
	Error("GetInputFileName", "Lepton not known! Abort!");
	gSystem->Abort(0);
    }

    if ( fInputFormat == kTreePerSyst ) {
	// Input tree is always in the nominal file
	// Files for systematics do not exist anymore

	if ( sample->IsHFOR0() ||
	     sample->IsHFOR1() ||
	     sample->IsHFOR2() ||
	     sample->IsHFOR3() ) {
		Info("GetInputFileName ","%d" ,sample->GetType());
	    Error("GetInputFileName",
		  "HFOR splitting not yet supported. Abort!");
	    gSystem->Abort();
	}
	file->Append(".root*");
	
    } else if ( fInputFormat == kFilePerSyst ) {
	// "v11" mode: one file per systematic
	
	// take nominal files in case of scale factor systematics
	if ( IsScaleFactorSystematic(syst) ||
	     IsBTagEVScaleFactorSystematic(syst) )
	    syst = 0;
	
	if ( syst != 0 ) {
	    file->Append( Form("_%s", 
			       GetSystematicName(syst)));
	}
	// nominal has no files extension
	// (only true for input files)
	// if nominal, do nothing
	
	// Add HFOR string if necessary
	if ( sample->IsHFOR0() ) {
	    file->Append("_hfor0.root*");
	} else if ( sample->IsHFOR1() ) {
	    file->Append("_hfor1.root*");
	} else if ( sample->IsHFOR2() ) {
	    file->Append("_hfor2.root*");
	} else if ( sample->IsHFOR3() ) {
	    file->Append("_hfor3.root*");
	} else {
	    file->Append(".root*");
	}
    } else {
	Error("GetInputFileName",
	      "InputFormat unknown! Abort!");
	gSystem->Abort();
    }

    // Remove glob for grid job
    if ( fGridJob ) {
        file->ReplaceAll("/*.root*", "");
    }
    return file->Data();
}

//____________________________________________________________________

const char* AtlTopLevelAnalysis::GetInputFileNameFromFile(AtlSample *sample,
							  Int_t lep,
							  Int_t syst) {
    //
    // Get suffix for choosing input files from List
    //
    
    TString *file = new TString;
    UInt_t lepton = 0x1 << lep;
    if ( lepton == AtlSelector::kElectron ) {
        file->Append("el");
    } else if ( lepton == AtlSelector::kMuon ) {
        file->Append("mu");
    } else {
        Error("GetInputFileName", "Lepton not known! Abort!");
        gSystem->Abort(0);
    }

    // take nominal files in case of scale factor systematics
    if ( IsScaleFactorSystematic(syst) ||
	 IsBTagEVScaleFactorSystematic(syst) ) 
        syst = 0;

    if ( syst != 0 ) {
        file->Append( Form("_%s",
                           GetSystematicName(syst)));
    }
    // nominal has no files extension
    // (only true for input files)
    // if nominal, do nothing

    // Add HFOR string if necessary
    if ( sample->IsHFOR0() ) {
        file->Append("_hfor0.root");
    } else if ( sample->IsHFOR1() ) {
        file->Append("_hfor1.root");
    } else if ( sample->IsHFOR2() ) {
        file->Append("_hfor2.root");
    } else if ( sample->IsHFOR3() ) {
        file->Append("_hfor3.root");
    } else {
        file->Append(".root");
    }

    return file->Data();
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetOutputTree(const char* name,
                                        const char* title) {
    //
    // Set name and title of the output tree
    //
    if ( fOutputTreeName ==  0 ) {
        fOutputTreeName = new TNamed(name, title);
    } else{
        fOutputTreeName->SetNameTitle(name, title);
    }
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetOutputFilePrefix(const char* prefix) {
    //
    // Set prefix of the output filename
    // (default = "hist")
    //
    delete fOutputFilePrefix;
    fOutputFilePrefix = new TString(prefix);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::BuildPlottingTasks(TTask *ParentTask,
					     Int_t Jetbin,
					     Int_t LepChannel,
					     Int_t Systematic) {
    //
    // Build Plotting Tasks
    //

    
    // ========================
    // Standard plotting tasks
    // ========================
    AtlDataMCPlotterTask *task_plot = new AtlDataMCPlotterTask("plotting",
							       "plotting");
    task_plot->SetDebug(fDebug);
    task_plot->SetDebugBuild(fDebugBuild);

    if ( fPlotDir == 0 ) {
	Error("BuildPlottingTasks",
	      "Please provide the plot directory! Abort!");
	gSystem->Abort(0);
    }

    TString *plotdir = 0;
    if ( fUsePlotterSchemeDir ) {
	plotdir = new TString(fPlotterScheme->Data());
    } else {
	plotdir = new TString("plots_datamc");
    }    
    
    TString *outfile = BuildOutputPath( fPlotDir, "",
					Jetbin, LepChannel,
					Systematic, plotdir->Data() );
    
    TString *outfileName = BuildOutputFileName( Jetbin, LepChannel,
						Systematic, "DataMCPlots" );
    outfile->Append(outfileName->Data());
    outfile->ReplaceAll("//","/");
	
    TString *tmphist = BuildOutputPath( fTempDir, Form("%s/hist", GetName()),
					Jetbin, LepChannel,
					Systematic, plotdir->Data() );
    tmphist->Append(outfileName->Data());
    tmphist->ReplaceAll("//","/");
	
    TString *tempPath = BuildOutputPath( fTempDir, Form("%s/plot", GetName()),
					 Jetbin, LepChannel,
					 Systematic, plotdir->Data() );
    TString *jobHome  = BuildOutputPath( fJobHomeDir, Form("%s/plot", GetName()),
					 Jetbin, LepChannel,
					 Systematic, "" );
    
    task_plot->SetOutputFile( outfile->Data() );
    task_plot->SetTempOutputFile( tmphist->Data() );
    task_plot->SetTempOutputPath( tempPath->Data() );
    task_plot->SetJobHome( jobHome->Data() );	
    task_plot->AddNtuplePlotCmds(fNtuplePlotCmds);

    // Loop over samples to produce the list of plotter samples
    AtlSample *sample = 0;
    TString *ana_outfile;
    TString *ana_outfileName;
    TIter next_sample(fListOfSamples);

    // Check the list of samples if using a sample systematic
    // If we are using a different sample (e.g. for a generator comparision),
    // we need to...
    // a) ... know which type of sample (or channel, e.g. ttbar) should be replaced.
    // b) ... remove all samples that belong to the channel in a) but...
    // c) ... keep the sample for this channel that belongs to the systematic
    AtlSample::EType syst_sample_type = AtlSample::kUndefined;
    if ( IsSampleSystematic(Systematic) ) {
	// Do a) here, b) and c) are dealt within the next while loop
	while ( (sample = (AtlSample*)next_sample()) ) {
	    if ( sample->IsSystematicSample() ) {
		// Check if this sample belongs to the systematic
		// if yes, save the type of the sample
		TString samplename = sample->GetName();
		if ( samplename.Contains(GetSystematicName(Systematic)) ) {
		    syst_sample_type = sample->GetType();
		    cout << sample->GetName() << endl;
		    break;
		}
	    }
	}
	next_sample.Reset();

	// Check if a sample type was found if using these type of systematics
	// Otherwise, the sample list is buggy.
	if ( syst_sample_type == AtlSample::kUndefined ) {
	    Error("BuildPlottingTasks",
		  "Try to plot a systematic which is not in your sample list. Abort!"); 
	    Error("BuildPlottingTasks",
		  "Systematik name: %s",
		  GetSystematicName(Systematic));
	    Error("BuildPlottingTasks",
		  "Undefined systematic sample type. Please check your sample list and/or systematic. Abort!");
	    gSystem->Abort();
	}
    }
    
    Bool_t AddTtbar = kTRUE; // add merged ttbar only once
    // Bool_t AddQCD   = kTRUE; // add merged QCD only once

    /* whether electron and muon channels share the same tree */
    bool mergedLeptonChannels = (GetCampaign() > 13);

    // Initialize a new list
    TList* ListOfPlotterSamples = new TList;
    while ( (sample = (AtlSample*)next_sample()) ) {

	// if ( fSkipData && sample->IsDATA() )
	//     continue; // skip data on request

        // Skip systematics for data and QCD samples
        if ( sample->IsDATA() && Systematic != kNOMINAL ) { continue; }
        if ( sample->IsQCD() && Systematic != kNOMINAL ) { continue; }

	// Skip egamma/muon stream if neccessary
	UInt_t lep = 0x1 << LepChannel;
	if ( sample->IsDATAEGamma() && lep != AtlSelector::kElectron && lep != AtlSelector::kLepton )
	    continue;
	if ( sample->IsDATAMuon()   && lep != AtlSelector::kMuon && lep != AtlSelector::kLepton )
	    continue;
	if ( sample->IsDATAMuon()   && lep == AtlSelector::kLepton )  //Skip either egamma or muon stream for kLepton. Otherwise data stream is read twice.
	    continue;	
	if ( sample->IsQCDEGamma() && lep != AtlSelector::kElectron && lep != AtlSelector::kLepton )
	    continue;
	if ( sample->IsQCDMuon()   && lep != AtlSelector::kMuon && lep != AtlSelector::kLepton ) 
	    continue;

	// Use Hfor Splitted wjets samples only
	if ( IsIgnored(sample) )
	    continue;

	// Split here in
	// I)  Systematic that replace certain samples with other samples
	// II) Nominal and systematics that act on all samples in the same way

	if ( IsSampleSystematic(Systematic) ) {
	    // I) We need to carefully select the correct sample:
	    //    we need to ...
	    // a) ... know which type of sample should be replaced (done above)
	    // b) ... remove all samples that belong to the channel in a) but...
	    // c) ... keep the sample for this channel that belongs to this systematic
	    // d) ... remove all systematic samples which do not belong to this systematic

	    TString samplename = sample->GetName();		 
	    if ( sample->GetType() == syst_sample_type &&                // <-- b)
		 !samplename.Contains(GetSystematicName(Systematic)) ) { // <-- c)
		continue;
	    }

	    // d)
	    if ( sample->IsSystematicSample() &&
		 !samplename.Contains(GetSystematicName(Systematic)) )
		continue;

	    // If the systematic sample has a different type as the sample it
	    // should replace, we need to manually check this here.

	    // This should be done 'systematic-wise', please use
	    // strcmp, since then the check is independent of the
	    // systematic's position within the fgSystematicNames[]

	    // Special case: 'MCatNLO' systematic
	    // This will replace all SgTop samples and all ttbar samples
	    // with the corresponding MCatNLO sample
	    //
	    // WARNING: The assumption is that those sample exist in the
	    //          sample list!
	    // 
	    if ( strcmp("MCatNLO", GetSystematicName(Systematic)) == 0 &&
		 (sample->IsSgTop() || sample->IsTtbar()) &&
		 !samplename.Contains(GetSystematicName(Systematic)) ) {
		continue;
	    }

	    // Special case: 'MCatNLO_SgTop' systematic
	    // This will replace all SgTop samples with the corresponding
	    // MCatNLO sample
	    //
	    // WARNING: The assumption is that those sample exist in the
	    //          sample list!
	    // 
	    if ( strcmp("MCatNLO_SgTop", GetSystematicName(Systematic)) == 0 &&
		 sample->IsSgTop() &&
		 !samplename.Contains(GetSystematicName(Systematic)) ) {
		continue;
	    }

	    // Special case: '(a)MCatNLO_channel' systematic
	    // This will replace only one channel at a time by the corresponding
	    // MCatNLO sample
	    //
	    // WARNING: The assumption is that those sample exist in the
	    //          sample list!
	    // 
	    if ( strcmp("MCatNLO_tchan", GetSystematicName(Systematic)) == 0 &&
		 sample->IsSgTopTChannel() &&
		 !samplename.Contains(GetSystematicName(Systematic)) ) {
		continue;
	    }
	    
	    if ( strcmp("aMCatNLO_tchan", GetSystematicName(Systematic)) == 0 &&
		 sample->IsSgTopTChannel() &&
		 !samplename.Contains(GetSystematicName(Systematic)) ) {
		continue;
	    }

	    if ( strcmp("PowhegPythia_t", GetSystematicName(Systematic)) == 0 &&
		 sample->IsSgTopTChannel() &&
		 !samplename.Contains(GetSystematicName(Systematic)) ) {
		continue;
	    }

	    if ( strcmp("PowhegPythia2011C_t", GetSystematicName(Systematic)) == 0 &&
		 sample->IsSgTopTChannel() &&
		 !samplename.Contains(GetSystematicName(Systematic)) ) {
		continue;
	    }

	    if ( strcmp("PowhegHerwig_t", GetSystematicName(Systematic)) == 0 &&
		 sample->IsSgTopTChannel() &&
		 !samplename.Contains(GetSystematicName(Systematic)) ) {
		continue;
	    }

	    if ( strcmp("MCatNLO_schan", GetSystematicName(Systematic)) == 0 &&
		 sample->IsSgTopSChannel() &&
		 !samplename.Contains(GetSystematicName(Systematic)) ) {
		continue;
	    }
	    
	    // Special case for tchannel scale varations
	    if ( ((strcmp("ScaleVar_tchan1", GetSystematicName(Systematic)) == 0) ||
		  (strcmp("ScaleVar_tchan2", GetSystematicName(Systematic)) == 0) ||
		  (strcmp("ScaleVar_tchan3", GetSystematicName(Systematic)) == 0) ||
		  (strcmp("ScaleVar_tchan4", GetSystematicName(Systematic)) == 0)) &&
		 (sample->IsSgTopTChannel()) &&
		 !samplename.Contains(GetSystematicName(Systematic)) ) {
		continue;
	    }

	    if ( strcmp("WjetsGen", GetSystematicName(Systematic)) == 0 &&
		 sample->IsWjets() &&
		 !samplename.Contains(GetSystematicName(Systematic)) ) {
		continue;
	    }

	} else {
	    // II) We need to remove all samples, that are only used for
	    //     a specific systematic
	    if ( sample->IsSystematicSample() )
		continue;
	}

	// Analysis OutputFileNames
	// ToDo: Replace by a GetAnalysisNames-Fct ???
	// if DATA or QCD take nominal
	if ( sample->IsDATA() )  {
	    ana_outfile = BuildOutputPath( fPlotDir, "",
					   Jetbin, LepChannel,
					   kNOMINAL, "" );
	    ana_outfileName = BuildOutputFileName( Jetbin, LepChannel,
						   kNOMINAL,
						   sample->GetName());
	// } else if ( sample->IsQCD() ) {
	//     // add QCD only once
	//     if ( !AddQCD ) { continue; }

	//     // use the merged file
	//     // (compare "QCD" with BuildMergingTasks(..) !)
	//     ana_outfile = BuildOutputPath( fPlotDir, "",
	// 				   Jetbin, LepChannel,
	// 				   kNOMINAL, "" );
	//     ana_outfileName = BuildOutputFileName( Jetbin, LepChannel,
	// 					   kNOMINAL,
	// 					   "QCD" );
	//     AddQCD = kFALSE;
	} else if ( sample->IsTtbar() &&
		    fTtbarIsSplitted ) {
	    // add ttbar only once
	    if ( !AddTtbar ) { continue; }

	    if ( IsSampleSystematic(Systematic) &&
		 !sample->IsSystematicSample() ) {
		// take nominal for non-systematic samples
		// use the merged file for ttbar
		ana_outfile = BuildOutputPath( fPlotDir, "",
					       Jetbin, LepChannel,
					       kNOMINAL, "" );
		ana_outfileName = BuildOutputFileName( Jetbin, LepChannel,
						       kNOMINAL,
						       "ttbar" );
	    } else {
		// Todo:
		// What happens here for ttbar samples which belong
		// to sample systematics ???
		// Are they merged?

		// use the merged file
		// (compare "ttbar" with BuildMergingTasks(..) !)
		ana_outfile = BuildOutputPath( fPlotDir, "",
					       Jetbin, LepChannel,
					       Systematic, "" );
		ana_outfileName = BuildOutputFileName( Jetbin, LepChannel,
						       Systematic,
						       "ttbar" );
	    }
	    AddTtbar = kFALSE;
	} else {
	    if ( IsSampleSystematic(Systematic) &&
		 !sample->IsSystematicSample() ) {
		// take nominal for all other samples
		ana_outfile = BuildOutputPath( fPlotDir, "",
					       Jetbin, LepChannel,
					       kNOMINAL, "" );
		ana_outfileName = BuildOutputFileName( Jetbin, LepChannel,
						       kNOMINAL,
						       sample->GetName() );
	    } else {
		ana_outfile = BuildOutputPath( fPlotDir, "",
					       Jetbin, LepChannel,
					       Systematic, "" );
		ana_outfileName = BuildOutputFileName( Jetbin, LepChannel,
						       Systematic,
						       sample->GetName() );
	    }
	}
	ana_outfile->Append(ana_outfileName->Data());
    	ana_outfile->ReplaceAll("//","/");
	
	if ( lep != AtlSelector::kLepton || mergedLeptonChannels ) {
	    AtlSample *PlotterSample = new AtlSample(sample->GetName(), "",
						     sample->GetColor(),
						     sample->GetType(),
						     sample->GetXsection(),
						     ana_outfile->Data());
	    ListOfPlotterSamples->Add(PlotterSample);
	} else {
	    // lnu plot: add electron file
	    TString ana_outfile_enu(*ana_outfile);
	    ana_outfile_enu.ReplaceAll(AtlSelector::GetLeptonLabel(AtlSelector::kLepton),
				       AtlSelector::GetLeptonLabel(AtlSelector::kElectron));
	    // in case of muon systematics use nominal electron file
	    if ( Systematic == kMUID_RES || 
		 Systematic == kMUMS_RES ||
		 Systematic == kMUSC_DOWN || Systematic == kMUSC_UP )
		ana_outfile_enu.ReplaceAll(fgSystematicNames[Systematic], fgSystematicNames[kNOMINAL]);
	    AtlSample *PlotterSample = new AtlSample(sample->GetName(), "",
						     sample->GetColor(),
						     sample->GetType(),
						     sample->GetXsection(),
						     ana_outfile_enu.Data());
	    ListOfPlotterSamples->Add(PlotterSample);
	    
	    // lnu plot: add muon file
	    TString ana_outfile_munu(*ana_outfile);
	    ana_outfile_munu.ReplaceAll(AtlSelector::GetLeptonLabel(AtlSelector::kLepton),
					AtlSelector::GetLeptonLabel(AtlSelector::kMuon));
	    // in case of electron systematics use nominal muon file
	    if ( Systematic == kEER_DOWN || Systematic == kEER_UP || 
		 Systematic == kEES_DOWN || Systematic == kEES_UP )
		ana_outfile_munu.ReplaceAll(fgSystematicNames[Systematic], fgSystematicNames[kNOMINAL]);
	    PlotterSample = new AtlSample(sample->GetName(), "",
					  sample->GetColor(),
					  sample->GetType(),
					  sample->GetXsection(),
					  ana_outfile_munu.Data());
	    ListOfPlotterSamples->Add(PlotterSample);
	}
	
	delete ana_outfile;
	delete ana_outfileName;
    }

    task_plot->AddFolders(GetPlottingFolders(fPlotterScheme->Data(),
					       ListOfPlotterSamples));
    task_plot->SetDataLuminosity(fDataLuminosity);
    task_plot->SetAtlasLabelStatus(fAtlasLabelStatus->Data());
    task_plot->SetCenterOfMassEnergyLabel(fCenterOfMassEnergyLabel->Data());
    task_plot->SetLuminosityLabel(fLuminosityLabel->Data());
    task_plot->SetDrawDataZeroEntryErrors(fDrawDataZeroEntryErrors);
    task_plot->SetUseOverflowUnderflow(fUseOverflowUnderflow);
    task_plot->SetDrawMCError(fDrawMCError);
    task_plot->SetExportPDF(fExportPDF);
    task_plot->SetDrawSignalOverlay(fDrawSignalOverlay);
    task_plot->SetScaleOverlay(fScaleOverlay);
    task_plot->SetDrawSignificance(fDrawSignificance);
    
    // clean up TString
    delete outfile;
    delete outfileName;
    delete tmphist;
    delete tempPath;
    delete jobHome;
    // clean up ListOfPlotters
    // don't delete content, it is used by the task!
    // The task will delete the list and it's contents
    delete ListOfPlotterSamples;
    delete plotdir;
    
    ParentTask->Add(task_plot);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::BuildMemTkAnalysisTasks(TTask *ParentTask,
						  Int_t Jetbin,
						  Int_t LepChannel,
						  Int_t Systematic) {
    //
    // Build MemTk Analysis Tasks
    //

    // Check if Input Directory is defined
    if ( fInputDir == 0 ) {
	Error("BuildMemTkAnalysisTasks",
	      "Please provide an input directory! Abort!");
	gSystem->Abort(0);
    }

    // =============================
    // Standard MemTk Analysis tasks
    // =============================
    AtlSample *sample = 0;
    TIter next_sample(fListOfSamples);
    AtlMemTkAnalysisTask *task_app = 0;

    // ===================
    // Standard DATA tasks
    // ===================
    TTask *task_datadir = new TTask("DATA", "DATA");
    ParentTask->Add(task_datadir);

    // =================
    // Standard MC tasks
    // =================
    TTask *task_mcdir = new TTask("MC", "MC");
    ParentTask->Add(task_mcdir);

    // ==================
    // Standard QCD tasks
    // ==================
    TTask *task_qcddir = new TTask("QCD", "QCD");
    ParentTask->Add(task_qcddir);
    
    // For analysis output filenames
    TString *ana_outfile;
    TString *ana_outfileName;

    while ( (sample = (AtlSample*)next_sample()) ) {
	
	if ( fUseHforSamples )
	    continue;
	    
	// Skip systematics for data samples for faster setup
	if ( sample->IsDATA() && Systematic != kNOMINAL ) { continue; }

	// Skip systematics for qcd samples (for now)
	if ( sample->IsQCD() && Systematic != kNOMINAL ) { continue; }

	// Skip egamma/muon stream if neccessary
	UInt_t lep = 0x1 << LepChannel;
	if ( sample->IsDATAEGamma() && lep == AtlSelector::kMuon ) { continue; }
	if ( sample->IsDATAMuon()   && lep == AtlSelector::kElectron ) { continue; }
	if ( sample->IsQCDEGamma() && lep == AtlSelector::kMuon ) { continue; }
	if ( sample->IsQCDMuon()   && lep == AtlSelector::kElectron ) { continue; }

	// Special treatment for samples only needed for a systematic
	// (e.g. generator systematic)
	// Samples are skipped if the systematic is...
	//  a) a sample systematic
	//     --> Skip all samples, but the one that belongs
	//         to this systematic. As identifier, the systematic
	//         name has to be used.
	//  b) not a sample systematic --> skip all samples that belong to a systematic
	TString samplename = sample->GetName();
	if ( (IsSampleSystematic(Systematic) && // a) skip all nominal files
	      !sample->IsSystematicSample()) || 
	     (IsSampleSystematic(Systematic) && // a) skip all files that do not belong
	      !samplename.Contains(GetSystematicName(Systematic))) || // to this systematic
	     (!IsSampleSystematic(Systematic) && // b) skip all systematic samples
	      sample->IsSystematicSample()) )    //    if using nominal or not a sample systematic
	    continue;
	
	// ---------------------------------------
	//  General Setup for DATA and MC Samples
	// ---------------------------------------
	task_app = new AtlMemTkAnalysisTask(sample->GetName(), GetSystematicName(Systematic)); //sample->GetTitle());
	task_app->SetDebug(fDebug);
	task_app->SetDebugBuild(fDebugBuild);

	TString *outfile = BuildOutputPath( fHistDir, Form("%s/MemTkAnalysis", GetName()),
					    Jetbin, LepChannel,
					    Systematic, "" );
	TString *outfileName = BuildOutputFileName( Jetbin, LepChannel,
						    Systematic, sample->GetName() );

        // add mem identifier
        outfileName->Prepend("mem_");
	outfile->Append(outfileName->Data());
	outfile->ReplaceAll("//","/");
	
	TString *tmphist = BuildOutputPath( fTempDir, Form("%s/MemTkAnalysis", GetName()),
					    Jetbin, LepChannel,
					    Systematic, sample->GetName() );
	tmphist->Append(outfileName->Data());
	tmphist->ReplaceAll("//","/");
	
	TString *tempPath = BuildOutputPath( fTempDir, Form("%s/MemTkAnalysis/analysis", GetName()),
					     Jetbin, LepChannel,
					     Systematic, sample->GetName() );
	TString *jobHome  = BuildOutputPath( fJobHomeDir, Form("%s/MemTkAnalysis/analysis", GetName()),
					     Jetbin, LepChannel,
					     Systematic, sample->GetName() );
	
	task_app->SetOutputFile( outfile->Data() );
	task_app->SetTempOutputFile( tmphist->Data() );
	task_app->SetTempOutputPath( tempPath->Data() );
	task_app->SetJobHome( jobHome->Data() );
    
	// Set grid options (only for single-top and ttbar
        //   except when analysis is also done on grid )
	if ( fGridJob && (fGridAnalysisOnGrid ||
                          sample->IsSgTopTChannel() ||
			  sample->IsSgTopSChannel() ||
			  sample->IsSgTopWt() ||
			  sample->IsDATA() || // t-channel specific config
			  sample->IsQCD() ||
			  sample->IsTtbar())) {
	    task_app->SetGridJob(kTRUE);
	    task_app->SetGridAnalysisOnGrid(fGridAnalysisOnGrid);
            task_app->SetGridRootVersion(fGridRootVersion->Data());
            task_app->SetGridCmtVersion(fGridCmtVersion->Data());
            task_app->SetGridUserName(fGridUserName->Data());
            task_app->SetGridSuffix(fGridSuffix->Data());
            task_app->SetGridIdSuffix(fGridIdSuffix->Data());
            task_app->SetTarBallOnly(fMEMTarBallOnly);
	}
	
	// Analysis OutputFileNames
	// ToDo: Replace by a GetAnalysisNames-Fct ???
	
	// if DATA or QCD take nominal
	if ( sample->IsDATA() )  {
	    ana_outfile = BuildOutputPath( fInputDir, "",
					   Jetbin, LepChannel,
					   kNOMINAL, "" );
	    ana_outfileName = BuildOutputFileName( Jetbin, LepChannel,
						   kNOMINAL,
						   sample->GetName() );
	} else if ( sample->IsQCD() ) {
	    // add QCD only once
	    ana_outfile = BuildOutputPath( fInputDir, "",
					   Jetbin, LepChannel,
					   kNOMINAL, "" );
	    ana_outfileName = BuildOutputFileName( Jetbin, LepChannel,
						   kNOMINAL,
						   sample->GetName() );
	} else {
	    ana_outfile = BuildOutputPath( fInputDir, "",
					   Jetbin, LepChannel,
					   Systematic, "" );
	    ana_outfileName = BuildOutputFileName( Jetbin, LepChannel,
						   Systematic,
						   sample->GetName() );
	}
	ana_outfile->Append(ana_outfileName->Data());
    	ana_outfile->ReplaceAll("//","/");
	
	// ---------------------------------------
	//  Same Task config for MC, DATA and QCD
	// ---------------------------------------

        // Automatic calculation of NSubJobs:
        //   if samples size N is larger than given n, split in k subjobs so that every
        //   subjob runs over less than n events, where k = ceil(N/n)
        Int_t NSubJobsAuto = 0;
        if ( fMaxEventsPerSubjob > 0 ) {
            if ( fSampleSizes == nullptr ) {
                Error(__FUNCTION__, "Sample size database is undefined; run ReadSampleSizeList(...) first!");
                gSystem->Abort(0);
            }
            TNamed * samplesize = (TNamed*)fSampleSizes->FindObject(sample->GetName());
            if ( samplesize == nullptr ) {
                Warning(__FUNCTION__, "Sample %s is not in sample size database! Skip!", sample->GetName());
                // gSystem->Abort(0);
                continue;
            }
            Int_t n_max = fMaxEventsPerSubjob;
            Int_t n_evt = ((TString)samplesize->GetTitle()).Atoi();
            NSubJobsAuto = ((n_evt-1) / n_max) + 1; // ceil division
        }
	
	// Do the splitting here:
        // skip splitting if Auto is chosen and no >=2 subjobs are needed
        Bool_t skip_subjobs = (fMaxEventsPerSubjob > 0) && (NSubJobsAuto < 2);
        // skip subjob splitting if A++ analysis was done on grid
        skip_subjobs |= fGridAnalysisOnGrid;
        if ( !skip_subjobs &&
             ( fMaxEventsPerSubjob > 0
               || sample->IsSgTopTChannel()
               || sample->IsSgTopSChannel()
               || sample->IsSgTopWt()
               || sample->IsTtbar()
               || (sample->GetType() == AtlSample::kWmunuInclB && lep == AtlSelector::kMuon)
               || (sample->GetType() == AtlSample::kWenuInclB  && lep == AtlSelector::kElectron)
               || (sample->GetType() == AtlSample::kWmunuInclC && lep == AtlSelector::kMuon)
               || (sample->GetType() == AtlSample::kWenuInclC  && lep == AtlSelector::kElectron)
               || (sample->GetType() == AtlSample::kWmunuInclLight && lep == AtlSelector::kMuon)
               || (sample->GetType() == AtlSample::kWenuInclLight  && lep == AtlSelector::kElectron)
               || sample->GetType() == AtlSample::kWbbNp1
               || sample->GetType() == AtlSample::kWcNp1
               || ( (fCampaign > 13) && sample->GetType() == AtlSample::kWmunuInclB )
               || ( (fCampaign > 13) && sample->GetType() == AtlSample::kWenuInclB )
               || (sample->IsZjetsB() && fNSubJobsZjetsB != 1)
               || (sample->IsDATA() && fNSubJobsData != 1)
               || (sample->IsQCD() && fNSubJobsQCD != 1) ) ){
	    
	    Int_t Nsubjobs = 1;
	    if ( sample->IsSgTopTChannel() )
		Nsubjobs = fNSubJobsSgTopTChannel;
	    
	    if ( sample->IsSgTopSChannel() )
		Nsubjobs = fNSubJobsSgTopSChannel;

	    if ( sample->IsSgTopWt() )
		Nsubjobs = fNSubJobsSgTopWt;

	    if ( sample->IsTtbar() )
		Nsubjobs = fNSubJobsTtbar;

	    // Wjets Sherpa
	    if ( (sample->GetType() == AtlSample::kWmunuInclB && lep == AtlSelector::kMuon)
		 || (sample->GetType() == AtlSample::kWenuInclB  && lep == AtlSelector::kElectron) )
		Nsubjobs = fNSubJobsWjetsB;

	    if ( (sample->GetType() == AtlSample::kWmunuInclC && lep == AtlSelector::kMuon)
		 || (sample->GetType() == AtlSample::kWenuInclC  && lep == AtlSelector::kElectron) )
		Nsubjobs = fNSubJobsWjetsC;

	    if ( (sample->GetType() == AtlSample::kWmunuInclLight && lep == AtlSelector::kMuon)
		 || (sample->GetType() == AtlSample::kWenuInclLight  && lep == AtlSelector::kElectron) )
		Nsubjobs = fNSubJobsWjetsLight;
	    
	    // Wjets Alpgen
	    if ( sample->GetType() == AtlSample::kWbbNp1 )
		Nsubjobs = fNSubJobsWjetsB;

	    if ( sample->GetType() == AtlSample::kWcNp1 )
		Nsubjobs = fNSubJobsWjetsC;

            // Wjets/Zjets MC15
	    if ( (fCampaign > 13) &&
                 ( (sample->GetType() == AtlSample::kWmunuInclB)
                   || (sample->GetType() == AtlSample::kWenuInclB) ) )
		Nsubjobs = fNSubJobsWjetsB;
            if ( sample->IsZjetsB() )
                Nsubjobs = fNSubJobsZjetsB;
            
	    // DATA
	    if ( sample->IsDATA() )
		Nsubjobs = fNSubJobsData;
	    
	    if ( sample->IsQCD() )
		Nsubjobs = fNSubJobsQCD;
        if(sample->IsQCDJetLepton()){
        	Nsubjobs = 60;
        }
            // overwrite subjob data if automatic splitting is chosen
            if ( NSubJobsAuto > 1 )
                Nsubjobs = NSubJobsAuto;

            Info(__FUNCTION__, "Using %d subjobs for sample %s", Nsubjobs, sample->GetName());
                   
	    TTask *t = 0;

	    if ( sample->IsMC() ) {
		// For MC: Create one folder containing all task of one category
		t = (TTask*) (task_mcdir->GetListOfTasks())->FindObject(sample->GetCategoryName());
		if ( t == 0 ) {
		    t = new TTask(sample->GetCategoryName(),
				  sample->GetCategoryName());
		    task_mcdir->Add(t);
		}
	    } else if ( sample->IsDATA() ) {
		t = task_datadir;
	    } else if ( sample->IsQCD() ) {
		t = task_qcddir;
	    } else {
		Error("BuildMemTkAnalysisTasks",
		      "Unknown sample type. Not MC nor DATA nor QCD! Abort!");
		gSystem->Abort();
	    }
	    
	    // Create one folder containing all task for a given set of subjobs
	    TTask *tsub = (TTask*) (t->GetListOfTasks())->FindObject(task_app->GetName());
	    if ( tsub == 0 ) {
		tsub = new TTask(task_app->GetName(),
				 task_app->GetName());
		t->Add(tsub);
	    }
	    
	    for ( Int_t i = 1; i <= Nsubjobs; i++ ) {
		// modify outputfile names (append i)
		// -outfile
		// -tmphist
		// -temppath
		// -jobhome
		TString name = TString(task_app->GetName());
		if ( Nsubjobs > 1 ) {
                    name.Append(Form("%03d", i));
		}
		AtlMemTkAnalysisTask *task_tmp = (AtlMemTkAnalysisTask*) task_app->Clone(name.Data());
		task_tmp->SetNSubjobs(Nsubjobs);
		    
		TString *outfile2 = new TString(outfile->Data());
		if ( Nsubjobs > 1 ) {
                    outfile2->ReplaceAll(".root", Form("%03d.root", i));
		}

		TString *outfileName2 = new TString(outfileName->Data());
		if ( Nsubjobs > 1 ) {
                    outfileName2->ReplaceAll(".root", Form("%03d.root", i));
		}
		    
		TString *tmphist2 = new TString(tmphist->Data());
                tmphist2->ReplaceAll(".root", Form("%03d.root", i));

		TString *jobHome2 = new TString(jobHome->Data());
                jobHome2->Append(Form("/%03d.root", i));
                jobHome2->ReplaceAll(Form("//%03d.root", i), Form("%03d/", i));

		task_tmp->SetOutputFile( outfile2->Data() );
		task_tmp->SetTempOutputFile( tmphist2->Data() );
		task_tmp->SetTempOutputPath( tempPath->Data() );
		task_tmp->SetJobHome( jobHome2->Data() );
		
		// Set MemTk run script
		if ( fMemTkRunScript == 0 ) {
		    Error("BuildMemTkAnalysisTasks",
			  "No MemTk runscript defined. Abort.");
		    gSystem->Abort();
		}
		task_tmp->SetMemTkRunScript(fMemTkRunScript->Data());
                task_tmp->SetMemTkTreatAllJetsAsBJets(fMemTkTreatAllJetsAsBJets);
                task_tmp->SetD3PDVersion(GetSgTopVersion()->Data());

		// Set grid options (Everything with subjobs is a gridjob)
		if ( fGridJob ) {
		    task_tmp->SetGridJob(kTRUE);
		    task_tmp->SetGridRootVersion(fGridRootVersion->Data());
		    task_tmp->SetGridCmtVersion(fGridCmtVersion->Data());
		    task_tmp->SetGridUserName(fGridUserName->Data());
		    task_tmp->SetGridSuffix(fGridSuffix->Data());
		    task_tmp->SetGridIdSuffix(fGridIdSuffix->Data());
                    task_tmp->SetTarBallOnly(fMEMTarBallOnly);
		}
		    
		task_tmp->AddInputFiles( ana_outfile->Data() );
		task_tmp->AddUserEnvs(fListOfUserEnvs);
		    
		task_tmp->SetJobId(i);

		// skip job if already successful
		if ( GetTaskStatus(jobHome2, outfileName2, outfile2) ) {
		    delete outfile2;
		    delete tmphist2;
		    delete jobHome2;
		    continue;
		} else {
		    delete outfile2;
		    delete tmphist2;
		    delete jobHome2;
		}
		tsub->Add(task_tmp);

		// const char* lepname = (lep == AtlSelector::kElectron ? "e" : "mu");
		// cout << "Adding Task " << task_tmp->GetName() << " " << lepname << " for systematic " << fgSystematicNames[Systematic] << endl;
	    }
	} else {
	    // These are all other MCs, DATA or QCD
	    task_app->AddInputFiles( ana_outfile->Data() );
	    task_app->AddUserEnvs(fListOfUserEnvs);

	    // Set MemTk run script
	    if ( fMemTkRunScript == 0 ) {
		Error("BuildMemTkAnalysisTasks",
		      "No MemTk runscript defined. Abort.");
		gSystem->Abort();
	    }
	    task_app->SetMemTkRunScript(fMemTkRunScript->Data());
            task_app->SetMemTkTreatAllJetsAsBJets(fMemTkTreatAllJetsAsBJets);
            task_app->SetD3PDVersion(GetSgTopVersion()->Data());

	    // skip job if already successful
	    if ( GetTaskStatus(jobHome, outfileName, outfile) ) {
		delete outfile;
		delete outfileName;
		delete tmphist;
		delete tempPath;
		delete jobHome;
		continue;
	    }

	    // const char* lepname = (lep == AtlSelector::kElectron ? "e" : "mu");
	    // cout << "Adding Task " << task_app->GetName() << " " << lepname << " for systematic " << fgSystematicNames[Systematic] << endl;

	    if ( sample->IsDATA() ) {
		task_datadir->Add(task_app);
	    } else if ( sample->IsQCD() ) {
		task_qcddir->Add(task_app);
	    } else if ( sample->IsMC() ) {
		// Assume it's MC
		TTask *t = (TTask*) (task_mcdir->GetListOfTasks())->FindObject(sample->GetCategoryName());
		if ( t == 0 ) {
		    // Folder does not exist, create it
		    t = new TTask(sample->GetCategoryName(),
				  sample->GetCategoryName());
		    task_mcdir->Add(t);
		}
		t->Add(task_app);
	    } else {
		Error("BuildMemTkAnalysisTasks",
		      "Unknown sample type. Not MC nor DATA nor QCD! Abort!");
		gSystem->Abort();
	    }			  
	}
	
	// clean up TString
	delete outfile;
	delete outfileName;
	delete tmphist;
	delete tempPath;
	delete jobHome;
    }
}

//____________________________________________________________________

void AtlTopLevelAnalysis::BuildMemDiscAnalysisTasks(TTask *ParentTask,
						Int_t Jetbin,
						Int_t LepChannel,
						Int_t Systematic) {
    //
    // Build MemDisc Analysis Tasks
    //

    // Check if Input Directory is defined
    if ( fInputDir == 0 ) {
	Error(__FUNCTION__,
	      "Please provide an input directory! Abort!");
	gSystem->Abort(0);
    }

    
    // ===============================
    // Standard MemDisc Analysis tasks
    // ===============================
    AtlSample *sample = 0;
    TIter next_sample(fListOfSamples);
    AtlAppAnalysisTask *task_memdisc = 0;

    // ===================
    // Standard DATA tasks
    // ===================
    TTask *task_datadir = new TTask("DATA", "DATA");
    ParentTask->Add(task_datadir);

    // =================
    // Standard MC tasks
    // =================
    TTask *task_mcdir = new TTask("MC", "MC");
    ParentTask->Add(task_mcdir);

    // ==================
    // Standard QCD tasks
    // ==================
    TTask *task_qcddir = new TTask("QCD", "QCD");
    ParentTask->Add(task_qcddir);
    
    // For analysis output filenames
    TString *ana_outfile;
    TString *ana_outfileName;

    while ( (sample = (AtlSample*)next_sample()) ) {
	
	if ( IsIgnored(sample) )
	    continue;
	    
	// Skip systematics for data samples for faster setup
	if ( sample->IsDATA() && Systematic != kNOMINAL ) { continue; }

	// Skip systematics for qcd samples (for now)
	if ( sample->IsQCD() && Systematic != kNOMINAL ) { continue; }

	// Skip egamma/muon stream if neccessary
	UInt_t lep = 0x1 << LepChannel;
	if ( sample->IsDATAEGamma() && lep == AtlSelector::kMuon ) { continue; }
	if ( sample->IsDATAMuon()   && lep == AtlSelector::kElectron ) { continue; }
	if ( sample->IsQCDEGamma() && lep == AtlSelector::kMuon ) { continue; }
	if ( sample->IsQCDMuon()   && lep == AtlSelector::kElectron ) { continue; }

	// Special treatment for samples only needed for a systematic
	// (e.g. generator systematic)
	// Samples are skipped if the systematic is...
	//  a) a sample systematic
	//     --> Skip all samples, but the one that belongs
	//         to this systematic. As identifier, the systematic
	//         name has to be used.
	//  b) not a sample systematic --> skip all samples that belong to a systematic
	TString samplename = sample->GetName();
	if ( (IsSampleSystematic(Systematic) && // a) skip all nominal files
	      !sample->IsSystematicSample()) || 
	     (IsSampleSystematic(Systematic) && // a) skip all files that do not belong
	      !samplename.Contains(GetSystematicName(Systematic))) || // to this systematic
	     (!IsSampleSystematic(Systematic) && // b) skip all systematic samples
	      sample->IsSystematicSample()) )    //    if using nominal or not a sample systematic
	    continue;

	// ---------------------------------------
	//  General Setup for DATA and MC Samples
	// ---------------------------------------
	
	task_memdisc = new AtlAppAnalysisTask(sample->GetName(), sample->GetTitle());
        task_memdisc->SetBatchNodeAll(kTRUE);
	task_memdisc->SetDebug(fDebug);
	task_memdisc->SetDebugBuild(fDebugBuild);
	task_memdisc->SetNProcessNthEventsOnly( fNProcessNthEventsOnly );
        // Special treatment for MC weight systematics, starting in v29
        // MC weight systematics are named MCWEIGHT_<weight_name>
        // The task has to be given the weight position in the
        //   mc_generator_weights vector and the totalEventsWeighted
        //   from the sumWeights Tree
        // Special handling for fixed weight positions: use MCWEIGHT_FIXEDPOS_<weight_number>
        TString systName = TString(GetSystematicName(Systematic));
        TString weightName = "";
        Int_t EvtReaderMCWeightPosition = -1;
        Double_t EvtReaderMCWeightTotalEvents = -1.;
        if ( systName.BeginsWith("MCWEIGHT_") ) {
            if ( systName.BeginsWith("MCWEIGHT_FIXEDPOS_") ) {
                // position is in systName
                TString systPosString(systName(18, systName.Length() - 18));
                EvtReaderMCWeightPosition = systPosString.Atoi();
            } else {
                // position depends on weight name
                weightName = systName(9, systName.Length() - 9);
                EvtReaderMCWeightPosition = GetMCWeightPosition(samplename.Data(), weightName.Data());
                if ( EvtReaderMCWeightPosition < 0 ) continue;
            }
            // find totalevents by position
            EvtReaderMCWeightTotalEvents = GetMCWeightTotalEvents(samplename.Data(), EvtReaderMCWeightPosition);
            if ( EvtReaderMCWeightTotalEvents < 0 ) continue;
            task_memdisc->SetEvtReaderMCWeightPosition(
                EvtReaderMCWeightPosition);
            task_memdisc->SetEvtReaderMCWeightTotalEvents(
                EvtReaderMCWeightTotalEvents);
        } else if ( systName.BeginsWith("MCWEIGHTS_") ) {
            // deconstruct MCWEIGHTS_N_X_name1Xname2X...XnameN:
            // - get number of systematics and separator
            std::unique_ptr<TObjArray> tokens(systName.Tokenize("_"));
            TString nsyst_str = static_cast<TObjString*>(tokens->At(1))->GetString();
            TString sep = static_cast<TObjString*>(tokens->At(2))->GetString();
            Int_t nsyst = nsyst_str.Atoi();
            if ( nsyst < 2 or nsyst > 9 ) {
                Error(__FUNCTION__, "Using n=%d weights is not supported. Skip!", nsyst);
                continue;
            }
            // - split weights by separator
            Int_t cut = 12 + nsyst_str.Length() + sep.Length();
            TString syst_vec_str(systName(cut, systName.Length()-cut));
            std::unique_ptr<TObjArray> tokens2(syst_vec_str.Tokenize(sep.Data()));
            if ( tokens2->GetEntries() != nsyst ) {
                Error(__FUNCTION__, "Check systematic name. Could not find %d systematics separated by %s in string %s for systematic %s. Skip!",
                      nsyst, sep.Data(), syst_vec_str.Data(), systName.Data());
                continue;
            }
            // - convert weights to names/positions
            Int_t weight_pos = -1;
            Double_t weight_total = -1.;
            Int_t err_code = 0;
            TString weight_pos_vec_str;
            TString weight_total_vec_str;
            for ( Int_t i = 0; i<tokens2->GetEntries(); i++ ) {
                auto obj = tokens2->At(i);
                TString weight_syst_name = static_cast<TObjString*>(obj)->GetString();
                if ( weight_syst_name.BeginsWith("FIXEDPOS_") ) {
                    // support fixed positions here
                    TString systPosString(weight_syst_name(9, weight_syst_name.Length()-9));
                    weight_pos = systPosString.Atoi();
                } else {
                    // if not fixed position, it has to be the weight name
                    weight_pos = GetMCWeightPosition(samplename.Data(), weight_syst_name.Data());
                }
                weight_total = GetMCWeightTotalEvents(samplename.Data(), weight_pos);
                if ( weight_pos < 0 or weight_total < 0 ) {
                    err_code = -1;
                    break;
                }
                // fill into strings to pass to task
                if ( weight_pos_vec_str.IsNull() ) {
                    weight_pos_vec_str = Form("%d", weight_pos);
                    weight_total_vec_str = Form("%g", weight_total);
                } else {
                    weight_pos_vec_str.Append(Form("_%d",weight_pos));
                    weight_total_vec_str.Append(Form("_%g", weight_total));
                }
            }
            if ( err_code != 0 ) {
                Error(__FUNCTION__, "Could not get weights from systematic string %s (error %d). Skip!",
                      systName.Data(), err_code);
                continue;
            }
            // - pass positions and totals to task
            task_memdisc->SetEvtReaderMCWeightPosition(-nsyst);
            Double_t total_events_nominal =  GetMCWeightTotalEvents(samplename.Data(), 0);
            task_memdisc->SetEvtReaderMCWeightTotalEvents(total_events_nominal);
            task_memdisc->SetEvtReaderMCWeightPositionString(weight_pos_vec_str);
            task_memdisc->SetEvtReaderMCWeightTotalEventsString(weight_total_vec_str);
        }

        // Set output paths
	TString *outfile = BuildOutputPath( fPlotDir, "",
					    Jetbin, LepChannel,
					    Systematic, "" );
	TString *outfileName = BuildOutputFileName( Jetbin, LepChannel,
						    Systematic, sample->GetName() );
	outfile->Append(outfileName->Data());
	outfile->ReplaceAll("//","/");
	
	TString *tmphist = BuildOutputPath( fTempDir, Form("%s/%s",
							   GetName(),
							   gSystem->BaseName(fPlotDir->Data())),
					    Jetbin, LepChannel,
					    Systematic, sample->GetName() );
	tmphist->Append(outfileName->Data());
	tmphist->ReplaceAll("//","/");
	
	TString *tempPath = BuildOutputPath( fTempDir, Form("%s/%s/analysis",
							    GetName(),
							    gSystem->BaseName(fPlotDir->Data())),
					     Jetbin, LepChannel,
					     Systematic, sample->GetName() );
	TString *jobHome  = BuildOutputPath( fJobHomeDir, Form("%s/%s/analysis",
							       GetName(),
							       gSystem->BaseName(fPlotDir->Data())),
					     Jetbin, LepChannel,
					     Systematic, sample->GetName() );
	
	task_memdisc->SetOutputFile( outfile->Data() );
	task_memdisc->SetTempOutputFile( tmphist->Data() );
	task_memdisc->SetTempOutputPath( tempPath->Data() );
	task_memdisc->SetJobHome( jobHome->Data() );

	//  Taken from BuildAnalysisTasks
        if ( fInputFormat == kTreePerSyst ) {
	    if ( GetCampaign() > 13 ) {
	        // the exact tree name is computed by the reader
                task_memdisc->SetInputTreeName("");
	    } else {
                // behavior for run-1: only fTreeName->Data()
                task_memdisc->SetInputTreeName(fTreeName->Data());
	    }
	} else if ( fInputFormat == kFilePerSyst ) {
	    task_memdisc->SetInputTreeName(fTreeName->Data());
	} else {
	    Error(__FUNCTION__,
		  "InputFormat not known! Abort!");
	    gSystem->Abort();
	}

	task_memdisc->SetSelector(fSelector->Data());
	task_memdisc->SetCtrlPlots(kFALSE);

	if ( fOutputTreeName != 0 ) {
	    task_memdisc->SetOutputTree(fOutputTreeName->GetName(),
				    fOutputTreeName->GetTitle());
	}

	TIter next_tool(fListOfTools);
	TObject *tool = 0;
	
	UInt_t jetmult = 0x1 << Jetbin;
	UInt_t lepton  = 0x1 << LepChannel;

	while ( (tool = next_tool()) ) {
	    if ( ((TString)tool->ClassName()).Contains("AtlObjectsToolD3PDSgTop") ) {
		task_memdisc->SetToolCut( tool->GetName(), "fJetMults", AtlSelector::GetJetMultEnum(jetmult) );
		task_memdisc->SetToolCut( tool->GetName(), "fLepChannel", AtlSelector::GetLeptonEnum(lepton) );
	    }
	    if ( ((TString)tool->ClassName()).Contains("AtlObjRecoScaleFactorTool") ) {
		task_memdisc->SetToolCut( tool->GetName(), "fSystematicMode", GetSystematicEnum(Systematic) );
		task_memdisc->SetToolCut( tool->GetName(),
				      "fXsecFile",
				      Form("\"%s\"", fCrossSectionFilePath.Data()) );
	    }
	}
	
	// Add config to Analysis Task
        if ( GetCampaign() > 13 ) {
            // set sample type for event reader
            TString reader;
            if ( sample->IsQCDJetLepton() ) {
                reader = "AtlEvtReaderMemR2JetLepton";
            } else if ( sample->IsQCDAntiMuon() ) {
                reader = "AtlEvtReaderMemR2";
            } else {
                reader = "AtlEvtReaderMemR2";
            }
            // set sample type for event reader
            char const * readerArgs = 0;
            if ( sample->IsMC() ) {
                assert(!readerArgs);
                readerArgs = Form("%s::kMC", reader.Data());
            }
            if ( sample->IsDATA() || sample->IsQCD() ) {
                assert(!readerArgs);
                readerArgs = Form("%s::kRealData", reader.Data());
            }
            if ( !readerArgs ) {
                Fatal(__FUNCTION__, "what kind of sample is this: %s ?! Abort!", sample->GetName());
		gSystem->Abort();
            }
            // Set the event reader, the data type (data/MC) and the SgTop-D3PD version
            task_memdisc->SetEvtReader(
                reader.Data(),
                Form("%s, \"%s\", \"%s\", \"%s\", %d",
                     readerArgs, GetSgTopVersion()->Data(),
                     GetSystematicName(Systematic), fTreeName->Data(),
                     EvtReaderMCWeightPosition >= 0 ? EvtReaderMCWeightPosition : 0));
        }
        task_memdisc->AddCuts(fListOfCuts);
	task_memdisc->AddTools(fListOfTools);
	task_memdisc->AddToolCuts(fListOfToolCuts);
	task_memdisc->AddUserEnvs(fListOfUserEnvs);

	
	// Analysis OutputFileNames, use as Input files for MemDisc
	// ToDo: Replace by a GetAnalysisNames-Fct ???

        TString input_dir = fInputDir->Data();
        if ( !fOverrideInputDir.IsNull() )
            input_dir = fOverrideInputDir;

        // if DATA or QCD take nominal
        // SF systematics also use nominal
        if ( sample->IsDATA() || sample->IsQCD() ||
             IsScaleFactorSystematic(Systematic) ||
             IsBTagEVScaleFactorSystematic(Systematic) ||
             Systematic == kPdf_up || Systematic == kPdf_down ) {
            ana_outfile = BuildOutputPath( &input_dir, "",
                                           Jetbin, LepChannel,
                                           kNOMINAL, "" );
            ana_outfileName = BuildOutputFileName( Jetbin, LepChannel,
                                                   kNOMINAL,
                                                   sample->GetName(),
                                                   fUseSubJobsAsInput);
        } else {
            ana_outfile = BuildOutputPath( &input_dir, "",
                                           Jetbin, LepChannel,
                                           Systematic, "" );
            ana_outfileName = BuildOutputFileName( Jetbin, LepChannel,
                                                   Systematic,
                                                   sample->GetName(),
                                                   fUseSubJobsAsInput);
        }
        ana_outfile->Append(ana_outfileName->Data());
        ana_outfile->ReplaceAll("//","/");

	if ( sample->IsDATA() ) {
	    // ---------------------
	    //  DATA specific stuff
	    // ---------------------
	    
	    task_memdisc->AddInputFiles( ana_outfile->Data() );
	    task_memdisc->AddUserEnvs(fListOfUserEnvs);

	    // Turn QCD Tool off (if present)
	    TIter next_tool(fListOfTools);
	    TObject *tool = 0;
	    while ( (tool = next_tool()) ) {
	    	if ( ((TString)tool->ClassName()).Contains("AtlQCDMMTool") )
	    	    task_memdisc->SetToolCut( tool->GetName(), "fProcessMode", "AtlAnalysisTool::kOff");
	    	// Do not turn off, but choose Nominal Mode
	    	if ( ((TString)tool->ClassName()).Contains("AtlPdfReweightingTool") )
	    	    task_memdisc->SetToolCut( tool->GetName(), "fSystMode", "AtlPdfReweightingTool::kNominal");
	    }
	    
	    // skip job if already successful
	    if ( GetTaskStatus(jobHome, outfileName, outfile) ) {
		delete outfile;
		delete outfileName;
		delete tmphist;
		delete tempPath;
		delete jobHome;
		continue;
	    }
	    task_datadir->Add(task_memdisc);

	} else if ( sample->IsMC() ) {
	    // -------------------
	    //  MC specific stuff
	    // -------------------

	    task_memdisc->AddInputFiles( ana_outfile->Data() );
	    task_memdisc->AddUserEnvs(fListOfUserEnvs);

	    // Turn QCD Tool off (if present)
	    TIter next_tool(fListOfTools);
	    TObject *tool = 0;
	    while ( (tool = next_tool()) ) {
	    	if ( ((TString)tool->ClassName()).Contains("AtlQCDMMTool") )
	    	    task_memdisc->SetToolCut( tool->GetName(), "fProcessMode", "AtlAnalysisTool::kOff");

		if ( Systematic == kPdf_up ||
		     Systematic == kPdf_down ) {
		    // Needed for PdfReweightingTool
		    // -----------------------------
		    // ToDo: Update Sample-List with DSIDs and
		    //       add a dm to AtlSample. Needed since
		    //       LMU does not have dataset names
		    //
		    TString *path = new TString(sample->GetPathName());
		    TObjArray *tok = path->Tokenize(".");
		    TString dsid = ((TObjString*) tok->At(3))->GetString();
		    
		    if ( ((TString)tool->ClassName()).Contains("AtlPdfReweightingTool") ) {
			task_memdisc->SetToolCut( tool->GetName(), "fDSID", dsid.Data());
			if ( Systematic == kPdf_up ) 
			    task_memdisc->SetToolCut( tool->GetName(), "fSystMode", "AtlPdfReweightingTool::kUp");
			if ( Systematic == kPdf_down ) 
			    task_memdisc->SetToolCut( tool->GetName(), "fSystMode", "AtlPdfReweightingTool::kDown");
		    }
		    delete path;
		    delete tok;
		}
	    }
	    
	    // skip job if already successful
	    if ( GetTaskStatus(jobHome, outfileName, outfile) ) {
		delete outfile;
		delete outfileName;
		delete tmphist;
		delete tempPath;
		delete jobHome;
		continue;
	    }
	    
	    TTask *t = (TTask*) (task_mcdir->GetListOfTasks())->FindObject(sample->GetCategoryName());
	    if ( t == 0 ) {
		// Folder does not exist, create it
		t = new TTask(sample->GetCategoryName(),
			      sample->GetCategoryName());
		task_mcdir->Add(t);
	    }
	    t->Add(task_memdisc);

	} else if ( sample->IsQCD() ) {
	    // --------------------
	    //  QCD specific stuff
	    // --------------------

	    // At least one qcd tool must be present
	    TIter next_tool(fListOfTools);
	    TObject *tool = 0;
	    Bool_t qcdtool = kFALSE;
            if ( sample->IsQCDJetLepton() || sample->IsQCDAntiMuon() )
                qcdtool = kTRUE;
	    while ( (tool = next_tool()) ) {
	    	if ( ((TString)tool->ClassName()).Contains("AtlQCDMMTool") ) {
	    	    qcdtool = kTRUE;
                    if ( sample->IsQCDJetLepton() || sample->IsQCDAntiMuon() )
                        task_memdisc->SetToolCut( tool->GetName(), "fProcessMode", "AtlAnalysisTool::kOff");
                }
	    	// Do not turn off, but choose Nominal Mode
	    	if ( ((TString)tool->ClassName()).Contains("AtlPdfReweightingTool") )
	    	    task_memdisc->SetToolCut( tool->GetName(), "fSystMode", "AtlPdfReweightingTool::kNominal");
	    }
	    
	    if ( !qcdtool ) {
	    	Error("BuildAnalysisTasks","No QCD Tool present. Abort!");
	    	gSystem->Abort(0);
	    }
	    
	    task_memdisc->AddInputFiles( ana_outfile->Data() );
	    task_memdisc->AddUserEnvs(fListOfUserEnvs);
	    
	    // skip job if already successful
	    if ( GetTaskStatus(jobHome, outfileName, outfile) ) {
		delete outfile;
		delete outfileName;
		delete tmphist;
		delete tempPath;
		delete jobHome;
		continue;
	    }
	    task_qcddir->Add(task_memdisc);
	}
	
	// clean up TString
	delete outfile;
	delete outfileName;
	delete tmphist;
	delete tempPath;
	delete jobHome;
    }
}

//____________________________________________________________________


void AtlTopLevelAnalysis::SetPlotterScheme(Option_t *scheme) {
    //
    // Sets the scheme of the plotter, currently available
    //
    //    "SgTop_t"             = single-top, t-channel
    //    "SgTop_s"             = single-top, s-channel
    //    "SgTop_s_reduced"     = single-top, s-channel, reduced the
    //                            number of templates:
    //                            merging sgtop t + Wt-channel and
    //                            wjetsLF + wjetsHF
    //    "SgTop_s_wjetsMerged" = single-top, s-channel, reduced the
    //                            number of templates:
    //                            merging wjetsLF + wjetsHF
    //    "SgTop_s_QCDsplit     = two folders for JL/AM QCD
    //    "SgTop_s_QCDsplit_wjetsMerged = combination of above
    //    "SgTop_s_splitall     = make every sample its own folder
    //    "SgTop_s_wjetsCB      = merge wjets light into wjets C
    //    "Ttbar"               = top pair-production
    //    "tZ"                  = tZ analyses
    //
    if ( fPlotterScheme != 0 ) {
	delete fPlotterScheme;
    }
    fPlotterScheme = new TString(scheme);
}


//____________________________________________________________________

TList* AtlTopLevelAnalysis::GetPlottingFolders(Option_t *option,
					       TList *samples ) {
    //
    // Sorts samples into AtlPlotterFolder and returns
    // list of AtlPlotterFolder.
    //
    // WARNING: User has to delete returned list
    //

    TString opt = option;
    if ( opt.Contains("SgTop_t") ) {
	return GetPlottingFoldersSgTop_t(samples);
    } else if ( opt.Contains("SgTop_s_reduced") ) {
	return GetPlottingFoldersSgTop_s_reduced(samples);
    } else if ( opt.Contains("SgTop_s_wjetsMerged") ) {
	return GetPlottingFoldersSgTop_s_wjetsMerged(samples);
    } else if ( opt.Contains("SgTop_s_QCDsplit_wjetsMerged") ) {
        return GetPlottingFoldersSgTop_s_QCDsplit_wjetsMerged(samples);
    } else if ( opt.Contains("SgTop_s_QCDsplit") ) {
        return GetPlottingFoldersSgTop_s_QCDsplit(samples);
    } else if ( opt.Contains("SgTop_s_splitall") ) {
        return GetPlottingFoldersSgTop_s_splitall(samples);
    } else if ( opt.Contains("SgTop_s_wjetsCB") ) {
	return GetPlottingFoldersSgTop_s_wjetsCB(samples);
    } else if ( opt.Contains("SgTop_s") ) {
        return GetPlottingFoldersSgTop_s(samples);
    } else if ( opt.Contains("Ttbar") ) {
	return GetPlottingFoldersTtbar(samples);
    } else if ( opt.Contains("tZ") ) {
	return GetPlottingFolderstZ(samples);
    } else if ( opt.Contains("detail") ) {
	return GetPlottingFoldersDetail(samples);
    } else {
	Error("GetPlottingFolders", "Plotter Scheme: '%s' unknown! Abort!",
	      fPlotterScheme->Data() );
	gSystem->Abort(0);
	return 0;
    }
}

//____________________________________________________________________

TList* AtlTopLevelAnalysis::GetPlottingFoldersSgTop_t(TList *samples) {
    //
    // Default structure of the plotter for a single top t-channel
    // analysis.
    //
    // First group in folder list appears at the bottom of the stacked
    // plot.
    //

    // Create all folders
    TList *folder = new TList;
    AtlPlotterFolder *folder_data           = new AtlPlotterFolder("DATA", "Data", "kBlack" );
    AtlPlotterFolder *folder_qcd            = new AtlPlotterFolder("QCD", "Multijet", "kMagenta+2" );
    AtlPlotterFolder *folder_zjets_diboson  = new AtlPlotterFolder("zjets_diboson", "Zjets / Diboson", "kOrange+1");
    AtlPlotterFolder *folder_ttbar          = new AtlPlotterFolder("ttbar", "t#bar{t}", "kRed-4" );
    AtlPlotterFolder *folder_wjetsLF        = new AtlPlotterFolder("wjetsLF", "Wjets light flavour", "kGreen-7" );
    AtlPlotterFolder *folder_wjetsHF        = new AtlPlotterFolder("wjetsHF", "Wjets heavy flavour", "kGreen-3" );
    AtlPlotterFolder *folder_sgtop_Wt       = new AtlPlotterFolder("sgtop_WtChannel", "Single-top Wt-production", "kAzure+3" );
    AtlPlotterFolder *folder_sgtop_sChannel = new AtlPlotterFolder("sgtop_sChannel", "Single-top s-channel", "kAzure-3" );
    AtlPlotterFolder *folder_sgtop_tChannel = new AtlPlotterFolder("sgtop_tChannel", "Single-top t-channel", "kAzure+5" );

    folder->Add(folder_data);
    folder->Add(folder_qcd);
    folder->Add(folder_zjets_diboson);
    folder->Add(folder_wjetsHF);
    folder->Add(folder_wjetsLF);
    folder->Add(folder_ttbar);
    folder->Add(folder_sgtop_Wt);
    folder->Add(folder_sgtop_sChannel);
    folder->Add(folder_sgtop_tChannel);
    
    TIter next_sample(samples);
    AtlSample *sample = 0;
    while ( (sample = (AtlSample*)next_sample()) ) {
	if ( sample->IsDATA() ) { folder_data->AddSample(sample); }
	if ( sample->IsQCD() ) { folder_qcd->AddSample(sample); }
	if ( sample->IsZjets() || sample->IsDiboson() ) {
	    folder_zjets_diboson->AddSample(sample);
	}
	if ( fUseHforSamples ) {
	    if ( sample->IsHFOR0() || sample->IsHFOR1() || sample->IsHFOR2() )
		folder_wjetsHF->AddSample(sample);
	    if ( sample->IsHFOR3() )
		folder_wjetsLF->AddSample(sample);
	} else {
	    if ( sample->IsWjetsHeavy() ) folder_wjetsHF->AddSample(sample);
	    if ( sample->IsWjetsLight() ) folder_wjetsLF->AddSample(sample);
	}
	if ( sample->IsTtbar() ) { folder_ttbar->AddSample(sample); }
	if ( sample->IsSgTopWt() ) { folder_sgtop_Wt->AddSample(sample); }
	if ( sample->IsSgTopSChannel() ) { folder_sgtop_sChannel->AddSample(sample); }
	if ( sample->IsSgTopTChannel() ) { folder_sgtop_tChannel->AddSample(sample); }
    }
    return folder;
}

//____________________________________________________________________

TList* AtlTopLevelAnalysis::GetPlottingFoldersDetail(TList *samples) {
    //
    // Default structure of the plotter for a detailed plotting scheme.
    //
    // First group in folder list appears at the bottom of the stacked
    // plot.
    //

    // Create all folders
    TList *folder = new TList;
    AtlPlotterFolder *folder_data           = new AtlPlotterFolder("DATA", "Data", "kBlack" );
    AtlPlotterFolder *folder_qcd            = new AtlPlotterFolder("QCD", "Multijet", "kMagenta+2" );
    AtlPlotterFolder *folder_diboson        = new AtlPlotterFolder("diboson", "Di-boson", "kOrange+1");
    AtlPlotterFolder *folder_zjetsLF        = new AtlPlotterFolder("zjetsLF", "Zjets light flavour", "kOrange+2");
    AtlPlotterFolder *folder_zjetsC         = new AtlPlotterFolder("zjetsC", "Z + cc", "kOrange+3");
    AtlPlotterFolder *folder_zjetsB         = new AtlPlotterFolder("zjetsB", "Z + bb", "kOrange+4");
    AtlPlotterFolder *folder_ttbar          = new AtlPlotterFolder("ttbar", "t#bar{t}", "kRed-4" );
    AtlPlotterFolder *folder_wjetsLF        = new AtlPlotterFolder("wjetsLF", "Wjets light flavour", "kGreen-7" );
    AtlPlotterFolder *folder_wjetsC         = new AtlPlotterFolder("wjetsC", "W+cc", "kGreen-5" );
    AtlPlotterFolder *folder_wjetsB         = new AtlPlotterFolder("wjetsB", "W+bb", "kGreen-3" );
    AtlPlotterFolder *folder_sgtop_Wt       = new AtlPlotterFolder("sgtop_WtChannel", "Single-top Wt-production", "kAzure+3" );
    AtlPlotterFolder *folder_sgtop_sChannel = new AtlPlotterFolder("sgtop_sChannel", "Single-top s-channel", "kAzure-3" );
    AtlPlotterFolder *folder_sgtop_tChannel = new AtlPlotterFolder("sgtop_tChannel", "Single-top t-channel", "kAzure+5" );

    folder->Add(folder_data);
    folder->Add(folder_qcd);
    folder->Add(folder_diboson);
    folder->Add(folder_zjetsLF);
    folder->Add(folder_zjetsC);
    folder->Add(folder_zjetsB);
    folder->Add(folder_wjetsC);
    folder->Add(folder_wjetsB);
    folder->Add(folder_wjetsLF);
    folder->Add(folder_ttbar);
    folder->Add(folder_sgtop_Wt);
    folder->Add(folder_sgtop_sChannel);
    folder->Add(folder_sgtop_tChannel);
    
    TIter next_sample(samples);
    AtlSample *sample = 0;
    while ( (sample = (AtlSample*)next_sample()) ) {
	if ( sample->IsDATA() ) { folder_data->AddSample(sample); }
	if ( sample->IsQCD() ) { folder_qcd->AddSample(sample); }
	if ( sample->IsDiboson() ) { folder_diboson->AddSample(sample); }
	if ( sample->IsZjetsLF() ) { folder_zjetsLF->AddSample(sample); }
	if ( sample->IsZjetsC() ) { folder_zjetsC->AddSample(sample); }
	if ( sample->IsZjetsB() ) { folder_zjetsB->AddSample(sample); }
	if ( fUseHforSamples ) {
	    if ( sample->IsHFOR0() ) { // hfor0 = BB
		folder_wjetsB->AddSample(sample);
	    }
	    if ( sample->IsHFOR1() // hfor1 = CC
		 || sample->IsHFOR2() ) { // hfor2 = C
		folder_wjetsC->AddSample(sample);
	    }
	    if ( sample->IsHFOR3() )
		folder_wjetsLF->AddSample(sample);
	} else {
	    if ( sample->IsWjetsB() ) folder_wjetsB->AddSample(sample);
	    if ( sample->IsWjetsC() ) folder_wjetsC->AddSample(sample);
	    if ( sample->IsWjetsLight() ) folder_wjetsLF->AddSample(sample);
	}
	if ( sample->IsTtbar() ) { folder_ttbar->AddSample(sample); }
	if ( sample->IsSgTopWt() ) { folder_sgtop_Wt->AddSample(sample); }
	if ( sample->IsSgTopSChannel() ) { folder_sgtop_sChannel->AddSample(sample); }
	if ( sample->IsSgTopTChannel() ) { folder_sgtop_tChannel->AddSample(sample); }
    }
    return folder;
}

//____________________________________________________________________

TList* AtlTopLevelAnalysis::GetPlottingFoldersSgTop_s(TList *samples) {
    //
    // Default structure of the plotter for a single top s-channel
    // analysis.
    //
    // First group in folder list appears at the bottom of the stacked
    // plot.
    //

    // Create all folders
    TList *folder = new TList;
    AtlPlotterFolder *folder_data           = new AtlPlotterFolder("DATA", "Data", "kBlack" );
    AtlPlotterFolder *folder_qcd            = new AtlPlotterFolder("QCD", "Multijet", "kMagenta+2" );
    AtlPlotterFolder *folder_zjets_diboson  = new AtlPlotterFolder("zjets_diboson", "Zjets + Diboson", "kOrange+1");
    AtlPlotterFolder *folder_ttbar          = new AtlPlotterFolder("ttbar", "t#bar{t}", "kRed-4" );
    AtlPlotterFolder *folder_wjetsLF        = new AtlPlotterFolder("wjetsLF", "Wjets light flavour", "kGreen-7" );
    AtlPlotterFolder *folder_wjetsHF        = new AtlPlotterFolder("wjetsHF", "Wjets heavy flavour", "kGreen-3" );
    AtlPlotterFolder *folder_sgtop_Wt       = new AtlPlotterFolder("sgtop_WtChannel", "Single-top Wt production", "kAzure-3" );
    AtlPlotterFolder *folder_sgtop_tChannel = new AtlPlotterFolder("sgtop_tChannel", "Single-top #it{t}-channel", "kAzure+5" );
    AtlPlotterFolder *folder_sgtop_sChannel = new AtlPlotterFolder("sgtop_sChannel", "Single-top #it{s}-channel", "kAzure+3" );

    folder->Add(folder_data);
    folder->Add(folder_qcd);
    folder->Add(folder_zjets_diboson);
    folder->Add(folder_wjetsHF);
    folder->Add(folder_wjetsLF);
    folder->Add(folder_ttbar);
    folder->Add(folder_sgtop_Wt);
    folder->Add(folder_sgtop_tChannel);
    folder->Add(folder_sgtop_sChannel);
    
    TIter next_sample(samples);
    AtlSample *sample = 0;
    while ( (sample = (AtlSample*)next_sample()) ) {
	// make sure that every sample is added exactly once
	if ( sample->IsDATA() ) { folder_data->AddSample(sample); continue; }
	if ( sample->IsQCD() ) { folder_qcd->AddSample(sample); continue; }
	if ( sample->IsZjets() || sample->IsDiboson() ) {
	    folder_zjets_diboson->AddSample(sample);
	    continue;
	}
	if ( fUseHforSamples ) {
	    if ( sample->IsHFOR0() || sample->IsHFOR1() || sample->IsHFOR2() ) {
		folder_wjetsHF->AddSample(sample);
		continue;
	    }
	    if ( sample->IsHFOR3() ) {
		folder_wjetsLF->AddSample(sample);
		continue;
	    }
	} else {
	    if ( sample->IsWjetsHeavy() ) {
		folder_wjetsHF->AddSample(sample);
		continue;
	    }
	    if ( sample->IsWjetsLight() ) {
		folder_wjetsLF->AddSample(sample);
		continue;
	    }
	    if ( sample->IsWjets() ) {
		folder_wjetsHF->AddSample(sample);
		Warning(__FUNCTION__,
			"Sample %s was added to wjetsHF, but is neither HF nor LF.",
			sample->GetName());
		continue;
	    }
	}
	if ( sample->IsTtbar() ) { folder_ttbar->AddSample(sample); continue; }
	if ( sample->IsSgTopWt() ) { folder_sgtop_Wt->AddSample(sample); continue; }
	if ( sample->IsSgTopSChannel() ) { folder_sgtop_sChannel->AddSample(sample); continue; }
	if ( sample->IsSgTopTChannel() ) { folder_sgtop_tChannel->AddSample(sample); continue; }
	Warning(__FUNCTION__, "Sample %s was not added to any folder.", sample->GetName());	
    }
    return folder;
}

//____________________________________________________________________

TList* AtlTopLevelAnalysis::GetPlottingFoldersSgTop_s_reduced(TList *samples) {
    //
    // Default structure of the plotter for a single top s-channel
    // analysis with reduced number of templates.
	// Merging wjetsLF + wjetsHF and single top t + Wt-channel.
    //
    // First group in folder list appears at the bottom of the stacked
    // plot.
    //

    // Create all folders
    TList *folder = new TList;
    AtlPlotterFolder *folder_data             = new AtlPlotterFolder("DATA", "Data", "kBlack" );
    AtlPlotterFolder *folder_qcd              = new AtlPlotterFolder("QCD", "Multijet", "kMagenta+2" );
    AtlPlotterFolder *folder_zjets_diboson    = new AtlPlotterFolder("zjets_diboson", "Zjets + Diboson", "kOrange+1");
    AtlPlotterFolder *folder_ttbar            = new AtlPlotterFolder("ttbar", "t#bar{t}", "kRed-4" );
    AtlPlotterFolder *folder_wjets            = new AtlPlotterFolder("wjets", "Wjets", "kGreen-3" );
    AtlPlotterFolder *folder_sgtop_tWtChannel = new AtlPlotterFolder("sgtop_tWtChannel", "Single-top background", "kAzure+5" );
    AtlPlotterFolder *folder_sgtop_sChannel   = new AtlPlotterFolder("sgtop_sChannel", "Single-top #it{s}-channel", "kAzure+3" );

    folder->Add(folder_data);
    folder->Add(folder_qcd);
    folder->Add(folder_zjets_diboson);
    folder->Add(folder_wjets);
    folder->Add(folder_ttbar);
    folder->Add(folder_sgtop_tWtChannel);
    folder->Add(folder_sgtop_sChannel);
    
    TIter next_sample(samples);
    AtlSample *sample = 0;
    while ( (sample = (AtlSample*)next_sample()) ) {
	if ( sample->IsDATA() ) { folder_data->AddSample(sample); }
	if ( sample->IsQCD() ) { folder_qcd->AddSample(sample); }
	if ( sample->IsZjets() || sample->IsDiboson() ) {
	    folder_zjets_diboson->AddSample(sample);
	}
	if ( sample->IsHFOR0() || sample->IsHFOR1() || sample->IsHFOR2() || sample->IsHFOR3()
		 || sample->IsWjetsHeavy() || sample->IsWjetsLight() )
		folder_wjets->AddSample(sample);
	if ( sample->IsTtbar() ) { folder_ttbar->AddSample(sample); }
	if ( sample->IsSgTopWt() || sample->IsSgTopTChannel() ) { folder_sgtop_tWtChannel->AddSample(sample); }
	if ( sample->IsSgTopSChannel() ) { folder_sgtop_sChannel->AddSample(sample); }
    }
    return folder;
}

//____________________________________________________________________

TList* AtlTopLevelAnalysis::GetPlottingFoldersSgTop_s_wjetsMerged(TList *samples) {
    //
    // Default structure of the plotter for a single top s-channel
    // analysis with reduced number of templates.
	// Merging wjetsLF + wjetsHF.
    //
    // First group in folder list appears at the bottom of the stacked
    // plot.
    //

    // Create all folders
    TList *folder = new TList;
    AtlPlotterFolder *folder_data           = new AtlPlotterFolder("DATA", "Data", "kBlack" );
    AtlPlotterFolder *folder_qcd            = new AtlPlotterFolder("QCD", "Multijet", "kMagenta+2" );
    AtlPlotterFolder *folder_zjets_diboson  = new AtlPlotterFolder("zjets_diboson", "Zjets + Diboson", "kOrange+1");
    AtlPlotterFolder *folder_ttbar          = new AtlPlotterFolder("ttbar", "t#bar{t}", "kRed-4" );
    AtlPlotterFolder *folder_wjets          = new AtlPlotterFolder("wjets", "Wjets", "kGreen-3" );
    AtlPlotterFolder *folder_sgtop_Wt       = new AtlPlotterFolder("sgtop_WtChannel", "Single-top Wt production", "kAzure-3" );
    AtlPlotterFolder *folder_sgtop_tChannel = new AtlPlotterFolder("sgtop_tChannel", "Single-top #it{t}-channel", "kAzure+5" );
    AtlPlotterFolder *folder_sgtop_sChannel = new AtlPlotterFolder("sgtop_sChannel", "Single-top #it{s}-channel", "kAzure+3" );

    folder->Add(folder_data);
    folder->Add(folder_qcd);
    folder->Add(folder_zjets_diboson);
    folder->Add(folder_wjets);
    folder->Add(folder_ttbar);
    folder->Add(folder_sgtop_Wt);
    folder->Add(folder_sgtop_tChannel);
    folder->Add(folder_sgtop_sChannel);
    
    TIter next_sample(samples);
    AtlSample *sample = 0;
    while ( (sample = (AtlSample*)next_sample()) ) {
	if ( sample->IsDATA() ) { folder_data->AddSample(sample); }
	if ( sample->IsQCD() ) { folder_qcd->AddSample(sample); }
	if ( sample->IsZjets() || sample->IsDiboson() ) {
	    folder_zjets_diboson->AddSample(sample);
	}
	if ( fUseHforSamples ) {
	    if ( sample->IsHFOR0() || sample->IsHFOR1() || sample->IsHFOR2() || sample->IsHFOR3() )
		folder_wjets->AddSample(sample);
	} else {
	    if ( sample->IsWjets() ) folder_wjets->AddSample(sample);
	}
	if ( sample->IsTtbar() ) { folder_ttbar->AddSample(sample); }
	if ( sample->IsSgTopWt() ) { folder_sgtop_Wt->AddSample(sample); }
	if ( sample->IsSgTopSChannel() ) { folder_sgtop_sChannel->AddSample(sample); }
	if ( sample->IsSgTopTChannel() ) { folder_sgtop_tChannel->AddSample(sample); }
    }
    return folder;
}

//____________________________________________________________________

TList* AtlTopLevelAnalysis::GetPlottingFoldersSgTop_s_QCDsplit(TList *samples) {
    //
    // Default structure of the plotter for a single top s-channel
    // analysis with reduced number of templates.
    // Merging wjetsLF + wjetsHF, splitting QCD methods
    //
    // First group in folder list appears at the bottom of the stacked
    // plot.
    //

    Info(__FUNCTION__, "Using plotting folders with QCD split");

    // Create all folders
    TList *folder = new TList;
    AtlPlotterFolder *folder_data           = new AtlPlotterFolder("DATA", "Data", "kBlack" );
    AtlPlotterFolder *folder_qcdmm          = new AtlPlotterFolder("QCDMM", "Multijet Matrix Method", "kMagenta+2" );
    AtlPlotterFolder *folder_qcdam          = new AtlPlotterFolder("QCDAM", "Multijet Anti-Muon Method", "kMagenta+2" );
    AtlPlotterFolder *folder_qcdjl          = new AtlPlotterFolder("QCDJL", "Multijet Jet-Lepton Method", "kMagenta+2" );
    AtlPlotterFolder *folder_zjets_diboson  = new AtlPlotterFolder("zjets_diboson", "Zjets + Diboson", "kOrange+1");
    AtlPlotterFolder *folder_ttbar          = new AtlPlotterFolder("ttbar", "t#bar{t}", "kRed-4" );
    AtlPlotterFolder *folder_wjetsLF        = new AtlPlotterFolder("wjetsLF", "Wjets light flavour", "kGreen-7" );
    AtlPlotterFolder *folder_wjetsHF        = new AtlPlotterFolder("wjetsHF", "Wjets heavy flavour", "kGreen-3" );
    AtlPlotterFolder *folder_sgtop_Wt       = new AtlPlotterFolder("sgtop_WtChannel", "Single-top Wt production", "kAzure-3" );
    AtlPlotterFolder *folder_sgtop_tChannel = new AtlPlotterFolder("sgtop_tChannel", "Single-top #it{t}-channel", "kAzure+5" );
    AtlPlotterFolder *folder_sgtop_sChannel = new AtlPlotterFolder("sgtop_sChannel", "Single-top #it{s}-channel", "kAzure+3" );

    folder->Add(folder_data);
    folder->Add(folder_qcdmm);
    folder->Add(folder_qcdam);
    folder->Add(folder_qcdjl);
    folder->Add(folder_zjets_diboson);
    folder->Add(folder_wjetsLF);
    folder->Add(folder_wjetsHF);
    folder->Add(folder_ttbar);
    folder->Add(folder_sgtop_Wt);
    folder->Add(folder_sgtop_tChannel);
    folder->Add(folder_sgtop_sChannel);

    TIter next_sample(samples);
    AtlSample *sample = 0;
    while ( (sample = (AtlSample*)next_sample()) ) {
        if ( sample->IsDATA() ) { folder_data->AddSample(sample); }
        if ( sample->IsQCD() ) {
            if (sample->IsQCDAntiMuon()) {
                folder_qcdam->AddSample(sample);
            } else if (sample->IsQCDJetLepton()) {
                folder_qcdjl->AddSample(sample);
            } else {
                folder_qcdmm->AddSample(sample);
            }
        }
        if ( sample->IsZjets() || sample->IsDiboson() ) {
            folder_zjets_diboson->AddSample(sample);
        }
        if ( sample->IsWjetsHeavy() ) {
            folder_wjetsHF->AddSample(sample);
        } else if ( sample->IsWjetsLight() ) {
            folder_wjetsLF->AddSample(sample);
        } else if ( sample->IsWjets() ) {
            folder_wjetsHF->AddSample(sample);
            Warning(__FUNCTION__,
                    "Sample %s was added to wjetsHF, but is neither HF nor LF.",
                    sample->GetName());
        }
        if ( sample->IsTtbar() ) { folder_ttbar->AddSample(sample); }
        if ( sample->IsSgTopWt() ) { folder_sgtop_Wt->AddSample(sample); }
        if ( sample->IsSgTopSChannel() ) { folder_sgtop_sChannel->AddSample(sample); }
        if ( sample->IsSgTopTChannel() ) { folder_sgtop_tChannel->AddSample(sample); }
    }
    return folder;
}

//____________________________________________________________________

TList* AtlTopLevelAnalysis::GetPlottingFoldersSgTop_s_QCDsplit_wjetsMerged(TList *samples) {
    //
    // Default structure of the plotter for a single top s-channel
    // analysis with reduced number of templates.
    // Merging wjetsLF + wjetsHF, splitting QCD methods
    //
    // First group in folder list appears at the bottom of the stacked
    // plot.
    //

    Info(__FUNCTION__, "Using plotting folders with QCD split");

    // Create all folders
    TList *folder = new TList;
    AtlPlotterFolder *folder_data           = new AtlPlotterFolder("DATA", "Data", "kBlack" );
    AtlPlotterFolder *folder_qcdmm          = new AtlPlotterFolder("QCDMM", "Multijet Matrix Method", "kMagenta+2" );
    AtlPlotterFolder *folder_qcdam          = new AtlPlotterFolder("QCDAM", "Multijet Anti-Muon Method", "kMagenta+2" );
    AtlPlotterFolder *folder_qcdjl          = new AtlPlotterFolder("QCDJL", "Multijet Jet-Lepton Method", "kMagenta+2" );
    AtlPlotterFolder *folder_zjets_diboson  = new AtlPlotterFolder("zjets_diboson", "Zjets + Diboson", "kOrange+1");
    AtlPlotterFolder *folder_ttbar          = new AtlPlotterFolder("ttbar", "t#bar{t}", "kRed-4" );
    AtlPlotterFolder *folder_wjets          = new AtlPlotterFolder("wjets", "Wjets", "kGreen-3" );
    AtlPlotterFolder *folder_sgtop_Wt       = new AtlPlotterFolder("sgtop_WtChannel", "Single-top Wt production", "kAzure-3" );
    AtlPlotterFolder *folder_sgtop_tChannel = new AtlPlotterFolder("sgtop_tChannel", "Single-top #it{t}-channel", "kAzure+5" );
    AtlPlotterFolder *folder_sgtop_sChannel = new AtlPlotterFolder("sgtop_sChannel", "Single-top #it{s}-channel", "kAzure+3" );

    folder->Add(folder_data);
    folder->Add(folder_qcdmm);
    folder->Add(folder_qcdam);
    folder->Add(folder_qcdjl);
    folder->Add(folder_zjets_diboson);
    folder->Add(folder_wjets);
    folder->Add(folder_ttbar);
    folder->Add(folder_sgtop_Wt);
    folder->Add(folder_sgtop_tChannel);
    folder->Add(folder_sgtop_sChannel);

    TIter next_sample(samples);
    AtlSample *sample = 0;
    while ( (sample = (AtlSample*)next_sample()) ) {
        if ( sample->IsDATA() ) { folder_data->AddSample(sample); }
        if ( sample->IsQCD() ) {
            if (sample->IsQCDAntiMuon()) {
                folder_qcdam->AddSample(sample);
            } else if (sample->IsQCDJetLepton()) {
                folder_qcdjl->AddSample(sample);
            } else {
                folder_qcdmm->AddSample(sample);
            }
        }
        if ( sample->IsZjets() || sample->IsDiboson() ) {
            folder_zjets_diboson->AddSample(sample);
        }
        if ( fUseHforSamples ) {
            if ( sample->IsHFOR0() || sample->IsHFOR1() || sample->IsHFOR2() || sample->IsHFOR3() )
                folder_wjets->AddSample(sample);
        } else {
            if ( sample->IsWjets() ) folder_wjets->AddSample(sample);
        }
        if ( sample->IsTtbar() ) { folder_ttbar->AddSample(sample); }
        if ( sample->IsSgTopWt() ) { folder_sgtop_Wt->AddSample(sample); }
        if ( sample->IsSgTopSChannel() ) { folder_sgtop_sChannel->AddSample(sample); }
        if ( sample->IsSgTopTChannel() ) { folder_sgtop_tChannel->AddSample(sample); }
    }
    return folder;
}

//____________________________________________________________________


TList* AtlTopLevelAnalysis::GetPlottingFoldersSgTop_s_splitall(TList *samples) {
    //
    // Default structure of the plotter for a single top s-channel
    // analysis with reduced number of templates.
	// Merging wjetsLF + wjetsHF, splitting QCD methods
    //
    // First group in folder list appears at the bottom of the stacked
    // plot.
    //

    Info(__FUNCTION__, "Using one folder per sample");

    // Create all folders
    TList *folder = new TList;
    TIter next_sample(samples);
    AtlSample *sample = 0;
    while ( (sample = (AtlSample*)next_sample()) ) {
        TString name(sample->GetName());
        AtlPlotterFolder *folder_tmp = new AtlPlotterFolder(name.Data(), name.Data(), "kBlack");
        folder_tmp->AddSample(sample);
        folder->Add(folder_tmp);
    }
    return folder;
}

//____________________________________________________________________

TList* AtlTopLevelAnalysis::GetPlottingFoldersSgTop_s_wjetsCB(TList *samples) {
    //
    // Default structure of the plotter for a single top s-channel
    // analysis with reduced number of templates.
    // Merging wjetsLF & wjets+C
    //
    // First group in folder list appears at the bottom of the stacked
    // plot.
    //

    // Create all folders
    TList *folder = new TList;
    AtlPlotterFolder *folder_data           = new AtlPlotterFolder("DATA", "Data", "kBlack" );
    AtlPlotterFolder *folder_qcd            = new AtlPlotterFolder("QCD", "Multijet", "kMagenta+2" );
    AtlPlotterFolder *folder_zjets_diboson  = new AtlPlotterFolder("zjets_diboson", "Zjets + Diboson", "kOrange+1");
    AtlPlotterFolder *folder_ttbar          = new AtlPlotterFolder("ttbar", "t#bar{t}", "kRed-4" );
    AtlPlotterFolder *folder_wjetsC         = new AtlPlotterFolder("wjetsC", "WjetsC", "kGreen-3" );
    AtlPlotterFolder *folder_wjetsB         = new AtlPlotterFolder("wjetsB", "WjetsB", "kGreen+3" );
    AtlPlotterFolder *folder_sgtop_Wt       = new AtlPlotterFolder("sgtop_WtChannel", "Single-top Wt production", "kAzure-3" );
    AtlPlotterFolder *folder_sgtop_tChannel = new AtlPlotterFolder("sgtop_tChannel", "Single-top #it{t}-channel", "kAzure+5" );
    AtlPlotterFolder *folder_sgtop_sChannel = new AtlPlotterFolder("sgtop_sChannel", "Single-top #it{s}-channel", "kAzure+3" );

    folder->Add(folder_data);
    folder->Add(folder_qcd);
    folder->Add(folder_zjets_diboson);
    folder->Add(folder_wjetsC);
    folder->Add(folder_wjetsB);
    folder->Add(folder_ttbar);
    folder->Add(folder_sgtop_Wt);
    folder->Add(folder_sgtop_tChannel);
    folder->Add(folder_sgtop_sChannel);
    
    TIter next_sample(samples);
    AtlSample *sample = 0;
    while ( (sample = (AtlSample*)next_sample()) ) {
	if ( sample->IsDATA() ) { folder_data->AddSample(sample); }
	if ( sample->IsQCD() ) { folder_qcd->AddSample(sample); }
	if ( sample->IsZjets() || sample->IsDiboson() ) {
	    folder_zjets_diboson->AddSample(sample);
	}
	if ( fUseHforSamples ) {
	    if ( sample->IsHFOR0() )
		folder_wjetsB->AddSample(sample);
	    if ( sample->IsHFOR1() || sample->IsHFOR2() || sample->IsHFOR3() )
		folder_wjetsC->AddSample(sample);
	} else {
	    if ( sample->IsWjetsC() || sample->IsWjetsLight() )
		folder_wjetsC->AddSample(sample);
	    if ( sample->IsWjetsB() )
		folder_wjetsB->AddSample(sample);
	}
	if ( sample->IsTtbar() ) { folder_ttbar->AddSample(sample); }
	if ( sample->IsSgTopWt() ) { folder_sgtop_Wt->AddSample(sample); }
	if ( sample->IsSgTopSChannel() ) { folder_sgtop_sChannel->AddSample(sample); }
	if ( sample->IsSgTopTChannel() ) { folder_sgtop_tChannel->AddSample(sample); }
    }
    return folder;
}

//____________________________________________________________________

TList* AtlTopLevelAnalysis::GetPlottingFoldersTtbar(TList *samples) {
    //
    // Default structure of the plotter for ttbar analyses 
    //
    // First group in folder list appears at the bottom of the stacked plot
    //

    // Create all folders
    TList *folder = new TList;
    AtlPlotterFolder *folder_data          = new AtlPlotterFolder("DATA", "DATA", "kBlack" );
    AtlPlotterFolder *folder_qcd           = new AtlPlotterFolder("QCD", "Multijet", "619" );
    AtlPlotterFolder *folder_zjets = new AtlPlotterFolder("zjets", "Zjets", "95");
    AtlPlotterFolder *folder_diboson = new AtlPlotterFolder("diboson", "Diboson", "kYellow");  
    AtlPlotterFolder *folder_wjets       = new AtlPlotterFolder("wjetsLF", "Wjets", "92" );
    AtlPlotterFolder *folder_sgtop         = new AtlPlotterFolder("sgtop", "Single-top", "62" );
    AtlPlotterFolder *folder_ttbar         = new AtlPlotterFolder("ttbar", "Ttbar", "0" );

    folder->Add(folder_data);
    folder->Add(folder_qcd);
    folder->Add(folder_zjets);
    folder->Add(folder_diboson);   
    folder->Add(folder_wjets);
    folder->Add(folder_sgtop);
    folder->Add(folder_ttbar);
    
    TIter next_sample(samples);
    AtlSample *sample = 0;
    while ( (sample = (AtlSample*)next_sample()) ) {
	if ( sample->IsDATA() ) folder_data->AddSample(sample);
	if ( sample->IsQCD() ) folder_qcd->AddSample(sample);
	if ( sample->IsZjets() ) folder_zjets->AddSample(sample);
	if ( sample->IsDiboson() ) folder_diboson->AddSample(sample);	
	if ( fUseHforSamples ) {
	    if ( sample->IsHFOR0() || sample->IsHFOR1() || sample->IsHFOR2() )
		folder_wjets->AddSample(sample);
	    if ( sample->IsHFOR3() )
		folder_wjets->AddSample(sample);
	} else {
 	    if ( sample->IsWjetsHeavy() ||  sample->IsWjetsLight()) folder_wjets->AddSample(sample);
//   	    if ( sample->IsWjetsLight() ) folder_wjetsLF->AddSample(sample);
	}
	if ( sample->IsTtbar() ) folder_ttbar->AddSample(sample);
	if ( sample->IsSgTop() ) folder_sgtop->AddSample(sample);
    }
    return folder;
}

//____________________________________________________________________

TList* AtlTopLevelAnalysis::GetPlottingFolderstZ(TList *samples) {
    //
    // Default structure of the plotter for a tZ analysis.
    // All SgTop, as well as Wjets LF and HF are merged, while Zjets
    // and Diboson are not.
    //
    // The first group in folder list appears at the bottom of the
    // stacked plot.
    //

    // Create all folders
    TList *folder = new TList;
    AtlPlotterFolder *folder_data     = new AtlPlotterFolder("DATA", "Data", "kBlack" );
    AtlPlotterFolder *folder_qcd      = new AtlPlotterFolder("QCD", "Multijet", "kMagenta+2" );
    AtlPlotterFolder *folder_diboson  = new AtlPlotterFolder("diboson", "Diboson", "kCyan+3");
    AtlPlotterFolder *folder_zjets    = new AtlPlotterFolder("zjets", "Z+jets", "kOrange+1");
    AtlPlotterFolder *folder_ttbar    = new AtlPlotterFolder("ttbar", "t#bar{t}", "kRed-4" );
    AtlPlotterFolder *folder_ttbarV   = new AtlPlotterFolder("ttbarV", "t#bar{t}+W/Z", "kRed+2");
    AtlPlotterFolder *folder_wjets    = new AtlPlotterFolder("wjets", "W+jets", "kGreen-3" );
    AtlPlotterFolder *folder_sgtop    = new AtlPlotterFolder("sgtop", "Single-top production", "kAzure+5" );
    AtlPlotterFolder *folder_tZj      = new AtlPlotterFolder("tZj", "tZj SM", "kAzure+3");
    AtlPlotterFolder *folder_FCNCtZ   = new AtlPlotterFolder("FCNC_tZ", "FCNC utZ #sigma = 1 pb", "kYellow" );

    folder->Add(folder_data);
    folder->Add(folder_qcd);
    folder->Add(folder_diboson);
    folder->Add(folder_zjets);
    folder->Add(folder_wjets);
    folder->Add(folder_ttbar);
    folder->Add(folder_ttbarV);
    folder->Add(folder_sgtop);
    folder->Add(folder_tZj);
    folder->Add(folder_FCNCtZ);
    
    TIter next_sample(samples);
    AtlSample *sample = 0;
    while ( (sample = (AtlSample*)next_sample()) ) {
	if ( sample->IsDATA() ) { folder_data->AddSample(sample); }
	if ( sample->IsQCD() ) { folder_qcd->AddSample(sample); }
	if ( sample->IsDiboson() ) { folder_diboson->AddSample(sample); }
	if ( sample->IsZjets() ) { folder_zjets->AddSample(sample); }
	if ( fUseHforSamples ) {
	    if ( sample->IsHFOR0() || sample->IsHFOR1() || sample->IsHFOR2() || sample->IsHFOR3() )
		folder_wjets->AddSample(sample);
	} else {
	    if ( sample->IsWjets() ) folder_wjets->AddSample(sample);
	}
	if ( sample->IsTtbar() ) { folder_ttbar->AddSample(sample); }
	if ( sample->IsTtbarV() ) { folder_ttbarV->AddSample(sample); }
	if ( sample->IsSgTopWt() ) { folder_sgtop->AddSample(sample); }
	if ( sample->IsSgTopSChannel() ) { folder_sgtop->AddSample(sample); }
	if ( sample->IsSgTopTChannel() ) { folder_sgtop->AddSample(sample); }
	if ( sample->IstZj() ) { folder_tZj->AddSample(sample); }
	if ( sample->IsFCNCtZ() ) { folder_FCNCtZ->AddSample(sample); }
    }
    return folder;
}

//____________________________________________________________________

void AtlTopLevelAnalysis::BuildMergingTasks(TTask *ParentTask,
					    Int_t Jetbin,
					    Int_t LepChannel,
					    Int_t Systematic) {
    //
    // Build Merging Tasks for Ttbar
    //

    // ========================
    //  Standard merging tasks
    // ========================
   
  
    if ( fPlotDir == 0 ) {
	Error("BuildMergingTasks",
	      "Please provide the plot directory! Abort!");
	gSystem->Abort(0);
    }  
  
    // Ttbar ------------------
    AtlMergingTask *task_merge_Ttbar = new AtlMergingTask("MergeTtbar", "MergeTtbar");
    TString *outfileName_Ttbar = BuildOutputFileName( Jetbin, LepChannel,
						      Systematic, "ttbar" );

    TString *outfile_Ttbar = BuildOutputPath( fPlotDir, "",
					      Jetbin, LepChannel,
					      Systematic, "" );
    
    outfile_Ttbar->Append(outfileName_Ttbar->Data());
    
    outfile_Ttbar->ReplaceAll("//","/");
    
    TString *jobHome_Ttbar  = BuildOutputPath( fJobHomeDir, Form("%s/merging", GetName()),
					       Jetbin, LepChannel,
					       Systematic, "ttbar" );    

    // QCD ---------------------
    AtlMergingTask *task_merge_QCD = new AtlMergingTask("MergeQCD", "MergeQCD");
    TString *outfileName_QCD = BuildOutputFileName( Jetbin, LepChannel,
						    Systematic, "QCD" );
    
    TString *outfile_QCD = BuildOutputPath( fPlotDir, "",
					    Jetbin, LepChannel,
					    Systematic, "" );
    
    outfile_QCD->Append(outfileName_QCD->Data());
    
    outfile_QCD->ReplaceAll("//","/");
    
    TString *jobHome_QCD  = BuildOutputPath( fJobHomeDir, Form("%s/merging", GetName()),
					     Jetbin, LepChannel,
					     Systematic, "QCD" );  
    // Loop over samples and create list for merging
    AtlSample *sample = 0;
    TIter next_sample(fListOfSamples);

    TString *ana_outfile;
    TString *ana_outfileName;
    
    while ( (sample = (AtlSample*)next_sample()) ) {
	// Set Outputfile depending on sample type

	// Skip egamma/muon stream if neccessary
	UInt_t lep = 0x1 << LepChannel;
	if ( sample->IsDATAEGamma() && lep == AtlSelector::kMuon ) { continue; }
	if ( sample->IsDATAMuon()   && lep == AtlSelector::kElectron ) { continue; }
	if ( sample->IsQCDEGamma() && lep == AtlSelector::kMuon ) { continue; }
	if ( sample->IsQCDMuon()   && lep == AtlSelector::kElectron ) { continue; }

	TString samplename = sample->GetName();
	if ( (IsSampleSystematic(Systematic) && // a) skip all nominal files
	      !sample->IsSystematicSample()) || 
	     (IsSampleSystematic(Systematic) && // a) skip all files that do not belong
	      !samplename.Contains(GetSystematicName(Systematic))) || // to this systematic
	     (!IsSampleSystematic(Systematic) && // b) skip all systematic samples
	      sample->IsSystematicSample()) )    //    if using nominal or not a sample systematic
	    continue;

	// Get Analysis output file name
	ana_outfile = BuildOutputPath( fPlotDir, "",
				       Jetbin, LepChannel,
				       Systematic, "" );
	
	ana_outfileName = BuildOutputFileName( Jetbin, LepChannel,
					       Systematic,
					       sample->GetName() );
	ana_outfile->Append(ana_outfileName->Data());
	ana_outfile->ReplaceAll("//","/");
	    
	// Add sample to merge list
	if ( sample->IsTtbar() ) {
	    task_merge_Ttbar->AddInputFiles(ana_outfile->Data());
	} else if (sample->IsQCD()) {
	    task_merge_QCD->AddInputFiles(ana_outfile->Data());
	}
	    
	delete ana_outfile;
	delete ana_outfileName;
		
    }
    task_merge_Ttbar->SetOutputFile(outfile_Ttbar->Data());
    task_merge_Ttbar->SetJobHome(jobHome_Ttbar->Data());
    
    task_merge_QCD->SetOutputFile(outfile_QCD->Data());
    task_merge_QCD->SetJobHome(jobHome_QCD->Data());
    
    ParentTask->Add(task_merge_Ttbar);
    if ( Systematic == 0 ) // Add QCD only for nominal
	ParentTask->Add(task_merge_QCD);
}



//____________________________________________________________________


void AtlTopLevelAnalysis::SetAtlasLabelStatus(const char* label) {
    //
    // Sets the Atlas label status of the plotter
    //
    if ( fAtlasLabelStatus != 0 ) {
	delete fAtlasLabelStatus;
    }
    fAtlasLabelStatus = new TString(label);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetCenterOfMassEnergyLabel(const char* label) {
    //
    // Sets the  of the plotter
    //
    if ( fCenterOfMassEnergyLabel != 0 ) {
	delete fCenterOfMassEnergyLabel;
    }
    fCenterOfMassEnergyLabel = new TString(label);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetLuminosityLabel(const char* label) {
    //
    // Sets the luminosity label of the plotter
    //
    if ( fLuminosityLabel != 0 ) {
       delete fLuminosityLabel;
    }
    fLuminosityLabel = new TString(label);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetDrawDataZeroEntryErrors(Bool_t status) {
    //
    // Sets whether the plotter shall draw zero entry errors for data
    //
    fDrawDataZeroEntryErrors = status;
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetDrawMCError(Bool_t status) {
    //
    // Sets whether the plotter shall draw the MC error
    //
    fDrawMCError = status;
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetExportPDF(Bool_t status) {
    //
    // Sets whether the plotter exports a Pdf a file for each canvas
    //
    fExportPDF = status;
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetUseOverflowUnderflow(Bool_t status) {
    //
    // Set flag for drawing the under- and overflow bins
    //
    fUseOverflowUnderflow = status;
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetDrawSignalOverlay(Bool_t status) {
    //
    // Set flag for drawing the signal MC (assumed to be the least
    // entry in the MC histogram stack) as overlay instead as part of
    // the stack
    //
    fDrawSignalOverlay = status;
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetScaleOverlay(Float_t scale) {
    //
    // Scale factor of the overlay histogram
    //
    fScaleOverlay = scale;
}

//____________________________________________________________________

void AtlTopLevelAnalysis::AddNtuplePlotCmd(const char* TreeName,
					   const char* VarExp,
					   const char* Selection,
					   const char* PlotName,
					   const char* PlotTitle, Int_t Nbins,
					   Double_t Xlow, Double_t Xup,
					   const char* XTitle,
					   const char* YTitle) {
    //
    // Add ntuple draw command (see TTree::Draw()) to be used by the
    // plotting task
    //
    fNtuplePlotCmds->Add(new HepNtuplePlotCmd(TreeName, VarExp,
					      Selection, PlotName,
					      PlotTitle, Nbins,
					      Xlow, Xup, XTitle,
					      YTitle));
}

//___________________________________________________________________

const char* AtlTopLevelAnalysis::GetSystematicEnum(Int_t syst) {
    //
    // Get Systematic Enum ('kNOMINAL', ...)
    //
    if ( syst >= fgDynamicSystematicTable.kOffset ) {
        /* dynamic systematics: we don't have an enum item to return,
         * but something that comes very close: */
        return Form("static_cast<AtlTopLevelAnalysis::ESystematic>(AtlTopLevelAnalysis::GetSystematicIdByName(\"%s\"))", GetSystematicName(syst));
    }
    switch ( syst ) {
	case kNOMINAL:
	    return "AtlTopLevelAnalysis::kNOMINAL";
	case kJEFF:
	    return "AtlTopLevelAnalysis::kJEFF";
	case kJER:
	    return "AtlTopLevelAnalysis::kJER";
	case kJES_DOWN:
	    return "AtlTopLevelAnalysis::kJES_DOWN";
	case kJES_UP:
	    return "AtlTopLevelAnalysis::kJES_UP";
	case kJVF_DOWN:
	    return "AtlTopLevelAnalysis::kJVF_DOWN";
	case kJVF_UP:
	    return "AtlTopLevelAnalysis::kJVF_UP";
	case kEER_DOWN:
	    return "AtlTopLevelAnalysis::kEER_DOWN";
	case kEER_UP:
	    return "AtlTopLevelAnalysis::kEER_UP";
	case kEES_DOWN:
	    return "AtlTopLevelAnalysis::kEES_DOWN";
	case kEES_UP:
	    return "AtlTopLevelAnalysis::kEES_UP";
	case kMUID_RES:
	    return "AtlTopLevelAnalysis::kMUID_RES";
	case kMUMS_RES:
	    return "AtlTopLevelAnalysis::kMUMS_RES";
	case kMUSC_DOWN:
	    return "AtlTopLevelAnalysis::kMUSC_DOWN";
	case kMUSC_UP:
	    return "AtlTopLevelAnalysis::kMUSC_UP";
	case kRES_SOFT_DOWN:
	    return "AtlTopLevelAnalysis::kRES_SOFT_DOWN";
	case kRES_SOFT_UP:
	    return "AtlTopLevelAnalysis::kRES_SOFT_UP";
	case kSC_SOFT_DOWN:
	    return "AtlTopLevelAnalysis::kSC_SOFT_DOWN";
	case kSC_SOFT_UP:
	    return "AtlTopLevelAnalysis::kSC_SOFT_UP";
	case kBJES_DOWN:
	    return "AtlTopLevelAnalysis::kBJES_DOWN";
	case kBJES_UP:
	    return "AtlTopLevelAnalysis::kBJES_UP";
	case kETAINTERCALIB_MODEL_DOWN:
	    return "AtlTopLevelAnalysis::kETAINTERCALIB_MODEL_DOWN";
	case kETAINTERCALIB_MODEL_UP:
	    return "AtlTopLevelAnalysis::kETAINTERCALIB_MODEL_UP";
	case kETAINTERCALIB_TOTALSTAT_DOWN:
	    return "AtlTopLevelAnalysis::kETAINTERCALIB_TOTALSTAT_DOWN";
	case kETAINTERCALIB_TOTALSTAT_UP:
	    return "AtlTopLevelAnalysis::kETAINTERCALIB_TOTALSTAT_UP";
	case kJESEFF_DET1_DOWN:
	    return "AtlTopLevelAnalysis::kJESEFF_DET1_DOWN";
	case kJESEFF_DET1_UP:
	    return "AtlTopLevelAnalysis::kJESEFF_DET1_UP";
	case kJESEFF_DET2_DOWN:
	    return "AtlTopLevelAnalysis::kJESEFF_DET2_DOWN";
	case kJESEFF_DET2_UP:
	    return "AtlTopLevelAnalysis::kJESEFF_DET2_UP";
	case kJESEFF_DET3_DOWN:
	    return "AtlTopLevelAnalysis::kJESEFF_DET3_DOWN";
	case kJESEFF_DET3_UP:
	    return "AtlTopLevelAnalysis::kJESEFF_DET3_UP";
	case kJESEFF_MIX1_DOWN:
	    return "AtlTopLevelAnalysis::kJESEFF_MIX1_DOWN";
	case kJESEFF_MIX1_UP:
	    return "AtlTopLevelAnalysis::kJESEFF_MIX1_UP";
	case kJESEFF_MIX2_DOWN:
	    return "AtlTopLevelAnalysis::kJESEFF_MIX2_DOWN";
	case kJESEFF_MIX2_UP:
	    return "AtlTopLevelAnalysis::kJESEFF_MIX2_UP";
	case kJESEFF_MIX3_DOWN:
	    return "AtlTopLevelAnalysis::kJESEFF_MIX3_DOWN";
	case kJESEFF_MIX3_UP:
	    return "AtlTopLevelAnalysis::kJESEFF_MIX3_UP";
	case kJESEFF_MIX4_DOWN:
	    return "AtlTopLevelAnalysis::kJESEFF_MIX4_DOWN";
	case kJESEFF_MIX4_UP:
	    return "AtlTopLevelAnalysis::kJESEFF_MIX4_UP";
	case kJESEFF_MODEL1_DOWN:
	    return "AtlTopLevelAnalysis::kJESEFF_MODEL1_DOWN";
	case kJESEFF_MODEL1_UP:
	    return "AtlTopLevelAnalysis::kJESEFF_MODEL1_UP";
	case kJESEFF_MODEL2_DOWN:
	    return "AtlTopLevelAnalysis::kJESEFF_MODEL2_DOWN";
	case kJESEFF_MODEL2_UP:
	    return "AtlTopLevelAnalysis::kJESEFF_MODEL2_UP";
	case kJESEFF_MODEL3_DOWN:
	    return "AtlTopLevelAnalysis::kJESEFF_MODEL3_DOWN";
	case kJESEFF_MODEL3_UP:
	    return "AtlTopLevelAnalysis::kJESEFF_MODEL3_UP";
	case kJESEFF_MODEL4_DOWN:
	    return "AtlTopLevelAnalysis::kJESEFF_MODEL4_DOWN";
	case kJESEFF_MODEL4_UP:
	    return "AtlTopLevelAnalysis::kJESEFF_MODEL4_UP";
	case kJESEFF_STAT1_DOWN:
	    return "AtlTopLevelAnalysis::kJESEFF_STAT1_DOWN";
	case kJESEFF_STAT1_UP:
	    return "AtlTopLevelAnalysis::kJESEFF_STAT1_UP";
	case kJESEFF_STAT2_DOWN:
	    return "AtlTopLevelAnalysis::kJESEFF_STAT2_DOWN";
	case kJESEFF_STAT2_UP:
	    return "AtlTopLevelAnalysis::kJESEFF_STAT2_UP";
	case kJESEFF_STAT3_DOWN:
	    return "AtlTopLevelAnalysis::kJESEFF_STAT3_DOWN";
	case kJESEFF_STAT3_UP:
	    return "AtlTopLevelAnalysis::kJESEFF_STAT3_UP";
	case kJESEFF_STAT4_DOWN:
	    return "AtlTopLevelAnalysis::kJESEFF_STAT4_DOWN";
	case kJESEFF_STAT4_UP:
	    return "AtlTopLevelAnalysis::kJESEFF_STAT4_UP";
	case kJESPILEUP_OFFMU_DOWN:
	    return "AtlTopLevelAnalysis::kJESPILEUP_OFFMU_DOWN";
	case kJESPILEUP_OFFMU_UP:
	    return "AtlTopLevelAnalysis::kJESPILEUP_OFFMU_UP";
	case kJESPILEUP_OFFNPV_DOWN:
	    return "AtlTopLevelAnalysis::kJESPILEUP_OFFNPV_DOWN";
	case kJESPILEUP_OFFNPV_UP:
	    return "AtlTopLevelAnalysis::kJESPILEUP_OFFNPV_UP";
	case kJESPILEUP_PT_DOWN:
	    return "AtlTopLevelAnalysis::kJESPILEUP_PT_DOWN";
	case kJESPILEUP_PT_UP:
	    return "AtlTopLevelAnalysis::kJESPILEUP_PT_UP";
	case kJESPILEUP_RHO_DOWN:
	    return "AtlTopLevelAnalysis::kJESPILEUP_RHO_DOWN";
	case kJESPILEUP_RHO_UP:
	    return "AtlTopLevelAnalysis::kJESPILEUP_RHO_UP";
	case kSINGLEPART_DOWN:
	    return "AtlTopLevelAnalysis::kSINGLEPART_DOWN";
	case kSINGLEPART_UP:
	    return "AtlTopLevelAnalysis::kSINGLEPART_UP";
	case kFLAVOR_COMP_DOWN:
	    return "AtlTopLevelAnalysis::kFLAVOR_COMP_DOWN";
	case kFLAVOR_COMP_UP:
	    return "AtlTopLevelAnalysis::kFLAVOR_COMP_UP";
	case kFLAVOR_RESP_DOWN:
	    return "AtlTopLevelAnalysis::kFLAVOR_RESP_DOWN";
	case kFLAVOR_RESP_UP:
	    return "AtlTopLevelAnalysis::kFLAVOR_RESP_UP";
	case kPUNCH_THROUGH_DOWN:
	    return "AtlTopLevelAnalysis::kPUNCH_THROUGH_DOWN";
	case kPUNCH_THROUGH_UP:
	    return "AtlTopLevelAnalysis::kPUNCH_THROUGH_UP";
	case kLEP_RECO_SF_DOWN:
	    return "AtlTopLevelAnalysis::kLEP_RECO_SF_DOWN";
	case kLEP_RECO_SF_UP:
	    return "AtlTopLevelAnalysis::kLEP_RECO_SF_UP";
	case kLEP_ID_SF_DOWN:
	    return "AtlTopLevelAnalysis::kLEP_ID_SF_DOWN";
	case kLEP_ID_SF_UP:
	    return "AtlTopLevelAnalysis::kLEP_ID_SF_UP";
	case kLEP_TRIG_SF_DOWN:
	    return "AtlTopLevelAnalysis::kLEP_TRIG_SF_DOWN";
	case kLEP_TRIG_SF_UP:
	    return "AtlTopLevelAnalysis::kLEP_TRIG_SF_UP";
	case kBTAGSF_DOWN:
	    return "AtlTopLevelAnalysis::kBTAGSF_DOWN";
	case kBTAGSF_UP:
	    return "AtlTopLevelAnalysis::kBTAGSF_UP";
	case kCTAGSF_DOWN:
	    return "AtlTopLevelAnalysis::kCTAGSF_DOWN";
	case kCTAGSF_UP:
	    return "AtlTopLevelAnalysis::kCTAGSF_UP";
	case kMISTAGSF_DOWN:
	    return "AtlTopLevelAnalysis::kMISTAGSF_DOWN";
	case kMISTAGSF_UP:
	    return "AtlTopLevelAnalysis::kMISTAGSF_UP";
	    // btag
	case kBTAGSF_DOWN_BREAK0:
	    return "AtlTopLevelAnalysis::kBTAGSF_DOWN_BREAK0";
	case kBTAGSF_DOWN_BREAK1:
	    return "AtlTopLevelAnalysis::kBTAGSF_DOWN_BREAK1";
	case kBTAGSF_DOWN_BREAK2:
	    return "AtlTopLevelAnalysis::kBTAGSF_DOWN_BREAK2";
	case kBTAGSF_DOWN_BREAK3:
	    return "AtlTopLevelAnalysis::kBTAGSF_DOWN_BREAK3";
	case kBTAGSF_DOWN_BREAK4:
	    return "AtlTopLevelAnalysis::kBTAGSF_DOWN_BREAK4";
	case kBTAGSF_DOWN_BREAK5:
	    return "AtlTopLevelAnalysis::kBTAGSF_DOWN_BREAK5";
	case kBTAGSF_DOWN_BREAK6:
	    return "AtlTopLevelAnalysis::kBTAGSF_DOWN_BREAK6";
	case kBTAGSF_DOWN_BREAK7:
	    return "AtlTopLevelAnalysis::kBTAGSF_DOWN_BREAK7";
	case kBTAGSF_DOWN_BREAK8:
	    return "AtlTopLevelAnalysis::kBTAGSF_DOWN_BREAK8";
	    //
	case kBTAGSF_UP_BREAK0:
	    return "AtlTopLevelAnalysis::kBTAGSF_UP_BREAK0";
	case kBTAGSF_UP_BREAK1:
	    return "AtlTopLevelAnalysis::kBTAGSF_UP_BREAK1";
	case kBTAGSF_UP_BREAK2:
	    return "AtlTopLevelAnalysis::kBTAGSF_UP_BREAK2";
	case kBTAGSF_UP_BREAK3:
	    return "AtlTopLevelAnalysis::kBTAGSF_UP_BREAK3";
	case kBTAGSF_UP_BREAK4:
	    return "AtlTopLevelAnalysis::kBTAGSF_UP_BREAK4";
	case kBTAGSF_UP_BREAK5:
	    return "AtlTopLevelAnalysis::kBTAGSF_UP_BREAK5";
	case kBTAGSF_UP_BREAK6:
	    return "AtlTopLevelAnalysis::kBTAGSF_UP_BREAK6";
	case kBTAGSF_UP_BREAK7:
	    return "AtlTopLevelAnalysis::kBTAGSF_UP_BREAK7";
	case kBTAGSF_UP_BREAK8:
	    return "AtlTopLevelAnalysis::kBTAGSF_UP_BREAK8";
	    // ctag	    
	case kCTAGSF_DOWN_BREAK0:
	    return "AtlTopLevelAnalysis::kCTAGSF_DOWN_BREAK0";
	case kCTAGSF_DOWN_BREAK1:
	    return "AtlTopLevelAnalysis::kCTAGSF_DOWN_BREAK1";
	case kCTAGSF_DOWN_BREAK2:
	    return "AtlTopLevelAnalysis::kCTAGSF_DOWN_BREAK2";
	case kCTAGSF_DOWN_BREAK3:
	    return "AtlTopLevelAnalysis::kCTAGSF_DOWN_BREAK3";
	case kCTAGSF_DOWN_BREAK4:
	    return "AtlTopLevelAnalysis::kCTAGSF_DOWN_BREAK4";
	case kCTAGSF_DOWN_BREAK5:
	    return "AtlTopLevelAnalysis::kCTAGSF_DOWN_BREAK5";
	case kCTAGSF_DOWN_BREAK6:
	    return "AtlTopLevelAnalysis::kCTAGSF_DOWN_BREAK6";
	    //
	case kCTAGSF_UP_BREAK0:
	    return "AtlTopLevelAnalysis::kCTAGSF_UP_BREAK0";
	case kCTAGSF_UP_BREAK1:
	    return "AtlTopLevelAnalysis::kCTAGSF_UP_BREAK1";
	case kCTAGSF_UP_BREAK2:
	    return "AtlTopLevelAnalysis::kCTAGSF_UP_BREAK2";
	case kCTAGSF_UP_BREAK3:
	    return "AtlTopLevelAnalysis::kCTAGSF_UP_BREAK3";
	case kCTAGSF_UP_BREAK4:
	    return "AtlTopLevelAnalysis::kCTAGSF_UP_BREAK4";
	case kCTAGSF_UP_BREAK5:
	    return "AtlTopLevelAnalysis::kCTAGSF_UP_BREAK5";
	case kCTAGSF_UP_BREAK6:
	    return "AtlTopLevelAnalysis::kCTAGSF_UP_BREAK6";
	    // mistag
	case kMISTAGSF_DOWN_BREAK0:
	    return "AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK0";
	case kMISTAGSF_DOWN_BREAK1:
	    return "AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK1";
	case kMISTAGSF_DOWN_BREAK2:
	    return "AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK2";
	case kMISTAGSF_DOWN_BREAK3:
	    return "AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK3";
	case kMISTAGSF_DOWN_BREAK4:
	    return "AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK4";
	case kMISTAGSF_DOWN_BREAK5:
	    return "AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK5";
	case kMISTAGSF_DOWN_BREAK6:
	    return "AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK6";
	case kMISTAGSF_DOWN_BREAK7:
	    return "AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK7";
	case kMISTAGSF_DOWN_BREAK8:
	    return "AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK8";
	case kMISTAGSF_DOWN_BREAK9:
	    return "AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK9";
	case kMISTAGSF_DOWN_BREAK10:
	    return "AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK10";
	case kMISTAGSF_DOWN_BREAK11:
	    return "AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK11";
	    //
	case kMISTAGSF_UP_BREAK0:
	    return "AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK0";
	case kMISTAGSF_UP_BREAK1:
	    return "AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK1";
	case kMISTAGSF_UP_BREAK2:
	    return "AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK2";
	case kMISTAGSF_UP_BREAK3:
	    return "AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK3";
	case kMISTAGSF_UP_BREAK4:
	    return "AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK4";
	case kMISTAGSF_UP_BREAK5:
	    return "AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK5";
	case kMISTAGSF_UP_BREAK6:
	    return "AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK6";
	case kMISTAGSF_UP_BREAK7:
	    return "AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK7";
	case kMISTAGSF_UP_BREAK8:
	    return "AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK8";
	case kMISTAGSF_UP_BREAK9:
	    return "AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK9";
	case kMISTAGSF_UP_BREAK10:
	    return "AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK10";
	case kMISTAGSF_UP_BREAK11:
	    return "AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK11";
	case kWtDS:
	    return "AtlTopLevelAnalysis::kWtDS";
	case kMCatNLO:
	    return "AtlTopLevelAnalysis::kMCatNLO";
	case kMCatNLO_SgTop:
	    return "AtlTopLevelAnalysis::kMCatNLO_SgTop";
	case kMCatNLO_tchan:
	    return "AtlTopLevelAnalysis::kMCatNLO_tchan";
	case kMCatNLO_schan:
	    return "AtlTopLevelAnalysis::kMCatNLO_schan";
	case kMCatNLO_wt:
	    return "AtlTopLevelAnalysis::kMCatNLO_wt";
	case kMCatNLO_ttbar:
	    return "AtlTopLevelAnalysis::kMCatNLO_ttbar";
	case kaMCatNLO_tchan:
	    return "AtlTopLevelAnalysis::kaMCatNLO_tchan";
	case kaMCatNLO_schan:
	    return "AtlTopLevelAnalysis::kaMCatNLO_schan";
	case kPowhegPythia_t:
	    return "AtlTopLevelAnalysis::kPowhegPythia_t";
	case kPowhegPythia2011C_t:
	    return "AtlTopLevelAnalysis::kPowhegPythia2011C_t";
	case kPowhegHerwig_t:
	    return "AtlTopLevelAnalysis::kPowhegHerwig_t";
	case kISRFSR_ttbar_More:
	    return "AtlTopLevelAnalysis::kISRFSR_ttbar_More";
	case kISRFSR_ttbar_Less:
	    return "AtlTopLevelAnalysis::kISRFSR_ttbar_Less";
	case kRadHi_ttbar:
	    return "AtlTopLevelAnalysis::kRadHi_ttbar";
	case kRadLo_ttbar:
	    return "AtlTopLevelAnalysis::kRadLo_ttbar";
	case kScaleVar1:
	    return "AtlTopLevelAnalysis::kScaleVar1";
	case kScaleVar2:
	    return "AtlTopLevelAnalysis::kScaleVar2";
	case kScaleVar3:
	    return "AtlTopLevelAnalysis::kScaleVar3";
	case kScaleVar4:
	    return "AtlTopLevelAnalysis::kScaleVar4";
	case kScaleVar5:
	    return "AtlTopLevelAnalysis::kScaleVar5";
	case kScaleVar6:
	    return "AtlTopLevelAnalysis::kScaleVar6";
	case kScaleVar_tchan1:
	    return "AtlTopLevelAnalysis::kScaleVar_tchan1";
	case kScaleVar_tchan2:
	    return "AtlTopLevelAnalysis::kScaleVar_tchan2";
	case kScaleVar_tchan3:
	    return "AtlTopLevelAnalysis::kScaleVar_tchan3";
	case kScaleVar_tchan4:
	    return "AtlTopLevelAnalysis::kScaleVar_tchan4";
	case kWjetsGen:
	    return "AtlTopLevelAnalysis::kWjetsGen";
	case kPdf_up:
	    return "AtlTopLevelAnalysis::kPdf_up";
	case kPdf_down:
	    return "AtlTopLevelAnalysis::kPdf_down";
	case kLEP_FAKE_BDECAYGEN_SHAPE:
	    return "AtlTopLevelAnalysis::kLEP_FAKE_BDECAYGEN_SHAPE";
	    // the end
	default:
	    cout << "<AtlTopLevelAnalysis::GetSystematicEnum>: Systematic not known! Abort!" << endl;
	    gSystem->Abort(0);
    }
    return "";
}

//___________________________________________________________________

Bool_t AtlTopLevelAnalysis::IsJESComponentSystematic(Int_t Systematic) {
    //
    // JES component systematic?
    //
    return ( (Systematic == kBJES_DOWN) ||
	     (Systematic == kBJES_UP)   ||
	     (Systematic == kETAINTERCALIB_MODEL_DOWN) ||
	     (Systematic == kETAINTERCALIB_MODEL_UP)   ||
	     (Systematic == kETAINTERCALIB_TOTALSTAT_DOWN) ||
	     (Systematic == kETAINTERCALIB_TOTALSTAT_UP)   ||
	     (Systematic == kJESEFF_DET1_DOWN) ||
	     (Systematic == kJESEFF_DET1_UP)   ||
	     (Systematic == kJESEFF_DET2_DOWN) ||
	     (Systematic == kJESEFF_DET2_UP)   ||
	     (Systematic == kJESEFF_DET3_DOWN) ||
	     (Systematic == kJESEFF_DET3_UP)   ||
	     (Systematic == kJESEFF_MIX1_DOWN) ||
	     (Systematic == kJESEFF_MIX1_UP)   ||
	     (Systematic == kJESEFF_MIX2_DOWN) ||
	     (Systematic == kJESEFF_MIX2_UP)   ||
	     (Systematic == kJESEFF_MIX3_DOWN) ||
	     (Systematic == kJESEFF_MIX3_UP)   ||
	     (Systematic == kJESEFF_MIX4_DOWN) ||
	     (Systematic == kJESEFF_MIX4_UP)   ||
	     (Systematic == kJESEFF_MODEL1_DOWN) ||
	     (Systematic == kJESEFF_MODEL1_UP)   ||
	     (Systematic == kJESEFF_MODEL2_DOWN) ||
	     (Systematic == kJESEFF_MODEL2_UP)   ||
	     (Systematic == kJESEFF_MODEL3_DOWN) ||
	     (Systematic == kJESEFF_MODEL3_UP)   ||
	     (Systematic == kJESEFF_MODEL4_DOWN) ||
	     (Systematic == kJESEFF_MODEL4_UP)   ||
	     (Systematic == kJESEFF_STAT1_DOWN) ||
	     (Systematic == kJESEFF_STAT1_UP)   ||
	     (Systematic == kJESEFF_STAT2_DOWN) ||
	     (Systematic == kJESEFF_STAT2_UP)   ||
	     (Systematic == kJESEFF_STAT3_DOWN) ||
	     (Systematic == kJESEFF_STAT3_UP)   ||
	     (Systematic == kJESEFF_STAT4_DOWN) ||
	     (Systematic == kJESEFF_STAT4_UP)   ||
	     (Systematic == kJESPILEUP_OFFMU_DOWN) ||
	     (Systematic == kJESPILEUP_OFFMU_UP)   ||
	     (Systematic == kJESPILEUP_OFFNPV_DOWN) ||
	     (Systematic == kJESPILEUP_OFFNPV_UP)   ||
	     (Systematic == kJESPILEUP_PT_DOWN) ||
	     (Systematic == kJESPILEUP_PT_UP)   ||
	     (Systematic == kJESPILEUP_RHO_DOWN) ||
	     (Systematic == kJESPILEUP_RHO_UP)   ||
	     (Systematic == kSINGLEPART_DOWN) ||
	     (Systematic == kSINGLEPART_UP)   ||
	     (Systematic == kFLAVOR_COMP_DOWN) ||
	     (Systematic == kFLAVOR_COMP_UP)   ||
	     (Systematic == kFLAVOR_RESP_DOWN) ||
	     (Systematic == kFLAVOR_RESP_UP)   ||
	     (Systematic == kPUNCH_THROUGH_DOWN) ||
	     (Systematic == kPUNCH_THROUGH_UP)     ) ? kTRUE : kFALSE;
}

//___________________________________________________________________

Bool_t AtlTopLevelAnalysis::IsScaleFactorSystematic(Int_t Systematic) {
    //
    // Scale factor systematic?
    //
    if (Systematic >= fgDynamicSystematicTable.kOffset) {
        TString name(fgDynamicSystematicTable.GetNameById(Systematic));
        return ( name.BeginsWith("EL_SF_") ||
                 name.BeginsWith("MU_SF_") ||
                 name.BeginsWith("Pileup_SF_") ||
                 name.BeginsWith("JVT_SF_") ||
				 name.BeginsWith("ForwardJVT_SF_") ||
                 name.BeginsWith("bTagSF_") ||
                 name.BeginsWith("MCWEIGHTS_")||
                 name.BeginsWith("MCWEIGHT_"));
    } else {
        assert(Systematic >= 0);
        assert(Systematic < fgNumSystematics);
        return ( (Systematic == kLEP_RECO_SF_DOWN) ||
                 (Systematic == kLEP_RECO_SF_UP)   ||
                 (Systematic == kLEP_ID_SF_DOWN)   ||
                 (Systematic == kLEP_ID_SF_UP)     ||
                 (Systematic == kLEP_TRIG_SF_DOWN) ||
                 (Systematic == kLEP_TRIG_SF_UP)   ||
                 (Systematic == kBTAGSF_DOWN)   ||
                 (Systematic == kBTAGSF_UP)     ||
                 (Systematic == kCTAGSF_DOWN)   ||
                 (Systematic == kCTAGSF_UP)     ||
                 (Systematic == kMISTAGSF_DOWN) ||
                 (Systematic == kMISTAGSF_UP) ) ? kTRUE : kFALSE;
    }
}

//___________________________________________________________________

Bool_t AtlTopLevelAnalysis::IsSampleSystematic(Int_t Systematic) {
    //
    // Sample systematic?
    //
    return ( (Systematic == kWtDS) ||
	     (Systematic == kMCatNLO)   ||
	     (Systematic == kMCatNLO_SgTop)  ||
	     (Systematic == kMCatNLO_tchan)  ||
	     (Systematic == kMCatNLO_schan)  ||
	     (Systematic == kMCatNLO_wt)     ||
	     (Systematic == kMCatNLO_ttbar)  ||
	     (Systematic == kaMCatNLO_tchan) ||
	     (Systematic == kaMCatNLO_schan) ||
	     (Systematic == kPowhegPythia_t) ||
	     (Systematic == kPowhegPythia2011C_t) ||
	     (Systematic == kPowhegHerwig_t) ||
	     (Systematic == kISRFSR_ttbar_More) ||
	     (Systematic == kISRFSR_ttbar_Less) ||
	     (Systematic == kRadHi_ttbar) ||
	     (Systematic == kRadLo_ttbar) ||
	     (Systematic == kScaleVar1) ||
	     (Systematic == kScaleVar2) ||
	     (Systematic == kScaleVar3) ||
	     (Systematic == kScaleVar4) ||
	     (Systematic == kScaleVar5) ||
	     (Systematic == kScaleVar6) ||
	     (Systematic == kWjetsGen)  ||
	     (Systematic == kScaleVar_tchan1) ||
	     (Systematic == kScaleVar_tchan2) ||
	     (Systematic == kScaleVar_tchan3) ||
	     (Systematic == kScaleVar_tchan4) ) ? kTRUE : kFALSE;
}

//___________________________________________________________________

Bool_t AtlTopLevelAnalysis::IsBTagEVScaleFactorSystematic(Int_t Systematic) {
    //
    // Scale factor systematic?
    //
    return ( (Systematic >= kBTAGSF_UP_BREAK0 &&
	      Systematic <= kBTAGSF_UP_BREAK8 ) ||
	     (Systematic >= kBTAGSF_DOWN_BREAK0 &&
	      Systematic <= kBTAGSF_DOWN_BREAK8 ) ||
	     (Systematic >= kCTAGSF_UP_BREAK0 &&
	      Systematic <= kCTAGSF_UP_BREAK6 ) ||
	     (Systematic >= kCTAGSF_DOWN_BREAK0 &&
	      Systematic <= kCTAGSF_DOWN_BREAK6 ) ||
	     (Systematic >= kMISTAGSF_UP_BREAK0 &&
	      Systematic <= kMISTAGSF_UP_BREAK11 ) ||
	     (Systematic >= kMISTAGSF_DOWN_BREAK0 &&
	      Systematic <= kMISTAGSF_DOWN_BREAK11 ) ) ? kTRUE : kFALSE;
}

//___________________________________________________________________

Bool_t AtlTopLevelAnalysis::GetTaskStatus(TString *jobHome,
					  TString *outfileName,
					  TString *outfile,
                                          Bool_t require_success) {
    //
    // Get status of the task
    //
    // Returns kTRUE in case of a successful job.
    // 
	
    if ( fForceRetry ) return kFALSE;
	
    // Retry option:
    // -------------
    // Do not add the task if ...
    // - ... the outputfile exists AND
    // - ... the job was successful (see logfile)
    //       (i.e. Job Summary was printed)
    //
    // However don't overwrite if it exists but does not have a logfile
    //   (e.g. created on grid and copied without logfile)

    // Check outputfile existence
    // GetPathInfo returns 0 on successful stat of file
    Long_t id,size,flags,mt;
    Bool_t outfile_exists = (gSystem->GetPathInfo(outfile->Data(), &id, &size, &flags, &mt) == 0);
    if ( !outfile_exists || (outfile_exists && (size == 0)) ) {
        if ( gDebug > 0 )
            Info(__FUNCTION__, "Output file %s not found", outfile->Data());
        return kFALSE;
    }

    // Check logfile existence
    TString logfile(jobHome->Data());
    logfile.Append( outfileName->Data() );
    logfile.ReplaceAll(".root", 5, ".log", 4);
    Bool_t logfile_exists = (gSystem->GetPathInfo(logfile.Data(), &id, &size, &flags, &mt) == 0);

    // Safeguard against overwriting copied output files
    if ( outfile_exists && !logfile_exists ) {
        Info(__FUNCTION__, "Output file already available without log; "
             "skipping job for file %s", outfileName->Data());
        return kTRUE;
    }

    // Check result in logfile
    TString jobstatus = TString();
    if ( logfile_exists ) {
        FILE *flog;
        if ( require_success ) {
            flog = gSystem->OpenPipe(
		Form("tac %s | grep -m1 \"Job status\" | grep -c successful ",
		     logfile.Data() ), "r" );
        }
        else {
            flog = gSystem->OpenPipe(
                Form("grep -c \"Job Summary\" %s",
                     logfile.Data() ), "r" );
        }
	jobstatus.Gets(flog);
	gSystem->ClosePipe(flog);
    }

    // Evaluate job status
    if ( outfile_exists && logfile_exists && jobstatus.Contains("1") ) {
        Info(__FUNCTION__, "Already done; skipping job for file %s",
             outfileName->Data());
	return kTRUE;
    }

    Info(__FUNCTION__, "Job not successful for file %s", outfileName->Data());
    return kFALSE;    
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetGridRootVersion(const char* RootVersion) {
    //
    // Set grid root version
    //
    if ( fGridRootVersion != 0 ) delete fGridRootVersion;
    fGridRootVersion = new TString(RootVersion);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetGridCmtVersion(const char* CmtVersion) {
    //
    // Set grid cmt version
    //
    if ( fGridCmtVersion != 0 ) delete fGridCmtVersion;
    fGridCmtVersion = new TString(CmtVersion);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetGridUserName(const char* UserName) {
    //
    // Set grid user name
    //
    if ( fGridUserName != 0 ) delete fGridUserName;
    fGridUserName = new TString(UserName);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetGridSuffix(const char* Suffix) {
    //
    // Set grid suffix for <id>.<suffix> (e.g. "v1")
    //
    if ( fGridSuffix != 0 ) delete fGridSuffix;
    fGridSuffix = new TString(Suffix);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetGridIdSuffix(const char* Suffix) {
    //
    // Set grid id suffix, i.e. an optional string to decorate the id part
    //   of the grid name without disturbing the sortable suffix part.
    //
    if ( nullptr != fGridIdSuffix ) delete fGridIdSuffix;
    fGridIdSuffix = new TString(Suffix);
}

//____________________________________________________________________

void AtlTopLevelAnalysis::SetMemTkRunScript(const char* script) {
    //
    // Set the MemTk run script for the AtlMemTkAnalysisTask
    //
    if ( fMemTkRunScript != 0 ) delete fMemTkRunScript;
    fMemTkRunScript = new TString(script);
}

//____________________________________________________________________

Bool_t AtlTopLevelAnalysis::IsIgnored(AtlSample * sample) {
    //
    // Whether the sample should be ignored by the Build*Tasks methods.
    // Doesn't take lepton flavour into account.
    //
    if ( GetCampaign() > 13 ) {
        if ( !fUseHforSamples )
            return kFALSE;
        Fatal(__FUNCTION__, "HFOR splitting isn't supported anymore.");
        return kTRUE;
    }
    if ( fUseHforSamples )
        return (sample->IsWjetsLight() || sample->IsWjetsHeavy());
    return (sample->IsHFOR0() || sample->IsHFOR1() || sample->IsHFOR2() || sample->IsHFOR3());
}

//____________________________________________________________________

void AtlTopLevelAnalysis::ReadSampleSizeList(const char* filename) {
    //
    // Read sample sizes from input file
    //
    // How to get sample sizes? use b) for now
    // a) loop over samples on disk, read GetEntries() from tree, add up
    //    + guaranteed to give correct number
    //    + no external script overhead
    //    - complicated to implement another loop here, possibly slow
    // b) take log output from cut analysis run
    //    + easy to implement with sh script similar to ReadSampleList
    //    - feels hackish

    Info(__FUNCTION__, "Reading sample sizes from file %s", filename);
    
    // FOR NOW: simply give filename of already prepared event count
    // better: have the script in ../Utils and use default locations to
    //   save the readout; this is better because event numbers could
    //   change for diff. systematics, and have to be calculated anyway
    //   for each new derivation, cut definition and/or region change
    TString SampleSizeListFile(filename);
    if ( SampleSizeListFile.IsNull() ) {
        Error(__FUNCTION__, "This feature is not yet implemented. You have to provide a filename. Abort!");
        gSystem->Abort(0);
    }

    // read from file similar to ReadSampleList and add to fSampleSizes
    ifstream SampleSizeList(SampleSizeListFile.Data());
    Int_t n_entries = 0;
    TString line;
    while ( !SampleSizeList.eof() ) {
        line.ReadLine(SampleSizeList);
        if ( line.BeginsWith("#") || line.Length() == 0 ) continue;
        std::unique_ptr<TObjArray> tokens(line.Tokenize(" "));
        TString name   = ((TObjString*)tokens->At(0))->GetString();
        Int_t n_events = ((TObjString*)tokens->At(1))->GetString().Atoi();
        //Float_t nev_weighted = ((TObjString*)tokens->At(2))->GetString().Atof();
        TObject * sample = fSampleSizes->FindObject(name);
        if ( sample == nullptr ) {
            fSampleSizes->Add(new TNamed(name.Data(), Form("%d", n_events)));
            n_entries++;
        }
        else {
            Error(__FUNCTION__, "Sample %s was already added! Abort!", name.Data());
            gSystem->Abort(0);
        }
    }
    Info(__FUNCTION__, "Read %d entries from %s", n_entries, SampleSizeListFile.Data());
    SampleSizeList.close();

}

//____________________________________________________________________

Int_t AtlTopLevelAnalysis::GetMCWeightPosition(const char* sample_name,
                                               const char* weight_name) {
    //
    // Read MC weight position from file
    //
    // The file to read from is fMCWeightNamesFile, to be set via
    // AtlTopLevelAnalysis::SetMCWeightNamesFile(filename).
    //
    if ( gDebug > 0 ) {
        Info(__FUNCTION__, "Getting position for weight %s in sample %s",
             weight_name, sample_name);
    }

    if ( fMCWeightNamesFile.IsNull() ) {
        Fatal(__FUNCTION__, "fMCWeightNamesFile is not set. Abort!");
    }

    Int_t wnum_out = -1;
    TString sample_in(sample_name);
    TString wname_in(weight_name);

    // read from file
    ifstream infile(fMCWeightNamesFile.Data());
    TString line;
    while ( !infile.eof() ) {
        line.ReadLine(infile);
        if ( line.BeginsWith("#") || line.Length() == 0 ) continue;
        std::unique_ptr<TObjArray> tokens(line.Tokenize(" "));
        TString sample = ((TObjString*)tokens->At(0))->GetString();
        Int_t wnum = ((TObjString*)tokens->At(1))->GetString().Atoi();
        TString wname = ((TObjString*)tokens->At(2))->GetString();
        // Double_t total = ((TObjString*)tokens->At(3))->GetString().Atof();

        // search until the correct combination has been found
        if ( sample.EqualTo(sample_in) and wname.EqualTo(wname_in) ) {
            wnum_out = wnum;
            break;
        }
    }

    // Report success/failure
    if ( wnum_out == -1 ) {
        Error(__FUNCTION__, "Could not find position for sample %s and weight %s in %s. Skip!",
              sample_name, weight_name, fMCWeightNamesFile.Data());
    } else if ( gDebug > 0 ) {
        Info(__FUNCTION__, "Using position %d.", wnum_out);
    }
    return wnum_out;
}

//____________________________________________________________________

Double_t AtlTopLevelAnalysis::GetMCWeightTotalEvents(const char* sample_name,
                                                     const char* weight_name) {
    //
    // Read MC weight totalEventsWeighted from file
    //
    // The file to read from is fMCWeightNamesFile, to be set via
    // AtlTopLevelAnalysis::SetMCWeightNamesFile(filename).
    //
    if ( gDebug > 0 ) {
        Info(__FUNCTION__, "Getting totalEventsWeighted for weight %s in sample %s",
             weight_name, sample_name);
    }

    if ( fMCWeightNamesFile.IsNull() ) {
        Fatal(__FUNCTION__, "fMCWeightNamesFile is not set. Abort!");
    }

    Double_t total_out = -1.;
    TString sample_in(sample_name);
    TString wname_in(weight_name);

    // read from file
    ifstream infile(fMCWeightNamesFile.Data());
    TString line;
    while ( !infile.eof() ) {
        line.ReadLine(infile);
        if ( line.BeginsWith("#") || line.Length() == 0 ) continue;
        std::unique_ptr<TObjArray> tokens(line.Tokenize(" "));
        TString sample = ((TObjString*)tokens->At(0))->GetString();
        // Int_t wnum = ((TObjString*)tokens->At(1))->GetString().Atoi();
        TString wname = ((TObjString*)tokens->At(2))->GetString();
        Double_t total = ((TObjString*)tokens->At(3))->GetString().Atof();

        // search until the correct combination has been found
        if ( sample.EqualTo(sample_in) and wname.EqualTo(wname_in) ) {
            total_out = total;
            break;
        }
    }

    // Report success/failure
    if ( total_out < 0 ) {
        Error(__FUNCTION__, "Could not find totalEventsWeighted for sample %s and weight %s in %s. Skip!",
              sample_name, weight_name, fMCWeightNamesFile.Data());
    } else if ( gDebug > 0 ) {
        Info(__FUNCTION__, "Using totalEventsWeighted=%f.", total_out);
    }
    return total_out;
}

//____________________________________________________________________

Double_t AtlTopLevelAnalysis::GetMCWeightTotalEvents(const char* sample_name,
                                                     Int_t weight_num) {
    //
    // Read MC weight totalEventsWeighted from file
    //
    // The file to read from is fMCWeightNamesFile, to be set via
    // AtlTopLevelAnalysis::SetMCWeightNamesFile(filename).
    //
    if ( gDebug > 0 ) {
        Info(__FUNCTION__, "Getting totalEventsWeighted for weight number %d in sample %s",
             weight_num, sample_name);
    }

    if ( fMCWeightNamesFile.IsNull() ) {
        Fatal(__FUNCTION__, "fMCWeightNamesFile is not set. Abort!");
    }

    Double_t total_out = -1.;
    TString sample_in(sample_name);

    // read from file
    ifstream infile(fMCWeightNamesFile.Data());
    TString line;
    while ( !infile.eof() ) {
        line.ReadLine(infile);
        if ( line.BeginsWith("#") || line.Length() == 0 ) continue;
        std::unique_ptr<TObjArray> tokens(line.Tokenize(" "));
        TString sample = ((TObjString*)tokens->At(0))->GetString();
        Int_t wnum = ((TObjString*)tokens->At(1))->GetString().Atoi();
        // TString wname = ((TObjString*)tokens->At(2))->GetString();
        Double_t total = ((TObjString*)tokens->At(3))->GetString().Atof();

        // search until the correct combination has been found
        if ( sample.EqualTo(sample_in) and wnum == weight_num ) {
            total_out = total;
            break;
        }
    }

    // Report success/failure
    if ( total_out < 0 ) {
        Error(__FUNCTION__, "Could not find totalEventsWeighted for sample %s and weight number %d in %s. Skip!",
              sample_name, weight_num, fMCWeightNamesFile.Data());
    } else if ( gDebug > 0 ) {
        Info(__FUNCTION__, "Using totalEventsWeighted=%f.", total_out);
    }
    return total_out;
}

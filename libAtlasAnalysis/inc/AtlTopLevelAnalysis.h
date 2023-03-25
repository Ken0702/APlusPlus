//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2013 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlTopLevelAnalysis
#define ATLAS_AtlTopLevelAnalysis
#ifndef ROOT_TTask
#include <TTask.h>
#endif
#ifndef ROOT_TList
#include <TList.h>
#endif
#ifndef ATLAS_AtlSample
#include <AtlSample.h>
#endif
#ifndef ATLAS_AtlToolCut
#include <AtlToolCut.h>
#endif
#ifndef ATLAS_AtlCut
#include <AtlCut.h>
#endif
#ifndef ATLAS_AtlAppAnalysisTask
#include <AtlAppAnalysisTask.h>
#endif
#ifndef ATLAS_AtlHforSplittingTask
#include <AtlHforSplittingTask.h>
#endif
#ifndef ATLAS_AtlMergingTask
#include <AtlMergingTask.h>
#endif
#ifndef ATLAS_AtlSelector
#include <AtlSelector.h>
#endif
#ifndef ATLAS_AtlPlotterFolder
#include <AtlPlotterFolder.h>
#endif
#ifndef ATLAS_AtlHistFactoryMeasurement
#include <AtlHistFactoryMeasurement.h>
#endif

class TROOT;
class TBrowser;
class TString;
class TObjString;
class TObjArray;
class AtlAnalysisTool;
class TBits;

class AtlTopLevelAnalysis : public TTask {

  public:
    enum EInputFormat { kInvalidFormat = -1,
			kFilePerSyst,  // one tree/file
			kTreePerSyst   // one file with many trees
    };
    
    enum EBuildMode { kInvalidMode = -1,
		      kHforSplitting,
		      kAnalysis,
		      kMerging,
		      kPlotting,
		      kMemTkAnalysis,
		      kMemDiscAnalysis
    };

    static const Int_t fgNumSystematics = 170;
    static const char* fgSystematicNames[fgNumSystematics];
	
    enum ESystematic {
	kNOMINAL = 0,
	// default systematics
	kJEFF,
	kJER,
	kJES_DOWN, kJES_UP,
	kJVF_DOWN, kJVF_UP,
	kEER_DOWN, kEER_UP,
	kEES_DOWN, kEES_UP,
	kMUID_RES,
	kMUMS_RES,
	kMUSC_DOWN, kMUSC_UP,
	kRES_SOFT_DOWN, kRES_SOFT_UP,
	kSC_SOFT_DOWN,  kSC_SOFT_UP,
	// JES components
	kBJES_DOWN, kBJES_UP,
	kETAINTERCALIB_MODEL_DOWN, kETAINTERCALIB_MODEL_UP,
	kETAINTERCALIB_TOTALSTAT_DOWN, kETAINTERCALIB_TOTALSTAT_UP,
	kJESEFF_DET1_DOWN,   kJESEFF_DET1_UP,
	kJESEFF_DET2_DOWN,   kJESEFF_DET2_UP,
	kJESEFF_DET3_DOWN,   kJESEFF_DET3_UP,
	kJESEFF_MIX1_DOWN,   kJESEFF_MIX1_UP,
	kJESEFF_MIX2_DOWN,   kJESEFF_MIX2_UP,
	kJESEFF_MIX3_DOWN,   kJESEFF_MIX3_UP,
	kJESEFF_MIX4_DOWN,   kJESEFF_MIX4_UP,
	kJESEFF_MODEL1_DOWN, kJESEFF_MODEL1_UP,
	kJESEFF_MODEL2_DOWN, kJESEFF_MODEL2_UP,
	kJESEFF_MODEL3_DOWN, kJESEFF_MODEL3_UP,
	kJESEFF_MODEL4_DOWN, kJESEFF_MODEL4_UP,
	kJESEFF_STAT1_DOWN,  kJESEFF_STAT1_UP,
	kJESEFF_STAT2_DOWN,  kJESEFF_STAT2_UP,
	kJESEFF_STAT3_DOWN,  kJESEFF_STAT3_UP,
	kJESEFF_STAT4_DOWN,  kJESEFF_STAT4_UP,
	kJESPILEUP_OFFMU_DOWN,  kJESPILEUP_OFFMU_UP,
	kJESPILEUP_OFFNPV_DOWN, kJESPILEUP_OFFNPV_UP,
	kJESPILEUP_PT_DOWN,  kJESPILEUP_PT_UP,
	kJESPILEUP_RHO_DOWN, kJESPILEUP_RHO_UP,
	kSINGLEPART_DOWN,  kSINGLEPART_UP,
	kFLAVOR_COMP_DOWN, kFLAVOR_COMP_UP,
	kFLAVOR_RESP_DOWN, kFLAVOR_RESP_UP,
	kPUNCH_THROUGH_DOWN, kPUNCH_THROUGH_UP,
	// scale factor systematics
	kLEP_RECO_SF_DOWN, kLEP_RECO_SF_UP,
	kLEP_ID_SF_DOWN  , kLEP_ID_SF_UP,
	kLEP_TRIG_SF_DOWN, kLEP_TRIG_SF_UP,
	kBTAGSF_UP, // order is important for btagging systematics!
	kBTAGSF_UP_BREAK0,
	kBTAGSF_UP_BREAK1,
	kBTAGSF_UP_BREAK2,
	kBTAGSF_UP_BREAK3,
	kBTAGSF_UP_BREAK4,
	kBTAGSF_UP_BREAK5,
	kBTAGSF_UP_BREAK6,
	kBTAGSF_UP_BREAK7,
	kBTAGSF_UP_BREAK8,
	kBTAGSF_DOWN,
	kBTAGSF_DOWN_BREAK0,
	kBTAGSF_DOWN_BREAK1,
	kBTAGSF_DOWN_BREAK2,
	kBTAGSF_DOWN_BREAK3,
	kBTAGSF_DOWN_BREAK4,
	kBTAGSF_DOWN_BREAK5,
	kBTAGSF_DOWN_BREAK6,
	kBTAGSF_DOWN_BREAK7,
	kBTAGSF_DOWN_BREAK8,
	kCTAGSF_UP,
	kCTAGSF_UP_BREAK0,
	kCTAGSF_UP_BREAK1,
	kCTAGSF_UP_BREAK2,
	kCTAGSF_UP_BREAK3,
	kCTAGSF_UP_BREAK4,
	kCTAGSF_UP_BREAK5,
	kCTAGSF_UP_BREAK6,
	kCTAGSF_DOWN,
	kCTAGSF_DOWN_BREAK0,
	kCTAGSF_DOWN_BREAK1,
	kCTAGSF_DOWN_BREAK2,
	kCTAGSF_DOWN_BREAK3,
	kCTAGSF_DOWN_BREAK4,
	kCTAGSF_DOWN_BREAK5,
	kCTAGSF_DOWN_BREAK6,
	kMISTAGSF_UP,
	kMISTAGSF_UP_BREAK0,
	kMISTAGSF_UP_BREAK1,
	kMISTAGSF_UP_BREAK2,
	kMISTAGSF_UP_BREAK3,
	kMISTAGSF_UP_BREAK4,
	kMISTAGSF_UP_BREAK5,
	kMISTAGSF_UP_BREAK6,
	kMISTAGSF_UP_BREAK7,
	kMISTAGSF_UP_BREAK8,
	kMISTAGSF_UP_BREAK9,
	kMISTAGSF_UP_BREAK10,
	kMISTAGSF_UP_BREAK11,
	kMISTAGSF_DOWN,
	kMISTAGSF_DOWN_BREAK0,
	kMISTAGSF_DOWN_BREAK1,
	kMISTAGSF_DOWN_BREAK2,
	kMISTAGSF_DOWN_BREAK3,
	kMISTAGSF_DOWN_BREAK4,
	kMISTAGSF_DOWN_BREAK5,
	kMISTAGSF_DOWN_BREAK6,
	kMISTAGSF_DOWN_BREAK7,
	kMISTAGSF_DOWN_BREAK8,
	kMISTAGSF_DOWN_BREAK9,
	kMISTAGSF_DOWN_BREAK10,
	kMISTAGSF_DOWN_BREAK11,
	// Sample systematics
	kWtDS,
	kMCatNLO,
	kMCatNLO_SgTop,
	kMCatNLO_tchan,
	kMCatNLO_schan,
	kMCatNLO_wt,
	kMCatNLO_ttbar,
	kaMCatNLO_tchan,
	kaMCatNLO_schan,
	kISRFSR_ttbar_More,
	kISRFSR_ttbar_Less,
	kRadHi_ttbar,
	kRadLo_ttbar,
	kScaleVar1, // ToDo: Rename kScaleVar1_schan
	kScaleVar2, //
	kScaleVar3, //
	kScaleVar4, //
	kScaleVar5, //
	kScaleVar6, //
	kScaleVar_tchan1,
	kScaleVar_tchan2,
	kScaleVar_tchan3,
	kScaleVar_tchan4,
	kWjetsGen,
	kPowhegPythia_t,      // t-channel syst.
	kPowhegPythia2011C_t, // t-channel syst.
	kPowhegHerwig_t,      // t-channel syst.
	// Pdf Systematic
	kPdf_down,
	kPdf_up,
	// Fakes
	kLEP_FAKE_BDECAYGEN_SHAPE,
	kUndefined
    };
	
    enum ESystematicGroup {
	kSystGrpNominal          = BIT(0),
	kSystGrpJES              = BIT(1),
	kSystGrpDefaultSyst      = BIT(2),
	kSystGrpScaleFactors     = BIT(3),
	kSystGrpJESCompOnly      = BIT(4),
	kSystGrpBTagEV           = BIT(5),
	kSystGrpSampleSyst       = BIT(6),
	kSystGrpMultiLeptonFakes = BIT(7),
	kSystGrpAllDefault   = (kSystGrpNominal      |
				kSystGrpJES          |
				kSystGrpDefaultSyst  |
				kSystGrpScaleFactors ),
	kSystGrpAllJESComp   = (kSystGrpNominal      |
				kSystGrpDefaultSyst  |
				kSystGrpScaleFactors |
				kSystGrpJESCompOnly ),
	kSystGrpAll          = (kSystGrpNominal      |
				kSystGrpJES          |
				kSystGrpDefaultSyst  |
				kSystGrpScaleFactors |
				kSystGrpJESCompOnly  )
    };

  private:
    EBuildMode   fBuildMode;         // Buildmode for BuildUserTasks
    EInputFormat fInputFormat;       // Input format, 1 or many tree(s)/file 

    TList    *fListOfSamples;        // List of all samples
    TList    *fListOfCuts;           // List of cut objects
    TList    *fListOfSubselectionCuts; // List of AtlSubselectionCuts objects
    TList    *fListOfTools;          // List of tools used in the analysis
    TList    *fListOfToolCuts;       // List of cuts and params for analysis tools
    TList    *fListOfUserEnvs;       // List of user environment variables
    
    UInt_t    fLepChannels;          // Lepton channels
    UInt_t    fJetMults;             // Jet multiplicities
    TBits    *fSystematics;          // List of systematics
    Bool_t    fUseLepChannelSplit;   // Split lepton channels (like run-1)

    Float_t   fDataLuminosity;       // Data luminosity for plotter
    
    TString  *fHostname;             // Name of the cluster (as/ms-machines)
    TString  *fSgTopVersion;         // Version of SgTopD3PDs
    TString  *fDSIDList;             // List for matching DSID with local files
    TString   fGridSampleList;       // List of samples if not running locally
    TString   fMCWeightNamesFile;    // File with weight names/positions/totals

    TString  *fSelector;             // Name of Analysis Selector
    TString  *fInputDir;             // Path to input files (if input is not provided by a sample list)
    TString  *fPlotDir;              // Path to plotter input files (i.e. the output dir w/o 'xJets/lepton/systematic')
    TString  *fHistDir;              // Path to Histogram folder (analysis output)
    TString  *fJobHomeDir;           // Path to Task folder (job submitlog)
    TString  *fTempDir;              // Temporary path (batch output)
    TString   fOverrideInputDir;     // Directory to override fInputDir for rerunning cut analysis
    Bool_t    fOverrideBatchNodeAll; // In case we want to force usage of all nodes
    TString  *fTreeName;             // Name of the input tree (default: physics)
    Bool_t    fLooseTreeOnly;        // Set if only loose trees are available
    TString  *fOutputFilePrefix;     // Prefix of the output filename (default = "hist")
    TString  *fPlotterScheme;        // Option to set the scheme of the plotter
    TString  *fAtlasLabelStatus;     // Status label for plotter (default: "Work in Progress")
    TString  *fLuminosityLabel;      // Luminosity Label for Plotter
    TString  *fCenterOfMassEnergyLabel; // CenterOfMassEnergyLabel for plotter
    Bool_t    fDrawDataZeroEntryErrors; // Flag for plotter (default = false)
    Bool_t    fUseOverflowUnderflow;    // Flag for plotter (default = false)
    Bool_t    fDrawMCError;             // Flag for plotter (default = ktrue)
    Bool_t    fExportPDF;               // Flag for plotter (default = kfalse)
    Bool_t    fDrawSignalOverlay;       // Flag for drawing the signal MC (assumed to be the least entry in the MC histogram stack) as overlay instead as part of the stack (default = false)
    Float_t   fScaleOverlay;            // Scale factor of the overlay histogram (default = 1.)
    Bool_t    fDrawSignificance;        // Draw significance panel

    	
    TNamed   *fOutputTreeName;          // Name and title of the output tree (if any)
    
    Bool_t    fUseSubJobsAsInput;       // Analysis runs on subjob output of previous analysis step?
    Bool_t    fForceRetry;              // Flag for forcing retry (useful for testing, default: kFALSE)
    Bool_t    fBuildHforTree;           // Flag for creating HforTree (default: kFALSE)
    Bool_t    fBuildAnalysisTree;       // Flag for creating AnalysisTree (default: kTRUE)
    Bool_t    fBuildMergingTree;        // Flag for creating MergingTree (default: kTRUE)
    Bool_t    fBuildPlottingTree;       // Flag for creating PlottingTree (default: kTRUE)
    Bool_t    fBuildMemTkAnalysisTree;  // Flag for creating MemTk AnalysisTree (default: kFALSE)
    Bool_t    fBuildMemDiscAnalysisTree;// Flag for creating MemDisc AnalysisTree (default: kFALSE)
    Bool_t    fBuildHistFactoryTree;    // Flag for creating HistFactoryTree (default: kFALSE)
    Bool_t    fUseHforSamples;          // Flag for using Hfor Samples (default: kTRUE)
    Bool_t    fTtbarIsSplitted;         // Flag for using ttbar with splitted dataset (default: kTRUE)
    Bool_t    fUsePlotterSchemeDir;     // Flag for using the plotter scheme name as output directory name (default: kFALSE --> "plots_datamc")
    TList    *fNtuplePlotCmds;          // List of ntuple draw commands used by the plotter tasks
    
    TString *fMemTkRunScript;         // Run script to be used for MemTk analysis
    TString *fGridRootVersion;        // Root version for Grid submission
    TString *fGridCmtVersion;         // Cmt version for Grid submission
    TString *fGridUserName;           // Grid user name
    TString *fGridSuffix;             // Suffix for grid dataset names
    TString *fGridIdSuffix;           // Suffix for middle part of grid dataset names
    Bool_t   fGridJob;                // Switch for selecting grid jobs
    Bool_t   fGridAnalysisOnGrid;     // Switch to show that A++ analysis is also done on grid
    Bool_t   fGridAnalysisTemplateOnly;  // Switch to create one template instead of whole tree
    /* Bool_t   fSkipData;               // Skip Data if requested */
    Bool_t   fMEMTarBallOnly;         // Just create a tarball, don't submit to grid
    Bool_t   fMemTkTreatAllJetsAsBJets; // pass this variable to MEM

    Int_t fPriority; 			  //Priority of Job in condor

    Int_t fNSubJobsData;          // Number of subjobs for Data
    Int_t fNSubJobsQCD;           // Number of subjobs for QCD
    Int_t fNSubJobsTtbar;         // Number of subjobs for ttbar
    Int_t fNSubJobsSgTopSChannel; // Number of subjobs for sgtop s-channel
    Int_t fNSubJobsSgTopTChannel; // Number of subjobs for sgtop t-channel
    Int_t fNSubJobsSgTopWt;       // Number of subjobs for sgtop Wt
    Int_t fNSubJobsWjetsB;        // Number of subjobs for Wjets B
    Int_t fNSubJobsWjetsC;        // Number of subjobs for Wjets C
    Int_t fNSubJobsWjetsLight;    // Number of subjobs for Wjets Light
    Int_t fNSubJobsZjetsB;        // Number of subjobs for Zjets B
    Int_t fNProcessNthEventsOnly; // process only every Nth event (default=1 every event)
    Int_t fMaxEventsPerSubjob;    // Calculate NSubJobs automatically with max events per subjob
    TObjArray * fSampleSizes;     // Save number of events per sample

    AtlHistFactoryMeasurement *fMeasurement; // HistFactory Measurement infos
    
    TCollection * fSelectedVariations;
    Int_t fCampaign;              // 11 = 7TeV, 12 = 8TeV, 14 = DC14, 15 = 13TeV
    TString fCrossSectionFilePath;

    Int_t fDebug;                 // Debug flag to be passed to the analysis
    Int_t fDebugBuild;            // Debug flag used for task building
    Bool_t fBatchNodeAutomatic;	  // Automatic Batch node: see AtlTask

  public:
    AtlTopLevelAnalysis(const char* name, const char* title);
    virtual ~AtlTopLevelAnalysis();

    AtlSample * AddSample(const char* Name, const char* Title, const char* color,
		   AtlSample::EType Type, Float_t xsec, const char* PathName);
    void AddLeptonChannel(AtlSelector::ELepChannel Channel);
    void AddJetMultiplicity(AtlSelector::EJetMult JetMult);
    void AddSystematic(ESystematic Systematic);
    void AddSystematic2(char const * name);
    void AddSystematicGroup(ESystematicGroup SystematicGrp);
    void AddUserEnv(const char* env); // *MENU*
    void AddNtuplePlotCmd(const char* TreeName, const char* VarExp,
			  const char* Selection, const char* PlotName,
			  const char* PlotTitle, Int_t Nbins,
			  Double_t Xlow, Double_t Xup,
			  const char* XTitle, const char* YTitle);
    
    void BuildTree(Bool_t OpenBrowser = kTRUE);
    void BuildHforSplittingTree(TTask *ParentTask);
    void BuildAnalysisTree(TTask *ParentTask);
    void BuildMemTkAnalysisTree(TTask *ParentTask);
    void BuildMemDiscAnalysisTree(TTask *ParentTask);
    void BuildHistFactoryTree(TTask *ParentTask);
    void BuildMergingTree(TTask *ParentTask);
    void BuildPlottingTree(TTask *ParentTask);
    
    void BuildJetBinFolders(TTask *ParentTask);
    void BuildLeptonChannelFolders(TTask *ParentTask, Int_t Jetbin);
    void BuildSystematicsFolders(TTask *ParentTask, Int_t Jetbin, Int_t LepChannel);
    void BuildHforSplittingTasks(TTask *ParentTask, Int_t Lepton, Int_t Systematic);
    void BuildAnalysisTasks(TTask *ParentTask, Int_t Jetbin, Int_t LepChannel, Int_t Systematic);
    void BuildMergingTasks(TTask *ParentTask, Int_t Jetbin, Int_t LepChannel, Int_t Systematic);
    void BuildMemTkAnalysisTasks(TTask *ParentTask, Int_t Jetbin, Int_t LepChannel, Int_t Systematic);
    void BuildMemDiscAnalysisTasks(TTask *ParentTask, Int_t Jetbin, Int_t LepChannel, Int_t Systematic);
    void BuildPlottingTasks(TTask *ParentTask, Int_t Jetbin, Int_t LepChannel, Int_t Systematic);
    void BuildUserTasks(TTask *ParentTask, Int_t Jetbin, Int_t LepChannel, Int_t Systematic);

    TString* BuildOutputFileName(Int_t jetbin, Int_t lepchannel, 
				 Int_t systematic, const char* sample,
				 Bool_t UseSubJobsAsInput = kFALSE);
    TString* BuildOutputPath(TString* dir, const char* ext, 
			     Int_t jetbin, Int_t lepchannel, Int_t systematic,
			     const char* sample );

    AtlToolCut* FindToolCut(const char* tool, const char* var);
    AtlCut*     FindCut(const char* var);
    TList * GetListOfSubselectionCuts() { return fListOfSubselectionCuts; }

    const char* GetInputFileName(AtlSample *sample, Int_t lep, Int_t syst);
    const char* GetInputFileName(AtlSample *sample, Int_t lep, Int_t syst, Int_t pathIndex);
    const char* GetInputFileNameFromFile(AtlSample *sample, Int_t lep, Int_t syst);
    static const char* GetSystematicEnum(Int_t syst);
    static char const * GetSystematicName(Int_t syst);
    TList* GetPlottingFolders(Option_t *option, TList *samples);
    TList* GetPlottingFoldersSgTop_t(TList *samples);
    TList* GetPlottingFoldersSgTop_s(TList *samples);
    TList* GetPlottingFoldersSgTop_s_reduced(TList *samples);
    TList* GetPlottingFoldersSgTop_s_wjetsMerged(TList *samples);
    TList* GetPlottingFoldersSgTop_s_wjetsCB(TList *samples);
    TList* GetPlottingFoldersSgTop_s_QCDsplit(TList *samples);
    TList* GetPlottingFoldersSgTop_s_QCDsplit_wjetsMerged(TList *samples);
    TList* GetPlottingFoldersSgTop_s_splitall(TList *samples);
    TList* GetPlottingFoldersTtbar(TList *samples);
    TList* GetPlottingFolderstZ(TList *samples);
    TList* GetPlottingFoldersDetail(TList *samples);
    Bool_t GetTaskStatus(TString *jobHome, TString *outfileName,
                         TString *outfile, Bool_t require_success = kFALSE);
    
    static Int_t GetSystematicIdByName(char const * name);
    static Bool_t IsScaleFactorSystematic(Int_t Systematic);
    static Bool_t IsBTagEVScaleFactorSystematic(Int_t Systematic);
    static Bool_t IsJESComponentSystematic(Int_t Systematic);
    static Bool_t IsSampleSystematic(Int_t Systematic);
    
    virtual void Print(Option_t *option = "") const;
    void PrintLeptonChannelNames() const;
    void PrintJetMultiplicityNames() const;
    void PrintSystematicNames() const;

    void ReadSampleList(const char* basedir = "");
    void ReadSampleSizeList(const char* filename = "");
    void SetMCWeightNamesFile(const char* filename) {
        fMCWeightNamesFile = TString(filename);
    }
    Int_t GetMCWeightPosition(const char* sample_name, const char* weight_name);
    Double_t GetMCWeightTotalEvents(const char* sample_name, const char* weight_name);
    Double_t GetMCWeightTotalEvents(const char* sample_name, Int_t weight_num);

    void SetToolCut(const char* tool, const char* var, const char* val);
    void SetCut(const char* var, const char* val);
    void SetDSIDList(const char* list);
    inline void SetUseLepChannelSplit(Bool_t split) {
        fUseLepChannelSplit = split;
    }

    // I/O options
    void SetInputDir(const char* path );
    inline void SetOverrideInputDir(const char* dir) { fOverrideInputDir = TString(dir);}
    inline void SetOverrideBatchNodeAll(Bool_t use_all) {
        ///
        /// Per default, batch jobs can only be executed on the machine with the
        /// input files due to IO, and running on another cluster fails the
        /// task. Using this switch opens the complete local cluster for
        /// jobs. Take care to not use this switch when initially running over
        /// ntuples with a lot of events.
        ///
        fOverrideBatchNodeAll = use_all;
    }
    void SetPlotDir(const char* path );
    void SetHistDir(const char* path );
    void SetSelector(const char* sel );
    void SetSgTopVersion(const char* ver);
    void SetCampaign(Int_t campaign);
    void SetCrossSectionFilePath(const char* path);
    void SetJobHomeDir(const char* path );
    void SetTempDir(const char* path );
    void SetTreeName(const char* name );
    inline void SetLooseTreeOnly(Bool_t loose_only = kTRUE) { fLooseTreeOnly = loose_only;}
    void SetOutputFilePrefix(const char* prefix);
    void SetOutputTree(const char* name, const char* title);
    inline void SetGridSampleList(const char *list) {
        fGridSampleList = TString(list);
    }

    // Plotting options
    void SetPlotterScheme(Option_t *scheme);
    inline void SetDataLuminosity(Double_t luminosity) { fDataLuminosity = luminosity;}
    void SetAtlasLabelStatus(const char* label); 
    void SetCenterOfMassEnergyLabel(const char* label);
    void SetLuminosityLabel(const char* label);
    void SetDrawDataZeroEntryErrors(Bool_t status);
    void SetDrawMCError(Bool_t status);
    void SetExportPDF(Bool_t status);
    void SetUseOverflowUnderflow(Bool_t status);
    void SetDrawSignalOverlay(Bool_t status);
    void SetScaleOverlay(Float_t scale);
    inline void SetDrawSignificance(Bool_t b = kTRUE) { fDrawSignificance = b;}

    // Grid options
    void SetGridRootVersion(const char* RootVersion);
    void SetGridCmtVersion(const char* CmtVersion);
    void SetGridUserName(const char* UserName);
    void SetGridSuffix(const char* suffix);
    void SetGridIdSuffix(const char* suffix);

    // MemTk options
    void SetMemTkRunScript(const char* script);
    void SetMemTkTarBallOnly(Bool_t tarballonly = kTRUE) { fMEMTarBallOnly = tarballonly; }
    void SetMemTkTreatAllJetsAsBJets(Bool_t b = kTRUE) { fMemTkTreatAllJetsAsBJets = b; }
    
    inline Bool_t GetForceRetry() const { return fForceRetry; }
    inline TString* GetDSIDList() const { return fDSIDList; }
    inline TString* GetHistDir() const { return fHistDir; }
    inline TString* GetSelector() const { return fSelector; }
    inline TString* GetSgTopVersion() const { return fSgTopVersion; }
    inline TString* GetJobHomeDir() const { return fJobHomeDir; }
    inline TString* GetTempDir() const { return fTempDir; }
    inline TString* GetTreeName() const { return fTreeName; }    
    
    Int_t GetCampaign() const;

    inline void AddTool(AtlAnalysisTool *tool) {
	// Add analysis tool
	fListOfTools->Add((TObject*)tool);
    }
    inline void SetParameter(const char* var, const char* val) {
	//
	// Set parameter value for analysis selector.
	//
	// The variable name must be a valid data member of the underlying
	// analysis selector class (see AtlSelector). This function can
	// also be used in order to change the value for an already
	// existing parameter
	//
	SetCut(var, val);
    }
    inline void SetToolParameter(const char* tool, const char* var,
				 const char* val) {
	//
	// Set parameter value for an A++ selector tool.
	//
	// The variable name must be a valid data member of the given A++
	// analysis tool class (see AtlAnalysisTool). This function can
	// also be used in order to change the value for an already
	// existing variable
	SetToolCut(tool, var, val);
    }
    inline void SetForceRetry(Bool_t flag) { fForceRetry = flag; }
    inline void SetBuildHforTree(Bool_t flag) { fBuildHforTree = flag; }
    inline void SetBuildAnalysisTree(Bool_t flag) { fBuildAnalysisTree = flag; }
    inline void SetBuildMemTkAnalysisTree(Bool_t flag) { fBuildMemTkAnalysisTree = flag; }
    inline void SetBuildMemDiscAnalysisTree(Bool_t flag) { fBuildMemDiscAnalysisTree = flag; }
    inline void SetBuildBDTAnalysisTree(Bool_t flag) {
        //
        // S.K.: don't break old task files
        //
        Warning(__FUNCTION__, "This is deprecated; use MemDisc instead of BDT!");
        fBuildMemDiscAnalysisTree = flag;
    }
    inline void SetBuildMergingTree(Bool_t flag) { fBuildMergingTree = flag; }
    inline void SetBuildPlottingTree(Bool_t flag) { fBuildPlottingTree = flag; }
    void SetBuildHistFactoryTree(Bool_t flag);
    inline void SetUseHforSamples(Bool_t flag) { fUseHforSamples = flag; }	
    inline void SetGridJob(Bool_t gridjob) { fGridJob = gridjob; }
    inline void SetGridAnalysisOnGrid(Bool_t ongrid = kTRUE) {
      fGridAnalysisOnGrid = ongrid;
    }
    inline void SetGridAnalysisTemplateOnly(Bool_t g = kTRUE) {
        fGridAnalysisTemplateOnly = g;
    }
    inline void SetUseSubJobsAsInput(Bool_t flag) { fUseSubJobsAsInput = flag; }
    inline void SetUsePlotterSchemeDir(Bool_t flag) { fUsePlotterSchemeDir = flag; }
    inline void SetTtbarIsSplitted(Bool_t flag) { fTtbarIsSplitted = flag; } 
    /* inline void SetSkipData(Bool_t flag) { fSkipData = flag; } */

    inline void SetInputFormat(EInputFormat format) { fInputFormat = format; }
    inline void SetNSubJobsData(Int_t jobs) { fNSubJobsData = jobs; }
    inline void SetNSubJobsQCD(Int_t jobs) { fNSubJobsQCD = jobs; }
    inline void SetNSubJobsTtbar(Int_t jobs) { fNSubJobsTtbar = jobs; }
    inline void SetNSubJobsSgTopSChannel(Int_t jobs) { fNSubJobsSgTopSChannel = jobs; }
    inline void SetNSubJobsSgTopTChannel(Int_t jobs) { fNSubJobsSgTopTChannel = jobs; }
    inline void SetNSubJobsSgTopWt(Int_t jobs) { fNSubJobsSgTopWt = jobs; }
    inline void SetNSubJobsWjetsB(Int_t jobs) { fNSubJobsWjetsB = jobs; }
    inline void SetNSubJobsWjetsC(Int_t jobs) { fNSubJobsWjetsC = jobs; }
    inline void SetNSubJobsWjetsLight(Int_t jobs) { fNSubJobsWjetsLight = jobs; }
    inline void SetNSubJobsZjetsB(Int_t jobs) { fNSubJobsZjetsB = jobs; }
    inline void SetNProcessNthEventsOnly(Int_t n) { fNProcessNthEventsOnly = n; }
    inline void SetMaxEventsPerSubjob(Int_t n) { fMaxEventsPerSubjob = n; }

    inline void SetMeasurement(AtlHistFactoryMeasurement *meas) { fMeasurement = meas; }

    inline void SetDebug(Int_t level = 1) { fDebug = level; }
    inline void SetDebugBuild(Int_t level = 1) { fDebugBuild = level; }
    inline void SetTaskBatchNodeAutomatic(Bool_t BatchNode) { fBatchNodeAutomatic = BatchNode; }

    inline void SetPriority(Int_t Prio){
        	fPriority = Prio;
        }

private:
    Bool_t IsIgnored(AtlSample * sample);

    ClassDef(AtlTopLevelAnalysis,0) // Top-level A++ analysis task
};
#endif


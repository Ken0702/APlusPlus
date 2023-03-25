//____________________________________________________________________
//
// BEGIN_HTML
// <h2>Customised base class for A++ tasks</h2>
// <h3>Description:</h3>
// <p>
// Tasks provide a folder-like structure for running analysis jobs,
// drawing plots etc. For a general description see the parent class
// TTask. The AtlAPPAnalysiTask class is especially designed for
// running A++ analysis jobs made of analysis selectors inheriting from
// AtlSelector (like the AtlExampleAnalysis). The common case is that
// the same analysis is run several times. First over DATA and
// afterwards over various Monte Carlo samples (signal and
// background). Often the MC samples are grouped into different
// processes (ttbar production, W+jets etc.) With the help of the
// AtlAppAnalysisTask class this probem can be easily solved.
// </p>
// <p>
// Execution of analysis tasks is possible on a local machine
// (interactive running), on a batch system or on the GRID via the
// PANDA distributed analysis system. Details are described below.
// </p>
// <p>
// Two things are needed for running analysis tasks:
// <ol>
// <li>A task class inheriting from this class. Here, the
// implementation of the InitSelector() routine is mandatory. In this
// routine the analysis selector is created and configured,
// i.e. analysis cuts can be set (via the routine SetCut() and
// analysis tools (see class AtlAnalysisTool) can be added and
// configured as well (via SetToolCut()). The advantage of this method
// is that the cuts and configurations given here apply likewise to
// each of the analyses managed by this task. As an example, the
// analysis task for the single-top t-channel analysis is shown.<br>
// Atl_tChannelTask.C:
// <pre>
// //____________________________________________________________________
// //
// // Single-top t-channel analysis task
// // 
// //  
// // Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// // Update: $Id$
// // Copyright: 2009 (C) Oliver Maria Kind
// //
// #ifndef SINGLETOP_Atl_tChannelTask
// #define SINGLETOP_Atl_tChannelTask
// #ifndef ATLAS_AtlAppAnalysisTask
// #include <AtlAppAnalysisTask.h>
// #endif
// #ifndef ATLAS_AtlOverlapRemoval
// #include <AtlOverlapRemoval.h>
// #endif
// #ifndef ATLAS_AtlSgTop_tChannelFinder
// #include <AtlSgTop_tChannelFinder.h>
// #endif
// #include <Atl_tChannelAnalysis.h>
//
// class Atl_tChannelTask : public AtlAppAnalysisTask {
//    
// public:
//     Atl_tChannelTask(const char* name, const char* title);
//     virtual ~Atl_tChannelTask();
//     virtual void InitSelector();
//    
//     ClassDef(Atl_tChannelTask,0) // Single-top t-channel analysis task
// };
// #endif
//
// #ifndef __CINT__
// ClassImp(Atl_tChannelTask);
// #endif
//
// //____________________________________________________________________
//
// Atl_tChannelTask::Atl_tChannelTask(const char* name,
// 				   const char* title) :
//     AtlAppAnalysisTask(name, title) {
//     //
//     // Normal constructor
//     //
//     SetInputTreeName("t_app");
//     AddUserEnv("LIBSINGLETOP");
// }
//
// //____________________________________________________________________
//
// Atl_tChannelTask::~Atl_tChannelTask() {
//     //
//     // Default destructor
//     //
// }
//
// //____________________________________________________________________
//
// void Atl_tChannelTask::InitSelector() {
//     //
//     // Initialise the t-channel analysis selector
//     //
//
//     // Create single-top t-channel analysis selector
//     fSelector = new Atl_tChannelAnalysis(fOutputFileName->Data());
//
//     // ============================    
//     // Definition of selection cuts  
//     // ============================
//  
//     //
//     // Leptons
//     // -------
//
//     // Electrons
//     SetCut("fSignalElectronAuthor",        "AtlEMShower::kHighPtElectron");// | AtlEMShower::kSoftElectron");
//     SetCut("fSignalElectronIsEM",          "AtlEMShower::kElectronTight");
//     SetCut("fSignalElectron_Pt_min",       "10.");
//     SetCut("fSignalElectron_Pt_max",       "10.e10");
//     SetCut("fSignalElectron_Eta_min",      "-2.5");
//     SetCut("fSignalElectron_Eta_max",      "2.5");
//     SetCut("fSignalElectron_EtCone20_max", "6.");
//     ...
//
//     // Add overlap removal procedure
//     AtlOverlapRemoval *overlap = new AtlOverlapRemoval("overlap_removal",
//  						       "Overlap Removal Single-Top t-Channel");
//     SetToolCut("overlap_removal","fFindWorkingPoint", "kFALSE");
//     SetToolCut("overlap_removal","fElDeltaRMax",      "0.2");
//     ...
//     fSelector->AddTool(overlap);
//    
//     // Add single-top t-channel kinematic fitter
//     AtlSgTop_tChannelFinder *kinfit =
// 	new AtlSgTop_tChannelFinder("sgtop_finder_kinfitter",
// 	    "Single-Top t-Channel Reconstruction by KinFitter");
//     SetToolCut("sgtop_finder_kinfitter", "fMode", "AtlSgTop_tChannelFinder::kKinFit");
//     ...
//     fSelector->AddTool(kinfit);
// }
// </pre>
// </li>
// <li>
// A task script which defines the task folder structure. Note that
// such a script can contain different taks, e.g. analysis and
// plotting. For example the script corresponding to the single-top
// task above, given below.<br>
// task_tChannelAnalysis.C:
// <pre>
// // {
//     gROOT->LoadMacro("Atl_tChannelTask.C+");
//
//     // ===============================  
//     // Luminosities of the MC samples
//     // ===============================
//
//     //-----------
//     // single top
//     //-----------
//     Float_t Lumi_singletop_tchannel_enu = (19750/14.46);
//     Float_t Lumi_singletop_tchannel_munu = (19999/14.46);
//     Float_t Lumi_singletop_tchannel_taunu = (19500/14.46) ;
//     ...
//    
//     // Main task
//     TTask *tChannel = new TTask("tChannel", "Single-top t-Channel Analysis");
//
//     //////////////////////////////////////////////////////////////////////
//     //
//     // Single-top production
//     //
//     //////////////////////////////////////////////////////////////////////
//     TTask *sgtop = new TTask("single-top", "Single-top production");
//     tChannel->Add(sgtop);
//  
//     // =========
//     // t-channel
//     // =========
//     TTask *sgtop_tchannel = new TTask("t_channel", "t-channel");
//     sgtop->Add(sgtop_tchannel);
//
//     // e-nu
//     Atl_tChannelTask *sgtop_tchannel_enu
// 	= new Atl_tChannelTask("sgtop_tchannel_enu", "e-nu decay");
//     sgtop_tchannel_enu->AddInputFiles("/dats3/atlas/mandry/user10.RoccoMandrysch.mc09_7TeV.108340.st_tchan_enu_McAtNlo_Jimmy.merge.AOD.e508_s765_s767_r1250_r1260.22.05.2010.app.test_2/user10.RoccoMandrysch.mc09_7TeV.108340.st_tchan_enu_McAtNlo_Jimmy.merge.AOD.e508_s765_s767_r1250_r1260.22.05.2010.app.test_2*.root");
//     sgtop_tchannel_enu->SetOutputFile("$LIBSINGLETOP/hist/tChannelAnalysis/tchannel_enu.root");
//     sgtop_tchannel->Add(sgtop_tchannel_enu);
//  
//     // mu-nu
//     Atl_tChannelTask *sgtop_tchannel_munu
// 	= new Atl_tChannelTask("sgtop_tchannel_munu", "mu-nu decay");
//     sgtop_tchannel_munu->AddInputFiles("/dats3/atlas/mandry/user10.RoccoMandrysch.mc09_7TeV.108341.st_tchan_munu_McAtNlo_Jimmy.merge.AOD.e508_s765_s767_r1250_r1260.22.05.2010.app.test/user10.RoccoMandrysch.mc09_7TeV.108341.st_tchan_munu_McAtNlo_Jimmy.merge.AOD.e508_s765_s767_r1250_r1260.22.05.2010.app.test*.root");
//     sgtop_tchannel_munu->SetOutputFile("$LIBSINGLETOP/hist/tChannelAnalysis/tchannel_munu.root");
//     sgtop_tchannel->Add(sgtop_tchannel_munu);
//  
//     // tau-nu
//     Atl_tChannelTask *sgtop_tchannel_taunu
// 	= new Atl_tChannelTask("sgtop_tchannel_taunu", "tau-nu decay");
//     sgtop_tchannel_taunu->AddInputFiles("/dats3/atlas/mandry/app_20_02_2010/user09.RoccoMandrysch.ganga.mc08.108342.st_tchan_taunu_McAtNlo_Jimmy.merge.AOD.e405_s495_s520_r808_r838.app.test_20.02.2010.WUPPERTALPROD_SCRATCHDISK/user09.RoccoMandrysch.ganga.mc08.108342.st_tchan_taunu_McAtNlo_Jimmy.merge.AOD.e405_s495_s520_r808_r838.app.test_*.root");
//     sgtop_tchannel_taunu->AddInputFiles("/dats3/atlas/mandry/user10.RoccoMandrysch.mc09_7TeV.108342.st_tchan_taunu_McAtNlo_Jimmy.merge.AOD.e508_s765_s767_r1250_r1260.22.05.2010.app.test/user10.RoccoMandrysch.mc09_7TeV.108342.st_tchan_taunu_McAtNlo_Jimmy.merge.AOD.e508_s765_s767_r1250_r1260.22.05.2010.app.test*.root");
//     sgtop_tchannel_taunu->SetOutputFile("$LIBSINGLETOP/hist/tChannelAnalysis/tchannel_taunu.root");
//     sgtop_tchannel->Add(sgtop_tchannel_taunu);
//
//     // =========
//     // s-channel
//     // =========
//     TTask *sgtop_schannel = new TTask("s_channel", "s-channel");
//     sgtop->Add(sgtop_schannel);
//  
//     // e-nu
//     Atl_tChannelTask *sgtop_schannel_enu
// 	= new Atl_tChannelTask("sgtop_schannel_enu", "e-nu decay");
//     sgtop_schannel_enu->AddInputFiles("/dats3/atlas/mandry/user10.RoccoMandrysch.mc09_7TeV.108343.st_schan_enu_McAtNlo_Jimmy.merge.AOD.e534_s765_s767_r1250_r1260.22.05.2010.app.test/user10.RoccoMandrysch.mc09_7TeV.108343.st_schan_enu_McAtNlo_Jimmy.merge.AOD.e534_s765_s767_r1250_r1260.22.05.2010.app.test*.root");
//     sgtop_schannel_enu->SetOutputFile("$LIBSINGLETOP/hist/tChannelAnalysis/schannel_enu.root");
//     sgtop_schannel->Add(sgtop_schannel_enu);
//     ...  
//    
//     //////////////////////////////////////////////////////////////////////
//     //
//     // W + jets production
//     //
//     //////////////////////////////////////////////////////////////////////
//     TTask *WplusJets = new TTask("WplusJets", "W + Jets production");
//     tChannel->Add(WplusJets);
//  
//     // ============
//     // e-nu channel
//     // ============
//     TTask *WplusJets_enu = new TTask("WplusJets_enu", "W+Jets, enu channel");
//     WplusJets->Add(WplusJets_enu);
//  
//     // Np0
//     Atl_tChannelTask *WplusJets_enu_Np0
// 	= new Atl_tChannelTask("WplusJets_enu_Np0", "W+Jets, enu channel, Np0");
//     WplusJets_enu_Np0->AddInputFiles("/dats5/atlas/mandry/user10.RoccoMandrysch.mc09_7TeV.107680.AlpgenJimmyWenuNp0_pt20.merge.AOD.e511_s765_s767_r1250_r1260.22.05.2010.app.test_2/user10.RoccoMandrysch.mc09_7TeV.107680.AlpgenJimmyWenuNp0_pt20.merge.AOD.e511_s765_s767_r1250_r1260.22.05.2010.app.test_2*.root");   
//     WplusJets_enu_Np0->SetOutputFile("$LIBSINGLETOP/hist/tChannelAnalysis/Wjets_enu_np0.root");
//     WplusJets_enu->Add(WplusJets_enu_Np0);
//     ...
//
//     //////////////////////////////////////////////////////////////////////
//     //
//     // DATA
//     //
//     //////////////////////////////////////////////////////////////////////
//
//     // L1Calo Trigger Stream (new)
//     Atl_tChannelTask *data_l1calo = new Atl_tChannelTask("data-l1-calo", "Data from L1Calo trigger stream");
//     data_l1calo->AddInputFiles("/dats2/atlas/mandry/user10.RoccoMandrysch.data10_7TeV.00154813.physics_L1Calo.merge.AOD.f255_m471.22.05.2010.app.test/user10.RoccoMandrysch.data10_7TeV.00154813.physics_L1Calo.merge.AOD.f255_m471.22.05.2010.app.test*.root");
//     data_l1calo->SetOutputFile("$LIBSINGLETOP/hist/tChannelAnalysis/data_l1calo.root");
//     tChannel->Add(data_l1calo);
//     ...  
//
//
//     //////////////////////////////////////////////////////////////////////
//     //
//     // Draw all MC-DATA plots
//     //
//     //////////////////////////////////////////////////////////////////////
//     HepDataMCPlotter *pl_sgtop = new HepDataMCPlotter("DATA-MC plotter", "Create all Data-MC plots");
//     pl_sgtop->SetWorkingDir("$LIBSINGLETOP/hist/tChannelAnalysis/plots_datamc/");
//     pl_sgtop->SetLumiDATA(0.003260);
//
//     // Enable/disable grouping of histograms
//     Bool_t group_histograms = kTRUE;
//
//     // ==========
//     // Single-top
//     // ==========
//     //Set grouping of histograms
//     pl_sgtop->SetGroupHistograms(group_histograms);
//     pl_sgtop->AddMCFolder("sgtop", "sgtop incl", kRed);
//
//     pl_sgtop->AddMCSample("sgtop","$LIBSINGLETOP/hist/tChannelAnalysis/tchannel_enu.root",
//                     "MC single-top t-channel e#nu", Lumi_singletop_tchannel_enu, kRed);
//     pl_sgtop->AddMCSample("sgtop","$LIBSINGLETOP/hist/tChannelAnalysis/tchannel_munu.root",
//                     "MC single-top t-channel #mu#nu", Lumi_singletop_tchannel_munu, kRed-4);
//     pl_sgtop->AddMCSample("sgtop","$LIBSINGLETOP/hist/tChannelAnalysis/tchannel_taunu.root",
//                     "MC single-top t-channel #tau#nu", Lumi_singletop_tchannel_taunu, kRed-7);
//     ...
//     tChannel->Add(pl_sgtop);
//
//
//     // Open task folder in Root's TObject browser
//     gROOT->GetListOfTasks()->Add(tChannel);
//     gROOT->GetListOfBrowsables()->Add(tChannel);
//     new TBrowser;
// }
// </pre>
// </li>
// </ol>
//
//
// <h3Selection cuts:</h3>
// Selection cuts are defined by calling the member function SetCut().
//
// <h3>Job submission (at HU cluster):</h3>
// Use SetBatchNodeAll(), SetBatchNode() or SetBatchNodeAutomatic() to set
// the type of batch job submssion. For details see AtlTask.cxx.
//
// <h3>Project libraries:</h3>
// User-defined project libraries beside the standard libraries
// libHepEvent, libAtlasRun, ... must be made known to the task
// object. The environment variable containing the path to each such
// library has to be given by the AddUserEnv() function in the ctor
// of the derived class, eg <br>
// <pre>
//    Atl_tChannelTask::Atl_tChannelTask(const char* name,
//                                    const char* title) :
//        AtlAppAnalysisTask(name, title) {
//        //
//        // Normal constructor
//        //
//        SetInputTreeName("t_app");
//        AddUserEnv("LIBSINGLETOP");
//    }
// </pre>
//
// <h3>Entry lists:</h3>
// You can apply a TEntryList onto your chain, previously created by the
// AtlSelector. Your chain and the trees have to be the same. 
// Just use task->AddInputEntryLists("elist.root") and set 
// task->SetLoadEntryList(1) to get the list applied to your chain. Your 
// analysis will now run only on the events stored in this list.
// If the flag fUseAntiList is set, the analysis will run on events which 
// are not stored in the lists (events stored in the lists will be skipped).
//
// END_HTML
//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2009 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlAppAnalysisTask
#include <AtlAppAnalysisTask.h>
#endif
#include <TChain.h>
#include <TString.h>
#include <TSystem.h>
#include <AtlSelector.h>
#include <iostream>
#include <fstream>
#include <TClass.h>
#include <TDataMember.h>
#include <TROOT.h>
#include <AtlAnalysisTool.h>
#include "AtlSubselectionCuts.h"

using namespace std;

#ifndef __CINT__
ClassImp(AtlAppAnalysisTask);
#endif

//____________________________________________________________________

AtlAppAnalysisTask::AtlAppAnalysisTask(const char* name, const char* title) :
    AtlTask(name, title),
    fReaderClass(new TString()),
    fReaderArgs(new TString()) {
    //
    // Default constructor
    //
    SetBatchJob(kTRUE);
    fCtrlPlots        = kTRUE;
    fRootScript = 0;
    fNEvents = TChain::kBigNumber;
    fListOfCuts     = new TList;
    fListOfSubselectionCuts = new TList;
    fListOfToolCuts = new TList;
    fListOfUserEnvs = new TList;
    fListOfTools    = new TList;
    fWriteEntryList   = kFALSE;
    fLoadEntryList    = kFALSE;
    fUseAntiList      = kFALSE;
    fNProcessNthEventsOnly = 1;
    fPrintEvent       = kFALSE;
    fPrintObjectTable = kFALSE;
    fSelector      = 0;
    fInputTreeName = 0;
    fXsection      = 0;
    fGridSubmit    = kFALSE;
    fGridTemplateOnly = kFALSE;
    fGridUsePrecompiledApp = kTRUE;
    fUseAppInput   = kFALSE;
    fEvtReaderMCWeightPosition = -1;
    fEvtReaderMCWeightTotalEvents = -1.;
    fEvtReaderMCWeightPositionString = "";
    fEvtReaderMCWeightTotalEventsString = "";
    
    fPriority = 0;

    // Set environment variables for common libraries
    AddUserEnv("LIBHEPEVENT");
    AddUserEnv("LIBATLASRUN");
    AddUserEnv("LIBATLASEVENT");
    AddUserEnv("LIBATLASANALYSIS");
}

//____________________________________________________________________

AtlAppAnalysisTask::~AtlAppAnalysisTask() {
    //
    // Default destructor
    //
    if ( fRootScript != 0 ) delete fRootScript;
    delete fListOfSubselectionCuts;
    fListOfCuts->Delete();     delete fListOfCuts;
    fListOfToolCuts->Delete(); delete fListOfToolCuts;
    fListOfUserEnvs->Delete(); delete fListOfUserEnvs;
    fListOfTools->Delete();    delete fListOfTools;
    delete fReaderClass;
    delete fReaderArgs;
}

//____________________________________________________________________

void AtlAppAnalysisTask::SetInputTreeName(const char* TreeName) {
    //
    // Set input tree name
    //
    fInputTreeName = new TString(TreeName);  
}

//____________________________________________________________________

void AtlAppAnalysisTask::SetSelector(const char* Selector) {
    //
    // Set Analysis Selector name
    //
    fSelector = new TString(Selector);  
}

//____________________________________________________________________

void AtlAppAnalysisTask::ExecInteractiveJob(Option_t *option) {
    //
    // Interactive execution
    //
    // The interactive execution is done via an intermediate step of
    // an analysis run .C script in the same manner as for batch
    // jobs. Thus only a single way of job execution has to be
    // defined. In particular all selection cuts are set in the same
    // way.
    //
    TString opt = option;
    
    // Control plots option
    if ( fCtrlPlots == kTRUE ) opt.Append(",CTRL");
    
    // Start analysis process

    // Check input chain
    CheckInputChain();

    // Create analysis selector
    InitSelector();
    
    CreateRootScript(opt.Data());
    gROOT->Macro(Form("%s/analysis_run.C", fJobHome->Data()));
}    

//____________________________________________________________________

Bool_t AtlAppAnalysisTask::ExecBatchJob(Option_t *option) {
    //
    // Batch job submission
    //
    // Returns kFALSE in case of error.
    //
    TString opt = option;
    
    // Control plots option
    if ( fCtrlPlots == kTRUE ) opt.Append(",CTRL");

    // Check input chain
    CheckInputChain();
    
    // Create analysis selector
    InitSelector();

    // Create submit scripts
    CreateRootScript(opt.Data());
    CreateRunScript();

    // Submit batch job
    return SubmitBatchJob();
}

//____________________________________________________________________

void AtlAppAnalysisTask::ExecNAFBatchJob(Option_t *option) {
    //
    // Batch job submission
    //
    TString opt = option;

    // Control plots option
    if ( fCtrlPlots == kTRUE ) opt.Append(",CTRL");

    // Check input chain
    CheckInputChain();

    // Create analysis selector
    InitSelector();

    // Create submit scripts
    CreateNAFBatchRootScript(opt.Data());
    CreateNAFBatchRunScript();

    // Submit batch job
    SubmitNAFBatchJob();

}

//____________________________________________________________________

void AtlAppAnalysisTask::ExecGridJob(Option_t *option) {
    //
    // Grid job submission
    //
    TString opt = option;

    // Control plots option
    if ( fCtrlPlots == kTRUE ) opt.Append(",CTRL");

    // Create analysis selector
    InitSelector(); 

    // Create submit scripts
    CreateRootScript(opt.Data());
    CreateGridRunScript();

    // Submit grid job: not done here, but use external program to manage
    // submission scripts
    // Create only the templates without submission?
    if ( !fGridTemplateOnly ) {
        SubmitGridJob();
    }
}

//____________________________________________________________________

void AtlAppAnalysisTask::CreateRootScript(Option_t *option) {
    //
    // Create Root script for A++ analysis job execution
    //
    TString opt = option;

    //
    // Check first if all cut variables are valid data members of the
    // selector
    //

    // Class introspection possible for this selector ?
    if ( fSelector == 0 ) {
	Error("CreateRootScript",
	      "Selector unknown. Please use SetSelector(). Abort!");
 	gSystem->Abort(0);
    }
    
    TClass *sel_cl = (TClass*)gROOT->GetClass( fSelector->Data() );
    if ( sel_cl == 0 ) {
     	Error("CreateRootScript",
	      "Class description for class \"%s\" not found. Abort!",
	      fSelector->Data());
 	gSystem->Abort(0);
    }
    
    // Selector cut variables exist ?
    TIter next_cut(GetListOfCuts());
    AtlCut *cut = 0;
    TDataMember *cut_memb = 0;
    TClass *cut_memb_base = 0; // return value is of type TClass not TDatamember
    while ( (cut = (AtlCut*)next_cut()) ) {

	// Find data member
	cut_memb = FindDataMember(sel_cl, cut->GetVar());
	
	// Data member not found ?
	if ( cut_memb == 0 ) {
	    Error("CreateRootScript",
		  "Selector - Data member of name \"%s\" does not exist. Abort!",
		  cut->GetVar());
	    gSystem->Abort(0);
	}
    }
    
    // Analysis tools cut variables exist ?
    TIter next_toolcut(GetListOfToolCuts());
    AtlToolCut *toolcut = 0;
    cut_memb = 0;
    cut_memb_base = 0;
    
    TClass *tool_cl = 0;
    while ( (toolcut = (AtlToolCut*)next_toolcut()) ) {
      tool_cl = (TClass*)gROOT->GetClass(((AtlAnalysisTool*)fListOfTools->FindObject(toolcut->GetTool().Data()))->ClassName());
	
      if ( tool_cl == 0 ) {
	Error("CreateRootScript",
	      "Class description for class \"%s\" not found. Abort!",
	      toolcut->GetTool().Data());
	gSystem->Abort(0);
      }
      
      cut_memb = tool_cl->GetDataMember(toolcut->GetVariable().Data());
      cut_memb_base = tool_cl->GetBaseDataMember(toolcut->GetVariable().Data());
      if ( cut_memb == 0 && cut_memb_base == 0) {
	  Error("CreateRootScript",
		"Toolcut - Data member of name \"%s\" does not exist. Abort!",
		toolcut->GetVariable().Data());
	  gSystem->Abort(0);
      }
    }

    // =============
    // Create script
    // =============
    if ( fRootScript != 0 ) delete fRootScript;
    fRootScript = new TString(fJobHome->Data());
    if ( fGridTemplateOnly ) {
        fRootScript->Append("/analysis_run_template.C");
    } else {
        fRootScript->Append("/analysis_run.C");
    }
    fRootScript->ReplaceAll("//","/");
    
    ofstream out;
    out.open(fRootScript->Data());
    
    // Header
    out << "{" << endl
	<< "// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
	<< "// !!! This is an automatically generated file !!!" << endl
	<< "// !!! D O   N O T   E D I T                   !!!" << endl
	<< "// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
	<< "//" << endl;
    if ( fGridJob ) {
	out << "// Root script for A++ analysis grid job execution" << endl;
    } else {
	out << "// Root script for A++ analysis batch job execution" << endl;
    }
    out << "//" << endl;
    
    // Turn on debugging
    if ( fDebug > 0 ) {
	out << "gDebug = " << fDebug << ";" << endl;
    }
    
    // Init selector
    out << fSelector->Data() << " *sel = new ";

    if ( fGridJob ) {
        // just use a generic output name so that the DS does not get too long
        out << fSelector->Data() << "(\"" << "app_output.root" << "\");" << endl;
        // out << fSelector->Data() << "(\"" << fGridAnalysisName.Data() << "\");" << endl;
    } else if ( fTempOutputFileName != 0 && !fInteractiveJob ) {
        out << fSelector->Data() << "(\"" << fTempOutputFileName->Data() << "\");" << endl;
    } else {
        out << fSelector->Data() << "(\"" << fOutputFileName->Data() << "\");" << endl;
    }

    // Set reader
    if ( !fReaderClass->IsNull() ) {
        out << *fReaderClass << " * reader = new " << *fReaderClass << "(sel";
        if ( !fReaderArgs->IsNull() ) {
            out << ", " << *fReaderArgs;
        }
        out << ");\n";
        // Set different input mode for MemDisc jobs:
        //   take number of events from job_info histogram.
        // The same is also necessary to run over previous A++ output
        if ( (fReaderClass->EqualTo("AtlEvtReaderMemR2")) ||
             (fUseAppInput) ) {
            out << "sel->SetEvtReader(reader, kTRUE);\n";
        } else {
            out << "sel->SetEvtReader(reader);\n";
        }
    }

    // Reader option for mc weight systematics
    if ( fEvtReaderMCWeightPosition != -1 ) {
        // only works with AtlEvtReaderD3PDSgTopR2, but for now only warn
        // also works with AtlEvtReaderMemR2
        if ( !fReaderClass->EqualTo("AtlEvtReaderD3PDSgTopR2") &&
             !fReaderClass->EqualTo("AtlEvtReaderMemR2") ) {
            Warning(__FUNCTION__, "Setting mc weight position and total events is not implemented yet for evt reader %s", fReaderClass->Data());
        }
        out << Form("reader->SetMCWeightPosition(%d);\n",
                    fEvtReaderMCWeightPosition);
        out << Form("reader->SetMCWeightTotalEvents(%g);\n",
                    fEvtReaderMCWeightTotalEvents);
        if ( fEvtReaderMCWeightPosition < -1 ) {
            // multiplication of several weight systematics
            out << Form("reader->SetMCWeightPositionString(\"%s\");\n",
                        fEvtReaderMCWeightPositionString.Data());
            out << Form("reader->SetMCWeightTotalEventsString(\"%s\");\n",
                        fEvtReaderMCWeightTotalEventsString.Data());
        }
    }

    // Create input chain
    if ( fInputTreeName->IsNull() ) {
        out << "TChain *ch = new TChain(reader->GetTreeName());" << endl;
    }
    else {
        out << "TChain *ch = new TChain(\"" << fInputTreeName->Data() << "\");" << endl;
    }

    if ( fGridJob ) {
        // previously, we used (?)
        //     out << "ch->Add(\"app.root\");" << endl;
        // now take input list from a file named input.txt populated by %IN
        out << "ifstream input_files(\"input.txt\");" << endl
            << "TString line;" << endl
            << "while ( !input_files.eof() ) {" << endl
            << "    line.ReadLine(input_files);" << endl
            << "    if ( line.BeginsWith(\"#\") || line.Length() == 0 ) continue;" << endl
            << "    // split by ," << endl
            << "    TObjArray * tokens = line.Tokenize(\",\");" << endl
            << "    TIter next_input(tokens);" << endl
            << "    while ( TObjString * input = (TObjString*)next_input() ) {" << endl
            << "        ch->Add(input->GetString().Data());" << endl
            << "    }" << endl
            << "}" << endl;
    } else {
	// Add input files
	TIter next(fInputFiles);
	TObjString *item = 0;
	while ( (item = (TObjString*)next()) ) {
	    out << "ch->Add(\"" << item->GetString().Data() << "\");" << endl;
	}
    }
    
    // Create full entry list (containing all events) for creation of anti entry list (if wanted)
    if ( fLoadEntryList && fUseAntiList ) {
	out << "ch->Draw(\">>antilist\",\"\",\"app_entrylist\");" << endl;
	out << "TEntryList *antilist = (TEntryList*)gDirectory->Get(\"antilist\");" << endl;
    }
    // Apply entry list if wanted (and subtract from full list if anti list is wanted)
    if(fLoadEntryList)  {
      TIter next2(fInputEntryLists);
      TObjString *item2 = 0;
      out << "TEntryList *inputentrylist = new TEntryList();" << endl;
      while ( (item2 = (TObjString*)next2()) ) {
	  out << "TFile *entrylistfile = new TFile(\""
	      << item2->GetString().Data() << "\",\"read\");" << endl;
	  out << "inputentrylist->Add((TEntryList*)entrylistfile->Get(\"app_entrylist\"));" << endl;
      }
      // Subtract from full list for anti list and apply on chain
      if ( fUseAntiList )  {
	  out << "antilist->Subtract(inputentrylist);" << endl;
	  out << "ch->SetEntryList(antilist);" << endl;
      }
      else  {
	  out << "ch->SetEntryList(inputentrylist);" << endl;
      }
    }

    // only every nth event
    if ( fNProcessNthEventsOnly > 1 ) {
        out << "sel->SetNProcessNthEventsOnly("
            << fNProcessNthEventsOnly << ");" << endl;
    }

    // Set Xsec as additional info for schannel analysis (norm. weights for BDT)
    // This does not work for template grid submission
    if ( fSelector->EqualTo("AtlSgTop_sChannelAnalysis") && !fGridTemplateOnly ) {
        out << "sel->SetXsection("
            << fXsection << ");" << endl;
    }

    // Set output tree (if any)
    if ( fOutputTreeName != 0 ) {
	out << "sel->SetOutputTree(\""
	    << fOutputTreeName->GetName() << "\", \""
	    << fOutputTreeName->GetTitle() << "\");" << endl;
    }
    
    // Switch on/off entry list creation
    out << "sel->SetWriteEntryList("
	<< fWriteEntryList << ");" << endl;

    // Switch on/off printing every event
    out << "sel->SetPrintEvent("
	<< fPrintEvent << ");" << endl;

    // Switch on/off printing Root's object table
    out << "sel->SetPrintObjectTable("
	<< fPrintObjectTable << ");" << endl;
    
    // Set selection cuts
    next_cut.Reset();
    while ( (cut = (AtlCut*)next_cut()) ) {
      out << "sel->" << cut->GetVar() << " = "
	    << cut->GetVal() << ";" << endl;
    }

    // Subselection
    bool subsel_declared = false;
    for ( TIter next(GetListOfSubselectionCuts()); AtlSubselectionCuts * subsel_cuts = static_cast<AtlSubselectionCuts *>(next()); ) {
        if ( !subsel_declared ) {
            out << *fSelector << "::Subselection * ";
            subsel_declared = true;
        }
        out << "subsel = new " << *fSelector << "::Subselection(\"" << subsel_cuts->GetName() << "\");\n";
        for ( TIter next2(subsel_cuts->GetListOfCuts()); (cut = static_cast<AtlCut *>(next2())); ) {
            out << "subsel->" << cut->GetVar() << " = " << cut->GetVal() << ";\n";
        }
        out << "sel->fListOfSubselections->Add(subsel);\n";
    }

    // Init tools
    TIter next_tool(fListOfTools);
    AtlAnalysisTool *tool = 0;
    Int_t i = 0;
    while ( (tool = (AtlAnalysisTool*)next_tool()) ) {
	i++;
	out << tool->ClassName() << " *tool" << i << " = new "
	    << tool->ClassName() << "(\"" << tool->GetName()
	    << "\", \"" << tool->GetTitle() << "\");" << endl
	    << "sel->AddTool(tool" << i << ");" << endl;

	// Loop over all cuts
	// TIter next_cut(GetListOfToolCuts());
	next_toolcut.Reset();
	toolcut = 0;
	while ( (toolcut = (AtlToolCut*)next_toolcut()) ) {
	  if ( strcmp(tool->GetName(), toolcut->GetTool().Data()) != 0 ) continue;

	    // Get datamember associated with cut
	    TClass *cl = gROOT->GetClass(tool->ClassName());
	    TClass *mem_class = cl->GetBaseDataMember(toolcut->GetVariable().Data());
	    TDataMember *mem = mem_class->GetDataMember(toolcut->GetVariable().Data());

	    // debug
	    if ( fDebugBuild > 1 ) {
		Info(__FUNCTION__, "\n"
		     "Tool Cut info: \n"
		     "  tool name: %s\n"
		     "  toolcut var: %s\n"
		     "  toolcut val: %s\n"
		     "  var TrueTypeName: %s",
		     tool->GetName(), toolcut->GetVariable().Data(),
		     toolcut->GetVal(), mem->GetTrueTypeName());
		Info(__FUNCTION__, "Data member dump:\n");
		mem->Dump();
	    }

	    // Check if cut is a public datamember
	    if ( (mem->Property() & kIsPublic) == 0 ) {
		Error(__FUNCTION__,
		      "Given cut \"%s\" is not a public datamember in tool class \"%s\", which is necessary. Abort!",
		      toolcut->GetVariable().Data(), tool->ClassName());
		gSystem->Abort();
	    }

	    // Set cut value.
	    // In case of TStrings or TLists the values are appended
	    out << "tool" << i << "->" << toolcut->GetVariable().Data();
	    if ( strcmp(mem->GetTrueTypeName(), "TString*") == 0 ) {
		out << "->Append(\"" << toolcut->GetVal() << "\");" << endl;
	    } else if ( strcmp(mem->GetTrueTypeName(), "TString") == 0 ) {
		out << " = \"" << toolcut->GetVal() << "\";" << endl;
	    } else if ( strcmp(mem->GetTrueTypeName(), "TList*") == 0 ) {
		out << "->Add(" << toolcut->GetVal() << ");" << endl;
	    } else {
		out << " = " << toolcut->GetVal() << ";" << endl;
	    }
	}
    }
    
    // Process chain
    out << "ch->Process((TSelector*)sel, \"" << opt.Data() << "\", " << fNEvents 
	<< ", " << fFirstEntry << ");" << endl; 

//     if ( fGridJob ) {
// 	// sel->SetOutputTree("t_app","");    
// 	out << "sel->SetOutputTree(\"t_app\",\"\");" << endl;	
//     }
    
    out << "}" << endl;

    out.close();
}

//____________________________________________________________________

void AtlAppAnalysisTask::CreateNAFBatchRootScript(Option_t *option) {
    //
    // Create Root script for A++ analysis job execution
    //
    TString opt = option;

    //
    // Check first if all cut variables are valid data members of the
    // selector
    //
    
    // Class introspection possible ?
    TClass *cl = (TClass*)gROOT->GetListOfClasses()
	->FindObject(fSelector->Data());
    if ( cl == 0 ) {
	Error("SetCut", "Class description for class \"%s\" not found. Abort!",
	      fSelector->Data());
	gSystem->Abort(0);
    }
    
    // Cut variable exists ?
    TIter next_cut(GetListOfCuts());
    TNamed *cut = 0;
    TDataMember *cut_memb = 0;
    while ( (cut = (TNamed*)next_cut()) ) { 
	cut_memb = cl->GetDataMember(cut->GetName());
	if ( cut_memb == 0 ) {
	    Error("SetCut", "Data member of name \"%s\" does not exist. Abort!",
		  cut->GetName());
	    gSystem->Abort(0);
	}
    }

    //
    // Create script
    //
    if ( fRootScript != 0 ) delete fRootScript;
    fRootScript = new TString(fJobHome->Data());
    fRootScript->Append("/analysis_run.C");
    
    ofstream out;
    out.open(fRootScript->Data());
    
    // Header
    out << "{" << endl
	<< "// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
	<< "// !!! This is an automatically generated file !!!" << endl
	<< "// !!! D O   N O T   E D I T                   !!!" << endl
	<< "// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
	<< "//" << endl
	<< "// Root script for A++ analysis batch job execution" << endl
	<< "//" << endl;
    
    // Turn on debugging
    if ( fDebug > 0 ) {
	out << "gDebug = " << fDebug << ";" << endl;
    }

    // Init selector
    std::string output_file_name = fOutputFileName->Data();
    size_t found=output_file_name.find_last_of("/\\");
    out << cl->ClassName() << " *sel = new "
        << cl->ClassName() << "(\"" << output_file_name.substr(found+1) << "\");" << endl;

    // Set reader
    if ( !fReaderClass->IsNull() ) {
        out << *fReaderClass << " * reader = new " << *fReaderClass << "(sel";
        if ( !fReaderArgs->IsNull() ) {
            out << ", " << *fReaderArgs;
        }
        out << ");\n";
        out << "sel->SetEvtReader(reader);\n";
    }

    // Create input chain
    if ( fInputTreeName->IsNull() ) {
        out << "TChain *ch = new TChain(reader->GetTreeName());" << endl;
    }
    else {
        out << "TChain *ch = new TChain(\"" << fInputTreeName->Data() << "\");" << endl;
    }
    
    // Add input files
    TIter next(fInputFiles);
    TObjString *item = 0;
    while ( (item = (TObjString*)next()) ) {
      TString input_file_path_item = item->GetString();
      if ( input_file_path_item.Contains("pnfs") ) {
	
	TObjArray *v_input_file_path;
	v_input_file_path = input_file_path_item.Tokenize(";");
		
	for (Int_t i=0; i<v_input_file_path->GetEntries(); i++) {
	  
	  TObjString *in_path  = (TObjString*)v_input_file_path->At(i);
	  TString input_file_path  = in_path->GetString();
	  TString command = "rm log; dcls ";
	  command.Append(input_file_path);
	  command.Append(" >> log");
	  gSystem->Exec(command.Data());
	  
	  string line;
	  ifstream inputfile ("log");
	  if (inputfile.is_open())
	    {
	      while ( inputfile.good() ) {
		getline (inputfile,line);
		TString newline = line;
		if ( newline.EndsWith("app.root") ) {
		  TString input_file_list;
		  if ( input_file_path.Contains("ifh.de") != 0 )
		    input_file_list = "dcap://lcg-dc0.ifh.de:22125";
		  if ( input_file_path.Contains("desy.de") )
		    input_file_list = "dcap://dcache-atlas-dcap.desy.de:22125";
		  input_file_list.Append(input_file_path).Append("/").Append(newline);
		  out << "ch->Add(\"" << input_file_list.Data() << "\");" << endl;
		}
	      }
	      inputfile.close();
	    }
	  else cout << "Unable to open file eith list";
	}
      }
      
     else {
       
       // Add input files
       TIter next(fInputFiles);
       TObjString *item = 0;
       while ( (item = (TObjString*)next()) ) {
	 out << "ch->Add(\"" << item->GetString().Data() << "\");" << endl;
       }
     }
      
    }
    
    // Apply entry list if wanted
    if(fLoadEntryList)  {
      TIter next2(fInputEntryLists);
      TObjString *item2 = 0;
      out << "TEntryList *inputentrylist = new TEntryList();" << endl;
      while ( (item2 = (TObjString*)next2()) ) {
	out << "TFile *entrylistfile = new TFile(\"" << item2->GetString().Data() << "\",\"read\");" << endl;
	out << "inputentrylist->Add((TEntryList*)entrylistfile->Get(\"app_entrylist\"));" << endl;
      }
      out << "ch->SetEntryList(inputentrylist);" << endl;
    }
    
    // Set output tree (if any)
    if ( fOutputTreeName != 0 ) {
	out << "sel->SetOutputTree(\""
	    << fOutputTreeName->GetName() << "\", \""
	    << fOutputTreeName->GetTitle() << "\");" << endl;
    }
    
    // Switch on/off entry list creation
    out << "sel->SetWriteEntryList("
	<< fWriteEntryList << ");" << endl;

    // Switch on/off printing every event
    out << "sel->SetPrintEvent("
	<< fPrintEvent << ");" << endl;

    // Switch on/off printing Root's object table
    out << "sel->SetPrintObjectTable("
	<< fPrintObjectTable << ");" << endl;

    // Set selection cuts
    next_cut.Reset();
    while ( (cut = (TNamed*)next_cut()) ) {
	out << "sel->" << cut->GetName() << " = "
	    << cut->GetTitle() << ";" << endl;
    }

    // Init tools
    TIter next_tool(fListOfTools);
    AtlAnalysisTool *tool = 0;
    Int_t i = 0;
    while ( (tool = (AtlAnalysisTool*)next_tool()) ) {
	i++;
	out << tool->ClassName() << " *tool" << i << " = new "
	    << tool->ClassName() << "(\"" << tool->GetName()
	    << "\", \"" << tool->GetTitle() << "\");" << endl
	    << "sel->AddTool(tool" << i << ");" << endl;

	// Loop over all cuts
	TIter next_cut(GetListOfToolCuts());
	AtlToolCut* cut = 0;
	while ( (cut = (AtlToolCut*)next_cut()) ) {
	  if ( strcmp(tool->GetName(), cut->GetTool().Data()) != 0 ) continue;

	    // Check if cut is a public datamember
	    TClass *cl = gROOT->GetClass(tool->ClassName());
	    if ( cl->GetListOfAllPublicDataMembers()
		 ->FindObject(cut->GetVariable().Data()) == 0 ) {
		Error("CreateRootScript",
		      "Given cut \"%s\" is not a public datamember in tool class \"%s\", which is necessary. Abort!",
		      cut->GetVariable().Data(), tool->ClassName());
		gSystem->Abort();
	    }

	    // Set cut value (do this differently for strings)
	    out << "tool" << i << "->" << cut->GetVariable().Data();
	    TDataMember *mem = (TDataMember*)cl->GetListOfAllPublicDataMembers()
	      ->FindObject(cut->GetVariable().Data());
	    if ( strcmp(mem->GetTrueTypeName(), "TString*") == 0 ) {
		out << "->Append(\"" << cut->GetVal() << "\");" << endl;
	    } else {
		out << " = " << cut->GetVal() << ";" << endl;
	    }
	}
    }
        
    // Process chain
    out << "ch->Process((TSelector*)sel, \"" << opt.Data() << "\", " << fNEvents << ");" << endl; 

    out << "}" << endl;

    out.close();
}

//____________________________________________________________________

void AtlAppAnalysisTask::CreateRunScript(Option_t *option) {
    //
    // Create run script for job submission
    //
    TString Machine ="as2.b";
    if(fPriority > 40 ){
    	Machine = "as1.b";
    }
    //if(fJobHome->Contains("MC16a")) Machine ="as2.b";
    ofstream out2;
    //std::cout<<*fJobHome+"submit"<<std::endl;
    out2.open(*fJobHome+"submit");
    out2 <<"Universe   = vanilla"<<endl
    <<"getenv = True"<<endl
    <<"Log = simple.log"<<endl
<<"Executable = "+*fRunScript<<endl
<<"requirements = regexp( \".*"<<Machine<<".*\", TARGET.Name )"<<endl
<<"Queue"<<endl;
    
    ofstream out;
    out.open(fRunScript->Data());
    out << "#!/bin/sh" << endl
	<< "# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
	<< "# !!! This is an automatically generated file !!!" << endl
	<< "# !!! D O   N O T   E D I T                   !!!" << endl
	<< "# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
	<< "#" << endl
	<< "# Run script for A++ analysis job submission" << endl
	<< "#" << endl << endl
	<< "# Set environment" << endl;

    // Platform
    if ( gSystem->Getenv("ARCH_TYPE") != 0 ) {
	out << "export ARCH_TYPE=" << gSystem->Getenv("ARCH_TYPE") << endl;
    } else {
	Error("CreateRunScript", "Environment variable ARCH_TYPE not set. Abort!");
	gSystem->Abort(0);
    }

    if (!WriteEnvSetup(out)) {

    // ROOT
    out << "export ROOTSYS=" << gSystem->Getenv("ROOTSYS") << endl
	<< "export PATH=${ROOTSYS}/bin:${PATH}" << endl
	<< "export LD_LIBRARY_PATH=${ROOTSYS}/lib:${LD_LIBRARY_PATH}" << endl;
    out << "hostname" << endl;
    // A++ and user-defined libraries
    TIter next_lib(fListOfUserEnvs);
    TNamed *env = 0;
    while ( (env = (TNamed*)next_lib()) ) {
	if ( gSystem->Getenv(env->GetName()) != 0 ) {
	    TString host2(gSystem->HostName());
	    if ( host2.Contains("lx") ) {	    
		out << "source /home/grid/lcg/sw/root_setup_sles11.sh-v5.34" << endl;
	    }
	    out << "export " << env->GetName() << "="
		<< gSystem->Getenv(env->GetName()) << endl
		<< "export LD_LIBRARY_PATH=${"
		<< env->GetName() << "}/lib/${ARCH_TYPE}:${LD_LIBRARY_PATH}"
		<< endl;
	} else {
	    Error("CreateRunScript", "Environment variable %s not set. Abort!",
		  env->GetName());
	    gSystem->Abort(0);
	}
    }

    }
    out	<< endl << endl
	<< "# Job execution" << endl;
//	<< "export PBS_O_HOME=" << fTempOutputPath->Data() << endl
//	<< "export PBS_O_WORKDIR=" << fTempOutputPath->Data() << endl;
//	<< "cd $PBS_O_WORKDIR" << endl;

    if (  fTempOutputFileName != 0 ) {
	out << "if [ ! -d " << gSystem->DirName( fTempOutputFileName->Data() ) << " ]; then " << endl
	    << "   mkdir -p " << gSystem->DirName( fTempOutputFileName->Data() ) << endl
	    << "fi" << endl;
    }
    
    if ( fTempOutputPath != 0 && fTempLogFilePath != 0 ) {
	out << "mkdir -p " << fTempOutputPath->Data() << endl
	    << "cp " << fJobHome->Data() << "/analysis_run.C " << fTempOutputPath->Data() << endl
	    << "cd " << fTempOutputPath->Data() << endl
	    << endl
	    << "# Remove old Logfiles " << endl
	    << "rm " << fTempLogFilePath->Data() << endl
	    << "rm " << fLogFilePath->Data() << endl
	    << "root -q -l -b analysis_run.C > " << fTempLogFilePath->Data()
	    << " 2>&1" << endl
	    << "mv " << fTempLogFilePath->Data() << " " << fLogFilePath->Data() << endl
	    << "chmod g+w -R " << gSystem->DirName(fLogFilePath->Data()) << endl;
    } else {
	out << "JOBHOME=" << fJobHome->Data() << endl
	    << "cd $JOBHOME" << endl	
	    << "root -q -l -b analysis_run.C > " << fLogFilePath->Data()
	    << " 2>&1" << endl
	    << "chmod g+w -R " << fLogFilePath->Data() << endl;
    }
    
    if (  fTempOutputFileName != 0 )
	out << "mv " << fTempOutputFileName->Data() << " " << fOutputFileName->Data() << endl
	    << "chmod g+w " << fOutputFileName->Data() << endl;
    
    out.close();
}

//____________________________________________________________________

void AtlAppAnalysisTask::CreateNAFBatchRunScript() {
    //
    // Create run script for job submission
    //
    ofstream out;
    out.open(fRunScript->Data());
    out << "# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
	<< "# !!! This is an automatically generated file !!!" << endl
	<< "# !!! D O   N O T   E D I T                   !!!" << endl
	<< "# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
	<< "#" << endl
	<< "# Run script for A++ analysis job submission" << endl
	<< "#" << endl
	<< "# Set environment" << endl
	<< "#" << endl
	<< "#" << endl
	<< "# (only accept jobs with correct resources)" << endl
	<< "#$ -w e" << endl
	<< "#" << endl
	<< "# (stderr and stdout are merged together to stdout)" << endl
	<< "#$ -j y" << endl
	<< "#" << endl
	<< "#$ -o "<< fJobHome->Data() << endl //"##$ -o /dev/null" << endl
	<< "#$ -e " << fJobHome->Data() << endl //"##$ -e /dev/null" << endl
	<< "#" << endl
	<< "# (send mail on job's end and abort)" << endl
	<< "##$ -m ae -M rocco.mandrysch@desy.de" << endl
	<< "#" << endl
	<< "# (put log files into current working directory)" << endl
	<< "#$ -cwd" << endl
	<< "#" << endl
	<< "# (use ATLAS project)" << endl
	<< "#$ -P atlas" << endl
	<< "#" << endl
	<< "# (choose memory and disc usage)" << endl
	<< "#$ -l h_vmem=1G" << endl
	<< "#$ -l h_stack=10M" << endl
	<< "#$ -l h_fsize=10G" << endl
	<< "#" << endl
	<< "# (choose time)" << endl
	<< "#$ -l h_cpu=12:00:00" << endl
	<< "" << endl
	<< "# change to scratch directory" << endl
	<< "cd $TMPDIR" << endl
	<< "#" << endl
	<< "#" << endl;
    if ( gSystem->Getenv("ARCH_TYPE") != 0 ) {
	out << "export ARCH_TYPE=" << gSystem->Getenv("ARCH_TYPE") << endl;
    } else {
	Error("CreateRunScript", "Environment variable ARCH_TYPE not set. Abort!");
	gSystem->Abort(0);
    }
    out << "export ROOTSYS=" << gSystem->Getenv("ROOTSYS") << endl
	<< "export PATH=${ROOTSYS}/bin:${PATH}" << endl
	<< "export LD_LIBRARY_PATH=${ROOTSYS}/lib:${LD_LIBRARY_PATH}" << endl;

    // A++ and user-defined libraries
    TIter next_lib(fListOfUserEnvs);
    TNamed *env = 0;
    while ( (env = (TNamed*)next_lib()) ) {
	if ( gSystem->Getenv(env->GetName()) != 0 ) {
	    out << "export " << env->GetName() << "="
		<< gSystem->Getenv(env->GetName()) << endl
		<< "export LD_LIBRARY_PATH=${"
		<< env->GetName() << "}/lib/${ARCH_TYPE}:${LD_LIBRARY_PATH}"
		<< endl;
	} else {
	    Error("CreateRunScript", "Environment variable %s not set. Abort!",
		  env->GetName());
	    gSystem->Abort(0);
	}
    }
    out	<< endl << endl
	<< "# Job execution" << endl
    	<< "cp " << fJobHome->Data() << "/analysis_run.C ." << endl
	<< "" << endl
	<< "root -q -l -b analysis_run.C" << endl
	<< "" << endl;
    
    std::string output_file_name = fOutputFileName->Data();
    size_t found=output_file_name.find_last_of("/\\");
    out	<< "cp " << output_file_name.substr(found+1) << " " << fOutputFileName->Data() << endl
	<< "" << endl;
    out << "exit 0" << endl;
    
    out.close();
}

//____________________________________________________________________

void AtlAppAnalysisTask::CreateGridRunScript() {
    //
    // Create run script for grid job submission.
    // This file is executed on the grid.
    //
    ofstream out;
    if ( fGridTemplateOnly ) {
        TString run_script = gSystem->ConcatFileName(gSystem->DirName(fRunScript->Data()), "app_template.run");
        out.open(run_script.Data());
    } else {
        out.open(fRunScript->Data());
    }
    out << "#!/bin/sh" << endl
        << "# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
        << "# !!! This is an automatically generated file !!!" << endl
        << "# !!! D O   N O T   E D I T                   !!!" << endl
        << "# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
        << "#" << endl
        << "# Run script for A++ analysis grid job submission" << endl
        << "#" << endl << endl
        << "# Set environment" << endl;

    // Platform:
    if ( gSystem->Getenv("ARCH_TYPE") != 0 ) {
        out << "export ARCH_TYPE=" << gSystem->Getenv("ARCH_TYPE") << endl;
    } else {
        Fatal(__FUNCTION__, "Environment variable ARCH_TYPE not set.");
    }
    out << "source APlusPlus/setup.sh" << endl;

    // environment setup: use bexec

    // change systematic for template
    TString root_script = gSystem->BaseName(fRootScript->Data());
    if ( fGridTemplateOnly ) {
        out << endl
            << "# Change systematic" << endl
            << "sed -i \"s|TEMPLATE|$1|g\" " << root_script.Data() << endl;
    }

    // Job execution
    out << endl
        << "# Job execution" << endl
        << "root -q -l -b $APP_HOME/rootlogon_example.C "
        << root_script.Data() << endl;

    // there is no need for a log file on grid

    out.close();
}

//____________________________________________________________________

void AtlAppAnalysisTask::CreateGridRunScript_old() {
    //
    // Create run script for grid job submission
    //
    ofstream out;
    out.open(fRunScript->Data());
    out << "# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
	<< "# !!! This is an automatically generated file !!!" << endl
	<< "# !!! D O   N O T   E D I T                   !!!" << endl
	<< "# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
	<< "#" << endl
	<< "# Run script for A++ analysis grid job submission" << endl
	<< "#" << endl
	<< endl << endl
	
	<< "# Job execution" << endl;

    //
    // Get input folder name
    //
    TString inputfolder = ((TObjString*)fInputFiles->At(0))->GetString();

    //
    // Build output folder name
    //
    TObjArray* array = inputfolder.Tokenize(".");
    TString outputfolder =
	"user."
	+(*fGridUser)
	+"."
	+((TObjString*)array->At(2))->String() // project tag
	+"."
	+((TObjString*)array->At(3))->String() // MC sample number / DATA period or run number
	+"."
	+((TObjString*)array->At(4))->String();// MC sample name / DATA stream
    
    // DATA?
    if ( ((TObjString*)array->At(2))->String().Contains("data") )
	outputfolder += ((TObjString*)array->At(5))->String();
    
    outputfolder +=
	+"."
	+(fGridAnalysisName)
	+"_"
	+GetName()
	+"_"
	+(*fGridSuffix)
	+"/";

    //
    // Build general prun command
    //
    TString generalcmd =
	"prun --inDS "
	+inputfolder
	+" --outDS "
	+outputfolder
	+" --athenaTag="
	+(*fAthenaTag)
	+" --extFile=*.root --noBuild --nGBPerJob=MAX --crossSite=50" //--mergeOutput
	+" --excludedSite="
	+(*fExcludedSites);

    TString generalcmd2 =
	+" --site="
	+(*fSingleSite);
    
    //
    // Build analysis command
    //
    TString analysiscmd=
	" --exec \'[ $(ls|grep APP) ]&&cd APPonGrid; source aplusplus_setup.sh; ./setAnalysisInput.sh \"%IN\"; root -b -l -q analysis_run.C | tee analysis.log\' --outputs analysis.log,"
	+(fGridAnalysisName)
	+".root";

    //
    // Write out complete prun command
    //
    if ( *fSingleSite != "" ) {
	out << generalcmd+generalcmd2+analysiscmd << endl
	    << "" << endl
	    << "exit 0" << endl;
    } else {
	out << generalcmd+analysiscmd << endl
	    << "" << endl
	    << "exit 0" << endl;
    }

    
    out.close();

    //
    // Copy runscript and analysis root script to grid working directory
    //
    TString cp_cmd("cp ");
    cp_cmd.Append(fRunScript->Data());
    cp_cmd.Append(" ");
    cp_cmd.Append(gSystem->ExpandPathName("$GRID_HOME"));
    gSystem->Exec(cp_cmd.Data());

    cp_cmd.Clear();
    cp_cmd.Append("cp ");
    cp_cmd.Append(fRootScript->Data());
    cp_cmd.Append(" ");
    cp_cmd.Append(gSystem->ExpandPathName("$GRID_HOME"));
    gSystem->Exec(cp_cmd.Data());

}

//____________________________________________________________________

void AtlAppAnalysisTask::CheckInputChain() {
    //
    // Check the input chain
    //
    Info("CheckInputChain", "Check input chain \"%s\" ...",
	 fInputTreeName->Data());
    TChain *InputChain = new TChain(fInputTreeName->Data());

    // Concatenate input files
    TIter next(fInputFiles);
    TObjString *item = 0;
    Int_t nfiles = 0;
    while ( (item = (TObjString*)next()) ) {
	Int_t nfiles_item = InputChain->Add(item->GetString().Data());
	if ( nfiles_item == 0 )
	    Warning("CheckInputChain", "No files found for search string %s",
		    item->GetString().Data());
	nfiles += nfiles_item;
    }
    if ( nfiles == 0 ) {
	Error("CheckInputChain", "No input files given. Abort!");
	gSystem->Abort(0);
    } else {
	Info("CheckInputChain", "Added %-d input files to chain \"%s\".",
	     nfiles, fInputTreeName->Data());
    }
    delete InputChain;
}

//____________________________________________________________________

void AtlAppAnalysisTask::SetCut(const char* var, const char* val) {
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

void AtlAppAnalysisTask::SetToolCut(const char* tool, const char* var,
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

AtlToolCut* AtlAppAnalysisTask::FindToolCut(const char* tool,
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

AtlCut* AtlAppAnalysisTask::FindCut(const char* var) {
    //
    // Find A++ analysis selector cut
    //
    return (AtlCut*)fListOfCuts->FindObject(var);
}

//____________________________________________________________________

void AtlAppAnalysisTask::AddUserEnv(const char* env) {
    //
    // Add user-defined environment variable for shared library search
    // paths
    //
    fListOfUserEnvs->Add(new TNamed(env, ""));
}

//____________________________________________________________________

void AtlAppAnalysisTask::Print( Option_t *option ) const {
    //
    // Print config
    //
    cout << "=============================" << endl;
    cout << " Task: " << fName << endl;
    cout << " ---------------------------" << endl;
    cout << " TreeName: " << fInputTreeName->Data() << endl;
    cout << " Selector: " << fSelector->Data() << endl;
    cout << " ---------------------------" << endl;
    cout << " JobHome : " << fJobHome->Data() << endl;
    cout << " OutputFileName: " << fOutputFileName->Data() << endl;
    cout << " " << endl;
    cout << " " << endl;
    cout << " " << endl;
    cout << "=============================" << endl;
}

//____________________________________________________________________

void AtlAppAnalysisTask::SubmitGridJob() {
    //
    // Submit A++ grid job
    //

    // Int_t success_code = -999;
    Bool_t success = kFALSE;
    TString cmd = "";
    Bool_t extra_dry = kTRUE; // don't use prun to create tarball

    // go to run directory
    TString run_dir = gSystem->ConcatFileName(gSystem->DirName(fRunScript->Data()), "run");
    gSystem->mkdir(run_dir.Data(), kTRUE);
    success = gSystem->ChangeDirectory(run_dir.Data());
    if ( !success )
        Fatal(__FUNCTION__, "Could not change to run directory %s", run_dir.Data());

    // copy files to run directory
    // 1. bexec script (setup on grid) OR precompiled A++
    TString grid_setup_script = gSystem->ConcatFileName(
        gSystem->Getenv("APP_HOME"), "project_setup.sh");
    TString app_tarfile = gSystem->ConcatFileName(
        gSystem->Getenv("APP_HOME"), "APlusPlus.tgz");
    if ( fGridUsePrecompiledApp ) {
        if ( fGridSubmit ) {
            // if we don't submit now, we can add A++ later
            cmd = Form("cp -t %s %s", run_dir.Data(), app_tarfile.Data());
            if ( gDebug )
                Info(__FUNCTION__, "Copy A++ to run directory:\n\"%s\"", cmd.Data());
            gSystem->Exec(cmd.Data());
        }
    } else {
        cmd = Form("cp -t %s %s", run_dir.Data(), grid_setup_script.Data());
        if ( gDebug )
            Info(__FUNCTION__, "Copy bexec script to run directory:\n\"%s\"", cmd.Data());
        gSystem->Exec(cmd.Data());
        cmd = Form("chmod +x %s", gSystem->ConcatFileName(
                       run_dir.Data(), "project_setup.sh"));
        if ( gDebug )
            Info(__FUNCTION__, "Make executable:\n\"%s\"", cmd.Data());
        gSystem->Exec(cmd.Data());
    }
    // 2. exec script (running on grid): fRunScript
    // 3. A++ root script: fRootScript
    cmd = Form("cp -t %s %s %s", run_dir.Data(),
               fRunScript->Data(), fRootScript->Data());
    if ( gDebug ) Info(__FUNCTION__, "Copy files to run directory:\n\"%s\"", cmd.Data());
    gSystem->Exec(cmd.Data());
    cmd = Form("chmod +x %s", gSystem->ConcatFileName(
                   run_dir.Data(), gSystem->BaseName(fRunScript->Data())));
    if ( gDebug ) Info(__FUNCTION__, "Make executable:\n\"%s\"", cmd.Data());
    gSystem->Exec(cmd.Data());

    // file to save run commands in
    TString command_file = gSystem->ConcatFileName(run_dir,"app_command.txt");

    // additional variable names
    TString inDS = gSystem->BaseName(((TObjString*) fInputFiles->At(0))->GetString().Data());
    // TDatime time;
    // TString gridid = TString(Form("%010d_%s", time.Get(), GetName()));
    TString gridid = Form("%s_%s", GetName(), GetTitle()); // don't use time
    gridid.Append(fGridIdSuffix.Data());

    // create prun submission command
    // other options to consider?
    // - log file in output?
    // - mergeOutput option?
    // - memory=2000 ?
    TString jobsub_cmd("prun ");
    // environment options
    jobsub_cmd.Append(Form("--rootVer=%s ", fGridRootVersion->Data()));
    jobsub_cmd.Append(Form("--cmtConfig=%s ", fGridCmtVersion->Data()));
    // run options
    if ( fGridUsePrecompiledApp ) {
        jobsub_cmd.Append("--bexec='tar -xzf APlusPlus.tgz' ");
    } else {
        jobsub_cmd.Append(Form("--bexec='./%s' ", gSystem->BaseName(grid_setup_script.Data())));
    }
    jobsub_cmd.Append(Form("--exec='./%s' ", gSystem->BaseName(fRunScript->Data())));
    jobsub_cmd.Append("--long ");
    // jobsub_cmd.Append("--maxCpuCount=172800 ");
    jobsub_cmd.Append("--excludedSite=ANALY_GOEGRID ");
    // jobsub_cmd.Append("--nGBPerJob=5 ");
    // I/O options
    jobsub_cmd.Append(Form("--inDS=%s ", inDS.Data()));
    jobsub_cmd.Append("--writeInputToTxt=IN:input.txt ");
    jobsub_cmd.Append("--nFilesPerJob=1 ");
    // jobsub_cmd.Append("--outputs=app_output.root,analysis.log ");
    jobsub_cmd.Append("--outputs=app_output.root ");
    // jobsub_cmd.Append(Form("--outputs=%s,analysis.log ",
    //                        gSystem->BaseName(fOutputFileName->Data())));
    jobsub_cmd.Append(Form("--outDS='user.%s.schannelAPP.%s.%s' ",
                           fGridUserName->Data(),
                           gridid.Data(),
                           fGridSuffix->Data()));
    jobsub_cmd.Append("--destSE=DESY-ZN_LOCALGROUPDISK ");
    // submission options
    if ( fDebugBuild ) {
        jobsub_cmd.Append("-v ");
    }

    // check if submit or just tarball
    if ( fGridSubmit ) {
        // Submit prun cmd and check success
        Info("SubmitGridJob", "Execute %s", jobsub_cmd.Data());
        Int_t err_code = gSystem->Exec(jobsub_cmd.Data());
        if ( err_code != 0 ) {
            Fatal(__FUNCTION__, "Submission command was not successful. Abort!");
        }
    } else {
        // create tarball in dry run
        TString tarball_name = TString(gSystem->BaseName(fOutputFileName->Data()));
        tarball_name.ReplaceAll(".root", ".tgz");
        if ( extra_dry ) {
            // don't execute prun because it takes a while;
            // simply build the archive with tar
            TString extra_dry_cmd(
                Form("tar -czvf %s %s %s ",
                     tarball_name.Data(),
                     gSystem->BaseName(fRunScript->Data()),
                     gSystem->BaseName(fRootScript->Data())));
            if ( fGridUsePrecompiledApp ) {
                // don't add anything here because it gets added later
                ;//jobsub_cmd.Append("--extFile=APlusPlus.tgz ");
                // extra_dry_cmd.Append("APlusPlus.tgz");
            } else {
                extra_dry_cmd.Append(gSystem->BaseName(grid_setup_script.Data()));
            }

            Info(__FUNCTION__, "Execute %s", extra_dry_cmd.Data());
            Int_t err_code = gSystem->Exec(extra_dry_cmd.Data());
            if ( err_code != 0 ) {
                Fatal(__FUNCTION__, "Submission command was not successful. Abort!");
            }
        } else {
            // prun --noSubmit to create archive
            TString dry_cmd = jobsub_cmd;
            dry_cmd.Append(Form("--outTarBall=%s ", tarball_name.Data()));
            dry_cmd.Append(Form("--noSubmit "));
            // Fatal(__FUNCTION__, "Need to add --extFile again to use this function.");
            dry_cmd.Append("--extFile=APlusPlus.tgz ");
            Info("SubmitGridJob", "Execute %s", dry_cmd.Data());
            Int_t err_code = gSystem->Exec(dry_cmd.Data());
            if ( err_code != 0 ) {
                Fatal(__FUNCTION__, "Submission command was not successful. Abort!");
            }
        }
        // write execution command to file
        jobsub_cmd.Append(Form("--inTarBall=%s ", tarball_name.Data()));
        ofstream out;
        out.open(command_file);
        out << jobsub_cmd.Data() << endl;
        out.close();
        Info(__FUNCTION__, "Job not submitted, submission command is in %s.",
             command_file.Data());
    }

}

//____________________________________________________________________

void AtlAppAnalysisTask::SetEvtReader(char const * readerClass, char const * readerArgs) {
    *fReaderClass = (readerClass ? readerClass : "");
    *fReaderArgs = (readerArgs ? readerArgs : "");
}

//____________________________________________________________________

TDataMember* AtlAppAnalysisTask::FindDataMember(TClass *cl,
						const char* DMName) const {
    //
    // Find data nember of given namen in the given class. If it's not
    // found inside the given class, all parent classes are searched
    // recursively.
    //

    // Test given class
    TDataMember *dm = cl->GetDataMember(DMName);
    if ( dm != 0 ) return dm;

    // Test list (multiple inheritance) of all parent classes
    // recursively
    TClass *cl_base = 0;
    TNamed *cl_base_name = 0;
    TIter next_base(cl->GetListOfBases());
    while ( (cl_base_name = (TNamed*)next_base()) ) {
	cl_base = (TClass*)gROOT->GetClass(cl_base_name->GetName());
	dm = FindDataMember(cl_base, DMName);
	if ( dm != 0 ) return dm;
    }

    // Not found ?
    return 0;
}

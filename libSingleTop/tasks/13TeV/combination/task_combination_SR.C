{
    //____________________________________________________________________
    //
    // SgTop combination analysis for the signal region
    //
    // Author: Oliver Maria Kind <mailto: kind@physik.hu-berlin.de>
    // Update: $Id: task_analysis_signal.C,v 1.2 2016/04/19 08:43:44 stamm Exp $
    // Copyright: 2014 (C) Oliver Maria Kind
    //

    enum ESgTopVersion { k15lj, k20lj, k21lj, k23lj, k24lj, k25lj, k29lj, k31lj, k32lj, k34lj};
    static const char* ST_v_str[10] = { "v15_lj", "v20_lj", "v21_lj", "v23_lj", "v24_lj", "v25_lj", "v29_lj",
    			"v31_lj", "v32_lj", "v34_lj"};
    enum EMCVersion { kMC15, kMC16, kMC16a, kMC16d, kMC16e };
    static const char* MC_v_str[5] = { "MC15", "MC16", "MC16a", "MC16d", "MC16e" };

    enum EAnalysisMode {
        kCut,          // 1. do cuts to define regions
        kMemTk,        // 2. MEM LLH calculation on output from 1.
        kMemDisc       // 3. discr./llh histograms from 2.
    };

    // select analysis modes for this file
    EAnalysisMode mode = kCut;
    //EAnalysisMode mode = kMemTk;
    //EAnalysisMode mode = kMemDisc;
    Bool_t is_ctrlplot = kTRUE;    // control plots for 1. or 3.

    // cut options
    Bool_t OnlyOdd = kFALSE ; 			//select only odd evennumber events
    Bool_t TreatAllJetsAsBJets = kFALSE;  // necessary to perform JL QCD MEM correctly!
    Bool_t UseDefaultCuts = kTRUE; //
    Bool_t UseTtbar3JVRCuts = kFALSE;
    Bool_t UseWjetsVRCuts = kFALSE;
    Bool_t UseOutputTree = kTRUE;  // don't write output tree if not needed for MEM
    Bool_t use_Max_cuts = kFALSE;   // see below; for kMemDisc job
    Bool_t UseTtbarSplit = kFALSE;  // v23lj MC16a is split; v13 uses full, v14 uses split
    // v25: don't split for AnalysisOnGrid submissions, manual local runs are split
    Bool_t OverrideInput = kFALSE;  // use previous output as input

    // grid or no grid?
    Bool_t GridJob = kFALSE;
    Bool_t AnalysisOnGrid = kFALSE;
    UseTtbarSplit &= !AnalysisOnGrid;

    // plotting options
    Bool_t UsePlotterSchemeDir = kTRUE;
    // const char* plotter_scheme = "SgTop_s_reduced";
    // const char* plotter_scheme = "SgTop_s";
     //const char* plotter_scheme = "SgTop_s_wjetsMerged";
    const char* plotter_scheme = "SgTop_s_QCDsplit_wjetsMerged"; //////////////////////////////
    //const char* plotter_scheme = "SgTop_s_QCDsplit";
    //const char* plotter_scheme = "SgTop_s_splitall"; //FOR QCD WITH SHAPE SYSTS, and samplesyst
    Bool_t DrawSignalOverlay = kFALSE;
    Bool_t DrawSignificance = kFALSE;

    // ========================
    // Configure top-level task
    // ========================

    // AtlTopLevelAnalysis options
    Bool_t ForceRetry        = kTRUE; // does not work for split MemTk anymore?
    Bool_t BuildHforTree     = kFALSE;  // old
    Bool_t BuildMergingTree  = UseTtbarSplit && is_ctrlplot;  // needed if ttbar is split
    Bool_t BuildAnalysisTree = ( mode == kCut );// && !is_ctrlplot;
    Bool_t BuildMemTkAnalysisTree = ( mode == kMemTk );
    Bool_t BuildMemDiscAnalysisTree = ( mode == kMemDisc ) && !is_ctrlplot;
    Bool_t BuildPlottingTree = is_ctrlplot && (mode != kMemTk);

    // Setup (local) directories
    const char* user = gSystem->Getenv("USER");
    ESgTopVersion sgtop_v = k34lj; // WARNING: removed QCD in ttbarsplit
    EMCVersion mc_v = kMC16e; //v32: a-->as0 , d-->as1, e --> ms
    TString mc_UP = TString(MC_v_str[mc_v]);
    TString mc_low = mc_UP.Copy(); mc_low.ToLower();
    // TString param_string = "eta:jetpt";
    // TString param_string = "jetpt:eta"; // wrong order!!
    // TString param_string = "nbtag";
    // TString param_string = "mtw";  // not available for real; will use eff of 1 in that case
    TString channel = "";  // "el";
    TString param_string_fake = "pt";
    TString param_string_real = "pt";
    TString param_string = Form(
        // "%s_r-%s_f-%s", channel.Data(),
        "r-%s_f-%s",
        param_string_real.Data(), param_string_fake.Data());
    Bool_t UseLepChannelSplit = kFALSE; // evaluate QCD separately for e+mu

    // TString param_string = "eta:jetpt:met";
    //
    // older version strings at bottom of file!
    //
    // switch to v25:
    Bool_t IsSampleSyst = kFALSE;

    ////VERSION v34
    //const char* version = Form("v02_v34lj_NewFullMEM_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v34lj_NewFullMem_Likelihood_%s", MC_v_str[mc_v]);
   // const char* version = Form("v02_v34lj_NewFullMem_Likelihood_QCD_%s", MC_v_str[mc_v]);

    const char* version = Form("v02_v34lj_IllustrateCuts_WithDiLep_%s", MC_v_str[mc_v]);

    //const char* version = Form("v02_v34lj_QCD_NewFullMEM_%s", MC_v_str[mc_v]);
    
    //const char* version = Form("v02_v34lj_77Btag_NewFullMEM_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v34lj_77Btag_QCD_NewFullMEM_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v34lj_sampleSysts_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v34lj_77Btag_sampleSysts_%s", MC_v_str[mc_v]);

    //const char* version = Form("v02_v34lj_Ttbar3jVR_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v34lj_Ttbar3jVR_NoMET_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v34lj_Ttbar3jVR_QCD_NoMET_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v34lj_Ttbar3jVR_NoMTW_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v34lj_Ttbar3jVR_QCD_NoMTW_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v34lj_Ttbar3jVR_QCD_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v34lj_TEST_%s", MC_v_str[mc_v]);


    //const char* version = Form("v02_v34lj_WjetsVR_%s", MC_v_str[mc_v]);
    /////////////
    //const char* version = Form("v02_v34lj_Run_TEST_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v34lj_TEST_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v34lj_EventSelectionFull_%s", MC_v_str[mc_v]);

    //const char* version = Form("v02_v34lj_QCDFIT_NoMTW_%s", MC_v_str[mc_v]);
   // const char* version = Form("v02_v34lj_QCDFIT_ShapeSyst_NoMET_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v34lj_QCDFIT_ShapeSyst_NoMTW_%s", MC_v_str[mc_v]);

    //const char* version = Form("v02_v34lj_MEM_s_channel_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v34lj_DL1r85_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v34lj_TestSampleSyst_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v34lj_tryout_%s", MC_v_str[mc_v]);



/////VERSION 32
   // const char* version = Form("v02_v31lj_TwoJets_condor_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v31lj_sChannel_NO_METandMTW_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v31lj_sChannel_NO_MET_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v31lj_sChannel_NO_MTW_%s", MC_v_str[mc_v]);

    //const char* version = Form("v02_v32lj_sChannel_%s", MC_v_str[mc_v]);
    // const char* version = Form("v02_v32lj_TEST_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v32lj_sChannel_%s", MC_v_str[mc_v]);
     //const char* version = Form("v02_v32lj_sChannel_NoMET_%s", MC_v_str[mc_v]);
   // const char* version = Form("v02_v32lj_2ndGridRun_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v32lj_GridRun_NominalOnly_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v32lj_MemTK_Test_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v32lj_2jVs3jRatioTest_0.8_%s", MC_v_str[mc_v]);


    // const char* version = Form("v02_v32lj_sChannel_NoMET_%s", MC_v_str[mc_v]);

    //const char* version = Form("v02_v31lj_sChannel_NO_MET2%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v31lj_AllJets_NoBTagCut_100thEvent_condor_%s", MC_v_str[mc_v]);
   // const char* version = Form("TEST_%s", MC_v_str[mc_v]);

    //const char* version = Form("v22_v29lj_TestPlotting_%s", MC_v_str[mc_v]);

    //const char* version = Form("v01_v29lj_TesttChannelCuts_%s", MC_v_str[mc_v]);

    /////////
    Bool_t IsManualRerun = kFALSE; // manual local rerun of failed grid jobs
    const char* ext_default = "sgtop_tchan_signal";
    const char* ext = "sgtop_tchan_signal";


    const char* memllhdir = "MemDisc";
    // const char* anadir = Form("/rdsk/dats5/atlas/%s/13TeV_s_channel/%s/%s",
    //                           user, version, ext);
    //const char* anadir = Form("/rdsk/datm7/atlas/%s/combination/A++/%s/%s",  user, version, ext);
    //const char* anadir = Form("/rdsk/dats1/atlas/%s/combination/A++/%s/%s", user, version, ext);
    //const char* anadir = Form("/rdsk/dat21/atlas/%s/combination/A++/%s/%s", user, version, ext);
    //const char* anadir = Form("/rdsk/dat23/atlas/%s/combination/A++/%s/%s", user, version, ext);
	//const char* anadir = Form("/rdsk/dat22/atlas/%s/combination/A++/%s/%s", user, version, ext);
    const char* anadir = Form("/rdsk/dat24/atlas/%s/combination/A++/%s/%s", user, version, ext);
    

    const char* histdir = anadir;
    const char* jobdir = anadir;
    const char* tempdir = Form("/datc/atlas/%s/%s/%s", user, ext, version);

    // to compare MC16a/d: set lumi in fb^-1
    Float_t force_lumi = 0.;//36184.86;
    TString force_plotdir_append = "";//"/plot_36184fb";

    // options for kMemtk
    // const char* GridIdSuffix = "_v14_SRd";   // some control for grid job names
    // const char* GridIdSuffix = "_v15_SRa_test";   // some control for grid job names
    // const char* GridIdSuffix = "_SRa_test_19";   // some control for grid job names
//    const char* GridIdSuffix = "_v25_d_test";   // some control for grid job names
    const char* GridIdSuffix = Form("_v25_%c", TString(MC_v_str[mc_v])[4]);
    const char* GridSuffix = "v1"; // v1: with new TF
    // const char* GridSuffix = "v2"; // v2: sample syst (incl AF2 nominals) and missing wjets samples
    // const char* GridSuffix = "v3"; // v3: rerun nominal
    Bool_t TarBallOnly = kTRUE;
    // TreatAllJetsAsBJets: set at top

    // Chose jet bin
   AtlSelector::EJetMult jetbin = AtlSelector::kTwoJet;

   if(UseTtbar3JVRCuts){
	   jetbin = AtlSelector::kThreeJet;
   }


    AtlTopLevelAnalysis *main
        = new AtlTopLevelAnalysis("AtlSgTop_tChannelAnalysis",
                                  "SgTop tChannel Analysis");

    // execute main (general) macro
    gROOT->Macro("task_combination_main.C");
    //main->SetDebug(2);
    main->SetDrawSignalOverlay(DrawSignalOverlay);
    main->SetScaleOverlay(20.);
    main->SetDrawSignificance(DrawSignificance);

    //main->SetPriority(45); // >10 means only submit to as2xb
    main->SetDebugBuild();
    // main->SetDebug();

    //main->SetNProcessNthEventsOnly(50000); //Process every nth event

    //main->SetNProcessNthEventsOnly(1000); //Process every nth event

    //Default v29
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v29_lj.txt", mc_low.Data()));

    //v31-----------------
    //Test
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v31_lj_Onlytchan.txt", mc_low.Data()));
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v31_lj_OnlyWjets.txt", mc_low.Data()));
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v31_lj_OnlyZ.txt", mc_low.Data()));
     //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v31_lj_OnlyData.txt", mc_low.Data()));
     //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v31_lj_MostImportant.txt", mc_low.Data()));
   //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v31_lj_NoData.txt", mc_low.Data()));
   // main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v31_lj_NoQCD.txt", mc_low.Data()));

    //Default
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v31_lj.txt", mc_low.Data()));


    //v32-------------------------
    //Default
     //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v32_lj.txt", mc_low.Data()));
    //Test
     //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v32_lj_NoTchan.txt", mc_low.Data()));
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v32_lj_FastTest.txt", mc_low.Data()));
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v32_lj_OnlyQCD.txt", mc_low.Data()));
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v32_lj_NoQCD.txt", mc_low.Data()));
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v32_lj_OnlyWJets.txt", mc_low.Data()));
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v32_lj_MemDiscTest.txt", mc_low.Data()));

   //v34-----------------------------------
    //Default
     //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v34_lj.txt", mc_low.Data()));
     main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v34_lj_NoQCD.txt", mc_low.Data()));

    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v34_lj_OnlyQCD.txt", mc_low.Data()));
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v34_lj_samplesyst.txt", mc_low.Data()));

    //ForMCWeights
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v34_lj_wJetsOnly.txt", mc_low.Data()));

    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v34_lj_ttAndWtAndSingle.txt", mc_low.Data()));

    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v34_lj_ttAndWt.txt", mc_low.Data()));
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v34_lj_SingleAndWt.txt", mc_low.Data()));

    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v34_lj_ttbarOnly.txt", mc_low.Data()));
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v34_lj_singleTopOnly.txt", mc_low.Data()));
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v34_lj_WtOnly.txt", mc_low.Data()));

    //For QCD Fit
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v34_lj_NOQCDJL.txt", mc_low.Data()));
     //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v34_lj_ONLYQCDJL.txt", mc_low.Data()));
     //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v34_lj_ONLYQCDAM.txt", mc_low.Data()));

    //Test
     //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v34_lj_TEST.txt", mc_low.Data()));

    Bool_t do_MCWEIGHT_wjetsonly = kFALSE;
    ////NOTE: DO YOU HAVE CORRECT SUWMWEIGHTS??
	Bool_t do_MCWEIGHT_All = kFALSE;
	Bool_t do_MCWEIGHT_ttWt = kFALSE;
	Bool_t do_MCWEIGHT_sWt = kFALSE;
	Bool_t do_MCWEIGHT_ttonly = kFALSE;
	Bool_t do_MCWEIGHT_sonly = kFALSE;
	Bool_t do_MCWEIGHT_wtonly = kFALSE;

     //Since Condor we can submit to all clusters
     main->SetOverrideBatchNodeAll(kTRUE);
     main->SetTaskBatchNodeAutomatic(kFALSE);

    if ( IsSampleSyst ) {
    	main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v34_lj_samplesyst.txt", mc_low.Data()));
        // main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v25_lj_samplesyst.txt", mc_low.Data()));
        //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v29_lj_samplesyst.txt", mc_low.Data()));
    // main->SetDSIDList("../SampleList/SgTopDSIDs_mc16a_v25_lj_samplesyst_grid.txt");
    // main->SetDSIDList("../SampleList/SgTopDSIDs_mc16d_v25_lj_samplesyst.txt");
    // main->SetDSIDList("../SampleList/SgTopDSIDs_mc16d_v25_lj_QCDonly.txt");
    }
    if ( AnalysisOnGrid && IsSampleSyst ) {
        // main->SetGridSampleList("/users/eeh/kaphle/atlas/download/mc16_v25lj/sample_syst_DIDs.txt");
        main->SetGridSampleList(Form("/users/eeh/kaphle/atlas/download/mc16_v29lj/systematics_%s.txt", mc_UP.Data()));
    }
    // main->SetDSIDList(Form("../SampleList/SgTopDSIDs_mc16d_v25_lj_364188-90.txt"));
    main->ReadSampleList("../SampleList");

    // run over previous output
    if ( OverrideInput ) {
        // for cut optimization studies
    	const char* version = Form("v02_v31lj_sChannel_NO_MET_%s", MC_v_str[mc_v]);

        const char* input_ext = ext_default;
        // const char* input_ext = "sgtop_schan_signal_noinvalid";
        // const char* input_ext = "sgtop_schan_firmensignal_test_rerunWithHarderJetPtCut";
        // const char* input_anadir = Form("/rdsk/dats5/atlas/%s/13TeV_s_channel/%s/%s",
        const char* input_anadir = Form("/rdsk/dats1/atlas/%s/combination/A++/%s/%s", user, version, ext);

        const char* override_inputdir = Form("%s/%s", input_anadir, main->GetName());
        //const char* override_inputdir = Form("%s/%s/MemTkAnalysis", input_anadir, main->GetName());
        //    const char* override_inputdir = Form("%s/%s/MemDisc", input_anadir, main->GetName());
        main->SetOverrideInputDir(override_inputdir);
        main->SetTreeName("physics");
        //main->SetCut("fEvtWriterCloneOnly", "kTRUE"); //was FALSE
        main->SetOverrideBatchNodeAll(kTRUE);
    }

    if ( mode == kMemDisc ) {
        main->SetCut("fProcessNegativeLlh", "kFALSE");
    }

    // MemTk config
    // main->SetMemTkRunScript("mem_sChannel_master.C");
    //main->SetMemTkRunScript("mem_sChannel_master_newTF.C");
    main->SetMemTkRunScript("mem_sChannel_master_v32TF_Dl1r.C"); //kkreul: new TF
    //main->SetMemTkRunScript("mem_sChannel_master_TEST.C");
    // main->SetMemTkRunScript("mem_sChannel_master_newTF_btag85.C");
    // main->SetMemTkRunScript("/mnt/eeh/kaphle/atlas/APlusPlus/libSingleTop/tasks/13TeV/s_channel/mem_sChannel_master.C");
    main->SetUseSubJobsAsInput(kTRUE); // for MemDisc
    // main->SetNSubJobsData(1);
    /*main->SetNSubJobsQCD(10); // 1
    main->SetNSubJobsSgTopSChannel(1); // 30
//    main->SetNSubJobsSgTopSChannel(10); // 30
    main->SetNSubJobsSgTopTChannel(10); // 5
    main->SetNSubJobsSgTopWt(50); // 1
    main->SetNSubJobsTtbar(100); // 3: 37h-> 100: ~1h
    main->SetNSubJobsWjetsB(10); // 3
    // main->SetNSubJobsWjetsC(1);
    main->SetNSubJobsZjetsB(10);*/

    // automatic splitting of MEM jobs
    if ( !AnalysisOnGrid && mode == kMemTk ) {
        // main->SetSampleSizeList("SampleSizeLists/mc16_v20_lj_test.txt");
        // main->ReadSampleSizeList("/rdsk/dats5/atlas/kaphle/13TeV_s_channel/v08_v20lj/sgtop_schan_signal/AtlSgTop_sChannelAnalysis/analysis/2Jets/lnu/nominal/");
        // main->ReadSampleSizeList("SampleSizeLists/mc16_v20_lj_test.txt");
        // main->ReadSampleSizeList("SampleSizeLists/mc16_v21_lj_SR.txt");
        // main->ReadSampleSizeList(Form("SampleSizeLists/v13_v23lj_defaultWithBDT_%s_SR.txt", MC_v_str[mc_v]));
        //main->ReadSampleSizeList(Form("SampleSizeLists/v14_v23lj_default_%s_SR.txt", MC_v_str[mc_v])); // v25 default
//        main->ReadSampleSizeList(Form("SampleSizeLists/%s_SR.txt", version));    // use for samplesyst
        // main->ReadSampleSizeList(Form("SampleSizeLists/v14_v23lj_testSyst_%s.txt", MC_v_str[mc_v]));
        if (IsSampleSyst) {
            // main->ReadSampleSizeList(Form("SampleSizeLists/v15_v25lj_default_samplesyst_manual_%s_SR.txt", mc_UP.Data()));    // use for samplesyst
            main->ReadSampleSizeList(
                Form("SampleSizeLists/v16_v29lj_default_samplesyst_%s_SR.txt",
                     mc_UP.Data()));
        } else if (IsManualRerun){
            main->ReadSampleSizeList(Form("SampleSizeLists/v15_v25lj_default_manual_%s_SR.txt", mc_UP.Data()));    // use for samplesyst
        } else {
            main->ReadSampleSizeList(Form("SampleSizeLists/%s_SR.txt", version));
        }
        //main->ReadSampleSizeList(Form("SampleSizeLists/v15_v25lj_default_%s_SR.txt", MC_v_str[mc_v]));
        //main->ReadSampleSizeList(Form("SampleSizeLists/v15_v25lj_noBTag_QCDJetLepton_%s_SR.txt", MC_v_str[mc_v]));
//        main->ReadSampleSizeList(Form("SampleSizeLists/v15_v25lj_default_QCDJetLepton_bTag85_%s_SR.txt", MC_v_str[mc_v]));
        //        main->SetMaxEventsPerSubjob(2000); // default
        //    main->SetMaxEventsPerSubjob(5000);
        // main->SetMaxEventsPerSubjob(1000); // ~16h per job
        //main->SetMaxEventsPerSubjob(250); // ~4h per job
        // main->SetGridMinEvents(300); // not implemented yet
    }
    main->SetMemTkTreatAllJetsAsBJets(TreatAllJetsAsBJets);
    main->SetMemTkTarBallOnly(TarBallOnly);
    main->SetGridAnalysisOnGrid(AnalysisOnGrid);

    // main->SetSkipDATA(kFALSE); // use dsidlist above
    // main->SetSkipQCD(kFALSE);

    // systematics trees; (U)nderstood and (R)uns correctly
    /////////////////
    //v34
    ///////////
    Bool_t do_nominal = kTRUE; // v14a+d

    Bool_t do_Full = kFALSE;


    Bool_t do_JET1 = kFALSE;
	Bool_t do_JET2 = kFALSE;
	Bool_t do_JET3 = kFALSE;
	Bool_t do_MET_MUON = kFALSE;
	Bool_t do_JER1 = kFALSE;
	Bool_t do_JER2 = kFALSE;
	Bool_t do_JET_PileUp = kFALSE;
	Bool_t do_EG = kFALSE;

	Bool_t do_ForwardJVT = kFALSE; //
	Bool_t do_bTagSF = kFALSE;
	Bool_t do_MUSF = kFALSE;
	Bool_t do_ELSF = kFALSE;
	Bool_t do_otherSF = kFALSE;

/*
	Bool_t do_MCWEIGHT_AllNeeded = kTRUE; // For now use this and just run everything that we need and more--> Then
											// in TRExFitter Config collect only what we really need
											// RUn this for ttbar

	Bool_t do_MCWEIGHT_ttbar = kFALSE;
	Bool_t do_MCWEIGHT_SingleTop = kFALSE;
	Bool_t do_MCWEIGHT_wJets = kFALSE;
	Bool_t do_MCWEIGHT_Wt = kFALSE;*/



	//sumWeightsFile
	main->SetMCWeightNamesFile(
	        Form("../SampleList/sumWeights_info_SampleList_as_mc16_v34_lj_SgTopDSIDs_%s_v34_lj.txt",
	             mc_low.Data()));




    //////////////////////////////////////////
    //v34 Systs
    //////////////////////////////////////////
    //main->AddSystematic2("EG_RESOLUTION_ALL__1up"); //checked: changed mc weight search for kkreul : was >= 0
    //main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpNominal);
    //main->AddSystematic2("MCWEIGHT_FIXEDPOS_112"); //checked: This runs on MEMTK nominal sample. Which is good



    //main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpNominal);
    // should follow https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/TopSystematics13TeV

    if ( do_nominal || do_Full ){
    	main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpNominal);
    }


    if ( do_EG || do_Full ){
    main->AddSystematic2("EG_RESOLUTION_ALL__1down");
    main->AddSystematic2("EG_RESOLUTION_ALL__1up");
    main->AddSystematic2("EG_SCALE_AF2__1down");
    main->AddSystematic2("EG_SCALE_AF2__1up");
    main->AddSystematic2("EG_SCALE_ALL__1down");
    main->AddSystematic2("EG_SCALE_ALL__1up");
    }
    if ( do_JET1 || do_Full ){
    main->AddSystematic2("JET_BJES_Response__1down");
    main->AddSystematic2("JET_BJES_Response__1up");
    main->AddSystematic2("JET_EffectiveNP_Detector1__1down");
    main->AddSystematic2("JET_EffectiveNP_Detector1__1up");
    main->AddSystematic2("JET_EffectiveNP_Detector2__1down");
    main->AddSystematic2("JET_EffectiveNP_Detector2__1up");
    main->AddSystematic2("JET_EffectiveNP_Mixed1__1down");
    main->AddSystematic2("JET_EffectiveNP_Mixed1__1up");
    main->AddSystematic2("JET_EffectiveNP_Mixed2__1down");
    main->AddSystematic2("JET_EffectiveNP_Mixed2__1up");
    main->AddSystematic2("JET_EffectiveNP_Mixed3__1down");
    main->AddSystematic2("JET_EffectiveNP_Mixed3__1up");
    main->AddSystematic2("JET_EffectiveNP_Modelling1__1down");
    main->AddSystematic2("JET_EffectiveNP_Modelling1__1up");
    main->AddSystematic2("JET_EffectiveNP_Modelling2__1down");
    main->AddSystematic2("JET_EffectiveNP_Modelling2__1up");
    main->AddSystematic2("JET_EffectiveNP_Modelling3__1down");
    main->AddSystematic2("JET_EffectiveNP_Modelling3__1up");
    main->AddSystematic2("JET_EffectiveNP_Modelling4__1down");
    main->AddSystematic2("JET_EffectiveNP_Modelling4__1up");

    }
    if ( do_JET2 || do_Full ){
    main->AddSystematic2("JET_EffectiveNP_Statistical1__1down");
    main->AddSystematic2("JET_EffectiveNP_Statistical1__1up");
    main->AddSystematic2("JET_EffectiveNP_Statistical2__1down");
    main->AddSystematic2("JET_EffectiveNP_Statistical2__1up");
    main->AddSystematic2("JET_EffectiveNP_Statistical3__1down");
    main->AddSystematic2("JET_EffectiveNP_Statistical3__1up");
    main->AddSystematic2("JET_EffectiveNP_Statistical4__1down");
    main->AddSystematic2("JET_EffectiveNP_Statistical4__1up");
    main->AddSystematic2("JET_EffectiveNP_Statistical5__1down");
    main->AddSystematic2("JET_EffectiveNP_Statistical5__1up");
    main->AddSystematic2("JET_EffectiveNP_Statistical6__1down");
    main->AddSystematic2("JET_EffectiveNP_Statistical6__1up");

    }
    if ( do_JET3 || do_Full ){
    main->AddSystematic2("JET_EtaIntercalibration_Modelling__1down");
    main->AddSystematic2("JET_EtaIntercalibration_Modelling__1up");
    main->AddSystematic2("JET_EtaIntercalibration_NonClosure_2018data__1down");
    main->AddSystematic2("JET_EtaIntercalibration_NonClosure_2018data__1up");
    main->AddSystematic2("JET_EtaIntercalibration_NonClosure_highE__1down");
    main->AddSystematic2("JET_EtaIntercalibration_NonClosure_highE__1up");
    main->AddSystematic2("JET_EtaIntercalibration_NonClosure_negEta__1down");
    main->AddSystematic2("JET_EtaIntercalibration_NonClosure_negEta__1up");
    main->AddSystematic2("JET_EtaIntercalibration_NonClosure_posEta__1down");
    main->AddSystematic2("JET_EtaIntercalibration_NonClosure_posEta__1up");
    main->AddSystematic2("JET_EtaIntercalibration_TotalStat__1down");
    main->AddSystematic2("JET_EtaIntercalibration_TotalStat__1up");
    main->AddSystematic2("JET_Flavor_Composition__1down");
    main->AddSystematic2("JET_Flavor_Composition__1up");
    main->AddSystematic2("JET_Flavor_Response__1down");
    main->AddSystematic2("JET_Flavor_Response__1up");

    }
    if ( do_JER1 || do_Full ){
    main->AddSystematic2("JET_JER_DataVsMC_MC16__1down");
    main->AddSystematic2("JET_JER_DataVsMC_MC16__1up");
    main->AddSystematic2("JET_JER_EffectiveNP_1__1down");
    main->AddSystematic2("JET_JER_EffectiveNP_1__1up");
    main->AddSystematic2("JET_JER_EffectiveNP_2__1down");
    main->AddSystematic2("JET_JER_EffectiveNP_2__1up");
    main->AddSystematic2("JET_JER_EffectiveNP_3__1down");
    main->AddSystematic2("JET_JER_EffectiveNP_3__1up");
    main->AddSystematic2("JET_JER_EffectiveNP_4__1down");
    main->AddSystematic2("JET_JER_EffectiveNP_4__1up");
    main->AddSystematic2("JET_JER_EffectiveNP_5__1down");
    main->AddSystematic2("JET_JER_EffectiveNP_5__1up");
    main->AddSystematic2("JET_JER_EffectiveNP_6__1down");
    main->AddSystematic2("JET_JER_EffectiveNP_6__1up");
    main->AddSystematic2("JET_JER_EffectiveNP_7__1down");
    main->AddSystematic2("JET_JER_EffectiveNP_7__1up");
    main->AddSystematic2("JET_JER_EffectiveNP_8__1down");
    main->AddSystematic2("JET_JER_EffectiveNP_8__1up");
    main->AddSystematic2("JET_JER_EffectiveNP_9__1down");
    main->AddSystematic2("JET_JER_EffectiveNP_9__1up");
    main->AddSystematic2("JET_JER_EffectiveNP_10__1down");
    main->AddSystematic2("JET_JER_EffectiveNP_10__1up");
    main->AddSystematic2("JET_JER_EffectiveNP_11__1down");
    main->AddSystematic2("JET_JER_EffectiveNP_11__1up");

    }
    if ( do_JER2 || do_Full ){
    main->AddSystematic2("JET_JER_EffectiveNP_12restTerm__1down");
    main->AddSystematic2("JET_JER_EffectiveNP_12restTerm__1up");
    main->AddSystematic2("JET_JER_DataVsMC_MC16__1down_PseudoData");
    main->AddSystematic2("JET_JER_DataVsMC_MC16__1up_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_1__1down_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_1__1up_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_2__1down_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_2__1up_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_3__1down_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_3__1up_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_4__1down_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_4__1up_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_5__1down_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_5__1up_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_6__1down_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_6__1up_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_7__1down_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_7__1up_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_8__1down_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_8__1up_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_9__1down_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_9__1up_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_10__1down_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_10__1up_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_11__1down_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_11__1up_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_12restTerm__1down_PseudoData");
    main->AddSystematic2("JET_JER_EffectiveNP_12restTerm__1up_PseudoData");

    }
    if ( do_JET_PileUp || do_Full ){
    main->AddSystematic2("JET_Pileup_OffsetMu__1down");
    main->AddSystematic2("JET_Pileup_OffsetMu__1up");
    main->AddSystematic2("JET_Pileup_OffsetNPV__1down");
    main->AddSystematic2("JET_Pileup_OffsetNPV__1up");
    main->AddSystematic2("JET_Pileup_PtTerm__1down");
    main->AddSystematic2("JET_Pileup_PtTerm__1up");
    main->AddSystematic2("JET_Pileup_RhoTopology__1down");
    main->AddSystematic2("JET_Pileup_RhoTopology__1up");
    main->AddSystematic2("JET_PunchThrough_MC16__1down");
    main->AddSystematic2("JET_PunchThrough_MC16__1up");
    main->AddSystematic2("JET_SingleParticle_HighPt__1down");
    main->AddSystematic2("JET_SingleParticle_HighPt__1up");

    }
    if ( do_MET_MUON || do_Full ){
    main->AddSystematic2("MET_SoftTrk_ResoPara");
    main->AddSystematic2("MET_SoftTrk_ResoPerp");
    main->AddSystematic2("MET_SoftTrk_Scale__1down");
    main->AddSystematic2("MET_SoftTrk_Scale__1up");
    main->AddSystematic2("MUON_ID__1down");
    main->AddSystematic2("MUON_ID__1up");
    main->AddSystematic2("MUON_MS__1down");
    main->AddSystematic2("MUON_MS__1up");
    main->AddSystematic2("MUON_SAGITTA_RESBIAS__1down");
    main->AddSystematic2("MUON_SAGITTA_RESBIAS__1up");
    main->AddSystematic2("MUON_SAGITTA_RHO__1down");
    main->AddSystematic2("MUON_SAGITTA_RHO__1up");
    main->AddSystematic2("MUON_SCALE__1down");
    main->AddSystematic2("MUON_SCALE__1up");
    }

    if ( do_bTagSF || do_Full ) {

            main->AddSystematic2("bTagSF_B_0__1down");
            main->AddSystematic2("bTagSF_B_0__1up");
            main->AddSystematic2("bTagSF_B_1__1down");
            main->AddSystematic2("bTagSF_B_1__1up");
            main->AddSystematic2("bTagSF_B_2__1down");
            main->AddSystematic2("bTagSF_B_2__1up");
            main->AddSystematic2("bTagSF_B_3__1down");
            main->AddSystematic2("bTagSF_B_3__1up");
            main->AddSystematic2("bTagSF_B_4__1down");
            main->AddSystematic2("bTagSF_B_4__1up");
            main->AddSystematic2("bTagSF_B_5__1down");
            main->AddSystematic2("bTagSF_B_5__1up");
            main->AddSystematic2("bTagSF_B_6__1down");
            main->AddSystematic2("bTagSF_B_6__1up");
            main->AddSystematic2("bTagSF_B_7__1down");
            main->AddSystematic2("bTagSF_B_7__1up");
            main->AddSystematic2("bTagSF_B_8__1down");
            main->AddSystematic2("bTagSF_B_8__1up");
            main->AddSystematic2("bTagSF_B_9__1down");
            main->AddSystematic2("bTagSF_B_9__1up");
            main->AddSystematic2("bTagSF_B_10__1down");
            main->AddSystematic2("bTagSF_B_10__1up");
            main->AddSystematic2("bTagSF_B_11__1down");
            main->AddSystematic2("bTagSF_B_11__1up");
            main->AddSystematic2("bTagSF_B_12__1down");
            main->AddSystematic2("bTagSF_B_12__1up");
            main->AddSystematic2("bTagSF_B_13__1down");
            main->AddSystematic2("bTagSF_B_13__1up");
            main->AddSystematic2("bTagSF_B_14__1down");
            main->AddSystematic2("bTagSF_B_14__1up");
            main->AddSystematic2("bTagSF_B_15__1down");
            main->AddSystematic2("bTagSF_B_15__1up");
            main->AddSystematic2("bTagSF_B_16__1down");
            main->AddSystematic2("bTagSF_B_16__1up");
            main->AddSystematic2("bTagSF_B_17__1down");
            main->AddSystematic2("bTagSF_B_17__1up");
            main->AddSystematic2("bTagSF_B_18__1down");
            main->AddSystematic2("bTagSF_B_18__1up");
            main->AddSystematic2("bTagSF_B_19__1down");
            main->AddSystematic2("bTagSF_B_19__1up");
            main->AddSystematic2("bTagSF_B_20__1down");
            main->AddSystematic2("bTagSF_B_20__1up");
            main->AddSystematic2("bTagSF_B_21__1down");
            main->AddSystematic2("bTagSF_B_21__1up");
            main->AddSystematic2("bTagSF_B_22__1down");
            main->AddSystematic2("bTagSF_B_22__1up");
            main->AddSystematic2("bTagSF_B_23__1down");
            main->AddSystematic2("bTagSF_B_23__1up");
            main->AddSystematic2("bTagSF_B_24__1down");
            main->AddSystematic2("bTagSF_B_24__1up");
            main->AddSystematic2("bTagSF_B_25__1down");
            main->AddSystematic2("bTagSF_B_25__1up");
            main->AddSystematic2("bTagSF_B_26__1down");
            main->AddSystematic2("bTagSF_B_26__1up");
            main->AddSystematic2("bTagSF_B_27__1down");
            main->AddSystematic2("bTagSF_B_27__1up");
            main->AddSystematic2("bTagSF_B_28__1down");
            main->AddSystematic2("bTagSF_B_28__1up");
            main->AddSystematic2("bTagSF_B_29__1down");
            main->AddSystematic2("bTagSF_B_29__1up");
            main->AddSystematic2("bTagSF_B_30__1down");
            main->AddSystematic2("bTagSF_B_30__1up");
            main->AddSystematic2("bTagSF_B_31__1down");
            main->AddSystematic2("bTagSF_B_31__1up");
            main->AddSystematic2("bTagSF_B_32__1down");
            main->AddSystematic2("bTagSF_B_32__1up");
            main->AddSystematic2("bTagSF_B_33__1down");
            main->AddSystematic2("bTagSF_B_33__1up");
            main->AddSystematic2("bTagSF_B_34__1down");
            main->AddSystematic2("bTagSF_B_34__1up");
            main->AddSystematic2("bTagSF_B_35__1down");
            main->AddSystematic2("bTagSF_B_35__1up");
            main->AddSystematic2("bTagSF_B_36__1down");
            main->AddSystematic2("bTagSF_B_36__1up");
            main->AddSystematic2("bTagSF_B_37__1down");
            main->AddSystematic2("bTagSF_B_37__1up");
            main->AddSystematic2("bTagSF_B_38__1down");
            main->AddSystematic2("bTagSF_B_38__1up");
            main->AddSystematic2("bTagSF_B_39__1down");
            main->AddSystematic2("bTagSF_B_39__1up");
            main->AddSystematic2("bTagSF_B_40__1down");
            main->AddSystematic2("bTagSF_B_40__1up");
            main->AddSystematic2("bTagSF_B_41__1down");
            main->AddSystematic2("bTagSF_B_41__1up");
            main->AddSystematic2("bTagSF_B_42__1down");
            main->AddSystematic2("bTagSF_B_42__1up");
            main->AddSystematic2("bTagSF_B_43__1down");
            main->AddSystematic2("bTagSF_B_43__1up");
            main->AddSystematic2("bTagSF_B_44__1down");
            main->AddSystematic2("bTagSF_B_44__1up");

            main->AddSystematic2("bTagSF_C_0__1down");
            main->AddSystematic2("bTagSF_C_0__1up");
            main->AddSystematic2("bTagSF_C_1__1down");
            main->AddSystematic2("bTagSF_C_1__1up");
            main->AddSystematic2("bTagSF_C_2__1down");
            main->AddSystematic2("bTagSF_C_2__1up");
            main->AddSystematic2("bTagSF_C_3__1down");
            main->AddSystematic2("bTagSF_C_3__1up");
            main->AddSystematic2("bTagSF_C_4__1down");
            main->AddSystematic2("bTagSF_C_4__1up");
            main->AddSystematic2("bTagSF_C_5__1down");
            main->AddSystematic2("bTagSF_C_5__1up");
            main->AddSystematic2("bTagSF_C_6__1down");
            main->AddSystematic2("bTagSF_C_6__1up");
            main->AddSystematic2("bTagSF_C_7__1down");
            main->AddSystematic2("bTagSF_C_7__1up");
            main->AddSystematic2("bTagSF_C_8__1down");
            main->AddSystematic2("bTagSF_C_8__1up");
            main->AddSystematic2("bTagSF_C_9__1down");
            main->AddSystematic2("bTagSF_C_9__1up");
            main->AddSystematic2("bTagSF_C_10__1down");
            main->AddSystematic2("bTagSF_C_10__1up");
            main->AddSystematic2("bTagSF_C_11__1down");
            main->AddSystematic2("bTagSF_C_11__1up");
            main->AddSystematic2("bTagSF_C_12__1down");
            main->AddSystematic2("bTagSF_C_12__1up");
            main->AddSystematic2("bTagSF_C_13__1down");
            main->AddSystematic2("bTagSF_C_13__1up");
            main->AddSystematic2("bTagSF_C_14__1down");
            main->AddSystematic2("bTagSF_C_14__1up");
            main->AddSystematic2("bTagSF_C_15__1down");
            main->AddSystematic2("bTagSF_C_15__1up");
            main->AddSystematic2("bTagSF_C_16__1down");
            main->AddSystematic2("bTagSF_C_16__1up");
            main->AddSystematic2("bTagSF_C_17__1down");
            main->AddSystematic2("bTagSF_C_17__1up");
            main->AddSystematic2("bTagSF_C_18__1down");
            main->AddSystematic2("bTagSF_C_18__1up");
            main->AddSystematic2("bTagSF_C_19__1down");
            main->AddSystematic2("bTagSF_C_19__1up");

            main->AddSystematic2("bTagSF_Light_0__1down");
            main->AddSystematic2("bTagSF_Light_0__1up");
            main->AddSystematic2("bTagSF_Light_1__1down");
            main->AddSystematic2("bTagSF_Light_1__1up");
            main->AddSystematic2("bTagSF_Light_2__1down");
            main->AddSystematic2("bTagSF_Light_2__1up");
            main->AddSystematic2("bTagSF_Light_3__1down");
            main->AddSystematic2("bTagSF_Light_3__1up");
            main->AddSystematic2("bTagSF_Light_4__1down");
            main->AddSystematic2("bTagSF_Light_4__1up");
            main->AddSystematic2("bTagSF_Light_5__1down");
            main->AddSystematic2("bTagSF_Light_5__1up");
            main->AddSystematic2("bTagSF_Light_6__1down");
            main->AddSystematic2("bTagSF_Light_6__1up");
            main->AddSystematic2("bTagSF_Light_7__1down");
            main->AddSystematic2("bTagSF_Light_7__1up");
            main->AddSystematic2("bTagSF_Light_8__1down");
            main->AddSystematic2("bTagSF_Light_8__1up");
            main->AddSystematic2("bTagSF_Light_9__1down");
            main->AddSystematic2("bTagSF_Light_9__1up");
            main->AddSystematic2("bTagSF_Light_10__1down");
            main->AddSystematic2("bTagSF_Light_10__1up");
            main->AddSystematic2("bTagSF_Light_11__1down");
            main->AddSystematic2("bTagSF_Light_11__1up");
            main->AddSystematic2("bTagSF_Light_12__1down");
            main->AddSystematic2("bTagSF_Light_12__1up");
            main->AddSystematic2("bTagSF_Light_13__1down");
            main->AddSystematic2("bTagSF_Light_13__1up");
            main->AddSystematic2("bTagSF_Light_14__1down");
            main->AddSystematic2("bTagSF_Light_14__1up");
            main->AddSystematic2("bTagSF_Light_15__1down");
            main->AddSystematic2("bTagSF_Light_15__1up");
            main->AddSystematic2("bTagSF_Light_16__1down");
            main->AddSystematic2("bTagSF_Light_16__1up");
            main->AddSystematic2("bTagSF_Light_17__1down");
            main->AddSystematic2("bTagSF_Light_17__1up");
            main->AddSystematic2("bTagSF_Light_18__1down");
            main->AddSystematic2("bTagSF_Light_18__1up");
            main->AddSystematic2("bTagSF_Light_19__1down");
            main->AddSystematic2("bTagSF_Light_19__1up");

        }

    if ( do_otherSF || do_Full ) {
            // v23: pileup not mentioned on twiki, jvt okay
            main->AddSystematic2("Pileup_SF__1down");
            main->AddSystematic2("Pileup_SF__1up");
            main->AddSystematic2("JVT_SF__1down");
            main->AddSystematic2("JVT_SF__1up");
            main->AddSystematic2("ForwardJVT_SF__1up");
            main->AddSystematic2("ForwardJVT_SF__1down");
        }

    if ( do_ELSF || do_Full ) {
            main->AddSystematic2("EL_SF_ID__1down");
            main->AddSystematic2("EL_SF_ID__1up");
            main->AddSystematic2("EL_SF_Isol__1down");
            main->AddSystematic2("EL_SF_Isol__1up");
            main->AddSystematic2("EL_SF_Reco__1down");
            main->AddSystematic2("EL_SF_Reco__1up");
            main->AddSystematic2("EL_SF_Trigger__1down");
            main->AddSystematic2("EL_SF_Trigger__1up");
        }

        if ( do_MUSF || do_Full ) {

            main->AddSystematic2("MU_SF_ID_STAT_LOWPT__1down");
            main->AddSystematic2("MU_SF_ID_STAT_LOWPT__1up");
            main->AddSystematic2("MU_SF_ID_STAT__1down");
            main->AddSystematic2("MU_SF_ID_STAT__1up");
            main->AddSystematic2("MU_SF_ID_SYST_LOWPT__1down");
            main->AddSystematic2("MU_SF_ID_SYST_LOWPT__1up");
            main->AddSystematic2("MU_SF_ID_SYST__1down");
            main->AddSystematic2("MU_SF_ID_SYST__1up");
            main->AddSystematic2("MU_SF_Isol_STAT__1down");
            main->AddSystematic2("MU_SF_Isol_STAT__1up");
            main->AddSystematic2("MU_SF_Isol_SYST__1down");
            main->AddSystematic2("MU_SF_Isol_SYST__1up");
            main->AddSystematic2("MU_SF_TTVA_STAT__1down");
            main->AddSystematic2("MU_SF_TTVA_STAT__1up");
            main->AddSystematic2("MU_SF_TTVA_SYST__1down");
            main->AddSystematic2("MU_SF_TTVA_SYST__1up");
            main->AddSystematic2("MU_SF_Trigger_STAT__1down");
            main->AddSystematic2("MU_SF_Trigger_STAT__1up");
            main->AddSystematic2("MU_SF_Trigger_SYST__1down");
            main->AddSystematic2("MU_SF_Trigger_SYST__1up");

        }

        if ( do_MCWEIGHT_All){
        	//MuR muF
        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_1");
        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_2");
        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_3");
        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_4");

        	//PDF
        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_11"); //nominal for the PDF

        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_115");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_116");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_117");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_118");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_119");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_120");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_121");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_122");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_123");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_124");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_125");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_126");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_127");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_128");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_129");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_130");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_131");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_132");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_133");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_134");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_135");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_136");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_137");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_138");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_139");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_140");
        	        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_141");
        }

        if ( do_MCWEIGHT_ttWt){
        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_142");
        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_143");
        }

        if ( do_MCWEIGHT_sWt){

        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_112");
        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_113");
        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_114");
        }


        if ( do_MCWEIGHT_ttonly){
        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_144");

        	//Parton SHower
        	        	//compare them to the nominal
        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_193");
        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_194");
        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_198");
        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_199");

        }

        if ( do_MCWEIGHT_sonly){
        	//Parton SHower
        	        	//compare them to the nominal
        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_171");
        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_172");
        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_166");
        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_167");
        }

        if ( do_MCWEIGHT_wtonly){
        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_147");
        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_148");
        }

       if ( do_MCWEIGHT_wjetsonly){

        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_5");
        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_6");
        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_8");
        	        	main->AddSystematic2("MCWEIGHT_FIXEDPOS_9");
        }


/*
    if ( fullsyst_v29 ) {
        // main->AddSystematic2("nominal");
        if ( !ignore_manual_done ) {
        main->AddSystematic2("EG_RESOLUTION_ALL__1down");
        main->AddSystematic2("EG_RESOLUTION_ALL__1up");
        main->AddSystematic2("EG_SCALE_AF2__1down");
        main->AddSystematic2("EG_SCALE_AF2__1up");
        main->AddSystematic2("EG_SCALE_ALL__1down");
        main->AddSystematic2("EG_SCALE_ALL__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_BJES_Response__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_BJES_Response__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Detector1__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Detector1__1up");
        // v29: det2 was added again
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Detector2__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Detector2__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Mixed1__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Mixed1__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Mixed2__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Mixed2__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Mixed3__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Mixed3__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Modelling1__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Modelling1__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Modelling2__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Modelling2__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Modelling3__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Modelling3__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Modelling4__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Modelling4__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Statistical1__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Statistical1__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Statistical2__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Statistical2__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Statistical3__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Statistical3__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Statistical4__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Statistical4__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Statistical5__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Statistical5__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Statistical6__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Statistical6__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_Modelling__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_Modelling__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_NonClosure_highE__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_NonClosure_highE__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_NonClosure_negEta__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_NonClosure_negEta__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_NonClosure_posEta__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_NonClosure_posEta__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_TotalStat__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_TotalStat__1up");
        }
        main->AddSystematic2("JET_CategoryReduction_JET_Flavor_Composition__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_Flavor_Composition__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_Flavor_Response__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_Flavor_Response__1up");
        // main->AddSystematic2("JET_CategoryReduction_JET_JER_DataVsMC__1down"); // no JER down
        // v29: name changed for DvsMC: added MC16
        main->AddSystematic2("JET_CategoryReduction_JET_JER_DataVsMC_MC16__1up");
        // main->AddSystematic2("JET_CategoryReduction_JET_JER_DataVsMC__1up");
        // main->AddSystematic2("JET_CategoryReduction_JET_JER_EffectiveNP_1__1down"); // no JER down
        main->AddSystematic2("JET_CategoryReduction_JET_JER_EffectiveNP_1__1up");
        // main->AddSystematic2("JET_CategoryReduction_JET_JER_EffectiveNP_2__1down"); // no JER down
        main->AddSystematic2("JET_CategoryReduction_JET_JER_EffectiveNP_2__1up");
        // main->AddSystematic2("JET_CategoryReduction_JET_JER_EffectiveNP_3__1down"); // no JER down
        main->AddSystematic2("JET_CategoryReduction_JET_JER_EffectiveNP_3__1up");
        // main->AddSystematic2("JET_CategoryReduction_JET_JER_EffectiveNP_4__1down"); // no JER down
        main->AddSystematic2("JET_CategoryReduction_JET_JER_EffectiveNP_4__1up");
        // main->AddSystematic2("JET_CategoryReduction_JET_JER_EffectiveNP_5__1down"); // no JER down
        main->AddSystematic2("JET_CategoryReduction_JET_JER_EffectiveNP_5__1up");
        // main->AddSystematic2("JET_CategoryReduction_JET_JER_EffectiveNP_6__1down"); // no JER down
        main->AddSystematic2("JET_CategoryReduction_JET_JER_EffectiveNP_6__1up");
        // main->AddSystematic2("JET_CategoryReduction_JET_JER_EffectiveNP_7restTerm__1down"); // no JER down
        main->AddSystematic2("JET_CategoryReduction_JET_JER_EffectiveNP_7restTerm__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_Pileup_OffsetMu__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_Pileup_OffsetMu__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_Pileup_OffsetNPV__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_Pileup_OffsetNPV__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_Pileup_PtTerm__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_Pileup_PtTerm__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_Pileup_RhoTopology__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_Pileup_RhoTopology__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_PunchThrough_MC16__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_PunchThrough_MC16__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_SingleParticle_HighPt__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_SingleParticle_HighPt__1up");
        main->AddSystematic2("MET_SoftTrk_ResoPara");
        main->AddSystematic2("MET_SoftTrk_ResoPerp");
        main->AddSystematic2("MET_SoftTrk_ScaleDown");
        main->AddSystematic2("MET_SoftTrk_ScaleUp");
        main->AddSystematic2("MUON_ID__1down");
        main->AddSystematic2("MUON_ID__1up");
        main->AddSystematic2("MUON_MS__1down");
        main->AddSystematic2("MUON_MS__1up");
        if ( !ignore_MU_sagitta_scale ) {
            main->AddSystematic2("MUON_SAGITTA_RESBIAS__1down");
            main->AddSystematic2("MUON_SAGITTA_RESBIAS__1up");
            main->AddSystematic2("MUON_SAGITTA_RHO__1down");
            main->AddSystematic2("MUON_SAGITTA_RHO__1up");
            main->AddSystematic2("MUON_SCALE__1down");
            main->AddSystematic2("MUON_SCALE__1up");
        }
    }
*/
    // Object Selection Tool -> main
    // Reco SF Tool -> main

    // Analysis selector + cuts
    if ( mode == kCut || mode == kMemDisc) {
        //if ( UseDefaultCuts ) {
        if(kFALSE){
        	std::cout<<"tChannel Default Cuts"<<std::endl;
        	//13 TeV tChannel Default
        	//main->SetCut("fNBTags_min", "1");
        	//main->SetCut("fNJets","2");
        	//main->SetCut("fNBTags_max", "1");
        	//main->SetCut("fApplyDileptonVeto", "kTRUE");
        	main->SetCut("fPt_Jet_min", "30.");
        	main->SetCut("fPt_Lepton_min", "30.");
        	//main->SetCut("fMET_min", "30.");
        	//main->SetCut("fPt_ForwardJet_min", "35.");
        	main->SetCut("fMtW_min","50.");
        	main->SetCut("fQCDVeto","kTRUE");

        }

    if(UseDefaultCuts){
    	std::cout<<"SCHANNEL CUTS"<<std::endl;
            // default cuts from 13 TeV sChannel analysis
            main->SetCut("fPt_Jet_min", "30.");
            main->SetCut("fPtJetlead_min", "40.");
            main->SetCut("fApplyDileptonVeto", "kTRUE");
            main->SetCut("fMET_min", "35.");
            main->SetCut("fMtW_minPre", "30.");
            main->SetCut("fMtW_min", "30.");
            main->SetCut("fAdditionalJetVeto", "kFALSE");
            main->SetCut("fNBTags_min", "2");
            main->SetCut("fNBTags_max", "2");
            main->SetCut("fLeadingJetIsBTagged", "kFALSE");
            //main->SetCut("fMCTruthStudy", "kFALSE");


            //main->SetCut("fAdditionalJetVeto", "kFALSE"); // for MEM
           // main->SetCut("fDoReconstructTop", "kFALSE");

            // new idea to avoid changing from default to non-default for JL
            if ( TreatAllJetsAsBJets ) {
                main->SetCut("fNBTags_min", "0");

            }
            if (OnlyOdd){
            	main->SetCut("fOnlyOdd", "kTRUE");
            }
        }
    else if(UseTtbar3JVRCuts){
    	std::cout<<"WARNING::: CHANGE PT and eta OF JET IN main FILE !!!!!!!!!!!!!!!!"<<std::endl;
    	main->SetCut("fPt_Jet_min", "25.");
    	main->SetCut("fPtJetlead_min", "25.");
    	//main->SetCut("fApplyDileptonVeto", "kTRUE");
    	main->SetCut("fMET_min", "35.");
    	main->SetCut("fMtW_minPre", "30.");
    	main->SetCut("fMtW_min", "30.");
    	//main->SetCut("fAdditionalJetVeto", "kTRUE");
    	main->SetCut("fNBTags_min", "2");
    	main->SetCut("fNBTags_max", "2");
    	main->SetCut("fLeadingJetIsBTagged", "kFALSE");
    	main->SetCut("fMCTruthStudy", "kFALSE");
    	 if ( TreatAllJetsAsBJets ) {
    	                main->SetCut("fNBTags_min", "0");

    	            }
    }
    else if(UseWjetsVRCuts){
    	std::cout<<"WARNING::: CHANGE PT OF JET IN main FILE !!!!!!!!!!!!!!!!"<<std::endl;
    	main->SetCut("fPt_Jet_min", "30.");
    	main->SetCut("fPtJetlead_min", "30.");
    	main->SetCut("fApplyDileptonVeto", "kTRUE");
    	main->SetCut("fMET_min", "35.");
    	main->SetCut("fMtW_minPre", "30.");
    	main->SetCut("fMtW_min", "30.");
    	//main->SetCut("fAdditionalJetVeto", "kTRUE");
    	main->SetCut("fNBTags_min", "2");
    	main->SetCut("fNBTags_max", "2");
    	main->SetCut("fLeadingJetIsBTagged", "kFALSE");
    	main->SetCut("fMCTruthStudy", "kFALSE");
    	
    	main->SetCut("fApplyBJetVeto", "kTRUE");   	
    	main->SetCut("fVetoBTagger", "AtlBTag::kDL1r_77");
        main->SetCut("fVetoBTagWeight_min", "0.5");
        
    	 if ( TreatAllJetsAsBJets ) {
    	                main->SetCut("fNBTags_min", "0");

    	            }
    }
    
    } else if ( mode == kMemDisc && use_Max_cuts ) {

    }


    main->SetUseLepChannelSplit(UseLepChannelSplit);


    // ===============
    // Build task tree
    // ===============
    main->BuildTree();
}



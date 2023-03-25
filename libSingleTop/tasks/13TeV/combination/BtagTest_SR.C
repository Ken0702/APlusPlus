{
    //____________________________________________________________________
    //
    // SgTop combination analysis for the signal region
    //
    // Author: Oliver Maria Kind <mailto: kind@physik.hu-berlin.de>
    // Update: $Id: task_analysis_signal.C,v 1.2 2016/04/19 08:43:44 stamm Exp $
    // Copyright: 2014 (C) Oliver Maria Kind
    //

    enum ESgTopVersion { k15lj, k20lj, k21lj, k23lj, k24lj, k25lj, k29lj, k31lj, k32lj};
    static const char* ST_v_str[9] = { "v15_lj", "v20_lj", "v21_lj", "v23_lj", "v24_lj", "v25_lj", "v29_lj",
    			"v31_lj", "v32_lj"};
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
    Bool_t UseDefaultCuts = kTRUE; // 8TeV cuts for kCut job  - UPDATE: remove AddJetVeto!!
    Bool_t UseOutputTree = kTRUE;  // don't write output tree if not needed for MEM
    Bool_t use_Max_cuts = kFALSE;   // see below; for kMemDisc job
    Bool_t UseTtbarSplit = kFALSE;  // v23lj MC16a is split; v13 uses full, v14 uses split
    // v25: don't split for AnalysisOnGrid submissions, manual local runs are split
    Bool_t OverrideInput = kFALSE;  // use previous output as input
    Bool_t TreatAllJetsAsBJets = kFALSE;  // necessary to perform JL QCD MEM correctly?

    // grid or no grid?
    Bool_t GridJob = kFALSE;
    Bool_t AnalysisOnGrid = kFALSE;
    UseTtbarSplit &= !AnalysisOnGrid;

    // plotting options
    Bool_t UsePlotterSchemeDir = kTRUE;
    // const char* plotter_scheme = "SgTop_s_reduced";
    // const char* plotter_scheme = "SgTop_s";
     //const char* plotter_scheme = "SgTop_s_wjetsMerged";
    const char* plotter_scheme = "SgTop_s_QCDsplit_wjetsMerged";
    //const char* plotter_scheme = "SgTop_s_QCDsplit";
//    const char* plotter_scheme = "SgTop_s_splitall";
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
    ESgTopVersion sgtop_v = k32lj; // WARNING: removed QCD in ttbarsplit
    EMCVersion mc_v = kMC16a; //v32: a-->as0 , d-->as1, e --> ms
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

    ////VERSION
    //const char* version = Form("v21_v29lj_OnlyTchan_%s", MC_v_str[mc_v]);
    //const char* version = Form("v21_v29lj_OnlyTTbar_3_%s", MC_v_str[mc_v]);
   // const char* version = Form("v22_%s_NoMETCutForQCDFit_%s",sgtop_v, MC_v_str[mc_v]);
    //const char* version = Form("v01_v31lj_Testv31_sChannelCuts_needTriggerMatchTrue_WithAddJetVeto_FULLMC16d_%s", MC_v_str[mc_v]);
   // const char* version = Form("v01_v31lj_Testv31_OnlyData_%s", MC_v_str[mc_v]);

   // const char* version = Form("v02_v31lj_TwoJets_condor_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v31lj_sChannel_NO_METandMTW_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v31lj_sChannel_NO_MET_%s", MC_v_str[mc_v]);
    //const char* version = Form("v02_v31lj_sChannel_NO_MTW_%s", MC_v_str[mc_v]);

    //const char* version = Form("v02_v31lj_sChannel__%s", MC_v_str[mc_v]);
     //const char* version = Form("v02_v32lj_TEST_%s", MC_v_str[mc_v]);
    const char* version = Form("v02_v32lj_BTag85_sChannel_%s", MC_v_str[mc_v]);
     //const char* version = Form("v02_v32lj_sChannel_NoMTW_%s", MC_v_str[mc_v]);
 //   const char* version = Form("v02_v32lj_MemTK_Test_%s", MC_v_str[mc_v]);
      // const char* version = Form("v02_v32lj_sChannel_NoMET_%s", MC_v_str[mc_v]);


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
    const char* anadir = Form("/rdsk/dats1/atlas/%s/combination/A++/%s/%s", user, version, ext);

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
    //AtlSelector::EJetMult jetbin = AtlSelector::kAllJets;


    AtlTopLevelAnalysis *main
        = new AtlTopLevelAnalysis("AtlSgTop_tChannelAnalysis",
                                  "SgTop tChannel Analysis");

    // execute main (general) macro
    gROOT->Macro("Btag_main.C");
    //main->SetDebug(2);
    main->SetDrawSignalOverlay(DrawSignalOverlay);
    main->SetScaleOverlay(20.);
    main->SetDrawSignificance(DrawSignificance);

    main->SetPriority(10); //Priority for condor
    main->SetDebugBuild();
    // main->SetDebug();
    main->SetTaskBatchNodeAutomatic(kTRUE);
    //main->SetNProcessNthEventsOnly(1000); //Process every nth event

    // Select DSIDList (to exclude/include DATA/QCD)
    //Test
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v29_lj_Fast.txt", mc_low.Data()));
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v29_lj_onlyttbar.txt", mc_low.Data()));
   //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v29_lj_NoData_WithQCD.txt", mc_low.Data()));
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v29_lj_OnlyQCD.txt", mc_low.Data()));

    //NoQCD
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v29_lj_NoQCD.txt", mc_low.Data()));
     // main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v29_lj_OnlyTchan.txt", mc_low.Data()));
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v29_lj_OnlyData.txt", mc_low.Data()));

    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v29_lj_NoTtbarData.txt", mc_low.Data()));
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v29_lj_NoData.txt", mc_low.Data()));

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
     main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v32_lj.txt", mc_low.Data()));
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v32_lj_FastTest.txt", mc_low.Data()));
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v32_lj_OnlyQCD.txt", mc_low.Data()));
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v32_lj_NoQCD.txt", mc_low.Data()));
    //main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v32_lj_OnlyWJets.txt", mc_low.Data()));


    if ( IsSampleSyst ) {
        // main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v25_lj_samplesyst.txt", mc_low.Data()));
        main->SetDSIDList(Form("../SampleList/SgTopDSIDs_%s_v29_lj_samplesyst.txt", mc_low.Data()));
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
        //    const char* input_version = Form("v15_v25lj_test_%s", MC_v_str[mc_v]); // no QCD yet
        // const char* input_version = Form("v15_v25lj_default_%s", MC_v_str[mc_v]);
        //    const char* input_version = Form("v15_v25lj_default_samplesyst_%s", MC_v_str[mc_v]);
        // const char* input_version = version;
        // for QCD SF
        // const char* input_version = Form("v16_v29ljstar_QCDSF_noMETMtWcuts_%s", MC_v_str[mc_v]);
        // for MC weights
        const char* input_version = Form("v16_v29lj_default_v29star_%s", MC_v_str[mc_v]);

        const char* input_ext = ext_default;
        // const char* input_ext = "sgtop_schan_signal_noinvalid";
        // const char* input_ext = "sgtop_schan_firmensignal_test_rerunWithHarderJetPtCut";
        // const char* input_anadir = Form("/rdsk/dats5/atlas/%s/13TeV_s_channel/%s/%s",
        const char* input_anadir = Form("/rdsk/datm7/atlas/%s/13TeV_s_channel/%s/%s",
                                        user, input_version, input_ext);
        //const char* override_inputdir = Form("%s/%s", input_anadir, main->GetName());
        const char* override_inputdir = Form("%s/%s/MemTkAnalysis", input_anadir, main->GetName());
        //    const char* override_inputdir = Form("%s/%s/MemDisc", input_anadir, main->GetName());
        main->SetOverrideInputDir(override_inputdir);
        main->SetTreeName("physics");
        main->SetCut("fEvtWriterCloneOnly", "kFALSE");
        main->SetOverrideBatchNodeAll(kTRUE);
    }

    if ( mode == kMemDisc ) {
        main->SetCut("fProcessNegativeLlh", "kFALSE");
    }

    // MemTk config
    // main->SetMemTkRunScript("mem_sChannel_master.C");
    main->SetMemTkRunScript("mem_sChannel_master_newTF.C");
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
    Bool_t do_nominal = kTRUE; // v14a+d
    Bool_t do_EG = kFALSE;      // v14a+d; tested for v23lj; U+R but AF2 unclear
    Bool_t do_JET1 = kFALSE;    // v14a+d; tested for v23lj, U+R
    Bool_t do_JET2 = kFALSE;    // v14a+d; tested for v23lj, U+R
    Bool_t do_JET3 = kFALSE;    // v14a+d; tested for v23lj, U+R but AF2 unclear
    Bool_t do_JER = kFALSE;     // new in v25lj
    Bool_t do_other = kFALSE;   // v14a+d; tested for v23lj, U+R
    // SF systematics
    Bool_t do_ELSF = kFALSE;    // v14a+d; tested for v23lj, U+R
    Bool_t do_MUSF = kFALSE;    // v14a+d; U+R, unclear what LOWPT is
    Bool_t do_otherSF = kFALSE; // v14a+d; U+R
    Bool_t do_bTagSF = kFALSE;  // v14a+partial? d;

    Bool_t fullsyst_v29 = kFALSE;// all systematics found in v29, minus SF syst
    Bool_t fullsyst_v29_SF = kFALSE; // all scalefactor systematics for v29
    Bool_t ignore_MU_sagitta_scale = kFALSE; //
    Bool_t ignore_manual_done = kFALSE;


    // // manual
    // // // redo ZjetsCL for mc16d: 8 systematics + nominal
    // // // note for CategoryReduction offline: I need to add JET_ later when copying
    // // main->AddSystematic2("nominal"); // don't trust yet, just do above/below
    // main->AddSystematic2("MUON_SAGITTA_RESBIAS__1down");
    // main->AddSystematic2("MUON_SAGITTA_RESBIAS__1up");
    // main->AddSystematic2("MUON_SAGITTA_RHO__1down");
    // main->AddSystematic2("MUON_SAGITTA_RHO__1up");
    // main->AddSystematic2("MUON_SCALE__1down");
    // main->AddSystematic2("MUON_SCALE__1up");
    // main->AddSystematic2("EG_RESOLUTION_ALL__1down");
    // // for mc16d, I ran on grid -> JET_ included, also one PU syst redone
    // // main->AddSystematic2("CategoryReduction_JET_EffectiveNP_Detector1__1down");
    // main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Detector1__1down");
    // main->AddSystematic2("JET_CategoryReduction_JET_Pileup_PtTerm__1up");

    // // PunchThrough was commented before
    // main->AddSystematic2("JET_CategoryReduction_JET_PunchThrough_MC16__1down");
    // main->AddSystematic2("JET_CategoryReduction_JET_PunchThrough_MC16__1up");

    // == MC weights =======
    // need weightnamesfile for a,d,e - change accordingly
    main->SetMCWeightNamesFile(
        Form("../SampleList/sumWeights_info_SampleList_as_mc16_v29_lj_SgTopDSIDs_%s_v29_lj.txt",
             mc_low.Data()));
    // main->SetMCWeightNamesFile(
    //     Form("../SampleList/sumWeights_info_nohighweights_SampleList_as_mc16_v29_lj_SgTopDSIDs_%s_v29_lj_wjets.txt",
    //          mc_low.Data()));
    // main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261000");

    // // template to copy/paste:
    // // main->AddSystematic2("MCWEIGHT_");
    // // main->AddSystematic2("MCWEIGHT_FIXEDPOS_");
    // // isr up: ren:2,fac:2, Var3c Up
    // // - not possible at same time, so just use separately
    // // - for ttbar, isr up affects 410480/82 with hdamp var, not nominal
    // main->AddSystematic2("MCWEIGHT_FIXEDPOS_5");
    // // main->AddSystematic2("MCWEIGHT_muR=050,muF=050"); // for sgtop, num 5
    // // main->AddSystematic2("MCWEIGHT_muR=05,muF=05"); // for ttbar, num 5
    // main->AddSystematic2("MCWEIGHT_Var3cUp");
    // // isr down: ren*2, fac*2, Var3c Down
    // main->AddSystematic2("MCWEIGHT_FIXEDPOS_6");
    // // main->AddSystematic2("MCWEIGHT_muR=200,muF=200"); // for sgtop, num 6
    // // main->AddSystematic2("MCWEIGHT_muR=20,muF=20"); // for ttbar, num 6
    // main->AddSystematic2("MCWEIGHT_Var3cDown");




    // main->AddSystematic2("MCWEIGHT_isr:muRfac=10_fsr:muRfac=20");
    // main->AddSystematic2("MCWEIGHT_isr:muRfac=10_fsr:muRfac=05");
    // PDF: PhPy8 recommendations are to use PDF4LHC error set,
    // i.e. weights 112-141 or PDFset=90901 to PDFset=90930 in sgtop
    // problem: also need 142-144 for ttbar
    // what about alpha_s?
    // need to also run for PDFSET baseline PDF
    Bool_t do_PDFset_PDF4LHC = kFALSE;
    // or should I use the NNPDF3.0 error set? be flexible, add both
    Bool_t do_PDFset_NNPDF30 = kFALSE;
    // for jets, I need different weight names/positions
    Bool_t do_PDFset_NNPDF30_wjets = kFALSE;
    // // wjets has 7-point systematic
    // main->AddSystematic2("MCWEIGHT_MUR05_MUF05_PDF261000");
    // main->AddSystematic2("MCWEIGHT_MUR05_MUF1_PDF261000");
    // main->AddSystematic2("MCWEIGHT_MUR1_MUF05_PDF261000");
    // main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261000");
    // main->AddSystematic2("MCWEIGHT_MUR1_MUF2_PDF261000");
    // main->AddSystematic2("MCWEIGHT_MUR2_MUF1_PDF261000");
    // main->AddSystematic2("MCWEIGHT_MUR2_MUF2_PDF261000");

    // extra jobs necessary for ttbar because it has 3 more weights
    // main->AddSystematic2("MCWEIGHT_FIXEDPOS_142");
    // main->AddSystematic2("MCWEIGHT_FIXEDPOS_143");
    // main->AddSystematic2("MCWEIGHT_FIXEDPOS_144");



    // should follow https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/TopSystematics13TeV
    if ( do_nominal )
        main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpNominal);

    // actually I implemented running two weights at same time for ttbar
    //main->AddSystematic2("MCWEIGHTS_2_X_muR=05,muF=05XVar3cUp");
    //main->AddSystematic2("MCWEIGHTS_2_X_muR=20,muF=20XVar3cDown");
    // // fsr: Py8 Var2; isr:muRfacXYZ according to PmgTopProcesses

    if ( do_EG ) {
        // v23: understood these
        // v25: same
        main->AddSystematic2("EG_RESOLUTION_ALL__1down");
        main->AddSystematic2("EG_RESOLUTION_ALL__1up");
        main->AddSystematic2("EG_SCALE_AF2__1down");
        main->AddSystematic2("EG_SCALE_AF2__1up");
        main->AddSystematic2("EG_SCALE_ALL__1down"); //temp for v25 a
        main->AddSystematic2("EG_SCALE_ALL__1up"); //temp for v25 d
    }

    if ( do_JET1 ) {
        // v23: understood those, 29 (28 for FS samples) JES systematics,
        // v23: 29NP_ByCategory_ → CategoryReduction_
        // v25: same
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Detector1__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Detector1__1up");
        // v23: det2 was removed
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
        // v23: stat7 was removed
        // main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Statistical7__1down");
        // main->AddSystematic2("JET_CategoryReduction_JET_EffectiveNP_Statistical7__1up");
    }

    if ( do_JET2 ) {
        // v25: same as v23
        main->AddSystematic2("JET_CategoryReduction_JET_BJES_Response__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_BJES_Response__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_Modelling__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_Modelling__1up");
        // v23: 3 nonclosure instead of 1
        main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_NonClosure_highE__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_NonClosure_highE__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_NonClosure_negEta__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_NonClosure_negEta__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_NonClosure_posEta__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_NonClosure_posEta__1up");
        // main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_NonClosure__1down");
        // main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_NonClosure__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_TotalStat__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_EtaIntercalibration_TotalStat__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_Flavor_Composition__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_Flavor_Composition__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_Flavor_Response__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_Flavor_Response__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_Pileup_OffsetMu__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_Pileup_OffsetMu__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_Pileup_OffsetNPV__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_Pileup_OffsetNPV__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_Pileup_PtTerm__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_Pileup_PtTerm__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_Pileup_RhoTopology__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_Pileup_RhoTopology__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_SingleParticle_HighPt__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_SingleParticle_HighPt__1up");
    }

    if ( do_JET3 ) {
        // v25: don't use this
        // this could be depending on sample (?)
        main->AddSystematic2("JET_CategoryReduction_JET_PunchThrough_AFII__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_PunchThrough_AFII__1up");
        // v23: mc15 -> mc16
        main->AddSystematic2("JET_CategoryReduction_JET_PunchThrough_MC16__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_PunchThrough_MC16__1up");
        // main->AddSystematic2("JET_CategoryReduction_JET_PunchThrough_MC15__1down");
        // main->AddSystematic2("JET_CategoryReduction_JET_PunchThrough_MC15__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_RelativeNonClosure_AFII__1down");
        main->AddSystematic2("JET_CategoryReduction_JET_RelativeNonClosure_AFII__1up");
        main->AddSystematic2("JET_JER_SINGLE_NP__1up");
    }

    if ( do_JER ) {
        // v25: new in this version, different JER NP; only up
        // v29: name changed for DvsMC: added MC16
        main->AddSystematic2("JET_CategoryReduction_JET_JER_DataVsMC_MC16__1up");
        // main->AddSystematic2("JET_CategoryReduction_JET_JER_DataVsMC__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_JER_EffectiveNP_1__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_JER_EffectiveNP_2__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_JER_EffectiveNP_3__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_JER_EffectiveNP_4__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_JER_EffectiveNP_5__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_JER_EffectiveNP_6__1up");
        main->AddSystematic2("JET_CategoryReduction_JET_JER_EffectiveNP_7restTerm__1up");
    }

    if ( do_other ) {
        // v23: understood, SAGITTA not expl mentioned but available
        // v25: same
        main->AddSystematic2("MET_SoftTrk_ResoPara");
        main->AddSystematic2("MET_SoftTrk_ResoPerp");
        main->AddSystematic2("MET_SoftTrk_ScaleDown");
        main->AddSystematic2("MET_SoftTrk_ScaleUp");
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

    if ( do_ELSF || fullsyst_v29_SF ) {
        // v23: el efficiency SF = SF(reco)*SF(ID)*SF(Iso)*SF(trig)
        // main->AddSystematic2("EL_SF_ChargeID__1down");
        // main->AddSystematic2("EL_SF_ChargeID__1up");
        // main->AddSystematic2("EL_SF_ChargeMisID_STAT__1down");
        // main->AddSystematic2("EL_SF_ChargeMisID_STAT__1up");
        // main->AddSystematic2("EL_SF_ChargeMisID_SYST__1down");
        // main->AddSystematic2("EL_SF_ChargeMisID_SYST__1up");
        // v25: same
        main->AddSystematic2("EL_SF_ID__1down");
        main->AddSystematic2("EL_SF_ID__1up");
        main->AddSystematic2("EL_SF_Isol__1down");
        main->AddSystematic2("EL_SF_Isol__1up");
        main->AddSystematic2("EL_SF_Reco__1down");
        main->AddSystematic2("EL_SF_Reco__1up");
        main->AddSystematic2("EL_SF_Trigger__1down");
        main->AddSystematic2("EL_SF_Trigger__1up");
    }

    if ( do_MUSF || fullsyst_v29_SF ) {
        // v23: all have stat/syst,  Trigger without stat/syst not used
        // v25: same
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
        // main->AddSystematic2("MU_SF_Trigger__1down");
        // main->AddSystematic2("MU_SF_Trigger__1up");
    }

    if ( do_otherSF || fullsyst_v29_SF ) {
        // v23: pileup not mentioned on twiki, jvt okay
        main->AddSystematic2("Pileup_SF__1down");
        main->AddSystematic2("Pileup_SF__1up");
        main->AddSystematic2("JVT_SF__1down");
        main->AddSystematic2("JVT_SF__1up");
    }

    if ( do_bTagSF || fullsyst_v29_SF ) {
        // v23: added extrap.; also do I know the # of components?
        // checked: there are 13 instead of 10 light components
        // v25: check by trial and error: B 0-8, C 0-2, L 0-3
        // v29: check with uproot: bTagSF up to 44 for B, up to 19 for C/Light
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
        // v29: C up to 19
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
        // v25: only up to 3
        // v29: up to 19
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
        // v23: include extrapolation
        // v29: remove extrapolation
        // main->AddSystematic2("bTagSF_extrapolation__1up");
        // main->AddSystematic2("bTagSF_extrapolation__1down");
        // main->AddSystematic2("bTagSF_extrapolation_from_charm__1up");
        // main->AddSystematic2("bTagSF_extrapolation_from_charm__1down");
    }

    if ( do_PDFset_PDF4LHC ) {
        // add baseline PDF4LHC weight
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_11");
        // these names are only for schan, wt, ttbar, not tchan
        // main->AddSystematic2("MCWEIGHT_PDFset=90901");
        // main->AddSystematic2("MCWEIGHT_PDFset=90902");
        // main->AddSystematic2("MCWEIGHT_PDFset=90903");
        // main->AddSystematic2("MCWEIGHT_PDFset=90904");
        // main->AddSystematic2("MCWEIGHT_PDFset=90905");
        // main->AddSystematic2("MCWEIGHT_PDFset=90906");
        // main->AddSystematic2("MCWEIGHT_PDFset=90907");
        // main->AddSystematic2("MCWEIGHT_PDFset=90908");
        // main->AddSystematic2("MCWEIGHT_PDFset=90909");
        // main->AddSystematic2("MCWEIGHT_PDFset=90910");
        // main->AddSystematic2("MCWEIGHT_PDFset=90911");
        // main->AddSystematic2("MCWEIGHT_PDFset=90912");
        // main->AddSystematic2("MCWEIGHT_PDFset=90913");
        // main->AddSystematic2("MCWEIGHT_PDFset=90914");
        // main->AddSystematic2("MCWEIGHT_PDFset=90915");
        // main->AddSystematic2("MCWEIGHT_PDFset=90916");
        // main->AddSystematic2("MCWEIGHT_PDFset=90917");
        // main->AddSystematic2("MCWEIGHT_PDFset=90918");
        // main->AddSystematic2("MCWEIGHT_PDFset=90919");
        // main->AddSystematic2("MCWEIGHT_PDFset=90920");
        // main->AddSystematic2("MCWEIGHT_PDFset=90921");
        // main->AddSystematic2("MCWEIGHT_PDFset=90922");
        // main->AddSystematic2("MCWEIGHT_PDFset=90923");
        // main->AddSystematic2("MCWEIGHT_PDFset=90924");
        // main->AddSystematic2("MCWEIGHT_PDFset=90925");
        // main->AddSystematic2("MCWEIGHT_PDFset=90926");
        // main->AddSystematic2("MCWEIGHT_PDFset=90927");
        // main->AddSystematic2("MCWEIGHT_PDFset=90928");
        // main->AddSystematic2("MCWEIGHT_PDFset=90929");
        // main->AddSystematic2("MCWEIGHT_PDFset=90930");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_112");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_113");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_114");
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
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_142");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_143");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_144");
    }

    if ( do_PDFset_NNPDF30 ) {
        // these names are only for schan, wt, ttbar, not tchan
        // main->AddSystematic2("MCWEIGHT_PDFset=260001");
        // main->AddSystematic2("MCWEIGHT_PDFset=260002");
        // main->AddSystematic2("MCWEIGHT_PDFset=260003");
        // main->AddSystematic2("MCWEIGHT_PDFset=260004");
        // main->AddSystematic2("MCWEIGHT_PDFset=260005");
        // main->AddSystematic2("MCWEIGHT_PDFset=260006");
        // main->AddSystematic2("MCWEIGHT_PDFset=260007");
        // main->AddSystematic2("MCWEIGHT_PDFset=260008");
        // main->AddSystematic2("MCWEIGHT_PDFset=260009");
        // main->AddSystematic2("MCWEIGHT_PDFset=260010");
        // main->AddSystematic2("MCWEIGHT_PDFset=260011");
        // main->AddSystematic2("MCWEIGHT_PDFset=260012");
        // main->AddSystematic2("MCWEIGHT_PDFset=260013");
        // main->AddSystematic2("MCWEIGHT_PDFset=260014");
        // main->AddSystematic2("MCWEIGHT_PDFset=260015");
        // main->AddSystematic2("MCWEIGHT_PDFset=260016");
        // main->AddSystematic2("MCWEIGHT_PDFset=260017");
        // main->AddSystematic2("MCWEIGHT_PDFset=260018");
        // main->AddSystematic2("MCWEIGHT_PDFset=260019");
        // main->AddSystematic2("MCWEIGHT_PDFset=260020");
        // main->AddSystematic2("MCWEIGHT_PDFset=260021");
        // main->AddSystematic2("MCWEIGHT_PDFset=260022");
        // main->AddSystematic2("MCWEIGHT_PDFset=260023");
        // main->AddSystematic2("MCWEIGHT_PDFset=260024");
        // main->AddSystematic2("MCWEIGHT_PDFset=260025");
        // main->AddSystematic2("MCWEIGHT_PDFset=260026");
        // main->AddSystematic2("MCWEIGHT_PDFset=260027");
        // main->AddSystematic2("MCWEIGHT_PDFset=260028");
        // main->AddSystematic2("MCWEIGHT_PDFset=260029");
        // main->AddSystematic2("MCWEIGHT_PDFset=260030");
        // main->AddSystematic2("MCWEIGHT_PDFset=260031");
        // main->AddSystematic2("MCWEIGHT_PDFset=260032");
        // main->AddSystematic2("MCWEIGHT_PDFset=260033");
        // main->AddSystematic2("MCWEIGHT_PDFset=260034");
        // main->AddSystematic2("MCWEIGHT_PDFset=260035");
        // main->AddSystematic2("MCWEIGHT_PDFset=260036");
        // main->AddSystematic2("MCWEIGHT_PDFset=260037");
        // main->AddSystematic2("MCWEIGHT_PDFset=260038");
        // main->AddSystematic2("MCWEIGHT_PDFset=260039");
        // main->AddSystematic2("MCWEIGHT_PDFset=260040");
        // main->AddSystematic2("MCWEIGHT_PDFset=260041");
        // main->AddSystematic2("MCWEIGHT_PDFset=260042");
        // main->AddSystematic2("MCWEIGHT_PDFset=260043");
        // main->AddSystematic2("MCWEIGHT_PDFset=260044");
        // main->AddSystematic2("MCWEIGHT_PDFset=260045");
        // main->AddSystematic2("MCWEIGHT_PDFset=260046");
        // main->AddSystematic2("MCWEIGHT_PDFset=260047");
        // main->AddSystematic2("MCWEIGHT_PDFset=260048");
        // main->AddSystematic2("MCWEIGHT_PDFset=260049");
        // main->AddSystematic2("MCWEIGHT_PDFset=260050");
        // main->AddSystematic2("MCWEIGHT_PDFset=260051");
        // main->AddSystematic2("MCWEIGHT_PDFset=260052");
        // main->AddSystematic2("MCWEIGHT_PDFset=260053");
        // main->AddSystematic2("MCWEIGHT_PDFset=260054");
        // main->AddSystematic2("MCWEIGHT_PDFset=260055");
        // main->AddSystematic2("MCWEIGHT_PDFset=260056");
        // main->AddSystematic2("MCWEIGHT_PDFset=260057");
        // main->AddSystematic2("MCWEIGHT_PDFset=260058");
        // main->AddSystematic2("MCWEIGHT_PDFset=260059");
        // main->AddSystematic2("MCWEIGHT_PDFset=260060");
        // main->AddSystematic2("MCWEIGHT_PDFset=260061");
        // main->AddSystematic2("MCWEIGHT_PDFset=260062");
        // main->AddSystematic2("MCWEIGHT_PDFset=260063");
        // main->AddSystematic2("MCWEIGHT_PDFset=260064");
        // main->AddSystematic2("MCWEIGHT_PDFset=260065");
        // main->AddSystematic2("MCWEIGHT_PDFset=260066");
        // main->AddSystematic2("MCWEIGHT_PDFset=260067");
        // main->AddSystematic2("MCWEIGHT_PDFset=260068");
        // main->AddSystematic2("MCWEIGHT_PDFset=260069");
        // main->AddSystematic2("MCWEIGHT_PDFset=260070");
        // main->AddSystematic2("MCWEIGHT_PDFset=260071");
        // main->AddSystematic2("MCWEIGHT_PDFset=260072");
        // main->AddSystematic2("MCWEIGHT_PDFset=260073");
        // main->AddSystematic2("MCWEIGHT_PDFset=260074");
        // main->AddSystematic2("MCWEIGHT_PDFset=260075");
        // main->AddSystematic2("MCWEIGHT_PDFset=260076");
        // main->AddSystematic2("MCWEIGHT_PDFset=260077");
        // main->AddSystematic2("MCWEIGHT_PDFset=260078");
        // main->AddSystematic2("MCWEIGHT_PDFset=260079");
        // main->AddSystematic2("MCWEIGHT_PDFset=260080");
        // main->AddSystematic2("MCWEIGHT_PDFset=260081");
        // main->AddSystematic2("MCWEIGHT_PDFset=260082");
        // main->AddSystematic2("MCWEIGHT_PDFset=260083");
        // main->AddSystematic2("MCWEIGHT_PDFset=260084");
        // main->AddSystematic2("MCWEIGHT_PDFset=260085");
        // main->AddSystematic2("MCWEIGHT_PDFset=260086");
        // main->AddSystematic2("MCWEIGHT_PDFset=260087");
        // main->AddSystematic2("MCWEIGHT_PDFset=260088");
        // main->AddSystematic2("MCWEIGHT_PDFset=260089");
        // main->AddSystematic2("MCWEIGHT_PDFset=260090");
        // main->AddSystematic2("MCWEIGHT_PDFset=260091");
        // main->AddSystematic2("MCWEIGHT_PDFset=260092");
        // main->AddSystematic2("MCWEIGHT_PDFset=260093");
        // main->AddSystematic2("MCWEIGHT_PDFset=260094");
        // main->AddSystematic2("MCWEIGHT_PDFset=260095");
        // main->AddSystematic2("MCWEIGHT_PDFset=260096");
        // main->AddSystematic2("MCWEIGHT_PDFset=260097");
        // main->AddSystematic2("MCWEIGHT_PDFset=260098");
        // main->AddSystematic2("MCWEIGHT_PDFset=260099");
        // main->AddSystematic2("MCWEIGHT_PDFset=260100");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_12");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_13");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_14");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_15");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_16");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_17");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_18");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_19");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_20");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_21");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_22");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_23");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_24");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_25");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_26");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_27");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_28");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_29");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_30");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_31");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_32");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_33");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_34");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_35");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_36");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_37");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_38");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_39");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_40");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_41");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_42");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_43");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_44");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_45");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_46");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_47");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_48");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_49");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_50");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_51");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_52");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_53");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_54");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_55");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_56");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_57");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_58");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_59");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_60");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_61");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_62");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_63");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_64");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_65");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_66");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_67");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_68");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_69");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_70");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_71");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_72");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_73");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_74");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_75");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_76");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_77");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_78");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_79");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_80");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_81");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_82");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_83");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_84");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_85");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_86");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_87");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_88");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_89");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_90");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_91");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_92");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_93");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_94");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_95");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_96");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_97");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_98");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_99");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_100");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_101");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_102");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_103");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_104");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_105");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_106");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_107");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_108");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_109");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_110");
        main->AddSystematic2("MCWEIGHT_FIXEDPOS_111");
    }

    if ( do_PDFset_NNPDF30_wjets ) {
        // these names are only for wjets
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261001");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261002");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261003");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261004");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261005");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261006");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261007");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261008");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261009");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261010");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261011");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261012");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261013");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261014");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261015");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261016");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261017");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261018");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261019");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261020");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261021");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261022");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261023");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261024");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261025");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261026");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261027");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261028");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261029");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261030");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261031");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261032");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261033");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261034");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261035");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261036");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261037");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261038");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261039");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261040");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261041");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261042");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261043");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261044");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261045");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261046");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261047");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261048");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261049");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261050");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261051");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261052");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261053");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261054");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261055");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261056");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261057");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261058");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261059");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261060");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261061");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261062");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261063");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261064");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261065");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261066");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261067");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261068");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261069");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261070");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261071");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261072");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261073");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261074");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261075");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261076");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261077");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261078");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261079");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261080");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261081");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261082");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261083");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261084");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261085");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261086");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261087");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261088");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261089");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261090");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261091");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261092");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261093");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261094");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261095");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261096");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261097");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261098");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261099");
        main->AddSystematic2("MCWEIGHT_MUR1_MUF1_PDF261100");
   }

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

    // Object Selection Tool -> main
    // Reco SF Tool -> main

    // Analysis selector + cuts
    if ( mode == kCut ) {
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

    if(kTRUE){
    	std::cout<<"SCHANNEL CUTS"<<std::endl;
            // default cuts from 13 TeV sChannel analysis
            main->SetCut("fPt_Jet_min", "30.");
            main->SetCut("fPtJetlead_min", "40.");
            main->SetCut("fApplyDileptonVeto", "kTRUE");
            main->SetCut("fMET_min", "35.");
            main->SetCut("fMtW_minPre", "30.");
            main->SetCut("fMtW_min", "30.");
            main->SetCut("fAdditionalJetVeto", "kTRUE");
            main->SetCut("fNBTags_min", "2");
            main->SetCut("fNBTags_max", "2");
            main->SetCut("fLeadingJetIsBTagged", "kFALSE");
            main->SetCut("fMCTruthStudy", "kFALSE");


            //main->SetCut("fAdditionalJetVeto", "kFALSE"); // for MEM
           // main->SetCut("fDoReconstructTop", "kFALSE");

            // new idea to avoid changing from default to non-default for JL
            if ( TreatAllJetsAsBJets ) {
                main->SetCut("fNBTags_min", "0");
            }
        }
    } else if ( mode == kMemDisc && use_Max_cuts ) {

    }

    // QCD MM Tool
    AtlQCDMMTool *tool_qcd = new AtlQCDMMTool("QCDMMTool", "QCD Matrix Method");
    main->AddTool(tool_qcd);
    main->SetUseLepChannelSplit(UseLepChannelSplit);

    // // Efficiencies by JB, 2018-07-02
    // main->SetToolParameter("QCDMMTool", "fEfficiencyPath2015", "$APP_HOME/Rel21_efficiencies_20180702");
    // main->SetToolParameter("QCDMMTool", "fEfficiencyPath2016", "$APP_HOME/Rel21_efficiencies_20180702");
    // main->SetToolParameter("QCDMMTool", "fEfficiencyPath2017", "$APP_HOME/Rel21_efficiencies_20180702");
    // main->SetToolParameter("QCDMMTool", "fParamString_fake2015", Form("2015:NS:%s", param_string.Data()));
    // main->SetToolParameter("QCDMMTool", "fParamString_real2015", Form("2015:NS:%s", param_string.Data()));
    // main->SetToolParameter("QCDMMTool", "fParamString_fake2016", Form("2016:NS:%s", param_string.Data()));
    // main->SetToolParameter("QCDMMTool", "fParamString_real2016", Form("2016:NS:%s", param_string.Data()));
    // main->SetToolParameter("QCDMMTool", "fParamString_fake2017", Form("2017:NS:%s", param_string.Data()));
    // main->SetToolParameter("QCDMMTool", "fParamString_real2017", Form("2017:NS:%s", param_string.Data()));

    // Efficiencies by NB, Oct 2018, diff. param for real/fake
    main->SetToolParameter("QCDMMTool", "fEfficiencyPath2015", "$APP_HOME/Rel21_efficiencies_October2018");
    main->SetToolParameter("QCDMMTool", "fEfficiencyPath2016", "$APP_HOME/Rel21_efficiencies_October2018");
    main->SetToolParameter("QCDMMTool", "fEfficiencyPath2017", "$APP_HOME/Rel21_efficiencies_October2018");
    main->SetToolParameter("QCDMMTool", "fParamString_fake2015", Form("2015:NS:%s", param_string_fake.Data()));
    main->SetToolParameter("QCDMMTool", "fParamString_real2015", Form("2015_Real:NS:%s", param_string_real.Data()));
    main->SetToolParameter("QCDMMTool", "fParamString_fake2016", Form("2016:NS:%s", param_string_fake.Data()));
    main->SetToolParameter("QCDMMTool", "fParamString_real2016", Form("2016_Real:NS:%s", param_string_real.Data()));
    main->SetToolParameter("QCDMMTool", "fParamString_fake2017", Form("2017:NS:%s", param_string_fake.Data()));
    main->SetToolParameter("QCDMMTool", "fParamString_real2017", Form("2017_Real:NS:%s", param_string_real.Data()));

    // ===============
    // Build task tree
    // ===============
    main->BuildTree();
}

// old version strings for reference
    // const char* version = "v08_v20lj";
    // const char* version = "v09_cuts_bTag60";
    // const char* version = "v10_v21lj_MC16a";
    // const char* version = "v10_v21lj_MC16a_newQCD";
    // const char* version = "v10_v21lj";
    // const char* version = "v10_v21lj_MC16d";
    // const char* version = "v11_v21lj_MC16a_cutopt_base";
    // const char* version = "v11_v21lj_MC16d_cutopt_base";
    // const char* version = "v11_v21lj_MC16a";
    // const char* version = Form("v11_v21lj_QCDtests_NB0615_%s_%s",
    //                            MC_v_str[mc_v], param_string.Data());
    // const char* version = Form("v12_v21lj_cutopt_addvars_%s_%s",
    // const char* version = Form("v12_v21lj_cutopt_fulltree_%s_%s",
    // const char* version = Form("v12_v21lj_cutopt_testcut_%s_%s_jet1pt80",
    //                            MC_v_str[mc_v], param_string.Data());
    // const char* version = Form("v13_v23lj_noQCD_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_QCDtests_AntiMuon_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_QCDtests_JetLepton_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_QCDtests_JetLeptonNoBTag_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_QCDtests__%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_default_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_testBDTCutflow_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_defaultWithBDT_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_testBDThist_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_QCDtests_JM0702_%s_%s",
    //                            MC_v_str[mc_v], param_string.Data());

    // planned default for v23lj including all systematics and output tree etc plus syst/splitting tests
    // const char* version = Form("v14_v23lj_default_%s", MC_v_str[mc_v]); // -addjetveto, +syst
    // const char* version = Form("v14_v23lj_testSyst_%s", MC_v_str[mc_v]); // -addjetveto, +syst

    // for study on the effects of the AddJetVeto in the signal region:
//    const char* version = Form("v14_v23lj_withoutAddJetVeto_%s", MC_v_str[mc_v]); // -addjetveto, +syst
    //const char* version = Form("v14_v23lj_withAddJetVeto_%s", MC_v_str[mc_v]); // +addjetveto, +syst
    // const char* version = Form("v14_v23lj_withAddJetVeto_pt_%s", MC_v_str[mc_v]); // + veto just with pt
    // const char* version = Form("v14_v23lj_withAddJetVeto_eta_%s", MC_v_str[mc_v]); // + veto just with eta

    // study new TF
    // const char* version = Form("v14_v23lj_default_testnewTF_%s", MC_v_str[mc_v]); // copied kCut result from default with cp -l
    // MC16d MEM: last submitted was 364122ZeeB003 (+some?) 364125ZeeB001

    // tests for v25lj grid submission campaign
    // const char* version = Form("v15_v25lj_testSFsyst_%s", MC_v_str[mc_v]);
    // const char* version = Form("v15_v25lj_test_%s", MC_v_str[mc_v]); // no QCD yet
//    const char* version = Form("v15_v25lj_test_newApp_%s", MC_v_str[mc_v]); // no QCD yet
    // const char* version = Form("v15_v25lj_testsubmission_19_%s", MC_v_str[mc_v]); // no QCD yet
    // const char* version = Form("v15_v25lj_test_19_%s", MC_v_str[mc_v]); // no QCD yet
    // const char* version = Form("v15_v25lj_test_leptonSF_tight_%s", MC_v_str[mc_v]);
    // const char* version = Form("v15_v25lj_test_rerun_%s", MC_v_str[mc_v]); // no QCD yet

// old QCD efficiency settings for reference
    // main->SetToolParameter("QCDMMTool", "fEfficiencyPath2015", "$APP_HOME/external/TopFakes/data/20p7/2015");
    // main->SetToolParameter("QCDMMTool", "fEfficiencyPath2016", "$APP_HOME/external/TopFakes/data/20p7/2016");
    // main->SetToolParameter("QCDMMTool", "fParamString_fake2015", "2015Test:NS:jetpt:eta:dR");
    // main->SetToolParameter("QCDMMTool", "fParamString_real2015", "2015Test:NS:jetpt:eta:dR");
    // main->SetToolParameter("QCDMMTool", "fParamString_fake2016", "2016Test:NS:jetpt:eta:dR");
    // main->SetToolParameter("QCDMMTool", "fParamString_real2016", "2016Test:NS:jetpt:eta:dR");

    // // including 2017:
    // main->SetToolParameter("QCDMMTool", "fEfficiencyPath", "$APP_HOME/Efficiencies_rel21/21p2");
    // main->SetToolParameter("QCDMMTool", "fParamString_fake", "Test:NS:jetpt:eta:dR");
    // main->SetToolParameter("QCDMMTool", "fParamString_real", "Test:NS:jetpt:eta:dR");

    // // Efficiencies by NB, 2018-06-15
    // main->SetToolParameter("QCDMMTool", "fEfficiencyPath2015", "$APP_HOME/Rel21_efficiencies_20180615");
    // main->SetToolParameter("QCDMMTool", "fEfficiencyPath2016", "$APP_HOME/Rel21_efficiencies_20180615");
    // main->SetToolParameter("QCDMMTool", "fEfficiencyPath2017", "$APP_HOME/Rel21_efficiencies_20180615");
    // main->SetToolParameter("QCDMMTool", "fParamString_fake2015", Form("2015:NS:%s", param_string.Data()));
    // main->SetToolParameter("QCDMMTool", "fParamString_real2015", Form("2015_Real:NS:%s", param_string.Data()));
    // main->SetToolParameter("QCDMMTool", "fParamString_fake2016", Form("2016:NS:%s", param_string.Data()));
    // main->SetToolParameter("QCDMMTool", "fParamString_real2016", Form("2016_Real:NS:%s", param_string.Data()));
    // main->SetToolParameter("QCDMMTool", "fParamString_fake2017", Form("2017:NS:%s", param_string.Data()));
    // main->SetToolParameter("QCDMMTool", "fParamString_real2017", Form("2017_Real:NS:%s", param_string.Data()));

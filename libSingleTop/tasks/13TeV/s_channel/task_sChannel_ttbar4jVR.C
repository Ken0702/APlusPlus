{
    //____________________________________________________________________
    //
    // SgTop s-channel analysis for the ttbar+4jets validation region
    //
    // Author: Stephan Kaphle <mailto:kaphle@physik.hu-berlin.de>
    // Update: $Id: task_analysis_signal.C,v 1.2 2016/04/19 08:43:44 stamm Exp $
    // Copyright: 2014 (C) Oliver Maria Kind
    //   

    enum ESgTopVersion { k15lj, k20lj, k21lj, k23lj, k24lj, k25lj, k29lj};
    static const char* ST_v_str[7] = { "v15_lj", "v20_lj", "v21_lj", "v23_lj", "v24_lj", "v25_lj", "v29_lj"};
    enum EMCVersion { kMC15, kMC16, kMC16a, kMC16d, kMC16e };
    static const char* MC_v_str[5] = { "MC15", "MC16", "MC16a", "MC16d", "MC16e" };

    enum EAnalysisMode {
        kCut,          // 1. do cuts to define regions
        kMemTk,        // 2. MEM LLH calculation on output from 1.
        kMemDisc       // 3. discr./llh histograms from 2. 
    };
    
    // select analysis modes for this file
    EAnalysisMode mode = kCut;
    // EAnalysisMode mode = kMemTk;
    // EAnalysisMode mode = kMemDisc;
    Bool_t is_ctrlplot = kTRUE;       // control plots for 1. or 3.
 
    // cut options
    Bool_t UseDefaultCuts = kFALSE; // 8TeV cuts for kCut job  - UPDATE: remove AddJetVeto!!
    Bool_t UseOutputTree = kFALSE;  // don't write output tree if not needed for MEM
    Bool_t UseTtbarSplit = kFALSE;  // v23lj MC16a is split; v13 uses full, v14 uses split
    Bool_t OverrideInput = kFALSE;  // use previous output as input

    // plotting options
    Bool_t UsePlotterSchemeDir = kTRUE;
    // const char* plotter_scheme = "SgTop_s";
    const char* plotter_scheme = "SgTop_s_QCDsplit_wjetsMerged";

    // ========================
    // Configure top-level task
    // ========================

    // AtlTopLevelAnalysis options
    Bool_t ForceRetry        = kFALSE;
    Bool_t BuildHforTree     = kFALSE;
    Bool_t BuildMergingTree  = UseTtbarSplit && is_ctrlplot;  // needed if ttbar is split
    Bool_t BuildAnalysisTree = ( mode == kCut ) && !is_ctrlplot;
    Bool_t BuildMemTkAnalysisTree = ( mode == kMemTk );
    Bool_t BuildMemDiscAnalysisTree = ( mode == kMemDisc ) && !is_ctrlplot;
    Bool_t BuildPlottingTree = is_ctrlplot && (mode != kMemTk);

    // Setup (local) directories
    const char* user = gSystem->Getenv("USER");
    ESgTopVersion sgtop_v = k29lj;
    EMCVersion mc_v = kMC16e;
    TString param_string = "NS:pt";
    // TString param_string = "eta";
    // TString param_string = "dR";
    // TString param_string = "eta:pt";
    // TString param_string = "eta:jetpt:met";

    // switch to v25:
//    const char* version = Form("v15_v25lj_default_%s", MC_v_str[mc_v]);
//    const char* version = Form("v15_v25lj_QCD_scale_factors_%s", MC_v_str[mc_v]);
//    const char* version = Form("v15_v25lj_WithAddJetVeto_%s", MC_v_str[mc_v]);
    
    // const char* version = Form("v11_v21lj_QCDtests_NB0615_%s_%s",
    // const char* version = Form("v12_v21lj_cutopt_addvars_%s_%s",
    //                            MC_v_str[mc_v], param_string.Data());
    // const char* version = Form("v13_v23lj_noQCD_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_AntiMuon_SF_derivation_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_QCDtests_AntiMuon_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_QCDtests_JetLepton_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_QCDtests_JM0702_%s_%s",
    //                            MC_v_str[mc_v], param_string.Data());
    // const char* version = Form("v13_v23lj_Wuppetal_cuts_%s", MC_v_str[mc_v]);
//    const char* version = Form("v13_v23lj_yields_noAddJetVeto_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_yields_noAddJetVeto_JetLeptonNoBTag_%s", MC_v_str[mc_v]);

    // switch to v29:
    // v29 partial implementation: Wjets already v29start, Zjets still v29
//    const char* version = Form("v16_v29lj_Wjv29star_Zjv29_%s", MC_v_str[mc_v]);
    // v29* full implementation: Wjets, Zjets, Diboson
    const char* version = Form("v16_v29lj_default_v29star_%s", MC_v_str[mc_v]);
    // const char* version = Form("v16_v29ljstar_QCDSF_noMETMtWcuts_%s", MC_v_str[mc_v]);
    // const char* version = Form("v16_v29ljstar_QCDSF_noMETcut_%s", MC_v_str[mc_v]);
//    const char* version = Form("v16_v29ljstar_QCDSF_noMtWcut_%s", MC_v_str[mc_v]);


    const char* ext = "sgtop_schan_ttbar4j";
//    const char* ext = "sgtop_schan_ttbar4j_noMETMtWcuts";
    const char* memllhdir = "MemDisc";
//    const char* anadir = Form("/rdsk/dats5/atlas/%s/13TeV_s_channel/%s/%s",
    const char* anadir = Form("/rdsk/datm8/atlas/%s/13TeV_s_channel/%s/%s",
                              user, version, ext);
    const char* histdir = anadir;
    const char* jobdir = anadir;
    const char* tempdir = Form("/datc/atlas/%s/%s/%s", user, ext, version);

     // to compare MC16a/d: set lumi in fb^-1
    Float_t force_lumi = 0.;//36184.86;
    TString force_plotdir_append = "";//"/plot_36184fb";

    // options for kMemtk
    Bool_t GridJob = kFALSE;
    const char* GridIdSuffix = "_ttbar4jVR_20th_4JetVeto"; // add _20th for every 20th event
    const char* GridSuffix = "v1"; // as v07_MEM

    // Chose jet bin
    AtlSelector::EJetMult jetbin = AtlSelector::kFourJet;

    AtlTopLevelAnalysis *main
	= new AtlTopLevelAnalysis("AtlSgTop_sChannelAnalysis",
				  "SgTop sChannel Analysis");

    // execute main (general) macro
    gROOT->Macro("task_sChannel_main.C");
    // main->SetDebug(1);

    // Select DSIDList (to exclude/include DATA/QCD) - only for overriding
    // main->SetDSIDList("../SampleList/SgTopDSIDs_mc16d_v21_lj.txt");
    // main->SetDSIDList("../SampleList/SgTopDSIDs_mc16d_v21_lj_withoutDATAQCD.txt");
    // main->SetDSIDList("../SampleList/SgTopDSIDs_mc16_v20_lj.txt");
    // main->SetDSIDList("../SampleList/SgTopDSIDs_mc15_v15_lj.txt");
//    main->SetDSIDList(Form("../SampleList/SgTopDSIDs_mc16d_v25_lj_364188-90.txt"));
    main->ReadSampleList("../SampleList");

    // run over previous output
    if ( OverrideInput ) {
        // const char* input_version = Form("v15_v25lj_default_%s", MC_v_str[mc_v]);
        const char* input_version = Form("v16_v29ljstar_QCDSF_noMETMtWcuts_%s", MC_v_str[mc_v]);
        const char* input_ext = "sgtop_schan_ttbar4j";
        // const char* input_anadir = Form("/rdsk/dats5/atlas/%s/13TeV_s_channel/%s/%s",
        const char* input_anadir = Form("/rdsk/datm8/atlas/%s/13TeV_s_channel/%s/%s",
                                        user, input_version, input_ext);
        const char* override_inputdir = Form("%s/%s", input_anadir, main->GetName());
        main->SetOverrideInputDir(override_inputdir);
        main->SetTreeName("physics");
        main->SetCut("fEvtWriterCloneOnly", "kFALSE");
        main->SetOverrideBatchNodeAll(kTRUE);
    }

    // MemTk config
    // main->SetMemTkRunScript("mem_sChannel_master.C");
    main->SetMemTkRunScript("mem_sChannel_ttbarVR.C");
    // main->SetMemTkRunScript("/mnt/eeh/kaphle/atlas/APlusPlus/libSingleTop/tasks/13TeV/s_channel/mem_sChannel_master.C");
    main->SetUseSubJobsAsInput(kTRUE);
    main->SetNSubJobsData(10); // 1
    main->SetNSubJobsQCD(10); // 1 // started at 1 with v07, so take care of change!
    main->SetNSubJobsSgTopSChannel(5); // 30
    main->SetNSubJobsSgTopTChannel(10); // 5
    main->SetNSubJobsSgTopWt(5); // 1
    main->SetNSubJobsTtbar(100); // 3: 37h-> 100: ~1h
    main->SetNSubJobsWjetsB(10); // 3
    // main->SetNSubJobsWjetsB(10); // 3
    // main->SetNSubJobsWjetsC(1);
    main->SetNSubJobsZjetsB(10);

    // Cut analysis config:
    //main->SetNProcessNthEventsOnly(20);

    main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpNominal);
     
    // Object Selection Tool -> main
    // Reco SF Tool -> main

    // Analysis selector + cuts
    if ( mode == kCut ) {
        if ( UseDefaultCuts ) {
            main->SetCut("fPt_Jet_min", "25.");
            main->SetCut("fPt_LeadingJet_min", "25.");
            main->SetCut("fApplyDileptonVeto", "kFALSE");
            main->SetCut("fMET_min", "35.");
            main->SetCut("fMtW_minPre", "30.");
            main->SetCut("fMtW_min", "30.");
            main->SetCut("fAdditionalJetVeto", "kTRUE"); // for MEM
            main->SetCut("fNBTags_min", "2");
            main->SetCut("fNBTags_max", "2");
            main->SetCut("fLeadingJetIsBTagged", "kFALSE");
            main->SetCut("fMCTruthStudy", "kFALSE");
            // main->SetCut("fApplyTtbar4JetVeto", "kTRUE");

            // // v14 new default: DON'T use the AdditionalJetVeto, use BDT vars
            // main->SetCut("fAdditionalJetVeto", "kFALSE"); // for MEM
            // main->SetCut("fDoReconstructTop", "kTRUE");

            // v16 new default: still don't use add jet veto; also do BDT later
            main->SetCut("fAdditionalJetVeto", "kFALSE"); // for MEM
            main->SetCut("fDoReconstructTop", "kFALSE");
        } else {
            // // cuts similar to Wuppertal group:
            // main->SetToolParameter("D3PDObjects", "fElectronPt_min", "28.");
            // main->SetToolParameter("D3PDObjects", "fMuonPt_min", "28.");
            // main->SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kMV2c10_85");
            // main->SetToolParameter("SFTool", "fBTagWP",
            //                        "AtlObjRecoScaleFactorTool::kMV2c10_85");
            // main->SetCut("fPt_Jet_min", "25.");
            // main->SetCut("fPt_LeadingJet_min", "25.");
            // main->SetCut("fNBTags_min", "2");
            // main->SetCut("fNBTags_max", "2");
            // main->SetCut("fMET_min", "50.");
            // // turned off:
            // main->SetCut("fApplyDileptonVeto", "kFALSE");
            // // main->SetCut("fMtW_minPre", "30.");
            // // main->SetCut("fMtW_min", "30.");
            // main->SetCut("fAdditionalJetVeto", "kFALSE"); // for MEM
            // main->SetCut("fLeadingJetIsBTagged", "kFALSE");
            // main->SetCut("fMCTruthStudy", "kFALSE");
            // // main->SetCut("fApplyTtbar4JetVeto", "kTRUE");

            // // cuts without MET cuts for antimuon method
            // main->SetCut("fPt_Jet_min", "25.");
            // main->SetCut("fPt_LeadingJet_min", "25.");
            // main->SetCut("fApplyDileptonVeto", "kFALSE");
            // // main->SetCut("fMET_min", "35.");
            // // main->SetCut("fMtW_minPre", "30.");
            // // main->SetCut("fMtW_min", "30.");
            // main->SetCut("fAdditionalJetVeto", "kTRUE"); // for MEM
            // main->SetCut("fNBTags_min", "2");
            // main->SetCut("fNBTags_max", "2");
            // main->SetCut("fLeadingJetIsBTagged", "kFALSE");
            // main->SetCut("fMCTruthStudy", "kFALSE");

            // // cuts to check yields/cutflows:
            // // default:            
            // main->SetCut("fPt_Jet_min", "25.");
            // main->SetCut("fPt_LeadingJet_min", "25.");
            // main->SetCut("fApplyDileptonVeto", "kFALSE");
            // main->SetCut("fMET_min", "35.");
            // main->SetCut("fMtW_minPre", "30.");
            // main->SetCut("fMtW_min", "30.");
            // main->SetCut("fAdditionalJetVeto", "kTRUE"); // for MEM
            // main->SetCut("fNBTags_min", "2");
            // main->SetCut("fNBTags_max", "2");
            // main->SetCut("fLeadingJetIsBTagged", "kFALSE");
            // main->SetCut("fMCTruthStudy", "kFALSE");
            // // 0. remove add jet veto cut
            // main->SetCut("fAdditionalJetVeto", "kFALSE"); // for MEM

            // v14/v15 default cuts
            main->SetCut("fPt_Jet_min", "25.");
            main->SetCut("fPt_LeadingJet_min", "25.");
            main->SetCut("fApplyDileptonVeto", "kFALSE");
            main->SetCut("fMET_min", "35.");
            main->SetCut("fMtW_minPre", "30.");
            main->SetCut("fMtW_min", "30.");
            main->SetCut("fAdditionalJetVeto", "kFALSE"); // for MEM
            main->SetCut("fNBTags_min", "2");
            main->SetCut("fNBTags_max", "2");
            main->SetCut("fLeadingJetIsBTagged", "kFALSE");
            main->SetCut("fMCTruthStudy", "kFALSE");

            // // v15 QCD scale factors; no MET or MtW cut
            // main->SetCut("fMET_min", "0.");
            // main->SetCut("fMtW_min", "0.");
            // main->SetCut("fMtW_minPre", "0.");
            // main->SetCut("fDoReconstructTop", "kFALSE");
            
            // // v15 AdditionalJetVeto, run over previous output
            // main->SetCut("fAdditionalJetVeto", "kTRUE"); // for MEM

            // v16 default - no reconstructtop
            main->SetCut("fDoReconstructTop", "kFALSE");
            // QCDJetLepton: no btag requirement
            main->SetCut("fNBTags_min", "0");
            
            // // QCD SF: use noMETMtWcuts
            // main->SetCut("fMET_min", "0.");
            // main->SetCut("fMtW_min", "0.");
            // main->SetCut("fMtW_minPre", "0.");
            // // // QCD SF noMETcut
            // // main->SetCut("fMtW_min", "30.");
            // // QCD SF noMtWcut, rerun over previous
            // main->SetCut("fMET_min", "35.");            
        }
    }


    // QCD MM Tool
    AtlQCDMMTool *tool_qcd = new AtlQCDMMTool("QCDMMTool", "QCD Matrix Method");
    main->AddTool(tool_qcd);
    // main->SetToolParameter("QCDMMTool", "fEfficiencyPath2015", "$APP_HOME/external/TopFakes/data/20p7/2015");
    // main->SetToolParameter("QCDMMTool", "fEfficiencyPath2016", "$APP_HOME/external/TopFakes/data/20p7/2016");
    // main->SetToolParameter("QCDMMTool", "fParamString_fake2015", "2015Test:NS:jetpt:eta:dR");
    // main->SetToolParameter("QCDMMTool", "fParamString_real2015", "2015Test:NS:jetpt:eta:dR");
    // main->SetToolParameter("QCDMMTool", "fParamString_fake2016", "2016Test:NS:jetpt:eta:dR");
    // main->SetToolParameter("QCDMMTool", "fParamString_real2016", "2016Test:NS:jetpt:eta:dR");

    // including 2017:
    // main->SetToolParameter("QCDMMTool", "fEfficiencyPath", "$APP_HOME/Efficiencies_rel21/21p2");

    // // previous default:
    // main->SetToolParameter("QCDMMTool", "fParamString_fake", "Test:NS:jetpt:eta:dR");
    // main->SetToolParameter("QCDMMTool", "fParamString_real", "Test:NS:jetpt:eta:dR");
    // // param0:
    // main->SetToolParameter("QCDMMTool", "fParamString_fake", "Test:NS:eta:jetpt:dR");
    // main->SetToolParameter("QCDMMTool", "fParamString_real", "Test:NS:eta:pt:dR");
    // // param1:
    // main->SetToolParameter("QCDMMTool", "fParamString_fake", "Test:NS:eta:pt:dR");
    // main->SetToolParameter("QCDMMTool", "fParamString_real", "Test:NS:eta:pt:dPhi");
    // // param2:
    // main->SetToolParameter("QCDMMTool", "fParamString_fake", "Test:NS:pt:dR");
    // main->SetToolParameter("QCDMMTool", "fParamString_real", "Test:NS:pt:nbtag");
    // // param3:
    // main->SetToolParameter("QCDMMTool", "fParamString_fake", "Test:NS:eta:dR:dPhi");
    // main->SetToolParameter("QCDMMTool", "fParamString_real", "Test:NS:eta:dR:dPhi");
    // // param4:
    // main->SetToolParameter("QCDMMTool", "fParamString_fake", "Test:NS:eta:jetpt:met");
    // main->SetToolParameter("QCDMMTool", "fParamString_real", "Test:NS:eta:jetpt:met");
    
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


    // Efficiencies by JM, 2018-07-02
    main->SetToolParameter("QCDMMTool", "fEfficiencyPath2015", "$APP_HOME/Rel21_efficiencies_20180702");
    main->SetToolParameter("QCDMMTool", "fEfficiencyPath2016", "$APP_HOME/Rel21_efficiencies_20180702");
    main->SetToolParameter("QCDMMTool", "fEfficiencyPath2017", "$APP_HOME/Rel21_efficiencies_20180702");
    main->SetToolParameter("QCDMMTool", "fParamString_fake2015", Form("2015:%s", param_string.Data()));
    main->SetToolParameter("QCDMMTool", "fParamString_real2015", Form("2015:%s", param_string.Data()));
    main->SetToolParameter("QCDMMTool", "fParamString_fake2016", Form("2016:%s", param_string.Data()));
    main->SetToolParameter("QCDMMTool", "fParamString_real2016", Form("2016:%s", param_string.Data()));
    main->SetToolParameter("QCDMMTool", "fParamString_fake2017", Form("2017:%s", param_string.Data()));
    main->SetToolParameter("QCDMMTool", "fParamString_real2017", Form("2017:%s", param_string.Data()));

    // // Efficiencies by NB, singletop from 2018-07-17
    // main->SetToolParameter("QCDMMTool", "fEfficiencyPath2015", "$APP_HOME/Rel21_efficiencies_singletop");
    // main->SetToolParameter("QCDMMTool", "fEfficiencyPath2016", "$APP_HOME/Rel21_efficiencies_singletop");
    // main->SetToolParameter("QCDMMTool", "fEfficiencyPath2017", "$APP_HOME/Rel21_efficiencies_singletop");
    // main->SetToolParameter("QCDMMTool", "fParamString_fake2015", Form("2015:NS:%s", param_string.Data()));
    // main->SetToolParameter("QCDMMTool", "fParamString_real2015", Form("2015_Real:NS:%s", param_string.Data()));
    // main->SetToolParameter("QCDMMTool", "fParamString_fake2016", Form("2016:NS:%s", param_string.Data()));
    // main->SetToolParameter("QCDMMTool", "fParamString_real2016", Form("2016_Real:NS:%s", param_string.Data()));
    // main->SetToolParameter("QCDMMTool", "fParamString_fake2017", Form("2017:NS:%s", param_string.Data()));
    // main->SetToolParameter("QCDMMTool", "fParamString_real2017", Form("2017_Real:NS:%s", param_string.Data()));

    // ===============
    // Build task tree
    // ===============
    main->BuildTree();
}

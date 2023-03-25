{
    //____________________________________________________________________
    //
    // SgTop s-channel analysis for the w+jets validation region
    //
    // Author: Oliver Maria Kind <mailto: kind@physik.hu-berlin.de>
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
    Bool_t UseOutputTree = kFALSE;  // don't write output tree if not needed for MEM or reruns
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
    EMCVersion mc_v = kMC16a;
    // param order in MMEffSet: eta_dR/eta - pt_dR/pt_jetpt/pt - jetpt_dR/jetpt - sumet - dR/dRpt - dPhi - d0sig - nbjets
    TString param_string = "NS:pt";

    // switch to v25:
    // const char* version = Form("v15_v25lj_test_%s", MC_v_str[mc_v]); // no QCD yet
    // const char* version = Form("v15_v25lj_test_JetLeptonNoBTag_%s", MC_v_str[mc_v]); // no QCD yet
    // const char* version = Form("v15_v25lj_test_leptonSF_tight_%s", MC_v_str[mc_v]);
//    const char* version = Form("v15_v25lj_default_%s", MC_v_str[mc_v]);
//    const char* version = Form("v15_v25lj_QCD_scale_factors_%s", MC_v_str[mc_v]);
//    const char* version = Form("v15_v25lj_WithAddJetVeto_%s", MC_v_str[mc_v]);

    // const char* version = Form("v11_v21lj_QCDtests_NBsgtop_newDL_%s_%s",
    // const char* version = Form("v11_v24lj_QCDtests_JM0702_%s_%s",
    //                            MC_v_str[mc_v], param_string.Data());
    // const char* version = Form("v11_v21lj_QCDtests_NB0615_%s_%s",
    //                            MC_v_str[mc_v], param_string.Data());
    // const char* version = Form("v13_v23lj_noQCD_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_QCDtests_JM0702_%s_%s",
    //                            MC_v_str[mc_v], param_string.Data());
    // const char* version = Form("v13_v23lj_AntiMuon_SF_derivation_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_QCDtests_AntiMuon_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_QCDtests_JetLepton_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_QCDtests_noBTag_JetLepton_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_QCDtests_JetLeptonNoBTag_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_yields_default_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_yields_noAddJetVeto_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_wjetsNoMET_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_wjetsNoMETNoMtW_%s", MC_v_str[mc_v]);
    // const char* version = Form("v14_v23lj_default_%s", MC_v_str[mc_v]); // -addjetveto, +syst

    // switch to v29:
    // v29 partial implementation: Wjets already v29start, Zjets still v29
    // const char* version = Form("v16_v29lj_Wjv29star_Zjv29_%s", MC_v_str[mc_v]);
    // v29* full implementation: Wjets, Zjets, Diboson
    // const char* version = Form("v16_v29lj_default_v29star_%s", MC_v_str[mc_v]);
    // const char* version = Form("v16_v29ljstar_QCDSF_noMETMtWcuts_%s", MC_v_str[mc_v]);
    // const char* version = Form("v16_v29ljstar_QCDSF_noMETcut_%s", MC_v_str[mc_v]);
    const char* version = Form("v16_v29ljstar_QCDSF_noMtWcut_%s", MC_v_str[mc_v]);

    const char* ext = "sgtop_schan_wjets";
//    const char* ext = "sgtop_schan_wjets_noMETMtWcuts";
    const char* memllhdir = "MemDisc";
//    const char* anadir = Form("/rdsk/dats5/atlas/%s/13TeV_s_channel/%s/%s",
    const char* anadir = Form("/rdsk/datm8/atlas/%s/13TeV_s_channel/%s/%s",
                              user, version, ext);
    const char* histdir = anadir;
    const char* jobdir = anadir;
    const char* tempdir = Form("/datc/atlas/%s/%s/%s", user, ext, version);
   
    // to compare MC16a/d: set lumi in fb^-1
    Float_t force_lumi = 0.;
    TString force_plotdir_append = "";//"/plot_1fb";

    // options for kMemtk
    Bool_t GridJob = kFALSE;
    const char* GridIdSuffix = "_WjetsVR";
    const char* GridSuffix = "v1";

    // Chose jet bin
    AtlSelector::EJetMult jetbin = AtlSelector::kTwoJet;

    AtlTopLevelAnalysis *main
	= new AtlTopLevelAnalysis("AtlSgTop_sChannelAnalysis",
				  "SgTop sChannel Analysis");

    // execute main (general) macro
    gROOT->Macro("task_sChannel_main.C");

    // Select DSIDList (to exclude/include DATA/QCD) - only for overriding
    main->ReadSampleList("../SampleList");

    // run over previous output
    if ( OverrideInput ) {
        // const char* input_version = Form("v15_v25lj_default_%s", MC_v_str[mc_v]);
        const char* input_version = Form("v16_v29ljstar_QCDSF_noMETMtWcuts_%s", MC_v_str[mc_v]);
        const char* input_ext = "sgtop_schan_wjets";
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
    main->SetMemTkRunScript("mem_sChannel_master.C");
    // main->SetMemTkRunScript("/mnt/eeh/kaphle/atlas/APlusPlus/libSingleTop/tasks/13TeV/s_channel/mem_sChannel_master.C");
    // main->SetMemTkRunScript("/mnt/eeh/kaphle/atlas/APlusPlus/libSingleTop/tasks/13TeV/s_channel/mem_sChannel_ttbarDL.C");
    main->SetUseSubJobsAsInput(kTRUE);
    main->SetNSubJobsData(10); // 1
    main->SetNSubJobsQCD(10); // 1
    main->SetNSubJobsSgTopSChannel(5); // 30
    main->SetNSubJobsSgTopTChannel(10); // 5
    main->SetNSubJobsSgTopWt(5); // 1
    main->SetNSubJobsTtbar(100); // 3: 37h-> 100: ~1h
    main->SetNSubJobsWjetsB(10); // 3
    // main->SetNSubJobsWjetsB(10); // 3
    // main->SetNSubJobsWjetsC(1);
    main->SetNSubJobsZjetsB(1);

    main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpNominal);

    // main->SetDebug(1);
    // main->SetDebugBuild(1);

     // Object Selection Tool
    main->SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kMV2c10_85");

    // Reco SF Tool
    main->SetToolParameter("SFTool", "fBTagWP",
			   "AtlObjRecoScaleFactorTool::kMV2c10_85");

    // Analysis selector + cuts
    if ( mode == kCut ) {
        if ( UseDefaultCuts ) {
            // default cuts from 8 TeV analysis
            main->SetCut("fPt_Jet_min", "30.");
            main->SetCut("fPt_LeadingJet_min", "40.");
            main->SetCut("fApplyDileptonVeto", "kTRUE");
            main->SetCut("fMET_min", "35.");
            main->SetCut("fMtW_minPre", "30.");
            main->SetCut("fMtW_min", "30.");
            main->SetCut("fAdditionalJetVeto", "kTRUE"); // for MEM
            main->SetCut("fNBTags_min", "2");
            main->SetCut("fNBTags_max", "2");
            main->SetCut("fLeadingJetIsBTagged", "kFALSE");
            main->SetCut("fMCTruthStudy", "kFALSE");
            main->SetCut("fVetoBTagger", "AtlBTag::kMV2c10_77");
            main->SetCut("fVetoBTagWeight_min", "0.5");

            // // v14 new default: DON'T use the AdditionalJetVeto, use BDT vars
            // main->SetCut("fAdditionalJetVeto", "kFALSE"); // for MEM
            // main->SetCut("fDoReconstructTop", "kTRUE");

            // v16 new default: still don't use add jet veto; also do BDT later
            main->SetCut("fAdditionalJetVeto", "kFALSE"); // for MEM
            main->SetCut("fDoReconstructTop", "kFALSE");
        } else {
            // // cuts without MET and MtW for AntiMuon
            // main->SetCut("fPt_Jet_min", "30.");
            // main->SetCut("fPt_LeadingJet_min", "40.");
            // main->SetCut("fApplyDileptonVeto", "kTRUE");
            // // main->SetCut("fMET_min", "35.");
            // // main->SetCut("fMtW_minPre", "30.");
            // // main->SetCut("fMtW_min", "30.");
            // main->SetCut("fAdditionalJetVeto", "kTRUE"); // for MEM
            // main->SetCut("fNBTags_min", "2");
            // main->SetCut("fNBTags_max", "2");
            // main->SetCut("fLeadingJetIsBTagged", "kFALSE");
            // main->SetCut("fMCTruthStudy", "kFALSE");
            // main->SetCut("fVetoBTagger", "AtlBTag::kMV2c10_77");
            // main->SetCut("fVetoBTagWeight_min", "0.5");

            // // cuts without b-tag requirement for JetLepton check
            // main->SetCut("fPt_Jet_min", "30.");
            // main->SetCut("fPt_LeadingJet_min", "40.");
            // main->SetCut("fApplyDileptonVeto", "kTRUE");
            // main->SetCut("fMET_min", "35.");
            // main->SetCut("fMtW_minPre", "30.");
            // main->SetCut("fMtW_min", "30.");
            // main->SetCut("fAdditionalJetVeto", "kTRUE"); // for MEM
            // // main->SetCut("fNBTags_min", "2");
            // // main->SetCut("fNBTags_max", "2");
            // main->SetCut("fLeadingJetIsBTagged", "kFALSE");
            // main->SetCut("fMCTruthStudy", "kFALSE");
            // main->SetCut("fVetoBTagger", "AtlBTag::kMV2c10_77");
            // main->SetCut("fVetoBTagWeight_min", "0.5");

            // // cuts to check yields/cutflows:
            // // default:
            // main->SetCut("fPt_Jet_min", "30.");
            // main->SetCut("fPt_LeadingJet_min", "40.");
            // main->SetCut("fApplyDileptonVeto", "kTRUE");
            // main->SetCut("fMET_min", "35.");
            // main->SetCut("fMtW_minPre", "30.");
            // main->SetCut("fMtW_min", "30.");
            // main->SetCut("fAdditionalJetVeto", "kTRUE"); // for MEM
            // main->SetCut("fNBTags_min", "2");
            // main->SetCut("fNBTags_max", "2");
            // main->SetCut("fLeadingJetIsBTagged", "kFALSE");
            // main->SetCut("fMCTruthStudy", "kFALSE");
            // main->SetCut("fVetoBTagger", "AtlBTag::kMV2c10_77");
            // main->SetCut("fVetoBTagWeight_min", "0.5");
            // // 1. remove add jet veto cut
            // main->SetCut("fAdditionalJetVeto", "kFALSE"); // for MEM

            // // cuts as new default, without MET cut
            // // update: also without mtw cut
            // main->SetCut("fPt_Jet_min", "30.");
            // main->SetCut("fPt_LeadingJet_min", "40.");
            // main->SetCut("fApplyDileptonVeto", "kTRUE");
            // // main->SetCut("fMET_min", "35.");
            // // main->SetCut("fMtW_minPre", "30.");
            // // main->SetCut("fMtW_min", "30.");
            // main->SetCut("fAdditionalJetVeto", "kFALSE"); // for MEM
            // main->SetCut("fNBTags_min", "2");
            // main->SetCut("fNBTags_max", "2");
            // main->SetCut("fLeadingJetIsBTagged", "kFALSE");
            // main->SetCut("fMCTruthStudy", "kFALSE");
            // main->SetCut("fVetoBTagger", "AtlBTag::kMV2c10_77");
            // main->SetCut("fVetoBTagWeight_min", "0.5");

            // default cuts after add jet veto removal (v14)
            main->SetCut("fPt_Jet_min", "30.");
            main->SetCut("fPt_LeadingJet_min", "40.");
            main->SetCut("fApplyDileptonVeto", "kTRUE");
            main->SetCut("fMET_min", "35.");
            main->SetCut("fMtW_minPre", "30.");
            main->SetCut("fMtW_min", "30.");
            main->SetCut("fAdditionalJetVeto", "kFALSE"); // for MEM
            main->SetCut("fNBTags_min", "2");
            main->SetCut("fNBTags_max", "2");
            main->SetCut("fLeadingJetIsBTagged", "kFALSE");
            main->SetCut("fMCTruthStudy", "kFALSE");
            main->SetCut("fVetoBTagger", "AtlBTag::kMV2c10_77");
            main->SetCut("fVetoBTagWeight_min", "0.5");
            main->SetCut("fDoReconstructTop", "kTRUE");

            // // // v15 for JetLepton: like default but without btag req and without BDT vars
            // // main->SetCut("fNBTags_min", "0");
            // // main->SetCut("fDoReconstructTop", "kFALSE");

            // // v15 QCD scale factors; no MET or MtW cut
            // main->SetCut("fMET_min", "0.");
            // main->SetCut("fMtW_min", "0.");
            // main->SetCut("fMtW_minPre", "0.");
            // main->SetCut("fDoReconstructTop", "kFALSE");
            
            // // test tagging all jets for jetlepton
            // main->SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kTagAll");

            // // v15 AdditionalJetVeto, run over previous output
            // main->SetCut("fAdditionalJetVeto", "kTRUE"); // for MEM
            // main->SetCut("fDoReconstructTop", "kFALSE");
            // // need to force btagged jets
            // main->SetCut("fNBTags_min", "2");
            // main->SetCut("fNBTags_max", "2");

            // // v15 rerun default with clone and therefore without reconstructtop
            // main->SetCut("fDoReconstructTop", "kFALSE");

            // v16 default - no reconstructtop
            main->SetCut("fDoReconstructTop", "kFALSE");
            // QCDJetLepton: no btag requirement
            main->SetCut("fNBTags_min", "0");

            // QCD SF: use noMETMtWcuts
            main->SetCut("fMET_min", "0.");
            main->SetCut("fMtW_min", "0.");
            main->SetCut("fMtW_minPre", "0.");            
            // // QCD SF noMETcut, rerun over previous
            // main->SetCut("fMtW_min", "30.");
            // QCD SF noMtWcut, rerun over previous
            main->SetCut("fMET_min", "35.");            
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

    // including 2017 with prelim. eff. by JM
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
    // main->SetToolParameter("QCDMMTool", "fEfficiencyPath2015", "$APP_HOME/Rel21_efficiencies_singletop_newDL");
    // main->SetToolParameter("QCDMMTool", "fEfficiencyPath2016", "$APP_HOME/Rel21_efficiencies_singletop_newDL");
    // main->SetToolParameter("QCDMMTool", "fEfficiencyPath2017", "$APP_HOME/Rel21_efficiencies_singletop_newDL");
    // // main->SetToolParameter("QCDMMTool", "fEfficiencyPath2015", "$APP_HOME/Rel21_efficiencies_singletop");
    // // main->SetToolParameter("QCDMMTool", "fEfficiencyPath2016", "$APP_HOME/Rel21_efficiencies_singletop");
    // // main->SetToolParameter("QCDMMTool", "fEfficiencyPath2017", "$APP_HOME/Rel21_efficiencies_singletop");
    // main->SetToolParameter("QCDMMTool", "fParamString_fake2015", Form("2015:%s", param_string.Data()));
    // main->SetToolParameter("QCDMMTool", "fParamString_real2015", Form("2015_Real:%s", param_string.Data()));
    // main->SetToolParameter("QCDMMTool", "fParamString_fake2016", Form("2016:%s", param_string.Data()));
    // main->SetToolParameter("QCDMMTool", "fParamString_real2016", Form("2016_Real:%s", param_string.Data()));
    // main->SetToolParameter("QCDMMTool", "fParamString_fake2017", Form("2017:%s", param_string.Data()));
    // main->SetToolParameter("QCDMMTool", "fParamString_real2017", Form("2017_Real:%s", param_string.Data()));
 
    // ===============
    // Build task tree
    // ===============
    main->BuildTree();
}

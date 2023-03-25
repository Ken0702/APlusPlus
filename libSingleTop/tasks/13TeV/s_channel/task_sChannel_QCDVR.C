{
    //____________________________________________________________________
    //
    // SgTop s-channel analysis for a QCD validation region [invert MET cut]
    //
    // Author: Oliver Maria Kind <mailto: kind@physik.hu-berlin.de>
    // Update: $Id: task_analysis_signal.C,v 1.2 2016/04/19 08:43:44 stamm Exp $
    // Copyright: 2014 (C) Oliver Maria Kind
    //   

    enum ESgTopVersion { k15lj, k20lj, k21lj, k23lj, k24lj};
    static const char* ST_v_str[5] = { "v15_lj", "v20_lj", "v21_lj", "v23_lj", "v24_lj" };
    enum EMCVersion { kMC15, kMC16, kMC16a, kMC16d };
    static const char* MC_v_str[4] = { "MC15", "MC16", "MC16a", "MC16d" };

    enum EAnalysisMode {
        kCut,          // 1. do cuts to define regions
        kMemTk,        // 2. MEM LLH calculation on output from 1.
        kMemDisc       // 3. discr./llh histograms from 2. 
    };
    
    // select analysis modes for this file
    EAnalysisMode mode = kCut;
//    EAnalysisMode mode = kMemTk;
//    EAnalysisMode mode = kMemDisc;
    Bool_t is_ctrlplot = kTRUE;       // control plots for 1. or 3.

    // cut options
    Bool_t useDefaultCuts = kTRUE;
    Bool_t UseOutputTree = kFALSE; // don't write output tree if not needed for MEM
    Bool_t use_Max_cuts = kFALSE;
 
    // plotting options
    Bool_t UsePlotterSchemeDir = kTRUE;
    const char* plotter_scheme = "SgTop_s";

    // ========================
    // Configure top-level task
    // ========================

    // AtlTopLevelAnalysis options
    Bool_t ForceRetry        = kFALSE;
    Bool_t BuildHforTree     = kFALSE;
    Bool_t BuildMergingTree  = kFALSE;
    Bool_t BuildAnalysisTree = ( mode == kCut ) && !is_ctrlplot;
    Bool_t BuildMemTkAnalysisTree = ( mode == kMemTk );
    Bool_t BuildMemDiscAnalysisTree = ( mode == kMemDisc ) && !is_ctrlplot;
    Bool_t BuildPlottingTree = is_ctrlplot && (mode != kMemTk);

    // Setup (local) directories
    const char* user = gSystem->Getenv("USER");
    ESgTopVersion sgtop_v = k23lj;
    EMCVersion mc_v = kMC16a;
    // param order in MMEffSet: eta_dR/eta - pt_dR/pt_jetpt/pt - jetpt_dR/jetpt - sumet - dR/dRpt - dPhi - d0sig - nbjets
    TString param_string = "NS:pt";
    // TString param_string = "ge4j2b:pt";
    // TString param_string = "ge4j2b:dR";
    // TString param_string = "ge4j2b:jetpt";
    // TString param_string = "ge4j2b:dPhi";
    // before running without NS:
    // TString param_string = "eta";
    // TString param_string = "pt";
    // TString param_string = "dR";
//    TString param_string = "eta:pt";
    // TString param_string = "eta:dR";
    // TString param_string = "pt:dR";
    // TString param_string = "eta:pt:dR";
    // TString param_string = "dPhi";
    // old (NB0615):
    // TString param_string = "dPhi";
    // TString param_string = "nbtag"; // should not work;
    // TString param_string = "mtw";  // not available!
    // TString param_string = "pt:eta"; // wrong order
    // TString param_string = "pt:dPhi"; // 
    // TString param_string = "eta:dPhi";
    // TString param_string = "dPhi:dR"; // wrong order
    // // TString param_string = "eta:jetpt:met";
    // TString param_string = "eta:jetpt";
    // TString param_string = "jetpt:eta";
    // const char* version = Form("v11_v21lj_QCDtests_JM0702_%s_%s",
//    const char* version = Form("v11_v21lj_QCDtests_NBsgtop_newDL_%s_%s",
                               // const char* ver = "v11_QCDtests_JM0702";
    // const char* version = Form("%s_%s_%s_%s", ver, ST_v_str[sgtop_v],
    // const char* version = Form("v11_v24lj_QCDtests_JM0702_%s_%s",
    //                            MC_v_str[mc_v], param_string.Data());
    // const char* version = "v00_test_v24lj_a";
    // const char* version = Form("v13_v23lj_noQCD_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_AntiMuon_SF_derivation_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_QCDtests_AntiMuon_%s", MC_v_str[mc_v]);
    const char* version = Form("v13_v23lj_QCDtests_JetLepton_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_QCDnoMtWcut_noQCD_%s", MC_v_str[mc_v]);
    // const char* version = Form("v13_v23lj_QCDtests_JM0702_%s_%s",
    //                            MC_v_str[mc_v], param_string.Data());
    // const char* version = Form("v13_v23lj_AntiMuon_SF_derivation_%s", MC_v_str[mc_v]);
    const char* ext = "sgtop_schan_QCD";
    const char* memllhdir = "MemDisc";
    const char* anadir = Form("/rdsk/dats5/atlas/%s/13TeV_s_channel/%s/%s",
                              user, version, ext);
    // const char* anadir = Form("/rdsk/datm5/atlas/%s/13TeV_s_channel/%s/%s",
    //                           user, version, ext);
    const char* histdir = anadir;
    const char* jobdir = anadir;
    const char* tempdir = Form("/datc/atlas/%s/%s/%s", user, ext, version);

    // to compare MC16a/d: set lumi in fb^-1
    Float_t force_lumi = 0.;//36184.86;
    TString force_plotdir_append = "";//"/plot_36184fb";

    // options for kMemtk
    Bool_t GridJob = kTRUE;
    const char* GridIdSuffix = "_QCD";
    const char* GridSuffix = "v0";

    // Chose jet bin
    AtlSelector::EJetMult jetbin = AtlSelector::kTwoJet;

    AtlTopLevelAnalysis *main
	= new AtlTopLevelAnalysis("AtlSgTop_sChannelAnalysis",
				  "SgTop sChannel Analysis");

    // execute main (general) macro
    gROOT->Macro("task_sChannel_main.C");

    // main->SetDebug(2);

    // Select DSIDList (to exclude/include DATA/QCD) - only for overriding
    main->ReadSampleList("../SampleList");

    // MemTk config
    main->SetMemTkRunScript("mem_sChannel_master.C");
    main->SetUseSubJobsAsInput(kTRUE);
    // main->SetNSubJobsData(1);
    main->SetNSubJobsQCD(10); // 1
    main->SetNSubJobsSgTopSChannel(10); // 30
    main->SetNSubJobsSgTopTChannel(10); // 5
    main->SetNSubJobsSgTopWt(50); // 1
    main->SetNSubJobsTtbar(100); // 3: 37h-> 100: ~1h
    main->SetNSubJobsWjetsB(10); // 3
    // main->SetNSubJobsWjetsC(1);
    main->SetNSubJobsZjetsB(10);

    main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpNominal);

    // Analysis selector + cuts
    if ( mode == kCut ) {
        if ( useDefaultCuts ) {
            main->SetCut("fPt_Jet_min", "30.");
            main->SetCut("fPt_LeadingJet_min", "40.");
            main->SetCut("fApplyDileptonVeto", "kTRUE");
            // main->SetCut("fMET_min", "35.");
            main->SetCut("fMET_max", "35.");
            main->SetCut("fMtW_minPre", "30.");
            main->SetCut("fMtW_min", "30.");
            main->SetCut("fAdditionalJetVeto", "kTRUE"); // for MEM
            main->SetCut("fNBTags_min", "2");
            main->SetCut("fNBTags_max", "2");
            main->SetCut("fLeadingJetIsBTagged", "kFALSE");
            main->SetCut("fMCTruthStudy", "kFALSE");
        } else {
            // drop mtW requirement because somehow this is contradicting the MET cut;
            // also agreement might improve?
          //  also drop met for antimuon region
            main->SetCut("fPt_Jet_min", "30.");
            main->SetCut("fPt_LeadingJet_min", "40.");
            main->SetCut("fApplyDileptonVeto", "kTRUE");
            // main->SetCut("fMET_min", "35.");
//            main->SetCut("fMET_max", "35.");
            // main->SetCut("fMtW_minPre", "30.");
            // main->SetCut("fMtW_min", "30.");
            main->SetCut("fAdditionalJetVeto", "kTRUE"); // for MEM
            main->SetCut("fNBTags_min", "2");
            main->SetCut("fNBTags_max", "2");
            main->SetCut("fLeadingJetIsBTagged", "kFALSE");
            main->SetCut("fMCTruthStudy", "kFALSE");
        }
    } else if ( mode == kMemDisc && use_Max_cuts ) {
        // yield without cuts: sch 1889, ttbar 56668, tot 84755
        // yield with cuts:    sch 1077, ttbar 18860, tot 28683
        // yield with cuts w/o jetpt min: sch 1441,ttbar 23991, total 39964
        // w/o jetpt min, w/o met: sch 1615, ttbar 28664, total 46623
        // from ~gottwalm/Desktop/Output/cuts13
        main->SetCut("fPt_Jet_min", "40."); // is in cut but turn off to see s-ch yield
        main->SetCut("fApplyDileptonVeto", "kTRUE");
        main->SetCut("fMET_max", "90."); // is cut but turn off to test
        main->SetCut("fMtW_maxPre","100.");
        main->SetCut("fMtW_max","100.");
    }

    // Object Selection Tool
    main->SetToolParameter("D3PDObjects", "fJetPt_min", "25.");
    main->SetToolParameter("D3PDObjects", "fJetEta_min", "0.");
    main->SetToolParameter("D3PDObjects", "fJetEta_max", "2.5");
    main->SetToolParameter("D3PDObjects", "fFwdJetPt_min", "25.");
    main->SetToolParameter("D3PDObjects", "fFwdJetEta_min", "0.");
    main->SetToolParameter("D3PDObjects", "fFwdJetEta_max", "2.5");
    main->SetToolParameter("D3PDObjects", "fElectronPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fMuonPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kMV2c10_77");

    // Reco SF Tool
    main->SetToolParameter("SFTool", "fBTagWP",
			   "AtlObjRecoScaleFactorTool::kMV2c10_77");

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

// fixed bugs/issues:
// - Cut: QCD analysis does not copy physics tree - fixed
// - MemTk: ttbar DL llh are not calculated because events are invalid. - fixed, rerun MEM
//     see MemProcBase::IsValidEvent and compare MemTk version to APlusPlusIntNote_v15
// - MemTk: fix ttbar DL llh calculation (1/2me factor?) - fixed
// - MemDisc: s-ch. 2j llh are not recognized - fixed
// - MemDisc: empty samples fail at assertion in AtlScaleFactor::SetValue - fixed
// - MemTk: some/all procs seem to not reuse any permtables -> fixed in issametopo
// - MemTk/A++: scripts have to run on grid -> fixed
//   - files for GRID_HOME in ~/atlas/grid_home
//   - local test run in ~/atlas/grid_home/grid_test_setup.sh
//- alphas vergleichen (wie sehen plots mit alten aus) -> done
//  - analysis macro not found for local jobs!? -> fixed, if PBS_ID in .zshenv
// - Plots without data have yields < 1 -> fixed, needed to change AtlDataMCPlotterTask.cxx
// - MemDisc: use new rebinning -> done, hardcoded
// - v20lj: find/create new Xsection file -> see main file


// Yield table (SR cuts w/ MET inverted)
// ===========================
// Process         Events     
// ===========================
// sgtop_sChannel  800 +- 5
// sgtop_tChannel  2733 +- 27
// sgtop_WtChannel 906 +- 15
// ttbar           12083 +- 57
// wjetsLF         246 +- 101
// wjetsHF         7710 +- 221
// zjets_diboson   2461 +- 72
// QCD             9779 +- 134
// ---------------------------
// MC total        36718 +- 294
// ---------------------------
// DATA            36382 +- 191
// ===========================
// S/B             0.0223
// S/err_B         2.7220
// ===========================

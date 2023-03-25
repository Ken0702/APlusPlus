{
    //____________________________________________________________________
    //
    // SgTop s-channel analysis main configuration
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!!   Do not execute this file   !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    // This file is used by task_sChannelAnlysis_Signal.C and the
    // corresponding scripts for the ctrl regions.
    //
    // Author: Oliver Maria Kind <mailto: kind@physik.hu-berlin.de>
    // Update: $Id$
    // Copyright: 2014 (C) Oliver Maria Kind
    //   

    // ========================
    // Configure top-level task
    // ========================
    AtlTopLevelAnalysis *main
	= new AtlTopLevelAnalysis("AtlSgTop_sChannelAnalysis",
				  "SgTop sChannel Analysis");

    // AtlTopLevelAnalysis options
    main->SetForceRetry(ForceRetry);
    main->SetBuildHforTree(BuildHforTree);

    // main->SetBuildAnalysisTree(kTRUE);
    // main->SetBuildMemTkAnalysisTree(kFALSE);

    main->SetBuildAnalysisTree(kFALSE);
    main->SetBuildMemTkAnalysisTree(kTRUE);

    main->SetBuildMergingTree(BuildMergingTree);
    main->SetBuildPlottingTree(BuildPlottingTree);
    if ( UseNewInputFormat ) {
	// v15
	main->SetUseHforSamples(kFALSE);
    } else {
	// v11
	main->SetUseHforSamples(kTRUE);
    }
    // option for RadHi/RadLo systematic for ttbar
    // main->SetTtbarIsSplitted(kFALSE);
    
    // Grid config
    // use latest root version that is available on gcc47
    // need gcc47 to be compatible with lhapdf version from cvmfs
    main->SetGridRootVersion("5.34/21");
    main->SetGridCmtVersion("x86_64-slc6-gcc47-opt");
    main->SetGridUserName("kind");
    main->SetGridSuffix("v16");
    main->SetGridJob(kTRUE);

    // MemTk config
    main->SetMemTkRunScript("run_master.C");

    // Subjob Config
    // default:
    // sgtop s-channel 30
    // sgtop t-channel 5
    // wjets           3
    // ttbar           3
    // main->SetNSubJobsTtbar(3);
    // main->SetNSubJobsWjets(10);

    // Setup Samples
    main->SetTreeName("physics");
    if ( UseNewInputFormat ) {
	// v15
	main->SetInputFormat(AtlTopLevelAnalysis::kTreePerSyst);
	main->SetSgTopVersion("v15");
	// default sample list
	main->SetDSIDList("SgTopDSIDs_v15.txt");
	// for pdf systematic only!
	// main->SetDSIDList("SgTopDSIDs_v15_pdf.txt");
    } else {
	// v11
	main->SetInputFormat(AtlTopLevelAnalysis::kFilePerSyst);
	main->SetSgTopVersion("v11");
	main->SetDSIDList("SgTopDSIDs.txt");
    }
    main->ReadSampleList();

    // Setup Directories
    if ( Selection == kSignalRegion ) {
	if ( UseNewInputFormat ) {
	    // v15
	    main->SetHistDir("/rdsk/datm7/atlas/kind/8TeV_s_channel/v15/sig_INTschanME");
	    main->SetJobHomeDir("/rdsk/datm7/atlas/kind/8TeV_s_channel/v15/sig_INTschanME");
	    main->SetTempDir("/datc/atlas/stamm/v15/sig_INTschanME");

	    // v15 pdf nominal (for running MemTk + MemDisc Jobs with pdf uncertainty only!)
	    // main->SetHistDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/sig_pdfnominal");
	    // main->SetJobHomeDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/sig_pdfnominal");
	    // main->SetTempDir("/datc/atlas/stamm/v15/sig_INTschanME");

	    // v15 no veto ctrl plots
	    // main->SetHistDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/sig_noveto");
	    // main->SetJobHomeDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/sig_noveto");
	    // main->SetTempDir("/datc/atlas/stamm/v15/sig_noveto");

	    // v15 dilepton veto ctrl plots
	    // main->SetHistDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/sig_dileptonveto");
	    // main->SetJobHomeDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/sig_dileptonveto");
	    // main->SetTempDir("/datc/atlas/stamm/v15/sig_dileptonveto");
	} else {
	    // v11
	    main->SetHistDir("/rdsk/datm7/atlas/kind/8TeV_s_channel/v11/sig_INTschanME");
	    main->SetJobHomeDir("/rdsk/datm7/atlas/kind/8TeV_s_channel/v11/sig_INTschanME");
	    main->SetTempDir("/datc/atlas/stamm/sig_INTschanME");
	}
    } else if ( Selection == kWjetsCtrlRegion ) {
	if ( UseNewInputFormat ) {
	    // v15
	    main->SetHistDir("/rdsk/datm7/atlas/kind/8TeV_s_channel/v15/ctrl_wjets_INTschanME");
	    main->SetJobHomeDir("/rdsk/datm7/atlas/kind/8TeV_s_channel/v15/ctrl_wjets_INTschanME");
	    main->SetTempDir("/datc/atlas/stamm/v15/ctrl_wjets_INTschanME");

	    // v15 pdf nominal
	    // -not needed, no MemTk or MemDisc jobs needed-
	    
	    // v15 no veto ctrl plots
	    // main->SetHistDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/ctrl_wjets_noveto");
	    // main->SetJobHomeDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/ctrl_wjets_noveto");
	    // main->SetTempDir("/datc/atlas/stamm/v15/ctrl_wjets_noveto");

	    // v15 dilepton veto ctrl plots
	    // main->SetHistDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/ctrl_wjets_dileptonveto");
	    // main->SetJobHomeDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/ctrl_wjets_dileptonveto");
	    // main->SetTempDir("/datc/atlas/stamm/v15/ctrl_wjets_dileptonveto");
	} else {
	    // v11
	    main->SetHistDir("/rdsk/datm7/atlas/kind/8TeV_s_channel/v11/ctrl_wjets_INTschanME");
	    main->SetJobHomeDir("/rdsk/datm7/atlas/kind/8TeV_s_channel/v11/ctrl_wjets_INTschanME");
	    main->SetTempDir("/datc/atlas/stamm/ctrl_wjets_INTschanME_hdamp");
	}
    } else if ( Selection == kTtbarCtrlRegion ) {
	if ( UseNewInputFormat ) {
	    // v15
	    main->SetHistDir("/rdsk/datm7/atlas/kind/8TeV_s_channel/v15/ctrl_ttbar_INTschanME");
	    main->SetJobHomeDir("/rdsk/datm7/atlas/kind/8TeV_s_channel/v15/ctrl_ttbar_INTschanME");
	    main->SetTempDir("/datc/atlas/stamm/v15/ctrl_ttbar_INTschanME");

	    // v15 no veto ctrl plots
	    // main->SetHistDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/ctrl_ttbar_noveto");
	    // main->SetJobHomeDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/ctrl_ttbar_noveto");
	    // main->SetTempDir("/datc/atlas/stamm/v15/ctrl_ttbar_noveto");

	    // v15 dilepton veto ctrl plots
	    // main->SetHistDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/ctrl_ttbar_dileptonveto");
	    // main->SetJobHomeDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/ctrl_ttbar_dileptonveto");
	    // main->SetTempDir("/datc/atlas/stamm/v15/ctrl_ttbar_dileptonveto");
	} else {
	    // v11
	    main->SetHistDir("/rdsk/datm7/atlas/kind/8TeV_s_channel/v11/ctrl_ttbar_INTschanME");
	    main->SetJobHomeDir("/rdsk/datm7/atlas/kind/8TeV_s_channel/v11/ctrl_ttbar_INTschanME");
	    main->SetTempDir("/datc/atlas/stamm/ctrl_ttbar_INTschanME");
	}
    }

    // For MemTk Analysis (i.e. "the histdir + main->GetName()")
    main->SetInputDir(Form("%s/%s",
			   main->GetHistDir()->Data(),
			   main->GetName()));
    
    // For A++-Analysis
    // (i.e. "histdir + main->GetName()")
    //
    // n.b. MemTk plotting --> 2nd task-file needed
    main->SetPlotDir(Form("%s/%s",
			  main->GetHistDir()->Data(),
			  main->GetName()));
        
    // Set output file prefix
    main->SetOutputFilePrefix("ntup_schan");
    
    // Set jet bins
    if ( Selection == kSignalRegion
	 || Selection == kWjetsCtrlRegion ) { 
	main->AddJetMultiplicity(AtlSelector::kTwoJet);
    } else if ( Selection == kTtbarCtrlRegion ) {
	main->AddJetMultiplicity(AtlSelector::kFourJet);
    }
    
    // Set lepton channels
    main->AddLeptonChannel(AtlSelector::kElectron);
    //main->AddLeptonChannel(AtlSelector::kMuon);
    
    // ===============
    // Set systematics
    // ===============
//    main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpNominal);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpScaleFactors);
    main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpDefaultSyst);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpJES);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpJESCompOnly);

    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpBTagEV);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpSampleSyst);

    // main->AddSystematic(AtlTopLevelAnalysis::kPdf_up);
    // main->AddSystematic(AtlTopLevelAnalysis::kPdf_down);
    
    // main->AddSystematic(AtlTopLevelAnalysis::kWtDS);
    // main->AddSystematic(AtlTopLevelAnalysis::kMCatNLO_wt);
    // main->AddSystematic(AtlTopLevelAnalysis::kMCatNLO_ttbar);
    // main->AddSystematic(AtlTopLevelAnalysis::kaMCatNLO_tchan);
    // main->AddSystematic(AtlTopLevelAnalysis::kaMCatNLO_schan);
    // main->AddSystematic(AtlTopLevelAnalysis::kRadHi_ttbar);
    // main->AddSystematic(AtlTopLevelAnalysis::kRadLo_ttbar);

    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar1);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar2);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar3);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar4);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar5);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar6);

    // main->AddSystematic(AtlTopLevelAnalysis::kBTAGSF_UP_BREAK0);
    // main->AddSystematic(AtlTopLevelAnalysis::kBTAGSF_UP_BREAK1);
    // main->AddSystematic(AtlTopLevelAnalysis::kBTAGSF_UP_BREAK2);
    // main->AddSystematic(AtlTopLevelAnalysis::kBTAGSF_UP_BREAK3);
    // main->AddSystematic(AtlTopLevelAnalysis::kBTAGSF_UP_BREAK4);
    // main->AddSystematic(AtlTopLevelAnalysis::kBTAGSF_UP_BREAK5);
    // main->AddSystematic(AtlTopLevelAnalysis::kBTAGSF_UP_BREAK6);
    // main->AddSystematic(AtlTopLevelAnalysis::kBTAGSF_UP_BREAK7);
    // main->AddSystematic(AtlTopLevelAnalysis::kBTAGSF_UP_BREAK8);
    // main->AddSystematic(AtlTopLevelAnalysis::kBTAGSF_DOWN_BREAK0);
    // main->AddSystematic(AtlTopLevelAnalysis::kBTAGSF_DOWN_BREAK1);
    // main->AddSystematic(AtlTopLevelAnalysis::kBTAGSF_DOWN_BREAK2);
    // main->AddSystematic(AtlTopLevelAnalysis::kBTAGSF_DOWN_BREAK3);
    // main->AddSystematic(AtlTopLevelAnalysis::kBTAGSF_DOWN_BREAK4);
    // main->AddSystematic(AtlTopLevelAnalysis::kBTAGSF_DOWN_BREAK5);
    // main->AddSystematic(AtlTopLevelAnalysis::kBTAGSF_DOWN_BREAK6);
    // main->AddSystematic(AtlTopLevelAnalysis::kBTAGSF_DOWN_BREAK7);
    // main->AddSystematic(AtlTopLevelAnalysis::kBTAGSF_DOWN_BREAK8);

    // main->AddSystematic(AtlTopLevelAnalysis::kCTAGSF_UP_BREAK0);
    // main->AddSystematic(AtlTopLevelAnalysis::kCTAGSF_UP_BREAK1);
    // main->AddSystematic(AtlTopLevelAnalysis::kCTAGSF_UP_BREAK2);
    // main->AddSystematic(AtlTopLevelAnalysis::kCTAGSF_UP_BREAK3);
    // main->AddSystematic(AtlTopLevelAnalysis::kCTAGSF_UP_BREAK4);
    // main->AddSystematic(AtlTopLevelAnalysis::kCTAGSF_UP_BREAK5);
    // main->AddSystematic(AtlTopLevelAnalysis::kCTAGSF_UP_BREAK6);
    // main->AddSystematic(AtlTopLevelAnalysis::kCTAGSF_DOWN_BREAK0);
    // main->AddSystematic(AtlTopLevelAnalysis::kCTAGSF_DOWN_BREAK1);
    // main->AddSystematic(AtlTopLevelAnalysis::kCTAGSF_DOWN_BREAK2);
    // main->AddSystematic(AtlTopLevelAnalysis::kCTAGSF_DOWN_BREAK3);
    // main->AddSystematic(AtlTopLevelAnalysis::kCTAGSF_DOWN_BREAK4);
    // main->AddSystematic(AtlTopLevelAnalysis::kCTAGSF_DOWN_BREAK5);
    // main->AddSystematic(AtlTopLevelAnalysis::kCTAGSF_DOWN_BREAK6);

    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK0);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK1);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK2);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK3);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK4);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK5);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK6);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK7);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK8);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK9);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK10);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_UP_BREAK11);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK0);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK1);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK2);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK3);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK4);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK5);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK6);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK7);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK8);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK9);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK10);
    // main->AddSystematic(AtlTopLevelAnalysis::kMISTAGSF_DOWN_BREAK11);


    // Oliver
    // main->AddSystematic(AtlTopLevelAnalysis::kBJES_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kBJES_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kETAINTERCALIB_MODEL_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kETAINTERCALIB_MODEL_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kETAINTERCALIB_TOTALSTAT_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kETAINTERCALIB_TOTALSTAT_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_DET1_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_DET1_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_DET2_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_DET2_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_DET3_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_DET3_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MIX1_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MIX1_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MIX2_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MIX2_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MIX3_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MIX3_UP);

    // Soeren
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MIX4_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MIX4_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MODEL1_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MODEL1_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MODEL2_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MODEL2_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MODEL3_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MODEL3_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MODEL4_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_MODEL4_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_STAT1_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_STAT1_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_STAT2_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_STAT2_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_STAT3_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_STAT3_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_STAT4_DOWN);

    // Patrick
    // main->AddSystematic(AtlTopLevelAnalysis::kJESEFF_STAT4_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESPILEUP_OFFMU_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESPILEUP_OFFMU_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESPILEUP_OFFNPV_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESPILEUP_OFFNPV_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESPILEUP_PT_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESPILEUP_PT_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESPILEUP_RHO_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kJESPILEUP_RHO_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kSINGLEPART_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kSINGLEPART_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kFLAVOR_COMP_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kFLAVOR_COMP_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kFLAVOR_RESP_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kFLAVOR_RESP_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kPUNCH_THROUGH_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kPUNCH_THROUGH_UP);

    // ==============
    // Setup Analysis
    // ==============
    
    // User environment
    main->AddUserEnv("LIBSINGLETOP");
    main->AddUserEnv("ROOTCOREDIR");

    // Analysis selector + Cuts
    main->SetSelector("AtlSgTop_sChannelAnalysis");
    if ( Selection == kSignalRegion ) {
	main->SetCut("fPt_Jet_min", "30.");
    } else if ( Selection == kWjetsCtrlRegion
		|| Selection == kTtbarCtrlRegion ) {
	main->SetCut("fPt_Jet_min", "25.");
    }
    main->SetCut("fMET_min", "35.");
    main->SetCut("fMtW_minPre", "30.");
    main->SetCut("fMtW_min", "30."); // final selection: 50.
    main->SetCut("fNBTags_min", "2");
    main->SetCut("fNBTags_max", "2");
    main->SetCut("fLeadingJetIsBTagged", "kFALSE");
    main->SetCut("fPt_BJetTop_min", "0."); // final selection: 50.
    main->SetCut("fAdditionalJetVeto", "kTRUE"); // for MEM
    if ( Selection == kWjetsCtrlRegion ) {
	main->SetCut("fVetoBTagWeight_min", "0.7892");
    }

    // Enable/disable nu,W,top reconstruction and the computation of
    // the LPSC BDT variables (default=off)
    main->SetCut("fDoReconstructTop", "kFALSE");
    // enable truth study
    main->SetCut("fMCTruthStudy", "kFALSE");
    
    // Set output tree and format
    main->SetOutputTree("physics", "SgTopD3PD s-channel selection");
    
    // Object Selection Tool
    AtlObjectsToolD3PDSgTop *tool_obj =
	new AtlObjectsToolD3PDSgTop("D3PDObjects","Analysis objects");
    main->AddTool(tool_obj);
    main->SetToolParameter("D3PDObjects", "fJetsAuthor",
			   "AtlJet::kAntiKt4LCTopo");
    main->SetToolParameter("D3PDObjects", "fJetPt_min", "25.");
    main->SetToolParameter("D3PDObjects", "fJetEta_min", "0.");
    main->SetToolParameter("D3PDObjects", "fJetEta_max", "2.5");
    main->SetToolParameter("D3PDObjects", "fElectronPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fMuonPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kMV1");
    if ( Selection == kSignalRegion
	|| Selection == kTtbarCtrlRegion ) {
	main->SetToolParameter("D3PDObjects",
			       "fBTagWeight_min", "0.7892"); // MV1@70%
    } else if ( Selection == kWjetsCtrlRegion ) {
	main->SetToolParameter("D3PDObjects",
			       "fBTagWeight_min", "0.3511 "); // MV1@80%
    }

    // Object Reco SF Tool
    AtlObjRecoScaleFactorTool *tool_sf = new AtlObjRecoScaleFactorTool("SFTool",
								       "SF tool");
    main->AddTool(tool_sf);
    // The task is not going to start if options do not match with the D3PDObjects Tool
    // (but it is always better to check, of course)
    if ( Selection == kSignalRegion
	|| Selection == kTtbarCtrlRegion ) {
	main->SetToolParameter("SFTool", "fBTagWP",
			       "AtlObjRecoScaleFactorTool::kMV1_70");
    } else if ( Selection == kWjetsCtrlRegion ) {
	main->SetToolParameter("SFTool", "fBTagWP",
			       "AtlObjRecoScaleFactorTool::kMV1_80");
    }
    main->SetToolParameter("SFTool", "fOperationMode",
			   "AtlObjRecoScaleFactorTool::kAllSF"); // check with D3PDObjects config!
    main->SetToolParameter("SFTool", "fUseCDI", "kTRUE"); // use CDI interface?

    // QCDMM Tool + Cuts
    AtlQCDMMTool *tool_qcd = new AtlQCDMMTool("QCDMMTool", "QCD MatrixMethod");
    main->AddTool(tool_qcd);

    // PdfReweightingTool + Cuts
    AtlPdfReweightingTool *tool_pdf = new AtlPdfReweightingTool("PdfReweightingTool", "Pdf Reweighting");
    main->AddTool(tool_pdf);

    
    // ==============
    // Setup Plotting
    // ==============
    // main->SetPlotterScheme("SgTop_s"); //Ttbar or SgTop
    main->SetPlotterScheme("SgTop_s_wjetsMerged");
    main->SetDataLuminosity(20276.900); // pb^{-1}
    main->SetAtlasLabelStatus("Internal"); 
    main->SetCenterOfMassEnergyLabel("8");
    main->SetLuminosityLabel("20 fb^{-1}");
    main->SetDrawDataZeroEntryErrors("kFALSE");
    main->SetDrawMCError("kTRUE");
    main->SetExportPDF("kFALSE");
    // main->SetSkipData(kTRUE);

    // ===============
    // Build task tree
    // ===============
    main->BuildTree( kTRUE );
}

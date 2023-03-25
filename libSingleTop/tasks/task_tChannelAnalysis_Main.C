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
	= new AtlTopLevelAnalysis("AtlSgTop_tChannelAnalysis",
				  "SgTop tChannel Analysis");

    // AtlTopLevelAnalysis options
    main->SetForceRetry(ForceRetry);
    main->SetBuildHforTree(BuildHforTree);

    main->SetBuildAnalysisTree(kTRUE);
    main->SetBuildMemTkAnalysisTree(kFALSE);

    // main->SetBuildAnalysisTree(kFALSE);
    // main->SetBuildMemTkAnalysisTree(kTRUE);

    Bool_t NoCutsSR = kTRUE;

    main->SetBuildMergingTree(BuildMergingTree);
    main->SetBuildPlottingTree(BuildPlottingTree);
    main->SetUseHforSamples(kFALSE);
    
    // Grid config
    main->SetGridRootVersion("5.34/21");
    main->SetGridCmtVersion("x86_64-slc6-gcc47-opt");
    main->SetGridUserName("sstamm");
    main->SetGridSuffix("v2b");
    // main->SetGridJob(kTRUE);
    main->SetGridJob(kFALSE);

    // MemTk config
    main->SetMemTkRunScript("run_tChannel.C");
    // t-channel defaults
    main->SetNSubJobsData(4);
    main->SetNSubJobsQCD(4);
    // main->SetNSubJobsSgTopSChannel(20);
    main->SetNSubJobsSgTopSChannel(4);
    main->SetNSubJobsSgTopTChannel(35);
    // main->SetNSubJobsTtbar(20);
    main->SetNSubJobsTtbar(2);
    main->SetNSubJobsWjetsB(12);
    main->SetNSubJobsWjetsC(1);

    // main->SetNSubJobsSgTopTChannel(50);

    // Setup Samples
    main->SetTreeName("physics");
    main->SetInputFormat(AtlTopLevelAnalysis::kTreePerSyst);
    main->SetSgTopVersion("v15");
    main->SetDSIDList("SgTopDSIDs_v15.txt");
    main->ReadSampleList();

    // Setup Directories
    if ( Selection == kSignalRegion ) {
	if ( UseMV1c ) {
	    // Only jes fix for MemTk
	    // main->SetHistDir("/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15/cut7dl");
	    // main->SetJobHomeDir("/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15/cut7dl");
	    // main->SetTempDir("/datc/atlas/stamm/cut7dl");

	    if ( NoCutsSR ) {
		main->SetHistDir("/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15f/signal_nocuts");
		main->SetJobHomeDir("/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15f/signal_nocuts");
		main->SetTempDir("/datc/atlas/stamm/v15f/8TeV_t_channel/signal_nocuts");
	    } else {
		// Full Preselection JES fix (new histo binning)
		main->SetHistDir("/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15f/signal");
		main->SetJobHomeDir("/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15f/signal");
		main->SetTempDir("/datc/atlas/stamm/v15f/8TeV_t_channel/signal");
	    }

	    // old testing for AF2 samples
	    // main->SetHistDir("/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15/AtlFast2_Samples");
	    // main->SetJobHomeDir("/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15/AtlFast2_Samples");
	    // main->SetTempDir("/datc/atlas/stamm/AtlFast2_Samples");
	} else {
	    main->SetHistDir("/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15/Signal_MV1");
	    main->SetJobHomeDir("/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15/Signal_MV1");
	    main->SetTempDir("/datc/atlas/stamm/Signal_MV1");
	}
    } else if ( Selection == kWjetsCtrlRegion ) {
	if ( UseMV1c ) {
	    main->SetHistDir("/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15f/ctrl_Wjets");
	    main->SetJobHomeDir("/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15f/ctrl_Wjets");
	    main->SetTempDir("/datc/atlas/stamm/v15f/ctrl_Wjets");
	} else {
	    main->SetHistDir("/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15f/ctrl_Wjets_MV1");
	    main->SetJobHomeDir("/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15f/ctrl_Wjets_MV1");
	    main->SetTempDir("/datc/atlas/stamm/v15f/ctrl_Wjets_MV1");
	}
    } else if ( Selection == kTtbarCtrlRegion ) {
	main->SetHistDir("/rdsk/datm7/atlas/stamm/8TeV_t_channel/v15/ctrl_Ttbar");
	main->SetJobHomeDir("/rdsk/datm7/atlas/stamm/8TeV_t_channel/v15/ctrl_Ttbar");
	main->SetTempDir("/datc/atlas/stamm/ctrl_Ttbar");
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
    main->SetOutputFilePrefix("ntup_tchan");
    
    // Set jet bins
    if ( Selection == kSignalRegion
	 || Selection == kWjetsCtrlRegion ) { 
	main->AddJetMultiplicity(AtlSelector::kTwoJet);
    } else if ( Selection == kTtbarCtrlRegion ) {
	main->AddJetMultiplicity(AtlSelector::kFourJet);
    }
    
    // Set lepton channels
    main->AddLeptonChannel(AtlSelector::kElectron);
    main->AddLeptonChannel(AtlSelector::kMuon);
    
    // ===============
    // Set systematics
    // ===============
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpNominal);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpJES);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpDefaultSyst);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpScaleFactors);

    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpSampleSyst);

    // main->AddSystematic(AtlTopLevelAnalysis::kWtDS);
    // main->AddSystematic(AtlTopLevelAnalysis::kMCatNLO_wt);
    // main->AddSystematic(AtlTopLevelAnalysis::kMCatNLO_ttbar); // 2 subjob < 15h
    // main->AddSystematic(AtlTopLevelAnalysis::kaMCatNLO_tchan); // 50 subjobs
    // main->AddSystematic(AtlTopLevelAnalysis::kaMCatNLO_schan);
    
    // main->AddSystematic(AtlTopLevelAnalysis::kRadHi_ttbar);
    // main->AddSystematic(AtlTopLevelAnalysis::kRadLo_ttbar);

    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar1); // 4 Subjobs a 24h
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar2);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar3);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar4);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar5);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar6);

    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar_tchan1); // 15 subjobs a 22h
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar_tchan2);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar_tchan3);
    // main->AddSystematic(AtlTopLevelAnalysis::kScaleVar_tchan4);

    main->AddSystematic(AtlTopLevelAnalysis::kPowhegPythia_t); // 20 subjobs a 24h
    main->AddSystematic(AtlTopLevelAnalysis::kPowhegHerwig_t); // 35 subjobs a 24h

    // main->AddSystematic(AtlTopLevelAnalysis::kWjetsGen); //

    // ==============
    // Setup Analysis
    // ==============
    
    // User environment
    main->AddUserEnv("LIBSINGLETOP");

    // Analysis selector + Cuts
    main->SetSelector("AtlSgTop_tChannelAnalysis");
    if ( Selection == kSignalRegion ) {
	main->SetCut("fPt_Jet_min", "35."); // was 35. or 30.
	main->SetCut("fPt_FwdJet_min", "35."); // does not matter with pt cut at 35.
	// Apply additional t-channel cuts    
	main->SetCut("fApplyDileptonVeto", "kTRUE");

	if ( !NoCutsSR ) {
	    // default values
	    main->SetCut("fMljb_min", "40.");
	    main->SetCut("fMljb_max", "160.");
	    main->SetCut("fMjlfjb_min", "80.");
	    main->SetCut("fMtW_max", "125.");
	    main->SetCut("fHt_min", "180.");
	    main->SetCut("fPtJetlead_min", "40.");
	    main ->SetCut("fMjlfjbHt_cut", "kTRUE");
	}
    } else if ( Selection == kWjetsCtrlRegion
		|| Selection == kTtbarCtrlRegion ) {
	main->SetCut("fPt_Jet_min", "25.");
	main->SetCut("fApplyDileptonVeto", "kFALSE");
    }
    main->SetCut("fMET_min", "30."); // was 40.
    main->SetCut("fMtW_minPre", "30.");
    main->SetCut("fMtW_min", "50.");
    if ( Selection == kSignalRegion ||
	 Selection == kWjetsCtrlRegion ) {
	main->SetCut("fNBTags_min", "1");
	main->SetCut("fNBTags_max", "1");
    } else if ( Selection == kTtbarCtrlRegion ) {
	main->SetCut("fNBTags_min", "2");
	main->SetCut("fNBTags_max", "2");
    }	
    main->SetCut("fLeadingJetIsBTagged", "kFALSE");
    main->SetCut("fAdditionalJetVeto", "kTRUE"); // for MEM
    main->SetCut("fVetoBTagWeight_min", "0.7892");
    if ( Selection == kWjetsCtrlRegion ) {
	main->SetCut("fApplyBJetVeto", "kTRUE");
	main->SetCut("fVetoBTagWeight_min", "0.9195");  // MV1c@50%
	main->SetCut("fVetoBTagger", "AtlBTag::kMV1c"); // MV1c
    }
    main->SetCut("fMCTruthStudy", "kFALSE");
    
    // Set output tree and format
    main->SetOutputTree("physics", "SgTopD3PD t-channel selection");
    
    // Object Selection Tool
    AtlObjectsToolD3PDSgTop *tool_obj =
	new AtlObjectsToolD3PDSgTop("D3PDObjects","Analysis objects");
    main->AddTool(tool_obj);
    main->SetToolParameter("D3PDObjects", "fJetsAuthor",
			   "AtlJet::kAntiKt4LCTopo");
    main->SetToolParameter("D3PDObjects", "fJetPt_min", "25.");
    main->SetToolParameter("D3PDObjects", "fJetEta_min", "0.");
    main->SetToolParameter("D3PDObjects", "fJetEta_max", "4.5");
    main->SetToolParameter("D3PDObjects", "fElectronPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fMuonPt_min", "30.");
    if ( Selection == kSignalRegion
	 || Selection == kTtbarCtrlRegion ) {
	if ( UseMV1c ) {
	    // Use MV1c 50%
	    main->SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kMV1c");
	    main->SetToolParameter("D3PDObjects",
				   "fBTagWeight_min", "0.9195");
	} else {
	    // Use MV1 70%
	    main->SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kMV1");
	    main->SetToolParameter("D3PDObjects",
				   "fBTagWeight_min", "0.7892"); // MV1@70% - s Channel WP
	}
    } else if ( Selection == kWjetsCtrlRegion ) {
	if ( UseMV1c ) {
	    // Use MV1 85%
	    main->SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kMV1c");
	    main->SetToolParameter("D3PDObjects",
				   "fBTagWeight_min", "0.4051"); // MV1@85%
	} else {
	    // Use MV1 85%
	    main->SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kMV1");
	    main->SetToolParameter("D3PDObjects",
				   "fBTagWeight_min", "0.1340"); // MV1@85%
	}
    }
    
    // Object Reco SF Tool
    AtlObjRecoScaleFactorTool *tool_sf = new AtlObjRecoScaleFactorTool("SFTool",
								       "SF tool");
    main->AddTool(tool_sf);
    if ( Selection == kSignalRegion
	 || Selection == kTtbarCtrlRegion ) {
	if ( UseMV1c ) {
	    main->SetToolParameter("SFTool", "fBTagWP",
				   "AtlObjRecoScaleFactorTool::kMV1c_50");
	} else {
	    main->SetToolParameter("SFTool", "fBTagWP",
				   "AtlObjRecoScaleFactorTool::kMV1_70");
	}
    } else if ( Selection == kWjetsCtrlRegion ) {
	if ( UseMV1c ) {
	    main->SetToolParameter("SFTool", "fBTagWP",
				   "AtlObjRecoScaleFactorTool::kMV1c_80");
	} else {
	    main->SetToolParameter("SFTool", "fBTagWP",
				   "AtlObjRecoScaleFactorTool::kMV1_85");
	}
    }
    main->SetToolParameter("SFTool", "fOperationMode",
			   "AtlObjRecoScaleFactorTool::kAllSF"); // check with D3PDObjects config!
    main->SetToolParameter("SFTool", "fUseCDI", "kTRUE"); // use CDI interface?

    // QCDMM Tool + Cuts
    AtlQCDMMTool *tool_qcd = new AtlQCDMMTool("QCDMMTool", "QCD MatrixMethod");
    main->AddTool(tool_qcd);
    
    // ==============
    // Setup Plotting
    // ==============
    main->SetPlotterScheme("SgTop_t"); //Ttbar or SgTop
    main->SetDataLuminosity(20276.900); // pb^{-1}
    main->SetAtlasLabelStatus("Work in Progress"); 
    main->SetCenterOfMassEnergyLabel("8");
    main->SetLuminosityLabel("20 fb^{-1}");
    main->SetDrawDataZeroEntryErrors("kFALSE");
    main->SetDrawMCError("kTRUE");
    main->SetExportPDF("kFALSE");

    // ===============
    // Build task tree
    // ===============
    main->BuildTree( kTRUE );
}

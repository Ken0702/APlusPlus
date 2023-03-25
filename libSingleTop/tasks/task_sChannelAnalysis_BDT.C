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
							  "SgTop sChannel BDT Analysis");

    enum ESelection {
	kSignalRegion,
	kWjetsCtrlRegion
    };
	
    // Shortcut to settings
    Bool_t UseNewInputFormat = true;
	
    Bool_t ForceRetry        = kTRUE;
    Bool_t BuildPlottingTree = kFALSE;
    Bool_t BuildMergingTree  = kFALSE;
	
    ESelection Selection   = kSignalRegion;
	
    // AtlTopLevelAnalysis options
    main->SetForceRetry(ForceRetry);
    main->SetBuildHforTree(kFALSE);
    main->SetBuildAnalysisTree(kFALSE);
    main->SetBuildMergingTree(BuildMergingTree);
    main->SetBuildPlottingTree(BuildPlottingTree);
    main->SetBuildMemTkAnalysisTree(kFALSE);
    main->SetBuildBDTAnalysisTree(kTRUE);
    if ( UseNewInputFormat ) {
	// v14
	main->SetUseHforSamples(kFALSE);
    } else {
	// v11
	main->SetUseHforSamples(kTRUE);
    }

    // Grid config
    main->SetGridRootVersion("5.34/12");
    main->SetGridCmtVersion("x86_64-slc5-gcc43-opt");
    main->SetGridUserName("prieck");
    main->SetGridSuffix("bdt1");
    main->SetGridJob(kFALSE);

    // Setup Samples
    main->SetTreeName("physics");
    if ( UseNewInputFormat ) {
	// v14
	main->SetInputFormat(AtlTopLevelAnalysis::kTreePerSyst);
	main->SetSgTopVersion("v14");
	main->SetDSIDList("SgTopDSIDs_v14.txt");
    } else {
	// v11
	main->SetInputFormat(AtlTopLevelAnalysis::kFilePerSyst);
	main->SetSgTopVersion("v11");
	main->SetDSIDList("SgTopDSIDs.txt");
    }
    main->ReadSampleList();

    // Setup Directories (same as in task_sChannelAnalysis_Main.C)
    if ( Selection == kSignalRegion ) {
	if ( UseNewInputFormat ) {
	    // v14
		main->SetHistDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v14/sig_INTschanME");
		main->SetJobHomeDir("/rdsk/datm7/atlas/rieck/8TeV_s_channel/v14/sig_INTschanME");
	    main->SetTempDir("/datc/atlas/rieck/sig_INTschanME");
	} else {
	    // v11
	    main->SetHistDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v11/sig_INTschanME");
	    main->SetJobHomeDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v11/sig_INTschanME");
	    main->SetTempDir("/datc/atlas/stamm/sig_INTschanME");
	}
    } else if ( Selection == kWjetsCtrlRegion ) {
	if ( UseNewInputFormat ) {
	    // v14
	    main->SetHistDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v14/ctrl_wjets_INTschanME");
	    main->SetJobHomeDir("/rdsk/datm7/atlas/rieck/8TeV_s_channel/v14/ctrl_wjets_INTschanME");
	    main->SetTempDir("/datc/atlas/rieck/ctrl_wjets_INTschanME");
	} else {
	    // v11
	    main->SetHistDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v11/ctrl_wjets_INTschanME");
	    main->SetJobHomeDir("/rdsk/datm7/atlas/rieck/8TeV_s_channel/v11/ctrl_wjets_INTschanME");
	    main->SetTempDir("/datc/atlas/rieck/ctrl_wjets_INTschanME");
	}
	}
	
    // For BDT Analysis
    // (i.e. "histdir + main->GetName() + MemTkAnalysis")
    //
    // Name of 'main' must be the same as task_sChannelAnalysis_Main.C
    main->SetInputDir(Form("%s/%s",
			   main->GetHistDir()->Data(),
			   main->GetName()));

    // For BDT Analysis
    // (i.e. "histdir + main->GetName() + BDTAnalysis")
    //
    // Name of 'main' must be the same as task_sChannelAnalysis_Main.C
    main->SetPlotDir(Form("%s/%s/%s",
			   main->GetHistDir()->Data(),
			   main->GetName(),
			   "BDTAnalysis"));
    
    // Set output file prefix
    main->SetOutputFilePrefix("ntup_schan");
    
    // Set jet bins
    main->AddJetMultiplicity(AtlSelector::kTwoJet);
    
    // Set lepton channels
    main->AddLeptonChannel(AtlSelector::kElectron);
    main->AddLeptonChannel(AtlSelector::kMuon);
    
    // ===============
    // Set systematics
    // ===============
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpNominal);
    // main->AddSystematic(AtlTopLevelAnalysis::kBTAGSF_UP);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpDefaultSyst);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpJES);
    // main->AddSystematic(AtlTopLevelAnalysis::kJEFF);
    // main->AddSystematic(AtlTopLevelAnalysis::kJER);
    // main->AddSystematic(AtlTopLevelAnalysis::kJVF_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kJVF_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kEER_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kEER_DOWN);
    // main->AddSystematic(AtlTopLevelAnalysis::kEES_UP);
    // main->AddSystematic(AtlTopLevelAnalysis::kEES_DOWN);
	
    main->AddSystematic(AtlTopLevelAnalysis::kNOMINAL);
	
    // ==============
    // Setup Analysis
    // ==============
    
    // User environment
    main->AddUserEnv("LIBSINGLETOP");
    main->AddUserEnv("TMVAHOME");

    // Analysis selector + Cuts
    main->SetSelector("AtlSgTop_sChannelBDT");

    // ------------------------------------------------------------
    // Check tools with task_sChannelAnalysis_Main - Signal region!
    // ------------------------------------------------------------

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
    if ( Selection == kSignalRegion ) {
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
    if ( Selection == kSignalRegion ) {
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

    // ==============
    // Setup Plotting
    // ==============
    main->SetPlotterScheme("SgTop_s"); //Ttbar or SgTop
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

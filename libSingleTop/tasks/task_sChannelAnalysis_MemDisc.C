{
    //____________________________________________________________________
    //
    // SgTop s-channel analysis configuration for running
    // the Mem Discriminant computation.
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!!   Do not execute this file   !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
				  "SgTop sChannel Mem Discriminant Analysis");

    // Configuration
    Bool_t UseNewInputFormat = kTRUE;
    
    Bool_t ForceRetry        = kFALSE;
    Bool_t BuildMergingTree  = kTRUE;
    Bool_t BuildPlottingTree = kTRUE;
    
    // AtlTopLevelAnalysis options
    main->SetForceRetry(ForceRetry);
    main->SetBuildHforTree(kFALSE);
    main->SetBuildAnalysisTree(kFALSE);
    main->SetBuildMemTkAnalysisTree(kFALSE);
    main->SetBuildMergingTree(BuildMergingTree);
    main->SetBuildPlottingTree(BuildPlottingTree);
    main->SetBuildBDTAnalysisTree(kTRUE);
    main->SetUseSubJobsAsInput(kTRUE);

    // option for RadHi/RadLo systematic for ttbar
    // main->SetTtbarIsSplitted(kFALSE);

    if ( UseNewInputFormat ) {
	// v14
	main->SetUseHforSamples(kFALSE);
    } else {
	// v11
	main->SetUseHforSamples(kTRUE);
    }

    // Grid config
    main->SetGridRootVersion("5.34/17");
    main->SetGridCmtVersion("x86_64-slc6-gcc47-opt");
    main->SetGridUserName("sstamm");
    main->SetGridSuffix("v2");
    main->SetGridJob(kFALSE);

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
    
    if ( UseNewInputFormat ) {
	main->SetHistDir("/rdsk/datm7/atlas/kind/8TeV_s_channel/v15/sig_INTschanME");
	main->SetJobHomeDir("/rdsk/datm7/atlas/kind/8TeV_s_channel/v15/sig_INTschanME");
	main->SetTempDir("/datc/atlas/stamm/v15/sig_INTschanME");

	// v15 pdf nominal (for running MemTk + MemDisc Jobs with pdf uncertainty only!)
	// main->SetHistDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/sig_pdfnominal");
	// main->SetJobHomeDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/sig_pdfnominal");
	// main->SetTempDir("/datc/atlas/stamm/v15/sig_INTschanME");
	
	// v15 no veto - SR
	// main->SetHistDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/sig_noveto");
	// main->SetJobHomeDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/sig_noveto");
	// main->SetTempDir("/datc/atlas/stamm/v15/sig_noveto");

	// v15 no veto - Wjets CR
	// main->SetHistDir("/rdsk/datm7/atlas/kind/8TeV_s_channel/v15/ctrl_wjets_INTschanME");
	// main->SetJobHomeDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/ctrl_wjets_INTschanME");
	// main->SetTempDir("/datc/atlas/stamm/v15/ctrl_wjets_INTschanME");

	// v15 no veto - ttbar CR
	// main->SetHistDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/ctrl_ttbar_noveto");
	// main->SetJobHomeDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/ctrl_ttbar_noveto");
	// main->SetTempDir("/datc/atlas/stamm/v15/ctrl_ttbar_INTschanME");

	// dilepton veto
	// main->SetHistDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/sig_dileptonveto");
	// main->SetJobHomeDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v15/sig_dileptonveto");
	// main->SetTempDir("/datc/atlas/stamm/v15/sig_dileptonveto");
    } else {
	// v11
	main->SetHistDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v11/sig_INTschanME");
	main->SetJobHomeDir("/rdsk/datm7/atlas/stamm/8TeV_s_channel/v11/sig_INTschanME");
	main->SetTempDir("/datc/atlas/stamm/sig_INTschanME");
    }
    
    // For Mem Discriminant Analysis
    // (i.e. "histdir + main->GetName() + MemTkAnalysis")
    //
    // Name of 'main' must be the same as task_sChannelAnalysis_Main.C
    main->SetInputDir(Form("%s/%s/%s",
			   main->GetHistDir()->Data(),
			   main->GetName(),
			   "MemTkAnalysis"));
    
    // For Mem Discriminant Analysis
    // (i.e. "histdir + main->GetName() + AnalysisName")
    //
    // Name of 'main' must be the same as task_sChannelAnalysis_Main.C
    main->SetPlotDir(Form("%s/%s/%s",
			  main->GetHistDir()->Data(),
			  main->GetName(),
			  "MemDiscAnalysis"));
    
    // Set output file prefix
    main->SetOutputFilePrefix("ntup_schan");
    
    // Set jet bin
    main->AddJetMultiplicity(AtlSelector::kTwoJet); // SR, wjets CR
    // main->AddJetMultiplicity(AtlSelector::kFourJet);   // ttbar CR
    
    // Set lepton channels
    main->AddLeptonChannel(AtlSelector::kElectron);
    main->AddLeptonChannel(AtlSelector::kMuon);
    
    // ===============
    // Set systematics
    // ===============
    main->AddSystematic(AtlTopLevelAnalysis::kNOMINAL);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpScaleFactors);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpDefaultSyst);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpJES);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpJESCompOnly);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpBTagEV);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpSampleSyst);

    // main->AddSystematic(AtlTopLevelAnalysis::kRadHi_ttbar);
    // main->AddSystematic(AtlTopLevelAnalysis::kRadLo_ttbar);

    // main->AddSystematic(AtlTopLevelAnalysis::kPdf_down);
    // main->AddSystematic(AtlTopLevelAnalysis::kPdf_up);
    
    // ==============
    // Setup Analysis
    // ==============
    
    // User environment
    main->AddUserEnv("LIBSINGLETOP");
	
    // Analysis selector + Cuts
    main->SetSelector("AtlSgTop_sChannelMemDisc");
    main->SetCut("fApplyDileptonVeto", "kTRUE");
    main->SetCut("fPt_LeadingJet_min", "40.");
    
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
    main->SetToolParameter("D3PDObjects",
			   "fBTagWeight_min", "0.7892"); // MV1@70%
    
    // Object Reco SF Tool
    AtlObjRecoScaleFactorTool *tool_sf = new AtlObjRecoScaleFactorTool("SFTool",
								       "SF tool");
    main->AddTool(tool_sf);
    main->SetToolParameter("SFTool", "fBTagWP",
			   "AtlObjRecoScaleFactorTool::kMV1_70"); // check with D3PDObjects fBTagWeight_min!
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
    main->SetPlotterScheme("SgTop_s_wjetsMerged"); //Ttbar or SgTop
    // main->SetPlotterScheme("SgTop_s"); //Ttbar or SgTop
    main->SetDataLuminosity(20276.900); // pb^{-1}
    main->SetAtlasLabelStatus("Internal"); 
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

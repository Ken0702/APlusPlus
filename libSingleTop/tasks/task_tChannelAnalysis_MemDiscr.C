{
    //____________________________________________________________________
    //
    // SgTop t-channel analysis configuration for running
    // the Mem Discriminant computation.
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
				  "SgTop tChannel Mem Discriminant Analysis");

    // Configuration
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
    main->SetUseHforSamples(kFALSE);

    // Grid config
    main->SetGridRootVersion("5.34/17");
    main->SetGridCmtVersion("x86_64-slc6-gcc47-opt");
    main->SetGridUserName("sstamm");
    main->SetGridSuffix("v2");
    main->SetGridJob(kFALSE);

    // Setup Samples
    main->SetTreeName("physics");
    main->SetInputFormat(AtlTopLevelAnalysis::kTreePerSyst);
    main->SetSgTopVersion("v15");
    // default sample list
    main->SetDSIDList("SgTopDSIDs_v15.txt");
    main->ReadSampleList();

    // Input directories
    main->SetHistDir("/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15/cut7dl");
    main->SetJobHomeDir("/rdsk/datm5/atlas/stamm/8TeV_t_channel/v15/cut7dl");
    main->SetTempDir("/datc/atlas/stamm/v15/cut7dl");
    
   
    // For Mem Discriminant Analysis
    // (i.e. "histdir + main->GetName() + MemTkAnalysis")
    //
    // Name of 'main' must be the same as task_tChannelAnalysis_Main.C
    main->SetInputDir(Form("%s/%s/%s",
			   main->GetHistDir()->Data(),
			   main->GetName(),
			   "MemTkAnalysis"));
    
    // For Mem Discriminant Analysis
    // (i.e. "histdir + main->GetName() + AnalysisName")
    //
    // Name of 'main' must be the same as task_tChannelAnalysis_Main.C
    main->SetPlotDir(Form("%s/%s/%s",
			  main->GetHistDir()->Data(),
			  main->GetName(),
			  "MemDiscrAnalysis"));
    
    // Set output file prefix
    main->SetOutputFilePrefix("ntup_tchan");
    
    // Set jet bin
    main->AddJetMultiplicity(AtlSelector::kTwoJet);
    
    // Set lepton channels
    main->AddLeptonChannel(AtlSelector::kElectron);
    // main->AddLeptonChannel(AtlSelector::kMuon);
    
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

    // ==============
    // Setup Analysis
    // ==============
    
    // User environment
    main->AddUserEnv("LIBSINGLETOP");
	
    // Analysis selector + Cuts
    main->SetSelector("AtlSgTop_tChannelMemDiscr");
    
    // ------------------------------------------------------------
    // Check tools with task_tChannelAnalysis_Main - Signal region!
    // ------------------------------------------------------------
    
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
    main->SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kMV1");
    main->SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kMV1c");
    main->SetToolParameter("D3PDObjects",
			   "fBTagWeight_min", "0.9195");
    
    // Object Reco SF Tool
    AtlObjRecoScaleFactorTool *tool_sf = new AtlObjRecoScaleFactorTool("SFTool",
								       "SF tool");
    main->AddTool(tool_sf);
    main->SetToolParameter("SFTool", "fBTagWP",
			   "AtlObjRecoScaleFactorTool::kMV1c_50");
    main->SetToolParameter("SFTool", "fOperationMode",
			   "AtlObjRecoScaleFactorTool::kAllSF"); // check with D3PDObjects config!
    main->SetToolParameter("SFTool", "fUseCDI", "kTRUE"); // use CDI interface?
    
    // QCDMM Tool + Cuts
    AtlQCDMMTool *tool_qcd = new AtlQCDMMTool("QCDMMTool", "QCD MatrixMethod");
    main->AddTool(tool_qcd);

    // PdfReweightingTool + Cuts
    // AtlPdfReweightingTool *tool_pdf = new AtlPdfReweightingTool("PdfReweightingTool", "Pdf Reweighting");
    // main->AddTool(tool_pdf);

    // ==============
    // Setup Plotting
    // ==============
    main->SetPlotterScheme("SgTop_t"); //Ttbar or SgTop
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

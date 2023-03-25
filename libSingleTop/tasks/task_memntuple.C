{
    // ========================
    // Configure top-level task
    // ========================
    AtlTopLevelAnalysis *main
	= new AtlTopLevelAnalysis("MemNtuple",
				  "MEM Ntuple Production");

    // AtlTopLevelAnalysis options
    main->SetForceRetry(kTRUE);
    main->SetBuildHforTree(kFALSE);
    main->SetBuildPlottingTree(kFALSE);
    main->SetUseHforSamples(kFALSE); // <<<===== !!! for now !!!
    main->SetBuildMemTkAnalysisTree(kTRUE);
    
    // Setup Samples
    main->SetTreeName("physics");
    main->SetSgTopVersion("v11");
    main->SetDSIDList("SgTopDSIDs.txt");
    main->ReadSampleList();

    // Setup Directories
    main->SetHistDir("/rdsk/datm7/atlas/kind/8TeV_s_channel/v11");
    main->SetJobHomeDir("/rdsk/datm8/atlas/kind/8TeV_s_channel/v11");
    main->SetTempDir("/datc/atlas/kind/");

    // Set output file prefix
    main->SetOutputFilePrefix("ntup_schan");
    
    // Set jet bins
    main->AddJetMultiplicity(AtlSelector::kTwoJet);
    
    // Set lepton channels
    main->AddLeptonChannel(AtlSelector::kElectron);
    main->AddLeptonChannel(AtlSelector::kMuon);

    // Set systematics
//    main->AddSystematic(AtlTopLevelAnalysis::kAllSyst);
    main->AddSystematic(AtlTopLevelAnalysis::kNOMINAL);

    // ==============
    // Setup Analysis
    // ==============
    
    // User environment
    main->AddUserEnv("LIBSINGLETOP");

    // Analysis selector + Cuts
    main->SetSelector("AtlMemNtupleProduction");
    main->SetOutputTree("physics", "SgTopD3PD s-channel PreSelection");
    main->SetCut("fOutputMode", "AtlSelector::kD3PDSgTop");
    main->SetCut("fMET_min", "35.");
    main->SetCut("fMtW_min", "30.");
    main->SetCut("fNBTags_min", "2");
    main->SetCut("fNBTags_max", "2");

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
    main->SetToolParameter("D3PDObjects", "fBTagWeight_min", "0.7892");

    // Object Reco SF Tool
    AtlObjRecoScaleFactorTool *tool_sf = new AtlObjRecoScaleFactorTool("SFTool","SF tool");
    main->AddTool(tool_sf);
    main->SetToolParameter("SFTool", "fBTagger", "AtlBTag::kMV1"); // check with D3PDObjects fBTagger!
    main->SetToolParameter("SFTool", "fBTagWP", "AtlObjRecoScaleFactorTool::kMV1_70"); // check with D3PDObjects fBTagWeight_min!
    main->SetToolParameter("SFTool", "fOperationMode", "AtlObjRecoScaleFactorTool::kAllSF"); // check with D3PDObjects config!

    // QCDMM Tool + Cuts
    AtlQCDMMTool *tool_qcd = new AtlQCDMMTool("QCDMMTool", "QCD MatrixMethod");
    main->AddTool(tool_qcd);
    
    // // Analysis Tools
    // AtlCutFlowTool *tool_cf = new AtlCutFlowTool("AtlCutFlowTool",
    // 						 "Cut-flow Tool");
    // main->AddTool(tool_cf);
    // main->SetToolParameter("AtlCutFlowTool", "fLabelsDeflate", "kFALSE");
    // main->SetToolParameter("AtlCutFlowTool", "fNBins", "2");
    // main->SetToolParameter("AtlCutFlowTool", "fBinLabels",
    // 			   "\"No Cuts (D3PD),Etmiss > 30 GeV\"");

    // ==============
    // Setup Plotting
    // ==============
    main->SetPlotterScheme("SgTop_s"); // Ttbar or SgTop_s|t
    main->SetDataLuminosity(20276.900);
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

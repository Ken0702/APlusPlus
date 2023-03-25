{
    // ========================
    // Configure top-level task
    // ========================
    AtlTopLevelAnalysis *main
	= new AtlTopLevelAnalysis("AtlSgTop_tChannelAnalysis",
				  "SgTop tChannel Analysis");
    // AtlTopLevelAnalysis options
    //main->SetForceRetry(kFALSE);
    main->SetForceRetry(kTRUE);
    main->SetBuildHforTree(kFALSE);
    main->SetUseHforSamples(kFALSE);

    // Setup Samples
    main->SetTreeName("physics");
    main->SetSgTopVersion("v8");
    main->SetDSIDList("SgTopDSIDs.txt");
    main->ReadSampleList();

    // Setup Directories
    main->SetHistDir("/users/eeh/stamm/atlas/APlusPlus/libSingleTop/hist");
    main->SetJobHomeDir("/rdsk/datm8/atlas/stamm/");
    main->SetTempDir("/datc/atlas/stamm/");
//    main->SetOutputFilePrefix("hists");
    
    // Set jet bins
    main->AddJetMultiplicity(AtlSelector::kTwoJet);
    
    // Set lepton channels
    main->AddLeptonChannel(AtlSelector::kElectron);
    main->AddLeptonChannel(AtlSelector::kMuon);
    

    // Set systematics
    // ===============
    // - systematic groups (see also AtlTopLevelAnalysis.h)
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpAllDefault);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpAllJESComp);
    // -----------------------------------------------------------------------
    // - individual systematics
    main->AddSystematic(AtlTopLevelAnalysis::kNOMINAL);
    // main->AddSystematic(AtlTopLevelAnalysis::kJES_UP);

    // ==============
    // Setup Analysis
    // ==============
    
    // User environment
    main->AddUserEnv("LIBSINGLETOP");

    // Analysis selector + Cuts
    main->SetSelector("AtlSgTop_tChannelAnalysis");
    main->SetCut("fMET_min", "30.");
    main->SetCut("fMtW_min", "30.");

    // Object Selection Tool
    AtlObjectsToolD3PDSgTop *tool_obj = new AtlObjectsToolD3PDSgTop("D3PDObjects","Analysis objects");
    main->AddTool(tool_obj);
    main->SetToolParameter("D3PDObjects", "fJetsAuthor","AtlJet::kAntiKt4LCTopo");
    main->SetToolParameter("D3PDObjects", "fJetPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fJetEta_min", "0.");
    main->SetToolParameter("D3PDObjects", "fJetEta_max", "4.5");
    main->SetToolParameter("D3PDObjects", "fElectronPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fMuonPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kMV1");
    main->SetToolParameter("D3PDObjects", "fBTagWeight_min", "0.7892"); // MV1@70%

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

    // Setup Plotting
    // ---------------------------------------------
    main->SetPlotterScheme("SgTop"); //Ttbar or SgTop
    main->SetDataLuminosity(20339.800); // pb^{-1}
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

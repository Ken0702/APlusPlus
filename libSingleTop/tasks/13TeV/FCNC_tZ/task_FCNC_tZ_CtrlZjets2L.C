void task_FCNC_tZ_CtrlZjets2L() {

    // ========================
    // Configure top-level task
    // ========================
    AtlTopLevelAnalysis *main = new AtlTopLevelAnalysis("AtlSgTopFCNC_tZ_CtrlZjets2L",
							"FCNC tZ, ctrl Z+jets+2L");

    // AtlTopLevelAnalysis options
    main->SetForceRetry(kTRUE);
    main->SetBuildHforTree(kFALSE);
    main->SetBuildAnalysisTree(kTRUE);
    main->SetBuildMemTkAnalysisTree(kFALSE);
    main->SetBuildMergingTree(kFALSE);
    main->SetBuildPlottingTree(kTRUE);
    main->SetUseHforSamples(kFALSE);
    main->SetTtbarIsSplitted(kFALSE);

    // Grid config
    main->SetGridRootVersion("5.34/21");
    main->SetGridCmtVersion("x86_64-slc6-gcc47-opt");
    main->SetGridUserName("kind");
    main->SetGridSuffix("v1");
    main->SetGridJob(kFALSE);

    // Setup Samples
    main->SetTreeName(""); // Use "" for tight and "_Loose" for loose lepton events
    main->SetInputFormat(AtlTopLevelAnalysis::kTreePerSyst);
    main->SetCampaign(15);
    main->SetSgTopVersion("v15");
    main->SetCrossSectionFilePath("/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisTop/2.4.29/TopDataPreparation/data/XSection-MC15-13TeV.data");
    main->SetDSIDList("../SampleList/SgTopDSIDs_mc15_v15.txt");
    main->ReadSampleList("../SampleList");

    // Setup Directories
    char const* user = gSystem->Getenv("USER");
    main->SetHistDir(Form("/rdsk/datm5/atlas/%s/sgtop_fcnc_tZ_ctrl_zjets2l", user));
    main->SetJobHomeDir(Form("/rdsk/datm5/atlas/%s/sgtop_fcnc_tZ_ctrl_zjets2l", user));
    main->SetTempDir(Form("/datc/atlas/%s/sgtop_fcnc_tZ_ctrl_zjets2l.tmp", user));
    main->SetInputDir(Form("%s/%s", main->GetHistDir()->Data(), main->GetName()));
    main->SetPlotDir(Form("%s/%s", main->GetHistDir()->Data(), main->GetName()));

    // Set output tree and format
    main->SetOutputTree("physics", "SgTopD3PD FCNC tZ CR Zjets2L");

    // Set output file prefix
    main->SetOutputFilePrefix("ntup_tZ");

    // ===============
    // Set systematics
    // ===============
    main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpNominal);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpJES);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpDefaultSyst);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpScaleFactors);
    // main->AddSystematicGroup(AtlTopLevelAnalysis::kSystGrpSampleSyst);
    
    // ==============
    // Setup Analysis
    // ==============
    
    // User environment
    main->AddUserEnv("LIBSINGLETOP");

    // Set jet multiplicity
    main->AddJetMultiplicity(AtlSelector::kOneOrMoreJets);

    // Set lepton channel (e or u)
    main->AddLeptonChannel(AtlSelector::kLepton);
    
    // Analysis selector + Cuts
    main->SetSelector("AtlSgTopFCNC_tZ_CtrlZjets2L");
    main->SetCut("fMassZ_min",   "81.");
    main->SetCut("fMassZ_max",  "101.");
    main->SetCut("fPt_Lep1_min", "27.");
    main->SetCut("fPt_Lep2_min", "25.");
    main->SetCut("fPt_Lep3_min", "15.");
    main->SetCut("fNLeptons_min", "2");
    main->SetCut("fNLeptons_max", "2");

    // Object Selection Tool
    AtlObjectsToolD3PDSgTop *tool_obj = new AtlObjectsToolD3PDSgTop("D3PDObjects", "Analysis objects");
    main->AddTool(tool_obj);
    main->SetToolParameter("D3PDObjects", "fJetsAuthor", "AtlJet::kAntiKt4TopoEM");
    main->SetToolParameter("D3PDObjects", "fJetPt_min", "30.");
    main->SetToolParameter("D3PDObjects", "fJetEta_max", "4.5");
    main->SetToolParameter("D3PDObjects", "fBJetEta_max", "2.5");
    main->SetToolParameter("D3PDObjects", "fElectronPt_min", "15.");
    main->SetToolParameter("D3PDObjects", "fMuonPt_min", "15.");
    main->SetToolParameter("D3PDObjects", "fBTagger", "AtlBTag::kMV2c10_77");
    main->SetToolParameter("D3PDObjects", "fBTagWeight_min", "0");

    // Object Reco SF Tool
    AtlObjRecoScaleFactorTool *tool_sf = new AtlObjRecoScaleFactorTool("SFTool",
                                                                       "SF tool");
    main->AddTool(tool_sf);
    main->SetToolParameter("SFTool", "fBTagWP",
			   "AtlObjRecoScaleFactorTool::kMV2c10_77");
    main->SetToolParameter("SFTool", "fOperationMode",
                           "AtlObjRecoScaleFactorTool::kAllSF"); // Check with D3PDObjects config!
    main->SetToolParameter("SFTool", "fUseCDI", "kFALSE"); // Do not use CDI interface

    // ==============
    // Setup Plotting
    // ==============
    main->SetPlotterScheme("tZ");
    main->SetDataLuminosity(36074.56); // pb^{-1}
    main->SetAtlasLabelStatus("Work in Progress"); 
    main->SetCenterOfMassEnergyLabel("13");
    main->SetLuminosityLabel("36.1 fb^{-1}");
    main->SetUseOverflowUnderflow(kTRUE);
    main->SetDrawDataZeroEntryErrors(kFALSE);
    main->SetDrawMCError(kTRUE);
    main->SetExportPDF(kFALSE);
    main->SetDrawSignalOverlay(kTRUE);
    main->SetScaleOverlay(1.);
    
    // ===============
    // Build task tree
    // ===============
    main->BuildTree(kTRUE);
}
